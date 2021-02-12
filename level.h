#include "room.h"

/********** definitions **********/
#define LEVEL_HEIGHT 21
#define LEVEL_WIDTH 80
#define LEVEL_BORDER_WIDTH 1
#define LEVEL_MIN_ROOM 6
#define LEVEL_MAX_ROOM 12
#define LEVEL_MIN_STAIRCASE_UP 1
#define LEVEL_MAX_STAIRCASE_UP 2
#define LEVEL_MIN_STAIRCASE_DOWN 1
#define LEVEL_MAX_STAIRCASE_DOWN 2
#define LEVEL_MAX_CHANCE_COUNT 150

/******* enums declarations *******/


/******* struct declarations ******/
struct Level {
	
	int print_height, print_width;
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

void level_generate_cooridor(struct Level *level, int start_y, int start_x, int end_y, int end_x);

void level_print(struct Level level, int print_border);