#include <Core/AppLoop.hpp>

int main() {
    AppLoop* app = new AppLoop();
    app -> init();

    while (!WindowShouldClose()) {

        // Prevent crashing incident
        if (IsWindowMinimized()) {
            WaitTime(1);
            continue;
        }

        app -> mainLoop();
    }

    delete app;

    CloseWindow();
    return 0;
}