#include <StateManager.hpp>
#include <algorithm>

///////////////////////////////////////
///       MEMBER FUNCTIONS          ///
//////////////////////////////////////


/// @brief Adding snapshots to the container, allowing to render in order of layers
/// @param newVersion 
void StateManager::addSnapshot(const Snapshot& newVersion) {
    if (history.empty()) {
        history.emplace_back(newVersion);
    } else {
        history.erase(history.begin() + currentStep + 1, history.end());
        history.emplace_back(newVersion);

        currentStep += 1;
    }

    std::sort(history.back().begin(), history.back().end(), [&] (ShapeState& A, ShapeState& B) {
        return A.layerID < B.layerID;
    });
}

bool StateManager::stepForward() {
    return currentStep + 1 < (int) history.size();
}

bool StateManager::stepBackward() {
    return currentStep > 0;
}

bool StateManager::tryStepForward() {
    if (stepForward()) {
        currentStep += 1;
        return true;
    }

    return false;
}

bool StateManager::tryStepBackward() {
    if (stepBackward()) {
        currentStep -= 1;
        return true;
    }

    return false;
}

Snapshot StateManager::getCurrentSnapShot() {
    return history.empty() ? Snapshot() : history[currentStep];
}