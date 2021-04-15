#include <stdint.h>

#ifndef COORDINATE_H
#define COORDINATE_H

/********** definitions ***********/



/************* macros *************/



/******* enums declarations *******/



/******** class declarations ******/
class Coordinate{
	public:
		uint8_t y;
		uint8_t x;
		
		/* CONSTRUCTORS */
		Coordinate();
		Coordinate(uint8_t coordinate_y, uint8_t coordinate_x);
		Coordinate(const Coordinate &rhs);
		
		/* FUNCTIONS */
		int is_same(Coordinate beta);
		int is_neighbor_card(Coordinate beta);
		int is_neighbor_diag(Coordinate beta);
};

/****** function declarations *****/


#endif /* COORDINATE_H */