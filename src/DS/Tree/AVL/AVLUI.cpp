#include <DS/Tree/AVL/AVLUI.hpp>
#include <Graphics/Helper.hpp>

AVLUI::AVLUI() {
    createOperationButtons("Insert", 20.0f, GetScreenHeight() - BUTTON_Y_BOTTOM_OFFSET);
    createOperationButtons("Remove", 20.0f + BUTTON_WIDTH + BUTTON_GAP, GetScreenHeight() - BUTTON_Y_BOTTOM_OFFSET);
    createOperationButtons("Search", 20.0f + (BUTTON_WIDTH + BUTTON_GAP) * 2, GetScreenHeight() - BUTTON_Y_BOTTOM_OFFSET);
    
    createField(40.0f, GetScreenHeight() - 2 * BUTTON_Y_BOTTOM_OFFSET);
}

///////////////////////////
///     UI OPTIONS      ///
///////////////////////////

/// @brief Input processing 
CommandPattern AVLUI::processInput(RawInputEvent nextInput) {
    updateOperationButtons(nextInput);
    updateField(nextInput);

    CommandPattern fieldSignal = fieldListenerRequest(nextInput);
    return fieldSignal;
}

void AVLUI::disableOption(std::string optionName) {
    {
        auto it = operationButtons.find(optionName);
        if (it != operationButtons.end()) {
            it -> second.setButtonState(ButtonState::DISABLED);
        }
    }
}
void AVLUI::enableOption(std::string optionName) {
    {
        auto it = operationButtons.find(optionName);
        if (it != operationButtons.end()) {
            it -> second.setButtonState(ButtonState::ACTIVE);
        }
    }
}
void AVLUI::disableAllOperations() {
    for (auto& it : operationButtons) {
        it.second.setButtonState(ButtonState::DISABLED);
    }
}
void AVLUI::enableAllOperations() {
    for (auto& it : operationButtons) {
        it.second.setButtonState(ButtonState::ACTIVE);
    }
}

/// @brief UI updates before rendering
void AVLUI::update() {
    for (auto& it : operationButtons) {
        if (it.second.getButtonState() == ButtonState::DISABLED || it.second.getButtonState() == ButtonState::HIDDEN) continue;
        if (it.second.getButtonShape().contains(GetMousePosition())) {
            it.second.setButtonState(ButtonState::HOVERED);
        } else {
            it.second.setButtonState(ButtonState::ACTIVE);
        }
    }

    if (fieldSubmit.getButtonShape().contains(GetMousePosition())) {
        fieldSubmit.setButtonState(ButtonState::HOVERED);
    } else {
        fieldSubmit.setButtonState(ButtonState::ACTIVE);
    }
}
/// @brief UI rendering
void AVLUI::render() {
    for (auto& it : operationButtons) {
        if (it.second.getButtonState() == ButtonState::DISABLED || it.second.getButtonState() == ButtonState::HIDDEN) continue;
        if (it.second.getButtonShape().contains(GetMousePosition())) {
            it.second.setButtonState(ButtonState::HOVERED);
        } else {
            it.second.setButtonState(ButtonState::ACTIVE);
        }
    }

    for (auto& it : operationButtons) {
        drawButton(it.second.getButtonShape());
    }

    if (operationPlaceholder.empty()) {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        return;
    }

    if (fieldTextbox.contains(GetMousePosition())) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    } else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    uiManager -> drawText(fieldPlaceholder);
    drawTextbox(fieldTextbox);
    drawButton(fieldSubmit.getButtonShape());
}
std::string AVLUI::getDSName() const { return "AVL_Tree"; }

//////////////////////////////////
///     UI Initialization      ///
//////////////////////////////////

void AVLUI::createOperationButtons(std::string buttonID, float x, float y) {
    ButtonController& addedButton = operationButtons[buttonID];

    addedButton.setButtonSettings(
        ButtonState::ACTIVE,

        Helper::createButton(
            Helper::createRectangle(
                getDSName() + "_" + Helper::rectangleStringBuffer(buttonID),
                Vector2{x, y}, Vector2{x + BUTTON_WIDTH, y + BUTTON_HEIGHT},
                2.0f, 
                GetColor(0xC9AE8AFF), BLACK,
                1
            ),

            Helper::createText(buttonID, GetFontDefault(), 20.0f, 5.0f, {0, 0}, BLACK)
        )
    );
    
    addedButton.setButtonSettings(
        ButtonState::DISABLED,
        
        Helper::createButton(
            Helper::createRectangle(
                getDSName() + "_" + Helper::rectangleStringBuffer(buttonID + "_disabled"),
                Vector2{x, y}, Vector2{x + BUTTON_WIDTH, y + BUTTON_HEIGHT},
                2.0f, 
                GetColor(0x3A3F46FF), BLACK,
                1
            ),

            Helper::createText(buttonID, GetFontDefault(), 20.0f, 5.0f, {0, 0}, GetColor(0x8A9098FF))
        )
    );

    addedButton.setButtonSettings(
        ButtonState::HOVERED,

        Helper::createButton(
            Helper::createRectangle(
                getDSName() + "_" + Helper::rectangleStringBuffer(buttonID + "_hovered"),
                Vector2{x, y}, Vector2{x + BUTTON_WIDTH, y + BUTTON_HEIGHT},
                2.0f, 
                GetColor(0x81A1C1FF), BLACK,
                1
            ),

            Helper::createText(buttonID, GetFontDefault(), 20.0f, 5.0f, {0, 0}, GetColor(0xECEFF4FF))
        )
    );
}

void AVLUI::createField(float x, float y) {
    Vector2 placeholderSize = MeasureTextEx(GetFontDefault(), "Value:", 20.0f, 5.0f);
    fieldPlaceholder = Helper::createText("Value:", GetFontDefault(), 20.0f, 5.0f, Vector2{x + placeholderSize.x / 2, y + FIELD_TEXTBOX_HEIGHT / 2}, GetColor(0xD9EBFFFF));
    fieldPlaceholder.setCenter(true);

    Vector2 fieldTextboxOrigin = {
        x + placeholderSize.x + FIELD_GAP,
        y
    };
    fieldTextbox = Helper::createTextbox(
        Helper::createRectangle(
            getDSName() + "_" + Helper::rectangleStringBuffer("Field_Textbox"),
            fieldTextboxOrigin, fieldTextboxOrigin + Vector2{FIELD_TEXTBOX_WIDTH, FIELD_TEXTBOX_HEIGHT}, 
            2.0f, 
            GetColor(0x3B4252FF), BLACK,
            1
        ),
        Helper::createText("", GetFontDefault(), 20.0f, 5.0f, {0, 0}, WHITE)
    );

    Vector2 fieldSubmitOrigin = {
        fieldTextbox.getBackground().endPosition.x + FIELD_GAP,
        y
    };

    fieldSubmit.setButtonSettings(
        ButtonState::ACTIVE,

        Helper::createButton(
            Helper::createRectangle(
                getDSName() + "_" + Helper::rectangleStringBuffer("Field_Submit"),
                fieldSubmitOrigin, fieldSubmitOrigin + Vector2{FIELD_SUBMIT_WIDTH, FIELD_SUBMIT_HEIGHT}, 
                2.0f, 
                GetColor(0xC9AE8AFF), BLACK,
                1
            ),
            
            Helper::createText("Submit", GetFontDefault(), 20.0f, 5.0f, {0, 0}, BLACK)
        )
    );
    fieldSubmit.setButtonSettings(
        ButtonState::HOVERED,

        Helper::createButton(
            Helper::createRectangle(
                getDSName() + "_" + Helper::rectangleStringBuffer("Field_Submit_hovered"),
                fieldSubmitOrigin, fieldSubmitOrigin + Vector2{FIELD_SUBMIT_WIDTH, FIELD_SUBMIT_HEIGHT},  
                2.0f, 
                GetColor(0x81A1C1FF), BLACK,
                1
            ),

            Helper::createText("Submit", GetFontDefault(), 20.0f, 5.0f, {0, 0}, GetColor(0xECEFF4FF))
        )
    );
}


////////////////////////////////////
///     SUPPORTIVE FUNCTIONS     ///
////////////////////////////////////

/// @brief Operation Interactions 
void AVLUI::updateOperationButtons(RawInputEvent nextInput) {
    if (nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED) {
        // Avoid closing the button sections when inputting/submitting the field
        if (!operationPlaceholder.empty()) {
            if (fieldTextbox.contains(nextInput.position)) return;

            if (fieldSubmit.getButtonShape().contains(nextInput.position)) return;
        }
        
        std::string chosenOperation = ""; 
        for (auto& it : operationButtons) {
            // Discard disabled/hidden buttons

            if (it.second.getButtonState() == ButtonState::DISABLED || it.second.getButtonState() == ButtonState::HIDDEN) continue;

            if (it.second.contains(nextInput.position)) {
                chosenOperation = it.first;
                break;
            }
        }

        if (chosenOperation != operationPlaceholder) {
            operationPlaceholder = chosenOperation;
            changeField();
        }
    }
}
 
/// @brief Field Interactions
void AVLUI::changeField() {
    fieldTextbox.clearLabelBuffer();
    isFieldTextboxFocused = false;
}
void AVLUI::updateField(RawInputEvent nextInput) {
    if (operationPlaceholder.empty()) return;
    
    if (nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED) {
        isFieldTextboxFocused = fieldTextbox.contains(nextInput.position);
    }
    if (nextInput.inputType == RawInputEvent::InputType::RIGHT_MOUSE_CLICKED) {
        isFieldTextboxFocused = false;
    }

    if (!isFieldTextboxFocused) return;
    if (nextInput.inputType != RawInputEvent::InputType::KEY_PRESSED) return;

    int keyID = (int) nextInput.keySignal;

    std::string inputStr = fieldTextbox.getLabelBuffer();
    if (keyID >= '0' and keyID <= '9' and (int) inputStr.length() < TEXTBOX_LENGTH_LIMIT) {
        inputStr.push_back(keyID);
    }
        
    if (keyID == (int) KeyboardKey::KEY_BACKSPACE && !inputStr.empty()) {
        inputStr.pop_back();
    }

    fieldTextbox.setLabelBuffer(inputStr);
}

/// @brief Responds requests to Data Structure  
CommandPattern AVLUI::fieldListenerRequest(RawInputEvent nextInput) {
    if (operationPlaceholder.empty()) return CommandPattern();

    bool submitEnter = nextInput.inputType == RawInputEvent::InputType::KEY_PRESSED
                    && nextInput.keySignal == KeyboardKey::KEY_ENTER;
    bool submitClick = nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED
                    && fieldSubmit.getButtonShape().contains(nextInput.position);
    if (submitEnter || submitClick) {
        std::string rawValue = fieldTextbox.getLabelBuffer();
        std::string chosenOperation = operationPlaceholder;

        operationPlaceholder = "";
        changeField();

        if (rawValue.empty()) return CommandPattern();

        return CommandPattern {
            chosenOperation == "Search" ? "QUERY" : "MODIFY",
            "INTERACT",
            getDSName(),
            Helper::upperString(chosenOperation),
            rawValue
        };
    }

    return CommandPattern();
}