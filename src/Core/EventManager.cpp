#include <Core/EventManager.hpp>
#include <Graphics/Helper.hpp>

#include <sstream>
#include <stdexcept>
void EventManager::setAnimationManager(AnimationManager* source) {
    animationManager = source;
}
void EventManager::setDSPointer(IDataStructure* source) {
    DSPointer = source;
}

/// @brief A function serving the purpose of task division.
void EventManager::handleCommand(std::string commandString) {
    if (commandString.empty()) return;
    
    std::stringstream ss(commandString);
    std::vector <std::string> parseList(1);
    while (ss >> parseList.back()) {
        parseList.push_back("");
    }
    parseList.pop_back();

    if (parseList[0] == "MODIFY") {
        if ((int) parseList.size() != 5) throw std::runtime_error("Invalid command word length");

        handleModification(parseList);
        return;
    }

    if (parseList[0] == "QUERY") {
        if ((int) parseList.size() != 5) throw std::runtime_error("Invalid command word length");

        handleQuery(parseList);
        return;
    }

    throw std::runtime_error("Invalid command");
}

/// @brief There are two cases. The first is modfication. 
void EventManager::handleModification(std::vector <std::string> parseList) {
    // Empty cases will be worked on to fill in parallelly with the progress
    if (parseList[1] == "SETTINGS") {

        return;
    }
    
    if (parseList[1] == "INTERACT") {
        const std::string& DataStructure = parseList[2];

        if (DataStructure == "AVL_Tree") {
            const std::string& operation = parseList[3];
            const std::string& rawValue = parseList[4];
            
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
void EventManager::handleQuery(std::vector <std::string> parseList) {
    // Empty cases will be worked on to fill in parallelly with the progress
    if (parseList[1] == "SETTINGS") {

        return;
    }
    if (parseList[1] == "INTERACT") {
        const std::string& DataStructure = parseList[2];

        if (DataStructure == "AVL_Tree") {
            const std::string& operation = parseList[3];
            const std::string& rawValue = parseList[4];
            
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

