/******** include std libs ********/
#include <iostream>
#include <fstream>

/******* include custom libs ******/
#include "../classdef/cell.h"

/********** definitions **********/


/*********** global vars **********/


/****** function definitions ******/
void Cell::disk_load(std::ifstream& file, std::vector<Item_Template> item_templates, std::vector<Item*> *items_loaded) {
	
	int i = 0;
	
	int num_items = 0;
	
	std::string line;
	if (file.is_open()) {
		
		if (std::getline(file, line) && line.size() > 0) { type_current = (CellType)std::stol(line); }
		type_next = type_current;
		if (std::getline(file, line) && line.size() > 0) { hardness 	= (uint8_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { visited 		= (uint8_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { location.y 	= (uint8_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { location.x 	= (uint8_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { num_items 	= (uint32_t)std::stol(line); }
		
		for (i = 0; i < num_items; i++) {
			
			Item *item = new Item();
			item->disk_load(file, item_templates);
			
			items.push_back(item);
			items_loaded->push_back(item);
		}
	}
	
	return;
}
void Cell::disk_save(std::ofstream& file) {
	
	int i = 0;
	
	if (file.is_open()) {
		
		if (type_current == CellType_Room) {
			
			i = 1;
		}
		
		file << std::to_string(type_next) 		<< "\n";
		file << std::to_string(hardness) 		<< "\n";
		file << std::to_string(visited) 		<< "\n";
		file << std::to_string(location.y) 		<< "\n";
		file << std::to_string(location.x) 		<< "\n";
		file << std::to_string(items.size())	<< std::endl;
		for (i = 0; i < (int)items.size(); i++) { items[i]->disk_save(file); }
	}
	
	return;
}