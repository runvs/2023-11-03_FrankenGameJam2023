#include "state_game.hpp"
#include "random/random.hpp"
#include "tilemap/tileson_loader.hpp"
#include <box2dwrapper/box2d_world_impl.hpp>
#include <color/color.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <hud/hud.hpp>
#include <math_helper.hpp>
#include <screeneffects/vignette.hpp>
#include <shape.hpp>
#include <state_menu.hpp>

void StateGame::onCreate()
{
    m_world = std::make_shared<jt::Box2DWorldImpl>(jt::Vector2f { 0.0f, 0.0f });

    using jt::Shape;

    jt::tilemap::TilesonLoader loader { getGame()->cache().getTilemapCache(),
        "assets/test_map.json" };
    m_tilemap = std::make_shared<jt::tilemap::TileLayer>(
        loader.loadTilesFromLayer("ground", textureManager()));
    loadLevelCollisions(loader);
    m_overlay = std::make_shared<jt::tilemap::TileLayer>(
        loader.loadTilesFromLayer("overlay", textureManager()));

    m_waves = std::make_shared<jt::Waves>("assets/waves.aseprite",
        jt::Rectf {
            0.0f, 0.0f, m_tilemap->getMapSizeInPixel().x, m_tilemap->getMapSizeInPixel().y },
        m_tileCollisionRects, 100);
    add(m_waves);

    m_dropFruitPS = std::make_shared<ParticleSystemDropFruits>();
    add(m_dropFruitPS);

    createHarbors(loader);
    createPlayer();
    spawnMonkey();

    m_vignette = std::make_shared<jt::Vignette>(GP::GetScreenSize());
    add(m_vignette);
    m_hud = std::make_shared<Hud>();
    add(m_hud);

    // StateGame will call drawObjects itself.
    setAutoDraw(false);

    m_soundFruitPickup = getGame()->audio().addTemporarySound("assets/sfx/fruit-pickup.ogg");
    m_soundFruitDeliver = getGame()->audio().addTemporarySound("assets/sfx/reward.ogg");
    m_soundMonkeyHitsEnemy
        = getGame()->audio().addTemporarySound("assets/sfx/monkey-hits-boat.ogg");
}

void StateGame::onEnter() { }

void StateGame::createPlayer()
{
    m_player = std::make_shared<Player>(m_world);

    // TODO spawn at correct harbor
    m_player->setPosition(m_harbors->at(0).lock()->getPosition());

    add(m_player);
}

void StateGame::createHarbors(jt::tilemap::TilesonLoader& loader)
{
    m_harbors = std::make_shared<jt::ObjectGroup<Harbor>>();
    add(m_harbors);

    auto const harborObjects = loader.loadObjectsFromLayer("harbors");

    for (auto const& h : harborObjects) {
        bool isOffering
            = h.properties.bools.contains("isOffering") && h.properties.bools.at("isOffering");
        auto const harbor = std::make_shared<Harbor>(h.position, isOffering);
        add(harbor);
        m_harbors->push_back(harbor);
    }
}

void StateGame::spawnMonkey()
{
    if (m_monkeys == nullptr) {
        m_monkeys = std::make_shared<jt::ObjectGroup<Monkey>>();
        add(m_monkeys);
    }

    jt::Vector2f position;
    do {
        position = jt::Random::getRandomPointIn(m_tilemap->getMapSizeInPixel());
    } while (!isValidMonkeySpawnPosition(position));

    auto const monkey = std::make_shared<Monkey>(m_world, position);
    add(monkey);
    m_monkeys->push_back(monkey);
}

bool StateGame::isValidMonkeySpawnPosition(jt::Vector2f position)
{
    auto const cameraOffset = getGame()->gfx().camera().getCamOffset();
    auto const screenRectWithExtraSpace = jt::Rectf { cameraOffset.x - GP::GetScreenSize().x * 0.5f,
        cameraOffset.y - GP::GetScreenSize().y * 0.5f, GP::GetScreenSize().x,
        GP::GetScreenSize().y };

    if (jt::MathHelper::checkIsIn(screenRectWithExtraSpace, position)) {
        return false;
    }

    for (auto& rect : m_tileCollisionRects) {
        if (jt::MathHelper::checkIsIn(rect, position)) {
            return false;
        }
    }

    return true;
}

void StateGame::onUpdate(float const elapsed)
{
    if (m_running) {
        m_world->step(elapsed, GP::PhysicVelocityIterations(), GP::PhysicPositionIterations());
        // update game logic here
        if (getGame()->input().keyboard()->pressed(jt::KeyCode::LShift)
            && getGame()->input().keyboard()->pressed(jt::KeyCode::Escape)) {
            endGame();
        }

        updateCamera(elapsed);
        updateHarbors(elapsed);
        updateMonkeys();
        updatePlayer();
    }

    m_tilemap->update(elapsed);
    m_overlay->update(elapsed);
    m_vignette->update(elapsed);
}

void StateGame::onDraw() const
{
    m_tilemap->draw(renderTarget());

    m_overlay->draw(renderTarget());
    drawObjects();
    m_dropFruitPS->draw();

    m_vignette->draw();
    m_hud->draw();
}

void StateGame::endGame()
{
    if (m_hasEnded) {
        // trigger this function only once
        return;
    }
    m_hasEnded = true;
    m_running = false;

    getGame()->stateManager().switchState(std::make_shared<StateMenu>());
}

std::string StateGame::getName() const { return "State Game"; }

void StateGame::updateHarbors(float const /*elapsed*/)
{
    auto const playerPos = m_player->getPosition();
    for (auto const& h : *m_harbors) {
        auto harbor = h.lock();
        auto const harborPos = harbor->getPosition();
        auto const l = jt::MathHelper::distanceBetweenSquared(harborPos, playerPos);
        if (l < GP::TileSizeInPixel() * GP::TileSizeInPixel()) {
            //            m_player->getGraphics().getDrawable()->flash(0.1f, jt::colors::Yellow);
            if (harbor->isOffering()) {
                if (harbor->canBeInteractedWith()) {
                    auto const harborFruits = harbor->getNumberOfFruitsToPickup();
                    for (auto i = 0; i != harborFruits; ++i) {
                        m_player->getCargo().addFruit("");
                    }
                    harbor->pickUpFruit();
                    spawnMonkey();
                    m_hud->getObserverScoreP1()->notify(m_player->getCargo().getNumberOfFruits());
                    m_soundFruitPickup->play();
                }
            } else {
                if (m_player->getCargo().getNumberOfFruits() > 0) {
                    if (harbor->canBeInteractedWith()) {
                        // TODO check if special fruit requested
                        m_player->getCargo().removeFruit("");

                        m_hud->getObserverScoreP1()->notify(
                            m_player->getCargo().getNumberOfFruits());

                        m_points += 1;
                        m_hud->getObserverScoreP2()->notify(m_points);

                        harbor->deliverFruit();
                        m_soundFruitDeliver->play();
                        m_player->getGraphics().flash(1.5f, jt::colors::White);
                    }
                }
            }
        }
    }
}

void StateGame::updateMonkeys()
{
    auto const playerPos = m_player->getPosition();
    for (auto const& m : *m_monkeys) {
        auto monkey = m.lock();

        monkey->updatePlayerPosition(playerPos);
        monkey->clampPositionOnMap(m_tilemap->getMapSizeInPixel());

        if (monkey->canAttack()) {
            auto const monkeyPos = monkey->getPosition();
            auto const l = jt::MathHelper::lengthSquared(playerPos - monkeyPos);
            if (l <= GP::TileSizeInPixel() * GP::TileSizeInPixel()) {
                m_soundMonkeyHitsEnemy->play();
                m_player->getDamage();
                m_dropFruitPS->fire(4, m_player->getPosition());
                monkey->attack();
                // TODO visual effect
                m_player->getCargo().removeFruit("");
            }
        }
    }
}

void StateGame::updateCamera(float const elapsed)
{
    auto velocity = m_player->getVelocity();
    float speedFactor;
    if (jt::MathHelper::length(velocity) > GP::cameraDragDistance) {
        speedFactor = 1;
    } else {
        speedFactor = jt::MathHelper::length(velocity) / GP::cameraDragDistance;
    }

    jt::MathHelper::normalizeMe(velocity);
    velocity = velocity * speedFactor * GP::cameraDragDistance;
    velocity = velocity * GP::cameraSmoothFactor
        + m_lastFrameCameraOffset * (1 - GP::cameraSmoothFactor);
    m_lastFrameCameraOffset = velocity;

    auto center = m_player->getPosition() - GP::GetScreenSize() * 0.5f;
    auto targetPosition = center + velocity;
    jt::Vector2f const camPosMax { m_tilemap->getMapSizeInPixel() - GP::GetScreenSize() };
    targetPosition = jt::MathHelper::clamp(targetPosition, jt::Vector2f { 0.0f, 0.0f }, camPosMax);
    getGame()->gfx().camera().setCamOffset(targetPosition);
}

void StateGame::loadLevelCollisions(jt::tilemap::TilesonLoader& loader)
{
    auto tileCollisions = loader.loadCollisionsFromLayer("ground");

    tileCollisions.refineColliders(16);
    m_tileCollisionRects = tileCollisions.getRects();
    for (auto const& r : tileCollisions.getRects()) {
        b2BodyDef bodyDef;
        bodyDef.fixedRotation = true;
        bodyDef.type = b2_staticBody;
        bodyDef.position.Set(r.left + r.width / 2.0f, r.top + r.height / 2.0f);

        b2FixtureDef fixtureDef;
        b2PolygonShape boxCollider {};
        boxCollider.SetAsBox(r.width / 2.0f, r.height / 2.0f);
        fixtureDef.shape = &boxCollider;

        auto collider = std::make_shared<jt::Box2DObject>(m_world, &bodyDef);

        collider->getB2Body()->CreateFixture(&fixtureDef);

        m_colliders.push_back(collider);
    }
}

void StateGame::updatePlayer()
{
    m_player->clampPositionOnMap(m_tilemap->getMapSizeInPixel() - jt::Vector2f { 16.0f, 16.0f });
}
