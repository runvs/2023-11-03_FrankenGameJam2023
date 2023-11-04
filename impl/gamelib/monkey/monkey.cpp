#include "monkey.h"
#include "player/graphics/graphics_component_impl.hpp"
#include "monkey/ai/ai_component_impl.h"

namespace {
std::string selectWalkAnimation(float const a)
{
    float clampedValue = a;

    while (clampedValue < 360) {
        clampedValue += 360;
    }
    while (clampedValue > 360) {
        clampedValue -= 360;
    }

    if (clampedValue < 22.5) {
        return "right";
    } else if (clampedValue < 22.5f + 45 * 1) {
        return "up-right";
    } else if (clampedValue < 22.5f + 45 * 2) {
        return "up";
    } else if (clampedValue < 22.5f + 45 * 3) {
        return "up-left";
    } else if (clampedValue < 22.5f + 45 * 4) {
        return "left";
    } else if (clampedValue < 22.5f + 45 * 5) {
        return "down-left";
    } else if (clampedValue < 22.5f + 45 * 6) {
        return "down";
    } else if (clampedValue < 22.5f + 45 * 7) {
        return "down-right";
    }
    return "right";
}
} // namespace

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
    m_ai->update(*m_b2Object, elapsed);
    m_graphics->setPosition(m_b2Object->getPosition());
    m_graphics->setAnimationIfNotSet(selectWalkAnimation(m_ai->getRotationAngle()));
    m_graphics->updateGraphics(elapsed);
}

void Monkey::doDraw() const { m_graphics->draw(renderTarget()); }

void Monkey::updatePlayerPosition(jt::Vector2f const playerPos) {
    m_ai->updatePlayerPosition(playerPos);
}
