#ifndef JAMTEMPLATE_AI_COMPONENT_INTERFACE_H
#define JAMTEMPLATE_AI_COMPONENT_INTERFACE_H

class AiComponentInterface {
public:
    virtual ~AiComponentInterface() = default;

    virtual void doUpdate() = 0;
    virtual float getRotationAngle() = 0;
};

#endif // JAMTEMPLATE_AI_COMPONENT_INTERFACE_H
