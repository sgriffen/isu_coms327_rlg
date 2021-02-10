#include "cell.h"

/********** definitions **********/
#define ROOM_BORDER_WIDTH 1
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

/****** function declarations *****/
struct Room room_init(int room_width, int room_height);

void room_init_cells(struct Room *room, struct Cell *top_left, struct Cell *bottom_right, struct Cell *center);

int rooms_intersect(int num_rooms, struct Room rooms[], struct Room *room, int space_between);

int h_rooms_intersect(struct Room room1, struct Room room2, int space_between);

int rooms_smallest_distance_y(struct Room room1, struct Room room2);

int rooms_smallest_distance_x(struct Room room1, struct Room room2);