#include <Core/AppLoop.hpp>
#include <Graphics/Helper.hpp>

AppLoop::AppLoop() = default;
AppLoop::~AppLoop() {
    delete playbackController; playbackController = nullptr;
    delete pseudocodePanel;    pseudocodePanel = nullptr;

    for (auto& it : DSPackage) {
        delete it.second.DataStructure;
        delete it.second.DataStructureUI;
    }

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

    options = {"AVL_Tree", "Trie", "Linked_List", "Hash_Table", "Dijkstra", "Kruskal"};
    appSection = "AVL_Tree";

    // Core Managers
    uiManager = new UIManager();
    stateManager = new StateManager(getDSOptions());

    // Camera Setup
    Camera2D camera;
    camera.target = (Vector2){ 0, 0 };
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    DSPackage[options[0]] = {
        camera,
        new AVL(),
        new AVLUI()
    };
    DSPackage[options[1]] = {
        camera,
        new Trie(),
        new TrieUI()
    };
    for (auto& it : DSPackage) {
        it.second.DataStructure -> setStateManager(stateManager);
        it.second.DataStructureUI -> setUIManager(uiManager);
    }

    animationManager = new AnimationManager();
    animationManager -> setStateManager(stateManager);    

    eventManager = new EventManager();
    eventManager -> setAnimationManager(animationManager);

    if (isVisualizing()) {
        eventManager -> setDSPointer(DSPackage[appSection].DataStructure);
        animationManager -> resetAnimationTimer();
    }
    

    // Core: UI Interactions
    inputManager = new InputManager();
    playbackController = new PlaybackController();
    playbackController -> setAnimationManager(animationManager);
    
    pseudocodePanel = new PseudocodePanel();
    pseudocodePanel -> setAnimationManager(animationManager);
    pseudocodePanel -> setUIManager(uiManager);
    
    
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

    SwitchSection();
}

void AppLoop::VisualizerInputHandling() {
    inputManager -> update();

    Camera2D& camera = DSPackage[appSection].camera;
    IDataStructure* DataStructure = DSPackage[appSection].DataStructure;
    IDataStructureUI* DataStructureUI = DSPackage[appSection].DataStructureUI;

    // Input Updates 
    while (inputManager -> hasEvents()) {
        RawInputEvent nextInput = inputManager -> pollEvent();
    
        // Data Structure operation signal request
        CommandPattern commandRequest = DataStructureUI -> processInput(nextInput); 
        if (!commandRequest.prefix.empty()) {
            eventManager -> pushToQueue(commandRequest);
            continue;
        }

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

/// @brief Updating the logical memebers
void AppLoop::VisualizerUpdate() {
    Camera2D& camera = DSPackage[appSection].camera;
    IDataStructure* DataStructure = DSPackage[appSection].DataStructure;
    IDataStructureUI* DataStructureUI = DSPackage[appSection].DataStructureUI;
    
    // Camera Update
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        Vector2 delta = GetMouseDelta();
        camera.target.x -= delta.x / camera.zoom;
        camera.target.y -= delta.y / camera.zoom;
    }
    camera.zoom += ((float)GetMouseWheelMove() * 0.1f);
    camera.zoom = std::max(std::min(camera.zoom, 3.0f), 0.25f);

    VisualizerCommandUpdate();
    // Interaction with DS Options
    const std::string& DSTarget = getScreenSection();
    bool operationEnabled = animationManager -> allowsOperations(DSTarget);

    if (!operationEnabled) {
        DataStructureUI -> disableAllOperations();
    } else {
        DataStructureUI -> enableAllOperations();
    }
    
    // Playback Update
    playbackController -> update(getScreenSection());
    // Data Structure UI update
    DataStructureUI -> update();
    // Pseudocode Panel update
    pseudocodePanel -> update();
}

/// @brief Requesting the events one by one to counter race conditions
void AppLoop::VisualizerCommandUpdate() {
    if (!eventManager -> hasPendingCommands()) return;

    CommandPattern commandRequest = eventManager -> peekNextCommand();
    if (commandRequest.prefix == "NAVIGATE") {
        pendingSection = commandRequest.property;
        eventManager -> popNextCommand();
        return;
    }

    const std::string& DSTarget = getScreenSection();
    bool operationEnabled = animationManager -> allowsOperations(DSTarget);

    if (operationEnabled) {
        commandRequest = eventManager -> popNextCommand();
        eventManager -> handleCommand(commandRequest);
    }
}

void AppLoop::VisualizerRender() {
    Camera2D& camera = DSPackage[appSection].camera;
    IDataStructure* DataStructure = DSPackage[appSection].DataStructure;
    IDataStructureUI* DataStructureUI = DSPackage[appSection].DataStructureUI;
    
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

/// @brief If there is a request for changing sections to Menu/Settings/Other data structures,
///        it will be solved in this function after the rendering to ensure no corruptions occur
///        during the animation
void AppLoop::SwitchSection() {
    if (pendingSection.empty()) return;
    for (char& x : pendingSection) {
        if (x == ' ') x = '_';
    }

    if (DSPackage.count(pendingSection)) {
        appSection = pendingSection;
        eventManager -> setDSPointer(DSPackage[appSection].DataStructure);

        animationManager -> setSnapshotIdx(appSection, animationManager -> getSize(appSection) - 1);
        animationManager -> resetAnimationTimer();
    }

    pendingSection = "";
}