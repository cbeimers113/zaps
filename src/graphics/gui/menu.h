#ifndef MENU_H
#define MENU_H

#include "button.h"
#include "gate.h"
#include "glyph.h"
#include "input.h"
#include "simulation.h"

static Button *button_AND;
static Button *button_OR;
static Button *button_XOR;
static Button *button_NOT;
static Button *button_NAND;
static Button *button_NOR;
static Button *button_XNOR;
static Button *button_BUF;
static Button *button_BTN;
static Button *button_SWCH;

Button *new_menu_button(Simulation *sim, int *xOffs, Gate gate);
void menu_init(Simulation *sim);
void menu_update(Input input);
void menu_render(uint32_t *buffer);
void menu_free();
void menu_button_callback(Button *button);

#endif