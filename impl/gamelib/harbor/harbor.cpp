#include "harbor.hpp"
#include <game_properties.hpp>

Harbor::Harbor(jt::Vector2f const& pos)
    : m_pos { pos }
{
    m_isOffering = true;
}

jt::Vector2f Harbor::getPosition() const { return m_pos; }

void Harbor::doCreate()
{
    m_offerTimerBar = std::make_shared<jt::Bar>(4, 16, false, textureManager());
    m_offerTimerBar->setMaxValue(GP::OfferTimerMaxValue());
    m_offerTimerBar->setPosition(m_pos + jt::Vector2f { 16.0f, 0.0f });
}

void Harbor::doUpdate(float const elapsed)
{
    m_offerTimer -= elapsed;
    m_offerTimerBar->setCurrentValue(m_offerTimer);
    m_offerTimerBar->update(elapsed);
}

void Harbor::doDraw() const
{
    if (m_offerTimer > 0) {
        m_offerTimerBar->draw(renderTarget());
    }
}

void Harbor::deliverFruit() { }

void Harbor::pickUpFruit()
{
    m_fruitOffer = "";
    m_offerTimer = GP::OfferTimerMaxValue();
}

bool Harbor::isOffering() const { return m_isOffering; }

std::string Harbor::getFruitOffering() const { return m_fruitOffer; }

bool Harbor::hasFruitToOffer() const { return m_offerTimer <= 0; }
