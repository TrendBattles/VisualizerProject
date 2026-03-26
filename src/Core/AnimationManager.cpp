#include <Core/AnimationManager.hpp>
#include <algorithm>
#include <cmath>

void AnimationManager::setStateManager(StateManager* source) {
    stateManager = source;
}

/// @brief Returns true if there are more animation steps to play for the given DS.
bool AnimationManager::hasMoreSteps(const std::string& DSTarget) {
    if (isPauseToggle) return false;

    return stateManager -> stepForward(DSTarget);
}

void AnimationManager::setStartAnimation() {
    if (isPauseToggle) return;

    timeStart = std::chrono::steady_clock::now();
}

/// @brief A Snapshot request function from UIManager to draw the data structures
///        on the screen
/// @return Interpolated snapshot
Snapshot AnimationManager::requestCurrentSnapshot(const std::string& DSTarget) {
    if (!hasMoreSteps(DSTarget)) {
        return stateManager -> getCurrentSnapShot(DSTarget);
    }

    timeEnd = std::chrono::steady_clock::now();
    std::chrono::duration <float> timeElapsed = timeEnd - timeStart;
    int currentIdx = stateManager -> getSnapshotIdx(DSTarget);

    float countdown = stateManager -> getSnapshotTransitionAt(DSTarget, currentIdx + 1) - timeElapsed.count();
    if (countdown <= 1e-9) {
        stateManager -> tryStepForward(DSTarget);
        setStartAnimation();
    }

    return stateManager -> snapshotTransition(DSTarget, currentIdx, currentIdx + 1, timeElapsed.count());
}