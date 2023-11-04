#ifndef JAMTEMPLATE_CARGO_COMPONENT_HPP
#define JAMTEMPLATE_CARGO_COMPONENT_HPP

#include <string>

class CargoComponent {
public:
    void addFruit(std::string const& fruit);
    void removeFruit(std::string const& fruit);

private:
    int m_fruits { 0 };
};

#endif // JAMTEMPLATE_CARGO_COMPONENT_HPP
