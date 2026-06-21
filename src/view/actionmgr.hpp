#ifndef INPUTMGR_HPP_
#define INPUTMGR_HPP_

#include <raylib.h>
#include "typedef.hpp"
#include "world.hpp"

class ActionManager {
private:
    const usize STEPS;
    const f32 INV_STEPS;

    World& world;
    usize pl_x, pl_y;

public:
    ActionManager(World& world) 
        : STEPS(util::cfg_f32("Settings.Timing", "ACTION_STEPS")),
          INV_STEPS(1.0f / static_cast<f32>(STEPS)),
          world(world) {}

    void set(usize x, usize y) { pl_x = x; pl_y = y; world.set(pl_x, pl_y, El::PLAYER_IDLE()); }
    usize get_x() const { return pl_x; }
    usize get_y() const { return pl_y; }

    void step();
};

#endif