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


/****** function definitions ******/
void dungeon_init(Dungeon *dungeon, uint8_t dungeon_height, uint8_t dungeon_width) {
	
//	int dungeon_area = dungeon_height * dungeon_width;
	
	srand(time(NULL));
	
	/* initialize dungeon memory */
	dungeon_mem_init(dungeon, dungeon_height, dungeon_width);

	/* initialize dungeon cells */
    dungeon_generate_cells(dungeon);
	
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
	
	int i = 0;
	
	srand(time(NULL));
	
	dungeon->height = dungeon_height;
	dungeon->width = dungeon_width;
	dungeon->volitile_height = dungeon_height-(DUNGEON_BORDER_WIDTH*2); //initialize dungeon height
	dungeon->volitile_width = dungeon_width-(DUNGEON_BORDER_WIDTH*2); //initialize dungeon width
	dungeon->num_cells = dungeon->height * dungeon->width;
	
	/* allocate memory for and initialize Dungeon cells */
	dungeon->cells = (Cell**)calloc(dungeon->height, sizeof(Cell*));
    for(i = 0; i < dungeon->height; i++) {
		
		dungeon->cells[i] = (Cell*)calloc(dungeon->width, sizeof(Cell)); 
//		for (j = 0; j < dungeon->width; j++) { dungeon->cells[i][j] = cell_init(i, j, 1); }
	}
	
	dungeon->num_rooms = 0;
	dungeon->rooms = NULL;
	
	dungeon->num_staircases_up = 0;
	dungeon->staircases_up = NULL;
	dungeon->num_staircases_down = 0;
	dungeon->staircases_down = NULL;
	
	dungeon->npc_id_next = 1;
	dungeon->num_npcs = 0;
	dungeon->npcs = NULL;
	
	return;
}

void dungeon_generate_cells(Dungeon *dungeon) {
	
	int i = 0, j = 0;
	
	for(i = 0; i < dungeon->height; i++) {
        for (j = 0; j < dungeon->width; j++) {
			
			dungeon->cells[i][j] = cell_init(i, j,-1);
			
//			printf("debug -- cell hardness: [%d]\n", dungeon->cells[i][j].hardness);
			
			if (i < DUNGEON_BORDER_WIDTH || j < DUNGEON_BORDER_WIDTH || i >= dungeon->height-DUNGEON_BORDER_WIDTH || j >= dungeon->width-DUNGEON_BORDER_WIDTH) {
				
				dungeon->cells[i][j].type = CellType_Border;
				dungeon->cells[i][j].hardness = 255;
			}
		}
    }
}

void dungeon_generate_rooms(Dungeon *dungeon) {
	
	int i = 0, j = 0, k = 0;
	int num_rooms = 0;
	int room_min_area = ROOM_MIN_HEIGHT * ROOM_MIN_WIDTH;
	int room_max_area = ROOM_MIN_HEIGHT * ROOM_MIN_WIDTH;
	
	num_rooms = utils_rand_between(DUNGEON_MIN_ROOM, DUNGEON_MAX_ROOM, NULL); //get random number of rooms for dungeon between min and max number of rooms possible
	room_max_area = (dungeon->volitile_height * dungeon->volitile_width) / (ROOM_BORDER_WIDTH*4*num_rooms);
	
	dungeon->rooms = (Room*)calloc(num_rooms, sizeof(Room)); //allocate room memory
	
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
		for (j = room.tl.y; j < room.br.y; j++) {
			for (k = room.tl.x; k < room.br.x; k++) {
				
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
	
	Coordinate loc_tl = {
		
		.y = utils_rand_between(ROOM_BORDER_WIDTH, room_max_y, NULL),
		.x = utils_rand_between(ROOM_BORDER_WIDTH, room_max_x, NULL)
	};
	Coordinate loc_br = {
		
		.y = loc_tl.y + room->height,
		.x = loc_tl.x + room->width
	};
	Coordinate loc_center = {
		
		.y = loc_tl.y + (room->height / 2),
		.x = loc_tl.x + (room->width / 2)
	};
	
	room_init_cells(room, loc_tl, loc_br, loc_center);
	
	while (rooms_intersect(dungeon->num_rooms, dungeon->rooms, room, ROOM_BORDER_WIDTH) && iterations < MAX_ROOM_ITERS) { //if the room intersects another room, find new coordinates
		
			
		loc_tl.y = utils_rand_between(ROOM_BORDER_WIDTH, room_max_y, NULL);
		loc_tl.x = utils_rand_between(ROOM_BORDER_WIDTH, room_max_x, NULL);
		
		loc_br.y = loc_tl.y + room->height;
		loc_br.x = loc_tl.x + room->width;
		
		loc_center.y = loc_tl.y + (room->height / 2);
		loc_center.x = loc_tl.x + (room->width / 2);
	
		room_init_cells(room, loc_tl, loc_br, loc_center);
		
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
		
		int delta_y = beta->center.y - alpha->center.y;
		int delta_x = beta->center.x - alpha->center.x;
		
		/* start at random spot on edge of room alpha */
		Coordinate cell_start = {
			
			.y = utils_rand_between(alpha->tl.y, alpha->br.y, NULL),
			.x = utils_rand_between(alpha->tl.x, alpha->br.x, NULL)
		};
		/* end at random spot on edge of room beta */
		Coordinate cell_end = {
			
			.y = utils_rand_between(beta->tl.y, beta->br.y, NULL),
			.x = utils_rand_between(beta->tl.x, beta->br.x, NULL)
		};
		
		if (delta_y < 0) { //room beta is above room alpha
		
			cell_start.y = alpha->tl.y;
			cell_end.y = beta->br.y;
		}
		if (delta_x > 0) { //room beta is to the right of room alpha
		
			cell_start.x = alpha->br.x;
			cell_end.x = beta->tl.x;
		}
			
		dungeon_generate_cooridor(dungeon, cell_start, cell_end);
		
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

void dungeon_generate_cooridor(Dungeon *dungeon, Coordinate start, Coordinate end) {
	
	Cell *cell;
	
//	float step_deviation_chance = 25.0;
	int step = 0;
	int step_prev_y = 0;
	int step_prev_x = 0;
	int step_count_y = 0;
	int step_count_x = 0;
	int delta_y = end.y - start.y;
	int delta_x = end.x - start.x;
	
	while(delta_x != 0 || delta_y != 0) {
		
		cell = &(dungeon->cells[start.y][start.x]);
		
		/* if cell is not a room, mark as a cooridor */
		if (cell->type != CellType_Room) {
			
			cell->type = CellType_Cooridor; 
			cell->hardness = 0; 
		}
		
		/* get next closest cell between room1 and room2, converge deltas to 0 */
		if (delta_y > 0) { //if y target is below current y, increase y coordinate
			
			step = 1;
			if (step_prev_y != 0 && step_prev_x != 0 && dungeon->cells[start.y+step][start.x].hardness > dungeon->cells[start.y][start.x].hardness) { step = 0; } //if the cell currently stepping to has a hardness greater than the 1 currently on and last round a new cooridor was made, do not step there
			
			start.y += step;
			step_count_y++;
		}
		else if (delta_y < 0) { //if y target is above current y, decrease y coordinate 
		
			step = -1;
			if (step_prev_y != 0 && step_prev_x != 0 && dungeon->cells[start.y+step][start.x].hardness > dungeon->cells[start.y][start.x].hardness) { step = 0; } //if the cell currently stepping to has a hardness greater than the 1 currently on and last round a new cooridor was made, do not step there
			
			start.y += step;
			step_count_y++;
		}
		step_prev_y = step;
		if (delta_x > 0) { //if x target is right of current x, increase x coordinate
		
			step = 1;
			if (step_prev_y != 0 && step_prev_x != 0 && dungeon->cells[start.y][start.x+step].hardness > dungeon->cells[start.y][start.x].hardness) { step = 0; } //if the cell currently stepping to has a hardness greater than the 1 currently on and last round a new cooridor was made, do not step there
			if (step_prev_y != 0 && step != 0) { //if stepping diagonally, fill in the corner cell
				
				Cell *corner = NULL;
				if (utils_rand_chance(0.50, NULL)) { corner = &(dungeon->cells[start.y][start.x]); }
				else { corner = &(dungeon->cells[start.y-step_prev_y][start.x+step]); }
				
				if (corner->type != CellType_Room) {
			
					corner->type = CellType_Cooridor; 
					corner->hardness = 0; 
				}
			}
			
			start.x += step;
			step_count_x++;
		}
		else if (delta_x < 0) { //if x target is left of current x, decrease x coordinate
		
			step = -1;
			if (step_prev_y != 0 && step_prev_x != 0 && dungeon->cells[start.y][start.x+step].hardness > dungeon->cells[start.y][start.x].hardness) { step = 0; } //if the cell currently stepping to has a hardness greater than the 1 currently on and last round a new cooridor was made, do not step there
			if (step_prev_y != 0 && step != 0) { //if stepping diagonally, fill in the corner cell
				
				Cell *corner = NULL;
				if (utils_rand_chance(0.50, NULL)) { corner = &(dungeon->cells[start.y][start.x]); }
				else { corner = &(dungeon->cells[start.y-step_prev_y][start.x+step]); }
				
				if (corner->type != CellType_Room) {
			
					corner->type = CellType_Cooridor; 
					corner->hardness = 0; 
				}
			}
			
			start.x += step;
			step_count_x++;
		}
		step_prev_x = step;
		
		delta_y = end.y - start.y;
		delta_x = end.x - start.x;
	}
	
	/* connect last cell */
	cell = &(dungeon->cells[start.y][start.x]);
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
	dungeon->staircases_down = (Cell**)calloc(num_staircase_down, sizeof(Cell*));
	dungeon->num_staircases_up = num_staircase_up;
	dungeon->staircases_up = (Cell**)calloc(num_staircase_up, sizeof(Cell*));
	
	/* place down staircases */
	for (i = 0; i < num_staircase_down; i++) {
		
		int room_index = dungeon->num_rooms-1;
		if (i != 0) { room_index = utils_rand_between(1, dungeon->num_rooms-2, NULL); }
		
		Coordinate loc = {
			
			.y = utils_rand_between(dungeon->rooms[room_index].tl.y+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br.y-ROOM_BORDER_WIDTH, NULL),
			.x = utils_rand_between(dungeon->rooms[room_index].tl.x+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br.x-ROOM_BORDER_WIDTH, NULL)
		};
		
		dungeon->cells[loc.y][loc.x].type = CellType_Stair_down;
		dungeon->cells[loc.y][loc.x].hardness = 0;
		
		dungeon->staircases_down[i] = &(dungeon->cells[loc.y][loc.x]);
	}
	/* place up staircases */
	for (i = 0; i < num_staircase_up; i++) {
		
		int room_index = utils_rand_between(1, dungeon->num_rooms-2, NULL);
		
		Coordinate loc = {
			
			.y = utils_rand_between(dungeon->rooms[room_index].tl.y+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br.y-ROOM_BORDER_WIDTH, NULL),
			.x = utils_rand_between(dungeon->rooms[room_index].tl.x+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br.x-ROOM_BORDER_WIDTH, NULL)
		};
		
		dungeon->cells[loc.y][loc.x].type = CellType_Stair_up;
		dungeon->cells[loc.y][loc.x].hardness = 0;
		
		dungeon->staircases_up[i] = &(dungeon->cells[loc.y][loc.x]);
	}
	
	return;
}

void dungeon_generate_pc(Dungeon *dungeon) {
	
	/*
		player character (pc) is placed at a random location in rooms[0]
	*/
	
	Character pc;
	
	Coordinate loc = {
		
		.y = utils_rand_between(dungeon->rooms[0].tl.y+ROOM_BORDER_WIDTH, dungeon->rooms[0].br.y-ROOM_BORDER_WIDTH, NULL),
		.x = utils_rand_between(dungeon->rooms[0].tl.x+ROOM_BORDER_WIDTH, dungeon->rooms[0].br.x-ROOM_BORDER_WIDTH, NULL)
	};
	
	pc = character_init(0, loc, CharacterType_PC);
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
	Character *npcs = (Character*)calloc(num_npcs, sizeof(Character));
	
	for (i = 0; i < num_npcs; i++) {
		
		int room_index = utils_rand_between(1, dungeon->num_rooms-1, NULL);
		
		Coordinate loc = {
		
			.y = utils_rand_between(dungeon->rooms[room_index].tl.y+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br.y-ROOM_BORDER_WIDTH, NULL),
			.x = utils_rand_between(dungeon->rooms[room_index].tl.x+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br.x-ROOM_BORDER_WIDTH, NULL)
		};
		
		npcs[i] = character_init(dungeon->npc_id_next, loc, CharacterType_NPC);
		dungeon->npc_id_next++;
	}
	
	dungeon->num_npcs = num_npcs;
	dungeon->npcs = npcs;
	
	return;
}

void dungeon_print(Dungeon dungeon, int print_fill, int print_color, int print_weight) {
	
	int i = 0, j = 0, k = 0;

	printf("\n");
	
	for (i = -2; i < dungeon.height; i++) {
		
		if (print_weight == 1 && i > -1) { printf("%2d - ", i+1); }
		
		for (j = 0; j < dungeon.width; j++) {
			
			int cell_empty = 1;
			
			if (print_weight == 1 && i == -2) {
				
				if (j == 0) { printf("     %2d", j);  }
				else { printf("%2d", j); }
			}
			else if (print_weight == 1 && i == -1) {
				
				if (j == 0) { printf("     __"); }
				else { printf("__"); }
			}
			else if (i > -1) {
				
				if (dungeon.pc.location.y == i && dungeon.pc.location.x == j) { //print pc

					cell_empty = 0;
					character_print(dungeon.pc, print_color);
				}
				for (k = 0; k < dungeon.num_npcs; k++) { //print npcs
					
					if (dungeon.npcs[k].location.y == i && dungeon.npcs[k].location.x == j) {
						
						cell_empty = 0;
						
						character_print(dungeon.npcs[k], print_color);
					}
				}
				if (cell_empty) { cell_print(dungeon.cells[i][j], print_fill, print_weight); }
			}
		}
		
		printf("\n");
	}
	
	printf("\n");
	
	return;
}