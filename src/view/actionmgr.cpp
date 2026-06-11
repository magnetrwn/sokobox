#include "actionmgr.hpp"
#include <raylib.h>

constexpr static i64 DIR_OFFSETS[4][2] = {
    { 0, -1 }, { 1, 0 }, { -1, 0 }, { 0, 1 }
};

constexpr static KeyboardKey DIR_KEYS[4] = {
    KEY_W, KEY_D, KEY_A, KEY_S
};

void ActionManager::step() {
    if (worldstate.is_player_moving())
        return;

    for (usize i = 0; i < 4; ++i) {
        const usize next_pl_x = pl_x + DIR_OFFSETS[i][0];
        const usize next_pl_y = pl_y + DIR_OFFSETS[i][1];
        const usize next2_pl_x = pl_x + DIR_OFFSETS[i][0] * 2;
        const usize next2_pl_y = pl_y + DIR_OFFSETS[i][1] * 2;
        WorldElement pl = worldstate.get(pl_x, pl_y);
        WorldElement next_pl = worldstate.get(next_pl_x, next_pl_y);
        WorldElement next2_pl;
        bool pushed_walkable_movable = false;
        WorldElement pushed_walkable_movable_el;

        if (IsKeyDown(DIR_KEYS[i]) and (next_pl.empty() or next_pl.is_movable() or next_pl.is_walkable() or next_pl.is_walkable_movable_stack())) {
            if (next_pl.is_movable() or next_pl.is_walkable_movable_stack()) {
                if (!worldstate.is_oob(next2_pl_x, next2_pl_y))
                        next2_pl = worldstate.get(next2_pl_x, next2_pl_y);
                else
                    continue;

                if (next_pl.is_walkable_movable_stack() and next2_pl.empty()) {
                    pushed_walkable_movable = true;
                    WorldElement next2_pl_update = next2_pl;
                    pushed_walkable_movable_el = next_pl;
                    next_pl.pop_front();
                    next2_pl_update.push_end(next_pl);

                    worldstate.unset(next_pl_x, next_pl_y);
                    worldstate.move_tile(
                        WorldTransition(
                            f32_2{ static_cast<f32>(next_pl_x), static_cast<f32>(next_pl_y) }, 
                            static_cast<f32>(next2_pl_x), 
                            static_cast<f32>(next2_pl_y),
                            f32_2{ 
                                static_cast<f32>(DIR_OFFSETS[i][0]) * INV_STEPS, 
                                static_cast<f32>(DIR_OFFSETS[i][1]) * INV_STEPS 
                            },
                            next_pl,
                            next2_pl_update, 
                            STEPS
                        )
                    );
                }

                else if (next2_pl.empty()) {
                    worldstate.unset(next_pl_x, next_pl_y);
                    worldstate.move_tile(
                        WorldTransition(
                            f32_2{ static_cast<f32>(next_pl_x), static_cast<f32>(next_pl_y) }, 
                            static_cast<f32>(next2_pl_x), 
                            static_cast<f32>(next2_pl_y),
                            f32_2{ 
                                static_cast<f32>(DIR_OFFSETS[i][0]) * INV_STEPS, 
                                static_cast<f32>(DIR_OFFSETS[i][1]) * INV_STEPS 
                            },
                            next_pl, next_pl, STEPS
                        )
                    );
                }

                else if (next2_pl.is_walkable()) {
                    WorldElement next2_pl_update;
                    next2_pl_update = next2_pl;
                    next2_pl_update.push_end(next_pl);

                    worldstate.unset(next_pl_x, next_pl_y);
                    worldstate.move_tile(
                        WorldTransition(
                            f32_2{ static_cast<f32>(next_pl_x), static_cast<f32>(next_pl_y) }, 
                            static_cast<f32>(next2_pl_x), 
                            static_cast<f32>(next2_pl_y),
                            f32_2{ 
                                static_cast<f32>(DIR_OFFSETS[i][0]) * INV_STEPS, 
                                static_cast<f32>(DIR_OFFSETS[i][1]) * INV_STEPS 
                            },
                            next_pl,
                            next2_pl_update, 
                            STEPS
                        )
                    );
                }

                else
                    continue;
            }

            WorldElement next_pl_update;
            if (next_pl.is_walkable()) {
                next_pl_update = next_pl;
                next_pl_update.push_end(El::PLAYER_IDLE());
            }

            else if (pushed_walkable_movable) {
                next_pl_update = pushed_walkable_movable_el;
                next_pl_update.pop_end();
                next_pl_update.push_end(El::PLAYER_IDLE());
            }

            pl.pop_end();
            worldstate.set(pl_x, pl_y, pl);

            worldstate.move_player(
                WorldTransition(
                    f32_2{ static_cast<f32>(pl_x), static_cast<f32>(pl_y) }, 
                    next_pl_x, next_pl_y, 
                    f32_2{ 
                        static_cast<f32>(DIR_OFFSETS[i][0]) * INV_STEPS, 
                        static_cast<f32>(DIR_OFFSETS[i][1]) * INV_STEPS
                    }, 
                    (IsKeyDown(KEY_LEFT_SHIFT)) 
                        ? El::PLAYER_MOVE_FAST() 
                        : ((i < 2) 
                            ? El::PLAYER_MOVE_UP_RIGHT() 
                            : El::PLAYER_MOVE_DOWN_LEFT()), 
                    (next_pl.is_walkable() or pushed_walkable_movable)
                      ? next_pl_update
                      : El::PLAYER_IDLE(), 
                    STEPS
                )
            );

            pl_x = next_pl_x;
            pl_y = next_pl_y;
            break;
        }
    }
}