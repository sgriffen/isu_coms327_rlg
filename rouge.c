/******** include std libs ********/
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <ncurses.h>
/******* include custom libs ******/
#include "./rouge.h"
#include "./src/utils/math_utils.h"

/****** function definitions ******/
int main(int argc, char *argv[]) {
	
	RunArgs run_args = {
		
		.print_config = {
			.fill = 0,
			.color = 0,
			.hardness = 0,
			.weight_ntunneling = 0,
			.weight_tunneling = 0,
			.traversal_cost = 0
		},
		.file_load = 0,
		.load_dir = NULL,
		.file_save = 0,
		.save_dir = NULL,
		.num_npcs = 0,
		.num_dungeons = 0
	};
	GameState g_state;
	
	srand(time(NULL));
	
	/* parse command args */
	args_parse(argc, argv, &run_args);
	
	/* load dungeons from file if desired. else, create new dungeons */
	rouge_init(&g_state, argv, run_args);
	
	/* main game logic */
	state_run(&g_state, run_args);
	
	/* save dungeons to file if desired */
	rouge_clean(&g_state, argv, run_args);
	
	return 0;
}

void args_parse(int argc, char *argv[], RunArgs *run_args) {
	
	int i = 0;
	
	for (i = 1; i < argc; i++) {
		
		if (i+1 < argc && (!strcmp(argv[i], "--print") || !strcmp(argv[i], "--p"))) { //print a (dungeons border) or (filled-in walls and empty rooms)
			
			if (!strcmp(argv[i+1], "b")) 		{ run_args->print_config.fill = 1; } //print (dungeons border)
			else if (!strcmp(argv[i+1], "f")) 	{ run_args->print_config.fill = 2; } //print (filled-in walls and empty rooms)
		}
		if (i+1 < argc && (!strcmp(argv[i], "--print-cell") || !strcmp(argv[i], "--pc"))) { //print the (cell type) or (cell hardness)
			
			while (i+1 < argc && argv[i+1][0] != '-') {
				
				if (!strcmp(argv[i+1], "h")) 		{ run_args->print_config.hardness = 1; } 			//print cell hardness
				else if (!strcmp(argv[i+1], "nt")) 	{ run_args->print_config.weight_ntunneling = 1; } 	//print cell weight (non-tunneling)
				else if (!strcmp(argv[i+1], "t")) 	{ run_args->print_config.weight_tunneling = 1; } 	//print cell weight (tunneling)
				else if (!strcmp(argv[i+1], "c"))	{ run_args->print_config.traversal_cost = 1; } 	//print cell traversal cost
			
				i++;
			}
		}
		
		if (!strcmp(argv[i], "--load") || !strcmp(argv[i], "--l")) { //load dungeons from bit-file
			
			run_args->file_load = 1;
			if (i+1 < argc && argv[i+1][0] != '-') {
				
				run_args->load_dir = (char*)calloc(strlen(argv[i+1]), sizeof(char));
				strcpy(run_args->load_dir, argv[i+1]);
			}
		}
		
		if (!strcmp(argv[i], "--save") || !strcmp(argv[i], "--s")) { //save generated dungeons to bit-file
			
			run_args->file_save = 1;
			if (i+1 < argc && argv[i+1][0] != '-') {
				
				run_args->save_dir = (char*)calloc(strlen(argv[i+1]), sizeof(char));
				strcpy(run_args->save_dir, argv[i+1]);
			}
		}
		
		if (!strcmp(argv[i], "--color")) { run_args->print_config.color = 1; } //print PC and NPCs in color
		
		if (!strcmp(argv[i], "--nummon") || !strcmp(argv[i], "--nm")) { run_args->num_npcs = atoi(argv[i+1]); } //define number of npcs per dungeons
		
		if (!strcmp(argv[i], "--numdun") || !strcmp(argv[i], "--nd")) { run_args->num_dungeons = atoi(argv[i+1]); } //define number dungeons
	}
	
	if (!(run_args->num_dungeons)) { run_args->num_dungeons = utils_rand_between(2, 10, NULL); }
	
	return;
}

void rouge_init(GameState *g_state, char *argv[], RunArgs run_args) {
	
	if (run_args.file_load) {
		
		/* initialize dungeons memory */
//		dungeons_mem_init(dungeons, (uint8_t)DUNGEON_HEIGHT, (uint8_t)DUNGEON_WIDTH);
		
//		int read_invalid = fread_dungeons(dungeons, run_args.load_dir);
//		if (read_invalid) { *g_state = state_init(run_args.num_dungeons, run_args.num_npcs); }
		*g_state = state_init(run_args.num_dungeons, run_args.num_npcs);
	} else { *g_state = state_init(run_args.num_dungeons, run_args.num_npcs); }
	
	rouge_init_terminal();
	
	return;
}

void rouge_init_terminal() {
	
	initscr();
	raw();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
}

void rouge_clean(GameState *g_state, char *argv[], RunArgs run_args) {
	
//	if (run_args.file_save) { fwrite_dungeons(*dungeons, run_args.save_dir); }
	
	free(g_state->dungeons);
	rouge_clean_terminal();
	
	return;
}

void rouge_clean_terminal() {
	
	endwin();
}

//int fread_game(GameState *g_state, char *f_name) {
//	
//	struct stat st = {0};
//	size_t f_read_offset = 0;
//	
//	if (f_name == NULL) {
//	
//		f_name = (char*)malloc(sizeof(char) * (strlen(getenv("HOME")) + 17));
//		f_name[sizeof(f_name)-1] = '\0';
//		
//		strcpy(f_name, getenv("HOME"));
//		strcat(f_name, "/.rlg327/");
//		if (stat(f_name, &st) == -1) { return 1; }
//		strcat(f_name, "dungeons");
//	}
//	
//	/* allocate initial memory to read to and create FileReadBuffers */
//	char* f_type = (char*)calloc(strlen("RLG327-S2021"), sizeof(char));
//	uint32_t f_marker = 0;
//	uint32_t f_size = 0;
//	uint8_t f_pc_x = 0;
//	uint8_t f_pc_y = 0;
//	uint8_t *f_cells = (uint8_t*)calloc(dungeons->num_cells, sizeof(uint8_t));
//	uint16_t f_num_rooms = 0;
//	uint8_t *f_rooms = NULL;
//	uint16_t f_num_stairs_up = 0;
//	uint8_t *f_stairs_up = NULL;
//	uint16_t f_num_stairs_down = 0;
//	uint8_t *f_stairs_down = NULL;
//	
//	FileReadBuffer f_buffer[] = {
//		{
//			.buffer = f_type,
//			.nmemb 	= strlen("RLG327-S2021"),
//			.size 	= sizeof(char)
//		},
//		{
//			.buffer = &f_marker,
//			.nmemb 	= 1,
//			.size 	= sizeof(uint32_t)
//		},
//		{
//			.buffer = &f_size,
//			.nmemb 	= 1,
//			.size 	= sizeof(uint32_t)
//		},
//		{
//			.buffer = &f_pc_x,
//			.nmemb 	= 1,
//			.size 	= sizeof(uint8_t)
//		},
//		{
//			.buffer = &f_pc_y,
//			.nmemb 	= 1,
//			.size 	= sizeof(uint8_t)
//		},
//		{
//			.buffer = f_cells,
//			.nmemb 	= dungeons->num_cells,
//			.size 	= sizeof(uint8_t)
//		},
//		{
//			.buffer = &f_num_rooms,
//			.nmemb 	= 1,
//			.size 	= sizeof(uint16_t)
//		},
//		{
//			.buffer = NULL,
//			.nmemb 	= 0,
//			.size 	= sizeof(uint8_t)
//		},
//		{
//			.buffer = &f_num_stairs_up,
//			.nmemb 	= 1,
//			.size 	= sizeof(uint16_t)
//		},
//		{
//			.buffer = NULL,
//			.nmemb 	= 0,
//			.size 	= sizeof(uint8_t)
//		},
//		{
//			.buffer = &f_num_stairs_down,
//			.nmemb 	= 1,
//			.size 	= sizeof(uint16_t)
//		},
//		{
//			.buffer = NULL,
//			.nmemb 	= 0,
//			.size 	= sizeof(uint8_t)
//		}
//	};
//
//	/* read dungeons from file */
//	f_read_offset = utils_fread(f_name, f_buffer[5], 0);
//	f_read_offset += utils_fread_m(f_name, 7, 0, f_buffer, 0, 1);
//	f_rooms = calloc((f_num_rooms*4), f_buffer[7].size);
//	f_buffer[7].buffer = f_rooms;
//	f_buffer[7].nmemb = f_num_rooms * 4;
//	f_read_offset += utils_fread_m(f_name, 9, 7, f_buffer, f_read_offset, 1);
//	f_stairs_up = calloc((f_num_stairs_up*2), f_buffer[9].size);
//	f_buffer[9].buffer = f_stairs_up;
//	f_buffer[9].nmemb = f_num_stairs_up * 2;
//	f_read_offset += utils_fread_m(f_name, 11, 9, f_buffer, f_read_offset, 1);
//	f_stairs_down = calloc((f_num_stairs_down*2), f_buffer[11].size);
//	f_buffer[11].buffer = f_stairs_down;
//	f_buffer[11].nmemb = f_num_stairs_up * 2;
//	f_read_offset += utils_fread(f_name, f_buffer[11], f_read_offset, 1);
//	
//	dungeons_finit(dungeons, f_pc_y, f_pc_x, f_cells, f_num_rooms, f_rooms, f_num_stairs_up, f_stairs_up, f_num_stairs_down, f_stairs_down);
//	
//	return 0;
//}

//int fwrite_dungeons(Dungeon dungeons, char *f_name) {
//	
//	int i = 0, j = 0, k = 0;
////	size_t f_write_len = 0;
//	struct stat st = {0};
//	
//	if (f_name == NULL) {
//		
//		f_name = (char*)calloc((strlen(getenv("HOME")) + 17), sizeof(char));
//		f_name[sizeof(f_name)-1] = '\0';
//		
//		strcpy(f_name, getenv("HOME"));
//		strcat(f_name, "/.rlg327/");
//		if (stat(f_name, &st) == -1) { mkdir(f_name, 0777); }
//		strcat(f_name, "dungeons");
//	}
//	
//	/* prepare dungeons information for writing */
//	char *f_type = "RLG327-S2021";
//	uint32_t f_marker = 0;
//	uint32_t f_size = 0;
//	uint8_t f_pc_x = dungeons.pc.location.x;
//	uint8_t f_pc_y = dungeons.pc.location.y;
//	uint8_t *f_cells = (uint8_t*)calloc(dungeons.num_cells, sizeof(uint8_t));  
//	uint16_t f_num_rooms = dungeons.num_rooms;
//	uint8_t *f_rooms = (uint8_t*)calloc((f_num_rooms*4), sizeof(uint8_t));
//	uint16_t f_num_stair_up = dungeons.num_staircases_up;
//	uint8_t *f_stair_up = (uint8_t*)calloc((f_num_stair_up*2), sizeof(uint8_t));
//	uint16_t f_num_stair_down = dungeons.num_staircases_down;
//	uint8_t *f_stair_down = (uint8_t*)calloc((f_num_stair_down*2), sizeof(uint8_t));
//	
//	for (i = 0; i < dungeons.height; i++) {
//		for(j = 0; j < dungeons.width; j++) {
//		
//			f_cells[k] = dungeons.cells[i][j].hardness;
//			k++;
//		}
//	}
//	for (i = 0; i < (f_num_rooms*4); i+=4) {
//		
//		f_rooms[i] = dungeons.rooms[i/4].tl.x;
//		f_rooms[i+1] = dungeons.rooms[i/4].tl.y;
//		f_rooms[i+2] = dungeons.rooms[i/4].width;
//		f_rooms[i+3] = dungeons.rooms[i/4].height;
//	}
//	for (i = 0; i < (f_num_stair_up*2); i+=2) {
//		
//		f_stair_up[i] = dungeons.staircases_up[i/2]->location.x;
//		f_stair_up[i+1] = dungeons.staircases_up[i/2]->location.y;
//	}
//	for (i = 0; i < (f_num_stair_down*2); i+=2) {
//		
//		f_stair_down[i] = dungeons.staircases_down[i/2]->location.x;
//		f_stair_down[i+1] = dungeons.staircases_down[i/2]->location.y;
//	}
//	
//	FileWriteBuffer f_buffer[] = {
//		
//		{
//			.buffer = f_type,
//			.nmemb 	= strlen(f_type),
//			.size 	= sizeof(f_type[0])
//			
//		},
//		{
//			.buffer = &f_marker,
//			.nmemb 	= 1,
//			.size 	= sizeof(f_marker)
//			
//		},
//		{
//			.buffer = &f_size,
//			.nmemb 	= 1,
//			.size 	= sizeof(f_size)
//			
//		},
//		{
//			.buffer = &f_pc_x,
//			.nmemb 	= 1,
//			.size 	= sizeof(f_pc_x)
//			
//		},
//		{
//			.buffer = &f_pc_y,
//			.nmemb	= 1,
//			.size 	= sizeof(f_pc_y)
//			
//		},
//		{
//			.buffer = f_cells,
//			.nmemb 	= dungeons.num_cells,
//			.size 	= sizeof(f_cells[0])
//			
//		},
//		{
//			.buffer = &f_num_rooms,
//			.nmemb 	= 1,
//			.size 	= sizeof(f_num_rooms)
//			
//		},
//		{
//			.buffer = f_rooms,
//			.nmemb 	= f_num_rooms * 4,
//			.size 	= sizeof(f_rooms[0])
//			
//		},
//		{
//			.buffer = &f_num_stair_up,
//			.nmemb 	= 1,
//			.size 	= sizeof(f_num_stair_up)
//			
//		},
//		{
//			.buffer = f_stair_up,
//			.nmemb 	= f_num_stair_up*2,
//			.size 	= sizeof(f_stair_up[0])
//			
//		},
//		{
//			.buffer = &f_num_stair_down,
//			.nmemb 	= 1,
//			.size 	= sizeof(f_num_stair_down)
//			
//		},
//		{
//			.buffer = f_stair_down,
//			.nmemb 	= f_num_stair_down*2,
//			.size 	= sizeof(f_stair_down[0])
//			
//		}
//	};
//	
//	/* write dungeons to file, keeping track of the size of the file as it increases */
//	utils_fwrite_m(f_name, 12, 0, f_buffer, 0, 1, 1);
//	f_write_len = utils_fwrite_m(f_name, 12, 0, f_buffer, 0, 1, 1);
//	f_size = utils_fsize(f_name, 1);
//	FileWriteBuffer f_size_buffer = {
//		
//		.buffer = &f_size,
//		.nmemb 	= 1,
//		.size = sizeof(uint32_t)
//	};
//	utils_fwrite(f_name, f_size_buffer, (sizeof(char) * strlen(f_type))+(sizeof(uint32_t)), 0, 1);
//	
//	return 0;
//}