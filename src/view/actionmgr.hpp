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
    WorldElement pl_idle, pl_move, movable_crate;
    usize pl_x, pl_y;

public:
    using El = WorldElement::WorldElementInit;

    ActionManager(WorldState& worldstate) 
        : STEPS(util::cfg_f32("Settings.Timing", "ACTION_STEPS")),
          INV_STEPS(1.0f / static_cast<f32>(STEPS)),
          worldstate(worldstate),
          pl_idle(El::PLAYER_IDLE()),
          pl_move(El::PLAYER_MOVE()),
          movable_crate(El::MOVABLE_CRATE()) {}

    inline void set(usize x, usize y) { pl_x = x; pl_y = y; worldstate.set(pl_x, pl_y, pl_idle); }
    inline usize get_x() const { return pl_x; }
    inline usize get_y() const { return pl_y; }

    void detect_player_action();
};

#endif