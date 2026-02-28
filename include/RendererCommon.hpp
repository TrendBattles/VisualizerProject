#ifndef RENDERERCOMMON_HPP
#define RENDERERCOMMON_HPP

#include "raylib.h"
#include <string>

/// @brief Builtin features for advanced settings
struct Text {
    std::string label = "";
    Font font = GetFontDefault();
    float fontSize = 10.0f, spacing = 5.0f;
    Color textColor = BLACK;
    Vector2 position = Vector2{0.0f, 0.0f};
    int centerToggle = false;
};

enum ShapeType { 
    CIRCLE, //BST
    LINE, // Graphs
    RECTANGLE, // Hash Tables
    TEXT, // Labeling
    ARROW // Edge direction
};

struct ShapeState {
    Vector2 startPosition, endPosition;
    ShapeType sType;
    float size, outlineSize;
    Color color, outlineColor;
    int layerID;
    Text content;
};  

Rectangle createRaylibRectangle(Vector2 upperLeft, Vector2 lowerRight);
#endif