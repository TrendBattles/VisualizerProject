#ifndef ANIMATIONMANAGER_HPP
#define ANIMATIONMANAGER_HPP
#include <Core/StateManager.hpp>
#include <Graphics/UIManager.hpp>
#include <chrono>

typedef std::chrono::time_point <std::chrono::steady_clock> ChronoClock;

/// @brief Animation Management
/// @brief Restriction: The animations aren't supported to render parallelly between different data structures

class AnimationManager {
public:
    void setStateManager(StateManager* source);
    void setUIManager(UIManager* source);

    /// @brief Returns true if there are more animation steps to play for the given DS.
    bool hasMoreSteps(const std::string& DSTarget);
    
    void setStartAnimation();
    void sendAnimationSignals(const std::string& DSTarget);

private:
    StateManager* stateManager = nullptr;
    UIManager* uiManager = nullptr;

    float animationFrameTime = 0.75f;
    ChronoClock timeStart, timeEnd;

    bool isPauseToggle = false;
};
#endif