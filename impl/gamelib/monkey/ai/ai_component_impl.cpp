#include "ai_component_impl.h"
#include "math_helper.hpp"
#include <iostream>

float AiComponentImpl::getRotationAngle() { return rotationAngle; }

void AiComponentImpl::update(AiTargetInterface& target, float const elapsed)
{
    // TODO: if player enters range, do something silly
    auto const movementSpeed = 100;
    auto const rotationSpeed = 10000;
    auto const chaseDistance = 300;

    auto const diff = playerPos - target.getPosition();
    auto const distanceToPlayer = jt::MathHelper::distanceBetween(playerPos, target.getPosition());
    if (distanceToPlayer > chaseDistance) {
        return;
    }

    auto const targetRotation = jt::MathHelper::angleOf(diff);

    // auto const angularDifference = targetRotation - rotationAngle;
    // if (std::abs(angularDifference) > 1) {
    //     if (angularDifference > 0) {
    //         rotationAngle += rotationSpeed * elapsed;
    //     } else {
    //         rotationAngle -= rotationSpeed * elapsed;
    //     };
    // }

    rotationAngle = targetRotation;

    auto const force
        = jt::MathHelper::rotateBy(jt::Vector2f { movementSpeed, 0.0f }, -rotationAngle);
    target.addForceToCenter(force);

    //    std::cout << "monke pos: " << target.getPosition() << " player pos: " << playerPos
    //              << " diff: " << diff << " current: " << rotationAngle << " target: " <<
    //              targetRotation
    //              << std::endl;
}

void AiComponentImpl::updatePlayerPosition(jt::Vector2f pos) { playerPos = pos; }
