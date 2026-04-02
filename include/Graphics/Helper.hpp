#ifndef HELPER_HPP
#define HELPER_HPP

#include <Graphics/RendererCommon.hpp>
#include <Graphics/Panels/Button.hpp>

namespace Helper {
    // Shape Creation
    ShapeState createCircle(std::string tempoID, Vector2 startPosition, float radius, float outlineSize, int layerID);
    ShapeState createRectangle(std::string tempoID, Vector2 startPosition, Vector2 endPosition, float outlineSize, int layerID);
    ShapeState createRectangle(std::string tempoID, Vector2 startPosition, Vector2 endPosition, float outlineSize, Color color, Color outlineColor, int layerID);
    ShapeState createLine(std::string tempoID, Vector2 startPosition, Vector2 endPosition, float size, float outlineSize, int layerID);
    ShapeState createLine(std::string tempoID, Vector2 startPosition, Vector2 endPosition, float size, float outlineSize, Color color, Color outlineColor, int layerID);
    ShapeState createArrow(std::string tempoID, Vector2 startPosition, Vector2 endPosition, float size, float outlineSize, int layerID);
    ShapeState createArrow(std::string tempoID, Vector2 startPosition, Vector2 endPosition, float size, float outlineSize, Color color, Color outlineColor, int layerID);

    //Text creation
    Text createText(std::string label, Font font, float fontSize, float spacing);
    Text createText(std::string label, Font font, float fontSize, float spacing, Vector2 position);
    Text createText(std::string label, Font font, float fontSize, float spacing, Vector2 position, Color color);
    
    //Animation Lerps
    ShapeState shapeTimeLerp(const ShapeState& shapeA, const ShapeState& shapeB, float rate);
    Color colorTimeLerp(Color colorA, Color colorB, float rate);
    
    //Intersection, Collision
    bool IsPointCollidedWithShape(Vector2 position, ShapeState shape);

    //Creation of more complex designs
    Button createButton(ShapeState background, Text label);
    Textbox createTextbox(ShapeState background, Text label);

    //Additionals
    std::string edgeStringBuffer(std::string objectA, std::string objectB);
    std::string edgePointStringBuffer(std::string object);
    std::string arrowStringBuffer(std::string objectA, std::string objectB);
    std::string arrowPointStringBuffer(std::string object);
    std::string nodeStringBuffer(int value);
    std::string nodeStringBuffer(std::string value);
    std::string rectangleStringBuffer(std::string shapeID);

    Rectangle createRaylibRectangle(Vector2 upperLeft, Vector2 lowerRight);

    std::string upperString(std::string str);
}

#endif