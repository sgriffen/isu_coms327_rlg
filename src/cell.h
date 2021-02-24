#ifndef CELL_H
#define CELL_H

#include <stdint.h>

/********** definitions **********/
#define CELL_HARDNESS_MIN 0
#define CELL_HARDNESS_MAX 255
#define CELL_NUM_NEIGHBORS 8
#define CELL_TRAVERSAL_COST 1

/************* macros ************/
#define CELL_HARDNESS_TRAVERSAL(hardness) ((hardness / 85) + CELL_TRAVERSAL_COST)


/******* enums declarations *******/
typedef enum {
	
	CellType_Wall,
	CellType_Room,
	CellType_Cooridor,
	CellType_Stair_up,
	CellType_Stair_down,
	CellType_Border
} CellType;

/******* struct declarations ******/
typedef struct {
	
	uint8_t y;
	uint8_t x;
} Coordinate;

typedef struct {
	
	Coordinate location;
	uint8_t hardness;
	
	uint8_t visited;
	int meta_data;
	uint8_t weight_ntunneling;
	uint8_t weight_tunneling;
	
	CellType type;
} Cell;

/****** function declarations *****/
Cell cell_init(uint8_t y, uint8_t x, int hardness);

int coordinate_is_same(Coordinate beta, Coordinate alpha);

int coordinate_is_neighbor(Coordinate beta, Coordinate alpha);

int cell_immutable(Cell cell);

void cell_print(Cell cell, int print_fill, int print_weight);

#endif /* CELL_H */