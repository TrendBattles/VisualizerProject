#include <Core/AppLoop.hpp>
#include <Graphics/Helper.hpp>

AppLoop::AppLoop() = default;
AppLoop::~AppLoop() {
    delete activeDS;
    delete globalUIManager;
    delete eventManager;

    delete animationManager;
    delete globalStateManager;
}

//////////////////////////////
///     MAIN FUNCTIONS     ///
//////////////////////////////

/// @brief Initalizing the app
void AppLoop::init() {
    // Initialization
    const int screenWidth = 1200;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "C++ Data Structure Visualizer");

    globalUIManager = new UIManager();
    globalStateManager = new StateManager(globalUIManager -> getDSOptions());
    globalUIManager -> setStateManger(globalStateManager);
    
    activeDS = new AVL();
    activeDS -> setStateManager(globalStateManager);
    
    animationManager = new AnimationManager();
    animationManager -> setStateManager(globalStateManager);
    animationManager -> setUIManager(globalUIManager);

    eventManager = new EventManager();
    eventManager -> init();

    // Camera Setup
    camera.target = (Vector2){ 0, 0 };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);
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
    camera.zoom = std::max(std::min(camera.zoom, 3.0f), 0.25f);

    // --- Drawing Logic ---
    BeginDrawing();
        ClearBackground(GetColor(0x181818FF)); // Modern Dark Theme

        updateEvent();
        updateTextBox();

        BeginMode2D(camera);
            animationManager -> sendAnimationSignals(globalUIManager -> getScreenSection());
        EndMode2D();

        // Draw UI (which doesn't move with the camera)
        DrawFPS(10, 10);
        DrawText("Right-click to pan | Scroll to zoom", 10, 40, 20, GRAY);


    EndDrawing();
}


/// @brief Event Updates on the screen
void AppLoop::updateEvent() {
    for (int eventIdx = 0; eventIdx < eventManager -> getSize(); ++eventIdx) {
        if (eventManager -> isButtonHidden(eventIdx)) continue;

        globalUIManager -> drawShape(eventManager -> getEventShape(eventIdx));
        globalUIManager -> drawShape(eventManager -> getEventTextBox(eventIdx));
    }

    for (int eventIdx = 0; IsMouseButtonDown(MOUSE_LEFT_BUTTON) && eventIdx < eventManager -> getSize(); ++eventIdx) {
        if (eventManager -> isButtonHidden(eventIdx)) continue;
        
        ShapeState currentTextBox = eventManager -> getEventTextBox(eventIdx);

        if (CheckCollisionPointRec(GetMousePosition(), Helper::createRaylibRectangle(currentTextBox.startPosition, currentTextBox.endPosition))) {
            eventManager -> setFocused(eventIdx, true);
        } else {
            eventManager -> setFocused(eventIdx, false);
        }
    }

    if (!IsMouseButtonDown(MOUSE_LEFT_BUTTON)) return;

    if (!eventManager -> isButtonHidden("Insert")) {
        ShapeState insertButton = eventManager -> getEventShape("Insert");
        Text insertText = eventManager -> getEventTextBox("Insert").content;

        if (CheckCollisionPointRec(GetMousePosition(), Helper::createRaylibRectangle(insertButton.startPosition, insertButton.endPosition))) {
            if (!insertText.label.empty() && activeDS -> insertNode(insertText.label)) {
                animationManager -> setStartAnimation();
            }
            
            eventManager -> clearTextBox("Insert");
        }
    }  

    if (!eventManager -> isButtonHidden("Remove")) {
        ShapeState removeButton = eventManager -> getEventShape("Remove");
        Text removeText = eventManager -> getEventTextBox("Remove").content;

        if (CheckCollisionPointRec(GetMousePosition(), Helper::createRaylibRectangle(removeButton.startPosition, removeButton.endPosition))) {
            if (!removeText.label.empty() && activeDS -> removeNode(removeText.label)) {
                animationManager -> setStartAnimation();
            } 

            eventManager -> clearTextBox("Remove");
        }
    }   
}

/// @brief Text Box Behaviors
void AppLoop::updateTextBox() {
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    
    for (int eventIdx = 0; eventIdx < eventManager -> getSize(); ++eventIdx) {
        if (eventManager -> isTextBoxHidden(eventIdx)) continue;
        
        ShapeState currentTextBox = eventManager -> getEventTextBox(eventIdx);

        if (CheckCollisionPointRec(GetMousePosition(), Helper::createRaylibRectangle(currentTextBox.startPosition, currentTextBox.endPosition))) {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            break;
        }
    }
    
    int focusID = -1;
    for (int eventIdx = 0; eventIdx < eventManager -> getSize(); ++eventIdx) {
        if (eventManager -> isTextBoxHidden(eventIdx)) continue;

        if (eventManager -> getFocused(eventIdx)) {
            focusID = eventIdx;
            break;
        }
    }

    ShapeState inputHitBox = eventManager -> getEventTextBox(focusID);

    int key = -1;
    while ((key = GetCharPressed()) > 0) {
        if (focusID == -1) {
            continue;
        }

        
        int currentLen = (int) inputHitBox.content.label.length();
        if (currentLen >= 5) {
            continue;
        }

        // Only allow numbers for inputs
        if (key >= '0' && key <= '9') {
            inputHitBox.content.label.push_back((char) key);
        }
    }
        
    if (IsKeyPressed(KEY_BACKSPACE) && !inputHitBox.content.label.empty()) {
        inputHitBox.content.label.pop_back();
    }

    eventManager -> setEventTextBox(focusID, inputHitBox);
}