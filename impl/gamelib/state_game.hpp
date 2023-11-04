﻿#ifndef GAME_STATE_GAME_HPP
#define GAME_STATE_GAME_HPP

#include "tilemap/tile_layer.hpp"
#include <box2dwrapper/box2d_world_interface.hpp>
#include <game_state.hpp>
#include <harbor/harbor.hpp>
#include <object_group.hpp>
#include <player/player.hpp>
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
    std::vector<std::shared_ptr<jt::Box2DObject>> m_colliders {};

    bool m_running { true };
    bool m_hasEnded { false };

    int m_scoreP1 { 0 };
    int m_scoreP2 { 0 };

    void onCreate() override;
    void onEnter() override;
    void onUpdate(float const elapsed) override;
    void onDraw() const override;

    void endGame();
    void createPlayer();
    void createHarbors(jt::tilemap::TilesonLoader& loader);
    void spawnMonkey();
    void updateHarbors(float const elapsed);
    void updateCamera(float const elapsed);
    void loadLevelCollisions(jt::tilemap::TilesonLoader& loader);
};

#endif
