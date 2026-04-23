#ifndef LINKEDLISTUI_HPP
#define LINKEDLISTUI_HPP

#include <DS/IDataStructureUI.hpp>

class LinkedListUI: public IDataStructureUI {
public:
    LinkedListUI();

    std::string getDSName() const override;
private:
    bool exceedsTextboxInitRange() override;
    void handleInputFieldInit() override;
};
#endif