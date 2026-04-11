#ifndef NAVIGATIONBAR_HPP
#define NAVIGATIONBAR_HPP

#include <Core/InputManager.hpp>

#include <Graphics/RendererCommon.hpp>
#include <Graphics/Panels/Button.hpp>

class NavigationBar {
public:
    NavigationBar();
    ~NavigationBar();
    static constexpr const char* TOGGLE_KEY = "TOGGLE";
    
    void setNavOrigin(Vector2 position);
    void setCenter(bool isTrue);
    void setBackground(ShapeState newBackground);
    ShapeState getBackground() const;
    
    void setButtonController(const std::string& buttonID, ButtonController buttonController);

    ButtonController* getButtonController(const std::string& buttonID);
    bool findButtonController(const std::string& buttonID);
    bool inButtonHitbox(const std::string& buttonID, Vector2 position);

    std::string processInput(RawInputEvent nextInput);
    void hoverButtonTrigger();
    
    Snapshot getNavigationSnapshot();
private:
    Vector2 navOrigin = {0.0f, 0.0f};
    bool isCentered = false;
    ShapeState navBackground;
    std::map <std::string, ButtonController> navButtons;
};
#endif