/******* include std libs ********/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/****** include custom libs ******/
#include "cell.h"
#include "utils/math_utils.h"

Cell cell_init(uint8_t y, uint8_t x, uint8_t hardness) {
	
	Cell cell;
	
	cell.y = y;
	cell.x = x;
	
	cell.meta_data = -1;
	
	cell.type = CellType_Wall;
	
	
	if (hardness < 0) { cell.hardness = (uint8_t)utils_rand_between(CELL_HARDNESS_MIN+1, CELL_HARDNESS_MAX-1, NULL); }
	else if (hardness == 0) { cell.hardness = (uint8_t)CELL_HARDNESS_MAX; }
	else { cell.hardness = (uint8_t)hardness; }
	
	return cell;
}

int cell_is_same(Cell beta, Cell alpha) {
	
	if (beta.y == alpha.y && beta.x == alpha.x) { return 1; }
	
	return 0;
}

void cell_print(Cell cell, int print_border) {
	
	switch (cell.type) {
	
	case CellType_Room:				//print Room cell type
		
		printf("%c", '.');
		break;
	case CellType_Cooridor:			//print Cooridor cell type
			
		printf("%c", '#');
		break;
	case CellType_Stair_up:			//print Stair_up cell type
		
		printf("%c", '<');
		break;
	case CellType_Stair_down:		//print Stair_down cell type
		
		printf("%c", '>');
		break;
	case CellType_Border_h:			//print horizontal Border cell type if desired, else print a regular Wall type
		
		if (print_border) { printf("%c", '-'); }
		else { printf("%c", ' '); }
		break;
	case CellType_Border_v: 		//print vertical Border cell type if desired, else print a regular Wall type
	
		if (print_border) { printf("%c", '|'); }
		else { printf("%c", ' '); }
		break;
	default:						//print Wall cell type
		
		printf("%c", ' ');
		break;
	}
	
	return;
}