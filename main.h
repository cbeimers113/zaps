#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>

#include <SDL2/SDL.h>

#include "glyph.h"
#include "input.h"
#include "simulation.h"

#define BOUNDS_CHECK(x, y)                                                 \
    if ((x) < 0 || (x) >= WIDTH * CELL || (y) < 0 || (y) >= HEIGHT * CELL) \
        return;

void sim_render(SDL_Renderer *renderer, uint32_t *buffer);
void set_pixel(uint32_t *buffer, int x, int y, uint32_t color);
void set_pixel_rgba(uint32_t *buffer, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void draw_rectangle(uint32_t *buffer, int x, int y, int w, int h, uint32_t color);

#endif