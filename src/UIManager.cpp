#include <UIManager.hpp>
#include "raymath.h"
#include <iostream>

UIManager::UIManager(StateManager* source) {
    stateManager = source;   
}

/// @brief Shape Drawing
/// @param shape 
void UIManager::drawShape(const ShapeState& shape) {
    switch(shape.sType) {
        case ShapeType::CIRCLE:
            DrawCircleV(shape.startPosition, shape.size + shape.outlineSize, shape.outlineColor);
            DrawCircleV(shape.startPosition, shape.size, shape.color);
            break;
        case ShapeType::LINE:
            DrawLineEx(shape.startPosition, shape.endPosition, shape.size + shape.outlineSize, shape.outlineColor);
            DrawLineEx(shape.startPosition, shape.endPosition, shape.size, shape.color);
            break;
        case ShapeType::RECTANGLE:
            DrawRectangleV(shape.startPosition, shape.endPosition - shape.startPosition, shape.color);
            DrawRectangleLinesEx(Rectangle {shape.startPosition.x, shape.startPosition.y, shape.endPosition.x - shape.startPosition.x, shape.endPosition.y - shape.startPosition.y}, shape.outlineSize, shape.outlineColor);
            break;
        case ShapeType::TEXT:
            break;
    }

    drawText(shape.content);
}

/// @brief Text Drawing
/// @param text 
void UIManager::drawText(const Text& text) {
    if (text.label.empty()) return;

    Vector2 finalPosition = text.position;
    if (text.centerToggle) {
        Vector2 labelSize = MeasureTextEx(text.font, text.label.c_str(), text.fontSize, text.spacing);
        finalPosition -= labelSize / 2;
    }

    DrawTextEx(text.font, text.label.c_str(), finalPosition, text.fontSize, text.spacing, text.textColor);
}

/// @brief Rendering the data structure
void UIManager::renderSnapshot() {
    if (stateManager == nullptr) return;

    Snapshot target = stateManager -> getCurrentSnapShot();

    for (ShapeState& shape : target) {
        drawShape(shape);
    }
}