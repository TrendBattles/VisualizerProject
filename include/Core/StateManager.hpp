#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include <Graphics/RendererCommon.hpp>
#include <vector>
#include <map>

/// @brief A manager designed to keep track of different versions of DS
typedef std::vector <ShapeState> Snapshot;

struct HistoryFrame {
    Snapshot capturedSnapshot;
    float duration = 0.75f;
};

class StateManager {
public:
    StateManager(const std::vector <std::string>& DSList);
    ~StateManager();

    int getSize(const std::string& DSTarget);
    void addSnapshot(const std::string& DSTarget, const HistoryFrame& newVersion);
    int getSnapshotIdx(const std::string& DSTarget);
    void setSnapshotIdx(const std::string& DSTarget, int idx);

    const Snapshot& getCurrentSnapShot(const std::string& DSTarget);
    const Snapshot& getSnapshotAt(const std::string& DSTarget, int idx);
    float getCurrentSnapshotTransition(const std::string& DSTarget);
    float getSnapshotTransitionAt(const std::string& DSTarget, int idx);
    void clearAllSnapshots(const std::string& DSTarget);

    bool stepForward(const std::string& DSTarget);
    bool stepBackward(const std::string& DSTarget);
    bool tryStepForward(const std::string& DSTarget);
    bool tryStepBackward(const std::string& DSTarget);

    Snapshot snapshotTransition(const std::string& DSTarget, int firstIdx, int secondIdx, float timePoint);
private:
    std::map <std::string, std::vector <HistoryFrame>> historyMap;
    std::map <std::string, int> stepMap;

    const Snapshot emptySnapshot = Snapshot();
    const HistoryFrame emptyHistory = {Snapshot(), 0.75f};
};


#endif