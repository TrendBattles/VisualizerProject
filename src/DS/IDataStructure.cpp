#include <DS/IDataStructure.hpp>

void IDataStructure::setStateManager(StateManager* source) {
    this -> stateManager = source;
}

ChangeMap IDataStructure::singleChange(std::string value, ChangeInfo changeInfo) {
    return ChangeMap {make_pair(value, changeInfo)};
}