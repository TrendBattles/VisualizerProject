#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include <RendererCommon.hpp>
#include <vector>

/// @brief A manager designed to keep track of different versions of DS
typedef std::vector <ShapeState> Snapshot;

class StateManager {
public:
    void addSnapshot(const Snapshot& newVersion);

    bool stepForward();
    bool stepBackward();
    bool tryStepForward();
    bool tryStepBackward();

    Snapshot getCurrentSnapShot();
private:
    std::vector <Snapshot> history;
    int currentStep = 0;
};


#endif