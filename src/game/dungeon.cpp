/******** include std libs ********/
#include <algorithm>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
/******* include custom libs ******/
#include "./classdef/dungeon.h"
#include "./utils/pathfinder.h"
#include "../utils/math_utils.h"

/********** definitions **********/
#define MAX_ROOM_ITERS 100

/*********** global vars **********/


/***** constructor definitions *****/
Dungeon::Dungeon() {
	
	height 				= 0;
	width 				= 0;
	volitile_height 	= 0;
	volitile_width 		= 0;
	
	num_cells 			= 0;
	num_volitile_cells 	= 0;
	cells 				= NULL;
	
	num_rooms 			= 0;
	rooms 				= NULL;
	
	num_staircases_up 	= 0;
	staircases_up 		= 0;
	
	num_staircases_down = 0;
	staircases_down 	= 0;
	
	num_npcs_defined 	= 0;
	npc_id_next 		= 1;
	num_npcs 			= 0;
	num_npcs_dead 		= 0;
	npcs 				= NULL;
	
	num_items_defined 	= 0;
	item_id_next 		= 1;
	num_items 			= 0;
	items 				= NULL;
}
Dungeon::Dungeon(uint8_t dungeon_height, uint8_t dungeon_width, int dungeon_num_npcs, int dungeon_num_items, int dungeon_num_stairs_up, int dungeon_num_stairs_down, int dungeon_ommit_stairs, std::vector<NPC_Template> npc_templates, std::vector<Item_Template> item_templates, std::vector<std::string> *npc_unique_placed, std::vector<std::string> *item_unique_placed) : Dungeon() {
	
	int i;
	srand(time(NULL));
	
	height 				= dungeon_height;
	width 				= dungeon_width;
	volitile_height 	= dungeon_height-(DUNGEON_BORDER_WIDTH*2);
	volitile_width 		= dungeon_width-(DUNGEON_BORDER_WIDTH*2);
	num_npcs 			= dungeon_num_npcs;
	num_items			= dungeon_num_items;
	num_staircases_up 	= dungeon_num_stairs_up;
	num_staircases_down = dungeon_num_stairs_down;
	num_cells 			= height * width;
	num_volitile_cells 	= volitile_height * volitile_width;
	
	cells 				= (Cell**)calloc(height, sizeof(Cell*));
	for(i = 0; i < height; i++) { cells[i] = (Cell*)calloc(width, sizeof(Cell)); }
	
	/* initialize dungeon cells */
	dungeon_generate_cells(this);
	/* generate and sort rooms */
	dungeon_generate_rooms(this);
	/* generate cooridors */
	dungeon_generate_cooridors(this);
	/* add staircases */
	dungeon_generate_staircases(this, dungeon_num_stairs_up, dungeon_num_stairs_down, dungeon_ommit_stairs);
	/* add npcs */
	dungeon_generate_npcs(this, dungeon_num_npcs, npc_templates, npc_unique_placed);
	/* add items */
	dungeon_generate_items(this, dungeon_num_items, item_templates, item_unique_placed);
}
/****** function definitions ******/
void Dungeon::clean() {
	
//	free(cells);
//	free(rooms);
//	free(staircases_up);
//	free(staircases_down);
//	free(npcs);
//	free(items);
	
	return;
}

void Dungeon::draw(uint8_t offset_y, uint8_t offset_x, int print_fog, int print_fill, int print_weight) {
	
	int i = 0, j = 0;
	int weight_offset_h = 0;
	int weight_offset_w = 0;
	
	if (print_weight == 1) { weight_offset_h = 2; }
	
	for (i = -2; i < height; i++) {
		
		weight_offset_w = 0;
		
		if (weight_offset_h && i > -1) {
			
			mvprintw((offset_y + (i + weight_offset_h)), 0, "%2d - ", i);
			weight_offset_w += 5;
		}
		
		for (j = 0; j < width; j++) {
			
			if (weight_offset_h && i == -2) {
				
				if (j == 0) {
					
					mvprintw((offset_y + (i + weight_offset_h)), (offset_x + weight_offset_w), "     %2d", j);
					weight_offset_w += 7;
				}
				else {
					
					mvprintw((offset_y + (i + weight_offset_h)), (offset_x + weight_offset_w), "%2d", j);
					weight_offset_w += 2;
				}
			}
			else if (weight_offset_h && i == -1) {
				
				if (j == 0) { 
				
					mvprintw((offset_y + (i + weight_offset_h)), (offset_x + weight_offset_w), "     --");
					weight_offset_w += 7;
				}
				else {
					
					mvprintw((offset_y + (i + weight_offset_h)), (offset_x + weight_offset_w), "--"); 
					weight_offset_w += 2;
				}
			}
			else if (i > -1) { weight_offset_w += cells[i][j].draw((offset_y + i + weight_offset_h), (offset_x + weight_offset_w), print_fog, print_fill, print_weight); }
		}
	}
	
	return;
}

void dungeon_generate_cells(Dungeon *dungeon) {
	
	int i = 0, j = 0;
	
	for(i = 0; i < dungeon->height; i++) {
        for (j = 0; j < dungeon->width; j++) {
			
			dungeon->cells[i][j] = Cell(i, j, -1);
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
			
			room = Room(i, room_height, room_width);
			
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
	
//	QueueNode node = nodes[0];
//	Coordinate *loc = (Coordinate*)(node.element);
//	while (!start->is_same(*loc)) {
//		
//		if (dungeon->cells[loc->y][loc->x].type_next != CellType_Room) {
//			
//			dungeon->cells[loc->y][loc->x].type_next = CellType_Cooridor;
//			dungeon->cells[loc->y][loc->x].hardness = 0;
//		} else {
//			
//			Room* node_room = room_contains_point(dungeon->num_rooms, &(dungeon->rooms), *loc);
//			if (node_room) { node_room->connected = 1; }
//		}
//		
//		node = *(node.from);
//	}
//	
//	if (dungeon->cells[loc->y][loc->x].type_next != CellType_Room) { //mark first cell in node array
//			
//		dungeon->cells[loc->y][loc->x].type_next = CellType_Cooridor;
//		dungeon->cells[loc->y][loc->x].hardness = 0;
//	}
	
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

void dungeon_generate_npcs(Dungeon *dungeon, int num_npcs, std::vector<NPC_Template> npc_templates, std::vector<std::string> *npc_unique_placed) {
	
	/*
		non-player characters (npcs) are placed at a random location in the dungeon
		npcs that can tunnel are placed anywhere, non-tunneling npcs are placed only in rooms; all locations exclude where the pc is placed
	*/
	int i = 0, j = 0;
	int unplaced_npcs = 0;
	
	/* get number of npcs to generate, and allocate memory in dungeon */
	dungeon->num_npcs_defined = 1;
	if (num_npcs < 1) {
		
		num_npcs = utils_rand_between(dungeon->num_rooms, (dungeon->num_rooms)*2);
		dungeon->num_npcs_defined = 0;
	}
	else if (num_npcs > ((dungeon->num_volitile_cells) - 1)) { num_npcs = (dungeon->num_volitile_cells) - 1; }
	dungeon->npcs = (NPC*)calloc(num_npcs, sizeof(NPC));
	
	for (i = 0; i < num_npcs; i++) {
		
		Coordinate loc;
		
		/* select npc template to create npc from, taking rarity into account and ignorning unique npcs already placed */
		int template_idx = utils_rand_between(0, npc_templates.size()-1);
		while (((std::find(npc_unique_placed->begin(), npc_unique_placed->end(), npc_templates[template_idx].name) != npc_unique_placed->end())) || (utils_rand_between(0, 99) < npc_templates[template_idx].rarity))
			{ template_idx = utils_rand_between(0, npc_templates.size()-1); }
		if (npc_templates[template_idx].type & NPC_TYPE_UNIQUE) { npc_unique_placed->push_back(npc_templates[template_idx].name); }
		
		/* if npc is of type TUNNELING, place anywhere in the dungeon */
		if (npc_templates[template_idx].type & NPC_TYPE_TUNNELING) {
			
			loc.y = utils_rand_between(DUNGEON_BORDER_WIDTH, dungeon->volitile_height);
			loc.x = utils_rand_between(DUNGEON_BORDER_WIDTH, dungeon->volitile_width);
			
			while(cell_immutable_tunneling(dungeon->cells[loc.y][loc.x]) && j < DUNGEON_MAX_CHANCE_COUNT) {
				
				loc.y = utils_rand_between(DUNGEON_BORDER_WIDTH, dungeon->volitile_height);
				loc.x = utils_rand_between(DUNGEON_BORDER_WIDTH, dungeon->volitile_width);
				
				j++;
			}
		} else { /* else, place npc anywhere in a room in the dungeon */
			
			int room_index = utils_rand_between(0, dungeon->num_rooms-1);
			
			loc.y = utils_rand_between(dungeon->rooms[room_index].tl.y, dungeon->rooms[room_index].br.y, NULL);
			loc.x = utils_rand_between(dungeon->rooms[room_index].tl.x, dungeon->rooms[room_index].br.x, NULL);
			
			while(cell_immutable_ntunneling(dungeon->cells[loc.y][loc.x]) && (j < DUNGEON_MAX_CHANCE_COUNT)) {
				
				room_index = utils_rand_between(0, dungeon->num_rooms-1);
				
				loc.y = utils_rand_between(dungeon->rooms[room_index].tl.y+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br.y-ROOM_BORDER_WIDTH, NULL);
				loc.x = utils_rand_between(dungeon->rooms[room_index].tl.x+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br.x-ROOM_BORDER_WIDTH, NULL);
				
				j++;
			}
		}
		
		if (j >= DUNGEON_MAX_CHANCE_COUNT) { unplaced_npcs++; }
		else {
			
			dungeon->npcs[i] = NPC(*(npc_templates[template_idx].new_npc()));
			dungeon->npcs[i].id = dungeon->npc_id_next;
			dungeon->npcs[i].location = loc;
			dungeon->npc_id_next++;
		}
		j = 0;
	}
	
	dungeon->num_npcs = num_npcs-unplaced_npcs;
    dungeon->num_npcs_dead = 0;
	
	return;
}

void dungeon_generate_items(Dungeon *dungeon, int num_items, std::vector<Item_Template> item_templates, std::vector<std::string> *item_unique_placed) {
	
	/*
		items are placed at a random location in a random room in the dungeon
		all locations exclude where the pc is placed
	*/
	
	int i = 0, j = 0;
	int unplaced_items = 0;
	
	/* get number of items to generate, and allocate memory in dungeon */
	dungeon->num_items_defined = 1;
	if (num_items < 1) {
		
		num_items = utils_rand_between(10, 20);
		dungeon->num_items_defined = 0;
	}
	dungeon->items =(Item*)calloc(num_items, sizeof(Item));
	
	for (i = 0; i < num_items; i++) {
		
		Coordinate loc;
		
		/* select item template to create item from, taking rarity into account and ignorning artifact items already placed */
		int template_idx = utils_rand_between(0, item_templates.size()-1);
		while (((std::find(item_unique_placed->begin(), item_unique_placed->end(), item_templates[template_idx].name) != item_unique_placed->end())) || (utils_rand_between(0, 99) < item_templates[template_idx].rarity))
			{ template_idx = utils_rand_between(0, item_templates.size()-1); }
		if (item_templates[template_idx].status) { item_unique_placed->push_back(item_templates[template_idx].name); }
		
		int room_index = utils_rand_between(0, dungeon->num_rooms-1);
		
		loc.y = utils_rand_between(dungeon->rooms[room_index].tl.y, dungeon->rooms[room_index].br.y, NULL);
		loc.x = utils_rand_between(dungeon->rooms[room_index].tl.x, dungeon->rooms[room_index].br.x, NULL);
		
		while(cell_immutable_ntunneling(dungeon->cells[loc.y][loc.x]) && (j < DUNGEON_MAX_CHANCE_COUNT)) {
			
			room_index = utils_rand_between(0, dungeon->num_rooms-1);
			
			loc.y = utils_rand_between(dungeon->rooms[room_index].tl.y+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br.y-ROOM_BORDER_WIDTH, NULL);
			loc.x = utils_rand_between(dungeon->rooms[room_index].tl.x+ROOM_BORDER_WIDTH, dungeon->rooms[room_index].br.x-ROOM_BORDER_WIDTH, NULL);
			
			j++;
		}
		
		if (j >= DUNGEON_MAX_CHANCE_COUNT) { unplaced_items++; }
		else {
			
			dungeon->items[i] = Item(*(item_templates[template_idx].new_item()));
			dungeon->items[i].id = dungeon->item_id_next;
			dungeon->items[i].location = loc;
			dungeon->item_id_next++;
			
			dungeon->cells[loc.y][loc.x].items.push_back(&(dungeon->items[i]));
		}
		j = 0;
	}
	
	dungeon->num_items = num_items-unplaced_items;
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
	
	if (end.is_same(start)) { return end; }
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

    while (!cell_immutable_ntunneling(dungeon.cells[current.y][current.x]) && end.is_same(current)) {

        current.y += ray_matrix[0];
        current.x += ray_matrix[1];
    }

    return end.is_same(current);
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
	
	int vision_buff = ((PC*)pc)->buff_light();
	for (i = -2 - vision_buff; i < 3 + vision_buff; i++) {
		for (j = -2 - vision_buff; j < 3 + vision_buff; j++) {
			
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

uint32_t* dungeon_resolve_collision(Dungeon *dungeon, Character *character, Coordinate next, int override_damage) {
	
	/*
		Resolves potential collisions with the next cell a character wants to move to, and the current character that's in that cell
		Returns 0 if there is no collision
		Returns -1 if an npc attacks into the cell the pc is located in (npc kills pc)
		Returns the id of an npc if a character moves into the cell the is located in (pc attacks npc or npc's swap positions)
	*/
	
	uint32_t collision_type = 0;
	uint32_t damage_roll = 0;
	uint32_t boss_defeated = 0;
	
	PC* pc_location = NULL;
	NPC* npc_location = NULL;
	
	if (!(character->id)) {
		
		dungeon_pc_los(dungeon, character, 0);
		
		character->prev_location = character->location;
		
		npc_location = dungeon->cells[next.y][next.x].contains_npc();
		if (npc_location) {
			
			//perform attack here
			if (override_damage) { npc_location->hp -= npc_location->hp; }
			else {
				
				damage_roll = ((PC*)character)->buff_damage() + character->damage.roll();
				if (damage_roll > 0) { npc_location->hp -= damage_roll; }
			}
			
			if (npc_location->hp < 1) {
				
				if (npc_location->type & NPC_TYPE_BOSS) { boss_defeated = 1; }
				if (!(npc_location->inventory.empty())) {
					
					std::vector<Item*>::iterator cell_item_itr;
					for (cell_item_itr = npc_location->inventory.begin(); cell_item_itr != npc_location->inventory.end(); cell_item_itr++) { dungeon->cells[npc_location->location.y][npc_location->location.x].items.push_back(*cell_item_itr); }
					npc_location->inventory.erase(npc_location->inventory.begin(), npc_location->inventory.end());
				}
				dungeon->cells[npc_location->location.y][npc_location->location.x].character = NULL;
				(dungeon->num_npcs_dead)++;
				
				((PC*)character)->num_kills++;
			}
			collision_type = npc_location->id;
			
			if (override_damage) {
				
				dungeon->cells[character->location.y][character->location.x].character = NULL;
			
				character->location = next;
				dungeon->cells[next.y][next.x].character = character;
			}
		} else {
			dungeon->cells[character->location.y][character->location.x].character = NULL;
			
			character->location = next;
			dungeon->cells[next.y][next.x].character = character;
		}
		
		dungeon_pc_los(dungeon, character, 1);
	} else {
		
		character->prev_location = character->location;
		
		pc_location = dungeon->cells[next.y][next.x].contains_pc();
		if (pc_location) {
			
			//perform attack here
			damage_roll = character->damage.roll() - pc_location->buff_defense();
			if (damage_roll > 0) { pc_location->hp -= damage_roll; }
			
			if (pc_location->hp < 1) { dungeon->cells[pc_location->location.y][pc_location->location.x].character = NULL; }
			
			collision_type = -1;
		}
		
		npc_location = dungeon->cells[next.y][next.x].contains_npc();
		if (npc_location && (npc_location->id != ((NPC*)(character))->id)) {
			
			/* move npc at location next to current character's location, and move current character to location next */
			npc_location->prev_location = npc_location->location;
			npc_location->location = character->location;
			dungeon->cells[character->location.y][character->location.x].character = (Character*)npc_location;
			
			character->location = next;
			dungeon->cells[next.y][next.x].character = character;
			
			collision_type = npc_location->id;
		}
		if (!pc_location && !npc_location) {
			
			dungeon->cells[character->location.y][character->location.x].character = NULL;
			
			character->location = next;
			dungeon->cells[next.y][next.x].character = character;
		}
	}
	
	uint32_t *arr = (uint32_t*)malloc(3 * sizeof(uint32_t));
	arr[0] = collision_type;
	arr[1] = damage_roll;
	arr[2] = boss_defeated;
	return arr;
}

int dungeon_contains_npcs(Dungeon *dungeon) { return dungeon->num_npcs > dungeon->num_npcs_dead; }