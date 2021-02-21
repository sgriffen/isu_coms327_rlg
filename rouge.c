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
		{ 0, 0 }, 	//display dungeon border
		{ 0, 0 },	//load dungeon in from file before running game
		{ 0, 0 }, 	//save current dungeon to file after dungeon is created
		{ 0, 0 },	//print color
		{ 0, 0 }, 	//read dungeon from specific file
		{ 0, 0 } 	//save dungeon to specific file
	};
	Dungeon dungeon;
	
	/* parse command args */
	args_parse(argc, argv, &run_args);
	
	/* load dungeon from file if desired. else, create new dungeon */
	if (run_args[1][0]) {
		
//		printf("debug -- loading dungeon from bit-file\n");
		
		dungeon_mem_init(&dungeon, (uint8_t)DUNGEON_HEIGHT, (uint8_t)DUNGEON_WIDTH);
		
		char *f_name = "";
		if (run_args[4][0]) {
			
			f_name = (char*)malloc(sizeof(char) * strlen(argv[run_args[4][1]]) + 1);
			f_name[sizeof(f_name)-1] = '\0';
			strcpy(f_name, argv[run_args[4][1]]); 
		}
		
		printf("%s\n", f_name);
		int read_invalid = fread_dungeon(&dungeon, f_name);
		if (read_invalid) { 
		
			printf("Dungeon read failed. Code [%d]\n", read_invalid);
			
			dungeon_init(&dungeon, (uint8_t)DUNGEON_HEIGHT, (uint8_t)DUNGEON_WIDTH);
		}
	}
	else { dungeon_init(&dungeon, (uint8_t)DUNGEON_HEIGHT, (uint8_t)DUNGEON_WIDTH); }
	
	/* print created/loaded dungeon */
	dungeon_print(dungeon, run_args[0][0], run_args[3][0]);
	
	/* save dungeon to file if desired */
	if (run_args[2][0]) {
		
//		printf("debug -- writing dungeon to bit-file\n");
		
		char *f_name = "";
		if (run_args[5][0]) {
			
			f_name = (char*)malloc(sizeof(char) * strlen(argv[run_args[5][1]]));
			f_name[sizeof(f_name)-1] = '\0';
			strcpy(f_name, argv[run_args[5][1]]); 
		}
		
		int write_invalid = fwrite_dungeon(dungeon, f_name);
		if (write_invalid) { printf("Dungeon write failed. Code [%d]\n", write_invalid); }
	}
	
	return 0;
}

void args_parse(int argc, char *argv[], int (*run_args)[ARGS_LENGTH][ARGS_DEPTH]) {
	
	int i = 1;
	
	for (i = 1; i < argc; i++) {
		
		if (!strcmp(argv[i], "--print")) { //print a (dungeon border) or (filled in walls and empty rooms)
			if (!strcmp(argv[i+1], "b")) { (*run_args)[0][0] = 1;  }
			else if (!strcmp(argv[i+1], "f")) { (*run_args)[0][0] = 2; }
		}
		
		if (!strcmp(argv[i], "--load")) { (*run_args)[1][0] = 1; } //load dungeon from bit-file
		
		if (!strcmp(argv[i], "--save")) { (*run_args)[2][0] = 1; } //save generated dungeon to bit-file
		
		if (!strcmp(argv[i], "--color")) { (*run_args)[3][0] = 1; } //print PC and NPCs in color
		
		if (!strcmp(argv[i], "--load-from")) { //load dungeon from specified bit-file
			
			(*run_args)[1][0] = 1;
			(*run_args)[4][0] = 1; 
			(*run_args)[4][1] = i+1; 
			
			printf("debug -- loading dungeon from specific file\n");
		}
		
		if (!strcmp(argv[i], "--save-to")) { //save generated dungeon to specified bit-file
			
			(*run_args)[2][0] = 1;
			(*run_args)[5][0] = 1; 
			(*run_args)[5][1] = i+1; 
		}
	}
	
	return;
}

int fread_dungeon(Dungeon *dungeon, char *f_name) {
	
	int i = 0, j = 0, k = 0;
	struct stat st = {0};
	size_t f_read_offset = 0;
	
	if (strlen(f_name) < 1) {
	
		f_name = (char*)malloc(sizeof(char) * (strlen(getenv("HOME")) + 17));
		f_name[sizeof(f_name)-1] = '\0';
		
		strcpy(f_name, getenv("HOME"));
		strcat(f_name, "/.rlg327/");
		if (stat(f_name, &st) == -1) { return 1; }
		strcat(f_name, "dungeon");
	}
	
//	printf("debug -- file name: [%s]\n", f_name);
//	printf("debug -- reading [%ld] bytes\n", utils_fsize(f_name, 1)); 
	
	/* allocate initial memory to read to and create FileReadBuffers */
	char* f_type = (char*)malloc(sizeof(char) * strlen("RLG327-S2021"));
	uint32_t f_marker = 0;
	uint32_t f_size = 0;
	uint8_t f_pc_x = 0;
	uint8_t f_pc_y = 0;
	uint8_t *f_cells = (uint8_t*)malloc(sizeof(uint8_t) * dungeon->num_cells);
	uint16_t f_num_rooms = 0;
	uint8_t *f_rooms = NULL;
	uint16_t f_num_stairs_up = 0;
	uint8_t *f_stairs_up = NULL;
	uint16_t f_num_stairs_down = 0;
	uint8_t *f_stairs_down = NULL;
	
	FileReadBuffer f_buffer[] = {
		{
			.buffer = f_type,
			.nmemb 	= strlen("RLG327-S2021"),
			.size 	= sizeof(char)
		},
		{
			.buffer = &f_marker,
			.nmemb 	= 1,
			.size 	= sizeof(uint32_t)
		},
		{
			.buffer = &f_size,
			.nmemb 	= 1,
			.size 	= sizeof(uint32_t)
		},
		{
			.buffer = &f_pc_x,
			.nmemb 	= 1,
			.size 	= sizeof(uint8_t)
		},
		{
			.buffer = &f_pc_y,
			.nmemb 	= 1,
			.size 	= sizeof(uint8_t)
		},
		{
			.buffer = f_cells,
			.nmemb 	= dungeon->num_cells,
			.size 	= sizeof(uint8_t)
		},
		{
			.buffer = &f_num_rooms,
			.nmemb 	= 1,
			.size 	= sizeof(uint16_t)
		},
		{
			.buffer = NULL,
			.nmemb 	= 0,
			.size 	= sizeof(uint8_t)
		},
		{
			.buffer = &f_num_stairs_up,
			.nmemb 	= 1,
			.size 	= sizeof(uint16_t)
		},
		{
			.buffer = NULL,
			.nmemb 	= 0,
			.size 	= sizeof(uint8_t)
		},
		{
			.buffer = &f_num_stairs_down,
			.nmemb 	= 1,
			.size 	= sizeof(uint16_t)
		},
		{
			.buffer = NULL,
			.nmemb 	= 0,
			.size 	= sizeof(uint8_t)
		}
	};

	/* read dungeon from file */
//	f_read_offset = utils_fread(f_name, f_buffer[5], 0);
	f_read_offset += utils_fread_m(f_name, 7, 0, f_buffer, 0, 1);
	f_rooms = malloc(f_buffer[7].size * (f_num_rooms*4));
	f_buffer[7].buffer = f_rooms;
	f_buffer[7].nmemb = f_num_rooms * 4;
	f_read_offset += utils_fread_m(f_name, 9, 7, f_buffer, f_read_offset, 1);
	f_stairs_up = malloc(f_buffer[9].size * (f_num_stairs_up*2));
	f_buffer[9].buffer = f_stairs_up;
	f_buffer[9].nmemb = f_num_stairs_up * 2;
	f_read_offset += utils_fread_m(f_name, 11, 9, f_buffer, f_read_offset, 1);
	f_stairs_down = malloc(f_buffer[11].size * (f_num_stairs_down*2));
	f_buffer[11].buffer = f_stairs_down;
	f_buffer[11].nmemb = f_num_stairs_up * 2;
	f_read_offset += utils_fread(f_name, f_buffer[11], f_read_offset, 1);
	
//	printf("debug -- file read complete\n");
	
	/* set dungeon cell hardness */
	for (i = 0; i < dungeon->height; i++) {
		for (j = 0; j < dungeon->width; j++) {
			
			dungeon->cells[i][j].hardness = f_cells[k];
			k++;
			
			if (i < DUNGEON_BORDER_WIDTH || i >= dungeon->height-DUNGEON_BORDER_WIDTH) { dungeon->cells[i][j].type = CellType_Border_h; }
			else if (j < DUNGEON_BORDER_WIDTH || j >= dungeon->width-DUNGEON_BORDER_WIDTH) { dungeon->cells[i][j].type = CellType_Border_v; }
		}
	}
//	printf("debug -- set cell hardness\n");
	/* mark and place dungeon rooms */
	dungeon->num_rooms = f_num_rooms;
	dungeon->rooms = (Room*)malloc(sizeof(Room) * f_num_rooms);
	for (i = 0; i < (f_num_rooms*4); i+=4) {
		
		Room room = room_init((i/4), f_rooms[i+3], f_rooms[i+2]);
		
		room.tl = &(dungeon->cells[f_rooms[i+1]][f_rooms[i]]);
		room.br = &(dungeon->cells[(room.tl->y)+room.height][(room.tl->x)+room.width]);
		room.center = &(dungeon->cells[(room.tl->y)+(room.height/2)][(room.tl->x)+(room.width/2)]);
		
		room.connected = 1;
		
		dungeon->rooms[i/4] = room;
		
		/* mark cells in dungeon as CellType Room  */
		for (j = room.tl->y; j < room.br->y; j++) {
			for (k = room.tl->x; k < room.br->x; k++) {
				
				dungeon->cells[j][k].type = CellType_Room;
				dungeon->cells[j][k].hardness = 0;
			}
		}
	}
//	printf("debug -- marked rooms\n");
	/* mark dungeon cooridors */
	for (i = 0; i < dungeon->height; i++) {
		for (j = 0; j < dungeon->width; j++) {
			
			if (dungeon->cells[i][j].hardness == 0 && dungeon->cells[i][j].type != CellType_Room) { dungeon->cells[i][j].type = CellType_Cooridor; }
		}
	}
//	printf("debug -- marked cooridors\n");
	/* place up staircases in dungeon */
	dungeon->num_staircases_up = f_num_stairs_up;
	dungeon->staircases_up = (Cell**)malloc(sizeof(Cell*) * f_num_stairs_up);
	for (i = 0; i < (f_num_stairs_up*2); i+=2) {
		
		dungeon->cells[f_stairs_up[i+1]][f_stairs_up[i]].type = CellType_Stair_up;
		dungeon->cells[f_stairs_up[i+1]][f_stairs_up[i]].hardness = 0;
		dungeon->staircases_up[i/2] = &(dungeon->cells[f_stairs_up[i+1]][f_stairs_up[i]]);
	}
//	printf("debug -- marked up staircases\n");
	/* place down staircases in dungeon */
	dungeon->num_staircases_down = f_num_stairs_down;
	dungeon->staircases_down = (Cell**)malloc(sizeof(Cell*) * f_num_stairs_down);
	for (i = 0; i < (f_num_stairs_down*2); i+=2) {
		
		dungeon->cells[f_stairs_down[i+1]][f_stairs_down[i]].type = CellType_Stair_down;
		dungeon->cells[f_stairs_down[i+1]][f_stairs_down[i]].hardness = 0;
		dungeon->staircases_down[i/2] = &(dungeon->cells[f_stairs_down[i+1]][f_stairs_down[i]]);
	}
//	printf("debug -- marked down staircases\n");
	/* place pc in dungeon */
	Character pc = character_init(0, &(dungeon->cells[f_pc_y][f_pc_x]), CharacterType_PC);
	dungeon->pc = pc;	
//	printf("debug -- placed PC\n");
	
//	printf("debug -- file read offset: [%ld]\n", f_read_offset);
//	printf("debug -- f type: [%s]\n", f_type);
//	printf("debug -- f marker: [%d]\n", f_marker);
//	printf("debug -- f size: [%d]\n", f_size);
//	printf("debug -- pc x: [%d]\n", f_pc_x);
//	printf("debug -- pc y: [%d]\n", f_pc_y);
//	printf("debug -- num rooms: [%d]\n", f_num_rooms);
//	printf("debug -- num stairs up: [%d]\n", f_num_stairs_up);
//	printf("debug -- num stairs down: [%d]\n", f_num_stairs_down);
	
	return 0;
}

int fwrite_dungeon(Dungeon dungeon, char *f_name) {
	
	int i = 0, j = 0, k = 0;
//	size_t f_write_len = 0;
	struct stat st = {0};
	
	if (strlen(f_name) < 1) {
		
		f_name = (char*)malloc(sizeof(char) * (strlen(getenv("HOME")) + 17));
		f_name[sizeof(f_name)-1] = '\0';
		
		strcpy(f_name, getenv("HOME"));
		strcat(f_name, "/.rlg327/");
		if (stat(f_name, &st) == -1) { mkdir(f_name, 0777); }
		strcat(f_name, "dungeon");
	}
	
//	printf("debug -- file name: [%s]\n", f_name);
	
	/* prepare dungeon information for writing */
	char *f_type = "RLG327-S2021";
	uint32_t f_marker = 0;
	uint32_t f_size = 0;
	uint8_t f_pc_x = dungeon.pc.location->x;
	uint8_t f_pc_y = dungeon.pc.location->y;
	uint8_t *f_cells = (uint8_t*)malloc(sizeof(uint8_t) * dungeon.num_cells);  
	uint16_t f_num_rooms = dungeon.num_rooms;
	uint8_t *f_rooms;
	uint16_t f_num_stair_up = dungeon.num_staircases_up;
	uint8_t *f_stair_up;
	uint16_t f_num_stair_down = dungeon.num_staircases_down;
	uint8_t *f_stair_down;
	
	for (i = 0; i < dungeon.height; i++) {
		for(j = 0; j < dungeon.width; j++) {
		
			f_cells[k] = dungeon.cells[i][j].hardness;
			k++;
		}
	}
	f_rooms = (uint8_t*)malloc(sizeof(uint8_t) * (f_num_rooms*4));
	for (i = 0; i < (f_num_rooms*4); i+=4) {
		
		f_rooms[i] = dungeon.rooms[i/4].tl->x;
		f_rooms[i+1] = dungeon.rooms[i/4].tl->y;
		f_rooms[i+2] = dungeon.rooms[i/4].width;
		f_rooms[i+3] = dungeon.rooms[i/4].height;
	}
	f_stair_up = (uint8_t*)malloc(sizeof(uint8_t) * (f_num_stair_up*2));
	for (i = 0; i < (f_num_stair_up*2); i+=2) {
		
		f_stair_up[i] = dungeon.staircases_up[i/2]->x;
		f_stair_up[i+1] = dungeon.staircases_up[i/2]->y;
	}
	f_stair_down = (uint8_t*)malloc(sizeof(uint8_t) * (f_num_stair_down*2));
	for (i = 0; i < (f_num_stair_down*2); i+=2) {
		
		f_stair_down[i] = dungeon.staircases_down[i/2]->x;
		f_stair_down[i+1] = dungeon.staircases_down[i/2]->y;
	}
	FileWriteBuffer f_buffer[] = {
		
		{
			.buffer = f_type,
			.nmemb 	= strlen(f_type),
			.size 	= sizeof(f_type[0])
			
		},
		{
			.buffer = &f_marker,
			.nmemb 	= 1,
			.size 	= sizeof(f_marker)
			
		},
		{
			.buffer = &f_size,
			.nmemb 	= 1,
			.size 	= sizeof(f_size)
			
		},
		{
			.buffer = &f_pc_x,
			.nmemb 	= 1,
			.size 	= sizeof(f_pc_x)
			
		},
		{
			.buffer = &f_pc_y,
			.nmemb	= 1,
			.size 	= sizeof(f_pc_y)
			
		},
		{
			.buffer = f_cells,
			.nmemb 	= dungeon.num_cells,
			.size 	= sizeof(f_cells[0])
			
		},
		{
			.buffer = &f_num_rooms,
			.nmemb 	= 1,
			.size 	= sizeof(f_num_rooms)
			
		},
		{
			.buffer = f_rooms,
			.nmemb 	= f_num_rooms * 4,
			.size 	= sizeof(f_rooms[0])
			
		},
		{
			.buffer = &f_num_stair_up,
			.nmemb 	= 1,
			.size 	= sizeof(f_num_stair_up)
			
		},
		{
			.buffer = f_stair_up,
			.nmemb 	= f_num_stair_up*2,
			.size 	= sizeof(f_stair_up[0])
			
		},
		{
			.buffer = &f_num_stair_down,
			.nmemb 	= 1,
			.size 	= sizeof(f_num_stair_down)
			
		},
		{
			.buffer = f_stair_down,
			.nmemb 	= f_num_stair_down * 2,
			.size 	= sizeof(f_stair_down[0])
			
		}
	};
	
	/* write dungeon to file, keeping track of the size of the file as it increases */
	utils_fwrite_m(f_name, 12, 0, f_buffer, 0, 1, 1);
//	f_write_len = utils_fwrite_m(f_name, 12, 0, f_buffer, 0, 1, 1);
	f_size = utils_fsize(f_name, 1);
	FileWriteBuffer f_size_buffer = {
		
		.buffer = &f_size,
		.nmemb 	= 1,
		.size = sizeof(uint32_t)
	};
	utils_fwrite(f_name, f_size_buffer, (sizeof(char) * strlen(f_type))+(sizeof(uint32_t)), 0, 1);
	
	/* write dungeon to file, keeping track of the size of the file as it increases */
//	printf("debug -- file write [%ld] bytes\n", f_write_len);
	
	return 0;
}