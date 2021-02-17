#ifndef CELL_H
#define CELL_H

#include <stdint.h>

/********** definitions **********/
#define CELL_HARDNESS_MIN 0
#define CELL_HARDNESS_MAX 255

/******* enums declarations *******/
typedef enum {
	
	CellType_Border_h,
	CellType_Border_v,
	CellType_Wall,
	CellType_Room,
	CellType_Cooridor,
	CellType_Stair_up,
	CellType_Stair_down
} CellType;

/******* struct declarations ******/
typedef struct {
	
	uint8_t y;
	uint8_t x;
	uint8_t hardness;
	
	CellType type;
	
	int meta_data;
} Cell;

#endif /* DUNGEON_H */

/****** function declarations *****/
Cell cell_init(uint8_t y, uint8_t x, uint8_t hardness);

int cell_is_same(Cell beta, Cell alpha);

void cell_print(Cell cell, int print_border);