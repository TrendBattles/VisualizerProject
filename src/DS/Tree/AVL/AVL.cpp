#include <DS/Tree/AVL/AVL.hpp>
#include <Graphics/Helper.hpp>

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

////////////////////////////////////
///     REPONSIVE FUNCTIONS      ///
////////////////////////////////////
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