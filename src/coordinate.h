#include <stdint.h>

#ifndef COORDINATE_H
#define COORDINATE_H

/********** definitions ***********/



/************* macros *************/



/******* enums declarations *******/



/******* struct declarations ******/
typedef struct {
	
	uint8_t y;
	uint8_t x;
} Coordinate;

/****** function declarations *****/
Coordinate coordinate_init(uint8_t y, uint8_t x);

int coordinate_is_same(Coordinate beta, Coordinate alpha);

int coordinate_is_neighbor_card(Coordinate beta, Coordinate alpha);

int coordinate_is_neighbor_diag(Coordinate beta, Coordinate alpha);


#endif /* COORDINATE_H */