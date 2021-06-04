#include <cmath>
#include <cstdint>

#ifndef FIBHEAP_H
#define FIBHEAP_H

/********** definitions **********/
#define FIBHEAP_SIZE_MAX 10000

/************* macros *************/


/******* enums declarations *******/


/******** class declarations ******/
class FibNode {
	public:
		
		int64_t key;
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
		FibNode(int64_t node_key, void *node_element);
		
		/* FUNCTIONS */
		
};

class FibHeap {
	public:
		uint16_t size;

		FibNode nodes[FIBHEAP_SIZE_MAX];
		FibNode *min;

		/* CONSTRUCTORS */
		FibHeap();
		FibHeap(const FibHeap &rhs);
		FibHeap(FibNode root);
		
		/* FUNCTIONS */
		int is_empty();
		int is_full();
		void clear();
		FibHeap* h_union(FibHeap *beta, FibHeap *alpha);
		
		FibNode* insert_key(int64_t in_key, void* in_element);
		FibNode* insert_node(FibNode in_node);
		
		FibNode* link(FibNode *beta, FibNode *alpha);
		
		FibNode* peak_min();
		FibNode extract_min();
		FibNode delete_key(int64_t del_key);
		FibNode delete_node(FibNode *del_node);
		
		FibNode* decrease_key(int64_t key_old, int64_t key_new);
		FibNode* decrease_node(FibNode *node, int64_t key_new);
		
		FibNode* find_key(int64_t find_key);
		FibNode* find_node(FibNode* node, int64_t find_key);
		
		void consolidate();
		void cut(FibNode *node, FibNode *temp);
		void cut_cascade(FibNode *node);
		void print();
};

#endif /* FIBHEAP_H */