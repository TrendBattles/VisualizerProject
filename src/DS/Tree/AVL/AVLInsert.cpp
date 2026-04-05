#include <DS/Tree/AVL/AVL.hpp>
#include <Graphics/Helper.hpp>

////////////////////////////////  
///     INSERT FUNCTIONS     ///
////////////////////////////////

void AVL::insertNode(int value) {
    // generateSnapshot(); // snapshot the state before insertion
    root = insert(root, value);

    generateSnapshot(1.0f, ChangeMap(), PseudocodeSection::AVL_INSERT); // snapshot the final state after insertion
}

/// @brief Insert a new key to the tree
///        Updated: Added path tracking and inserting/rotating transitions
/// @param node 
/// @param key 
/// @return node (the updated tree)
AVLNode* AVL::insert(AVLNode* node, int key, AVLNode* parent) {
    generateSnapshot(
        0.3f,
        node == nullptr ? ChangeMap() 
                        : singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
        PseudocodeSection::AVL_INSERT,
        std::vector <int> {1}
    );

    // If a new node is inserted, generate a new highlight
    if (node == nullptr) {
        node = new AVLNode(key);
        // If root is null, update the root
        if (root == nullptr) root = node;

        // Update the parent - children relationship
        if (parent != nullptr) {
            node -> parent = parent;

            if (parent -> key < node -> key) parent -> rightChild = node;
            else parent -> leftChild = node;
        }

        generateSnapshot(
            0.5f, 
            singleChange(Helper::nodeStringBuffer(node -> key), Highlight::INSERTED),
            PseudocodeSection::AVL_INSERT,
            std::vector <int> {2}
        ); 

        return node;
    }

    // Activating the node on the inserting path
    generateSnapshot(
        0.3f, 
        singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
        PseudocodeSection::AVL_INSERT,
        std::vector <int> {3}
    ); 
    
    //If found, stop searching
    if (node -> key == key) {
        generateSnapshot(
            0.3f, 
            singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
            PseudocodeSection::AVL_INSERT,
            std::vector <int> {4}
        ); 

        return node;
    }

    generateSnapshot(
        0.3f, 
        singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
        PseudocodeSection::AVL_INSERT,
        std::vector <int> {5}
    );

    if (node -> key < key) {
        generateSnapshot(
            0.3f, 
            singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
            PseudocodeSection::AVL_INSERT,
            std::vector <int> {6}
        );
        node -> rightChild = insert(node -> rightChild, key, node);
    } else {
        generateSnapshot(
            0.3f, 
            singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
            PseudocodeSection::AVL_INSERT,
            std::vector <int> {8}
        );

        node -> leftChild = insert(node -> leftChild, key, node);
    }

    // Updating the path
    generateSnapshot(
        0.3f, 
        singleChange(Helper::nodeStringBuffer(node -> key), Highlight::INACTIVE),
        PseudocodeSection::AVL_INSERT,
        std::vector <int> {9}
    );

    node = rebalance(node);

    generateSnapshot(
        0.3f, 
        singleChange(Helper::nodeStringBuffer(node -> key), Highlight::INACTIVE),
        PseudocodeSection::AVL_INSERT,
        std::vector <int> {10}
    );
    return node;
}