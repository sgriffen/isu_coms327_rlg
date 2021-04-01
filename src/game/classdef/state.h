#include <vector>

#include <stdint.h>

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "./npc_template.h"
#include "./item_template.h"
#include "./dungeon.h"
#include "./character.h"
#include "../utils/classdef/queue.h"
#include "../../res/config.h"

/********** definitions **********/


/************* macros *************/


/******* enums declarations *******/


/******* class declarations ******/
class GameState {
	public:
		uint64_t turn;
		uint8_t allow_move_pc;
		uint8_t allow_move_npc;
		uint8_t fog_enabled;
		
		PC pc;
		
		std::vector<NPC_Template> npc_templates;
		std::vector<Item_Template> item_templates;
		
		Character* *characters;
		
		uint64_t queue_position;
		Queue queue;
		
		uint8_t menu_index;
		uint16_t menu_offset;
		
		uint32_t num_dungeons;
		int64_t dungeon_index;
		uint32_t num_dungeons_generated;
		Dungeon *dungeons;
		
		Coordinate cursor;
		
		char *message;
		
		/* CONSTRUCTORS */
		GameState();
		GameState(uint32_t state_num_dungeons, uint32_t state_num_npcs, std::vector<NPC_Template> state_npc_templates, std::vector<Item_Template> state_item_templates);
		
		/* FUNCTIONS */
		
};

/****** function declarations *****/
void state_init_pc(GameState *g_state);

void state_init_queue(GameState *g_state);

void state_deinit_queue(GameState *g_state);

void state_run(GameState *g_state, RunArgs run_args);

int state_turn(GameState *g_state, RunArgs run_args);

int state_parse_input(GameState *g_state, Character *wrapper);

int state_move_pc(Dungeon *dungeon, Character *wrapper, int direction[2]);

void state_move_npc(Dungeon *dungeon, Character *wrapper, PC pc);

void state_increase_dungeon(GameState *g_state);

void state_decrease_dungeon(GameState *g_state);

int state_contains_npcs(GameState *g_state);

void state_gameover(GameState *g_state, int print_fill, int print_weight);

void state_draw(GameState g_state, int print_fill, int print_weight);

void state_draw_dungeon(GameState g_state, int print_fill, int print_weight);

void state_draw_menu_npc(GameState g_state);

void state_draw_menu_warp(GameState g_state, int print_fill, int print_weight);

void state_draw_gameover_win(GameState g_state);

void state_draw_gameover_lose(GameState g_state, int print_fill, int print_weight);

void state_draw_gameover_unexpected(GameState g_state);


#endif /* GAMESTATE_H */