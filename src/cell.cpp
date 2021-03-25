/******* include std libs ********/
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
/****** include custom libs ******/
#include "./cell.h"
#include "./dungeon.h"
#include "./utils/math_utils.h"

/****** function definitions ******/
Cell cell_init(uint8_t y, uint8_t x, int hardness) {
	
	uint8_t cell_hardness = 0;
	if (hardness < 0) { cell_hardness = (uint8_t)utils_rand_between(CELL_HARDNESS_MIN+1, CELL_HARDNESS_MAX-1, NULL); }
	else if (hardness == 0) { cell_hardness = (uint8_t)CELL_HARDNESS_MAX; }
	else { cell_hardness = (uint8_t)hardness; }
	
	Cell cell;
	cell.location = coordinate_init(y, x);
	cell.type_next = CellType_Wall;
	cell.type_current = CellType_Wall;
	cell.character = NULL;
	cell.hardness = cell_hardness;
	cell.visited = 0;
	cell.visible = 0;
	cell.meta_data = -1;
	cell.weight_ntunneling = UINT16_MAX;
	cell.weight_tunneling = UINT16_MAX;
	
	return cell;
}

int cell_immutable_ntunneling(Cell cell) { return cell.hardness > 0; }

int cell_immutable_tunneling(Cell cell) { return cell.hardness >= CELL_HARDNESS_MAX; }

PC* cell_contains_pc(Cell cell) { return (cell.character && cell.character->id < 1) ? ((PC*)(cell.character)) : (NULL); }

NPC* cell_contains_npc(Cell cell) { return (cell.character && cell.character->id > 0) ? ((NPC*)(cell.character)) : (NULL); }

void cell_draw(Cell cell, uint8_t y, uint8_t x, int print_fog, int print_fill, int print_color, int print_weight) {
	
	if (print_weight == 1) {
		
		if (cell.character && cell.character->id < 1) 									{ character_print_pc(*((PC*)cell.character), y, x, 1, print_color); }
		else if (cell.character && cell.character->id > 0 && cell.character->hp > 0)  	{ character_print_npc(*((NPC*)cell.character), y, x, 1, print_color); }
	}
	else {
		
		if (cell_immutable_tunneling(cell)) {
			
			if (print_weight == 4) 		{ mvaddch(y, x, 'X'); }										//print cell traversal cost
			else if (print_fill == 1) 	{
				
				if (cell.location.y == 0 || cell.location.y == DUNGEON_HEIGHT-1) 	{ mvaddch(y, x, '-'); } //print as horizontal line
				else 																{ mvaddch(y, x, '|'); } //print as vertical line
			}
			else if (print_fill == 2) 	{ mvprintw(y, x, "%s", "\u2588"); } 	//print as solid block
			else 						{ mvaddch(y, x, ' '); }
		}
		else if (print_fog && !cell.visited) {
			
			mvaddch(y, x, ' ');
			return;
		}
		else {
		
			if (print_fog && cell.visible) 			{ attron(A_BOLD); }
			else if (print_fog && !(cell.visible)) 	{ attron(A_DIM); }
			  
			if (cell.character && cell.character->id < 1)          													{ character_print_pc(*((PC*)cell.character), y, x, 0, print_color); }
			else if ((print_fog && cell.visible && cell.character && cell.character->id > 0 && cell.character->hp > 0)
						|| (!print_fog && cell.character && cell.character->id > 0 && cell.character->hp > 0))  	{ character_print_npc(*((NPC*)cell.character), y, x, 0, print_color); }
			else {
				
				CellType switch_type = cell.type_current;
				if (~print_fog) { switch_type = cell.type_next; }
				
				init_pair(0, COLOR_WHITE, COLOR_BLACK);
				attron(COLOR_PAIR(0));
				switch (switch_type) {
				
				case CellType_Room:			//print Room cell type
					
					if (print_weight == 2) { //print non-tunneling weight mod 10
						
						if (cell.weight_ntunneling >= UINT16_MAX-1) { mvaddch(y, x, 'X'); }
						else 										{ mvprintw(y, x, "%d", cell.weight_ntunneling % 10); }
					}
					else if (print_weight == 3) { 	//print tunneling weight mod 10
						
						if (cell.weight_tunneling >= UINT16_MAX-1) 	{ mvaddch(y, x, 'X'); }
						else 										{ mvprintw(y, x, "%d", cell.weight_tunneling % 10); }
					}
					else if (print_weight == 4) { mvprintw(y, x, "%d", CELL_TRAVERSAL_COST(cell.hardness)); }	//print cell traversal cost
					else if (print_fill == 2) 	{ mvaddch(y, x, ' '); } 	//print as empty space
					else 						{ mvaddch(y, x, '.'); }
				break;
				case CellType_Cooridor:		//print Cooridor cell type
					
					if (print_weight == 2) { 	//print non-tunneling weight mod 10
						
						if (cell.weight_ntunneling >= UINT16_MAX-1) { mvaddch(y, x, 'X'); }
						else 										{ mvprintw(y, x, "%d", cell.weight_ntunneling % 10); }
					}
					else if (print_weight == 3) { 	//print tunneling weight mod 10
						
						if (cell.weight_tunneling >= UINT16_MAX-1) 	{ mvaddch(y, x, 'X'); }
						else 										{ mvprintw(y, x, "%d", cell.weight_tunneling % 10); }
					}
					else if (print_weight == 4) { mvprintw(y, x, "%d", CELL_TRAVERSAL_COST(cell.hardness)); }	//print cell traversal cost
					else if (print_fill == 2) 	{ mvaddch(y, x, ' '); } 	//print as empty space
					else 						{ mvaddch(y, x, '#'); }
				break;
				case CellType_Stair_up:		//print Stair_up cell type
					
					if (print_weight == 2) { 	//print non-tunneling weight mod 10
						
						if (cell.weight_ntunneling >= UINT16_MAX-1) { mvaddch(y, x, 'X'); }
						else 										{ mvprintw(y, x, "%d", cell.weight_ntunneling % 10); }
					}
					else if (print_weight == 3) { 	//print tunneling weight mod 10
						
						if (cell.weight_tunneling >= UINT16_MAX-1) 	{ mvaddch(y, x, 'X'); }
						else 										{ mvprintw(y, x, "%d", cell.weight_tunneling % 10); }
					}
					else if (print_weight == 4) { mvprintw(y, x, "%d", CELL_TRAVERSAL_COST(cell.hardness)); }	//print cell traversal cost
					else 						{ mvaddch(y, x, '<'); }
					break;
				case CellType_Stair_down:	//print Stair_down cell type
					
					if (print_weight == 2) { 	//print non-tunneling weight mod 10
						
						if (cell.weight_ntunneling >= UINT16_MAX-1) { mvaddch(y, x, 'X'); }
						else 										{ mvprintw(y, x, "%d", cell.weight_ntunneling % 10); }
					}
					else if (print_weight == 3) {	 //print tunneling weight mod 10
						
						if (cell.weight_tunneling >= UINT16_MAX-1) 	{ mvaddch(y, x, 'X'); }
						else 										{ mvprintw(y, x, "%d", cell.weight_tunneling % 10); }
					}
					else if (print_weight == 4) 					{ mvprintw(y, x, "%d", CELL_TRAVERSAL_COST(cell.hardness)); } 	//print cell traversal cost
					else 											{ mvaddch(y, x, '>'); }
				break;
				default:					//print Wall cell type
					
					if (print_weight == 3) 		{ mvprintw(y, x, "%d", cell.weight_tunneling % 10); } 	//print tunneling weight mod 10
					else if (print_weight == 4) { mvprintw(y, x, "%d", CELL_TRAVERSAL_COST(cell.hardness)); }	//print cell traversal cost
					else if (print_fill == 2) 	{
						
						if (cell.hardness < CELL_HARDNESS_MAX / 3) 		{ mvprintw(y, x, "%s", "\u2591"); } // print as somewhat-filled block
						else if (cell.hardness < CELL_HARDNESS_MAX / 2) { mvprintw(y, x, "%s", "\u2592"); } // print as half-filled block
						else if (cell.hardness < CELL_HARDNESS_MAX)		{ mvprintw(y, x, "%s", "\u2593"); } // print as mostly-filled block
					}
					else { mvaddch(y, x, ' '); }
				break;
				}
				attroff(COLOR_PAIR(0));
			}
			attroff(A_BOLD);
			attroff(A_DIM);
		}
	}
	
	return;
}