#include <DS/Tree/AVL.hpp>
#include <algorithm>
#include <iostream>
#include "raylib.h"

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
void AVL::insertNode(int value) {
    root = insert(root, value);

    render();
}
void AVL::removeNode(int value) {
    root = remove(root, value);
}
void AVL::stepForward() {

}
void AVL::stepBackward() {

}
void AVL::render() {
    if (stateManager == nullptr) {
        std::cerr << "[ERROR]: State Manager not found\n";
        return;
    }

    if (root == nullptr) return;

    Snapshot storage;
    const float radius = 30.0f, outlineSize = 3.0f, maximumHorizontal = (radius + outlineSize) * (1 << getHeight(root)), lengthVertical = 100;
    
    auto Call = [&] (auto self, AVLNode* node, float posX, float posY, float spacingHorizontal) -> void {
        if (node == nullptr) return;

        if (node -> leftChild != nullptr) {
            self(self, node -> leftChild, posX - spacingHorizontal / 2, posY + lengthVertical, spacingHorizontal / 2);

            ShapeState leftLine = {
                Vector2{posX, posY}, 
                Vector2{posX - spacingHorizontal / 2, posY + lengthVertical}, 
                ShapeType::LINE, 
                3.0f, 1.5f,
                GREEN, RED,
                -1000,
                Text()
            };
            storage.emplace_back(leftLine);
        }

        if (node -> rightChild != nullptr) {
            self(self, node -> rightChild, posX + spacingHorizontal / 2, posY + lengthVertical, spacingHorizontal / 2);

            ShapeState rightLine = {
                Vector2{posX, posY}, 
                Vector2{posX + spacingHorizontal / 2, posY + lengthVertical}, 
                ShapeType::LINE, 
                3.0f, 1.5f, 
                GREEN, RED,
                -1000,
                Text()
            };

            storage.emplace_back(rightLine);
        }

        ShapeState nodeShape = {
            Vector2{posX, posY}, 
            Vector2{posX, posY}, 
            ShapeType::CIRCLE, 
            radius, outlineSize, 
            WHITE, YELLOW,
            0,
            Text {
                std::to_string(node -> key), 
                GetFontDefault(),
                20.0f, 2.0f,
                BLACK,
                Vector2{posX, posY},
                true,
            }
        };
        storage.emplace_back(nodeShape);
    };

    Call(Call, root, 0, -GetScreenHeight() / 2.0f + radius * 2, maximumHorizontal);

    stateManager -> addSnapshot(storage);
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
    if (balance > +1) { // Left-Heavy
        if (balanceFactor(node -> leftChild) < 0) {
            node -> leftChild = rotateLeft(node -> leftChild);
        }

        node = rotateRight(node);
    }

    if (balance < -1) { // Right-Heavy
        if (balanceFactor(node -> rightChild) > 0) {
            node -> rightChild = rotateRight(node -> rightChild);
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

    if (node -> key < key) {
        node -> rightChild = insert(node -> rightChild, key);
    } else {
        node -> leftChild = insert(node -> leftChild, key);
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

    node -> leftChild = removeSuccessor(node -> leftChild);
    node = rebalance(node);

    return node;
}

/// @brief Remove the target key from the tree
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
        node -> rightChild = removeSuccessor(node -> rightChild);

        node = rebalance(node);
        return node;
    }


    if (node -> key < key) {
        node -> rightChild = remove(node -> rightChild, key);
    } else {
        node -> leftChild = remove(node -> leftChild, key);
    }

    node = rebalance(node);
    return node;
}