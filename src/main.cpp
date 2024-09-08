#include <raylib.h>
#include "atlasmgr.hpp"

int main() {
    SetTargetFPS(60);
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(1024, 768, "Hello, Raylib!");

    {

    double event1_time = 0.0f;
    double event1_delay = 0.065f;
    usize sprite1_id = 0;
    usize sprite2_id = 0;
    usize sprite3_id = 0;
    usize sprite4_id = 0;
    
    AtlasManager graphics("static/res/graphics.png", { 18.0f, 18.0f });
    AtlasManager a_items("static/res/a_items.png", { 32.0f, 32.0f });
    AtlasManager s_items("static/res/s_items.png", { 32.0f, 32.0f });
    AtlasManager player("static/res/player.png", { 32.0f, 32.0f });

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Color{ 0x27, 0x28, 0x22, 0xff });

        if (GetTime() > event1_time + event1_delay) {
            event1_time = GetTime();
            sprite1_id = (sprite1_id + 1) % graphics.count();
            sprite2_id = (sprite2_id + 1) % a_items.count();
            sprite3_id = (sprite3_id + 1) % s_items.count();
            sprite4_id = (sprite4_id + 1) % player.count();
        }

        graphics.draw(sprite1_id, { 0, 0, 8.0f, 8.0f });
        a_items.draw(sprite2_id, { 144.0f, 0, 4.5f, 4.5f });
        s_items.draw(sprite3_id, { 0, 144.0f, 4.5f, 4.5f });
        player.draw(sprite4_id, { 144.0f, 144.0f, 4.5f, 4.5f });

        EndDrawing();
    }

    }

    CloseWindow();
    return 0;
}