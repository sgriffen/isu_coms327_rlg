/******** include std libs ********/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/******* include custom libs ******/
#include "queue.h"

/********** definitions **********/



/*********** global vars **********/


/****** function definitions ******/
Queue queue_init(uint16_t length) {
	
	Queue queue = {
		
		.size = length,
		.index = 0,
		.nodes = (QueueNode*)calloc(length, sizeof(QueueNode))
	};
	
	return queue;
}

QueueNode queue_node_init(Coordinate loc, uint32_t priority) {
	
	QueueNode node = {
		
		.priority = priority,
		.cell_loc = loc
	};
	
	return node;
}

int queue_is_empty(Queue queue) { return !queue.index; }

int queue_is_full(Queue queue) { return queue.index == queue.size; }

QueueNode* queue_peek(Queue *queue) { return &(queue->nodes[(queue->index)-1]); }

QueueNode* queue_enqueue(Queue *queue, QueueNode node) {
	
	int i = 0;
	
	if (queue_is_full(*queue)) { return NULL; }
	
	if (queue_is_empty(*queue)) { queue->nodes[queue->index] = node; }
	else {
		
		for (i = queue->index-1; i >= 0; i--) { //start at back of queue
			
			if (node.priority > queue->nodes[i].priority) { queue->nodes[i+1] = queue->nodes[i]; } //if node adding has a lower priority (higher value) than the current node in the queue, move current node further forward in queue
			else { break; }
		}
		
		queue->nodes[i+1] = node;
	}
	
	queue->index++;
	return &(queue->nodes[queue->index]);
}

QueueNode queue_dequeue(Queue *queue) {
	
	if (!queue_is_empty(*queue)) {
		
		(queue->index)--;
		return queue->nodes[queue->index];
	}
	
	return (QueueNode){0};
}

int queue_find_neighbors(Queue *queue, QueueNode from, int num_neighbors, uint16_t *neighbor_indexes) {
	
	int i = 0;
	int visited = 0;
	
	for (i = queue->index-1; i >= 0 && visited < num_neighbors; i--) {
		
		if (coordinate_is_neighbor(queue->nodes[i].cell_loc, from.cell_loc)) {
			
			neighbor_indexes[visited] = i;
			visited++;
		}
	}
	
	return visited;
}

void queue_sort(Queue *queue) {
	
	int i = 0, j = 0;
	
	for (i = queue->index-1; i > 0; i--) {
		for (j = i-1; j >= 0; j--) {
			
			if (queue->nodes[j].priority < queue->nodes[i].priority) { queue_node_swap(&(queue->nodes[j]), &(queue->nodes[i])); } //if nodes[j] has a higher priority (lower value) than nodes[i], swap
		}
	}
	
	return;
}

void queue_node_swap(QueueNode *beta, QueueNode *alpha) {
	
	QueueNode temp = *alpha;
	*alpha = *beta;
	*beta = temp;
}

void queue_print(Queue queue) {
	
	int i = 0;
	
	printf("queue -- index: [%d]\n", queue.index);
	
	for (i = queue.index-1; i >= 0; i--) { printf("queue -- position: [%3d]   priority: [%d]\n", i, queue.nodes[i].priority); }
	
	return;
}