#ifndef RENDERERCOMMON_HPP
#define RENDERERCOMMON_HPP

#include <Common.hpp>

/// @brief Builtin features for advanced settings
struct Text {
    std::string label = "";
    Font font = GetFontDefault();
    float fontSize = 10.0f, spacing = 5.0f;
    Color textColor = BLACK;
    Vector2 position = Vector2{0.0f, 0.0f};
    bool centerToggle = false;

    void setCenter(bool isCentered);
    void clearTextBuffer();
    std::string getLabel() const;
};

enum ShapeType { 
    CIRCLE,
    LINE,
    RECTANGLE,
    ROUNDED_RECTANGLE,
    TEXT,
    ARROW
};

/// @brief The root definition of the shapes in the program
///        From this, we can develop generalized patterns
struct ShapeState {
    //Identity
    std::string shapeID = "";

    //Geometry
    Vector2 startPosition = Vector2{0.0f, 0.0f}, endPosition = Vector2{0.0f, 0.0f};
    float size = 0, outlineSize = 0, roundness = 0;

    //Recognition
    ShapeType sType = ShapeType::TEXT;
    int layerID = 0;

    //Style
    Color color = Color{0, 0, 0, 0}, outlineColor = Color{0, 0, 0, 0};
    bool isVisible = true;
    
    //Content
    Text content;
    bool hasText = true;
    
    void setColor(Color color, Color outlineColor);

    void setShapeVisibility(bool isTrue);
    void setText(Text content);
    void setTextVisibility(bool isTrue);
    void setTextCenter(bool isCentered);

    void positionTransitAllBy(Vector2 delta);
};  

struct CoreFonts {
    inline static Font CascadiaMonoRegular, CascadiaMonoBold;
    inline static Font Consolas;
    inline static Font Aptos, AptosBold;
    
    // Call this ONCE in AppLoop initialization
    static void load();

    // Call this ONCE before app closes
    static void unload();
};

/// @brief Pseudocode Dashboard Indicator
enum class PseudocodeSection {
    NONE,
    AVL_INSERT,
    AVL_REMOVE,
    AVL_REMOVE_SUCCESSOR,
    AVL_SEARCH,
    AVL_REBALANCE,

    TRIE_INSERT,
    TRIE_REMOVE,
    TRIE_SEARCH,
};

/// @brief A storage designedx to keep track of different versions of DS
typedef std::vector <ShapeState> Snapshot;

struct HistoryFrame {
    Snapshot capturedSnapshot;
    float duration;

    PseudocodeSection pseudoFrame;
    std::vector <int> pseudoActiveLines;
};
#endif