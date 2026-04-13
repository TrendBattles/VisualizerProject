#include <DS/Tree/AVL/AVL.hpp>
#include <Graphics/Helper.hpp>

AVL::AVL() {
    root = nullptr;
}

AVL::~AVL() {
    clearAll();
}

void AVL::destroyTree(AVLNode* node) {
    if (node == nullptr) return;

    destroyTree(node -> leftChild);
    destroyTree(node -> rightChild);
    
    node -> parent = nullptr;
    delete node;
}

std::string AVL::getDSID() const {
    return "AVL_Tree";
}

////////////////////////////////////
///     REPONSIVE FUNCTIONS      ///
////////////////////////////////////
bool AVL::insertNode(std::string value) {
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
bool AVL::removeNode(std::string value) {
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
bool AVL::searchNode(std::string value) {
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

    if (stateManager != nullptr)
        stateManager -> clearAllSnapshots(getDSID());
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