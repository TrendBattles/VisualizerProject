#ifndef APPLOOP_HPP
#define APPLOOP_HPP

#include <Core/InputManager.hpp>
#include <Core/EventManager.hpp>
#include <Core/AnimationManager.hpp>
#include <Core/PlaybackController.hpp>

#include <DS/Tree/AVL/AVL.hpp>
#include <DS/Tree/AVL/AVLUI.hpp>

#include <Graphics/UIManager.hpp>

#include <Common.hpp>

#include <iostream>

class AppLoop {
public:
    AppLoop();
    ~AppLoop();
    void init();
    void mainLoop();

private:
    const std::vector <std::string>& getDSOptions() const;
    void setScreenSection(const std::string& sectionID);
    const std::string& getScreenSection() const;
    bool isVisualizing() const;

    //Menu, Settings, DS options, ....
    std::vector <std::string> options;
    std::string appSection = "";

    void VisualizerLoop();
    void VisualizerInputHandling();
    void VisualizerUpdate();
    void VisualizerRender();

    Camera2D camera;
    UIManager* uiManager = nullptr;
    
    InputManager* inputManager = nullptr;
    EventManager* eventManager = nullptr;

    StateManager* stateManager = nullptr;
    AnimationManager* animationManager = nullptr;

    IDataStructure* DataStructure = nullptr;
    IDataStructureUI* DataStructureUI = nullptr;

    PlaybackController* playbackController = nullptr;
    PseudocodePanel* pseudocodePanel = nullptr;
};

#endif