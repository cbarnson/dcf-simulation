#ifndef CONFIG_H
#define CONFIG_H

extern const double PROP_DELAY;
extern const double TX_TIME;
extern const double SLOT_TIME;
extern const double SIFS;
extern const double DIFS;

extern const double FRAME_ARRIVAL_RATE;
extern const double SIMULATION_TIME_MAX;

extern const int FRAME_SIZE_BITS;
extern const int CW_MIN_N; // contention window, (0, 2^CW_MIN_N - 1)
extern const int CW_MAX_N; // contention window, (0, 2^CW_MAX_N - 1)

extern double PHI;



#endif
