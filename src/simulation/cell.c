#include "cell.h"

// Create a new cell
Cell *new_cell(Gate gate)
{
    Cell *cell = malloc(sizeof(Cell));
    cell->gate = gate;
    cell->state = false;
    cell->stateBuf = false;
    cell->numInputs = 0;
    cell->inputCapacity = 3;
    cell->inputs = (Cell **)malloc(cell->inputCapacity * sizeof(Cell *));

    return cell;
}

// Adds an input to the cell if its gate has space
bool cell_add_input(Cell *cell, Cell *input)
{
    if (cell->gate.maxInputs != -1 && cell->numInputs == cell->gate.maxInputs)
        return false;

    // Check if we need to allocate more inputs to the heap
    if (cell->numInputs == cell->inputCapacity)
    {
        cell->inputCapacity *= 2;
        Cell **inputs = (Cell **)malloc(cell->inputCapacity * sizeof(Cell *));

        // Move old array into new
        for (int i = 0; i < cell->numInputs; i++)
            inputs[i] = cell->inputs[i];

        // Free the old array and take the new one
        free(cell->inputs);
        cell->inputs = inputs;
    }

    cell->inputs[cell->numInputs++] = input;
    return true;
}

// Computes new state from inputs, writes to stateBuf instead of in-place
void cell_update(Cell *cell)
{
    if (!cell)
        return;

    // Gather inputs as gate abstractions,
    // BTN and SWCH are special cases
    Inputs inputs;
    if (gates_equal(cell->gate, BTN))
    {
        cell->stateBuf = 0;
        return;
    }
    else if (gates_equal(cell->gate, SWCH))
    {
        inputs = (Inputs){
            .n = 1,
            .vals = (bool *)malloc(1 * sizeof(bool)),
        };
        inputs.vals[0] = cell->state;
    }
    else
    {
        inputs = (Inputs){
            .n = cell->numInputs,
            .vals = (bool *)malloc(cell->numInputs * sizeof(bool)),
        };
        for (int i = 0; i < inputs.n; i++)
            inputs.vals[i] = cell->inputs[i]->state;
    }

    // Compute the new state
    cell->stateBuf = cell->gate.compute(inputs);
}

// Flips the stateBuf into the active state
void cell_flip(Cell *cell)
{
    if (!cell)
        return;
    cell->state = cell->stateBuf;
}

// Free Cell resources from the heap
void cell_free(Cell *cell)
{
    free(cell->inputs);
    free(cell);
}