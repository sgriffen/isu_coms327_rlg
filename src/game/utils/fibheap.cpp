/* code based on implementation from:
	https://www.geeksforgeeks.org/fibonacci-heap-deletion-extract-min-and-decrease-key/
	and
	https://www.sanfoundry.com/cpp-program-implement-fibonacci-heap/
*/

/******** include std libs ********/
#include <iostream>
#include <algorithm>

/******* include custom libs ******/
#include "./classdef/fibheap.h"

/********** definitions **********/


/*********** global vars **********/


/***** constructor definitions *****/
FibNode::FibNode() {
	
	this->key		= INT32_MAX;
	this->element 	= NULL;
	this->parent 	= NULL;
	this->child 	= NULL;
	this->sib_left 	= this;
	this->sib_right = this;
	
	this->degree 	= 0;
	this->mark 		= 0;
	this->found 	= 0;
}
FibNode::FibNode(const FibNode &rhs) {
	
	this->key		= rhs.key;
	this->element 	= rhs.element;
	this->parent 	= rhs.parent;
	this->child 	= rhs.child;
	this->sib_left 	= rhs.sib_left;
	this->sib_right = rhs.sib_right;
	
	this->degree 	= rhs.degree;
	this->mark 		= rhs.mark;
	this->found 	= rhs.found;
}
FibNode::FibNode(int64_t node_key, void *node_element) : FibNode() {
	
	this->key		= node_key;
	this->element 	= node_element;
}

FibHeap::FibHeap() {
	
	this->size 	= 0;
	this->min 	= NULL;
}
FibHeap::FibHeap(const FibHeap &rhs) {
	
	this->size 	= rhs.size;
	this->min 	= rhs.min;
}
FibHeap::FibHeap(FibNode root) : FibHeap() {
	
	this->size 	= 1;
	nodes[0] = root;
	this->min 	= &nodes[size-1];
}

/****** function definitions ******/
int FibHeap::is_empty() { return !(this->min); }
int FibHeap::is_full() { return this->size >= FIBHEAP_SIZE_MAX; }
void FibHeap::clear() {
	
	this->size = 0;
	this->min = NULL;
	
	return;
}
FibHeap* FibHeap::h_union(FibHeap *beta, FibHeap *alpha) {
	
	FibNode* node = NULL;
	
	FibNode* beta_min = beta->min;
	FibNode* alpha_min = alpha->min;
	
	(beta_min->sib_left)->sib_right = alpha_min;
	(alpha_min->sib_left)->sib_right = beta_min;
	
	node = beta_min->sib_left;
	
	beta_min->sib_left = alpha_min->sib_left;
	alpha_min->sib_left = node;
	
	return beta;
}

FibNode* FibHeap::insert_key(int64_t in_key, void* in_element) {
	
	FibNode node = FibNode(in_key, in_element);
	return insert_node(node);
}
FibNode* FibHeap::insert_node(FibNode in_node) {

    if (is_full()) { return NULL; }

    nodes[size] = in_node;
    FibNode *node =  &nodes[size];
	if (this->min) {

		((this->min)->sib_left)->sib_right = node;
        node->sib_right = this->min;
        node->sib_left = (this->min)->sib_left;
		(this->min)->sib_left = node;
		if (node->key < (this->min)->key) { this->min = node; }
	} else { this->min = node; }
	
	size++;
	return &nodes[size-1];
}

FibNode* FibHeap::link(FibNode *beta, FibNode *alpha) {
	
	(beta->sib_left)->sib_right = beta->sib_right;
	(beta->sib_right)->sib_left = beta->sib_left;
	if (alpha->sib_right == alpha) { this->min = alpha; }
	
	beta->sib_left = beta;
	beta->sib_right = beta;
	beta->parent = alpha;
	if (!(alpha->child)) { alpha->child = beta; }
	
	beta->sib_right = alpha->child;
    beta->sib_left = (alpha->child)->sib_left;
    ((alpha->child)->sib_left)->sib_right = beta;
    (alpha->child)->sib_left = beta;
    if (beta->key < (alpha->child)->key) { alpha->child = beta; }
	
    (alpha->degree)++;
	
	return this->min;
}

FibNode* FibHeap::peak_min() { return min; }
FibNode FibHeap::extract_min() {
	
	if (this->min) {
		
		FibNode extracted = *(this->min);
		
		FibNode* temp_a = this->min;
		FibNode* temp_b = temp_a;
		FibNode* temp_c = NULL;
		
		if (temp_a->child) {
			
			temp_c = temp_a->child;
			do {
				
				temp_b = temp_c->sib_right;
				((this->min)->sib_left)->sib_right = temp_c;
				temp_c->sib_right = this->min;
				temp_c->sib_left = (this->min)->sib_left;
				(this->min)->sib_left = temp_c;
				
				if (temp_c->key < (this->min)->key) { this->min = temp_c; }
				
				temp_c->parent = NULL;
				temp_c = temp_b;
			} while (temp_b != temp_a->child);
		}
		(temp_a->sib_left)->sib_right = temp_a->sib_right;
		(temp_a->sib_right)->sib_left = temp_a->sib_left;
		
		this->min = temp_a->sib_right;
		
		if (temp_a == temp_a->sib_right && !(temp_a->child)) { this->min = NULL; }
		else {
			
			this->min = temp_a->sib_right;
			consolidate();
		}
		
		size--;
		return extracted;
	}
	
	return FibNode();
}

FibNode FibHeap::delete_key(int64_t del_key) {
	
	FibNode* temp = find_key(del_key);
	return delete_node(temp);
}
FibNode FibHeap::delete_node(FibNode *del_node) {
	
	if (del_node) {
		
		FibNode removed = *del_node;
		
		decrease_node(del_node, INT32_MIN);
		extract_min();
		
		return removed;
	}
	
	return FibNode();
}

FibNode* FibHeap::decrease_key(int64_t key_old, int64_t key_new) {
	
	FibNode* node = find_key(key_old);
	return decrease_node(node, key_new);
}
FibNode* FibHeap::decrease_node(FibNode *node, int64_t key_new) {
	
	if (node) {
		
		node->key = key_new;
		
		FibNode* temp = node->parent;
		if (temp && (node->key < temp->key)) {
				
			cut(node, temp);
			cut_cascade(temp);
		}
		if (node->key < (this->min)->key) { this->min = node; }
		
		return node;
	}
	return NULL;
}

FibNode* FibHeap::find_key(int64_t find_key) { return find_node(this->min, find_key); }
FibNode* FibHeap::find_node(FibNode* start, int64_t find_key) {
	
	FibNode* temp = start;
	temp->found = 1;
	FibNode* node_found = NULL;
	
	if (temp->key == find_key) {
		
		node_found = temp;
		temp->found = 0;
		return node_found;
	}
	if (!node_found) {
		
		if (temp->child) { node_found = find_node(temp->child, find_key); }
		if (!((temp->sib_right)->found)) { node_found = find_node(temp->sib_right, find_key); }
	}
	
	start->found = 0;
	return node_found;
}

void FibHeap::consolidate() {
	
	int i = 0;
	
	int temp_a = 0;
	float temp_b = (log(size)) / (log(2));
	int temp_c = temp_b;
	
	FibNode *arr_node[temp_c];
	for (i = 0; i <= temp_c; i++) { arr_node[i] = NULL; }
	
	FibNode *node_a = this->min;
	FibNode *node_b;
	FibNode *node_d = node_a;
	
	do {
		
		node_d = node_d->sib_right;
		temp_a = node_a->degree;
		while (arr_node[temp_a]) {
			
			node_b = arr_node[temp_a];
			if (node_a->key > node_b->key) {
				
				FibNode* temp = node_a;
				node_a = node_b;
				node_b = temp;
			}
			if (node_b == this->min) { this->min = node_a; }
			link(node_b, node_a);
			if (node_a->sib_right == node_a) { this->min = node_a; }
			arr_node[temp_a] = NULL;
			temp_a++;
		}
		arr_node[temp_a] = node_a;
		node_a = node_a->sib_right;
	} while (node_a != this->min);
	
	this->min = NULL;
	
	for (i = 0; i <= temp_c; i++) {
		
		if (arr_node[i]) {
			
			arr_node[i]->sib_left = arr_node[i];
			arr_node[i]->sib_right = arr_node[i];
			if (this->min) {
				
				((this->min)->sib_left)->sib_right = arr_node[i];
				arr_node[i]->sib_right = this->min;
				arr_node[i]->sib_left = (this->min)->sib_left;
				if (arr_node[i]->key < (this->min)->key) { this->min = arr_node[i]; }
			} else { this->min = arr_node[i]; }
			
			if (!(this->min)) { this->min = arr_node[i]; }
			else if (arr_node[i]->key < (this->min)->key) { (this->min) = arr_node[i]; }
		}
	}
	
	return;
}
void FibHeap::cut(FibNode *node, FibNode *temp) {
	
	if (node == node->sib_right) { temp->child = NULL; }
	
	(node->sib_left)->sib_right = node->sib_right;
	(node->sib_right)->sib_left = node->sib_left;
	if (node == temp->child) { temp->child = node->sib_right; }
	
	(temp->degree)--;
	node->sib_right = node;
	node->sib_left = node;
	((this->min)->sib_left)->sib_right = node;
	node->sib_right = this->min;
	node->sib_left = (this->min)->sib_left;
	(this->min)->sib_left = node;
	
	node->parent = NULL;
	node->mark = 1;
	
	return;
}
void FibHeap::cut_cascade(FibNode *node) {
	
	FibNode *temp = node->parent;
	if (temp) {
		if (!(node->mark)) { node->mark = 1; }
		else {
			
			cut(node, temp);
			cut_cascade(temp);
		}
	}
	
	return;
}

void FibHeap::print() {
	
	if (!(this->min)) { std::cout << "Heap is Empty" << std::endl; }
	else {
		
		FibNode* node = this->min;
		std::cout << "Root nodes are: " << std::endl;
		do {
			
			std::cout << "{ key:[" << node->key << "], element:[" << node->element << "] }";
			node = node->sib_right;
			if (node != this->min) { std::cout << "-->"; }
		} while (node != this->min && node->sib_right);
		
		std::cout << std::endl << "Heap contains [" << size << "] nodes" << std::endl;
	}
	
	return;
}