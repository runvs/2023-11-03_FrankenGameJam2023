#ifndef JAMTEMPLATE_AI_COMPONENT_INTERFACE_H
#define JAMTEMPLATE_AI_COMPONENT_INTERFACE_H

#include "vector.hpp"
#include "ai_target_interface.h"

class AiComponentInterface {
public:
    virtual ~AiComponentInterface() = default;

    virtual void update(AiTargetInterface& target, float const elapsed) = 0;
    virtual float getRotationAngle() = 0;
    virtual void updatePlayerPosition(jt::Vector2f const pos) = 0;
};

#endif // JAMTEMPLATE_AI_COMPONENT_INTERFACE_H
