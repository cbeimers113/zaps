#include "input.h"

#include <stdio.h>

static Uint8 current[SDL_NUM_SCANCODES];
static Uint8 previous[SDL_NUM_SCANCODES];

static int mouseButton;
static int lastMouseButton;

void input_update()
{
    memcpy(previous, current, sizeof(current));
    memcpy(current, SDL_GetKeyboardState(NULL), sizeof(current));
}

static bool pressed(SDL_Scancode code)
{
    return current[code] && !previous[code];
}

Input input_get(int mouseX, int mouseY, int mouseB)
{
    lastMouseButton = mouseButton;
    mouseButton = mouseB;

    return (Input){
        .mouseX = mouseX,
        .mouseY = mouseY,

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

        .holdShift = current[SDL_SCANCODE_LSHIFT],

        .mouse_button = mouse_button,
        .read_mouse = read_mouse,
    };
}

// Returns the current state of the mouse button
int mouse_button()
{
    return mouseButton;
}

// Returns the current state of the mouse button on edge events
int read_mouse()
{
    return mouseButton != lastMouseButton ? mouseButton : 0;
}