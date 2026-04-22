#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <DS/IDataStructure.hpp>

struct HashNode {
    int value, shapeID;
    HashNode* next;

    HashNode(int v = 0, int sID = 0): value(v), shapeID(sID), next(nullptr) {}
};

class HashTable : public IDataStructure {
public:
    HashTable();
    ~HashTable();

    void setStateManager(StateManager* source) override;
    void initDS(const std::vector <std::string>& rawValue) override;
    
    bool insertNode(std::string value) override;
    bool removeNode(std::string value) override;
    bool searchNode(std::string value) override;

    void generateSnapshot(float duration, ChangeMap changeMap = ChangeMap(), PseudocodeSection pseudoFrame = PseudocodeSection::NONE, std::vector <int> pseudoActiveLines = {}) override;

    void clearAll() override;
    std::string getDSID() const override;
private:
    static const int TABLE_SIZE = 7;
    HashNode* table[TABLE_SIZE];
    int HashPointer = 0;

    std::vector <HashNode*> getHashNodeList(int id);
    Snapshot buildSnapshot(ChangeMap changeMap);
    void destroyTable();

    void insertNode(int value);
    void remove(int value);
    void removeNode(int value);
    bool searchNode(int value);
};
#endif