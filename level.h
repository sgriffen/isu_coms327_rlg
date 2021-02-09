#include "cell.h"

/********** definitions **********/
#define LEVEL_HEIGHT 21
#define LEVEL_WIDTH 80
#define LEVEL_MIN_ROOM 6
#define LEVEL_MAX_ROOM 16
#define LEVEL_MAX_STAIRCASE_UP 2
#define LEVEL_MAX_STAIRCASE_DOWN 2
#define ROOM_MIN_SPACE 1
#define ROOM_MIN_HEIGHT 3
#define ROOM_MIN_WIDTH 4

/******* enums declarations *******/


/******* struct declarations ******/
struct Room {
	
	int height, width;
	
	int connected;
	
	struct Cell *tl;
	struct Cell *br;
	struct Cell *center;
};
struct Level {
	
	int height, width;
	
	int num_cells;
	struct Cell **cells;
	
	int num_rooms;
	struct Room *rooms;
};

/****** function declarations *****/
struct Level level_init(int level_width, int level_height);

void level_generate_rooms(struct Level *level);

void level_generate_cooridors(struct Level *level);

void level_generate_staircases(struct Level *level);

int level_place_room(struct Level *level, struct Room *room);

int rooms_intersect(struct Level *level, struct Room *room, int space_between);

int rooms_smallest_distance_y(struct Room room1, struct Room room2);

int rooms_smallest_distance_x(struct Room room1, struct Room room2);

int h_rooms_intersect(struct Room room1, struct Room room2, int space_between);

void h_generate_cooridor(struct Level *level, int start_y, int start_x, int end_y, int end_x);

void level_print(struct Level level);