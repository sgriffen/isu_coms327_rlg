#include "src/dungeon.h"

/********** definitions **********/
#define ARGS_LENGTH 3
#define ARGS_DEPTH 1

/******* enums declarations *******/


/******* struct declarations ******/


/****** function declarations *****/
void args_parse(int argc, char *argv[], int (*run_args)[ARGS_LENGTH][ARGS_DEPTH]);

int fread_dungeon(Dungeon *dungeon);

int fwrite_dungeon(Dungeon dungeon);