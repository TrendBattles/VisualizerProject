#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include <Graphics/RendererCommon.hpp>
#include <Graphics/Panels/Button.hpp>
#include <Graphics/UIManager.hpp>

class MainMenu {
public:
    MainMenu();
    void init(const std::vector<std::string>& DSOptions);

    void setUIManager(UIManager* source);
    void update();
    void render();
    void reset();

    const std::string& getPendingSection() const { return pendingSection; }
    void clearPendingSection() { pendingSection = ""; }

    Color getThemeColor() const;
private:
    std::string pendingSection = "";

    void aboutRender();

    const float BUTTON_WIDTH = 200.0f, BUTTON_HEIGHT = 60.0f, BUTTON_GAP = 40.0f;
    ButtonController exploreButton, aboutButton, modeButton;
    ButtonController aboutBackButton;
    ButtonController visualizerBackButton;

    std::map <std::string, ButtonController> optionButton;
    std::vector <std::string> options;
    
    enum MenuPhase {
        MAIN_MENU,
        ABOUT,
        EXPLORE
    };
    MenuPhase menuPhase = MenuPhase::MAIN_MENU;

    const std::vector <int> backgroundThemes = {
        0x1F2937FF, // Theme 1 (Dark Blue Theme)
        0x2B2B2BFF, // Theme 2 (Dark Brown Theme) 
        0x002D3436  // Theme 3 (Dark Navy Green Theme)
    };
    const float MODE_TRANSITION = 0.5f;
    float modeTimeLeft = 0.0f;
    int currentThemeIndex = 0;


    void setupButton(ButtonController& buttonController, Vector2 position, Vector2 size, const std::string& labelText);
    void drawButton(const Button& button);
    UIManager* uiManager = nullptr;
};

#endif
