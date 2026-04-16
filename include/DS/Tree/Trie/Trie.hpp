#ifndef TRIE_HPP
#define TRIE_HPP

#include <DS/IDataStructure.hpp>

struct TrieNode {
    int prefixCnt = 0;
    TrieNode* child[10];
    TrieNode();
};

class Trie: public IDataStructure {
public:
    Trie();
    ~Trie();

    void setStateManager(StateManager* source) override;
    
    bool insertNode(std::string value) override;
    bool removeNode(std::string value) override;
    bool searchNode(std::string value) override;
    void clearAll() override;

    void generateSnapshot(float duration, ChangeMap changeMap = ChangeMap(), PseudocodeSection pseudoFrame = PseudocodeSection::NONE, std::vector <int> pseudoActiveLines = {}) override;
    std::string getDSID() const override;
private:
    TrieNode* root = nullptr;
    void destroyTree(TrieNode* node);

    Snapshot buildSnapshot(const ChangeMap& changeMap);
    
    bool isDeadNode(TrieNode* node);
    void insert(TrieNode* node, const std::string& value, int idx);
    void insertString(std::string value);

    TrieNode* remove(TrieNode* node, const std::string& value, int idx);
    void removeString(std::string value);

    bool search(TrieNode* node, const std::string& value, int idx, std::vector <std::pair <std::string, ChangeInfo>>& changeInfoList);
    bool searchString(std::string value);
};
#endif