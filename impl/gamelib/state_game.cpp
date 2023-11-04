#include "state_game.hpp"
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

    createPlayer();

    createHarbors(loader);
    spawnMonkey();

    m_vignette = std::make_shared<jt::Vignette>(GP::GetScreenSize());
    add(m_vignette);
    m_hud = std::make_shared<Hud>();
    add(m_hud);

    // StateGame will call drawObjects itself.
    setAutoDraw(false);
}

void StateGame::onEnter() { }

void StateGame::createPlayer()
{
    m_player = std::make_shared<Player>(m_world);
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
    if (m_monkeys == nullptr)
    {
        m_monkeys = std::make_shared<jt::ObjectGroup<Monkey>>();
    }
    add(m_monkeys);

    // TODO spawn monkeys randomly over time
    auto const monkey = std::make_shared<Monkey>(m_world, GP::GetScreenSize() * 0.75f);
    add(monkey);
    m_monkeys->push_back(monkey);
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
    }

    m_tilemap->update(elapsed);
    m_vignette->update(elapsed);
}

void StateGame::onDraw() const
{
    m_tilemap->draw(renderTarget());
    drawObjects();
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

    getGame()->stateManager().switchToStoredState("menu");
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
            m_player->getGraphics().getDrawable()->flash(0.1f, jt::colors::Yellow);
            if (harbor->isOffering()) {
                if (harbor->canBeInteractedWith()) {
                    m_player->getCargo().addFruit(harbor->getFruitOffering());
                    harbor->pickUpFruit();
                    m_hud->getObserverScoreP1()->notify(m_player->getCargo().getNumberOfFruits());
                }
            } else {
                if (m_player->getCargo().getNumberOfFruits() > 0) {
                    if (harbor->canBeInteractedWith()) {
                        // TODO check if special fruit requested
                        m_player->getCargo().removeFruit("");
                        m_hud->getObserverScoreP1()->notify(
                            m_player->getCargo().getNumberOfFruits());
                        harbor->deliverFruit();
                    }
                }
            }
        }
    }
}

void StateGame::updateCamera(float const elapsed)
{
    float const camMovementSpeed = 60;
    auto playerPositionOnScreen = m_player->getGraphics().getDrawable()->getScreenPosition();
    if (playerPositionOnScreen.x < GP::ScreenSizeScrollBound() * 1.5) {
        getGame()->gfx().camera().move(jt::Vector2f { -camMovementSpeed * elapsed, 0.0f });
    } else if (playerPositionOnScreen.x + 8
        > GP::GetScreenSize().x - GP::ScreenSizeScrollBound() * 1.5) {
        getGame()->gfx().camera().move(jt::Vector2f { camMovementSpeed * elapsed, 0.0f });
    }

    if (playerPositionOnScreen.y < GP::ScreenSizeScrollBound() * 1.5) {
        getGame()->gfx().camera().move(jt::Vector2f { 0.0f, -camMovementSpeed * elapsed });
    } else if (playerPositionOnScreen.y + 8
        > GP::GetScreenSize().y - GP::ScreenSizeScrollBound() * 1.5) {
        getGame()->gfx().camera().move(jt::Vector2f { 0.0f, camMovementSpeed * elapsed });
    }

    if (playerPositionOnScreen.x < GP::ScreenSizeScrollBound()) {
        getGame()->gfx().camera().move(jt::Vector2f { -camMovementSpeed * elapsed, 0.0f });
    } else if (playerPositionOnScreen.x + 8 > GP::GetScreenSize().x - GP::ScreenSizeScrollBound()) {
        getGame()->gfx().camera().move(jt::Vector2f { camMovementSpeed * elapsed, 0.0f });
    }

    if (playerPositionOnScreen.y < GP::ScreenSizeScrollBound()) {
        getGame()->gfx().camera().move(jt::Vector2f { 0.0f, -camMovementSpeed * elapsed });
    } else if (playerPositionOnScreen.y + 8 > GP::GetScreenSize().y - GP::ScreenSizeScrollBound()) {
        getGame()->gfx().camera().move(jt::Vector2f { 0.0f, camMovementSpeed * elapsed });
    }

    auto camPos = getGame()->gfx().camera().getCamOffset();
    jt::Vector2f const camPosMax { m_tilemap->getMapSizeInPixel() - GP::GetScreenSize() };
    camPos = jt::MathHelper::clamp(camPos, jt::Vector2f { 0.0f, 0.0f }, camPosMax);
    getGame()->gfx().camera().setCamOffset(camPos);
}

void StateGame::loadLevelCollisions(jt::tilemap::TilesonLoader& loader)
{
    auto tileCollisions = loader.loadCollisionsFromLayer("ground");

    tileCollisions.refineColliders(16);
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
