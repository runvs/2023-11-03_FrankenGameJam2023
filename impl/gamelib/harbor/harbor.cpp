#include "harbor.hpp"
#include <game_properties.hpp>

Harbor::Harbor(jt::Vector2f const& pos, bool isOffering)
    : m_pos { pos }
    , m_isOffering { isOffering }
{
}

jt::Vector2f Harbor::getPosition() const { return m_pos; }

void Harbor::doCreate()
{
    m_interactionTimerBar = std::make_shared<jt::Bar>(4, 16, false, textureManager());
    m_interactionTimerBar->setMaxValue(GP::HarborInteractionTimerMax());
    m_interactionTimerBar->setPosition(m_pos + jt::Vector2f { 16.0f, 0.0f });
}

void Harbor::doUpdate(float const elapsed)
{
    m_interactionTimer -= elapsed;
    m_interactionTimerBar->setCurrentValue(m_interactionTimer);
    m_interactionTimerBar->update(elapsed);
}

void Harbor::doDraw() const
{
    if (m_interactionTimer > 0) {
        m_interactionTimerBar->draw(renderTarget());
    }
}

void Harbor::deliverFruit() { m_interactionTimer = GP::HarborInteractionTimerMax(); }

void Harbor::pickUpFruit()
{
    m_fruitOffer = "";
    m_interactionTimer = GP::HarborInteractionTimerMax();
}

bool Harbor::isOffering() const { return m_isOffering; }

std::string Harbor::getFruitOffering() const { return m_fruitOffer; }

bool Harbor::canBeInteractedWith() const { return m_interactionTimer <= 0; }
