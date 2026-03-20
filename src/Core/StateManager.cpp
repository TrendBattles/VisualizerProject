#include <Core/StateManager.hpp>
#include <algorithm>

///////////////////////////////////////
///       MEMBER FUNCTIONS          ///
//////////////////////////////////////

/// @brief Construction
StateManager::StateManager(const std::vector <std::string>& DSList) {
    for (const std::string& DSName : DSList) {
        historyMap.insert(std::make_pair(DSName, std::vector <Snapshot> ()));
        stepMap.insert(std::make_pair(DSName, 0));
    }
}

/// @brief Destruction
StateManager::~StateManager() {
    historyMap.clear();
}

int StateManager::getSize(const std::string& DSTarget) {
    return (int) historyMap[DSTarget].size();
}

/// @brief Adding snapshots to the container, allowing to render in order of layers
/// @param newVersion 
void StateManager::addSnapshot(const std::string& DSTarget, const Snapshot& newVersion) {
    std::vector <Snapshot>& targetList = historyMap[DSTarget];
    int& currentStep = stepMap[DSTarget];

    targetList.emplace_back(newVersion);
    currentStep = (int) targetList.size() - 1;

    std::sort(targetList.back().begin(), targetList.back().end(), [&] (ShapeState& A, ShapeState& B) {
        return A.layerID < B.layerID;
    });
}

/// @brief Snapshot Access Functions 
int StateManager::getSnapshotIdx(const std::string& DSTarget) {
    return stepMap[DSTarget];
}
void StateManager::setSnapshotIdx(const std::string& DSTarget, int idx) {
    if (idx < 0 || idx >= getSize(DSTarget)) return;

    stepMap[DSTarget] = idx;
}
void StateManager::clearAllSnapshots(const std::string& DSTarget) {
    historyMap[DSTarget].clear();
    historyMap[DSTarget].shrink_to_fit();

    stepMap[DSTarget] = 0;
}

/// @brief History Traversals 
bool StateManager::stepForward(const std::string& DSTarget) {
    return stepMap[DSTarget] + 1 < getSize(DSTarget);
}
bool StateManager::stepBackward(const std::string& DSTarget) {
    return stepMap[DSTarget] > 0;
}
bool StateManager::tryStepForward(const std::string& DSTarget) {
    if (stepForward(DSTarget)) {
        stepMap[DSTarget] += 1;
        return true;
    }

    return false;
}
bool StateManager::tryStepBackward(const std::string& DSTarget) {
    if (stepBackward(DSTarget)) {
        stepMap[DSTarget] -= 1;
        return true;
    }

    return false;
}
const Snapshot& StateManager::getCurrentSnapShot(const std::string& DSTarget) {
    return getSnapshotAt(DSTarget, stepMap[DSTarget]);
}
const Snapshot& StateManager::getSnapshotAt(const std::string& DSTarget, int idx) {
    std::vector <Snapshot>& targetList = historyMap[DSTarget];

    return idx >= 0 && idx < (int) targetList.size() ? targetList[idx] : emptySnapshot;
}