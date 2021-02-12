/******** include std libs ********/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
/******* include custom libs ******/
#include "level.h"
#include "utils.h"

/********** definitions **********/
#define MAX_ROOM_ITERS 100

/*********** global vars **********/

struct Level level_init(int level_height, int level_width) {
	
	int i = 0, j = 0;
	
	struct Level level;
	
//	int level_area = level_height * level_width;
	
	srand(time(NULL));
	
	/* initialize level vars */
	level.print_height = level_height;
	level.print_width = level_width;
	level.height = level_height-(LEVEL_BORDER_WIDTH*2); //initialize level height
	level.width = level_width-(LEVEL_BORDER_WIDTH*2); //initialize level width
	level.num_cells = level.height * level.width;
	level.num_rooms = 0;
	
	/* allocate memory for and initialize Level cells */
	level.cells = (struct Cell**)malloc(sizeof(struct Cell*) * level_height);
    for(i = 0; i < level_height; i++) {

        level.cells[i] = (struct Cell*)malloc(sizeof(struct Cell) * level_width);
        for (j = 0; j < level_width; j++) {
			
			level.cells[i][j] = cell_init_rand(i, j);
			
			if (i < LEVEL_BORDER_WIDTH || i >= level_height-LEVEL_BORDER_WIDTH) { level.cells[i][j].type = Border_h; }
			else if (j < LEVEL_BORDER_WIDTH || j >= level_width-LEVEL_BORDER_WIDTH) { level.cells[i][j].type = Border_v; }
		}
    }
	
	/* generate and sort rooms */
	level_generate_rooms(&level);
	
	//level_print(level, 0);
	
	/* generate cooridors */
	level_generate_cooridors(&level);
	
	/* add staircases */
	level_generate_staircases(&level);
	
	return level;
}

void level_generate_rooms(struct Level *level) {
	
	int i = 0, j = 0, k = 0;
	int num_rooms = 0;
	int room_min_area = ROOM_MIN_HEIGHT * ROOM_MIN_WIDTH;
	int room_max_area = ROOM_MIN_HEIGHT * ROOM_MIN_WIDTH;
	
	num_rooms = utils_rand_between(LEVEL_MIN_ROOM, LEVEL_MAX_ROOM, NULL); //get random number of rooms for level between min and max number of rooms possible
	room_max_area = (level->height * level->width) / (ROOM_BORDER_WIDTH*4*num_rooms);
	
	level->rooms = (struct Room*)malloc(sizeof(struct Room) * num_rooms); //allocate room memory
	
	for (i = 0; i < num_rooms; i++) {
		
		struct Room room;
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
			
			/* place room in level */
			room_placed = level_place_room(level, &room);
		}
		
		level->rooms[i] = room; //add room to arr
		level->num_rooms++; //increase room count
		
		/* mark room cells as rooms */
		for (j = room.tl->y; j < room.br->y; j++) {
			for (k = room.tl->x; k < room.br->x; k++) {
				
				level->cells[j][k].type = Room;
				level->cells[j][k].hardness = 0;
			}
		}
		
		room.connected = 0;
	}
	
	return;
}

int level_place_room(struct Level *level, struct Room *room) {
	
	int iterations = 0;

	int room_max_y = level->height - room->height - (ROOM_BORDER_WIDTH);
	int room_max_x = level->width - room->width - (ROOM_BORDER_WIDTH);
	
	int cell_y = utils_rand_between(ROOM_BORDER_WIDTH, room_max_y, NULL);
	int cell_x = utils_rand_between(ROOM_BORDER_WIDTH, room_max_x, NULL);
	
	room_init_cells(room, &(level->cells[cell_y][cell_x]), &(level->cells[cell_y + room->height][cell_x + room->width]), &(level->cells[cell_y + (room->height / 2)][cell_x + (room->width / 2)]));
	
	while (rooms_intersect(level->num_rooms, level->rooms, room, ROOM_BORDER_WIDTH) && iterations < MAX_ROOM_ITERS) { //if the room intersects another room, find new coordinates
		
		cell_y = utils_rand_between(ROOM_BORDER_WIDTH, room_max_y, NULL);
		cell_x = utils_rand_between(ROOM_BORDER_WIDTH, room_max_x, NULL);
	
		room_init_cells(room, &(level->cells[cell_y][cell_x]), &(level->cells[cell_y + room->height][cell_x + room->width]), &(level->cells[cell_y + (room->height / 2)][cell_x + (room->width / 2)]));
		
		iterations++;
	}
	
	if (iterations >= MAX_ROOM_ITERS) { return 0; }
	
	return 1;
}

void level_generate_cooridors(struct Level *level) {
	
	/* 
		Old strategy: 
		Find shortest path between 0 and 1, and draw a direct path to it. Do same for 1 and 2, and so on to num_rooms - 1
		New strategy:
		Find closest room to room 0, draw direct path to it if both rooms are not already connected, and there is an unconnected room. Else, draw direct path to closest room. Do same for 1 and 2 and so on to num_rooms - 1
	*/
	
	int i = 0;
	
//	for (i = 0; i < level->num_rooms-1; i++) { //CHANGE BOUNDS TO [level->num_rooms-1] for release
//		
//		struct Room *room1 = &(level->rooms[i]);
//		struct Room *room2 = &(level->rooms[i+1]);
//		
//		int delta_y = room2->center->y - room1->center->y;
//		int delta_x = room2->center->x - room1->center->x;
//		
//		/* start at random spot on edge of room1 */
//		int cell_start_y = utils_rand_between(room1->tl->y, room1->br->y, NULL);
//		int cell_start_x = utils_rand_between(room1->tl->x, room1->br->x, NULL);
//		/* end at random spot on edge of room2 */
//		int cell_end_y = utils_rand_between(room2->tl->y, room2->br->y, NULL);
//		int cell_end_x = utils_rand_between(room2->tl->x, room2->br->x, NULL);
//		
//		if (delta_y < 0) { //room2 is above room1
//		
//			cell_start_y = room1->tl->y;
//			cell_end_y = room2->br->y;
//		}
//		if (delta_x > 0) { //room2 is to the right of room1
//		
//			cell_start_x = room1->br->x;
//			cell_end_x = room2->tl->x;
//		}
//			
//		level_generate_cooridor(level, cell_start_y, cell_start_x, cell_end_y, cell_end_x);
//		
//		room1->connected = 1;
//		room2->connected = 1;
//	}
	
	int num_connected_rooms = 0;
	struct Room connected_rooms[level->num_rooms];
	
	while (num_connected_rooms < level->num_rooms) {
		
		struct Room *alpha = &(level->rooms[i]);
		struct Room *beta = room_find_closest(level->num_rooms, level->rooms, num_connected_rooms, connected_rooms, alpha);
		
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
			
		level_generate_cooridor(level, cell_start_y, cell_start_x, cell_end_y, cell_end_x);
		
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
		
//		level_print(*level, 0);
		
//		char temp;
//		scanf("%c", &temp);
	}
	
	return;
}

void level_generate_cooridor(struct Level *level, int start_y, int start_x, int end_y, int end_x) {
	
	struct Cell *cell;
	
//	float step_deviation_chance = 25.0;
	int step = 0;
	int step_count_y = 0;
	int step_count_x = 0;
	int delta_y = end_y - start_y;
	int delta_x = end_x - start_x;
	
	while(delta_x != 0 || delta_y != 0) {
		
		cell = &(level->cells[start_y][start_x]);
		
		/* if cell is not a room, mark as a cooridor */
		if (cell->type != Room) { cell->type = Cooridor; }
		
		/* get next closest cell between room1 and room2, converge deltas to 0 */
		if (delta_y > 0) { //if y target is below current y, increase y coordinate
			
			step = 1;
//			if (utils_rand_chance(step_deviation_chance, NULL) && start_y-1 > level->height && step_count_y < LEVEL_MAX_CHANCE_COUNT) { step = -1; }
			
			start_y += step;
			step_count_y++;
		}
		else if (delta_y < 0) { //if y target is above current y, decrease y coordinate 
		
			step = -1;
//			if (utils_rand_chance(step_deviation_chance, NULL) && start_y+1 < level->height && step_count_y < LEVEL_MAX_CHANCE_COUNT) { step = 1; }
			
			start_y += step;
			step_count_y++;
		} else { //random chance of adding or subtracting 1 from delta_y
			
			step = 0;
//			if (utils_rand_chance(step_deviation_chance, NULL) && start_y-1 > level->height) { step = -1; }
//			if (utils_rand_chance(step_deviation_chance, NULL) && start_y+1 < level->height) { step = 1; }
			
			start_y += step;
			step_count_y++;
		}
		
		if (delta_x > 0) { //if x target is right of current x, increase x coordinate
		
			step = 1;
//			if (utils_rand_chance(step_deviation_chance, NULL) && start_x-1 > level->width && step_count_x < LEVEL_MAX_CHANCE_COUNT) { step = -1; }
			
			start_x += step;
			step_count_x++;
		}
		else if (delta_x < 0) { //if x target is left of current x, decrease x coordinate
		
			step = -1;
//			if (utils_rand_chance(step_deviation_chance, NULL) && start_x+1 < level->width && step_count_x < LEVEL_MAX_CHANCE_COUNT) { step = 1; }
			
			start_x += step;
			step_count_x++;
		} else { //random chance of adding or subtracting 1 from delta_y
			
			step = 0;
//			if (utils_rand_chance(step_deviation_chance, NULL) && start_x-1 > level->width && step_count_x < LEVEL_MAX_CHANCE_COUNT) { step = -1; }
//			if (utils_rand_chance(step_deviation_chance, NULL) && start_x+1 < level->width && step_count_x < LEVEL_MAX_CHANCE_COUNT) { step = 1; }
			
			start_x += step;
			step_count_x++;
		}
		
		delta_y = end_y - start_y;
		delta_x = end_x - start_x;
	}
	
	/* connect last cell */
	cell = &(level->cells[start_y][start_x]);
	if (cell->type != Room) { cell->type = Cooridor; }

	return;
}

void level_generate_staircases(struct Level *level) {
	
	/*
		1 up staircase is always placed at a random location in rooms[0]. Any more are randomly placed (excluding rooms[0] and rooms[num_rooms - 1])
		1 down staircase is always placed at a random location in rooms[num_rooms-1].  Any more are randomly placed (excluding rooms[0] and rooms[num_rooms - 1])
	*/
	
	int i = 0;
	
	int num_staircase_down = utils_rand_between(LEVEL_MIN_STAIRCASE_DOWN, LEVEL_MAX_STAIRCASE_DOWN, NULL);
	int num_staircase_up = utils_rand_between(LEVEL_MIN_STAIRCASE_UP, LEVEL_MAX_STAIRCASE_UP, NULL);
	
	/* place down staircases */
	for (i = 0; i < num_staircase_down; i++) {
		
		int room_index = level->num_rooms-1;
		if (i != 0) { room_index = utils_rand_between(1, level->num_rooms-2, NULL); }
		
		int stair_y = utils_rand_between(level->rooms[room_index].tl->y+ROOM_BORDER_WIDTH, level->rooms[room_index].br->y-ROOM_BORDER_WIDTH, NULL);
		int stair_x = utils_rand_between(level->rooms[room_index].tl->x+ROOM_BORDER_WIDTH, level->rooms[room_index].br->x-ROOM_BORDER_WIDTH, NULL);
		
		level->cells[stair_y][stair_x].type = Stair_down;
		level->cells[stair_y][stair_x].hardness = 0;
	}
	/* place up staircases */
	for (i = 0; i < num_staircase_up; i++) {
		
		int room_index = 0;
		if (i != 0) { room_index = utils_rand_between(1, level->num_rooms-2, NULL); }
		
		int stair_y = utils_rand_between(level->rooms[room_index].tl->y+ROOM_BORDER_WIDTH, level->rooms[room_index].br->y-ROOM_BORDER_WIDTH, NULL);
		int stair_x = utils_rand_between(level->rooms[room_index].tl->x+ROOM_BORDER_WIDTH, level->rooms[room_index].br->x-ROOM_BORDER_WIDTH, NULL);
		
		level->cells[stair_y][stair_x].type = Stair_up;
		level->cells[stair_y][stair_x].hardness = 0;
	}
	
	return;
}

void level_print(struct Level level, int print_border) {
	
	int i, j;

	printf("\n");
	
	for (i = 0; i < level.print_height; i++) {
		
		for (j = 0; j < level.print_width; j++) { cell_print(level.cells[i][j], print_border); } //print cell
		
		printf("\n");
	}
	
	printf("\n");
	
	return;
}