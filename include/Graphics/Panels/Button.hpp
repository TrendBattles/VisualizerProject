#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <Graphics/RendererCommon.hpp>

class Button {
public:
    Button(ShapeState background = ShapeState(), Text label = Text());
    bool contains(Vector2 position) const;

    void setBackground(ShapeState newBackground);
    ShapeState getBackground() const;
    void setLabel(Text newLabel);
    Text getLabel() const;

    void setLabelBuffer(std::string buffer);
    void clearLabelBuffer();
    std::string getLabelBuffer();

    void positionTransitAllBy(Vector2 delta); 
private:
    ShapeState background;
    Text label;
};

enum class ButtonState {
    CLICKED,
    HOLD,
    ACTIVE,
    INACTIVE,
    DISABLED,
    HOVERED,
    HIDDEN,
};

class ButtonController {
public:
    void setButtonSettings(ButtonState state, Button button);
    void setButtonState(ButtonState newState);
    ButtonState getButtonState();
    Button getButtonShape();

    bool contains(Vector2 position);
private:
    ButtonState currentButtonState = ButtonState::ACTIVE;
    std::map <ButtonState, Button> mapContainer;
};  

using Textbox = Button;
#endif