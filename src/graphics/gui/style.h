#ifndef STYLE_H
#define STYLE_H

#include <stdint.h>

typedef struct
{
    int textSize;

    uint32_t borderColor;
    uint32_t backgroundColor;
    uint32_t textColor;

    uint32_t borderColorHover;
    uint32_t backgroundColorHover;
    uint32_t textColorHover;
} Style;

Style new_style(int textSize, uint32_t color);

#endif