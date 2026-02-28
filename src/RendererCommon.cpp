#include <RendererCommon.hpp>

Rectangle createRaylibRectangle(Vector2 upperLeft, Vector2 lowerRight) {
    return Rectangle {
        upperLeft.x,
        upperLeft.y,
        lowerRight.x - upperLeft.x,
        lowerRight.y - upperLeft.y
    };
}