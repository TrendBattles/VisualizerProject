#include <DS/Tree/Trie/Trie.hpp>

#include <Graphics/Panels/HighlightTheme.hpp>
#include <Graphics/Helper.hpp>

TrieNode::TrieNode() {
    for (int i = 0; i < 10; ++i) child[i] = nullptr;
}

Trie::Trie() {
    root = new TrieNode();
    root -> prefixCnt = 0;
}
Trie::~Trie() {
    if (stateManager != nullptr)
        stateManager -> clearAllSnapshots(getDSID());

    destroyTree(root);
    root = nullptr;
}
void Trie::setStateManager(StateManager* source) {
    stateManager = source;
    generateSnapshot(0.0f);
}

void Trie::destroyTree(TrieNode* node) {
    if (node == nullptr) return;
    for (int i = 0; i < 10; ++i) {
        destroyTree(node -> child[i]);
    }

    delete node;
}
std::string Trie::getDSID() const {
    return "Trie";
}

bool Trie::isDeadNode(TrieNode* node) {
    if (node == nullptr) return true;
    if (node -> prefixCnt != 0) return false;
    for (int i = 0; i < 10; ++i) {
        if (node -> child[i] != nullptr) return false;
    }
    return true;
}

////////////////////////////////////
///     REPONSIVE FUNCTIONS      ///
////////////////////////////////////
void Trie::initDS(const std::vector <std::string>& rawValue) {
    clearAll();
    
    for (const std::string& value : rawValue) {
        insertNode(value);
    }

    if (stateManager != nullptr)
        stateManager -> clearAllSnapshots(getDSID());
    
    generateSnapshot(0.0f);
}

bool Trie::insertNode(std::string value) {
    insertString(value);
    return true;
}
bool Trie::removeNode(std::string value) {
    removeString(value);
    return true;
}
bool Trie::searchNode(std::string value) {
    return searchString(value);
}

void Trie::clearAll() {
    destroyTree(root);
    root = new TrieNode();
    root -> prefixCnt = 0;
    
    if (stateManager != nullptr)
        stateManager -> clearAllSnapshots(getDSID());
    generateSnapshot(0.0f);
}

////////////////////////////////  
///     INSERT FUNCTIONS     ///
////////////////////////////////

void Trie::insertString(std::string value) {
    insert(root, value, 0);

    generateSnapshot(1.0f);
}

void Trie::insert(TrieNode* node, const std::string& value, int idx) {
    generateSnapshot(
        0.3f,
        singleChange(Helper::nodeStringBuffer(value.substr(0, idx)), Highlight::ACTIVE),
        PseudocodeSection::TRIE_INSERT,
        {1}
    );
    if (idx == (int) value.length()) {
        node -> prefixCnt += 1;
        root -> prefixCnt += 1;
        
        generateSnapshot(
            0.5f,
            singleChange(Helper::nodeStringBuffer(value.substr(0, idx)), Highlight::INSERTED),
            PseudocodeSection::TRIE_INSERT,
            {2, 3}   
        );
        return;
    }

    generateSnapshot(
        0.3f,
        singleChange(Helper::nodeStringBuffer(value.substr(0, idx)), Highlight::ACTIVE),
        PseudocodeSection::TRIE_INSERT,
        {4}
    );

    int numID = value[idx] - '0';
    TrieNode*& nxtNode = node -> child[numID];

    generateSnapshot(
        0.3f,
        singleChange(Helper::nodeStringBuffer(value.substr(0, idx)), Highlight::ACTIVE),
        PseudocodeSection::TRIE_INSERT,
        {5}
    );

    if (nxtNode == nullptr) {
        generateSnapshot(
            0.3f,
            singleChange(Helper::nodeStringBuffer(value.substr(0, idx)), Highlight::MODIFIED),
            PseudocodeSection::TRIE_INSERT,
            {6}
        );

        nxtNode = new TrieNode();
    }

    generateSnapshot(
        0.3f,
        singleChange(Helper::nodeStringBuffer(value.substr(0, idx)), Highlight::MODIFIED),
        PseudocodeSection::TRIE_INSERT,
        {7}
    );

    insert(nxtNode, value, idx + 1);
}

////////////////////////////////  
///     REMOVE FUNCTIONS     ///
////////////////////////////////

TrieNode* Trie::remove(TrieNode* node, const std::string& value, int idx) {
    generateSnapshot(
        0.3f,
        node == nullptr ? ChangeMap() : singleChange(Helper::nodeStringBuffer(value.substr(0, idx)), Highlight::ACTIVE),
        PseudocodeSection::TRIE_REMOVE,
        {1}    
    );
    if (node == nullptr) {
        generateSnapshot(
            0.3f,
            ChangeMap(),
            PseudocodeSection::TRIE_REMOVE,
            {2}    
        );

        return nullptr;
    }

    generateSnapshot(
        0.3f,
        singleChange(Helper::nodeStringBuffer(value.substr(0, idx)), Highlight::ACTIVE),
        PseudocodeSection::TRIE_REMOVE,
        {3}
    );

    if (idx == (int) value.length()) {
        node -> prefixCnt -= 1;
        root -> prefixCnt -= 1;

        generateSnapshot(
            0.3f,
            node == nullptr ? ChangeMap() : singleChange(Helper::nodeStringBuffer(value.substr(0, idx)), Highlight::REMOVED),
            PseudocodeSection::TRIE_REMOVE,
            {4, 5}    
        );

        return node;
    }

    generateSnapshot(
        0.3f,
        node == nullptr ? ChangeMap() : singleChange(Helper::nodeStringBuffer(value.substr(0, idx)), Highlight::ACTIVE),
        PseudocodeSection::TRIE_REMOVE,
        {6}    
    );

    int numID = value[idx] - '0';
    TrieNode*& nxtNode = node -> child[numID];
    
    generateSnapshot(
        0.3f,
        node == nullptr ? ChangeMap() : singleChange(Helper::nodeStringBuffer(value.substr(0, idx)), Highlight::ACTIVE),
        PseudocodeSection::TRIE_REMOVE,
        {7}    
    );

    if (nxtNode == nullptr) {
        generateSnapshot(
            0.3f,
            node == nullptr ? ChangeMap() : singleChange(Helper::nodeStringBuffer(value.substr(0, idx)), Highlight::INACTIVE),
            PseudocodeSection::TRIE_REMOVE,
            {8}    
        );

        return node;
    }

    generateSnapshot(
        0.3f,
        node == nullptr ? ChangeMap() : singleChange(Helper::nodeStringBuffer(value.substr(0, idx)), Highlight::ACTIVE),
        PseudocodeSection::TRIE_REMOVE,
        {9}    
    );
    nxtNode = remove(nxtNode, value, idx + 1);

    generateSnapshot(
        0.3f,
        singleChange(Helper::nodeStringBuffer(value.substr(0, idx)), Highlight::INACTIVE),
        PseudocodeSection::TRIE_REMOVE,
        {10}    
    );

    if (isDeadNode(nxtNode)) {
        generateSnapshot(
            0.3f,
            singleChange(Helper::nodeStringBuffer(value.substr(0, idx + 1)), Highlight::REMOVED),
            PseudocodeSection::TRIE_REMOVE,
            {11}     
        );

        delete nxtNode;
        nxtNode = nullptr;
    }

    generateSnapshot(
        0.3f,
        singleChange(Helper::nodeStringBuffer(value.substr(0, idx)), Highlight::MODIFIED),
        PseudocodeSection::TRIE_REMOVE,
        {12}    
    );

    return node;
}
void Trie::removeString(std::string value) {
    root = remove(root, value, 0);

    generateSnapshot(1.0f);
}

////////////////////////////////  
///     SEARCH FUNCTIONS     ///
////////////////////////////////

bool Trie::search(TrieNode* node, const std::string& value, int idx, std::vector <std::pair <std::string, ChangeInfo>>& changeInfoList) {
    if (node != nullptr) {
        changeInfoList.emplace_back(Helper::nodeStringBuffer(value.substr(0, idx)), Highlight::ACTIVE);
    }

    generateSnapshot(
        0.3f,
        ChangeMap(changeInfoList.begin(), changeInfoList.end()),
        PseudocodeSection::TRIE_SEARCH,
        {1}
    );

    if (node == nullptr) {
        generateSnapshot(
            1.0f,
            ChangeMap(),
            PseudocodeSection::TRIE_SEARCH,
            {2}
        );
        return false;
    }

    generateSnapshot(
        0.3f,
        ChangeMap(changeInfoList.begin(), changeInfoList.end()),
        PseudocodeSection::TRIE_SEARCH,
        {3}
    );

    if (idx == (int) value.length()) {
        for (auto& it : changeInfoList) it.second = Highlight::FOUND;

        generateSnapshot(
            1.5f,
            node -> prefixCnt == 0 ? ChangeMap() : ChangeMap(changeInfoList.begin(), changeInfoList.end()),
            PseudocodeSection::TRIE_SEARCH,
            {4}
        );

        if (node -> prefixCnt == 0) return false;
        return true;
    }

    generateSnapshot(
        0.3f,
        ChangeMap(changeInfoList.begin(), changeInfoList.end()),
        PseudocodeSection::TRIE_SEARCH,
        {5, 6}
    );

    int numID = value[idx] - '0';
    TrieNode* nxtNode = node -> child[numID];

    return search(nxtNode, value, idx + 1, changeInfoList);
}
bool Trie::searchString(std::string value) {
    std::vector <std::pair <std::string, ChangeInfo>> changeInfoList;
    return search(root, value, 0, changeInfoList);
}


///////////////////////////////////
///         SNAPSHOTS           ///
///////////////////////////////////

/// @brief Builds a Snapshot of the current tree state without touching the StateManager.
///        Separates layout/serialization from persistence so that callers
///        can decorate the result before committing it.
Snapshot Trie::buildSnapshot(const ChangeMap& changeMap) {
    const float radius = 25.0f, outlineSize = 5.0f, lengthVertical = 120.0f;
    const float padding = 20.0f;

    // Calculating the position of the nodes efficiently
    // Expected time complexity: O(N)
    // Approach: InOrder Traversal

    Snapshot storage;
    auto DrawTree = [&] (auto self, TrieNode* node, int& nodeCnt, int depth, std::string numStr = "") -> Vector2 {
        if (node == nullptr) return Vector2{0, 0};

        // Drawing edges
        // A child always has exactly one parent, so this ID is stable across
        // rotations - the edge will smoothly interpolate its endpoints
        // rather than blinking out and back in with a new parent-keyed ID.
        auto DrawEdge = [&] (Vector2 parentVec, Vector2 childVec, int childValue) -> void {
            Vector2 normVec = Vector2Normalize(childVec - parentVec);

            ShapeState arrow = Helper::createLine(
                Helper::arrowPointStringBuffer(numStr + std::to_string(childValue)),
                parentVec + normVec * (radius + outlineSize),
                childVec - normVec * (radius + outlineSize),
                3.0f, 1.5f, 
                GetColor(0x8D99AEFF), GetColor(0x4A4F60FF),
                -100
            );
                
            storage.emplace_back(arrow);

            Vector2 midpoint = (parentVec + childVec) * 0.5f;
            Vector2 size = MeasureTextEx(CoreFonts::Consolas, std::to_string(childValue).c_str(), 20.0f, 2.0f);
            
            ShapeState digit = Helper::createRectangle(
                Helper::rectangleStringBuffer(numStr + std::to_string(childValue)),
                midpoint - size * 0.5f,
                midpoint + size * 0.5f,
                0.0f,
                GetColor(0x1F2937FF), GetColor(0),
                -10
            );

            digit.setText(Helper::createText(std::to_string(childValue), CoreFonts::Consolas, 20.0f, 2.0f, midpoint, GetColor(0xEDF2F4FF)));
            digit.setTextCenter(true);

            storage.emplace_back(digit);
        };

        Vector2 parentVec = {0, 0};
        std::vector <std::pair <int, Vector2>> childVecSet;

        int flagCnt = nodeCnt;
        for (int i = 0; i < 10; ++i) {
            if (node -> child[i] == nullptr) continue;

            Vector2 childVec = self(self, node -> child[i], nodeCnt, depth + 1, numStr + std::to_string(i));
            childVecSet.emplace_back(i, childVec);
        }

        nodeCnt += childVecSet.empty();
        int subtreeCnt = nodeCnt - flagCnt;
        parentVec = {(subtreeCnt - 1) * 0.5f * (2 * radius + 2 * outlineSize + padding), lengthVertical * depth};
        parentVec.x += flagCnt * (2 * radius + 2 * outlineSize + padding);
        
        for (auto data : childVecSet) {
            DrawEdge(parentVec, data.second, data.first);
        }
        
        //Node's sprite
        ShapeState nodeShape = Helper::createCircle(
            Helper::nodeStringBuffer(numStr),
            parentVec,
            radius, outlineSize, 
            Highlight::NORMAL.backgroundColor, Highlight::NORMAL.outlineColor,
            0
        );

        nodeShape.setText(Helper::createText(std::to_string(node -> prefixCnt), CoreFonts::Consolas, 20.0f, 2.0f, parentVec, GetColor(0xEDF2F4FF)));
        nodeShape.setTextCenter(true);

        storage.emplace_back(nodeShape);
        return parentVec;
    };

    int nodeCnt = 0;
    // Offset all positions so the root sits near the top-centre of the screen.
    // Nodes are appended last in the storage, so we can iterate the whole
    // list once without a second pass.
    Vector2 rootPosition = {0, -GetScreenHeight() / 2.0f + radius * 2};
    Vector2 transitDelta = rootPosition - DrawTree(DrawTree, root, nodeCnt, 0);

    for (ShapeState& tempoState : storage) {
        tempoState.positionTransitAllBy(transitDelta);
    }

    // Override the color information of the target shapes
    for (ShapeState& finalState : storage) {
        auto it = changeMap.find(finalState.shapeID);
        if (it == changeMap.end()) continue;

        finalState.color = it -> second.backgroundColor;
        finalState.outlineColor = it -> second.outlineColor;
        finalState.content.textColor = it -> second.textColor;
    }

    return storage;
}

void Trie::generateSnapshot(float duration, ChangeMap changeMap, PseudocodeSection pseudoFrame, std::vector <int> pseudoActiveLines) {
    if (stateManager == nullptr) {
        std::cerr << "[ERROR]: State Manager not found\n";
        return;
    }

    stateManager -> addSnapshot(getDSID(), HistoryFrame{ buildSnapshot(changeMap), duration, pseudoFrame, pseudoActiveLines});
}