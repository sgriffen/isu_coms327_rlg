/******** include std libs ********/
#include <iostream>

/******* include custom libs ******/
#include "./classdef/fibheap.h"

/********** definitions **********/


/*********** global vars **********/


/***** constructor definitions *****/
FibNode::FibNode() {
	
	key			= UINT16_MAX;
	element 	= NULL;
	parent 		= NULL;
	child 		= NULL;
	sib_left 	= NULL;
	sib_right 	= NULL;
	
	degree 		= 0;
	mark 		= 0;
	found 		= 0;
}
FibNode::FibNode(const FibNode &rhs) {
	
	key			= rhs.key;
	element 	= rhs.element;
	parent 		= rhs.parent;
	child 		= rhs.child;
	sib_left 	= rhs.sib_left;
	sib_right 	= rhs.sib_right;
	
	degree 		= rhs.degree;
	mark 		= rhs.mark;
	found 		= rhs.found;
}
FibNode::FibNode(uint64_t key, void *element) : FibNode() {
	
	key			= key;
	element 	= element;
}

FibHeap::FibHeap() {
	
	size = 0;
	min = FibNode();
}
FibHeap::FibHeap(const FibHeap &rhs) {
	
	size = rhs.size;
	min = rhs.min;
}
FibHeap::FibHeap(FibNode root) : FibHeap() {
	
	min = FibNode(&root);
}

/****** function definitions ******/
FibHeap* FibHeap::heap_union(FibHeap *alpha, FibHeap *beta) {
	
	
	return NULL;
}
void FibHeap::heap_consolidate() {
	
	
	return;
}
void FibHeap::heap_cut(FibNode *node) {
	
	
	return;
}
void FibHeap::heap_cascade_cut(FibNode *node) {
	
	
	return;
}

FibNode* FibHeap::node_find(uint64_t key) {
	
	
	return NULL;
}
FibNode* FibHeap::node_link(FibNode *alpha, FibNode *beta) {
	
	
	return NULL;
}

FibNode* FibHeap::insert_key(uint64_t in_key, void* element) {
	
	
	return NULL;
}
FibNode* FibHeap::insert_node(FibNode *in_node) {
	
	
	return NULL;
}

FibNode FibHeap::extract_min(FibNode *in_node) {
	
	
	return FibNode();
}

FibNode FibHeap::delete_key(uint64_t del_key) {
	
	
	return FibNode();
}
FibNode FibHeap::delete_node(FibNode *del_node) {
	
	
	return FibNode();
}

FibNode* FibHeap::decrease_key(uint64_t key_old, uint64_t key_new) {
	
	
	return NULL;
}
FibNode* FibHeap::decrease_node(FibNode *node, uint64_t key_new) {
	
	
	return NULL;
}

void FibHeap::print() {
	
	
	return;
}