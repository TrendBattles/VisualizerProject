#include <Graphics/UIManager.hpp>
#include <Graphics/Helper.hpp>
#include "raymath.h"
#include <iostream>

/////////////////////////////////
///     UI Initialization     ///
/////////////////////////////////
UIManager::UIManager() {
    options = {"AVL Tree", "Trie", "Linked List", "Hash Table", "Graph"};
    appSignal = "AVL Tree";
}

void UIManager::setStateManger(StateManager* source) {
    stateManager = source;
}
const std::vector <std::string>& UIManager::getDSOptions() const {
    return options;
}

//////////////////////////
///     UI Renders     ///
//////////////////////////

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
            DrawRectangleLinesEx(Helper::createRaylibRectangle(shape.startPosition, shape.endPosition), shape.outlineSize, shape.outlineColor);
            break;
        case ShapeType::ARROW: {
            float headSize = std::min(shape.size * 3.0f, Vector2Distance(shape.startPosition, shape.endPosition) * 0.5f);
            Vector2 normVec = Vector2Normalize(shape.endPosition - shape.startPosition);

            Vector2 headCenter = shape.endPosition - normVec * headSize;

            Vector2 lineEndPosition = headCenter - normVec * (headSize * cos(PI / 3.0));
            DrawLineEx(shape.startPosition, lineEndPosition, shape.size + shape.outlineSize, shape.outlineColor);
            DrawLineEx(shape.startPosition, lineEndPosition, shape.size, shape.color);

            Vector2 v1 = headCenter + Vector2Rotate(normVec * (headSize + shape.outlineSize), -PI * 2 / 3.0);
            Vector2 v2 = headCenter + normVec * (headSize + shape.outlineSize);
            Vector2 v3 = headCenter + Vector2Rotate(normVec * (headSize + shape.outlineSize), +PI * 2 / 3.0);
            DrawTriangle(v1, v3, v2, shape.outlineColor);
            

            v1 = headCenter + Vector2Rotate(normVec * headSize, -PI * 2 / 3.0);
            v2 = headCenter + normVec * headSize;
            v3 = headCenter + Vector2Rotate(normVec * headSize, +PI * 2 / 3.0);
            DrawTriangle(v1, v3, v2, shape.color);
            
            break;
        }
        case ShapeType::TEXT:
            break;
    }

    if (shape.hasText) drawText(shape.content);
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

    const Snapshot& target = stateManager -> getCurrentSnapShot(appSignal);

    for (const ShapeState& shape : target) {
        drawShape(shape);
    }
}

/*
Animation ?
Line: Time limit, time elapsed, start Vector, end Vector -> Path
Circle Path: Time limit, time elapsed, center, radius -> Path
-> Animate anything on the screen as long as we implement functions and store a list of animations
in another class. -> Approach solved

Where should we put drawing functions ? UIManager
UIManager: 
    + Generalization
    + Adding duplicate functions
    -> Time elapsed, time limit -> Treat as a new ShapeState
*/