#include <string>
#include <vector>

#include <stdint.h>

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "./npc_template.h"
#include "./item_template.h"
#include "./dungeon.h"
#include "./character.h"
#include "./item.h"
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
		uint8_t boss_defeated;
		
		PC pc;
		
		std::vector<NPC_Template> npc_templates;
		std::vector<Item_Template> item_templates;
		
		Character* *characters;
		
		std::vector<std::string> npc_unique_placed;
		std::vector<std::string> item_unique_placed;
		
		uint64_t queue_position;
		Queue queue;
		
		uint8_t menu_index;
		uint16_t menu_offset;
		
		uint32_t num_dungeons;
		int64_t dungeon_index;
		uint32_t num_dungeons_generated;
		Dungeon *dungeons;
		
		Coordinate cursor_map;
		
		uint8_t cursor_menu;
		uint8_t cursor_menu_max;
		
		std::string message;
		
		/* CONSTRUCTORS */
		GameState();
		GameState(uint32_t state_num_dungeons, uint32_t state_num_npcs, uint32_t state_num_items, std::vector<NPC_Template> state_npc_templates, std::vector<Item_Template> state_item_templates);
		
		/* FUNCTIONS */
		void run(RunArgs run_args);
		
		int contains_npcs();
		
		void draw(int print_fill, int print_weight);
	
	private:
		void init_pc();
		void init_queue();
		void deinit_queue();
		
		int run_turn(RunArgs run_args);
		
		int parse_input();
		
		int run_turn_pc(Dungeon *dungeon, int direction[2]);
		void run_turn_npc(Dungeon *dungeon, Character *wrapper);
		
		void gameover(int print_fill, int print_weight);
		
		void increase_dungeon();
		void decrease_dungeon();
		
		int item_pickup(Cell *cell);
		
		int io_default(int key);
		int io_menu_npc_list(int key);
		int io_menu_pc_warp(int key);
		int io_menu_npc_target(int key);
		int io_menu_npc_target_description(int key);
		int io_menu_item_pickup(int key);
		int io_menu_pc_inventory(int key);
		int io_menu_pc_inventory_inspect(int key);
		int io_menu_pc_inventory_inspect_description(int key);
		int io_menu_pc_drop(int key);
		int io_menu_pc_expunge(int key);
		int io_menu_pc_equipment(int key);
		int io_menu_pc_equip(int key);
		int io_menu_pc_unequip(int key);
		
		void draw_dungeon(int print_fill, int print_weight);
		
		void draw_menu_npc_list();
		void draw_menu_pc_warp(int print_fill, int print_weight);
		void draw_menu_npc_target(int print_fill, int print_weight);
		void draw_menu_npc_target_description();
		void draw_menu_item_pickup();
		void draw_menu_pc_inventory();
		void draw_menu_pc_inventory_inspect();
		void draw_menu_pc_inventory_inspect_description();
		void draw_menu_pc_drop();
		void draw_menu_pc_expunge();
		void draw_menu_pc_equipment();
		void draw_menu_pc_equip();
		void draw_menu_pc_unequip();
		
		void draw_gameover_win();
		void draw_gameover_lose(int print_fill, int print_weight);
		void draw_gameover_unexpected();
};

/****** function declarations *****/


#endif /* GAMESTATE_H */