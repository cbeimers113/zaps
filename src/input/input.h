#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

#include <SDL2/SDL.h>

#define MB_LEFT 1
#define MB_RIGHT 4

#define KEY_ENTER 0
#define KEY_SPACEBAR 1
#define KEY_CTRL 2
#define KEY_SHIFT 3
#define KEY_ALT 4
#define KEY_TAB 5

#define KEY_UP 6
#define KEY_DOWN 7
#define KEY_LEFT 8
#define KEY_RIGHT 9

typedef struct {
    int mouse_x;
    int mouse_y;
    int mouse_button;

    const Uint8 *keys;

    bool enter, spacebar, ctrl, shift, alt, tab;
    bool up, down, left, right;
} Input;

// Snapshot the keyboard state; call once per real frame before input_get.
void input_update(void);

// Build an Input for this frame. Named key fields are debounced: true only
// on the frame a key transitions from up to down.
Input input_get(int mouse_x, int mouse_y, int mouse_button);

#endif
