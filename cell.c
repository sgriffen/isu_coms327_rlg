/******* include std libs ********/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/****** include custom libs ******/
#include "utils.h"
#include "cell.h"

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
	cell.hardness = utils_rand_between(1, CELL_HARDNESS_MAX, NULL);
	
	return cell;
}

void cell_print(struct Cell cell) {
	
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
	default:			//print Wall cell type
		
		printf(" ");
		break;
	}
	
	return;
}