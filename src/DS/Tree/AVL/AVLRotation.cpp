#include <DS/Tree/AVL/AVL.hpp>
#include <Graphics/Helper.hpp>

//////////////////////////////////  
///     ROTATION FUNCTIONS     ///
//////////////////////////////////

/// @brief Left Rotation
/// @param node 
/// @return node
AVLNode* AVL::rotateLeft(AVLNode* node, std::vector <int> pseudoActiveLines) {
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

    generateSnapshot(0.3f, rotateMap, PseudocodeSection::AVL_REBALANCE, pseudoActiveLines);

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
    generateSnapshot(0.3f, rotateMap, PseudocodeSection::AVL_REBALANCE, pseudoActiveLines);
    return R;
}

/// @brief Right rotation
/// @param node 
/// @return node
AVLNode* AVL::rotateRight(AVLNode* node, std::vector <int> pseudoActiveLines) {
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
    generateSnapshot(0.3f, rotateMap, PseudocodeSection::AVL_REBALANCE, pseudoActiveLines);
    
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
    generateSnapshot(0.3f, rotateMap, PseudocodeSection::AVL_REBALANCE, pseudoActiveLines);
    return L;
}

/// @brief Rebalancing the subtree by LL, LR, RL, RR rotations
/// @param node 
/// @return node
AVLNode* AVL::rebalance(AVLNode* node) {
    if (node == nullptr) return node;

    generateSnapshot(
        0.3f, 
        singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
        PseudocodeSection::AVL_REBALANCE,
        std::vector <int> {1}
    );
    update(node);

    generateSnapshot(
        0.3f, 
        singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
        PseudocodeSection::AVL_REBALANCE,
        std::vector <int> {2}
    );
    int balance = balanceFactor(node);
    //Only snapshot when a rotation occurred.
    
    generateSnapshot(
        0.3f, 
        singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
        PseudocodeSection::AVL_REBALANCE,
        std::vector <int> {3}
    );
    if (balance > +1) { // Left-Heavy
        generateSnapshot(
            0.3f, 
            singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
            PseudocodeSection::AVL_REBALANCE,
            std::vector <int> {4}
        );
        if (balanceFactor(node -> leftChild) < 0) {
            node -> leftChild = rotateLeft(node -> leftChild, {5});
        }

        node = rotateRight(node, {6});
    }

    generateSnapshot(
        0.3f, 
        singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
        PseudocodeSection::AVL_REBALANCE,
        std::vector <int> {7}
    );
    if (balance < -1) { // Right-Heavy
        generateSnapshot(
            0.3f, 
            singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
            PseudocodeSection::AVL_REBALANCE,
            std::vector <int> {8}
        );

        if (balanceFactor(node -> rightChild) > 0) {
            node -> rightChild = rotateRight(node -> rightChild, {9});
        }

        node = rotateLeft(node, {10});
    }

    generateSnapshot(
        0.3f, 
        singleChange(Helper::nodeStringBuffer(node -> key), Highlight::MODIFIED),
        PseudocodeSection::AVL_REBALANCE,
        std::vector <int> {11}
    );

    return node;
}