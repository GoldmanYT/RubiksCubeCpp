#include "Data.hpp"

RotationData ROTATION_SIDES[3] = {
    { false, { GREEN, ORANGE, BLUE, RED } },
    { true, { WHITE, ORANGE, YELLOW, RED } },
    { false, { WHITE, BLUE, YELLOW, GREEN } }
};

int ROTATION_SIDE[3][2] = {
    { YELLOW, WHITE },
    { BLUE, GREEN },
    { ORANGE, RED }
};