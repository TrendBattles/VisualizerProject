#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <DS/IDataStructure.hpp>

struct LLNode {
    int value, shapeID;
    LLNode* next;

    LLNode(int v = 0, int sID = 0): value(v), shapeID(sID), next(nullptr) {}
};

class LinkedList : public IDataStructure {
public:
    LinkedList();
    ~LinkedList();
    
    void setStateManager(StateManager* source) override;
    
    bool insertNode(std::string value) override;
    bool removeNode(std::string value) override;
    bool searchNode(std::string value) override;
    bool updateNode(std::string oldValue, std::string newValue) override;
    void clearAll() override;

    void generateSnapshot(float duration, ChangeMap changeMap = ChangeMap(), PseudocodeSection pseudoFrame = PseudocodeSection::NONE, std::vector <int> pseudoActiveLines = {}) override;
    std::string getDSID() const override;
private:
    LLNode* head = nullptr;
    int LLpointer = 0;
    
    void destroyList(LLNode* node);
    Snapshot buildSnapshot(const ChangeMap& changeMap);
    
    void insertNode(int value);
    void removeNode(int value);
    bool searchNode(int value);
    void updateNode(int oldValue, int newValue);
    
    void remove(int value);
    bool search(int value);
};
#endif