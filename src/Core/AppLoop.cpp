#include <Core/AppLoop.hpp>
#include <Graphics/Helper.hpp>

AppLoop::AppLoop() = default;
AppLoop::~AppLoop() {
    delete DataStructureUI;  DataStructureUI = nullptr;
    delete DataStructure;    DataStructure   = nullptr;
    delete eventManager;     eventManager    = nullptr;
    delete inputManager;     inputManager    = nullptr;
    delete animationManager; animationManager = nullptr;
    delete stateManager;     stateManager    = nullptr;
    delete uiManager;        uiManager       = nullptr;
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

    uiManager = new UIManager();
    stateManager = new StateManager(uiManager -> getDSOptions());

    animationManager = new AnimationManager();
    animationManager -> setStateManager(stateManager);

    uiManager -> setStateManger(stateManager);
    uiManager -> setAnimationManager(animationManager);
    
    DataStructure = new AVL();
    DataStructure -> setStateManager(stateManager);
    DataStructureUI = new AVLUI();
    DataStructureUI -> setUIManager(uiManager);

    eventManager = new EventManager();
    eventManager -> setAnimationManager(animationManager);
    eventManager -> setDSPointer(DataStructure);

    inputManager = new InputManager();

    // Camera Setup
    camera.target = (Vector2){ 0, 0 };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);
}
    
/// @brief The main app workflow
void AppLoop::mainLoop() {
    if (uiManager -> isVisualizing()) {
        VisualizerLoop();
        return;
    }

    
}

void AppLoop::VisualizerLoop() {
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
        ClearBackground(GetColor(0x1F2937FF)); // Modern Dark Theme

        DataStructureUI -> render();
        
        VisualizerInputHandling();
        // updateEvent();
        // updateTextBox();

        BeginMode2D(camera);
            uiManager -> renderSnapshot();
        EndMode2D();

        // Draw UI (which doesn't move with the camera)
        DrawFPS(10, 10);
        DrawText("Right-click to pan | Scroll to zoom", 10, 40, 20, WHITE);
    EndDrawing();
}

void AppLoop::VisualizerInputHandling() {
    inputManager -> update();

    while (inputManager -> hasEvents()) {
        RawInputEvent nextInput = inputManager -> pollEvent();
        std::string commandRequest = DataStructureUI -> processInput(nextInput); 
        eventManager -> handleCommand(commandRequest);
    }
}