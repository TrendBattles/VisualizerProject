#include <DS/Tree/AVL.hpp>
#include <Graphics/Helper.hpp>

#include <algorithm>
#include <iostream>

AVL::AVL() {
    root = nullptr;
}

AVL::~AVL() {
    destroyTree(root);
    root = nullptr;
}

void AVL::destroyTree(AVLNode* node) {
    if (node == nullptr) return;

    destroyTree(node -> leftChild);
    destroyTree(node -> rightChild);
    
    delete node;
}

/////////////////////////////////
///  REPONSIVE FUNCTIONS      ///
/////////////////////////////////
bool AVL::insertNode(std::string value) {
    int valueParse = 0;
    try {
        valueParse = std::stoi(value);
    } catch (const std::invalid_argument& ia) {
	    std::cerr << "[INPUT ERROR]: " << ia.what() << '\n';
        return false;
    }

    if (abs(valueParse) >= 10'000) return false;

    insertNode(valueParse);
    return true;
}
bool AVL::removeNode(std::string value) {
    int valueParse = 0;
    try {
        valueParse = std::stoi(value);
    } catch (const std::invalid_argument& ia) {
	    std::cerr << "[INPUT ERROR]: " << ia.what() << '\n';
        return false;
    }

    if (abs(valueParse) >= 10'000) return false;

    removeNode(valueParse);
    return true;
}
void AVL::clearAll() {
    destroyTree(root);
    root = nullptr;

    stateManager -> clearAllSnapshots("AVL Tree");
}

void AVL::insertNode(int value) {
    // generateSnapshot(); // snapshot the state before insertion
    root = insert(root, value);
    generateSnapshot(1.0f); // snapshot the final state after insertion
}
void AVL::removeNode(int value) {
    // generateSnapshot(); // snapshot the state before removal
    root = remove(root, value);
    generateSnapshot(1.0f); // snapshot the final state after removal
}

/// @brief Builds a Snapshot of the current tree state without touching the StateManager.
///        Separates layout/serialization from persistence so that callers
///        can decorate the result before committing it.
Snapshot AVL::buildSnapshot() {
    if (root == nullptr) return Snapshot();

    const float radius = 30.0f, outlineSize = 3.0f, lengthVertical = 100;
    const float padding = 5.0f;

    // Calculating the position of the nodes efficiently
    // Expected time complexity: O(N)
    // Approach: InOrder Traversal
    Snapshot storage;
    auto DrawTree = [&] (auto self, AVLNode* node, int& nodeCnt, int depth) -> Vector2 {
        if (node == nullptr) return Vector2{0, 0};

        // Drawing edges
        // A child always has exactly one parent, so this ID is stable across
        // rotations - the edge will smoothly interpolate its endpoints
        // rather than blinking out and back in with a new parent-keyed ID.
        auto DrawEdge = [&] (Vector2 parentVec, Vector2 childVec, int childValue) -> void {
            Vector2 normVec = Vector2Normalize(childVec - parentVec);

            ShapeState arrow = Helper::createArrow(
                "arrow_" + std::to_string(childValue),
                parentVec + normVec * (radius + outlineSize),
                childVec - normVec * (radius + outlineSize),
                3.0f, 1.5f, -1000
            );
            arrow.setColor(GREEN, RED);

            storage.emplace_back(arrow);
        };

        //Left Edge
        Vector2 parentVec = {0, 0};
        if (node -> leftChild != nullptr) {
            Vector2 childVec = self(self, node -> leftChild, nodeCnt, depth + 1);
            parentVec = {nodeCnt * (radius + outlineSize + padding), lengthVertical * depth};

            DrawEdge(parentVec, childVec, node -> leftChild -> key);
        }

        parentVec = {nodeCnt * (radius + outlineSize + padding), lengthVertical * depth};
        nodeCnt += 1;

        //Right Edge
        if (node -> rightChild != nullptr) {
            Vector2 childVec = self(self, node -> rightChild, nodeCnt, depth + 1);
            DrawEdge(parentVec, childVec, node -> rightChild -> key);
        }

        //Node's sprite
        ShapeState nodeShape = Helper::createCircle(
            Helper::nodeStringBuffer(std::to_string(node -> key)),
            parentVec,
            radius, outlineSize, 
            0
        );

        nodeShape.setColor(WHITE, YELLOW);
        nodeShape.setText(Helper::createText(std::to_string(node -> key), GetFontDefault(), 20.0f, 2.0f, parentVec, BLACK));
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

    return storage;
}

void AVL::generateSnapshot(float duration) {
    if (stateManager == nullptr) {
        std::cerr << "[ERROR]: State Manager not found\n";
        return;
    }

    stateManager -> addSnapshot("AVL Tree", HistoryFrame{ buildSnapshot(), duration });
}


///////////////////////////////////////
///       SUPPRORTIVE FUNCTIONS     ///
//////////////////////////////////////
int AVL::getHeight(AVLNode* node) {
    return node == nullptr ? -1 : node -> height;
}
void AVL::update(AVLNode* node) {
    if (node == nullptr) return;

    node -> height = 1 + std::max(getHeight(node -> leftChild), getHeight(node -> rightChild));
}
int AVL::balanceFactor(AVLNode* node) {
    return node == nullptr ? 0 : getHeight(node -> leftChild) - getHeight(node -> rightChild);
}

/// @brief Left Rotation
/// @param node 
/// @return node
AVLNode* AVL::rotateLeft(AVLNode* node) {
    if (node == nullptr) return node;

    auto R = node -> rightChild;
    auto RL = R -> leftChild;

    node -> rightChild = RL;
    R -> leftChild = node;

    update(node);
    update(R);

    return R;
}

/// @brief Right rotation
/// @param node 
/// @return node
AVLNode* AVL::rotateRight(AVLNode* node) {
    if (node == nullptr) return node;

    auto L = node -> leftChild;
    auto LR = L -> rightChild;

    node -> leftChild = LR;
    L -> rightChild = node;

    update(node);
    update(L);

    return L;
}

/// @brief Rebalancing the subtree by LL, LR, RL, RR rotations
/// @param node 
/// @return node
AVLNode* AVL::rebalance(AVLNode* node) {
    if (node == nullptr) return node;

    update(node);

    int balance = balanceFactor(node);
    //Only snapshot when a rotation occurred.

    if (balance > +1) { // Left-Heavy
        if (balanceFactor(node -> leftChild) < 0) {
            node -> leftChild = rotateLeft(node -> leftChild);
            generateSnapshot(1.5f);
        }

        node = rotateRight(node);
    } else if (balance < -1) { // Right-Heavy
        if (balanceFactor(node -> rightChild) > 0) {
            node -> rightChild = rotateRight(node -> rightChild);
            generateSnapshot(1.5f);
        }

        node = rotateLeft(node);
    }

    return node;
}

/// @brief Insert a new key to the tree
/// @param node 
/// @param key 
/// @return node (the updated tree)
AVLNode* AVL::insert(AVLNode* node, int key) {
    if (node == nullptr) {
        node = new AVLNode(key);
        return node;
    }

    if (node -> key == key) {
        return node;
    }

    bool recentlyAdded = false;

    if (node -> key < key) {
        recentlyAdded = node -> rightChild == nullptr;
        node -> rightChild = insert(node -> rightChild, key);
    } else {
        recentlyAdded = node -> leftChild == nullptr;
        node -> leftChild = insert(node -> leftChild, key);
    }

    if (recentlyAdded) {
        generateSnapshot(0.75f);
    }

    node = rebalance(node);
    return node;
}


AVLNode* AVL::removeSuccessor(AVLNode* node) {
    if (node -> leftChild == nullptr) {
        auto tmp = node -> rightChild;

        delete node;
        node = tmp;

        return node;
    }

    bool recentlyRemoved = node -> leftChild -> leftChild == nullptr;
    node -> leftChild = removeSuccessor(node -> leftChild);

    if (recentlyRemoved) {
        generateSnapshot(1.0f);
    }

    node = rebalance(node);

    return node;
}

/// @brief Remove the target key from the tree
///        Updated: Added snapshot generation to smooth transitions
/// @param node 
/// @param key 
/// @return node (the updated tree)
AVLNode* AVL::remove(AVLNode* node, int key) {
    if (node == nullptr) return node;

    if (node -> key == key) {
        // In case this node doesn't have two subtrees, we can just move the subtree up.
        if (node -> leftChild == nullptr || node -> rightChild == nullptr) {
            auto tmp = node -> leftChild ? node -> leftChild : node -> rightChild;

            delete node;
            node = tmp;

            return node;
        }

        AVLNode* successor = node -> rightChild;
        while (successor -> leftChild != nullptr) successor = successor -> leftChild;

        node -> key = successor -> key;
        bool recentlyRemoved = node -> rightChild -> leftChild == nullptr;
        node -> rightChild = removeSuccessor(node -> rightChild);
        
        if (recentlyRemoved) {
            generateSnapshot(1.0f);
        }

        node = rebalance(node);
        return node;
    }

    bool recentlyRemoved = false;
    bool subrootChanged = false;

    if (node -> key < key) {
        recentlyRemoved = 
            node -> rightChild != nullptr &&
            node -> rightChild -> key == key &&
            (
                node -> rightChild -> leftChild == nullptr ||
                node -> rightChild -> rightChild == nullptr
            );
        
        subrootChanged = node -> rightChild != nullptr;
        int tempKey = subrootChanged ? node -> rightChild -> key : 0;

        node -> rightChild = remove(node -> rightChild, key);
        
        if (subrootChanged) subrootChanged &= node -> rightChild == nullptr || node -> rightChild -> key != tempKey; 
    } else {
        recentlyRemoved = 
            node -> leftChild != nullptr &&
            node -> leftChild -> key == key &&
            (
                node -> leftChild -> leftChild == nullptr ||
                node -> leftChild -> rightChild == nullptr
            );
        
        subrootChanged = node -> leftChild != nullptr;
        int tempKey = subrootChanged ? node -> leftChild -> key : 0;

        node -> leftChild = remove(node -> leftChild, key);
        
        if (subrootChanged) subrootChanged &= node -> leftChild == nullptr || node -> leftChild -> key != tempKey; 
    }

    if (recentlyRemoved) {
        generateSnapshot(1.0f);
    } else if (subrootChanged) {
        generateSnapshot(1.5f);
    }

    node = rebalance(node);
    return node;
}