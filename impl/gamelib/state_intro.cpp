#include "state_intro.hpp"
#include <audio/sound/sound.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <state_menu.hpp>
#include <tweens/tween_alpha.hpp>
#include <tweens/tween_position.hpp>

void StateIntro::onCreate()
{
    m_sprite = std::make_shared<jt::Sprite>("assets/runvs_logo.png", textureManager());
    m_sprite->setOrigin(jt::OriginMode::CENTER);
    //    m_sprite->setOffset(jt::OffsetMode::CENTER);
    m_sprite->setScale(jt::Vector2f { 0.5f, 0.5f });

    m_jingle = getGame()->audio().addTemporarySound("assets/intro.mp3");
}

void StateIntro::onEnter()
{
    jt::Vector2f const startPos { GP::GetScreenSize().x / 2,
        -m_sprite->getGlobalBounds().height + 50.0f };
    jt::Vector2f const endPos { GP::GetScreenSize().x / 2, GP::GetScreenSize().y / 2 };
    m_sprite->setPosition(startPos);
    m_sprite->update(0.0f);

    auto twp = jt::TweenPosition::create(m_sprite, 0.8f, startPos, endPos);
    add(twp);

    auto twa2 = jt::TweenAlpha::create(m_sprite, 0.3f, 255u, 0u);
    twa2->setStartDelay(1.4f);
    add(twa2);

    m_jingle->play();
}

void StateIntro::onUpdate(float elapsed)
{
    m_sprite->update(elapsed);
    if (getAge() >= 1.7) {
        getGame()->stateManager().switchState(std::make_shared<StateMenu>());
    }
}

void StateIntro::onDraw() const { m_sprite->draw(renderTarget()); }
