/******** include std libs ********/
#include <stdlib.h>
#include <stdint.h>
#include <ncurses.h>
#include <string.h>

/******* include custom libs ******/
#include "./state.h"
#include "./utils/movement.h"
#include "./utils/math_utils.h"
#include "./utils/pathfinder.h"

/********** definitions **********/


/*********** global vars **********/


/****** function definitions ******/
GameState state_init(uint32_t num_dungeons, uint32_t num_npcs) {
	
	GameState state = {
		
		.allow_move = 0,
		.menu_index = 0,
		.dungeon_index = 0,
		.num_dungeons = num_dungeons,
		.num_dungeons_generated = 0,
		.message = NULL,
		.characters = NULL
	};
	
	state.dungeons = (Dungeon*)calloc(num_dungeons, sizeof(Dungeon));
	state.dungeons[0] = dungeon_init((uint8_t)DUNGEON_HEIGHT, (uint8_t)DUNGEON_WIDTH, num_npcs, 0, 0, 1);
	(state.num_dungeons_generated)++;
	
	state_init_pc(&state);
	
	return state;
}

void state_init_pc(GameState *g_state) {
	
	/*
		player character (pc) is placed at a random location in rooms[0] of dungeon[0]
	*/
	
	Character_PC pc;
	
	Coordinate loc = {
		
		.y = utils_rand_between(g_state->dungeons[0].rooms[0].tl.y+ROOM_BORDER_WIDTH, g_state->dungeons[0].rooms[0].br.y-ROOM_BORDER_WIDTH, NULL),
		.x = utils_rand_between(g_state->dungeons[0].rooms[0].tl.x+ROOM_BORDER_WIDTH, g_state->dungeons[0].rooms[0].br.x-ROOM_BORDER_WIDTH, NULL)
	};
	
	pc = character_init_pc(loc, 1, 1, PC_SPEED);
	g_state->pc = pc;
	
	return;
}

void state_init_queue(GameState *g_state) {
	
	int i = 0;
	
	g_state->characters = (Character_Wrapper*)calloc((g_state->dungeons[g_state->dungeon_index].num_npcs)+1, sizeof(Character_Wrapper));
	g_state->queue = queue_init((g_state->dungeons[g_state->dungeon_index].num_npcs)+1);
	g_state->queue_position = 0;
	
	/* wrap pc and npc inside Character wrappers for queue */
	Character_Wrapper wrapper_pc = {
		
		.pc = &(g_state->pc),
		.npc = NULL
	};
    g_state->characters[0] = wrapper_pc;
	g_state->dungeons[g_state->dungeon_index].cells[g_state->characters[0].pc->location.y][g_state->characters[0].pc->location.x].character = g_state->characters[0];
	for (i = 0; i < (g_state->dungeons[g_state->dungeon_index].num_npcs); i++) {
		
		Character_Wrapper wrapper_npc = {
			
			.pc = NULL,
			.npc = &(g_state->dungeons[g_state->dungeon_index].npcs[i])
		};
        g_state->characters[i+1] = wrapper_npc;
		
		g_state->dungeons[g_state->dungeon_index].cells[g_state->characters[i+1].npc->location.y][g_state->characters[i+1].npc->location.x].character = g_state->characters[i+1];
	}

    /* organize pc and npcs into priority queue based on movement speed */
    for (i = 0; i < g_state->dungeons[g_state->dungeon_index].num_npcs; i++) {

        queue_enqueue(&(g_state->queue), queue_node_init(&(g_state->characters[i+1]), CHARACTER_TURN(g_state->characters[i+1].npc->speed)));
        g_state->dungeons[g_state->dungeon_index].cells[g_state->characters[i+1].npc->location.y][g_state->characters[i+1].npc->location.x].character.npc = g_state->characters[i+1].npc;
    }
    queue_enqueue(&(g_state->queue), queue_node_init(&(g_state->characters[0]), CHARACTER_TURN(g_state->characters[0].pc->speed)));
    g_state->dungeons[g_state->dungeon_index].cells[g_state->characters[0].pc->location.y][g_state->characters[0].pc->location.x].character.pc = g_state->characters[0].pc;

    g_state->queue_position = g_state->queue.nodes[0].priority + 1;
}

void state_deinit_queue(GameState *g_state) {
	
	int i = 0;
	int j = 0;
	
	for (i = 0; i < g_state->dungeons[g_state->dungeon_index].volitile_height; i++) {
		for (j = 0; j < g_state->dungeons[g_state->dungeon_index].volitile_width; j++) {
			
			g_state->dungeons[g_state->dungeon_index].cells[i][j].character.pc = NULL;
			g_state->dungeons[g_state->dungeon_index].cells[i][j].character.npc = NULL;
		}
	}
	
	g_state->queue.index = 0;
	g_state->queue.size = 0;
	free(g_state->queue.nodes);
	free(g_state->characters);
}

void state_run(GameState *g_state, RunArgs run_args) {
	
	state_init_queue(g_state);
	
	while (g_state->pc.hp > 0 && state_contains_npcs(g_state)) {
		
		if (g_state->menu_index) {
			
			state_draw(*g_state, run_args.print_config.fill, run_args.print_config.color, 0);
			state_parse_input(g_state, NULL);
		}
		else {
			
			if (g_state->allow_move) {
				
				pathfinder_ntunneling(&(g_state->dungeons[g_state->dungeon_index]), &(g_state->pc.location));
				pathfinder_tunneling(&(g_state->dungeons[g_state->dungeon_index]), &(g_state->pc.location));
			}
			if (!state_turn(g_state, run_args)) { break; }
		}
	}
	
	state_gameover(g_state, run_args.print_config.fill, run_args.print_config.color, 0);
	
	return;
}

int state_turn(GameState *g_state, RunArgs run_args) {

	int i = 0;
	
	int game_run = 1;
	
	/* draw dungeon with updated npc positions */
	state_draw(*g_state, run_args.print_config.fill, run_args.print_config.color, 0);
	
	/* dequeue nodes, stopping at pc node */
	while ((i < ((g_state->queue.index) + 1)) && !queue_is_empty(g_state->queue)) {
		
		QueueNode node = queue_dequeue(&(g_state->queue));
        node.priority = g_state->queue_position;
		Character_Wrapper *wrapper = (Character_Wrapper*)(node.element);
		
		if (wrapper->pc) {
			
			/* move pc */
			if (wrapper->pc->hp > 0) {
				
				game_run = state_parse_input(g_state, wrapper);
				queue_enqueue(&(g_state->queue), node);
				
				/* draw dungeon with updated npc positions */
				state_draw(*g_state, run_args.print_config.fill, run_args.print_config.color, 0);
			}
		} else {
			
			if (!g_state->allow_move) { queue_enqueue(&(g_state->queue), node); }
			else if (wrapper->npc->hp > 0) {

				state_move_npc(&(g_state->dungeons[g_state->dungeon_index]), *wrapper, g_state->pc);
				queue_enqueue(&(g_state->queue), node);
			} else {
				
				g_state->dungeons[g_state->dungeon_index].cells[wrapper->npc->location.y][wrapper->npc->location.x].character.npc = NULL;
				(g_state->dungeons[g_state->dungeon_index].num_npcs_dead)++;
				(g_state->pc.num_kills)++;
			}
		}
		
		if (g_state->allow_move) { (g_state->turn)++; }
        (g_state->queue_position)++;
		i++;
		
		if (game_run != 1) { break; }
	}
	
	return game_run;
}

int state_parse_input(GameState *g_state, Character_Wrapper *wrapper) {
	
	g_state->message = NULL;
	
	int move_matrix[2] = { 0, 0 };
	
	int key = getch();
	switch (g_state->menu_index) {
		
	case 1:		// in npc menu
		
		g_state->allow_move = 0;
		switch(key) {
		case 0x1b: 			// escape key
			
			g_state->menu_index = 0;
			return 2;
			break;
		case KEY_DOWN:		// down arrow
			
			if (g_state->menu_offset < (g_state->dungeons[g_state->dungeon_index].num_npcs - 20) && g_state->dungeons[g_state->dungeon_index].num_npcs > 21) { (g_state->menu_offset)++; }
			break;
		case KEY_UP:		// up arrow
			
			if (g_state->menu_offset) { (g_state->menu_offset)--; }
			break;
		default: 
			
			
			break; 	//no-op entered, do nothing
		}
		
		return 1;
		break;
	default:	// in no menu
		
		g_state->allow_move = 1;
		switch(key) {

		case KEY_UP:		// keypad '8'
			CASE_MOVE_U:
			
			move_matrix[0] = -1;
			move_matrix[1] = 0;
			if (!state_move_pc(&(g_state->dungeons[g_state->dungeon_index]), *wrapper, move_matrix)) {
				
				g_state->message = (char*)calloc(strlen("Could not move PC up")+1, sizeof(char));
				printf((g_state->message), "Could not move PC up");
			}
			break;
		case KEY_PPAGE:		// keypad '9'
			CASE_MOVE_UR:
			
			move_matrix[0] = -1;
			move_matrix[1] = 1;
			if (!state_move_pc(&(g_state->dungeons[g_state->dungeon_index]), *wrapper, move_matrix)) {
				
				g_state->message = (char*)calloc(strlen("Could not move PC up-right")+1, sizeof(char));
				sprintf((g_state->message), "Could not move PC up-right");
			}
			break;
		case KEY_RIGHT:		// keypad '6'
			CASE_MOVE_R:
			
			move_matrix[0] = 0;
			move_matrix[1] = 1;
			if (!state_move_pc(&(g_state->dungeons[g_state->dungeon_index]), *wrapper, move_matrix)) {
				
				g_state->message = (char*)calloc(strlen("Could not move PC right")+1, sizeof(char));
				sprintf((g_state->message), "Could not move PC right");
			}
			break;
		case KEY_NPAGE:		// keypad '3'
			CASE_MOVE_DR:
			
			move_matrix[0] = 1;
			move_matrix[1] = 1;
			if (!state_move_pc(&(g_state->dungeons[g_state->dungeon_index]), *wrapper, move_matrix)) {
				
				g_state->message = (char*)calloc(strlen("Could not move PC down-right")+1, sizeof(char));
				sprintf((g_state->message), "Could not move PC down-right");
			}
			break;
		case KEY_DOWN:		// keypad '2'
			CASE_MOVE_D:
			
			move_matrix[0] = 1;
			move_matrix[1] = 0;
			if (!state_move_pc(&(g_state->dungeons[g_state->dungeon_index]), *wrapper, move_matrix)) {
				
				g_state->message = (char*)calloc(strlen("Could not move PC down")+1, sizeof(char));
				sprintf((g_state->message), "Could not move PC down");
			}
			break;
		case KEY_END:		// keypad '1'
			CASE_MOVE_DL:
			
			move_matrix[0] = 1;
			move_matrix[1] = -1;
			if (!state_move_pc(&(g_state->dungeons[g_state->dungeon_index]), *wrapper, move_matrix)) {
				
				g_state->message = (char*)calloc(strlen("Could not move PC down-left")+1, sizeof(char));
				sprintf((g_state->message), "Could not move PC down-left");
			}
			break;
		case KEY_LEFT:		// keypad '4'
			CASE_MOVE_L:
			
			move_matrix[0] = 0;
			move_matrix[1] = -1;
			if (!state_move_pc(&(g_state->dungeons[g_state->dungeon_index]), *wrapper, move_matrix)) {
				
				g_state->message = (char*)calloc(strlen("Could not move PC left")+1, sizeof(char));
				sprintf((g_state->message), "Could not move PC left");
			}
			break;
		case KEY_HOME:		// keypad '7'
			CASE_MOVE_UL:
			
			move_matrix[0] = -1;
			move_matrix[1] = -1;
			if (!state_move_pc(&(g_state->dungeons[g_state->dungeon_index]), *wrapper, move_matrix)) {
				
				g_state->message = (char*)calloc(strlen("Could not move PC up-left")+1, sizeof(char));
				printf((g_state->message), "Could not move PC up-left");
			}
			break;
		case KEY_B2:		// keypad '5'
			CASE_MOVE_REST:
			
			move_matrix[0] = 0;
			move_matrix[1] = 0;
			if (!state_move_pc(&(g_state->dungeons[g_state->dungeon_index]), *wrapper, move_matrix)) {
				
				g_state->message = (char*)calloc(strlen("Could not let PC rest")+1, sizeof(char));
				sprintf((g_state->message), "Could not let PC rest");
			}
			break;
		case '<':			// go up staircase
			
			g_state->allow_move = 0;
			if (((g_state->dungeon_index - 1) > -1) && g_state->dungeons[g_state->dungeon_index].cells[g_state->pc.location.y][g_state->pc.location.x].type == CellType_Stair_up) { state_decrease_dungeon(g_state); }
			break;
		case '>':			// go down staircase
			
			g_state->allow_move = 0;
			if (((g_state->dungeon_index + 1) < g_state->num_dungeons) && g_state->dungeons[g_state->dungeon_index].cells[g_state->pc.location.y][g_state->pc.location.x].type == CellType_Stair_down) { state_increase_dungeon(g_state); }
			break;
		case 'm':			// enter npc menu
			
			g_state->allow_move = 0;
			g_state->menu_index = 1;
			return 2;
			break;
		case 'Q': 			// quit game
			
			g_state->allow_move = 0;
			return 0;
			break;
		case 'k':			// move up with 'k'
			
			goto CASE_MOVE_U;
			break;
		case '8':			// move up with '8'
			
			goto CASE_MOVE_U;
			break;
		case 'u':			// move up-right with 'u'
			
			goto CASE_MOVE_UR;
			break;
		case '9':			// move up-right with '9'
			
			goto CASE_MOVE_UR;
			break;
		case 'l':			// move right with 'l'
			
			goto CASE_MOVE_R;
			break;
		case '6':			// move right with '6'
			
			goto CASE_MOVE_R;
			break;
		case 'n':			// move down-right with 'n'
			
			goto CASE_MOVE_DR;
			break;
		case '3':			// move down-right with '3'
			
			goto CASE_MOVE_DR;
			break;
		case 'j':			// move down with 'j'
			
			goto CASE_MOVE_D;
			break;
		case '2':			// move down with '2'
			
			goto CASE_MOVE_D;
			break;
		case 'b':			// move down-left with 'b'
			
			goto CASE_MOVE_DL;
			break;
		case '1':			// move down-left with '1'
			
			goto CASE_MOVE_DL;
			break;
		case 'h':			// move left with 'h'
			
			goto CASE_MOVE_L;
			break;
		case '4':			// move left with '4'
			
			goto CASE_MOVE_L;
			break;
		case 'y':			// move up-left with 'y'
			
			goto CASE_MOVE_UL;
			break;
		case '7':			// move up-left with '7'
			
			goto CASE_MOVE_UL;
			break;
		case ' ':			// ' '
		
			goto CASE_MOVE_REST;
			break;
		case '.':			// '.'
		
			goto CASE_MOVE_REST;
			break;
		default: 
			
			g_state->allow_move = 0;
			g_state->message = (char*)calloc(strlen("Unknown key pressed: %o")+1, sizeof(char));
			sprintf((g_state->message), "Unknown key pressed: %o", key);
			break; 	//no-op entered, do nothing
		}
		
		return 1;
		break;
	}
}

int state_move_pc(Dungeon *dungeon, Character_Wrapper wrapper, int direction[2]) {
	
	Character_PC *pc = wrapper.pc;
	Coordinate next = move_pc(dungeon, pc, direction);
	
	if (cell_immutable_ntunneling(dungeon->cells[next.y][next.x])) { return 0; }
	
	dungeon_resolve_collision(dungeon, wrapper, next);
	return 1;
}

void state_move_npc(Dungeon *dungeon, Character_Wrapper wrapper, Character_PC pc) {
	
	Character_NPC *npc = wrapper.npc;
	Coordinate next = {
		
		.x = 0,
		.y = 0
	};
	
	switch(npc->type) {
	
	case 1:
	
		next = move_npc1(dungeon, npc, pc.location);
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
		
		next = move_npc5(dungeon, npc, pc.location);
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
		
		next = move_npc9(dungeon, npc, pc.location);
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
		
		next = move_npcD(dungeon, npc, pc.location);
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

void state_increase_dungeon(GameState *g_state) {
	
	int i = 0;
	
	uint8_t num_npcs_defined = g_state->dungeons[g_state->dungeon_index].num_npcs_defined;
	uint16_t num_npcs = g_state->dungeons[g_state->dungeon_index].num_npcs;
	uint16_t num_stairs_up = g_state->dungeons[g_state->dungeon_index].num_staircases_down;
	uint8_t stair_index = 0;
	
	for (i = 0; i < g_state->dungeons[g_state->dungeon_index].num_staircases_down; i++) {
		
		if (coordinate_is_same(g_state->pc.location, g_state->dungeons[g_state->dungeon_index].staircases_down[i]->location)) {
			
			stair_index = i;
			break;
		}
	}
	
	if (!num_npcs_defined) { num_npcs = 0; }
	
	state_deinit_queue(g_state);
	g_state->dungeon_index++;
	
	if (g_state->dungeon_index == g_state->num_dungeons_generated) {
		
		if (g_state->dungeon_index == ((g_state->num_dungeons)-1)) { g_state->dungeons[g_state->dungeon_index] = dungeon_init((uint8_t)DUNGEON_HEIGHT, (uint8_t)DUNGEON_WIDTH, num_npcs, num_stairs_up, 0, -1); }
		else { g_state->dungeons[g_state->dungeon_index] = dungeon_init((uint8_t)DUNGEON_HEIGHT, (uint8_t)DUNGEON_WIDTH, num_npcs, num_stairs_up, 0, 0); }
		
		(g_state->num_dungeons_generated)++;
	}
	g_state->pc.location.y = g_state->dungeons[g_state->dungeon_index].staircases_up[stair_index]->location.y;
	g_state->pc.location.x = g_state->dungeons[g_state->dungeon_index].staircases_up[stair_index]->location.x;
	
	state_init_queue(g_state);
	return;
}

void state_decrease_dungeon(GameState *g_state) {
	
	int i = 0;
	
	uint8_t stair_index = 0;
	
	for (i = 0; i < g_state->dungeons[g_state->dungeon_index].num_staircases_up; i++) {
		
		if (coordinate_is_same(g_state->pc.location, g_state->dungeons[g_state->dungeon_index].staircases_up[i]->location)) {
			
			stair_index = i;
			break;
		}
	}
	
	state_deinit_queue(g_state);
	g_state->dungeon_index--;
	
	g_state->pc.location.y = g_state->dungeons[g_state->dungeon_index].staircases_down[stair_index]->location.y;
	g_state->pc.location.x = g_state->dungeons[g_state->dungeon_index].staircases_down[stair_index]->location.x;
	
	state_init_queue(g_state);
	return;
}

int state_contains_npcs(GameState *g_state) {
	
	int i;
	
	if (g_state->num_dungeons_generated < g_state->num_dungeons) { return 1; }
	
	for (i = 0; i < g_state->num_dungeons_generated; i++) {
		
		if (dungeon_contains_npcs((&g_state->dungeons[i]))) { return 1; }
	}
	
	return 0;
}

void state_gameover(GameState *g_state, int print_fill, int print_color, int print_weight) {
	
	/* pc died */
	if (g_state->pc.hp < 1) { state_draw_gameover_lose(*g_state, print_fill, print_color, print_weight); }
	/* all npcs in lowest-level dungeon are dead */
	else if (g_state->num_dungeons_generated == g_state->num_dungeons && !dungeon_contains_npcs(&(g_state->dungeons[g_state->num_dungeons - 1]))) { state_draw_gameover_win(*g_state); }
	
	return;
}

void state_draw(GameState g_state, int print_fill, int print_color, int print_weight) {
	
	clear();
	
	start_color();
	
	switch (g_state.menu_index) {
		
	case 1: 			//draw monster menu	
		
		state_draw_menu_npc(g_state);
		break;
	default:			//draw current dungeon
		
		state_draw_dungeon(g_state, print_fill, print_color, print_weight);
		break;
	}
	
	refresh();
	
	return;
}

void state_draw_dungeon(GameState g_state, int print_fill, int print_color, int print_weight) {
	
	if (g_state.message) 	{ mvprintw(0, 0, "%s", g_state.message); }
	else 					{ mvprintw(0, 0, "griffens -- rlg327 implentation", g_state.turn); }
	
	dungeon_draw(g_state.dungeons[g_state.dungeon_index], 1, 0, print_fill, print_color, print_weight);
	
	mvprintw(22, 0, "[%d] NPCs left in dungeon level [%d]", (g_state.dungeons[g_state.dungeon_index].num_npcs - g_state.dungeons[g_state.dungeon_index].num_npcs_dead), g_state.dungeon_index);
	mvprintw(23, 0, "Turn [%d]   Level [%d] of [%d]", g_state.turn, (g_state.dungeon_index+1), g_state.num_dungeons);
	
	return;
}

void state_draw_menu_npc(GameState g_state) {
	
	int i = 0;
	int j = g_state.menu_offset;
	
	mvprintw(0, 0, "[%d] NPCs left in current dungeon level [%d]:", g_state.dungeons[g_state.dungeon_index].num_npcs - g_state.dungeons[g_state.dungeon_index].num_npcs_dead, (g_state.dungeon_index + 1));
	mvprintw(1, 0, "  id :: type :: speed :: distance from player");
	mvprintw(2, 0, "-------------------------------------");
	
	for (i = 3; i < 23 && j < g_state.dungeons[g_state.dungeon_index].num_npcs; i++) {
		
		uint32_t id = g_state.dungeons[g_state.dungeon_index].npcs[j].id;
		uint8_t type = g_state.dungeons[g_state.dungeon_index].npcs[j].type;
		uint8_t speed = g_state.dungeons[g_state.dungeon_index].npcs[j].speed;
		int y_pos = g_state.dungeons[g_state.dungeon_index].npcs[j].location.y - g_state.pc.location.y;
		int x_pos = g_state.dungeons[g_state.dungeon_index].npcs[j].location.x - g_state.pc.location.x;
		
		if (g_state.dungeons[g_state.dungeon_index].npcs[j].hp > 0) {
			
			if (y_pos > 0) {
				
				if (x_pos > 0) 	{ mvprintw(i, 0, "%4d :: %4x :: %5d :: %4d South, %4d East", id, type, speed, abs(y_pos), abs(x_pos)); }
				else 			{ mvprintw(i, 0, "%4d :: %4x :: %5d :: %4d South, %4d West", id, type, speed, abs(y_pos), abs(x_pos)); }
			}
			else {
				
				if (x_pos > 0) 	{ mvprintw(i, 0, "%4d :: %4x :: %5d :: %4d North, %4d East", id, type, speed, abs(y_pos), abs(x_pos)); }
				else 			{ mvprintw(i, 0, "%4d :: %4x :: %5d :: %4d North, %4d West", id, type, speed, abs(y_pos), abs(x_pos)); }
			}
		} else { i--; }
		
		j++;
	}
	
	mvprintw(23, 0, "KEY_UP to scroll up, KEY_Down to scroll down, ESC to exit menu");
	
	return;
}

void state_draw_gameover_win(GameState g_state) {
	
	clear();
	
	mvprintw(0, 0, "WINNER!", g_state.turn);
	
	mvprintw(12, 0, "...this page needs some work... :/");
	
	mvprintw(22, 0, "PC successfully eradicated [%d] monsters from the underworld on turn [%d]! :D", g_state.pc.num_kills, g_state.turn);
	mvprintw(23, 0, "Press ESC key to exit");
	
	refresh();
	
	while (getch() != 0x1b);
	
	return;
}

void state_draw_gameover_lose(GameState g_state, int print_fill, int print_color, int print_weight) {
	
	clear();
	
	mvprintw(0, 0, "GAME OVER", g_state.turn);
	
	dungeon_draw(g_state.dungeons[g_state.dungeon_index], 1, 0, print_fill, print_color, print_weight);
	
	mvprintw(22, 0, "PC was defeated on turn [%d] having slain [%d] monsters in vain ... :(", g_state.turn, g_state.pc.num_kills);
	mvprintw(23, 0, "Press ESC key to exit");
	
	refresh();
	
	while (getch() != 0x1b);
	
	return;
}

void state_draw_gameover_unexpected(GameState g_state) {
	
	clear();
	
	
	
	mvprintw(22, 0, "Game exited in unknown state on turn [%d] :|", g_state.turn);
	mvprintw(23, 0, "Press ESC key to exit");
	
	refresh();
	
	while (getch() != 0x1b);
	
	return;
}