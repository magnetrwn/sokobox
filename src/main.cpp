#include <raylib.h>

int main() {
    SetTargetFPS(60);
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(800, 450, "Hello, Raylib!");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello, Raylib!", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}