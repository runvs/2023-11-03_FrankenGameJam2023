#include "state_game.hpp"
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

    float const w = static_cast<float>(GP::GetWindowSize().x);
    float const h = static_cast<float>(GP::GetWindowSize().y);

    using jt::Shape;

    m_background = std::make_shared<Shape>();
    m_background->makeRect({ w, h }, textureManager());
    m_background->setColor(GP::PaletteBackground());
    m_background->setIgnoreCamMovement(true);
    m_background->update(0.0f);

    createPlayer();

    createHarbors();

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

void StateGame::createHarbors()
{
    m_harbors = std::make_shared<jt::ObjectGroup<Harbor>>();
    add(m_harbors);

    // TODO create harbors based on map
    auto const harbor = std::make_shared<Harbor>(GP::GetScreenSize() * 0.5f, true);
    add(harbor);
    m_harbors->push_back(harbor);

    auto const harbor2 = std::make_shared<Harbor>(
        GP::GetScreenSize() * 0.5f + jt::Vector2f { GP::GetScreenSize().x * 0.25f, 0.0f }, false);
    add(harbor2);
    m_harbors->push_back(harbor2);
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

        updateHarbors(elapsed);
    }

    m_background->update(elapsed);
    m_vignette->update(elapsed);
}

void StateGame::onDraw() const
{
    m_background->draw(renderTarget());
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
                // TODO check if special fruit requested
                if (harbor->canBeInteractedWith()) {
                    m_player->getCargo().removeFruit("");
                    m_hud->getObserverScoreP1()->notify(m_player->getCargo().getNumberOfFruits());
                    harbor->deliverFruit();
                }
            }
        }
    }
}
