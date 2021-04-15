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
	
	allow_move_npc = 1;
	allow_move_pc = 1;
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
		
		allow_move_npc = 0;
		if (((dungeon_index - 1) > -1) && dungeons[dungeon_index].cells[pc.location.y][pc.location.x].type_current == CellType_Stair_up) {
			
			decrease_dungeon();
			(turn)++;
		}
		break;
	case '>':			// pc go down staircase
		
		allow_move_npc = 0;
		if (((dungeon_index + 1) < num_dungeons) && dungeons[dungeon_index].cells[pc.location.y][pc.location.x].type_current == CellType_Stair_down) {
			
			increase_dungeon();
			(turn)++;
		}
		break;
	case 'f':			// enable/disable fog
		
		allow_move_npc = 0;
		allow_move_pc = 0;
		fog_enabled = !(fog_enabled);
		break;
	case 'm':			// enter npc menu
		
		allow_move_npc = 0;
		allow_move_pc = 0;
		menu_index = 1;
		return 2;
		break;
	case 'g':			// enter pc warping menu
		
		allow_move_npc = 0;
		allow_move_pc = 0;
		cursor_map = pc.location;
		menu_index = 2;
		return 2;
		break;
	case 'L':			// enter npc target menu
		
		allow_move_npc = 0;
		allow_move_pc = 0;
		cursor_map = pc.location;
		menu_index = 3;
		return 2;
		break;
	case ',':			// pickup item or enter item pickup menu
		
		allow_move_npc = 0;
		allow_move_pc = 0;
		return item_pickup(&(dungeons[dungeon_index].cells[pc.location.y][pc.location.x]));
		break;
	case 'i':			// enter pc inventory menu
		
		allow_move_npc = 0;
		allow_move_pc = 0;
		menu_index = 6;
		return 2;
		break;
	case 'I':			// enter pc item inspect menu
		
		allow_move_npc = 0;
		allow_move_pc = 0;
		menu_index = 7;
		cursor_menu = 0;
		cursor_menu_max = pc.inventory.size();
		return 2;
		break;
	case 'd':			// enter pc drop item menu
		
		allow_move_npc = 0;
		allow_move_pc = 0;
		menu_index = 9;
		cursor_menu = 0;
		cursor_menu_max = pc.inventory.size();
		return 2;
		break;
	case 'x':			// enter pc expunge item menu
		
		allow_move_npc = 0;
		allow_move_pc = 0;
		menu_index = 10;
		cursor_menu = 0;
		cursor_menu_max = pc.inventory.size();
		return 2;
		break;
	case 'e':			// enter pc equipment menu
		
		allow_move_npc = 0;
		allow_move_pc = 0;
		menu_index = 11;
		return 2;
		break;
	case 'w':			// enter pc equip item menu
			
		allow_move_npc = 0;
		allow_move_pc = 0;
		menu_index = 12;
		cursor_menu = 0;
		cursor_menu_max = pc.inventory.size();
		break;
	case 't':			// enter pc unequip item menu
		
		allow_move_npc = 0;
		allow_move_pc = 0;
		menu_index = 13;
		cursor_menu = 0;
		cursor_menu_max = PC_EQUIPMENT_MAX;
		return 2;
		break;
	case 'Q': 			// quit game
		
		allow_move_npc = 0;
		allow_move_pc = 0;
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
		
		allow_move_npc = 0;
		allow_move_pc = 0;
		message.assign("Unknown key pressed");
		break; 	//no-op entered, do nothing
	}
	return 1;
}
int GameState::io_menu_npc_list(int key) {
	
	allow_move_pc = 0;
	allow_move_npc = 0;
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
	
	int *collision;
	
	allow_move_pc = 0;
	allow_move_npc = 0;
	switch(key) {
	
	case 0x1b: 			// escape key
		
		menu_index = 0;
		return 2;
		break;
	case 'g':
	
		next = cursor_map;
		
		collision = dungeon_resolve_collision(&(dungeons[dungeon_index]), &(pc), next, 1);
		if (!collision[0]) {
			if (cell_contains_items(dungeons[dungeon_index].cells[next.y][next.x])) {
				
				if (dungeons[dungeon_index].cells[next.y][next.x].items.size() > 1) { message.assign("Multiple items found. Press ',' to decide which one to pick up"); }
				else { message.assign("Item found. Press ',' to pickup"); }
			}
		}
		if (collision[2]) { boss_defeated = 1; }
		
		menu_index = 0;
		allow_move_pc = 0;
		break;
	case 'r':
		
		next.y = (uint8_t)utils_rand_between(DUNGEON_BORDER_WIDTH, DUNGEON_HEIGHT-DUNGEON_BORDER_WIDTH, NULL);
		next.x = (uint8_t)utils_rand_between(DUNGEON_BORDER_WIDTH, DUNGEON_WIDTH-DUNGEON_BORDER_WIDTH, NULL);
		
		if (dungeon_resolve_collision(&(dungeons[dungeon_index]), &(pc), next)) { (pc.num_kills)++; }
		
		menu_index = 0;
		allow_move_pc = 0;
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
	
	allow_move_pc = 0;
	allow_move_npc = 0;
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
	
	allow_move_pc = 0;
	allow_move_npc = 0;
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
	
	allow_move_pc = 0;
	allow_move_npc = 0;
	switch(key) {
	
	case 0x1b: 			// escape key
		
		menu_index = 0;
		menu_offset = 0;
		return 2;
		break;
	case ',':			// pickup selected item
		
		if (pc.inventory.size() < PC_INVENTORY_MAX) {
			
			Cell *cell = &(dungeons[dungeon_index].cells[pc.location.y][pc.location.x]);
			if (cell_contains_items(*cell)) {
				
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
	
	allow_move_pc = 0;
	allow_move_npc = 0;
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
	
	allow_move_pc = 0;
	allow_move_npc = 0;
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
	
	allow_move_pc = 0;
	allow_move_npc = 0;
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
	
	allow_move_pc = 0;
	allow_move_npc = 0;
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
	
	allow_move_pc = 0;
	allow_move_npc = 0;
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
	
	allow_move_pc = 0;
	allow_move_npc = 0;
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
	
	allow_move_pc = 0;
	allow_move_npc = 0;
	switch(tolower(key)) {
		
	case 0x1b: 			// escape key, exit menu
		
		menu_index = 0;
		return 2;
		break;
	case 'w':
		
		if (!pc.inventory.empty()) {
			
			if ((pc.inventory[cursor_menu]->type & ITEM_TYPE_WEAPON)) {
				if (pc.equipment_weapon) {
					
					Item* temp = pc.equipment_weapon;
					pc.equipment_weapon = pc.inventory[cursor_menu];
					pc.inventory[cursor_menu] = temp;
				} else {
					
					pc.equipment_weapon = pc.inventory[cursor_menu];
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					(pc.num_items_equipped)++;
					
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
				}
			}
			else if ((pc.inventory[cursor_menu]->type & ITEM_TYPE_OFFHAND)) {
				
				if (pc.equipment_offhand) {
					
					Item* temp = pc.equipment_offhand;
					pc.equipment_offhand = pc.inventory[cursor_menu];
					pc.inventory[cursor_menu] = temp;
				} else {
					
					pc.equipment_offhand = pc.inventory[cursor_menu];
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					(pc.num_items_equipped)++;
					
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
				}
			}
			else if ((pc.inventory[cursor_menu]->type & ITEM_TYPE_RANGED)) {
				
				if (pc.equipment_ranged) {
					
					Item* temp = pc.equipment_ranged;
					pc.equipment_ranged = pc.inventory[cursor_menu];
					pc.inventory[cursor_menu] = temp;
				} else {
					
					pc.equipment_ranged = pc.inventory[cursor_menu];
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					(pc.num_items_equipped)++;
					
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
				}
			}
			else if ((pc.inventory[cursor_menu]->type & ITEM_TYPE_ARMOR)) {
				
				if (pc.equipment_armor) {
					
					Item* temp = pc.equipment_armor;
					pc.equipment_armor = pc.inventory[cursor_menu];
					pc.inventory[cursor_menu] = temp;
				} else {
					
					pc.equipment_armor = pc.inventory[cursor_menu];
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					(pc.num_items_equipped)++;
					
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
				}
			}
			else if ((pc.inventory[cursor_menu]->type & ITEM_TYPE_HELMET)) {
				
				if (pc.equipment_helmet) {
					
					Item* temp = pc.equipment_helmet;
					pc.equipment_helmet = pc.inventory[cursor_menu];
					pc.inventory[cursor_menu] = temp;
				} else {
					
					pc.equipment_helmet = pc.inventory[cursor_menu];
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					(pc.num_items_equipped)++;
					
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
				}
			}
			else if ((pc.inventory[cursor_menu]->type & ITEM_TYPE_CLOAK)) {
				
				if (pc.equipment_cloak) {
					
					Item* temp = pc.equipment_cloak;
					pc.equipment_cloak = pc.inventory[cursor_menu];
					pc.inventory[cursor_menu] = temp;
				} else {
					
					pc.equipment_cloak = pc.inventory[cursor_menu];
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					(pc.num_items_equipped)++;
					
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
				}
			}
			else if ((pc.inventory[cursor_menu]->type & ITEM_TYPE_GLOVES)) {
				
				if (pc.equipment_gloves) {
					
					Item* temp = pc.equipment_gloves;
					pc.equipment_gloves = pc.inventory[cursor_menu];
					pc.inventory[cursor_menu] = temp;
				} else {
					
					pc.equipment_gloves = pc.inventory[cursor_menu];
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					(pc.num_items_equipped)++;
					
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
				}
			}
			else if ((pc.inventory[cursor_menu]->type & ITEM_TYPE_BOOTS)) {
				
				if (pc.equipment_boots) {
					
					Item* temp = pc.equipment_boots;
					pc.equipment_boots = pc.inventory[cursor_menu];
					pc.inventory[cursor_menu] = temp;
				} else {
					
					pc.equipment_boots = pc.inventory[cursor_menu];
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					(pc.num_items_equipped)++;
					
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
				}
			}
			else if ((pc.inventory[cursor_menu]->type & ITEM_TYPE_AMULET)) {
				
				if (pc.equipment_amulet) {
					
					Item* temp = pc.equipment_amulet;
					pc.equipment_amulet = pc.inventory[cursor_menu];
					pc.inventory[cursor_menu] = temp;
				} else {
					
					pc.equipment_amulet = pc.inventory[cursor_menu];
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					(pc.num_items_equipped)++;
					
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
				}
			}
			else if ((pc.inventory[cursor_menu]->type & ITEM_TYPE_LIGHT)) {
				
				if (pc.equipment_light) {
					
					Item* temp = pc.equipment_light;
					pc.equipment_light = pc.inventory[cursor_menu];
					pc.inventory[cursor_menu] = temp;
				} else {
					
					pc.equipment_light = pc.inventory[cursor_menu];
					pc.inventory.erase(pc.inventory.begin() + cursor_menu);
					(pc.num_items_equipped)++;
					
					cursor_menu_max = pc.inventory.size();
					if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
				}
			}
			else if ((pc.inventory[cursor_menu]->type & ITEM_TYPE_RING)) {
				
				
				if (key == 'w') {
					
					if (pc.equipment_ring0) {
						
						Item* temp = pc.equipment_ring0;
						pc.equipment_ring0 = pc.inventory[cursor_menu];
						pc.inventory[cursor_menu] = temp;
					} else {
						
						pc.equipment_ring0 = pc.inventory[cursor_menu];
						pc.inventory.erase(pc.inventory.begin() + cursor_menu);
						(pc.num_items_equipped)++;
						
						cursor_menu_max = pc.inventory.size();
						if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
					}
				}
				else if (key == 'W') {
					
					if (pc.equipment_ring1) {
						
						Item* temp = pc.equipment_ring1;
						pc.equipment_ring1 = pc.inventory[cursor_menu];
						pc.inventory[cursor_menu] = temp;
					} else {
						
						pc.equipment_ring1 = pc.inventory[cursor_menu];
						pc.inventory.erase(pc.inventory.begin() + cursor_menu);
						(pc.num_items_equipped)++;
						
						cursor_menu_max = pc.inventory.size();
						if (cursor_menu > cursor_menu_max-1) { cursor_menu = cursor_menu_max-1; }
					}
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
	
	allow_move_pc = 0;
	allow_move_npc = 0;
	switch(key) {
		
	case 0x1b: 			// escape key, exit menu
		
		menu_index = 0;
		return 2;
		break;
	case 't':
	
		if (pc.num_items_equipped && pc.inventory.size()+1 < PC_INVENTORY_MAX) {
			
			switch(cursor_menu) {
				
			case 1:			//unequip offhand equipment
				
				if (pc.equipment_offhand) {
					
					pc.inventory.push_back(pc.equipment_offhand);
					pc.equipment_offhand = NULL;
					(pc.num_items_equipped)--;
				}
				break;
			case 2:			//unequip ranged equipment
				
				if (pc.equipment_ranged) {
					
					pc.inventory.push_back(pc.equipment_ranged);
					pc.equipment_ranged = NULL;
					(pc.num_items_equipped)--;
				}
				break;
			case 3:			//unequip armor equipment
			
				if (pc.equipment_armor) {
					
					pc.inventory.push_back(pc.equipment_armor);
					pc.equipment_armor = NULL;
					(pc.num_items_equipped)--;
				}
				break;
			case 4:			//unequip helmet equipment
			
				if (pc.equipment_helmet) {
					
					pc.inventory.push_back(pc.equipment_helmet);
					pc.equipment_helmet = NULL;
					(pc.num_items_equipped)--;
				}
				break;
			case 5:			//unequip cloak equipment
			
				if (pc.equipment_cloak) {
					
					pc.inventory.push_back(pc.equipment_cloak);
					pc.equipment_cloak = NULL;
					(pc.num_items_equipped)--;
				}
				break;
			case 6:			//unequip gloves equipment
			
				if (pc.equipment_gloves) {
					
					pc.inventory.push_back(pc.equipment_gloves);
					pc.equipment_gloves = NULL;
					(pc.num_items_equipped)--;
				}
				break;
			case 7:			//unequip boots equipment
			
				if (pc.equipment_boots) {
					
					pc.inventory.push_back(pc.equipment_boots);
					pc.equipment_boots = NULL;
					(pc.num_items_equipped)--;
				}
				break;
			case 8:			//unequip amulet equipment
			
				if (pc.equipment_amulet) {
					
					pc.inventory.push_back(pc.equipment_amulet);
					pc.equipment_amulet = NULL;
					(pc.num_items_equipped)--;
				}
				break;
			case 9:			//unequip light equipment
			
				if (pc.equipment_light) {
					
					pc.inventory.push_back(pc.equipment_light);
					pc.equipment_light = NULL;
					(pc.num_items_equipped)--;
				}
				break;
			case 10:		//unequip ring0 equipment
			
				if (pc.equipment_ring0) {
					
					pc.inventory.push_back(pc.equipment_ring0);
					pc.equipment_ring0 = NULL;
					(pc.num_items_equipped)--;
				}
				break;
			case 11:		//unequip ring1 equipment
			
				if (pc.equipment_ring1) {
					
					pc.inventory.push_back(pc.equipment_ring1);
					pc.equipment_ring1 = NULL;
					(pc.num_items_equipped)--;
				}
				break;
			default:		//unequip weapon equipment
			
				if (pc.equipment_weapon) {
					
					pc.inventory.push_back(pc.equipment_weapon);
					pc.equipment_weapon = NULL;
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