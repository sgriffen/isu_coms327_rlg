/******* include std libs ********/
#include <stdint.h>

/****** include custom libs ******/
#include "./classdef/coordinate.h"


/***** constructor definitions *****/
Coordinate::Coordinate() {
	
	y = 0;
	x = 0;
}
Coordinate::Coordinate(uint8_t coordinate_y, uint8_t coordinate_x) : Coordinate() {
	
	y = coordinate_y;
	x = coordinate_x;
}
Coordinate::Coordinate(const Coordinate &rhs) {
	
	y = rhs.y;
	x = rhs.x;
}

/****** function definitions ******/
int Coordinate::is_same(Coordinate beta) { return (beta.y == y && beta.x == x); }
int Coordinate::is_neighbor_card(Coordinate beta) {
	
	if (beta.y == (y)-1 && beta.x == x) { return 1; }
	if (beta.y == (y)+1 && beta.x == x) { return 1; }
	if (beta.x == (x)-1 && beta.y == y) { return 1; }
	if (beta.x == (x)+1 && beta.y == y) { return 1; }
	
	return 0;
}
int Coordinate::is_neighbor_diag(Coordinate beta) {
	
	return (!is_same(beta)
		&& (beta.y == (y)-1 || beta.y == y || beta.y == (y)+1)
		&& (beta.x == (x)-1 || beta.x == x || beta.x == (x)+1)
	);
}