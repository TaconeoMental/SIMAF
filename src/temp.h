#ifndef TEMP_H
#define TEMP_H

#include <inttypes.h>

#define TEMP_ERROR_MARGIN 0.3

#define TEMP_N_MIN 35
#define TEMP_N_MAX 37
#define TEMP_NORMAL(t) ((t) <= TEMP_N_MAX && (t) >= TEMP_N_MIN)

double readStableTemp(uint16_t ms);

#endif

