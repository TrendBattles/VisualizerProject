#include <Graphics/RendererCommon.hpp>

///////////////////
///     TEXT    ///
///////////////////

/// @brief Text Functions 
void Text::setCenter(bool isCentered) {
    centerToggle = isCentered;
}
void Text::clearTextBuffer() {
    label.clear();
}
std::string Text::getLabel() const {
    return label;
}

////////////////////
///     SHAPE    ///
////////////////////

/// @brief Shape Functions
void ShapeState::setColor(Color color, Color outlineColor) {
    this -> color = color;
    this -> outlineColor = outlineColor;
}

/// @brief Shape Visibility setting 
void ShapeState::setShapeVisibility(bool isTrue) {
    isVisible = isTrue;
}

/// @brief Text modification 
void ShapeState::setText(Text content) {
    this -> content = content;
}

/// @brief Text Visibility setting 
void ShapeState::setTextVisibility(bool isTrue) {
    hasText = isTrue;
}
/// @brief Text Centering setting 
void ShapeState::setTextCenter(bool isCentered) {
    content.setCenter(isCentered);
}

/// @brief Transiting the shapes by the same delta vector
void ShapeState::positionTransitAllBy(Vector2 delta) {
    startPosition += delta;
    endPosition += delta;
    content.position += delta;
}

///////////////////////
///     FONTS       ///
///////////////////////
void CoreFonts::load() {
    const std::string directory = "../Assets/Font/";

    const std::string CascadiaMonoDir = directory + "CascadiaMono/";
    CascadiaMonoLight = LoadFont((CascadiaMonoDir + "CascadiaMono-Light.otf").c_str());
    CascadiaMonoRegular = LoadFont((CascadiaMonoDir + "CascadiaMono-Regular.otf").c_str());
    CascadiaMonoBold = LoadFont((CascadiaMonoDir + "CascadiaMono-Bold.otf").c_str());

    Consolas = LoadFont((directory + "Consolas/CONSOLAB.TTF").c_str());
}   
void CoreFonts::unload() {
    UnloadFont(CascadiaMonoLight);
    UnloadFont(CascadiaMonoRegular);
    UnloadFont(CascadiaMonoBold);

    UnloadFont(Consolas);
}