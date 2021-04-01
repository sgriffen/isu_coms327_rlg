/******** include std libs ********/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
/******* include custom libs ******/
#include "./classdef/room.h"
#include "../utils/math_utils.h"

/*********** global vars **********/


/***** constructor definitions *****/
Room::Room() {
	
	index 		= 0;
	height 		= 0;
	width 		= 0;
	connected 	= 0;
}
Room::Room(uint8_t room_index, uint8_t room_height, uint8_t room_width) : Room() {
	
	index 	= room_index;
	height 	= room_height;
	width 	= room_width;
}

/****** function definitions ******/
void room_init_cells(Room *room, Coordinate top_left, Coordinate bottom_right, Coordinate center) {
	
	room->tl = top_left;
	room->br = bottom_right;
	room->center = center;
}

int rooms_intersect(int num_rooms, Room rooms[], Room *room, int space_between) {
	
	int i = 0;
	
	for (i = 0; i < num_rooms; i++) {
		
		if (h_rooms_intersect(*room, rooms[i], space_between)) { return 1; }
	}
	
	return 0;
}

int h_rooms_intersect(Room room1, Room room2, int space_between) {

	if ((room1.br.y+space_between) < (room2.tl.y-space_between) || (room2.br.y+space_between) < (room1.tl.y-space_between)) { return 0; }
	if ((room1.tl.x-space_between) > (room2.br.x+space_between) || (room2.tl.x-space_between) > (room1.br.x+space_between)) { return 0; }
	
	return 1;
}

Room* room_contains_point(int num_rooms, Room *(rooms[]), Coordinate coord) {
	
	int i = 0;
	
	for (i = 0; i < num_rooms; i++) {
	
		if (coord.y >= rooms[i]->tl.y && coord.y <= rooms[i]->br.y && coord.x >= rooms[i]->tl.x && coord.x <= rooms[i]->br.x) { return rooms[i]; }
	}
	
	return NULL;
}

Room* room_find_closest(int num_rooms, Room rooms[], int num_exclude, Room exclude_rooms[], Room *alpha) {
	
	int i = 0, j = 0;
	
	Room *closest;
	if (alpha->index+1 >= num_rooms) { closest = &(rooms[0]); }
	else { closest = &(rooms[alpha->index+1]); }
		
	for (i = 0; i < num_rooms; i++) {
		
		int room_excluded = 0;
		
		for (j = 0; j < num_exclude; j++) {
			
			if (room_is_same(exclude_rooms[j], rooms[i])) { room_excluded = 1; }
		}
		
		if (!room_excluded && !room_is_same(rooms[i], *alpha)) {
			
			if (rooms_smallest_distance_y(rooms[i], *alpha) < rooms_smallest_distance_y(*closest, *alpha) && rooms_smallest_distance_x(rooms[i], *alpha) < rooms_smallest_distance_x(*closest, *alpha)) { closest = &(rooms[i]); }
		}
	}
	
	return closest;
}

int room_is_same(Room beta, Room alpha) {
	
	if (alpha.tl.is_same(beta.tl) && alpha.br.is_same(beta.br)) { return 1; }
	
	return 0;
}

int rooms_smallest_distance_y(Room room1, Room room2) {
	
	if (room2.center.y - room1.center.y < 0) { return abs(room2.tl.y - room1.br.y); } //if room2 is below room1, distance in y is top edge of room2 - bottom edge of room1
	else { return abs(room1.tl.y - room2.br.y); } //if room2 is directly centered or above room1, distance in y is top edge of room1 - bottom edge of room2
}

int rooms_smallest_distance_x(Room room1, Room room2) {
	
	if (room2.center.x - room1.center.x < 0) { return abs(room1.tl.x - room2.br.x); } //if room2 is left of room1, distance in x is the left edge of room1 - right edge of room2
	else { return abs(room2.tl.x - room1.br.x); } //if room2 is directly centered or left of room1, distance in x is the left edge of room2 - right edge of room1
}