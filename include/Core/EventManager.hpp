#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include <Core/AnimationManager.hpp>

#include <DS/IDataStructure.hpp>

struct CommandPattern {
    std::string prefix = "";
    std::string section = "";
    std::string property = "";
    std::string operation = "";
    std::string value = "";
};

/// @brief Event Management
class EventManager {
public:
    EventManager() = default;
    void setAnimationManager(AnimationManager* source);
    void setDSPointer(IDataStructure* source);
    
    void handleCommand(CommandPattern commandMessage);
    void handleModification(CommandPattern commandMessage);
    void handleQuery(CommandPattern commandMessage);
private:
    AnimationManager* animationManager = nullptr;
    IDataStructure* DSPointer = nullptr;
};

#endif