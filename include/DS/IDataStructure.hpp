#ifndef IDATASTRUCTURE_HPP
#define IDATASTRUCTURE_HPP

#include <Core/StateManager.hpp>
#include <Core/AnimationManager.hpp>
#include <Core/InputManager.hpp>

#include <Graphics/Panels/HighlightTheme.hpp>
#include <Graphics/Panels/PseudocodePanel.hpp>

/// @brief Data Structure Interface: Representation
class IDataStructure {
public:
    virtual void setStateManager(StateManager* source);

    virtual ~IDataStructure() = default;
    virtual bool insertNode(std::string value) { return true; }
    virtual bool removeNode(std::string value) { return true; }
    virtual bool searchNode(std::string value) { return true; }
    virtual bool updateNode(std::string oldValue, std::string newValue) { return true; }


    virtual bool insertEdge(std::string FEdge, std::string SEdge, std::string weight) { return true; }
    virtual bool removeEdge(std::string FEdge, std::string SEdge) { return true; }

    virtual bool runDijkstra(std::string start, std::string dest) { return true; }
    virtual bool runKruskal() { return true; }

    virtual void clearAll() = 0;
    virtual void update() {}
    virtual void reset() {}

    virtual bool processInput(RawInputEvent nextInput) { return false; }

    virtual void generateSnapshot(float duration, ChangeMap changeMap = ChangeMap(), PseudocodeSection pseudoFrame = PseudocodeSection::NONE, std::vector <int> pseudoActiveLines = {}) = 0;
    virtual Snapshot getCurrentSnapshot() { return Snapshot(); }
    virtual std::string getDSID() const = 0;
protected:
    StateManager* stateManager = nullptr;

    ChangeMap singleChange(std::string value, ChangeInfo changeInfo);
};
#endif