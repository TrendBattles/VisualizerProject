#include <DS/Graph/GraphUI.hpp>
#include <Graphics/Helper.hpp>

GraphUI::GraphUI() {
    createNavbar();
    
    createField();
}

///////////////////////////
///     UI OPTIONS      ///
///////////////////////////

/// @brief Input processing 
CommandPattern GraphUI::processInput(RawInputEvent nextInput) {
    CommandPattern navbarSignal = navbarListenerRequest(nextInput);
    if (!navbarSignal.prefix.empty()) return navbarSignal;
    
    CommandPattern fieldSignal = fieldListenerRequest(nextInput);
    if (!fieldSignal.prefix.empty()) return fieldSignal;

    processInputNavbar(nextInput);
    processInputField(nextInput);

    return CommandPattern();
}

void GraphUI::disableOption(const std::string& optionName) {
    for (auto& it : navbarMap) {
        ButtonController* targetController = it.second.getButtonController(optionName);
        if (targetController != nullptr) {
            targetController -> setButtonState(ButtonState::DISABLED);
        }
    }
}
void GraphUI::enableOption(const std::string& optionName) {
    for (auto& it : navbarMap) {
        ButtonController* targetController = it.second.getButtonController(optionName);
        if (targetController != nullptr) {
            targetController -> setButtonState(ButtonState::ACTIVE);
        }
    }
}
void GraphUI::disableAllOperations() {
    for (const std::string& buttonID : operationList) {
        ButtonController* targetController = navbarMap[NavPhase::NAV_OPERATIONS].getButtonController(buttonID);
        
        if (targetController != nullptr) {
            targetController -> setButtonState(ButtonState::DISABLED);
        }
    }
}
void GraphUI::enableAllOperations() {
    for (const std::string& buttonID : operationList) {
        ButtonController* targetController = navbarMap[NavPhase::NAV_OPERATIONS].getButtonController(buttonID);
        if (targetController != nullptr) {
            targetController -> setButtonState(ButtonState::ACTIVE);
        }
    }
}

/////////////////////////
///     UI UPDATE     ///
/////////////////////////

/// @brief UI updates before rendering
void GraphUI::update() {
    NavigationBar& navbar = navbarMap[navPhase];
    navbar.hoverButtonTrigger();

    if (operationPlaceholder.empty()) {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        return;
    }

    ButtonController* targetController = navbarMap[navPhase].getButtonController(operationPlaceholder);
    ShapeState targetBackground = targetController -> getButtonShape().getBackground();
    Vector2 rootPos = {FIELD_GAP_X + BUTTON_WIDTH, targetBackground.startPosition.y};

    hoverTextboxTrigger(rootPos);

    if (operationPlaceholder == "Insert Edge") {
        hoverButtonTrigger(rootPos + Vector2{0.0f, FIELD_GAP_Y + FIELD_TEXTBOX_HEIGHT});
    } else if (operationPlaceholder == "Remove Edge") {
        hoverButtonTrigger(rootPos + Vector2{0.0f, FIELD_GAP_Y + FIELD_TEXTBOX_HEIGHT});
    } else if (operationPlaceholder == "Remove Node") {
        hoverButtonTrigger(rootPos + Vector2{FIELD_GAP_X + FIELD_TEXTBOX_WIDTH, 0.0f});
    } else if (operationPlaceholder == "Dijkstra") {
        hoverButtonTrigger(rootPos + Vector2{0.0f, FIELD_GAP_Y + FIELD_TEXTBOX_HEIGHT});
    }
}
void GraphUI::hoverTextboxTrigger(Vector2 rootPos) {
    bool hoveredTextbox = false;
    for (int i = 0; i < activeFieldCount() && !hoveredTextbox; ++i) {
        Vector2 nextPos = rootPos + Vector2{i * (FIELD_TEXTBOX_WIDTH + FIELD_GAP_X), 0.0f};
        if (fieldTextbox[i].contains(GetMousePosition() - nextPos)) {
            hoveredTextbox = true;
        }
    }
    if (hoveredTextbox) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    } else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}
void GraphUI::hoverButtonTrigger(Vector2 rootPos) {
    if (fieldRandom.getButtonShape().contains(GetMousePosition() - rootPos)) {
        fieldRandom.setButtonState(ButtonState::HOVERED);
    } else {
        fieldRandom.setButtonState(ButtonState::ACTIVE);
    }

    rootPos += Vector2{FIELD_GAP_X + FIELD_BUTTON_WIDTH, 0.0f};

    if (fieldSubmit.getButtonShape().contains(GetMousePosition() - rootPos)) {
        fieldSubmit.setButtonState(ButtonState::HOVERED);
    } else {
        fieldSubmit.setButtonState(ButtonState::ACTIVE);
    }
}

/////////////////////////
///     UI RENDER     ///
/////////////////////////

void GraphUI::render() {
    if (navPhase != NavPhase::NAV_CLOSED) {
        DrawRectangle(BUTTON_WIDTH, 0, GetScreenWidth() - BUTTON_WIDTH, GetScreenHeight(), Fade(GetColor(0x2E3440FF), 0.5f));
    }
    uiManager -> renderSnapshot(navbarMap[navPhase].getNavigationSnapshot());

    if (operationPlaceholder.empty()) {
        return;
    }

    ButtonController* targetController = navbarMap[navPhase].getButtonController(operationPlaceholder);
    ShapeState targetBackground = targetController -> getButtonShape().getBackground();
    Vector2 rootPos = {FIELD_GAP_X + BUTTON_WIDTH, targetBackground.startPosition.y};

    renderTextbox(rootPos);

    if (operationPlaceholder == "Insert Edge") {
        renderButton(rootPos + Vector2{0.0f, FIELD_GAP_Y + FIELD_TEXTBOX_HEIGHT});
    } else if (operationPlaceholder == "Remove Edge") {
        renderButton(rootPos + Vector2{0.0f, FIELD_GAP_Y + FIELD_TEXTBOX_HEIGHT});
    } else if (operationPlaceholder == "Remove Node") {
        renderButton(rootPos + Vector2{FIELD_GAP_X + FIELD_TEXTBOX_WIDTH, 0.0f});
    } else if (operationPlaceholder == "Dijkstra") {
        renderButton(rootPos + Vector2{0.0f, FIELD_GAP_Y + FIELD_TEXTBOX_HEIGHT});
    }
}
void GraphUI::renderTextbox(Vector2 rootPos) {
    for (int i = 0; i < activeFieldCount(); ++i) {
        Vector2 nextPos = rootPos + Vector2{i * (FIELD_TEXTBOX_WIDTH + FIELD_GAP_X), 0.0f};

        Textbox tempTextbox = fieldTextbox[i];
        tempTextbox.positionTransitAllBy(nextPos);

        if (textboxFocusID == i) {
            ShapeState background = tempTextbox.getBackground();
            background.outlineColor = GetColor(0x81A1C1FF);
            tempTextbox.setBackground(background);
        }

        drawTextbox(tempTextbox);
    }
}
void GraphUI::renderButton(Vector2 rootPos) {
    Button tempRandom = fieldRandom.getButtonShape();
    tempRandom.positionTransitAllBy(rootPos);

    rootPos += Vector2{FIELD_GAP_X + FIELD_BUTTON_WIDTH, 0.0f};
    
    Button tempSubmit = fieldSubmit.getButtonShape();
    tempSubmit.positionTransitAllBy(rootPos);

    drawButton(tempRandom);
    drawButton(tempSubmit);
}

std::string GraphUI::getDSName() const { return "Graph"; }

int GraphUI::activeFieldCount() const {
    if (operationPlaceholder == "Insert Edge") {
        return 3;
    } else if (operationPlaceholder == "Remove Edge") {
        return 2;
    } else if (operationPlaceholder == "Remove Node") {
        return 1;
    } else if (operationPlaceholder == "Dijkstra") {
        return 2;
    }
    
    return 0;
}

///////////////////////
///     NAVBAR      ///
///////////////////////
/// @brief Creating the nagivation sidebar

void GraphUI::createNavbar() {
    NavigationBar& phase0 = navbarMap[NavPhase::NAV_CLOSED];
    Vector2 navigationHiddenSize = {BUTTON_WIDTH / 2, (float) GetScreenHeight()};
    phase0.setBackground(Helper::createRectangle(
        getDSName() + "_hiddenNav",
        {0.0f, 0.0f},
        navigationHiddenSize,
        1.0f,
        Fade(BLACK, 0.2f), Fade(WHITE, 0.1f),
        -1
    ));
    phase0.setCenter(true);

    
    NavigationBar& phase1 = navbarMap[NavPhase::NAV_OPERATIONS];
    phase1 = createNavBar(operationList);

    NavigationBar& phase2 = navbarMap[NavPhase::NAV_OPTIONS];
    phase2 = createNavBar(optionList);

    NavigationBar& phase3 = navbarMap[NavPhase::NAV_DS_OPTIONS];
    phase3 = createNavBar(DSList);

    createNavbarToggle();
}

NavigationBar GraphUI::createNavBar(const std::vector <std::string>& buttonList) {
    NavigationBar tempoNav;
    Vector2 navigationSize = {BUTTON_WIDTH, (float) GetScreenHeight()};
    tempoNav.setBackground(Helper::createRectangle(
        getDSName() + "_Nav",
        {0.0f, 0.0f},
        navigationSize,
        1.0f,
        Fade(BLACK, 0.2f), Fade(WHITE, 0.1f),
        -1
    ));

    Vector2 totalSize = Vector2{BUTTON_WIDTH, BUTTON_HEIGHT * (int) buttonList.size() + BUTTON_GAP * ((int) buttonList.size() - 1)};
    Vector2 startPos = (navigationSize - totalSize) * 0.5f;

    for (int i = 0; i < (int) buttonList.size(); ++i) {
        ButtonController operationButton = createNavbarButtons(buttonList[i], startPos.x, startPos.y + i * (BUTTON_HEIGHT + BUTTON_GAP));
        tempoNav.setButtonController(buttonList[i], operationButton);
    }
    
    tempoNav.setCenter(true);
    return tempoNav;
}

ButtonController GraphUI::createNavbarButtons(const std::string& buttonID, float x, float y) {
    auto createButton = [&] (std::string suffix, Font font, Color backgroundColor, Color outlineColor, Color textColor) -> Button {
        return Helper::createButton(
            Helper::createRectangle(
                getDSName() + "_" + Helper::rectangleStringBuffer(buttonID + suffix),
                Vector2{x, y}, Vector2{x + BUTTON_WIDTH, y + BUTTON_HEIGHT},
                3.0f, 
                backgroundColor, outlineColor,
                1
            ),

            Helper::createText(buttonID, font, 25.0f, 2.5f, {0, 0}, textColor)
        );
    }; 
    ButtonController addedButton;

    addedButton.setButtonSettings(ButtonState::ACTIVE, createButton("", CoreFonts::Aptos, GetColor(0), GetColor(0), WHITE));
    addedButton.setButtonSettings(ButtonState::DISABLED, createButton("_disabled", CoreFonts::Aptos, GetColor(0), GetColor(0), Fade(GetColor(0x3A3F46FF), 0.5f)));
    addedButton.setButtonSettings(ButtonState::HOVERED, createButton("_hovered", CoreFonts::AptosBold, GetColor(0), GetColor(0x81A1C1FF), GetColor(0x88C0D0FF)));

    return addedButton;
}

void GraphUI::createNavbarToggle() {
    ButtonController openToggle, closedToggle;
    Vector2 toggleSize = {70.0f, 40.0f};
    Vector2 startPos = {0.0f, 60.0f};

    auto createButton = [&] (std::string suffix, std::string text, Font font, Color backgroundColor, Color outlineColor, Color textColor) -> Button {
        return Helper::createButton(
            Helper::createRectangle(
                getDSName() + "_" + Helper::rectangleStringBuffer(std::string(NavigationBar::TOGGLE_KEY) + suffix),
                startPos, startPos + toggleSize,
                3.0f, 
                backgroundColor, outlineColor,
                1
            ),

            Helper::createText(text, font, 35.0f, 3.5f, {0, 0}, textColor)
        );
    }; 

    openToggle.setButtonSettings(
        ButtonState::ACTIVE,
        createButton("_active", "<<", CoreFonts::Aptos, GetColor(0), GetColor(0), WHITE)
    );
    openToggle.setButtonSettings(
        ButtonState::HOVERED,
        createButton("_hovered", "<<", CoreFonts::AptosBold, GetColor(0), GetColor(0x81A1C1FF), GetColor(0xECEFF4FF))
    );

    closedToggle.setButtonSettings(
        ButtonState::ACTIVE,
        createButton("_active", ">>", CoreFonts::Aptos, GetColor(0), GetColor(0), WHITE)
    );
    closedToggle.setButtonSettings(
        ButtonState::HOVERED,
        createButton("_hovered", ">>", CoreFonts::AptosBold, GetColor(0), GetColor(0x81A1C1FF), GetColor(0xECEFF4FF))
    );


    for (auto& it : navbarMap) {
        if (it.first == NavPhase::NAV_CLOSED) {
            it.second.setButtonController(NavigationBar::TOGGLE_KEY, closedToggle);
        } else {
            it.second.setButtonController(NavigationBar::TOGGLE_KEY, openToggle);
        }
    }
}

///////////////////////
///     FIELD       ///
///////////////////////
/// @brief Creating input fields

void GraphUI::createField() {
    for (int i = 0; i < MAX_TEXTBOX; ++i) {
        fieldTextbox[i] = Helper::createTextbox(
            Helper::createRectangle(
                getDSName() + "_" + Helper::rectangleStringBuffer("Field_Textbox_" + std::to_string(i)),
                Vector2 {0.0f, 0.0f}, Vector2{FIELD_TEXTBOX_WIDTH, FIELD_TEXTBOX_HEIGHT}, 
                FIELD_TEXTBOX_OUTLINE,
                GetColor(0x3B4252FF), BLACK, 1
            ),
            Helper::createText("", GetFontDefault(), 20.0f, 5.0f, {0, 0}, WHITE)
        );

        edgeHolderSize[i] = MeasureTextEx(CoreFonts::CascadiaMonoRegular, edgeHolder[i].c_str(), FIELD_HOLDER_FONT_SIZE, 0.1f * FIELD_HOLDER_FONT_SIZE);
    }
    

    auto createButton = [&] (std::string buttonID, std::string text, Color bgColor, Font font) -> Button {
        return Helper::createButton(
            Helper::createRectangle(
                getDSName() + "_" + Helper::rectangleStringBuffer(buttonID),
                Vector2 {0.0f, 0.0f}, Vector2{FIELD_BUTTON_WIDTH, FIELD_BUTTON_HEIGHT}, 
                2.0f, 
                bgColor, BLACK,
                1
            ),
            
            Helper::createText(text, font, FIELD_BUTTON_FONT_SIZE, 0.1f * FIELD_BUTTON_FONT_SIZE, {0, 0}, BLACK)
        );
    };

    fieldRandom.setButtonSettings(ButtonState::ACTIVE, createButton("Field_Random", "Random", GetColor(0xC9AE8AFF), CoreFonts::Aptos));
    fieldRandom.setButtonSettings(ButtonState::HOVERED, createButton("Field_Random_hovered", "Random", GetColor(0x81A1C1FF), CoreFonts::AptosBold));

    fieldSubmit.setButtonSettings(ButtonState::ACTIVE, createButton("Field_Submit", "Submit", GetColor(0xC9AE8AFF), CoreFonts::Aptos));
    fieldSubmit.setButtonSettings(ButtonState::HOVERED, createButton("Field_Submit_hovered", "Submit", GetColor(0x81A1C1FF), CoreFonts::AptosBold));
}


////////////////////////////////////
///     SUPPORTIVE FUNCTIONS     ///
////////////////////////////////////

CommandPattern GraphUI::navbarListenerRequest(RawInputEvent nextInput) {
    std::string signal = navbarMap[navPhase].processInput(nextInput);
    
    if (signal == "Clear") {
        navPhase = NavPhase::NAV_CLOSED;
        operationPlaceholder = "";
        changeField();
        return CommandPattern{"MODIFY", "INTERACT", getDSName(), "CLEAR", {}};
    }

    if (find(DSList.begin(), DSList.end(), signal) != DSList.end()) {
        navPhase = NavPhase::NAV_CLOSED;
        operationPlaceholder = "";
        changeField();
        return CommandPattern{"NAVIGATE", "DS_SWITCH", signal, "", {}};
    }

    if (signal == "Kruskal") {
        navPhase = NavPhase::NAV_CLOSED;
        operationPlaceholder = "";
        changeField();
        
        return CommandPattern{"QUERY", "INTERACT", getDSName(), "KRUSKAL", {}};
    }

    return CommandPattern();
}

/// @brief Navbar Interactions
void GraphUI::processInputNavbar(RawInputEvent nextInput) {
    std::string signal = navbarMap[navPhase].processInput(nextInput);
    if (signal.empty()) return;
    
    if (signal == "TOGGLE") {
        // When the toggle button is triggered, the navigation bar will change its status.
        // And the input buffer will be cleared just to assure emptyness.

        if (navPhase == NavPhase::NAV_DS_OPTIONS) navPhase = NavPhase::NAV_OPTIONS;
        else if (navPhase == NavPhase::NAV_OPTIONS) navPhase = NavPhase::NAV_CLOSED;
        else if (navPhase == NavPhase::NAV_CLOSED) navPhase = NavPhase::NAV_OPTIONS;
        else if (navPhase == NavPhase::NAV_OPERATIONS) navPhase = NavPhase::NAV_OPTIONS;

        operationPlaceholder = "";
        changeField();
        return;
    }
    
    if (signal == "Main Menu") {
        // Return to the main menu

        return;
    }

    if (signal == "Data Structures") {
        // View the Data Structure list

        navPhase = NavPhase::NAV_DS_OPTIONS;
        return;
    }

    if (signal == "Operations") {
        // View the Operation list of the current DS

        navPhase = NavPhase::NAV_OPERATIONS;
        return;
    }
    
    if (signal != operationPlaceholder) {
        // DS Operation chosen
        
        operationPlaceholder = signal;
        changeField();
    }
}

 
/// @brief Field Interaction Change
void GraphUI::changeField() {
    for (int i = 0; i < MAX_TEXTBOX; ++i) {
        fieldTextbox[i].clearLabelBuffer();
    }

    textboxFocusID = -1;
}

/// @brief Field Input
void GraphUI::processInputField(RawInputEvent nextInput) {
    if (operationPlaceholder.empty()) return;
    
    ButtonController* targetController = navbarMap[navPhase].getButtonController(operationPlaceholder);
    ShapeState targetBackground = targetController -> getButtonShape().getBackground();
    Vector2 rootPos = {FIELD_GAP_X + BUTTON_WIDTH, targetBackground.startPosition.y};

    bool randomClick = nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED;
    if (operationPlaceholder == "Insert Edge" || operationPlaceholder == "Remove Edge" || operationPlaceholder == "Dijkstra") {
        randomClick &= fieldRandom.getButtonShape().contains(nextInput.position - rootPos - Vector2{0.0f, FIELD_GAP_Y + FIELD_TEXTBOX_HEIGHT});
    } else if (operationPlaceholder == "Remove Node") {
        randomClick &= fieldRandom.getButtonShape().contains(nextInput.position - rootPos - Vector2{FIELD_GAP_X + FIELD_TEXTBOX_WIDTH, 0.0f});
    } else {
        randomClick = false;
    }

    // When the random request is required, set a random value from 0 to 99999 for the input box
    if (randomClick) {
        if (textboxFocusID != -1)
            fieldTextbox[textboxFocusID].setLabelBuffer(std::to_string(Helper::random_gen(textboxFocusID < 2, textboxFocusID >= 2 ? 99 : 10)));
        return;
    }
    
    if (nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED) {
        textboxFocusID = -1;

        for (int i = 0; i < activeFieldCount(); ++i) {
            Vector2 nextPos = rootPos + Vector2{i * (FIELD_TEXTBOX_WIDTH + FIELD_GAP_X), 0.0f};
            if (fieldTextbox[i].contains(nextInput.position - nextPos)) {
                textboxFocusID = i;
                break;
            }
        }
    } else if (nextInput.inputType == RawInputEvent::InputType::RIGHT_MOUSE_CLICKED) {
        textboxFocusID = -1;
    } else if (nextInput.inputType == RawInputEvent::InputType::KEY_PRESSED
            && nextInput.keySignal == KeyboardKey::KEY_TAB) {
        
        if (textboxFocusID != -1) textboxFocusID = (textboxFocusID + 1) % activeFieldCount();
    }

    if (textboxFocusID == -1 || nextInput.inputType != RawInputEvent::InputType::KEY_PRESSED) return;

    // If the field is being focused, we crawl the key input
    // If that key is a number and the string hasn't reached its limit, insert that key to the buffer.
    // If KEY_BACKSPACE is triggered, remove the last character in the buffer.
    int keyID = (int) nextInput.keySignal;

    std::string inputStr = fieldTextbox[textboxFocusID].getLabelBuffer();
    if (keyID >= '0' && keyID <= '9' && (int) inputStr.length() < TEXTBOX_LENGTH_LIMIT) {
        inputStr.push_back(keyID);
    }
        
    if (keyID == (int) KeyboardKey::KEY_BACKSPACE && !inputStr.empty()) {
        inputStr.pop_back();
    }

    fieldTextbox[textboxFocusID].setLabelBuffer(inputStr);
}

/// @brief Responds requests to Data Structure  
CommandPattern GraphUI::fieldListenerRequest(RawInputEvent nextInput) {
    if (operationPlaceholder.empty()) return CommandPattern();

    ButtonController* targetController = navbarMap[navPhase].getButtonController(operationPlaceholder);
    ShapeState targetBackground = targetController -> getButtonShape().getBackground();
    Vector2 rootPos = {FIELD_GAP_X + BUTTON_WIDTH, targetBackground.startPosition.y};
    
    bool submitEnter = nextInput.inputType == RawInputEvent::InputType::KEY_PRESSED
                    && nextInput.keySignal == KeyboardKey::KEY_ENTER;
    bool submitClick = nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED;
    if (operationPlaceholder == "Insert Edge" || operationPlaceholder == "Remove Edge" || operationPlaceholder == "Dijkstra") {
        submitClick &= fieldRandom.getButtonShape().contains(nextInput.position - rootPos - Vector2{FIELD_GAP_X + FIELD_BUTTON_WIDTH, FIELD_GAP_Y + FIELD_TEXTBOX_HEIGHT});
    } else if (operationPlaceholder == "Remove Node") {
        submitClick &= fieldRandom.getButtonShape().contains(nextInput.position - rootPos - Vector2{FIELD_GAP_X * 2 + FIELD_BUTTON_WIDTH + FIELD_TEXTBOX_WIDTH, 0.0f});
    } else {
        submitClick = false;
    }
    
    if (!submitEnter && !submitClick) return CommandPattern();
    
    // If the Submit button is triggered, the value from the input field is crawled
    // After that, the input buffer will be removed, and the navigation sidebar will close.
    // Solving edge cases: when the input is empty

    std::vector <std::string> rawValue;
    for (int i = 0; i < activeFieldCount(); ++i) {
        rawValue.push_back(fieldTextbox[i].getLabelBuffer());

        if (rawValue[i].empty()) rawValue[i] = "0";
    }
    std::string chosenOperation = operationPlaceholder;
    
    operationPlaceholder = "";
    navPhase = NavPhase::NAV_CLOSED;
    changeField();

    if (chosenOperation.find("Insert") != std::string::npos || chosenOperation.find("Remove") != std::string::npos)
        return CommandPattern {
            "MODIFY",
            "INTERACT",
            getDSName(),
            Helper::upperString(chosenOperation),
            rawValue
        };
    else 
        return CommandPattern {
            "QUERY",
            "INTERACT",
            getDSName(),
            Helper::upperString(chosenOperation),
            rawValue
        };
}