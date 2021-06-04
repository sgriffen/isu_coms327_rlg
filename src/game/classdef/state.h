#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <stdint.h>

#include "./dungeon.h"
#include "./character.h"
#include "./item.h"
#include "../utils/classdef/queue.h"
#include "../../config/config.h"

/********** definitions **********/
#define STATE_MENU_NPC 1
#define STATE_MENU_PC_WARP 2
#define STATE_MENU_NPC_TARGET 3
#define STATE_MENU_NPC_TARGET_SUB 4
#define STATE_MENU_ITEM_PICKUP 5
#define STATE_MENU_INVENTORY 6
#define STATE_MENU_INVENTORY_INSPECT 7
#define STATE_MENU_INVENTORY_INSPECT_SUB 8
#define STATE_MENU_INVENTORY_DROP 9
#define STATE_MENU_INVENTORY_EXPUNGE 10
#define STATE_MENU_EQUIPMENT 11
#define STATE_MENU_EQUIPMENT_EQUIP 12
#define STATE_MENU_EQUIPMENT_UNEQUIP 13
#define STATE_MENU_MAP_DEFAULT 14
#define STATE_MENU_MAP_NTUNNELING 15
#define STATE_MENU_MAP_TUNNELING 16
#define STATE_MENU_MAP_HARDNESS 17
#define STATE_MENU_OPTIONS 18

/************* macros *************/


/******* enums declarations *******/


/******* class declarations ******/
class GameState;

class GameState {
	public:
		uint64_t turn;
		uint8_t allow_move;
		uint8_t fog_enabled;
		uint8_t boss_defeated;
		
		PC pc;
		
		std::vector<NPC_Template> npc_templates;
		std::vector<Item_Template> item_templates;
		std::vector<Item*> items_loaded;
		
		Character **characters;
		
		std::vector<std::string> npc_unique_placed;
		std::vector<std::string> item_unique_placed;
		
		Queue queue_turn;
		
		RunArgs run_args;
		
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
		std::string dir_disk;
		
		/* CONSTRUCTORS */
		GameState();
		GameState(uint32_t state_num_dungeons, uint32_t state_num_npcs, uint32_t state_num_items, std::vector<NPC_Template> state_npc_templates, std::vector<Item_Template> state_item_templates);
		
		/* FUNCTIONS */
		void run();
		void clean();
		
		void disk_load();
		
		int contains_npcs();
		
		void draw(int print_fill);
	
	private:
		void init_pc();
		void init_queue();
		void deinit_queue();
		
		int run_turn();
		
		void disk_save();
		
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
		int io_menu_map_default(int key);
		int io_menu_map_ntunneling(int key);
		int io_menu_map_tunneling(int key);
		int io_menu_map_hardness(int key);
		
		void draw_dungeon_default(int print_fill);
		void draw_dungeon_ntunneling(int print_fill);
		void draw_dungeon_tunneling(int print_fill);
		void draw_dungeon_hardness(int print_fill);
		
		void draw_menu_npc_list();
		void draw_menu_pc_warp(int print_fill);
		void draw_menu_npc_target(int print_fill);
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