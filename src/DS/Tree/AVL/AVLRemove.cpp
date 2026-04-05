#include <DS/Tree/AVL/AVL.hpp>
#include <Graphics/Helper.hpp>

////////////////////////////////  
///     REMOVE FUNCTIONS     ///
////////////////////////////////

void AVL::removeNode(int value) {
    // generateSnapshot(); // snapshot the state before removal
    root = remove(root, value);
    generateSnapshot(0.5f); // snapshot the final state after removal
}

/// @brief Removing a node with a single child
/// @param node 
/// @return node 
AVLNode* AVL::removeSingleChildNode(AVLNode* node) {
    if (node == nullptr) return node;

    auto tmp = node -> leftChild ? node -> leftChild : node -> rightChild;

    if (tmp != nullptr) tmp -> parent = node -> parent;
    if (node -> parent != nullptr) {
        auto parent = node -> parent;
        if (parent -> leftChild == node) parent -> leftChild = tmp;
        else parent -> rightChild = tmp;
    }
    if (root == node) root = tmp;

    delete node;
    node = tmp;

    return node;
}

/// @brief Removing the InOrder successor of a node
/// @param node 
/// @return node 
AVLNode* AVL::removeSuccessor(AVLNode* node) {
    generateSnapshot(
        0.3f, 
        singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
        PseudocodeSection::AVL_REMOVE_SUCCESSOR,
        std::vector <int> {1}
    );
    
    if (node -> leftChild == nullptr) {
        generateSnapshot(
            0.3f, 
            singleChange(Helper::nodeStringBuffer(node -> key), Highlight::REMOVED),
            PseudocodeSection::AVL_REMOVE_SUCCESSOR,
            std::vector <int> {2}
        );

        return removeSingleChildNode(node);
    }

    generateSnapshot(
        0.3f, 
        singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
        PseudocodeSection::AVL_REMOVE_SUCCESSOR,
        std::vector <int> {3}
    );

    node -> leftChild = removeSuccessor(node -> leftChild);

    generateSnapshot(
        0.3f, 
        singleChange(Helper::nodeStringBuffer(node -> key), Highlight::INACTIVE),
        PseudocodeSection::AVL_REMOVE_SUCCESSOR,
        std::vector <int> {4}
    );

    node = rebalance(node);

    generateSnapshot(
        0.3f, 
        singleChange(Helper::nodeStringBuffer(node -> key), Highlight::INACTIVE),
        PseudocodeSection::AVL_REMOVE_SUCCESSOR,
        std::vector <int> {5}
    );
    return node;
}

/// @brief Remove the target key from the tree
///        Updated: Added path tracking, removing/balancing transitions.
/// @param node 
/// @param key 
/// @return node (the updated tree)
AVLNode* AVL::remove(AVLNode* node, int key) {
    generateSnapshot(
        0.3f,
        node == nullptr ? ChangeMap() 
                        : singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
        PseudocodeSection::AVL_REMOVE,
        std::vector <int> {1}
    );

    if (node == nullptr) {
        generateSnapshot(
            0.3f,
            ChangeMap(),
            PseudocodeSection::AVL_REMOVE,
            std::vector <int> {2}
        );
        return node;
    }
    
    //Update path tracking
    generateSnapshot(
        0.3f,
        singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
        PseudocodeSection::AVL_REMOVE,
        std::vector <int> {3}
    );

    if (node -> key < key) {
        generateSnapshot(
            0.3f,
            singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
            PseudocodeSection::AVL_REMOVE,
            std::vector <int> {4}
        );

        node -> rightChild = remove(node -> rightChild, key);
    } else {
        generateSnapshot(
            0.3f,
            singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
            PseudocodeSection::AVL_REMOVE,
            std::vector <int> {5}
        );

        if (node -> key > key) {
            generateSnapshot(
                0.3f,
                singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
                PseudocodeSection::AVL_REMOVE,
                std::vector <int> {6}
            );
            node -> leftChild = remove(node -> leftChild, key);
        } else {
            generateSnapshot(
                0.3f,
                singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
                PseudocodeSection::AVL_REMOVE,
                std::vector <int> {8}
            );

            // In case this node doesn't have two subtrees, move the subtree up.
            if (node -> leftChild == nullptr || node -> rightChild == nullptr) {
                generateSnapshot(
                    0.3f,
                    singleChange(Helper::nodeStringBuffer(node -> key), Highlight::REMOVED),
                    PseudocodeSection::AVL_REMOVE,
                    std::vector <int> {9}
                );

                return removeSingleChildNode(node);
            }

            generateSnapshot(
                0.3f,
                singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
                PseudocodeSection::AVL_REMOVE,
                std::vector <int> {10}
            );

            //Otherwise, find the successor's node and copy its content to the current node.
            //After that, remove that successor node and do balancing rotations.
            AVLNode* successor = node -> rightChild;
            while (successor -> leftChild != nullptr) successor = successor -> leftChild;

            generateSnapshot(
                0.3f,
                singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
                PseudocodeSection::AVL_REMOVE,
                std::vector <int> {11}
            );

            node -> key = successor -> key;
            successor -> key = -1;

            generateSnapshot(
                0.3f,
                singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
                PseudocodeSection::AVL_REMOVE,
                std::vector <int> {12}
            );

            node -> rightChild = removeSuccessor(node -> rightChild);
            if (node -> rightChild != nullptr) node -> rightChild -> parent = node;
        }
    }

    // Updating the path
    generateSnapshot(
        0.3f, 
        singleChange(Helper::nodeStringBuffer(node -> key), Highlight::INACTIVE),
        PseudocodeSection::AVL_REMOVE,
        std::vector <int> {13}
    );

    node = rebalance(node);

    generateSnapshot(
        0.3f, 
        singleChange(Helper::nodeStringBuffer(node -> key), Highlight::INACTIVE),
        PseudocodeSection::AVL_REMOVE,
        std::vector <int> {14}
    );
    return node;
}