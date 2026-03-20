#include <Graphics/Helper.hpp>

///////////////////////////////////
///     BUILTIN CONSTRUCTION    ///
///////////////////////////////////

///
/// Shape Creation
///

ShapeState Helper::createCircle(std::string tempoID, Vector2 startPosition, float radius, float outlineSize, int layerID) {
    ShapeState tempShape;
    tempShape.shapeID = tempoID;
    tempShape.startPosition = startPosition;
    tempShape.size = radius;

    tempShape.sType = ShapeType::CIRCLE;

    tempShape.outlineSize = outlineSize;
    tempShape.layerID = layerID;

    return tempShape;
}

ShapeState Helper::createRectangle(std::string tempoID, Vector2 startPosition, Vector2 endPosition, float outlineSize, int layerID) {
    ShapeState tempShape;
    tempShape.shapeID = tempoID;
    tempShape.startPosition = startPosition;
    tempShape.endPosition = endPosition;

    tempShape.sType = ShapeType::RECTANGLE;

    tempShape.outlineSize = outlineSize;
    tempShape.layerID = layerID;

    return tempShape;
}

ShapeState Helper::createLine(std::string tempoID, Vector2 startPosition, Vector2 endPosition, float size, float outlineSize, int layerID) {
    ShapeState tempShape;
    tempShape.shapeID = tempoID;
    tempShape.startPosition = startPosition;
    tempShape.endPosition = endPosition;

    tempShape.sType = ShapeType::LINE;

    tempShape.size = size;
    tempShape.outlineSize = outlineSize;
    tempShape.layerID = layerID;

    return tempShape;
}

ShapeState Helper::createArrow(std::string tempoID, Vector2 startPosition, Vector2 endPosition, float size, float outlineSize, int layerID) {
    ShapeState tempShape;
    tempShape.shapeID = tempoID;
    tempShape.startPosition = startPosition;
    tempShape.endPosition = endPosition;

    tempShape.sType = ShapeType::ARROW;

    tempShape.size = size;
    tempShape.outlineSize = outlineSize;
    tempShape.layerID = layerID;

    return tempShape;
}

///
/// Text creation
///
Text Helper::createText(std::string label, Font font, float fontSize, float spacing) {
    Text tempText;
    tempText.label = label;
    tempText.font = font;
    tempText.fontSize = fontSize;
    tempText.spacing = spacing;

    return tempText;
}

Text Helper::createText(std::string label, Font font, float fontSize, float spacing, Vector2 position) {
    Text tempText;
    tempText.label = label;
    tempText.font = font;
    tempText.fontSize = fontSize;
    tempText.spacing = spacing;
    tempText.position = position;

    return tempText;
}

Text Helper::createText(std::string label, Font font, float fontSize, float spacing, Vector2 position, Color color) {
    Text tempText;
    tempText.label = label;
    tempText.font = font;
    tempText.fontSize = fontSize;
    tempText.spacing = spacing;
    tempText.position = position;
    tempText.textColor = color;

    return tempText;
}

///
/// Additionals
///
std::string Helper::edgeStringBuffer(std::string objectA, std::string objectB) {
    return "edge_" + objectA + "_" + objectB;
}
std::string Helper::arrowStringBuffer(std::string objectA, std::string objectB) {
    return "arrow_" + objectA + "_" + objectB;
}
std::string Helper::nodeStringBuffer(std::string value) {
    return "node_" + value;
}
std::string Helper::rectangleStringBuffer(std::string shapeID) {
    return "rect_" + shapeID;
}


Rectangle Helper::createRaylibRectangle(Vector2 upperLeft, Vector2 lowerRight) {
    return Rectangle {
        upperLeft.x,
        upperLeft.y,
        lowerRight.x - upperLeft.x,
        lowerRight.y - upperLeft.y
    };
}