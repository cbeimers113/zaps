#ifndef GUI_H
#define GUI_H

#include <stdint.h>

#include "button.h"
#include "input.h"
#include "menu.h"
#include "simulation.h"

void gui_init(Simulation *sim);
void gui_update(Input input);
void gui_render(uint32_t *buffer);
void gui_free();

#endif