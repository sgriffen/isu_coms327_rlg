/******** include std libs ********/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
/******* include custom libs ******/
#include "dungeon.h"
#include "utils/math_utils.h"

/********** definitions **********/
#define MAX_ROOM_ITERS 100

/*********** global vars **********/

void dungeon_init(Dungeon *dungeon, uint8_t dungeon_height, uint8_t dungeon_width) {
	
	int i = 0, j = 0;
	
//	int dungeon_area = dungeon_height * dungeon_width;
	
	srand(time(NULL));
	
	/* initialize dungeon memory */
	dungeon_mem_init(dungeon, dungeon_height, dungeon_width);

	/* initialize dungeon cells */
    for(i = 0; i < dungeon->height; i++) {
        for (j = 0; j < dungeon->width; j++) {
			
			dungeon->cells[i][j] = cell_init(i, j, -1);
			
			if (i < DUNGEON_BORDER_WIDTH || i >= dungeon->height-DUNGEON_BORDER_WIDTH) {
				
				dungeon->cells[i][j].type = CellType_Border_h;
				dungeon->cells[i][j].hardness = 255;
			}
			else if (j < DUNGEON_BORDER_WIDTH || j >= dungeon->width-DUNGEON_BORDER_WIDTH) {
				
				dungeon->cells[i][j].type = CellType_Border_v;
				dungeon->cells[i][j].hardness = 255;
			}
		}
    }
	
	/* generate and sort rooms */
	dungeon_generate_rooms(dungeon);
	
	/* generate cooridors */
	dungeon_generate_cooridors(dungeon);
	
	/* add staircases */
	dungeon_generate_staircases(dungeon);
	
	/* add pc and npcs */
	dungeon_generate_pc(dungeon);
	dungeon_generate_npcs(dungeon);
	
	return;
}

void dungeon_mem_init(Dungeon *dungeon, uint8_t dungeon_height, uint8_t dungeon_width) {
	
	int i = 0, j = 0;
	
	srand(time(NULL));
	
	dungeon->height = dungeon_height;
	dungeon->width = dungeon_width;
	dungeon->volitile_height = dungeon_height-(DUNGEON_BORDER_WIDTH*2); //initialize dungeon height
	dungeon->volitile_width = dungeon_width-(DUNGEON_BORDER_WIDTH*2); //initialize dungeon width
	dungeon->num_cells = dungeon->height * dungeon->width;
	
	/* allocate memory for and initialize Dungeon cells */
	dungeon->cells = (Cell**)malloc(sizeof(Cell*) * dungeon->height);
    for(i = 0; i < dungeon->height; i++) {
		
		dungeon->cells[i] = (Cell*)malloc(sizeof(Cell) * dungeon->width); 
		for (j = 0; j < dungeon->width; j++) { dungeon->cells[i][j] = cell_init(i, j, 0); }
	}
	
	dungeon->num_rooms = 0;
	dungeon->rooms = NULL;
	
	dungeon->num_staircases_up = 0;
	dungeon->staircases_up = NULL;
	dungeon->num_staircases_down = 0;
	dungeon->staircases_down = NULL;
	
	
	
	dungeon->num_npcs = 0;
	dungeon->npcs = NULL;
	
	return;
}

void dungeon_generate_rooms(Dungeon *dungeon) {
	
	int i = 0, j = 0, k = 0;
	int num_rooms = 0;
	int room_min_area = ROOM_MIN_HEIGHT * ROOM_MIN_WIDTH;
	int room_max_area = ROOM_MIN_HEIGHT * ROOM_MIN_WIDTH;
	
	num_rooms = utils_rand_between(DUNGEON_MIN_ROOM, DUNGEON_MAX_ROOM, NULL); //get random number of rooms for dungeon between min and max number of rooms possible
	room_max_area = (dungeon->volitile_height * dungeon->volitile_width) / (ROOM_BORDER_WIDTH*4*num_rooms);
	
	dungeon->rooms = (Room*)malloc(sizeof(Room) * num_rooms); //allocate room memory
	
	for (i = 0; i < num_rooms; i++) {
		
		Room room;
		int room_area = 0;
		int room_height = 0;
		int room_width = 0;
		int room_placed = 0;
	
		while (!room_placed) {
			
			/* find room dimensions */
			room_area = utils_rand_between(room_min_area, room_max_area, NULL);
			room_height = utils_rand_between(ROOM_MIN_HEIGHT, room_area/ROOM_MIN_WIDTH, NULL);
			room_width = room_area / room_height;
			
			room = room_init(i, room_height, room_width);
			
			/* place room in dungeon */
			room_placed = dungeon_place_room(dungeon, &room);
		}
		
		dungeon->rooms[i] = room; //add room to arr
		dungeon->num_rooms++; //increase room count
		
		/* mark room cells as rooms */
		for (j = room.tl->y; j < room.br->y; j++) {
			for (k = room.tl->x; k < room.br->x; k++) {
				
				dungeon->cells[j][k].type = CellType_Room;
				dungeon->cells[j][k].hardness = 0;
			}
		}
		
		room.connected = 0;
	}
	
	return;
}

int dungeon_place_room(Dungeon *dungeon, Room *room) {
	
	int iterations = 0;

	int room_max_y = dungeon->volitile_height - room->height - (ROOM_BORDER_WIDTH);
	int room_max_x = dungeon->volitile_width - room->width - (ROOM_BORDER_WIDTH);
	
	int cell_y = utils_rand_between(ROOM_BORDER_WIDTH, room_max_y, NULL);
	int cell_x = utils_rand_between(ROOM_BORDER_WIDTH, room_max_x, NULL);
	
	room_init_cells(room, &(dungeon->cells[cell_y][cell_x]), &(dungeon->cells[cell_y + room->height][cell_x + room->width]), &(dungeon->cells[cell_y + (room->height / 2)][cell_x + (room->width / 2)]));
	
	while (rooms_intersect(dungeon->num_rooms, dungeon->rooms, room, ROOM_BORDER_WIDTH) && iterations < MAX_ROOM_ITERS) { //if the room intersects another room, find new coordinates
		
		cell_y = utils_rand_between(ROOM_BORDER_WIDTH, room_max_y, NULL);
		cell_x = utils_rand_between(ROOM_BORDER_WIDTH, room_max_x, NULL);
	
		room_init_cells(room, &(dungeon->cells[cell_y][cell_x]), &(dungeon->cells[cell_y + room->height][cell_x + room->width]), &(dungeon->cells[cell_y + (room->height / 2)][cell_x + (room->width / 2)]));
		
		iterations++;
	}
	
	if (iterations >= MAX_ROOM_ITERS) { return 0; }
	
	return 1;
}

void dungeon_generate_cooridors(Dungeon *dungeon) {
	
	/* 
		Strategy:
		Find closest room to room 0, draw direct path to it if both rooms are not already connected, and there is an unconnected room. Else, draw direct path to closest room. Do same for 1 and 2 and so on to num_rooms - 1
	*/
	
	int i = 0;
	
	int num_connected_rooms = 0;
	Room connected_rooms[dungeon->num_rooms];
	
	while (num_connected_rooms < dungeon->num_rooms) {
		
		Room *alpha = &(dungeon->rooms[i]);
		Room *beta = room_find_closest(dungeon->num_rooms, dungeon->rooms, num_connected_rooms, connected_rooms, alpha);
		
		int delta_y = beta->center->y - alpha->center->y;
		int delta_x = beta->center->x - alpha->center->x;
		
		/* start at random spot on edge of room alpha */
		int cell_start_y = utils_rand_between(alpha->tl->y, alpha->br->y, NULL);
		int cell_start_x = utils_rand_between(alpha->tl->x, alpha->br->x, NULL);
		/* end at random spot on edge of room beta */
		int cell_end_y = utils_rand_between(beta->tl->y, beta->br->y, NULL);
		int cell_end_x = utils_rand_between(beta->tl->x, beta->br->x, NULL);
		
		if (delta_y < 0) { //room beta is above room alpha
		
			cell_start_y = alpha->tl->y;
			cell_end_y = beta->br->y;
		}
		if (delta_x > 0) { //room beta is to the right of room alpha
		
			cell_start_x = alpha->br->x;
			cell_end_x = beta->tl->x;
		}
			
		dungeon_generate_cooridor(dungeon, cell_start_y, cell_start_x, cell_end_y, cell_end_x);
		
		if (!alpha->connected) {
			
			alpha->connected = 1;
			connected_rooms[num_connected_rooms] = *alpha;
			num_connected_rooms++;
		}
		if (!beta->connected) {
			
			beta->connected = 1;
			connected_rooms[num_connected_rooms] = *beta;
			num_connected_rooms++;
		}
		
		i = beta->index;
		
//		printf("num connected rooms: [%d]\n", num_connected_rooms);
//		printf("Room alpha: index [%d]   height [%d] width [%d]   tl.x [%d] tl.y [%d]   br.x [%d] br.y [%d]   connected [%d]\n", alpha->index, alpha->height, alpha->width, alpha->tl->x, alpha->tl->y, alpha->br->x, alpha->br->y, alpha->connected);
//		printf("Room beta: index [%d]   height [%d] width [%d]   tl.x [%d] tl.y [%d]   br.x [%d] br.y [%d]   connected [%d]\n", beta->index, beta->height, beta->width, beta->tl->x, beta->tl->y, beta->br->x, beta->br->y, beta->connected);
		
//		dungeon_print(*dungeon, 0);
		
//		char temp;
//		scanf("%c", &temp);
	}
	
	return;
}

void dungeon_generate_cooridor(Dungeon *dungeon, int start_y, int start_x, int end_y, int end_x) {
	
	Cell *cell;
	
//	float step_deviation_chance = 25.0;
	int step = 0;
	int step_count_y = 0;
	int step_count_x = 0;
	int delta_y = end_y - start_y;
	int delta_x = end_x - start_x;
	
	while(delta_x != 0 || delta_y != 0) {
		
		cell = &(dungeon->cells[start_y][start_x]);
		
		/* if cell is not a room, mark as a cooridor */
		if (cell->type != CellType_Room) {
			
			cell->type = CellType_Cooridor; 
			cell->hardness = 0; 
		}
		
		/* get next closest cell between room1 and room2, converge deltas to 0 */
		if (delta_y > 0) { //if y target is below current y, increase y coordinate
			
			step = 1;
//			if (utils_rand_chance(step_deviation_chance, NULL) && start_y-1 > dungeon->volitile_height && step_count_y < DUNGEON_MAX_CHANCE_COUNT) { step = -1; }
			
			start_y += step;
			step_count_y++;
		}
		else if (delta_y < 0) { //if y target is above current y, decrease y coordinate 
		
			step = -1;
//			if (utils_rand_chance(step_deviation_chance, NULL) && start_y+1 < dungeon->volitile_height && step_count_y < DUNGEON_MAX_CHANCE_COUNT) { step = 1; }
			
			start_y += step;
			step_count_y++;
		} else { //random chance of adding or subtracting 1 from delta_y
			
			step = 0;
//			if (utils_rand_chance(step_deviation_chance, NULL) && start_y-1 > dungeon->volitile_height) { step = -1; }
//			if (utils_rand_chance(step_deviation_chance, NULL) && start_y+1 < dungeon->volitile_height) { step = 1; }
			
			start_y += step;
			step_count_y++;
		}
		
		if (delta_x > 0) { //if x target is right of current x, increase x coordinate
		
			step = 1;
//			if (utils_rand_chance(step_deviation_chance, NULL) && start_x-1 > dungeon->volitile_width && step_count_x < DUNGEON_MAX_CHANCE_COUNT) { step = -1; }
			
			start_x += step;
			step_count_x++;
		}
		else if (delta_x < 0) { //if x target is left of current x, decrease x coordinate
		
			step = -1;
//			if (utils_rand_chance(step_deviation_chance, NULL) && start_x+1 < dungeon->volitile_width && step_count_x < DUNGEON_MAX_CHANCE_COUNT) { step = 1; }
			
			start_x += step;
			step_count_x++;
		} else { //random chance of adding or subtracting 1 from delta_y
			
			step = 0;
//			if (utils_rand_chance(step_deviation_chance, NULL) && start_x-1 > dungeon->volitile_width && step_count_x < DUNGEON_MAX_CHANCE_COUNT) { step = -1; }
//			if (utils_rand_chance(step_deviation_chance, NULL) && start_x+1 < dungeon->volitile_width && step_count_x < DUNGEON_MAX_CHANCE_COUNT) { step = 1; }
			
			start_x += step;
			step_count_x++;
		}
		
		delta_y = end_y - start_y;
		delta_x = end_x - start_x;
	}
	
	/* connect last cell */
	cell = &(dungeon->cells[start_y][start_x]);
	if (cell->type != CellType_Room) {
		
		cell->type = CellType_Cooridor; 
		cell->hardness = 0; 
	}

	return;
}

void dungeon_generate_staircases(Dungeon *dungeon) {
	
	/*
		up staircases are are randomly placed (excluding rooms[0] and rooms[num_rooms - 1])
		1 down staircase is always placed at a random location in rooms[num_rooms-1].  Any more are randomly placed (excluding rooms[0] and rooms[num_rooms - 1])
	*/
	
	int i = 0;
	
	int num_staircase_down = utils_rand_between(DUNGEON_MIN_STAIRCASE_DOWN, DUNGEON_MAX_STAIRCASE_DOWN, NULL);
	int num_staircase_up = utils_rand_between(DUNGEON_MIN_STAIRCASE_UP, DUNGEON_MAX_STAIRCASE_UP, NULL);
	
	/* initialize struct data */
	dungeon->num_staircases_down = num_staircase_down;
	dungeon->staircases_down = (Cell**)malloc(sizeof(Cell*) * num_staircase_down);
	dungeon->num_staircases_up = num_staircase_up;
	dungeon->staircases_up = (Cell**)malloc(sizeof(Cell*) * num_staircase_up);
	
	/* place down staircases */
	for (i = 0; i < num_staircase_down; i++) {
		
		int room_index = dungeon->num_rooms-1;
		if (i != 0) { room_index = utils_rand_between(1, dungeon->num_rooms-2, NULL); }
		
		int stair_y = utils_rand_between(dungeon->rooms[room_index].tl->y+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br->y-ROOM_BORDER_WIDTH, NULL);
		int stair_x = utils_rand_between(dungeon->rooms[room_index].tl->x+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br->x-ROOM_BORDER_WIDTH, NULL);
		
		dungeon->cells[stair_y][stair_x].type = CellType_Stair_down;
		dungeon->cells[stair_y][stair_x].hardness = 0;
		
		dungeon->staircases_down[i] = &(dungeon->cells[stair_y][stair_x]);
	}
	/* place up staircases */
	for (i = 0; i < num_staircase_up; i++) {
		
		int room_index = utils_rand_between(1, dungeon->num_rooms-2, NULL);
		
		int stair_y = utils_rand_between(dungeon->rooms[room_index].tl->y+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br->y-ROOM_BORDER_WIDTH, NULL);
		int stair_x = utils_rand_between(dungeon->rooms[room_index].tl->x+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br->x-ROOM_BORDER_WIDTH, NULL);
		
		dungeon->cells[stair_y][stair_x].type = CellType_Stair_up;
		dungeon->cells[stair_y][stair_x].hardness = 0;
		
		dungeon->staircases_up[i] = &(dungeon->cells[stair_y][stair_x]);
	}
	
	return;
}

void dungeon_generate_pc(Dungeon *dungeon) {
	
	/*
		player character (pc) is placed at a random location in rooms[0]
	*/
	
	Character pc;
	
	int loc_y = utils_rand_between(dungeon->rooms[0].tl->y+ROOM_BORDER_WIDTH, dungeon->rooms[0].br->y-ROOM_BORDER_WIDTH, NULL);
	int loc_x = utils_rand_between(dungeon->rooms[0].tl->x+ROOM_BORDER_WIDTH, dungeon->rooms[0].br->x-ROOM_BORDER_WIDTH, NULL);
	
	pc = character_init(&(dungeon->cells[loc_y][loc_x]), CharacterType_PC);
	dungeon->pc = pc;
	
	return;
}

void dungeon_generate_npcs(Dungeon *dungeon) {
	
	/*
		non-player characters (npcs) are placed at a random location in the dungeon (excluding rooms[0])
	*/
	int i = 0;
	
	//int num_npcs = utils_rand_between(1, DUNGEON_MAX_NPCS. NULL);
	int num_npcs = 0;
	Character *npcs = (Character*)malloc(sizeof(Character) * num_npcs);
	
	for (i = 0; i < num_npcs; i++) {
		
		int room_index = utils_rand_between(1, dungeon->num_rooms-1, NULL);
		
		int loc_y = utils_rand_between(dungeon->rooms[room_index].tl->y+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br->y-ROOM_BORDER_WIDTH, NULL);
		int loc_x = utils_rand_between(dungeon->rooms[room_index].tl->x+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br->x-ROOM_BORDER_WIDTH, NULL);
		
		npcs[i] = character_init(&(dungeon->cells[loc_y][loc_x]), CharacterType_NPC);
	}
	
	dungeon->num_npcs = num_npcs;
	dungeon->npcs = npcs;
	
	return;
}

void dungeon_print(Dungeon dungeon, int print_border) {
	
	int i = 0, j = 0, k = 0;

	printf("\n");
	
	for (i = 0; i < dungeon.height; i++) {
		for (j = 0; j < dungeon.width; j++) { 
			
			int cell_empty = 1;
			
			if (dungeon.pc.location->y == i && dungeon.pc.location->x == j) { //print pc

				cell_empty = 0;
				character_print(dungeon.pc);
			}
			for (k = 0; k < dungeon.num_npcs; k++) { //print npcs
				
				if (dungeon.npcs[k].location->y == i && dungeon.npcs[k].location->x == j) {
					
					cell_empty = 0;
					
					character_print(dungeon.npcs[k]);
				}
			}
			if (cell_empty) { cell_print(dungeon.cells[i][j], print_border); }
		}
		
		printf("\n");
	}
	
	printf("\n");
	
	return;
}