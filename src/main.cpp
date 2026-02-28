#include <AppLoop.hpp>

int main() {
    AppLoop* app = new AppLoop();
    app -> init();

    while (!WindowShouldClose()) {
        app -> mainLoop();
    }

    CloseWindow();

    delete app;
    return 0;
}