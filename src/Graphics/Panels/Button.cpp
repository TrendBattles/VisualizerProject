#include <Graphics/Panels/Button.hpp>
#include <Graphics/Helper.hpp>

bool Button::contains(Vector2 point) const {
    return Helper::IsPointCollidedWithShape(point, background);
}