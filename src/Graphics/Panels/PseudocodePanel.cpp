#include <Graphics/Panels/PseudocodePanel.hpp>

PseudocodePanel::PseudocodePanel() {
    init();
}
PseudocodePanel::~PseudocodePanel() {

}

///////////////////////////////
///     INITIALIZATION      ///
///////////////////////////////
void PseudocodePanel::init() {
    pseudocodeContent[PseudocodeSection::NONE] = {};

    //AVL_Tree
    pseudocodeContent[PseudocodeSection::AVL_INSERT] = {
        "insert(node, key):",
        "    if node is null:",
        "        return new Node(key)",
        "    if node.key == key:",
        "        return node",
        "    if node.key < key:",
        "        node.right = insert(node.right, key)",
        "    else:",
        "        node.left = insert(node.left, key)",
        "    node = rebalance(node)",
        "    return node",
    };
    
    pseudocodeContent[PseudocodeSection::AVL_REMOVE] = {
        "remove(node, key):",
        "    if node is null:",
        "        return null",
        "    if node.key < key:",
        "        node.right = remove(node.right, key)",
        "    else if node.key > key:",
        "        node.left = remove(node.left, key)",
        "    else:",
        "        if node has 0 or 1 child:",
        "            return removeSingleChild(node)",
        "        successor = findSuccessor(node.right)",
        "        node.key = successor.key",
        "        node.right = remove(node.right, successor.key)",
        "    node = rebalance(node)",
        "    return node"
    };

    pseudocodeContent[PseudocodeSection::AVL_REMOVE_SUCCESSOR] = {
        "removeSuccessor(node):",
        "    if node.left is null:",
        "        return removeSingleChild(node)",
        "    node.left = removeSuccessor(node.left)",
        "    node = rebalance(node)",
        "    return node"
    };

    pseudocodeContent[PseudocodeSection::AVL_SEARCH] = {
        "search(node, key):",
        "    if node is null:",
        "        return null",
        "    if node.key == key:",
        "        return node",
        "    if node.key < key:",
        "        return search(node.right, key)",
        "    else:",
        "        return search(node.left, key)"
    };

    pseudocodeContent[PseudocodeSection::AVL_REBALANCE] = {
        "rebalance(node):",
        "    updateHeight(node)",
        "    bf = balanceFactor(node)",
        "    if bf > 1: // Left-Heavy",
        "        if balanceFactor(node.left) < 0:",
        "            node.left = rotateLeft(node.left)",
        "        node = rotateRight(node)",
        "    if bf < -1: // Right-Heavy",
        "        if balanceFactor(node.right) > 0:",
        "            node.right = rotateRight(node.right)",
        "        node = rotateLeft(node)",
        "    return node"
    };
}
void PseudocodePanel::render(std::pair <PseudocodeSection, std::vector <int>> pseudoInfo) {
    drawLogicalCode(pseudoInfo.first, pseudoInfo.second);
}

void PseudocodePanel::drawLogicalCode(PseudocodeSection pseudoFrame, std::vector <int> pseudoActiveLines) {
    Vector2 ORIGIN_POS = Vector2{(float) GetScreenWidth(), panelSize.y} - panelSize;
    
    // Pseudocode highlight background
    DrawRectangleV(ORIGIN_POS, panelSize, Fade(BLACK, 0.3f));
    DrawRectangleLinesEx(
        Rectangle { ORIGIN_POS.x, ORIGIN_POS.y, panelSize.x, panelSize.y },
        1.0f,
        GetColor(0x374151FF)
    );

    Vector2 currentTextPos = ORIGIN_POS + textOffset;
    std::vector <std::string>& pseudoText = pseudocodeContent[pseudoFrame];
    
    int p = 0;
    
    // Pseudocode highlight text
    for (int i = 0; i < (int) pseudoText.size(); ++i) {
        DrawTextEx(CoreFonts::CascadiaMonoRegular, std::to_string(i + 1).c_str(), Vector2{ORIGIN_POS.x, currentTextPos.y}, fontSize, 0.1f * fontSize, WHITE);
        if (p < (int) pseudoActiveLines.size() && pseudoActiveLines[p] == i) {
            DrawTextEx(CoreFonts::CascadiaMonoBold, pseudoText[i].c_str(), currentTextPos, fontSize, 0.1f * fontSize, RED);
            p += 1;
        } else {
            DrawTextEx(CoreFonts::CascadiaMonoRegular, pseudoText[i].c_str(), currentTextPos, fontSize, 0.1f * fontSize, WHITE);
        }

        currentTextPos += textPadding;
    }
}