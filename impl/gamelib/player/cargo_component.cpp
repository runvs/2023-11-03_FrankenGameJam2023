#include "cargo_component.hpp"

void CargoComponent::addFruit(std::string const& fruit) { ++m_fruits; }

void CargoComponent::removeFruit(std::string const& fruit)
{
    if (m_fruits <= 0) {
        return;
    }
    --m_fruits;
}
