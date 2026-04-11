#include <Graphics/Panels/NavigationBar.hpp>

///////////////////////////////
///     INITIALIZATIONS     ///
///////////////////////////////
NavigationBar::NavigationBar() = default;
NavigationBar::~NavigationBar() = default;

void NavigationBar::setNavOrigin(Vector2 position) {
    navOrigin = position;
}
void NavigationBar::setCenter(bool isTrue) {
    isCentered = isTrue;
}

void NavigationBar::setBackground(ShapeState newBackground) {
    newBackground.endPosition -= newBackground.startPosition;
    newBackground.startPosition = {0.0f, 0.0f};

    navBackground = newBackground;
}
ShapeState NavigationBar::getBackground() const {
    return navBackground;
}

///////////////////////////////////////////////////////////////
///     BUTTON CONTROLLER FORMATS/MODIFCATIONS/REQUESTS     ///
///////////////////////////////////////////////////////////////
void NavigationBar::setButtonController(const std::string& buttonID, ButtonController buttonController) {
    navButtons[buttonID] = buttonController;
}
ButtonController* NavigationBar::getButtonController(const std::string& buttonID) {
    auto it = navButtons.find(buttonID);
    if (it == navButtons.end()) return nullptr;

    return &it -> second;
}
bool NavigationBar::findButtonController(const std::string& buttonID) {
    return navButtons.find(buttonID) != navButtons.end();
}
bool NavigationBar::inButtonHitbox(const std::string& buttonID, Vector2 position) {
    if (!findButtonController(buttonID)) return false;

    ButtonController& targetController = navButtons[buttonID];
    Button buttonState = targetController.getButtonShape();
    ShapeState buttonShape = buttonState.getBackground();
    
    if (isCentered) {
        float centerX = (buttonShape.startPosition.x + buttonShape.endPosition.x) / 2;
        float targetCenterX = navBackground.endPosition.x / 2;
                
        buttonShape.positionTransitAllBy({targetCenterX - centerX, 0.0f});
        buttonState.setBackground(buttonShape);
    }

    return buttonState.contains(position - navOrigin);
}

/// @brief Returning the buttonID key that was triggered by the user input
std::string NavigationBar::processInput(RawInputEvent nextInput) {
    if (nextInput.inputType != RawInputEvent::InputType::LEFT_MOUSE_CLICKED) {
        return "";
    }

    for (auto& it : navButtons) {
        if (it.second.getButtonState() == ButtonState::HIDDEN || it.second.getButtonState() == ButtonState::DISABLED) continue;
        if (inButtonHitbox(it.first, nextInput.position)) {
            return it.first;
        }
    }

    return "";
}

/// @brief This functions processes hovering effects of the buttons in the navigation sidebar
void NavigationBar::hoverButtonTrigger() {
    Vector2 mouse = GetMousePosition();

    for (auto& it : navButtons) {
        if (it.second.getButtonState() == ButtonState::HIDDEN || it.second.getButtonState() == ButtonState::DISABLED) continue;

        Button buttonState = it.second.getButtonShape();
        ShapeState buttonShape = buttonState.getBackground();
        
        if (isCentered) {
            float centerX = (buttonShape.startPosition.x + buttonShape.endPosition.x) / 2;
            float targetCenterX = navBackground.endPosition.x / 2;
                    
            buttonShape.positionTransitAllBy({targetCenterX - centerX, 0.0f});
            buttonState.setBackground(buttonShape);
        }

        if (buttonState.contains(mouse - navOrigin)) {
            it.second.setButtonState(ButtonState::HOVERED);
        } else {
            it.second.setButtonState(ButtonState::ACTIVE);
        }
    }
}

/// @brief Return a snapshot of the navigation sidebar in order to render on the screen 
Snapshot NavigationBar::getNavigationSnapshot() {
    Snapshot navSnapshot;
    navSnapshot.emplace_back(navBackground);
    navSnapshot.back().positionTransitAllBy(navOrigin);

    for (auto& it : navButtons) {
        Button buttonState = it.second.getButtonShape();
        ShapeState buttonShape = buttonState.getBackground();
        buttonShape.setText(buttonState.getLabel());

        if (isCentered) {
            float centerX = (buttonShape.startPosition.x + buttonShape.endPosition.x) / 2;
            float targetCenterX = navBackground.endPosition.x / 2;
                
            buttonShape.positionTransitAllBy({targetCenterX - centerX, 0.0f});
        }

        buttonShape.positionTransitAllBy(navOrigin);
        navSnapshot.emplace_back(buttonShape);
    }

    sort(navSnapshot.begin(), navSnapshot.end(), [&] (ShapeState& A, ShapeState& B) {
        return A.layerID < B.layerID;
    });
    
    return navSnapshot;
}

