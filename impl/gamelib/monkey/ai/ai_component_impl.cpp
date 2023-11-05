#include "ai_component_impl.h"
#include "game_properties.hpp"
#include "math_helper.hpp"
#include "lerp.hpp"

float AiComponentImpl::getRotationAngle() { return rotationAngle; }

void AiComponentImpl::update(
    AiTargetInterface& target, float const elapsed, float const randomSpeedMultiplier)
{
    auto const diff = playerPos - target.getPosition();
    auto const distanceToPlayer = jt::MathHelper::distanceBetween(playerPos, target.getPosition());
    if (distanceToPlayer > GP::monkeyChaseDistance) {
        return;
    }

    auto const targetRotation = jt::MathHelper::angleOf(diff);

    if (targetRotation - rotationAngle > 0) {
        rotationAngle = std::clamp(rotationAngle + GP::monkeyRotationSpeed * elapsed, rotationAngle, targetRotation);
    } else {
        rotationAngle = std::clamp(rotationAngle - GP::monkeyRotationSpeed * elapsed, targetRotation, rotationAngle);
    };

    auto const force
        = jt::MathHelper::rotateBy(jt::Vector2f { GP::monkeyForwardStrength * randomSpeedMultiplier, 0.0f }, -rotationAngle);
    target.addForceToCenter(force);
}

void AiComponentImpl::updatePlayerPosition(jt::Vector2f pos) { playerPos = pos; }
