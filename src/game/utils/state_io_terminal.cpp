/******** include std libs ********/

/******* include custom libs ******/
#include "../classdef/state.h"
#include "../../utils/math_utils.h"

/********** definitions **********/


/*********** global vars **********/


/****** function definitions ******/
int GameState::io_default(int key) {
	
	Coordinate next;
	int move_matrix[2] = { 0, 0 };
	
	allow_move = 1;
	switch(key) {

	case KEY_UP:		// pc move up
		DEFAULT_CASE_MOVE_U:
		
		move_matrix[0] = -1;
		move_matrix[1] = 0;
		if (!run_turn_pc(&(dungeons[dungeon_index]), move_matrix)) { message.assign("Could not move PC up"); }
		else { (turn)++; }
		break;
	case KEY_PPAGE:		// pc move up-right
		DEFAULT_CASE_MOVE_UR:
		
		move_matrix[0] = -1;
		move_matrix[1] = 1;
		if (!run_turn_pc(&(dungeons[dungeon_index]), move_matrix)) { message.assign("Could not move PC up-right"); }
		else { (turn)++; }
		break;
	case KEY_RIGHT:		// pc move right
		DEFAULT_CASE_MOVE_R:
		
		move_matrix[0] = 0;
		move_matrix[1] = 1;
		if (!run_turn_pc(&(dungeons[dungeon_index]), move_matrix)) { message.assign("Could not move PC right"); }
		else { (turn)++; }
		break;
	case KEY_NPAGE:		// pc move down-right
		DEFAULT_CASE_MOVE_DR:
		
		move_matrix[0] = 1;
		move_matrix[1] = 1;
		if (!run_turn_pc(&(dungeons[dungeon_index]), move_matrix)) { message.assign("Could not move PC down-right"); }
		else { (turn)++; }
		break;
	case KEY_DOWN:		// pc move down
		DEFAULT_CASE_MOVE_D:
		
		move_matrix[0] = 1;
		move_matrix[1] = 0;
		if (!run_turn_pc(&(dungeons[dungeon_index]), move_matrix)) { message.assign("Could not move PC down"); }
		else { (turn)++; }
		break;
	case KEY_END:		// pc move down-left
		DEFAULT_CASE_MOVE_DL:
		
		move_matrix[0] = 1;
		move_matrix[1] = -1;
		if (!run_turn_pc(&(dungeons[dungeon_index]), move_matrix)) { message.assign("Could not move PC down-left"); }
		else { (turn)++; }
		break;
	case KEY_LEFT:		// pc move left
		DEFAULT_CASE_MOVE_L:
		
		move_matrix[0] = 0;
		move_matrix[1] = -1;
		if (!run_turn_pc(&(dungeons[dungeon_index]), move_matrix)) { message.assign("Could not move PC left"); }
		else { (turn)++; }
		break;
	case KEY_HOME:		// pc move up-left
		DEFAULT_CASE_MOVE_UL:
		
		move_matrix[0] = -1;
		move_matrix[1] = -1;
		if (!run_turn_pc(&(dungeons[dungeon_index]), move_matrix)) { message.assign("Could not move PC up-left"); }
		else { (turn)++; }
		break;
	case KEY_B2:		// pc rest
		DEFAULT_CASE_MOVE_REST:
		
		move_matrix[0] = 0;
		move_matrix[1] = 0;
		if (!run_turn_pc(&(dungeons[dungeon_index]), move_matrix)) { message.assign("Could not let PC rest"); }
		else { (turn)++; }
		break;
	case '<':			// pc go up staircase
		
		allow_move = 0;
		if (((dungeon_index - 1) > -1) && dungeons[dungeon_index].cells[pc.location.y][pc.location.x].type_current == CellType_Stair_up) {
			
			decrease_dungeon();
			(turn)++;
		}
		break;
	case '>':			// pc go down staircase
		
		allow_move = 0;
		if (((dungeon_index + 1) < num_dungeons) && dungeons[dungeon_index].cells[pc.location.y][pc.location.x].type_current == CellType_Stair_down) {
			
			increase_dungeon();
			(turn)++;
		}
		break;
	case 'f':			// enable/disable fog
		
		allow_move = 0;
		fog_enabled = !(fog_enabled);
		break;
	case 'm':			// enter npc menu
		
		allow_move = 0;
		menu_index = STATE_MENU_NPC;
		return 2;
		break;
	case 'g':			// enter pc warping menu
		
		allow_move = 0;
		cursor_map = pc.location;
		menu_index = STATE_MENU_PC_WARP;
		return 2;
		break;
	case 'L':			// enter npc target menu
		
		allow_move = 0;
		cursor_map = pc.location;
		menu_index = STATE_MENU_NPC_TARGET;
		return 2;
		break;
	case ',':			// pickup item or enter item pickup menu
		
		allow_move = 0;
		return item_pickup(&(dungeons[dungeon_index].cells[pc.location.y][pc.location.x]));
		break;
	case 'i':			// enter pc inventory menu
		
		allow_move = 0;
		menu_index = STATE_MENU_INVENTORY;
		return 2;
		break;
	case 'I':			// enter pc item inspect menu
		
		allow_move = 0;
		cursor_menu = 0;
		cursor_menu_max = pc.inventory.size();
		menu_index = STATE_MENU_INVENTORY_INSPECT;
		return 2;
		break;
	case 'd':			// enter pc drop item menu
		
		allow_move = 0;
		cursor_menu = 0;
		cursor_menu_max = pc.inventory.size();
		menu_index = STATE_MENU_INVENTORY_DROP;
		return 2;
		break;
	case 'x':			// enter pc expunge item menu
		
		allow_move = 0;
		cursor_menu = 0;
		cursor_menu_max = pc.inventory.size();
		menu_index = STATE_MENU_INVENTORY_EXPUNGE;
		return 2;
		break;
	case 'e':			// enter pc equipment menu
		
		allow_move = 0;
		menu_index = STATE_MENU_EQUIPMENT;
		return 2;
		break;
	case 'w':			// enter pc equip item menu
			
		allow_move = 0;
		cursor_menu = 0;
		cursor_menu_max = pc.inventory.size();
		menu_index = STATE_MENU_EQUIPMENT_EQUIP;
		break;
	case 't':			// enter pc unequip item menu
		
		allow_move = 0;
		cursor_menu = 0;
		cursor_menu_max = PC_EQUIPMENT_MAX;
		menu_index = STATE_MENU_EQUIPMENT_UNEQUIP;
		return 2;
		break;
	case 'D':			// display ntunneling distance map
		
		allow_move = 0;
		menu_index = STATE_MENU_MAP_NTUNNELING;
		return 2;
		break;
	case 'T':			// display tunneling distance map
		
		allow_move = 0;
		menu_index = STATE_MENU_MAP_TUNNELING;
		return 2;
		break;
	case 'H':			// display hardness map
	
		allow_move = 0;
		menu_index = STATE_MENU_MAP_HARDNESS;
		return 2;
		break;
	case 'M':			//mute game sound
		
		allow_move = 0;
		break;
	case 'S':			// save current game state
		
		allow_move = 0;
		disk_save();
		message.assign("Game state saved");
		break;
	case 'N':			// load game state from file
	
		clear();
		mvprintw(0, 0, "Game state loading...");
		refresh();
		
		clean();
		disk_load();
		message.assign("Game state loaded");
		
		break;
	case 'Q': 			// quit game
		
		allow_move = 0;
		return 0;
		break;
	case 'k':			// move up with 'k'
		
		goto DEFAULT_CASE_MOVE_U;
		break;
	case '8':			// move up with '8'
		
		goto DEFAULT_CASE_MOVE_U;
		break;
	case 'u':			// move up-right with 'u'
		
		goto DEFAULT_CASE_MOVE_UR;
		break;
	case '9':			// move up-right with '9'
		
		goto DEFAULT_CASE_MOVE_UR;
		break;
	case 'l':			// move right with 'l'
		
		goto DEFAULT_CASE_MOVE_R;
		break;
	case '6':			// move right with '6'
		
		goto DEFAULT_CASE_MOVE_R;
		break;
	case 'n':			// move down-right with 'n'
		
		goto DEFAULT_CASE_MOVE_DR;
		break;
	case '3':			// move down-right with '3'
		
		goto DEFAULT_CASE_MOVE_DR;
		break;
	case 'j':			// move down with 'j'
		
		goto DEFAULT_CASE_MOVE_D;
		break;
	case '2':			// move down with '2'
		
		goto DEFAULT_CASE_MOVE_D;
		break;
	case 'b':			// move down-left with 'b'
		
		goto DEFAULT_CASE_MOVE_DL;
		break;
	case '1':			// move down-left with '1'
		
		goto DEFAULT_CASE_MOVE_DL;
		break;
	case 'h':			// move left with 'h'
		
		goto DEFAULT_CASE_MOVE_L;
		break;
	case '4':			// move left with '4'
		
		goto DEFAULT_CASE_MOVE_L;
		break;
	case 'y':			// move up-left with 'y'
		
		goto DEFAULT_CASE_MOVE_UL;
		break;
	case '7':			// move up-left with '7'
		
		goto DEFAULT_CASE_MOVE_UL;
		break;
	case ' ':			// ' '
	
		goto DEFAULT_CASE_MOVE_REST;
		break;
	case '.':			// '.'
	
		goto DEFAULT_CASE_MOVE_REST;
		break;
	default: 
		
		allow_move = 0;
		message.assign("Unknown key pressed");
		break; 	//no-op entered, do nothing
	}
	return 1;
}
int GameState::io_menu_npc_list(int key) {
	
	allow_move = 0;
	switch(key) {
	case 0x1b: 			// escape key
		
		menu_index = 0;
		menu_offset = 0;
		return 2;
		break;
	case KEY_DOWN:		// down arrow
		NPC_LIST_CASE_MOVE_D:
		
		if (menu_offset < (dungeons[dungeon_index].num_npcs - 17) && dungeons[dungeon_index].num_npcs > 18) { (menu_offset)++; }
		break;
	case KEY_UP:		// up arrow
		NPC_LIST_CASE_MOVE_U:
		
		if (menu_offset) { (menu_offset)--; }
		break;
	case 'Q': 			// quit game
		
		return 0;
		break;
	/***** START REDUNDANT KEYS *****/
	case 'j':			// move down with 'j'
		
		goto NPC_LIST_CASE_MOVE_D;
		break;
	case '2':			// move down with '2'
		
		goto NPC_LIST_CASE_MOVE_D;
		break;
	case 'k':			// move up with 'k'
		
		goto NPC_LIST_CASE_MOVE_U;
		break;
	case '8':			// move up with '8'
		
		goto NPC_LIST_CASE_MOVE_U;
		break;
	default: 
		
		
		break; 	//no-op entered, do nothing
	}
	
	return 1;
}
int GameState::io_menu_pc_warp(int key) {
	
	Coordinate next;
	
	uint32_t *collision;
	
	allow_move = 0;
	switch(key) {
	
	case 0x1b: 			// escape key
		
		menu_index = 0;
		return 2;
		break;
	case 'g':
	
		next = cursor_map;
		
		collision = dungeon_resolve_collision(&(dungeons[dungeon_index]), &(pc), next, 1);
		if (!collision[0]) {
			if (dungeons[dungeon_index].cells[next.y][next.x].contains_items()) {
				
				if (dungeons[dungeon_index].cells[next.y][next.x].items.size() > 1) { message.assign("Multiple items found. Press ',' to decide which one to pick up"); }
				else { message.assign("Item found. Press ',' to pickup"); }
			}
		}
		if (collision[2]) { boss_defeated = 1; }
		
		free(collision);
		
		menu_index = 0;
		break;
	case 'r':
		
		next.y = (uint8_t)utils_rand_between(DUNGEON_BORDER_WIDTH, DUNGEON_HEIGHT-DUNGEON_BORDER_WIDTH, NULL);
		next.x = (uint8_t)utils_rand_between(DUNGEON_BORDER_WIDTH, DUNGEON_WIDTH-DUNGEON_BORDER_WIDTH, NULL);
		
		if (dungeon_resolve_collision(&(dungeons[dungeon_index]), &(pc), next)) { (pc.num_kills)++; }
		
		menu_index = 0;
		break;
	case KEY_UP:		// cursor_map move up
		PC_WARP_CASE_MOVE_U:
		
		next = cursor_map;
		next.y -= 1;
		if (dungeon_coordinate_inbounds(next)) { cursor_map = next; }
		break;
	case KEY_PPAGE:		// cursor_map move up-right
		PC_WARP_CASE_MOVE_UR:
		
		next = cursor_map;
		next.y -= 1;
		next.x += 1;
		if (dungeon_coordinate_inbounds(next)) {cursor_map = next; }
		break;
	case KEY_RIGHT:		// cursor_map move right
		PC_WARP_CASE_MOVE_R:
		
		next = cursor_map;
		next.x += 1;
		if (dungeon_coordinate_inbounds(next)) { cursor_map = next; }
		break;
	case KEY_NPAGE:		// cursor_map move down-right
		PC_WARP_CASE_MOVE_DR:
		
		next = cursor_map;
		next.y += 1;
		next.x += 1;
		if (dungeon_coordinate_inbounds(next)) { cursor_map = next; }
		break;
	case KEY_DOWN:		// cursor_map move down
		PC_WARP_CASE_MOVE_D:
		
		next = cursor_map;
		next.y += 1;
		if (dungeon_coordinate_inbounds(next)) { cursor_map = next; }
		break;
	case KEY_END:		// cursor_map move down-left
		PC_WARP_CASE_MOVE_DL:
		
		next = cursor_map;
		next.y += 1;
		next.x -= 1;
		if (dungeon_coordinate_inbounds(next)) { cursor_map = next; }
		break;
	case KEY_LEFT:		// cursor_map move left
		PC_WARP_CASE_MOVE_L:
		
		next = cursor_map;
		next.x -= 1;
		if (dungeon_coordinate_inbounds(next)) { cursor_map = next; }
		break;
	case KEY_HOME:		// cursor_map move up-left
		PC_WARP_CASE_MOVE_UL:
		
		next = cursor_map;
		next.y -= 1;
		next.x -= 1;
		if (dungeon_coordinate_inbounds(next)) { cursor_map = next; }
		break;
	case 'Q': 			// quit game
		
		return 0;
		break;
	/***** START REDUNDANT KEYS *****/
	case 'k':			// move up with 'k'
		
		goto PC_WARP_CASE_MOVE_U;
		break;
	case '8':			// move up with '8'
		
		goto PC_WARP_CASE_MOVE_U;
		break;
	case 'u':			// move up-right with 'u'
		
		goto PC_WARP_CASE_MOVE_UR;
		break;
	case '9':			// move up-right with '9'
		
		goto PC_WARP_CASE_MOVE_UR;
		break;
	case 'l':			// move right with 'l'
		
		goto PC_WARP_CASE_MOVE_R;
		break;
	case '6':			// move right with '6'
		
		goto PC_WARP_CASE_MOVE_R;
		break;
	case 'n':			// move down-right with 'n'
		
		goto PC_WARP_CASE_MOVE_DR;
		break;
	case '3':			// move down-right with '3'
		
		goto PC_WARP_CASE_MOVE_DR;
		break;
	case 'j':			// move down with 'j'
		
		goto PC_WARP_CASE_MOVE_D;
		break;
	case '2':			// move down with '2'
		
		goto PC_WARP_CASE_MOVE_D;
		break;
	case 'b':			// move down-left with 'b'
		
		goto PC_WARP_CASE_MOVE_DL;
		break;
	case '1':			// move down-left with '1'
		
		goto PC_WARP_CASE_MOVE_DL;
		break;
	case 'h':			// move left with 'h'
		
		goto PC_WARP_CASE_MOVE_L;
		break;
	case '4':			// move left with '4'
		
		goto PC_WARP_CASE_MOVE_L;
		break;
	case 'y':			// move up-left with 'y'
		
		goto PC_WARP_CASE_MOVE_UL;
		break;
	case '7':			// move up-left with '7'
		
		goto PC_WARP_CASE_MOVE_UL;
		break;
	default:			// no-op do nothing
		
		
		break;
	}
	
	return 1;
}
int GameState::io_menu_npc_target(int key) {
	
	Coordinate next;
	
	allow_move = 0;
	switch(key) {
		
	case 0x1b: 			// escape key
		
		menu_index = 0;
		return 2;
		break;
	case 'L':
		
		menu_index = 4;
		return 2;
		break;
	case KEY_UP:		// cursor_map move up
		NPC_TARGET_CASE_MOVE_U:
		
		next = cursor_map;
		next.y -= 1;
		if (dungeon_coordinate_inbounds(next)) { cursor_map = next; }
		break;
	case KEY_PPAGE:		// cursor_map move up-right
		NPC_TARGET_CASE_MOVE_UR:
		
		next = cursor_map;
		next.y -= 1;
		next.x += 1;
		if (dungeon_coordinate_inbounds(next)) {cursor_map = next; }
		break;
	case KEY_RIGHT:		// cursor_map move right
		NPC_TARGET_CASE_MOVE_R:
		
		next = cursor_map;
		next.x += 1;
		if (dungeon_coordinate_inbounds(next)) { cursor_map = next; }
		break;
	case KEY_NPAGE:		// cursor_map move down-right
		NPC_TARGET_CASE_MOVE_DR:
		
		next = cursor_map;
		next.y += 1;
		next.x += 1;
		if (dungeon_coordinate_inbounds(next)) { cursor_map = next; }
		break;
	case KEY_DOWN:		// cursor_map move down
		NPC_TARGET_CASE_MOVE_D:
		
		next = cursor_map;
		next.y += 1;
		if (dungeon_coordinate_inbounds(next)) { cursor_map = next; }
		break;
	case KEY_END:		// cursor_map move down-left
		NPC_TARGET_CASE_MOVE_DL:
		
		next = cursor_map;
		next.y += 1;
		next.x -= 1;
		if (dungeon_coordinate_inbounds(next)) { cursor_map = next; }
		break;
	case KEY_LEFT:		// cursor_map move left
		NPC_TARGET_CASE_MOVE_L:
		
		next = cursor_map;
		next.x -= 1;
		if (dungeon_coordinate_inbounds(next)) { cursor_map = next; }
		break;
	case KEY_HOME:		// cursor_map move up-left
		NPC_TARGET_CASE_MOVE_UL:
		
		next = cursor_map;
		next.y -= 1;
		next.x -= 1;
		if (dungeon_coordinate_inbounds(next)) { cursor_map = next; }
		break;
	case 'Q': 			// quit game
		
		return 0;
		break;
	/***** START REDUNDANT KEYS *****/
	case 'k':			// move up with 'k'
		
		goto NPC_TARGET_CASE_MOVE_U;
		break;
	case '8':			// move up with '8'
		
		goto NPC_TARGET_CASE_MOVE_U;
		break;
	case 'u':			// move up-right with 'u'
		
		goto NPC_TARGET_CASE_MOVE_UR;
		break;
	case '9':			// move up-right with '9'
		
		goto NPC_TARGET_CASE_MOVE_UR;
		break;
	case 'l':			// move right with 'l'
		
		goto NPC_TARGET_CASE_MOVE_R;
		break;
	case '6':			// move right with '6'
		
		goto NPC_TARGET_CASE_MOVE_R;
		break;
	case 'n':			// move down-right with 'n'
		
		goto NPC_TARGET_CASE_MOVE_DR;
		break;
	case '3':			// move down-right with '3'
		
		goto NPC_TARGET_CASE_MOVE_DR;
		break;
	case 'j':			// move down with 'j'
		
		goto NPC_TARGET_CASE_MOVE_D;
		break;
	case '2':			// move down with '2'
		
		goto NPC_TARGET_CASE_MOVE_D;
		break;
	case 'b':			// move down-left with 'b'
		
		goto NPC_TARGET_CASE_MOVE_DL;
		break;
	case '1':			// move down-left with '1'
		
		goto NPC_TARGET_CASE_MOVE_DL;
		break;
	case 'h':			// move left with 'h'
		
		goto NPC_TARGET_CASE_MOVE_L;
		break;
	case '4':			// move left with '4'
		
		goto NPC_TARGET_CASE_MOVE_L;
		break;
	case 'y':			// move up-left with 'y'
		
		goto NPC_TARGET_CASE_MOVE_UL;
		break;
	case '7':			// move up-left with '7'
		
		goto NPC_TARGET_CASE_MOVE_UL;
		break;
	default:			// no-op do nothing
		
		break;
	}
	
	return 1;
}
int GameState::io_menu_npc_target_description(int key) {
	
	allow_move = 0;
	switch(key) {
		
	case 0x1b: 			// escape key
		
		menu_index = 3;
		return 2;
		break;
	case 'Q': 			// quit game
		
		return 0;
		break;
	default:			// no-op do nothing
		
		break;
	}
	
	return 1;
}
int GameState::io_menu_item_pickup(int key) {
	
	allow_move = 0;
	switch(key) {
	
	case 0x1b: 			// escape key
		
		menu_index = 0;
		menu_offset = 0;
		return 2;
		break;
	case ',':			// pickup selected item
		
		if (pc.inventory.size() < PC_INVENTORY_MAX) {
			
			Cell *cell = &(dungeons[dungeon_index].cells[pc.location.y][pc.location.x]);
			if (cell->contains_items()) {
				
				pc.inventory.push_back(cell->items[cursor_menu]);
				cell->items.erase(cell->items.begin() + cursor_menu);
				message.assign("Picked up item(s). Press 'i' for details");
				
				cursor_menu_max = cell->items.size();
				if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
			}
		} else { message.assign("Inventory full. Drop or expunge an item from inventory"); }
		return 2;
		break;
	case KEY_DOWN:		// down arrow
		ITEM_PICKUP_CASE_MOVE_D:
		
		if (cursor_menu < cursor_menu_max-1) { (cursor_menu)++; }
		if (menu_offset < (dungeons[dungeon_index].cells[pc.location.y][pc.location.x].items.size() - 17) && dungeons[dungeon_index].cells[pc.location.y][pc.location.x].items.size() > 18) { (menu_offset)++; }
		break;
	case KEY_UP:		// up arrow
		ITEM_PICKUP_CASE_MOVE_U:
		
		if (cursor_menu) { (cursor_menu)--; }
		if (!cursor_menu && menu_offset) { (menu_offset)--; }
		break;
	case 'Q': 			// quit game
		
		return 0;
		break;
	/***** START REDUNDANT KEYS *****/
	case 'j':			// move down with 'j'
		
		goto ITEM_PICKUP_CASE_MOVE_D;
		break;
	case '2':			// move down with '2'
		
		goto ITEM_PICKUP_CASE_MOVE_D;
		break;
	case 'k':			// move up with 'k'
		
		goto ITEM_PICKUP_CASE_MOVE_U;
		break;
	case '8':			// move up with '8'
		
		goto ITEM_PICKUP_CASE_MOVE_U;
		break;
	default: 			// no-op do nothing
	
		break;
	}
	return 1;
}
int GameState::io_menu_pc_inventory(int key) {
	
	allow_move = 0;
	switch(key) {
		
	case 0x1b: 			// escape key, exit menu
		
		menu_index = 0;
		return 2;
		break;
	case KEY_DOWN:		// down arrow
		PC_INVENTORY_CASE_MOVE_D:
		
		if (menu_offset < (dungeons[dungeon_index].cells[pc.location.y][pc.location.x].items.size() - 17) && dungeons[dungeon_index].cells[pc.location.y][pc.location.x].items.size() > 18) { (menu_offset)++; }
		break;
	case KEY_UP:		// up arrow
		PC_INVENTORY_CASE_MOVE_U:
		
		if (!cursor_menu && menu_offset) { (menu_offset)--; }
		break;
	case 'Q': 			// quit game
			
		return 0;
		break;
	/***** START REDUNDANT KEYS *****/
	case 'j':			// move down with 'j'
		
		goto PC_INVENTORY_CASE_MOVE_D;
		break;
	case '2':			// move down with '2'
		
		goto PC_INVENTORY_CASE_MOVE_D;
		break;
	case 'k':			// move up with 'k'
		
		goto PC_INVENTORY_CASE_MOVE_U;
		break;
	case '8':			// move up with '8'
		
		goto PC_INVENTORY_CASE_MOVE_U;
		break;
	default:			// no-op do nothing
		
		break;
	}
	
	return 1;
}
int GameState::io_menu_pc_inventory_inspect(int key) {
	
	allow_move = 0;
	switch(key) {
		
	case 0x1b: 			// escape key, exit menu
		
		menu_index = 0;
		return 2;
		break;
	case 'I':
		
		menu_index = 8;
		return 2;
		break;
	case KEY_DOWN:		// down arrow
		PC_INVENTORY_INSPECT_CASE_MOVE_D:
		
		if (cursor_menu < cursor_menu_max-1) { (cursor_menu)++; }
		if (menu_offset < (dungeons[dungeon_index].cells[pc.location.y][pc.location.x].items.size() - 17) && dungeons[dungeon_index].cells[pc.location.y][pc.location.x].items.size() > 18) { (menu_offset)++; }
		break;
	case KEY_UP:		// up arrow
		PC_INVENTORY_INSPECT_CASE_MOVE_U:
		
		if (cursor_menu) { (cursor_menu)--; }
		if (!cursor_menu && menu_offset) { (menu_offset)--; }
		break;
	case 'Q': 			// quit game
			
		return 0;
		break;
	/***** START REDUNDANT KEYS *****/
	case 'j':			// move down with 'j'
		
		goto PC_INVENTORY_INSPECT_CASE_MOVE_D;
		break;
	case '2':			// move down with '2'
		
		goto PC_INVENTORY_INSPECT_CASE_MOVE_D;
		break;
	case 'k':			// move up with 'k'
		
		goto PC_INVENTORY_INSPECT_CASE_MOVE_U;
		break;
	case '8':			// move up with '8'
		
		goto PC_INVENTORY_INSPECT_CASE_MOVE_U;
		break;
	default:			// no-op do nothing
		
		break;
	}
	
	return 1;
}
int GameState::io_menu_pc_inventory_inspect_description(int key) {
	
	allow_move = 0;
	switch(key) {
		
	case 0x1b: 			// escape key, exit menu
		
		menu_index = 7;
		return 2;
		break;
	case 'Q': 			// quit game
		
		return 0;
		break;
	default:			// no-op do nothing
		
		break;
	}
	
	return 1;
}
int GameState::io_menu_pc_drop(int key) {
	
	allow_move = 0;
	switch(key) {
		
	case 0x1b: 			// escape key, exit menu
		
		menu_index = 0;
		return 2;
		break;
	case 'd':
		
		if (!(pc.inventory.empty())) {
			
			dungeons[dungeon_index].cells[pc.location.y][pc.location.x].items.push_back(pc.inventory[cursor_menu]);
			pc.inventory.erase(pc.inventory.begin() + cursor_menu);
			
			cursor_menu_max = pc.inventory.size();
			if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
		}
		break;
	case KEY_DOWN:		// down arrow
		ITEM_DROP_CASE_MOVE_D:
		
		if (cursor_menu < cursor_menu_max-1) { (cursor_menu)++; }
		if (menu_offset < (dungeons[dungeon_index].cells[pc.location.y][pc.location.x].items.size() - 17) && dungeons[dungeon_index].cells[pc.location.y][pc.location.x].items.size() > 18) { (menu_offset)++; }
		break;
	case KEY_UP:		// up arrow
		ITEM_DROP_CASE_MOVE_U:
		
		if (cursor_menu) { (cursor_menu)--; }
		if (!cursor_menu && menu_offset) { (menu_offset)--; }
		break;
	case 'Q': 			// quit game
		
		return 0;
		break;
	/***** START REDUNDANT KEYS *****/
	case 'j':			// move down with 'j'
		
		goto ITEM_DROP_CASE_MOVE_D;
		break;
	case '2':			// move down with '2'
		
		goto ITEM_DROP_CASE_MOVE_D;
		break;
	case 'k':			// move up with 'k'
		
		goto ITEM_DROP_CASE_MOVE_U;
		break;
	case '8':			// move up with '8'
		
		goto ITEM_DROP_CASE_MOVE_U;
		break;
	default:			// no-op do nothing
		
		break;
	}
	
	return 1;
}
int GameState::io_menu_pc_expunge(int key) {
	
	allow_move = 0;
	switch(key) {
		
	case 0x1b: 			// escape key, exit menu
		
		menu_index = 0;
		return 2;
		break;
	case 'x':
		
		if (!(pc.inventory.empty())) {
			
//			delete pc.inventory[cursor_menu];
			(dungeons[dungeon_index].num_items)--;
			pc.inventory.erase(pc.inventory.begin() + cursor_menu);
			
			cursor_menu_max = pc.inventory.size();
			if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
		}
		break;
	case KEY_DOWN:		// down arrow
		ITEM_EXPUNGE_CASE_MOVE_D:
		
		if (cursor_menu < cursor_menu_max-1) { (cursor_menu)++; }
		if (menu_offset < (dungeons[dungeon_index].cells[pc.location.y][pc.location.x].items.size() - 17) && dungeons[dungeon_index].cells[pc.location.y][pc.location.x].items.size() > 18) { (menu_offset)++; }
		break;
	case KEY_UP:		// up arrow
		ITEM_EXPUNGE_CASE_MOVE_U:
		
		if (cursor_menu) { (cursor_menu)--; }
		if (!cursor_menu && menu_offset) { (menu_offset)--; }
		break;
	/***** START REDUNDANT KEYS *****/
	case 'j':			// move down with 'j'
		
		goto ITEM_EXPUNGE_CASE_MOVE_D;
		break;
	case '2':			// move down with '2'
		
		goto ITEM_EXPUNGE_CASE_MOVE_D;
		break;
	case 'k':			// move up with 'k'
		
		goto ITEM_EXPUNGE_CASE_MOVE_U;
		break;
	case '8':			// move up with '8'
		
		goto ITEM_EXPUNGE_CASE_MOVE_U;
		break;
	case 'Q': 			// quit game
			
		return 0;
		break;
	default:			// no-op do nothing
		
		break;
	}
	
	return 1;
}
int GameState::io_menu_pc_equipment(int key) {
	
	allow_move = 0;
	switch(key) {
		
	case 0x1b: 			// escape key, exit menu
		
		menu_index = 0;
		return 2;
		break;
	case KEY_DOWN:		// down arrow
		PC_EQUIPMENT_CASE_MOVE_D:
		
		if (menu_offset < (dungeons[dungeon_index].cells[pc.location.y][pc.location.x].items.size() - 17) && dungeons[dungeon_index].cells[pc.location.y][pc.location.x].items.size() > 18) { (menu_offset)++; }
		break;
	case KEY_UP:		// up arrow
		PC_EQUIPMENT_CASE_MOVE_U:
		
		if (!cursor_menu && menu_offset) { (menu_offset)--; }
		break;
	case 'Q': 			// quit game
		
		return 0;
		break;
	/***** START REDUNDANT KEYS *****/
	case 'j':			// move down with 'j'
		
		goto PC_EQUIPMENT_CASE_MOVE_D;
		break;
	case '2':			// move down with '2'
		
		goto PC_EQUIPMENT_CASE_MOVE_D;
		break;
	case 'k':			// move up with 'k'
		
		goto PC_EQUIPMENT_CASE_MOVE_U;
		break;
	case '8':			// move up with '8'
		
		goto PC_EQUIPMENT_CASE_MOVE_U;
		break;
	default:			// no-op do nothing
		
		break;
	}
	
	return 1;
}
int GameState::io_menu_pc_equip(int key) {
	
	allow_move = 0;
	Item *equip = NULL;
	
	switch(tolower(key)) {
		
	case 0x1b: 			// escape key, exit menu
		
		menu_index = 0;
		return 2;
		break;
	case 'w':
	
		if (!pc.inventory.empty()) {
			
			equip = pc.inventory[cursor_menu];
			if ((equip->type >> ITEM_TYPE_WEAPON_SHIFT) & 1) {
				if (pc.equipment_weapon) { pc.inventory[cursor_menu] = pc.equipment_weapon; }
				else {
					
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
					
					(pc.num_items_equipped)++;
				}
				pc.equipment_weapon = equip;
				pc.equipped |= 0x0001;
			}
			else if ((equip->type >> ITEM_TYPE_OFFHAND_SHIFT) & 1) {
				
				if (pc.equipment_offhand) { pc.inventory[cursor_menu] = pc.equipment_offhand; }
				else {
					
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
					
					(pc.num_items_equipped)++;
				}
				pc.equipment_offhand = equip;
				pc.equipped |= 0x0002;
			}
			else if ((equip->type >> ITEM_TYPE_RANGED_SHIFT) & 1) {
				
				if (pc.equipment_ranged) { pc.inventory[cursor_menu] = pc.equipment_ranged; }
				else {
					
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
					
					(pc.num_items_equipped)++;
				}
				pc.equipment_ranged = equip;
				pc.equipped |= 0x0004;
			}
			else if ((equip->type >> ITEM_TYPE_ARMOR_SHIFT) & 1) {
				
				if (pc.equipment_armor) { pc.inventory[cursor_menu] = pc.equipment_armor; }
				else {
					
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
					
					(pc.num_items_equipped)++;
				}
				pc.equipment_armor = equip;
				pc.equipped |= 0x0008;
			}
			else if ((equip->type >> ITEM_TYPE_HELMET_SHIFT) & 1) {
				
				if (pc.equipment_helmet) { pc.inventory[cursor_menu] = pc.equipment_helmet; }
				else {
					
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
					
					(pc.num_items_equipped)++;
				}
				pc.equipment_helmet = equip;
				pc.equipped |= 0x0010;
			}
			else if ((equip->type >> ITEM_TYPE_CLOAK_SHIFT) & 1) {
				
				if (pc.equipment_cloak) { pc.inventory[cursor_menu] = pc.equipment_cloak; }
				else {
					
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
					
					(pc.num_items_equipped)++;
				}
				pc.equipment_cloak = equip;
				pc.equipped |= 0x0020;
			}
			else if ((equip->type >> ITEM_TYPE_GLOVES_SHIFT) & 1) {
				
				if (pc.equipment_gloves) { pc.inventory[cursor_menu] = pc.equipment_gloves; }
				else {
					
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
					
					(pc.num_items_equipped)++;
				}
				pc.equipment_gloves = equip;
				pc.equipped |= 0x0040;
			}
			else if ((equip->type >> ITEM_TYPE_BOOTS_SHIFT) & 1) {
				
				if (pc.equipment_boots) { pc.inventory[cursor_menu] = pc.equipment_boots; }
				else {
					
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
					
					(pc.num_items_equipped)++;
				}
				pc.equipment_boots = equip;
				pc.equipped |= 0x0080;
			}
			else if ((equip->type >> ITEM_TYPE_AMULET_SHIFT) & 1) {
				
				if (pc.equipment_amulet) { pc.inventory[cursor_menu] = pc.equipment_amulet; }
				else {
					
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
					
					(pc.num_items_equipped)++;
				}
				pc.equipment_amulet = equip;
				pc.equipped |= 0x0100;
			}
			else if ((equip->type >> ITEM_TYPE_LIGHT_SHIFT) & 1) {
				
				if (pc.equipment_light) { pc.inventory[cursor_menu] = pc.equipment_light; }
				else {
					
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
					
					(pc.num_items_equipped)++;
				}
				pc.equipment_light = equip;
				pc.equipped |= 0x0200;
			}
			else if ((equip->type >> ITEM_TYPE_RING_SHIFT) & 1) {
				
				if (key == 'w') {
					
					if (pc.equipment_ring0) { pc.inventory[cursor_menu] = pc.equipment_ring0; }
					else {
						
						pc.inventory.erase(pc.inventory.begin() + cursor_menu);
						cursor_menu_max = pc.inventory.size();
						if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
						
						(pc.num_items_equipped)++;
					}
					pc.equipment_ring0 = equip;
					pc.equipped |= 0x0400;
				}
				else if (key == 'W') {
					
					if (pc.equipment_ring1) { pc.inventory[cursor_menu] = pc.equipment_ring1; }
					else {
						
						pc.inventory.erase(pc.inventory.begin() + cursor_menu);
						cursor_menu_max = pc.inventory.size();
						if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
						
						(pc.num_items_equipped)++;
					}
					pc.equipment_ring1 = equip;
					pc.equipped |= 0x0800;
				}
			}
		}
		
		break;
	case KEY_DOWN:		// down arrow
		PC_EQUIP_CASE_MOVE_D:
		
		if (cursor_menu < cursor_menu_max-1) { (cursor_menu)++; }
		if (menu_offset < (dungeons[dungeon_index].cells[pc.location.y][pc.location.x].items.size() - 17) && dungeons[dungeon_index].cells[pc.location.y][pc.location.x].items.size() > 18) { (menu_offset)++; }
		break;
	case KEY_UP:		// up arrow
		PC_EQUIP_CASE_MOVE_U:
		
		if (cursor_menu) { (cursor_menu)--; }
		if (!cursor_menu && menu_offset) { (menu_offset)--; }
		break;
	case 'q': 			// quit game
		
		return 0;
		break;
	/***** START REDUNDANT KEYS *****/
	case 'j':			// move down with 'j'
		
		goto PC_EQUIP_CASE_MOVE_D;
		break;
	case '2':			// move down with '2'
		
		goto PC_EQUIP_CASE_MOVE_D;
		break;
	case 'k':			// move up with 'k'
		
		goto PC_EQUIP_CASE_MOVE_U;
		break;
	case '8':			// move up with '8'
		
		goto PC_EQUIP_CASE_MOVE_U;
		break;
	default:			// no-op do nothing
		
		break;
	}
	
	return 1;
}
int GameState::io_menu_pc_unequip(int key) {
	
	allow_move = 0;
	switch(key) {
		
	case 0x1b: 			// escape key, exit menu
		
		menu_index = 0;
		return 2;
		break;
	case 't':
	
		if (pc.num_items_equipped && (pc.inventory.size()+1 < PC_INVENTORY_MAX)) {
			
			switch(cursor_menu) {
				
			case 1:			//unequip offhand equipment
				
				if (pc.equipment_offhand) {
					
					pc.inventory.push_back(pc.equipment_offhand);
					pc.equipment_offhand = NULL;
					pc.equipped &= 0xFFFD;
					(pc.num_items_equipped)--;
				}
				break;
			case 2:			//unequip ranged equipment
				
				if (pc.equipment_ranged) {
					
					pc.inventory.push_back(pc.equipment_ranged);
					pc.equipment_ranged = NULL;
					pc.equipped &= 0xFFFB;
					(pc.num_items_equipped)--;
				}
				break;
			case 3:			//unequip armor equipment
			
				if (pc.equipment_armor) {
					
					pc.inventory.push_back(pc.equipment_armor);
					pc.equipment_armor = NULL;
					pc.equipped &= 0xFFF7;
					(pc.num_items_equipped)--;
				}
				break;
			case 4:			//unequip helmet equipment
			
				if (pc.equipment_helmet) {
					
					pc.inventory.push_back(pc.equipment_helmet);
					pc.equipment_helmet = NULL;
					pc.equipped &= 0xFFEF;
					(pc.num_items_equipped)--;
				}
				break;
			case 5:			//unequip cloak equipment
			
				if (pc.equipment_cloak) {
					
					pc.inventory.push_back(pc.equipment_cloak);
					pc.equipment_cloak = NULL;
					pc.equipped &= 0xFFDF;
					(pc.num_items_equipped)--;
				}
				break;
			case 6:			//unequip gloves equipment
			
				if (pc.equipment_gloves) {
					
					pc.inventory.push_back(pc.equipment_gloves);
					pc.equipment_gloves = NULL;
					pc.equipped &= 0xFFBF;
					(pc.num_items_equipped)--;
				}
				break;
			case 7:			//unequip boots equipment
			
				if (pc.equipment_boots) {
					
					pc.inventory.push_back(pc.equipment_boots);
					pc.equipment_boots = NULL;
					pc.equipped &= 0xFF7F;
					(pc.num_items_equipped)--;
				}
				break;
			case 8:			//unequip amulet equipment
			
				if (pc.equipment_amulet) {
					
					pc.inventory.push_back(pc.equipment_amulet);
					pc.equipment_amulet = NULL;
					pc.equipped &= 0xFEFF;
					(pc.num_items_equipped)--;
				}
				break;
			case 9:			//unequip light equipment
			
				if (pc.equipment_light) {
					
					pc.inventory.push_back(pc.equipment_light);
					pc.equipment_light = NULL;
					pc.equipped &= 0xFDFF;
					(pc.num_items_equipped)--;
				}
				break;
			case 10:		//unequip ring0 equipment
			
				if (pc.equipment_ring0) {
					
					pc.inventory.push_back(pc.equipment_ring0);
					pc.equipment_ring0 = NULL;
					pc.equipped &= 0xFBFF;
					(pc.num_items_equipped)--;
				}
				break;
			case 11:		//unequip ring1 equipment
			
				if (pc.equipment_ring1) {
					
					pc.inventory.push_back(pc.equipment_ring1);
					pc.equipment_ring1 = NULL;
					pc.equipped &= 0xF7FF;
					(pc.num_items_equipped)--;
				}
				break;
			default:		//unequip weapon equipment
			
				if (pc.equipment_weapon) {
					
					pc.inventory.push_back(pc.equipment_weapon);
					pc.equipment_weapon = NULL;
					pc.equipped &= 0xFFFE;
					(pc.num_items_equipped)--;
				}
				break;
			}
		}
		break;
	case KEY_DOWN:		// down arrow
		PC_UNEQUIP_CASE_MOVE_D:
		
		if (cursor_menu < cursor_menu_max-1) { (cursor_menu)++; }
		if (menu_offset < (dungeons[dungeon_index].cells[pc.location.y][pc.location.x].items.size() - 17) && dungeons[dungeon_index].cells[pc.location.y][pc.location.x].items.size() > 18) { (menu_offset)++; }
		break;
	case KEY_UP:		// up arrow
		PC_UNEQUIP_CASE_MOVE_U:
		
		if (cursor_menu) { (cursor_menu)--; }
		if (!cursor_menu && menu_offset) { (menu_offset)--; }
		break;
	case 'Q': 			// quit game
		
		return 0;
		break;
	/***** START REDUNDANT KEYS *****/
	case 'j':			// move down with 'j'
		
		goto PC_UNEQUIP_CASE_MOVE_D;
		break;
	case '2':			// move down with '2'
		
		goto PC_UNEQUIP_CASE_MOVE_D;
		break;
	case 'k':			// move up with 'k'
		
		goto PC_UNEQUIP_CASE_MOVE_U;
		break;
	case '8':			// move up with '8'
		
		goto PC_UNEQUIP_CASE_MOVE_U;
		break;
	default:			// no-op do nothing
		
		break;
	}
	
	return 1;
}

int GameState::io_menu_map_ntunneling(int key) {
	
	allow_move = 0;
	switch(key) {
		
	case 0x1b: 			// escape key, exit menu
		MAP_NTUNNELING_EXIT:
		
		menu_index = 0;
		return 2;
		break;
	/***** START REDUNDANT KEYS *****/
	case 'D':			// exit menu
		
		goto MAP_NTUNNELING_EXIT;
		break;
	case 's':
		
		goto MAP_NTUNNELING_EXIT;
		break;
	case 'Q': 			// quit game
		
		return 0;
		break;
	default:			// no-op do nothing
		
		break;
	}
	
	return 1;
}
int GameState::io_menu_map_tunneling(int key) {
	
	allow_move = 0;
	switch(key) {
		
	case 0x1b: 			// escape key, exit menu
		MAP_TUNNELING_EXIT:
		
		menu_index = 0;
		return 2;
		break;
	/***** START REDUNDANT KEYS *****/
	case 'T':			// exit menu
		
		goto MAP_TUNNELING_EXIT;
		break;
	case 's':
		
		goto MAP_TUNNELING_EXIT;
		break;
	case 'Q': 			// quit game
		
		return 0;
		break;
	default:			// no-op do nothing
		
		break;
	}
	
	return 1;
}
int GameState::io_menu_map_hardness(int key) {
	
	allow_move = 0;
	switch(key) {
		
	case 0x1b: 			// escape key, exit menu
		MAP_HARDNESS_EXIT:
		
		menu_index = 0;
		return 2;
		break;
	/***** START REDUNDANT KEYS *****/
	case 'H':			// exit menu
		
		goto MAP_HARDNESS_EXIT;
		break;
	case 's':
		
		goto MAP_HARDNESS_EXIT;
		break;
	case 'Q': 			// quit game
		
		return 0;
		break;
	default:			// no-op do nothing
		
		break;
	}
	
	return 1;
}