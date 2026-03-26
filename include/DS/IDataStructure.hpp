#ifndef IDATASTRUCTURE_HPP
#define IDATASTRUCTURE_HPP

#include <Core/StateManager.hpp>
#include <Core/AnimationManager.hpp>
#include "raymath.h"

/// @brief Data Structure Interface: Representation
class IDataStructure {
public:
    void setStateManager(StateManager* source);
    void setAnimationManager(AnimationManager* source);

    virtual ~IDataStructure() = default;
    virtual bool insertNode(std::string value) = 0;
    virtual bool removeNode(std::string value) = 0;
    virtual void clearAll() = 0;

    virtual void generateSnapshot(float duration) = 0;
protected:
    StateManager* stateManager = nullptr;
    AnimationManager* animationManager = nullptr;
};
#endif