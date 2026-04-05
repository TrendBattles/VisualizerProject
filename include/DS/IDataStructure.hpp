#ifndef IDATASTRUCTURE_HPP
#define IDATASTRUCTURE_HPP

#include <Core/StateManager.hpp>
#include <Core/AnimationManager.hpp>

#include <Graphics/Panels/HighlightTheme.hpp>
#include <Graphics/Panels/PseudocodePanel.hpp>

/// @brief Data Structure Interface: Representation
class IDataStructure {
public:
    void setStateManager(StateManager* source);
    void setAnimationManager(AnimationManager* source);

    virtual ~IDataStructure() = default;
    virtual int insertNode(std::string value) = 0;
    virtual int removeNode(std::string value) = 0;
    virtual int searchNode(std::string value) { return 1; }
    virtual void clearAll() = 0;

    virtual void generateSnapshot(float duration, ChangeMap changeMap, PseudocodeSection pseudoFrame = PseudocodeSection::NONE, std::vector <int> pseudoActiveLines = {}) = 0;
protected:
    StateManager* stateManager = nullptr;
    AnimationManager* animationManager = nullptr;

    ChangeMap singleChange(std::string value, ChangeInfo changeInfo);
};
#endif