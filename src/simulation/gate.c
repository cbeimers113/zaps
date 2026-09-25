#include "gate.h"

bool gates_equal(Gate a, Gate b)
{
    return !strcmp(a.name, b.name);
}

bool compute_AND(Inputs inputs)
{
    for (int i = 0; i < inputs.n; i++)
        if (!inputs.vals[i])
            return false;

    return inputs.n > 0;
}

bool compute_OR(Inputs inputs)
{
    for (int i = 0; i < inputs.n; i++)
        if (inputs.vals[i])
            return true;

    return false;
}

bool compute_XOR(Inputs inputs)
{
    bool val = false;
    for (int i = 0; i < inputs.n; i++)
        if (inputs.vals[i])
        {
            if (val)
                return false;

            val = true;
        }

    return val;
}

bool compute_NOT(Inputs inputs)
{
    return inputs.n == 0 || !inputs.vals[0];
}

bool compute_NAND(Inputs inputs)
{
    return !compute_AND(inputs);
}

bool compute_NOR(Inputs inputs)
{
    return !compute_OR(inputs);
}

bool compute_XNOR(Inputs inputs)
{
    return !compute_XOR(inputs);
}

bool compute_identity(Inputs inputs)
{
    return !compute_NOT(inputs);
}