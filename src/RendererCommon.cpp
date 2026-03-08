#include <RendererCommon.hpp>


////////////////////////////
/// BUILTIN CONSTRUCTION ///
////////////////////////////

ShapeState createCircle(Vector2 startPosition, float radius, float outlineSize, int layerID) {
    ShapeState tempShape;
    tempShape.startPosition = startPosition;
    tempShape.size = radius;

    tempShape.sType = ShapeType::CIRCLE;

    tempShape.outlineSize = outlineSize;
    tempShape.layerID = layerID;

    return tempShape;
}

ShapeState createRectangle(Vector2 startPosition, Vector2 endPosition, float outlineSize, int layerID) {
    ShapeState tempShape;
    tempShape.startPosition = startPosition;
    tempShape.endPosition = endPosition;

    tempShape.sType = ShapeType::RECTANGLE;

    tempShape.outlineSize = outlineSize;
    tempShape.layerID = layerID;

    return tempShape;
}

ShapeState createLine(Vector2 startPosition, Vector2 endPosition, float size, float outlineSize, int layerID) {
    ShapeState tempShape;
    tempShape.startPosition = startPosition;
    tempShape.endPosition = endPosition;

    tempShape.sType = ShapeType::LINE;

    tempShape.size = size;
    tempShape.outlineSize = outlineSize;
    tempShape.layerID = layerID;

    return tempShape;
}


Text createText(std::string label, Font font, float fontSize, float spacing) {
    Text tempText;
    tempText.label = label;
    tempText.font = font;
    tempText.fontSize = fontSize;
    tempText.spacing = spacing;

    return tempText;
}

Text createText(std::string label, Font font, float fontSize, float spacing, Vector2 position) {
    Text tempText;
    tempText.label = label;
    tempText.font = font;
    tempText.fontSize = fontSize;
    tempText.spacing = spacing;
    tempText.position = position;

    return tempText;
}

Text createText(std::string label, Font font, float fontSize, float spacing, Vector2 position, Color color) {
    Text tempText;
    tempText.label = label;
    tempText.font = font;
    tempText.fontSize = fontSize;
    tempText.spacing = spacing;
    tempText.position = position;
    tempText.textColor = color;

    return tempText;
}

Rectangle createRaylibRectangle(Vector2 upperLeft, Vector2 lowerRight) {
    return Rectangle {
        upperLeft.x,
        upperLeft.y,
        lowerRight.x - upperLeft.x,
        lowerRight.y - upperLeft.y
    };
}

///////////////////////////////
///     MEMBER FUNCTIONS    ///
///////////////////////////////

void Text::setCenter(bool isCentered) {
    centerToggle = isCentered;
}

void ShapeState::setColor(Color color, Color outlineColor) {
    this -> color = color;
    this -> outlineColor = outlineColor;
}

void ShapeState::setText(Text content) {
    this -> content = content;
}

void ShapeState::setTextVisibility(bool isTrue) {
    hasText = isTrue;
}
void ShapeState::setTextCenter(bool isCentered) {
    content.setCenter(isCentered);
}