#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

#include <SDL2/SDL.h>

#define MB_LEFT 1
#define MB_RIGHT 4

typedef struct {
    int mouseX;
    int mouseY;

    const Uint8 *keys;

    bool enter, spacebar, ctrl, shift, alt, tab;
    bool up, down, left, right;
    bool holdShift;

    int (*mouse_button)();
    int (*read_mouse)();
} Input;

// Snapshot the keyboard state; call once per real frame before input_get.
void input_update();

// Build an Input for this frame. Named key fields are debounced: true only
// on the frame a key transitions from up to down.
Input input_get(int mouseX, int mouseY, int mouseB);
int mouse_button();
int read_mouse();

#endif
