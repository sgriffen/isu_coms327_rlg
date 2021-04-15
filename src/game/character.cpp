/******** include std libs ********/
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

/******* include custom libs ******/
#include "./classdef/character.h"
#include "./utils/movement.h"

/*********** global vars **********/


/***** constructor definitions *****/
Character::Character() {
	
	id 			= 0;
	name 		= "";
	description = "";
	symbol 		= '\0';
	
	speed 		= 0;
	prev_speed	= 0;
	hp 			= 1;
	damage 		= Dice();
	color 		= std::vector<uint8_t>();
	inventory	= std::vector<Item*>();
}
Character::Character(const Character &rhs) {
	
	id 			= rhs.id;
	name 		= rhs.name;
	description = rhs.description;
	symbol 		= rhs.symbol;
	
	speed 		= rhs.speed;
	prev_speed 	= rhs.prev_speed;
	hp 			= rhs.hp;
	damage 		= rhs.damage;
	color 		= rhs.color;
	inventory	= rhs.inventory;
}
Character::Character(uint32_t character_id, Coordinate character_loc, uint16_t character_hp, Dice character_damage, uint8_t character_speed) : Character() {
	
	location		= Coordinate(character_loc);
	prev_location 	= Coordinate(character_loc);
	id 				= character_id;
	symbol 			= '\0';
	
	speed 			= character_speed;
	prev_speed 		= character_speed;
	hp 				= character_hp;
	damage 			= character_damage;
}
Character::Character(std::string character_name, std::string character_description, char character_symbol, std::vector<uint8_t> character_color, uint32_t character_id, Coordinate character_loc, uint16_t character_hp, Dice character_damage, uint8_t character_speed) : Character(character_id, character_loc, character_hp, character_damage, character_speed) {
	
	name.assign(character_name);
	description.assign(character_description);
	
	symbol 		= character_symbol;
	color 		= character_color;
}

PC::PC() : Character() {
	
	num_kills = 0;
	num_items_equipped	= 0;
	equipment_weapon 	= NULL;
	equipment_offhand 	= NULL;
	equipment_ranged 	= NULL;
	equipment_armor 	= NULL;
	equipment_helmet 	= NULL;
	equipment_cloak 	= NULL;
	equipment_gloves 	= NULL;
	equipment_boots 	= NULL;
	equipment_amulet 	= NULL;
	equipment_light		= NULL;
	equipment_ring0 	= NULL;
	equipment_ring1 	= NULL;
}
PC::PC(const PC &rhs) : Character(rhs) {
	
	num_kills 			= rhs.num_kills;
	num_items_equipped	= rhs.num_items_equipped;
	equipment_weapon 	= rhs.equipment_weapon;
	equipment_offhand 	= rhs.equipment_offhand;
	equipment_ranged 	= rhs.equipment_weapon;
	equipment_armor 	= rhs.equipment_armor;
	equipment_helmet 	= rhs.equipment_helmet;
	equipment_cloak 	= rhs.equipment_cloak;
	equipment_gloves 	= rhs.equipment_gloves;
	equipment_boots 	= rhs.equipment_boots;
	equipment_amulet 	= rhs.equipment_amulet;
	equipment_light		= rhs.equipment_light;
	equipment_ring0 	= rhs.equipment_ring0;
	equipment_ring1 	= rhs.equipment_ring1;
}
PC::PC(Coordinate pc_loc, uint16_t pc_hp, Dice pc_damage, uint8_t pc_speed) : Character(0, pc_loc, pc_hp, pc_damage, pc_speed) { 
	
	num_kills = 0;
	num_items_equipped	= 0;
	equipment_weapon 	= NULL;
	equipment_offhand 	= NULL;
	equipment_ranged 	= NULL;
	equipment_armor 	= NULL;
	equipment_helmet 	= NULL;
	equipment_cloak 	= NULL;
	equipment_gloves 	= NULL;
	equipment_boots 	= NULL;
	equipment_amulet 	= NULL;
	equipment_light		= NULL;
	equipment_ring0 	= NULL;
	equipment_ring1 	= NULL;
}
PC::PC(std::string pc_name, std::string pc_description, char pc_symbol, std::vector<uint8_t> pc_color, Coordinate pc_loc, uint16_t pc_hp, Dice pc_damage, uint8_t pc_speed) : Character(pc_name, pc_description, pc_symbol, pc_color, 0, pc_loc, pc_hp, pc_damage, pc_speed) {
	
	num_kills = 0;
	num_items_equipped	= 0;
	equipment_weapon 	= NULL;
	equipment_offhand 	= NULL;
	equipment_ranged 	= NULL;
	equipment_armor 	= NULL;
	equipment_helmet 	= NULL;
	equipment_cloak 	= NULL;
	equipment_gloves 	= NULL;
	equipment_boots 	= NULL;
	equipment_amulet 	= NULL;
	equipment_light		= NULL;
	equipment_ring0 	= NULL;
	equipment_ring1 	= NULL;
}

NPC::NPC() : Character() {
	
	type 	= 0x00;
	rarity 	= 1;
}
NPC::NPC(const NPC &rhs) : Character(rhs) {
	
	type = rhs.type;
	rarity = rhs.rarity;
}
NPC::NPC(uint32_t npc_id, Coordinate npc_loc, uint16_t npc_hp, uint16_t npc_type, Dice npc_damage, uint8_t npc_speed) : Character(npc_id, npc_loc, npc_hp, npc_damage, npc_speed) { 
	
	type 	= npc_type;
	rarity 	= 1;
}
NPC::NPC(uint8_t npc_rarity, std::string npc_name, std::string npc_description, char npc_symbol, std::vector<uint8_t> npc_color, uint32_t npc_id, Coordinate npc_loc, uint16_t npc_hp, uint16_t npc_type, Dice npc_damage, uint8_t npc_speed) : Character(npc_name, npc_description, npc_symbol, npc_color, npc_id, npc_loc, npc_hp, npc_damage, npc_speed) {
	
	type 	= npc_type;
	rarity 	= npc_rarity;
}

/****** function definitions ******/
void Character::draw(uint8_t print_y, uint8_t print_x, int print_fill) {
	
	attron(COLOR_PAIR(color[0]));
	
	if (print_fill) { mvprintw(print_y, print_x, "%2c", symbol); } 
	else 			{ mvaddch(print_y, print_x, symbol); }
	
	attroff(COLOR_PAIR(color[0]));
	
	return;
}

int PC::buff_damage() {
	
	int buff = 0;
	
	(equipment_weapon) 	? (buff += equipment_weapon->bonus_damage.roll()) 	: (buff += 0);
	(equipment_offhand) ? (buff += equipment_offhand->bonus_damage.roll())	: (buff += 0);
	(equipment_ranged) 	? (buff += equipment_ranged->bonus_damage.roll()) 	: (buff += 0);
	(equipment_armor) 	? (buff += equipment_armor->bonus_damage.roll()) 	: (buff += 0);
	(equipment_helmet) 	? (buff += equipment_helmet->bonus_damage.roll()) 	: (buff += 0);
	(equipment_cloak) 	? (buff += equipment_cloak->bonus_damage.roll()) 	: (buff += 0);
	(equipment_gloves) 	? (buff += equipment_gloves->bonus_damage.roll()) 	: (buff += 0);
	(equipment_boots) 	? (buff += equipment_boots->bonus_damage.roll()) 	: (buff += 0);
	(equipment_amulet) 	? (buff += equipment_amulet->bonus_damage.roll()) 	: (buff += 0);
	(equipment_ring0) 	? (buff += equipment_ring0->bonus_damage.roll()) 	: (buff += 0);
	(equipment_ring1) 	? (buff += equipment_ring1->bonus_damage.roll()) 	: (buff += 0);
	
	return buff;
}
int PC::buff_defense() {
	
	int buff = 0;
	
	(equipment_weapon) 	? (buff += equipment_weapon->bonus_defense) 	: (buff += 0);
	(equipment_offhand) ? (buff += equipment_offhand->bonus_defense) 	: (buff += 0);
	(equipment_ranged) 	? (buff += equipment_ranged->bonus_defense) 	: (buff += 0);
	(equipment_armor) 	? (buff += equipment_armor->bonus_defense) 		: (buff += 0);
	(equipment_helmet) 	? (buff += equipment_helmet->bonus_defense) 	: (buff += 0);
	(equipment_cloak) 	? (buff += equipment_cloak->bonus_defense) 		: (buff += 0);
	(equipment_gloves) 	? (buff += equipment_gloves->bonus_defense) 	: (buff += 0);
	(equipment_boots) 	? (buff += equipment_boots->bonus_defense) 		: (buff += 0);
	(equipment_amulet) 	? (buff += equipment_amulet->bonus_defense) 	: (buff += 0);
	(equipment_ring0) 	? (buff += equipment_ring0->bonus_defense) 		: (buff += 0);
	(equipment_ring1) 	? (buff += equipment_ring1->bonus_defense) 		: (buff += 0);
	
	return buff;
}
int PC::buff_speed() {
	
	int buff = 0;
	
	(equipment_weapon) 	? (buff += equipment_weapon->bonus_speed) 	: (buff += 0);
	(equipment_offhand) ? (buff += equipment_offhand->bonus_speed) 	: (buff += 0);
	(equipment_ranged) 	? (buff += equipment_ranged->bonus_speed) 	: (buff += 0);
	(equipment_armor) 	? (buff += equipment_armor->bonus_speed) 	: (buff += 0);
	(equipment_helmet) 	? (buff += equipment_cloak->bonus_speed) 	: (buff += 0);
	(equipment_gloves) 	? (buff += equipment_gloves->bonus_speed) 	: (buff += 0);
	(equipment_boots) 	? (buff += equipment_boots->bonus_speed) 	: (buff += 0);
	(equipment_amulet) 	? (buff += equipment_amulet->bonus_speed) 	: (buff += 0);
	(equipment_ring0) 	? (buff += equipment_ring0->bonus_speed) 	: (buff += 0);
	(equipment_ring1) 	? (buff += equipment_ring1->bonus_speed) 	: (buff += 0);
	
	return buff;
}
int PC::buff_light() {
	
	int buff = 0;
	
	(equipment_light)	? (buff += equipment_light->value) : (buff += 0);
	
	return buff;
}