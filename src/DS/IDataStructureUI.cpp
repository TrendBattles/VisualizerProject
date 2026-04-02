#include <DS/IDataStructureUI.hpp>

void IDataStructureUI::setUIManager(UIManager* source) {
    uiManager = source;
}
void IDataStructureUI::drawButton(const Button& button) {
    uiManager -> drawShape(button.getBackground());
    uiManager -> drawText(button.getLabel());
}
void IDataStructureUI::drawTextbox(const Textbox& textbox) {
    drawButton(textbox);
}