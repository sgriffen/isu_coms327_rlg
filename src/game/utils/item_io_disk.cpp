/******** include std libs ********/
#include <iostream>
#include <fstream>

/******* include custom libs ******/
#include "../classdef/item.h"

/********** definitions **********/


/*********** global vars **********/


/****** function definitions ******/
void Item::disk_load(std::ifstream& file, std::vector<Item_Template> item_templates) {
	
	int i = 0;
	
	std::string line;
	if (file.is_open()) {
		
		if (std::getline(file, line) && line.size() > 0) { id 				= (uint32_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { type 			= (uint32_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { name 			= line; }
		if (std::getline(file, line) && line.size() > 0) { symbol 			= line.at(0); }
		if (std::getline(file, line) && line.size() > 0) { status 			= (uint8_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { rarity 			= (uint8_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { value 			= (uint32_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { weight 			= (uint32_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { attribute 		= (uint32_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { bonus_speed 		= (uint32_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { bonus_defense 	= (uint32_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { bonus_hit 		= (uint32_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { bonus_dodge 		= (uint32_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { bonus_damage 	= Dice(line); }
		
		for (i = 0; i < (int)item_templates.size(); i++) {
			
			if (type == item_templates[i].type && name == item_templates[i].name) {
				
				color = item_templates[i].color;
				description = item_templates[i].description;
				break;
			}
		}
	}
	
	return;
}
void Item::disk_save(std::ofstream& file) {
	
	if (file.is_open()) {
		
		file << std::to_string(id) 				<< "\n";
		file << std::to_string(type)			<< "\n";
		file << name 							<< "\n";
		file << symbol 							<< "\n";
		file << std::to_string(status)			<< "\n";
		file << std::to_string(rarity)			<< "\n";
		file << std::to_string(value) 			<< "\n";
		file << std::to_string(weight) 			<< "\n";
		file << std::to_string(attribute) 		<< "\n";
		file << std::to_string(bonus_speed) 	<< "\n";
		file << std::to_string(bonus_defense) 	<< "\n";
		file << std::to_string(bonus_hit) 		<< "\n";
		file << std::to_string(bonus_dodge) 	<< "\n";
		file << bonus_damage.to_string() 		<< "\n";
	}
	
	return;
}