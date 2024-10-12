#include <raylib.h>
#include "atlas.hpp"
#include "isoview.hpp"
#include "worldstate.hpp"
#include "util.hpp"
#include "actionmgr.hpp"

int main() {
    using El = WorldElement::WorldElementInit;
    constexpr static auto END = WorldElement::END;
    constexpr static auto SKIP = WorldElement::SKIP;
    
    // [Settings.Window]
    const f32 WINDOW_W = util::cfg_f32("Settings.Window", "WINDOW_W");
    const f32 WINDOW_H = util::cfg_f32("Settings.Window", "WINDOW_H");
    const usize WINDOW_FPS = util::cfg_usize("Settings.Window", "WINDOW_FPS");
    const bool WINDOW_VSYNC = util::cfg_bool("Settings.Window", "WINDOW_VSYNC");

    // [Resources.Atlases]
    const std::string GRAPHICS = (util::cfg_string("Resources.Atlases", "GRAPHICS"));
    const std::string A_ITEMS = (util::cfg_string("Resources.Atlases", "A_ITEMS"));
    const std::string S_ITEMS = (util::cfg_string("Resources.Atlases", "S_ITEMS"));
    const std::string PLAYER = (util::cfg_string("Resources.Atlases", "PLAYER"));
    const f32 GRAPHICS_SZ = util::cfg_f32("Resources.Atlases", "GRAPHICS_SZ");
    const f32 A_ITEMS_SZ = util::cfg_f32("Resources.Atlases", "A_ITEMS_SZ");
    const f32 S_ITEMS_SZ = util::cfg_f32("Resources.Atlases", "S_ITEMS_SZ");
    const f32 PLAYER_SZ = util::cfg_f32("Resources.Atlases", "PLAYER_SZ");

    // [Settings.Isometric]
    const f32 TILES_SCALE = util::cfg_f32("Settings.Isometric", "TILES_SCALE");

    // [Settings.Timing]
    const f32 LOOP_ANIMATION_STEP_TRIG = util::cfg_f32("Settings.Timing", "LOOP_ANIMATION_STEP_TRIG");
    const f32 LOOP_TRANSITION_STEP_TRIG = util::cfg_f32("Settings.Timing", "LOOP_TRANSITION_STEP_TRIG");    

    const usize LEVEL_W = 250;
    const usize LEVEL_H = 250;

    SetTargetFPS(WINDOW_FPS);
    if (WINDOW_VSYNC)
        SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(WINDOW_W, WINDOW_H, "~Sokobox~");

    {

    Atlas atlas_graphics(GRAPHICS.c_str(), GRAPHICS_SZ);
    Atlas atlas_animated(A_ITEMS.c_str(), A_ITEMS_SZ);
    Atlas atlas_static(S_ITEMS.c_str(), S_ITEMS_SZ);
    Atlas atlas_player(PLAYER.c_str(), PLAYER_SZ);

    IsometricView isometric(f32_2{ WINDOW_W, WINDOW_H }, f32_2{ WINDOW_W / 2, TILES_SCALE / 4 });
    isometric.with(atlas_graphics, TILES_SCALE / GRAPHICS_SZ);
    isometric.with(atlas_animated, TILES_SCALE / A_ITEMS_SZ);
    isometric.with(atlas_static, TILES_SCALE / S_ITEMS_SZ);
    isometric.with(atlas_player, TILES_SCALE / PLAYER_SZ);

    WorldState worldstate(LEVEL_W, LEVEL_H, isometric);

    ActionManager player_in(worldstate);

    // Edge boxes
    for (usize x = 0; x < LEVEL_W; ++x)
        for (usize y = 0; y < LEVEL_H; ++y)
            if (x == 0 or x == LEVEL_W - 1 or y == 0 or y == LEVEL_H - 1)
                worldstate.set(x, y, El(0, util::randi(0, 11)));
    
    // Crates
    /*worldstate.set(4, 2, El(0, 15));
    worldstate.set(4, 3, El(0, 15));
    worldstate.set(6, 5, El(0, 15));

    // Tall box on top of crate
    worldstate.set(5, 6, El(0, { 15, 19, SKIP, 14, END }));

    // Stack of boxes
    worldstate.set(1, 7, El(0, { 9, 19, 9, 7, END }));

    // Angled boxes
    worldstate.set(6, 6, El(0, 10));
    worldstate.set(1, 4, El(0, 11));

    // Opened boxes
    worldstate.set(1, 1, El(0, 24));
    worldstate.set(1, 3, El(0, 24));
    worldstate.set(8, 2, El(0, 24));

    // Animated objects
    worldstate.set(7, 1, El(1, 8, 6));
    worldstate.set(3, 5, El(1, 112, 8));
    worldstate.set(7, 7, El(1, 72, 8));*/

    for (usize i = LEVEL_W + 1; i < LEVEL_W * LEVEL_H - LEVEL_W - 1; ++i) {
        i32 rnd = util::randi(0, 5);
        if (rnd != 0 or i % LEVEL_W == 0 or i % LEVEL_W == LEVEL_W - 1)
            continue;

        if (i % 6 == 0) {
            worldstate.set(i % LEVEL_W, i / LEVEL_H, El(0, 8));
            continue;
        }
        
        rnd = util::randi(-1, 24);
        if (rnd != -1 and rnd != 19 and rnd != 14) {
            worldstate.set(i % LEVEL_W, i / LEVEL_H, El(0, rnd));
            continue;
        }

        if (rnd == 14 or rnd == 19) {
            worldstate.set(i % LEVEL_W, i / LEVEL_H, El(0, { 19, SKIP, 14, END }));
            continue;
        }

        rnd = util::randi(-1, 5);
        switch(rnd) {
            case 0: worldstate.set(i % LEVEL_W, i / LEVEL_H, El(1, 8, 6)); break;
            case 1: worldstate.set(i % LEVEL_W, i / LEVEL_H, El(1, 112, 8)); break;
            case 2: worldstate.set(i % LEVEL_W, i / LEVEL_H, El(1, 72, 8)); break;
            case 3: worldstate.set(i % LEVEL_W, i / LEVEL_H, El(0, { 15, 19, SKIP, 14, END })); break;
            case 4: worldstate.set(i % LEVEL_W, i / LEVEL_H, El(0, { 9, 19, 9, 7, END })); break;
        }
    }

    player_in.set(5, 7);
    isometric.set_camera({ 5, 7 });

    double anim_step_time = 0.0f;
    double anim_step_delay = LOOP_ANIMATION_STEP_TRIG;

    double tran_step_time = 0.0f;
    double tran_step_delay = LOOP_TRANSITION_STEP_TRIG;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Color{ 0x27, 0x28, 0x22, 0xff });

        player_in.detect_player_action();
        worldstate.draw();

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            isometric.update_pos(GetMouseDelta());
        else
            isometric.step_camera(GetFrameTime() / 0.016f, 3);

        isometric.update_scale(1.0f + GetMouseWheelMove() * 0.1f, GetMousePosition());

        if (GetTime() > anim_step_time + anim_step_delay * (IsKeyDown(KEY_RIGHT_SHIFT) ? 0.5f : 1.0f)) {
            anim_step_time = GetTime();
            worldstate.step_animations();
        }

        if (GetTime() > tran_step_time + tran_step_delay * (IsKeyDown(KEY_RIGHT_SHIFT) ? 0.5f : 1.0f)) {
            tran_step_time = GetTime();
            worldstate.step_transitions();
        }

        DrawText(std::to_string(GetFPS()).c_str(), 10, 10, 72, RAYWHITE);
        DrawText("Move with arrow keys, use right shift to speed up,", WINDOW_W - 540, 18, 20, RAYWHITE);
        DrawText("use the mouse to move and scroll to zoom.", WINDOW_W - 540, 38, 20, RAYWHITE);
        DrawText("You can push crates, but not cardboard boxes!", WINDOW_W - 540, 58 + 6, 20, RAYWHITE);
        
        EndDrawing();
    }

    }

    CloseWindow();
    return 0;
}

        // ((x + 1) ^ (y + a) ^ (y * x * (3 + a))) % 15
