#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include "raylib.h"
#include <StateManager.hpp>

/// @brief UI Management
class UIManager {
public:
    UIManager(StateManager* source);
    ~UIManager() = default;

    void drawShape(const ShapeState& shape);
    void drawText(const Text& text);
    void renderSnapshot();
private:
    StateManager* stateManager;
};

#endif