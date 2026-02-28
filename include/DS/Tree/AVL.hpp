#ifndef AVL_HPP
#define AVL_HPP

#include <DS/IDataStructure.hpp>

/// @brief AVL implementation
struct AVLNode {
    int key, height;
    AVLNode* leftChild, * rightChild;

    AVLNode(int value = 0): key(value), height(0), leftChild(nullptr), rightChild(nullptr) {}
};
    
class AVL : public IDataStructure {
public:
    AVL();
    ~AVL();

    void insertNode(int value) override;
    void removeNode(int value) override;

    void stepForward() override;
    void stepBackward() override;
    void render() override;
private:
    AVLNode* root;
    void destroyTree(AVLNode* node);

    int getHeight(AVLNode* node);
    void update(AVLNode* node);
    int balanceFactor(AVLNode* node);

    AVLNode* rotateLeft(AVLNode* node);
    AVLNode* rotateRight(AVLNode* node);
    AVLNode* rebalance(AVLNode* node);

    AVLNode* insert(AVLNode* node, int key);

    AVLNode* removeSuccessor(AVLNode* node);
    AVLNode* remove(AVLNode* node, int key);
};
#endif