#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <type_traits>

#include <stdint.h>
#include <ncurses.h>

#include "./coordinate.h"
#include "../utils/classdef/dice.h"
#include "../../config/color.h"

/********** definitions **********/
#define ITEM_NUM_PARAM			14
#define ITEM_TYPE_WEAPON 			0x00000001
#define ITEM_TYPE_WEAPON_SHIFT 		0
#define ITEM_TYPE_OFFHAND 			0x00000002
#define ITEM_TYPE_OFFHAND_SHIFT 	1
#define ITEM_TYPE_RANGED 			0x00000004
#define ITEM_TYPE_RANGED_SHIFT 		2
#define ITEM_TYPE_ARMOR 			0x00000008
#define ITEM_TYPE_ARMOR_SHIFT 		3
#define ITEM_TYPE_HELMET 			0x00000010
#define ITEM_TYPE_HELMET_SHIFT 		4
#define ITEM_TYPE_CLOAK 			0x00000020
#define ITEM_TYPE_CLOAK_SHIFT 		5
#define ITEM_TYPE_GLOVES 			0x00000040
#define ITEM_TYPE_GLOVES_SHIFT 		6
#define ITEM_TYPE_BOOTS 			0x00000080
#define ITEM_TYPE_BOOTS_SHIFT 		7
#define ITEM_TYPE_AMULET 			0x00000100
#define ITEM_TYPE_AMULET_SHIFT 		8
#define ITEM_TYPE_LIGHT 			0x00000200
#define ITEM_TYPE_LIGHT_SHIFT 		9
#define ITEM_TYPE_RING 				0x00000400
#define ITEM_TYPE_RING_SHIFT 		10
#define ITEM_TYPE_SCROLL 			0x00000800
#define ITEM_TYPE_SCROLL_SHIFT 		11
#define ITEM_TYPE_BOOK 				0x00001000
#define ITEM_TYPE_BOOK_SHIFT 		12
#define ITEM_TYPE_FLASK 			0x00002000
#define ITEM_TYPE_FLASK_SHIFT 		13
#define ITEM_TYPE_GOLD 				0x00004000
#define ITEM_TYPE_GOLD_SHIFT 		14
#define ITEM_TYPE_AMMUNITION 		0x00008000
#define ITEM_TYPE_AMMUNITION_SHIFT 	15
#define ITEM_TYPE_FOOD 				0x00010000
#define ITEM_TYPE_FOOD_SHIFT 		16
#define ITEM_TYPE_WAND 				0x00020000
#define ITEM_TYPE_WAND_SHIFT 		17
#define ITEM_TYPE_CONTAINER 		0x00040000
#define ITEM_TYPE_CONTAINER_SHIFT 	18


/************* macros *************/


/******* enums declarations *******/


/******* class declarations ******/
class Item;

class Item_Template;

class Item {
	public:
		
		uint32_t id;
		std::string name;
		std::string description;
		char symbol;
		
		Coordinate location;
		
		uint8_t status;
		uint32_t type;
		uint8_t rarity;
		std::vector<uint8_t> color;
		
		uint32_t value;
		uint32_t weight;
		uint32_t attribute;
		uint32_t bonus_speed;
		uint32_t bonus_defense;
		uint32_t bonus_hit;
		uint32_t bonus_dodge;
		
		Dice bonus_damage;
		
		/* CONSTRUCTORS */
		Item();
		Item(const Item &rhs);
		
		/* FUNCTIONS */
		void clean();
		
		void disk_load(std::ifstream& file, std::vector<Item_Template> item_templates);
		void disk_save(std::ofstream& file);
		
		void draw(uint8_t print_y, uint8_t print_x, int print_fill);
};

class Item_Template {
	public:
		std::string name;
		std::string description;
		
		uint8_t status;
		uint32_t type;
		uint8_t rarity;
		std::vector<uint8_t> color;
		
		Dice value;
		Dice weight;
		Dice attribute;
		Dice bonus_speed;
		Dice bonus_defense;
		Dice bonus_damage;
		Dice bonus_hit;
		Dice bonus_dodge;
		
		/* CONSTRUCTORS */
		Item_Template();
		Item_Template(const Item_Template &rhs);
		
		
		/* FUNCTIONS */
		Item* new_item();
		
		char get_symbol(uint32_t item_type);
		
		void print();
};

/****** function definitions *****/


#endif /* ITEM_H */