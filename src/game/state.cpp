/******** include std libs ********/
#include <string>
#include <sstream>

#include <stdlib.h>
#include <stdint.h>
#include <ncurses.h>
#include <string.h>

/******* include custom libs ******/
#include "./classdef/state.h"
#include "./utils/character_movement.h"
#include "./utils/pathfinder.h"
#include "../utils/math_utils.h"

/********** definitions **********/


/*********** global vars **********/


/***** constructor definitions *****/
GameState::GameState() {
	
	allow_move 				= 0;
	characters 				= NULL;
	npc_unique_placed		= std::vector<std::string>();
	item_unique_placed		= std::vector<std::string>();
	menu_index 				= 0;
	dungeon_index 			= 0;
	num_dungeons 			= 0;
	num_dungeons_generated 	= 0;
	message 				= "";
	turn 					= 0;
	fog_enabled 			= 1;
	boss_defeated			= 0;
	cursor_map.y 			= 0;
	cursor_map.x 			= 0;
	cursor_menu				= 0;
	cursor_menu_max			= 0;
	menu_offset 			= 0;
	dungeons 				= NULL;
	
	npc_templates			= std::vector<NPC_Template>();
	item_templates			= std::vector<Item_Template>();
}
GameState::GameState(uint32_t state_num_dungeons, uint32_t state_num_npcs, uint32_t state_num_items, std::vector<NPC_Template> state_npc_templates, std::vector<Item_Template> state_item_templates) : GameState() {
	
	num_dungeons = state_num_dungeons;
	
	dungeons = (Dungeon*)calloc(num_dungeons, sizeof(Dungeon));
	dungeons[0] = Dungeon((uint8_t)DUNGEON_HEIGHT, (uint8_t)DUNGEON_WIDTH, state_num_npcs, state_num_items, 0, 0, 1, state_npc_templates, state_item_templates, &npc_unique_placed, &item_unique_placed);
	num_dungeons_generated++;
	
	npc_templates = state_npc_templates;
	item_templates = state_item_templates;
	
	init_pc();
}

/****** function definitions ******/
void GameState::init_pc() {
	
	/*
		player character (pc) is placed at a random location in rooms[0] of dungeon[0]
	*/
	
	Coordinate loc;
	loc.y = (uint8_t)utils_rand_between(dungeons[0].rooms[0].tl.y+ROOM_BORDER_WIDTH, dungeons[0].rooms[0].br.y-ROOM_BORDER_WIDTH);
	loc.x = (uint8_t)utils_rand_between(dungeons[0].rooms[0].tl.x+ROOM_BORDER_WIDTH, dungeons[0].rooms[0].br.x-ROOM_BORDER_WIDTH);
	
	std::vector<uint8_t> colors = { PAIR_GREEN };
	pc = PC("Hero", "Our only hope!", '@', colors, loc, 1000, Dice(1, 2, 6), PC_SPEED);
	
	return;
}

void GameState::clean() {
	
	int i = 0;
	
	for (i = 0; i < (int)num_dungeons; i++) { dungeons[i].clean(); }
//	free(dungeons);
//	free(characters);
	
//	npc_unique_placed.clear();
//	item_unique_placed.clear();
//	items_loaded.clear();
	
	return;
}

int GameState::contains_npcs() {
	
	if (num_dungeons_generated < num_dungeons) 					{ return 1; }
	if (dungeon_contains_npcs((&dungeons[(num_dungeons)-1]))) 	{ return 1; }
	
	return 0;
}

void GameState::init_queue() {
	
	int i = 0;
	
	queue_turn = Queue();
	
	/* organize pc and npcs into priority queue based on movement speed */
	dungeons[dungeon_index].cells[pc.location.y][pc.location.x].character = &pc;
	for (i = 0; i < (dungeons[dungeon_index].num_npcs); i++) {
		
		Character *character = &(dungeons[dungeon_index].npcs[i]);
		queue_turn.enqueue(QueueNode(CHARACTER_TURN(character->speed), character));
		dungeons[dungeon_index].cells[character->location.y][character->location.x].character = character;
	}
	queue_turn.enqueue(QueueNode(CHARACTER_TURN(pc.speed), &pc));
}

void GameState::deinit_queue() {
	
	int i = 0;
	int j = 0;
	
	for (i = 0; i < dungeons[dungeon_index].volitile_height; i++) {
		for (j = 0; j < dungeons[dungeon_index].volitile_width; j++) {
			
			dungeons[dungeon_index].cells[i][j].character = NULL;
			dungeons[dungeon_index].cells[i][j].character = NULL;
		}
	}
	
	queue_turn.clear();
	free(characters);
}

void GameState::run() {
	
	init_queue();
	
	dungeon_pc_los(&(dungeons[dungeon_index]), (Character*)(&(pc)), 1);
	
	pathfinder_ntunneling(&(dungeons[dungeon_index]), &(pc.location));
	pathfinder_tunneling(&(dungeons[dungeon_index]), &(pc.location));
	
	while (pc.hp > 0 && contains_npcs() && !boss_defeated) {
		
		if (menu_index) {
			
			draw(run_args.print_config.fill);
			if (!parse_input()) { break; }
		}
		else if (!run_turn()) { break; }
	}
	
	gameover(run_args.print_config.fill, 0);
	
	return;
}

int GameState::run_turn() {

	int game_run = 1;
	
	/* draw dungeon with updated npc positions */
	dungeon_pc_los(&(dungeons[dungeon_index]), (Character*)(&(pc)), 1);
	draw(run_args.print_config.fill);
	
	if (!allow_move) {
		
		game_run = parse_input();
		/* draw dungeon with updated npc positions */
		draw(run_args.print_config.fill);
	}
	/* dequeue nodes if move allowed, stopping at pc node */
	while (!queue_turn.is_empty() && !boss_defeated) {
		
		QueueNode node = queue_turn.dequeue();
		Character *character = (Character*)(node.element);
		if (!(character->id)) {
			
			/* move pc */
			if (allow_move && character->hp > 0) {
				
				game_run = parse_input();
				if (character->prev_speed != (character->speed + ((PC*)character)->buff_speed())) {
					
					character->prev_speed = character->speed;
					character->speed = character->speed + ((PC*)character)->buff_speed();
					node.key -= (character->speed - character->prev_speed);
				} else { node.key = (queue_turn.tail->key + 1); }
				
				queue_turn.enqueue(node);
				
				pathfinder_ntunneling(&(dungeons[dungeon_index]), &(pc.location));
				pathfinder_tunneling(&(dungeons[dungeon_index]), &(pc.location));
			}
			else if (character->hp <= 0) { game_run = 0; }
			else { queue_turn.append(node); }
			
			/* draw dungeon with updated npc positions */
			draw(run_args.print_config.fill);
		} else {
			
			if (allow_move && character->hp > 0) {

				run_turn_npc(&(dungeons[dungeon_index]), character);
				(turn)++;
			}
			queue_turn.append(node);
		}
		
		if (game_run != 1 || !allow_move) { break; }
	}
	
	
	return game_run;
}

int GameState::parse_input() {
	
	int key = getch();
	switch (menu_index) {
		
	case STATE_MENU_NPC:		// in npc menu
		
		return io_menu_npc_list(key);
		break;
	case STATE_MENU_PC_WARP:		// in pc warp menu
		
		return io_menu_pc_warp(key);
		break;
	case STATE_MENU_NPC_TARGET:		//in npc target menu
		
		return io_menu_npc_target(key);
		break;
	case STATE_MENU_NPC_TARGET_SUB:	// in monster description menu (from case 3)
		
		return io_menu_npc_target_description(key);
		break;
	case STATE_MENU_ITEM_PICKUP:		// in item pickup menu
	
		return io_menu_item_pickup(key);
		break;
	case STATE_MENU_INVENTORY:		// in pc inventory menu
	
		return io_menu_pc_inventory(key);
		break;
	case STATE_MENU_INVENTORY_INSPECT:		// in pc inventory inspect menu
	
		return io_menu_pc_inventory_inspect(key);
		break;
	case STATE_MENU_INVENTORY_INSPECT_SUB:		// in pc inventory inspect description menu
	
		return io_menu_pc_inventory_inspect_description(key);
		break;
	case STATE_MENU_INVENTORY_DROP:		// in item drop menu
	
		return io_menu_pc_drop(key);
		break;
	case STATE_MENU_INVENTORY_EXPUNGE:	// in item expunge menu
	
		return io_menu_pc_expunge(key);
		break;
	case STATE_MENU_EQUIPMENT:	// in pc equipment menu
	
		return io_menu_pc_equipment(key);
		break;
	case STATE_MENU_EQUIPMENT_EQUIP:	// in pc equip menu
	
		return io_menu_pc_equip(key);
		break;
	case STATE_MENU_EQUIPMENT_UNEQUIP:	// in pc unequip menu
	
		return io_menu_pc_unequip(key);
		break;
	case STATE_MENU_MAP_NTUNNELING:	// in map ntunneling menu
	
		return io_menu_map_ntunneling(key);
		break;
	case STATE_MENU_MAP_TUNNELING:	// in map tunneling menu
	
		return io_menu_map_tunneling(key);
		break;
	case STATE_MENU_MAP_HARDNESS:	// in map hardness menu
	
		return io_menu_map_hardness(key);
		break;
	default:	// in no menu
		
		return io_default(key);
		break;
	}
}

int GameState::run_turn_pc(Dungeon *dungeon, int direction[2]) {
	
	Coordinate next = move_pc(dungeon, &pc, direction);
	
	message = "";
	
	if (cell_immutable_ntunneling(dungeon->cells[next.y][next.x])) { return 0; }
	
	uint32_t *collision = dungeon_resolve_collision(dungeon, (Character*)&pc, next);
	if (collision[0] > 0 && collision[1] > 0) {
		
		std::stringstream stream;
		stream << "PC attacked NPC at (" << std::to_string(next.y) << ", " << std::to_string(next.x) << ") for " << std::to_string(collision[1]);
		message.assign(stream.str());
	}
	if (!collision[0]) {
		if (dungeon->cells[next.y][next.x].contains_items()) {
			
			if (dungeon->cells[next.y][next.x].items.size() > 1) { message.assign("Multiple items found. Press ',' to decide which one to pick up"); }
			else { message.assign("Item found. Press ',' to pickup"); }
		}
	}
	if (collision[2]) { boss_defeated = 1; }
	
	free(collision);
	
	return 1;
}

void GameState::run_turn_npc(Dungeon *dungeon, Character *wrapper) {
	
	int i = 0;
	
	NPC *npc = (NPC*)wrapper;
	Coordinate next;
	next.x = 0;
	next.y = 0;
	
	uint8_t type_base = (uint8_t)((npc->type) & (NPC_TYPE_ERRATIC | NPC_TYPE_TUNNELING | NPC_TYPE_TELEPATHIC | NPC_TYPE_INTELLIGENT));
	switch(type_base) {
	
	case 1:
	
		next = move_npc1(dungeon, npc, pc.location);
		break;
	case 2:
		
		next = move_npc2(dungeon, npc);
		break;
	case 3:
		
		next = move_npc3(dungeon, npc);
		break;
	case 4:
		
		next = move_npc4(dungeon, npc);
		break;
	case 5:
		
		next = move_npc5(dungeon, npc, pc.location);
		break;
	case 6:
		
		next = move_npc6(dungeon, npc);
		break;
	case 7:
		
		next = move_npc7(dungeon, npc);
		break;
	case 8:
		
		next = move_npc8(dungeon, npc);
		break;
	case 9:
		
		next = move_npc9(dungeon, npc, pc.location);
		break;
	case 10:
		
		next = move_npcA(dungeon, npc);
		break;
	case 11:
		
		next = move_npcB(dungeon, npc);
		break;
	case 12:
		
		next = move_npcC(dungeon, npc);
		break;
	case 13:
		
		next = move_npcD(dungeon, npc, pc.location);
		break;
	case 14:
		
		next = move_npcE(dungeon, npc);
		break;
	case 15:
		
		next = move_npcF(dungeon, npc);
		break;
	default:
		
		next = move_npc0(dungeon, npc);
		break;
	}
	
	uint32_t *collision = dungeon_resolve_collision(dungeon, wrapper, next);
	if (collision[0] > 0 && collision[1] > 0) {
		
		std::stringstream stream;
		stream << "NPC at (" << std::to_string(npc->location.y) << ", " << std::to_string(npc->location.x) << ") attacked PC for " << std::to_string(collision[1]);
		message.assign(stream.str());
	}
	
	if (!collision[0]) { //only update cell at location next if no collision was detected
		
		if ((npc->type & NPC_TYPE_TUNNELING) && !((npc->type & NPC_TYPE_PASS)) && cell_immutable_ntunneling(dungeon->cells[next.y][next.x])) {
			
			dungeon->cells[next.y][next.x].hardness = 0;
			dungeon->cells[next.y][next.x].type_next = CellType_Cooridor;
		}
		
		if ((npc->type & NPC_TYPE_DESTROY) && dungeon->cells[next.y][next.x].contains_items()) {
			
			std::vector<Item*>::iterator cell_item_itr;
			for (cell_item_itr = dungeon->cells[next.y][next.x].items.begin(); cell_item_itr != dungeon->cells[next.y][next.x].items.end(); cell_item_itr++) {
				for (i = 0; i < dungeon->num_items; i++) {
					
					if (dungeon->items[i].id == (*cell_item_itr)->id) {
						
						Item temp = Item(dungeon->items[i]);
						dungeon->items[i] = Item(dungeon->items[(dungeon->num_items)-1]);
						dungeon->items[(dungeon->num_items)-1] = Item(temp);
						(dungeon->num_items)--;
					}
				}
			}
			
			dungeon->cells[next.y][next.x].items = std::vector<Item*>();
		}
		else if ((npc->type & NPC_TYPE_PICKUP) && dungeon->cells[next.y][next.x].contains_items()) {
			
			std::vector<Item*>::iterator cell_item_itr;
			for (cell_item_itr = dungeon->cells[next.y][next.x].items.begin(); cell_item_itr != dungeon->cells[next.y][next.x].items.end(); cell_item_itr++) { npc->inventory.push_back(*(cell_item_itr)); }
			
			dungeon->cells[next.y][next.x].items.erase(dungeon->cells[next.y][next.x].items.begin(), dungeon->cells[next.y][next.x].items.end());
		}
	}
	
	free(collision);
}

void GameState::gameover(int print_fill, int print_weight) {
	
	/* pc died */
	if (pc.hp < 1) { draw_gameover_lose(print_fill, print_weight); }
	/* all npcs in lowest-level dungeon are dead */
	else if (boss_defeated) { draw_gameover_win(); }
	
	return;
}

void GameState::increase_dungeon() {
	
	int i = 0;
	
	uint8_t num_npcs_defined = dungeons[dungeon_index].num_npcs_defined;
	uint16_t num_npcs = dungeons[dungeon_index].num_npcs;
	uint8_t num_items_defined = dungeons[dungeon_index].num_items_defined;
	uint16_t num_items = dungeons[dungeon_index].num_items;
	uint16_t num_stairs_up = dungeons[dungeon_index].num_staircases_down;
	uint8_t stair_index = 0;
	
	for (i = 0; i < dungeons[dungeon_index].num_staircases_down; i++) {
		
		if (dungeons[dungeon_index].staircases_down[i]->location.is_same(pc.location)) {
			
			stair_index = i;
			break;
		}
	}
	
	if (!num_npcs_defined)	{ num_npcs = 0; }
	if (!num_items_defined) { num_items = 0; }
	
	deinit_queue();
	dungeon_index++;
	
	if (dungeon_index == num_dungeons_generated) {
		
		if (dungeon_index == ((num_dungeons)-1)) 	{ dungeons[dungeon_index] = Dungeon((uint8_t)DUNGEON_HEIGHT, (uint8_t)DUNGEON_WIDTH, num_npcs, num_items, num_stairs_up, 0, -1, npc_templates, item_templates, &(npc_unique_placed), &(item_unique_placed)); }
		else 										{ dungeons[dungeon_index] = Dungeon((uint8_t)DUNGEON_HEIGHT, (uint8_t)DUNGEON_WIDTH, num_npcs, num_items, num_stairs_up, 0, 0, npc_templates, item_templates, &(npc_unique_placed), &(item_unique_placed)); }
		(num_dungeons_generated)++;
	}
	
	pc.location.y = dungeons[dungeon_index].staircases_up[stair_index]->location.y;
	pc.location.x = dungeons[dungeon_index].staircases_up[stair_index]->location.x;
	
	init_queue();
	
	return;
}
void GameState::decrease_dungeon() {
	
	int i = 0;
	
	uint8_t stair_index = 0;
	
	for (i = 0; i < dungeons[dungeon_index].num_staircases_up; i++) {
		
		if (dungeons[dungeon_index].staircases_up[i]->location.is_same(pc.location)) {
			
			stair_index = i;
			break;
		}
	}
	
	deinit_queue();
	dungeon_index--;
	
	pc.location.y = dungeons[dungeon_index].staircases_down[stair_index]->location.y;
	pc.location.x = dungeons[dungeon_index].staircases_down[stair_index]->location.x;
	
	init_queue();
	
	return;
}

int GameState::item_pickup(Cell *cell) {
	
	if (cell->contains_items()) {
		
		if (cell->items.size() > 1) {
			
			cursor_menu = 0;
			cursor_menu_max = cell->items.size();
			menu_index = STATE_MENU_ITEM_PICKUP;
			return 2;
		} else {
			
			if (pc.inventory.size() < PC_INVENTORY_MAX) {
				
				pc.inventory.push_back(*(cell->items.begin()));
				cell->items.erase(cell->items.begin());
				message.assign("Picked up item. Press 'i' for details");
			} else { message.assign("Inventory full. Drop or expunge an item from inventory"); }
		}
	}
	
	return 1;
}

void GameState::draw(int print_fill) {
	
	clear();
	
	start_color();
	
	switch (menu_index) {
		
	case STATE_MENU_NPC: 			//draw monster menu	
		
		draw_menu_npc_list();
		break;
	case STATE_MENU_PC_WARP:
		
		draw_menu_pc_warp(print_fill);
		break;
	case STATE_MENU_NPC_TARGET:
		
		draw_menu_npc_target(print_fill);
		break;
	case STATE_MENU_NPC_TARGET_SUB:
		
		draw_menu_npc_target_description();
		break;
	case STATE_MENU_ITEM_PICKUP:
		
		draw_menu_item_pickup();
		break;
	case STATE_MENU_INVENTORY:
		
		draw_menu_pc_inventory();
		break;
	case STATE_MENU_INVENTORY_INSPECT:
		
		draw_menu_pc_inventory_inspect();
		break;
	case STATE_MENU_INVENTORY_INSPECT_SUB:
		
		draw_menu_pc_inventory_inspect_description();
		break;
	case STATE_MENU_INVENTORY_DROP:
		
		draw_menu_pc_drop();
		break;
	case STATE_MENU_INVENTORY_EXPUNGE:
		
		draw_menu_pc_expunge();
		break;
	case STATE_MENU_EQUIPMENT:
		
		draw_menu_pc_equipment();
		break;
	case STATE_MENU_EQUIPMENT_EQUIP:
		
		draw_menu_pc_equip();
		break;
	case STATE_MENU_EQUIPMENT_UNEQUIP:
		
		draw_menu_pc_unequip();
		break;
	case STATE_MENU_MAP_NTUNNELING:
		
		draw_dungeon_ntunneling(print_fill);
		break;
	case STATE_MENU_MAP_TUNNELING:
		
		draw_dungeon_tunneling(print_fill);
		break;
	case STATE_MENU_MAP_HARDNESS:
		
		draw_dungeon_hardness(print_fill);
		break;
	default:			//draw current dungeon
		
		draw_dungeon_default(print_fill);
		break;
	}
	
	refresh();
	
	return;
}