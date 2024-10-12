#include "actionmgr.hpp"

constexpr static i64 DIR_OFFSETS[4][2] = {
    { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 }
};

constexpr static KeyboardKey DIR_KEYS[4] = {
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

void ActionManager::detect_player_action() {
    if (worldstate.is_player_moving())
        return;

    for (usize i = 0; i < 4; ++i) {
        const usize next_pl_x = pl_x + DIR_OFFSETS[i][0];
        const usize next_pl_y = pl_y + DIR_OFFSETS[i][1];

        if (IsKeyDown(DIR_KEYS[i]) and (worldstate.get(next_pl_x, next_pl_y).empty() or worldstate.get(next_pl_x, next_pl_y) == movable_crate)) {
            worldstate.iso.target_camera(f32_2{ static_cast<f32>(next_pl_x), static_cast<f32>(next_pl_y) });

            if (worldstate.get(next_pl_x, next_pl_y) == movable_crate) {
                if (worldstate.get(next_pl_x + DIR_OFFSETS[i][0], next_pl_y + DIR_OFFSETS[i][1]).empty()) {
                    worldstate.unset(next_pl_x, next_pl_y);
                    worldstate.move_tile(
                        WorldTransition(
                            f32_2{ static_cast<f32>(next_pl_x), static_cast<f32>(next_pl_y) },
                            pl_x + static_cast<f32>(DIR_OFFSETS[i][0]) * 2, 
                            pl_y + static_cast<f32>(DIR_OFFSETS[i][1]) * 2,
                            f32_2{ 
                                static_cast<f32>(DIR_OFFSETS[i][0]) * INV_STEPS, 
                                static_cast<f32>(DIR_OFFSETS[i][1]) * INV_STEPS 
                            }, 
                            movable_crate, movable_crate, STEPS
                        )
                    );
                }

                else
                    continue;
            }

            worldstate.unset(pl_x, pl_y);
            worldstate.move_player(
                WorldTransition(
                    f32_2{ static_cast<f32>(pl_x), static_cast<f32>(pl_y) }, 
                    next_pl_x, next_pl_y, 
                    f32_2{ 
                        static_cast<f32>(DIR_OFFSETS[i][0]) * INV_STEPS, 
                        static_cast<f32>(DIR_OFFSETS[i][1]) * INV_STEPS
                    }, 
                    pl_move, pl_idle, STEPS
                )
            );

            pl_x = next_pl_x;
            pl_y = next_pl_y;
            break;
        }
    }
}