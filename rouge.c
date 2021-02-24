/******** include std libs ********/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <sys/stat.h>
/******* include custom libs ******/
#include "rouge.h"

/****** function definitions ******/
int main(int argc, char *argv[]) {
	
	int run_args[ARGS_LENGTH][ARGS_DEPTH] = {
		{ 0, 0 }, 	//print types: [0, *] = dungeon, no border; [1, *] = dungeon, with border; [2, *] = dungeon, filled walls; [*, 0] = print cell type; [*, 1] = print cell hardness
		{ 0, 0 },	//load dungeon in from file before running game
		{ 0, 0 }, 	//save current dungeon to file after dungeon is created
		{ 0, 0 },	//print color
	};
	Dungeon dungeon;
	
	/* parse command args */
	args_parse(argc, argv, &run_args);
	
	/* load dungeon from file if desired. else, create new dungeon */
	rouge_init(&dungeon, argv, run_args);
	
	/* main game logic */
	rouge_run(&dungeon, run_args);
	
	/* save dungeon to file if desired */
	rouge_clean(&dungeon, argv, run_args);
	
	return 0;
}

void args_parse(int argc, char *argv[], int (*run_args)[ARGS_LENGTH][ARGS_DEPTH]) {
	
	int i = 0;
	
	for (i = 1; i < argc; i++) {
		
		if (!strcmp(argv[i], "--print") || !strcmp(argv[i], "--p")) { //print a (dungeon border) or (filled-in walls and empty rooms)
			
			if (i+1 < argc && !strcmp(argv[i+1], "b")) { (*run_args)[0][0] = 1;  } //print (dungeon border)
			else if (i+1 < argc && !strcmp(argv[i+1], "f")) { (*run_args)[0][0] = 2; } //print (filled-in walls and empty rooms)
		}
		if (!strcmp(argv[i], "--print-cell") || !strcmp(argv[i], "--pc")) { //print the (cell type) or (cell hardness)
			
			if (i+1 < argc && !strcmp(argv[i+1], "h")) { (*run_args)[0][1] = 1; } //print cell hardness
			else if (i+1 < argc && !strcmp(argv[i+1], "nt")) { (*run_args)[0][1] = 2;  } //print cell weight (tunneling)
			else if (i+1 < argc && !strcmp(argv[i+1], "t")) { (*run_args)[0][1] = 3; } //print cell weight (non-tunneling)
		}
		
		if (!strcmp(argv[i], "--load") || !strcmp(argv[i], "--l")) { //load dungeon from bit-file
			
			(*run_args)[1][0] = 1;
			if (i+1 < argc && argv[i+1][0] != '-') { (*run_args)[1][1] = i+1; }
		} 
		
		if (!strcmp(argv[i], "--save") || !strcmp(argv[i], "--s")) { //save generated dungeon to bit-file
			
			(*run_args)[2][0] = 1;
			if (i+1 < argc && argv[i+1][0] != '-') { (*run_args)[2][1] = i+1; }
		}
		
		if (!strcmp(argv[i], "--color")) { (*run_args)[3][0] = 1; } //print PC and NPCs in color
	}
	
	return;
}

void rouge_init(Dungeon *dungeon, char *argv[], int run_args[ARGS_LENGTH][ARGS_DEPTH]) {
	
	if (run_args[1][0]) {
		
//		printf("debug -- loading dungeon from bit-file\n");
		
		dungeon_mem_init(dungeon, (uint8_t)DUNGEON_HEIGHT, (uint8_t)DUNGEON_WIDTH);
		
		char *f_name = "";
		if (run_args[1][1]) {
			
			f_name = (char*)malloc(sizeof(char) * strlen(argv[run_args[1][1]]) + 1);
			f_name[sizeof(f_name)-1] = '\0';
			strcpy(f_name, argv[run_args[1][1]]); 
		}
		
		printf("%s\n", f_name);
		int read_invalid = fread_dungeon(dungeon, f_name);
		if (read_invalid) { 
		
//			printf("Dungeon read failed. Code [%d]\n", read_invalid);
			
			dungeon_init(dungeon, (uint8_t)DUNGEON_HEIGHT, (uint8_t)DUNGEON_WIDTH);
		}
	}
	else { dungeon_init(dungeon, (uint8_t)DUNGEON_HEIGHT, (uint8_t)DUNGEON_WIDTH); }
	
	return;
}

void rouge_run(Dungeon *dungeon, int run_args[ARGS_LENGTH][ARGS_DEPTH]) {
	
	/* print created/loaded dungeon with desired print flags */
	dungeon_print(*dungeon, run_args[0][0], run_args[3][0], 0); //replace last argument with run_args[0][1] after assignment 1.03
	
	/* generate paths to player */
	pathfinder_ntunneling(dungeon, dungeon->pc.location); //generate non-tunneling paths to player
//	pathfinder_tunneling(dungeon, dungeon->pc.location); //generate tunneling paths to player
	
	/* print generated path for non-tunneling entities */
	dungeon_print(*dungeon, run_args[0][0], run_args[3][0], 2); //temporary for assignment 1.03
	/* print generated path for tunneling entities */
//	dungeon_print(*dungeon, run_args[0][0], run_args[3][0], 3); //temporary for assignment 1.03
	/* print cell hardness values */
//	dungeon_print(*dungeon, run_args[0][0], run_args[3][0], 1); //temporary for assignment 1.03
	
	return;
}

void rouge_clean(Dungeon *dungeon, char *argv[], int run_args[ARGS_LENGTH][ARGS_DEPTH]) {
	
	if (run_args[2][0]) {
		
//		printf("debug -- writing dungeon to bit-file\n");
		
		char *f_name = "";
		if (run_args[2][1]) {
			
			f_name = (char*)malloc(sizeof(char) * strlen(argv[run_args[2][1]]));
			f_name[sizeof(f_name)-1] = '\0';
			strcpy(f_name, argv[run_args[2][1]]); 
		}
		
		int write_invalid = fwrite_dungeon(*dungeon, f_name);
		if (write_invalid) { printf("Dungeon write failed. Code [%d]\n", write_invalid); }
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
	char* f_type = (char*)calloc(strlen("RLG327-S2021"), sizeof(char));
	uint32_t f_marker = 0;
	uint32_t f_size = 0;
	uint8_t f_pc_x = 0;
	uint8_t f_pc_y = 0;
	uint8_t *f_cells = (uint8_t*)calloc(dungeon->num_cells, sizeof(uint8_t));
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
	f_rooms = calloc((f_num_rooms*4), f_buffer[7].size);
	f_buffer[7].buffer = f_rooms;
	f_buffer[7].nmemb = f_num_rooms * 4;
	f_read_offset += utils_fread_m(f_name, 9, 7, f_buffer, f_read_offset, 1);
	f_stairs_up = calloc((f_num_stairs_up*2), f_buffer[9].size);
	f_buffer[9].buffer = f_stairs_up;
	f_buffer[9].nmemb = f_num_stairs_up * 2;
	f_read_offset += utils_fread_m(f_name, 11, 9, f_buffer, f_read_offset, 1);
	f_stairs_down = calloc((f_num_stairs_down*2), f_buffer[11].size);
	f_buffer[11].buffer = f_stairs_down;
	f_buffer[11].nmemb = f_num_stairs_up * 2;
	f_read_offset += utils_fread(f_name, f_buffer[11], f_read_offset, 1);
	
//	printf("debug -- file read complete\n");
	
	/* set dungeon cell hardness */
	dungeon_generate_cells(dungeon);
	for (i = 0; i < dungeon->height; i++) {
		for (j = 0; j < dungeon->width; j++) {
			
			dungeon->cells[i][j].hardness = f_cells[k];
			k++;
		}
	}
//	printf("debug -- set cell hardness\n");
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
	dungeon->staircases_up = (Cell**)calloc(f_num_stairs_up, sizeof(Cell*));
	for (i = 0; i < (f_num_stairs_up*2); i+=2) {
		
		dungeon->cells[f_stairs_up[i+1]][f_stairs_up[i]].type = CellType_Stair_up;
		dungeon->cells[f_stairs_up[i+1]][f_stairs_up[i]].hardness = 0;
		dungeon->staircases_up[i/2] = &(dungeon->cells[f_stairs_up[i+1]][f_stairs_up[i]]);
	}
//	printf("debug -- marked up staircases\n");
	/* place down staircases in dungeon */
	dungeon->num_staircases_down = f_num_stairs_down;
	dungeon->staircases_down = (Cell**)calloc(f_num_stairs_down, sizeof(Cell*));
	for (i = 0; i < (f_num_stairs_down*2); i+=2) {
		
		dungeon->cells[f_stairs_down[i+1]][f_stairs_down[i]].type = CellType_Stair_down;
		dungeon->cells[f_stairs_down[i+1]][f_stairs_down[i]].hardness = 0;
		dungeon->staircases_down[i/2] = &(dungeon->cells[f_stairs_down[i+1]][f_stairs_down[i]]);
	}
//	printf("debug -- marked down staircases\n");
	/* place pc in dungeon */
	Coordinate pc_loc = {
		
		.y = f_pc_y,
		.x = f_pc_x
	};
	Character pc = character_init(0, pc_loc, CharacterType_PC);
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
		
		f_name = (char*)calloc((strlen(getenv("HOME")) + 17), sizeof(char));
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
	uint8_t f_pc_x = dungeon.pc.location.x;
	uint8_t f_pc_y = dungeon.pc.location.y;
	uint8_t *f_cells = (uint8_t*)calloc(dungeon.num_cells, sizeof(uint8_t));  
	uint16_t f_num_rooms = dungeon.num_rooms;
	uint8_t *f_rooms = (uint8_t*)calloc((f_num_rooms*4), sizeof(uint8_t));
	uint16_t f_num_stair_up = dungeon.num_staircases_up;
	uint8_t *f_stair_up = (uint8_t*)calloc((f_num_stair_up*2), sizeof(uint8_t));
	uint16_t f_num_stair_down = dungeon.num_staircases_down;
	uint8_t *f_stair_down = (uint8_t*)calloc((f_num_stair_down*2), sizeof(uint8_t));
	
	for (i = 0; i < dungeon.height; i++) {
		for(j = 0; j < dungeon.width; j++) {
		
			f_cells[k] = dungeon.cells[i][j].hardness;
			k++;
		}
	}
	for (i = 0; i < (f_num_rooms*4); i+=4) {
		
		f_rooms[i] = dungeon.rooms[i/4].tl.x;
		f_rooms[i+1] = dungeon.rooms[i/4].tl.y;
		f_rooms[i+2] = dungeon.rooms[i/4].width;
		f_rooms[i+3] = dungeon.rooms[i/4].height;
	}
	for (i = 0; i < (f_num_stair_up*2); i+=2) {
		
		f_stair_up[i] = dungeon.staircases_up[i/2]->location.x;
		f_stair_up[i+1] = dungeon.staircases_up[i/2]->location.y;
	}
	for (i = 0; i < (f_num_stair_down*2); i+=2) {
		
		f_stair_down[i] = dungeon.staircases_down[i/2]->location.x;
		f_stair_down[i+1] = dungeon.staircases_down[i/2]->location.y;
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
			.nmemb 	= f_num_stair_down*2,
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