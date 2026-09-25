#ifndef SIMULATION_H
#define SIMULATION_H

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

#include "cell.h"
#include "connection.h"
#include "common.h"
#include "draw.h"
#include "gate.h"
#include "input.h"
#include "text.h"

typedef struct
{
    Cell *cells[WIDTH * HEIGHT];

    int activeX;
    int activeY;
    int srcX;
    int srcY;

    Gate selectedGate;
    Connection *pendingConnection;
    Connection **connections;
    int numConnections;
    int connectionsCapacity;

    int ticks;
    int tickLength;  // measured in 1/60th of a second
} Simulation;

Simulation *sim_init();
void sim_update(Input input);
void sim_render(uint32_t *pixelBuffer);
bool in_bounds(int x, int y);
uint8_t edge_point(double theta);

#endif
