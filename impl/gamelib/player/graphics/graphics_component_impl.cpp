#include "graphics_component_impl.hpp"
#include <game_properties.hpp>
#include <math_helper.hpp>

GraphicsComponentImpl::GraphicsComponentImpl(
    std::shared_ptr<jt::GameInterface> gameInterface, std::string asepriteFileName)
{
    createAnimation(gameInterface->gfx().textureManager(), asepriteFileName);
}

void GraphicsComponentImpl::createAnimation(
    jt::TextureManagerInterface& textureManager, std::string asepriteFileName)
{
    m_animation = std::make_shared<jt::Animation>();
    m_animation->loadFromAseprite(asepriteFileName, textureManager);
    m_animation->play("right");
    m_animation->setZ(GP::ZLayerBoat);
}

void GraphicsComponentImpl::updateGraphics(float elapsed) { m_animation->update(elapsed); }

void GraphicsComponentImpl::setPosition(jt::Vector2f const& playerPosition)
{
    auto spritePosition = playerPosition - GP::PlayerSize() * 0.5f;

    m_animation->setPosition(spritePosition);
}

void GraphicsComponentImpl::draw(std::shared_ptr<jt::RenderTargetInterface> target)
{
    m_animation->draw(target);
}

void GraphicsComponentImpl::flash(float time, jt::Color const& color)
{
    m_animation->flash(time, color);
}

bool GraphicsComponentImpl::setAnimationIfNotSet(std::string const& newAnimationName)
{
    if (newAnimationName == "") {
        return false;
    }
    std::string const currentAnimationName = m_animation->getCurrentAnimationName();

    if (currentAnimationName != newAnimationName) {
        m_animation->play(newAnimationName);
        return true;
    }
    return false;
}

std::string GraphicsComponentImpl::getCurrentAnimation() const
{
    return m_animation->getCurrentAnimationName();
}

std::shared_ptr<jt::DrawableInterface> GraphicsComponentImpl::getDrawable() { return m_animation; }
