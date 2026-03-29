#include <DS/IDataStructureUI.hpp>

void IDataStructureUI::setUIManager(UIManager* source) {
    uiManager = source;
}
void IDataStructureUI::drawButton(const Button& button) {
    uiManager -> drawShape(button.background);
    uiManager -> drawText(button.label);
}
void IDataStructureUI::drawTextBox(const TextBox& textbox) {
    drawButton(textbox);
}