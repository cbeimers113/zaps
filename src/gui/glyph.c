#include "glyph.h"

// Render a glyph at a given position and size to the pixel buffer
void glyph_render(int x, int y, char index, int size, uint32_t *buffer)
{
    if (!buffer || (size != GLYPH_SIZE_16 && size != GLYPH_SIZE_32))
        return;

    uint32_t *glyph = size == GLYPH_SIZE_16 ? GLYPHS_16[index] : GLYPHS_32[index];
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

            // Draw the glyph pixels
            uint32_t pixel = glyph[xOffs + yOffs * size];
            buffer[xx + yy * WIDTH * CELL] = pixel;
        }
    }
}
