#ifndef JAMTEMPLATE_PLAYER_HPP
#define JAMTEMPLATE_PLAYER_HPP

#include "bar.hpp"
#include <box2dwrapper/box2d_object.hpp>
#include <game_object.hpp>
#include <player/cargo_component.hpp>
#include <player/graphics/graphics_component_interface.hpp>
#include <player/input/input_component_interface.hpp>
#include <screeneffects/trailing_waves.hpp>

class StateGame;

class Player : public jt::GameObject {
public:
    Player(std::shared_ptr<jt::Box2DWorldInterface> world);

    jt::Vector2f getPosition() const;
    void setPosition(jt::Vector2f const&);
    jt::Vector2f getVelocity() const;

    GraphicsComponentInterface& getGraphics();
    CargoComponent& getCargo();

    void clampPositionOnMap(jt::Vector2f const& mapSize);

    void getDamage();

    int getHealth() const;

private:
    std::unique_ptr<InputComponentInterface> m_input { nullptr };
    std::unique_ptr<GraphicsComponentInterface> m_graphics { nullptr };

    std::unique_ptr<jt::Box2DObject> m_b2Object { nullptr };

    std::shared_ptr<jt::Bar> m_NitroBar { nullptr };

    std::shared_ptr<jt::TrailingWaves> m_trailingWaves { nullptr };

    std::shared_ptr<jt::Animation> m_heartContainer { nullptr };
    int m_health { 5 };

    CargoComponent m_cargo;

    virtual void doCreate();
    virtual void doUpdate(float const elapsed);
    virtual void doDraw() const;
};

#endif // JAMTEMPLATE_PLAYER_HPP
