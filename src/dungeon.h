#include <stdint.h>

#ifndef DUNGEON_H
#define DUNGEON_H

#include "./coordinate.h"
#include "./cell.h"
#include "./room.h"
#include "./character.h"
#include "./utils/queue.h"

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
#define DUNGEON_MAX_NPCS 0
#define DUNGEON_MAX_CHANCE_COUNT 5000
#define DUNGEON_MIN_CHANCE_COUNT 50

/************* macros *************/


/******* enums declarations *******/


/******* struct declarations ******/
typedef struct {
	
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
	
	Character_PC pc;
	
	uint32_t npc_id_next;
	uint16_t num_npcs;
	Character_NPC *npcs;
} Dungeon;

/****** function declarations *****/
void dungeon_init(Dungeon *dungeon, uint8_t dungeon_height, uint8_t dungeon_width, int num_npcs);

void dungeon_finit(Dungeon *dungeon, uint8_t f_pc_y, uint8_t f_pc_x, uint8_t *f_cells, uint16_t f_num_rooms, uint8_t *f_rooms, uint16_t f_num_stairs_up, uint8_t *f_stairs_up, uint16_t f_num_stairs_down, uint8_t *f_stairs_down);

void dungeon_mem_init(Dungeon *dungeon, uint8_t dungeon_height, uint8_t dungeon_width);

void dungeon_generate_cells(Dungeon *dungeon);

void dungeon_generate_rooms(Dungeon *dungeon);

int dungeon_place_room(Dungeon *dungeon, Room *room);

void dungeon_generate_cooridors(Dungeon *dungeon);

void dungeon_generate_cooridor(Dungeon *dungeon, Coordinate start, Coordinate end);

int dungeon_draw_cooridor(Dungeon *dungeon, QueueNode *nodes, Coordinate *start);

void dungeon_generate_staircases(Dungeon *dungeon);

void dungeon_generate_pc(Dungeon *dungeon);

void dungeon_generate_npcs(Dungeon *dungeon, int num_npcs);

int dungeon_coordinate_inbounds(Coordinate location);

Coordinate dungeon_move_towards_ntunneling(Dungeon dungeon, Coordinate start, Coordinate end);

Coordinate dungeon_move_towards_tunneling(Dungeon dungeon, Coordinate start, Coordinate end);

Coordinate dungeon_move_min_ntunneling(Dungeon dungeon, Coordinate location);

Coordinate dungeon_move_min_tunneling(Dungeon dungeon, Coordinate location);

Coordinate dungeon_move_rand_ntunneling(Dungeon dungeon, Coordinate location);

Coordinate dungeon_move_rand_tunneling(Dungeon dungeon, Coordinate location);

Coordinate dungeon_move_pc(Dungeon *dungeon, Character_PC *pc);

Coordinate dungeon_move_npc0(Dungeon *dungeon, Character_NPC *npc);

Coordinate dungeon_move_npc1(Dungeon *dungeon, Character_NPC *npc);

Coordinate dungeon_move_npc2(Dungeon *dungeon, Character_NPC *npc);

Coordinate dungeon_move_npc3(Dungeon *dungeon, Character_NPC *npc);

Coordinate dungeon_move_npc4(Dungeon *dungeon, Character_NPC *npc);

Coordinate dungeon_move_npc5(Dungeon *dungeon, Character_NPC *npc);

Coordinate dungeon_move_npc6(Dungeon *dungeon, Character_NPC *npc);

Coordinate dungeon_move_npc7(Dungeon *dungeon, Character_NPC *npc);

Coordinate dungeon_move_npc8(Dungeon *dungeon, Character_NPC *npc);

Coordinate dungeon_move_npc9(Dungeon *dungeon, Character_NPC *npc);

Coordinate dungeon_move_npcA(Dungeon *dungeon, Character_NPC *npc);

Coordinate dungeon_move_npcB(Dungeon *dungeon, Character_NPC *npc);

Coordinate dungeon_move_npcC(Dungeon *dungeon, Character_NPC *npc);

Coordinate dungeon_move_npcD(Dungeon *dungeon, Character_NPC *npc);

Coordinate dungeon_move_npcE(Dungeon *dungeon, Character_NPC *npc);

Coordinate dungeon_move_npcF(Dungeon *dungeon, Character_NPC *npc);

Coordinate dungeon_los(Dungeon dungeon, Coordinate start, Coordinate end);

Coordinate dungeon_los_lowhi(Dungeon dungeon, Coordinate start, Coordinate end);

Coordinate dungeon_los_hilow(Dungeon dungeon, Coordinate start, Coordinate end);

void dungeon_resolve_collision(Dungeon *dungeon, Character_Wrapper character, Coordinate next);

int dungeon_cell_contains_pc(Dungeon dungeon, Coordinate coord);

int dungeon_cell_contains_npc(Dungeon dungeon, Coordinate coord, uint32_t npc_exclude);

void dungeon_npc_sort(Dungeon *dungeon);

void dungeon_print(Dungeon dungeon, int print_fill, int print_color, int print_weight);


#endif /* DUNGEON_H */