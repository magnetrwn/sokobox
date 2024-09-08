#include <raylib.h>
#include "atlasmgr.hpp"
#include "isomgr.hpp"
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
    AtlasManager player("static/res/player.png", PLAYER_SZ);

    IsometricManager isometric;
    isometric.with(graphics, { WINDOW_W / 2, TILES_SCALE / 4 }, TILES_SCALE / GRAPHICS_SZ);
    isometric.with(a_items, { WINDOW_W / 2, TILES_SCALE / 4 }, TILES_SCALE / A_ITEMS_SZ);
    isometric.with(s_items, { WINDOW_W / 2, TILES_SCALE / 4 }, TILES_SCALE / S_ITEMS_SZ);
    isometric.with(player, { WINDOW_W / 2, TILES_SCALE / 4 }, TILES_SCALE / PLAYER_SZ);

    double event1_time = 0.0f;
    double event1_delay = 0.130f;
    usize a = 0;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Color{ 0x27, 0x28, 0x22, 0xff });

        if (GetTime() > event1_time + event1_delay) {
            event1_time = GetTime();
        }

        for (usize y = 0; y < 10; ++y)
            for (usize x = 0; x < 10; ++x)
                if (x == 0 || y == 0 || x == 9 || y == 9) isometric.draw(0, a % 25, { static_cast<f32>(x), static_cast<f32>(y) });
        // ((x + 1) ^ (y + a) ^ (y * x * (3 + a)))
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            ++a;

        //graphics.draw(sprite1_id, { 0, 0, 8.0f, 8.0f });
        /*a_items.draw(sprite2_id, { 144.0f, 0, 4.5f, 4.5f });
        s_items.draw(sprite3_id, { 0, 144.0f, 4.5f, 4.5f });
        player.draw(sprite4_id, { 144.0f, 144.0f, 4.5f, 4.5f });*/

        EndDrawing();
    }

    }

    CloseWindow();
    return 0;
}