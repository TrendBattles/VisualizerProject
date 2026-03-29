#ifndef ANIMATIONMANAGER_HPP
#define ANIMATIONMANAGER_HPP
#include <Core/StateManager.hpp>

/// @brief Animation Management
/// @brief Restriction: The animations aren't supported to render parallelly between different data structures

class AnimationManager {
public:
    void setStateManager(StateManager* source);

    bool hasMoreSteps(const std::string& DSTarget);
    
    void resetAnimationTimer();
    Snapshot requestCurrentSnapshot(const std::string& DSTarget);
private:
    StateManager* stateManager = nullptr;
    ChronoClock timeStart, timeEnd;

    float elapsedSeconds();
    bool isPauseToggle = false;
};
#endif