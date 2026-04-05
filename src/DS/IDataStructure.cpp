#include <DS/IDataStructure.hpp>

void IDataStructure::setStateManager(StateManager* source) {
    this -> stateManager = source;
}
void IDataStructure::setAnimationManager(AnimationManager* source) {
    this -> animationManager = source;
}

ChangeMap IDataStructure::singleChange(std::string value, ChangeInfo changeInfo) {
    return ChangeMap {make_pair(value, changeInfo)};
}