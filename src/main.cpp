#include <raylib.h>
#include "atlasmgr.hpp"
#include "isomgr.hpp"
#include "worldview.hpp"
#include "util.hpp"

int main() {
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

    SetTargetFPS(WINDOW_FPS);
    if (WINDOW_VSYNC)
        SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(WINDOW_W, WINDOW_H, "~Sokobox~");

    {

    AtlasManager graphics(GRAPHICS.c_str(), GRAPHICS_SZ);
    AtlasManager a_items(A_ITEMS.c_str(), A_ITEMS_SZ);
    AtlasManager s_items(S_ITEMS.c_str(), S_ITEMS_SZ);
    AtlasManager player(PLAYER.c_str(), PLAYER_SZ);

    IsometricManager isometric(f32_2{ WINDOW_W / 2, TILES_SCALE / 4 });
    isometric.with(graphics, TILES_SCALE / GRAPHICS_SZ);
    isometric.with(a_items, TILES_SCALE / A_ITEMS_SZ);
    isometric.with(s_items, TILES_SCALE / S_ITEMS_SZ);
    isometric.with(player, TILES_SCALE / PLAYER_SZ);

    WorldView worldview(10, 10, isometric);

    // Edge boxes
    for (usize i = 0; i < 10; ++i) {
        worldview.set(0, i, WorldElement(0, 0));
        worldview.set(i, 0, WorldElement(0, 0));
        worldview.set(9, i, WorldElement(0, 0));
        worldview.set(i, 9, WorldElement(0, 0));
    }
    
    // Crates
    worldview.set(4, 2, WorldElement(0, 15));
    worldview.set(4, 3, WorldElement(0, 15));
    worldview.set(6, 5, WorldElement(0, 15));

    // Tall box on top of crate
    worldview.set(5, 6, WorldElement(0, { 15, 19, WorldElement::NOT_SET, 14 }));

    // Stack of boxes
    worldview.set(1, 7, WorldElement(0, { 9, 19, 9, 7 }));

    // Angled boxes
    worldview.set(6, 6, WorldElement(0, 10));
    worldview.set(1, 4, WorldElement(0, 11));

    // Opened boxes
    worldview.set(1, 1, WorldElement(0, 24));
    worldview.set(1, 3, WorldElement(0, 24));
    worldview.set(8, 2, WorldElement(0, 24));

    // Animated objects
    worldview.set(7, 1, WorldElement(1, 8, 6));
    worldview.set(3, 5, WorldElement(1, 112, 8));
    worldview.set(7, 7, WorldElement(1, 72, 8));

    // Player
    usize pl_x = 5;
    usize pl_y = 7;
    worldview.set(pl_x, pl_y, WorldElement(3, 0, 4));

    double event1_time = 0.0f;
    double event1_delay = 0.125f;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Color{ 0x27, 0x28, 0x22, 0xff });

        if (IsKeyPressed(KEY_UP)) {
            if (pl_y == 0) {
                
            } else {
                worldview.unset(pl_x, pl_y);
                worldview.set(pl_x, --pl_y, WorldElement(3, 0, 4));
            }
        }

        if (IsKeyPressed(KEY_DOWN)) {
            worldview.unset(pl_x, pl_y);
            worldview.set(pl_x, ++pl_y, WorldElement(3, 0, 4));
        }

        if (IsKeyPressed(KEY_LEFT)) {
            worldview.unset(pl_x, pl_y);
            worldview.set(--pl_x, pl_y, WorldElement(3, 0, 4));
        }

        if (IsKeyPressed(KEY_RIGHT)) {
            worldview.unset(pl_x, pl_y);
            worldview.set(++pl_x, pl_y, WorldElement(3, 0, 4));
        }

        worldview.draw();

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            isometric.update_pos(GetMouseDelta());

        isometric.update_scale(1.0f + GetMouseWheelMove() * 0.1f, { WINDOW_W / 2, WINDOW_H / 2 });

        if (GetTime() > event1_time + event1_delay) {
            event1_time = GetTime();
            worldview.step();
        }

        EndDrawing();
    }

    }

    CloseWindow();
    return 0;
}

        // ((x + 1) ^ (y + a) ^ (y * x * (3 + a))) % 15
