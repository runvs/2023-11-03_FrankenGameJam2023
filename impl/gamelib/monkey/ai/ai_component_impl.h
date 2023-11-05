#ifndef JAMTEMPLATE_AI_COMPONENT_IMPL_H
#define JAMTEMPLATE_AI_COMPONENT_IMPL_H

#include "ai_component_interface.h"

class AiComponentImpl : public AiComponentInterface {
public:
    void update(
        AiTargetInterface& target, float const elapsed, float const randomSpeedMultiplier) override;
    float getRotationAngle() override;
    void updatePlayerPosition(const jt::Vector2f pos) override;
    MonkeyState getState() override;

private:
    float rotationAngle = 180.0f;
    jt::Vector2f playerPos;
    MonkeyState m_state { MonkeyState::Idle };
};

#endif // JAMTEMPLATE_AI_COMPONENT_IMPL_H
