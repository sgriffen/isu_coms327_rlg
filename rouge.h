#ifndef ROUGE_H
#define ROUGE_H

#include "./src/state.h"
//#include "./src/utils/file_utils.h"
#include "./src/utils/config.h"

/********** definitions ***********/


/************* macros *************/


/******* enums declarations *******/


/******* struct declarations ******/


/****** function definitions ******/
void args_parse(int argc, char *argv[], RunArgs *run_args);

void rouge_init(GameState *g_state, char *argv[], RunArgs run_args);

void rouge_init_terminal();

void rouge_clean(GameState *g_state, char *argv[], RunArgs run_args);

void rouge_clean_terminal();

//int fread_dungeon(Dungeon *dungeon, char* f_name);

//int fwrite_dungeon(Dungeon dungeon, char* f_name);


#endif /* ROUGE_H */