#include "common.h"
#include "draw.h"

// Draw a pixel to the buffer
void set_pixel(uint32_t *buffer, int x, int y, uint32_t color)
{
    if (x < 0 || x >= WIDTH * CELL || y < 0 || y >= HEIGHT * CELL)
        return;

    buffer[x + y * WIDTH * CELL] = color;
}

// Draw a pixel to the buffer with decomposed RGBA parameters
void set_pixel_rgba(uint32_t *buffer, int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    if (x < 0 || x >= WIDTH * CELL || y < 0 || y >= HEIGHT * CELL)
        return;

    uint32_t color = (r << 24) | (g << 16) | (b << 8) | a;
    buffer[x + y * WIDTH * CELL] = color;
}

// Draw a line between start and end coords using Bresenham's algorithm
void draw_line(uint32_t *buffer, int x1, int y1, int x2, int y2, uint32_t color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    dx = dx < 0 ? -dx : dx;
    dy = dy < 0 ? -dy : dy;

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1)
    {
        set_pixel(buffer, x1, y1, color);
        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

// Draw a rectangle to the buffer
void draw_rectangle(uint32_t *buffer, int x, int y, int w, int h, uint32_t color)
{
    for (int yOffs = 0; yOffs < h; yOffs++)
    {
        int yy = y + yOffs;
        if (yy < 0 || yy >= HEIGHT * CELL)
            continue;

        // Draw left and right edges
        set_pixel(buffer, x, yy, color);
        set_pixel(buffer, x + w, yy, color);

        // Draw top and bottom edges
        if (yOffs > 0 && yOffs < h - 1)
            continue;

        for (int xOffs = 0; xOffs < w; xOffs++)
        {
            int xx = x + xOffs;
            if (xx < 0 || xx >= WIDTH * CELL)
                continue;

            set_pixel(buffer, xx, yy, color);
        }
    }
}

// Draw a filled rectangle to the buffer
void fill_rectangle(uint32_t *buffer, int x, int y, int w, int h, uint32_t color)
{
    for (int yOffs = 0; yOffs < h; yOffs++)
    {
        int yy = y + yOffs;
        if (yy < 0 || yy >= HEIGHT * CELL)
            continue;

        for (int xOffs = 0; xOffs < w; xOffs++)
        {
            int xx = x + xOffs;
            if (xx < 0 || xx >= WIDTH * CELL)
                continue;

            set_pixel(buffer, xx, yy, color);
        }
    }
}

// Draw a circle to the buffer, centred at (x, y)
void draw_circle(uint32_t *buffer, int x, int y, int r, uint32_t color)
{
    for (int yOffs = -r; yOffs <= r; yOffs++)
    {
        int yy = y + yOffs;
        if (yy < 0 || yy >= HEIGHT * CELL)
            continue;

        for (int xOffs = -r; xOffs <= r; xOffs++)
        {
            int xx = x + xOffs;
            if (xx < 0 || xx >= WIDTH * CELL)
                continue;

            float dx = (float)xOffs;
            float dy = (float)yOffs;
            if ((int)sqrtf(dx * dx + dy * dy) != r)
                continue;

            set_pixel(buffer, xx, yy, color);
        }
    }
}

// Draw a filled circle to the buffer, centred at (x, y)
void fill_circle(uint32_t *buffer, int x, int y, int r, uint32_t color)
{
    for (int yOffs = -r; yOffs <= r; yOffs++)
    {
        int yy = y + yOffs;
        if (yy < 0 || yy >= HEIGHT * CELL)
            continue;

        for (int xOffs = -r; xOffs <= r; xOffs++)
        {
            int xx = x + xOffs;
            if (xx < 0 || xx >= WIDTH * CELL)
                continue;

            float dx = (float)xOffs;
            float dy = (float)yOffs;
            if ((int)sqrtf(dx * dx + dy * dy) > r)
                continue;

            set_pixel(buffer, xx, yy, color);
        }
    }
}