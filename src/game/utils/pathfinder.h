#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <stdint.h>

#include "../classdef/dungeon.h"
#include "./classdef/queue.h"
#include "./classdef/fibheap.h"

/******* enums declarations *******/



/******* struct declarations ******/



/****** function declarations *****/
void pathfinder_ntunneling(Dungeon *dungeon, Coordinate *start);

void pathfinder_tunneling(Dungeon *dungeon, Coordinate *start);

void pathfinder_mark_neighbors(Queue *queue, QueueNode* queue_nodes[DUNGEON_HEIGHT][DUNGEON_WIDTH], QueueNode from, uint32_t cost, uint8_t tunneling);
void pathfinder_mark_neighbors_node(Queue *queue, QueueNode* queue_nodes[DUNGEON_HEIGHT][DUNGEON_WIDTH], QueueNode from, uint32_t cost, uint8_t tunneling, int8_t offset_h, int8_t offset_w);

void pathfinder_mark_neighbors(FibHeap *heap, FibNode* heap_nodes[DUNGEON_HEIGHT][DUNGEON_WIDTH], FibNode from, uint32_t cost, uint8_t tunneling);
void pathfinder_mark_neighbors_node(FibHeap *heap, FibNode* heap_nodes[DUNGEON_HEIGHT][DUNGEON_WIDTH], FibNode from, uint32_t cost, uint8_t tunneling, int8_t offset_h, int8_t offset_w);

#endif /* QUEUE_H */