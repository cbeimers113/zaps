#include "color.h"

// Applies the given color to a base pixel value, which
// is assumed to be an RGBA value with (near-)equal values
// for the R, G, and B components
uint32_t color_pixel(uint32_t pixel, uint32_t color)
{
    uint32_t c = 0;
    for (int i = 1; i <= 3; i++)
    {
        int bits = 8*i;
        int bitmask = 0xff << bits;
        int comp_pixel = (pixel & bitmask) >> bits;
        int comp_color = (color & bitmask) >> bits;

        float scale = (float)comp_pixel / 255.0f;
        c |= (int)((float)comp_color * scale) << bits;
    }    

    return c;
}