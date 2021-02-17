/******** include std libs ********/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
/******* include custom libs ******/
#include "math_utils.h"

int utils_rand_between(int lower_bound, int upper_bound, time_t *seed) {
	
	//srand(time(seed));
	
	return (rand() % ((upper_bound + 1) - lower_bound)) + lower_bound;
}

int utils_rand_chance(float chance, time_t *seed) {
	
	//srand(time(seed));

	return (rand() % 10000) < (chance*100);
}

int utils_min(int beta, int alpha) {

	if (beta < alpha) { return beta; }
	
	return alpha;
}

int utils_max(int beta, int alpha) {
	
	if (beta > alpha) { return beta; }
	
	return alpha;
}