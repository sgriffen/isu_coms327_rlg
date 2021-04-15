/******** include std libs ********/
#include <iostream>

#include <stdlib.h>
#include <stdint.h>

/******* include custom libs ******/
#include "./classdef/npc_template.h"

/********** definitions **********/


/*********** global vars **********/


/***** constructor definitions *****/
NPC_Template::NPC_Template() {
	
	name 		= "";
	description = "";
	symbol 		= '\0';
	type 		= 0x00;
	rarity 		= 1;
	color 		= std::vector<uint8_t>();
	speed 		= Dice();
	hp 			= Dice();
	damage 		= Dice();
}
NPC_Template::NPC_Template(const NPC_Template &rhs) {
	
	name 		= rhs.name;
	description = rhs.description;
	symbol 		= rhs.symbol;
	type 		= rhs.type;
	rarity 		= rhs.rarity;
	color 		= rhs.color;
	speed 		= rhs.speed;
	hp 			= rhs.hp;
	damage 		= rhs.damage;
}

/****** function definitions ******/
NPC* NPC_Template::new_npc() {
	
	NPC *npc = new NPC();
	
	npc->name.assign(name);
	npc->description.assign(description);
	npc->symbol = symbol;
	
	npc->type 	= type;
	npc->rarity = rarity;
	npc->color 	= color;
	
	npc->speed 	= speed.roll();
	npc->hp 	= hp.roll();
	
	npc->damage = damage;
	
	return npc;
}

void NPC_Template::print() {
	
	std::cout << name << std::endl;
	
	std::cout << description << std::endl;
	
	std::cout << symbol << std::endl;
	
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
	
	speed.print();
	std::cout << std::endl;
	
	if (type > 0) {
		
		if (type & NPC_TYPE_INTELLIGENT) 	{ std::cout << "SMART "; }
		if (type & NPC_TYPE_TELEPATHIC) 	{ std::cout << "TELE "; }
		if (type & NPC_TYPE_TUNNELING) 		{ std::cout << "TUNNEL "; }
		if (type & NPC_TYPE_ERRATIC) 		{ std::cout << "ERRATIC "; }
		if (type & NPC_TYPE_PASS) 			{ std::cout << "PASS "; }
		if (type & NPC_TYPE_PICKUP) 		{ std::cout << "PICKUP "; }
		if (type & NPC_TYPE_DESTROY) 		{ std::cout << "DESTROY "; }
		if (type & NPC_TYPE_UNIQUE) 		{ std::cout << "UNIQ "; }
		if (type & NPC_TYPE_BOSS) 			{ std::cout << "BOSS"; }
		std::cout << std::endl;
	} else { std::cout << "INVALID TYPE"; }
	std::cout << std::endl;
	
	hp.print();
	std::cout << std::endl;
	
	damage.print();
	std::cout << std::endl;
	
	std::cout << unsigned(rarity) << std::endl;
	
	return;
}