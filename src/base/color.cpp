#include "../../inc/base/color.h"

using joszva::graphics::color;

const color color::black(0, 0, 0);
const color color::white(255, 255, 255);

color::color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : r(r), g(g), b(b), a(a)
{
}

color::~color()
{
}