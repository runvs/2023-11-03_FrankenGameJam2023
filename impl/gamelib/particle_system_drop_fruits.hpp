#ifndef FRUITLOVINMONKEYPIRATES_PARTICLE_SYSTEM_DROP_FRUITS_HPP
#define FRUITLOVINMONKEYPIRATES_PARTICLE_SYSTEM_DROP_FRUITS_HPP

#include <animation.hpp>
#include <game_object.hpp>
#include <particle_system.hpp>
#include <tween_collection.hpp>

class ParticleSystemDropFruits : public jt::GameObject {
public:
    void fire(int number, jt::Vector2f const& pos);

private:
    void doCreate() override;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;

    std::shared_ptr<jt::ParticleSystem<jt::Animation, 12>> m_particles {};
    std::shared_ptr<jt::TweenCollection> m_tweens {};
};

#endif // FRUITLOVINMONKEYPIRATES_PARTICLE_SYSTEM_DROP_FRUITS_HPP
