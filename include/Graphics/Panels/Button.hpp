#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <Graphics/RendererCommon.hpp>

struct Button {
    ShapeState background;
    Text label;

    bool contains(Vector2 position) const;
};

using TextBox = Button;
#endif