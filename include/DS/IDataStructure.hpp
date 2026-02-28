#ifndef IDATASTRUCTURE_HPP
#define IDATASTRUCTURE_HPP

#include <StateManager.hpp>

/// @brief Data Structure Interface: Representation
class IDataStructure {
protected:
    StateManager* stateManager = nullptr;
public:
    void setStateManager(StateManager* source);

    virtual ~IDataStructure() = default;
    virtual void insertNode(int value) = 0;
    virtual void removeNode(int value) = 0;

    virtual void stepForward() = 0;
    virtual void stepBackward() = 0;
    virtual void render() = 0;
};
#endif