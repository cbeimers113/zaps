#include "button.h"

// Create a new button
Button *new_button(Simulation *sim, int x, int y, const char *text, Style style, void (*callback)())
{
    Button *button = malloc(sizeof(Button));

    button->x = x;
    button->y = y;
    button->text = text;
    button->style = style;

    // Measure button (text plus padding)
    int l = 0;
    for (int i = 0; text[i] != '\0'; i++)
        l++;

    int padding = style.textSize / 2;
    button->w = (int)((float)style.textSize * 0.8f) * l + 2 * padding;
    button->h = style.textSize + 2 * padding;

    button->callback = callback;
    button->sim = sim;

    return button;
}

// Update a button
void button_update(Button *button, Input input)
{
    button->hover = false;

    // Check if in bounds of button
    if (input.mouseX < button->x || input.mouseX >= button->x + button->w)
        return;
    if (input.mouseY < button->y || input.mouseY >= button->y + button->h)
        return;

    // Set hover flag
    button->hover = true;

    // On click
    if (input.read_mouse() == MB_LEFT && button->callback)
        button->callback(button);
}

// Render a button at a given position to the pixel buffer
void button_render(Button *button, uint32_t *buffer)
{
    // Background (if active)
    if (button->text == button->sim->selectedGate.name)
        fill_rectangle(buffer, button->x, button->y, button->w, button->h, button->style.borderColor);

    // Bounding box
    uint32_t borderColor = button->hover ? button->style.borderColorHover : button->style.borderColor;
    draw_rectangle(buffer, button->x, button->y, button->w, button->h, borderColor);

    // Button text
    uint32_t textColor = button->hover ? button->style.textColorHover : button->style.textColor;
    int padding = button->style.textSize / 2;
    text_render(button->x + padding, button->y + padding, button->text, button->style.textSize, textColor, 0x11111111, buffer);
}