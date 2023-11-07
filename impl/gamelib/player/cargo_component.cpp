#include "cargo_component.hpp"
#include <game_properties.hpp>

void CargoComponent::addFruit(std::string const& fruit)
{
    m_fruits += 1;
    if (m_fruits > GP::PlayerMaxFruitInCargo) {
        m_fruits = GP::PlayerMaxFruitInCargo;
    }
}

void CargoComponent::removeFruit(std::string const& fruit)
{
    if (m_fruits <= 0) {
        return;
    }
    --m_fruits;
}

int CargoComponent::getNumberOfFruits() const { return m_fruits; }
