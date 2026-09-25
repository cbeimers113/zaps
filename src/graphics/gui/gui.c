#include <stdio.h>

#include "gui.h"

// Initialize the GUI components
void gui_init(Simulation *sim)
{
    menu_init(sim);
}

// Update the GUI components
void gui_update(Input input)
{
    menu_update(input);
}

// Render the GUI components
void gui_render(uint32_t *buffer)
{
    menu_render(buffer);
}

// Free the GUI components
void gui_free()
{
    menu_free();
}