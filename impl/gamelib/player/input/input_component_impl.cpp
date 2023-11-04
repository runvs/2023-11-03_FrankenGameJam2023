#include "input_component_impl.hpp"
#include "math_helper.hpp"
#include <game_properties.hpp>

InputComponentImpl::InputComponentImpl(std::shared_ptr<jt::KeyboardInterface> keyboardInterface)
    : m_keyboard { keyboardInterface }
{
}

void InputComponentImpl::updateMovement(InputTargetInterface& target, float const elapsed)
{
    auto const shiftPressed = m_keyboard->pressed(jt::KeyCode::LShift);
    auto boost = false;
    if (m_boostInRefill) {
        m_boostNitro += 0.5f * elapsed;
        if (m_boostNitro >= 1.0f) {
            m_boostInRefill = false;
            m_boostNitro = 1.0f;
        }
    } else {
        if (shiftPressed) {
            m_boostNitro -= elapsed;
            boost = true;
            if (m_boostNitro <= 0) {
                m_boostNitro = 0;
                m_boostInRefill = true;
            }

        } else {
        }
    }

    if (m_keyboard->pressed(jt::KeyCode::A) || m_keyboard->pressed(jt::KeyCode::Left)) {
        rotationAngle += GP::playerRotationStrength * elapsed;
        if (rotationAngle > 360) {
            rotationAngle -= 360;
        }
    }
    if (m_keyboard->pressed(jt::KeyCode::D) || m_keyboard->pressed(jt::KeyCode::Right)) {
        rotationAngle -= GP::playerRotationStrength * elapsed;
        if (rotationAngle < 0) {
            rotationAngle += 360;
        }
    }

    if (m_keyboard->pressed(jt::KeyCode::W) || m_keyboard->pressed(jt::KeyCode::Up)) {
        auto const force = jt::MathHelper::rotateBy(
                               jt::Vector2f { GP::playerForwardStrength, 0.0f }, -rotationAngle)
            * (boost ? GP::playerBoostFactor : 1.0f);
        target.addForceToCenter(force);
    } else if (m_keyboard->pressed(jt::KeyCode::S) || m_keyboard->pressed(jt::KeyCode::Down)) {
        target.setVelocity(target.getVelocity() * GP::playerBreakMultiplier);
    }
}

float InputComponentImpl::getRotationAngle() { return rotationAngle; }

float InputComponentImpl::getBoostNitro() const { return m_boostNitro; }
