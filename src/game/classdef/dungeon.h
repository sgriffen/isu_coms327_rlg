#include <string>
#include <vector>

#include <stdint.h>

#ifndef DUNGEON_H
#define DUNGEON_H

#include "./coordinate.h"
#include "./cell.h"
#include "./room.h"
#include "./character.h"
#include "./item.h"
#include "./npc_template.h"
#include "./item_template.h"
#include "../utils/classdef/queue.h"

/********** definitions ***********/
#define DUNGEON_HEIGHT 21
#define DUNGEON_WIDTH 80
#define DUNGEON_BORDER_WIDTH 1
#define DUNGEON_MIN_ROOM 6
#define DUNGEON_MAX_ROOM 12
#define DUNGEON_MIN_STAIRCASE_UP 1
#define DUNGEON_MAX_STAIRCASE_UP 2
#define DUNGEON_MIN_STAIRCASE_DOWN 1
#define DUNGEON_MAX_STAIRCASE_DOWN 2
#define DUNGEON_MAX_CHANCE_COUNT 5000

/************* macros *************/


/******* enums declarations *******/


/******* struct declarations ******/
class Dungeon {
	public:
		uint8_t height;
		uint8_t	width;
		uint8_t volitile_height;
		uint8_t volitile_width;
		
		uint16_t num_cells;
		uint16_t num_volitile_cells;
		Cell **cells;
		
		uint16_t num_rooms;
		Room *rooms;
		
		uint16_t num_staircases_up;
		Cell **staircases_up;
		
		uint16_t num_staircases_down;
		Cell **staircases_down;
		
		uint8_t num_npcs_defined;
		uint32_t npc_id_next;
		uint16_t num_npcs;
		uint16_t num_npcs_dead;
		NPC *npcs;
		
		uint8_t num_items_defined;
		uint32_t item_id_next;
		uint16_t num_items;
		Item *items;
		
		/* CONSTRUCTORS */
		Dungeon();
		Dungeon(uint8_t dungeon_height, uint8_t dungeon_width, int dungeon_num_npcs, int dungeon_num_items, int dungeon_num_stairs_up, int dungeon_num_stairs_down, int dungeon_ommit_stairs, std::vector<NPC_Template> npc_templates, std::vector<Item_Template> item_templates, std::vector<std::string> *npc_unique_placed, std::vector<std::string> *item_unique_placed);
		
		/* FUNCTIONS */
		void draw(uint8_t offset_y, uint8_t offset_x, int print_fog, int print_fill, int print_weight);
};

/****** function declarations *****/
void dungeon_generate_cells(Dungeon *dungeon);

void dungeon_generate_rooms(Dungeon *dungeon);

int dungeon_place_room(Dungeon *dungeon, Room *room);

void dungeon_generate_cooridors(Dungeon *dungeon);

void dungeon_generate_cooridor(Dungeon *dungeon, Coordinate start, Coordinate end);

int dungeon_draw_cooridor(Dungeon *dungeon, QueueNode *nodes, Coordinate *start);

void dungeon_generate_staircases(Dungeon *dungeon, int num_staircase_up, int num_staircase_down, int ommit_stairs);

void dungeon_generate_npcs(Dungeon *dungeon, int num_npcs, std::vector<NPC_Template> npc_templates, std::vector<std::string> *npc_unique_placed);

void dungeon_generate_items(Dungeon *dungeon, int num_items, std::vector<Item_Template> item_templates, std::vector<std::string> *item_unique_placed);

int dungeon_coordinate_inbounds(Coordinate location);

Coordinate dungeon_los(Dungeon dungeon, Coordinate start, Coordinate end);

int dungeon_los_ray(Dungeon dungeon, Coordinate start, Coordinate end, int const ray_matrix[2]);

int dungeon_los_lowhi(Dungeon dungeon, Coordinate start, Coordinate end);

int dungeon_los_hilow(Dungeon dungeon, Coordinate start, Coordinate end);

void dungeon_pc_los(Dungeon *dungeon, Character *pc, uint8_t mark);

int* dungeon_resolve_collision(Dungeon *dungeon, Character *character, Coordinate next, int override_damage = 0);

int dungeon_contains_npcs(Dungeon *dungeon);

#endif /* DUNGEON_H */