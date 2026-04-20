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
    ORIGIN_POS = Vector2{GetScreenWidth() - 10.0f, GetScreenHeight() - 10.0f} - panelSize;

    auto createButton = [&] (std::string prefix, std::string label, Vector2 position, Color backgroundColor, Color outlineColor, Color textColor) -> Button {
        return Helper::createButton(
            Helper::createRectangle(
                "PP" + Helper::rectangleStringBuffer(prefix),
                position, position + toggleSize,
                2.0f, 
                backgroundColor, outlineColor,
                1
            ),

            Helper::createText(label, CoreFonts::Consolas, 25.0f, 2.5f, {0, 0}, textColor)
        );
    }; 

    activeButton.setButtonSettings(ButtonState::ACTIVE, createButton("_active", "x", {0.0f, 0.0f}, GetColor(0), GetColor(0), WHITE));
    activeButton.setButtonSettings(ButtonState::HOVERED, createButton("_active_hovered", "x", {0.0f, 0.0f}, GetColor(0), GetColor(0x81A1C1FF), GetColor(0xECEFF4FF)));
    inactiveButton.setButtonSettings(ButtonState::ACTIVE, createButton("_inactive", "+", panelSize - toggleSize, Fade(BLACK, 0.3f), GetColor(0), WHITE));
    inactiveButton.setButtonSettings(ButtonState::HOVERED, createButton("_inactive_hovered", "+", panelSize - toggleSize, GetColor(0), GetColor(0x81A1C1FF), GetColor(0xECEFF4FF)));
    
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

    // Trie
    pseudocodeContent[PseudocodeSection::TRIE_INSERT] = {
        "insert(node, str, idx):",
        "  if idx == length(str):",
        "    node.count += 1",
        "    return",
        "  nxtNode = node.child[str[idx]]",
        "  if nxtNode is null:",
        "    initialize nxtNode",
        "  insert(nxtNode, str, idx + 1)"
    };

    pseudocodeContent[PseudocodeSection::TRIE_REMOVE] = {
        "remove(node, str, idx):",
        "  if node is null:",
        "    return null",
        "  if idx == length(str):",
        "    node.count -= 1",
        "    return node",
        "  nxtNode = node.child[str[idx]]",
        "  if nxtNode is null:",
        "    return node",
        "  nxtNode = remove(nxtNode, str, idx + 1)",
        "  if nxtNode is truncated:",
        "       delete nxtNode",
        "  return node"
    };

    pseudocodeContent[PseudocodeSection::TRIE_SEARCH] = {
        "search(node, str, idx):",
        "  if node is null:",
        "    return false",
        "  if idx == length(str):",
        "    return succeeded if node.count > 0",
        "  nxtNode = node.child[str[idx]]",
        "  return search(nxtNode, str + 1, idx)"
    };

    // Linked_List
    pseudocodeContent[PseudocodeSection::LINKED_LIST_INSERT] = {
        "insert(value):",
        "  newNode = createNode(value)",
        "  newNode.next = head",
        "  head = newNode"
    };
    pseudocodeContent[PseudocodeSection::LINKED_LIST_REMOVE] = {
        "remove(value):",
        "  if head is null:",
        "    return",
        "  if head.value == value:",
        "    head = head.next",
        "    return",
        "  iter = head",
        "  while iter.next isn't null and iter.next.value != value:",
        "    iter = iter.next",
        "  if iter.next isn't null:",
        "    iter.next = iter.next.next"
    };
    pseudocodeContent[PseudocodeSection::LINKED_LIST_SEARCH] = {
        "search(value):",
        "  iter = head",
        "  while iter isn't null:",
        "    if iter.value == value:",
        "      return true",
        "    iter = iter.next",
        "  return false"
    };
    pseudocodeContent[PseudocodeSection::LINKED_LIST_UPDATE] = {
        "update(oldValue, newValue):",
        "  iter = head",
        "  while iter isn't null:",
        "    if iter.value == oldValue:",
        "      iter.value = newValue",
        "      break",
        "    iter = iter.next",
    };

    // Hash_Table
    pseudocodeContent[PseudocodeSection::HASH_TABLE_INSERT] = {
        "insert(value):",
        "  newNode = createNode(value)",
        "  newNode.next = table[value % TABLE_SIZE]",
        "  table[value % TABLE_SIZE] = newNode"
    };
    pseudocodeContent[PseudocodeSection::HASH_TABLE_REMOVE] = {
        "remove(value):",
        "  head = table[value % TABLE_SIZE]"
        "  if head is null:",
        "    return",
        "  if head.value == value:",
        "    head = head.next",
        "    return",
        "  iter = head",
        "  while iter.next isn't null and iter.next.value != value:",
        "    iter = iter.next",
        "  if iter.next isn't null:",
        "    iter.next = iter.next.next"
    };
    pseudocodeContent[PseudocodeSection::HASH_TABLE_SEARCH] = {
        "search(value):",
        "  iter = table[value % TABLE_SIZE]",
        "  while iter isn't null:",
        "    if iter.value == value:",
        "      return true",
        "    iter = iter.next",
        "  return false"
    };

    // Graph
    pseudocodeContent[PseudocodeSection::DIJKSTRA] = {
        "dijkstra(u, v):",
        "  initalize minDist, heap",
        "  minDist[u] = 0",
        "  add (minDist[u], u) to heap",
        "  while heap isn't empty:",
        "    d, i = heap.pop()",
        "    if d > minDist[i]:",
        "      continue",
        "    for (j, w) in adjList(i):",
        "      if minDist[j] > minDist[i] + w",
        "         update minDist[j]",
        "         add (minDist[j], j) to heap",
        "  if minDist[v] != inf",
        "     trace the path from u to v"
    };
    pseudocodeContent[PseudocodeSection::KRUSKAL] = {
        "kruskal():",
        "  sort edges in order of increasing weight",
        "  root[MAX_NODES] = {1,2,...,MAX_NODES}",
        "  for (u, v) in edges:",
        "    if root[u] != root[v]:",
        "       join edge (u, v)",
        "       update root[]"
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

void PseudocodePanel::update() {
    Vector2 mouse = GetMousePosition();

    if (panelStatus == PanelStatus::OPEN) {
        activeButton.setButtonState(activeButton.contains(mouse - ORIGIN_POS) ? ButtonState::HOVERED : ButtonState::ACTIVE);

        inactiveButton.setButtonState(ButtonState::ACTIVE);
    } else {
        inactiveButton.setButtonState(inactiveButton.contains(mouse - ORIGIN_POS) ? ButtonState::HOVERED : ButtonState::ACTIVE);
        activeButton.setButtonState(ButtonState::ACTIVE);
    }
}

/// @brief User Input processing 
///        Returns true if the toggle button is triggered
bool PseudocodePanel::processInput(RawInputEvent nextInput) {
    if (nextInput.inputType != RawInputEvent::InputType::LEFT_MOUSE_CLICKED) return false;
    
    // Opening / Closing the panel
    if (panelStatus == PanelStatus::OPEN && activeButton.contains(nextInput.position - ORIGIN_POS)) {
        panelStatus = PanelStatus::CLOSED;
        return true;
    } else if (panelStatus == PanelStatus::CLOSED && inactiveButton.contains(nextInput.position - ORIGIN_POS)) {
        panelStatus = PanelStatus::OPEN;
        return true;
    }

    return false;
}

// Rendering pseudocode on the panel
void PseudocodePanel::renderPseudoInfo(std::pair <PseudocodeSection, std::vector <int>> pseudoInfo) {
    PseudocodeSection pseudoFrame = pseudoInfo.first; 
    std::vector <int> pseudoActiveLines = pseudoInfo.second;

    if (panelStatus == PanelStatus::CLOSED) {
        // If the pseudocode panel is closed, make sure to leave a toggle button to open up.

        ShapeState toggleShape = inactiveButton.getButtonShape().getBackground();
        toggleShape.setText(inactiveButton.getButtonShape().getLabel());
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
    ShapeState toggleShape = activeButton.getButtonShape().getBackground();
    toggleShape.setText(activeButton.getButtonShape().getLabel());
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