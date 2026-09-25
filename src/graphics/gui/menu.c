#include "menu.h"

void menu_init(Simulation *sim)
{
    int spacing = 0;
    button_AND = new_menu_button(sim, &spacing, AND);
    button_OR = new_menu_button(sim, &spacing, OR);
    button_XOR = new_menu_button(sim, &spacing, XOR);
    button_NOT = new_menu_button(sim, &spacing, NOT);
    button_NAND = new_menu_button(sim, &spacing, NAND);
    button_NOR = new_menu_button(sim, &spacing, NOR);
    button_XNOR = new_menu_button(sim, &spacing, XNOR);
    button_BUF = new_menu_button(sim, &spacing, BUF);
    button_BTN = new_menu_button(sim, &spacing, BTN);
    button_SWCH = new_menu_button(sim, &spacing, SWCH);

    // Pre-select AND as the active gate type
    button_AND->callback(button_AND);
}

void menu_update(Input input)
{
    button_update(button_AND, input);
    button_update(button_OR, input);
    button_update(button_XOR, input);
    button_update(button_NOT, input);
    button_update(button_NAND, input);
    button_update(button_NOR, input);
    button_update(button_XNOR, input);
    button_update(button_BUF, input);
    button_update(button_BTN, input);
    button_update(button_SWCH, input);
}

void menu_render(uint32_t *buffer)
{
    button_render(button_AND, buffer);
    button_render(button_OR, buffer);
    button_render(button_XOR, buffer);
    button_render(button_NOT, buffer);
    button_render(button_NAND, buffer);
    button_render(button_NOR, buffer);
    button_render(button_XNOR, buffer);
    button_render(button_BUF, buffer);
    button_render(button_BTN, buffer);
    button_render(button_SWCH, buffer);
}

void menu_free()
{
    free(button_AND);
    free(button_OR);
    free(button_XOR);
    free(button_NOT);
    free(button_NAND);
    free(button_NOR);
    free(button_XNOR);
    free(button_BUF);
    free(button_BTN);
    free(button_SWCH);
}

void menu_button_callback(Button *button)
{
    button->sim->selectedGate =
        button == button_AND ? AND : button == button_OR ? OR
                                      : button == button_XOR  ? XOR
                                      : button == button_NOT  ? NOT
                                      : button == button_NAND ? NAND
                                      : button == button_NOR  ? NOR
                                      : button == button_XNOR ? XNOR
                                      : button == button_BUF  ? BUF
                                      : button == button_BTN  ? BTN
                                      : button == button_SWCH ? SWCH
                                                              : button->sim->selectedGate;
}

Button *new_menu_button(Simulation *sim, int *xOffs, Gate gate)
{
    int y = HEIGHT * CELL - 48;
    int spacing = 16;

    Button *button = new_button(sim, *xOffs + spacing, y, gate.name, new_style(GLYPH_SIZE_16, gate.color), menu_button_callback);
    *xOffs = button->x + button->w;

    return button;
}