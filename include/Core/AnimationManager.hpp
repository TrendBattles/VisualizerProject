#ifndef ANIMATIONMANAGER_HPP
#define ANIMATIONMANAGER_HPP
#include <Core/StateManager.hpp>

#include <chrono>

typedef std::chrono::time_point <std::chrono::steady_clock> ChronoClock;

/// @brief Animation Management
/// @brief Restriction: The animations aren't supported to render parallelly between different data structures

class AnimationManager {
public:
    void setStateManager(StateManager* source);

    bool hasMoreSteps(const std::string& DSTarget);
    
    void setStartAnimation();
    Snapshot requestCurrentSnapshot(const std::string& DSTarget);

private:
    StateManager* stateManager = nullptr;
    ChronoClock timeStart, timeEnd;

    bool isPauseToggle = false;
};
#endif