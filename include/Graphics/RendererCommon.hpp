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
    CIRCLE,
    LINE,
    RECTANGLE,
    TEXT,
    ARROW
};

struct ShapeState {
    //Identity
    std::string shapeID = "";

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

    void positionTransitAllBy(Vector2 delta);
};  
#endif