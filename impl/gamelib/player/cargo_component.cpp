#include "cargo_component.hpp"
#include <iostream>

void CargoComponent::addFruit(std::string const& fruit)
{
    m_fruits += 1;
    std::cout << "add fruit " << m_fruits << std::endl;
}

void CargoComponent::removeFruit(std::string const& fruit)
{
    if (m_fruits <= 0) {
        return;
    }
    --m_fruits;
}

int CargoComponent::getNumberOfFruits() const { return m_fruits; }
