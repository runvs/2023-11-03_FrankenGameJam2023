#ifndef GAME_PLAYER_INPUT_COMPONENT_HPP
#define GAME_PLAYER_INPUT_COMPONENT_HPP

#include <input/input_manager_interface.hpp>
#include <player/input/input_component_interface.hpp>
#include <memory>

class InputComponentImpl : public InputComponentInterface {
public:
    explicit InputComponentImpl(std::shared_ptr<jt::KeyboardInterface> keyboardInterface);

    void updateMovement(InputTargetInterface& player, float const elapsed) override;
    float getRotationAngle() override;

    float getBoostNitro() const override;

    void setMaxVelocityFactor(float factor) override;

private:
    std::shared_ptr<jt::KeyboardInterface> m_keyboard { nullptr };
    float rotationAngle { 90.0f };

    float m_boostNitro { 1.0f };
    float m_timeSinceShiftPressed { 0.0f };
    float m_timeSinceShiftJustPressed { 0.0f };

    float m_maxVelocityFactor { 1.0f };
};

#endif // GAME_PLAYER_INPUT_COMPONENT_HPP
