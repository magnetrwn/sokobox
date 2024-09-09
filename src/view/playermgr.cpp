#include "playermgr.hpp"

constexpr i64 DIR_OFFSETS[4][2] = {
    { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 }
};

constexpr KeyboardKey DIR_KEYS[4] = {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

void PlayerManager::detect_player_action() {
    if (worldview.is_transition())
        return;

    for (usize i = 0; i < 4; ++i) {
        const usize next_pl_x = pl_x + DIR_OFFSETS[i][0];
        const usize next_pl_y = pl_y + DIR_OFFSETS[i][1];

        if (IsKeyDown(DIR_KEYS[i]) and worldview.get(next_pl_x, next_pl_y).empty()) {
            worldview.unset(pl_x, pl_y);
            worldview.transition(
                WorldTransition(
                    f32_2{ static_cast<f32>(pl_x), static_cast<f32>(pl_y) }, 
                    next_pl_x, next_pl_y, 
                    f32_2{ 
                        static_cast<f32>(DIR_OFFSETS[i][0]) / 4.0f, 
                        static_cast<f32>(DIR_OFFSETS[i][1]) / 4.0f 
                    }, 
                    pl_move, pl_idle, 4
                )
            );

            if (worldview.get(next_pl_x, next_pl_y) == movable_crate) {
                TraceLog(LOG_INFO, "Moving crate");
                worldview.unset(next_pl_x, next_pl_y);
                worldview.transition(
                    WorldTransition(
                        f32_2{ static_cast<f32>(next_pl_x), static_cast<f32>(next_pl_y) }, pl_x, pl_y,
                        f32_2{ 
                            static_cast<f32>(DIR_OFFSETS[i][0]) / 4.0f, 
                            static_cast<f32>(DIR_OFFSETS[i][1]) / 4.0f 
                        }, 
                        movable_crate, movable_crate, 4
                    )
                );
            }

            pl_x = next_pl_x;
            pl_y = next_pl_y;
            break;
        }
    }
}