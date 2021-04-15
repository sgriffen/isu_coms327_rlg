#include <iostream>
#include <string>
#include <vector>

#include <stdint.h>
#include <type_traits>
#include <ncurses.h>

#ifndef CHARACTER_H
#define CHARACTER_H

#include "./coordinate.h"
#include "./item.h"
#include "../utils/classdef/dice.h"
#include "../../res/color.h"

/*********** definitions **********/
#define PC_SPEED 10
#define PC_INVENTORY_MAX 10
#define PC_EQUIPMENT_MAX 12

/************* macros *************/
#define CHARACTER_TURN(speed) (1000/speed)

/******* enums definitions ********/


/******** class definitions *******/
class Character {
	public:
		
		uint32_t id;
		std::string name;
		std::string description;
		char symbol;
		
		std::vector<Item*> inventory;
		
		Coordinate location;
		Coordinate prev_location;
		
		uint32_t speed;
		uint32_t prev_speed;
		int32_t hp;
		std::vector<uint8_t> color;
		
		Dice damage;
		
		/* CONSTRUCTORS */
		Character();
		Character(const Character &rhs);
		Character(uint32_t character_id, Coordinate character_loc, uint16_t character_hp, Dice character_damage, uint8_t character_speed);
		Character(std::string character_name, std::string character_description, char character_symbol, std::vector<uint8_t> character_color, uint32_t character_id, Coordinate character_loc, uint16_t character_hp, Dice character_damage, uint8_t character_speed);

		/* FUNCTIONS */
		void draw(uint8_t print_y, uint8_t print_x, int print_fill);
};

class PC : public Character {
	public:
		Character* last_seen;
		
		uint8_t num_items_equipped;
		Item* equipment_weapon;
		Item* equipment_offhand;
		Item* equipment_ranged;
		Item* equipment_armor;
		Item* equipment_helmet;
		Item* equipment_cloak;
		Item* equipment_gloves;
		Item* equipment_boots;
		Item* equipment_amulet;
		Item* equipment_light;
		Item* equipment_ring0;
		Item* equipment_ring1;
		
		uint32_t num_kills;
		
		/* CONSTRUCTORS */
		PC();
		PC(const PC &rhs);
		PC(Coordinate pc_loc, uint16_t pc_hp, Dice pc_damage, uint8_t pc_speed);
		PC(std::string pc_name, std::string pc_description, char pc_symbol, std::vector<uint8_t> pc_color, Coordinate pc_loc, uint16_t pc_hp, Dice pc_damage, uint8_t pc_speed);

		/* FUNCTIONS */
		int buff_damage();
		int buff_defense();
		int buff_speed();
		int buff_light();
};

class NPC : public Character {
	public: 
		uint16_t type;
		uint8_t rarity;
		
		Coordinate pc_last_known;
		
		/* CONSTRUCTORS */
		NPC();
		NPC(const NPC &rhs);
		NPC(uint32_t npc_id, Coordinate npc_loc, uint16_t npc_hp, uint16_t npc_type, Dice npc_damage, uint8_t npc_speed);
		NPC(uint8_t npc_rarity, std::string npc_name, std::string npc_description, char npc_symbol, std::vector<uint8_t> npc_color, uint32_t npc_id, Coordinate npc_loc, uint16_t npc_hp, uint16_t npc_type, Dice npc_damage, uint8_t npc_speed);

		/* FUNCTIONS */
		
};

/******* function definitions *****/


#endif /* CHARACTER_H */