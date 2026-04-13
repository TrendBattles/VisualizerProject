#ifndef APPLOOP_HPP
#define APPLOOP_HPP

#include <Core/InputManager.hpp>
#include <Core/EventManager.hpp>
#include <Core/AnimationManager.hpp>
#include <Core/PlaybackController.hpp>

#include <DS/Tree/AVL/AVL.hpp>
#include <DS/Tree/AVL/AVLUI.hpp>
#include <DS/Tree/Trie/Trie.hpp>
#include <DS/Tree/Trie/TrieUI.hpp>

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
    std::string appSection = "", pendingSection = "";

    struct DataStructurePackage {
        Camera2D camera;
        IDataStructure* DataStructure = nullptr;
        IDataStructureUI* DataStructureUI = nullptr;
    };
    std::map <std::string, DataStructurePackage> DSPackage;

    void VisualizerLoop();
    void VisualizerInputHandling();
    void VisualizerUpdate();
    void VisualizerCommandUpdate();
    void VisualizerRender();
    void SwitchSection();

    UIManager* uiManager = nullptr;
    
    InputManager* inputManager = nullptr;
    EventManager* eventManager = nullptr;

    StateManager* stateManager = nullptr;
    AnimationManager* animationManager = nullptr;

    PlaybackController* playbackController = nullptr;
    PseudocodePanel* pseudocodePanel = nullptr;
};

#endif