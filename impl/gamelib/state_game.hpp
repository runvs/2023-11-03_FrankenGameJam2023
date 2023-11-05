#ifndef GAME_STATE_GAME_HPP
#define GAME_STATE_GAME_HPP

#include "particle_system_drop_fruits.hpp"
#include <audio/sound/sound_interface.hpp>
#include <box2dwrapper/box2d_world_interface.hpp>
#include <game_state.hpp>
#include <harbor/harbor.hpp>
#include <object_group.hpp>
#include <player/player.hpp>
#include <screeneffects/mario_clouds.hpp>
#include <screeneffects/trailing_waves.hpp>
#include <screeneffects/waves.hpp>
#include <tilemap/tile_layer.hpp>
#include "monkey/monkey.h"
#include <memory>
#include <vector>

// fwd decls
namespace jt {
class Shape;
class Sprite;
class Vignette;

namespace tilemap {
class TilesonLoader;
}
} // namespace jt

class Hud;

class StateGame : public jt::GameState {
public:
    std::string getName() const override;

private:
    std::shared_ptr<jt::Vignette> m_vignette;
    std::shared_ptr<Hud> m_hud;
    std::shared_ptr<jt::Box2DWorldInterface> m_world { nullptr };
    std::shared_ptr<Player> m_player { nullptr };

    std::shared_ptr<jt::ObjectGroup<Harbor>> m_harbors;
    std::shared_ptr<jt::ObjectGroup<Monkey>> m_monkeys;
    std::shared_ptr<jt::tilemap::TileLayer> m_tilemap {};
    std::shared_ptr<jt::tilemap::TileLayer> m_overlay {};
    std::vector<std::shared_ptr<jt::Box2DObject>> m_colliders {};
    std::vector<jt::Rectf> m_tileCollisionRects {};

    std::shared_ptr<jt::Waves> m_waves {};
    std::shared_ptr<ParticleSystemDropFruits> m_dropFruitPS {};
    std::shared_ptr<MarioClouds> m_clouds {};

    std::shared_ptr<jt::SoundInterface> m_soundFruitPickup;
    std::shared_ptr<jt::SoundInterface> m_soundFruitDeliver;
    std::shared_ptr<jt::SoundInterface> m_soundMonkeyHitsEnemy;
    std::vector<std::shared_ptr<jt::SoundInterface>> m_soundMonkeyScreams {};

    bool m_running { true };
    bool m_hasEnded { false };

    int m_points { 0 };
    jt::Vector2f m_lastFrameCameraOffset = jt::Vector2f { 0.0f, 0.0f };

    void onCreate() override;
    void onEnter() override;
    void onUpdate(float const elapsed) override;
    void onDraw() const override;

    void endGame();
    void createPlayer();
    void createHarbors(jt::tilemap::TilesonLoader& loader);
    void spawnMonkey();
    bool isValidMonkeySpawnPosition(jt::Vector2f position);
    void updateHarbors(float const elapsed);
    void updateMonkeys();
    void updateCamera(float const elapsed);
    void loadLevelCollisions(jt::tilemap::TilesonLoader& loader);
    void updatePlayer();
};

#endif
