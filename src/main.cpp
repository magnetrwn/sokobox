#include <stdexcept>
#include <raylib.h>

#include "atlasmgr.hpp"

int main() {
    SetTargetFPS(60);
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(1024, 768, "Hello, Raylib!");

    {

    double event_time = 0.0f;
    double event_delay = 0.333f;
    usize sprite_id = 0;
    
    AtlasManager graphics("static/res/graphics.png", { 18.0f, 18.0f });

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Color{ 0x27, 0x28, 0x22, 0xff });

        if (GetTime() > event_time + event_delay) {
            event_time = GetTime();
            sprite_id = (sprite_id + 1) % 25;
        }

        graphics.draw(sprite_id, { 0, 0, 8.0f, 8.0f });

        EndDrawing();
    }

    }

    CloseWindow();
    return 0;
}