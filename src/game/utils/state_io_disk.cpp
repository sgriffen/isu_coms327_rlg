/******** include std libs ********/
#include <iostream>
#include <fstream>

/******* include custom libs ******/
#include "../classdef/state.h"
#include "./pathfinder.h"

/********** definitions **********/


/*********** global vars **********/


/****** function definitions ******/
void GameState::disk_load() {
	
	int i = 0;
	
	std::string line;
	std::ifstream file(dir_disk);
	if (!file.is_open()) { std::cout << "file [" << dir_disk << "] invalid" << "\n"; }
	else {
		
		if (std::getline(file, line) && line.size() > 0) { turn 			= (uint64_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { boss_defeated 	= (uint8_t)std::stol(line); }
		
		pc.disk_load(file, item_templates, &items_loaded);
		
		if (std::getline(file, line) && line.size() > 0) { num_dungeons 			= (uint32_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { num_dungeons_generated 	= (uint32_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { dungeon_index 			= (int64_t)std::stol(line); }
		dungeons = (Dungeon*)calloc(num_dungeons, sizeof(Dungeon));
		for (i = 0; i < (int)num_dungeons; i++) { dungeons[i].disk_load(file, npc_templates, item_templates, &items_loaded); }
		
		turn = 0;
		
		file.close();
		
		dungeons[dungeon_index].cells[pc.location.y][pc.location.x].character = &(pc);
		
		deinit_queue();
		init_queue();
		
		dungeon_pc_los(&(dungeons[dungeon_index]), (Character*)(&(pc)), 1);
	
		pathfinder_ntunneling(&(dungeons[dungeon_index]), &(pc.location));
		pathfinder_tunneling(&(dungeons[dungeon_index]), &(pc.location));
	}
	
	return;
}
void GameState::disk_save() {
	
	int i = 0;
	
	std::ofstream file(dir_disk);
	if (!file.is_open()) { std::cout << "file [" << dir_disk << "] invalid" << "\n"; }
	else {
		
		file << std::to_string(turn) 					<< "\n";
		file << std::to_string(boss_defeated) 			<< "\n";
		
		pc.disk_save(file);
		
		file << std::to_string(num_dungeons) 			<< "\n";
		file << std::to_string(num_dungeons_generated) 	<< "\n";
		file << std::to_string(dungeon_index) 			<< "\n";
		for (i = 0; i < (int)num_dungeons_generated; i++) { dungeons[i].disk_save(file); }
		
		file << std::endl;
		
		file.close();
	}
	
	return;
}