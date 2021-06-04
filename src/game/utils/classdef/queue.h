#ifndef QUEUE_H
#define QUEUE_H

#include <cstdlib>
#include <cstdint>
#include <vector>

/********** definitions **********/
#define QUEUE_SIZE_MAX 10000

/************* macros *************/


/******* enums declarations *******/


/******** class declarations ******/
class QueueNode {
public:

    int64_t index;
    int64_t key;
    void *element;
    uint8_t removed;

    QueueNode *previous;	//node that comes before this node in a sorted queue
    QueueNode *next;		//node that comes after this node in a sorted queue

    /* CONSTRUCTORS */
    QueueNode();
    QueueNode(const QueueNode &rhs);
    QueueNode(int64_t key, void *element);

    /* FUNCTIONS */

};

class Queue {
public:
    uint64_t size;
    uint64_t index;

    QueueNode nodes[QUEUE_SIZE_MAX];
	
    QueueNode *head;
    QueueNode *tail;

    /* CONSTRUCTORS */
    Queue();
	Queue(QueueNode queue_head);
    Queue(const Queue &rhs);

    /* FUNCTIONS */
    int is_empty();
    int is_full();
    void clear();

    QueueNode* append(void *element);
    QueueNode* append(QueueNode node);
    QueueNode* enqueue(int64_t key, void *element);
    QueueNode* enqueue(QueueNode node);

    QueueNode* peek();
    QueueNode dequeue();

    void change_key(int64_t key_old, int64_t key_new);
    void change_key(QueueNode *node, int64_t key_new);

    QueueNode* find(int64_t key_find);
    QueueNode* find(QueueNode *start, int64_t key_find, int direction_fwd);

    void print();
private:
    QueueNode* insert_front(QueueNode node);
    QueueNode* insert_back(QueueNode node);

    void decrease_swap(QueueNode *node);
    void increase_swap(QueueNode *node);
    void node_swap(QueueNode *previous, QueueNode *next);
};

/****** function declarations *****/


#endif /* QUEUE_H */