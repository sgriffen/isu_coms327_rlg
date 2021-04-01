/******** include std libs ********/
#include <stdlib.h>
#include <stdio.h>

/******* include custom libs ******/
#include "./pathfinder.h"

/********** definitions **********/
#define QUEUE_MAX_SIZE 1700

/*********** global vars **********/


/****** function definitions ******/
void pathfinder_rewind(Dungeon *dungeon, Coordinate *start, Coordinate *end, int (*rewind_to_start)(Dungeon*, QueueNode*, Coordinate *start)) {
	
	int i = 0, j = 0;
	int end_found = 0;
	int num_traversed = 0;
	QueueNode *traversed = (QueueNode*)calloc((dungeon->width)*(dungeon->height), sizeof(QueueNode));
	Queue queue = queue_init(QUEUE_MAX_SIZE);
	
	for (i = 0; i < dungeon->height; i++) { //initialize all weights to infinity traversal distance (UINT32_HAX), except starting cell
		for (j = 0; j < dungeon->width; j++) {
			
//			dungeon->cells[i][j].weight[0] = CELL_HARDNESS_MAX;
			
			if (!cell_immutable_tunneling(dungeon->cells[i][j])) {
				
				if (start->is_same(dungeon->cells[i][j].location)) 	{ queue_enqueue(&queue, queue_node_init(&(dungeon->cells[i][j].location), CELL_HARDNESS_MIN)); }
				else 												{ queue_enqueue(&queue, queue_node_init(&(dungeon->cells[i][j].location), UINT16_MAX)); }
				
//				printf("-- debug -- adding to queue y:[%d] x:[%d]\n", dungeon->cells[i][j].location.y, dungeon->cells[i][j].location.x);
			}
		}
	}
	
//	printf("-- debug -- queue:\n");
//	queue_print(queue);
//	printf("-- end debug --\n");
	
//	printf("-- debug -- starting queue at y:[%d] x:[%d]\n", start->y, start->x);
	
//	printf("-- debug -- queue size:[%d]\n", queue.size);
//	printf("-- debug -- queue index:[%d]\n", queue.index);
//	printf("-- debug -- first in queue at index:[%d] - priority:[%d] y:[%d] x:[%d]\n", queue.index-1, queue_peek(&queue)->priority, queue_peek(&queue)->cell_loc->y, queue_peek(&queue)->cell_loc->x);
//	printf("-- debug -- ending search at y:[%d] x:[%d]\n", end->y, end->x);
	
	/* traverse queue (execute Dijkstra’s Algorithm) */
	
	while (!queue_is_empty(queue)) {
		
		QueueNode node = queue_dequeue(&queue);
		
//		printf("-- debug -- looking at y:[%d] x:[%d]\n", node.cell_loc->y, node.cell_loc->x);
		
		Coordinate *loc = (Coordinate*)(node.element);
		pathfinder_mark_neighbors(&queue, node, CELL_NUM_NEIGHBORS/4, CELL_TRAVERSAL_COST(dungeon->cells[loc->y][loc->x].hardness), queue_find_neighbors_card);
		traversed[num_traversed] = node;
		num_traversed++;
		
		if (end->is_same(*loc)) {
			
			end_found = 1;
			break;
		}
	}
	
	if (end_found) {
		
		printf("-- debug -- search end found, calling callback function. traversing number of cells:[%d]\n", num_traversed);
		
		for (i = 0; i < num_traversed/2; i++) { //flip traversed array
			
			QueueNode temp = traversed[i];
			traversed[i] = traversed[num_traversed-i];
			traversed[num_traversed-i] = temp;
		}
		printf("-- debug -- flipped rewind array\n");
		for (i = 0; i < num_traversed; i++) {
			
			Coordinate *loc = (Coordinate*)(traversed[i].element);
			printf("-- debug -- rewinding to cell y:[%d] x:[%d]\n", loc->y, loc->x);
		}
		rewind_to_start(dungeon, traversed, start);
	}
	else { printf("-- debug -- search end not found\n"); }
	
	free(queue.nodes);
	free(traversed);
	
	return;
}

void pathfinder_ntunneling(Dungeon *dungeon, Coordinate *start) {
	
	int i = 0, j = 0;
	Queue queue = queue_init(QUEUE_MAX_SIZE);
	
	for (i = 0; i < dungeon->height; i++) { //initialize all weights to infinity traversal distance (UINT32_HAX), except starting cell
		for (j = 0; j < dungeon->width; j++) {
			
//			dungeon->cells[i][j].weight[0] = CELL_HARDNESS_MAX;
			
			if (!cell_immutable_ntunneling(dungeon->cells[i][j])) {
				
				if (start->is_same(dungeon->cells[i][j].location)) 	{ queue_enqueue(&queue, queue_node_init(&(dungeon->cells[i][j].location), CELL_HARDNESS_MIN)); }
				else 												{ queue_enqueue(&queue, queue_node_init(&(dungeon->cells[i][j].location), UINT16_MAX)); }
				
//				printf("-- debug -- adding to queue y:[%d] x:[%d]\n", dungeon->cells[i][j].location.y, dungeon->cells[i][j].location.x);
			}
		}
	}
	
//	printf("-- debug -- queue:\n");
//	queue_print(queue);
//	printf("-- end debug --\n");
	
//	printf("-- debug -- starting queue at y:[%d] x:[%d]\n", start->y, start->x);
	
//	printf("-- debug -- queue size:[%d]\n", queue.size);
//	printf("-- debug -- queue index:[%d]\n", queue.index);
//	printf("-- debug -- first in queue at index:[%d] - priority:[%d] y:[%d] x:[%d]\n", queue.index-1, queue_peek(&queue)->priority, queue_peek(&queue)->cell_loc->y, queue_peek(&queue)->cell_loc->x);
	
	/* traverse queue (execute Dijkstra’s Algorithm) */
	while (!queue_is_empty(queue)) {
		
		QueueNode node = queue_dequeue(&queue);
//		printf("-- debug -- dequeuing index:[%d] y:[%d] x:[%d] priority:[%d]\n", queue.index, node.cell_loc->y, node.cell_loc->x, node.priority);
		
		Coordinate *loc = (Coordinate*)(node.element);
		dungeon->cells[loc->y][loc->x].weight_ntunneling = node.priority;
		
		pathfinder_mark_neighbors(&queue, node, CELL_NUM_NEIGHBORS, CELL_TRAVERSAL_COST(dungeon->cells[loc->y][loc->x].hardness), queue_find_neighbors_diag);
		
//		dungeon_print(*dungeon, 0, 0, 2);
//		if (queue.index > 200) { queue_print(queue); }
	}
	
	free(queue.nodes);
	
	return;
}

void pathfinder_tunneling(Dungeon *dungeon, Coordinate *start) {
	
	int i = 0, j = 0;
	Queue queue = queue_init(QUEUE_MAX_SIZE);
	
	for (i = 0; i < dungeon->height; i++) { //initialize all weights to infinity traversal distance (CELL_HARDNESS_MAX), except starting cell
		for (j = 0; j < dungeon->width; j++) {
			
//			dungeon->cells[i][j].weight[1] = CELL_HARDNESS_MAX;
			
			if (!cell_immutable_tunneling(dungeon->cells[i][j])) {
				
				if (start->is_same(dungeon->cells[i][j].location)) 	{ queue_enqueue(&queue, queue_node_init(&(dungeon->cells[i][j].location), CELL_HARDNESS_MIN)); }
				else 												{ queue_enqueue(&queue, queue_node_init(&(dungeon->cells[i][j].location), UINT16_MAX)); }
				
//				printf("-- debug -- adding to queue y:[%d] x:[%d]\n", dungeon->cells[i][j].location.y, dungeon->cells[i][j].location.x);
			}
		}
	}
	
//	printf("-- debug -- queue:\n");
//	queue_print(queue);
//	printf("-- end debug --\n");
	
//	printf("-- debug -- starting queue at y:[%d] x:[%d]\n", start->y, start->x);
	
//	printf("-- debug -- queue size:[%d]\n", queue.size);
//	printf("-- debug -- queue index:[%d]\n", queue.index);
//	printf("-- debug -- first in queue at index:[%d] - priority:[%d] y:[%d] x:[%d]\n", queue.index-1, queue_peek(&queue)->priority, queue_peek(&queue)->cell_loc->y, queue_peek(&queue)->cell_loc->x);
	
	/* traverse queue (execute Dijkstra’s Algorithm) */
	while (!queue_is_empty(queue)) {
		
		QueueNode node = queue_dequeue(&queue);
//		printf("-- debug -- dequeuing index:[%d] y:[%d] x:[%d] priority:[%d]\n", queue.index, node.cell_loc->y, node.cell_loc->x, node.priority);
		
		Coordinate *loc = (Coordinate*)(node.element);
		dungeon->cells[loc->y][loc->x].weight_tunneling = node.priority;
		
		pathfinder_mark_neighbors(&queue, node, CELL_NUM_NEIGHBORS, CELL_TRAVERSAL_COST(dungeon->cells[loc->y][loc->x].hardness), queue_find_neighbors_diag);
		
//		dungeon_print(*dungeon, 0, 0, 2);
//		if (queue.index > 200) { queue_print(queue); }
	}
	
	free(queue.nodes);
	
	return;
}

void pathfinder_mark_neighbors(Queue *queue, QueueNode from, int max_num_neighbors, uint32_t cost, int (*mark)(Queue*, QueueNode, int, QueueNode**)) {
	
	int i = 0;
	int num_neighbors = 0;
	int priority_changed = 0;
	QueueNode **neighbors = (QueueNode**)calloc(max_num_neighbors, sizeof(QueueNode*));
	
	num_neighbors = mark(queue, from, max_num_neighbors, neighbors);
	
//	printf("-- debug -- found neighbors:[%d]\n", num_neighbors);
	
	for (i = 0; i < num_neighbors; i++) {
		
//		printf("-- debug -- marking neighbor:[%d]\n", i);
		
		if (neighbors[i]->priority > (from.priority + cost)) {
			
			neighbors[i]->priority = from.priority + cost;
			neighbors[i]->from = &(from);
			priority_changed = 1;
		}
	}
	
	if (priority_changed) { queue_sort(queue); }
	
	free(neighbors);
	
	return;
}