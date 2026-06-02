#ifndef INPUTMGR_HPP_
#define INPUTMGR_HPP_

#include <raylib.h>
#include "typedef.hpp"
#include "worldstate.hpp"

class ActionManager {
private:
    const usize STEPS;
    const f32 INV_STEPS;

    WorldState& worldstate;
    WorldElement pl_idle, pl_move_up_right, pl_move_down_left, pl_move_fast, movable_crate;
    usize pl_x, pl_y;

    El MOVABLE_CRATE() {
        return El(
            util::cfg_usize("Resources.Tiledata", "MOVABLE_CRATE_TILESET"),
            util::cfg_usize("Resources.Tiledata", "MOVABLE_CRATE_INDEX")
        );
    }

    El PLAYER_IDLE() {
        return El(
            util::cfg_usize("Resources.Tiledata", "PLAYER_IDLE_TILESET"),
            util::cfg_usize("Resources.Tiledata", "PLAYER_IDLE_INDEX"),
            util::cfg_usize("Resources.Tiledata", "PLAYER_IDLE_STEPS")
        );
    }

    El PLAYER_MOVE_UP_RIGHT() {
        return El(
            util::cfg_usize("Resources.Tiledata", "PLAYER_MOVE_UP_RIGHT_TILESET"),
            util::cfg_usize("Resources.Tiledata", "PLAYER_MOVE_UP_RIGHT_INDEX"),
            util::cfg_usize("Resources.Tiledata", "PLAYER_MOVE_UP_RIGHT_STEPS")
        );
    }

    El PLAYER_MOVE_DOWN_LEFT() {
        return El(
            util::cfg_usize("Resources.Tiledata", "PLAYER_MOVE_DOWN_LEFT_TILESET"),
            util::cfg_usize("Resources.Tiledata", "PLAYER_MOVE_DOWN_LEFT_INDEX"),
            util::cfg_usize("Resources.Tiledata", "PLAYER_MOVE_DOWN_LEFT_STEPS")
        );
    }

    El PLAYER_MOVE_FAST() {
        return El(
            util::cfg_usize("Resources.Tiledata", "PLAYER_MOVE_FAST_TILESET"),
            util::cfg_usize("Resources.Tiledata", "PLAYER_MOVE_FAST_INDEX"),
            util::cfg_usize("Resources.Tiledata", "PLAYER_MOVE_FAST_STEPS")
        );
    }

public:
    ActionManager(WorldState& worldstate) 
        : STEPS(util::cfg_f32("Settings.Timing", "ACTION_STEPS")),
          INV_STEPS(1.0f / static_cast<f32>(STEPS)),
          worldstate(worldstate),
          pl_idle(PLAYER_IDLE()),
          pl_move_up_right(PLAYER_MOVE_UP_RIGHT()),
          pl_move_down_left(PLAYER_MOVE_DOWN_LEFT()),
          pl_move_fast(PLAYER_MOVE_FAST()),
          movable_crate(MOVABLE_CRATE()) {}

    void set(usize x, usize y) { pl_x = x; pl_y = y; worldstate.set(pl_x, pl_y, pl_idle); }
    usize get_x() const { return pl_x; }
    usize get_y() const { return pl_y; }

    void detect_player_action();
};

#endif