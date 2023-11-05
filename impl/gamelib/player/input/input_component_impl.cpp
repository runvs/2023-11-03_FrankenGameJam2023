#include "input_component_impl.hpp"
#include "math_helper.hpp"
#include <game_properties.hpp>

#include <iostream>

InputComponentImpl::InputComponentImpl(std::shared_ptr<jt::KeyboardInterface> keyboardInterface)
    : m_keyboard { keyboardInterface }
{
}

void InputComponentImpl::updateMovement(InputTargetInterface& target, float const elapsed)
{
    auto const shiftPressed = m_keyboard->pressed(jt::KeyCode::LShift);
    auto const shiftJustPressed = m_keyboard->justPressed(jt::KeyCode::LShift);
    auto boost = false;
    auto maxVelocity = GP::playerMaxVelocity;
    if (shiftPressed) {
        if (m_boostNitro > 0) {
            m_timeSinceShiftPressed = 0.0f;
            boost = true;
            m_boostNitro -= elapsed;
            maxVelocity = GP::playerMaxVelocityWhenBoosting;
        }
    } else {
        m_timeSinceShiftPressed += elapsed;
        if (m_timeSinceShiftPressed >= 1.0f) {
            m_boostNitro += 0.5f * elapsed;
        }
    }
    m_boostNitro = jt::MathHelper::clamp(m_boostNitro, 0.0f, 1.0f);

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

    //    std::cout << jt::MathHelper::length(target.getVelocity()) << std::endl;
    if (shiftJustPressed) {
        if (m_timeSinceShiftJustPressed > 0.2f) {
            m_timeSinceShiftJustPressed = 0.0f;
            auto dir = jt::MathHelper::rotateBy(jt::Vector2f { 1.0f, 0.0f }, -rotationAngle);
            auto velocityCapped = dir * maxVelocity * 0.9;
            target.addVelocity(velocityCapped);
        }
    } else {
        m_timeSinceShiftJustPressed += elapsed;
    }

    // clamp speed to max velocity
    auto v = target.getVelocity();
    auto vl = jt::MathHelper::length(v);
    if (vl >= maxVelocity) {
        auto velocityCapped = v / vl * maxVelocity;
        target.setVelocity(velocityCapped);
    }
}

float InputComponentImpl::getRotationAngle() { return rotationAngle; }

float InputComponentImpl::getBoostNitro() const { return m_boostNitro; }
