#ifndef TRIEUI_HPP
#define TRIEUI_HPP

#include <DS/IDataStructureUI.hpp>

class TrieUI: public IDataStructureUI {
public:
    TrieUI();

    std::string getDSName() const override;
private:
    bool exceedsTextboxInitRange() override;
    void handleInputFieldInit() override;
};
#endif