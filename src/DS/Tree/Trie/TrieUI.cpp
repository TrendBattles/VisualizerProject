#include <DS/Tree/Trie/TrieUI.hpp>
#include <Graphics/Helper.hpp>

TrieUI::TrieUI() {
    createNavbar();
    
    createField();
}

///////////////////////////
///     UI OPTIONS      ///
///////////////////////////

/// @brief Input processing 
CommandPattern TrieUI::processInput(RawInputEvent nextInput) {
    CommandPattern navbarSignal = navbarListenerRequest(nextInput);
    if (!navbarSignal.prefix.empty()) return navbarSignal;
    
    CommandPattern fieldSignal = fieldListenerRequest(nextInput);
    if (!fieldSignal.prefix.empty()) return fieldSignal;

    updateNavbar(nextInput);
    updateField(nextInput);

    return CommandPattern();
}

void TrieUI::disableOption(const std::string& optionName) {
    for (auto& it : navbarMap) {
        ButtonController* targetController = it.second.getButtonController(optionName);
        if (targetController != nullptr) {
            targetController -> setButtonState(ButtonState::DISABLED);
        }
    }
}
void TrieUI::enableOption(const std::string& optionName) {
    for (auto& it : navbarMap) {
        ButtonController* targetController = it.second.getButtonController(optionName);
        if (targetController != nullptr) {
            targetController -> setButtonState(ButtonState::ACTIVE);
        }
    }
}
void TrieUI::disableAllOperations() {
    for (const std::string& buttonID : operationList) {
        ButtonController* targetController = navbarMap[NavPhase::NAV_OPERATIONS].getButtonController(buttonID);
        
        if (targetController != nullptr) {
            targetController -> setButtonState(ButtonState::DISABLED);
        }
    }
}
void TrieUI::enableAllOperations() {
    for (const std::string& buttonID : operationList) {
        ButtonController* targetController = navbarMap[NavPhase::NAV_OPERATIONS].getButtonController(buttonID);
        if (targetController != nullptr) {
            targetController -> setButtonState(ButtonState::ACTIVE);
        }
    }
}

/////////////////////////
///     UI DESIGN     ///
/////////////////////////

/// @brief UI updates before rendering
void TrieUI::update() {
    if (!operationPlaceholder.empty()) {
        ButtonController* targetController = navbarMap[navPhase].getButtonController(operationPlaceholder);
        ShapeState targetBackground = targetController -> getButtonShape().getBackground();
        Vector2 rootPos = {20.0f + BUTTON_WIDTH, targetBackground.startPosition.y};

        if (fieldRandom.getButtonShape().contains(GetMousePosition() - rootPos)) {
            fieldRandom.setButtonState(ButtonState::HOVERED);
        } else {
            fieldRandom.setButtonState(ButtonState::ACTIVE);
        }

        if (fieldSubmit.getButtonShape().contains(GetMousePosition() - rootPos)) {
            fieldSubmit.setButtonState(ButtonState::HOVERED);
        } else {
            fieldSubmit.setButtonState(ButtonState::ACTIVE);
        }

        if (fieldTextbox.contains(GetMousePosition() - rootPos)) {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
        } else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }
    }
    

    NavigationBar& navbar = navbarMap[navPhase];
    navbar.hoverButtonTrigger();
}
/// @brief UI rendering
void TrieUI::render() {
    if (navPhase != NavPhase::NAV_CLOSED) {
        DrawRectangle(BUTTON_WIDTH, 0, GetScreenWidth() - BUTTON_WIDTH, GetScreenHeight(), Fade(GetColor(0x2E3440FF), 0.5f));
    }
    uiManager -> renderSnapshot(navbarMap[navPhase].getNavigationSnapshot());

    if (operationPlaceholder.empty()) {
        return;
    }
    
    ButtonController* targetController = navbarMap[navPhase].getButtonController(operationPlaceholder);
    ShapeState targetBackground = targetController -> getButtonShape().getBackground();
    Vector2 rootPos = {20.0f + BUTTON_WIDTH, targetBackground.startPosition.y};

    Textbox tempTextbox = fieldTextbox;
    Button tempRandom = fieldRandom.getButtonShape();
    Button tempSubmit = fieldSubmit.getButtonShape();
    
    tempTextbox.positionTransitAllBy(rootPos);
    tempRandom.positionTransitAllBy(rootPos);
    tempSubmit.positionTransitAllBy(rootPos);

    drawTextbox(tempTextbox);
    drawButton(tempRandom);
    drawButton(tempSubmit);
}
std::string TrieUI::getDSName() const { return "Trie"; }

///////////////////////////////
///     INITIALIZATION      ///
///////////////////////////////

/// @brief Creating the nagivation sidebar
void TrieUI::createNavbar() {
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

NavigationBar TrieUI::createNavBar(const std::vector <std::string>& buttonList) {
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

ButtonController TrieUI::createNavbarButtons(const std::string& buttonID, float x, float y) {
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

void TrieUI::createNavbarToggle() {
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
        createButton("_active", "<", CoreFonts::Aptos, GetColor(0), GetColor(0), WHITE)
    );
    openToggle.setButtonSettings(
        ButtonState::HOVERED,
        createButton("_hovered", "<", CoreFonts::AptosBold, GetColor(0), GetColor(0x81A1C1FF), GetColor(0xECEFF4FF))
    );

    closedToggle.setButtonSettings(
        ButtonState::ACTIVE,
        createButton("_active", ">", CoreFonts::Aptos, GetColor(0), GetColor(0), WHITE)
    );
    closedToggle.setButtonSettings(
        ButtonState::HOVERED,
        createButton("_hovered", ">", CoreFonts::AptosBold, GetColor(0), GetColor(0x81A1C1FF), GetColor(0xECEFF4FF))
    );


    for (auto& it : navbarMap) {
        if (it.first == NavPhase::NAV_CLOSED) {
            it.second.setButtonController(NavigationBar::TOGGLE_KEY, closedToggle);
        } else {
            it.second.setButtonController(NavigationBar::TOGGLE_KEY, openToggle);
        }
    }
}

/// @brief Field Inputs
void TrieUI::createField() {
    Vector2 nextPos = {0.0f, 0.0f};

    fieldTextbox = Helper::createTextbox(
        Helper::createRectangle(
            getDSName() + "_" + Helper::rectangleStringBuffer("Field_Textbox"),
            nextPos, nextPos + Vector2{FIELD_TEXTBOX_WIDTH, FIELD_TEXTBOX_HEIGHT}, 
            2.0f, 
            GetColor(0x3B4252FF), BLACK,
            1
        ),
        Helper::createText("", GetFontDefault(), 20.0f, 5.0f, {0, 0}, WHITE)
    );

    auto createButton = [&] (std::string buttonID, std::string text, Color bgColor, Font font) -> Button {
        return Helper::createButton(
            Helper::createRectangle(
                getDSName() + "_" + Helper::rectangleStringBuffer(buttonID),
                nextPos, nextPos + Vector2{FIELD_SUBMIT_WIDTH, FIELD_SUBMIT_HEIGHT}, 
                2.0f, 
                bgColor, BLACK,
                1
            ),
            
            Helper::createText(text, font, 25.0f, 2.5f, {0, 0}, BLACK)
        );
    };

    nextPos += Vector2{FIELD_TEXTBOX_WIDTH + FIELD_GAP, 0};

    fieldRandom.setButtonSettings(ButtonState::ACTIVE, createButton("Field_Random", "Random", GetColor(0xC9AE8AFF), CoreFonts::Aptos));
    fieldRandom.setButtonSettings(ButtonState::HOVERED, createButton("Field_Random_hovered", "Random", GetColor(0x81A1C1FF), CoreFonts::AptosBold));

    nextPos += Vector2{FIELD_SUBMIT_WIDTH + FIELD_GAP, 0};

    fieldSubmit.setButtonSettings(ButtonState::ACTIVE, createButton("Field_Submit", "Submit", GetColor(0xC9AE8AFF), CoreFonts::Aptos));
    fieldSubmit.setButtonSettings(ButtonState::HOVERED, createButton("Field_Submit_hovered", "Submit", GetColor(0x81A1C1FF), CoreFonts::AptosBold));
}


////////////////////////////////////
///     SUPPORTIVE FUNCTIONS     ///
////////////////////////////////////

CommandPattern TrieUI::navbarListenerRequest(RawInputEvent nextInput) {
    std::string signal = navbarMap[navPhase].processInput(nextInput);
    if (signal == "Clear") {
        navPhase = NavPhase::NAV_CLOSED;
        operationPlaceholder = "";
        changeField();
        return CommandPattern{"MODIFY", "INTERACT", getDSName(), "CLEAR", ""};
    }

    if (find(DSList.begin(), DSList.end(), signal) != DSList.end()) {
        navPhase = NavPhase::NAV_CLOSED;
        operationPlaceholder = "";
        changeField();
        return CommandPattern{"NAVIGATE", "DS_SWITCH", signal, "", ""};
    }

    return CommandPattern();
}

/// @brief Navbar Interactions
void TrieUI::updateNavbar(RawInputEvent nextInput) {
    std::string signal = navbarMap[navPhase].processInput(nextInput);
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

    if (!signal.empty() && signal != operationPlaceholder) {
        // DS Operation chosen
        
        operationPlaceholder = signal;
        changeField();
    }
}

 
/// @brief Field Interaction Change
void TrieUI::changeField() {
    fieldTextbox.clearLabelBuffer();
    isFieldTextboxFocused = false;
}

/// @brief Field Input
void TrieUI::updateField(RawInputEvent nextInput) {
    if (operationPlaceholder.empty()) return;
    
    ButtonController* targetController = navbarMap[navPhase].getButtonController(operationPlaceholder);
    ShapeState targetBackground = targetController -> getButtonShape().getBackground();
    Vector2 rootPos = {20.0f + BUTTON_WIDTH, targetBackground.startPosition.y};

    if (nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED) {
        isFieldTextboxFocused = fieldTextbox.contains(nextInput.position - rootPos);
    } else if (nextInput.inputType == RawInputEvent::InputType::RIGHT_MOUSE_CLICKED) {
        isFieldTextboxFocused = false;
    }

    if (!isFieldTextboxFocused || nextInput.inputType != RawInputEvent::InputType::KEY_PRESSED) return;

    // If the field is being focused, we crawl the key input
    // If that key is a number and the string hasn't reached its limit, insert that key to the buffer.
    // If KEY_BACKSPACE is triggered, remove the last character in the buffer.
    int keyID = (int) nextInput.keySignal;

    std::string inputStr = fieldTextbox.getLabelBuffer();
    if (keyID >= '0' && keyID <= '9' && (int) inputStr.length() < TEXTBOX_LENGTH_LIMIT) {
        inputStr.push_back(keyID);
    }
        
    if (keyID == (int) KeyboardKey::KEY_BACKSPACE && !inputStr.empty()) {
        inputStr.pop_back();
    }

    fieldTextbox.setLabelBuffer(inputStr);
}

/// @brief Responds requests to Data Structure  
CommandPattern TrieUI::fieldListenerRequest(RawInputEvent nextInput) {
    if (operationPlaceholder.empty()) return CommandPattern();

    ButtonController* targetController = navbarMap[navPhase].getButtonController(operationPlaceholder);
    ShapeState targetBackground = targetController -> getButtonShape().getBackground();
    Vector2 rootPos = {20.0f + BUTTON_WIDTH, targetBackground.startPosition.y};

    bool submitEnter = nextInput.inputType == RawInputEvent::InputType::KEY_PRESSED
                    && nextInput.keySignal == KeyboardKey::KEY_ENTER;
    bool submitClick = nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED
                    && fieldSubmit.getButtonShape().contains(nextInput.position - rootPos);
    bool randomClick = nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED
                    && fieldRandom.getButtonShape().contains(nextInput.position - rootPos);

    // When the random request is required, set a random value from 0 to 9999 for the input box
    if (randomClick) {
        int randLength = Helper::random_gen(1, TEXTBOX_LENGTH_LIMIT);
        std::string str = "";
        for (int i = 0; i < randLength; ++i) {
            str.push_back(Helper::random_gen('0', '9'));
        }

        fieldTextbox.setLabelBuffer(str);
        return CommandPattern();
    }
    if (!submitEnter && !submitClick) return CommandPattern();
    
    // If the Submit button is triggered, the value from the input field is crawled
    // After that, the input buffer will be removed, and the navigation sidebar will close.
    // Solving edge cases: when the input is empty
    std::string rawValue = fieldTextbox.getLabelBuffer();
    std::string chosenOperation = operationPlaceholder;

    operationPlaceholder = "";
    navPhase = NavPhase::NAV_CLOSED;
    changeField();

    if (rawValue.empty()) return CommandPattern();

    return CommandPattern {
        Helper::upperString(chosenOperation) == "SEARCH" ? "QUERY" : "MODIFY",
        "INTERACT",
        getDSName(),
        Helper::upperString(chosenOperation),
        rawValue
    };
}