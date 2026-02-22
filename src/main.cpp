#include "raylib.h"

int main() {
    // 1. Initialization
    const int screenWidth = 1200;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "C++ Data Structure Visualizer");

    // 2. Setup a Camera (so you can zoom in/out of big trees)
    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0, 0 };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);

    // 3. Main Loop
    while (!WindowShouldClose()) {
        // --- Update Logic ---
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 delta = GetMouseDelta();
            camera.target.x -= delta.x / camera.zoom;
            camera.target.y -= delta.y / camera.zoom;
        }
        camera.zoom += ((float)GetMouseWheelMove() * 0.05f);

        // --- Drawing Logic ---
        BeginDrawing();
            ClearBackground(GetColor(0x181818FF)); // Modern Dark Theme

            BeginMode2D(camera);
                // Draw your data structures here
                DrawCircle(0, 0, 40, BLUE); 
                DrawText("Root Node", -30, -10, 15, WHITE);
            EndMode2D();

            // Draw UI (which doesn't move with the camera)
            DrawFPS(10, 10);
            DrawText("Right-click to pan | Scroll to zoom", 10, 40, 20, GRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}