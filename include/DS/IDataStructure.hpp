#ifndef IDATASTRUCTURE_HPP
#define IDATASTRUCTURE_HPP

#include <Core/StateManager.hpp>
#include <Core/AnimationManager.hpp>

#include <Graphics/Panels/HighlightTheme.hpp>
#include <Graphics/Panels/PseudocodePanel.hpp>

/// @brief Data Structure Interface: Representation
class IDataStructure {
public:
    virtual void setStateManager(StateManager* source);

    virtual ~IDataStructure() = default;
    virtual bool insertNode(std::string value) = 0;
    virtual bool removeNode(std::string value) = 0;
    virtual bool searchNode(std::string value) { return 1; }
    virtual void clearAll() = 0;

    virtual void generateSnapshot(float duration, ChangeMap changeMap = ChangeMap(), PseudocodeSection pseudoFrame = PseudocodeSection::NONE, std::vector <int> pseudoActiveLines = {}) = 0;
    virtual std::string getDSID() const = 0;
protected:
    StateManager* stateManager = nullptr;

    ChangeMap singleChange(std::string value, ChangeInfo changeInfo);
};
#endif