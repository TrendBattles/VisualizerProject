#include <AppLoop.hpp>

AppLoop::AppLoop() = default;
AppLoop::~AppLoop() {
    delete activeDS;
    delete uiManager;
    delete globalStateManager;
}

//////////////////////////////
///     MAIN FUNCTIONS     ///
//////////////////////////////

/// @brief Initalizing the app
void AppLoop::init() {
    // 1. Initialization
    const int screenWidth = 1200;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "C++ Data Structure Visualizer");

    // 2. Camera Setup
    camera.target = (Vector2){ 0, 0 };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);


    globalStateManager = new StateManager();
    uiManager = new UIManager(globalStateManager);
    activeDS = new AVL();
    activeDS -> setStateManager(globalStateManager);

    isTextBoxFocused = false;
    inputHitBox = ShapeState {
        Vector2{120.0f, (float) GetScreenHeight() - 60.0f},
        Vector2{220.0f, (float) GetScreenHeight()},
        ShapeType::RECTANGLE,
        -100.0f, 5.0f,
        Color {170, 170, 170, 255}, BLUE,
        0,
        Text {
            std::string(),
            GetFontDefault(),
            20.0f, 2.0f,
            BLACK,
            Vector2 {0.0f, 0.0f},
            true
        }
    };
    inputHitBox.content.position = Vector2Lerp(inputHitBox.startPosition, inputHitBox.endPosition, 0.5f);
}

/// @brief The main app workflow
void AppLoop::mainLoop() {
    // --- Update Logic ---
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 delta = GetMouseDelta();
        camera.target.x -= delta.x / camera.zoom;
        camera.target.y -= delta.y / camera.zoom;
    }
    camera.zoom += ((float)GetMouseWheelMove() * 0.1f);

    // --- Drawing Logic ---
    BeginDrawing();
        ClearBackground(GetColor(0x181818FF)); // Modern Dark Theme
        BeginMode2D(camera);
            uiManager -> renderSnapshot();
        EndMode2D();

        // Draw UI (which doesn't move with the camera)
        DrawFPS(10, 10);
        DrawText("Right-click to pan | Scroll to zoom", 10, 40, 20, GRAY);

        ShapeState button = {
            Vector2{0.0f, (float) GetScreenHeight() - 60.0f},
            Vector2{100.f, (float) GetScreenHeight()},
            ShapeType::RECTANGLE,
            -100.0f, 5.0f,
            Color {170, 170, 170, 255}, BLUE,
            0,
            Text {
                "Insert", 
                GetFontDefault(),
                20.0f, 5.0f,
                RED,
                Vector2 {0.0f, 0.0f},
                true
            }
        };
        button.content.position = Vector2Lerp(button.startPosition, button.endPosition, 0.5f);

        updateEvent();
        updateTextBox();

        uiManager -> drawShape(inputHitBox);
        uiManager -> drawShape(button);

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), createRaylibRectangle(button.startPosition, button.endPosition))) {
            if (!inputHitBox.content.label.empty()) activeDS -> insertNode(stoi(inputHitBox.content.label));
            inputHitBox.content.label.clear();
        }
    EndDrawing();
}


/// @brief Event Updates on the screen
void AppLoop::updateEvent() {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), createRaylibRectangle(inputHitBox.startPosition, inputHitBox.endPosition))) {
            isTextBoxFocused = true;
        } else {
            isTextBoxFocused = false;
        }
    }

    // Hovering the text box -> In typing state
    if (CheckCollisionPointRec(GetMousePosition(), createRaylibRectangle(inputHitBox.startPosition, inputHitBox.endPosition))) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);
    } else {
        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}

/// @brief Text Box Behaviors
void AppLoop::updateTextBox() {
    if (isTextBoxFocused) {
        int key = GetCharPressed();
        while (key > 0) {
            int currentLen = (int) inputHitBox.content.label.length();
            if (currentLen > 0 and currentLen >= 5 + (inputHitBox.content.label[0] == '-')) {
                key = GetCharPressed();
                continue;
            }

            // Only allow numbers for inputs
            if (key >= '0' && key <= '9') {
                inputHitBox.content.label.push_back((char) key);
            }
            if ((char) key == '-' and inputHitBox.content.label.find('-') == std::string::npos) {
                inputHitBox.content.label.push_back((char) key);
            }

            key = GetCharPressed();
        }
        
        if (IsKeyPressed(KEY_BACKSPACE) && !inputHitBox.content.label.empty()) {
            inputHitBox.content.label.pop_back();
        }
    }
}