#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <stdint.h>

#include "../dungeon.h"
#include "queue.h"

/******* enums declarations *******/



/******* struct declarations ******/



/****** function declarations *****/
void pathfinder_ntunneling(Dungeon *dungeon, Coordinate start);

void pathfinder_tunneling(Dungeon *dungeon, Coordinate start);

void pathfinder_mark_neighbors(Queue *queue, QueueNode from, uint32_t cost);


#endif /* QUEUE_H */