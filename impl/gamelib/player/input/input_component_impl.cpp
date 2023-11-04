#include "input_component_impl.hpp"
#include <game_properties.hpp>
#include "math_helper.hpp"

InputComponentImpl::InputComponentImpl(std::shared_ptr<jt::KeyboardInterface> keyboardInterface)
    : m_keyboard { keyboardInterface }
{
}

void InputComponentImpl::updateMovement(InputTargetInterface& target, float const elapsed)
{
    if (m_keyboard->pressed(jt::KeyCode::A) || m_keyboard->pressed(jt::KeyCode::Left)) {
        rotationAngle -= GP::playerRotationStrength * elapsed;
        if (rotationAngle < 0) {
            rotationAngle += 360;
        }
    }
    if (m_keyboard->pressed(jt::KeyCode::D) || m_keyboard->pressed(jt::KeyCode::Right)) {
        rotationAngle += GP::playerRotationStrength * elapsed;
        if (rotationAngle > 360) {
            rotationAngle -= 360;
        }
    }

    if (m_keyboard->pressed(jt::KeyCode::W) || m_keyboard->pressed(jt::KeyCode::Up)) {
        auto const force= jt::MathHelper::rotateBy(jt::Vector2f { 0.0f, -GP::playerForwardStrength }, rotationAngle);
        target.addForceToCenter(force);
    } else if (m_keyboard->pressed(jt::KeyCode::S) || m_keyboard->pressed(jt::KeyCode::Down)) {
        target.setVelocity(target.getVelocity() * GP::playerBreakMultiplier);
    }
}

float InputComponentImpl::getRotationAngle()
{
    return rotationAngle;
}
