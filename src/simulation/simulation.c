#include "simulation.h"

#include <stdio.h>

static Simulation state;

// Initialize the simulation state
void sim_init(void) {
    for (int i = 0; i < WIDTH * HEIGHT; i++) state.cells[i] = 0;
}

// Update the simulation state based on input
void sim_update(Input input) {
    printf("Simulation update\n");
}

// Get the current simulation state
const Simulation *sim_get(void) {
    return &state;
}
