#include <Graphics/Panels/Button.hpp>
#include <Graphics/Helper.hpp>

////////////////////////////////////
///     BUTTON CONSTRUCTION      ///
////////////////////////////////////

Button::Button(ShapeState background, Text label) {
    this -> background = background;
    this -> label = label;
}
bool Button::contains(Vector2 point) const {
    return Helper::IsPointCollidedWithShape(point, background);
}
void Button::setBackground(ShapeState newBackground) {
    background = newBackground;
}
ShapeState Button::getBackground() const {
    return background;
}
void Button::setLabel(Text newLabel) {
    label = newLabel;
}
Text Button::getLabel() const {
    return label;
}

void Button::setLabelBuffer(std::string buffer) {
    label.label = buffer;
}
void Button::clearLabelBuffer() {
    label.clearTextBuffer();
}
std::string Button::getLabelBuffer() {
    return label.getLabel();
}


////////////////////////////////
///     BUTTON CONTROLS      ///
////////////////////////////////

void ButtonController::setButtonSettings(ButtonState state, Button button) {
    mapContainer[state] = button;
}
void ButtonController::setButtonState(ButtonState state) {
    currentButtonState = state;
}
ButtonState ButtonController::getButtonState() {
    return currentButtonState;
}
Button ButtonController::getButtonShape() {
    auto it = mapContainer.find(currentButtonState);
    if (it == mapContainer.end()) {
        throw std::runtime_error("Target button setting hasn't added\n");
    }

    return it -> second;
}

bool ButtonController::contains(Vector2 position) {
    return getButtonShape().contains(position);
}