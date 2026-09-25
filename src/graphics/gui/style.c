#include "style.h"

Style new_style(int textSize, uint32_t color)
{
    int r = color >> 24;
    int g = (color >> 16) & 0xff;
    int b = (color >> 8) & 0xff;

    int lr = (float)r * 1.15f;
    int lg = (float)g * 1.15f;
    int lb = (float)b * 1.15f;

    lr = lr > 0xff ? 0xff : lr;
    lg = lg > 0xff ? 0xff : lg;
    lb = lb > 0xff ? 0xff : lb;

    uint32_t light = (lr << 24) | (lg << 16) | (lb << 8) | 0xff;

    Style style = {
        .textSize = textSize,

        .borderColor = color,
        .backgroundColor = 0x000000ff,
        .textColor = color,

        .borderColorHover = light,
        .backgroundColorHover = 0x000000ff,
        .textColorHover = light,
    };

    return style;
}
