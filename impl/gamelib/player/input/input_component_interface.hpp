#ifndef JAMTEMPLATE_INPUT_COMPONENT_INTERFACE_HPP
#define JAMTEMPLATE_INPUT_COMPONENT_INTERFACE_HPP

#include <player/input/input_target_interface.hpp>

class InputComponentInterface {
public:
    virtual ~InputComponentInterface() = default;

    virtual void updateMovement(InputTargetInterface& target, float const elapsed) = 0;
    virtual float getRotationAngle() = 0;
    virtual float getBoostNitro() const = 0;
    virtual void setMaxVelocityFactor(float factor) = 0;
};

#endif // JAMTEMPLATE_INPUT_COMPONENT_INTERFACE_HPP
