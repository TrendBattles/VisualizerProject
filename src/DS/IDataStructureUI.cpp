#include <DS/IDataStructureUI.hpp>

void IDataStructureUI::setUIManager(UIManager* source) {
    uiManager = source;
}
void IDataStructureUI::drawButton(const Button& button) {
    uiManager -> drawShape(button.getBackground());
    uiManager -> drawText(button.getLabel());
}
void IDataStructureUI::drawTextbox(const Textbox& textbox) {
    drawButton(textbox);
}
IDataStructureUI::NavPhase IDataStructureUI::getNavPhase() const {
    return navPhase;
}

/////////////////////////////////////
///     GLOBAL HEADER FUNCTION    ///
/////////////////////////////////////

void IDataStructureUI::update() {
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
void IDataStructureUI::render() {
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
CommandPattern IDataStructureUI::processInput(RawInputEvent nextInput) {
    CommandPattern navbarSignal = navbarListenerRequest(nextInput);
    if (!navbarSignal.prefix.empty()) return navbarSignal;
    
    CommandPattern fieldSignal = fieldListenerRequest(nextInput);
    if (!fieldSignal.prefix.empty()) return fieldSignal;

    processInputNavbar(nextInput);
    processInputField(nextInput);

    return CommandPattern();
}

/////////////////////////////////////////
///     GLOBAL SUPPORTIVE FUNCTION    ///
/////////////////////////////////////////

void IDataStructureUI::updateInit(Vector2 rootPos) {
    hoverButtonTrigger(rootPos);
}
void IDataStructureUI::updateOthers(Vector2 rootPos) {
    hoverTextboxTrigger(rootPos);
    hoverButtonTrigger(rootPos);
}
void IDataStructureUI::hoverTextboxTrigger(Vector2 rootPos) {
    bool hoveredTextbox = false;
    for (int i = 0; i < activeFieldCount() && !hoveredTextbox; ++i) {
        Vector2 nextPos = rootPos + Vector2{0.0f, i * (FIELD_TEXTBOX_HEIGHT + FIELD_GAP_Y)};
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
void IDataStructureUI::hoverButtonTrigger(Vector2 rootPos) {
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

void IDataStructureUI::renderInit(Vector2 rootPos) {
    renderInitInput(rootPos + Vector2{0.0f, FIELD_GAP_Y + FIELD_BUTTON_HEIGHT});
    renderButton(rootPos, true);
}
void IDataStructureUI::renderOthers(Vector2 rootPos) {
    renderTextbox(rootPos);
    renderButton(rootPos);
}
void IDataStructureUI::renderInitInput(Vector2 textboxPos) {
    Vector2 textboxSize = Vector2{FIELD_BUTTON_WIDTH * 2 + FIELD_GAP_X, 300.0f};
    DrawRectangleV(textboxPos, textboxSize, Fade(BLACK, 0.3f));
    DrawRectangleLinesEx(
        Rectangle { textboxPos.x, textboxPos.y, textboxSize.x, textboxSize.y },
        2.0f,
        GetColor(0x374151FF)
    );

    Vector2 alignOffset = Vector2{10.0f, 10.0f};
    Vector2 position = alignOffset;

    std::vector <std::string> rawParseBuffer = Helper::keywordParse(initBuffer);
    std::string tempBuffer = "";

    for (const std::string& word : rawParseBuffer) {
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
void IDataStructureUI::renderTextbox(Vector2 rootPos) {
    for (int i = 0; i < activeFieldCount(); ++i) {
        Vector2 nextPos = rootPos + Vector2{0.0f, i * (FIELD_TEXTBOX_HEIGHT + FIELD_GAP_Y)};

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
void IDataStructureUI::renderButton(Vector2 rootPos, bool hasFileButton) {
    if (hasFileButton) {
        Button tempFile = fieldFile.getButtonShape();
        tempFile.positionTransitAllBy(rootPos);
        drawButton(tempFile);
    }
    rootPos += Vector2{FIELD_GAP_X + FIELD_BUTTON_WIDTH, 0.0f};

    Button tempRandom = fieldRandom.getButtonShape();
    tempRandom.positionTransitAllBy(rootPos);
    drawButton(tempRandom);

    rootPos += Vector2{FIELD_GAP_X + FIELD_BUTTON_WIDTH, 0.0f};
    
    Button tempSubmit = fieldSubmit.getButtonShape();
    tempSubmit.positionTransitAllBy(rootPos);    
    drawButton(tempSubmit);
}

///////////////////////////////
///     OPTION SETTINGS     ///
///////////////////////////////
void IDataStructureUI::disableOption(const std::string& optionName) {
    for (auto& it : navbarMap) {
        ButtonController* targetController = it.second.getButtonController(optionName);
        if (targetController != nullptr) {
            targetController -> setButtonState(ButtonState::DISABLED);
        }
    }
}
void IDataStructureUI::enableOption(const std::string& optionName) {
    for (auto& it : navbarMap) {
        ButtonController* targetController = it.second.getButtonController(optionName);
        if (targetController != nullptr) {
            targetController -> setButtonState(ButtonState::ACTIVE);
        }
    }
}
void IDataStructureUI::disableAllOperations() {
    for (const std::string& buttonID : operationList) {
        ButtonController* targetController = navbarMap[NavPhase::NAV_OPERATIONS].getButtonController(buttonID);
        
        if (targetController != nullptr) {
            targetController -> setButtonState(ButtonState::DISABLED);
        }
    }
}
void IDataStructureUI::enableAllOperations() {
    for (const std::string& buttonID : operationList) {
        ButtonController* targetController = navbarMap[NavPhase::NAV_OPERATIONS].getButtonController(buttonID);
        if (targetController != nullptr) {
            targetController -> setButtonState(ButtonState::ACTIVE);
        }
    }
}

/////////////////////////////////////
///     NAVBAR INITIALIZATION     ///
/////////////////////////////////////
void IDataStructureUI::createNavbar() {
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

NavigationBar IDataStructureUI::createNavBar(const std::vector <std::string>& buttonList) {
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

ButtonController IDataStructureUI::createNavbarButtons(const std::string& buttonID, float x, float y) {
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

void IDataStructureUI::createNavbarToggle() {
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

///////////////////////////////////////
///     NAVBAR INPUT PROCESSING     ///
///////////////////////////////////////
CommandPattern IDataStructureUI::navbarListenerRequest(RawInputEvent nextInput) {
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
void IDataStructureUI::processInputNavbar(RawInputEvent nextInput) {
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

///////////////////////////////////
///     FIELD INITIALIZATION    ///
///////////////////////////////////
void IDataStructureUI::createField() {
    for (int i = 0; i < MAX_TEXTBOX; ++i) {
        fieldTextbox[i] = Helper::createTextbox(
            Helper::createRectangle(
                getDSName() + "_" + Helper::rectangleStringBuffer("Field_Textbox_" + std::to_string(i)),
                Vector2 {0.0f, 0.0f}, Vector2{FIELD_TEXTBOX_WIDTH, FIELD_TEXTBOX_HEIGHT}, 
                FIELD_TEXTBOX_OUTLINE,
                GetColor(0x3B4252FF), BLACK, 1
            ),
            Helper::createText("", CoreFonts::Aptos, FIELD_TEXTBOX_FONTSIZE, 0.2f * FIELD_TEXTBOX_FONTSIZE, {0, 0}, WHITE)
        );
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

    fieldFile.setButtonSettings(ButtonState::ACTIVE, createButton("Field_File", "File", GetColor(0xC9AE8AFF), CoreFonts::Aptos));
    fieldFile.setButtonSettings(ButtonState::HOVERED, createButton("Field_File_hovered", "File", GetColor(0x81A1C1FF), CoreFonts::AptosBold));
}
void IDataStructureUI::changeField() {
    for (int i = 0; i < MAX_TEXTBOX; ++i) {
        fieldTextbox[i].clearLabelBuffer();
    }
    initBuffer.clear();

    textboxFocusID = activeFieldCount() > 1 ? 0 : -1;
}
int IDataStructureUI::activeFieldCount() const {
    if (operationPlaceholder == "Insert" 
     || operationPlaceholder == "Remove" 
     || operationPlaceholder == "Search") {
        return 1;
    }
    if (operationPlaceholder == "Update") {
        return 2;
    }
    if (operationPlaceholder == "Insert Edge") {
        return 3;
    }
    if (operationPlaceholder == "Remove Edge") {
        return 2;
    }
    if (operationPlaceholder == "Remove Node") {
        return 1;
    }
    if (operationPlaceholder == "Dijkstra") {
        return 2;
    }
    
    return 0;
}

///////////////////////////////////////
///     FIELD INPUT PROCESSING      ///
///////////////////////////////////////

/// @brief A function for handling reading inputs from files
void IDataStructureUI::InitReadFile() {
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
}

/// @brief A function to randomize the initialized data
void IDataStructureUI::InitRandom() {
    initBuffer.clear();

    if (getDSName() == "Graph") {
        int N = Helper::random_gen(1, 10);
        for (int i = 1; i <= N; ++i) {
            if (!initBuffer.empty()) initBuffer += " ";

            int u = Helper::random_gen(1, 10), v = Helper::random_gen(1, 10), w = Helper::random_gen(0, 99);
            initBuffer += std::to_string(u) + " " + std::to_string(v) + " " + std::to_string(w);
        }

        return;
    }
    if (getDSName() == "Trie") {
        int N = Helper::random_gen(1, 10);
        for (int i = 1; i <= N; ++i) {
            if (!initBuffer.empty()) initBuffer += " ";
                
            int length = Helper::random_gen(1, TEXTBOX_LENGTH_LIMIT);
            for (int j = 1; j <= length; ++j) {
                initBuffer.push_back(Helper::random_gen('0', '9'));
            }
        }
        return;
    }

    int N = Helper::random_gen(1, 20);
    for (int i = 1; i <= N; ++i) {
        if (!initBuffer.empty()) initBuffer += " ";
            
        initBuffer += std::to_string(Helper::random_gen(0, 9999));
    }
}
void IDataStructureUI::InitReadKeyboard(RawInputEvent nextInput) {
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

    // Now, depending on different data structures, there are cases to handle input management
    // AVL, Hash Table, Linked Lists: 0 - 9999
    // Trie: Maximum length of 5
    // Graph: Nodes <= 10, Weight <= 99
    if (!inputStr.empty() && isdigit(inputStr.back())) {
        std::string buffer = "";
        while (!inputStr.empty() && isdigit(inputStr.back())) {
            buffer.push_back(inputStr.back());
            inputStr.pop_back();
        }

        std::reverse(buffer.begin(), buffer.end());
        if (getDSName() == "Trie") {
            while ((int) buffer.size() > TEXTBOX_LENGTH_LIMIT)
                buffer.pop_back();

            inputStr += buffer;
        } else if (getDSName() == "Graph") {
            int bufferCnt = std::count(inputStr.begin(), inputStr.end(), ' ') + 1;
            int value = std::stoi(buffer);

            if (bufferCnt % 3 == 0 && value > 99) value /= 10;
            if (bufferCnt % 3 && value > 10) value /= 10;

            inputStr += std::to_string(value);
        } else {
            int value = std::stoi(buffer);

            if (value > 9999) value /= 10;
            inputStr += std::to_string(value);
        }
    }

    initBuffer = inputStr;
}
void IDataStructureUI::processInputFieldInit(RawInputEvent nextInput) {
    ButtonController* targetController = navbarMap[navPhase].getButtonController(operationPlaceholder);
    ShapeState targetBackground = targetController -> getButtonShape().getBackground();
    Vector2 rootPos = {FIELD_GAP_X + BUTTON_WIDTH, targetBackground.startPosition.y};

    bool fileClick = nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED
                    && fieldFile.getButtonShape().contains(nextInput.position - rootPos);
    rootPos += Vector2{FIELD_BUTTON_WIDTH + FIELD_GAP_X, 0.0f};
    bool randomClick = nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED
                    && fieldRandom.getButtonShape().contains(nextInput.position - rootPos);

    if (fileClick) {
        InitReadFile();
        return;
    }    
    
    if (randomClick) {
        InitRandom();
        return;
    }

    InitReadKeyboard(nextInput);
}

void IDataStructureUI::OthersRandom() {
    int currentTextboxID = textboxFocusID == -1 ? 0 : textboxFocusID;

    if (getDSName() == "Graph") {
        fieldTextbox[currentTextboxID].setLabelBuffer(std::to_string(Helper::random_gen(currentTextboxID < 2, currentTextboxID >= 2 ? 99 : 10)));
        return;
    }
    if (getDSName() == "Trie") {
        std::string inputBuffer = "";
        int length = Helper::random_gen(1, TEXTBOX_LENGTH_LIMIT);
        for (int i = 1; i <= length; ++i) {
            inputBuffer.push_back(Helper::random_gen('0', '9'));
        }

        fieldTextbox[currentTextboxID].setLabelBuffer(inputBuffer);
        return;
    }

    fieldTextbox[currentTextboxID].setLabelBuffer(std::to_string(Helper::random_gen(0, 9999)));
}
void IDataStructureUI::OthersReadKeyboard(RawInputEvent nextInput) {
    if (nextInput.inputType != RawInputEvent::InputType::KEY_PRESSED) return;
    int currentTextboxID = textboxFocusID == -1 ? 0 : textboxFocusID;

    // If the field is being focused, we crawl the key input
    // If that key is a number and the string hasn't reached its limit, insert that key to the buffer.
    // If KEY_BACKSPACE is triggered, remove the last character in the buffer.
    int keyID = (int) nextInput.keySignal;
    
    std::string inputStr = fieldTextbox[currentTextboxID].getLabelBuffer();
    if (keyID >= '0' && keyID <= '9' && (int) inputStr.length() < TEXTBOX_LENGTH_LIMIT) {
        inputStr.push_back(keyID);
    }
        
    if (keyID == (int) KeyboardKey::KEY_BACKSPACE && !inputStr.empty()) {
        inputStr.pop_back();
    }

    if (!inputStr.empty() && getDSName() != "Trie") {
        int value = std::stoi(inputStr);
        inputStr = std::to_string(value);
    }

    fieldTextbox[currentTextboxID].setLabelBuffer(inputStr);
}
void IDataStructureUI::processInputFieldOthers(RawInputEvent nextInput) {
    ButtonController* targetController = navbarMap[navPhase].getButtonController(operationPlaceholder);
    ShapeState targetBackground = targetController -> getButtonShape().getBackground();
    Vector2 rootPos = {FIELD_GAP_X + BUTTON_WIDTH, targetBackground.startPosition.y};

    bool randomClick = nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED
                    && fieldRandom.getButtonShape().contains(nextInput.position - rootPos - Vector2{FIELD_GAP_X + FIELD_TEXTBOX_WIDTH, 0.0f});
    
    
    // When the random request is required, set a random value
    if (randomClick) {
        OthersRandom();
        return;
    }
    
    if (nextInput.inputType == RawInputEvent::InputType::KEY_PRESSED
     && nextInput.keySignal == KeyboardKey::KEY_TAB) {
        if (textboxFocusID != -1) textboxFocusID = (textboxFocusID + 1) % activeFieldCount();
    }

    OthersReadKeyboard(nextInput);
}
void IDataStructureUI::processInputField(RawInputEvent nextInput) {
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

/// @brief Waiting for submitting initalized data 
CommandPattern IDataStructureUI::fieldInitListenerRequest(RawInputEvent nextInput) {
    ButtonController* targetController = navbarMap[navPhase].getButtonController(operationPlaceholder);
    ShapeState targetBackground = targetController -> getButtonShape().getBackground();
    Vector2 rootPos = {FIELD_GAP_X + BUTTON_WIDTH, targetBackground.startPosition.y};
    
    rootPos += Vector2{FIELD_GAP_X + FIELD_BUTTON_WIDTH, 0.0f} * 2;
    bool submitEnter = nextInput.inputType == RawInputEvent::InputType::KEY_PRESSED
                    && nextInput.keySignal == KeyboardKey::KEY_ENTER;
    bool submitClick = nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED
                    && fieldSubmit.getButtonShape().contains(nextInput.position - rootPos);

    if (!submitEnter && !submitClick) return CommandPattern();

    std::vector <std::string> rawValue = Helper::keywordParse(initBuffer);
    if (getDSName() == "Graph") {
        while ((int) rawValue.size() % 3) rawValue.pop_back();
    }

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
/// @brief Waiting for submitting other operations
CommandPattern IDataStructureUI::fieldOthersListenerRequest(RawInputEvent nextInput) {
    ButtonController* targetController = navbarMap[navPhase].getButtonController(operationPlaceholder);
    ShapeState targetBackground = targetController -> getButtonShape().getBackground();
    Vector2 rootPos = {FIELD_GAP_X + BUTTON_WIDTH, targetBackground.startPosition.y};
    
    bool submitEnter = nextInput.inputType == RawInputEvent::InputType::KEY_PRESSED
                    && nextInput.keySignal == KeyboardKey::KEY_ENTER;
    bool submitClick = nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED
                    && fieldSubmit.getButtonShape().contains(nextInput.position - rootPos - Vector2{FIELD_GAP_X * 2 + FIELD_BUTTON_WIDTH + FIELD_TEXTBOX_WIDTH, 0.0f});
    
    if (!submitEnter && !submitClick) return CommandPattern();
    
    // If the Submit button is triggered, the value from the input field is crawled
    // After that, the input buffer will be removed, and the navigation sidebar will close.
    // Solving edge cases: when the input is empty, set it to 0.

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
CommandPattern IDataStructureUI::fieldListenerRequest(RawInputEvent nextInput) {
    if (operationPlaceholder.empty()) return CommandPattern();

    if (operationPlaceholder == "Init") {
        return fieldInitListenerRequest(nextInput);   
    }

    return fieldOthersListenerRequest(nextInput);
}