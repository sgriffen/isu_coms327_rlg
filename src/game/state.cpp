/******** include std libs ********/
#include <string>
#include <sstream>

#include <stdlib.h>
#include <stdint.h>
#include <ncurses.h>
#include <string.h>

/******* include custom libs ******/
#include "./classdef/state.h"
#include "./utils/movement.h"
#include "./utils/pathfinder.h"
#include "../utils/math_utils.h"
#include "../res/color.h"

/********** definitions **********/


/*********** global vars **********/


/***** constructor definitions *****/
GameState::GameState() {
	
	allow_move_pc 			= 0;
	allow_move_npc 			= 0;
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
void GameState::run(RunArgs run_args) {
	
	init_queue();
	allow_move_pc = 1;
	
	dungeon_pc_los(&(dungeons[dungeon_index]), (Character*)(&(pc)), 1);
	
	while (pc.hp > 0 && contains_npcs() && !boss_defeated) {
		
		if (menu_index) {
			
			draw(run_args.print_config.fill, 0);
			if (!parse_input()) { break; }
		}
		else {
			
			if (allow_move_npc) {
				
				pathfinder_ntunneling(&(dungeons[dungeon_index]), &(pc.location));
				pathfinder_tunneling(&(dungeons[dungeon_index]), &(pc.location));
			}
			if (!run_turn(run_args)) { break; }
		}
	}
	
	gameover(run_args.print_config.fill, 0);
	
	return;
}

int GameState::contains_npcs() {
	
	if (num_dungeons_generated < num_dungeons) 					{ return 1; }
	if (dungeon_contains_npcs((&dungeons[(num_dungeons)-1]))) 	{ return 1; }
	
	return 0;
}

void GameState::draw(int print_fill, int print_weight) {
	
	clear();
	
	start_color();
	
	switch (menu_index) {
		
	case 1: 			//draw monster menu	
		
		draw_menu_npc_list();
		break;
	case 2:
		
		draw_menu_pc_warp(print_fill, print_weight);
		break;
	case 3:
		
		draw_menu_npc_target(print_fill, print_weight);
		break;
	case 4:
		
		draw_menu_npc_target_description();
		break;
	case 5:
		
		draw_menu_item_pickup();
		break;
	case 6:
		
		draw_menu_pc_inventory();
		break;
	case 7:
		
		draw_menu_pc_inventory_inspect();
		break;
	case 8:
		
		draw_menu_pc_inventory_inspect_description();
		break;
	case 9:
		
		draw_menu_pc_drop();
		break;
	case 10:
		
		draw_menu_pc_expunge();
		break;
	case 11:
		
		draw_menu_pc_equipment();
		break;
	case 12:
		
		draw_menu_pc_equip();
		break;
	case 13:
		
		draw_menu_pc_unequip();
		break;
	default:			//draw current dungeon
		
		draw_dungeon(print_fill, print_weight);
		break;
	}
	
	refresh();
	
	return;
}

void GameState::init_pc() {
	
	/*
		player character (pc) is placed at a random location in rooms[0] of dungeon[0]
	*/
	
	Coordinate loc;
	loc.y = (uint8_t)utils_rand_between(dungeons[0].rooms[0].tl.y+ROOM_BORDER_WIDTH, dungeons[0].rooms[0].br.y-ROOM_BORDER_WIDTH);
	loc.x = (uint8_t)utils_rand_between(dungeons[0].rooms[0].tl.x+ROOM_BORDER_WIDTH, dungeons[0].rooms[0].br.x-ROOM_BORDER_WIDTH);
	
	std::vector<uint8_t> colors = { PAIR_GREEN };
	pc = PC("Hero", "Our only hope!", '@', colors, loc, 100, Dice(1, 2, 6), PC_SPEED);
	
	return;
}

void GameState::init_queue() {
	
	int i = 0;
	
	characters = (Character**)calloc((dungeons[dungeon_index].num_npcs)+1, sizeof(Character*));
	queue = queue_init((dungeons[dungeon_index].num_npcs)+1);
	queue_position = 0;
	
	/* wrap pc and npc inside Character wrappers for queue and assign cell pointers */
    characters[0] = &(pc);
	dungeons[dungeon_index].cells[characters[0]->location.y][characters[0]->location.x].character = characters[0];
	for (i = 0; i < (dungeons[dungeon_index].num_npcs); i++) {
		
        characters[i+1] = &(dungeons[dungeon_index].npcs[i]);
		dungeons[dungeon_index].cells[characters[i+1]->location.y][characters[i+1]->location.x].character = characters[i+1];
	}
	
    /* organize pc and npcs into priority queue based on movement speed */
    for (i = 0; i < dungeons[dungeon_index].num_npcs; i++) {

        queue_enqueue(&(queue), queue_node_init(characters[i+1], CHARACTER_TURN(characters[i+1]->speed)));
        dungeons[dungeon_index].cells[characters[i+1]->location.y][characters[i+1]->location.x].character = characters[i+1];
    }
    queue_enqueue(&(queue), queue_node_init(characters[0], CHARACTER_TURN(characters[0]->speed)));
    dungeons[dungeon_index].cells[characters[0]->location.y][characters[0]->location.x].character = characters[0];

    queue_position = queue.nodes[0].priority + 1;
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
	
	queue.index = 0;
	queue.size = 0;
	free(queue.nodes);
	free(characters);
}

int GameState::run_turn(RunArgs run_args) {

	int i = 0;
	
	int game_run = 1;
	
	/* draw dungeon with updated npc positions */
	dungeon_pc_los(&(dungeons[dungeon_index]), (Character*)(&(pc)), 1);
	draw(run_args.print_config.fill, 0);
	
	if ((!allow_move_npc && !allow_move_pc)) { game_run = parse_input(); }
	else {
		/* dequeue nodes, stopping at pc node */
		while ((i < ((queue.index) + 1)) && !queue_is_empty(queue) && !boss_defeated) {
			
			QueueNode node = queue_dequeue(&(queue));
			node.priority = queue_position;
			Character *wrapper = (Character*)(node.element);
			if (!(wrapper->id)) {
				
				/* move pc */
				if (allow_move_pc && wrapper->hp > 0) {
					
					game_run = parse_input();
					if (wrapper->prev_speed != (wrapper->speed + ((PC*)wrapper)->buff_speed())) {
						
						wrapper->prev_speed = wrapper->speed;
						wrapper->speed = wrapper->speed + ((PC*)wrapper)->buff_speed();
						node.priority -= (wrapper->speed - wrapper->prev_speed);
					}
					queue_enqueue(&(queue), node);
					
					/* draw dungeon with updated npc positions */
					draw(run_args.print_config.fill, 0);
				}
			} else {
				
				if (!allow_move_npc) { queue_enqueue(&(queue), node); }
				else if (wrapper->hp > 0) {

					run_turn_npc(&(dungeons[dungeon_index]), wrapper);
					queue_enqueue(&(queue), node);
					(turn)++;
				}
			}
			
			(queue_position)++;
			i++;
			
			if (game_run != 1) { break; }
		}
	}
	
	return game_run;
}

int GameState::parse_input() {
	
	int key = getch();
	switch (menu_index) {
		
	case 1:		// in npc menu
		
		return io_menu_npc_list(key);
		break;
	case 2:		// in pc warp menu
		
		return io_menu_pc_warp(key);
		break;
	case 3:		//in npc target menu
		
		return io_menu_npc_target(key);
		break;
	case 4:	// in monster description menu (from case 3)
		
		return io_menu_npc_target_description(key);
		break;
	case 5:		// in item pickup menu
	
		return io_menu_item_pickup(key);
		break;
	case 6:		// in pc inventory menu
	
		return io_menu_pc_inventory(key);
		break;
	case 7:		// in pc inventory inspect menu
	
		return io_menu_pc_inventory_inspect(key);
		break;
	case 8:		// in pc inventory inspect description menu
	
		return io_menu_pc_inventory_inspect_description(key);
		break;
	case 9:		// in item drop menu
	
		return io_menu_pc_drop(key);
		break;
	case 10:	// in item expunge menu
	
		return io_menu_pc_expunge(key);
		break;
	case 11:	// in pc equipment menu
	
		return io_menu_pc_equipment(key);
		break;
	case 12:	// in pc equip menu
	
		return io_menu_pc_equip(key);
		break;
	case 13:	// in pc unequip menu
	
		return io_menu_pc_unequip(key);
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
	
	int *collision = dungeon_resolve_collision(dungeon, (Character*)&pc, next);
	if (collision[0] > 0 && collision[1] > 0) {
		
		std::stringstream stream;
		stream << "PC attacked NPC at (" << std::to_string(next.y) << ", " << std::to_string(next.x) << ") for " << std::to_string(collision[1]);
		message.assign(stream.str());
	}
	if (!collision[0]) {
		if (cell_contains_items(dungeon->cells[next.y][next.x])) {
			
			if (dungeon->cells[next.y][next.x].items.size() > 1) { message.assign("Multiple items found. Press ',' to decide which one to pick up"); }
			else { message.assign("Item found. Press ',' to pickup"); }
		}
	}
	if (collision[2]) { boss_defeated = 1; }
	
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
	
	int *collision = dungeon_resolve_collision(dungeon, wrapper, next);
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
		
		if ((npc->type & NPC_TYPE_DESTROY) && cell_contains_items(dungeon->cells[next.y][next.x])) {
			
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
		else if ((npc->type & NPC_TYPE_PICKUP) && cell_contains_items(dungeon->cells[next.y][next.x])) {
			
			std::vector<Item*>::iterator cell_item_itr;
			for (cell_item_itr = dungeon->cells[next.y][next.x].items.begin(); cell_item_itr != dungeon->cells[next.y][next.x].items.end(); cell_item_itr++) { npc->inventory.push_back(*(cell_item_itr)); }
			
			dungeon->cells[next.y][next.x].items.erase(dungeon->cells[next.y][next.x].items.begin(), dungeon->cells[next.y][next.x].items.end());
		}
	}
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
	
	if (cell_contains_items(*cell)) {
		
		if (cell->items.size() > 1) {
			
			menu_index = 5;
			cursor_menu = 0;
			cursor_menu_max = cell->items.size();
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

void GameState::draw_dungeon(int print_fill, int print_weight) {
	
	if (message.size() > 0) { mvprintw(0, 0, "%s", message.c_str()); }
	else 					{ mvprintw(0, 0, "griffens -- hw 1.9", turn); }
	
	dungeons[dungeon_index].draw(1, 0, fog_enabled, print_fill, print_weight);
	
	mvprintw(22, 0, "PC location: (%d, %d) :: PC HP: [%d]", pc.location.y, pc.location.x, pc.hp);
	mvprintw(23, 0, "Turn [%d] :: [%d] NPCs left in level [%d] of [%d]", turn, (dungeons[dungeon_index].num_npcs - dungeons[dungeon_index].num_npcs_dead), (dungeon_index+1), num_dungeons);
	
	return;
}

void GameState::draw_menu_npc_list() {
	
	int i = 0;
	int j = menu_offset;
	int scrollable_offset = 5;
	
	mvprintw(0, 0, "MENU - LEVEL NPC LIST");
	mvprintw(1, 0, "________________________________________________________________________________");
	mvprintw(2, 0, "[%d] NPCs left in current dungeon level [%d]:", dungeons[dungeon_index].num_npcs - dungeons[dungeon_index].num_npcs_dead, (dungeon_index + 1));
	mvprintw(3, 0, "  id :: type :: speed :: distance from player   ");
	mvprintw(4, 0, "------------------------------------------------");
	
	for (i = scrollable_offset; i < 22 && j < dungeons[dungeon_index].num_npcs; i++, j++) {
		
		uint32_t id = dungeons[dungeon_index].npcs[j].id;
		uint8_t type = dungeons[dungeon_index].npcs[j].type;
		uint8_t speed = dungeons[dungeon_index].npcs[j].speed;
		int y_pos = dungeons[dungeon_index].npcs[j].location.y - pc.location.y;
		int x_pos = dungeons[dungeon_index].npcs[j].location.x - pc.location.x;
		
		if (dungeons[dungeon_index].npcs[j].hp > 0) {
			
			if (y_pos > 0) {
				
				if (x_pos > 0) 	{ mvprintw(i, 0, "%4d :: %4x :: %5d :: %4d South, %4d East", id, type, speed, abs(y_pos), abs(x_pos)); }
				else 			{ mvprintw(i, 0, "%4d :: %4x :: %5d :: %4d South, %4d West", id, type, speed, abs(y_pos), abs(x_pos)); }
			}
			else {
				
				if (x_pos > 0) 	{ mvprintw(i, 0, "%4d :: %4x :: %5d :: %4d North, %4d East", id, type, speed, abs(y_pos), abs(x_pos)); }
				else 			{ mvprintw(i, 0, "%4d :: %4x :: %5d :: %4d North, %4d West", id, type, speed, abs(y_pos), abs(x_pos)); }
			}
		} else { i--; }
	}
	
	mvprintw(22, 0, "KEY_UP to scroll up, KEY_DOWN to scroll down");
	mvprintw(23, 0, "ESC to exit menu");
	
	return;
}
void GameState::draw_menu_pc_warp(int print_fill, int print_weight) {
	
	mvprintw(0, 0, "MENU - PC WARP MODE", turn);	
	
	dungeons[dungeon_index].draw(1, 0, 0, print_fill, print_weight);
	
	attron(COLOR_PAIR(PAIR_BLUE));
	attron(A_BOLD);
	mvaddch((cursor_map.y)+1, cursor_map.x, '*');
	attroff(A_BOLD);
	attroff(COLOR_PAIR(PAIR_BLUE));
	
	mvprintw(22, 0, "Warping to location (%d, %d)", cursor_map.y, cursor_map.x);
	mvprintw(23, 0, "'g' to warp to cursor_map; 'r' to warp to random location; Move input to move cursor; ESC to exit");
	
	return;
}
void GameState::draw_menu_npc_target(int print_fill, int print_weight) {
	
	mvprintw(0, 0, "MENU - CHARACTER LOOKUP", turn);
	
	dungeons[dungeon_index].draw(1, 0, fog_enabled, print_fill, print_weight);
	
	attron(COLOR_PAIR(PAIR_RED));
	attron(A_BOLD);
	mvaddch((cursor_map.y)+1, cursor_map.x, '*');
	attroff(A_BOLD);
	attroff(COLOR_PAIR(PAIR_RED));
	
	mvprintw(22, 0, "Lookup character at location (%d, %d)", cursor_map.y, cursor_map.x);
	mvprintw(23, 0, "'L' to lookup character at cursor; Move input to move cursor; ESC to exit");
	
	return;
}
void GameState::draw_menu_npc_target_description() {
	
	int i = 0;
	
	mvprintw(i++, 0, "SUB-MENU -- CHARACTER DESCRIPTION", turn);
	mvprintw(i++, 0, "________________________________________________________________________________");
	
	Character* c = dungeons[dungeon_index].cells[cursor_map.y][cursor_map.x].character;
	if (!c) { mvprintw(2, 0, "No character found at location (%d, %d)", cursor_map.y, cursor_map.x); }
	else {
	
		mvprintw(i++, 0, "Name: %s", c->name.c_str());
		mvprintw(i++, 0, "Description: ");
		mvprintw(i++, 0, "%s", c->description.c_str());
//		mvprintw(i++, 0, "Symbol: %c", c->symbol);
//		mvprintw(i++, 0, "HP: %d", c->hp);
//		mvprintw(i++, 0, "Speed: %d", c->speed);
//		mvprintw(i++, 0, "Damage roll: %s", c->damage.to_string().c_str());
	}
	
	mvprintw(22, 0, "Character description at (%d, %d)", cursor_map.y, cursor_map.x);
	mvprintw(23, 0, "ESC to go back to character lookup mode");
}
void GameState::draw_menu_item_pickup() {
	
	int i = 0;
	int j = menu_offset;
	int scrollable_offset = 5;
	
	std::vector<Item*> items = dungeons[dungeon_index].cells[pc.location.y][pc.location.x].items;
	
	mvprintw(0, 0, "MENU - ITEM PICKUP");
	mvprintw(1, 0, "________________________________________________________________________________");
	mvprintw(2, 0, "[%d] Items in cell (%d, %d)", items.size(), pc.location.y, pc.location.x);
	mvprintw(3, 0, "   type  :: name");
	mvprintw(4, 0, "--------------");
	
	for (i = scrollable_offset; i < 22 && j < (int)items.size(); i++, j++) {
		
		uint32_t type = items[j]->type;
		const char *name = items[j]->name.c_str();
		
		if (cursor_menu == i - scrollable_offset) { mvaddch(i, 0, '*'); }
		mvprintw(i, 2, "%6x :: %s", type, name);
	}
	
	mvprintw(22, 0, "KEY_UP to scroll up; KEY_DOWN to scroll down; ',' to pickup selected item");
	mvprintw(23, 0, "ESC to exit menu");
	
	return;
}
void GameState::draw_menu_pc_inventory() {
	
	int i = 0;
	int j = menu_offset;
	int scrollable_offset = 5;
	
	mvprintw(0, 0, "MENU - INVENTORY");
	mvprintw(1, 0, "________________________________________________________________________________");
	mvprintw(2, 0, "[%d] Items in inventory :: [%d] Items equipped", pc.inventory.size(), pc.num_items_equipped);
	mvprintw(3, 0, "slot --  type  :: name");
	mvprintw(4, 0, "--------------------");
	
	for (i = scrollable_offset; i < 22 && j < (int)pc.inventory.size(); i++, j++) {
		
		uint32_t type =  pc.inventory[j]->type;
		const char *name = pc.inventory[j]->name.c_str();
		
		mvprintw(i, 0, "%4d -- %6x :: %s", j, type, name);
	}
	
	mvprintw(22, 0, "KEY_UP to scroll up; KEY_DOWN to scroll down");
	mvprintw(23, 0, "ESC to exit menu");
	
	return;
}
void GameState::draw_menu_pc_inventory_inspect() {
	
	int i = 0;
	int j = menu_offset;
	int scrollable_offset = 5;
	
	mvprintw(0, 0, "MENU - ITEM LOOKUP");
	mvprintw(1, 0, "________________________________________________________________________________");
	mvprintw(2, 0, "[%d] Items in inventory", pc.inventory.size());
	mvprintw(3, 0, "   type  :: name");
	mvprintw(4, 0, "--------------");
	
	for (i = scrollable_offset; i < 22 && j < (int)pc.inventory.size(); i++, j++) {
		
		uint32_t type = pc.inventory[j]->type;
		const char *name = pc.inventory[j]->name.c_str();
		
		if (cursor_menu == i - scrollable_offset) { mvaddch(i, 0, '*'); }
		mvprintw(i, 2, "%6x :: %s", type, name);
	}
	
	mvprintw(22, 0, "KEY_UP to scroll up; KEY_DOWN to scroll down; 'I' to see selected item's attributes");
	mvprintw(23, 0, "ESC to exit menu");
	
	return;
}
void GameState::draw_menu_pc_inventory_inspect_description() {
	
	int i = 0;
	
	Item *item = pc.inventory[cursor_menu];
	
	mvprintw(i++, 0, "SUB-MENU -- ITEM DESCRIPTION");
	mvprintw(i++, 0, "________________________________________________________________________________");
	
	mvprintw(i++, 0, "Name: %s", item->name.c_str());
	mvprintw(i++, 0, "Description: ");
	mvprintw(i++, 0, "%s", item->description.c_str());
//	mvprintw(i++, 0, "Symbol: %c", item->symbol);
//	mvprintw(i++, 0, "Weight: %d", item->weight);
//	mvprintw(i++, 0, "Value: %d", item->value);
//	mvprintw(i++, 0, "Attribute: %d", item->attribute);
//	if (item->bonus_damage.num_dice) { mvprintw(i++, 0, "Damage roll: %s", item->bonus_damage.to_string().c_str()); }
//	if (item->bonus_speed) { mvprintw(i++, 0, "Bonus speed: %d", item->bonus_speed); }
//	if (item->bonus_defense) { mvprintw(i++, 0, "Bonus defense: %d", item->bonus_defense); }
//	if (item->bonus_hit) { mvprintw(i++, 0, "Bonus hit: %d", item->bonus_hit); }
//	if (item->bonus_dodge) { mvprintw(i++, 0, "Bonus dodge: %d", item->bonus_dodge); }
	
	mvprintw(22, 0, "Item description in PC inventory slot [%d]", cursor_menu);
	mvprintw(23, 0, "ESC to select another item attribute");
	
	return;
}
void GameState::draw_menu_pc_drop() {
	
	int i = 0;
	int j = menu_offset;
	int scrollable_offset = 5;
	
	mvprintw(0, 0, "MENU - ITEM DROP");
	mvprintw(1, 0, "________________________________________________________________________________");
	mvprintw(2, 0, "[%d] Items in inventory", pc.inventory.size());
	mvprintw(3, 0, "   type  :: name");
	mvprintw(4, 0, "--------------");
	
	for (i = scrollable_offset; i < 22 && j < (int)pc.inventory.size(); i++, j++) {
		
		uint32_t type = pc.inventory[j]->type;
		const char *name = pc.inventory[j]->name.c_str();
		
		if (cursor_menu == i - scrollable_offset) { mvaddch(i, 0, '*'); }
		mvprintw(i, 2, "%6x :: %s", type, name);
	}
	
	mvprintw(22, 0, "KEY_UP to scroll up; KEY_DOWN to scroll down; 'd' to drop selected Item from inventory");
	mvprintw(23, 0, "ESC to exit menu");
	
	return;
}
void GameState::draw_menu_pc_expunge() {
	
	int i = 0;
	int j = menu_offset;
	int scrollable_offset = 5;
	
	mvprintw(0, 0, "MENU - ITEM EXPUNGE");
	mvprintw(1, 0, "________________________________________________________________________________");
	mvprintw(2, 0, "[%d] Items in inventory", pc.inventory.size());
	mvprintw(3, 0, " type  :: name");
	mvprintw(4, 0, "------------");
	
	for (i = scrollable_offset; i < 22 && j < (int)pc.inventory.size(); i++, j++) {
		
		uint32_t type = pc.inventory[j]->type;
		const char *name = pc.inventory[j]->name.c_str();
		
		if (cursor_menu == i - scrollable_offset) { mvaddch(i, 0, '*'); }
		mvprintw(i, 2, "%6x :: %s", type, name);
	}
	
	mvprintw(22, 0, "KEY_UP to scroll up; KEY_DOWN to scroll down; 'x' to remove selected Item from game");
	mvprintw(23, 0, "ESC to exit menu");
	
	return;
}
void GameState::draw_menu_pc_equipment() {
	
	int i = 0;
	int j = menu_offset;
	int scrollable_offset = 5;
	
	std::vector<char> item_labels = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l' };
	std::vector<Item*> items = { pc.equipment_weapon, pc.equipment_offhand, pc.equipment_ranged, pc.equipment_armor, pc.equipment_helmet, pc.equipment_cloak, pc.equipment_gloves, pc.equipment_boots, pc.equipment_amulet, pc.equipment_light, pc.equipment_ring0, pc.equipment_ring1 };
	
	mvprintw(0, 0, "MENU - EQUIPMENT LIST");
	mvprintw(1, 0, "________________________________________________________________________________");
	mvprintw(2, 0, "[%d] Items equipped", pc.num_items_equipped);
	mvprintw(3, 0, "label ::  type  :: name");
	mvprintw(4, 0, "---------------------");
	
	for (i = scrollable_offset; i < 22 && j < (int)items.size(); i++, j++) {
		
		char label = item_labels[j];
		if (items[j]) {
			
			uint32_t type = items[j]->type;
			const char *name = items[j]->name.c_str();
			
			mvprintw(i, 0, "%5c :: %6x :: %s", label, type, name);
		} else { mvprintw(i, 0, "%5c :: empty equipment slot", label); }
	}
	
	mvprintw(22, 0, "KEY_UP to scroll up; KEY_DOWN to scroll down");
	mvprintw(23, 0, "ESC to exit menu");
	
	return;
}
void GameState::draw_menu_pc_equip() {
	
	int i = 0;
	int j = menu_offset;
	int scrollable_offset = 5;
	
	mvprintw(0, 0, "MENU - ITEM EQUIP");
	mvprintw(1, 0, "________________________________________________________________________________");
	mvprintw(2, 0, "[%d] Items in inventory :: [%d] Items equipped", pc.inventory.size(), pc.num_items_equipped);
	mvprintw(3, 0, "   type  :: name");
	mvprintw(4, 0, "--------------");
	
	for (i = scrollable_offset; i < 22 && j < (int)pc.inventory.size(); i++, j++) {
		
		uint32_t type = pc.inventory[j]->type;
		const char *name = pc.inventory[j]->name.c_str();
		
		if (cursor_menu == i - scrollable_offset) { mvaddch(i, 0, '*'); }
		mvprintw(i, 2, "%6x :: %s", type, name);
	}
	
	mvprintw(22, 0, "KEY_UP to scroll up; KEY_DOWN to scroll down; 'w' or 'W' to equip selected Item.");
	mvprintw(23, 0, "ESC to exit menu");
	
	return;
}
void GameState::draw_menu_pc_unequip() {
	
	int i = 0;
	int j = menu_offset;
	int scrollable_offset = 5;
	
	std::vector<char> item_labels = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l' };
	std::vector<Item*> items = { pc.equipment_weapon, pc.equipment_offhand, pc.equipment_ranged, pc.equipment_armor, pc.equipment_helmet, pc.equipment_cloak, pc.equipment_gloves, pc.equipment_boots, pc.equipment_amulet, pc.equipment_light, pc.equipment_ring0, pc.equipment_ring1 };
	
	mvprintw(0, 0, "MENU - ITEM UNEQUIP");
	mvprintw(1, 0, "________________________________________________________________________________");
	mvprintw(2, 0, "[%d] Items equipped :: [%d] Items in inventory", pc.num_items_equipped, pc.inventory.size());
	mvprintw(3, 0, "  label ::  type  :: name");
	mvprintw(4, 0, "-----------------------");
	
	for (i = scrollable_offset; i < 22 && j < (int)items.size(); i++, j++) {
		
		char label = item_labels[j];
		if (cursor_menu == i - scrollable_offset) { mvaddch(i, 0, '*'); }
		if (items[j]) {
			
			uint32_t type = items[j]->type;
			const char *name = items[j]->name.c_str();
			
			mvprintw(i, 2, "%5c :: %6x :: %s", label, type, name);
		} else { mvprintw(i, 2, "%5c :: empty equipment slot", label); }
	}
	
	mvprintw(22, 0, "KEY_UP to scroll up; KEY_DOWN to scroll down; 't' to unequip selected Item from equipment");
	mvprintw(23, 0, "ESC to exit menu");
	
	return;
}

void GameState::draw_gameover_win() {
	
	int key = 0;
	
	clear();
	
	mvprintw(0, 0, "WINNER!");
	
	mvprintw(12, 0, "...this page needs some work... :/");
	
	mvprintw(22, 0, "PC successfully eradicated [%d] monsters from the underworld on turn [%d]! :D", pc.num_kills, turn);
	mvprintw(23, 0, "Press ESC key to exit");

	refresh();
	
	key = getch();
	while (key != 0x1b) { key = getch(); }

	return;
}
void GameState::draw_gameover_lose(int print_fill, int print_weight) {
	
	int key = 0;
	
	clear();
	
	mvprintw(0, 0, "GAME OVER");
	
	dungeons[dungeon_index].draw(1, 0, 0, print_fill, print_weight);
	
	mvprintw(22, 0, "PC was defeated on turn [%d] having slain [%d] monsters in vain ... :(", turn, pc.num_kills);
	mvprintw(23, 0, "Press ESC key to exit");
	
	refresh();
	
	key = getch();
	while (key != 0x1b) { key = getch(); }
	
	return;
}
void GameState::draw_gameover_unexpected() {
	
	int key = 0;
	
	clear();
	
	mvprintw(22, 0, "Game exited in unknown state on turn [%d] :|", turn);
	mvprintw(23, 0, "Press ESC key to exit");
	
	refresh();
	
	key = getch();
	while (key != 0x1b) { key = getch(); }
	
	return;
}