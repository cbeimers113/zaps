#include "input.h"

#include <string.h>

static Uint8 current[SDL_NUM_SCANCODES];
static Uint8 previous[SDL_NUM_SCANCODES];

void input_update(void)
{
    memcpy(previous, current, sizeof(current));
    memcpy(current, SDL_GetKeyboardState(NULL), sizeof(current));
}

static bool pressed(SDL_Scancode code)
{
    return current[code] && !previous[code];
}

Input input_get(int mouse_x, int mouse_y, int mouse_button)
{
    return (Input){
        .mouse_x = mouse_x,
        .mouse_y = mouse_y,
        .mouse_button = mouse_button,

        .keys = current,

        .enter = pressed(SDL_SCANCODE_RETURN),
        .spacebar = pressed(SDL_SCANCODE_SPACE),
        .ctrl = pressed(SDL_SCANCODE_LCTRL),
        .shift = pressed(SDL_SCANCODE_LSHIFT),
        .alt = pressed(SDL_SCANCODE_LALT),
        .tab = pressed(SDL_SCANCODE_TAB),

        .up = pressed(SDL_SCANCODE_UP),
        .down = pressed(SDL_SCANCODE_DOWN),
        .left = pressed(SDL_SCANCODE_LEFT),
        .right = pressed(SDL_SCANCODE_RIGHT),
    };
}
