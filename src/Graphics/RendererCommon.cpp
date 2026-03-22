#include <Graphics/RendererCommon.hpp>
#include "raymath.h"

///////////////////////////////
///     MEMBER FUNCTIONS    ///
///////////////////////////////

/// @brief Text Functions 
void Text::setCenter(bool isCentered) {
    centerToggle = isCentered;
}

/// @brief Shape Functions
void ShapeState::setColor(Color color, Color outlineColor) {
    this -> color = color;
    this -> outlineColor = outlineColor;
}

/// @brief Text modification 
void ShapeState::setText(Text content) {
    this -> content = content;
}

/// @brief Text Visibility settings 
void ShapeState::setTextVisibility(bool isTrue) {
    hasText = isTrue;
}
/// @brief Text Centering settings 
void ShapeState::setTextCenter(bool isCentered) {
    content.setCenter(isCentered);
}

/// @brief Transiting the shapes by the same delta vector
void ShapeState::positionTransitAllBy(Vector2 delta) {
    startPosition += delta;
    endPosition += delta;
    content.position += delta;
}