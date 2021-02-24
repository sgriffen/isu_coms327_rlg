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

QueueNode* queue_peek(Queue queue) { return &(queue.nodes[(queue.size)-1]); }

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
	
	(queue->index)--;
	return queue->nodes[queue->index];
}

QueueNode* queue_find_neighbor(Queue queue, QueueNode node) {
	
	int i = 0;
	
	for (i = queue.index-1; i >= 0; i--) {
		
		if (coordinate_is_neighbor(queue.nodes[i].cell_loc, node.cell_loc)) { return &(queue.nodes[i]); }
	}
	
	return NULL;
}

void queue_sort(Queue *queue) {
	
	int i = 0, j = 0;
	
	for (i = queue->index-1; i > 0; i--) {
		for (j = i-1; j >= 0; j--) {
			
			if (queue->nodes[j].priority < queue->nodes[i].priority) { //if nodes[j] has a higher priority (lower value) than nodes[i], swap
				
				QueueNode temp = queue->nodes[i];
				queue->nodes[i] = queue->nodes[j];
				queue->nodes[j] = temp;
			}
		}
	}
	
	return;
}

void queue_print(Queue queue) {
	
	int i = 0;
	
	printf("queue -- size: [%d]\n", queue.index);
	
	for (i = queue.index-1; i >= 0; i--) { printf("queue -- position: [%d]   priority: [%d]\n", i, queue.nodes[i].priority); }
	
	return;
}