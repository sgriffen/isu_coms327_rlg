/******** include std libs ********/
#include <stdlib.h>
/******* include custom libs ******/
#include "./character_movement.h"
#include "../../utils/math_utils.h"

/********** definitions **********/
#define MAX_ROOM_ITERS 100

/*********** global vars **********/


/****** function definitions ******/
Coordinate move_away(Dungeon *dungeon, Coordinate start, Coordinate end, int avoid_npc, int (*immutable)(Cell)) {

	Coordinate next;
	next.y = 0;
	next.x = 0;

    if (start.y < end.y) 		{ next.y = start.y - 1; }
    else if (start.y > end.y) 	{ next.y = start.y + 1; }
    else                        { next.y = start.y; }

    if (start.x < end.x) 		{ next.x = start.x - 1; }
    else if (start.x > end.x) 	{ next.x = start.x + 1; }
    else                        { next.x = start.x; }

    if ((avoid_npc && dungeon->cells[next.y][next.x].contains_npc()) || immutable(dungeon->cells[next.y][next.x]) || start.is_same(next)) { next = move_rand(dungeon, start, avoid_npc, immutable); }

    return next;
}

Coordinate move_towards(Dungeon *dungeon, Coordinate start, Coordinate end, int avoid_npc, int (*immutable)(Cell)) {
	
	Coordinate next;
	next.y = 0;
	next.x = 0;
	
	if (start.y < end.y) 		{ next.y = start.y + 1; }
	else if (start.y > end.y) 	{ next.y = start.y - 1; }
    else                        { next.y = start.y; }
	
	if (start.x < end.x) 		{ next.x = start.x + 1; }
	else if (start.x > end.x) 	{ next.x = start.x - 1; }
    else                        { next.x = start.x; }

    if ((avoid_npc && dungeon->cells[next.y][next.x].contains_npc()) || immutable(dungeon->cells[next.y][next.x]) || start.is_same(next)) { next = move_rand(dungeon, start, avoid_npc, immutable); }

	return next;
}

Coordinate move_min_ntunneling(Dungeon *dungeon, Coordinate loc, int avoid_npc) {
	
	Coordinate next;
	next.y = 0;
	next.x = 0;
	
	uint32_t curr_weight = dungeon->cells[loc.y][loc.x].weight_ntunneling;
	
	if (dungeon->cells[(loc.y)-1][(loc.x)].weight_ntunneling < curr_weight) {
		
		next.y = loc.y - 1;
		next.x = loc.x;
	}
	else if (dungeon->cells[(loc.y)-1][(loc.x)+1].weight_ntunneling < curr_weight) {
		
		next.y = loc.y - 1;
		next.x = loc.x + 1;
	}
	else if (dungeon->cells[(loc.y)][(loc.x)+1].weight_ntunneling < curr_weight) {
		
		next.y = loc.y;
		next.x = loc.x + 1;
	}
	else if (dungeon->cells[(loc.y)+1][(loc.x)+1].weight_ntunneling < curr_weight) {
		
		next.y = loc.y + 1;
		next.x = loc.x + 1;
	}
	else if (dungeon->cells[(loc.y)+1][(loc.x)].weight_ntunneling < curr_weight) {
		
		next.y = loc.y + 1;
		next.x = loc.x;
	}
	else if (dungeon->cells[(loc.y)+1][(loc.x)-1].weight_ntunneling < curr_weight) {
		
		next.y = loc.y + 1;
		next.x = loc.x - 1;
	}
	else if (dungeon->cells[(loc.y)][(loc.x)-1].weight_ntunneling < curr_weight) {
		
		next.y = loc.y;
		next.x = loc.x - 1;
	}
	else {
		
		next.y = loc.y - 1;
		next.x = loc.x - 1;
	}

    if ((avoid_npc && dungeon->cells[next.y][next.x].contains_npc()) || cell_immutable_ntunneling(dungeon->cells[next.y][next.x])) { next = move_rand(dungeon, loc, avoid_npc, cell_immutable_ntunneling); }

	return next;
}

Coordinate move_min_tunneling(Dungeon *dungeon, Coordinate loc, int avoid_npc) {
	
	Coordinate next;
	next.y = 0;
	next.x = 0;
	
	uint32_t curr_weight = dungeon->cells[loc.y][loc.x].weight_tunneling;
	
	if (dungeon->cells[(loc.y)-1][(loc.x)].weight_tunneling < curr_weight) {
		
		next.y = loc.y - 1;
		next.x = loc.x;
	}
	else if (dungeon->cells[(loc.y)-1][(loc.x)+1].weight_tunneling < curr_weight) {
		
		next.y = loc.y - 1;
		next.x = loc.x + 1;
	}
	else if (dungeon->cells[(loc.y)][(loc.x)+1].weight_tunneling < curr_weight) {
		
		next.y = loc.y;
		next.x = loc.x + 1;
	}
	else if (dungeon->cells[(loc.y)+1][(loc.x)+1].weight_tunneling < curr_weight) {
		
		next.y = loc.y + 1;
		next.x = loc.x + 1;
	}
	else if (dungeon->cells[(loc.y)+1][(loc.x)].weight_tunneling < curr_weight) {
		
		next.y = loc.y + 1;
		next.x = loc.x;
	}
	else if (dungeon->cells[(loc.y)+1][(loc.x)-1].weight_tunneling < curr_weight) {
		
		next.y = loc.y + 1;
		next.x = loc.x - 1;
	}
	else if (dungeon->cells[(loc.y)][(loc.x)-1].weight_tunneling < curr_weight) {
		
		next.y = loc.y;
		next.x = loc.x - 1;
	}
	else {
		
		next.y = loc.y - 1;
		next.x = loc.x - 1;
	}

    if ((avoid_npc && dungeon->cells[next.y][next.x].contains_npc()) || cell_immutable_tunneling(dungeon->cells[next.y][next.x])) { next = move_rand(dungeon, loc, avoid_npc, cell_immutable_tunneling); }

    return next;
}

Coordinate move_rand(Dungeon *dungeon, Coordinate loc, int avoid_npc, int (*immutable)(Cell)) {
	
	int i = 0;
	
	Coordinate next;
	next.y = 0;
	next.x = 0;

    int move = utils_rand_between(0, 7, NULL);
	for (i = 0; i < 8; i++) {

	    if (move > 7) { move = 0; }
	    switch (move) {

        case 1:

            if (loc.y > DUNGEON_BORDER_WIDTH && loc.x < (DUNGEON_WIDTH-DUNGEON_BORDER_WIDTH)) {

                next.y = (loc.y)-1;
                next.x = (loc.x)+1;
            }
            break;
        case 2:

            if (loc.x < (DUNGEON_WIDTH-DUNGEON_BORDER_WIDTH)) {

                next.y = (loc.y);
                next.x = (loc.x)+1;
            }
            break;
        case 3:

            if (loc.y < (DUNGEON_HEIGHT-DUNGEON_BORDER_WIDTH) && loc.x < DUNGEON_WIDTH-DUNGEON_BORDER_WIDTH) {

                next.y = (loc.y)+1;
                next.x = (loc.x)+1;
            }
            break;
        case 4:

            if (loc.y < (DUNGEON_HEIGHT-DUNGEON_BORDER_WIDTH)) {

                next.y = (loc.y)+1;
                next.x = (loc.x);
            }
            break;
        case 5:

            if (loc.y < (DUNGEON_HEIGHT-DUNGEON_BORDER_WIDTH) && loc.x > DUNGEON_BORDER_WIDTH) {

                next.y = (loc.y)+1;
                next.x = (loc.x)-1;
            }
            break;
        case 6:

            if (loc.x > DUNGEON_BORDER_WIDTH) {

                next.y = (loc.y);
                next.x = (loc.x)-1;
            }
            break;
        case 7:

            if (loc.y > DUNGEON_BORDER_WIDTH && loc.x > DUNGEON_BORDER_WIDTH) {

                next.y = (loc.y)-1;
                next.x = (loc.x)-1;
            }
            break;
        default:

            if (loc.y > DUNGEON_BORDER_WIDTH) {

                next.y = (loc.y)-1;
                next.x = (loc.x);
            }
            break;
	    }

	    if ((avoid_npc && dungeon->cells[next.y][next.x].contains_npc()) || immutable(dungeon->cells[next.y][next.x])) { move++; }
	    else { break; }
	}

	if (immutable(dungeon->cells[next.y][next.x])) { return loc; }
	return next;
}

Coordinate move_pc(Dungeon *dungeon, PC *pc, int direction[2]) {
	
	Coordinate next;
	if (pc) {
		Coordinate next;
		next.y = pc->location.y + direction[0];
		next.x = pc->location.x + direction[1];
		
		if (dungeon_coordinate_inbounds(next) && !cell_immutable_ntunneling(dungeon->cells[next.y][next.x])) { return next; }
		if (direction[0] && direction[1]) {
			
			next.y = pc->location.y;
			next.x = pc->location.x + direction[1];
			if (dungeon_coordinate_inbounds(next) && !cell_immutable_ntunneling(dungeon->cells[next.y][next.x])) { return next; }
			next.y = pc->location.y + direction[0];
			next.x = pc->location.x;
		}
	}
	next.y = 0;
	next.x = 0;
	
	return next;
}

Coordinate move_npc0(Dungeon *dungeon, NPC *npc) {
	
	/* 
		npc is unintelligent, non-telepathic, non-tunneling, and predictable
		it randomly selectes a direction and moves in a straight line in that direction until it meets a cell that's immutable or a wall
		at which point it thinks hard for a turn and picks a random direction and continues in a straight line
	*/
	
	return move_away(dungeon, npc->location, npc->prev_location, 0, cell_immutable_ntunneling);
}

Coordinate move_npc1(Dungeon *dungeon, NPC *npc, Coordinate reference) {
	
	/*
		npc is intelligent, non-telepathic, non-tunneling, and predictable
		it wanders like npc_0 until it sees the pc, then takes the shortest, non-tunneling path to it
		if it cannot see the pc, and it has seen the pc already, it takes the shortest path to the pc's last known location
		after it reaches said location without seeing the player, it goes back to wandering
		it also intentionally avoids cells other npcs occupy as to not kill them
	*/

    /* get known coordinate of player if los established */
	Coordinate known = dungeon_los(*dungeon, npc->location, reference);
	if (!cell_immutable_tunneling(dungeon->cells[known.y][known.x])) {

		npc->pc_last_known = reference;
        return move_min_ntunneling(dungeon, npc->location, 1);
	}
    else if (npc->location.is_same(npc->pc_last_known)) {

        npc->pc_last_known.y = 0;
        npc->pc_last_known.x = 0;
    }

	if (!cell_immutable_tunneling(dungeon->cells[npc->pc_last_known.y][npc->pc_last_known.x]))  { return move_towards(dungeon, npc->location, npc->pc_last_known, 1, cell_immutable_ntunneling); }
	return move_away(dungeon, npc->location, npc->prev_location, 1, cell_immutable_ntunneling);
}

Coordinate move_npc2(Dungeon *dungeon, NPC *npc) {
	
	/* 
		npc is unintelligent, telepathic, non-tunneling, and predictable
		it moves towards the player, not necessarily taking the shortest path, excluding immutable and wall cells
	*/
	
	Coordinate next = move_min_ntunneling(dungeon, npc->location, 0);
	if (utils_rand_chance(45, NULL)) {
		
		if (utils_rand_chance(50, NULL) && !cell_immutable_ntunneling(dungeon->cells[npc->location.y][next.x])) { next.y = npc->location.y; }
		else if (!cell_immutable_ntunneling(dungeon->cells[next.y][npc->location.x]))							            { next.x = npc->location.x; }
	}
	
	return next;
}
Coordinate move_npc3(Dungeon *dungeon, NPC *npc) {
	
	/* 
		npc is intelligent, telepathic, non-tunneling, and predictable
		it moves towards the player constantly, always taking the shortest non-tunneling path
		it also intentionally avoids cells other npcs occupy as to not kill them
	*/
	
	return move_min_ntunneling(dungeon, npc->location, 1);
}

Coordinate move_npc4(Dungeon *dungeon, NPC *npc) {
	
	/* 
		npc is unintelligent, non-telepathic, tunneling, and predictable
		it moves like npc_0, but it can tunnel through walls
		if left alone long enough, this npc will eventually make every cell not immutable and not a room a corridor
	*/
	
	return move_away(dungeon, npc->location, npc->prev_location, 0, cell_immutable_tunneling);
}

Coordinate move_npc5(Dungeon *dungeon, NPC *npc, Coordinate reference) {
	
	/* 
		npc is intelligent, non-telepathic, tunneling, and predictable
		it wanders like npc_4 until it sees the pc, then takes the shortest path to it
		if it cannot see the pc, and it has already seen the pc, it takes the shortest path to the pc's last known location
		after it reaches said location without seeing the player, it goes back to wandering
		it also intentionally avoids cells other npcs occupy as to not kill them
	*/

    /* get known coordinate of player if los established */
    Coordinate known = dungeon_los(*dungeon, npc->location, reference);
    if (!cell_immutable_tunneling(dungeon->cells[known.y][known.x])) {

        npc->pc_last_known = reference;
        return move_min_tunneling(dungeon, npc->location, 1);
    }
    else if (npc->location.is_same(npc->pc_last_known)) {

        npc->pc_last_known.y = 0;
        npc->pc_last_known.x = 0;
    }

    if (!cell_immutable_tunneling(dungeon->cells[npc->pc_last_known.y][npc->pc_last_known.x]))  { return move_towards(dungeon, npc->location, npc->pc_last_known, 1, cell_immutable_tunneling); }
    return move_away(dungeon, npc->location, npc->prev_location, 1, cell_immutable_tunneling);
}

Coordinate move_npc6(Dungeon *dungeon, NPC *npc) {
	
	/* 
		npc is unintelligent, telepathic, tunneling, and predictable
		it moves towards the player, not necessarily taking the shortest path, excluding immutable cells
		it always tries to bring the delta y and delta x between it and the player to 0; if both already are 0, it acts like a CASE_0 npc
	*/
	
	Coordinate next = move_min_tunneling(dungeon, npc->location, 0);
    if (utils_rand_chance(45, NULL)) {

        if (utils_rand_chance(50, NULL) && !cell_immutable_tunneling(dungeon->cells[npc->location.y][next.x]))  { next.y = npc->location.y; }
        else if (!cell_immutable_tunneling(dungeon->cells[next.y][npc->location.x]))							            { next.x = npc->location.x; }
    }

	return next;
}

Coordinate move_npc7(Dungeon *dungeon, NPC *npc) {
	
	/* 
		npc is intelligent, telepathic, tunneling, and predictable
		it moves towards the player constantly, always taking the shortest path
		it also intentionally avoids cells other npcs occupy as to not kill them
	*/

    Coordinate next = move_min_tunneling(dungeon, npc->location, 1);
    return next;
}

Coordinate move_npc8(Dungeon *dungeon, NPC *npc) {
	
	/* 
		npc is unintelligent, non-telepathic, non-tunneling, and eradic
		it it has a 50% chance of being a type_0 npc, or randomly selecting a room/cooridor cell next to it to move to
	*/
	
	if (utils_rand_chance(50, NULL)) { return move_npc0(dungeon, npc); }
	return move_rand(dungeon, npc->location, 0, cell_immutable_ntunneling);
}

Coordinate move_npc9(Dungeon *dungeon, NPC *npc, Coordinate reference) {
	
	/* 
		npc is intelligent, non-telepathic, non-tunneling, and eradic
		it has a 50% chance of being a type_1 npc, or randomly moving to a room/cooridor cell next to it
	*/
	
	if (utils_rand_chance(50, NULL)) { return move_npc1(dungeon, npc, reference); }
	return move_rand(dungeon, npc->location, 1, cell_immutable_ntunneling);
}

Coordinate move_npcA(Dungeon *dungeon, NPC *npc) {
	
	/* 
		npc is unintelligent, telepathic, non-tunneling, and eradic
		it has a 50% chance of being a type_2 npc, or randomly moving to a room/cooridor cell next to it
	*/
	
	if (utils_rand_chance(50, NULL)) { return move_npc2(dungeon, npc); }
	return move_rand(dungeon, npc->location, 0, cell_immutable_ntunneling);
}

Coordinate move_npcB(Dungeon *dungeon, NPC *npc) {
	
	/* 
		npc is intelligent, telepathic, non-tunneling, and eradic
		it has a 50% chance of being a type_3 npc, or randomly moving to a room/cooridor cell next to it
	*/
	
	if (utils_rand_chance(50, NULL)) { return move_npc3(dungeon, npc); }
	return move_rand(dungeon, npc->location, 1, cell_immutable_ntunneling);
}

Coordinate move_npcC(Dungeon *dungeon, NPC *npc) {
	
	/* 
		npc is unintelligent, non-telepathic, tunneling, and eradic
		it has a 50% chance of being a type_4 npc, or randomly moving to any cell (besides immutable ones)
	*/
	
	if (utils_rand_chance(50, NULL)) { return move_npc4(dungeon, npc); }
	return move_rand(dungeon, npc->location, 0, cell_immutable_tunneling);
}

Coordinate move_npcD(Dungeon *dungeon, NPC *npc, Coordinate reference) {
	
	/* 
		npc is intelligent, non-telepathic, tunneling, and eradic
		it has a 50% chance of being a type_5 npc, or randomly moving to any cell (besides immutable ones)
	*/
	
	if (utils_rand_chance(50, NULL)) { return move_npc5(dungeon, npc, reference); }
	return move_rand(dungeon, npc->location, 1, cell_immutable_tunneling);
}

Coordinate move_npcE(Dungeon *dungeon, NPC *npc) {
	
	/* 
		npc is unintelligent, telepathic, tunneling, and eradic
		it has a 50% change of being a type_6 npc, or randomly moving to any cell (besides immutable ones)
	*/
	
	if (utils_rand_chance(50, NULL)) { return move_npc6(dungeon, npc); }
	return move_rand(dungeon, npc->location, 0, cell_immutable_tunneling);
}

Coordinate move_npcF(Dungeon *dungeon, NPC *npc) {
	
	/* 
		npc is intelligent, telepathic, tunneling, and eradic
		it has a 50% chance of being a type_7 npc, or randomly moving to any cell (besides immutable ones)
	*/
	
	if (utils_rand_chance(50, NULL)) { return move_npc7(dungeon, npc); }
	return move_rand(dungeon, npc->location, 1, cell_immutable_tunneling);
}