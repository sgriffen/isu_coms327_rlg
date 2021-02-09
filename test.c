/******** include std libs ********/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"
#include "rouge.h"

int main(int argc, char *argv[]) {
	
	int i = 0, j = 0;
	
	struct Cell tl_1;
	struct Cell br_1;
	struct Cell center_1;
	struct Cell tl_2;
	struct Cell br_2;
	struct Cell center_2;
	struct Cell tl_3;
	struct Cell br_3;
	struct Cell center_3;
	struct Cell tl_4;
	struct Cell br_4;
	struct Cell center_4;
	
	struct Room room1;
	struct Room room2;
	struct Room room3;
	struct Room room4;
	
	struct Level level;
	
	tl_1.x = 0;
	tl_1.y = 0;
	br_1.x = 2;
	br_1.y = 2;
	center_1.x = br_1.x - tl_1.x;
	center_1.y = br_1.y - tl_1.y;
	tl_2.x = 7;
	tl_2.y = 8;
	br_2.x = 10;
	br_2.y = 10;
	center_2.x = br_2.x - tl_2.x;
	center_2.y = br_2.y - tl_2.y;
	tl_3.x = 3;
	tl_3.y = 4;
	br_3.x = 4;
	br_3.y = 4;
	center_3.x = br_3.x - tl_3.x;
	center_3.y = br_3.y - tl_3.y;
	tl_4.x = 0;
	tl_4.y = 3;
	br_4.x = 2;
	br_4.y = 4;
	center_4.x = br_4.x - tl_4.x;
	center_4.y = br_4.y - tl_4.y;
	
	room1.tl = &tl_1;
	room1.br = &br_1;
	room1.center = &center_1;
	room2.tl = &tl_3;
	room2.br = &br_3;
	room2.center = &center_3;
	room3.tl = &tl_2;
	room3.br = &br_2;
	room3.center = &center_2;
	room4.tl = &tl_4;
	room4.br = &br_4;
	room4.center = &center_4;
	
	level.num_rooms = 4;
	level.rooms = (struct Room*)malloc(sizeof(struct Room) * 4); //allocate room memory
	level.rooms[0] = room1;
	level.rooms[1] = room2;
	level.rooms[2] = room3;
	level.rooms[3] = room4;
	
	printf("Room 1 - tl: (%d, %d)   br: (%d, %d)\n", level.rooms[0].tl->x, level.rooms[0].tl->y, level.rooms[0].br->x, level.rooms[0].br->y);
	printf("Room 2 - tl: (%d, %d)   br: (%d, %d)\n", level.rooms[1].tl->x, level.rooms[1].tl->y, level.rooms[1].br->x, level.rooms[1].br->y);
	printf("Room 3 - tl: (%d, %d)   br: (%d, %d)\n", level.rooms[2].tl->x, level.rooms[2].tl->y, level.rooms[2].br->x, level.rooms[2].br->y);
	printf("Room 4 - tl: (%d, %d)   br: (%d, %d)\n", level.rooms[3].tl->x, level.rooms[3].tl->y, level.rooms[3].br->x, level.rooms[3].br->y);
	
	for (i = 1; i < level.num_rooms-1; i++) {
		for (j = i+1; j < level.num_rooms; j++) {
			
			//printf("Test Room 1 - tl: (%d, %d)   br: (%d, %d)\n", level.rooms[i-1].tl->x, level.rooms[i-1].tl->y, level.rooms[i-1].br->x, level.rooms[i-1].br->y);
			//printf("Test Room 2 - tl: (%d, %d)   br: (%d, %d)\n", level.rooms[i].tl->x, level.rooms[i].tl->y, level.rooms[i].br->x, level.rooms[i].br->y);
			//printf("Test Room 3 - tl: (%d, %d)   br: (%d, %d)\n", level.rooms[j].tl->x, level.rooms[j].tl->y, level.rooms[j].br->x, level.rooms[j].br->y);
			
			if (rooms_smallest_distance_y(level.rooms[i-1], level.rooms[j]) < rooms_smallest_distance_y(level.rooms[i-1], level.rooms[i])
				&& rooms_smallest_distance_x(level.rooms[i-1], level.rooms[j]) < rooms_smallest_distance_x(level.rooms[i-1], level.rooms[i])) {
					
				//printf("Room [%d] is closer to Room [%d] than Room [%d] is\n", j, i-1, i);
				
				struct Room temp = level.rooms[i];
				level.rooms[i] = level.rooms[j];
				level.rooms[j] = temp;
			}
		}
	}
	
	printf("New Room 1 - tl: (%d, %d)   br: (%d, %d)\n", level.rooms[0].tl->x, level.rooms[0].tl->y, level.rooms[0].br->x, level.rooms[0].br->y);
	printf("New Room 2 - tl: (%d, %d)   br: (%d, %d)\n", level.rooms[1].tl->x, level.rooms[1].tl->y, level.rooms[1].br->x, level.rooms[1].br->y);
	printf("New Room 3 - tl: (%d, %d)   br: (%d, %d)\n", level.rooms[2].tl->x, level.rooms[2].tl->y, level.rooms[2].br->x, level.rooms[2].br->y);
	printf("New Room 4 - tl: (%d, %d)   br: (%d, %d)\n", level.rooms[3].tl->x, level.rooms[3].tl->y, level.rooms[3].br->x, level.rooms[3].br->y);
	
	return 0;
}