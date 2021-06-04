#ifndef ROUGE_H
#define ROUGE_H

#include <string>
#include <vector>

#include "./src/game/classdef/state.h"
#include "./src/config/config.h"
#include "./src/config/color.h"

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

std::vector<NPC_Template> rouge_parse_npc(std::string filename);

std::vector<Item_Template> rouge_parse_item(std::string filename);

#endif /* ROUGE_H */