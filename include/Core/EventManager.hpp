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
    
    void handleCommand(CommandPattern commandRequest);
    void handleModification(CommandPattern commandRequest);
    void handleQuery(CommandPattern commandRequest);

    void pushToQueue(CommandPattern commandRequest);
    CommandPattern popNextCommand();
    CommandPattern peekNextCommand();
    bool hasPendingCommands() const;
private:
    AnimationManager* animationManager = nullptr;
    IDataStructure* DSPointer = nullptr;

    std::queue <CommandPattern> commandQueue;
};

#endif