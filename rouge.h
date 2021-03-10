#ifndef ROUGE_H
#define ROUGE_H

#include "./src/dungeon.h"
#include "./src/character.h"
#include "./src/utils/file_utils.h"
#include "./src/utils/pathfinder.h"
#include "./src/utils/movement.h"


/********** definitions ***********/


/************* macros *************/


/******* enums declarations *******/


/******* struct declarations ******/
typedef struct {
	
	float fps;
	
	uint8_t print_type;
	uint8_t print_color;
	uint8_t print_hardness;
	uint8_t print_weight_ntunneling;
	uint8_t print_weight_tunneling;
	uint8_t print_traversal_cost;
	
	uint8_t file_load;
	char *load_dir;
	uint8_t file_save;
	char *save_dir;
	
	uint32_t num_npcs;
} RunArgs;

/****** function definitions ******/
void args_parse(int argc, char *argv[], RunArgs *run_args);

void rouge_init(Dungeon *dungeon, char *argv[], RunArgs run_args);

void rouge_run(Dungeon *dungeon, RunArgs run_args);

void rouge_turn(Dungeon *dungeon, RunArgs run_args, Character_Wrapper *characters, Queue *movement_queue, uint64_t *turn, uint64_t *queue_position, uint8_t *pc_moved);

void rouge_move_pc(Dungeon *dungeon, Character_Wrapper wrapper, float fps);

void rouge_move_npc(Dungeon *dungeon, Character_Wrapper wrapper);

void rouge_gameover(Dungeon *dungeon, RunArgs run_args, uint64_t turn, uint8_t pc_moved);

void rouge_clean(Dungeon *dungeon, char *argv[], RunArgs run_args);

int fread_dungeon(Dungeon *dungeon, char* f_name);

int fwrite_dungeon(Dungeon dungeon, char* f_name);


#endif /* ROUGE_H */