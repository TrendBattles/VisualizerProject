#ifndef HIGHLIGHTTHEME_HPP
#define HIGHLIGHTTHEME_HPP

#include <Common.hpp>

struct ChangeInfo {
    Color backgroundColor;
    Color outlineColor;
    Color textColor;
};
using ChangeMap = std::map <std::string, ChangeInfo>;

namespace Highlight {
    // GREEN: New node inserted
    inline const ChangeInfo INSERTED = {
        GetColor(0x23D96CFF), // Vivid Green
        GetColor(0x00732CFF), // Deep Forest Outline
        WHITE
    };

    // RED: Node deletion
    inline const ChangeInfo REMOVED = {
        GetColor(0xBA2D3BFF), // Crimson Red
        GetColor(0x7A1C25FF), // Dark Blood Outline
        WHITE
    };

    // CYAN: Specifically for nodes involved in a rotation.
    // Distinct from "Modified" Blue.
    inline const ChangeInfo ROTATION = {
        GetColor(0x00ACC1FF), // Vivid Teal/Cyan
        GetColor(0x006064FF), // Deep Cyan Outline
        WHITE
    };

    // GOLD: Node Found
    inline const ChangeInfo FOUND = {
        GetColor(0xFFA000FF), // Amber/Gold
        GetColor(0xFFD54FFF), // Lighter Gold Outline (Glow effect)
        BLACK                 // Black text is much sharper on Amber
    };

    // DIMMED: For nodes that were visited but are no longer active.
    inline const ChangeInfo INACTIVE = {
        GetColor(0x1B1D2AFF), // Darker than NORMAL background
        GetColor(0x3D405BFF), // Muted Slate Outline
        GetColor(0x6B708BFF)  // Dimmed Grey Text
    };

    // PURPLE: The active nodes
    inline const ChangeInfo ACTIVE = {
        GetColor(0x7B5EFEFF), // Electric Purple
        WHITE,                // White Outline makes it "pop" as the primary focus
        WHITE
    };

    // BLUE: Modified nodes
    inline const ChangeInfo MODIFIED = {
        GetColor(0x42A5F5FF), // Material Blue
        GetColor(0x1565C0FF), // Deep Blue Outline
        WHITE
    };

    // DARK NAVY: Normal nodes
    inline const ChangeInfo NORMAL = {
        GetColor(0x2B2D42FF), // Deep Navy/Charcoal
        GetColor(0x8D99AEFF), // Muted Blue-Grey outline
        GetColor(0xEDF2F4FF)  // Off-white text (Soft on the eyes)
    };

    inline const ChangeInfo PINNED = {
        GetColor(0x2B2D42FF), // Deep Navy/Charcoal
        RED, // Muted Blue-Grey outline
        GetColor(0xEDF2F4FF)  // Off-white text (Soft on the eyes)
    };
}
#endif