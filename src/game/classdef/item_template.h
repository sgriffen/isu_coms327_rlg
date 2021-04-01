#include <string>

#include <stdint.h>

#ifndef ITEM_TEMPLATE_H
#define ITEM_TEMPLATE_H

#include "../../res/color.h"
#include "../utils/classdef/dice.h"

/********** definitions **********/
#define ITEM_NUM_PARAM			14
#define ITEM_TYPE_WEAPON 		0b0000000000000000001
#define ITEM_TYPE_OFFHAND 		0b0000000000000000010
#define ITEM_TYPE_RANGED 		0b0000000000000000100
#define ITEM_TYPE_ARMOR 		0b0000000000000001000
#define ITEM_TYPE_HELMET 		0b0000000000000010000
#define ITEM_TYPE_CLOAK 		0b0000000000000100000
#define ITEM_TYPE_GLOVES 		0b0000000000001000000
#define ITEM_TYPE_BOOTS 		0b0000000000010000000
#define ITEM_TYPE_RING 			0b0000000000100000000
#define ITEM_TYPE_AMULET 		0b0000000001000000000
#define ITEM_TYPE_LIGHT 		0b0000000010000000000
#define ITEM_TYPE_SCROLL 		0b0000000100000000000
#define ITEM_TYPE_BOOK 			0b0000001000000000000
#define ITEM_TYPE_FLASK 		0b0000010000000000000
#define ITEM_TYPE_GOLD 			0b0000100000000000000
#define ITEM_TYPE_AMMUNITION 	0b0001000000000000000
#define ITEM_TYPE_FOOD 			0b0010000000000000000
#define ITEM_TYPE_WAND 			0b0100000000000000000
#define ITEM_TYPE_CONTAINER 	0b1000000000000000000


/************* macros *************/


/******* enums declarations *******/


/******* class declarations ******/
class Item_Template {
	public:
		std::string name;
		std::string description;
		
		uint8_t status;
		uint32_t type;
		uint8_t rarity;
		uint8_t color;
		
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
		void print();
};

#endif /* ITEM_TEMPLATE_H */