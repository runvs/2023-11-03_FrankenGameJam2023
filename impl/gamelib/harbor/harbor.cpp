#include "harbor.hpp"
#include "math_helper.hpp"
#include "random/random.hpp"
#include <game_properties.hpp>
#include <system_helper.hpp>

Harbor::Harbor(jt::Vector2f const& pos, bool isOffering)
    : m_pos { pos }
    , m_isOffering { isOffering }
{
}

jt::Vector2f Harbor::getPosition() const { return m_pos; }

void Harbor::doCreate()
{
    m_numberOfFruitsForPickup = jt::Random::getInt(1, 3);
    for (auto i = 0; i != 5; ++i) {
        auto anim = std::make_shared<jt::Animation>();
        anim->loadFromAseprite("assets/fruits.aseprite", textureManager());
        auto const allAnims = anim->getAllAvailableAnimationsNames();
        auto animName = *jt::SystemHelper::select_randomly(allAnims.cbegin(), allAnims.cend());
        anim->play(animName);
        anim->setPosition(getPosition() + jt::Vector2f { -20 + i * 20.0f, -18.0f });
        anim->setOutline(jt::Color { 10, 10, 10, 175 }, 1);
        anim->setShadow(jt::Color { 10, 10, 10, 100 }, { 4, 1 });
        m_fruitIcons.push_back(anim);
    }
}

void Harbor::doUpdate(float const elapsed)
{
    for (auto& anim : m_fruitIcons) {
        anim->update(elapsed);
    }
    m_flashTimer -= elapsed;
    if (m_isOffering) {
        if (m_numberOfFruitsForPickup == 0) {
            if (m_flashTimer > 0) {
                auto const v = static_cast<std::uint8_t>(
                    255 * jt::MathHelper::clamp(m_flashTimer / m_flashTimerMax, 0.0f, 1.0f));
                for (auto& a : m_fruitIcons) {
                    a->setColor(jt::Color { 255, 255, 255, v });
                }
            } else {
                for (auto& a : m_fruitIcons) {
                    a->setColor(jt::Color { 255, 255, 255, 0 });
                }
            }

            m_timerToRefillFruits -= elapsed;
            if (m_timerToRefillFruits <= 0) {
                m_timerToRefillFruits = 0.0f;
                m_numberOfFruitsForPickup = jt::Random::getInt(1, 5);
            }
        } else {
            for (auto& a : m_fruitIcons) {
                a->setColor(jt::Color { 255, 255, 255, 255 });
            }
        }
    }
}

void Harbor::doDraw() const
{
    if (m_isOffering) {
        for (auto i = 0; i != m_numberOfFruitsForPickup; ++i) {
            m_fruitIcons.at(i)->draw(renderTarget());
        }
    }
    if (m_flashTimer >= 0) {
        if (m_numberOfFruitsForPickup == 0) {
            for (auto i = 0; i != m_oldNumberOfFruitsForPickup; ++i) {
                m_fruitIcons.at(i)->draw(renderTarget());
            }
        }
    }
}

void Harbor::deliverFruit() { }

void Harbor::pickUpFruit()
{
    m_oldNumberOfFruitsForPickup = m_numberOfFruitsForPickup;
    m_numberOfFruitsForPickup = 0;
    m_timerToRefillFruits = 15.0f;

    m_flashTimer = 0.75f;
    for (auto& f : m_fruitIcons) {
        f->flash(0.75f);
    }
}

bool Harbor::isOffering() const { return m_isOffering; }

bool Harbor::canBeInteractedWith() const
{
    if (m_isOffering) {
        return m_numberOfFruitsForPickup != 0;
    }
    return true;
}

int Harbor::getNumberOfFruitsToPickup() const { return m_numberOfFruitsForPickup; }
