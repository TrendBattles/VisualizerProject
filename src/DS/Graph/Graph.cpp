#include <DS/Graph/Graph.hpp>
#include <Graphics/Helper.hpp>

Graph::Graph() {
    memset(edgeWeight, -1, sizeof edgeWeight);
    memset(minDist, -1, sizeof minDist);
    for (int i = 0; i < MAX_NODES; ++i) {
        node[i].id = i + 1;
    }

    cameraOrigin = Vector2{(float) GetScreenWidth(), (float) GetScreenHeight()} * 0.5f;
}

Graph::~Graph() {
    if (stateManager != nullptr) 
        stateManager -> clearAllSnapshots(getDSID());
}

void Graph::clearAll() {
    for (int i = 0; i < MAX_NODES; ++i) {
        node[i].isActive = node[i].isPinned = node[i].isDragging = false;
    }

    memset(edgeWeight, -1, sizeof edgeWeight);
    holdIdx = -1;
    if (stateManager != nullptr) 
        stateManager -> clearAllSnapshots(getDSID());
}

std::string Graph::getDSID() const { return "Graph"; }

///////////////////////////////////////
///     SUPPORTIVE FUNCTIONS        ///
///////////////////////////////////////

/// @brief Removing nodes
bool Graph::removeNode(std::string value) {
    int valueParse = 0;
    try {
        valueParse = std::stoi(value);
    } catch (const std::invalid_argument& ia) {
	    std::cerr << "[INPUT ERROR]: " << ia.what() << '\n';
        return false;
    }

    removeNode(valueParse);
    return true;
}

/// @brief Inserting edges 
bool Graph::insertEdge(std::string FEdge, std::string SEdge, std::string weight) {
    int FEdgeV = 0, SEdgeV = 0, weightV = 0;
    try {
        FEdgeV = std::stoi(FEdge);
        SEdgeV = std::stoi(SEdge);
        weightV = std::stoi(weight);
    } catch (const std::invalid_argument& ia) {
	    std::cerr << "[INPUT ERROR]: " << ia.what() << '\n';
        return false;
    }

    setEdge(FEdgeV, SEdgeV, weightV);
    resetRefreshCounter();
    return true;
}
/// @brief Removing edges
bool Graph::removeEdge(std::string FEdge, std::string SEdge) {
    int FEdgeV = 0, SEdgeV = 0, weightV = 0;
    try {
        FEdgeV = std::stoi(FEdge);
        SEdgeV = std::stoi(SEdge);
    } catch (const std::invalid_argument& ia) {
	    std::cerr << "[INPUT ERROR]: " << ia.what() << '\n';
        return false;
    }

    setEdge(FEdgeV, SEdgeV, -1);
    resetRefreshCounter();
    return true;
}

bool Graph::runDijkstra(std::string start, std::string dest) {
    int u = 0, v = 0;
    try {
        u = std::stoi(start);
        v = std::stoi(dest);
    } catch (const std::invalid_argument& ia) {
	    std::cerr << "[INPUT ERROR]: " << ia.what() << '\n';
        return false;
    }

    Dijkstra(u, v);
    return true;
}

bool Graph::runKruskal() {
    Kruskal();
    return true;
}


///////////////////////////////
///     MEMBER FUNCTIONS    ///
///////////////////////////////

void Graph::setEdge(int u, int v, int w) {
    u -= 1; v -= 1;
    if (u == v || std::min(u, v) < 0 || std::max(u, v) >= MAX_NODES) {
        return;
    }   

    edgeWeight[u][v] = edgeWeight[v][u] = w;

    if (w == -1) return;

    insertNode(u);
    insertNode(v);
}
void Graph::insertNode(int u) {
    if (node[u].isActive) return;
    node[u].isActive = true;
    node[u].velocity = node[u].impactForce = Vector2{0.0f, 0.0f};

    node[u].position.x = Helper::random_gen(-50000, +50000) * 0.01f;
    node[u].position.y = Helper::random_gen(-50000, +50000) * 0.01f;
    resetRefreshCounter();
}
void Graph::removeNode(int u) {
    u -= 1;
    if (u < 0 || u >= MAX_NODES) return;
    if (!node[u].isActive) return;

    node[u].isActive = node[u].isDragging = node[u].isPinned = false;
    for (int v = 0; v < MAX_NODES; ++v) {
        edgeWeight[u][v] = edgeWeight[v][u] = -1;
    }

    resetRefreshCounter();
}

void Graph::Dijkstra(int u, int v) {
    u -= 1; v -= 1;
    if (std::min(u, v) < 0 || std::max(u, v) >= MAX_NODES) {
        return;
    }
    if (!node[u].isActive || !node[v].isActive) {
        return;
    }

    memset(minDist, 0x3f, sizeof minDist);
    memset(optimalPrev, -1, sizeof optimalPrev);

    generateSnapshot(
        0.5f,
        singleChange(Helper::nodeStringBuffer(node[u].id), Highlight::INSERTED),
        PseudocodeSection::DIJKSTRA,
        {1, 2, 3}
    );

    minDist[u] = 0;
    std::priority_queue <std::pair <int, int>, std::vector <std::pair <int, int>>, std::greater <std::pair <int, int>>> heap;
    heap.emplace(0, u);

    std::vector <std::pair <std::string, ChangeInfo>> changeList;
    generateSnapshot(
        0.3f,
        ChangeMap(),
        PseudocodeSection::DIJKSTRA,
        {4}
    );

    while (!heap.empty()) {
        int d, i; std::tie(d, i) = heap.top();
        heap.pop();
        
        generateSnapshot(
            0.3f,
            ChangeMap(changeList.begin(), changeList.end()),
            PseudocodeSection::DIJKSTRA,
            {5, 6}
        );

        if (d != minDist[i]) {
            generateSnapshot(
                0.3f,
                ChangeMap(changeList.begin(), changeList.end()),
                PseudocodeSection::DIJKSTRA,
                {7}
            );
            continue;
        }

        changeList.emplace_back(Helper::nodeStringBuffer(node[i].id), Highlight::ACTIVE);
        int nodeFlag = (int) changeList.size() - 1;

        generateSnapshot(
            0.3f,
            ChangeMap(changeList.begin(), changeList.end()),
            PseudocodeSection::DIJKSTRA,
            {}
        );

        for (int j = 0; j < MAX_NODES; ++j) {
            if (edgeWeight[i][j] == -1) continue;
            generateSnapshot(
                0.3f,
                ChangeMap(changeList.begin(), changeList.end()),
                PseudocodeSection::DIJKSTRA,
                {8}
            );

            int x = std::min(i, j), y = std::max(i, j);
            for (int t = 1; t <= 2; ++t) {
                std::string edgeBuffer = Helper::edgeStringBuffer(std::to_string(node[x].id), std::to_string(node[y].id) + "-" + std::to_string(t));
                changeList.emplace_back(edgeBuffer, Highlight::INSERTED);
            }
            
            generateSnapshot(
                0.3f,
                ChangeMap(changeList.begin(), changeList.end()),
                PseudocodeSection::DIJKSTRA,
                {9}
            );
            
            if (minDist[i] + edgeWeight[i][j] < minDist[j]) {
                minDist[j] = minDist[i] + edgeWeight[i][j];
                optimalPrev[j] = i;
                heap.emplace(minDist[j], j);

                changeList.emplace_back(Helper::nodeStringBuffer(node[j].id), Highlight::INSERTED);
                generateSnapshot(
                    0.5f,
                    ChangeMap(changeList.begin(), changeList.end()),
                    PseudocodeSection::DIJKSTRA,
                    {10, 11}
                );
                changeList.pop_back();
            }

            changeList.back().second = changeList[(int) changeList.size() - 2].second = Highlight::REMOVED;
            generateSnapshot(
                0.3f,
                ChangeMap(changeList.begin(), changeList.end()),
                PseudocodeSection::DIJKSTRA,
                {}
            );
        }

        changeList[nodeFlag].second = Highlight::REMOVED;
        generateSnapshot(
            0.5f,
            ChangeMap(changeList.begin(), changeList.end()),
            PseudocodeSection::DIJKSTRA,
            {4}
        );
    }

    generateSnapshot(
        0.3f,
        ChangeMap(),
        PseudocodeSection::DIJKSTRA,
        {12}
    );
    if (minDist[v] < inf_32 / 2) {
        changeList.clear();
        int x = v;

        while (x != -1) {
            changeList.emplace_back(Helper::nodeStringBuffer(node[x].id), Highlight::FOUND);
            generateSnapshot(
                0.3f,
                ChangeMap(changeList.begin(), changeList.end()),
                PseudocodeSection::DIJKSTRA,
                {13}
            );

            if (optimalPrev[x] != -1) {
                int q = std::min(x, optimalPrev[x]), r = std::max(x, optimalPrev[x]);

                for (int t = 1; t <= 2; ++t) {
                    std::string edgeBuffer = Helper::edgeStringBuffer(std::to_string(node[q].id), std::to_string(node[r].id) + "-" + std::to_string(t));
                    changeList.emplace_back(edgeBuffer, Highlight::FOUND);
                }
            }
            
            x = optimalPrev[x];
        }

        generateSnapshot(
            2.0f,
            ChangeMap(changeList.begin(), changeList.end())
        );
    }
    
    memset(minDist, -1, sizeof minDist);
    generateSnapshot(1.0f);
}
void Graph::Kruskal() {
    std::vector <std::tuple <int, int, int>> edges;
    for (int u = 0; u < MAX_NODES; ++u) {
        for (int v = u + 1; v < MAX_NODES; ++v) {
            if (edgeWeight[u][v] == -1) continue;

            edges.emplace_back(edgeWeight[u][v], u, v);
        }
    }

    sort(edges.begin(), edges.end());

    std::vector <int> root(MAX_NODES);
    std::iota(root.begin(), root.end(), 0);

    auto Find_Root = [&] (auto self, int u) -> int {
        return root[u] == u ? u : root[u] = self(self, root[u]);
    };

    generateSnapshot(
        0.5f,
        ChangeMap(),
        PseudocodeSection::KRUSKAL,
        {1, 2}
    );

    std::vector <std::pair <std::string, ChangeInfo>> changeList;
    for (auto& it : edges) {
        int u, v, w; std::tie(w, u, v) = it;
        for (int t = 1; t <= 2; ++t) {
            std::string edgeBuffer = Helper::edgeStringBuffer(std::to_string(node[u].id), std::to_string(node[v].id) + "-" + std::to_string(t));
            changeList.emplace_back(edgeBuffer, Highlight::MODIFIED);
        }

        generateSnapshot(
            0.3f,
            ChangeMap(changeList.begin(), changeList.end()),
            PseudocodeSection::KRUSKAL,
            {3}
        );
        generateSnapshot(
            0.3f,
            ChangeMap(changeList.begin(), changeList.end()),
            PseudocodeSection::KRUSKAL,
            {4}
        );
        if (Find_Root(Find_Root, u) != Find_Root(Find_Root, v)) {
            root[Find_Root(Find_Root, u)] = Find_Root(Find_Root, v);
            changeList.back().second = changeList[(int) changeList.size() - 2].second = Highlight::INSERTED;
            generateSnapshot(
                0.3f,
                ChangeMap(changeList.begin(), changeList.end()),
                PseudocodeSection::KRUSKAL,
                {5, 6}
            );
        } else {
            changeList.back().second = changeList[(int) changeList.size() - 2].second = Highlight::REMOVED;
            generateSnapshot(
                0.3f,
                ChangeMap(changeList.begin(), changeList.end()),
                PseudocodeSection::KRUSKAL,
                {}
            );
        }
    }
}

///////////////////////////////////
///     GRAPH FLOATING ALGO     ///
///////////////////////////////////

void Graph::update() {
    refreshLayout();
}
void Graph::reset() {
    for (int u = 0; u < MAX_NODES; ++u) {
        if (!node[u].isActive) continue;
        node[u].position.x = Helper::random_gen(-5000, +5000) * 0.01f;
        node[u].position.y = Helper::random_gen(-5000, +5000) * 0.01f;

        node[u].velocity = node[u].impactForce = Vector2{0.0f, 0.0f};
    }
    memset(minDist, -1, sizeof minDist);

    resetRefreshCounter();
}
void Graph::refreshLayout() {
    int activeCnt = 0;
    for (int i = 0; i < MAX_NODES; ++i) activeCnt += node[i].isActive;
    if (activeCnt == 0) return;

    const float k_electric = 1000.0f, k_spring = 0.5f;
    const float friction = 0.3f, gravityCoeff = 0.1f;
    const float idealSpringLength = 100.0f + 50.0f / activeCnt - 2.0f * activeCnt;

    Vector2 disp[MAX_NODES];
    for (int i = 0; i < MAX_NODES; ++i) disp[i] = Vector2{0.0f, 0.0f};

    // Electric Repulsion
    for (int u = 0; u < MAX_NODES; ++u) {
        if (!node[u].isActive) continue;

        for (int v = u + 1; v < MAX_NODES; ++v) {
            if (!node[v].isActive) continue;

            Vector2 deltaPosition = node[v].position - node[u].position;
            if (Vector2Length(deltaPosition) <= 0.1f) {
                deltaPosition = Vector2{idealSpringLength, idealSpringLength} * 0.5f;
            }

            float distSqr = Vector2Length(deltaPosition);
            
            if (!node[u].isDragging && !node[u].isPinned) disp[u] -= Vector2Normalize(deltaPosition) * k_electric / distSqr;
            if (!node[v].isDragging && !node[v].isPinned) disp[v] += Vector2Normalize(deltaPosition) * k_electric / distSqr;
        }
    }

    // Spring Force
    for (int u = 0; u < MAX_NODES; ++u) {
        for (int v = u + 1; v < MAX_NODES; ++v) {
            if (edgeWeight[u][v] == -1) continue;

            Vector2 deltaPosition = node[v].position - node[u].position;
            float dist = Vector2Length(deltaPosition);
            float deltaForce = k_spring * (dist - idealSpringLength);

            if (!node[u].isDragging && !node[u].isPinned) node[u].impactForce += Vector2Normalize(deltaPosition) * deltaForce;
            if (!node[u].isDragging && !node[u].isPinned) node[v].impactForce -= Vector2Normalize(deltaPosition) * deltaForce;
        }
    }

    // Gravity
    for (int u = 0; u < MAX_NODES; ++u) {
        if (!node[u].isActive || node[u].isDragging || node[u].isPinned) continue;

        disp[u] -= node[u].position * gravityCoeff;
    }

    const float deltaTime = GetFrameTime();
    for (int u = 0; u < MAX_NODES; ++u) {
        if (!node[u].isActive || node[u].isDragging || node[u].isPinned) continue;

        Vector2 acceleration = node[u].impactForce * (1.0f - friction);
        disp[u] += node[u].velocity * deltaTime + acceleration * 0.5f * deltaTime * deltaTime;

        node[u].velocity += acceleration * deltaTime;   
        node[u].impactForce = Vector2{0.0f, 0.0f};
    }

    // Heuristic displacement by using heat control
    for (int u = 0; u < MAX_NODES; ++u) {
        if (!node[u].isActive || node[u].isDragging || node[u].isPinned) continue;

        node[u].position += Vector2Normalize(disp[u]) * std::min(Vector2Length(disp[u]), temp);
        node[u].position = Vector2Clamp(node[u].position, cameraOrigin * -1.0f, cameraOrigin);
    }

    temp = std::max(temp * COOLING_CONSTANT, MIN_TEMP);
}
void Graph::resetRefreshCounter() {
    temp = MAX_TEMP;
}

bool Graph::processInput(RawInputEvent nextInput) {
    if (nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_HOLD) {
        if (holdIdx >= 0) {
            node[holdIdx].position = nextInput.position - cameraOrigin;
            return true;
        }

        if (holdIdx == -1) {
            holdIdx = -2;

            for (int i = 0; i < MAX_NODES; ++i) {
                if (!node[i].isActive) continue;

                Vector2 actualPosition = node[i].position + cameraOrigin;
                if (Vector2Distance(actualPosition, nextInput.position) <= radius + outlineSize) {
                    holdIdx = i;
                    break;
                }
            }

            if (holdIdx >= 0) node[holdIdx].isDragging = true;
            return holdIdx >= 0;
        }

        return false;
    }

    if (holdIdx >= 0) node[holdIdx].isDragging = false;
    holdIdx = -1;

    if (nextInput.inputType == RawInputEvent::InputType::LEFT_MOUSE_CLICKED) {
        bool triggered = false;
        for (int i = 0; i < MAX_NODES; ++i) {
            if (!node[i].isActive) continue;

            Vector2 actualPosition = node[i].position + cameraOrigin;
            if (Vector2Distance(actualPosition, nextInput.position) <= radius + outlineSize) {
                triggered = true;
                node[i].isPinned ^= 1;
                break;
            }
        }

        return triggered;
    }

    return false;
}

///////////////////////////////////
///     SNAPSHOT GENERATION     ///
///////////////////////////////////

Snapshot Graph::buildSnapshot(const ChangeMap& changeMap) {
    Snapshot storage;
    for (int i = 0; i < MAX_NODES; ++i) {
        if (!node[i].isActive) continue;
        
        ShapeState nodeShape = Helper::createCircle(
            Helper::nodeStringBuffer(node[i].id),
            node[i].position,
            radius, outlineSize, 
            Highlight::NORMAL.backgroundColor, Highlight::NORMAL.outlineColor,
            MAX_NODES - i
        );

        if (node[i].isPinned && minDist[i] < 0) {
            nodeShape.setColor(Highlight::PINNED.backgroundColor, Highlight::PINNED.outlineColor);
        }

        nodeShape.setText(Helper::createText(std::to_string(node[i].id), CoreFonts::Consolas, 25.0f, 2.5f, node[i].position, GetColor(0xEDF2F4FF)));
        nodeShape.setTextCenter(true);

        storage.emplace_back(nodeShape);
    }

    for (int i = 0; i < MAX_NODES; ++i) {
        if (!node[i].isActive || minDist[i] < 0) continue;

        std::string distLabel = minDist[i] >= inf_32 / 2 ? "inf" : std::to_string(minDist[i]);
        Vector2 size = MeasureTextEx(CoreFonts::Consolas, distLabel.c_str(), 15.0, 1.5f);
        Vector2 position = node[i].position - Vector2{0.0f, size.y + radius + outlineSize + 10.0f};

        ShapeState textShape;
        textShape.shapeID = "Dijkstra_" + std::to_string(node[i].id);
        textShape.layerID = MAX_NODES - i;
        textShape.setText(Helper::createText(distLabel, CoreFonts::Consolas, 15.0f, 1.5f, position, RED));
        textShape.setTextCenter(true);

        storage.emplace_back(textShape);
    }

    auto DrawEdge = [&] (Vector2 parentVec, Vector2 childVec, int u, int v) -> void {
        Vector2 normVec = Vector2Normalize(childVec - parentVec);

        Vector2 midpoint = (parentVec + childVec) * 0.5f;
        Vector2 size = MeasureTextEx(CoreFonts::Consolas, std::to_string(edgeWeight[u][v]).c_str(), 15.0f, 1.5f);
            
        ShapeState weightText = Helper::createRectangle(
            Helper::rectangleStringBuffer(std::to_string(node[u].id) + "-" + std::to_string(node[v].id)),
            midpoint - size * 0.5f,
            midpoint + size * 0.5f,
            0.0f,
            GetColor(0), GetColor(0),
            -10
        );

        weightText.setText(Helper::createText(std::to_string(edgeWeight[u][v]), CoreFonts::Consolas, 15.0f, 1.5f, midpoint, GetColor(0xEDF2F4FF)));
        weightText.setTextCenter(true);
        storage.emplace_back(weightText);

        ShapeState line1 = Helper::createLine(
            Helper::edgeStringBuffer(std::to_string(node[u].id), std::to_string(node[v].id) + "-1"),
            parentVec + normVec * (radius + outlineSize),
            midpoint - normVec * Vector2Length(size),

            5.0f, 2.5f, 
            GetColor(0x8D99AEFF), GetColor(0x4A4F60FF),
            -100
        );
        ShapeState line2 = Helper::createLine(
            Helper::edgeStringBuffer(std::to_string(node[u].id), std::to_string(node[v].id) + "-2"),
            midpoint + normVec * Vector2Length(size),
            childVec - normVec * (radius + outlineSize),

            5.0f, 2.5f, 
            GetColor(0x8D99AEFF), GetColor(0x4A4F60FF),
            -100
        );
        storage.emplace_back(line1);
        storage.emplace_back(line2);
    };

    for (int u = 0; u < MAX_NODES; ++u) {
        for (int v = u + 1; v < MAX_NODES; ++v) {
            if (edgeWeight[u][v] == -1) continue;

            DrawEdge(node[u].position, node[v].position, u, v);
        }
    }
    
    // Override the color information of the target shapes
    for (ShapeState& finalState : storage) {
        auto it = changeMap.find(finalState.shapeID);
        if (it == changeMap.end()) continue;

        finalState.color = it -> second.backgroundColor;
        finalState.outlineColor = it -> second.outlineColor;
        finalState.content.textColor = it -> second.textColor;
    }

    sort(storage.begin(), storage.end(), [&] (ShapeState& A, ShapeState& B) {
        return A.layerID < B.layerID;
    });
    return storage;
}

void Graph::generateSnapshot(float duration, ChangeMap changeMap, PseudocodeSection pseudoFrame, std::vector <int> pseudoActiveLines) {
    if (stateManager == nullptr) {
        std::cerr << "[ERROR]: State Manager not found\n";
        return;
    }

    stateManager -> addSnapshot(getDSID(), HistoryFrame{ buildSnapshot(changeMap), duration, pseudoFrame, pseudoActiveLines});
}
Snapshot Graph::getCurrentSnapshot() {
    return buildSnapshot(ChangeMap());
}