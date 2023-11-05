#ifndef JAMTEMPLATE_MONKEY_H
#define JAMTEMPLATE_MONKEY_H

#include "audio/sound/sound_group.hpp"
#include <box2dwrapper/box2d_object.hpp>
#include <box2dwrapper/box2d_world_interface.hpp>
#include <game_object.hpp>
#include <player/graphics/graphics_component_interface.hpp>
#include <screeneffects/trailing_waves.hpp>
#include "monkey/ai/ai_component_interface.h"

class Monkey : public jt::GameObject {
public:
    Monkey(std::shared_ptr<jt::Box2DWorldInterface> world, jt::Vector2f const position, std::shared_ptr<jt::SoundInterface> sound);

    jt::Vector2f getPosition() const;

    GraphicsComponentInterface& getGraphics();

    virtual void updatePlayerPosition(jt::Vector2f const playerPos);

    bool canAttack() const;
    void attack();

    void clampPositionOnMap(jt::Vector2f const& mapSize);
    void setAggroRange(float const aggroRange);

private:
    std::unique_ptr<GraphicsComponentInterface> m_graphics { nullptr };
    std::unique_ptr<AiComponentInterface> m_ai { nullptr };

    std::unique_ptr<jt::Box2DObject> m_b2Object { nullptr };

    std::shared_ptr<jt::TrailingWaves> m_trailingWaves { nullptr };
    std::shared_ptr<jt::SoundInterface> m_soundScreams { nullptr };

    float m_attackTimer { 0.0f };
    float m_randomSpeedMultiplier { 1.0f };

    virtual void doCreate();
    virtual void doUpdate(float const elapsed);
    virtual void doDraw() const;
};

#endif // JAMTEMPLATE_MONKEY_H
