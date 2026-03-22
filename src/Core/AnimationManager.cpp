#include <Core/AnimationManager.hpp>
#include <algorithm>
#include <cmath>

void AnimationManager::setStateManager(StateManager* source) {
    stateManager = source;
}
void AnimationManager::setUIManager(UIManager* source) {
    uiManager = source;
}

bool AnimationManager::hasMoreSteps(const std::string& DSTarget) {
    if (isPauseToggle) return false;

    return stateManager -> stepForward(DSTarget);
}

void AnimationManager::setStartAnimation() {
    if (isPauseToggle) return;

    timeStart = std::chrono::steady_clock::now();
}

void AnimationManager::sendAnimationSignals(const std::string& DSTarget) {
    if (!hasMoreSteps(DSTarget)) {
        uiManager -> renderSnapshot(stateManager -> getCurrentSnapShot(DSTarget));
        return;
    }

    timeEnd = std::chrono::steady_clock::now();
    std::chrono::duration <float> timeElapsed = timeEnd - timeStart;

    float timeHolder = std::min(timeElapsed.count(), animationFrameTime);

    int currentIdx = stateManager -> getSnapshotIdx(DSTarget);

    uiManager -> renderSnapshot(stateManager -> snapshotTransition(DSTarget, currentIdx, currentIdx + 1, timeHolder / animationFrameTime));

    if (std::fabs(timeHolder - animationFrameTime) <= 1e-9) {
        stateManager -> tryStepForward(DSTarget);
        setStartAnimation();
    }
}