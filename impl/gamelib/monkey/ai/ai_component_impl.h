#ifndef JAMTEMPLATE_AI_COMPONENT_IMPL_H
#define JAMTEMPLATE_AI_COMPONENT_IMPL_H

#include "ai_component_interface.h"

class AiComponentImpl : public AiComponentInterface  {
public:
    void doUpdate() override;
    float getRotationAngle() override;

private:
    float rotationAngle = 180.0f;
};

#endif // JAMTEMPLATE_AI_COMPONENT_IMPL_H
