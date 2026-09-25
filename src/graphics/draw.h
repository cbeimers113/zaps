#ifndef DRAW_H
#define DRAW_H

#include <math.h>
#include <stdint.h>

void set_pixel(uint32_t *buffer, int x, int y, uint32_t color);
void set_pixel_rgba(uint32_t *buffer, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void draw_line(uint32_t *buffer, int x1, int y1, int x2, int y2, uint32_t color);
void draw_rectangle(uint32_t *buffer, int x, int y, int w, int h, uint32_t color);
void fill_rectangle(uint32_t *buffer, int x, int y, int w, int h, uint32_t color);
void draw_circle(uint32_t *buffer, int x, int y, int r, uint32_t color);
void fill_circle(uint32_t *buffer, int x, int y, int r, uint32_t color);

#endif