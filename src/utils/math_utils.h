#include <time.h>

int utils_rand_between(int lower_bound, int upper_bound, time_t *seed);

int utils_rand_chance(float chance, time_t *seed);

int utils_min(int beta, int alpha);

int utils_max(int beta, int alpha);