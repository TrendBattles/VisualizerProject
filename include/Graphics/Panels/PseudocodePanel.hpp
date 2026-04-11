#ifndef PSEUDOCODEPANEL_HPP
#define PSEUDOCODEPANEL_HPP

#include <Core/AnimationManager.hpp>
#include <Core/InputManager.hpp>

#include <Graphics/UIManager.hpp>
#include <Graphics/Panels/Button.hpp>
#include <Graphics/RendererCommon.hpp>

class PseudocodePanel {
public:
    PseudocodePanel();
    ~PseudocodePanel();
    
    void setAnimationManager(AnimationManager* source);
    void setUIManager(UIManager* source);
    void render(std::string DSTarget);
    void renderPseudoInfo(std::pair <PseudocodeSection, std::vector <int>> pseudoInfo = {PseudocodeSection::NONE, {}}); 

    void processInput(RawInputEvent nextInput);
private:
    AnimationManager* animationManager = nullptr;
    UIManager* uiManager = nullptr;

    void init();
    std::map<PseudocodeSection, std::vector <std::string>> pseudocodeContent;
    
    const Vector2 toggleSize = {80.0f, 40.0f};
    ButtonController toggleButton;

    const Vector2 panelSize = {350.0f, 500.0f}, textOffset = {30.0f, 60.0f}, textPadding = {0.0f, 15.0f};
    Vector2 ORIGIN_POS;
    const float fontSize = 12.0f;
    void drawLogicalCode(PseudocodeSection pseudoFrame, std::vector <int> pseudoActiveLines);
};
#endif