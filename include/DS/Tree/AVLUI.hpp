#ifndef AVLUI_HPP
#define AVLUI_HPP

#include <DS/IDataStructureUI.hpp>

class AVLUI: public IDataStructureUI {
public:
    AVLUI();
    
    std::string processInput(RawInputEvent nextInput) override;

    void disableOption(std::string optionName) override;
    void enableOption(std::string optionName) override;
    void disableAllOperations() override;
    void enableAllOperations() override;
    
    void render() override;
    std::string getDSName() const override;
private:
    const std::vector <std::string> operationList = {"Insert", "Remove", "Search"};
    std::map <std::string, std::array <Button, 3>> buttonMap;
    std::map <std::string, bool> isButtonDisabledMap;

    const float BUTTON_HEIGHT = 60.0f;
    const float BUTTON_WIDTH = 120.0f;
    const float BUTTON_GAP = 20.0f;
    const float BUTTON_Y_BOTTOM_OFFSET = 100.0f;
    void createButtons(std::string buttonID, float x, float y);
    std::string buttonListenerRequest(RawInputEvent nextInput);

    const float FIELD_GAP = 30.0f;
    const float FIELD_TEXTBOX_WIDTH = 110.0f;
    const float FIELD_TEXTBOX_HEIGHT = 60.0f;
    const float FIELD_SUBMIT_WIDTH = 120.0f;
    const float FIELD_SUBMIT_HEIGHT = 60.0f;

    const int TEXTBOX_LENGTH_LIMIT = 4;
    Text fieldPlaceholder;
    TextBox fieldTextBox;
    bool isFieldTextBoxFocused = false;
    std::array <Button, 2> fieldSubmit;
    std::string operationPlaceholder = "";
    
    void createField(float x, float y);
    std::string fieldListenerRequest(RawInputEvent nextInput);
};
#endif