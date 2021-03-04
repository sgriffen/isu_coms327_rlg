#include <stdint.h>

#ifndef QUEUE_H
#define QUEUE_H

#include "../coordinate.h"


/********** definitions **********/


/************* macros *************/


/******* enums declarations *******/


/******* struct declarations ******/
typedef struct QueueNode QueueNode;

struct QueueNode {
	
	uint32_t priority;
	
	void *element;
	QueueNode *from;
};

typedef struct {
	
	uint16_t size;
	uint16_t index;
	
	QueueNode *nodes;
} Queue;

/****** function declarations *****/
Queue queue_init(uint16_t size);

QueueNode queue_node_init(void *loc, uint32_t priority);

int queue_is_empty(Queue queue);

int queue_is_full(Queue queue);

QueueNode* queue_peek(Queue *queue);

QueueNode* queue_enqueue(Queue *queue, QueueNode node);

QueueNode queue_dequeue(Queue *queue);

int queue_find_neighbors_card(Queue *queue, QueueNode from, int num_neighbors, QueueNode **neighbors);

int queue_find_neighbors_diag(Queue *queue, QueueNode from, int num_neighbors, QueueNode **neighbors);

void queue_sort(Queue *queue);

void queue_node_swap(QueueNode *beta, QueueNode *alpha);

void queue_print(Queue queue);

#endif /* QUEUE_H */