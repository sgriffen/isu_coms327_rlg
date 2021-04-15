#include <string>
#include <vector>

#include <stdint.h>

#ifndef NPC_TEMPLATE_H
#define NPC_TEMPLATE_H

#include "./character.h"
#include "../../res/color.h"
#include "../utils/classdef/dice.h"

/********** definitions **********/
#define NPC_NUM_PARAM			9
#define NPC_TYPE_INTELLIGENT 	0b000000001
#define NPC_TYPE_TELEPATHIC 	0b000000010
#define NPC_TYPE_TUNNELING 		0b000000100
#define NPC_TYPE_ERRATIC 		0b000001000
#define NPC_TYPE_PASS			0b000010000
#define NPC_TYPE_PICKUP			0b000100000
#define NPC_TYPE_DESTROY		0b001000000
#define NPC_TYPE_UNIQUE			0b010000000
#define NPC_TYPE_BOSS			0b100000000
#define NPC_SPEED_MIN			1
#define NPC_SPEED_MAX			20

/************* macros *************/


/******* enums declarations *******/


/******* class declarations ******/
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

#endif /* NPC_TEMPLATE_H */