#include <stdint.h>

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "./dungeon.h"
#include "./character.h"
#include "./utils/queue.h"
#include "./utils/config.h"

/********** definitions **********/


/************* macros *************/


/******* enums declarations *******/


/******* struct declarations ******/
typedef struct {
	
	uint64_t turn;
	uint8_t allow_move;
	
	Character_PC pc;
	
	Character_Wrapper *characters;
	uint64_t queue_position;
	Queue queue;
	
	uint8_t menu_index;
	uint16_t menu_offset;
	
	uint32_t num_dungeons;
	int64_t dungeon_index;
	uint32_t num_dungeons_generated;
	Dungeon *dungeons;
	
	char *message;
} GameState;

/****** function declarations *****/
GameState state_init(uint32_t num_dungeons, uint32_t num_npcs);

void state_init_pc(GameState *g_state);

void state_init_queue(GameState *g_state);

void state_deinit_queue(GameState *g_state);

void state_run(GameState *g_state, RunArgs run_args);

int state_turn(GameState *g_state, RunArgs run_args);

int state_parse_input(GameState *g_state, Character_Wrapper *wrapper);

int state_move_pc(Dungeon *dungeon, Character_Wrapper wrapper, int direction[2]);

void state_move_npc(Dungeon *dungeon, Character_Wrapper wrapper, Character_PC pc);

void state_increase_dungeon(GameState *g_state);

void state_decrease_dungeon(GameState *g_state);

int state_contains_npcs(GameState *g_state);

void state_gameover(GameState *g_state, int print_fill, int print_color, int print_weight);

void state_draw(GameState g_state, int print_fill, int print_color, int print_weight);

void state_draw_dungeon(GameState g_state, int print_fill, int print_color, int print_weight);

void state_draw_menu_npc(GameState g_state);

void state_draw_gameover_win(GameState g_state);

void state_draw_gameover_lose(GameState g_state, int print_fill, int print_color, int print_weight);

void state_draw_gameover_unexpected(GameState g_state);


#endif /* GAMESTATE_H */