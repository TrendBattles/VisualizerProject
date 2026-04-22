#ifndef GRAPHUI_HPP
#define GRAPHUI_HPP

#include <DS/IDataStructureUI.hpp>

class GraphUI: public IDataStructureUI {
public:
    GraphUI();
    
    CommandPattern processInput(RawInputEvent nextInput) override;

    void disableOption(const std::string& optionName) override;
    void enableOption(const std::string& optionName) override;
    void disableAllOperations() override;
    void enableAllOperations() override;
    
    void update() override;
    void render() override;
    std::string getDSName() const override;
private:
    int activeFieldCount() const;

    void hoverTextboxTrigger(Vector2 rootPos);
    void hoverButtonTrigger(Vector2 rootPos);
    void updateInit(Vector2 rootPos);
    void updateOthers(Vector2 rootPos);

    bool exceedsTextboxInitRange();
    void renderInitInput(Vector2 textboxPos);
    void renderTextbox(Vector2 rootPos);
    void renderButton(Vector2 rootPos, bool hasFileButton = false);

    void renderInit(Vector2 rootPos);
    void renderOthers(Vector2 rootPos);

    const std::vector <std::string> operationList = {"Init", "Insert Edge", "Remove Edge", "Remove Node", "Clear", "Dijkstra", "Kruskal"};
    const std::vector <std::string> DSList = {"AVL Tree", "Trie", "Linked List", "Hash Table", "Graph"};
    const std::vector <std::string> optionList = {"Main Menu", "Data Structures", "Operations"};
    const float BUTTON_HEIGHT = 60.0f;
    const float BUTTON_WIDTH = 190.0f;
    const float BUTTON_GAP = 20.0f;
    
    void createNavbar() override;
    NavigationBar createNavBar(const std::vector <std::string>& buttonList);
    ButtonController createNavbarButtons(const std::string& buttonID, float x, float y);
    void createNavbarToggle();
    
    CommandPattern navbarListenerRequest(RawInputEvent nextInput);
    void processInputNavbar(RawInputEvent nextInput) override;

    const int TEXTBOX_LENGTH_LIMIT = 2;
    std::string operationPlaceholder = "";

    const float FIELD_GAP_X = 30.0f, FIELD_GAP_Y = 20.0f;
    const float FIELD_TEXTBOX_WIDTH = 120.0f;
    const float FIELD_TEXTBOX_HEIGHT = 60.0f;
    const float FIELD_TEXTBOX_OUTLINE = 2.0f, FIELD_TEXTBOX_GAP = 30.0f;
    const float FIELD_HOLDER_FONT_SIZE = 20.0f;

    static const int MAX_TEXTBOX = 3;
    Textbox fieldTextbox[MAX_TEXTBOX];
    int textboxFocusID = 0;
    std::string initBuffer = "";

    const float FIELD_BUTTON_WIDTH = 120.0f;
    const float FIELD_BUTTON_HEIGHT = 60.0f;
    const float FIELD_BUTTON_FONT_SIZE = 25.0f; 
    ButtonController fieldRandom, fieldSubmit, fieldFile;
    
    void createField();
    void changeField();
    
    void handleInputFieldInit();
    void processInputFieldInit(RawInputEvent nextInput);
    void processInputFieldOthers(RawInputEvent nextInput);
    void processInputField(RawInputEvent nextInput);

    CommandPattern fieldInitListenerRequest(RawInputEvent nextInput);
    CommandPattern fieldOthersListenerRequest(RawInputEvent nextInput);
    CommandPattern fieldListenerRequest(RawInputEvent nextInput);
};
#endif