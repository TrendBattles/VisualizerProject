#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include <Core/StateManager.hpp>
#include <Core/AnimationManager.hpp>

/// @brief UI Management
class UIManager {
public:
    UIManager();
    ~UIManager();

    void drawShape(const ShapeState& shape);
    void drawText(const Text& text);

    void renderSnapshot(const Snapshot& modifiedSnapshot);
private:

};

#endif