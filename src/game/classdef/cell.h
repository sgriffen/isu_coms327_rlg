#ifndef CELL_H
#define CELL_H

#include <fstream>
#include <vector>

#include <stdint.h>

#include "./coordinate.h"
#include "./character.h"
#include "./item.h"
#include "../../config/color.h"

/********** definitions ***********/
#define CELL_HARDNESS_MIN 0
#define CELL_HARDNESS_MAX 255
#define CELL_NUM_NEIGHBORS 8

/************* macros *************/
#define CELL_TRAVERSAL_COST(hardness) ((hardness / 85) + 1)


/******* enums declarations *******/
enum CellType {
	
	CellType_Wall,
	CellType_Room,
	CellType_Cooridor,
	CellType_Stair_up,
	CellType_Stair_down,
	CellType_Border
};

/******* struct declarations ******/
class Cell;

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
		int immutable_ntunneling();
		int immutable_tunneling();
		PC* contains_pc();
		NPC* contains_npc();
		std::vector<Item*>* contains_items();
		
		void clean();
		
		void disk_load(std::ifstream& file, std::vector<Item_Template> item_templates, std::vector<Item*> *items_loaded);
		void disk_save(std::ofstream& file);
		
		int draw(uint8_t print_y, uint8_t print_x, int print_fog, int print_fill, int print_weight);
};

/****** function declarations *****/
int cell_immutable_ntunneling(Cell cell);

int cell_immutable_tunneling(Cell cell);

#endif /* CELL_H */