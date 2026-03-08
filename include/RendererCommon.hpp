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

    void setCenter(bool isCentered);
};

enum ShapeType { 
    CIRCLE, //BST
    LINE, // Graphs
    RECTANGLE, // Hash Tables
    TEXT, // Labeling
    ARROW // Edge direction
};

struct ShapeState {
    //Geometry
    Vector2 startPosition, endPosition;
    float size, outlineSize;

    //Recognition
    ShapeType sType;
    int layerID;

    //Style
    Color color, outlineColor;
    bool isHidden = false;
    
    //Content
    Text content;
    bool hasText = true;

    void setColor(Color color, Color outlineColor);
    void setText(Text content);
    void setTextVisibility(bool isTrue);
    void setTextCenter(bool isCentered);
};  

ShapeState createCircle(Vector2 startPosition, float radius, float outlineSize, int layerID);
ShapeState createRectangle(Vector2 startPosition, Vector2 endPosition, float outlineSize, int layerID);
ShapeState createLine(Vector2 startPosition, Vector2 endPosition, float size, float outlineSize, int layerID);

Text createText(std::string label, Font font, float fontSize, float spacing);
Text createText(std::string label, Font font, float fontSize, float spacing, Vector2 position);
Text createText(std::string label, Font font, float fontSize, float spacing, Vector2 position, Color color);

Rectangle createRaylibRectangle(Vector2 upperLeft, Vector2 lowerRight);
#endif