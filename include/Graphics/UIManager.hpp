#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include <Core/StateManager.hpp>
#include <Core/AnimationManager.hpp>

/// @brief UI Management
class UIManager {
public:
    UIManager();
    ~UIManager() = default;

    void setStateManger(StateManager* source);
    void setAnimationManager(AnimationManager* source);
    
    const std::vector <std::string>& getDSOptions() const;
    void setScreenSection(const std::string& sectionID);
    const std::string& getScreenSection() const;
    bool isVisualizing() const;

    void drawShape(const ShapeState& shape);
    void drawText(const Text& text);

    void renderSnapshot();
    void renderSnapshot(const Snapshot& modifiedSnapshot);
private:
    // To save the snapshots of the data structure
    StateManager* stateManager = nullptr;
    AnimationManager* animationManager = nullptr;

    //Menu, Settings, DS options, ....
    std::vector <std::string> options;
    std::string appSection;
};

#endif