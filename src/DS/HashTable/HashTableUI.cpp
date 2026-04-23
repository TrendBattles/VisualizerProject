#include <DS/HashTable/HashTableUI.hpp>
#include <Graphics/Helper.hpp>

HashTableUI::HashTableUI() {
    operationList = {"Init", "Insert", "Remove", "Search", "Clear"};
    TEXTBOX_LENGTH_LIMIT = 4;
    createNavbar();
    createField();
}

std::string HashTableUI::getDSName() const { return "Hash_Table"; }

//////////////////////////////////////
///     FIELD INPUT PROCESSING     ///
//////////////////////////////////////
bool HashTableUI::exceedsTextboxInitRange() {
    Vector2 textboxSize = Vector2{FIELD_BUTTON_WIDTH * 2 + FIELD_GAP_X, 300.0f};
    Vector2 alignOffset = Vector2{10.0f, 10.0f};
    Vector2 position = alignOffset;

    std::stringstream ss(initBuffer);
    std::string word = "", tempBuffer = "";

    while (ss >> word) {
        std::string nextBuffer = tempBuffer + (!tempBuffer.empty() ? ", " : "") + word;

        Vector2 size = MeasureTextEx(CoreFonts::Aptos, nextBuffer.c_str(), 20.0f, 2.0f);
        if (size.x + alignOffset.x <= textboxSize.x - alignOffset.x) {
            tempBuffer = nextBuffer;
            continue;
        }

        position += Vector2{0.0f, 20.0f};
        tempBuffer = word;

        if (position.y + 20.0f > textboxSize.y - alignOffset.y) {
            return true;
        }
    }
        
    if (tempBuffer.empty() || position.y + 20.0f <= textboxSize.y - alignOffset.y) {
        return false;
    }
    return true;
}
void HashTableUI::handleInputFieldInit() {
    std::string inputStr = initBuffer;

    initBuffer = "";
    std::stringstream ss(inputStr);
    std::string number;
    while (ss >> number) {
        int value = 0;
        try {
            value = std::stoi(number);
        } catch (const std::invalid_argument& ia) {
            continue;
        } catch (const std::out_of_range& oor) {
            continue;
        }

        if (value > 9999) {
            continue;
        }

        if (!initBuffer.empty()) initBuffer += " ";
        initBuffer += std::to_string(value);
    }

    while (exceedsTextboxInitRange()) {
        if (initBuffer.back() == ' ') {
            while (!initBuffer.empty() && initBuffer.back() == ' ') initBuffer.pop_back();
        } else {
            while (!initBuffer.empty() && isdigit(initBuffer.back())) initBuffer.pop_back();
            while (!initBuffer.empty() && initBuffer.back() == ' ') initBuffer.pop_back();
        }
    }
}