#include <Core/EventManager.hpp>
#include <Graphics/Helper.hpp>

#include <stdexcept>
void EventManager::setAnimationManager(AnimationManager* source) {
    animationManager = source;
}
void EventManager::setDSPointer(IDataStructure* source) {
    DSPointer = source;
}

/// @brief A function serving the purpose of task division.
void EventManager::handleCommand(CommandPattern commandRequest) {
    if (commandRequest.prefix.empty()) return;
    
    if (commandRequest.prefix == "MODIFY") {
        handleModification(commandRequest);
        return;
    }

    if (commandRequest.prefix == "QUERY") {
        handleQuery(commandRequest);
        return;
    }
}

/// @brief There are two cases. The first is modfication. 
void EventManager::handleModification(CommandPattern commandRequest) {
    // Empty cases will be worked on to fill in parallelly with the progress
    if (commandRequest.section == "SETTINGS") {

        return;
    }
    
    if (commandRequest.section == "INTERACT") {
        const std::string& DataStructure = commandRequest.property;

        if (DataStructure == "AVL_Tree") {
            const std::string& operation = commandRequest.operation;
            const std::string& rawValue = commandRequest.value;
            
            if (!animationManager -> isPaused())
                animationManager -> resetAnimationTimer();
                
            if (operation == "INSERT") {
                DSPointer -> insertNode(rawValue);
                return;
            }
            if (operation == "REMOVE") {
                DSPointer -> removeNode(rawValue);
                return;
            }
            if (operation == "CLEAR") {
                DSPointer -> clearAll();
                return;
            }

            throw std::runtime_error("Invalid AVL function");
        }

        if (DataStructure == "Trie") {
            const std::string& operation = commandRequest.operation;
            const std::string& rawValue = commandRequest.value;
            
            if (!animationManager -> isPaused())
                animationManager -> resetAnimationTimer();
                
            if (operation == "INSERT") {
                DSPointer -> insertNode(rawValue);
                return;
            }
            if (operation == "REMOVE") {
                DSPointer -> removeNode(rawValue);
                return;
            }
            if (operation == "CLEAR") {
                DSPointer -> clearAll();
                return;
            }

            throw std::runtime_error("Invalid Trie function");
        }
        if (DataStructure == "Linked_List") {

            return;
        }   
        if (DataStructure == "Hash_Table") {

            return;
        }
        if (DataStructure == "Graph") {

            return;
        }

        throw std::runtime_error("Data Structure not found");
    }

    throw std::runtime_error("Invalid section");
}
/// @brief The second is query in contrast. 
void EventManager::handleQuery(CommandPattern commandRequest) {
    // Empty cases will be worked on to fill in parallelly with the progress
    if (commandRequest.section == "SETTINGS") {

        return;
    }
    if (commandRequest.section == "INTERACT") {
        const std::string& DataStructure = commandRequest.property;

        if (DataStructure == "AVL_Tree") {
            const std::string& operation = commandRequest.operation;
            const std::string& rawValue = commandRequest.value;
            
            if (!animationManager -> isPaused())
                animationManager -> resetAnimationTimer();
                
            if (operation == "SEARCH") {
                DSPointer -> searchNode(rawValue);
                return;
            }

            throw std::runtime_error("Invalid AVL function");
        }

        if (DataStructure == "Trie") {
            const std::string& operation = commandRequest.operation;
            const std::string& rawValue = commandRequest.value;
            
            if (!animationManager -> isPaused())
                animationManager -> resetAnimationTimer();
                
            if (operation == "SEARCH") {
                DSPointer -> searchNode(rawValue);
                return;
            }

            throw std::runtime_error("Invalid Trie function");
        }
        if (DataStructure == "Linked_List") {
            
            return;
        }   
        if (DataStructure == "Hash_Table") {

            return;
        }
        if (DataStructure == "Graph") {

            return;
        }

        throw std::runtime_error("Data Structure not found");
    }

    throw std::runtime_error("Invalid section");
}

void EventManager::pushToQueue(CommandPattern commandRequest) {
    commandQueue.push(commandRequest);
}
CommandPattern EventManager::popNextCommand() {
    if (commandQueue.empty())
        return CommandPattern();

    CommandPattern firstCommand = commandQueue.front();
    commandQueue.pop();
    return firstCommand;
}
CommandPattern EventManager::peekNextCommand() {
    if (commandQueue.empty())
        return CommandPattern();

    return commandQueue.front();
}
bool EventManager::hasPendingCommands() const {
    return !commandQueue.empty();
}