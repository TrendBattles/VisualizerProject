#include <Core/AnimationManager.hpp>

void AnimationManager::setStateManager(StateManager* source) {
    stateManager = source;
}
void AnimationManager::setPauseToggle(bool isTrue) {
    isPauseToggle = isTrue;
}
bool AnimationManager::isPaused() const {
    return isPauseToggle;
}

/// @brief Returns true if there are more animation steps to play for the given DS.
bool AnimationManager::hasMoreSteps(const std::string& DSTarget) {
    if (transitionCoeff == 0) return false;
    
    if (transitionCoeff == +1)
        return stateManager -> canStepForward(DSTarget);
    return stateManager -> canStepBackward(DSTarget);
}

/// @brief The operations are allowed to take inputs when the app isn't currently animating the data structure. 
bool AnimationManager::allowsOperations(const std::string& DSTarget) {
    return !canStepForward(DSTarget) && getTransitionCoeff() >= 0;
}


///////////////////////////////////////////////
///     STATE MANAGER WRAPPER FUNCTIONS     ///
///////////////////////////////////////////////
int AnimationManager::getSize(const std::string& DSTarget) {
    return stateManager -> getSize(DSTarget);
}
void AnimationManager::setSnapshotIdx(const std::string& DSTarget, int idx) {
    stateManager -> setSnapshotIdx(DSTarget, idx);
}
int AnimationManager::getSnapshotIdx(const std::string& DSTarget) {
    return stateManager -> getSnapshotIdx(DSTarget);
}
bool AnimationManager::stepBackward(const std::string& DSTarget) {
    return stateManager -> stepBackward(DSTarget);
}
bool AnimationManager::stepForward(const std::string& DSTarget) {
    return stateManager -> stepForward(DSTarget);
}
bool AnimationManager::canStepBackward(const std::string& DSTarget) {
    return stateManager -> canStepBackward(DSTarget);
}
bool AnimationManager::canStepForward(const std::string& DSTarget) {
    return stateManager -> canStepForward(DSTarget);
}
std::pair <PseudocodeSection, std::vector <int>> AnimationManager::getPseudoInfoAt(const std::string& DSTarget, int idx) {
    return stateManager -> getPseudoInfoAt(DSTarget, idx);
}


///////////////////////////////////
///     ANIMATION FUNCTIONS     ///
///////////////////////////////////

/// @brief Transition jumps, usable for going backward / forward, pausing 
int AnimationManager::getTransitionCoeff() const {
    return transitionCoeff;
}
void AnimationManager::setTransitionCoeff(int newCoeff) {
    transitionCoeff = newCoeff;
}

/// @brief Reset the animation clock
void AnimationManager::resetAnimationTimer() {
    timeStart = std::chrono::steady_clock::now();
}

/// @brief A Snapshot request function from UIManager to draw the data structures
///        on the screen
/// @return Interpolated snapshot
Snapshot AnimationManager::requestCurrentSnapshot(const std::string& DSTarget, float speed) {
    if (transitionCoeff == 0 || !hasMoreSteps(DSTarget)) {
        return stateManager -> getCurrentSnapShot(DSTarget);
    }
    
    float elapsed = elapsedSeconds() * speed;

    int currentIdx = stateManager -> getSnapshotIdx(DSTarget);
    float duration = stateManager -> getSnapshotTransitionAt(DSTarget, std::max(currentIdx, currentIdx + transitionCoeff));
    
    // If the animation is done, jump to the next transition
    if (duration <= elapsed) {
        stateManager -> setSnapshotIdx(DSTarget, currentIdx + transitionCoeff);
        resetAnimationTimer();
        
        if (isPaused()) transitionCoeff = 0;

        return stateManager -> getCurrentSnapShot(DSTarget);
    }
    
    return stateManager -> snapshotTransition(DSTarget, currentIdx, currentIdx + transitionCoeff, elapsed);
}
 
float AnimationManager::elapsedSeconds() {
    timeEnd = std::chrono::steady_clock::now();
    std::chrono::duration <float> timeElapsed = timeEnd - timeStart;

    return timeElapsed.count();
}