#ifndef IDATASTRUCTUREUI_HPP
#define IDATASTRUCTUREUI_HPP

#include <Core/InputManager.hpp>
#include <Core/EventManager.hpp>

#include <Graphics/UIManager.hpp>
#include <Graphics/Panels/Button.hpp>
#include <Graphics/Panels/NavigationBar.hpp>

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
    virtual CommandPattern processInput(RawInputEvent nextInput) = 0;

    virtual void disableOption(const std::string& optionName) = 0;
    virtual void enableOption(const std::string& optionName) = 0;
    virtual void disableAllOperations() = 0;
    virtual void enableAllOperations() = 0;
    
    virtual void update() = 0;
    virtual void render() = 0;
    virtual std::string getDSName() const = 0;
protected:
    void drawButton(const Button& button);
    void drawTextbox(const Textbox& button);

    std::map <NavPhase, NavigationBar> navbarMap;
    NavPhase navPhase = NavPhase::NAV_CLOSED;

    virtual void createNavbar() = 0;
    virtual void processInputNavbar(RawInputEvent nextInput) = 0;

    UIManager* uiManager = nullptr;
};
#endif