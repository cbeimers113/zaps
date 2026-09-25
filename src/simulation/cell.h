#ifndef CELL_H
#define CELL_H

#include <stdbool.h>
#include <stdlib.h>

#include "gate.h"

typedef struct Cell Cell;
struct Cell
{
    Gate gate;
    bool state;
    bool stateBuf;

    Cell **inputs;
    int numInputs;
    int inputCapacity;
};

Cell *new_cell(Gate gate);
bool cell_add_input(Cell *cell, Cell *input);
void cell_update(Cell *cell);
void cell_flip(Cell *cell);
void cell_free(Cell *cell);

#endif