#ifndef JAMTEMPLATE_HARBOR_HPP
#define JAMTEMPLATE_HARBOR_HPP

#include "animation.hpp"
#include <bar.hpp>
#include <game_object.hpp>

class Harbor : public jt::GameObject {
public:
    Harbor(jt::Vector2f const& pos, bool offering);

    jt::Vector2f getPosition() const;

    void pickUpFruit();
    void deliverFruit();

    bool isOffering() const;
    bool canBeInteractedWith() const;

    int getNumberOfFruitsToPickup() const;

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    jt::Vector2f m_pos { 0.0f, 0.0f };

    bool m_isOffering { false };

    int m_numberOfFruitsForPickup { 1 };
    int m_oldNumberOfFruitsForPickup { 1 };
    std::vector<std::shared_ptr<jt::Animation>> m_fruitIcons {};

    float m_timerToRefillFruits { 0.0f };

    float m_flashTimer { 0.0f };
    float m_flashTimerMax = 0.75f;
};

#endif // JAMTEMPLATE_HARBOR_HPP
