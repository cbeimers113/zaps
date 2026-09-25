#include "simulation.h"

#include <stdio.h>

static Simulation sim;

// Initialize the simulation state
Simulation *sim_init()
{
    sim.connectionsCapacity = 10;
    sim.connections = (Connection **)malloc(sim.connectionsCapacity * sizeof(Connection *));
    return &sim;
}

// Update the simulation state
void sim_update(Input input)
{
    // Find hovered cell
    int x = input.mouseX / CELL;
    int y = input.mouseY / CELL;

        Cell *cell = in_bounds(x, y) ? sim.cells[x + y * WIDTH] : NULL;
    sim.activeX = x;
    sim.activeY = y;

    // Place a gate or activate switches & buttons on left click
    if (!input.holdShift && input.read_mouse() == MB_LEFT)
    {

        if (!cell)
            sim.cells[x + y * WIDTH] = new_cell(sim.selectedGate);
        else if (gates_equal(cell->gate, BTN))
            cell->state = true;
        else if (gates_equal(cell->gate, SWCH))
            cell->state = !cell->state;
    }

    // Drag-n-drop to connect gates, from output to input
    // Check if we need to place the source coordinates
    else if (input.holdShift &&
             input.mouse_button() == MB_LEFT &&
             (sim.srcX == -1 || sim.srcY == -1) &&
             cell)
    {
        sim.srcX = x;
        sim.srcY = y;
    }

    // Clear connection source x and y when release mouse,
    // create connection if valid target
    else if (!input.mouse_button())
    {
        Cell *src = sim.cells[sim.srcX + sim.srcY * WIDTH];
        bool connected = false;

        if (src && cell && sim.pendingConnection && cell_add_input(cell, src))
        {
            // Success, add the connection but grow the connections array if needed
            if (sim.numConnections == sim.connectionsCapacity)
            {
                // Allocate new space
                sim.connectionsCapacity *= 2;
                Connection **connections = (Connection **)malloc(sim.connectionsCapacity * sizeof(Connection *));

                // Copy old elements
                for (int i = 0; i < sim.numConnections; i++)
                    connections[i] = sim.connections[i];

                // Release old space and load new connections
                free(sim.connections);
                sim.connections = connections;
            }

            // Commit pending connection
            sim.pendingConnection->source = src;
            sim.pendingConnection->target = cell;
            sim.connections[sim.numConnections++] = sim.pendingConnection;
            sim.pendingConnection = NULL;
            connected = true;
        }

        if (!connected && sim.pendingConnection)
        {
            // Pending connection not committed, clear it
            free(sim.pendingConnection);
            sim.pendingConnection = NULL;
        }

        sim.srcX = -1;
        sim.srcY = -1;
    }

    // Update all the cells
    for (int i = 0; sim.cells[i]; i++)
        cell_update(sim.cells[i]);
    for (int i = 0; sim.cells[i]; i++)
        cell_flip(sim.cells[i]);
}

// Render the simulation
void sim_render(uint32_t *buffer)
{
    // Draw cell grid
    int offs = CELL / 2 - GLYPH_SIZE_16;
    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            if (in_bounds(x, y))
            {
                Cell *cell = sim.cells[x + y * WIDTH];
                if (cell)
                {
                    // Glow if active
                    uint32_t bgColor = cell->state ? 0xbdb7a0aa : 0;
                    text_render(x * CELL + offs, y * CELL + offs, cell->gate.symbol, GLYPH_SIZE_32, cell->gate.color, bgColor, buffer);
                }

                draw_rectangle(buffer, x * CELL, y * CELL, CELL, CELL, 0x333333ff);
            }

    // Highlight active cell
    if (in_bounds(sim.activeX, sim.activeY))
        draw_rectangle(buffer, sim.activeX * CELL, sim.activeY * CELL, CELL, CELL, 0xaaaaaaff);

    // Compute connection line for pending connection
    if (sim.srcX != -1 && sim.srcY != -1)
    {
        // Find best point on edges of each cell to connect the line to
        uint8_t srcPoint = edge_point(atan2(sim.activeY - sim.srcY, sim.activeX - sim.srcX));
        uint8_t tgtPoint = edge_point(atan2(sim.srcY - sim.activeY, sim.srcX - sim.activeX));

        // Find local start and stop coords on cell edges
        int srcXOffs = (int)(((srcPoint & 0b1000) >> 3) * ((srcPoint & 0b0010) ? -1 : 1));
        int srcYOffs = (int)(((srcPoint & 0b0100) >> 2) * ((srcPoint & 0b0001) ? -1 : 1));
        int tgtXOffs = (int)(((tgtPoint & 0b1000) >> 3) * ((tgtPoint & 0b0010) ? -1 : 1));
        int tgtYOffs = (int)(((tgtPoint & 0b0100) >> 2) * ((tgtPoint & 0b0001) ? -1 : 1));

        int srcX = sim.srcX * CELL + CELL / 2 + srcXOffs * CELL / 2;
        int srcY = sim.srcY * CELL + CELL / 2 + srcYOffs * CELL / 2;
        int tgtX = sim.activeX * CELL + CELL / 2 + tgtXOffs * CELL / 2;
        int tgtY = sim.activeY * CELL + CELL / 2 + tgtYOffs * CELL / 2;

        // Find left and right arrow head lines' stop coords
        float d = 15.0f;
        float theta = 3.0f * 3.14159f / 4.0f;
        float base = atan2f(sim.activeY - sim.srcY, sim.activeX - sim.srcX);
        int arrowLeftX = tgtX + (int)(d * cosf(base - theta));
        int arrowLeftY = tgtY + (int)(d * sinf(base - theta));
        int arrowRightX = tgtX + (int)(d * cosf(base + theta));
        int arrowRightY = tgtY + (int)(d * sinf(base + theta));

        // Update the pending connection
        if (!sim.pendingConnection)
            sim.pendingConnection = malloc(sizeof(Connection));
        sim.pendingConnection->source = NULL;
        sim.pendingConnection->target = NULL;
        sim.pendingConnection->sx = srcX;
        sim.pendingConnection->sy = srcY;
        sim.pendingConnection->tx = tgtX;
        sim.pendingConnection->ty = tgtY;
        sim.pendingConnection->alx = arrowLeftX;
        sim.pendingConnection->aly = arrowLeftY;
        sim.pendingConnection->arx = arrowRightX;
        sim.pendingConnection->ary = arrowRightY;
    }

    // Draw connections
    for (int i = 0; i < sim.numConnections; i++)
    {
        uint32_t color = (sim.pendingConnection &&
                          sim.pendingConnection->source &&
                          sim.pendingConnection->source->state)
                             ? 0xfff5c9ff
                             : 0x3ebd2dff;
        connection_render(sim.connections[i], color, buffer);
    }
    if (sim.pendingConnection)
        connection_render(sim.pendingConnection, 0x81d477ff, buffer);
}

// Free simulation memory resources
void sim_free()
{
    for (int i = 0; sim.cells[i]; i++)
        cell_free(sim.cells[i]);
    for (int i = 0; i < sim.numConnections; i++)
        free(sim.connections[i]);
}

// Check if a given coordinate is in bounds of the cell grid
bool in_bounds(int x, int y)
{
    return x > 0 && x < WIDTH - 1 && y > 0 && y < HEIGHT - 2;
}

// Round the given angle to the closest angle of a set corresponding
// to the 8 edge points for inter-cell connecting lines. Packs the resulting
// x and y offsets into 4 bits: abs(xOffs), abs(yOffs), xOffs < 0, yOffs < 0
uint8_t edge_point(double theta)
{
    double pi = 3.141592;
    double angles[8] = {
        0,
        -pi / 4,
        -pi / 2,
        -3 * pi / 4,
        pi,
        3 * pi / 4,
        pi / 2,
        pi / 4,
    };

    uint8_t points[8] = {
        0b1000,
        0b1101,
        0b0101,
        0b1111,
        0b1010,
        0b1110,
        0b0100,
        0b1100,
    };

    double closest = angles[0];
    uint8_t point = points[0];

    for (int i = 1; i < 8; i++)
        if (fabs(theta - angles[i]) < fabs(theta - closest))
        {
            closest = angles[i];
            point = points[i];
        }

    return point;
}
