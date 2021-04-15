/******** include std libs ********/
#include <iostream>

#include <stdlib.h>
#include <stdint.h>

/******* include custom libs ******/
#include "./classdef/item_template.h"

/********** definitions **********/


/*********** global vars **********/


/***** constructor definitions *****/
Item_Template::Item_Template() {
	
	name 			= "";
	description 	= "";
	status 			= 1;
	type 			= 0x00;
	rarity 			= 1;
	color 			= std::vector<uint8_t>();
	value 			= Dice();
	weight 			= Dice();
	attribute 		= Dice();
	bonus_speed 	= Dice();
	bonus_defense 	= Dice();
	bonus_damage 	= Dice();
	bonus_hit 		= Dice();
	bonus_dodge 	= Dice();
}
Item_Template::Item_Template(const Item_Template &rhs) {
	
	name 			= rhs.name;
	description 	= rhs.description;
	status 			= rhs.status;
	type 			= rhs.type;
	rarity 			= rhs.rarity;
	color 			= rhs.color;
	value 			= rhs.value;
	weight 			= rhs.weight;
	attribute 		= rhs.attribute;
	bonus_speed 	= rhs.bonus_speed;
	bonus_defense 	= rhs.bonus_defense;
	bonus_damage 	= rhs.bonus_damage;
	bonus_hit 		= rhs.bonus_hit;
	bonus_dodge 	= rhs.bonus_dodge;
}

/****** function definitions ******/
Item* Item_Template::new_item() {
	
	Item *item = new Item();
	
	item->name.assign(name);
	item->description.assign(description);
	item->symbol = get_symbol(type);
	
	item->status 	= status;
	item->type		= type;
	item->rarity 	= rarity;
	item->color 	= color;
	
	item->value			= value.roll();
	item->weight 		= weight.roll();
	item->attribute 	= attribute.roll();
	item->bonus_speed 	= bonus_speed.roll();
	item->bonus_defense = bonus_defense.roll();
	item->bonus_hit 	= bonus_hit.roll();
	item->bonus_dodge 	= bonus_dodge.roll();
	
	item->bonus_damage = bonus_damage;
	
	return item;
}

char Item_Template::get_symbol(uint32_t item_type) {
	
	if (item_type & ITEM_TYPE_WEAPON) 		{ return '|'; }
	if (item_type & ITEM_TYPE_OFFHAND) 		{ return ')'; }
	if (item_type & ITEM_TYPE_RANGED) 		{ return '}'; }
	if (item_type & ITEM_TYPE_ARMOR)		{ return '['; }
	if (item_type & ITEM_TYPE_HELMET) 		{ return ']'; }
	if (item_type & ITEM_TYPE_CLOAK) 		{ return '('; }
	if (item_type & ITEM_TYPE_GLOVES) 		{ return '{'; }
	if (item_type & ITEM_TYPE_BOOTS) 		{ return '\\'; }
	if (item_type & ITEM_TYPE_RING) 		{ return '='; }
	if (item_type & ITEM_TYPE_AMULET)		{ return '"'; }
	if (item_type & ITEM_TYPE_LIGHT) 		{ return '_'; }
	if (item_type & ITEM_TYPE_SCROLL) 		{ return '~'; }
	if (item_type & ITEM_TYPE_BOOK)			{ return '?'; }
	if (item_type & ITEM_TYPE_FLASK)		{ return '!'; }
	if (item_type & ITEM_TYPE_GOLD)			{ return '$'; }
	if (item_type & ITEM_TYPE_AMMUNITION)	{ return '/'; }
	if (item_type & ITEM_TYPE_FOOD)			{ return ','; }
	if (item_type & ITEM_TYPE_WAND)			{ return '-'; }
	if (item_type & ITEM_TYPE_CONTAINER)	{ return '%'; }
	
	return '*';
}

void Item_Template::print() {
	
	std::cout << name << std::endl;
	
	std::cout << description << std::endl;
	
	if (type > 0) {
		
		if (type & ITEM_TYPE_WEAPON) 		{ std::cout << "WEAPON "; }
		if (type & ITEM_TYPE_OFFHAND) 		{ std::cout << "OFFHAND "; }
		if (type & ITEM_TYPE_RANGED) 		{ std::cout << "RANGED "; }
		if (type & ITEM_TYPE_ARMOR) 		{ std::cout << "ARMOR "; }
		if (type & ITEM_TYPE_HELMET) 		{ std::cout << "HELMET "; }
		if (type & ITEM_TYPE_CLOAK) 		{ std::cout << "CLOAK "; }
		if (type & ITEM_TYPE_GLOVES) 		{ std::cout << "GLOVES "; }
		if (type & ITEM_TYPE_BOOTS) 		{ std::cout << "BOOTS "; }
		if (type & ITEM_TYPE_RING) 			{ std::cout << "RING "; }
		if (type & ITEM_TYPE_AMULET) 		{ std::cout << "AMULET "; }
		if (type & ITEM_TYPE_LIGHT) 		{ std::cout << "LIGHT "; }
		if (type & ITEM_TYPE_SCROLL) 		{ std::cout << "SCROLL "; }
		if (type & ITEM_TYPE_BOOK) 			{ std::cout << "BOOK "; }
		if (type & ITEM_TYPE_FLASK) 		{ std::cout << "FLASK "; }
		if (type & ITEM_TYPE_GOLD) 			{ std::cout << "GOLD "; }
		if (type & ITEM_TYPE_AMMUNITION) 	{ std::cout << "AMMUNITION "; }
		if (type & ITEM_TYPE_FOOD) 			{ std::cout << "FOOD "; }
		if (type & ITEM_TYPE_WAND) 			{ std::cout << "WAND "; }
		if (type & ITEM_TYPE_CONTAINER) 	{ std::cout << "CONTAINER"; }
		std::cout << std::endl;
	} else { std::cout << "INVALID_TYPE"; } //should never happen
	std::cout << std::endl;
	
	if (color.size()) {
		
		std::vector<uint8_t>::iterator color_itr;
		for (color_itr = color.begin(); color_itr != color.end(); color_itr++) {
			switch (*color_itr) {
				
			case PAIR_BLACK:
				
				std::cout << "BLACK ";
				break;
			case PAIR_RED:
				
				std::cout << "RED ";
				break;
			case PAIR_BLUE:
				
				std::cout << "BLUE ";
				break;
			case PAIR_CYAN:
				
				std::cout << "CYAN ";
				break;
			case PAIR_YELLOW:
				
				std::cout << "YELLOW ";
				break;
			case PAIR_MAGENTA:
				
				std::cout << "MAGENTA ";
				break;
			default:
				
				std::cout << "WHITE ";
				break;
			}
		}
	} else { std::cout << "INVALID COLOR"; }
	std::cout << std::endl;
	
	value.print();
	std::cout << std::endl;
	
	weight.print();
	std::cout << std::endl;
	
	attribute.print();
	std::cout << std::endl;
	
	bonus_speed.print();
	std::cout << std::endl;
	
	bonus_defense.print();
	std::cout << std::endl;
	
	bonus_damage.print();
	std::cout << std::endl;
	
	bonus_hit.print();
	std::cout << std::endl;
	
	bonus_dodge.print();
	std::cout << std::endl;
	
	if (status) { std::cout << "TRUE" << std::endl; }
	else 		{ std::cout << "FALSE" << std::endl;}
	std::cout << unsigned(rarity) << std::endl;
	
	return;
}