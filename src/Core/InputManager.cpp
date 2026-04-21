#include <Core/InputManager.hpp>

///////////////////////////////////
///     MOUSE CONSTRUCTION      ///
///////////////////////////////////

/// @brief This updates that the mouse button is being pressed.
///        After sufficient time, it pushes a HOLD event.
void MouseState::pressUpdate(std::queue <RawInputEvent>& inputQueue, bool isLeft) {
    if (!pressed) {
        pressed = true;
        timestamp = ChronoStart::now();
        position = GetMousePosition();
    }  

    ChronoClock currentTime = ChronoStart::now();
    std::chrono::duration <float> timeElapsed = currentTime - timestamp;

    if (timeElapsed.count() >= 0.25f) {
        RawInputEvent holdInput;
        holdInput.inputType = isLeft ? RawInputEvent::InputType::LEFT_MOUSE_HOLD : RawInputEvent::InputType::RIGHT_MOUSE_HOLD;
        holdInput.position = GetMousePosition();

        inputQueue.push(holdInput);
    }
}

/// @brief This updates that the mouse button has been released.
///        Processing cases where it's a CLICKED event or a RELEASED event
void MouseState::releaseUpdate(std::queue <RawInputEvent>& inputQueue, bool isLeft) {
    if (!pressed) return;

    pressed = false;
    ChronoClock currentTime = ChronoStart::now();
    std::chrono::duration <float> timeElapsed = currentTime - timestamp;
            
    if (timeElapsed.count() < 0.25f) {
        RawInputEvent clickedInput;
        clickedInput.inputType = isLeft ? RawInputEvent::InputType::LEFT_MOUSE_CLICKED : RawInputEvent::InputType::RIGHT_MOUSE_CLICKED;
        clickedInput.position = position;
        inputQueue.push(clickedInput);
    } else {
        RawInputEvent releasedInput;
        releasedInput.inputType = isLeft ? RawInputEvent::InputType::LEFT_MOUSE_RELEASED : RawInputEvent::InputType::RIGHT_MOUSE_RELEASED;
        releasedInput.position = GetMousePosition();
        inputQueue.push(releasedInput);
    }
}

void InputManager::handleBackspace() {
    RawInputEvent newInput;
    newInput.inputType = RawInputEvent::InputType::KEY_PRESSED;
    newInput.keySignal = KEY_BACKSPACE;

    if (IsKeyPressed(KEY_BACKSPACE)) {
        inputQueue.push(newInput);
        return;
    }

    if (IsKeyDown(KEY_BACKSPACE)) {
        backspaceTime += GetFrameTime();
        
        if (backspaceTime >= BACKSPACE_DELAY + BACKSPACE_INTERVAL) {
            inputQueue.push(newInput);
            inputQueue.push(newInput);
            backspaceTime = BACKSPACE_DELAY;
        }
    } else {
        backspaceTime = 0.0f;
    }
}
/// @brief Main Input Update
void InputManager::update() {
    if (GetMouseWheelMove() != 0) {
        RawInputEvent newInput;
        newInput.inputType = RawInputEvent::InputType::MOUSE_SCROLL;
        newInput.scrollDelta = GetMouseWheelMove();

        inputQueue.push(newInput);
    }

    KeyboardKey keyGet = (KeyboardKey) GetKeyPressed();
    if (keyGet != KEY_NULL && keyGet != KEY_BACKSPACE) {
        RawInputEvent newInput;
        newInput.inputType = RawInputEvent::InputType::KEY_PRESSED;
        newInput.keySignal = keyGet;

        inputQueue.push(newInput);
    }

    handleBackspace();

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        LeftMouse.pressUpdate(inputQueue, true);
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        LeftMouse.releaseUpdate(inputQueue, true);
    }

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
        RightMouse.pressUpdate(inputQueue, false);
    }
    if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON)) {
        RightMouse.releaseUpdate(inputQueue, false);
    }
}

bool InputManager::hasEvents() {
    return !inputQueue.empty();
}
RawInputEvent InputManager::pollEvent() {
    if (inputQueue.empty()) {
        return RawInputEvent();
    }

    RawInputEvent event = inputQueue.front();
    inputQueue.pop();
    return event;
}