#include "text.h"

// Render text at a given position and size to the pixel buffer
void text_render(int x, int y, const char *text, int size, uint32_t *buffer)
{
    if (x < 0 || x >= WIDTH * CELL || y < 0 || y >= HEIGHT * CELL)
        return;

    for (int i = 0; text[i] != '\0'; i++)
        glyph_render(x + i * size, y, text[i], size, buffer);
}