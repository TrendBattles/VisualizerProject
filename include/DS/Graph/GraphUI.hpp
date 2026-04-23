#ifndef GRAPHUI_HPP
#define GRAPHUI_HPP

#include <DS/IDataStructureUI.hpp>

class GraphUI: public IDataStructureUI {
public:
    GraphUI();
    
    std::string getDSName() const override;
private:    
    void renderInitInput(Vector2 textboxPos) override;

    bool exceedsTextboxInitRange() override;
    void handleInputFieldInit() override;
};
#endif