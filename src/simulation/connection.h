#ifndef CONNECTION_H
#define CONNECTION_H

#include <stdint.h>

#include "cell.h"
#include "draw.h"

typedef struct
{
    Cell *source;
    Cell *target;

    int sx;  // source x
    int sy;  // source y
    int tx;  // target x
    int ty;  // target y
    int alx; // arrow left x
    int aly; // arrow left y
    int arx; // arrow right x
    int ary; // arrow right y
} Connection;

void connection_render(Connection *connection, uint32_t color, uint32_t *buffer);

#endif