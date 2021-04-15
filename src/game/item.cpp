/******** include std libs ********/
#include <stdlib.h>
#include <stdint.h>

/******* include custom libs ******/
#include "./classdef/item.h"

/********** definitions **********/


/*********** global vars **********/


/***** constructor definitions *****/
Item::Item() {
	
	id				= 0;
	name 			= "";
	description 	= "";
	symbol			= '\0';
	status 			= 1;
	type 			= 0x0000;
	rarity 			= 1;
	value 			= 0;
	weight 			= 0;
	attribute 		= 0;
	bonus_speed 	= 0;
	bonus_defense 	= 0;
	bonus_hit 		= 0;
	bonus_dodge 	= 0;
	bonus_damage 	= Dice();
	color 			= std::vector<uint8_t>();
}
Item::Item(const Item &rhs) {
	
	id				= rhs.id;
	name 			= rhs.name;
	description 	= rhs.description;
	symbol			= rhs.symbol;
	location		= rhs.location;
	status 			= rhs.status;
	type 			= rhs.type;
	rarity 			= rhs.rarity;
	value 			= rhs.value;
	weight 			= rhs.weight;
	attribute 		= rhs.attribute;
	bonus_speed 	= rhs.bonus_speed;
	bonus_defense 	= rhs.bonus_defense;
	bonus_hit 		= rhs.bonus_hit;
	bonus_dodge 	= rhs.bonus_dodge;
	bonus_damage 	= rhs.bonus_damage;
	color 			= rhs.color;
}

/****** function definitions ******/
void Item::draw(uint8_t print_y, uint8_t print_x, int print_fill) {
	
	attron(COLOR_PAIR(color[0]));
	
	if (print_fill) { mvprintw(print_y, print_x, "%2c", symbol); }
	else 			{ mvaddch(print_y, print_x, symbol); }
	
	attroff(COLOR_PAIR(color[0]));
	
	return;
}