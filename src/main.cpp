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
    AtlasManager player(PLAYER.c_str(), PLAYER_SZ);

    IsometricManager isometric(f32_2{ WINDOW_W / 2, TILES_SCALE / 4 });
    isometric.with(graphics, TILES_SCALE / GRAPHICS_SZ);
    isometric.with(a_items, TILES_SCALE / A_ITEMS_SZ);
    isometric.with(s_items, TILES_SCALE / S_ITEMS_SZ);
    isometric.with(player, TILES_SCALE / PLAYER_SZ);

    double event1_time = 0.0f;
    double event1_delay = 0.125f;
    usize heartt = 0;
    usize goldkeyt = 0;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Color{ 0x27, 0x28, 0x22, 0xff });

        if (GetTime() > event1_time + event1_delay) {
            event1_time = GetTime();
            heartt = (heartt + 1) % 6;
            goldkeyt = (goldkeyt + 1) % 8;
        }

        for (f32 f = 0.0f; f < 9.01f; f += 1.0f) {
            isometric.draw(0, 0, { 0.0f, f });
            isometric.draw(0, 0, { f, 0.0f });
        }

        for (f32 f = 0.0f; f < 9.01f; f += 1.0f) {
            isometric.draw(0, 0, { 9.0f, f });
            isometric.draw(0, 0, { f, 9.0f });
        }

        isometric.draw(1, 112 + goldkeyt, { 3.0f, 5.0f });

        isometric.draw(0, 15, { 4.0f, 2.0f });
        isometric.draw(0, 15, { 4.0f, 3.0f });
        isometric.draw(0, 15, { 6.0f, 5.0f });
        isometric.draw(0, 15, { 5.0f, 6.0f });

        isometric.draw(0, 14, { 2.0f, 3.0f });
        isometric.draw(0, 19, { 4.0f, 5.0f });

        isometric.draw(0, 10, { 6.0f, 6.0f });

        isometric.draw(1, 8 + heartt, { 7.0f, 1.0f });

        // ((x + 1) ^ (y + a) ^ (y * x * (3 + a))) % 15

        //if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        //    ++a;

        EndDrawing();
    }

    }

    CloseWindow();
    return 0;
}