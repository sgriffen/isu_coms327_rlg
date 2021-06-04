/******* include std libs ********/
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
/****** include custom libs ******/
#include "./classdef/cell.h"
#include "./classdef/dungeon.h"
#include "../utils/math_utils.h"

/***** constructor definitions *****/
Cell::Cell() {
	
	hardness 			= CELL_HARDNESS_MAX;
	type_next 			= CellType_Wall;
	type_current 		= CellType_Wall;
	character 			= NULL;
	items				= std::vector<Item*>();
	visited 			= 0;
	visible 			= 0;
	meta_data 			= -1;
	weight_ntunneling 	= INT32_MAX;
	weight_tunneling 	= INT32_MAX;
}
Cell::Cell(uint8_t cell_y, uint8_t cell_x, int cell_hardness) : Cell() {
	
	location = Coordinate(cell_y, cell_x);
	if (cell_hardness < 0) { hardness = (uint8_t)utils_rand_between(CELL_HARDNESS_MIN+1, CELL_HARDNESS_MAX-1, NULL); }
	else if (cell_hardness == 0) { hardness = (uint8_t)CELL_HARDNESS_MAX; }
	else { hardness = (uint8_t)cell_hardness; }
}

/****** function definitions ******/
int Cell::immutable_ntunneling() 	{ return hardness > 0; }

int Cell::immutable_tunneling() 	{ return hardness >= CELL_HARDNESS_MAX; }

PC* Cell::contains_pc() 	{ return (character && character->id < 1) ? ((PC*)(character)) : (NULL); }

NPC* Cell::contains_npc() 	{ return (character && character->id > 0) ? ((NPC*)(character)) : (NULL); }

std::vector<Item*>* Cell::contains_items() { return (!items.empty()) ? &(items) : NULL; }

void Cell::clean() {
	
	character = NULL;
	items.clear();
	
	return;
}

int Cell::draw(uint8_t print_y, uint8_t print_x, int print_fog, int print_fill, int print_weight) {
	
	int print_width = 1;
	
	if (print_weight == 1) {
		
		if (character && character->hp > 0) { character->draw(print_y, print_x, 1); }
		else if (items.size()) {
			
			if (items.size() > 1) { mvprintw(print_y, print_x, "%2c", '&'); }
			else { items[0]->draw(print_y, print_x, 0); }
		}
		else { mvprintw(print_y, print_x, "%2x", hardness); }
		print_width = 2;
	}
	else {
		
		if (print_fog && !visited) {
			
			mvaddch(print_y, print_x, ' ');
			return 1;
		}
		if (cell_immutable_tunneling(*(this))) {
			
			if (print_weight == 4) 		{ mvaddch(print_y, print_x, 'X'); }									//print cell traversal cost
			else if (print_fill == 1) 	{
				
				if (location.y == 0 || location.y == DUNGEON_HEIGHT-1) 		{ mvaddch(print_y, print_x, '-'); } //print as horizontal line
				else if (location.x == 0 || location.x == DUNGEON_WIDTH-1)	{ mvaddch(print_y, print_x, '|'); } //print as vertical line
				else 														{ mvaddch(print_y, print_x, ' '); }
			}
			else if (print_fill == 2) 	{ mvprintw(print_y, print_x, "%s", "\u2588"); } 	//print as solid block
			else 						{ mvaddch(print_y, print_x, ' '); }
		}
		else {
		
			if (print_fog && visible) 			{ attron(A_BOLD); }
			else if (print_fog && !(visible)) 	{ attron(A_DIM); }
			  
			if ((print_fog && visible && character && character->hp > 0) || (!print_fog && character && character->hp > 0)) 
				{ character->draw(print_y, print_x, 0); }
			else if ((print_fog && visible && items.size()) || (!print_fog && items.size())) {
						
				if (items.size() > 1) { mvaddch(print_y, print_x, '&'); }
				else { items[0]->draw(print_y, print_x, 0); }
			}
			else {
				
				CellType switch_type = type_current;
				if (~print_fog) { switch_type = type_next; }
				
				attron(COLOR_PAIR(1));
				switch (switch_type) {
				
				case CellType_Room:			//print Room cell type
					
					if (print_weight == 2) { //print non-tunneling weight mod 10
						
						if (weight_ntunneling >= INT32_MAX-1) 	{ mvaddch(print_y, print_x, 'X'); }
						else 									{ mvprintw(print_y, print_x, "%d", weight_ntunneling % 10); }
					}
					else if (print_weight == 3) { 	//print tunneling weight mod 10
						
						if (weight_tunneling >= INT16_MAX-1) 	{ mvaddch(print_y, print_x, 'X'); }
						else 									{ mvprintw(print_y, print_x, "%d", weight_tunneling % 10); }
					}
					else if (print_weight == 4) { mvprintw(print_y, print_x, "%d", CELL_TRAVERSAL_COST(hardness)); }	//print cell traversal cost
					else if (print_fill == 2) 	{ mvaddch(print_y, print_x, ' '); } 	//print as empty space
					else 						{ mvaddch(print_y, print_x, '.'); }
				break;
				case CellType_Cooridor:		//print Cooridor cell type
					
					if (print_weight == 2) { 	//print non-tunneling weight mod 10
						
						if (weight_ntunneling >= INT32_MAX-1) 	{ mvaddch(print_y, print_x, 'X'); }
						else 									{ mvprintw(print_y, print_x, "%d", weight_ntunneling % 10); }
					}
					else if (print_weight == 3) { 	//print tunneling weight mod 10
						
						if (weight_tunneling >= INT16_MAX-1) 	{ mvaddch(print_y, print_x, 'X'); }
						else 									{ mvprintw(print_y, print_x, "%d", weight_tunneling % 10); }
					}
					else if (print_weight == 4) { mvprintw(print_y, print_x, "%d", CELL_TRAVERSAL_COST(hardness)); }	//print cell traversal cost
					else if (print_fill == 2) 	{ mvaddch(print_y, print_x, ' '); } 	//print as empty space
					else 						{ mvaddch(print_y, print_x, '#'); }
				break;
				case CellType_Stair_up:		//print Stair_up cell type
					
					if (print_weight == 2) { 	//print non-tunneling weight mod 10
						
						if (weight_ntunneling >= INT32_MAX-1) 	{ mvaddch(print_y, print_x, 'X'); }
						else 									{ mvprintw(print_y, print_x, "%d", weight_ntunneling % 10); }
					}
					else if (print_weight == 3) { 	//print tunneling weight mod 10
						
						if (weight_tunneling >= INT16_MAX-1) 	{ mvaddch(print_y, print_x, 'X'); }
						else 									{ mvprintw(print_y, print_x, "%d", weight_tunneling % 10); }
					}
					else if (print_weight == 4) { mvprintw(print_y, print_x, "%d", CELL_TRAVERSAL_COST(hardness)); }	//print cell traversal cost
					else 						{ mvaddch(print_y, print_x, '<'); }
					break;
				case CellType_Stair_down:	//print Stair_down cell type
					
					if (print_weight == 2) { 	//print non-tunneling weight mod 10
						
						if (weight_ntunneling >= INT32_MAX-1) 	{ mvaddch(print_y, print_x, 'X'); }
						else 									{ mvprintw(print_y, print_x, "%d", weight_ntunneling % 10); }
					}
					else if (print_weight == 3) {	 //print tunneling weight mod 10
						
						if (weight_tunneling >= INT16_MAX-1) 	{ mvaddch(print_y, print_x, 'X'); }
						else 									{ mvprintw(print_y, print_x, "%d", weight_tunneling % 10); }
					}
					else if (print_weight == 4) 				{ mvprintw(print_y, print_x, "%d", CELL_TRAVERSAL_COST(hardness)); } 	//print cell traversal cost
					else 										{ mvaddch(print_y, print_x, '>'); }
				break;
				default:					//print Wall cell type
					
					if (print_weight == 3) 		{ mvprintw(print_y, print_x, "%d", weight_tunneling % 10); } 	//print tunneling weight mod 10
					else if (print_weight == 4) { mvprintw(print_y, print_x, "%d", CELL_TRAVERSAL_COST(hardness)); }	//print cell traversal cost
					else if (print_fill == 2) 	{
						
						if (hardness < CELL_HARDNESS_MAX / 3) 		{ mvprintw(print_y, print_x, "%s", "\u2591"); } // print as somewhat-filled block
						else if (hardness < CELL_HARDNESS_MAX / 2) 	{ mvprintw(print_y, print_x, "%s", "\u2592"); } // print as half-filled block
						else if (hardness < CELL_HARDNESS_MAX)		{ mvprintw(print_y, print_x, "%s", "\u2593"); } // print as mostly-filled block
					}
					else { mvaddch(print_y, print_x, ' '); }
					break;
				}
				attroff(COLOR_PAIR(1));
			}
			attroff(A_BOLD);
			attroff(A_DIM);
		}
	}
	
	return print_width;
}
int cell_immutable_ntunneling(Cell cell) { return cell.hardness > 0; }

int cell_immutable_tunneling(Cell cell) { return cell.hardness >= CELL_HARDNESS_MAX; }

PC* cell_contains_pc(Cell cell) { return (cell.character && cell.character->id < 1) ? ((PC*)(cell.character)) : (NULL); }

NPC* cell_contains_npc(Cell cell) { return (cell.character && cell.character->id > 0) ? ((NPC*)(cell.character)) : (NULL); }

std::vector<Item*>* cell_contains_items(Cell cell) { return (!cell.items.empty()) ? &(cell.items) : NULL; }