/******** include std libs ********/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
/******* include custom libs ******/
#include "utils.h"

int utils_rand_between(int lower_bound, int upper_bound, time_t *seed) {
	
	//srand(time(seed));
	
	return (rand() % ((upper_bound + 1) - lower_bound)) + lower_bound;
}

int utils_rand_chance(int chance_hundredths, time_t *seed) {
	
	//srand(time(seed));
	
	return (rand() % 10000) < chance_hundredths;
}