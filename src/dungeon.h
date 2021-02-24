#ifndef DUNGEON_H
#define DUNGEON_H

#include <stdint.h>

#include "cell.h"
#include "room.h"
#include "character.h"

/********** definitions **********/
#define DUNGEON_HEIGHT 21
#define DUNGEON_WIDTH 80
#define DUNGEON_BORDER_WIDTH 1
#define DUNGEON_MIN_ROOM 6
#define DUNGEON_MAX_ROOM 12
#define DUNGEON_MIN_STAIRCASE_UP 1
#define DUNGEON_MAX_STAIRCASE_UP 2
#define DUNGEON_MIN_STAIRCASE_DOWN 1
#define DUNGEON_MAX_STAIRCASE_DOWN 2
#define DUNGEON_MAX_NPCS 0
#define DUNGEON_MAX_CHANCE_COUNT 150

/******* enums declarations *******/


/******* struct declarations ******/
typedef struct {
	
	uint8_t height;
	uint8_t	width;
	uint8_t volitile_height;
	uint8_t volitile_width;
	
	uint16_t num_cells;
	Cell **cells;
	
	uint16_t num_rooms;
	Room *rooms;
	
	uint16_t num_staircases_up;
	Cell **staircases_up;
	
	uint16_t num_staircases_down;
	Cell **staircases_down;
	
	Character pc;
	
	uint32_t npc_id_next;
	uint16_t num_npcs;
	Character *npcs;
} Dungeon;

/****** function declarations *****/
void dungeon_init(Dungeon *dungeon, uint8_t dungeon_height, uint8_t dungeon_width);

void dungeon_mem_init(Dungeon *dungeon, uint8_t dungeon_height, uint8_t dungeon_width);

void dungeon_generate_cells(Dungeon *dungeon);

void dungeon_generate_rooms(Dungeon *dungeon);

int dungeon_place_room(Dungeon *dungeon, Room *room);

void dungeon_generate_cooridors(Dungeon *dungeon);

void dungeon_generate_cooridor(Dungeon *dungeon, Coordinate start, Coordinate end);

void dungeon_generate_staircases(Dungeon *dungeon);

void dungeon_generate_pc(Dungeon *dungeon);

void dungeon_generate_npcs(Dungeon *dungeon);

void dungeon_print(Dungeon dungeon, int print_fill, int print_color, int print_weight);

#endif /* DUNGEON_H */