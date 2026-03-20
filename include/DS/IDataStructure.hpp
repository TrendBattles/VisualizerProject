#ifndef IDATASTRUCTURE_HPP
#define IDATASTRUCTURE_HPP

#include <Core/StateManager.hpp>
#include "raymath.h"

/// @brief Data Structure Interface: Representation
class IDataStructure {
protected:
    StateManager* stateManager = nullptr;
public:
    void setStateManager(StateManager* source);

    virtual ~IDataStructure() = default;
    virtual bool insertNode(std::string value) = 0;
    virtual bool removeNode(std::string value) = 0;
    virtual void clearAll() = 0;

    virtual void generateSnapshot() = 0;
};
#endif