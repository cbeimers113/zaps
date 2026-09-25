#include <stdio.h>

#include "text.h"

// Decode one UTF-8 code point and advance *s past it; malformed input yields '?'
static uint32_t utf8_next(const unsigned char **s)
{
    const unsigned char *p = *s;
    int extra = *p >= 0xf0 ? 3 : *p >= 0xe0 ? 2 : *p >= 0xc0 ? 1 : 0;
    uint32_t cp = extra == 0 ? *p : *p & (0x3f >> extra);
    p++;

    for (; extra > 0; extra--, p++)
    {
        if ((*p & 0xc0) != 0x80)
        {
            *s = p;
            return '?';
        }
        cp = (cp << 6) | (*p & 0x3f);
    }

    *s = p;
    return cp;
}

// Render text at a given position and size to the pixel buffer
void text_render(int x, int y, const char *text, int size, uint32_t fgColor, uint32_t bgColor, uint32_t *buffer)
{
    if (x < 0 || x >= WIDTH * CELL || y < 0 || y >= HEIGHT * CELL)
        return;

    int w = (int)((float)size * 0.8f);
    const unsigned char *p = (const unsigned char *)text;
    for (int i = 0; *p; i++)
        glyph_render(x + i * w, y, utf8_next(&p), size, fgColor, bgColor, buffer);
}