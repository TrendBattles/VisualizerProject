#include <Core/EventManager.hpp>
#include <Graphics/Helper.hpp>
#include <algorithm>
#include "raylib.h"
#include "raymath.h"

///////////////////////////////////////
///     EVENT INITIALIZATIONS       ///
///////////////////////////////////////
void EventManager::init() {
    eventButtonsID = {"Insert", "Remove", "Search"};
    eventButtons.resize((int) eventButtonsID.size());
    eventTextBox.resize((int) eventButtonsID.size());   
    isFocused.assign((int) eventButtons.size(), false);


    //Event buttons
    eventButtons[0] = Helper::createRectangle("rect_insert_button", Vector2{0.0f, (float) GetScreenHeight() - 60.0f}, Vector2{100.0f, (float) GetScreenHeight()}, 5.0f, 0);
    eventButtons[0].setColor(Color {170, 170, 170, 255}, BLUE);
    eventButtons[0].setText(Helper::createText("Insert", GetFontDefault(), 20.0f, 5.0f, Vector2Lerp(eventButtons[0].startPosition, eventButtons[0].endPosition, 0.5f), RED));
    eventButtons[0].setTextCenter(true);

    eventButtons[1] = Helper::createRectangle("rect_remove_button", Vector2{240.0f, (float) GetScreenHeight() - 60.0f}, Vector2{340.0f, (float) GetScreenHeight()}, 5.0f, 0);
    eventButtons[1].setColor(Color {170, 170, 170, 255}, BLUE);
    eventButtons[1].setText(Helper::createText("Remove", GetFontDefault(), 20.0f, 5.0f, Vector2Lerp(eventButtons[1].startPosition, eventButtons[1].endPosition, 0.5f), RED));
    eventButtons[1].setTextCenter(true);

    eventButtons[2].isHidden = true;

    //Event Text Box
    eventTextBox[0] = Helper::createRectangle("rect_insert_textbox", Vector2{120.0f, (float) GetScreenHeight() - 60.0f}, Vector2{220.0f, (float) GetScreenHeight()}, 5.0f, 0);
    eventTextBox[0].setColor(Color {170, 170, 170, 255}, BLUE);
    eventTextBox[0].setText(Helper::createText(std::string(), GetFontDefault(), 20.0f, 2.0f, Vector2Lerp(eventTextBox[0].startPosition, eventTextBox[0].endPosition, 0.5f), BLACK));
    eventTextBox[0].setTextCenter(true);
    
    eventTextBox[1] = Helper::createRectangle("rect_remove_textbox", Vector2{360.0f, (float) GetScreenHeight() - 60.0f}, Vector2{460.0f, (float) GetScreenHeight()}, 5.0f, 0);
    eventTextBox[1].setColor(Color {170, 170, 170, 255}, BLUE);
    eventTextBox[1].setText(Helper::createText(std::string(), GetFontDefault(), 20.0f, 2.0f, Vector2Lerp(eventTextBox[1].startPosition, eventTextBox[1].endPosition, 0.5f), BLACK));
    eventTextBox[1].setTextCenter(true);
}

///////////////////////////////
///     EVENT FUNCTIONS     ///
///////////////////////////////
int EventManager::getSize() {
    return (int) eventButtons.size();
}

std::string EventManager::getEventName(int idx) {
    return idx < 0 || idx >= getSize() ? "" : eventButtonsID[idx];
}

/// Shape modifications 
void EventManager::setEventShape(int idx, ShapeState newShape) {
    if (idx < 0 || idx >= getSize()) return;

    eventButtons[idx] = newShape;
}
void EventManager::setEventShape(std::string message, ShapeState newShape) {
    setEventShape(find(eventButtonsID.begin(), eventButtonsID.end(), message) - eventButtonsID.begin(), newShape);
}
ShapeState EventManager::getEventShape(int idx) {
    return idx < 0 || idx >= getSize() ? ShapeState() : eventButtons[idx];
}
ShapeState EventManager::getEventShape(std::string message) {
    return getEventShape(find(eventButtonsID.begin(), eventButtonsID.end(), message) - eventButtonsID.begin());
}

/// Button modifications 
bool EventManager::isButtonHidden(int idx) {
    return idx < 0 || idx >= getSize() ? true : eventButtons[idx].isHidden;
}
bool EventManager::isButtonHidden(std::string message) {
    return isButtonHidden(find(eventButtonsID.begin(), eventButtonsID.end(), message) - eventButtonsID.begin());
}
void EventManager::setButtonVisibility(int idx, bool isTrue) {
    if (idx < 0 || idx >= getSize()) return;

    eventButtons[idx].isHidden = isTrue;
}
void EventManager::setButtonVisibility(std::string message, bool isTrue) {
    setButtonVisibility(find(eventButtonsID.begin(), eventButtonsID.end(), message) - eventButtonsID.begin(), isTrue);
}

/// Text box modifications 
void EventManager::setEventTextBox(int idx, ShapeState newShape) {
    if (idx < 0 || idx >= getSize()) return;

    eventTextBox[idx] = newShape;
}
void EventManager::setEventTextBox(std::string message, ShapeState newShape) {
    setEventTextBox(find(eventButtonsID.begin(), eventButtonsID.end(), message) - eventButtonsID.begin(), newShape);
}
ShapeState EventManager::getEventTextBox(int idx) {
    return idx < 0 || idx >= getSize() ? ShapeState() : eventTextBox[idx];
}
ShapeState EventManager::getEventTextBox(std::string message) {
    return getEventTextBox(find(eventButtonsID.begin(), eventButtonsID.end(), message) - eventButtonsID.begin());
}

bool EventManager::isTextBoxHidden(int idx) {
    return idx < 0 || idx >= getSize() ? true : eventButtons[idx].isHidden;
}
bool EventManager::isTextBoxHidden(std::string message) {
    return isTextBoxHidden(find(eventButtonsID.begin(), eventButtonsID.end(), message) - eventButtonsID.begin());
}
void EventManager::setTextBoxVisibility(int idx, bool isTrue) {
    if (idx < 0 || idx >= getSize()) return;

    eventButtons[idx].isHidden = isTrue;
}
void EventManager::setTextBoxVisibility(std::string message, bool isTrue) {
    setTextBoxVisibility(find(eventButtonsID.begin(), eventButtonsID.end(), message) - eventButtonsID.begin(), isTrue);
}

void EventManager::setFocused(int idx, int state) {
    if (idx < 0 || idx >= getSize()) return;

    isFocused[idx] = state;
}
void EventManager::setFocused(std::string message, int state) {
    setFocused(find(eventButtonsID.begin(), eventButtonsID.end(), message) - eventButtonsID.begin(), state);   
}
int EventManager::getFocused(int idx) {
    return idx < 0 || idx >= getSize() ? false : isFocused[idx];
}
int EventManager::getFocused(std::string message) {
    return getFocused(find(eventButtonsID.begin(), eventButtonsID.end(), message) - eventButtonsID.begin());
}

void EventManager::clearTextBox(int idx) {
    if (idx < 0 || idx >= getSize()) return;

    eventTextBox[idx].content.label.clear();
}
void EventManager::clearTextBox(std::string message) {
    clearTextBox(find(eventButtonsID.begin(), eventButtonsID.end(), message) - eventButtonsID.begin());
}