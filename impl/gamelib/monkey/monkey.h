#ifndef JAMTEMPLATE_MONKEY_H
#define JAMTEMPLATE_MONKEY_H

#include "box2dwrapper/box2d_object.hpp"
#include "box2dwrapper/box2d_world_interface.hpp"
#include "game_object.hpp"
#include "player/graphics/graphics_component_interface.hpp"
#include "player/sound/sound_component_interface.hpp"
#include "monkey/ai/ai_component_interface.h"

class Monkey : public jt::GameObject {
public:
    Monkey(std::shared_ptr<jt::Box2DWorldInterface> world, jt::Vector2f const position);

    jt::Vector2f getPosition() const;

    GraphicsComponentInterface& getGraphics();

private:
    std::unique_ptr<GraphicsComponentInterface> m_graphics { nullptr };
    std::unique_ptr<SoundComponentInterface> m_sound { nullptr };
    std::unique_ptr<AiComponentInterface> m_ai { nullptr };

    std::unique_ptr<jt::Box2DObject> m_b2Object { nullptr };

    virtual void doCreate();
    virtual void doUpdate(float const elapsed);
    virtual void doDraw() const;
};

#endif // JAMTEMPLATE_MONKEY_H
