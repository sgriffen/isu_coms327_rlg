/******** include std libs ********/
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

/******* include custom libs ******/
#include "./classdef/character.h"
#include "./utils/movement.h"
#include "../res/color.h"

/*********** global vars **********/


/***** constructor definitions *****/
Character::Character() {
	
	id 			= 0;
	name 		= "";
	description = "";
	symbol 		= '\0';
	
	speed 		= 0;
	hp 			= 1;
	damage 		= 1;
	color 		= PAIR_WHITE;
}
Character::Character(uint32_t character_id, Coordinate character_loc, uint16_t character_hp, uint16_t character_damage, uint8_t character_speed) : Character() {
	
	location		= character_loc;
	prev_location 	= character_loc;
	id 				= character_id;
	symbol 			= '\0';
	
	speed 			= character_speed;
	hp 				= character_hp;
	damage 			= character_damage;
	color 			= PAIR_WHITE;
}
Character::Character(std::string character_name, std::string character_description, char character_symbol, uint8_t character_color, uint32_t character_id, Coordinate character_loc, uint16_t character_hp, uint16_t character_damage, uint8_t character_speed) : Character(character_id, character_loc, character_hp, character_damage, character_speed) {
	
	name.assign(character_name);
	description.assign(character_description);
	
	symbol 		= character_symbol;
	color 		= character_color;
}

PC::PC() : Character() {
	
	num_kills = 0;
}
PC::PC(Coordinate pc_loc, uint16_t pc_hp, uint16_t pc_damage, uint8_t pc_speed) : Character(0, pc_loc, pc_hp, pc_damage, pc_speed) { 
	
	num_kills = 0;
}
PC::PC(std::string pc_name, std::string pc_description, char pc_symbol, uint8_t pc_color, Coordinate pc_loc, uint16_t pc_hp, uint16_t pc_damage, uint8_t pc_speed) : Character(pc_name, pc_description, pc_symbol, pc_color, 0, pc_loc, pc_hp, pc_damage, pc_speed) {
	
	num_kills = 0;
}


NPC::NPC() : Character() {
	
	type 	= 0;
	rarity 	= 0;
}
NPC::NPC(uint32_t npc_id, Coordinate npc_loc, uint16_t npc_hp, uint16_t npc_type, uint16_t npc_damage, uint8_t npc_speed) : Character(npc_id, npc_loc, npc_hp, npc_damage, npc_speed) { 
	
	type 	= npc_type;
	rarity 	= 0;
}
NPC::NPC(uint8_t npc_rarity, std::string npc_name, std::string npc_description, char npc_symbol, uint8_t npc_color, uint32_t npc_id, Coordinate npc_loc, uint16_t npc_hp, uint16_t npc_type, uint16_t npc_damage, uint8_t npc_speed) : Character(npc_name, npc_description, npc_symbol, npc_color, npc_id, npc_loc, npc_hp, npc_damage, npc_speed) {
	
	type 	= npc_type;
	rarity 	= npc_rarity;
}

/****** function definitions ******/
void Character::draw(uint8_t print_y, uint8_t print_x, int print_fill) {
	
	attron(COLOR_PAIR(color));
	
	if (print_fill) { mvprintw(print_y, print_x, "%2c", symbol); } 
	else 			{ mvaddch(print_y, print_x, symbol); }
	
	attroff(COLOR_PAIR(color));
	
	return;
}