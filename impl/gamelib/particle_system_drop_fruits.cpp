
#include "particle_system_drop_fruits.hpp"
#include <random/random.hpp>
#include <system_helper.hpp>
#include <tweens/tween_alpha.hpp>
#include <tweens/tween_position.hpp>

void ParticleSystemDropFruits::doCreate()
{
    m_tweens = std::make_shared<jt::TweenCollection>();

    m_particles = jt::ParticleSystem<jt::Animation, 12>::createPS(
        [this]() {
            auto a = std::make_shared<jt::Animation>();
            a->loadFromAseprite("assets/fruits.aseprite", textureManager());
            a->play(a->getRandomAnimationName());

            a->setOffset(jt::OffsetMode::CENTER);
            a->setPosition(jt::Vector2f { -2000.0f, -2000.0f });
            return a;
        },
        [this](auto& a, auto pos) {
            auto const tweenTime = 0.7f;
            a->setPosition(pos);
            a->setColor(jt::colors::White);
            a->update(0.0f);

            std::shared_ptr<jt::Tween> twp1 = jt::TweenPosition::create(
                a, tweenTime / 2.0f, pos, pos + jt::Random::getRandomPointInCircle(48.0f));
            twp1->addCompleteCallback([this, tweenTime, &a]() { a->flicker(tweenTime / 2.0f); });
            m_tweens->add(twp1);

            auto const fadeOutDuration = 0.05f;
            std::shared_ptr<jt::Tween> twa = jt::TweenAlpha::create(a, fadeOutDuration, 255u, 0u);
            twa->setStartDelay(tweenTime - fadeOutDuration);
            m_tweens->add(twa);
        });
    m_particles->setGameInstance(getGame());
    m_particles->create();
}

void ParticleSystemDropFruits::doUpdate(float const elapsed)
{
    m_tweens->update(elapsed);

    m_particles->update(elapsed);
}

void ParticleSystemDropFruits::doDraw() const { m_particles->draw(); }

void ParticleSystemDropFruits::fire(int number, jt::Vector2f const& pos)
{
    m_particles->fire(number, pos);
}
