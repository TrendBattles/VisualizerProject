#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include "raylib.h"
#include <StateManager.hpp>
#include <vector>

/// @brief UI Management
class UIManager {
public:
    UIManager(StateManager* source);
    ~UIManager() = default;

    void drawShape(const ShapeState& shape);
    void drawText(const Text& text);
    void renderSnapshot();
private:
    // To save the snapshots of the data structure
    StateManager* stateManager;

    //Menu, Settings, DS options, ....
    std::vector <std::string> options;
    std::string appSignal;
};

#endif