#include "input_component_impl.hpp"
#include <game_properties.hpp>
#include <lerp.hpp>
#include <math_helper.hpp>

InputComponentImpl::InputComponentImpl(std::shared_ptr<jt::KeyboardInterface> keyboardInterface)
    : m_keyboard { keyboardInterface }
{
}

void InputComponentImpl::updateMovement(InputTargetInterface& target, float const elapsed)
{
    auto const boostPressed
        = m_keyboard->pressed(jt::KeyCode::LShift) || m_keyboard->pressed(jt::KeyCode::Space);
    auto const boostJustPressed = m_keyboard->justPressed(jt::KeyCode::LShift)
        || m_keyboard->justPressed(jt::KeyCode::Space);
    auto boost = false;
    auto maxVelocity = GP::playerMaxVelocity * m_maxVelocityFactor;
    if (boostPressed) {
        if (m_boostNitro > 0) {
            m_timeSinceShiftPressed = 0.0f;
            boost = true;
            m_boostNitro -= elapsed;
            maxVelocity = GP::playerMaxVelocityWhenBoosting * m_maxVelocityFactor;
        }
    } else {
        m_timeSinceShiftPressed += elapsed;
        if (m_timeSinceShiftPressed >= 1.0f) {
            m_boostNitro += 0.5f * elapsed;
        }
    }
    m_boostNitro = jt::MathHelper::clamp(m_boostNitro, 0.0f, 1.0f);

    float rotationSpeed;
    if (boost) {
        rotationSpeed = GP::playerRotationStrengthAtBoost;
    } else {
        auto const velocity = jt::MathHelper::length(target.getVelocity());
        if (velocity < GP::playerMaxVelocity) {
            auto const t = velocity / GP::playerMaxVelocity;
            rotationSpeed = jt::Lerp::linear(
                GP::playerRotationStrengthAtMinSpeed, GP::playerRotationStrengthAtMaxSpeed, t);
        } else {
            auto const t = (velocity - GP::playerMaxVelocity)
                / (GP::playerMaxVelocityWhenBoosting - GP::playerMaxVelocity);
            rotationSpeed = jt::Lerp::linear(
                GP::playerRotationStrengthAtMaxSpeed, GP::playerRotationStrengthAtBoost, t);
        }
    }

    if (m_keyboard->pressed(jt::KeyCode::A) || m_keyboard->pressed(jt::KeyCode::Left)) {
        rotationAngle += rotationSpeed * elapsed;
        if (rotationAngle > 360) {
            rotationAngle -= 360;
        }
    }
    if (m_keyboard->pressed(jt::KeyCode::D) || m_keyboard->pressed(jt::KeyCode::Right)) {
        rotationAngle -= rotationSpeed * elapsed;
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
    if (boostJustPressed) {
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

void InputComponentImpl::setMaxVelocityFactor(float factor) { m_maxVelocityFactor = factor; }
