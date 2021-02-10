/******** include std libs ********/
#include <stdlib.h>
#include <stdio.h>
/******* include custom libs ******/
#include "level.h"
#include "utils.h"

int main(int argc, char *argv[]) {
	
	struct Level level;
	
	/* create level */
	level = level_init((int)LEVEL_HEIGHT, (int)LEVEL_WIDTH);
	
	/* print created level (width and height including level border) */
	level_print(level);
	
	/* de-allocate memory for level */
	//free(level);
	
	return 0;
}