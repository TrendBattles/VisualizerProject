#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <Core/AnimationManager.hpp>

#include <DS/IDataStructure.hpp>

/// @brief Event Management
class EventManager {
public:
    EventManager() = default;
    void setAnimationManager(AnimationManager* source);
    void setDSPointer(IDataStructure* source);
    
    void handleCommand(std::string commandString);
    void handleModification(std::vector <std::string> parseList);
    void handleQuery(std::vector <std::string> parseList);
private:
    AnimationManager* animationManager = nullptr;
    IDataStructure* DSPointer = nullptr;
};

#endif