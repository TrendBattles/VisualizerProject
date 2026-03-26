#ifndef AVL_HPP
#define AVL_HPP

#include <DS/IDataStructure.hpp>

/// @brief AVL Auxiliary Structure
struct AVLNode {
    int key, height;
    AVLNode* leftChild, * rightChild;

    AVLNode(int value = 0): key(value), height(0), leftChild(nullptr), rightChild(nullptr) {}
};
    
class AVL : public IDataStructure {
public:
    AVL();
    ~AVL();

    bool insertNode(std::string value) override;
    bool removeNode(std::string value) override;
    void clearAll() override;
    
    void generateSnapshot(float duration) override;
private:
    AVLNode* root;
    void destroyTree(AVLNode* node);

    Snapshot buildSnapshot();

    int getHeight(AVLNode* node);
    void update(AVLNode* node);
    int balanceFactor(AVLNode* node);

    AVLNode* rotateLeft(AVLNode* node);
    AVLNode* rotateRight(AVLNode* node);
    AVLNode* rebalance(AVLNode* node);

    AVLNode* insert(AVLNode* node, int key);
    void insertNode(int value);

    AVLNode* removeSuccessor(AVLNode* node);
    AVLNode* remove(AVLNode* node, int key);
    void removeNode(int value);
};
#endif