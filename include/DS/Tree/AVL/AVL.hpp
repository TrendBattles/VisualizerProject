#ifndef AVL_HPP
#define AVL_HPP

#include <DS/IDataStructure.hpp>

/// @brief AVL Auxiliary Structure
struct AVLNode {
    int key, height;
    AVLNode* parent;
    AVLNode* leftChild;
    AVLNode* rightChild;

    AVLNode(int value = 0): key(value), height(0), parent(nullptr), leftChild(nullptr), rightChild(nullptr) {}
};
    
class AVL : public IDataStructure {
public:
    AVL();
    ~AVL();

    int insertNode(std::string value) override;
    int removeNode(std::string value) override;
    int searchNode(std::string value) override;

    void clearAll() override;
    void generateSnapshot(float duration, ChangeMap changeMap = ChangeMap(), PseudocodeSection pseudoFrame = PseudocodeSection::NONE, std::vector <int> pseudoActiveLines = {}) override;
private:
    AVLNode* root;
    void destroyTree(AVLNode* node);

    void getAllNodesInSubtree(AVLNode* root, std::vector <AVLNode*>& nodeList);
    std::vector <AVLNode*> getAllNodesInSubtree(AVLNode* root);

    Snapshot buildSnapshot(const ChangeMap& changeMap);

    int getHeight(AVLNode* node);
    void update(AVLNode* node);
    int balanceFactor(AVLNode* node);

    AVLNode* rotateLeft(AVLNode* node, std::vector <int> pseudoActiveLines = {});
    AVLNode* rotateRight(AVLNode* node, std::vector <int> pseudoActiveLines = {});
    AVLNode* rebalance(AVLNode* node);

    AVLNode* insert(AVLNode* node, int key, AVLNode* parent = nullptr);
    void insertNode(int value);

    AVLNode* removeSingleChildNode(AVLNode* node);
    AVLNode* removeSuccessor(AVLNode* node);
    AVLNode* remove(AVLNode* node, int key);
    void removeNode(int value);

    AVLNode* search(AVLNode* node, int key);
    AVLNode* searchNode(int value);
};
#endif