#ifndef ANIMATIONMANAGER_HPP
#define ANIMATIONMANAGER_HPP
#include <Core/StateManager.hpp>

/// @brief Animation Management
/// @brief Restriction: The animations aren't supported to render parallelly between different data structures

class AnimationManager {
public:
    void setStateManager(StateManager* source);
    void setPauseToggle(bool isTrue);

    bool isPaused() const;
    bool hasMoreSteps(const std::string& DSTarget);
    bool allowsOperations(const std::string& DSTarget);

    int getSize(const std::string& DSTarget);
    void setSnapshotIdx(const std::string& DSTarget, int idx);
    int getSnapshotIdx(const std::string& DSTarget);
    bool stepBackward(const std::string& DSTarget);
    bool stepForward(const std::string& DSTarget);
    bool canStepBackward(const std::string& DSTarget);
    bool canStepForward(const std::string& DSTarget);
    std::pair <PseudocodeSection, std::vector <int>> getPseudoInfoAt(const std::string& DSTarget, int idx);
    
    int getTransitionCoeff() const;
    void setTransitionCoeff(int newCoeff);
    void resetAnimationTimer();

    void setAnimationSpeed(float newSpeed);
    float getAnimationSpeed() const;

    Snapshot requestCurrentSnapshot(const std::string& DSTarget);
private:
    StateManager* stateManager = nullptr;
    ChronoClock timeStart, timeEnd;

    float elapsedSeconds();
    const float delayTransition = 0.25f;
    bool isPauseToggle = false;

    float animationSpeed = 1.0f;
    static constexpr float MIN_SPEED = 0.25f;
    static constexpr float MAX_SPEED = 4.0f;

    int transitionCoeff = +1;
};
#endif