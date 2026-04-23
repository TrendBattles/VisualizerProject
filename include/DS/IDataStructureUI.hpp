#ifndef IDATASTRUCTUREUI_HPP
#define IDATASTRUCTUREUI_HPP

#include <Core/InputManager.hpp>
#include <Core/EventManager.hpp>

#include <Graphics/UIManager.hpp>
#include <Graphics/Panels/Button.hpp>
#include <Graphics/Panels/NavigationBar.hpp>
#include <Graphics/Helper.hpp>

#include <tinyfiledialogs.h>

/// @brief Data Structure UI Interface : Interaction
class IDataStructureUI {
public:
    void setUIManager(UIManager* source);
    enum class NavPhase {
        NAV_CLOSED,
        NAV_OPTIONS,
        NAV_DS_OPTIONS,
        NAV_OPERATIONS
    };
    NavPhase getNavPhase() const;

    virtual ~IDataStructureUI() = default;
    CommandPattern processInput(RawInputEvent nextInput);

    void disableOption(const std::string& optionName);
    void enableOption(const std::string& optionName);
    void disableAllOperations();
    void enableAllOperations();
    
    void update();
    void render();
    virtual std::string getDSName() const = 0;
protected:
    void hoverTextboxTrigger(Vector2 rootPos);
    void hoverButtonTrigger(Vector2 rootPos);
    void updateInit(Vector2 rootPos);
    void updateOthers(Vector2 rootPos);

    virtual void renderInitInput(Vector2 textboxPos);
    void renderTextbox(Vector2 rootPos);
    void renderButton(Vector2 rootPos, bool hasFileButton = false);
    void renderInit(Vector2 rootPos);
    void renderOthers(Vector2 rootPos);

    void drawButton(const Button& button);
    void drawTextbox(const Textbox& button);
    UIManager* uiManager = nullptr;
    std::map <NavPhase, NavigationBar> navbarMap;
    NavPhase navPhase = NavPhase::NAV_CLOSED;

    std::vector <std::string> operationList;
    const std::vector <std::string> DSList = {"AVL Tree", "Trie", "Linked List", "Hash Table", "Graph"};
    const std::vector <std::string> optionList = {"Main Menu", "Data Structures", "Operations"};

    const float BUTTON_HEIGHT = 60.0f;
    const float BUTTON_WIDTH = 190.0f;
    const float BUTTON_GAP = 20.0f;
    
    void createNavbar();
    NavigationBar createNavBar(const std::vector <std::string>& buttonList);
    ButtonController createNavbarButtons(const std::string& buttonID, float x, float y);
    void createNavbarToggle();
    CommandPattern navbarListenerRequest(RawInputEvent nextInput);
    void processInputNavbar(RawInputEvent nextInput);

    std::string operationPlaceholder = "";
    std::string initBuffer = "";
    const float FIELD_GAP_X = 30.0f, FIELD_GAP_Y = 20.0f;
    const float FIELD_TEXTBOX_WIDTH = 120.0f;
    const float FIELD_TEXTBOX_HEIGHT = 60.0f;
    const float FIELD_TEXTBOX_OUTLINE = 2.0f;
    const float FIELD_TEXTBOX_FONTSIZE = 25.0f;

    static const int MAX_TEXTBOX = 3;
    Textbox fieldTextbox[MAX_TEXTBOX];
    int textboxFocusID = -1;

    const float FIELD_BUTTON_WIDTH = 120.0f;
    const float FIELD_BUTTON_HEIGHT = 60.0f;
    const float FIELD_BUTTON_FONT_SIZE = 25.0f; 
    ButtonController fieldRandom, fieldSubmit, fieldFile;

    int TEXTBOX_LENGTH_LIMIT = 0;
    void createField();
    void changeField();
    int activeFieldCount() const;

    virtual bool exceedsTextboxInitRange() = 0;
    virtual void handleInputFieldInit() = 0;
    void InitReadFile();
    void InitRandom();
    void InitReadKeyboard(RawInputEvent nextInput);
    void processInputFieldInit(RawInputEvent nextInput);
    void OthersRandom();
    void OthersReadKeyboard(RawInputEvent nextInput);
    void processInputFieldOthers(RawInputEvent nextInput);
    void processInputField(RawInputEvent nextInput);

    CommandPattern fieldInitListenerRequest(RawInputEvent nextInput);
    CommandPattern fieldOthersListenerRequest(RawInputEvent nextInput);
    CommandPattern fieldListenerRequest(RawInputEvent nextInput);
};
#endif