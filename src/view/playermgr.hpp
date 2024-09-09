#ifndef INPUTMGR_HPP_
#define INPUTMGR_HPP_

#include <raylib.h>
#include "typedef.hpp"
#include "worldview.hpp"

class PlayerManager {
private:
    WorldView& worldview;
    WorldElement pl_idle, pl_move, movable_crate;
    usize pl_x, pl_y;

public:
    PlayerManager(WorldView& worldview) 
        : worldview(worldview),
          pl_idle(3, 0, 4),
          pl_move(3, 4, 4),
          movable_crate(0, 15) {}

    inline void set(usize x, usize y) { pl_x = x; pl_y = y; worldview.set(pl_x, pl_y, pl_idle); }
    inline usize get_x() const { return pl_x; }
    inline usize get_y() const { return pl_y; }

    void detect_player_action();
};

#endif