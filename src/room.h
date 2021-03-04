#include <stdint.h>

#ifndef ROOM_H
#define ROOM_H

#include "./coordinate.h"

/********** definitions ***********/
#define ROOM_BORDER_WIDTH 1
#define ROOM_MIN_HEIGHT 3
#define ROOM_MIN_WIDTH 4

/************* macros *************/


/******* enums declarations *******/


/******* struct declarations ******/
typedef struct {
	
	uint8_t index;
	uint8_t height;
	uint8_t width;
	
	int connected;
	
	Coordinate tl;
	Coordinate br;
	Coordinate center;
} Room;

/****** function declarations *****/
Room room_init(uint8_t index, uint8_t room_width, uint8_t room_height);

void room_init_cells(Room *room, Coordinate top_left, Coordinate bottom_right, Coordinate center);

int rooms_intersect(int num_rooms, Room rooms[], Room *room, int space_between);

int h_rooms_intersect(Room room1, Room room2, int space_between);

Room* room_contains_point(int num_rooms, Room *(rooms[]), Coordinate coord);

Room* room_find_closest(int num_rooms, Room rooms[], int num_exclude, Room exclude_rooms[], Room *alpha);

int room_is_same(Room beta, Room alpha);

int rooms_smallest_distance_y(Room room1, Room room2);

int rooms_smallest_distance_x(Room room1, Room room2);

#endif /* ROOM_H */