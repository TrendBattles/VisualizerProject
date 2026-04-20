#ifndef AVLUI_HPP
#define AVLUI_HPP

#include <DS/IDataStructureUI.hpp>

class AVLUI: public IDataStructureUI {
public:
    AVLUI();
    
    CommandPattern processInput(RawInputEvent nextInput) override;

    void disableOption(const std::string& optionName) override;
    void enableOption(const std::string& optionName) override;
    void disableAllOperations() override;
    void enableAllOperations() override;
    
    void update() override;
    void render() override;
    std::string getDSName() const override;
private:
    const std::vector <std::string> operationList = {"Insert", "Remove", "Search", "Clear"};
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

    const int TEXTBOX_LENGTH_LIMIT = 4;
    std::string operationPlaceholder = "";

    const float FIELD_GAP = 30.0f;
    const float FIELD_TEXTBOX_WIDTH = 120.0f;
    const float FIELD_TEXTBOX_HEIGHT = 60.0f;
    const float FIELD_RANDOM_WIDTH = 120.0f;
    const float FIELD_RANDOM_HEIGHT = 60.0f;
    const float FIELD_SUBMIT_WIDTH = 120.0f;
    const float FIELD_SUBMIT_HEIGHT = 60.0f;

    Textbox fieldTextbox;
    bool isFieldTextboxFocused = false;
    ButtonController fieldRandom, fieldSubmit;
    
    void createField();
    void changeField();
    void processInputField(RawInputEvent nextInput);
    CommandPattern fieldListenerRequest(RawInputEvent nextInput);
};
#endif