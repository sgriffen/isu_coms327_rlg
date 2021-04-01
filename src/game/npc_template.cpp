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
	type 		= 0x0;
	rarity 		= 1;
	color 		= PAIR_WHITE;
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
void NPC_Template::print() {
	
	std::cout << name << std::endl;
	std::cout << description << std::endl;
	std::cout << symbol << std::endl;
	switch (color) {
		
	case PAIR_BLACK:
		
		std::cout << "BLACK" << std::endl;
		break;
	case PAIR_RED:
		
		std::cout << "RED" << std::endl;
		break;
	case PAIR_BLUE:
		
		std::cout << "BLUE" << std::endl;
		break;
	case PAIR_CYAN:
		
		std::cout << "CYAN" << std::endl;
		break;
	case PAIR_YELLOW:
		
		std::cout << "YELLOW" << std::endl;
		break;
	case PAIR_MAGENTA:
		
		std::cout << "MAGENTA" << std::endl;
		break;
	default:
		
		std::cout << "WHITE" << std::endl;
		break;
	}
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
	}
	hp.print();
	std::cout << std::endl;
	damage.print();
	std::cout << std::endl;
	std::cout << unsigned(rarity) << std::endl;
	
	return;
}