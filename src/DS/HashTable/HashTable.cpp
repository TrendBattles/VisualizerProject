#include <DS/HashTable/HashTable.hpp>
#include <Graphics/Helper.hpp>

HashTable::HashTable() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        table[i] = nullptr;
    }
}
HashTable::~HashTable() {
    destroyTable();
    
    if (stateManager != nullptr)
        stateManager -> clearAllSnapshots(getDSID());
}
void HashTable::setStateManager(StateManager* source) {
    stateManager = source;
    generateSnapshot(0.0f);
}

void HashTable::destroyTable() {
    HashPointer = 0;
    for (int i = 0; i < TABLE_SIZE; ++i) {
        while (table[i] != nullptr) {
            HashNode* nxt = table[i] -> next;
            delete table[i];
            table[i] = nxt;
        }
    }
}
void HashTable::clearAll() {
    destroyTable();
    if (stateManager != nullptr) {
        stateManager -> clearAllSnapshots(getDSID());
    }
    generateSnapshot(0.0f);
}
std::string HashTable::getDSID() const { return "Hash_Table"; }

///////////////////////////////////
///     SUPPORTIVE FUNCTIONS    ///
///////////////////////////////////
bool HashTable::insertNode(std::string value) {
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
bool HashTable::removeNode(std::string value) {
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
bool HashTable::searchNode(std::string value) {
    int valueParse = 0;
    try {
        valueParse = std::stoi(value);
    } catch (const std::invalid_argument& ia) {
	    std::cerr << "[INPUT ERROR]: " << ia.what() << '\n';
        return false;
    }
    
    return searchNode(valueParse);
}

///////////////////////////////  
///     INSERT FUNCTIONS    ///
///////////////////////////////

void HashTable::insertNode(int value) {
    int hashID = value % TABLE_SIZE;
    HashNode* node = new HashNode(value, ++HashPointer);
    node -> next = table[hashID];

    generateSnapshot(0.5f, ChangeMap(), PseudocodeSection::HASH_TABLE_INSERT, {1, 2});
    
    table[hashID] = node;

    generateSnapshot(
        0.5f,
        singleChange(Helper::rectangleStringBuffer(node -> shapeID), Highlight::INSERTED),
        PseudocodeSection::HASH_TABLE_INSERT,
        {3}
    );

    generateSnapshot(1.0f);
}

///////////////////////////////
///     REMOVE FUNCTIONS    ///
///////////////////////////////

void HashTable::remove(int value) {
    int hashID = value % TABLE_SIZE;
    HashNode*& head = table[hashID];

    generateSnapshot(
        0.3f,
        ChangeMap(),
        PseudocodeSection::HASH_TABLE_REMOVE,
        {1, 2}
    );

    if (head == nullptr) {
        generateSnapshot(
            0.3f,
            ChangeMap(),
            PseudocodeSection::HASH_TABLE_REMOVE,
            {3}
        );
        return;
    }
    
    generateSnapshot(
        0.3f,
        singleChange(Helper::rectangleStringBuffer(head -> shapeID), Highlight::ACTIVE),
        PseudocodeSection::HASH_TABLE_REMOVE,
        {4}
    );

    if (head -> value == value) {
        generateSnapshot(
            0.3f,
            singleChange(Helper::rectangleStringBuffer(head -> shapeID), Highlight::REMOVED),
            PseudocodeSection::HASH_TABLE_REMOVE,
            {5}
        );

        HashNode* nxt = head -> next;
        delete head;
        head = nxt;

        generateSnapshot(
            0.3f,
            head == nullptr ? ChangeMap() : singleChange(Helper::rectangleStringBuffer(head -> shapeID), Highlight::MODIFIED),
            PseudocodeSection::HASH_TABLE_REMOVE,
            {6}
        );

        return;
    }

    HashNode* iter = head;
    generateSnapshot(
        0.3f,
        singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
        PseudocodeSection::HASH_TABLE_REMOVE,
        {7, 8}
    );
    while (iter -> next != nullptr && iter -> next -> value != value) {
        generateSnapshot(
            0.3f,
            singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
            PseudocodeSection::HASH_TABLE_REMOVE,
            {9}
        );
        iter = iter -> next;
        generateSnapshot(
            0.3f,
            singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
            PseudocodeSection::HASH_TABLE_REMOVE,
            {8}
        );
    }

    generateSnapshot(
        0.3f,
        singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
        PseudocodeSection::HASH_TABLE_REMOVE,
        {10}
    );
    if (iter -> next != nullptr) {
        generateSnapshot(
            0.3f,
            singleChange(Helper::rectangleStringBuffer(iter -> next -> shapeID), Highlight::REMOVED),
            PseudocodeSection::HASH_TABLE_REMOVE,
            {11}
        );
        HashNode* remNode = iter -> next;
        iter -> next = remNode -> next;

        delete remNode;

        generateSnapshot(
            0.3f,
            singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::MODIFIED),
            PseudocodeSection::HASH_TABLE_REMOVE,
            {11}
        );
    }
}
void HashTable::removeNode(int value) {
    remove(value);
    generateSnapshot(1.0f);
}

bool HashTable::searchNode(int value) {
    int hashID = value % TABLE_SIZE;
    HashNode* iter = table[hashID];

    generateSnapshot(
        0.3f,
        iter == nullptr ? ChangeMap() : singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
        PseudocodeSection::HASH_TABLE_SEARCH,
        {1, 2}
    );

    while (iter != nullptr) {
        generateSnapshot(
            0.3f,
            singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
            PseudocodeSection::HASH_TABLE_SEARCH,
            {3}
        );
        if (iter -> value == value) {
            generateSnapshot(
                1.5f,
                singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::FOUND),
                PseudocodeSection::HASH_TABLE_SEARCH,
                {4}
            );
            return true;
        }

        iter = iter -> next;
        generateSnapshot(
            0.3f,
            iter == nullptr ? ChangeMap() : singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
            PseudocodeSection::HASH_TABLE_SEARCH,
            {5}
        );
        generateSnapshot(
            0.3f,
            iter == nullptr ? ChangeMap() : singleChange(Helper::rectangleStringBuffer(iter -> shapeID), Highlight::ACTIVE),
            PseudocodeSection::HASH_TABLE_SEARCH,
            {2}
        );
    }

    generateSnapshot(1.0f, ChangeMap(), PseudocodeSection::HASH_TABLE_SEARCH, {6});
    return false;
}

///////////////////////////////////
///     SNAPSHOT GENERATION     ///
///////////////////////////////////
std::vector <HashNode*> HashTable::getHashNodeList(int id) {
    if (id < 0 || id >= TABLE_SIZE) return std::vector <HashNode*> ();

    std::vector <HashNode*> arr;
    HashNode* head = table[id];

    while (head != nullptr) {
        arr.push_back(head);
        head = head -> next;
    }

    return arr;
}

Snapshot HashTable::buildSnapshot(ChangeMap changeMap) {
    const float RECT_WIDTH = 100.0f, RECT_HEIGHT = 60.0f;
    const float RECT_GAP_X = 70.0f, RECT_GAP_Y = 42.0f;
    const float outlineSize = 5.0f;
    const Vector2 origin = Vector2{-GetScreenWidth() * 0.5f + 220.0f, -GetScreenHeight() * 0.5f + 30.0f};

    // A template to create the shapes of linked list nodes
    auto createLLRect = [&] (std::string bufferID, Vector2 root, std::string label, float fontSize) -> ShapeState {
        ShapeState resRect = Helper::createRectangle(
            bufferID,
            root, root + Vector2{RECT_WIDTH, RECT_HEIGHT},
            outlineSize, 
            Highlight::NORMAL.backgroundColor, Highlight::NORMAL.outlineColor,
            0
        );

        resRect.setText(Helper::createText(
            label, CoreFonts::Consolas, fontSize, 0.1f * fontSize, 
            root + Vector2{RECT_WIDTH, RECT_HEIGHT} * 0.5f, 
            GetColor(0xEDF2F4FF)
        ));
        resRect.setTextCenter(true);

        return resRect;
    };

    Snapshot storage;
    auto createArrow = [&] (Vector2 parentVec, Vector2 childVec, int childValue) -> ShapeState {
        Vector2 normVec = Vector2Normalize(childVec - parentVec);
        float padding = RECT_WIDTH * 0.5f + outlineSize;
        return Helper::createArrow(
            Helper::arrowPointStringBuffer(std::to_string(childValue)),
            parentVec + normVec * padding,
            childVec - normVec * padding,
            3.0f, 1.5f,
            GetColor(0x8D99AEFF), GetColor(0x4A4F60FF),
            -100
        );
    };

    for (int id = 0; id < TABLE_SIZE; ++id) {
        Vector2 startPos = origin + Vector2{0.0f, id * (RECT_HEIGHT + RECT_GAP_Y + 2 * outlineSize)};

        storage.emplace_back(createLLRect(Helper::rectangleStringBuffer("Table" + std::to_string(id)), startPos, "TABLE " + std::to_string(id), 20.0f));
        storage.back().color = GetColor(0);

        auto hashChain = getHashNodeList(id);
        for (int i = 0; i < (int) hashChain.size(); ++i) {
            Vector2 currentPos = startPos + Vector2{(i + 1) * (RECT_WIDTH + RECT_GAP_X + 2 * outlineSize), 0.0f};
            Vector2 nextPos = currentPos + Vector2{RECT_WIDTH + RECT_GAP_X + 2 * outlineSize, 0.0f};

            ShapeState nodeShape = createLLRect(
                Helper::rectangleStringBuffer(hashChain[i] -> shapeID),
                currentPos,
                std::to_string(hashChain[i] -> value),
                25.0f
            );

            storage.emplace_back(nodeShape);
            if (i + 1 < (int) hashChain.size()) {
                storage.emplace_back(
                    createArrow(currentPos + Vector2{RECT_WIDTH, RECT_HEIGHT} * 0.5f, nextPos + Vector2{RECT_WIDTH, RECT_HEIGHT} * 0.5f, hashChain[i + 1] -> shapeID)
                );
            }
        }
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

void HashTable::generateSnapshot(float duration, ChangeMap changeMap, PseudocodeSection pseudoFrame, std::vector <int> pseudoActiveLines) {
    if (stateManager == nullptr) {
        std::cerr << "[ERROR]: State Manager not found\n";
        return;
    }

    stateManager -> addSnapshot(getDSID(), HistoryFrame{ buildSnapshot(changeMap), duration, pseudoFrame, pseudoActiveLines});
}
