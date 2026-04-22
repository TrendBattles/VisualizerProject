#ifndef HASHTABLEUI_HPP
#define HASHTABLEUI_HPP

#include <DS/IDataStructureUI.hpp>

class HashTableUI: public IDataStructureUI {
public:
    HashTableUI();
    
    CommandPattern processInput(RawInputEvent nextInput) override;

    void disableOption(const std::string& optionName) override;
    void enableOption(const std::string& optionName) override;
    void disableAllOperations() override;
    void enableAllOperations() override;
    
    void update() override;
    void render() override;
    std::string getDSName() const override;
private:
    const std::vector <std::string> operationList = {"Init", "Insert", "Remove", "Search", "Clear"};
    const std::vector <std::string> DSList = {"AVL Tree", "Trie", "Linked List", "Hash Table", "Graph"};
    const std::vector <std::string> optionList = {"Main Menu", "Data Structures", "Operations"};

    void updateInit(Vector2 rootPos = Vector2{0.0f, 0.0f});
    void updateOthers(Vector2 rootPos = Vector2{0.0f, 0.0f});

    bool exceedsTextboxInitRange();
    void renderInit(Vector2 rootPos = Vector2{0.0f, 0.0f});
    void renderOthers(Vector2 rootPos = Vector2{0.0f, 0.0f});

    const float BUTTON_HEIGHT = 60.0f;
    const float BUTTON_WIDTH = 190.0f;
    const float BUTTON_GAP = 20.0f;
    
    void createNavbar() override;
    NavigationBar createNavBar(const std::vector <std::string>& buttonList);
    ButtonController createNavbarButtons(const std::string& buttonID, float x, float y);
    void createNavbarToggle();
    
    CommandPattern navbarListenerRequest(RawInputEvent nextInput);
    void processInputNavbar(RawInputEvent nextInput) override;

    const int TEXTBOX_LENGTH_LIMIT = 4;
    std::string operationPlaceholder = "";

    const float FIELD_GAP_X = 30.0f, FIELD_GAP_Y = 20.0f;
    const float FIELD_TEXTBOX_WIDTH = 120.0f;
    const float FIELD_TEXTBOX_HEIGHT = 60.0f;
    const float FIELD_BUTTON_WIDTH = 120.0f;
    const float FIELD_BUTTON_HEIGHT = 60.0f;

    Textbox fieldTextbox;
    ButtonController fieldRandom, fieldSubmit, fieldFile;
    std::string initBuffer = "";
    
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