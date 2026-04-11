#include <Graphics/Panels/PseudocodePanel.hpp>
#include <Graphics/Helper.hpp>
#include <sstream>

PseudocodePanel::PseudocodePanel() {
    init();
}
PseudocodePanel::~PseudocodePanel() = default;

///////////////////////////////
///     INITIALIZATION      ///
///////////////////////////////
void PseudocodePanel::setAnimationManager(AnimationManager* source) {
    animationManager = source;
}
void PseudocodePanel::setUIManager(UIManager* source) {
    uiManager = source;
}

void PseudocodePanel::init() {
    ORIGIN_POS = Vector2{GetScreenWidth() - 10.0f, panelSize.y + 10.0f} - panelSize;

    auto createButton = [&] (std::string prefix, std::string label, Color backgroundColor, Color textColor) -> Button {
        return Helper::createButton(
            Helper::createRectangle(
                "PP" + Helper::rectangleStringBuffer(prefix),
                Vector2{panelSize.x - toggleSize.x, 0}, Vector2{panelSize.x, toggleSize.y},
                2.0f, 
                backgroundColor, BLACK,
                1
            ),

            Helper::createText(label, CoreFonts::Consolas, 25.0f, 2.5f, {0, 0}, textColor)
        );
    }; 

    toggleButton.setButtonSettings(ButtonState::ACTIVE, createButton("_active", "x", GetColor(0xC9AE8AFF), BLACK));
    toggleButton.setButtonSettings(ButtonState::INACTIVE, createButton("_inactive", "+", GetColor(0xC9AE8AFF), BLACK));

    pseudocodeContent[PseudocodeSection::NONE] = {};

    //AVL_Tree
    pseudocodeContent[PseudocodeSection::AVL_INSERT] = {
        "insert(node, key):",
        "  if node is null:",
        "    return new Node(key)",
        "  if node.key == key:",
        "    return node",
        "  if node.key < key:",
        "    node.right = insert(node.right, key)",
        "  else:",
        "    node.left = insert(node.left, key)",
        "  node = rebalance(node)",
        "  return node",
    };
    
    pseudocodeContent[PseudocodeSection::AVL_REMOVE] = {
        "remove(node, key):",
        "  if node is null:",
        "    return null",
        "  if node.key < key:",
        "    node.right = remove(node.right, key)",
        "  else if node.key > key:",
        "    node.left = remove(node.left, key)",
        "  else:",
        "    if node has 0 or 1 child:",
        "      return removeSingleChild(node)",
        "    successor = findSuccessor(node.right)",
        "    node.key = successor.key",
        "    node.right = removeSuccessor(node.right)",
        "  node = rebalance(node)",
        "  return node"
    };

    pseudocodeContent[PseudocodeSection::AVL_REMOVE_SUCCESSOR] = {
        "removeSuccessor(node):",
        "  if node.left is null:",
        "    return removeSingleChild(node)",
        "  node.left = removeSuccessor(node.left)",
        "  node = rebalance(node)",
        "  return node"
    };

    pseudocodeContent[PseudocodeSection::AVL_SEARCH] = {
        "search(node, key):",
        "  if node is null:",
        "    return null",
        "  if node.key == key:",
        "    return node",
        "  if node.key < key:",
        "    return search(node.right, key)",
        "  else:",
        "    return search(node.left, key)"
    };

    pseudocodeContent[PseudocodeSection::AVL_REBALANCE] = {
        "rebalance(node):",
        "  bf = balanceFactor(node)",
        "  if bf > 1:",
        "    if balanceFactor(node.left) < 0:",
        "      node.left = rotateLeft(node.left)",
        "    node = rotateRight(node)",
        "  if bf < -1:",
        "    if balanceFactor(node.right) > 0:",
        "      node.right = rotateRight(node.right)",
        "    node = rotateLeft(node)",
        "  return node"
    };
}

void PseudocodePanel::render(std::string DSTarget) {
    int currentIdx = animationManager -> getSnapshotIdx(DSTarget);
    int coeff = animationManager -> getTransitionCoeff();
    
    // There is a limit of pseudocode demonstration when pausing / going forward
    if (coeff >= 0 && !animationManager -> canStepForward(DSTarget)) {
        renderPseudoInfo();
        return;
    }

    // Getting the correctly labeled pseudocode highlight.
    // If coeff = +1 => show the next step
    // If coeff = 0 => show the current step
    // If coeff = -1 => show the previous step
    int pseudoTargetIdx = currentIdx + coeff;

    // Prevent out-of-bounds crashes
    pseudoTargetIdx = std::min(pseudoTargetIdx, animationManager -> getSize(DSTarget) - 1);
    pseudoTargetIdx = std::max(pseudoTargetIdx, 0);

    auto pseudoSignalRequest = animationManager -> getPseudoInfoAt(DSTarget, pseudoTargetIdx);
    renderPseudoInfo(pseudoSignalRequest);
}

void PseudocodePanel::renderPseudoInfo(std::pair <PseudocodeSection, std::vector <int>> pseudoInfo) {
    drawLogicalCode(pseudoInfo.first, pseudoInfo.second);
}

/// @brief User Input processing 
void PseudocodePanel::processInput(RawInputEvent nextInput) {
    if (nextInput.inputType != RawInputEvent::InputType::LEFT_MOUSE_CLICKED) return;
    if (toggleButton.contains(nextInput.position - ORIGIN_POS)) {
        bool isPanelHidden = toggleButton.getButtonState() == ButtonState::INACTIVE;
        toggleButton.setButtonState(isPanelHidden ? ButtonState::ACTIVE : ButtonState::INACTIVE);
    }
}

void PseudocodePanel::drawLogicalCode(PseudocodeSection pseudoFrame, std::vector <int> pseudoActiveLines) {
    if (toggleButton.getButtonState() == ButtonState::INACTIVE) {
        // If the pseudocode panel is closed, make sure to leave a toggle button to open up.

        ShapeState toggleShape = toggleButton.getButtonShape().getBackground();
        toggleShape.setText(toggleButton.getButtonShape().getLabel());
        toggleShape.positionTransitAllBy(ORIGIN_POS);

        uiManager -> drawShape(toggleShape);
        return;
    }

    // Pseudocode highlight background
    DrawRectangleV(ORIGIN_POS, panelSize, Fade(BLACK, 0.3f));
    DrawRectangleLinesEx(
        Rectangle { ORIGIN_POS.x, ORIGIN_POS.y, panelSize.x, panelSize.y },
        1.0f,
        GetColor(0x374151FF)
    );
    
    // Pseudocode Toggle button
    ShapeState toggleShape = toggleButton.getButtonShape().getBackground();
    toggleShape.setText(toggleButton.getButtonShape().getLabel());
    toggleShape.positionTransitAllBy(ORIGIN_POS);
    uiManager -> drawShape(toggleShape);

    Vector2 currentTextPos = ORIGIN_POS + textOffset;
    std::vector <std::string>& pseudoText = pseudocodeContent[pseudoFrame];
    
    // Pseudocode highlight text
    auto CustomDrawText = [&] (std::string content, Font textFont, Color textColor) -> void {
        if (content.empty()) return;

        int indentCnt = 0;
        while (indentCnt < (int) content.size() and content[indentCnt] == ' ') indentCnt += 1;

        std::string indentStr = indentCnt == 0 ? "" : std::string(indentCnt, ' ');
        content = content.substr(indentCnt);

        std::string buffer = "";
        for (char x : content) {
            buffer.push_back(x);
            Vector2 size = MeasureTextEx(textFont, (indentStr + buffer).c_str(), fontSize, 0.1 * fontSize);

            if (currentTextPos.x + size.x <= ORIGIN_POS.x + panelSize.x) {
                continue;
            }

            std::string nextBuffer = "";
            while (!buffer.empty() && !isspace(buffer.back())) {
                nextBuffer.push_back(buffer.back());
                buffer.pop_back();
            }

            std::reverse(nextBuffer.begin(), nextBuffer.end());
            DrawTextEx(textFont, (indentStr + buffer).c_str(), currentTextPos, fontSize, 0.1f * fontSize, textColor);
            currentTextPos += textPadding;
            buffer = nextBuffer;
        }

        if (!buffer.empty()) {
            DrawTextEx(textFont, (indentStr + buffer).c_str(), currentTextPos, fontSize, 0.1f * fontSize, textColor);
            currentTextPos += textPadding;
        }
    };

    int p = 0;
    for (int i = 0; i < (int) pseudoText.size(); ++i) {
        DrawTextEx(CoreFonts::CascadiaMonoRegular, std::to_string(i + 1).c_str(), Vector2{ORIGIN_POS.x, currentTextPos.y}, fontSize, 0.1f * fontSize, WHITE);
        if (p < (int) pseudoActiveLines.size() && pseudoActiveLines[p] == i) {
            CustomDrawText(pseudoText[i], CoreFonts::CascadiaMonoBold, RED);
            p += 1;
        } else {
            CustomDrawText(pseudoText[i], CoreFonts::CascadiaMonoRegular, WHITE);
        }
    }
}