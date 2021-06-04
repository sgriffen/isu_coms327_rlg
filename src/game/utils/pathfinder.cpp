/******** include std libs ********/
#include <stdlib.h>
#include <stdio.h>

/******* include custom libs ******/
#include "pathfinder.h"

/********** definitions **********/
#define QUEUE_MAX_SIZE 1700

/*********** global vars **********/


/****** function definitions ******/
void pathfinder_ntunneling(Dungeon *dungeon, Coordinate *start) {
	
	int i = 0, j = 0;
//	Queue queue = queue_init(QUEUE_MAX_SIZE);
	Queue queue = Queue();
	QueueNode* queue_nodes[DUNGEON_HEIGHT][DUNGEON_WIDTH];
	for (i = 0; i < dungeon->height; i++) {
		for (j = 0; j < dungeon->width; j++) { queue_nodes[i][j] = NULL; }
	}
	
	for (i = 0; i < dungeon->height; i++) { //initialize all weights to infinity traversal distance (UINT32_HAX), except starting cell
		for (j = 0; j < dungeon->width; j++) {
			
			if (!cell_immutable_ntunneling(dungeon->cells[i][j])) {
				
				if (dungeon->cells[i][j].location.is_same(*start)) 	{ queue_nodes[i][j] = queue.enqueue(CELL_HARDNESS_MIN, &(dungeon->cells[i][j])); }
				else 												{ queue_nodes[i][j] = queue.enqueue(INT32_MAX, &(dungeon->cells[i][j])); }
			}
		}
	}
	
	/* traverse queue (execute Dijkstra’s Algorithm) */
	while (!queue.is_empty()) {
		
		QueueNode node = queue.dequeue();
		
		Cell *cell = (Cell*)node.element;
		cell->weight_ntunneling = node.key;
		
		pathfinder_mark_neighbors(&queue, queue_nodes, node, CELL_TRAVERSAL_COST(cell->hardness), 0);
	}
	
	return;
}

void pathfinder_tunneling(Dungeon *dungeon, Coordinate *start) {
	
	int i = 0, j = 0;
	Queue queue = Queue();
	QueueNode* queue_nodes[DUNGEON_HEIGHT][DUNGEON_WIDTH];
	for (i = 0; i < dungeon->height; i++) {
		for (j = 0; j < dungeon->width; j++) { queue_nodes[i][j] = NULL; }
	}
	
	for (i = 0; i < dungeon->height; i++) { //initialize all weights to infinity traversal distance (CELL_HARDNESS_MAX), except starting cell
		for (j = 0; j < dungeon->width; j++) {
			
			if (!cell_immutable_tunneling(dungeon->cells[i][j])) {
				
				if (dungeon->cells[i][j].location.is_same(*start)) 	{ queue_nodes[i][j] = queue.enqueue(CELL_HARDNESS_MIN, &(dungeon->cells[i][j])); }
				else 												{ queue_nodes[i][j] = queue.enqueue(INT32_MAX, &(dungeon->cells[i][j])); }
			}
		}
	}
	
	/* traverse queue (execute Dijkstra’s Algorithm) */
	while (!queue.is_empty()) {
		
		QueueNode node = queue.dequeue();
		
		Cell *cell = (Cell*)node.element;
		cell->weight_tunneling = node.key;
		
		pathfinder_mark_neighbors(&queue, queue_nodes, node, CELL_TRAVERSAL_COST(cell->hardness), 1);
	}
	
	return;
}

void pathfinder_mark_neighbors(Queue *queue, QueueNode* queue_nodes[DUNGEON_HEIGHT][DUNGEON_WIDTH], QueueNode from, uint32_t cost, uint8_t tunneling) {

	pathfinder_mark_neighbors_node(queue, queue_nodes, from, cost, tunneling, -1, 0);
	pathfinder_mark_neighbors_node(queue, queue_nodes, from, cost, tunneling, -1, 1);
	pathfinder_mark_neighbors_node(queue, queue_nodes, from, cost, tunneling, 0, 1);
	pathfinder_mark_neighbors_node(queue, queue_nodes, from, cost, tunneling, 1, 1);
	pathfinder_mark_neighbors_node(queue, queue_nodes, from, cost, tunneling, 1, 0);
	pathfinder_mark_neighbors_node(queue, queue_nodes, from, cost, tunneling, 1, -1);
	pathfinder_mark_neighbors_node(queue, queue_nodes, from, cost, tunneling, 0, -1);
	pathfinder_mark_neighbors_node(queue, queue_nodes, from, cost, tunneling, -1, -1);
	
	return;
}
void pathfinder_mark_neighbors_node(Queue *queue, QueueNode* queue_nodes[DUNGEON_HEIGHT][DUNGEON_WIDTH], QueueNode from, uint32_t cost, uint8_t tunneling, int8_t offset_h, int8_t offset_w) {
	
	Cell *cell = (Cell*)from.element;
	Coordinate loc = cell->location;
	QueueNode *neighbor_node = queue_nodes[(loc.y)+offset_h][(loc.x)+offset_w];
	if (neighbor_node && !(neighbor_node->removed) && neighbor_node->key > (from.key + cost)) {

        queue->change_key(neighbor_node, (from.key + cost));
		Cell* neighbor_cell = (Cell*)neighbor_node->element;
		
		if (tunneling) 	{ neighbor_cell->weight_tunneling = (from.key + cost); }
		else 			{ neighbor_cell->weight_ntunneling = (from.key + cost); }
	}
	
	return;
}


void pathfinder_mark_neighbors(FibHeap *heap, FibNode* heap_nodes[DUNGEON_HEIGHT][DUNGEON_WIDTH], FibNode from, uint32_t cost, uint8_t tunneling) {

	pathfinder_mark_neighbors_node(heap, heap_nodes, from, cost, tunneling, -1, 0);
	pathfinder_mark_neighbors_node(heap, heap_nodes, from, cost, tunneling, -1, 1);
	pathfinder_mark_neighbors_node(heap, heap_nodes, from, cost, tunneling, 0, 1);
	pathfinder_mark_neighbors_node(heap, heap_nodes, from, cost, tunneling, 1, 1);
	pathfinder_mark_neighbors_node(heap, heap_nodes, from, cost, tunneling, 1, 0);
	pathfinder_mark_neighbors_node(heap, heap_nodes, from, cost, tunneling, 1, -1);
	pathfinder_mark_neighbors_node(heap, heap_nodes, from, cost, tunneling, 0, -1);
	pathfinder_mark_neighbors_node(heap, heap_nodes, from, cost, tunneling, -1, -1);
	
	return;
}
void pathfinder_mark_neighbors_node(FibHeap *heap, FibNode* heap_nodes[DUNGEON_HEIGHT][DUNGEON_WIDTH], FibNode from, uint32_t cost, uint8_t tunneling, int8_t offset_h, int8_t offset_w) {
	
	Coordinate loc = ((Cell*)from.element)->location;
	if (heap_nodes[(loc.y)+offset_h][(loc.x)+offset_w] && ((from.key + cost) < heap_nodes[(loc.y)+offset_h][(loc.x)+offset_w]->key)) {
		
		heap->decrease_node(heap_nodes[(loc.y)+offset_h][(loc.x)+offset_w], (from.key + cost));
		Cell* neighbor = (Cell*)heap_nodes[(loc.y)+offset_h][(loc.x)+offset_w]->element;
		
		if (tunneling) 	{ neighbor->weight_tunneling = (from.key + cost); }
		else 			{ neighbor->weight_ntunneling = (from.key + cost); }
	}
	
	return;
}