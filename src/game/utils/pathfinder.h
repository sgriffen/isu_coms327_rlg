#include <stdint.h>

#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "./classdef/queue.h"
#include "../classdef/dungeon.h"

/*********** definitions **********/


/************* macros *************/


/******* enums declarations *******/


/******* struct declarations ******/


/****** function declarations *****/
void pathfinder_rewind(Dungeon *dungeon, Coordinate *start, Coordinate *end, int (*rewind_to_start)(Dungeon*, QueueNode*, Coordinate *start));

void pathfinder_ntunneling(Dungeon *dungeon, Coordinate *start);

void pathfinder_tunneling(Dungeon *dungeon, Coordinate *start);

void pathfinder_mark_neighbors(Queue *queue, QueueNode from, int max_num_neighbors, uint32_t cost, int (*mark)(Queue*, QueueNode, int, QueueNode**));


#endif /* PATHFINDER_H */