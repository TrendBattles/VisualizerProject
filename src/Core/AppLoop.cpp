#include <Core/AppLoop.hpp>
#include <Graphics/Helper.hpp>

AppLoop::AppLoop() = default;
AppLoop::~AppLoop() {
    delete playbackController; playbackController = nullptr;
    delete pseudocodePanel;    pseudocodePanel = nullptr;

    delete DataStructureUI;    DataStructureUI = nullptr;
    delete DataStructure;      DataStructure   = nullptr;
    delete eventManager;       eventManager    = nullptr;
    delete inputManager;       inputManager    = nullptr;
    delete animationManager;   animationManager = nullptr;
    delete stateManager;       stateManager    = nullptr;
    delete uiManager;          uiManager       = nullptr;

    CoreFonts::unload();
}

//////////////////////////////
///     MAIN FUNCTIONS     ///
//////////////////////////////

/// @brief Initalizing the app
void AppLoop::init() {
    // Initialization
    const int screenWidth = 1280;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "C++ Data Structure Visualizer");

    CoreFonts::load();

    options = {"AVL_Tree", "Trie", "Linked_List", "Hash_Table", "Graph"};
    appSection = "AVL_Tree";

    // Core Managers
    uiManager = new UIManager();
    stateManager = new StateManager(getDSOptions());

    animationManager = new AnimationManager();
    animationManager -> setStateManager(stateManager);

    DataStructure = new AVL();
    DataStructure -> setStateManager(stateManager);
    DataStructureUI = new AVLUI();
    DataStructureUI -> setUIManager(uiManager);

    eventManager = new EventManager();
    eventManager -> setAnimationManager(animationManager);
    eventManager -> setDSPointer(DataStructure);

    // Core: UI Interactions
    inputManager = new InputManager();
    playbackController = new PlaybackController();
    playbackController -> setAnimationManager(animationManager);
    
    pseudocodePanel = new PseudocodePanel();
    pseudocodePanel -> setAnimationManager(animationManager);
    pseudocodePanel -> setUIManager(uiManager);
    
    // Camera Setup
    camera.target = (Vector2){ 0, 0 };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);
}
    
/// @brief The main app workflow
void AppLoop::mainLoop() {
    if (isVisualizing()) {
        VisualizerLoop();
        return;
    }
}


const std::vector <std::string>& AppLoop::getDSOptions() const {
    return options;
}
void AppLoop::setScreenSection(const std::string& sectionID) {
    appSection = sectionID;
}
const std::string& AppLoop::getScreenSection() const {
    return appSection;
}
bool AppLoop::isVisualizing() const {
    return find(options.begin(), options.end(), appSection) != options.end();
}


void AppLoop::VisualizerLoop() {
    VisualizerInputHandling();
    VisualizerUpdate();
    VisualizerRender();
}

void AppLoop::VisualizerInputHandling() {
    inputManager -> update();

    // Input Updates 
    while (inputManager -> hasEvents()) {
        RawInputEvent nextInput = inputManager -> pollEvent();
    
        // Data Structure operation signal request
        CommandPattern commandRequest = DataStructureUI -> processInput(nextInput); 
        eventManager -> handleCommand(commandRequest);

        if (DataStructureUI -> getNavPhase() == IDataStructureUI::NavPhase::NAV_CLOSED) {
            // Playback Controller Input
            playbackController -> processInput(nextInput);

            // Pseudocode Panel Input
            pseudocodePanel -> processInput(nextInput);

            if (nextInput.keySignal == KeyboardKey::KEY_SPACE) {
                camera.target = {0.0f, 0.0f};
            }
        }
    }
}
void AppLoop::VisualizerUpdate() {
    // Camera Update
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 delta = GetMouseDelta();
        camera.target.x -= delta.x / camera.zoom;
        camera.target.y -= delta.y / camera.zoom;
    }
    camera.zoom += ((float)GetMouseWheelMove() * 0.1f);
    camera.zoom = std::max(std::min(camera.zoom, 3.0f), 0.25f);

    const std::string& DSTarget = getScreenSection();
    bool operationEnabled = !animationManager -> canStepForward(DSTarget)
                        &&  animationManager -> getTransitionCoeff() >= 0;

    if (!operationEnabled) {
        DataStructureUI -> disableAllOperations();
    } else {
        DataStructureUI -> enableAllOperations();
    }
    
    // Playback Update
    playbackController -> update(getScreenSection());

    // Data Structure UI update
    DataStructureUI -> update();
}
void AppLoop::VisualizerRender() {
    BeginDrawing();
        ClearBackground(GetColor(0x1F2937FF)); // Modern Dark Theme
        
        // Animated Data Structures
        BeginMode2D(camera);
            uiManager -> renderSnapshot(animationManager -> requestCurrentSnapshot(getScreenSection()));
        EndMode2D();

        // Overlays
        if (DataStructureUI -> getNavPhase() == IDataStructureUI::NavPhase::NAV_CLOSED) {
            pseudocodePanel -> render(getScreenSection());
            playbackController -> render();
        }

        DataStructureUI -> render();
        
        DrawFPS(10, 10);
        DrawText("Right-click to pan | Scroll to zoom", 10, 40, 20, WHITE);
    EndDrawing();
}
