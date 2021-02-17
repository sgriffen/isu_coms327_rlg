/******** include std libs ********/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <sys/stat.h>
/******* include custom libs ******/
#include "rouge.h"
#include "src/utils/file_utils.h"

int main(int argc, char *argv[]) {
	
	int run_args[ARGS_LENGTH][ARGS_DEPTH] = {
		{ 0 }, 	//display dungeon border
		{ 0 },	//load dungeon in from file before running game
		{ 0 } 	//save current dungeon to file after dungeon is created
	};
	Dungeon dungeon;
	
	/* parse command args */
	args_parse(argc, argv, &run_args);
	
	/* load dungeon from file if desired. else, create new dungeon */
	if (run_args[1][0]) {
		
		dungeon_mem_init(&dungeon, (uint8_t)DUNGEON_HEIGHT, (uint8_t)DUNGEON_WIDTH);
		
		int read_invalid = fread_dungeon(&dungeon);
		if (read_invalid) { 
		
			printf("Dungeon read failed. Code [%d]\n", read_invalid);
			
			dungeon_init(&dungeon, (uint8_t)DUNGEON_HEIGHT, (uint8_t)DUNGEON_WIDTH);
		}
	}
	else { dungeon_init(&dungeon, (uint8_t)DUNGEON_HEIGHT, (uint8_t)DUNGEON_WIDTH); }
	
	/* print created/loaded dungeon */
	dungeon_print(dungeon, run_args[0][0]);
	
	/* save dungeon to file if desired */
	if (run_args[2][0]) {
		
		int write_invalid = fwrite_dungeon(dungeon);
		if (write_invalid) { printf("Dungeon write failed. Code [%d]\n", write_invalid); }
	}
	
	return 0;
}

void args_parse(int argc, char *argv[], int (*run_args)[ARGS_LENGTH][ARGS_DEPTH]) {
	
	int i = 1;
	
	for (i = 1; i < argc; i++) {
		
		if (!strcmp(argv[i], "--border")) { (*run_args)[0][0] = 1; }
			
		if (!strcmp(argv[i], "--load")) { (*run_args)[1][0] = 1; }
		
		if (!strcmp(argv[i], "--save")) { (*run_args)[2][0] = 1; }
	}
	
	return;
}

int fread_dungeon(Dungeon *dungeon) {
	
	int i = 0, j = 0, k = 0;
	
	struct stat st = {0};
	
	char f_name[(strlen(getenv("HOME")) + 13)];
	memset(f_name, '\0', sizeof(f_name));
	
	strcpy(f_name, getenv("HOME"));
	strcat(f_name, "/.rlg327/");
	if (stat(f_name, &st) == -1) { return 1; }
	strcat(f_name, "dungeon");
	
	printf("debug -- file name: [%s]\n", f_name);
	printf("debug -- reading [%ld] bytes\n", utils_fsize(f_name, 1)); 
	
	/* allocate initial memory to read to */
	char *f_type = (char*)malloc(sizeof(char) * strlen("RLG327-S2021"));
	uint32_t f_marker = 0;
	uint32_t f_size = 0;
	uint8_t f_pc_x = 0;
	uint8_t f_pc_y = 0;
	uint8_t *f_cells_hardness = (uint8_t*)malloc(sizeof(uint8_t) * dungeon->num_cells);  
	uint16_t f_num_rooms = 0;
	uint8_t *f_room_info = NULL;
	uint16_t f_num_staircase_up = 0;
	uint8_t *f_staircases_up = NULL;
	uint16_t f_num_staircase_down = 0;
	uint8_t *f_staircases_down = NULL;
	
	/* read dungeon from file */
	size_t f_running_size = 0;
	f_running_size += utils_fread(f_name, f_type, sizeof(char), strlen("RLG327-S2021"), 0);
	if (!f_running_size) { return -1; }
//	printf("debug -- file read offset: [%ld]\n", f_running_size);
	f_running_size += utils_fread(f_name, &f_marker, sizeof(uint32_t), 1, f_running_size);
	if (!f_running_size) { return -2; }
//	printf("debug -- file read offset: [%ld]\n", f_running_size);
	f_running_size += utils_fread(f_name, &f_size, sizeof(uint32_t), 1, f_running_size);
	if (!f_running_size) { return -3; }
//	printf("debug -- file read offset: [%ld]\n", f_running_size);
	f_running_size += utils_fread(f_name, &f_pc_x, sizeof(uint8_t), 1, f_running_size);
	if (!f_running_size) { return -4; }
	f_running_size += utils_fread(f_name, &f_pc_y, sizeof(uint8_t), 1, f_running_size);
	if (!f_running_size) { return -5; }
//	printf("debug -- file read offset: [%ld]\n", f_running_size);
	f_running_size += utils_fread(f_name, f_cells_hardness, sizeof(uint8_t), (sizeof(f_cells_hardness[0])*dungeon->num_cells), f_running_size);
	if (!f_running_size) { return -6; }
//	printf("debug -- file read offset: [%ld]\n", f_running_size);
	f_running_size += utils_fread(f_name, &f_num_rooms, sizeof(uint16_t), 1, f_running_size);
	if (!f_running_size) { return -7; }
//	printf("debug -- file read offset: [%ld]\n", f_running_size);
	f_room_info = (uint8_t*)malloc(sizeof(uint8_t) * (f_num_rooms*4));
	f_running_size += utils_fread(f_name, f_room_info, sizeof(uint8_t), (sizeof(f_room_info[0])*(f_num_rooms*4)), f_running_size);
	if (!f_running_size) { return -8; }
//	printf("debug -- file read offset: [%ld]\n", f_running_size);
	f_running_size += utils_fread(f_name, &f_num_staircase_up, sizeof(uint16_t), 1, f_running_size);
	if (!f_running_size) { return -9; }
//	printf("debug -- file read offset: [%ld]\n", f_running_size);
	f_staircases_up = (uint8_t*)malloc(sizeof(uint8_t) * (f_num_staircase_up*2));
	f_running_size += utils_fread(f_name, f_staircases_up, sizeof(uint8_t), (sizeof(f_staircases_up[0])*(f_num_staircase_up*2)), f_running_size);
	if (!f_running_size) { return -10; }
//	printf("debug -- file read offset: [%ld]\n", f_running_size);
	f_running_size += utils_fread(f_name, &f_num_staircase_down, sizeof(uint16_t), 1, f_running_size);
	if (!f_running_size) { return -11; }
//	printf("debug -- file read offset: [%ld]\n", f_running_size);
	f_staircases_down = (uint8_t*)malloc(sizeof(uint8_t)*(f_num_staircase_down*2));
	f_running_size += utils_fread(f_name, f_staircases_down, sizeof(uint8_t), (sizeof(f_staircases_down[0])*(f_num_staircase_down*2)), f_running_size);
	if (!f_running_size) { return -12; }
//	printf("debug -- file read offset: [%ld]\n", f_running_size);

	/* set dungeon cell hardness */
	for (i = 0; i < dungeon->height; i++) {
		for (j = 0; j < dungeon->width; j++) {
			
			dungeon->cells[i][j].hardness = f_cells_hardness[k];
			k++;
			
			if (i < DUNGEON_BORDER_WIDTH || i >= dungeon->height-DUNGEON_BORDER_WIDTH) { dungeon->cells[i][j].type = CellType_Border_h; }
			else if (j < DUNGEON_BORDER_WIDTH || j >= dungeon->width-DUNGEON_BORDER_WIDTH) { dungeon->cells[i][j].type = CellType_Border_v; }
		}
	}
	
	/* mark and place dungeon rooms */
	dungeon->num_rooms = f_num_rooms;
	dungeon->rooms = (Room*)malloc(sizeof(Room) * f_num_rooms);
	for (i = 0; i < (f_num_rooms*4); i+=4) {
		
		Room room = room_init((i/4), f_room_info[i+3], f_room_info[i+2]);
		
		room.tl = &(dungeon->cells[f_room_info[i+1]][f_room_info[i]]);
		room.br = &(dungeon->cells[(room.tl->y)+room.height][(room.tl->x)+room.width]);
		room.center = &(dungeon->cells[(room.tl->y)+(room.height/2)][(room.tl->x)+(room.width/2)]);
		
		room.connected = 1;
		
		dungeon->rooms[i/4] = room;
		
		/* mark cells in dungeon as CellType Room */
		for (j = room.tl->y; j < room.br->y; j++) {
			for (k = room.tl->x; k < room.br->x; k++) {
				
				dungeon->cells[j][k].type = CellType_Room;
				dungeon->cells[j][k].hardness = 0;
			}
		}
	}
	/* mark dungeon cooridors */
	for (i = 0; i < dungeon->height; i++) {
		for (j = 0; j < dungeon->width; j++) {
			
			if (dungeon->cells[i][j].hardness == 0 && dungeon->cells[i][j].type != CellType_Room) { dungeon->cells[i][j].type = CellType_Cooridor; }
		}
	}
	/* place up staircases in dungeon */
	dungeon->num_staircases_up = f_num_staircase_up;
	dungeon->staircases_up = (Cell**)malloc(sizeof(Cell*) * f_num_staircase_up);
	for (i = 0; i < (f_num_staircase_up*2); i+=2) {
		
		dungeon->cells[f_staircases_up[i+1]][f_staircases_up[i]].type = CellType_Stair_up;
		dungeon->staircases_up[i/2] = &(dungeon->cells[f_staircases_up[i+1]][f_staircases_up[i]]);
	}
	/* place down staircases in dungeon */
	dungeon->num_staircases_down = f_num_staircase_down;
	dungeon->staircases_down = (Cell**)malloc(sizeof(Cell*) * f_num_staircase_down);
	for (i = 0; i < (f_num_staircase_down*2); i+=2) {
		
		dungeon->cells[f_staircases_down[i+1]][f_staircases_down[i]].type = CellType_Stair_down;
		dungeon->staircases_down[i/2] = &(dungeon->cells[f_staircases_down[i+1]][f_staircases_down[i]]);
	}
	/* place pc in dungeon */
	Character pc = character_init(&(dungeon->cells[f_pc_y][f_pc_x]), CharacterType_PC);
	dungeon->pc = pc;
	
	return 0;
}

int fwrite_dungeon(Dungeon dungeon) {
	
	int i = 0, j = 0, k = 0;
	
	struct stat st = {0};
	
	char f_name[(strlen(getenv("HOME")) + 13)];
	memset(f_name, '\0', sizeof(f_name));
	
	strcpy(f_name, getenv("HOME"));
	strcat(f_name, "/.rlg327/");
	if (stat(f_name, &st) == -1) { mkdir(f_name, 0777); }
	strcat(f_name, "dungeon");
	
	printf("debug -- file name: [%s]\n", f_name);
	
	/* prepare dungeon information for writing */
	char *f_type = "RLG327-S2021";
	uint32_t f_marker = 0;
	uint32_t f_size = 255;
	uint8_t f_pc_x = dungeon.pc.location->x;
	uint8_t f_pc_y = dungeon.pc.location->y;
	uint8_t *f_cells_hardness = (uint8_t*)malloc(sizeof(uint8_t) * dungeon.num_cells);  
	for (i = 0; i < dungeon.height; i++) {
		for(j = 0; j < dungeon.width; j++) {
		
			f_cells_hardness[k] = dungeon.cells[i][j].hardness;
			k++;
		}
	}
	uint16_t f_num_rooms = dungeon.num_rooms;
	uint8_t *f_room_info = (uint8_t*)malloc(sizeof(uint8_t) * (dungeon.num_rooms*4));
	for (i = 0; i < (dungeon.num_rooms*4); i+=4) {
		
		f_room_info[i] = dungeon.rooms[i/4].tl->x;
		f_room_info[i+1] = dungeon.rooms[i/4].tl->y;
		f_room_info[i+2] = dungeon.rooms[i/4].width;
		f_room_info[i+3] = dungeon.rooms[i/4].height;
	}
	uint16_t f_num_staircase_up = dungeon.num_staircases_up;
	uint8_t *f_staircases_up = (uint8_t*)malloc(sizeof(uint8_t) * (dungeon.num_staircases_up*2));
	for (i = 0; i < (dungeon.num_staircases_up*2); i+=2) {
		
		f_staircases_up[i] = dungeon.staircases_up[i/2]->x;
		f_staircases_up[i+1] = dungeon.staircases_up[i/2]->y;
	}
	uint16_t f_num_staircase_down = dungeon.num_staircases_down;
	uint8_t *f_staircases_down = (uint8_t*)malloc(sizeof(uint8_t) * (dungeon.num_staircases_down*2));
	for (i = 0; i < (dungeon.num_staircases_down*2); i+=2) {
		
		f_staircases_down[i] = dungeon.staircases_down[i/2]->x;
		f_staircases_down[i+1] = dungeon.staircases_down[i/2]->y;
	}
	
	/* write dungeon to file, keeping track of the size of the file as it increases */
	size_t f_running_size = 0;
	f_running_size += utils_fwrite(f_name, f_type, sizeof(char), strlen(f_type), 0, 1);
	if (!f_running_size) { return -1; }
//	printf("debug -- file write offset: [%ld]\n", f_running_size);
	f_running_size += utils_fwrite(f_name, &f_marker, sizeof(uint32_t), 1, f_running_size, 0);
	if (!f_running_size) { return -2; }
//	printf("debug -- file write offset: [%ld]\n", f_running_size);
	f_running_size += utils_fwrite(f_name, &f_size, sizeof(uint32_t), 1, f_running_size, 0);
	if (!f_running_size) { return -3; }
//	printf("debug -- file write offset: [%ld]\n", f_running_size);
	f_running_size += utils_fwrite(f_name, &f_pc_x, sizeof(uint8_t), 1, f_running_size, 0);
	if (!f_running_size) { return -4; }
	f_running_size += utils_fwrite(f_name, &f_pc_y, sizeof(uint8_t), 1, f_running_size, 0);
	if (!f_running_size) { return -5; }
//	printf("debug -- file write offset: [%ld]\n", f_running_size);
	f_running_size += utils_fwrite(f_name, f_cells_hardness, sizeof(uint8_t), (sizeof(f_cells_hardness[0])*dungeon.num_cells), f_running_size, 0);
	if (!f_running_size) { return -6; }
//	printf("debug -- file write offset: [%ld]\n", f_running_size);
	f_running_size += utils_fwrite(f_name, &f_num_rooms, sizeof(uint16_t), 1, f_running_size, 0);
	if (!f_running_size) { return -7; }
//	printf("debug -- file write offset: [%ld]\n", f_running_size);
	f_running_size += utils_fwrite(f_name, f_room_info, sizeof(uint8_t), (sizeof(f_room_info[0])*(f_num_rooms*4)), f_running_size, 0);
	if (!f_running_size) { return -8; }
//	printf("debug -- file write offset: [%ld]\n", f_running_size);
	f_running_size += utils_fwrite(f_name, &f_num_staircase_up, sizeof(uint16_t), 1, f_running_size, 0);
	if (!f_running_size) { return -9; }
//	printf("debug -- file write offset: [%ld]\n", f_running_size);
	f_running_size += utils_fwrite(f_name, f_staircases_up, sizeof(uint8_t), (sizeof(f_staircases_up[0]*f_num_staircase_up)), f_running_size, 0);
	if (!f_running_size) { return -10; }
//	printf("debug -- file write offset: [%ld]\n", f_running_size);
	f_running_size += utils_fwrite(f_name, &f_num_staircase_down, sizeof(uint16_t), 1, f_running_size, 0);
	if (!f_running_size) { return -11; }
//	printf("debug -- file write offset: [%ld]\n", f_running_size);
	f_running_size += utils_fwrite(f_name, f_staircases_down, sizeof(uint8_t), (sizeof(f_staircases_down[0]*f_num_staircase_down*2)), f_running_size, 0);
	if (!f_running_size) { return -12; }
//	printf("debug -- file write offset: [%ld]\n", f_running_size);
	
	f_size = utils_fsize(f_name, 1);
	printf("debug -- wrote [%d] bytes\n", f_size);
	utils_fwrite(f_name, &f_size, sizeof(uint32_t), 1, strlen(f_type)+sizeof(f_marker), 0);
	
	return 0;
}