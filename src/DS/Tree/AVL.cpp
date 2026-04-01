#include <DS/Tree/AVL.hpp>
#include <Graphics/Helper.hpp>
#include <cassert>

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
    
    node -> parent = nullptr;
    delete node;
}

/// @brief A node list of the entire subtree 
void AVL::getAllNodesInSubtree(AVLNode* root, std::vector <AVLNode*>& nodeList) {
    if (root == nullptr) return;
    nodeList.push_back(root);

    getAllNodesInSubtree(root -> leftChild);
    getAllNodesInSubtree(root -> rightChild);
}
std::vector <AVLNode*> AVL::getAllNodesInSubtree(AVLNode* root) {
    std::vector <AVLNode*> nodeList;
    getAllNodesInSubtree(root, nodeList);

    return nodeList;
}

/////////////////////////////////
///  REPONSIVE FUNCTIONS      ///
/////////////////////////////////
int AVL::insertNode(std::string value) {
    int valueParse = 0;
    try {
        valueParse = std::stoi(value);
    } catch (const std::invalid_argument& ia) {
	    std::cerr << "[INPUT ERROR]: " << ia.what() << '\n';
        return false;
    }
    
    insertNode(valueParse);
    return true;
}
int AVL::removeNode(std::string value) {
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
int AVL::searchNode(std::string value) {
    int valueParse = 0;
    try {
        valueParse = std::stoi(value);
    } catch (const std::invalid_argument& ia) {
	    std::cerr << "[INPUT ERROR]: " << ia.what() << '\n';
        return -1;
    }

    return searchNode(valueParse) != nullptr;
}

void AVL::clearAll() {
    destroyTree(root);
    root = nullptr;

    stateManager -> clearAllSnapshots("AVL_Tree");
}

void AVL::insertNode(int value) {
    // generateSnapshot(); // snapshot the state before insertion
    bool isNull = root == nullptr;
    root = insert(root, value);

    // Special case of highlight when the tree is null
    if (isNull) {
        generateSnapshot(0.5f, {
            make_pair(
                Helper::nodeStringBuffer(std::to_string(root -> key)),
                Highlight::INSERTED
            )
        });
    }

    generateSnapshot(1.0f); // snapshot the final state after insertion
}
void AVL::removeNode(int value) {
    // generateSnapshot(); // snapshot the state before removal
    root = remove(root, value);
    generateSnapshot(1.0f); // snapshot the final state after removal
}
AVLNode* AVL::searchNode(int value) {
    AVLNode* result = search(root, value);

    //Returning back to normal tree
    generateSnapshot(1.0f);
    return result;
}

/// @brief Builds a Snapshot of the current tree state without touching the StateManager.
///        Separates layout/serialization from persistence so that callers
///        can decorate the result before committing it.
Snapshot AVL::buildSnapshot(const ChangeMap& changeMap) {
    if (root == nullptr) return Snapshot();

    const float radius = 30.0f, outlineSize = 6.0f, lengthVertical = 100;
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
                Helper::arrowPointStringBuffer(std::to_string(childValue)),
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

void AVL::generateSnapshot(float duration, ChangeMap changeMap) {
    if (stateManager == nullptr) {
        std::cerr << "[ERROR]: State Manager not found\n";
        return;
    }

    stateManager -> addSnapshot("AVL_Tree", HistoryFrame{ buildSnapshot(changeMap), duration });
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

    // Highlighting the rotated nodes first
    ChangeMap rotateMap = {
        make_pair(
            Helper::nodeStringBuffer(std::to_string(node -> key)), 
            Highlight::ROTATION
        ),
        make_pair(
            Helper::nodeStringBuffer(std::to_string(R -> key)), 
            Highlight::ROTATION
        ),
    };
    if (RL != nullptr) {
        rotateMap.insert(make_pair(
            Helper::nodeStringBuffer(std::to_string(RL -> key)), 
            Highlight::ROTATION
        ));
    }
    generateSnapshot(1.0f, rotateMap);

    // Doing the rotation
    node -> rightChild = RL;
    R -> leftChild = node;
    if (node == root) root = R;

    // Updating the parent of the nodes
    R -> parent = node -> parent;
    if (node -> parent != nullptr) {
        auto parent = node -> parent;

        if (parent -> leftChild == node) parent -> leftChild = R;
        else parent -> rightChild = R; 
    }

    node -> parent = R;
    if (RL != nullptr) RL -> parent = node;

    // Height update
    update(node);
    update(R);

    // Highlighting once again to see the transition
    generateSnapshot(1.5f, rotateMap);
    return R;
}

/// @brief Right rotation
/// @param node 
/// @return node
AVLNode* AVL::rotateRight(AVLNode* node) {
    if (node == nullptr) return node;

    auto L = node -> leftChild;
    auto LR = L -> rightChild;

    // Highlighting the rotated nodes first
    ChangeMap rotateMap = {
        make_pair(
            Helper::nodeStringBuffer(std::to_string(node -> key)), 
            Highlight::ROTATION
        ),
        make_pair(
            Helper::nodeStringBuffer(std::to_string(L -> key)), 
            Highlight::ROTATION
        ),
    };
    if (LR != nullptr) {
        rotateMap.insert(make_pair(
            Helper::nodeStringBuffer(std::to_string(LR -> key)), 
            Highlight::ROTATION
        ));
    }
    generateSnapshot(1.0f, rotateMap);
    
    // Doing the rotation
    node -> leftChild = LR;
    L -> rightChild = node;
    if (node == root) root = L;

    // Updating the parent of the nodes
    L -> parent = node -> parent;
    if (node -> parent != nullptr) {
        auto parent = node -> parent;

        if (parent -> leftChild == node) parent -> leftChild = L;
        else parent -> rightChild = L;
    }

    node -> parent = L;
    if (LR != nullptr) LR -> parent = node;

    // Height update
    update(node);
    update(L);

    // Highlighting once again to see the transition
    generateSnapshot(1.5f, rotateMap);
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
        }

        node = rotateRight(node);
    } else if (balance < -1) { // Right-Heavy
        if (balanceFactor(node -> rightChild) > 0) {
            node -> rightChild = rotateRight(node -> rightChild);
        }

        node = rotateLeft(node);
    }

    return node;
}

/// @brief Insert a new key to the tree
///        Updated: Added path tracking and inserting/rotating transitions
/// @param node 
/// @param key 
/// @return node (the updated tree)
AVLNode* AVL::insert(AVLNode* node, int key) {
    if (node == nullptr) {
        node = new AVLNode(key);
        return node;
    }

    // Activating the node on the inserting path
    generateSnapshot(0.5f, {
        make_pair(
            Helper::nodeStringBuffer(std::to_string(node -> key)),
            Highlight::ACTIVE
        )
    }); 
    
    //If found, stop searching
    if (node -> key == key) {
        return node;
    }

    AVLNode* addedNode = nullptr;
    if (node -> key < key) {
        bool recentlyAdded = node -> rightChild == nullptr;
        node -> rightChild = insert(node -> rightChild, key);

        if (recentlyAdded) addedNode = node -> rightChild;
        node -> rightChild -> parent = node;
    } else {
        bool recentlyAdded = node -> leftChild == nullptr;
        node -> leftChild = insert(node -> leftChild, key);

        if (recentlyAdded) addedNode = node -> leftChild;
        node -> leftChild -> parent = node;
    }

    // If a new node is inserted, generate a new highlight
    if (addedNode != nullptr) {
        generateSnapshot(0.5f, {
            make_pair(
                Helper::nodeStringBuffer(std::to_string(addedNode -> key)),
                Highlight::INSERTED
            )
        }); 
    }

    // Updating the path
    generateSnapshot(0.5f, {
        make_pair(
            Helper::nodeStringBuffer(std::to_string(node -> key)),
            Highlight::ACTIVE
        )
    });

    node = rebalance(node);
    return node;
}

/// @brief Removing a node with a single child
/// @param node 
/// @return node 
AVLNode* AVL::removeSingleChildNode(AVLNode* node) {
    if (node == nullptr) return node;

    auto tmp = node -> leftChild ? node -> leftChild : node -> rightChild;
    // Generate a snapshot of removing that node
    generateSnapshot(1.5f, {
        make_pair(
            Helper::nodeStringBuffer(std::to_string(node -> key)),
            Highlight::REMOVED
        )
    });

    if (tmp != nullptr) tmp -> parent = node -> parent;
    if (node -> parent != nullptr) {
        auto parent = node -> parent;
        if (parent -> leftChild == node) parent -> leftChild = tmp;
        else parent -> rightChild = tmp;
    }
    if (root == node) root = tmp;

    delete node;
    node = tmp;
    
    //Activate the uplifting node for path tracking
    if (tmp == nullptr) {
        generateSnapshot(0.5f);
    } else {
        generateSnapshot(1.5f, {
            make_pair(
                Helper::nodeStringBuffer(std::to_string(tmp -> key)),
                Highlight::ACTIVE
            )
        });
    }

    return node;
}

/// @brief Removing the InOrder successor of a node
/// @param node 
/// @return node 
AVLNode* AVL::removeSuccessor(AVLNode* node) {
    if (node -> leftChild == nullptr) {
        return removeSingleChildNode(node);
    }

    node -> leftChild = removeSuccessor(node -> leftChild);
    if (node -> leftChild != nullptr) node -> leftChild -> parent = node;

    generateSnapshot(0.5f, {
        make_pair(
            Helper::nodeStringBuffer(std::to_string(node -> key)),
            Highlight::ACTIVE
        )
    });
    node = rebalance(node);

    return node;
}

/// @brief Remove the target key from the tree
///        Updated: Added path tracking, removing/balancing transitions.
/// @param node 
/// @param key 
/// @return node (the updated tree)
AVLNode* AVL::remove(AVLNode* node, int key) {
    if (node == nullptr) return node;
    
    //Update path tracking
    generateSnapshot(0.5f, {
        make_pair(
            Helper::nodeStringBuffer(std::to_string(node -> key)),
            Highlight::ACTIVE
        )
    });

    if (node -> key == key) {
        // In case this node doesn't have two subtrees, move the subtree up.
        if (node -> leftChild == nullptr || node -> rightChild == nullptr) {
            return removeSingleChildNode(node);
        }

        //Otherwise, find the successor's node and copy its content to the current node.
        //After that, remove that successor node and do balancing rotations.
        AVLNode* successor = node -> rightChild;
        while (successor -> leftChild != nullptr) successor = successor -> leftChild;

        node -> key = successor -> key;
        successor -> key = -1;
        node -> rightChild = removeSuccessor(node -> rightChild);
        if (node -> rightChild != nullptr) node -> rightChild -> parent = node;

        generateSnapshot(0.5f, {
            make_pair(
                Helper::nodeStringBuffer(std::to_string(node -> key)),
                Highlight::ACTIVE
            )
        });

        node = rebalance(node);
        return node;
    }

    if (node -> key < key) {
        node -> rightChild = remove(node -> rightChild, key);
        if (node -> rightChild != nullptr) node -> rightChild -> parent = node;
    } else {
        node -> leftChild = remove(node -> leftChild, key);
        if (node -> leftChild != nullptr) node -> leftChild -> parent = node;
    }

    //Update path tracking
    generateSnapshot(0.5f, {
        make_pair(
            Helper::nodeStringBuffer(std::to_string(node -> key)),
            Highlight::ACTIVE
        )
    });

    node = rebalance(node);
    return node;
}

/// @brief Searching the target key in the tree.
///        Updated: Added animations on path tracking
/// @param node, key 
/// @return node
AVLNode* AVL::search(AVLNode* node, int key) {
    if (node == nullptr) return nullptr;

    //Update path tracking
    generateSnapshot(0.5f, {
        make_pair(
            Helper::nodeStringBuffer(std::to_string(node -> key)),
            Highlight::ACTIVE
        )
    });

    if (node -> key == key) {
        //Update Found state
        generateSnapshot(1.0f, {
            make_pair(
                Helper::nodeStringBuffer(std::to_string(node -> key)),
                Highlight::FOUND
            )
        });
        generateSnapshot(0.5f, {
            make_pair(
                Helper::nodeStringBuffer(std::to_string(node -> key)),
                Highlight::FOUND
            )
        });

        return node;
    }
    
    if (node -> key < key) {
        return search(node -> rightChild, key);
    }
    return search(node -> leftChild, key);
}