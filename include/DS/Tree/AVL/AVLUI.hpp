#ifndef AVLUI_HPP
#define AVLUI_HPP

#include <DS/IDataStructureUI.hpp>

class AVLUI: public IDataStructureUI {
public:
    AVLUI();

    std::string getDSName() const override;
private:
    bool exceedsTextboxInitRange() override;
    void handleInputFieldInit() override;
};
#endif