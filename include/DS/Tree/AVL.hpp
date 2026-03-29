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

    int insertNode(std::string value) override;
    int removeNode(std::string value) override;
    int searchNode(std::string value) override;

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

    AVLNode* search(AVLNode* node, int key);
    AVLNode* searchNode(int value);
};
#endif