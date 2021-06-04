/******** include std libs ********/
#include <iostream>
#include <fstream>

/******* include custom libs ******/
#include "../classdef/character.h"

/********** definitions **********/


/*********** global vars **********/


/****** function definitions ******/
void Character::disk_load(std::ifstream& file, std::vector<Item_Template> item_templates, std::vector<Item*> *items_loaded) {
	
	int i = 0;
	
	int inventory_size = 0;
	
	std::string line;
	if (file.is_open()) {
		
		if (std::getline(file, line) && line.size() > 0) { id 				= (uint32_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { name 			= line; }
		if (std::getline(file, line) && line.size() > 0) { symbol 			= line.at(0); }
		if (std::getline(file, line) && line.size() > 0) { location.y 		= (uint8_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { location.x 		= (uint8_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { speed 			= (uint32_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { hp 				= (uint32_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { damage 			= Dice(line); }
		if (std::getline(file, line) && line.size() > 0) { inventory_size 	= (uint32_t)std::stol(line); }
		if (inventory_size) {
			
			for (i = 0; i < inventory_size; i++) {
				
				Item *item = new Item();
				item->disk_load(file, item_templates);
				
				inventory.push_back(item);
				items_loaded->push_back(item);
			}
		}
	}
	
	return;
}
void Character::disk_save(std::ofstream& file) {
	
	int i = 0;
	
	if (file.is_open()) {
		
		file << std::to_string(id) 					<< "\n";
		file << name								<< "\n";
		file << symbol								<< "\n";
		file << std::to_string(location.y)			<< "\n";
		file << std::to_string(location.x)			<< "\n";
		file << std::to_string(speed) 				<< "\n";
		file << std::to_string(hp) 					<< "\n";
		file << damage.to_string()					<< "\n";
		file << std::to_string(inventory.size())	<< "\n";
		for (i = 0; i < (int)inventory.size(); i++) { inventory[i]->disk_save(file); }
	}
	
	return;
}

void PC::disk_load(std::ifstream& file, std::vector<Item_Template> item_templates, std::vector<Item*> *items_loaded) {
	
	int i = 0;
	
	std::string line;
	
	if (file.is_open()) {
		
		Character::disk_load(file, item_templates, items_loaded);
		
		if (std::getline(file, line) && line.size() > 0) { num_kills 			= (uint32_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { num_items_equipped 	= (uint8_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { equipped 			= (uint16_t)std::stol(line); }
		for (i = 0; i < num_items_equipped; i++) {
			
			Item *item = new Item();
			item->disk_load(file, item_templates);
			
			if ((equipped >> 0) & 1) { equipment_weapon 	= item; }
			if ((equipped >> 1) & 1) { equipment_offhand 	= item; }
			if ((equipped >> 2) & 1) { equipment_ranged 	= item; }
			if ((equipped >> 3) & 1) { equipment_armor 		= item; }
			if ((equipped >> 4) & 1) { equipment_helmet 	= item; }
			if ((equipped >> 5) & 1) { equipment_cloak 		= item; }
			if ((equipped >> 6) & 1) { equipment_gloves 	= item; }
			if ((equipped >> 7) & 1) { equipment_boots 		= item; }
			if ((equipped >> 8) & 1) { equipment_amulet 	= item; }
			if ((equipped >> 9) & 1) { equipment_light 		= item; }
			if ((equipped >> 10) & 1) { equipment_ring0 	= item; }
			if ((equipped >> 11) & 1) { equipment_ring1 	= item; }
			
			items_loaded->push_back(item);
		}
		
		description.assign("Our only hope!");
		color.push_back(PAIR_GREEN);
	}
	
	return;
}
void PC::disk_save(std::ofstream& file) {
	
	if (file.is_open()) {
		
		Character::disk_save(file);
		file << std::to_string(num_kills) 			<< "\n";
		file << std::to_string(num_items_equipped) 	<< "\n";
		file << equipped 							<< "\n";
		
		if ((equipped >> 0) & 1) { equipment_weapon->disk_save(file); }
		if ((equipped >> 1) & 1) { equipment_offhand->disk_save(file); }
		if ((equipped >> 2) & 1) { equipment_ranged->disk_save(file); }
		if ((equipped >> 3) & 1) { equipment_armor->disk_save(file); }
		if ((equipped >> 4) & 1) { equipment_helmet->disk_save(file); }
		if ((equipped >> 5) & 1) { equipment_cloak->disk_save(file); }
		if ((equipped >> 6) & 1) { equipment_gloves->disk_save(file); }
		if ((equipped >> 7) & 1) { equipment_boots->disk_save(file); }
		if ((equipped >> 8) & 1) { equipment_amulet->disk_save(file); }
		if ((equipped >> 9) & 1) { equipment_light->disk_save(file); }
		if ((equipped >> 10) & 1) { equipment_ring0->disk_save(file); }
		if ((equipped >> 11) & 1) { equipment_ring1->disk_save(file); }
	}
	
	return;
}

void NPC::disk_load(std::ifstream& file, std::vector<NPC_Template> npc_templates, std::vector<Item_Template> item_templates, std::vector<Item*> *items_loaded) {
	
	int i = 0;
	
	std::string line;
	
	if (file.is_open()) {
		
		Character::disk_load(file, item_templates, items_loaded);
		
		if (std::getline(file, line) && line.size() > 0) { type 	= (uint16_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { rarity 	= (uint8_t)std::stol(line); }
		
		for (i = 0; i < (int)npc_templates.size(); i++) {
			
			if (type == npc_templates[i].type && name == npc_templates[i].name) {
				
				color = npc_templates[i].color;
				description.assign(npc_templates[i].description);
				break;
			}
		}
	}
	
	return;
}
void NPC::disk_save(std::ofstream& file) {
	
	if (file.is_open()) {
		
		Character::disk_save(file);
		file << std::to_string(type) 	<< "\n";
		file << std::to_string(rarity) 	<< "\n";
	}
	
	return;
}