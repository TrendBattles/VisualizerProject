#ifndef HASHTABLEUI_HPP
#define HASHTABLEUI_HPP

#include <DS/IDataStructureUI.hpp>

class HashTableUI: public IDataStructureUI {
public:
    HashTableUI();
    
    std::string getDSName() const override;
private:
    bool exceedsTextboxInitRange() override;
    void handleInputFieldInit() override;
};
#endif