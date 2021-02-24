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
			}
		}
	}
	
//	printf("-- debug -- queue:\n");
//	queue_print(queue);
//	printf("-- end debug --\n");
	
	while (!queue_is_empty(queue)) {
		
		QueueNode node = queue_dequeue(&queue);
		dungeon->cells[node.cell_loc.y][node.cell_loc.x].weight_ntunneling = node.priority;
		
		pathfinder_mark_neighbors(&queue, node, CELL_TRAVERSAL_COST);
		
//		dungeon_print(*dungeon, 0, 0, 2);
		
//		if (queue.index % 100 == 0 || queue.index < 10) queue_print(queue);
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
	
	int visited = 0;
	
	while (visited < CELL_NUM_NEIGHBORS) {
		
		QueueNode *neighbor = queue_find_neighbor(*queue, from);
		
		if (neighbor != NULL && neighbor->priority > (from.priority + cost)) {
			
			neighbor->priority = from.priority + cost;
			queue_sort(queue);
		}
		
		visited++;
	}
}