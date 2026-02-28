#ifndef APPLOOP_HPP
#define APPLOOP_HPP

#include "raylib.h"
#include "raymath.h"
#include <DS/Tree/AVL.hpp>
#include <UIManager.hpp>
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
    AVL* activeDS = nullptr;

    bool isTextBoxFocused;
    ShapeState inputHitBox;
};

#endif