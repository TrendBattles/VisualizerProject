#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include <Common.hpp>
#include <queue>

struct RawInputEvent {
    enum class InputType {
        EMPTY,
        LEFT_MOUSE_CLICKED,
        RIGHT_MOUSE_CLICKED,
        LEFT_MOUSE_RELEASED,
        RIGHT_MOUSE_RELEASED,
        LEFT_MOUSE_HOLD,
        RIGHT_MOUSE_HOLD,

        KEY_PRESSED,

        MOUSE_SCROLL,
    };

    InputType inputType = InputType::EMPTY;
    Vector2 position = {0, 0};
    KeyboardKey keySignal = KEY_NULL;
    float scrollDelta = 0.0f;
};

struct MouseState {
    ChronoClock timestamp;
    bool pressed = false;
    Vector2 position = {0, 0};

    void pressUpdate(std::queue <RawInputEvent>& inputQueue, bool isLeft);
    void releaseUpdate(std::queue <RawInputEvent>& inputQueue, bool isLeft);
};

class InputManager {
public:
    void update();

    bool hasEvents();
    RawInputEvent pollEvent();
private:
    std::queue <RawInputEvent> inputQueue;

    MouseState LeftMouse, RightMouse;
};
#endif