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

QueueNode queue_node_init(void *element, uint64_t priority) {
	
	QueueNode node = {
		
		.priority = priority,
		.element = element,
		.from = NULL
	};
	
	return node;
}

int queue_is_empty(Queue queue) { return !queue.index; }

int queue_is_full(Queue queue) { return queue.index == queue.size; }

QueueNode* queue_append(Queue *queue, QueueNode node) {
	
	if (queue_is_full(*queue)) { return NULL; }
	
	node.priority = (queue->nodes[(queue->index)-1].priority) + 1;
	return queue_enqueue(queue, node);
}

QueueNode* queue_enqueue(Queue *queue, QueueNode node) {
	
	int i = 0;
	
	if (queue_is_full(*queue)) { return NULL; }
	
	if (queue_is_empty(*queue)) { queue->nodes[queue->index] = node; }
	else {
		
		for (i = queue->index-1; i >= 0; i--) { //start at back of queue
			
			if (node.priority > queue->nodes[i].priority) { queue->nodes[i+1] = queue->nodes[i]; } //if node adding has a lower priority (higher value) than the current node in the queue, move current node further back in queue
			else { break; }
		}
		
		queue->nodes[i+1] = node;
	}
	
	queue->index++;
	return &(queue->nodes[i+1]);
}

QueueNode* queue_peek(Queue *queue) { return &(queue->nodes[(queue->index)-1]); }

QueueNode queue_dequeue(Queue *queue) {
	
	if (!queue_is_empty(*queue)) {
		
		(queue->index)--;
		return queue->nodes[queue->index];
	}
	
	return (QueueNode){0};
}

int queue_find_neighbors_card(Queue *queue, QueueNode from, int num_neighbors, QueueNode **neighbors) {
	
	int i = 0;
	int visited = 0;
	
	for (i = queue->index-1; i >= 0 && visited < num_neighbors; i--) {
		
		Coordinate *loc = (Coordinate*)(queue->nodes[i].element);
		Coordinate *loc_from = (Coordinate*)(from.element);
		if (coordinate_is_neighbor_card(*loc, *loc_from)) {
			
			neighbors[visited] = &(queue->nodes[i]);
			visited++;
		}
	}
	
	return visited;
}

int queue_find_neighbors_diag(Queue *queue, QueueNode from, int num_neighbors, QueueNode **neighbors) {
	
	int i = 0;
	int visited = 0;
	
	for (i = queue->index-1; i >= 0 && visited < num_neighbors; i--) {
		
		Coordinate *loc = (Coordinate*)(queue->nodes[i].element);
		Coordinate *loc_from = (Coordinate*)(from.element);
		if (coordinate_is_neighbor_diag(*loc, *loc_from)) {
			
			neighbors[visited] = &(queue->nodes[i]);
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
	
	for (i = queue.index-1; i >= 0; i--) { printf("queue -- position: [%3d]   priority: [%lu]\n", i, queue.nodes[i].priority); }
	
	return;
}