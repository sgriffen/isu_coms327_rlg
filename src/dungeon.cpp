/******** include std libs ********/
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
/******* include custom libs ******/
#include "./dungeon.h"
#include "./utils/pathfinder.h"
#include "./utils/math_utils.h"

/********** definitions **********/
#define MAX_ROOM_ITERS 100

/*********** global vars **********/


/****** function definitions ******/
Dungeon dungeon_init(uint8_t dungeon_height, uint8_t dungeon_width, int num_npcs, int num_stairs_up, int num_stairs_down, int ommit_stairs) {
	
//	int dungeon_area = dungeon_height * dungeon_width;
	
	Dungeon dungeon;
	
	/* initialize dungeon memory */
	dungeon_mem_init(&dungeon, dungeon_height, dungeon_width);

	/* initialize dungeon cells */
    dungeon_generate_cells(&dungeon);
	
	/* generate and sort rooms */
	dungeon_generate_rooms(&dungeon);
	
	/* generate cooridors */
	dungeon_generate_cooridors(&dungeon);
	
	/* add staircases */
	dungeon_generate_staircases(&dungeon, num_stairs_up, num_stairs_down, ommit_stairs);
	
	/* add pc and npcs */
//	dungeon_generate_pc(dungeon);
	dungeon_generate_npcs(&dungeon, num_npcs);
	
	return dungeon;
}

void dungeon_finit(Dungeon *dungeon, uint8_t f_pc_y, uint8_t f_pc_x, uint8_t *f_cells, uint16_t f_num_rooms, uint8_t *f_rooms, uint16_t f_num_stairs_up, uint8_t *f_stairs_up, uint16_t f_num_stairs_down, uint8_t *f_stairs_down) {
	
	int i = 0, j = 0, k = 0;
	
	/* set dungeon cell hardness */
	dungeon_generate_cells(dungeon);
	for (i = 0; i < dungeon->height; i++) {
		for (j = 0; j < dungeon->width; j++) {
			
			dungeon->cells[i][j].hardness = f_cells[k];
			k++;
		}
	}
	/* mark and place dungeon rooms */
	dungeon->num_rooms = f_num_rooms;
	dungeon->rooms = (Room*)calloc(f_num_rooms, sizeof(Room));
	for (i = 0; i < (f_num_rooms*4); i+=4) {
		
		Room room = room_init((i/4), f_rooms[i+3], f_rooms[i+2]);
		
		room.tl = dungeon->cells[f_rooms[i+1]][f_rooms[i]].location;
		room.br = dungeon->cells[(room.tl.y)+room.height][(room.tl.x)+room.width].location;
		room.center = dungeon->cells[(room.tl.y)+(room.height/2)][(room.tl.x)+(room.width/2)].location;
		
		room.connected = 1;
		
		dungeon->rooms[i/4] = room;
		
		/* mark cells in dungeon as CellType Room  */
		for (j = room.tl.y; j < room.br.y; j++) {
			for (k = room.tl.x; k < room.br.x; k++) {
				
				dungeon->cells[j][k].type_next = CellType_Room;
				dungeon->cells[j][k].hardness = 0;
			}
		}
	}
	/* mark dungeon cooridors */
	for (i = 0; i < dungeon->height; i++) {
		for (j = 0; j < dungeon->width; j++) {
			
			if (dungeon->cells[i][j].hardness == 0 && dungeon->cells[i][j].type_next != CellType_Room) { dungeon->cells[i][j].type_next = CellType_Cooridor; }
		}
	}
	/* place up staircases in dungeon */
	dungeon->num_staircases_up = f_num_stairs_up;
	dungeon->staircases_up = (Cell**)calloc(f_num_stairs_up, sizeof(Cell*));
	for (i = 0; i < (f_num_stairs_up*2); i+=2) {
		
		dungeon->cells[f_stairs_up[i+1]][f_stairs_up[i]].type_next = CellType_Stair_up;
		dungeon->cells[f_stairs_up[i+1]][f_stairs_up[i]].hardness = 0;
		dungeon->staircases_up[i/2] = &(dungeon->cells[f_stairs_up[i+1]][f_stairs_up[i]]);
	}
	/* place down staircases in dungeon */
	dungeon->num_staircases_down = f_num_stairs_down;
	dungeon->staircases_down = (Cell**)calloc(f_num_stairs_down, sizeof(Cell*));
	for (i = 0; i < (f_num_stairs_down*2); i+=2) {
		
		dungeon->cells[f_stairs_down[i+1]][f_stairs_down[i]].type_next = CellType_Stair_down;
		dungeon->cells[f_stairs_down[i+1]][f_stairs_down[i]].hardness = 0;
		dungeon->staircases_down[i/2] = &(dungeon->cells[f_stairs_down[i+1]][f_stairs_down[i]]);
	}
//	/* place pc in dungeon */
//	Coordinate pc_loc = {
//		
//		.y = f_pc_y,
//		.x = f_pc_x
//	};
//	PC pc = character_init_pc(pc_loc, 1, 1, PC_SPEED);
//	dungeon->pc = pc;

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
	dungeon->num_volitile_cells = dungeon->volitile_height * dungeon->volitile_width;
	
	/* allocate memory for and initialize Dungeon cells */
	dungeon->cells = (Cell**)calloc(dungeon->height, sizeof(Cell*));
    for(i = 0; i < dungeon->height; i++) { dungeon->cells[i] = (Cell*)calloc(dungeon->width, sizeof(Cell)); }
	
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
			if (i < DUNGEON_BORDER_WIDTH || j < DUNGEON_BORDER_WIDTH || i >= dungeon->height-DUNGEON_BORDER_WIDTH || j >= dungeon->width-DUNGEON_BORDER_WIDTH) {
				
				dungeon->cells[i][j].type_next = CellType_Border;
				dungeon->cells[i][j].type_current = CellType_Border;
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
				
				dungeon->cells[j][k].type_next = CellType_Room;
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
	
	Coordinate loc_tl;
	Coordinate loc_br;
	Coordinate loc_center;
		
	loc_tl.y = (uint8_t)utils_rand_between(ROOM_BORDER_WIDTH, room_max_y, NULL);
	loc_tl.x = (uint8_t)utils_rand_between(ROOM_BORDER_WIDTH, room_max_x, NULL);
	loc_br.y = loc_tl.y + room->height;
	loc_br.x = loc_tl.x + room->width;
	loc_center.y = loc_tl.y + (room->height / 2);
	loc_center.x = loc_tl.x + (room->width / 2);
	
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
		
		Coordinate cell_start;
		Coordinate cell_end;
		/* start at random spot on edge of room alpha */
		cell_start.y = utils_rand_between(alpha->tl.y, alpha->br.y, NULL);
		cell_start.x = utils_rand_between(alpha->tl.x, alpha->br.x, NULL);
		/* end at random spot on edge of room beta */
		cell_end.y = utils_rand_between(beta->tl.y, beta->br.y, NULL);
		cell_end.x = utils_rand_between(beta->tl.x, beta->br.x, NULL);
		
		if (delta_y < 0) { //room beta is above room alpha
		
			cell_start.y = alpha->tl.y;
			cell_end.y = beta->br.y;
		}
		if (delta_x > 0) { //room beta is to the right of room alpha
		
			cell_start.x = alpha->br.x;
			cell_end.x = beta->tl.x;
		}
		
//		pathfinder_rewind(dungeon, &cell_start, &cell_end, dungeon_draw_cooridor);
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
		if (cell->type_next != CellType_Room) {
			
			cell->type_next = CellType_Cooridor; 
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
				if (utils_rand_chance(35, NULL)) { corner = &(dungeon->cells[start.y][start.x]); }
				else { corner = &(dungeon->cells[start.y-step_prev_y][start.x+step]); }
				
				if (corner->type_next != CellType_Room) {
			
					corner->type_next = CellType_Cooridor; 
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
				if (utils_rand_chance(35, NULL)) { corner = &(dungeon->cells[start.y][start.x]); }
				else { corner = &(dungeon->cells[start.y-step_prev_y][start.x+step]); }
				
				if (corner->type_next != CellType_Room) {
			
					corner->type_next = CellType_Cooridor; 
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
	if (cell->type_next != CellType_Room) {
		
		cell->type_next = CellType_Cooridor; 
		cell->hardness = 0; 
	}

	return;
}
int dungeon_draw_cooridor(Dungeon *dungeon, QueueNode *nodes, Coordinate *start) {
	
	QueueNode node = nodes[0];
	Coordinate *loc = (Coordinate*)(node.element);
	while (!coordinate_is_same(*loc, *start)) {
		
		if (dungeon->cells[loc->y][loc->x].type_next != CellType_Room) {
			
			dungeon->cells[loc->y][loc->x].type_next = CellType_Cooridor;
			dungeon->cells[loc->y][loc->x].hardness = 0;
		} else {
			
			Room* node_room = room_contains_point(dungeon->num_rooms, &(dungeon->rooms), *loc);
			if (node_room) { node_room->connected = 1; }
		}
		
		node = *(node.from);
	}
	
	if (dungeon->cells[loc->y][loc->x].type_next != CellType_Room) { //mark first cell in node array
			
		dungeon->cells[loc->y][loc->x].type_next = CellType_Cooridor;
		dungeon->cells[loc->y][loc->x].hardness = 0;
	}
	
	return 0;
}

void dungeon_generate_staircases(Dungeon *dungeon, int num_staircase_up, int num_staircase_down, int ommit_stairs) {
	
	/*
		up staircases are are randomly placed (excluding rooms[0] and rooms[num_rooms - 1])
		1 down staircase is always placed at a random location in rooms[num_rooms-1].  Any more are randomly placed (excluding rooms[0] and rooms[num_rooms - 1])
	*/
	
	int i = 0;
	
	if (!num_staircase_up) 		{ num_staircase_up = utils_rand_between(DUNGEON_MIN_STAIRCASE_UP, DUNGEON_MAX_STAIRCASE_UP, NULL); }
	if (!num_staircase_down) 	{ num_staircase_down = utils_rand_between(DUNGEON_MIN_STAIRCASE_DOWN, DUNGEON_MAX_STAIRCASE_DOWN, NULL); }
	
	/* initialize struct data */
	dungeon->num_staircases_down = num_staircase_down;
	dungeon->staircases_down = (Cell**)calloc(num_staircase_down, sizeof(Cell*));
	dungeon->num_staircases_up = num_staircase_up;
	dungeon->staircases_up = (Cell**)calloc(num_staircase_up, sizeof(Cell*));
	
	/* place down staircases */
	if (ommit_stairs > -1) {
		
		for (i = 0; i < num_staircase_down; i++) {
			
			int room_index = dungeon->num_rooms-1;
			if (i != 0) { room_index = utils_rand_between(1, dungeon->num_rooms-2, NULL); }
			
			Coordinate loc;
			loc.y = utils_rand_between(dungeon->rooms[room_index].tl.y+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br.y-ROOM_BORDER_WIDTH, NULL);
			loc.x = utils_rand_between(dungeon->rooms[room_index].tl.x+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br.x-ROOM_BORDER_WIDTH, NULL);
			
			dungeon->cells[loc.y][loc.x].type_next = CellType_Stair_down;
			dungeon->cells[loc.y][loc.x].hardness = 0;
			
			dungeon->staircases_down[i] = &(dungeon->cells[loc.y][loc.x]);
		}
	}
	/* place up staircases */
	if (ommit_stairs < 1) {
		
		for (i = 0; i < num_staircase_up; i++) {
			
			int room_index = utils_rand_between(1, dungeon->num_rooms-2, NULL);
			
			Coordinate loc;	
			loc.y = utils_rand_between(dungeon->rooms[room_index].tl.y+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br.y-ROOM_BORDER_WIDTH, NULL);
			loc.x = utils_rand_between(dungeon->rooms[room_index].tl.x+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br.x-ROOM_BORDER_WIDTH, NULL);
			
			dungeon->cells[loc.y][loc.x].type_next = CellType_Stair_up;
			dungeon->cells[loc.y][loc.x].hardness = 0;
			
			dungeon->staircases_up[i] = &(dungeon->cells[loc.y][loc.x]);
		}
	}
	
	return;
}

void dungeon_generate_npcs(Dungeon *dungeon, int num_npcs) {
	
	/*
		non-player characters (npcs) are placed at a random location in the dungeon
		npcs that can tunnel are placed anywhere, non-tunneling npcs are placed only in rooms; all locations exclude where the pc is placed
	*/
	int i = 0, j = 0;
	int unplaced_npcs = 0;
	uint8_t speed = 0;
	
	dungeon->num_npcs_defined = 1;
	if (num_npcs < 1) {
		
		num_npcs = utils_rand_between(dungeon->num_rooms, (dungeon->num_rooms)*2, NULL);
		dungeon->num_npcs_defined = 0;
	}
	else if (num_npcs > ((dungeon->num_volitile_cells) - 1)) { num_npcs = (dungeon->num_volitile_cells) - 1; }
	dungeon->npcs =(NPC*)calloc(num_npcs, sizeof(NPC));
	
	for (i = 0; i < num_npcs; i++) {
		
		uint8_t type = ((utils_rand_chance(50, NULL)) << 3) | ((utils_rand_chance(50, NULL)) << 2) | ((utils_rand_chance(50, NULL)) << 1) | ((utils_rand_chance(50, NULL)) << 0);
//		uint8_t type = 0; //define type for debugging
		Coordinate loc;
		
		if (type & NPC_TYPE_TUNNELING) {
			
			loc.y = utils_rand_between(DUNGEON_BORDER_WIDTH, dungeon->volitile_height, NULL);
			loc.x = utils_rand_between(DUNGEON_BORDER_WIDTH, dungeon->volitile_width, NULL);
			
			while(cell_immutable_tunneling(dungeon->cells[loc.y][loc.x]) && j < DUNGEON_MAX_CHANCE_COUNT) {
				
				loc.y = utils_rand_between(DUNGEON_BORDER_WIDTH, dungeon->volitile_height, NULL);
				loc.x = utils_rand_between(DUNGEON_BORDER_WIDTH, dungeon->volitile_width, NULL);
				
				j++;
			}
            if (j >= DUNGEON_MAX_CHANCE_COUNT) { unplaced_npcs++; }
            j = 0;
		} else {
			
			int room_index = utils_rand_between(0, dungeon->num_rooms-1, NULL);
			
			loc.y = utils_rand_between(dungeon->rooms[room_index].tl.y, dungeon->rooms[room_index].br.y, NULL);
			loc.x = utils_rand_between(dungeon->rooms[room_index].tl.x, dungeon->rooms[room_index].br.x, NULL);
			
			while(cell_immutable_ntunneling(dungeon->cells[loc.y][loc.x]) && j < DUNGEON_MAX_CHANCE_COUNT) {
				
				loc.y = utils_rand_between(dungeon->rooms[room_index].tl.y+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br.y-ROOM_BORDER_WIDTH, NULL);
				loc.x = utils_rand_between(dungeon->rooms[room_index].tl.x+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br.x-ROOM_BORDER_WIDTH, NULL);
				
				j++;
			}
            if (j >= DUNGEON_MAX_CHANCE_COUNT) { unplaced_npcs++; }
            j = 0;
		}
		
		speed = utils_rand_between(NPC_SPEED_MIN, NPC_SPEED_MAX, NULL);
		
		dungeon->npcs[i] = character_init_npc(dungeon->npc_id_next, loc, type, 1, 1, speed);
		dungeon->npc_id_next++;
	}
	
	dungeon->num_npcs = num_npcs-unplaced_npcs;
    dungeon->num_npcs_dead = 0;
	
	return;
}

int dungeon_coordinate_inbounds(Coordinate coord) { return (coord.y > 0 && coord.y < DUNGEON_HEIGHT && coord.x > 0 && coord.x < DUNGEON_WIDTH); }

Coordinate dungeon_los(Dungeon dungeon, Coordinate start, Coordinate end) {

	/*
	    uses Bresenham's line algorithm to draw a line between the start cell and the end cell.
		if it comes across a cell that's immutable to non-tunneling npcs, it returns coordinate (0, 0)
		otherwise it returns the end coordinate
	    also
	    draw rays in all 8 directions until an ntunneling immutable cell is reached.
	    if the end coordinate is found before then, return the end coordinate
	    else, return (0, 0)
	 */

	Coordinate zero;
	zero.y = 0;
	zero.x = 0;

	int matrix_u[2]     =  { -1,  0 };
    int matrix_ul[2]    =  { -1,  1 };
    int matrix_l[2]     =  {  0,  1 };
    int matrix_dl[2]    =  {  1,  1 };
    int matrix_d[2]     =  {  1,  0 };
    int matrix_dr[2]    =  {  1, -1 };
    int matrix_r[2]     =  {  0, -1 };
    int matrix_ur[2]    =  { -1, -1 };
	
	if (coordinate_is_same(start, end)) { return end; }
	if (    dungeon_los_ray(dungeon, start, end, matrix_u)
        ||  dungeon_los_ray(dungeon, start, end, matrix_ul)
        ||  dungeon_los_ray(dungeon, start, end, matrix_l)
        ||  dungeon_los_ray(dungeon, start, end, matrix_dl)
        ||  dungeon_los_ray(dungeon, start, end, matrix_d)
        ||  dungeon_los_ray(dungeon, start, end, matrix_dr)
        ||  dungeon_los_ray(dungeon, start, end, matrix_r)
        ||  dungeon_los_ray(dungeon, start, end, matrix_ur)
        )
	{ return end; }
	else {

        if (abs(end.y - start.y) < abs(end.x - start.x)) {

            if (start.x > end.x) 	{ return dungeon_los_lowhi(dungeon, end, start) ? end : zero; }
            return dungeon_los_lowhi(dungeon, start, end) ? end : zero;
        } else {

            if (start.y > end.y) 	{ return dungeon_los_hilow(dungeon, end, start) ? end : zero; }
            return dungeon_los_hilow(dungeon, start, end) ? end : zero;
        }
	}
}

int dungeon_los_ray(Dungeon dungeon, Coordinate start, Coordinate end, int const ray_matrix[2]) {

	Coordinate current;
	current.y = start.y;
	current.x = start.x;

    while (!cell_immutable_ntunneling(dungeon.cells[current.y][current.x]) && coordinate_is_same(current, end)) {

        current.y += ray_matrix[0];
        current.x += ray_matrix[1];
    }

    return coordinate_is_same(current, end);
}

int dungeon_los_lowhi(Dungeon dungeon, Coordinate start, Coordinate end) {
	
	int i = 0;
	
	Coordinate next = {
		
		.y = 0,
		.x = 0
	};
	
	int delta_x = end.x - start.x;
	int delta_y = end.y - start.y;
	int inc_y = 1;
	int delta = 0;
	
	if (!delta_y) { inc_y = 0; }
	else if (delta_y < 0) {
		
		inc_y = -1;
		delta_y = 0 - delta_y;
	}
	delta = (2 * delta_y) - delta_x;
	next.y = start.y;
	
	for (i = start.x; i < end.x; i++) {
		
		next.x = i;
		if (cell_immutable_ntunneling(dungeon.cells[next.y][next.x])) { return 0; }
		if (delta > 0) {
			
			next.y = next.y + inc_y;
			delta = delta + (2 * (delta_y - delta_x));
		} else { delta = delta + (2 * delta_y); }
	}
	
	return 1;
}

int dungeon_los_hilow(Dungeon dungeon, Coordinate start, Coordinate end) {
	
	int i = 0;
	
	Coordinate next;	
	next.y = 0;
	next.x = 0;
	
	int delta_x = end.x - start.x;
	int delta_y = end.y - start.y;
	int inc_x = 1;
	int delta = 0;
	
	if (!delta_x) { inc_x = 0; }
	else if (delta_x < 0) {
		
		inc_x = -1;
		delta_x = 0 - delta_x;
	}
	delta = (2 * delta_x) - delta_y;
	next.x = start.x;
	
	for (i = start.y; i < end.y; i++) {
		
		next.y = i;
		if (cell_immutable_ntunneling(dungeon.cells[next.y][next.x])) { return 0; }
		if (delta > 0) {
			
			next.x = next.x + inc_x;
			delta = delta + (2 * (delta_x - delta_y));
		} else { delta = delta + (2 * delta_x); }
	}
	
	return 1;
}

void dungeon_pc_los(Dungeon *dungeon, Character *pc, uint8_t mark) {
	
	int i = 0;
	int j = 0;
	
	for (i = -2; i < 3; i++) {
		for (j = -2 ; j < 3; j++) {
			
			Coordinate location = pc->location;
			location.y += i;
			location.x += j;
			if (!mark && dungeon_coordinate_inbounds(location)) { dungeon->cells[location.y][location.x].visible = mark; }
			else if (dungeon_coordinate_inbounds(location)) {
				
				Coordinate los = dungeon_los(*(dungeon), pc->location, location);
				if (los.y || los.x) {
					
					dungeon->cells[location.y][location.x].visited = 1;
					dungeon->cells[location.y][location.x].visible = mark;
					dungeon->cells[location.y][location.x].type_current = dungeon->cells[location.y][location.x].type_next;
				}
			}
		}
	}
}

void dungeon_resolve_collision(Dungeon *dungeon, Character *character, Coordinate next) {
	
	/*
		Resolves potential collisions with the next cell a character wants to move to, and the current character that's in that cell
		Returns 0 if there is no collision
		Returns -1 if an npc moves into the cell the pc is located in (npc kills pc)
		Returns the id of an npc if a character moves into the cell the is located in (pc or npc kills npc)
	*/
	
	PC* pc_location = NULL;
	NPC* npc_location = NULL;
	
	if (character->id < 1) {
		
		dungeon_pc_los(dungeon, character, 0);
		
		npc_location = cell_contains_npc(dungeon->cells[next.y][next.x]);
		if (npc_location) {
			
			npc_location->hp -= character->damage;
			if (npc_location->hp < 1) {
				
				dungeon->cells[npc_location->location.y][npc_location->location.x].character = NULL;
				(dungeon->num_npcs_dead)++;
			}
		}
		
		dungeon->cells[character->location.y][character->location.x].character = NULL;
		dungeon->cells[next.y][next.x].character = character;
		
		character->prev_location = character->location;
		character->location = next;
		
		dungeon_pc_los(dungeon, character, 1);
	} else {
		
		pc_location = cell_contains_pc(dungeon->cells[next.y][next.x]);
		if (pc_location) {
			
			pc_location->hp -= character->damage;
			if (pc_location->hp < 1) { dungeon->cells[pc_location->location.y][pc_location->location.x].character = NULL; }
		}
		
		npc_location = cell_contains_npc(dungeon->cells[next.y][next.x]);
		if (npc_location && (npc_location->id != ((NPC*)(character))->id)) {
			
			npc_location->hp -= character->damage;
			if (npc_location->hp < 1) {
				
				dungeon->cells[npc_location->location.y][npc_location->location.x].character = NULL;
				(dungeon->num_npcs_dead)++;
			}
		}
		
		dungeon->cells[character->location.y][character->location.x].character = NULL;
		dungeon->cells[next.y][next.x].character = character;
		
		character->prev_location = character->location;
		character->location = next;
	}
	
	return;
}

int dungeon_contains_npcs(Dungeon *dungeon) { return dungeon->num_npcs > dungeon->num_npcs_dead; }

void dungeon_draw(Dungeon dungeon, uint8_t offset_y, uint8_t offset_x, int print_fog, int print_fill, int print_color, int print_weight) {
	
	int i = 0, j = 0;
	int weight_offset = 0;
	
	if (print_weight == 1) { weight_offset = 2; }
	
	for (i = -2; i < dungeon.height; i++) {
		
		if (weight_offset && i > -1) { mvprintw((offset_y + (i + weight_offset)), 0, "%2d - ", i); }
		
		for (j = 0; j < dungeon.width; j++) {
			
			if (weight_offset && i == -2) {
				
				if (j == 0) { mvprintw((offset_y + (i + weight_offset)), (offset_x + j), "     %2d", j); }
				else 		{ mvprintw((offset_y + (i + weight_offset)), (offset_x + j), "%2d", j); }
			}
			else if (weight_offset && i == -1) {
				
				if (j == 0) { mvprintw((offset_y + (i + weight_offset)), (offset_x + j), "     --"); }
				else 		{ mvprintw((offset_y + (i + weight_offset)), (offset_x + j), "--"); }
			}
			else if (i > -1) { cell_draw(dungeon.cells[i][j], (offset_y + i + weight_offset), (offset_x + j), print_fog, print_fill, print_color, print_weight); }
		}
	}
	
	return;
}