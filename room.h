#include "cell.h"
#include "room.h"

/********** definitions **********/
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

/****** function declarations *****/