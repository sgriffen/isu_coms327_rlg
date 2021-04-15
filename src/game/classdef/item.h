#include <iostream>
#include <string>
#include <vector>

#include <stdint.h>
#include <type_traits>
#include <ncurses.h>

#ifndef ITEM_H
#define ITEM_H

#include "./coordinate.h"
#include "../utils/classdef/dice.h"
#include "../../res/color.h"

/********** definitions ***********/


/************* macros *************/


/******* enums definitions ********/


/******* class definitions ********/
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
		void draw(uint8_t print_y, uint8_t print_x, int print_fill);
};

/****** function definitions *****/


#endif /* ITEM_H */