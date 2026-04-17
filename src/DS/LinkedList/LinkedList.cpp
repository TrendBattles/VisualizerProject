#include <DS/LinkedList/LinkedList.hpp>
#include <Graphics/Helper.hpp>

LinkedList::LinkedList() {

}
LinkedList::~LinkedList() {
    destroyList(head);
    head = nullptr;

    if (stateManager != nullptr)
        stateManager -> clearAllSnapshots(getDSID());
}
void LinkedList::setStateManager(StateManager* source) {
    stateManager = source;
    generateSnapshot(0.0f);
}

void LinkedList::destroyList(LLNode* node) {
    if (node == nullptr) return;

    destroyList(node -> next);

    delete node;
    node = nullptr;
}


///////////////////////////////////////
///     SUPPORTIVE FUNCTIONS        ///
///////////////////////////////////////
bool LinkedList::insertNode(std::string value) {
    int valueParse = 0;
    try {
        valueParse = std::stoi(value);
    } catch (const std::invalid_argument& ia) {
	    std::cerr << "[INPUT ERROR]: " << ia.what() << '\n';
        return false;
    }
    
    insertNode(valueParse);
    return true;
}
bool LinkedList::removeNode(std::string value) {
    int valueParse = 0;
    try {
        valueParse = std::stoi(value);
    } catch (const std::invalid_argument& ia) {
	    std::cerr << "[INPUT ERROR]: " << ia.what() << '\n';
        return false;
    }
    
    removeNode(valueParse);
    return true;
}
bool LinkedList::searchNode(std::string value) {
    int valueParse = 0;
    try {
        valueParse = std::stoi(value);
    } catch (const std::invalid_argument& ia) {
	    std::cerr << "[INPUT ERROR]: " << ia.what() << '\n';
        return false;
    }

    return searchNode(valueParse);
}
bool LinkedList::updateNode(std::string oldValue, std::string newValue) {
    int oldVal = 0, newVal = 0;
    try {
        oldVal = std::stoi(oldValue);
        newVal = std::stoi(newValue);
    } catch (const std::invalid_argument& ia) {
	    std::cerr << "[INPUT ERROR]: " << ia.what() << '\n';
        return false;
    }

    updateNode(oldVal, newVal);
    return true;
}
void LinkedList::clearAll() {
    LLpointer = 0;

    destroyList(head);
    head = nullptr;

    if (stateManager != nullptr)
        stateManager -> clearAllSnapshots(getDSID());
    generateSnapshot(0.0f);
}

std::string LinkedList::getDSID() const { return "Linked_List"; }


/////////////////////////////////
///     INSERT FUNCTIONS      ///
/////////////////////////////////
void LinkedList::insertNode(int value) {
    LLNode* newNode = new LLNode(value, ++LLpointer);
    newNode -> next = head;
    generateSnapshot(0.5f, ChangeMap(), PseudocodeSection::LINKED_LIST_INSERT, {1, 2});

    head = newNode;
    generateSnapshot(
        0.5f,
        singleChange(Helper::rectangleStringBuffer(newNode -> shapeID), Highlight::INSERTED),
        PseudocodeSection::LINKED_LIST_INSERT,
        {3}
    );

    generateSnapshot(1.0f);
}

///////////////////////////////
///     REMOVE FUNCTIONS    ///
///////////////////////////////
void LinkedList::remove(int value) {
    generateSnapshot(
        0.3f,
        ChangeMap(),
        PseudocodeSection::LINKED_LIST_REMOVE,
        {1}
    );

    if (head == nullptr) {
        generateSnapshot(
            0.3f,
            ChangeMap(),
            PseudocodeSection::LINKED_LIST_REMOVE,
            {2}
        );
        return;
    }

    generateSnapshot(
        0.3f,
        singleChange(Helper::rectangleStringBuffer(head -> shapeID), Highlight::ACTIVE),
        PseudocodeSection::LINKED_LIST_REMOVE,
        {3}
    );

    if (head -> value == value) {
        generateSnapshot(
            0.5f,
            singleChange(Helper::rectangleStringBuffer(head -> shapeID), Highlight::REMOVED),
            PseudocodeSection::LINKED_LIST_REMOVE,
            {4}
        );

        LLNode* nxt = head -> next;
        delete head;
        head = nxt;

        generateSnapshot(
            0.5f,
            head == nullptr ? ChangeMap() : singleChange(Helper::rectangleStringBuffer(head -> shapeID), Highlight::MODIFIED),
            PseudocodeSection::LINKED_LIST_REMOVE,
            {5}
        );

        return;
    }

    LLNode* iter = head;
    generateSnapshot(
        0.3f,
        singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
        PseudocodeSection::LINKED_LIST_REMOVE,
        {6}
    );

    generateSnapshot(
        0.3f,
        singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
        PseudocodeSection::LINKED_LIST_REMOVE,
        {7}
    );

    while (iter -> next != nullptr && iter -> next -> value != value) {
        iter = iter -> next;

        generateSnapshot(
            0.3f,
            singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
            PseudocodeSection::LINKED_LIST_REMOVE,
            {8}
        );

        generateSnapshot(
            0.3f,
            singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
            PseudocodeSection::LINKED_LIST_REMOVE,
            {7}
        );
    }

    generateSnapshot(
        0.3f,
        singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
        PseudocodeSection::LINKED_LIST_REMOVE,
        {9}
    );

    if (iter -> next == nullptr) return;

    LLNode* remNode = iter -> next;
    iter -> next = remNode -> next;
    delete remNode;

    generateSnapshot(
        0.5f,
        singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::MODIFIED),
        PseudocodeSection::LINKED_LIST_REMOVE,
        {10}
    );
}
void LinkedList::removeNode(int value) {
    remove(value);
    generateSnapshot(1.0f);
}

///////////////////////////////
///     SEARCH FUNCTIONS    ///
///////////////////////////////

bool LinkedList::search(int value) {
    generateSnapshot(
        0.3f,
        head == nullptr ? ChangeMap() : singleChange(Helper::rectangleStringBuffer(head -> shapeID), Highlight::ACTIVE),
        PseudocodeSection::LINKED_LIST_SEARCH,
        {1}
    );

    LLNode* iter = head;

    generateSnapshot(
        0.3f,
        head == nullptr ? ChangeMap() : singleChange(Helper::rectangleStringBuffer(head -> shapeID), Highlight::ACTIVE),
        PseudocodeSection::LINKED_LIST_SEARCH,
        {2}
    );

    while (iter != nullptr) {
        generateSnapshot(
            0.3f,
            singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
            PseudocodeSection::LINKED_LIST_SEARCH,
            {3}
        );

        if (iter -> value == value) {
            generateSnapshot(
                1.5f,
                singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::FOUND),
                PseudocodeSection::LINKED_LIST_SEARCH,
                {4}
            );

            return true;
        }

        iter = iter -> next;
        generateSnapshot(
            0.3f,
            iter == nullptr ? ChangeMap() : singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
            PseudocodeSection::LINKED_LIST_SEARCH,
            {5}
        );

        generateSnapshot(
            0.3f,
            iter == nullptr ? ChangeMap() : singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
            PseudocodeSection::LINKED_LIST_SEARCH,
            {2}
        );
    }

    generateSnapshot(
        0.5f,
        ChangeMap(),
        PseudocodeSection::LINKED_LIST_SEARCH,
        {6}
    );

    return false;
}
bool LinkedList::searchNode(int value) {
    return search(value);
}

///////////////////////////////
///     UPDATE FUNCTIONS    ///
///////////////////////////////
void LinkedList::updateNode(int oldValue, int newValue) {
    LLNode* iter = head;

    generateSnapshot(
        0.3f,
        iter == nullptr ? ChangeMap() : singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
        PseudocodeSection::LINKED_LIST_UPDATE,
        {1, 2}
    );

    while (iter != nullptr) {
        generateSnapshot(
            0.3f,
            singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
            PseudocodeSection::LINKED_LIST_UPDATE,
            {3}
        );

        if (iter -> value == oldValue) {
            iter -> value = newValue;
            generateSnapshot(
                0.5f,
                singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::MODIFIED),
                PseudocodeSection::LINKED_LIST_UPDATE,
                {4, 5}
            );
            break;
        }
        
        iter = iter -> next;
        generateSnapshot(
            0.3f,
            iter == nullptr ? ChangeMap() : singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
            PseudocodeSection::LINKED_LIST_UPDATE,
            {6}
        );
        generateSnapshot(
            0.3f,
            iter == nullptr ? ChangeMap() : singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
            PseudocodeSection::LINKED_LIST_UPDATE,
            {2}
        );
    }

    generateSnapshot(1.0f);
}

/////////////////////////
///     SNAPSHOTS     ///
/////////////////////////

Snapshot LinkedList::buildSnapshot(const ChangeMap& changeMap) {
    Vector2 startPos = Vector2{-GetScreenWidth() * 0.5f + 220.0f, -GetScreenHeight() * 0.5f + 80.0f};
    const float RECT_WIDTH = 100.0f, RECT_HEIGHT = 60.0f;
    const float RECT_GAP_X = 70.0f, RECT_GAP_Y = 42.0f;
    const float outlineSize = 5.0f;
    const int MAX_NODE = 6;
    
    // A template to create the shapes of linked list nodes
    auto createLLRect = [&] (std::string bufferID, Vector2 origin, std::string label) -> ShapeState {
        ShapeState resRect = Helper::createRectangle(
            bufferID,
            origin, origin + Vector2{RECT_WIDTH, RECT_HEIGHT},
            outlineSize, 
            Highlight::NORMAL.backgroundColor, Highlight::NORMAL.outlineColor,
            0
        );

        resRect.setText(Helper::createText(
            label, CoreFonts::Consolas, 25.0f, 2.5f, 
            origin + Vector2{RECT_WIDTH, RECT_HEIGHT} * 0.5f, 
            GetColor(0xEDF2F4FF)
        ));
        resRect.setTextCenter(true);

        return resRect;
    };

    Snapshot storage;
    storage.emplace_back(createLLRect(
        Helper::rectangleStringBuffer(0), 
        startPos, "HEAD"
    ));

    if (head == nullptr) return storage;

    LLNode* iter = head;
    int idx = 0;

    // Linked List iteration to identify the position of the nodes
    while (iter != nullptr) {
        idx += 1;
        int row = idx / MAX_NODE, column = idx % MAX_NODE;

        float displacement_x = (RECT_WIDTH + RECT_GAP_X + outlineSize * 2) * (row % 2 ? MAX_NODE - 1 - column : column);
        float displacement_y = (RECT_HEIGHT + RECT_GAP_Y + outlineSize * 2) * row;

        ShapeState nodeShape = createLLRect(
            Helper::rectangleStringBuffer(iter -> shapeID),
            startPos + Vector2{displacement_x, displacement_y},
            std::to_string(iter -> value)
        );

        storage.emplace_back(nodeShape);

        iter = iter -> next;
    }

    auto RectCenter = [&] (int idx) -> Vector2 {
        return Vector2Lerp(storage[idx].startPosition, storage[idx].endPosition, 0.5f);
    };
    auto createArrow = [&] (Vector2 parentVec, Vector2 childVec, int childValue, float padding) -> ShapeState {
        Vector2 normVec = Vector2Normalize(childVec - parentVec);

        return Helper::createArrow(
            Helper::arrowPointStringBuffer(std::to_string(childValue)),
            parentVec + normVec * padding,
            childVec - normVec * padding,
            3.0f, 1.5f,
            GetColor(0x8D99AEFF), GetColor(0x4A4F60FF),
            -100
        );
    };

    iter = head;
    // Arrow Drawing
    for (int i = 1; i + 1 <= idx; ++i) {
        Vector2 parentVec = RectCenter(i), childVec = RectCenter(i + 1);
        if ((i + 1) % MAX_NODE == 0) {
            storage.emplace_back(createArrow(parentVec, childVec, iter -> shapeID, RECT_HEIGHT * 0.5f + outlineSize));
        } else {
            storage.emplace_back(createArrow(parentVec, childVec, iter -> shapeID, RECT_WIDTH * 0.5f + outlineSize));
        }
        iter = iter -> next;
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
void LinkedList::generateSnapshot(float duration, ChangeMap changeMap, PseudocodeSection pseudoFrame, std::vector <int> pseudoActiveLines) {
    if (stateManager == nullptr) {
        std::cerr << "[ERROR]: State Manager not found\n";
        return;
    }

    stateManager -> addSnapshot(getDSID(), HistoryFrame{ buildSnapshot(changeMap), duration, pseudoFrame, pseudoActiveLines});
}