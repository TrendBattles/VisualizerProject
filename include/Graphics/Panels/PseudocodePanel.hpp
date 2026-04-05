#ifndef PSEUDOCODEPANEL_HPP
#define PSEUDOCODEPANEL_HPP

#include <Graphics/RendererCommon.hpp>

enum class PseudocodeSection {
    NONE,
    AVL_INSERT,
    AVL_REMOVE,
    AVL_REMOVE_SUCCESSOR,
    AVL_SEARCH,
    AVL_REBALANCE
};

class PseudocodePanel {
public:
    PseudocodePanel();
    ~PseudocodePanel();
    void render(std::pair <PseudocodeSection, std::vector <int>> pseudoInfo = {PseudocodeSection::NONE, {}}); 
private:
    void init();
    std::map<PseudocodeSection, std::vector <std::string>> pseudocodeContent;

    const Vector2 panelSize = {400.0f, 500.0f}, textOffset = {30.0f, 30.0f}, textPadding = {0.0f, 20.0f};
    const float fontSize = 15.0f;
    void drawLogicalCode(PseudocodeSection pseudoFrame, std::vector <int> pseudoActiveLines);
};
#endif