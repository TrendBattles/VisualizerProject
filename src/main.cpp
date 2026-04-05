#include <Core/AppLoop.hpp>

int main() {
    AppLoop* app = new AppLoop();
    app -> init();

    CoreFonts::load();

    while (!WindowShouldClose()) {

        // Prevent crashing incident
        if (IsWindowMinimized()) {
            WaitTime(1);
            continue;
        }

        app -> mainLoop();
    }

    CloseWindow();

    CoreFonts::unload();
    delete app;
    return 0;
}