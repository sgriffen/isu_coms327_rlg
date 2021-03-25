/******* include std libs ********/
#include <stdint.h>

/****** include custom libs ******/
#include "./coordinate.h"


/****** function definitions ******/
Coordinate coordinate_init(uint8_t y, uint8_t x) {
	
	Coordinate coordinate;
	coordinate.y = y;
	coordinate.x = x;
	
	return coordinate;
}

int coordinate_is_same(Coordinate beta, Coordinate alpha) { return (beta.y == alpha.y && beta.x == alpha.x); }

int coordinate_is_neighbor_card(Coordinate beta, Coordinate alpha) {
	
	if (beta.y == (alpha.y)-1 && beta.x == alpha.x) { return 1; }
	if (beta.y == (alpha.y)+1 && beta.x == alpha.x) { return 1; }
	if (beta.x == (alpha.x)-1 && beta.y == alpha.y) { return 1; }
	if (beta.x == (alpha.x)+1 && beta.y == alpha.y) { return 1; }
	
	return 0;
}

int coordinate_is_neighbor_diag(Coordinate beta, Coordinate alpha) {
	
	return (!coordinate_is_same(beta, alpha)
		&& (beta.y == (alpha.y)-1 || beta.y == alpha.y || beta.y == (alpha.y)+1)
		&& (beta.x == (alpha.x)-1 || beta.x == alpha.x || beta.x == (alpha.x)+1)
	);
}