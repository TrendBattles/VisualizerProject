#include <Core/StateManager.hpp>
#include <Graphics/Helper.hpp>
#include <algorithm>
#include <cassert>

///////////////////////////////////////
///       MEMBER FUNCTIONS          ///
//////////////////////////////////////

/// @brief Construction
StateManager::StateManager(const std::vector <std::string>& DSList) {
    for (const std::string& DSName : DSList) {
        historyMap.insert(std::make_pair(DSName, std::vector <Snapshot> (1, emptySnapshot)));
        stepMap.insert(std::make_pair(DSName, 0));
    }
}

/// @brief Destruction
StateManager::~StateManager() {
    historyMap.clear();
}

int StateManager::getSize(const std::string& DSTarget) {
    assert(historyMap.find(DSTarget) != historyMap.end());

    return (int) historyMap[DSTarget].size();
}

/// @brief Adding snapshots to the container, allowing to render in order of layers
/// @param newVersion 
void StateManager::addSnapshot(const std::string& DSTarget, const Snapshot& newVersion) {
    std::vector <Snapshot>& targetList = historyMap[DSTarget];
    targetList.emplace_back(newVersion);

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
    std::vector <Snapshot>& targetList = historyMap[DSTarget];
    targetList.clear();
    targetList.shrink_to_fit();
    targetList.emplace_back(emptySnapshot);
    
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

Snapshot StateManager::snapshotTransition(const std::string& DSTarget, int firstIdx, int secondIdx, float rate) {
    const Snapshot& oldSnapshot = getSnapshotAt(DSTarget, firstIdx);
    const Snapshot& newSnapshot = getSnapshotAt(DSTarget, secondIdx);

    Snapshot mergedSnapshot;

    for (const ShapeState& newShape : newSnapshot) {
        ShapeState refShape = newShape;
        refShape.color.a = refShape.outlineColor.a = 0;
        refShape.content.textColor.a = 0;

        for (const ShapeState& oldShape : oldSnapshot) {
            if (oldShape.shapeID == newShape.shapeID) {
                refShape = oldShape;
                break;
            }
        }

        mergedSnapshot.emplace_back(Helper::shapeTimeLerp(refShape, newShape, rate));    
    }

    for (const ShapeState& oldShape : oldSnapshot) {
        bool shapeIDFound = false;

        for (const ShapeState& newShape : newSnapshot) {
            if (oldShape.shapeID == newShape.shapeID) {
                shapeIDFound = true;
                break;
            }
        }

        if (shapeIDFound) continue;

        ShapeState refShape = oldShape;
        refShape.color.a = refShape.outlineColor.a = 0;
        refShape.content.textColor.a = 0;
        mergedSnapshot.emplace_back(Helper::shapeTimeLerp(oldShape, refShape, rate));    
    }

    std::sort(mergedSnapshot.begin(), mergedSnapshot.end(), [&] (ShapeState& A, ShapeState& B) {
        return A.layerID < B.layerID;
    });

    return mergedSnapshot;
}