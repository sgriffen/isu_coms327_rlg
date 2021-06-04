/******** include std libs ********/
#include <iostream>

#include "./classdef/queue.h"

/******* include custom libs ******/

/********** definitions **********/


/*********** global vars **********/


/***** constructor definitions *****/
QueueNode::QueueNode() {
	
    index       = 0;
    key 		= INT32_MAX;
    removed     = 0;
    element 	= NULL;
    previous 	= NULL;
    next 		= NULL;
}
QueueNode::QueueNode(const QueueNode &rhs) {

    index       = rhs.index;
    key 		= rhs.key;
    removed     = rhs.removed;
    element 	= rhs.element;
    previous 	= rhs.previous;
    next 		= rhs.next;
}
QueueNode::QueueNode(int64_t queue_key, void *queue_element) : QueueNode() {

    key 	= queue_key;
    element = queue_element;
}

Queue::Queue() {

    size    = 0;
    index   = 0;
    head    = NULL;
    tail    = NULL;
}
Queue::Queue(QueueNode queue_head) : Queue() {

    size    		= 1;
    index   		= 1;
	nodes[0]   		= queue_head;
    head    		= &(nodes[index-1]);
    tail    		= &(nodes[index-1]);
	head->next 		= &(nodes[index-1]);
	tail->previous 	= &(nodes[index-1]);
}
Queue::Queue(const Queue &rhs) {
	
    size 	= rhs.size;
    index   = rhs.index;
    head 	= rhs.head;
    tail 	= rhs.tail;
}

/****** function definitions ******/
int Queue::is_empty() { return !(head); }
int Queue::is_full() { return index >= QUEUE_SIZE_MAX; }
void Queue::clear() {

    size    = 0;
    index   = 0;
    head    = NULL;
    tail    = NULL;
	
    return;
}

QueueNode* Queue::append(void *element) {

    QueueNode node = QueueNode(INT32_MAX, element);
    return append(node);
}
QueueNode* Queue::append(QueueNode node) {

    if (is_full()) { return NULL; }

    node.key = (tail->key) + 1;

    return insert_back(node);
}
QueueNode* Queue::enqueue(int64_t key, void *element) {

    QueueNode node = QueueNode(key, element);
    return enqueue(node);
}
QueueNode* Queue::enqueue(QueueNode node) { //lower key = higher priority (aka closer to head which is dequeued first)

    if (is_full()) { return NULL; }

    if (is_empty()) {

        node.index = index;
        nodes[index] = node;

        head = &(nodes[index]);
        tail = &(nodes[index]);

        head->previous = NULL;
        tail->next = NULL;

        size++;
        index++;
        return &(nodes[index-1]);
    }
    else {

        if (node.key < head->key)       { return insert_front(node); }
        else if (node.key >= tail->key) { return insert_back(node); }
        else {

            //get average of head key and tail key, decide which direction to iterate through queue to insert node to
            int avg_key = (head->key + tail->key) / 2;

            //insert node before head, move up queue until sorted
            if (node.key < avg_key) { increase_swap(insert_front(node)); }
            // insert node after tail, move down queue until sorted
            else 					{ decrease_swap(insert_back(node)); }

            return &(nodes[index-1]);
        }
    }
}

QueueNode* Queue::peek() { return head; }

QueueNode Queue::dequeue() {

    if (!is_empty()) {

        head->removed = 1;
        QueueNode dequeued = *head;

        if (!head->next) {

            clear();
            return dequeued;
        }

        (head->next)->previous = NULL;
        head = head->next;

        size--;

        return dequeued;
    }

    return QueueNode();
}
void Queue::change_key(int64_t key_old, int64_t key_new) {

    QueueNode *node = find(key_old);
    return change_key(node, key_new);
}
void Queue::change_key(QueueNode *node, int64_t key_new) {

    if (node) {

        int key_old = node->key;
        node->key = key_new;
        if (key_new < key_old) 	{ decrease_swap(node); }
        else 					{ increase_swap(node); }

        return;
    }

    return;
}

QueueNode* Queue::find(int64_t key_find) {

    int avg_key = (head->key + tail->key) / 2;

    if (key_find < avg_key) { return find(head, key_find, 1); }
    else 					{ return find(tail, key_find, 0); }
}
QueueNode* Queue::find(QueueNode *start, int64_t key_find, int direction_fwd) {

    while (start) {

        if (start->key == key_find) { return start; }
        if (direction_fwd) { start = start->next; }
        else { start = start->previous; }
    }

    return NULL;
}

void Queue::print() {

    if (!head) { std::cout << "Queue is empty" << std::endl; }
    else {

        QueueNode *start = head;
        do {

            std::cout << "{ key:[" << start->key << "], element:[" << start->element << "] }";
            if (start->next) { std::cout << "<--->"; }
            start = start->next;
        } while (start);
        std::cout << std::endl << "Queue contains [" << size << "] nodes" << std::endl;
    }
}

QueueNode* Queue::insert_front(QueueNode node) {

    nodes[index] = node;

    nodes[index].previous = NULL;
    nodes[index].next = head;
    head->previous = &nodes[index];

    head = &nodes[index];

    size++;
    index++;
    return &nodes[index-1];
}
QueueNode* Queue::insert_back(QueueNode node) {

    nodes[index] = node;

    nodes[index].previous = tail;
    nodes[index].next = NULL;
    tail->next = &nodes[index];

    tail = &nodes[index];

    size++;
    index++;
    return &nodes[index-1];
}

void Queue::decrease_swap(QueueNode *node) {

    if (node->previous && node->key < (node->previous)->key) {

        node_swap(node->previous, node);
        decrease_swap(node);
    }

    return;
}
void Queue::increase_swap(QueueNode *node) {

    if (node->next && node->key >= (node->next)->key) {

        node_swap(node, node->next);
        increase_swap(node);
    }

    return;
}
void Queue::node_swap(QueueNode *previous, QueueNode *next) { // swap previous and next positions

    if (!previous || !next || previous == next) { return; }

    if (!(previous->previous) || !(next->previous)) {

        QueueNode *node_alpha;
        QueueNode *node_beta;
        if (!(previous->previous)) {

            node_alpha = previous;
            node_beta = next;
        } else {

            node_alpha = next;
            node_beta = previous;
        }
        head = node_beta;
        node_beta->previous = NULL;
        node_alpha->next = node_beta->next;
        (node_beta->next)->previous = node_alpha;
        node_beta->next = node_alpha;
        node_alpha->previous = node_beta;
    }
    else if (!(next->next) || !(previous->next)) {

        QueueNode *node_alpha;
        QueueNode *node_beta;
        if (!(next->next)) {

            node_alpha = previous;
            node_beta = next;
        } else {

            node_alpha = next;
            node_beta = previous;
        }
        tail = node_alpha;
        node_alpha->next = NULL;
        node_beta->previous = node_alpha->previous;
        (node_alpha->previous)->next = node_beta;
        node_alpha->previous = node_beta;
        node_beta->next = node_alpha;
    } else {

        previous->previous->next = next;
        next->next->previous = previous;
        previous->next = next->next;
        next->previous = previous->previous;
        previous->previous = next;
        next->next = previous;
    }

    return;
}