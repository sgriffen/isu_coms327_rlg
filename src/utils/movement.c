/******** include std libs ********/
#include <stdlib.h>
#include <stdio.h>
/******* include custom libs ******/
#include "./movement.h"
#include "./math_utils.h"

/********** definitions **********/
#define MAX_ROOM_ITERS 100

/*********** global vars **********/


/****** function definitions ******/

Coordinate move_away_ntunneling(Dungeon *dungeon, Coordinate start, Coordinate end) {

    Coordinate next = {

            .y = end.y,
            .x = end.x
    };

    if (start.y < end.y && !cell_immutable_ntunneling(dungeon->cells[(start.y)+1][start.x])) 		{ next.y = end.y + 1; }
    else if (start.y > end.y && !cell_immutable_ntunneling(dungeon->cells[(start.y)-1][start.x])) 	{ next.y = end.y - 1; }
    else 																							{ next.y = end.y; }

    if (start.x < end.x && !cell_immutable_ntunneling(dungeon->cells[start.y][(start.x)+1])) 		{ next.x = end.x + 1; }
    else if (start.x > end.x && !cell_immutable_ntunneling(dungeon->cells[start.y][(start.x)-1]))	{ next.x = end.x - 1; }
    else 																							    { next.x = end.x; }

    return next;
}

Coordinate move_away_tunneling(Dungeon *dungeon, Coordinate start, Coordinate end) {

    Coordinate next = {

            .y = end.y,
            .x = end.x
    };

    if (start.y < end.y && !cell_immutable_tunneling(dungeon->cells[(start.y)+1][start.x])) 		{ next.y = end.y + 1; }
    else if (start.y > end.y && !cell_immutable_tunneling(dungeon->cells[(start.y)-1][start.x])) 	{ next.y = end.y - 1; }
    else 																							    { next.y = end.y; }

    if (start.x < end.x && !cell_immutable_tunneling(dungeon->cells[(start.y)+1][start.x])) 		{ next.x = end.x + 1; }
    else if (start.x > end.x && !cell_immutable_tunneling(dungeon->cells[(start.y)-1][start.x])) 	{ next.x = end.x - 1; }
    else 																							    { next.x = end.x; }

    return next;
}

Coordinate move_towards_ntunneling(Dungeon *dungeon, Coordinate start, Coordinate end) {
	
	Coordinate next = {
		
		.y = start.y,
		.x = start.x
	};
	
	if (start.y < end.y && !cell_immutable_ntunneling(dungeon->cells[(start.y)+1][start.x])) 		{ next.y = start.y + 1; }
	else if (start.y > end.y && !cell_immutable_ntunneling(dungeon->cells[(start.y)-1][start.x])) 	{ next.y = start.y - 1; }
	else 																							{ next.y = start.y; }
	
	if (start.x < end.x && !cell_immutable_ntunneling(dungeon->cells[start.y][(start.x)+1])) 		{ next.x = start.x + 1; }
	else if (start.x > end.x && !cell_immutable_ntunneling(dungeon->cells[start.y][(start.x)-1]))	{ next.x = start.x - 1; }
	else 																							{ next.x = start.x; }
	
	return next;
}

Coordinate move_towards_tunneling(Dungeon *dungeon, Coordinate start, Coordinate end) {
	
	Coordinate next = {
		
		.y = start.y,
		.x = start.x
	};
	
	if (!dungeon_coordinate_inbounds(start) || !dungeon_coordinate_inbounds(end)) {
		
		next.y = 0;
		next.x = 0;
		
		return next;
	}
	
	if (start.y < end.y && !cell_immutable_tunneling(dungeon->cells[(start.y)+1][start.x])) 		{ next.y = start.y + 1; }
	else if (start.y > end.y && !cell_immutable_tunneling(dungeon->cells[(start.y)-1][start.x])) 	{ next.y = start.y - 1; }
	else 																							{ next.y = start.y; }
	
	if (start.x < end.x && !cell_immutable_tunneling(dungeon->cells[(start.y)+1][start.x])) 		{ next.x = start.x + 1; }
	else if (start.x > end.x && !cell_immutable_tunneling(dungeon->cells[(start.y)-1][start.x])) 	{ next.x = start.x - 1; }
	else 																							{ next.x = start.x; }
		
	return next;
}

Coordinate move_min_ntunneling(Dungeon *dungeon, Coordinate location) {
	
	Coordinate next = {
		
		.y = location.y,
		.x = location.x
	};
	
	uint32_t curr_weight = dungeon->cells[location.y][location.x].weight_ntunneling;
	
	if (dungeon->cells[(location.y)-1][(location.x)].weight_ntunneling < curr_weight) {
		
		next.y = location.y - 1;
		next.x = location.x;
	}
	else if (dungeon->cells[(location.y)-1][(location.x)+1].weight_ntunneling < curr_weight) {
		
		next.y = location.y - 1;
		next.x = location.x + 1;
	}
	else if (dungeon->cells[(location.y)][(location.x)+1].weight_ntunneling < curr_weight) {
		
		next.y = location.y;
		next.x = location.x + 1;
	}
	else if (dungeon->cells[(location.y)+1][(location.x)+1].weight_ntunneling < curr_weight) {
		
		next.y = location.y + 1;
		next.x = location.x + 1;
	}
	else if (dungeon->cells[(location.y)+1][(location.x)].weight_ntunneling < curr_weight) {
		
		next.y = location.y + 1;
		next.x = location.x;
	}
	else if (dungeon->cells[(location.y)+1][(location.x)-1].weight_ntunneling < curr_weight) {
		
		next.y = location.y + 1;
		next.x = location.x - 1;
	}
	else if (dungeon->cells[(location.y)][(location.x)-1].weight_ntunneling < curr_weight) {
		
		next.y = location.y;
		next.x = location.x - 1;
	}
	else {
		
		next.y = location.y - 1;
		next.x = location.x - 1;
	}
	
	return next;
}

Coordinate move_min_tunneling(Dungeon *dungeon, Coordinate location) {
	
	Coordinate next = {
		
		.y = location.y,
		.x = location.x
	};
	
	uint32_t curr_weight = dungeon->cells[location.y][location.x].weight_tunneling;
	
	if (dungeon->cells[(location.y)-1][(location.x)].weight_tunneling < curr_weight) {
		
		next.y = location.y - 1;
		next.x = location.x;
	}
	else if (dungeon->cells[(location.y)-1][(location.x)+1].weight_tunneling < curr_weight) {
		
		next.y = location.y - 1;
		next.x = location.x + 1;
	}
	else if (dungeon->cells[(location.y)][(location.x)+1].weight_tunneling < curr_weight) {
		
		next.y = location.y;
		next.x = location.x + 1;
	}
	else if (dungeon->cells[(location.y)+1][(location.x)+1].weight_tunneling < curr_weight) {
		
		next.y = location.y + 1;
		next.x = location.x + 1;
	}
	else if (dungeon->cells[(location.y)+1][(location.x)].weight_tunneling < curr_weight) {
		
		next.y = location.y + 1;
		next.x = location.x;
	}
	else if (dungeon->cells[(location.y)+1][(location.x)-1].weight_tunneling < curr_weight) {
		
		next.y = location.y + 1;
		next.x = location.x - 1;
	}
	else if (dungeon->cells[(location.y)][(location.x)-1].weight_tunneling < curr_weight) {
		
		next.y = location.y;
		next.x = location.x - 1;
	}
	else {
		
		next.y = location.y - 1;
		next.x = location.x - 1;
	}
	
	return next;
}

Coordinate move_rand_ntunneling(Dungeon *dungeon, Coordinate location) {
	
	int i = 0;
	
	Coordinate next = {
		
		.y = location.y,
		.x = location.x
	};
	Coordinate prev_next = next;
	
	do {
		
		int move = utils_rand_between(1, 8, NULL);
		
		if (move == 1 && location.y > DUNGEON_BORDER_WIDTH) {
				
			next.y = (location.y)-1;
			next.x = (location.x);
		}
		else if (move == 2 && location.y > DUNGEON_BORDER_WIDTH && location.x < (DUNGEON_WIDTH-DUNGEON_BORDER_WIDTH)) {
			
			next.y = (location.y)-1;
			next.x = (location.x)+1;
		}
		else if (move == 3 && location.x < (DUNGEON_WIDTH-DUNGEON_BORDER_WIDTH)) {
			
			next.y = (location.y);
			next.x = (location.x)+1;
		}
		else if (move == 4 && location.y < (DUNGEON_HEIGHT-DUNGEON_BORDER_WIDTH) && location.x < DUNGEON_WIDTH-DUNGEON_BORDER_WIDTH) {
			
			next.y = (location.y)+1;
			next.x = (location.x)+1;
		}
		else if (move == 5 && location.y < (DUNGEON_HEIGHT-DUNGEON_BORDER_WIDTH)) {
				
			next.y = (location.y)+1;
			next.x = (location.x);
		}
		else if (move == 6 && location.y < (DUNGEON_HEIGHT-DUNGEON_BORDER_WIDTH) && location.x > DUNGEON_BORDER_WIDTH) {
			
			next.y = (location.y)+1;
			next.x = (location.x)-1;
		}
		else if (move == 7 && location.x > DUNGEON_BORDER_WIDTH) {
			
			next.y = (location.y);
			next.x = (location.x)-1;
		}
		else if (location.y > DUNGEON_BORDER_WIDTH && location.x > DUNGEON_BORDER_WIDTH) {
			
			next.y = (location.y)-1;
			next.x = (location.x)-1;
		}
		i++;
	} while (cell_immutable_ntunneling(dungeon->cells[next.y][next.x]) && i < DUNGEON_MIN_CHANCE_COUNT);
	if (i >= DUNGEON_MIN_CHANCE_COUNT) { next = prev_next; }
	i = 0;
	
	return next;
}

Coordinate move_rand_tunneling(Dungeon *dungeon, Coordinate location) {
	
	int i = 0;
	
	Coordinate next = {
		
		.y = location.y,
		.x = location.x
	};
	Coordinate prev_next = next;
	
	do {
		
		int move = utils_rand_between(1, 8, NULL);
		
		if (move == 1 && location.y > DUNGEON_BORDER_WIDTH) {
				
			next.y = (location.y)-1;
			next.x = (location.x);
		}
		else if (move == 2 && location.y > DUNGEON_BORDER_WIDTH && location.x < (DUNGEON_WIDTH-DUNGEON_BORDER_WIDTH)) {
			
			next.y = (location.y)-1;
			next.x = (location.x)+1;
		}
		else if (move == 3 && location.x < (DUNGEON_WIDTH-DUNGEON_BORDER_WIDTH)) {
			
			next.y = (location.y);
			next.x = (location.x)+1;
		}
		else if (move == 4 && location.y < (DUNGEON_HEIGHT-DUNGEON_BORDER_WIDTH) && location.x < DUNGEON_WIDTH-DUNGEON_BORDER_WIDTH) {
			
			next.y = (location.y)+1;
			next.x = (location.x)+1;
		}
		else if (move == 5 && location.y < (DUNGEON_HEIGHT-DUNGEON_BORDER_WIDTH)) {
				
			next.y = (location.y)+1;
			next.x = (location.x);
		}
		else if (move == 6 && location.y < (DUNGEON_HEIGHT-DUNGEON_BORDER_WIDTH) && location.x > DUNGEON_BORDER_WIDTH) {
			
			next.y = (location.y)+1;
			next.x = (location.x)-1;
		}
		else if (move == 7 && location.x > DUNGEON_BORDER_WIDTH) {
			
			next.y = (location.y);
			next.x = (location.x)-1;
		}
		else if (location.y > DUNGEON_BORDER_WIDTH && location.x > DUNGEON_BORDER_WIDTH) {
			
			next.y = (location.y)-1;
			next.x = (location.x)-1;
		}
		i++;
	} while (cell_immutable_tunneling(dungeon->cells[next.y][next.x]) && i < DUNGEON_MIN_CHANCE_COUNT);
	if (i >= DUNGEON_MIN_CHANCE_COUNT) { next = prev_next; }
	i = 0;
	
	return next;
}

Coordinate move_pc(Dungeon *dungeon, Character_PC *pc) {
	
	Coordinate next = {
		
		.y = pc->location.y,
		.x = pc->location.x
	};
	
	if (pc->location.y > DUNGEON_BORDER_WIDTH) { next.y = (pc->location.y)-1; }
	if (pc->location.x > DUNGEON_BORDER_WIDTH) { next.x = (pc->location.x)-1; }
	
	return next;
}

Coordinate move_npc0(Dungeon *dungeon, Character_NPC *npc) {
	
	/* 
		npc is unintelligent, non-telepathic, non-tunneling, and predictable
		it randomly selectes a direction and moves in a straight line in that direction until it meets a cell that's immutable or a wall
		at which point it thinks hard for a turn and picks a random direction and continues in a straight line
	*/
	
	Coordinate next = {
		
		.y = 0,
		.x = 0
	};
	
	next = move_away_ntunneling(dungeon, npc->prev_location, npc->location);
	
//	printf("-- debug -- npc_0 wants to move to y:[%d] x:[%d]\n", next.y, next.x);
	
	if (coordinate_is_same(next, npc->prev_location) || cell_immutable_ntunneling(dungeon->cells[next.y][next.x])) { next = move_rand_ntunneling(dungeon, npc->location); }
	
//	printf("-- debug -- npc_0 moving to y:[%d] x:[%d]\n", next.y, next.x);

	return next;
}

Coordinate move_npc1(Dungeon *dungeon, Character_NPC *npc) {
	
	/*
		npc is intelligent, non-telepathic, non-tunneling, and predictable
		it wanders like npc_0 until it sees the pc, then takes the shortest, non-tunneling path to it
		if it cannot see the pc, and it has seen the pc already, it takes the shortest path to the pc's last known location
		after it reaches said location without seeing the player, it goes back to wandering
		it also intentionally avoids cells other npcs occupy as to not kill them
	*/
	
	int i = 0;
	
	Coordinate next = {
		
		.y = 0,
		.x = 0
	};
	
//	printf("-- debug -- npc_1 last known player location y:[%d] x:[%d]\n", npc->last_known.y, npc->last_known.x);
	
	Coordinate known = dungeon_los(*dungeon, npc->location, dungeon->pc.location);
	if (known.y && known.x) {
		
		known = dungeon->pc.location;
		npc->last_known = dungeon->pc.location;
	}
//	printf("-- debug -- npc_1 known player location y:[%d] x:[%d]\n", known.y, known.x);
	
	if (cell_immutable_ntunneling(dungeon->cells[npc->last_known.y][npc->last_known.x]) && cell_immutable_ntunneling(dungeon->cells[known.y][known.x])) {
		
		next = move_npc0(dungeon, npc);
		do {
			
			next = move_rand_ntunneling(dungeon, npc->location);
			i++;
		} while (dungeon_cell_contains_npc(dungeon, next) && i < DUNGEON_MIN_CHANCE_COUNT);
		if (i >= DUNGEON_MIN_CHANCE_COUNT) { next = npc->location; }
		i = 0;
	}
	
//	printf("-- debug -- npc_1 wants to move to location y:[%d] x:[%d]\n", next.y, next.x);

	if (!cell_immutable_ntunneling(dungeon->cells[known.y][known.x])) { next = move_min_ntunneling(dungeon, known); }
	else if (!cell_immutable_ntunneling(dungeon->cells[npc->last_known.y][npc->last_known.x])) { next = move_towards_ntunneling(dungeon, npc->location, npc->last_known); }
	
	if (!(npc->last_known.y - npc->location.y) && !(npc->last_known.x - npc->location.x)) {
		
		do {
			
			next = move_rand_ntunneling(dungeon, npc->location);
			i++;
		} while (dungeon_cell_contains_npc(dungeon, next) && i < DUNGEON_MIN_CHANCE_COUNT);
		if (i >= DUNGEON_MIN_CHANCE_COUNT) { next = npc->location; }
		i = 0;
	}
	
//	printf("-- debug -- npc_1 moving to location y:[%d] x:[%d]\n", next.y, next.x);
	
	if (cell_immutable_ntunneling(dungeon->cells[next.y][next.x])) { next = npc->location; }
	return next;
}

Coordinate move_npc2(Dungeon *dungeon, Character_NPC *npc) {
	
	/* 
		npc is unintelligent, telepathic, non-tunneling, and predictable
		it moves towards the player, not necessarily taking the shortest path, excluding immutable and wall cells
	*/
	
	Coordinate next = {
		
		.y = 0,
		.x = 0
	};
	
	next = move_min_ntunneling(dungeon, npc->location);
	
//	printf("-- debug -- npc_2 wants to move to y:[%d] x:[%d]\n", next.y, next.x);
	
	if (utils_rand_chance(45, NULL)) {
		
		if (utils_rand_chance(50, NULL) && !cell_immutable_ntunneling(dungeon->cells[npc->location.y][next.x])) { next.y = npc->location.y; }
		else if (!cell_immutable_ntunneling(dungeon->cells[next.y][npc->location.x]))							            { next.x = npc->location.x; }
	}
	
//	printf("-- debug -- npc_2 moving to y:[%d] x:[%d]\n", next.y, next.x);
	
	if (cell_immutable_ntunneling(dungeon->cells[next.y][next.x])) { next = npc->location; }
	return next;
}
Coordinate move_npc3(Dungeon *dungeon, Character_NPC *npc) {
	
	/* 
		npc is intelligent, telepathic, non-tunneling, and predictable
		it moves towards the player constantly, always taking the shortest non-tunneling path
		it also intentionally avoids cells other npcs occupy as to not kill them
	*/
	
	int i = 0;
	
	Coordinate next = {
		
		.y = 0,
		.x = 0
	};
	
	next = move_min_ntunneling(dungeon, npc->location);
	
//	printf("-- debug -- npc_3 wants to move to y:[%d] x:[%d]\n", next.y, next.x);
	
	if (dungeon_cell_contains_npc(dungeon, next)) {
		
		do {
			
			next = move_rand_ntunneling(dungeon, npc->location);
			i++;
		} while (dungeon_cell_contains_npc(dungeon, next) && i < DUNGEON_MIN_CHANCE_COUNT);
		if (i >= DUNGEON_MIN_CHANCE_COUNT) { next = npc->location; }
		i = 0;
	}
	
//	printf("-- debug -- npc_3 moving to y:[%d] x:[%d]\n", next.y, next.x);
	
	if (cell_immutable_ntunneling(dungeon->cells[next.y][next.x])) { next = npc->location; }
	return next;
}

Coordinate move_npc4(Dungeon *dungeon, Character_NPC *npc) {
	
	/* 
		npc is unintelligent, non-telepathic, tunneling, and predictable
		it moves like npc_0, but it can tunnel through walls
		if left alone long enough, this npc will eventually make every cell not immutable and not a room a cooridor
	*/
	
	Coordinate next = {
		
		.y = 0,
		.x = 0
	};
	
	next = move_away_tunneling(dungeon, npc->prev_location, npc->location);
	
//	printf("-- debug -- npc_0 wants to move to y:[%d] x:[%d]\n", next.y, next.x);
	
	if (coordinate_is_same(npc->location, npc->prev_location)) { next = move_rand_tunneling(dungeon, npc->location); }
	
//	printf("-- debug -- npc_0 moving to y:[%d] x:[%d]\n", next.y, next.x);
	
	if (cell_immutable_tunneling(dungeon->cells[next.y][next.x])) { next = npc->location; }
	return next;
}

Coordinate move_npc5(Dungeon *dungeon, Character_NPC *npc) {
	
	/* 
		npc is intelligent, non-telepathic, tunneling, and predictable
		it wanders like npc_3 until it sees the pc, then takes the shortest path to it
		if it cannot see the pc, and it has already seen the pc, it takes the shortest path to the pc's last known location
		after it reaches said location without seeing the player, it goes back to wandering
		it also intentionally avoids cells other npcs occupy as to not kill them
	*/
	
	int i = 0;
	
		Coordinate next = {
		
		.y = 0,
		.x = 0
	};
	
//	printf("-- debug -- npc_5 last known player location y:[%d] x:[%d]\n", npc->last_known.y, npc->last_known.x);
	
	Coordinate known = dungeon_los(*dungeon, npc->location, dungeon->pc.location);
	if (known.y && known.x) {
		
		known = dungeon->pc.location;
		npc->last_known = dungeon->pc.location;
	}
//	printf("-- debug -- npc_5 known player location y:[%d] x:[%d]\n", known.y, known.x);
	
	if (cell_immutable_tunneling(dungeon->cells[npc->last_known.y][npc->last_known.x]) && cell_immutable_tunneling(dungeon->cells[known.y][known.x])) {
		
		next = move_npc3(dungeon, npc);
		do {
			
			next = move_rand_ntunneling(dungeon, npc->location);
			i++;
		} while (dungeon_cell_contains_npc(dungeon, next) && i < DUNGEON_MIN_CHANCE_COUNT);
		if (i >= DUNGEON_MIN_CHANCE_COUNT) { next = npc->location; }
		i = 0;
	}
	
//	printf("-- debug -- npc_5 wants to move to location y:[%d] x:[%d]\n", next.y, next.x);

	if (!cell_immutable_tunneling(dungeon->cells[known.y][known.x])) { next = move_min_tunneling(dungeon, known); }
	else if (!cell_immutable_tunneling(dungeon->cells[npc->last_known.y][npc->last_known.x])) { next = move_towards_tunneling(dungeon, npc->location, npc->last_known); }
	
	if (!(npc->last_known.y - npc->location.y) && !(npc->last_known.x - npc->location.x)) {
		
		do {
			
			next = move_rand_tunneling(dungeon, npc->location);
			i++;
		} while (dungeon_cell_contains_npc(dungeon, next) && i < DUNGEON_MIN_CHANCE_COUNT);
		if (i >= DUNGEON_MIN_CHANCE_COUNT) { next = npc->location; }
		i = 0;
	}
	
//	printf("-- debug -- npc_5 moving to location y:[%d] x:[%d]\n", next.y, next.x);
	
	if (cell_immutable_tunneling(dungeon->cells[next.y][next.x])) { next = npc->location; }
	return next;
}

Coordinate move_npc6(Dungeon *dungeon, Character_NPC *npc) {
	
	/* 
		npc is unintelligent, telepathic, tunneling, and predictable
		it moves towards the player, not necessarily taking the shortest path, excluding immutable cells
		it always tries to bring the delta y and delta x between it and the player to 0; if both already are 0, it acts like a CASE_0 npc
	*/
	
	Coordinate next = {
		
		.y = 0,
		.x = 0
	};
	
	next = move_min_tunneling(dungeon, npc->location);
	
//	printf("-- debug -- npc_6 wants to move to y:[%d] x:[%d]\n", next.y, next.x);
	
	if (utils_rand_chance(45, NULL)) {
		
		if (utils_rand_chance(50, NULL) && !cell_immutable_tunneling(dungeon->cells[npc->location.y][next.x])) 	{ next.y = npc->location.y; }
		else if (!cell_immutable_tunneling(dungeon->cells[next.y][npc->location.x]))							            { next.x = npc->location.x; }
	}
	
//	printf("-- debug -- npc_6 moving to y:[%d] x:[%d]\n", next.y, next.x);
	
	if (cell_immutable_tunneling(dungeon->cells[next.y][next.x])) { next = npc->location; }
	return next;
}

Coordinate move_npc7(Dungeon *dungeon, Character_NPC *npc) {
	
	/* 
		npc is intelligent, telepathic, tunneling, and predictable
		it moves towards the player constantly, always taking the shortest path
		it also intentionally avoids cells other npcs occupy as to not kill them
	*/
	
	int i = 0;
	
	Coordinate next = {
		
		.y = 0,
		.x = 0
	};
	
	next = move_min_tunneling(dungeon, npc->location);
	
//	printf("-- debug -- npc_7 wants to move to y:[%d] x:[%d]\n", next.y, next.x);
	
	if (dungeon_cell_contains_npc(dungeon, next)) {
		
		do {
			
			next = move_rand_tunneling(dungeon, npc->location);
			i++;
		} while (dungeon_cell_contains_npc(dungeon, next) && i < DUNGEON_MIN_CHANCE_COUNT);
	}
	if (i >= DUNGEON_MIN_CHANCE_COUNT) { next = npc->location; }
	i = 0;
	
//	printf("-- debug -- npc_7 moving to y:[%d] x:[%d]\n", next.y, next.x);
	
	if (cell_immutable_tunneling(dungeon->cells[next.y][next.x])) { next = npc->location; }
	return next;
}

Coordinate move_npc8(Dungeon *dungeon, Character_NPC *npc) {
	
	/* 
		npc is unintelligent, non-telepathic, non-tunneling, and eradic
		it it has a 50% chance of being a type_0 npc, or randomly selecting a room/cooridor cell next to it to move to
	*/
	
	if (utils_rand_chance(50, NULL)) { return move_npc0(dungeon, npc); }
	return move_rand_ntunneling(dungeon, npc->location);
}

Coordinate move_npc9(Dungeon *dungeon, Character_NPC *npc) {
	
	/* 
		npc is intelligent, non-telepathic, non-tunneling, and eradic
		it has a 50% chance of being a type_1 npc, or randomly moving to a room/cooridor cell next to it
	*/
	
//	if (utils_rand_chance(50, NULL)) { return move_npc1(dungeon, npc); }
	return move_rand_ntunneling(dungeon, npc->location);
}

Coordinate move_npcA(Dungeon *dungeon, Character_NPC *npc) {
	
	/* 
		npc is unintelligent, telepathic, non-tunneling, and eradic
		it has a 50% chance of being a type_2 npc, or randomly moving to a room/cooridor cell next to it
	*/
	
	if (utils_rand_chance(50, NULL)) { return move_npc2(dungeon, npc); }
	return move_rand_ntunneling(dungeon, npc->location);
}

Coordinate move_npcB(Dungeon *dungeon, Character_NPC *npc) {
	
	/* 
		npc is intelligent, telepathic, non-tunneling, and eradic
		it has a 50% chance of being a type_3 npc, or randomly moving to a room/cooridor cell next to it
	*/
	
	if (utils_rand_chance(50, NULL)) { return move_npc3(dungeon, npc); }
	return move_rand_ntunneling(dungeon, npc->location);
}

Coordinate move_npcC(Dungeon *dungeon, Character_NPC *npc) {
	
	/* 
		npc is unintelligent, non-telepathic, tunneling, and eradic
		it has a 50% chance of being a type_4 npc, or randomly moving to any cell (besides immutable ones)
	*/
	
	if (utils_rand_chance(50, NULL)) { return move_npc4(dungeon, npc); }
	return move_rand_tunneling(dungeon, npc->location);
}

Coordinate move_npcD(Dungeon *dungeon, Character_NPC *npc) {
	
	/* 
		npc is intelligent, non-telepathic, tunneling, and eradic
		it has a 50% chance of being a type_5 npc, or randomly moving to any cell (besides immutable ones)
	*/
	
	if (utils_rand_chance(50, NULL)) { return move_npc5(dungeon, npc); }
	return move_rand_tunneling(dungeon, npc->location);
}

Coordinate move_npcE(Dungeon *dungeon, Character_NPC *npc) {
	
	/* 
		npc is unintelligent, telepathic, tunneling, and eradic
		it has a 50% change of being a type_6 npc, or randomly moving to any cell (besides immutable ones)
	*/
	
	if (utils_rand_chance(50, NULL)) { return move_npc6(dungeon, npc); }
	return move_rand_tunneling(dungeon, npc->location);
}

Coordinate move_npcF(Dungeon *dungeon, Character_NPC *npc) {
	
	/* 
		npc is intelligent, telepathic, tunneling, and eradic
		it has a 50% chance of being a type_7 npc, or randomly moving to any cell (besides immutable ones)
	*/
	
	if (utils_rand_chance(50, NULL)) { return move_npc7(dungeon, npc); }
	return move_rand_tunneling(dungeon, npc->location);
}