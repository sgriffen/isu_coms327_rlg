/******** include std libs ********/
#include <iostream>

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/******* include custom libs ******/
#include "./classdef/dice.h"
#include "../../utils/str_utils.h"
#include "../../utils/math_utils.h"

/********** definitions **********/


/*********** global vars **********/


/***** constructor definitions *****/
Dice::Dice() {
	
	base 		= 0;
	num_dice 	= 0;
	num_sides 	= 0;
}
Dice::Dice(uint32_t dice_base, uint32_t dice_num_dice, uint32_t dice_num_sides) : Dice() {
	
	base 		= dice_base;
	num_dice 	= dice_num_dice;
	num_sides 	= dice_num_sides;
}
Dice::Dice(std::string dice_str) : Dice() {
	
	if (dice_str.size() >= 5) {
		
		std::vector<std::string> str_splt0 = strsplit(dice_str, "+");
		std::vector<std::string> str_splt1 = strsplit(str_splt0[1], "d");
		
		base 		= std::stoi(str_splt0[0]);
		num_dice 	= std::stoi(str_splt1[0]);
		num_sides 	= std::stoi(str_splt1[1]);
	}
}

/****** function definitions ******/
uint32_t Dice::roll() {
	
	uint32_t i;
	uint32_t roll = 0;
	
	for (i = 0; i < num_dice; i++) { roll += (uint32_t)utils_rand_between(1, num_sides); }
	
	return base + roll;
}

std::string Dice::to_string() {
	
	std::string str = std::to_string(base);
	str.append("+");
	str.append(std::to_string(num_dice));
	str.append("d");
	str.append(std::to_string(num_sides));
	
	return str;
}

void Dice::print() {
	
	std::cout << base << "+" << num_dice << "d" << num_sides;
	
	return;
}