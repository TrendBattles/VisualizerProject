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
void EventManager::handleCommand(CommandPattern commandMessage) {
    if (commandMessage.prefix.empty()) return;
    
    if (commandMessage.prefix == "MODIFY") {
        handleModification(commandMessage);
        return;
    }

    if (commandMessage.prefix == "QUERY") {
        handleQuery(commandMessage);
        return;
    }

    throw std::runtime_error("Invalid command");
}

/// @brief There are two cases. The first is modfication. 
void EventManager::handleModification(CommandPattern commandMessage) {
    // Empty cases will be worked on to fill in parallelly with the progress
    if (commandMessage.section == "SETTINGS") {

        return;
    }
    
    if (commandMessage.section == "INTERACT") {
        const std::string& DataStructure = commandMessage.property;

        if (DataStructure == "AVL_Tree") {
            const std::string& operation = commandMessage.operation;
            const std::string& rawValue = commandMessage.value;
            
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

            throw std::runtime_error("Invalid AVL function");
        }

        if (DataStructure == "Trie") {

            return;
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
void EventManager::handleQuery(CommandPattern commandMessage) {
    // Empty cases will be worked on to fill in parallelly with the progress
    if (commandMessage.section == "SETTINGS") {

        return;
    }
    if (commandMessage.section == "INTERACT") {
        const std::string& DataStructure = commandMessage.property;

        if (DataStructure == "AVL_Tree") {
            const std::string& operation = commandMessage.operation;
            const std::string& rawValue = commandMessage.value;
            
            if (!animationManager -> isPaused())
                animationManager -> resetAnimationTimer();
                
            if (operation == "SEARCH") {
                DSPointer -> searchNode(rawValue);
                return;
            }

            throw std::runtime_error("Invalid AVL function");
        }

        if (DataStructure == "Trie") {

            return;
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

