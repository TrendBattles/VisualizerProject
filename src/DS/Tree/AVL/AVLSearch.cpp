#include <DS/Tree/AVL/AVL.hpp>
#include <Graphics/Helper.hpp>

////////////////////////////////  
///     SEARCH FUNCTIONS     ///
////////////////////////////////

AVLNode* AVL::searchNode(int value) {
    AVLNode* result = search(root, value);

    //Returning back to normal tree
    generateSnapshot(0.5f);
    return result;
}

/// @brief Searching the target key in the tree.
///        Updated: Added animations on path tracking
/// @param node, key 
/// @return node
AVLNode* AVL::search(AVLNode* node, int key) {
    generateSnapshot(
        0.3f,
        node == nullptr ? ChangeMap() 
                        : singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
        PseudocodeSection::AVL_SEARCH,
        std::vector <int> {1}
    );

    if (node == nullptr) {
        generateSnapshot(
            0.3f,
            node == nullptr ? ChangeMap() 
                            : singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
            PseudocodeSection::AVL_SEARCH,
            std::vector <int> {2}
        );

        return nullptr;
    }

    //Update path tracking
    generateSnapshot(
        0.3f,
        singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
        PseudocodeSection::AVL_SEARCH,
        std::vector <int> {3}
    );

    if (node -> key == key) {
        //Update Found state
        generateSnapshot(
            1.5f,
            singleChange(Helper::nodeStringBuffer(node -> key), Highlight::FOUND),
            PseudocodeSection::AVL_SEARCH,
            std::vector <int> {4}
        );

        return node;
    }
    
    generateSnapshot(
        0.3f,
        singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
        PseudocodeSection::AVL_SEARCH,
        std::vector <int> {5}
    );

    if (node -> key < key) {
        generateSnapshot(
            0.3f,
            singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
            PseudocodeSection::AVL_SEARCH,
            std::vector <int> {6}
        );

        return search(node -> rightChild, key);
    }

    generateSnapshot(
        0.3f,
        singleChange(Helper::nodeStringBuffer(node -> key), Highlight::ACTIVE),
        PseudocodeSection::AVL_SEARCH,
        std::vector <int> {8}
    );

    return search(node -> leftChild, key);
}