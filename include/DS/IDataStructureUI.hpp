#ifndef IDATASTRUCTUREUI_HPP
#define IDATASTRUCTUREUI_HPP

#include <Core/InputManager.hpp>

#include <Graphics/UIManager.hpp>
#include <Graphics/Panels/Button.hpp>

class IDataStructureUI {
public:
    void setUIManager(UIManager* source);

    virtual ~IDataStructureUI() = default;
    virtual std::string processInput(RawInputEvent nextInput) = 0;

    virtual void disableOption(std::string optionName) = 0;
    virtual void enableOption(std::string optionName) = 0;
    virtual void disableAllOperations() = 0;
    virtual void enableAllOperations() = 0;
    
    virtual void render() = 0;
    virtual std::string getDSName() const = 0;
protected:
    void drawButton(const Button& button);
    void drawTextBox(const TextBox& button);
    UIManager* uiManager = nullptr;
};
#endif