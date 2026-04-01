#include <DS/Tree/AVLUI.hpp>
#include <Graphics/Helper.hpp>

AVLUI::AVLUI() {
    createButtons("Insert", 20.0f, GetScreenHeight() - BUTTON_Y_BOTTOM_OFFSET);
    createButtons("Remove", 20.0f + BUTTON_WIDTH + BUTTON_GAP, GetScreenHeight() - BUTTON_Y_BOTTOM_OFFSET);
    createButtons("Search", 20.0f + (BUTTON_WIDTH + BUTTON_GAP) * 2, GetScreenHeight() - BUTTON_Y_BOTTOM_OFFSET);
    
    createField(40.0f, GetScreenHeight() - 2 * BUTTON_Y_BOTTOM_OFFSET);
}

std::string AVLUI::processInput(RawInputEvent nextInput) {
    std::string buttonSignal = buttonListenerRequest(nextInput);
    if (!buttonSignal.empty()) return buttonSignal;

    std::string fieldSignal = fieldListenerRequest(nextInput);
    if (!fieldSignal.empty()) return fieldSignal;

    return "";
}

void AVLUI::disableOption(std::string optionName) {
    if (find(operationList.begin(), operationList.end(), optionName) != operationList.end()) {
        isButtonDisabledMap[optionName] = true;
    }
}
void AVLUI::enableOption(std::string optionName) {
    if (find(operationList.begin(), operationList.end(), optionName) != operationList.end()) {
        isButtonDisabledMap[optionName] = false;
    }
}
void AVLUI::disableAllOperations() {
    for (std::string operation : operationList) {
        isButtonDisabledMap[operation] = true;
    }
}
void AVLUI::enableAllOperations() {
    for (std::string operation : operationList) {
        isButtonDisabledMap[operation] = false;
    }
}

void AVLUI::render() {
    for (std::string operation : operationList) {
        int buttonState = isButtonDisabledMap[operation] ? 1 : (int) buttonMap[operation][0].contains(GetMousePosition()) * 2;
        Button& currentButton = buttonMap[operation][buttonState];
        drawButton(currentButton);
    }

    if (operationPlaceholder.empty()) {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        return;
    }

    if (fieldTextBox.contains(GetMousePosition())) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    } else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    uiManager -> drawText(fieldPlaceholder);
    drawTextBox(fieldTextBox);

    int fieldSubmitState = fieldSubmit[0].contains(GetMousePosition());
    drawButton(fieldSubmit[fieldSubmitState]);
}
std::string AVLUI::getDSName() const { return "AVL_Tree"; }

void AVLUI::createButtons(std::string buttonID, float x, float y) {
    std::array <Button, 3>& addedButton = buttonMap[buttonID];
    isButtonDisabledMap[buttonID] = false;

    addedButton[0] = Helper::createButton(
        Helper::createRectangle("rect_AVL" + buttonID + "_button", Vector2{x, y}, Vector2{x + BUTTON_WIDTH, y + BUTTON_HEIGHT}, 2.0f, 1),
        Helper::createText(buttonID, GetFontDefault(), 20.0f, 5.0f, {0, 0}, BLACK)
    );
    addedButton[0].background.setColor(GetColor(0xC9AE8AFF), BLACK);

    addedButton[1] = Helper::createButton(
        Helper::createRectangle("rect_AVL" + buttonID + "_button_disabled", Vector2{x, y}, Vector2{x + BUTTON_WIDTH, y + BUTTON_HEIGHT}, 2.0f, 1),
        Helper::createText(buttonID, GetFontDefault(), 20.0f, 5.0f, {0, 0}, GetColor(0x8A9098FF))
    );
    addedButton[1].background.setColor(GetColor(0x3A3F46FF), BLACK);

    addedButton[2] = Helper::createButton(
        Helper::createRectangle("rect_AVL" + buttonID + "_button_hovered", Vector2{x, y}, Vector2{x + BUTTON_WIDTH, y + BUTTON_HEIGHT}, 2.0f, 1),
        Helper::createText(buttonID, GetFontDefault(), 20.0f, 5.0f, {0, 0}, GetColor(0xECEFF4FF))
    );
    addedButton[2].background.setColor(GetColor(0x81A1C1FF), BLACK);
}
std::string AVLUI::buttonListenerRequest(RawInputEvent nextInput) {
    if (nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED) {
        if (!operationPlaceholder.empty()) {
            if (fieldTextBox.contains(nextInput.position)) return "";

            if (fieldSubmit[0].contains(nextInput.position)) return "";
        }
        
        std::string chosenOperation = ""; 
        for (std::string operation : operationList) {
            if (isButtonDisabledMap[operation]) continue;
            
            if (buttonMap[operation][0].contains(nextInput.position)) {
                chosenOperation = operation;
                break;
            }
        }

        if (chosenOperation != operationPlaceholder) {
            operationPlaceholder = chosenOperation;
            fieldTextBox.label.clearTextBuffer();
            isFieldTextBoxFocused = false;
        }
    }

    return "";
}

void AVLUI::createField(float x, float y) {
    Vector2 placeholderSize = MeasureTextEx(GetFontDefault(), "Value:", 20.0f, 5.0f);
    fieldPlaceholder = Helper::createText("Value:", GetFontDefault(), 20.0f, 5.0f, Vector2{x + placeholderSize.x / 2, y + FIELD_TEXTBOX_HEIGHT / 2}, GetColor(0xD9EBFFFF));
    fieldPlaceholder.setCenter(true);

    fieldTextBox = Helper::createTextBox(
        Helper::createRectangle("rect_AVL_Field_TextBox", Vector2{x + placeholderSize.x + FIELD_GAP, y}, Vector2{x + placeholderSize.x + FIELD_GAP + FIELD_TEXTBOX_WIDTH, y + FIELD_TEXTBOX_HEIGHT}, 2.0f, 1),
        Helper::createText("", GetFontDefault(), 20.0f, 5.0f, {0, 0}, WHITE)
    );
    fieldTextBox.background.setColor(GetColor(0x3B4252FF), BLACK);
    //fieldTextBox.background.setColor(GetColor(0xFFFFFFCC), BLACK);

    fieldSubmit[0] = Helper::createButton(
        Helper::createRectangle("rect_AVL_Field_Submit", Vector2{fieldTextBox.background.endPosition.x, fieldTextBox.background.startPosition.y} + Vector2{FIELD_GAP, 0}, Vector2{fieldTextBox.background.endPosition.x, fieldTextBox.background.startPosition.y} + Vector2{FIELD_GAP + FIELD_SUBMIT_WIDTH, FIELD_SUBMIT_HEIGHT}, 2.0f, 1),
        Helper::createText("Submit", GetFontDefault(), 20.0f, 5.0f, {0, 0}, BLACK)
    );
    fieldSubmit[0].background.setColor(GetColor(0xC9AE8AFF), BLACK);

    fieldSubmit[1] = Helper::createButton(
        Helper::createRectangle("rect_AVL_Field_Submit", Vector2{fieldTextBox.background.endPosition.x, fieldTextBox.background.startPosition.y} + Vector2{FIELD_GAP, 0}, Vector2{fieldTextBox.background.endPosition.x, fieldTextBox.background.startPosition.y} + Vector2{FIELD_GAP + FIELD_SUBMIT_WIDTH, FIELD_SUBMIT_HEIGHT}, 2.0f, 1),
        Helper::createText("Submit", GetFontDefault(), 20.0f, 5.0f, {0, 0}, GetColor(0xECEFF4FF))
    );
    fieldSubmit[1].background.setColor(GetColor(0x81A1C1FF), BLACK);
}
std::string AVLUI::fieldListenerRequest(RawInputEvent nextInput) {
    if (operationPlaceholder.empty()) return "";

    if (nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED) {
        if (fieldSubmit[0].contains(nextInput.position)) {
            std::string rawValue = fieldTextBox.label.getLabel();
            std::string chosenOperation = operationPlaceholder;

            fieldTextBox.label.clearTextBuffer();
            isFieldTextBoxFocused = false;
            operationPlaceholder = "";

            if (rawValue.empty()) return "";

            std::string command = (chosenOperation == "Search" ? "QUERY INTERACT " : "MODIFY INTERACT ");
            command += getDSName() + " " + Helper::upperString(chosenOperation) + " ";
            command += rawValue;
        
            return command;
        }

        isFieldTextBoxFocused = fieldTextBox.contains(nextInput.position);
    }

    if (!isFieldTextBoxFocused) return "";

    if (nextInput.inputType != RawInputEvent::InputType::KEY_PRESSED) return "";

    int keyID = (int) nextInput.keySignal;

    std::string& inputStr = fieldTextBox.label.label;
    if (keyID >= '0' and keyID <= '9' and (int) inputStr.length() < TEXTBOX_LENGTH_LIMIT) {
        inputStr.push_back(keyID);
    }
        
    if (keyID == (int) KeyboardKey::KEY_BACKSPACE && !inputStr.empty()) {
        inputStr.pop_back();
    }
    return "";
}