#ifndef BUTTON_H
#define BUTTON_H

#include <stdbool.h>
#include <stdint.h>

#include "common.h"
#include "color.h"
#include "draw.h"
#include "input.h"
#include "simulation.h"
#include "style.h"
#include "text.h"

typedef struct
{
    Simulation *sim;
    const char *text;

    int x;
    int y;
    int w;
    int h;

    bool hover;
    bool pressed;
    bool pressedLast;

    Style style;

    void (*callback)();
} Button;

Button *new_button(Simulation *sim, int x, int y, const char *text, Style style, void (*callback)());
void button_update(Button *button, Input input);
void button_render(Button *button, uint32_t *buffer);

#endif