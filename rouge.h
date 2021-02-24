#ifndef ROUGE_H
#define ROUGE_H

#include "src/dungeon.h"
#include "src/utils/file_utils.h"
#include "src/utils/pathfinder.h"

/********** definitions **********/
#define ARGS_LENGTH 4
#define ARGS_DEPTH 2

/******* enums declarations *******/


/******* struct declarations ******/


/****** function definitions ******/
void args_parse(int argc, char *argv[], int (*run_args)[ARGS_LENGTH][ARGS_DEPTH]);

void rouge_init(Dungeon *dungeon, char *argv[], int run_args[ARGS_LENGTH][ARGS_DEPTH]);

void rouge_run(Dungeon *dungeon, int run_args[ARGS_LENGTH][ARGS_DEPTH]);

void rouge_clean(Dungeon *dungeon, char *argv[], int run_args[ARGS_LENGTH][ARGS_DEPTH]);

int fread_dungeon(Dungeon *dungeon, char* f_name);

int fwrite_dungeon(Dungeon dungeon, char* f_name);

#endif /* ROUGE_H */