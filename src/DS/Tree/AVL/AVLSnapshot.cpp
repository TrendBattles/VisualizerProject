#include <DS/Tree/AVL/AVL.hpp>

#include <Graphics/Panels/HighlightTheme.hpp>
#include <Graphics/Helper.hpp>

/// @brief Builds a Snapshot of the current tree state without touching the StateManager.
///        Separates layout/serialization from persistence so that callers
///        can decorate the result before committing it.
Snapshot AVL::buildSnapshot(const ChangeMap& changeMap) {
    if (root == nullptr) return Snapshot();

    const float radius = 30.0f, outlineSize = 6.0f, lengthVertical = 100;
    const float padding = 5.0f;

    // Calculating the position of the nodes efficiently
    // Expected time complexity: O(N)
    // Approach: InOrder Traversal
    Snapshot storage;
    auto DrawTree = [&] (auto self, AVLNode* node, int& nodeCnt, int depth) -> Vector2 {
        if (node == nullptr) return Vector2{0, 0};

        // Drawing edges
        // A child always has exactly one parent, so this ID is stable across
        // rotations - the edge will smoothly interpolate its endpoints
        // rather than blinking out and back in with a new parent-keyed ID.
        auto DrawEdge = [&] (Vector2 parentVec, Vector2 childVec, int childValue) -> void {
            Vector2 normVec = Vector2Normalize(childVec - parentVec);

            ShapeState arrow = Helper::createArrow(
                Helper::arrowPointStringBuffer(std::to_string(childValue)),
                parentVec + normVec * (radius + outlineSize),
                childVec - normVec * (radius + outlineSize),
                3.0f, 1.5f, 
                GetColor(0x8D99AEFF), GetColor(0x4A4F60FF),
                -100
            );

            storage.emplace_back(arrow);
        };

        //Left Edge
        Vector2 parentVec = {0, 0};
        if (node -> leftChild != nullptr) {
            Vector2 childVec = self(self, node -> leftChild, nodeCnt, depth + 1);
            parentVec = {nodeCnt * (radius + outlineSize + padding), lengthVertical * depth};

            DrawEdge(parentVec, childVec, node -> leftChild -> key);
        }

        parentVec = {nodeCnt * (radius + outlineSize + padding), lengthVertical * depth};
        nodeCnt += 1;

        //Right Edge
        if (node -> rightChild != nullptr) {
            Vector2 childVec = self(self, node -> rightChild, nodeCnt, depth + 1);
            DrawEdge(parentVec, childVec, node -> rightChild -> key);
        }

        //Node's sprite
        ShapeState nodeShape = Helper::createCircle(
            Helper::nodeStringBuffer(std::to_string(node -> key)),
            parentVec,
            radius, outlineSize, 
            Highlight::NORMAL.backgroundColor, Highlight::NORMAL.outlineColor,
            0
        );

        nodeShape.setText(Helper::createText(std::to_string(node -> key), CoreFonts::Consolas, 20.0f, 2.0f, parentVec, GetColor(0xEDF2F4FF)));
        nodeShape.setTextCenter(true);

        storage.emplace_back(nodeShape);
        return parentVec;
    };

    int nodeCnt = 0;
    // Offset all positions so the root sits near the top-centre of the screen.
    // Nodes are appended last in the storage, so we can iterate the whole
    // list once without a second pass.
    Vector2 rootPosition = {0, -GetScreenHeight() / 2.0f + radius * 2};
    Vector2 transitDelta = rootPosition - DrawTree(DrawTree, root, nodeCnt, 0);

    for (ShapeState& tempoState : storage) {
        tempoState.positionTransitAllBy(transitDelta);
    }

    // Override the color information of the target shapes
    for (ShapeState& finalState : storage) {
        auto it = changeMap.find(finalState.shapeID);
        if (it == changeMap.end()) continue;

        finalState.color = it -> second.backgroundColor;
        finalState.outlineColor = it -> second.outlineColor;
        finalState.content.textColor = it -> second.textColor;
    }

    return storage;
}

void AVL::generateSnapshot(float duration, ChangeMap changeMap, PseudocodeSection pseudoFrame, std::vector <int> pseudoActiveLines) {
    if (stateManager == nullptr) {
        std::cerr << "[ERROR]: State Manager not found\n";
        return;
    }

    stateManager -> addSnapshot("AVL_Tree", HistoryFrame{ buildSnapshot(changeMap), duration, pseudoFrame, pseudoActiveLines});
}