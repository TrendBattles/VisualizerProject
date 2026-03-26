#ifndef APPLOOP_HPP
#define APPLOOP_HPP

#include "raylib.h"
#include "raymath.h"

#include <Core/EventManager.hpp>
#include <Core/AnimationManager.hpp>

#include <DS/Tree/AVL.hpp>

#include <Graphics/UIManager.hpp>

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
    UIManager* globalUIManager = nullptr;

    EventManager* eventManager = nullptr;
    StateManager* globalStateManager = nullptr;
    AnimationManager* animationManager = nullptr;

    IDataStructure* activeDS = nullptr;
};

#endif