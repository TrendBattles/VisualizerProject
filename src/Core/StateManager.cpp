#include <Core/StateManager.hpp>
#include <Graphics/Helper.hpp>

///////////////////////////////////////
///       MEMBER FUNCTIONS          ///
//////////////////////////////////////

/// @brief Construction
StateManager::StateManager(const std::vector <std::string>& DSList) {
    for (const std::string& DSName : DSList) {
        historyMap.insert(std::make_pair(DSName, std::vector <HistoryFrame> (1)));
        stepMap.insert(std::make_pair(DSName, 0));
    }
}

/// @brief Destruction
StateManager::~StateManager() {
    historyMap.clear();
}

int StateManager::getSize(const std::string& DSTarget) {
    return getHistory(DSTarget).size();
}

/// @brief Adding snapshots to the container, allowing to render in order of layers
/// @param newVersion 
void StateManager::addSnapshot(const std::string& DSTarget, const HistoryFrame& newVersion) {
    std::vector <HistoryFrame>& targetList = historyMap[DSTarget];
    targetList.emplace_back(newVersion);

    Snapshot& updatedSnapshot = targetList.back().capturedSnapshot;
    std::sort(updatedSnapshot.begin(), updatedSnapshot.end(), [&] (ShapeState& A, ShapeState& B) {
        return A.layerID < B.layerID;
    });
}

/// @brief SnapshotIdx requests
int StateManager::getSnapshotIdx(const std::string& DSTarget) {
    auto it = stepMap.find(DSTarget);
    if (it == stepMap.end()) 
        throw std::out_of_range("[StateManager] Unknown DS key: " + DSTarget);

    return it -> second;
}
void StateManager::setSnapshotIdx(const std::string& DSTarget, int idx) {
    auto it = stepMap.find(DSTarget);
    if (it == stepMap.end()) 
        throw std::out_of_range("[StateManager] Unknown DS key: " + DSTarget);

    if (idx < 0 || idx >= getSize(DSTarget)) return;

    it -> second = idx;
}

//////////////////////////////////
///     HISTORY REQUESTS       ///
//////////////////////////////////
const Snapshot& StateManager::getCurrentSnapShot(const std::string& DSTarget) {
    return getSnapshotAt(DSTarget, stepMap[DSTarget]);
}
const Snapshot& StateManager::getSnapshotAt(const std::string& DSTarget, int idx) {
    return getHistoryFrame(DSTarget, idx).capturedSnapshot;
}
float StateManager::getCurrentSnapshotTransition(const std::string& DSTarget) {
    return getSnapshotTransitionAt(DSTarget, stepMap[DSTarget]);
}
float StateManager::getSnapshotTransitionAt(const std::string& DSTarget, int idx) {
    return getHistoryFrame(DSTarget, idx).duration;
}
std::pair <PseudocodeSection, std::vector <int>> StateManager::getCurrentPseudoInfo(const std::string& DSTarget) {
    return getPseudoInfoAt(DSTarget, stepMap[DSTarget]);
}
std::pair <PseudocodeSection, std::vector <int>> StateManager::getPseudoInfoAt(const std::string& DSTarget, int idx) {
    const HistoryFrame& historyFrame = getHistoryFrame(DSTarget, idx);

    return make_pair(historyFrame.pseudoFrame, historyFrame.pseudoActiveLines);
}


/// @brief Clearing the full history of the data structure
void StateManager::clearAllSnapshots(const std::string& DSTarget) {
    auto it = historyMap.find(DSTarget);
    if (it == historyMap.end())
        throw std::out_of_range("[StateManager] Unknown DS key: " + DSTarget);

    it -> second.clear();
    it -> second.shrink_to_fit();
    it -> second.emplace_back(emptyHistory);
    
    stepMap[DSTarget] = 0;
}

/// @brief History Traversals 
bool StateManager::canStepForward(const std::string& DSTarget) {
    auto it = stepMap.find(DSTarget);
    if (it == stepMap.end()) 
        throw std::out_of_range("[StateManager] Unknown DS key: " + DSTarget);

    return it -> second + 1 < getSize(DSTarget);
}
bool StateManager::canStepBackward(const std::string& DSTarget) {
    auto it = stepMap.find(DSTarget);
    if (it == stepMap.end()) 
        throw std::out_of_range("[StateManager] Unknown DS key: " + DSTarget);

    return it -> second > 0;
}
bool StateManager::stepForward(const std::string& DSTarget) {
    if (canStepForward(DSTarget)) {
        stepMap[DSTarget] += 1;
        return true;
    }

    return false;
}
bool StateManager::stepBackward(const std::string& DSTarget) {
    if (canStepBackward(DSTarget)) {
        stepMap[DSTarget] -= 1;
        return true;
    }

    return false;
}

/// @brief Making a interpolated snapshot between two chosen snapshots,
///        working even with OutOfBound cases but then this function will return values
///        based on the 2nd state.
/// @return  The resulting snapshot
///        Expected Time Complexity: O(N log N)
Snapshot StateManager::snapshotTransition(const std::string& DSTarget, int firstIdx, int secondIdx, float timePoint) {
    const Snapshot& oldSnapshot = getSnapshotAt(DSTarget, firstIdx);
    const Snapshot& newSnapshot = getSnapshotAt(DSTarget, secondIdx);

    float duration = getSnapshotTransitionAt(DSTarget, secondIdx);
    if (abs(duration) <= 1e-9) {
        return newSnapshot;
    }

    float rate = std::min(1.0f, timePoint / duration);
    // Cubic Eased-Out formula based on time to make the transition smoother
    // f(r) = 1 - (1 - r)^3
    float easedRate = 1.0f - std::pow(1.0f - rate, 3.0f);
    
    Snapshot mergedSnapshot;

    std::map <std::string, const ShapeState*> oldSnapshotMap;
    for (const ShapeState& oldShape : oldSnapshot) {
        oldSnapshotMap[oldShape.shapeID] = &oldShape;
    }

    // Handling new snapshots
    for (const ShapeState& newShape : newSnapshot) {
        ShapeState refShape = newShape;
        refShape.color.a = refShape.outlineColor.a = 0;
        refShape.content.textColor.a = 0;

        auto it = oldSnapshotMap.find(newShape.shapeID);
        if (it != oldSnapshotMap.end()) {
            refShape = *it -> second;
            oldSnapshotMap.erase(it);
        }

        mergedSnapshot.emplace_back(Helper::shapeTimeLerp(refShape, newShape, easedRate));    
    }

    //Removing traces of the modified shapes
    for (const ShapeState& newShape : newSnapshot) {
        auto it = oldSnapshotMap.find(newShape.shapeID);
        if (it != oldSnapshotMap.end()) oldSnapshotMap.erase(it);
    }

    // Handling transitions of removed shapes
    for (const ShapeState& oldShape : oldSnapshot) {
        if (!oldSnapshotMap.count(oldShape.shapeID)) continue;

        ShapeState refShape = oldShape;
        refShape.color.a = refShape.outlineColor.a = 0;
        refShape.content.textColor.a = 0;
        mergedSnapshot.emplace_back(Helper::shapeTimeLerp(oldShape, refShape, easedRate));    
    }

    std::sort(mergedSnapshot.begin(), mergedSnapshot.end(), [&] (ShapeState& A, ShapeState& B) {
        return A.layerID < B.layerID;
    });

    return mergedSnapshot;
}

const std::vector <HistoryFrame>& StateManager::getHistory(const std::string& DSTarget) {
    auto it = historyMap.find(DSTarget);
    if (it == historyMap.end())
        throw std::out_of_range("[StateManager] Unknown DS key: " + DSTarget);

    return it -> second;
}
const HistoryFrame& StateManager::getHistoryFrame(const std::string& DSTarget, int idx) {
    const std::vector <HistoryFrame>& fullHistory = getHistory(DSTarget);

    return idx < 0 || idx >= (int) fullHistory.size() ? emptyHistory : fullHistory[idx];
}