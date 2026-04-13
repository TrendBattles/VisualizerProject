#include <Graphics/UIManager.hpp>
#include <Graphics/Helper.hpp>

/////////////////////////////////
///     UI Initialization     ///
/////////////////////////////////
UIManager::UIManager() = default;
UIManager::~UIManager() = default;

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
        case ShapeType::ROUNDED_RECTANGLE:
            DrawRectangleRoundedLinesEx(Helper::createRaylibRectangle(shape.startPosition, shape.endPosition), shape.roundness, 10, shape.outlineSize, shape.outlineColor);
            DrawRectangleRounded(Helper::createRaylibRectangle(shape.startPosition, shape.endPosition), shape.roundness, 10, shape.color);
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

/// @brief Rendering the shape list
void UIManager::renderSnapshot(const Snapshot& modifiedSnapshot) {
    for (const ShapeState& shape : modifiedSnapshot) {
        if (shape.isVisible) drawShape(shape);
    }
}