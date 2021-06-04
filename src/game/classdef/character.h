#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <type_traits>

#include <stdint.h>
#include <ncurses.h>

#include "./coordinate.h"
#include "./item.h"
#include "../utils/classdef/dice.h"
#include "../../config/color.h"

/*********** definitions **********/
#define PC_SPEED 10
#define PC_INVENTORY_MAX 10
#define PC_EQUIPMENT_MAX 12

#define NPC_NUM_PARAM			9
#define NPC_TYPE_INTELLIGENT 		0x001
#define NPC_TYPE_INTELLIGENT_SHIFT 	0
#define NPC_TYPE_TELEPATHIC 		0x002
#define NPC_TYPE_TELEPATHIC_SHIFT 	1
#define NPC_TYPE_TUNNELING 			0x004
#define NPC_TYPE_TUNNELING_SHIFT 	2
#define NPC_TYPE_ERRATIC 			0x008
#define NPC_TYPE_ERRATIC_SHIFT 		3
#define NPC_TYPE_PASS				0x010
#define NPC_TYPE_PASS_SHIFT			4
#define NPC_TYPE_PICKUP				0x020
#define NPC_TYPE_PICKUP_SHIFT		5
#define NPC_TYPE_DESTROY			0x040
#define NPC_TYPE_DESTROY_SHIFT		6
#define NPC_TYPE_UNIQUE				0x080
#define NPC_TYPE_UNIQUE_SHIFT		7
#define NPC_TYPE_BOSS				0x200
#define NPC_TYPE_BOSS_SHIFT			8

#define NPC_SPEED_MIN			1
#define NPC_SPEED_MAX			20

/************* macros *************/
#define CHARACTER_TURN(speed) (1000/speed)

/******* enums definitions ********/


/******** class definitions *******/
class Character;
class PC;
class NPC;

class NPC_Template;

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
		Character(uint32_t character_id, Coordinate character_loc, uint16_t character_hp, Dice character_damage, uint8_t character_speed);
		Character(std::string character_name, std::string character_description, char character_symbol, std::vector<uint8_t> character_color, uint32_t character_id, Coordinate character_loc, uint16_t character_hp, Dice character_damage, uint8_t character_speed);
		
		Character(const Character &rhs);

		/* FUNCTIONS */
		void clean();
		
		void disk_load(std::ifstream& file, std::vector<Item_Template> item_templates, std::vector<Item*> *items_loaded);
		void disk_save(std::ofstream& file);
		
		void draw(uint8_t print_y, uint8_t print_x, int print_fill);
};
class PC : public Character {
	public:
		Character* last_seen;
		
		uint8_t num_items_equipped;
		uint16_t equipped;
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
		PC(Coordinate pc_loc, uint16_t pc_hp, Dice pc_damage, uint8_t pc_speed);
		PC(std::string pc_name, std::string pc_description, char pc_symbol, std::vector<uint8_t> pc_color, Coordinate pc_loc, uint16_t pc_hp, Dice pc_damage, uint8_t pc_speed);
		
		PC(const PC &rhs);

		/* FUNCTIONS */
		int buff_damage();
		int buff_defense();
		int buff_speed();
		int buff_light();
		
		void disk_load(std::ifstream& file, std::vector<Item_Template> item_templates, std::vector<Item*> *items_loaded);
		void disk_save(std::ofstream& file);
};
class NPC : public Character {
	public: 
		uint16_t type;
		uint8_t rarity;
		
		Coordinate pc_last_known;
		
		/* CONSTRUCTORS */
		NPC();
		NPC(uint32_t npc_id, Coordinate npc_loc, uint16_t npc_hp, uint16_t npc_type, Dice npc_damage, uint8_t npc_speed);
		NPC(uint8_t npc_rarity, std::string npc_name, std::string npc_description, char npc_symbol, std::vector<uint8_t> npc_color, uint32_t npc_id, Coordinate npc_loc, uint16_t npc_hp, uint16_t npc_type, Dice npc_damage, uint8_t npc_speed);
		
		NPC(const NPC &rhs);
		
		/* FUNCTIONS */
		void disk_load(std::ifstream& file, std::vector<NPC_Template> npc_templates, std::vector<Item_Template> item_templates, std::vector<Item*> *items_loaded);
		void disk_save(std::ofstream& file);
};

class NPC_Template {
	public:
		std::string name;
		std::string description;
		char symbol;
		
		uint16_t type;
		uint8_t rarity;
		uint8_t num_colors;
		std::vector<uint8_t> color;
		
		Dice speed;
		Dice hp;
		Dice damage;
		
		/* CONSTRUCTORS */
		NPC_Template();
		NPC_Template(const NPC_Template &rhs);
		
		
		/* FUNCTIONS */
		NPC* new_npc();
		
		void print();
};

/******* function definitions *****/


#endif /* CHARACTER_H */