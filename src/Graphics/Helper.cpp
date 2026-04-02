#include <Graphics/Helper.hpp>
#include <stdexcept>

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
    tempShape.endPosition = startPosition;
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

    tempShape.size = 0;
    tempShape.outlineSize = outlineSize;
    tempShape.layerID = layerID;

    return tempShape;
}
ShapeState Helper::createRectangle(std::string tempoID, Vector2 startPosition, Vector2 endPosition, float outlineSize, Color color, Color outlineColor, int layerID) {
    ShapeState tempShape;
    tempShape.shapeID = tempoID;
    tempShape.startPosition = startPosition;
    tempShape.endPosition = endPosition;

    tempShape.sType = ShapeType::RECTANGLE;

    tempShape.size = 0;
    tempShape.outlineSize = outlineSize;
    tempShape.color = color;
    tempShape.outlineColor = outlineColor;
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
ShapeState Helper::createLine(std::string tempoID, Vector2 startPosition, Vector2 endPosition, float size, float outlineSize, Color color, Color outlineColor, int layerID) {
    ShapeState tempShape;
    tempShape.shapeID = tempoID;
    tempShape.startPosition = startPosition;
    tempShape.endPosition = endPosition;

    tempShape.sType = ShapeType::LINE;

    tempShape.size = size;
    tempShape.outlineSize = outlineSize;
    tempShape.color = color;
    tempShape.outlineColor = outlineColor;
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
ShapeState Helper::createArrow(std::string tempoID, Vector2 startPosition, Vector2 endPosition, float size, float outlineSize, Color color, Color outlineColor, int layerID) {
    //Text creation
    ShapeState tempShape;
    tempShape.shapeID = tempoID;
    tempShape.startPosition = startPosition;
    tempShape.endPosition = endPosition;

    tempShape.sType = ShapeType::ARROW;

    tempShape.size = size;
    tempShape.outlineSize = outlineSize;
    tempShape.color = color;
    tempShape.outlineColor = outlineColor;
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
/// Animation Lerps
///
ShapeState Helper::shapeTimeLerp(const ShapeState& shapeA, const ShapeState& shapeB, float rate) {
    if (shapeA.shapeID != shapeB.shapeID) throw std::runtime_error("[ERROR]: Shape Lerps unable to continue because of different IDs.");
    if (rate < 0 || rate > 1) throw std::runtime_error("[ERROR]: Lerp only accepts rates in [0, 1]");

    //Shape Lerp
    ShapeState lerpResult = shapeA;
    lerpResult.startPosition = Vector2Lerp(shapeA.startPosition, shapeB.startPosition, rate);
    lerpResult.endPosition = Vector2Lerp(shapeA.endPosition, shapeB.endPosition, rate);

    lerpResult.size = Lerp(shapeA.size, shapeB.size, rate);
    lerpResult.outlineSize = Lerp(shapeA.outlineSize, shapeB.outlineSize, rate);

    lerpResult.color = colorTimeLerp(shapeA.color, shapeB.color, rate);
    lerpResult.outlineColor = colorTimeLerp(shapeA.outlineColor, shapeB.outlineColor, rate);

    //Text Lerp
    Text& lerpText = lerpResult.content;

    lerpText.textColor = colorTimeLerp(shapeA.content.textColor, shapeB.content.textColor, rate);
    lerpText.position = Vector2Lerp(shapeA.content.position, shapeB.content.position, rate);

    return lerpResult;
} 
Color Helper::colorTimeLerp(Color colorA, Color colorB, float rate) {
    if (rate < 0 || rate > 1) throw std::runtime_error("[ERROR]: Lerp only accepts rates in [0, 1]");

    return Color {
        (unsigned char) (int) Lerp((int) colorA.r, (int) colorB.r, rate),
        (unsigned char) (int) Lerp((int) colorA.g, (int) colorB.g, rate),
        (unsigned char) (int) Lerp((int) colorA.b, (int) colorB.b, rate),
        (unsigned char) (int) Lerp((int) colorA.a, (int) colorB.a, rate),
    };
} 


///
/// Intersection, Collision
///
bool Helper::IsPointCollidedWithShape(Vector2 position, ShapeState shape) {
    if (shape.sType == ShapeType::RECTANGLE) {
        Vector2 boundary = {shape.outlineSize, shape.outlineSize};

        return CheckCollisionPointRec(position, createRaylibRectangle(shape.startPosition - boundary, shape.endPosition + boundary));
    }

    if (shape.sType == ShapeType::CIRCLE) {
        return Vector2Distance(position, shape.startPosition) <= shape.size + shape.outlineSize;
    }

    return false;
}

///
/// Creation of more complex designs
///
Button Helper::createButton(ShapeState background, Text label) {
    label.setCenter(true);
    label.position = Vector2Lerp(background.startPosition, background.endPosition, 0.5f);
    return Button {background, label};
}
Textbox Helper::createTextbox(ShapeState background, Text label) {
    return createButton(background, label);
}

///
/// Additionals
///
std::string Helper::edgeStringBuffer(std::string objectA, std::string objectB) {
    return "edge_" + objectA + "_" + objectB;
}
std::string Helper::edgePointStringBuffer(std::string object) {
    return "edge_" + object;
}
std::string Helper::arrowStringBuffer(std::string objectA, std::string objectB) {
    return "arrow_" + objectA + "_" + objectB;
}
std::string Helper::arrowPointStringBuffer(std::string object) {
    return "arrow_" + object;
}
std::string Helper::nodeStringBuffer(int value) {
    return nodeStringBuffer(std::to_string(value));
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

std::string Helper::upperString(std::string str) {
    for (char& x : str) {
        if (x >= 'a' and x <= 'z') {
            x = x - 'a' + 'A';
        }
    }

    return str;
}