#inblude <vector>
#include <cmath>
#include <cstdint>

#ifndef FIBHEAP_H
#define FIBHEAP_H

/********** definitions **********/


/************* macros *************/


/******* enums declarations *******/


/******* struct declarations ******/
class FibNode {
	public:
		
		uint64_t key;
		void *element;
		
		FibNode *parent;
		FibNode *child;
		FibNode *sib_left;
		FibNode *sib_right;
		
		int32_t degree;
		uint8_t mark;
		uint8_t found;
		
		/* CONSTRUCTORS */
		FibNode();
		FibNode(const FibNode &rhs);
		FibNode(uint64_t key, void *element);
		
		/* FUNCTIONS */
		
};

class FibHeap {
	public:
		uint16_t size;
	
		FibNode min;
		
		/* CONSTRUCTORS */
		FibHeap();
		FibHeap(const FibHeap &rhs);
		FibHeap(FibNode root);
		
		/* FUNCTIONS */
		FibHeap* heap_union(FibHeap *alpha, FibHeap *beta);
		
		FibNode* node_find(uint64_t key);
		FibNode* node_link(FibNode *alpha, FibNode *beta);
		
		FibNode* insert_key(uint64_t in_key, void* element);
		FibNode* insert_node(FibNode *in_node);
		
		FibNode extract_min(FibNode *in_node);
		
		FibNode delete_key(uint64_t del_key);
		FibNode delete_node(FibNode *del_node);
		
		FibNode* decrease_key(uint64_t key_old, uint64_t key_new);
		FibNode* decrease_node(FibNode *node, uint64_t key_new);
		
		void heap_consolidate();
		void heap_cut(FibNode *node);
		void heap_cascade_cut(FibNode *node);
		void print();
};

#endif /* FIBHEAP_H */