#include "player.hpp"
#include <game_interface.hpp>
#include <math_helper.hpp>
#include <player/graphics/graphics_component_impl.hpp>
#include <player/input/input_component_impl.hpp>
#include <player/sound/sound_component_impl.hpp>
#include <state_game.hpp>
#include <Box2D/Box2D.h>

namespace {
std::string selectWalkAnimation(jt::Vector2f const& velocity)
{
    auto a = jt::MathHelper::angleOf(velocity);

    std::cout << velocity.x << " " << velocity.y << " " << a << "\n";

    if (a > 0 && a < 22.5) {
        return "right";
    } else if (a < 22.5f + 45 * 1) {
        return "up-right";
    } else if (a < 22.5f + 45 * 2) {
        return "up";
    } else if (a < 22.5f + 45 * 3) {
        return "up-left";
    } else if (a < 22.5f + 45 * 4) {
        return "left";
    } else if (a < 22.5f + 45 * 5) {
        return "down-left";
    } else if (a < 22.5f + 45 * 6) {
        return "down";
    } else if (a < 22.5f + 45 * 7) {
        return "down-right";
    } else if (a < 360) {
        return "right";
    }
}
} // namespace

Player::Player(std::shared_ptr<jt::Box2DWorldInterface> world)
{
    b2BodyDef def {};
    def.type = b2BodyType::b2_dynamicBody;
    m_b2Object = std::make_unique<jt::Box2DObject>(world, &def);
}

void Player::doCreate()
{
    m_input = std::make_unique<InputComponentImpl>(getGame()->input().keyboard());
    m_sound = std::make_unique<SoundComponentImpl>(getGame()->audio(), getGame()->logger());
    m_graphics = std::make_unique<GraphicsComponentImpl>(getGame());
}

void Player::doUpdate(float const elapsed)
{
    m_input->updateMovement(*m_b2Object);
    m_graphics->setPosition(m_b2Object->getPosition());
    m_graphics->setAnimationIfNotSet(selectWalkAnimation(m_b2Object->getVelocity()));
    m_graphics->updateGraphics(elapsed);
}

void Player::doDraw() const { m_graphics->draw(renderTarget()); }
