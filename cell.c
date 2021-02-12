/******* include std libs ********/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/****** include custom libs ******/
#include "cell.h"
#include "utils.h"

struct Cell cell_init(int y, int x) {
	
	struct Cell cell;
	
	cell.y = y;
	cell.x = x;
	
	cell.meta_data = -1;
	
	cell.type = Wall;
	cell.hardness = CELL_HARDNESS_MAX;
	
	return cell;
}

struct Cell cell_init_rand(int y, int x) {
	
	struct Cell cell;
	
	cell.y = y;
	cell.x = x;
	
	cell.meta_data = -1;
	
	cell.type = Wall;
	cell.hardness = utils_rand_between(CELL_HARDNESS_MIN+1, CELL_HARDNESS_MAX, NULL);
	
	return cell;
}

int cell_is_same(struct Cell beta, struct Cell alpha) {
	
	if (beta.y == alpha.y && beta.x == alpha.x) { return 1; }
	
	return 0;
}

void cell_print(struct Cell cell, int print_border) {
	
	switch (cell.type) {
	
	case Room:			//print Room cell type
		
		printf(".");
		break;
	case Cooridor:		//print Cooridor cell type
			
		printf("#");
		break;
	case Stair_up:		//print Stair_up cell type
		
		printf("<");
		break;
	case Stair_down:	//print Stair_down cell type
		
		printf(">");
		break;
	case Border_h:		//print horizontal Border cell type if desired, else print a regular Wall type
		
		if (print_border) { printf("-"); }
		else { printf(" "); }
		break;
	case Border_v: 		//print vertical Border cell type if desired, else print a regular Wall type
	
		if (print_border) { printf("|"); }
		else { printf(" "); }
		break;
	default:			//print Wall cell type
		
		printf(" ");
		break;
	}
	
	return;
}