#include <DS/HashTable/HashTableUI.hpp>
#include <Graphics/Helper.hpp>

HashTableUI::HashTableUI() {
    createNavbar();
    
    createField();
}

///////////////////////////
///     UI OPTIONS      ///
///////////////////////////

/// @brief Input processing 
CommandPattern HashTableUI::processInput(RawInputEvent nextInput) {
    CommandPattern navbarSignal = navbarListenerRequest(nextInput);
    if (!navbarSignal.prefix.empty()) return navbarSignal;
    
    CommandPattern fieldSignal = fieldListenerRequest(nextInput);
    if (!fieldSignal.prefix.empty()) return fieldSignal;

    processInputNavbar(nextInput);
    processInputField(nextInput);

    return CommandPattern();
}

void HashTableUI::disableOption(const std::string& optionName) {
    for (auto& it : navbarMap) {
        ButtonController* targetController = it.second.getButtonController(optionName);
        if (targetController != nullptr) {
            targetController -> setButtonState(ButtonState::DISABLED);
        }
    }
}
void HashTableUI::enableOption(const std::string& optionName) {
    for (auto& it : navbarMap) {
        ButtonController* targetController = it.second.getButtonController(optionName);
        if (targetController != nullptr) {
            targetController -> setButtonState(ButtonState::ACTIVE);
        }
    }
}
void HashTableUI::disableAllOperations() {
    for (const std::string& buttonID : operationList) {
        ButtonController* targetController = navbarMap[NavPhase::NAV_OPERATIONS].getButtonController(buttonID);
        
        if (targetController != nullptr) {
            targetController -> setButtonState(ButtonState::DISABLED);
        }
    }
}
void HashTableUI::enableAllOperations() {
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
bool HashTableUI::exceedsTextboxInitRange() {
    Vector2 textboxSize = Vector2{FIELD_BUTTON_WIDTH * 2 + FIELD_GAP_X, 300.0f};
    Vector2 alignOffset = Vector2{10.0f, 10.0f};
    Vector2 position = alignOffset;

    std::stringstream ss(initBuffer);
    std::string word = "", tempBuffer = "";

    while (ss >> word) {
        std::string nextBuffer = tempBuffer + (!tempBuffer.empty() ? ", " : "") + word;

        Vector2 size = MeasureTextEx(CoreFonts::Aptos, nextBuffer.c_str(), 20.0f, 2.0f);
        if (size.x + alignOffset.x <= textboxSize.x - alignOffset.x) {
            tempBuffer = nextBuffer;
            continue;
        }

        position += Vector2{0.0f, 20.0f};
        tempBuffer = word;

        if (position.y + 20.0f > textboxSize.y - alignOffset.y) {
            return true;
        }
    }
        
    if (tempBuffer.empty() || position.y + 20.0f <= textboxSize.y - alignOffset.y) {
        return false;
    }
    return true;
}
void HashTableUI::updateInit(Vector2 rootPos) {
    if (fieldFile.getButtonShape().contains(GetMousePosition() - rootPos)) {
        fieldFile.setButtonState(ButtonState::HOVERED);
    } else {
        fieldFile.setButtonState(ButtonState::ACTIVE);
    }

    rootPos += Vector2{FIELD_BUTTON_WIDTH + FIELD_GAP_X, 0.0f};

    if (fieldRandom.getButtonShape().contains(GetMousePosition() - rootPos)) {
        fieldRandom.setButtonState(ButtonState::HOVERED);
    } else {
        fieldRandom.setButtonState(ButtonState::ACTIVE);
    }

    rootPos += Vector2{FIELD_BUTTON_WIDTH + FIELD_GAP_X, 0.0f};

    if (fieldSubmit.getButtonShape().contains(GetMousePosition() - rootPos)) {
        fieldSubmit.setButtonState(ButtonState::HOVERED);
    } else {
        fieldSubmit.setButtonState(ButtonState::ACTIVE);
    }
}
void HashTableUI::updateOthers(Vector2 rootPos) {
    if (fieldTextbox.contains(GetMousePosition() - rootPos)) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    } else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }

    rootPos += Vector2{FIELD_TEXTBOX_WIDTH + FIELD_GAP_X, 0.0f};

    if (fieldRandom.getButtonShape().contains(GetMousePosition() - rootPos)) {
        fieldRandom.setButtonState(ButtonState::HOVERED);
    } else {
        fieldRandom.setButtonState(ButtonState::ACTIVE);
    }

    rootPos += Vector2{FIELD_BUTTON_WIDTH + FIELD_GAP_X, 0.0f};

    if (fieldSubmit.getButtonShape().contains(GetMousePosition() - rootPos)) {
        fieldSubmit.setButtonState(ButtonState::HOVERED);
    } else {
        fieldSubmit.setButtonState(ButtonState::ACTIVE);
    }
}
void HashTableUI::update() {
    NavigationBar& navbar = navbarMap[navPhase];
    navbar.hoverButtonTrigger();

    if (operationPlaceholder.empty()) {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        return;
    }

    ButtonController* targetController = navbarMap[navPhase].getButtonController(operationPlaceholder);
    ShapeState targetBackground = targetController -> getButtonShape().getBackground();
    Vector2 rootPos = {FIELD_GAP_X + BUTTON_WIDTH, targetBackground.startPosition.y};

    if (operationPlaceholder == "Init") {
        updateInit(rootPos);
        return;
    }
    
    updateOthers(rootPos);
}

/////////////////////////
///     UI RENDER     ///
/////////////////////////
void HashTableUI::renderInit(Vector2 rootPos) {
    Vector2 textboxPos = rootPos + Vector2{0.0f, FIELD_GAP_Y + FIELD_BUTTON_HEIGHT};
    Vector2 textboxSize = Vector2{FIELD_BUTTON_WIDTH * 2 + FIELD_GAP_X, 300.0f};
    DrawRectangleV(textboxPos, textboxSize, Fade(BLACK, 0.3f));
    DrawRectangleLinesEx(
        Rectangle { textboxPos.x, textboxPos.y, textboxSize.x, textboxSize.y },
        2.0f,
        GetColor(0x374151FF)
    );
    {
        Vector2 alignOffset = Vector2{10.0f, 10.0f};
        Vector2 position = alignOffset;

        std::stringstream ss(initBuffer);
        std::string word = "", tempBuffer = "";

        while (ss >> word) {
            std::string nextBuffer = tempBuffer + (!tempBuffer.empty() ? ", " : "") + word;

            Vector2 size = MeasureTextEx(CoreFonts::Aptos, nextBuffer.c_str(), 20.0f, 2.0f);
            if (size.x + alignOffset.x <= textboxSize.x - alignOffset.x) {
                tempBuffer = nextBuffer;
                continue;
            }

            tempBuffer += ",";
            DrawTextEx(CoreFonts::Aptos, tempBuffer.c_str(), position + textboxPos, 20.0f, 2.0f, WHITE);
            position += Vector2{0.0f, 20.0f};
            tempBuffer = word;
        }
        
        if (!tempBuffer.empty()) {
            if (initBuffer.back() == ' ') tempBuffer += ", ";
            DrawTextEx(CoreFonts::Aptos, tempBuffer.c_str(), position + textboxPos, 20.0f, 2.0f, WHITE);
        }
    }
    

    Button tempFile = fieldFile.getButtonShape();
    Button tempRandom = fieldRandom.getButtonShape();
    Button tempSubmit = fieldSubmit.getButtonShape();

    tempFile.positionTransitAllBy(rootPos);
    rootPos += Vector2{FIELD_BUTTON_WIDTH + FIELD_GAP_X, 0.0f};
    tempRandom.positionTransitAllBy(rootPos);
    rootPos += Vector2{FIELD_BUTTON_WIDTH + FIELD_GAP_X, 0.0f};
    tempSubmit.positionTransitAllBy(rootPos);
    
    drawButton(tempFile);
    drawButton(tempRandom);
    drawButton(tempSubmit);
}
void HashTableUI::renderOthers(Vector2 rootPos) {
    Textbox tempTextbox = fieldTextbox;
    Button tempRandom = fieldRandom.getButtonShape();
    Button tempSubmit = fieldSubmit.getButtonShape();
    
    tempTextbox.positionTransitAllBy(rootPos);
    rootPos += Vector2{FIELD_TEXTBOX_WIDTH + FIELD_GAP_X, 0.0f};

    tempRandom.positionTransitAllBy(rootPos);
    rootPos += Vector2{FIELD_BUTTON_WIDTH + FIELD_GAP_X, 0.0f};

    tempSubmit.positionTransitAllBy(rootPos);
    
    drawTextbox(tempTextbox);
    drawButton(tempRandom);
    drawButton(tempSubmit);
}
void HashTableUI::render() {
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

    if (operationPlaceholder == "Init") {
        renderInit(rootPos);
        return;
    }

    renderOthers(rootPos);
}
std::string HashTableUI::getDSName() const { return "Hash_Table"; }

///////////////////////////////
///     INITIALIZATION      ///
///////////////////////////////

/// @brief Creating the nagivation sidebar
void HashTableUI::createNavbar() {
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

NavigationBar HashTableUI::createNavBar(const std::vector <std::string>& buttonList) {
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

ButtonController HashTableUI::createNavbarButtons(const std::string& buttonID, float x, float y) {
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

void HashTableUI::createNavbarToggle() {
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

/// @brief Field Inputs
void HashTableUI::createField() {
    fieldTextbox = Helper::createTextbox(
        Helper::createRectangle(
            getDSName() + "_" + Helper::rectangleStringBuffer("Field_Textbox"),
            Vector2{0.0f, 0.0f}, Vector2{FIELD_TEXTBOX_WIDTH, FIELD_TEXTBOX_HEIGHT}, 
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
                Vector2{0.0f, 0.0f}, Vector2{FIELD_BUTTON_WIDTH, FIELD_BUTTON_HEIGHT}, 
                2.0f, 
                bgColor, BLACK,
                1
            ),
            
            Helper::createText(text, font, 25.0f, 2.5f, {0, 0}, BLACK)
        );
    };

    fieldRandom.setButtonSettings(ButtonState::ACTIVE, createButton("Field_Random", "Random", GetColor(0xC9AE8AFF), CoreFonts::Aptos));
    fieldRandom.setButtonSettings(ButtonState::HOVERED, createButton("Field_Random_hovered", "Random", GetColor(0x81A1C1FF), CoreFonts::AptosBold));

    fieldSubmit.setButtonSettings(ButtonState::ACTIVE, createButton("Field_Submit", "Submit", GetColor(0xC9AE8AFF), CoreFonts::Aptos));
    fieldSubmit.setButtonSettings(ButtonState::HOVERED, createButton("Field_Submit_hovered", "Submit", GetColor(0x81A1C1FF), CoreFonts::AptosBold));

    fieldFile.setButtonSettings(ButtonState::ACTIVE, createButton("Field_File", "File", GetColor(0xC9AE8AFF), CoreFonts::Aptos));
    fieldFile.setButtonSettings(ButtonState::HOVERED, createButton("Field_File_hovered", "File", GetColor(0x81A1C1FF), CoreFonts::AptosBold));
}


////////////////////////////////////
///     SUPPORTIVE FUNCTIONS     ///
////////////////////////////////////

CommandPattern HashTableUI::navbarListenerRequest(RawInputEvent nextInput) {
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

    return CommandPattern();
}

/// @brief Navbar Interactions
void HashTableUI::processInputNavbar(RawInputEvent nextInput) {
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
void HashTableUI::changeField() {
    initBuffer.clear();
    fieldTextbox.clearLabelBuffer();
}

///////////////////////////
///     FIELD INPUT     ///
///////////////////////////
void HashTableUI::handleInputFieldInit() {
    std::string inputStr = initBuffer;

    initBuffer = "";
    std::stringstream ss(inputStr);
    std::string number;
    while (ss >> number) {
        int value = 0;
        try {
            value = std::stoi(number);
        } catch (const std::invalid_argument& ia) {
            continue;
        } catch (const std::out_of_range& oor) {
            continue;
        }

        if (value > 9999) {
            continue;
        }

        if (!initBuffer.empty()) initBuffer += " ";
        initBuffer += std::to_string(value);
    }

    while (exceedsTextboxInitRange()) {
        if (initBuffer.back() == ' ') {
            while (!initBuffer.empty() && initBuffer.back() == ' ') initBuffer.pop_back();
        } else {
            while (!initBuffer.empty() && isdigit(initBuffer.back())) initBuffer.pop_back();
            while (!initBuffer.empty() && initBuffer.back() == ' ') initBuffer.pop_back();
        }
    }
}
void HashTableUI::processInputFieldInit(RawInputEvent nextInput) {
    ButtonController* targetController = navbarMap[navPhase].getButtonController(operationPlaceholder);
    ShapeState targetBackground = targetController -> getButtonShape().getBackground();
    Vector2 rootPos = {FIELD_GAP_X + BUTTON_WIDTH, targetBackground.startPosition.y};

    bool fileClick = nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED
                    && fieldFile.getButtonShape().contains(nextInput.position - rootPos);
    rootPos += Vector2{FIELD_BUTTON_WIDTH + FIELD_GAP_X, 0.0f};
    bool randomClick = nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED
                    && fieldRandom.getButtonShape().contains(nextInput.position - rootPos);

    if (fileClick) {
        // Define the parameters
        const char* title = "Select Input File";
        const char* defaultPath = "";
        const char* filterPatterns[1] = { "*.txt" }; // Only show text files
        const char* filterDescription = "Text Files (.txt)";

        // Open the dialog
        // This function halts execution until the user picks a file or cancels
        const char* selectedPath = tinyfd_openFileDialog(
            title,
            defaultPath,
            1,               // Number of filter patterns
            filterPatterns,
            filterDescription,
            0                // 0 = single file, 1 = multiple files
        );

        if (selectedPath == nullptr) return;

        std::ifstream input(selectedPath);
        if (!input.is_open()) return;

        std::string word;
        initBuffer = "";
        while (input >> word) {
            bool allDigits = true;
            for (char c : word) allDigits &= isdigit(c);

            if (!allDigits) continue;
            if (!initBuffer.empty()) initBuffer += " ";
            initBuffer += word;
        }

        handleInputFieldInit();
        return;
    }    
    
    if (randomClick) {
        int N = Helper::random_gen(1, 20);
        initBuffer.clear();
        for (int i = 1; i <= N; ++i) {
            if (!initBuffer.empty()) initBuffer += " ";
            
            initBuffer += std::to_string(Helper::random_gen(0, 9999));
        }

        return;
    }

    if (nextInput.inputType != RawInputEvent::InputType::KEY_PRESSED) return;

    // If the field is being focused, we crawl the key input
    // If that key is a number and the string hasn't reached its limit, insert that key to the buffer.
    // If KEY_BACKSPACE is triggered, remove the last character in the buffer.
    int keyID = (int) nextInput.keySignal;

    std::string inputStr = initBuffer;
    if (keyID >= '0' && keyID <= '9') {
        inputStr.push_back(keyID);
    } else if (keyID == (int) KeyboardKey::KEY_SPACE) {
        if (!inputStr.empty() && inputStr.back() != ' ') inputStr.push_back(' ');
    } else if (keyID == (int) KeyboardKey::KEY_BACKSPACE && !inputStr.empty()) {
        inputStr.pop_back();
    }

    if (!inputStr.empty() && isdigit(inputStr.back())) {
        std::string buffer = "";
        while (!inputStr.empty() && isdigit(inputStr.back())) {
            buffer.push_back(inputStr.back());
            inputStr.pop_back();
        }

        std::reverse(buffer.begin(), buffer.end());
        int value = std::stoi(buffer);
        if (value > 9999) value /= 10;

        if (!inputStr.empty()) inputStr.push_back(' ');
        inputStr += std::to_string(value);
    }

    initBuffer = inputStr;
}
void HashTableUI::processInputFieldOthers(RawInputEvent nextInput) {    
    ButtonController* targetController = navbarMap[navPhase].getButtonController(operationPlaceholder);
    ShapeState targetBackground = targetController -> getButtonShape().getBackground();
    Vector2 rootPos = {FIELD_GAP_X + BUTTON_WIDTH, targetBackground.startPosition.y};

    bool randomClick = nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED
                    && fieldRandom.getButtonShape().contains(nextInput.position - rootPos - Vector2{FIELD_TEXTBOX_WIDTH + FIELD_GAP_X, 0.0f});

    // When the random request is required, set a random value with a random length [1, TEXTBOX_LENGTH]
    if (randomClick) {        
        int randLength = Helper::random_gen(1, TEXTBOX_LENGTH_LIMIT);
        std::string str = "";
        for (int i = 0; i < randLength; ++i) {
            str.push_back(Helper::random_gen('0', '9'));
        }

        fieldTextbox.setLabelBuffer(str);
        return;
    }

    if (nextInput.inputType != RawInputEvent::InputType::KEY_PRESSED) return;

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
void HashTableUI::processInputField(RawInputEvent nextInput) {
    if (operationPlaceholder.empty()) return;
    
    if (operationPlaceholder == "Init") {
        processInputFieldInit(nextInput);
        return;
    }

    processInputFieldOthers(nextInput);
}

/////////////////////////////////////
///     FIELD RESPONSE REQUEST    ///
/////////////////////////////////////
CommandPattern HashTableUI::fieldInitListenerRequest(RawInputEvent nextInput) {
    ButtonController* targetController = navbarMap[navPhase].getButtonController(operationPlaceholder);
    ShapeState targetBackground = targetController -> getButtonShape().getBackground();
    Vector2 rootPos = {FIELD_GAP_X + BUTTON_WIDTH, targetBackground.startPosition.y};

    rootPos += Vector2{2 * (FIELD_BUTTON_WIDTH + FIELD_GAP_X), 0.0f};

    bool submitEnter = nextInput.inputType == RawInputEvent::InputType::KEY_PRESSED
                    && nextInput.keySignal == KeyboardKey::KEY_ENTER;
    bool submitClick = nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED
                    && fieldSubmit.getButtonShape().contains(nextInput.position - rootPos);

    if (!submitEnter && !submitClick) return CommandPattern();
    
    std::vector <std::string> rawValue = Helper::keywordParse(initBuffer);

    operationPlaceholder = "";
    navPhase = NavPhase::NAV_CLOSED;
    changeField();

    return CommandPattern {
        "MODIFY",
        "INTERACT",
        getDSName(),
        "INIT",
        rawValue
    };
}
CommandPattern HashTableUI::fieldOthersListenerRequest(RawInputEvent nextInput) {
    ButtonController* targetController = navbarMap[navPhase].getButtonController(operationPlaceholder);
    ShapeState targetBackground = targetController -> getButtonShape().getBackground();
    Vector2 rootPos = {FIELD_GAP_X + BUTTON_WIDTH, targetBackground.startPosition.y};
    
    rootPos += Vector2{FIELD_BUTTON_WIDTH + FIELD_TEXTBOX_WIDTH + 2 * FIELD_GAP_X, 0.0f};

    bool submitEnter = nextInput.inputType == RawInputEvent::InputType::KEY_PRESSED
                    && nextInput.keySignal == KeyboardKey::KEY_ENTER;
    bool submitClick = nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED
                    && fieldSubmit.getButtonShape().contains(nextInput.position - rootPos);
    
    if (!submitEnter && !submitClick) return CommandPattern();
    
    // If the Submit button is triggered, the value from the input field is crawled
    // After that, the input buffer will be removed, and the navigation sidebar will close.
    // Solving edge cases: when the input is empty
    std::string rawValue = fieldTextbox.getLabelBuffer();
    std::string chosenOperation = operationPlaceholder;

    operationPlaceholder = "";
    navPhase = NavPhase::NAV_CLOSED;
    changeField();

    if (rawValue.empty()) rawValue = "0";

    return CommandPattern {
        Helper::upperString(chosenOperation) == "SEARCH" ? "QUERY" : "MODIFY",
        "INTERACT",
        getDSName(),
        Helper::upperString(chosenOperation),
        {rawValue}
    };
}
CommandPattern HashTableUI::fieldListenerRequest(RawInputEvent nextInput) {
    if (operationPlaceholder.empty()) return CommandPattern();

    if (operationPlaceholder == "Init") {
        return fieldInitListenerRequest(nextInput);
    }
    
    return fieldOthersListenerRequest(nextInput);
}