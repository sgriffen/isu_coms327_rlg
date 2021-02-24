#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>

#include "../cell.h"

/********** definitions **********/


/******* enums declarations *******/



/******* struct declarations ******/
typedef struct {
	
	uint32_t priority;
	
	Coordinate cell_loc;
	Coordinate from;
} QueueNode;

typedef struct {
	
	uint16_t size;
	uint16_t index;
	
	QueueNode *nodes;
} Queue;

/****** function declarations *****/
Queue queue_init(uint16_t size);

QueueNode queue_node_init(Coordinate loc, uint32_t priority);

int queue_is_empty(Queue queue);

int queue_is_full(Queue queue);

QueueNode* queue_peek(Queue *queue);

QueueNode* queue_enqueue(Queue *queue, QueueNode node);

QueueNode queue_dequeue(Queue *queue);

int queue_find_neighbors(Queue *queue, QueueNode from, int num_neighbors, uint16_t *neighbor_indexes);

void queue_sort(Queue *queue);

void queue_node_swap(QueueNode *beta, QueueNode *alpha);

void queue_print(Queue queue);

#endif /* QUEUE_H */