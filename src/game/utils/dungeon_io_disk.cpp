/******** include std libs ********/
#include <iostream>
#include <fstream>

/******* include custom libs ******/
#include "../classdef/dungeon.h"

/********** definitions **********/


/*********** global vars **********/


/****** function definitions ******/
void Dungeon::disk_load(std::ifstream& file, std::vector<NPC_Template> npc_templates, std::vector<Item_Template> item_templates, std::vector<Item*> *items_loaded) {
	
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	
	std::string line;
	if (file.is_open()) {
		
		if (std::getline(file, line) && line.size() > 0) { height 	= (uint8_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { width 	= (uint8_t)std::stol(line); }
		volitile_height = height - (DUNGEON_BORDER_WIDTH*2);
		volitile_width = width - (DUNGEON_BORDER_WIDTH*2);
		
		if (std::getline(file, line) && line.size() > 0) { num_staircases_up 	= (uint16_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { num_staircases_down 	= (uint16_t)std::stol(line); }
		staircases_up = (Cell**)calloc(num_staircases_up, sizeof(Cell*));
		staircases_down = (Cell**)calloc(num_staircases_down, sizeof(Cell*));
		
		if (std::getline(file, line) && line.size() > 0) { num_cells = (uint16_t)std::stol(line); }
		num_volitile_cells = volitile_height * volitile_width;
		cells = (Cell**)calloc(height, sizeof(Cell*));
		for (i = 0; i < height; i++) { cells[i] = (Cell*)calloc(width, sizeof(Cell)); }
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
				
				cells[i][j].disk_load(file, item_templates, items_loaded);
				
				if (cells[i][j].type_current == CellType_Stair_up)		{ staircases_up[k++] = &(cells[i][j]); }
				if (cells[i][j].type_current == CellType_Stair_down) 	{ staircases_down[l++] = &(cells[i][j]); }
			}
		}
		
		if (std::getline(file, line) && line.size() > 0) { num_rooms = (uint16_t)std::stol(line); }
		rooms = (Room*)calloc(num_rooms, sizeof(Room));
		for (i = 0; i < num_rooms; i++) { rooms[i].disk_load(file); }
		
		if (std::getline(file, line) && line.size() > 0) { npc_id_next 		= (uint32_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { num_npcs 		= (uint16_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { num_npcs_defined = (uint8_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { num_npcs_dead 	= (uint16_t)std::stol(line); }
		npcs = (NPC*)calloc(num_npcs, sizeof(NPC));
		for (i = 0; i < num_npcs; i++) {
			
			npcs[i].disk_load(file, npc_templates, item_templates, items_loaded);
			if (npcs[i].hp > 0) { cells[npcs[i].location.y][npcs[i].location.x].character = &(npcs[i]); }
		}
	}
	
	return;
}
void Dungeon::disk_save(std::ofstream& file) {
	
	int i = 0;
	int j = 0;
	
	if (file.is_open()) {
		
		file << std::to_string(height)				<< "\n";
		file << std::to_string(width) 				<< "\n";
		
		file << std::to_string(num_staircases_up) 	<< "\n";
		file << std::to_string(num_staircases_down) << "\n";
		
		file << std::to_string(num_cells)			<< "\n";
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) { cells[i][j].disk_save(file); }
		}
		
		file << std::to_string(num_rooms) 			<< "\n";
		for (i = 0; i < num_rooms; i++) { rooms[i].disk_save(file); }
		
		file << std::to_string(npc_id_next) 		<< "\n";
		file << std::to_string(num_npcs) 			<< "\n";
		file << std::to_string(num_npcs_defined) 	<< "\n";
		file << std::to_string(num_npcs_dead) 		<< "\n";
		for (i = 0; i < num_npcs; i++) { npcs[i].disk_save(file); }
	}
	
	return;
}