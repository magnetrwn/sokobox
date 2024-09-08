#include <raylib.h>
#include "atlasmgr.hpp"
#include "isomgr.hpp"

f32_2 isometric(f32 x, f32 y, f32 tile_sz) {
    f32_2 iso;
    iso.x = (x - y) * (tile_sz / 2.0f) + 512 - tile_sz / 2.0f;
    iso.y = (x + y) * (tile_sz / 4.0f);
    return iso;
}

int main() {
    SetTargetFPS(60);
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(1024, 768, "Hello, Raylib!");

    {

    double event1_time = 0.0f;
    double event1_delay = 0.130f;

    auto a = AtlasManager("static/res/graphics.png", { 18.0f, 18.0f });

    IsometricManager isometric;
    isometric.with(a, { 512, 0 }, 144.0f / 18.0f);
    //isometric.with(AtlasManager("static/res/a_items.png", { 32.0f, 32.0f }), { 512, 0 }, 144.0f / 32.0f);
    //isometric.with(AtlasManager("static/res/s_items.png", { 32.0f, 32.0f }), { 512, 0 }, 144.0f / 32.0f);
    //isometric.with(AtlasManager("static/res/player.png", { 32.0f, 32.0f }), { 512, 0 }, 144.0f / 32.0f);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Color{ 0x27, 0x28, 0x22, 0xff });

        if (GetTime() > event1_time + event1_delay) {
            event1_time = GetTime();
        }

        for (usize y = 0; y < 10; ++y)
            for (usize x = 0; x < 10; ++x)
                isometric.draw(0, 5, { static_cast<f32>(x), static_cast<f32>(y) });

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