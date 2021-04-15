#include <vector>

#include <stdint.h>

#ifndef CELL_H
#define CELL_H

#include "./coordinate.h"
#include "./character.h"
#include "./item.h"
#include "../../res/config.h"

/********** definitions ***********/
#define CELL_HARDNESS_MIN 0
#define CELL_HARDNESS_MAX 255
#define CELL_NUM_NEIGHBORS 8

/************* macros *************/
#define CELL_TRAVERSAL_COST(hardness) ((hardness / 85) + 1)


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
class Cell {
	public:
		Coordinate location;
		uint8_t hardness;
		CellType type_next;
		CellType type_current;
		Character *character;
		
		std::vector<Item*> items;
		
		uint8_t visited;
		uint8_t visible;
		int meta_data;
		uint32_t weight_ntunneling;
		uint32_t weight_tunneling;
		
		/* CONSTRUCTORS */
		Cell();
		Cell(uint8_t cell_y, uint8_t cell_x, int cell_hardness);
		
		/* FUNCTIONS */
		void draw(uint8_t print_y, uint8_t print_x, int print_fog, int print_fill, int print_weight);
};

/****** function declarations *****/
int cell_immutable_ntunneling(Cell cell);

int cell_immutable_tunneling(Cell cell);

PC* cell_contains_pc(Cell cell);

NPC* cell_contains_npc(Cell cell);

std::vector<Item*>* cell_contains_items(Cell cell);

void cell_draw(Cell cell, uint8_t y, uint8_t x, int print_fog, int print_fill, int print_color, int print_weight);

#endif /* CELL_H */