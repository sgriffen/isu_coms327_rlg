/******** include std libs ********/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
/******* include custom libs ******/
#include "rouge.h"

/****** function definitions ******/
int main(int argc, char *argv[]) {
	
	RunArgs run_args = {
		
		.fps = 1.0,
		
		.print_type = 0,
		.print_color = 0,
		.print_hardness = 0,
		.print_weight_ntunneling = 0,
		.print_weight_tunneling = 0,
		.print_traversal_cost = 0,
		
		.file_load = 0,
		.load_dir = NULL,
		.file_save = 0,
		.save_dir = NULL,
		.num_npcs = 0
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

void args_parse(int argc, char *argv[], RunArgs *run_args) {
	
	int i = 0;
	
	for (i = 1; i < argc; i++) {
		
		if (i+1 < argc && (!strcmp(argv[i], "--print") || !strcmp(argv[i], "--p"))) { //print a (dungeon border) or (filled-in walls and empty rooms)
			
			if (!strcmp(argv[i+1], "b")) 		{ run_args->print_type = 1; } //print (dungeon border)
			else if (!strcmp(argv[i+1], "f")) 	{ run_args->print_type = 2; } //print (filled-in walls and empty rooms)
		}
		if (i+1 < argc && (!strcmp(argv[i], "--print-cell") || !strcmp(argv[i], "--pc"))) { //print the (cell type) or (cell hardness)
			
			while (i+1 < argc && argv[i+1][0] != '-') {
				
				if (!strcmp(argv[i+1], "h")) 		{ run_args->print_hardness = 1; } 			//print cell hardness
				else if (!strcmp(argv[i+1], "nt")) 	{ run_args->print_weight_ntunneling = 1; } 	//print cell weight (non-tunneling)
				else if (!strcmp(argv[i+1], "t")) 	{ run_args->print_weight_tunneling = 1; } 	//print cell weight (tunneling)
				else if (!strcmp(argv[i+1], "c"))	{ run_args->print_traversal_cost = 1; } 	//print cell traversal cost
			
				i++;
			}
		}
		
		if (!strcmp(argv[i], "--load") || !strcmp(argv[i], "--l")) { //load dungeon from bit-file
			
			run_args->file_load = 1;
			if (i+1 < argc && argv[i+1][0] != '-') {
				
				run_args->load_dir = (char*)calloc(strlen(argv[i+1]), sizeof(char));
				strcpy(run_args->load_dir, argv[i+1]);
			}
		}
		
		if (!strcmp(argv[i], "--save") || !strcmp(argv[i], "--s")) { //save generated dungeon to bit-file
			
			run_args->file_save = 1;
			if (i+1 < argc && argv[i+1][0] != '-') {
				
				run_args->save_dir = (char*)calloc(strlen(argv[i+1]), sizeof(char));
				strcpy(run_args->save_dir, argv[i+1]);
			}
		}
		
		if (!strcmp(argv[i], "--color")) { run_args->print_color = 1; } //print PC and NPCs in color
		
		if (!strcmp(argv[i], "--nummon") || !strcmp(argv[i], "--nm")) { run_args->num_npcs = atoi(argv[i+1]); } //define number of npcs
		
		if (!strcmp(argv[i], "--fps")) { run_args->fps = (float)atof(argv[i+1]); } //define fps
	}
	
	return;
}

void rouge_init(Dungeon *dungeon, char *argv[], RunArgs run_args) {
	
	if (run_args.file_load) {
		
//		printf("-- debug -- loading dungeon from bit-file\n");
		
		/* initialize dungeon memory */
		dungeon_mem_init(dungeon, (uint8_t)DUNGEON_HEIGHT, (uint8_t)DUNGEON_WIDTH);
		
		int read_invalid = fread_dungeon(dungeon, run_args.load_dir);
		if (read_invalid) { 
		
			printf("Dungeon read failed. Code [%d]\n", read_invalid);
			
			dungeon_init(dungeon, (uint8_t)DUNGEON_HEIGHT, (uint8_t)DUNGEON_WIDTH, run_args.num_npcs);
		}
	}
	else { dungeon_init(dungeon, (uint8_t)DUNGEON_HEIGHT, (uint8_t)DUNGEON_WIDTH, run_args.num_npcs); }
	
	return;
}

void rouge_run(Dungeon *dungeon, RunArgs run_args) {
	
	int i = 0;
	
	uint64_t turn = 0;
	uint64_t queue_position = 0;
	uint8_t pc_moved = 0;
	
	/* wrap pc and npc inside Character wrappers for queue */
    Character_Wrapper *characters = (Character_Wrapper*)calloc((dungeon->num_npcs)+1, sizeof(Character_Wrapper));
	Character_Wrapper wrapper_pc = {
		
		.pc = &(dungeon->pc),
		.npc = NULL
	};
    characters[0] = wrapper_pc;
	for (i = 0; i < (dungeon->num_npcs); i++) {
		
		Character_Wrapper wrapper_npc = {
			
			.pc = NULL,
			.npc = &(dungeon->npcs[i])
		};
        characters[i+1] = wrapper_npc;
	}

    /* organize pc and npcs into priority queue based on movement speed */
    Queue movement_queue = queue_init((dungeon->num_npcs)+1);
    for (i = 0; i < dungeon->num_npcs; i++) {

        queue_enqueue(&movement_queue, queue_node_init(&(characters[i+1]), CHARACTER_TURN(characters[i+1].npc->speed)));
        dungeon->cells[characters[i+1].npc->location.y][characters[i+1].npc->location.x].character.npc = characters[i+1].npc;
    }
    queue_enqueue(&movement_queue, queue_node_init(&(characters[0]), CHARACTER_TURN(characters[0].pc->speed)));
    dungeon->cells[characters[0].pc->location.y][characters[0].pc->location.x].character.pc = characters[0].pc;

    queue_position = movement_queue.nodes[0].priority + 1;
	
	while (dungeon->pc.hp > 0 && dungeon_contains_npcs(dungeon)) {
	
		/* generate paths to player */
		pathfinder_ntunneling(dungeon, &(dungeon->pc.location)); //generate non-tunneling paths to player
		pathfinder_tunneling(dungeon, &(dungeon->pc.location)); //generate tunneling paths to player
		
		/* move npc and npc */
		rouge_turn(dungeon, run_args, characters, &movement_queue, &turn, &queue_position, &pc_moved);
		
		/* print created/loaded dungeon with desired print flags */
		if (run_args.print_weight_ntunneling) 	{ dungeon_print(*dungeon, run_args.print_type, run_args.print_color, 2); }	//print non-tunneling weights
		if (run_args.print_weight_tunneling) 	{ dungeon_print(*dungeon, run_args.print_type, run_args.print_color, 3); }	//print tunneling weights
		if (run_args.print_hardness) 			{ dungeon_print(*dungeon, run_args.print_type, run_args.print_color, 1); }	//print cell hardness
		if (run_args.print_traversal_cost) 		{ dungeon_print(*dungeon, run_args.print_type, run_args.print_color, 4); }	//print traversal cost
	}
	
	free(characters);
	
	rouge_gameover(dungeon, run_args, turn, pc_moved);
	return;
}

void rouge_turn(Dungeon *dungeon, RunArgs run_args, Character_Wrapper *characters, Queue *movement_queue, uint64_t *turn, uint64_t *queue_position, uint8_t *pc_moved) {

	int i = 0;
	*pc_moved = 0;

	if (!(*turn)) { dungeon_print(*dungeon, run_args.print_type, run_args.print_color, 0); }

	/* dequeue nodes, stopping at pc node */
	while ((i < ((movement_queue->index) + 1)) && !queue_is_empty(*movement_queue)) {
		
		QueueNode node = queue_dequeue(movement_queue);
        node.priority = *queue_position;
		Character_Wrapper *wrapper = (Character_Wrapper*)(node.element);
		
		if (wrapper->pc) {
			
			/* print dungeon with updated npc positions */
			if (*turn) { dungeon_print(*dungeon, run_args.print_type, run_args.print_color, 0); }
			
			/* move pc */
			if (wrapper->pc->hp > 0) {
				
				rouge_move_pc(dungeon, *wrapper, run_args.fps);
				queue_enqueue(movement_queue, node);
				*pc_moved = 1;
			}
		} else {
			
			if (wrapper->npc->hp > 0) {

			    rouge_move_npc(dungeon, *wrapper);
                queue_enqueue(movement_queue, node);
			} else { (dungeon->num_npcs_dead)++; }
		}
		
		(*turn)++;
        (*queue_position)++;
		i++;
	}
}

void rouge_move_pc(Dungeon *dungeon, Character_Wrapper wrapper, float fps) {
	
	Character_PC *pc = wrapper.pc;
	Coordinate next;
	
	next = move_pc(dungeon, pc);
	
	dungeon_resolve_collision(dungeon, wrapper, next);
	if (cell_immutable_ntunneling(dungeon->cells[next.y][next.x])) {
		
		dungeon->cells[next.y][next.x].hardness = 0;
		dungeon->cells[next.y][next.x].type = CellType_Cooridor;
	}
	
	if (fps > 0) { usleep((int)(1000000 / fps)); }
	else {
	
		char temp;
		scanf("%c", &temp); 
	}
}

void rouge_move_npc(Dungeon *dungeon, Character_Wrapper wrapper) {
	
	Character_NPC *npc = wrapper.npc;
	Coordinate next = {
		
		.x = 0,
		.y = 0
	};
	
	switch(npc->type) {
	
	case 1:
	
		next = move_npc1(dungeon, npc);
		break;
	case 2:
		
		next = move_npc2(dungeon, npc);
		break;
	case 3:
		
		next = move_npc3(dungeon, npc);
		break;
	case 4:
		
		next = move_npc4(dungeon, npc);
		break;
	case 5:
		
		next = move_npc5(dungeon, npc);
		break;
	case 6:
		
		next = move_npc6(dungeon, npc);
		break;
	case 7:
		
		next = move_npc7(dungeon, npc);
		break;
	case 8:
		
		next = move_npc8(dungeon, npc);
		break;
	case 9:
		
		next = move_npc9(dungeon, npc);
		break;
	case 10:
		
		next = move_npcA(dungeon, npc);
		break;
	case 11:
		
		next = move_npcB(dungeon, npc);
		break;
	case 12:
		
		next = move_npcC(dungeon, npc);
		break;
	case 13:
		
		next = move_npcD(dungeon, npc);
		break;
	case 14:
		
		next = move_npcE(dungeon, npc);
		break;
	case 15:
		
		next = move_npcF(dungeon, npc);
		break;
	default:
		
		next = move_npc0(dungeon, npc);
		break;
	}
	
	dungeon_resolve_collision(dungeon, wrapper, next);
	
	if ((npc->type & NPC_TYPE_TUNNELING) && cell_immutable_ntunneling(dungeon->cells[next.y][next.x])) {
		
		dungeon->cells[next.y][next.x].hardness = 0;
		dungeon->cells[next.y][next.x].type = CellType_Cooridor;
	}
}

void rouge_gameover(Dungeon *dungeon, RunArgs run_args, uint64_t turn, uint8_t pc_moved) {
	
	if (!pc_moved) { dungeon_print(*dungeon, run_args.print_type, run_args.print_color, 0); }
	
	if (dungeon->pc.hp < 1) { printf("Game Over on turn [%ld] - PC died!\n", turn); }
	else if (!dungeon_contains_npcs(dungeon)) { printf("Game Over on turn [%ld] - PC defeated all monsters!\n", turn); }
	else { printf("Game Over on turn [%ld] - not sure why tho...\n", turn); }
	
	return;
}

void rouge_clean(Dungeon *dungeon, char *argv[], RunArgs run_args) {
	
	if (run_args.file_save) {
		
//		printf("-- debug -- writing dungeon to bit-file\n");
		
//		fwrite_dungeon(*dungeon, run_args.save_dir);
		int write_invalid = fwrite_dungeon(*dungeon, run_args.save_dir);
		if (write_invalid) { printf("Dungeon write failed. Code [%d]\n", write_invalid); }
	}
	
	return;
}

int fread_dungeon(Dungeon *dungeon, char *f_name) {
	
	struct stat st = {0};
	size_t f_read_offset = 0;
	
	if (f_name == NULL) {
	
		f_name = (char*)malloc(sizeof(char) * (strlen(getenv("HOME")) + 17));
		f_name[sizeof(f_name)-1] = '\0';
		
		strcpy(f_name, getenv("HOME"));
		strcat(f_name, "/.rlg327/");
		if (stat(f_name, &st) == -1) { return 1; }
		strcat(f_name, "dungeon");
	}
	
//	printf("-- debug -- file name: [%s]\n", f_name);
//	printf("-- debug -- reading [%ld] bytes\n", utils_fsize(f_name, 1)); 
	
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
	
//	printf("-- debug -- file read complete\n");
	
	dungeon_finit(dungeon, f_pc_y, f_pc_x, f_cells, f_num_rooms, f_rooms, f_num_stairs_up, f_stairs_up, f_num_stairs_down, f_stairs_down);
	
	return 0;
}

int fwrite_dungeon(Dungeon dungeon, char *f_name) {
	
	int i = 0, j = 0, k = 0;
//	size_t f_write_len = 0;
	struct stat st = {0};
	
	if (f_name == NULL) {
		
		f_name = (char*)calloc((strlen(getenv("HOME")) + 17), sizeof(char));
		f_name[sizeof(f_name)-1] = '\0';
		
		strcpy(f_name, getenv("HOME"));
		strcat(f_name, "/.rlg327/");
		if (stat(f_name, &st) == -1) { mkdir(f_name, 0777); }
		strcat(f_name, "dungeon");
	}
	
//	printf("-- debug -- file name: [%s]\n", f_name);
	
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
//	printf("-- debug -- file write [%ld] bytes\n", f_write_len);
	
	return 0;
}