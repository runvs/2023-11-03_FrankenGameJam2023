#ifndef JAMTEMPLATE_HARBOR_HPP
#define JAMTEMPLATE_HARBOR_HPP

#include <bar.hpp>
#include <game_object.hpp>

class Harbor : public jt::GameObject {
public:
    explicit Harbor(jt::Vector2f const& pos);

    jt::Vector2f getPosition() const;

    void pickUpFruit();
    void deliverFruit();

    bool isOffering() const;
    std::string getFruitOffering() const;

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    jt::Vector2f m_pos { 0.0f, 0.0f };

    bool m_isOffering { false };
    std::string m_fruitOffer { "" };
    float m_offerTimer { 0.0f };
    std::shared_ptr<jt::Bar> m_offerTimerBar { nullptr };
};

#endif // JAMTEMPLATE_HARBOR_HPP
