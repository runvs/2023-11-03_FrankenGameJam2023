﻿#include "state_menu.hpp"
#include <build_info.hpp>
#include <color/color.hpp>
#include <drawable_helpers.hpp>
#include <game_interface.hpp>
#include <game_properties.hpp>
#include <input/input_manager.hpp>
#include <lerp.hpp>
#include <log/license_info.hpp>
#include <math_helper.hpp>
#include <oalpp/effects/utility/gain.hpp>
#include <screeneffects/vignette.hpp>
#include <state_game.hpp>
#include <state_manager/state_manager_transition_fade_to_black.hpp>
#include <text.hpp>
#include <tweens/tween_alpha.hpp>
#include <tweens/tween_color.hpp>
#include <tweens/tween_position.hpp>

void StateMenu::onCreate()
{
    getGame()->gfx().createZLayer(GP::ZLayerBoat);
    getGame()->gfx().createZLayer(GP::ZLayerClouds);
    getGame()->gfx().createZLayer(GP::ZLayerHUD);
    createMenuText();
    createShapes();
    createVignette();

    add(std::make_shared<jt::LicenseInfo>());

    getGame()->stateManager().setTransition(std::make_shared<jt::StateManagerTransitionFadeToBlack>(
        GP::GetScreenSize(), textureManager()));

    oalpp::effects::utility::Gain gain { 1.0f };
    auto bgma = getGame()->audio().getPermanentSound("bgma");
    if (!bgma) {
        bgma = getGame()->audio().addPermanentSound(
            "bgma", "assets/sfx/theme-A-start.ogg", "assets/sfx/theme-A-loop.ogg", gain);
        bgma->setVolumeGroup("music");
        bgma->setVolume(0.8f);
        bgma->play();
    }

    auto bgmb = getGame()->audio().getPermanentSound("bgmb");
    if (!bgmb) {
        bgmb = getGame()->audio().addPermanentSound(
            "bgmb", "assets/sfx/theme-B-start.ogg", "assets/sfx/theme-B-loop.ogg", gain);
        bgmb->setVolumeGroup("music");
        bgmb->setVolume(0.8f);
        bgmb->play();
    }
    bgmb->setVolume(0.0f);

    auto bgmc = getGame()->audio().getPermanentSound("bgmc");
    if (!bgmc) {
        bgmc = getGame()->audio().addPermanentSound("bgmc", "assets/sfx/theme-C.ogg");
        bgmc->setVolumeGroup("music");
        bgmc->setLoop(true);
        bgmc->play();
    }
    bgmc->setVolume(0.0f);

    m_vinyl = std::make_shared<jt::Animation>();
    m_vinyl->loadFromAseprite("assets/disc.aseprite", textureManager());
    m_vinyl->setScale({ 2.0f, 2.0f });
    m_vinyl->setLoopingAll(false);
    m_vinyl->setAnimationSpeedFactor(4.0f);
    m_vinyl->play("2to0", true);
    m_vinyl->setPosition(jt::Vector2f { 280, 100 });
}

void StateMenu::onEnter()
{
    createTweens();
    m_started = false;
}

void StateMenu::createVignette()
{
    m_vignette = std::make_shared<jt::Vignette>(GP::GetScreenSize());
    add(m_vignette);
}

void StateMenu::createShapes()
{
    m_background = jt::dh::createShapeRect(
        GP::GetScreenSize(), jt::Color { 97, 162, 255, 255 }, textureManager());
    m_title = std::make_shared<jt::Animation>();
    m_title->loadFromAseprite("assets/title.aseprite", textureManager());
    m_title->play("idle");
    m_overlay = jt::dh::createShapeRect(GP::GetScreenSize(), jt::colors::Black, textureManager());
}

void StateMenu::createMenuText()
{
    createTextTitle();
    createTextStart();
    createTextExplanation();
    createTextCredits();
}

void StateMenu::createTextExplanation()
{
    m_textExplanation = jt::dh::createText(
        renderTarget(), GP::ExplanationText(), 16u, jt::Color { 146, 65, 243, 255 });
    auto const half_width = GP::GetScreenSize().x / 2.0f;
    m_textExplanation->setPosition({ half_width, 100 });
    m_textExplanation->setShadow(jt::Color { 97, 16, 162 }, jt::Vector2f { 1, 1 });
}

void StateMenu::createTextCredits()
{
    m_textCredits = jt::dh::createText(renderTarget(),
        "Created by " + GP::AuthorName() + " for " + GP::JamName() + "\nF9 for License Information",
        14u, jt::Color { 146, 65, 243, 255 });
    m_textCredits->setTextAlign(jt::Text::TextAlign::LEFT);
    m_textCredits->setPosition({ 10, GP::GetScreenSize().y - 70 });
    m_textCredits->setShadow(jt::Color { 97, 16, 162 }, jt::Vector2f { 1, 1 });

    m_textVersion = jt::dh::createText(renderTarget(), "", 14u, jt::Color { 146, 65, 243, 255 });
    if (jt::BuildInfo::gitTagName() != "") {
        m_textVersion->setText(jt::BuildInfo::gitTagName());
    } else {
        m_textVersion->setText(
            jt::BuildInfo::gitCommitHash().substr(0, 6) + " " + jt::BuildInfo::timestamp());
    }
    m_textVersion->setTextAlign(jt::Text::TextAlign::RIGHT);
    m_textVersion->setPosition({ GP::GetScreenSize().x - 5.0f, GP::GetScreenSize().y - 20.0f });
    m_textVersion->setShadow(jt::Color { 97, 16, 162 }, jt::Vector2f { 1, 1 });
}

void StateMenu::createTextStart()
{
    auto const half_width = GP::GetScreenSize().x / 2.0f;
    m_textStart = jt::dh::createText(
        renderTarget(), "Press Space to start the game", 16u, GP::PaletteFontFront());
    m_textStart->setPosition({ half_width, 77 });
    m_textStart->setShadow(jt::Color { 97, 16, 162 }, jt::Vector2f { 2, 2 });
}

void StateMenu::createTextTitle()
{
    float half_width = GP::GetScreenSize().x / 2;
    m_textTitle
        = jt::dh::createText(renderTarget(), GP::GameName(), 32u, jt::Color { 146, 65, 243, 255 });
    m_textTitle->setPosition({ half_width, -5 });
    m_textTitle->setShadow(GP::PaletteFontShadow(), jt::Vector2f { 1, 1 });
}

void StateMenu::createTweens()
{
    createTweenOverlayAlpha();
    createTweenTitleAlpha();
    createTweenCreditsPosition();
    createTweenExplanation();
}

void StateMenu::createInstructionTweenColor1()
{
    auto tc = jt::TweenColor::create(
        m_textStart, 0.5f, GP::PaletteFontFront(), GP::PalleteFrontHighlight());
    tc->addCompleteCallback([this]() { createInstructionTweenColor2(); });
    tc->setAgePercentConversion([](float age) {
        return jt::Lerp::cubic(0.0f, 1.0f, jt::MathHelper::clamp(age, 0.0f, 1.0f));
    });
    add(tc);
}

void StateMenu::createInstructionTweenColor2()
{
    auto tc = jt::TweenColor::create(
        m_textStart, 0.45f, GP::PalleteFrontHighlight(), GP::PaletteFontFront());
    tc->setAgePercentConversion([](float age) {
        return jt::Lerp::cubic(0.0f, 1.0f, jt::MathHelper::clamp(age, 0.0f, 1.0f));
    });
    tc->setStartDelay(0.2f);
    tc->addCompleteCallback([this]() { createInstructionTweenColor1(); });
    add(tc);
}

void StateMenu::createTweenExplanation()
{
    auto const start = m_textStart->getPosition() + jt::Vector2f { -1000, 0 };
    auto const end = m_textStart->getPosition();

    auto tween = jt::TweenPosition::create(m_textStart, 0.5f, start, end);
    tween->setStartDelay(0.3f);
    tween->setSkipFrames();

    tween->addCompleteCallback([this]() { createInstructionTweenColor1(); });
    add(tween);
}

void StateMenu::createTweenTitleAlpha()
{
    auto tween = jt::TweenAlpha::create(m_textTitle, 0.55f, 0, 255);
    tween->setStartDelay(0.2f);
    tween->setSkipFrames();
    add(tween);
}

void StateMenu::createTweenOverlayAlpha()
{
    auto tween = jt::TweenAlpha::create(m_overlay, 0.5f, std::uint8_t { 255 }, std::uint8_t { 0 });
    tween->setSkipFrames();
    add(tween);
}

void StateMenu::createTweenCreditsPosition()
{
    auto creditsPositionStart = m_textCredits->getPosition() + jt::Vector2f { 0, 150 };
    auto creditsPositionEnd = m_textCredits->getPosition();

    auto tweenCredits
        = jt::TweenPosition::create(m_textCredits, 0.75f, creditsPositionStart, creditsPositionEnd);
    tweenCredits->setStartDelay(0.8f);
    tweenCredits->setSkipFrames();
    add(tweenCredits);

    auto versionPositionStart = m_textVersion->getPosition() + jt::Vector2f { 0, 150 };
    auto versionPositionEnd = m_textVersion->getPosition();
    auto tweenVersion
        = jt::TweenPosition::create(m_textVersion, 0.75f, versionPositionStart, versionPositionEnd);
    tweenVersion->setStartDelay(0.8f);
    tweenVersion->setSkipFrames();
    add(tweenVersion);
}

void StateMenu::onUpdate(float const elapsed)
{
    updateDrawables(elapsed);
    checkForTransitionToStateGame();
    m_vinyl->update(elapsed);

    if (keyboard()->justPressed(jt::KeyCode::B)) {
        auto bgma = getGame()->audio().getPermanentSound("bgma");
        auto bgmb = getGame()->audio().getPermanentSound("bgmb");
        auto bgmc = getGame()->audio().getPermanentSound("bgmc");
        auto aVolume = bgma ? bgma->getVolume() : 0.0f;
        auto bVolume = bgmb ? bgmb->getVolume() : 0.0f;
        auto cVolume = bgmc ? bgmc->getVolume() : 0.0f;

        selected++;
        if (selected == 3) {
            selected = 0;
        }
        auto const time = 0.25f;
        if (selected == 0) {
            getGame()->audio().fades().volumeFade(bgma, time, aVolume, 1.0f);
            getGame()->audio().fades().volumeFade(bgmb, time, bVolume, 0.0f);
            getGame()->audio().fades().volumeFade(bgmc, time, cVolume, 0.0f);
            m_vinyl->play("2to0", true);
        } else if (selected == 1) {
            getGame()->audio().fades().volumeFade(bgma, time, aVolume, 0.0f);
            getGame()->audio().fades().volumeFade(bgmb, time, bVolume, 1.0f);
            getGame()->audio().fades().volumeFade(bgmc, time, cVolume, 0.0f);
            m_vinyl->play("0to1", true);

        } else {
            getGame()->audio().fades().volumeFade(bgma, time, aVolume, 0.0f);
            getGame()->audio().fades().volumeFade(bgmb, time, bVolume, 0.0f);
            getGame()->audio().fades().volumeFade(bgmc, time, cVolume, 1.0f);
            m_vinyl->play("1to2", true);
        }
    }
}

void StateMenu::updateDrawables(float const& elapsed)
{
    m_background->update(elapsed);
    m_title->update(elapsed);
    m_textTitle->update(elapsed);
    m_textStart->update(elapsed);
    m_textExplanation->update(elapsed);
    m_textCredits->update(elapsed);
    m_textVersion->update(elapsed);
    m_overlay->update(elapsed);
    m_vignette->update(elapsed);
}

void StateMenu::checkForTransitionToStateGame()
{
    auto const keysToTriggerTransition = { jt::KeyCode::Space, jt::KeyCode::Enter };

    if (std::any_of(keysToTriggerTransition.begin(), keysToTriggerTransition.end(),
            [this](auto const k) { return getGame()->input().keyboard()->justPressed(k); })) {
        startTransitionToStateGame();
    }
}

void StateMenu::startTransitionToStateGame()
{
    if (!m_started) {
        m_started = true;
        getGame()->stateManager().storeCurrentState("menu");
        getGame()->stateManager().switchState(std::make_shared<StateGame>());
    }
}

void StateMenu::onDraw() const
{
    m_background->draw(renderTarget());

    m_title->draw(renderTarget());
    m_vinyl->draw(renderTarget());
    m_textStart->draw(renderTarget());
    m_textExplanation->draw(renderTarget());
    m_textCredits->draw(renderTarget());
    m_textVersion->draw(renderTarget());
    m_overlay->draw(renderTarget());
    m_vignette->draw();
}

std::string StateMenu::getName() const { return "State Menu"; }
