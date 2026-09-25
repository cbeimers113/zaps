#ifndef GATE_H
#define GATE_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct
{
    bool *vals;
    int n;
} Inputs;

typedef struct
{
    uint32_t color;
    const char *name;
    const char *symbol;
    int maxInputs;
    bool (*compute)(Inputs inputs);
} Gate;

bool gates_equal(Gate a, Gate b);
bool compute_AND(Inputs inputs);
bool compute_OR(Inputs inputs);
bool compute_XOR(Inputs inputs);
bool compute_NOT(Inputs inputs);
bool compute_NAND(Inputs inputs);
bool compute_NOR(Inputs inputs);
bool compute_XNOR(Inputs inputs);
bool compute_identity(Inputs inputs);

static Gate AND = {
    .color = 0xf2c368ff,
    .name = "AND",
    .symbol = "∧",
    .maxInputs = -1,
    .compute = compute_AND,
};

static Gate OR = {
    .color = 0x6ad98bff,
    .name = "OR",
    .symbol = "∨",
    .maxInputs = -1,
    .compute = compute_OR,
};

static Gate XOR = {
    .color = 0x68c4ffff,
    .name = "XOR",
    .symbol = "⊕",
    .maxInputs = -1,
    .compute = compute_XOR,
};

static Gate NOT = {
    .color = 0xf4a1a1ff,
    .name = "NOT",
    .symbol = "¬",
    .maxInputs = 1,
    .compute = compute_NOT,
};

static Gate NAND = {
    .color = 0xd8663cff,
    .name = "NAND",
    .symbol = "↑",
    .maxInputs = -1,
    .compute = compute_NAND,
};

static Gate NOR = {
    .color = 0x5ec8beff,
    .name = "NOR",
    .symbol = "↓",
    .maxInputs = -1,
    .compute = compute_NOR,
};

static Gate XNOR = {
    .color = 0x8d8af0ff,
    .name = "XNOR",
    .symbol = "⊙",
    .maxInputs = -1,
    .compute = compute_XNOR,
};

static Gate BUF = {
    .color = 0xdfe7eeff,
    .name = "BUF",
    .symbol = "≡",
    .maxInputs = 1,
    .compute = compute_identity,
};

static Gate BTN = {
    .color = 0xd8f57aff,
    .name = "BTN",
    .symbol = "⚡",
    .maxInputs = 0,
    .compute = compute_identity,
};

static Gate SWCH = {
    .color = 0x7d34ebff,
    .name = "SWCH",
    .symbol = "⏻",
    .maxInputs = 0,
    .compute = compute_identity,
};

#endif