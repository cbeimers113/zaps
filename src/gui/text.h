#ifndef TEXT_H
#define TEXT_H

#include <stdint.h>

#include "common.h"
#include "glyph.h"

void text_render(int x, int y, const char *text, int size, uint32_t *buffer);

#endif