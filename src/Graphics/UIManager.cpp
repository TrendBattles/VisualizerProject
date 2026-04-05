#include <Graphics/UIManager.hpp>
#include <Graphics/Helper.hpp>

/////////////////////////////////
///     UI Initialization     ///
/////////////////////////////////
UIManager::UIManager() {
    options = {"AVL_Tree", "Trie", "Linked_List", "Hash_Table", "Graph"};
    appSection = "AVL_Tree";

    pseudocodePanel = new PseudocodePanel();
}
UIManager::~UIManager() {
    delete pseudocodePanel;
}

void UIManager::setAnimationManager(AnimationManager* source) {
    animationManager = source;
}
const std::vector <std::string>& UIManager::getDSOptions() const {
    return options;
}
void UIManager::setScreenSection(const std::string& sectionID) {
    appSection = sectionID;
}
const std::string& UIManager::getScreenSection() const {
    return appSection;
}
bool UIManager::isVisualizing() const {
    return find(options.begin(), options.end(), appSection) != options.end();
}

//////////////////////////
///     UI Renders     ///
//////////////////////////

/// @brief Shape Drawing
/// @param shape 
void UIManager::drawShape(const ShapeState& shape) {
    switch(shape.sType) {
        case ShapeType::CIRCLE:
            DrawCircleV(shape.startPosition, shape.size + shape.outlineSize, shape.outlineColor);
            DrawCircleV(shape.startPosition, shape.size, shape.color);
            break;
        case ShapeType::LINE:
            DrawLineEx(shape.startPosition, shape.endPosition, shape.size + shape.outlineSize, shape.outlineColor);
            DrawLineEx(shape.startPosition, shape.endPosition, shape.size, shape.color);
            break;
        case ShapeType::RECTANGLE:
            DrawRectangleRoundedLinesEx(Helper::createRaylibRectangle(shape.startPosition, shape.endPosition), 0.3f, 10, shape.outlineSize, shape.outlineColor);
            DrawRectangleRounded(Helper::createRaylibRectangle(shape.startPosition, shape.endPosition), 0.3f, 10, shape.color);

            // DrawRectangleV(shape.startPosition, shape.endPosition - shape.startPosition, shape.color);
            // DrawRectangleLinesEx(Helper::createRaylibRectangle(shape.startPosition, shape.endPosition), shape.outlineSize, shape.outlineColor);
            break;
        case ShapeType::ARROW: {
            float headSize = std::min(shape.size * 3.0f, Vector2Distance(shape.startPosition, shape.endPosition) * 0.5f);
            Vector2 normVec = Vector2Normalize(shape.endPosition - shape.startPosition);

            Vector2 headCenter = shape.endPosition - normVec * headSize;

            Vector2 lineEndPosition = headCenter - normVec * (headSize * cos(PI / 3.0));
            DrawLineEx(shape.startPosition, lineEndPosition, shape.size + shape.outlineSize, shape.outlineColor);
            DrawLineEx(shape.startPosition, lineEndPosition, shape.size, shape.color);

            Vector2 v1 = headCenter + Vector2Rotate(normVec * (headSize + shape.outlineSize), -PI * 2 / 3.0);
            Vector2 v2 = headCenter + normVec * (headSize + shape.outlineSize);
            Vector2 v3 = headCenter + Vector2Rotate(normVec * (headSize + shape.outlineSize), +PI * 2 / 3.0);
            DrawTriangle(v1, v3, v2, shape.outlineColor);
            

            v1 = headCenter + Vector2Rotate(normVec * headSize, -PI * 2 / 3.0);
            v2 = headCenter + normVec * headSize;
            v3 = headCenter + Vector2Rotate(normVec * headSize, +PI * 2 / 3.0);
            DrawTriangle(v1, v3, v2, shape.color);
            
            break;
        }
        case ShapeType::TEXT:
            break;
    }

    if (shape.hasText) drawText(shape.content);
}


/// @brief Text Drawing
/// @param text 
void UIManager::drawText(const Text& text) {
    if (text.label.empty()) return;

    Vector2 finalPosition = text.position;
    if (text.centerToggle) {
        Vector2 labelSize = MeasureTextEx(text.font, text.label.c_str(), text.fontSize, text.spacing);
        finalPosition -= labelSize / 2;
    }

    DrawTextEx(text.font, text.label.c_str(), finalPosition, text.fontSize, text.spacing, text.textColor);
}

/// @brief Rendering the pseudocode panel
void UIManager::renderPseudocodePanel() {
    int currentIdx = animationManager -> getSnapshotIdx(getScreenSection());
    int coeff = animationManager -> getTransitionCoeff();
    
    // There is a limit of pseudocode demonstration when pausing / going forward
    if (coeff >= 0 && !animationManager -> canStepForward(getScreenSection())) {
        pseudocodePanel -> render();
        return;
    }

    // Getting the correctly labeled pseudocode highlight.
    // If coeff = +1 => show the next step
    // If coeff = 0 => show the current step
    // If coeff = -1 => show the previous step
    int pseudoTargetIdx = currentIdx + coeff;

    // Prevent out-of-bounds crashes
    pseudoTargetIdx = std::min(pseudoTargetIdx, animationManager -> getSize(getScreenSection()) - 1);
    pseudoTargetIdx = std::max(pseudoTargetIdx, 0);

    auto pseudoSignalRequest = animationManager -> getPseudoInfoAt(getScreenSection(), pseudoTargetIdx);
    pseudocodePanel -> render(pseudoSignalRequest);
}

/// @brief Rendering the data structure
void UIManager::renderSnapshot() {
    renderSnapshot(animationManager -> requestCurrentSnapshot(getScreenSection()));
}
void UIManager::renderSnapshot(const Snapshot& modifiedSnapshot) {
    for (const ShapeState& shape : modifiedSnapshot) {
        drawShape(shape);
    }
}