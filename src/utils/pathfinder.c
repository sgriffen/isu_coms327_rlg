/******** include std libs ********/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/******* include custom libs ******/
#include "pathfinder.h"

/********** definitions **********/
#define QUEUE_MAX_SIZE 1700

/*********** global vars **********/


/****** function definitions ******/
void pathfinder_ntunneling(Dungeon *dungeon, Coordinate start) {
	
	int i = 0, j = 0;
	Queue queue = queue_init(QUEUE_MAX_SIZE);
	
	for (i = 0; i < dungeon->height; i++) { //initialize all weights to infinity traversal distance (UINT32_HAX), except starting cell
		for (j = 0; j < dungeon->width; j++) {
			
//			dungeon->cells[i][j].weight[0] = CELL_HARDNESS_MAX;
			
			if (dungeon->cells[i][j].hardness == CELL_HARDNESS_MIN) {
				
				if (coordinate_is_same(dungeon->cells[i][j].location, start)) 	{ queue_enqueue(&queue, queue_node_init(dungeon->cells[i][j].location, CELL_HARDNESS_MIN)); }
				else 															{ queue_enqueue(&queue, queue_node_init(dungeon->cells[i][j].location, INT32_MAX)); }
				
//				printf("-- debug -- adding to queue y:[%d] x:[%d]\n", dungeon->cells[i][j].location.y, dungeon->cells[i][j].location.x);
			}
		}
	}
	
//	printf("-- debug -- queue:\n");
//	queue_print(queue);
//	printf("-- end debug --\n");
	
	printf("-- debug -- starting queue at y:[%d] x:[%d]\n", start.y, start.x);
	
	printf("-- debug -- queue size:[%d]\n", queue.size);
	printf("-- debug -- queue index:[%d]\n", queue.index);
	printf("-- debug -- first in queue at index:[%d] - priority:[%d] y:[%d] x:[%d]\n", queue.index-1, queue_peek(&queue)->priority, queue_peek(&queue)->cell_loc.y, queue_peek(&queue)->cell_loc.x);
	
	while (!queue_is_empty(queue)) {
		
		QueueNode node = queue_dequeue(&queue);
		
		printf("-- debug -- dequeuing index:[%d] y:[%d] x:[%d]\n", queue.index, node.cell_loc.y, node.cell_loc.x);
		
		dungeon->cells[node.cell_loc.y][node.cell_loc.x].weight_ntunneling = node.priority;
		
		pathfinder_mark_neighbors(&queue, node, CELL_TRAVERSAL_COST);
		
//		dungeon_print(*dungeon, 0, 0, 2);
		
		if (queue.index > 200) { queue_print(queue); }
	}
	
	return;
}

void pathfinder_tunneling(Dungeon *dungeon, Coordinate start) {
	
	int i = 0, j = 0;
	Queue queue = queue_init(QUEUE_MAX_SIZE);
	
	for (i = 0; i < dungeon->height; i++) { //initialize all weights to infinity traversal distance (CELL_HARDNESS_MAX), except starting cell
		for (j = 0; j < dungeon->width; j++) {
			
//			dungeon->cells[i][j].weight[1] = CELL_HARDNESS_MAX;
			
			if (!cell_immutable(dungeon->cells[i][j])) {
				
				if (coordinate_is_same(dungeon->cells[i][j].location, start)) 	{ queue_enqueue(&queue, queue_node_init(dungeon->cells[i][j].location, CELL_HARDNESS_MIN)); }
				else 															{ queue_enqueue(&queue, queue_node_init(dungeon->cells[i][j].location, INT32_MAX)); }
				
//				printf("-- debug -- adding to queue y:[%d] x:[%d]\n", dungeon->cells[i][j].location.y, dungeon->cells[i][j].location.x);
			}
		}
	}
	
//	printf("-- debug -- queue:\n");
//	queue_print(queue);
//	printf("-- end debug --\n");
	
	while (!queue_is_empty(queue)) {
		
		QueueNode node = queue_dequeue(&queue);
		dungeon->cells[node.cell_loc.y][node.cell_loc.x].weight_tunneling = node.priority;
		
		pathfinder_mark_neighbors(&queue, node, CELL_HARDNESS_TRAVERSAL(dungeon->cells[node.cell_loc.y][node.cell_loc.x].hardness));
	}
	
	return;
}

void pathfinder_mark_neighbors(Queue *queue, QueueNode from, uint32_t cost) {
	
	int i = 0;
	int num_neighbors = 0;
	uint16_t *neighbor_indexes = calloc(CELL_NUM_NEIGHBORS, sizeof(uint16_t));
	
	num_neighbors = queue_find_neighbors(queue, from, CELL_NUM_NEIGHBORS, neighbor_indexes);
	
	printf("-- debug -- found neighbors:[%d]\n", num_neighbors);
	
	for (i = 0; i < num_neighbors; i++) {
		
		printf("-- debug -- marking neighbor:[%d]\n", i);
		
		if (queue->nodes[neighbor_indexes[i]].priority > (from.priority + cost)) {
			
			queue->nodes[neighbor_indexes[i]].priority = from.priority + cost;
			queue_sort(queue);
		}
	}
	
	free(neighbor_indexes);
	
	return;
}