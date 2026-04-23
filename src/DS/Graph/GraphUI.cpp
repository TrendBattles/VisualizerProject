#include <DS/Graph/GraphUI.hpp>

GraphUI::GraphUI() {
    operationList = {"Init", "Insert Edge", "Remove Edge", "Remove Node", "Clear", "Dijkstra", "Kruskal"};
    TEXTBOX_LENGTH_LIMIT = 2;
    createNavbar();
    createField();
}

std::string GraphUI::getDSName() const { return "Graph"; }

/////////////////////////
///     UI RENDER     ///
/////////////////////////

void GraphUI::renderInitInput(Vector2 textboxPos) {
    Vector2 textboxSize = Vector2{FIELD_BUTTON_WIDTH * 2 + FIELD_GAP_X, 300.0f};
    DrawRectangleV(textboxPos, textboxSize, Fade(BLACK, 0.3f));
    DrawRectangleLinesEx(
        Rectangle { textboxPos.x, textboxPos.y, textboxSize.x, textboxSize.y },
        2.0f,
        GetColor(0x374151FF)
    );
    
    Vector2 alignOffset = Vector2{10.0f, 10.0f};
    Vector2 position = alignOffset;

    std::vector <std::string> rawParseBuffer = Helper::keywordParse(initBuffer);
    
    std::string tempBuffer = "";

    int bufferCnt = 0;
    for (const std::string& word : rawParseBuffer) {
        std::string nextBuffer = tempBuffer + (!tempBuffer.empty() ? " "  : "") + word;
        tempBuffer = nextBuffer;

        bufferCnt += 1;
        if (bufferCnt % 3) {
            continue;
        }

        DrawTextEx(CoreFonts::Aptos, tempBuffer.c_str(), position + textboxPos, 20.0f, 2.0f, WHITE);
        tempBuffer.clear();
        position += Vector2{0.0f, 20.0f};
    }
        
    if (!tempBuffer.empty()) {
        if (initBuffer.back() == ' ') tempBuffer += " ";
        DrawTextEx(CoreFonts::Aptos, tempBuffer.c_str(), position + textboxPos, 20.0f, 2.0f, WHITE);
    }
}

///////////////////////////
///     FIELD INPUT     ///
///////////////////////////

bool GraphUI::exceedsTextboxInitRange() {
    Vector2 textboxSize = Vector2{FIELD_BUTTON_WIDTH * 2 + FIELD_GAP_X, 300.0f};
    Vector2 alignOffset = Vector2{10.0f, 10.0f};
    Vector2 position = alignOffset;

    std::vector <std::string> rawParseBuffer = Helper::keywordParse(initBuffer);
    std::string tempBuffer = "";

    int bufferCnt = 0;
    for (const std::string& word : rawParseBuffer) {
        if (position.y > textboxSize.y - alignOffset.y) return true;

        std::string nextBuffer = tempBuffer + (!tempBuffer.empty() ? " "  : "") + word;
        bufferCnt += 1;
        if (bufferCnt % 3) continue;

        tempBuffer.clear();
        position += Vector2{0.0f, 20.0f};
    }
        
    return false;
}
void GraphUI::handleInputFieldInit() {
    std::vector <std::string> rawParseBuffer = Helper::keywordParse(initBuffer);
    int bufferCnt = 0;
    for (int i = 0; i < (int) rawParseBuffer.size(); ++i) {
        int value = 0;
        try {
            value = std::stoi(rawParseBuffer[i]);
        } catch (const std::invalid_argument& ia) {
            continue;
        } catch (const std::out_of_range& oor) {
            continue;
        } 

        if (bufferCnt % 3 == 2 && value > 99) continue;
        if (bufferCnt % 3 < 2 && (value <= 0 || value > 10)) continue; 

        rawParseBuffer[bufferCnt++] = std::to_string(value);
    }
    bufferCnt = bufferCnt / 3 * 3;

    initBuffer = "";
    for (int i = 0; i < bufferCnt; ++i) {
        if (!initBuffer.empty()) initBuffer += " ";
        initBuffer += rawParseBuffer[i];
    }

    while (exceedsTextboxInitRange()) {
        bufferCnt -= 3;

        initBuffer = "";
        for (int i = 0; i < bufferCnt; ++i) {
            if (!initBuffer.empty()) initBuffer += " ";
            initBuffer += rawParseBuffer[i];
        }
    }
}
