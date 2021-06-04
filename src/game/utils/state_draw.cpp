/******** include std libs ********/

/******* include custom libs ******/
#include "../classdef/state.h"

/********** definitions **********/


/*********** global vars **********/


/****** function definitions ******/
void GameState::draw_dungeon_default(int print_fill) {
	
	if (message.size() > 0) { mvprintw(0, 0, "%s", message.c_str()); }
	else 					{ mvprintw(0, 0, "griffens -- assignment 1.10", allow_move, queue_turn.size); }
	
	dungeons[dungeon_index].draw(1, 0, fog_enabled, print_fill, 0);
	
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
void GameState::draw_menu_pc_warp(int print_fill) {
	
	mvprintw(0, 0, "MENU - PC WARP MODE", turn);	
	
	dungeons[dungeon_index].draw(1, 0, 0, print_fill, 0);
	
	attron(COLOR_PAIR(PAIR_BLUE));
	attron(A_BOLD);
	mvaddch((cursor_map.y)+1, cursor_map.x, '*');
	attroff(A_BOLD);
	attroff(COLOR_PAIR(PAIR_BLUE));
	
	mvprintw(22, 0, "Warping to location (%d, %d)", cursor_map.y, cursor_map.x);
	mvprintw(23, 0, "'g' to warp to cursor_map; 'r' to warp to random location; Move input to move cursor; ESC to exit");
	
	return;
}
void GameState::draw_menu_npc_target(int print_fill) {
	
	mvprintw(0, 0, "MENU - CHARACTER LOOKUP", turn);
	
	dungeons[dungeon_index].draw(1, 0, fog_enabled, print_fill, 0);
	
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
	
	mvprintw(i++, 0, "SUB-MENU (1) -- CHARACTER DESCRIPTION", turn);
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
	
	mvprintw(i++, 0, "SUB-MENU (1) -- ITEM DESCRIPTION");
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
		} else { mvprintw(i, 0, "%5c :: ", label); }
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
		} else { mvprintw(i, 2, "%5c :: ", label); }
	}
	
	mvprintw(22, 0, "KEY_UP to scroll up; KEY_DOWN to scroll down; 't' to unequip selected Item from equipment");
	mvprintw(23, 0, "ESC to exit menu");
	
	return;
}
void GameState::draw_dungeon_ntunneling(int print_fill) {
	
	if (message.size() > 0) { mvprintw(0, 0, "%s", message.c_str()); }
	else 					{ mvprintw(0, 0, "DUNGEON - CELL NON-TUNNELING COSTS", allow_move, queue_turn.size); }
	
	dungeons[dungeon_index].draw(1, 0, 0, print_fill, 2);
	
	mvprintw(22, 0, "'s' or 'D' to return to game");
	mvprintw(23, 0, "ESC to exit menu");
	
	return;
}
void GameState::draw_dungeon_tunneling(int print_fill) {
	
	if (message.size() > 0) { mvprintw(0, 0, "%s", message.c_str()); }
	else 					{ mvprintw(0, 0, "DUNGEON - CELL TUNNELING COSTS", allow_move, queue_turn.size); }
	
	dungeons[dungeon_index].draw(1, 0, 0, print_fill, 3);
	
	mvprintw(22, 0, "'s' or 'T' to return to game");
	mvprintw(23, 0, "ESC to exit menu");
	
	return;
}
void GameState::draw_dungeon_hardness(int print_fill) {
	
	if (message.size() > 0) { mvprintw(0, 0, "%s", message.c_str()); }
	else 					{ mvprintw(0, 0, "DUNGEON - CELL HARDNESS VALUES", allow_move, queue_turn.size); }
	
	dungeons[dungeon_index].draw(1, 0, 0, print_fill, 1);
	
	mvprintw(25, 0, "'s' or 'H' to return to game");
	mvprintw(26, 0, "ESC to exit menu");
	
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