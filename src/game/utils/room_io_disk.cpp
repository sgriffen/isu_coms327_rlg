/******** include std libs ********/
#include <iostream>
#include <fstream>

/******* include custom libs ******/
#include "../classdef/room.h"

/********** definitions **********/


/*********** global vars **********/


/****** function definitions ******/
void Room::disk_load(std::ifstream& file) {
	
	std::string line;
	if (file.is_open()) {
		
		if (std::getline(file, line) && line.size() > 0) { index 	= (uint8_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { height 	= (uint8_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { width 	= (uint8_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { tl.y 	= (uint8_t)std::stol(line); }
		if (std::getline(file, line) && line.size() > 0) { tl.x 	= (uint8_t)std::stol(line); }
		
		br.y = tl.y + height;
		br.x = tl.x + width;
		center.y = (uint8_t)((tl.y + br.y) / 2);
		center.x = (uint8_t)((tl.x + br.x) / 2);
		
		connected = 1;
	}
	
	return;
}
void Room::disk_save(std::ofstream& file) {
	
	if (file.is_open()) {
		
		file << std::to_string(index) 	<< "\n";
		file << std::to_string(height) 	<< "\n";
		file << std::to_string(width) 	<< "\n";
		file << std::to_string(tl.y) 	<< "\n";
		file << std::to_string(tl.x) 	<< "\n";
	}
	
	return;
}