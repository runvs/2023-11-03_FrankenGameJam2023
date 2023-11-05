#include "ai_component_impl.h"
#include "game_properties.hpp"
#include "lerp.hpp"
#include "math_helper.hpp"

float AiComponentImpl::getRotationAngle() { return rotationAngle; }

void AiComponentImpl::update(
    AiTargetInterface& target, float const elapsed, float const randomSpeedMultiplier)
{
    auto const diff = playerPos - target.getPosition();
    auto const distanceToPlayer = jt::MathHelper::distanceBetween(playerPos, target.getPosition());
    if (distanceToPlayer > GP::monkeyChaseDistance) {
        if (m_state == MonkeyState::Angry) {
            m_state = MonkeyState::Idle;
        }
        return;
    }

    if (m_state == MonkeyState::Idle) {
        m_state = MonkeyState::Angry;
    }

    auto const targetRotation = jt::MathHelper::angleOf(diff);
    auto const rotationDistance = std::abs(rotationAngle - targetRotation);

    if (targetRotation > rotationAngle) {
        if (rotationDistance > 180) {
            rotationAngle = rotationAngle - GP::monkeyRotationSpeed * elapsed;
        } else {
            rotationAngle = rotationAngle + GP::monkeyRotationSpeed * elapsed;
        }
    } else {
        if (rotationDistance > 180) {
            rotationAngle = rotationAngle + GP::monkeyRotationSpeed * elapsed;
        } else {
            rotationAngle = rotationAngle - GP::monkeyRotationSpeed * elapsed;
        }
    };

    if (rotationAngle > 180) {
        rotationAngle -= 360;
    } else if (rotationAngle < -180) {
        rotationAngle += 360;
    }

    auto const force
        = jt::MathHelper::rotateBy(jt::Vector2f { GP::monkeyForwardStrength * randomSpeedMultiplier, 0.0f }, -rotationAngle);
    target.addForceToCenter(force);
}

void AiComponentImpl::updatePlayerPosition(jt::Vector2f pos) { playerPos = pos; }

MonkeyState AiComponentImpl::getState() { return m_state; }
