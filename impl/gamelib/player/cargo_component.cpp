#include "cargo_component.hpp"
#include <game_properties.hpp>

void CargoComponent::addFruit(std::string const& fruit)
{
    m_fruits += 1;
    if (m_fruits > GP::PlayerMaxFruitInCargo) {
        m_fruits = GP::PlayerMaxFruitInCargo;
    }
}

void CargoComponent::removeAllFruits() { m_fruits = 0; }

void CargoComponent::removeFruits(int number)
{
    m_fruits -= number;
    if (m_fruits < 0) {
        m_fruits = 0;
    }
}

int CargoComponent::getNumberOfFruits() const { return m_fruits; }
