#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <vector>
#include <RendererCommon.hpp>

/// @brief Event Management
class EventManager {
public:
    EventManager() = default;
    void init();
    
    int getSize();
    std::string getEventName(int idx);

    void setEventShape(int idx, ShapeState newShape);
    void setEventShape(std::string message, ShapeState newShape);
    ShapeState getEventShape(int idx);
    ShapeState getEventShape(std::string message);

    void setEventTextBox(int idx, ShapeState newShape);
    void setEventTextBox(std::string message, ShapeState newShape);

    ShapeState getEventTextBox(int idx);
    ShapeState getEventTextBox(std::string message);

    bool isHidden(int idx);
    bool isHidden(std::string message);

    void setFocused(int idx, int state);
    void setFocused(std::string message, int state);
    int getFocused(int idx);
    int getFocused(std::string message);

    void clearTextBox(int idx);
    void clearTextBox(std::string message);
private:
    std::vector <ShapeState> eventButtons, eventTextBox;
    std::vector <std::string> eventButtonsID;
    
    std::vector <int> isFocused;
};

#endif