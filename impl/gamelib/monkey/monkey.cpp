#include "monkey.h"
#include "player/graphics/graphics_component_impl.hpp"
#include "monkey/ai/ai_component_impl.h"

Monkey::Monkey(std::shared_ptr<jt::Box2DWorldInterface> world, jt::Vector2f const position) {
    b2BodyDef def {};
    def.type = b2BodyType::b2_dynamicBody;
    def.linearDamping = 1.0;
    def.angularDamping = 1.0;
    m_b2Object = std::make_unique<jt::Box2DObject>(world, &def);
    m_b2Object->setPosition(position);
}

jt::Vector2f Monkey::getPosition() const { return m_b2Object->getPosition(); }

GraphicsComponentInterface& Monkey::getGraphics()
{
    return *m_graphics;
}

void Monkey::doCreate()
{
    m_graphics = std::make_unique<GraphicsComponentImpl>(getGame(), "assets/affe.aseprite");
    m_ai = std::make_unique<AiComponentImpl>();
}

void Monkey::doUpdate(float const elapsed)
{
    m_ai->doUpdate();
    m_graphics->setPosition(m_b2Object->getPosition());
    m_graphics->updateGraphics(elapsed);
}

void Monkey::doDraw() const { m_graphics->draw(renderTarget()); }
