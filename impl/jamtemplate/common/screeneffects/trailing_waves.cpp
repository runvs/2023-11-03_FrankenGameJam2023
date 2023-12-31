#include "trailing_waves.hpp"
#include <random/random.hpp>
#include <tweens/tween_alpha.hpp>

void jt::TrailingWaves::doCreate()
{
    m_tweens = std::make_shared<jt::TweenCollection>();
    m_particles = ParticleSystemType::createPS(
        [this]() {
            auto a = std::make_shared<jt::Animation>();
            a->loadFromAseprite("assets/circles.aseprite", textureManager());
            a->play("idle");

            a->setOffset(jt::OffsetMode::CENTER);
            a->setPosition(jt::Vector2f { -2000.0f, -2000.0f });
            return a;
        },
        [this](auto& a, auto pos) {
            auto const alpha = jt::MathHelper::clamp(
                static_cast<std::uint8_t>(jt::Random::getFloat(0.7f, 1.3f) * m_maxAlpha),
                std::uint8_t { 0u }, std::uint8_t { 255u });
            jt::Color startColor { 255u, 255u, 255u, alpha };
            a->setColor(startColor);
            a->setPosition(pos);
            a->play("idle", 0, true);
            a->update(0.0f);

            auto twa = jt::TweenAlpha::create(a,
                a->getCurrentAnimTotalTime() * jt::Random::getFloat(0.75f, 0.95f), startColor.a,
                0u);
            this->m_tweens->add(twa);
        });

    m_particles->setGameInstance(getGame());
}

void jt::TrailingWaves::doUpdate(float const elapsed)
{
    m_tweens->update(elapsed);
    m_particles->update(elapsed);

    m_timer += elapsed;
    if (m_timerMax > 0) {
        if (m_timer >= m_timerMax) {
            m_timer = 0.0f;
            m_particles->fire(1, m_pos);
        }
    }
}

void jt::TrailingWaves::doDraw() const { m_particles->draw(); }

void jt::TrailingWaves::setPosition(jt::Vector2f const& pos) { m_pos = pos; }

void jt::TrailingWaves::setTimerMax(float max) { m_timerMax = max; }

void jt::TrailingWaves::setMaxAlpha(std::uint8_t maxAlpha) { m_maxAlpha = maxAlpha; }
