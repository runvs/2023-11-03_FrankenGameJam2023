#include "monkey.h"
#include <math_helper.hpp>
#include <player/graphics/graphics_component_impl.hpp>
#include <monkey/ai/ai_component_impl.h>

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

Monkey::Monkey(std::shared_ptr<jt::Box2DWorldInterface> world, jt::Vector2f const position)
{
    b2BodyDef def {};
    def.type = b2BodyType::b2_dynamicBody;
    def.linearDamping = 1.0;
    def.angularDamping = 1.0;
    m_b2Object = std::make_unique<jt::Box2DObject>(world, &def);
    m_b2Object->setPosition(position);
}

jt::Vector2f Monkey::getPosition() const { return m_b2Object->getPosition(); }

GraphicsComponentInterface& Monkey::getGraphics() { return *m_graphics; }

void Monkey::doCreate()
{
    b2FixtureDef fixtureDef;
    b2CircleShape circleCollider {};
    circleCollider.m_radius = 8.0f;
    fixtureDef.shape = &circleCollider;
    m_b2Object->getB2Body()->CreateFixture(&fixtureDef);

    m_graphics = std::make_unique<GraphicsComponentImpl>(getGame(), "assets/affe.aseprite");
    m_ai = std::make_unique<AiComponentImpl>();

    m_trailingWaves = std::make_shared<jt::TrailingWaves>();
    m_trailingWaves->setGameInstance(getGame());
    m_trailingWaves->create();
    m_trailingWaves->setMaxAlpha(120u);
}

void Monkey::doUpdate(float const elapsed)
{
    m_ai->update(*m_b2Object, elapsed);
    m_graphics->setPosition(m_b2Object->getPosition());
    m_graphics->setAnimationIfNotSet(selectWalkAnimation(m_ai->getRotationAngle()));
    m_graphics->updateGraphics(elapsed);

    m_attackTimer -= elapsed;

    m_trailingWaves->update(elapsed);
    m_trailingWaves->setPosition(getPosition());
    if (jt::MathHelper::lengthSquared(m_b2Object->getVelocity()) > 25.0f) {
        m_trailingWaves->setTimerMax(0.2f);
    } else {
        m_trailingWaves->setTimerMax(-1.0f);
    }
}

void Monkey::clampPositionOnMap(jt::Vector2f const& mapSize)
{
    auto pos = m_b2Object->getPosition();
    pos = jt::MathHelper::clamp(pos, { 0.0f, 0.0f }, mapSize);
    m_b2Object->setPosition(pos);
}

void Monkey::doDraw() const
{
    m_graphics->draw(renderTarget());
    m_trailingWaves->draw();
}

void Monkey::updatePlayerPosition(jt::Vector2f const playerPos)
{
    m_ai->updatePlayerPosition(playerPos);
}

bool Monkey::canAttack() const { return m_attackTimer <= 0; }

void Monkey::attack() { m_attackTimer = 1.0f; }
