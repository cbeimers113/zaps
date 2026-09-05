#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdint.h>

#include "common.h"
#include "input.h"

typedef struct
{
    uint8_t cells[WIDTH * HEIGHT];
} Simulation;

void sim_init(void);
void sim_update(Input input);
const Simulation *sim_get(void);

#endif
