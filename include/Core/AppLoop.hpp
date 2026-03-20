#ifndef APPLOOP_HPP
#define APPLOOP_HPP

#include "raylib.h"
#include "raymath.h"
#include <DS/Tree/AVL.hpp>
#include <Graphics/UIManager.hpp>
#include <Core/EventManager.hpp>
#include <iostream>

class AppLoop {
public:
    AppLoop();
    ~AppLoop();
    void init();
    void mainLoop();

    void updateEvent();
    void updateTextBox();
private:
    Camera2D camera;
    StateManager* globalStateManager = nullptr;
    UIManager* uiManager = nullptr;
    IDataStructure* activeDS = nullptr;
    EventManager* eventManager = nullptr;
};

#endif