/******** include std libs ********/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/******* include custom libs ******/
#include "rouge.h"
#include "utils.h"

int main(int argc, char *argv[]) {
	
	int run_args[ARGS_LENGTH][ARGS_DEPTH] = {
		{ 0 }
	};
	struct Level level;
	
	/* parse command args */
	args_parse(argc, argv, &run_args);
	
	/* create level */
	level = level_init((int)LEVEL_HEIGHT, (int)LEVEL_WIDTH);
	
	/* print created level */
	level_print(level, run_args[0][0]);
	
	/* de-allocate memory for level */
	//free(&level);
	
	return 0;
}

void args_parse(int argc, char *argv[], int (*run_args)[ARGS_LENGTH][ARGS_DEPTH]) {
	
	int i = 1;
	
	for (i = 1; i < argc; i++) {
		
		if (!strcmp(argv[i], "--border")) { (*run_args)[0][0] = 1; }
	}
}

