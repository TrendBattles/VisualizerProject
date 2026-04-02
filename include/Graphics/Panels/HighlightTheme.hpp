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
    inline const ChangeInfo INSERTED = {
        GetColor(0x23D96CFF),
        GetColor(0x00732CFF),
        WHITE
    };

    inline const ChangeInfo REMOVED = {
        GetColor(0xE05C6FFF),
        GetColor(0xE05C6FFF),
        WHITE
    };

    inline const ChangeInfo ROTATION = {
        WHITE,
        GetColor(0x00D4FFFF),
        BLACK
    };

    inline const ChangeInfo FOUND = {
        GetColor(0xFFB300FF),
        GetColor(0xFFD54FFF),
        BLACK
    };

    inline const ChangeInfo INACTIVE = {
        WHITE,
        GRAY,
        GRAY
    };

    inline const ChangeInfo ACTIVE = {
        WHITE,
        GetColor(0x7B5EFEFF),
        GetColor(0x7B5EFEFF)
    };
}
#endif