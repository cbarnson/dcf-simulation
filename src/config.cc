#include "config.h"

const double PROP_DELAY =
    200 / 3e8;  // 200m diameter of network, speed of light
const double TX_TIME = 8000 * 8 / 54e6;  // 8000 bytes, data rate of 54 mbps
const double SLOT_TIME = 2 * PROP_DELAY;
const double SIFS = 10e-6;  // given
const double DIFS = SIFS + 2 * SLOT_TIME;

const double FRAME_ARRIVAL_RATE = 840.7063;
const double SIMULATION_TIME_MAX = 1.0;

const int FRAME_SIZE_BITS = 8000 * 8;
const int CW_MIN_N = 3;  // 2^3 - 1 = 7; i.e. (0, 7)
const int CW_MAX_N = 7;  // 2^7 - 1 = 127; i.e. (0, 127)

// fraction of total frames generated on average for simulation time
// This value is set from main.cc
double PHI = 1.0;
