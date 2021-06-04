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
void Item::clean() {
	
	color.clear();
	
	return;
}

void Item::draw(uint8_t print_y, uint8_t print_x, int print_fill) {
	
	attron(COLOR_PAIR(color[0]));
	
	if (print_fill) { mvprintw(print_y, print_x, "%2c", symbol); }
	else 			{ mvaddch(print_y, print_x, symbol); }
	
	attroff(COLOR_PAIR(color[0]));
	
	return;
}


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
	
	if ((type >> ITEM_TYPE_WEAPON_SHIFT) & 1) 		{ return '|'; }
	if ((type >> ITEM_TYPE_OFFHAND_SHIFT) & 1) 		{ return ')'; }
	if ((type >> ITEM_TYPE_RANGED_SHIFT) & 1) 		{ return '}'; }
	if ((type >> ITEM_TYPE_ARMOR_SHIFT) & 1) 		{ return '['; }
	if ((type >> ITEM_TYPE_HELMET_SHIFT) & 1) 		{ return ']'; }
	if ((type >> ITEM_TYPE_CLOAK_SHIFT) & 1) 		{ return '('; }
	if ((type >> ITEM_TYPE_GLOVES_SHIFT) & 1) 		{ return '{'; }
	if ((type >> ITEM_TYPE_BOOTS_SHIFT) & 1) 		{ return '\\'; }
	if ((type >> ITEM_TYPE_RING_SHIFT) & 1) 		{ return '='; }
	if ((type >> ITEM_TYPE_AMULET_SHIFT) & 1) 		{ return '"'; }
	if ((type >> ITEM_TYPE_LIGHT_SHIFT) & 1) 		{ return '_'; }
	if ((type >> ITEM_TYPE_SCROLL_SHIFT) & 1) 		{ return '~'; }
	if ((type >> ITEM_TYPE_BOOK_SHIFT) & 1) 		{ return '~'; }
	if ((type >> ITEM_TYPE_FLASK_SHIFT) & 1) 		{ return '?'; }
	if ((type >> ITEM_TYPE_GOLD_SHIFT) & 1) 		{ return '$';}
	if ((type >> ITEM_TYPE_AMMUNITION_SHIFT) & 1) 	{ return '/'; }
	if ((type >> ITEM_TYPE_FOOD_SHIFT) & 1) 		{ return ','; }
	if ((type >> ITEM_TYPE_WAND_SHIFT) & 1) 		{ return '-'; }
	if ((type >> ITEM_TYPE_CONTAINER_SHIFT) & 1) 	{ return '%'; }
	
	return '*';
}

void Item_Template::print() {
	
	std::cout << name << std::endl;
	
	std::cout << description << std::endl;
	
	if (type > 0) {
		
		if ((type >> ITEM_TYPE_WEAPON_SHIFT) & 1) 		{ std::cout << "WEAPON "; }
		if ((type >> ITEM_TYPE_OFFHAND_SHIFT) & 1) 		{ std::cout << "OFFHAND "; }
		if ((type >> ITEM_TYPE_RANGED_SHIFT) & 1) 		{ std::cout << "RANGED "; }
		if ((type >> ITEM_TYPE_ARMOR_SHIFT) & 1) 		{ std::cout << "ARMOR "; }
		if ((type >> ITEM_TYPE_HELMET_SHIFT) & 1) 		{ std::cout << "HELMET "; }
		if ((type >> ITEM_TYPE_CLOAK_SHIFT) & 1) 		{ std::cout << "CLOAK "; }
		if ((type >> ITEM_TYPE_GLOVES_SHIFT) & 1) 		{ std::cout << "GLOVES "; }
		if ((type >> ITEM_TYPE_BOOTS_SHIFT) & 1) 		{ std::cout << "BOOTS "; }
		if ((type >> ITEM_TYPE_RING_SHIFT) & 1) 		{ std::cout << "RING "; }
		if ((type >> ITEM_TYPE_AMULET_SHIFT) & 1) 		{ std::cout << "AMULET "; }
		if ((type >> ITEM_TYPE_LIGHT_SHIFT) & 1) 		{ std::cout << "LIGHT "; }
		if ((type >> ITEM_TYPE_SCROLL_SHIFT) & 1) 		{ std::cout << "SCROLL "; }
		if ((type >> ITEM_TYPE_BOOK_SHIFT) & 1) 		{ std::cout << "BOOK "; }
		if ((type >> ITEM_TYPE_FLASK_SHIFT) & 1) 		{ std::cout << "FLASK "; }
		if ((type >> ITEM_TYPE_GOLD_SHIFT) & 1) 		{ std::cout << "GOLD "; }
		if ((type >> ITEM_TYPE_AMMUNITION_SHIFT) & 1) 	{ std::cout << "AMMUNITION "; }
		if ((type >> ITEM_TYPE_FOOD_SHIFT) & 1) 		{ std::cout << "FOOD "; }
		if ((type >> ITEM_TYPE_WAND_SHIFT) & 1) 		{ std::cout << "WAND "; }
		if ((type >> ITEM_TYPE_CONTAINER_SHIFT) & 1) 	{ std::cout << "CONTAINER"; }
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