#include <Graphics/Panels/MainMenu.hpp>
#include <Graphics/Helper.hpp>
MainMenu::MainMenu() {

}
void MainMenu::setUIManager(UIManager* source) {
    uiManager = source;
}

void MainMenu::init(const std::vector<std::string>& DSOptions) {
    setupButton(exploreButton, {800.0f, 300.0f}, {BUTTON_WIDTH, BUTTON_HEIGHT}, "EXPLORE");
    setupButton(aboutButton, {800.0f, 400.0f}, {BUTTON_WIDTH, BUTTON_HEIGHT},"ABOUT");
    setupButton(modeButton, {800.0f, 500.0f}, {BUTTON_WIDTH, BUTTON_HEIGHT}, "CHANGE MODE");
    setupButton(aboutBackButton, {10.0f, 10.0f}, {BUTTON_WIDTH / 2, BUTTON_HEIGHT}, "BACK");
    
    options = DSOptions;
    for (std::string& o : options) {
        for (char& x : o) {
            if (x == '_') x = ' ';
        }
    }

    float optionHeight = BUTTON_HEIGHT * (int) options.size() + BUTTON_GAP * ((int) options.size() - 1);
    float currentHeight = (GetScreenHeight() - optionHeight) * 0.5f;
    setupButton(visualizerBackButton, {700.0f, currentHeight - BUTTON_HEIGHT - 10.0f}, {BUTTON_WIDTH / 2, BUTTON_HEIGHT}, "BACK");

    for (std::string& o : options) {
        setupButton(optionButton[o], Vector2{800.0f, currentHeight}, {BUTTON_WIDTH, BUTTON_HEIGHT}, o);
        currentHeight += BUTTON_HEIGHT + BUTTON_GAP;
    }
}

void MainMenu::update() {
    Vector2 mousePos = GetMousePosition();
    bool clickedLeft = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (menuPhase == MenuPhase::MAIN_MENU) {
        // Update Hover States
        exploreButton.setButtonState(exploreButton.contains(mousePos) ? ButtonState::HOVERED : ButtonState::ACTIVE);
        aboutButton.setButtonState(aboutButton.contains(mousePos) ? ButtonState::HOVERED : ButtonState::ACTIVE);
        modeButton.setButtonState(modeButton.contains(mousePos) ? ButtonState::HOVERED : ButtonState::ACTIVE);

        // Handle Clicks
        if (clickedLeft) {
            if (exploreButton.contains(mousePos)) {
                menuPhase = MenuPhase::EXPLORE;
            } else if (modeButton.contains(mousePos) && modeTimeLeft == 0.0f) {
                currentThemeIndex = (currentThemeIndex + 1) % (int) backgroundThemes.size();
                modeTimeLeft = MODE_TRANSITION;
            } else if (aboutButton.contains(mousePos)) {
                menuPhase = MenuPhase::ABOUT;
            }
        }

        return;
    }
    
    if (menuPhase == MenuPhase::EXPLORE) {
        for (auto& it : optionButton) {
            it.second.setButtonState(it.second.contains(mousePos) ? ButtonState::HOVERED : ButtonState::ACTIVE);
            if (clickedLeft && it.second.contains(mousePos)) {
                pendingSection = it.first;
            }    
        }

        visualizerBackButton.setButtonState(visualizerBackButton.contains(mousePos) ? ButtonState::HOVERED : ButtonState::ACTIVE);

        if (clickedLeft && visualizerBackButton.contains(mousePos)) {
            menuPhase = MenuPhase::MAIN_MENU;
            visualizerBackButton.setButtonState(ButtonState::ACTIVE);
        }
        return;
    }

    aboutBackButton.setButtonState(aboutBackButton.contains(mousePos) ? ButtonState::HOVERED : ButtonState::ACTIVE);
    if (clickedLeft && aboutBackButton.contains(mousePos)) {
        menuPhase = MenuPhase::MAIN_MENU;
        aboutBackButton.setButtonState(ButtonState::ACTIVE);
    }
}

void MainMenu::render() {
    if (modeTimeLeft > 0.0f) {
        int nowIdx = currentThemeIndex;
        int lastIdx = (nowIdx + (int) backgroundThemes.size() - 1) % (int) backgroundThemes.size();

        float rate = modeTimeLeft / MODE_TRANSITION;
        float easedRate = 1.0f - std::pow(1.0f - rate, 3.0f);

        ClearBackground(Helper::colorTimeLerp(GetColor(backgroundThemes[nowIdx]), GetColor(backgroundThemes[lastIdx]), easedRate));
        modeTimeLeft = std::max(modeTimeLeft - GetFrameTime(), 0.0f);
    } else {
        ClearBackground(getThemeColor());
    }
    
    if (menuPhase == MenuPhase::ABOUT) {
        aboutRender();
        return;
    }

    // Title text
    DrawTextEx(CoreFonts::AptosBold, "DATA STRUCTURE", {100.0f, 350.0f}, 48.0f, 2.0f, GetColor(0xE0D4C8FF));
    DrawTextEx(CoreFonts::AptosBold, "VISUALIZER", {100.0f, 420.0f}, 48.0f, 2.0f, GetColor(0xE0D4C8FF));

    // Render buttons

    if (menuPhase == MenuPhase::EXPLORE) {
        drawButton(visualizerBackButton.getButtonShape());
        for (auto& it : optionButton) {
            drawButton(it.second.getButtonShape());
        }

        return;
    }

    drawButton(exploreButton.getButtonShape());
    drawButton(aboutButton.getButtonShape());
    drawButton(modeButton.getButtonShape());
}
void MainMenu::reset() {
    modeTimeLeft = 0.0f;
    pendingSection = "";
    menuPhase = MenuPhase::MAIN_MENU;
}
void MainMenu::aboutRender() {
    const std::vector <std::string> messageList = {
        "This is a data structure visualizer app for ",
        "the final project of CS163. The main goal is to ",
        "visualize some of the most common data structures, ",
        "with smooth animation transitions and pseudocode ",
        "highlights in order to give the best experiences ",
        "for the users. In addition, the project is scheduled ", 
        "to render minimal designs, while handling complex cases ",
        "and core managements.",
        "",
        "This is single-handedly introduced and implemented by: ",
        "25125006 - Nguyen Nguyen Gia Bao - K25 APCS student",
        "",
        "As the creator of the app, I wish you - the users - have ",
        "a great time learning and experiementing the algorithms."
    };

    float posY = 100.0f;
    for (const std::string& m : messageList) {
        Vector2 size = MeasureTextEx(CoreFonts::CascadiaMonoBold, m.c_str(), 30.0f, 3.0f);
        DrawTextEx(CoreFonts::Consolas, m.c_str(), Vector2{(GetScreenWidth() - size.x) * 0.5f, posY}, 30.0f, 3.0f, WHITE);
        posY += 40.0f;
    }

    drawButton(aboutBackButton.getButtonShape());
}


void MainMenu::setupButton(ButtonController& buttonController, Vector2 position, Vector2 size, const std::string& labelText) {
    ShapeState background = Helper::createRoundedRectangle(
        "MenuButton",
        position,
        position + size,
        2.0f, 0.2f,
        GetColor(0), GetColor(0),
        0
    );
    
    Text text = Helper::createText(
        labelText, CoreFonts::Aptos, 
        24.0f, 2.4f, 
        Vector2Lerp(background.startPosition, background.endPosition, 0.5f), 
        GetColor(0xE0D4C8FF)
    );
    text.setCenter(true);

    Button btnActive(background, text);
    
    // Hovered state
    background.setColor(GetColor(0), GetColor(0x80A0C0FF));
    Button btnHovered(background, text);

    buttonController.setButtonSettings(ButtonState::ACTIVE, btnActive);
    buttonController.setButtonSettings(ButtonState::HOVERED, btnHovered);
    buttonController.setButtonState(ButtonState::ACTIVE);
}

void MainMenu::drawButton(const Button& button) {
    uiManager -> drawShape(button.getBackground());
    uiManager -> drawText(button.getLabel());
}

Color MainMenu::getThemeColor() const {
    return GetColor(backgroundThemes[currentThemeIndex]);
}
