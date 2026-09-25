#include "glyph.h"

// Render a glyph at a given position and size to the pixel buffer
void glyph_render(int x, int y, uint32_t codepoint, int size, uint32_t fgColor, uint32_t bgColor, uint32_t *buffer)
{
    if (!buffer || (size != GLYPH_SIZE_16 && size != GLYPH_SIZE_32))
        return;

    // Tables are sparse, indexed by Unicode code point; missing entries are NULL
    uint32_t **table = size == GLYPH_SIZE_16 ? GLYPHS_16 : GLYPHS_32;
    if (codepoint >= sizeof(GLYPHS_16) / sizeof(GLYPHS_16[0]) || !table[codepoint])
        return;

    uint32_t *glyph = table[codepoint];
    for (int yOffs = 0; yOffs < size; yOffs++)
    {
        int yy = y + yOffs;
        if (yy < 0 || yy >= HEIGHT * CELL)
            continue;

        for (int xOffs = 0; xOffs < size; xOffs++)
        {
            int xx = x + xOffs;
            if (xx < 0 || xx >= WIDTH * CELL)
                continue;

            // Draw the glyph pixels;
            // if there is a pixel to render, color it with fgColor
            // if there is no pixel, only render bgColor if bgColor is not 0
            uint32_t pixel = glyph[xOffs + yOffs * size];
            pixel = (pixel >> 8) ? color_pixel(pixel, fgColor) : bgColor ? bgColor
                                                                         : 0;

            // TODO: alpha channel
            if (pixel)
                buffer[xx + yy * WIDTH * CELL] = pixel;
        }
    }
}
