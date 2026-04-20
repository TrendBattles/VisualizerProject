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

    void hoverTextboxTrigger(Vector2 rootPos = Vector2{0.0f, 0.0f});
    void hoverButtonTrigger(Vector2 rootPos = Vector2{0.0f, 0.0f});

    void renderTextbox(Vector2 rootPos = Vector2{0.0f, 0.0f});
    void renderButton(Vector2 rootPos = Vector2{0.0f, 0.0f});

    const std::vector <std::string> operationList = {"Insert Edge", "Remove Edge", "Remove Node", "Clear", "Dijkstra", "Kruskal"};
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
    std::string edgeHolder[MAX_TEXTBOX] = {"u:", "v:", "w:"};
    Vector2 edgeHolderSize[MAX_TEXTBOX];
    int textboxFocusID = -1;

    const float FIELD_BUTTON_WIDTH = 120.0f;
    const float FIELD_BUTTON_HEIGHT = 60.0f;
    const float FIELD_BUTTON_FONT_SIZE = 25.0f; 
    ButtonController fieldRandom, fieldSubmit;
    
    void createField();
    void changeField();
    void processInputField(RawInputEvent nextInput);
    CommandPattern fieldListenerRequest(RawInputEvent nextInput);
};
#endif