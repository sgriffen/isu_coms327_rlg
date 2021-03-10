/******* include std libs ********/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/****** include custom libs ******/
#include "cell.h"
#include "dungeon.h"
#include "utils/math_utils.h"


/****** function definitions ******/
Cell cell_init(uint8_t y, uint8_t x, int hardness) {
	
	uint8_t cell_hardness = 0;
	if (hardness < 0) { cell_hardness = (uint8_t)utils_rand_between(CELL_HARDNESS_MIN+1, CELL_HARDNESS_MAX-1, NULL); }
	else if (hardness == 0) { cell_hardness = (uint8_t)CELL_HARDNESS_MAX; }
	else { cell_hardness = (uint8_t)hardness; }
	
	Coordinate location = coordinate_init(y, x);
	Character_Wrapper wrapper = {
        .pc = NULL,
        .npc = NULL
	};

	Cell cell = {
		
		.location = location,
		.type = CellType_Wall,
		.character = wrapper,
		.hardness = cell_hardness,
		.visited = 0,
		.meta_data = -1,
		.weight_ntunneling = UINT16_MAX,
		.weight_tunneling = UINT16_MAX
	};
	
	return cell;
}

int cell_immutable_ntunneling(Cell cell) { return cell.hardness > 0; }

int cell_immutable_tunneling(Cell cell) { return cell.hardness >= CELL_HARDNESS_MAX; }

Character_PC* cell_contains_pc(Cell cell) { return (cell.character.pc) ? cell.character.pc : NULL; }

Character_NPC* cell_contains_npc(Cell cell) { return (cell.character.npc) ? cell.character.npc : NULL; }

void cell_print(Cell cell, int print_fill, int print_weight, int print_color) {
	
	
	if (print_weight == 1) {
		
		if (cell.character.pc)          { character_print_pc(*(cell.character.pc), 1, print_color); }
		else if (cell.character.npc)    { character_print_npc(*(cell.character.npc), 1, print_color); }
	}
	else {

        if (cell.character.pc)          { character_print_pc(*(cell.character.pc), 0, print_color); }
        else if (cell.character.npc)    { character_print_npc(*(cell.character.npc), 0, print_color); }
		else {
		
			switch (cell.type) {
			
			case CellType_Room:			//print Room cell type
				
				if (print_weight == 2) { //print non-tunneling weight mod 10
					
					if (cell.weight_ntunneling >= UINT16_MAX-1) { printf("%s%c%s", "\x1b[97m", 'X', "\x1b[0m"); }
					else { printf("%s%d%s", "\x1b[97m", cell.weight_ntunneling % 10, "\x1b[0m"); }
				}
				else if (print_weight == 3) { 	//print tunneling weight mod 10
					
					if (cell.weight_tunneling >= UINT16_MAX-1) { printf("%s%c%s", "\x1b[97m", 'X', "\x1b[0m"); }
					else { printf("%s%d%s", "\x1b[97m", cell.weight_tunneling % 10, "\x1b[0m"); }
				}
				else if (print_weight == 4) { printf("%s%d%s", "\x1b[97m", CELL_TRAVERSAL_COST(cell.hardness), "\x1b[0m"); }	//print cell traversal cost
				else if (print_fill == 2) { printf("%s%c%s", "\x1b[97m", ' ', "\x1b[0m"); } 	//print as empty space
				else { printf("%s%c%s", "\x1b[97m", '.', "\x1b[0m"); }
			break;
			case CellType_Cooridor:		//print Cooridor cell type
				
				if (print_weight == 2) { 	//print non-tunneling weight mod 10
					
					if (cell.weight_ntunneling >= UINT16_MAX-1) { printf("%s%c%s", "\x1b[97m", 'X', "\x1b[0m"); }
					else { printf("%s%d%s", "\x1b[97m", cell.weight_ntunneling % 10, "\x1b[0m"); }
				}
				else if (print_weight == 3) { 	//print tunneling weight mod 10
					
					if (cell.weight_tunneling >= UINT16_MAX-1) { printf("%s%c%s", "\x1b[97m", 'X', "\x1b[0m"); }
					else { printf("%s%d%s", "\x1b[97m", cell.weight_tunneling % 10, "\x1b[0m"); }
				}
				else if (print_weight == 4) { printf("%s%d%s", "\x1b[97m", CELL_TRAVERSAL_COST(cell.hardness), "\x1b[0m"); }	//print cell traversal cost
				else if (print_fill == 2) { printf("%s%c%s", "\x1b[97m", ' ', "\x1b[0m"); } 	//print as empty space
				else { printf("%s%c%s", "\x1b[97m", '#', "\x1b[0m"); }
			break;
			case CellType_Stair_up:		//print Stair_up cell type
				
				if (print_weight == 2) { 	//print non-tunneling weight mod 10
					
					if (cell.weight_ntunneling >= UINT16_MAX-1) { printf("%s%c%s", "\x1b[97m", 'X', "\x1b[0m"); }
					else { printf("%s%d%s", "\x1b[97m", cell.weight_ntunneling % 10, "\x1b[0m"); }
				}
				else if (print_weight == 3) { 	//print tunneling weight mod 10
					
					if (cell.weight_tunneling >= UINT16_MAX-1) { printf("%s%c%s", "\x1b[97m", 'X', "\x1b[0m"); }
					else { printf("%s%d%s", "\x1b[97m", cell.weight_tunneling % 10, "\x1b[0m"); }
				}
				else if (print_weight == 4) { printf("%s%d%s", "\x1b[97m", CELL_TRAVERSAL_COST(cell.hardness), "\x1b[0m"); }	//print cell traversal cost
				else { printf("%s%c%s", "\x1b[97m", '<', "\x1b[0m"); }
				break;
			case CellType_Stair_down:	//print Stair_down cell type
				
				if (print_weight == 2) { 	//print non-tunneling weight mod 10
					
					if (cell.weight_ntunneling >= UINT16_MAX-1) { printf("%s%c%s", "\x1b[97m", 'X', "\x1b[0m"); }
					else { printf("%s%d%s", "\x1b[97m", cell.weight_ntunneling % 10, "\x1b[0m"); }
				}
				else if (print_weight == 3) {	 //print tunneling weight mod 10
					
					if (cell.weight_tunneling >= UINT16_MAX-1) { printf("%s%c%s", "\x1b[97m", 'X', "\x1b[0m"); }
					else { printf("%s%d%s", "\x1b[97m", cell.weight_tunneling % 10, "\x1b[0m"); }
				}
				else if (print_weight == 4) { printf("%s%d%s", "\x1b[97m", CELL_TRAVERSAL_COST(cell.hardness), "\x1b[0m"); } 	//print cell traversal cost
				else { printf("%s%c%s", "\x1b[97m", '>', "\x1b[0m"); }
			break;
			case CellType_Border:		//print border cell type if desired, else print a regular Wall type
				
				if (print_weight == 4) { printf("%s%c%s", "\x1b[97m", 'X', "\x1b[0m"); }										//print cell traversal cost
				else if (print_fill == 1) {
					
					if (cell.location.y == 0 || cell.location.y == DUNGEON_HEIGHT-1) 	{ printf("%s%c%s", "\x1b[97m", '-', "\x1b[0m"); } //print as horizontal line
					else 																{ printf("%s%c%s", "\x1b[97m", '|', "\x1b[0m"); } //print as vertical line
				}
				else if (print_fill == 2) { printf("%s%s%s", "\x1b[97m", "\u2588", "\x1b[0m"); } 	//print as solid block
				else { printf("%s%c%s", "\x1b[97m", ' ', "\x1b[0m"); }
			break;
			default:					//print Wall cell type
				
				if (print_weight == 3) { printf("%s%d%s", "\x1b[97m", cell.weight_tunneling % 10, "\x1b[0m"); } 	//print tunneling weight mod 10
				else if (print_weight == 4) { printf("%s%d%s", "\x1b[97m", CELL_TRAVERSAL_COST(cell.hardness), "\x1b[0m"); }	//print cell traversal cost
				else if (print_fill == 2) {
					
					if (cell.hardness < CELL_HARDNESS_MAX / 3) { printf("%s%s%s", "\x1b[97m", "\u2591", "\x1b[0m"); } 		//print as somewhat-filled block
					else if (cell.hardness < CELL_HARDNESS_MAX / 2) { printf("%s%s%s", "\x1b[97m", "\u2592", "\x1b[0m"); } 	//print as half-filled block
					else if (cell.hardness < CELL_HARDNESS_MAX)	{ printf("%s%s%s", "\x1b[97m", "\u2593", "\x1b[0m"); } 		//print as mostly-filled block
				}
				else { printf("%s%c%s", "\x1b[97m", ' ', "\x1b[0m"); }
			break;
			}
		}
	}
	
	return;
}