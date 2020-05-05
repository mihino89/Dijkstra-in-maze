#include <stdio.h>
#include <stdlib.h> 

#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

typedef struct heap_node{
    int index;
    int value;
} HEAP_NODE;

typedef struct heap{
    int heap_act_size;
    int heap_max_size;
    HEAP_NODE *heap_node;
} HEAP;


HEAP *init_heap(int num_of_nodes);
HEAP_NODE *init_heap_node(int index, int path_cost);

void print_heap(HEAP *heap);
int get_path_index_of_min(HEAP *heap);

HEAP *change_value_heap_node(HEAP *heap, int index, int new_value);
void swap_heap_nodes(HEAP_NODE *h1, HEAP_NODE *h2);
HEAP_NODE *find_parent(HEAP *heap, int index);

HEAP *clean_in_heap(HEAP *heap, int index_heap_node_need_clean);
HEAP *insert_heap_node(HEAP *heap, int index, int path_cost);
void heapifyTtB(HEAP *heap, int index);
HEAP *push_and_pop_min(HEAP *heap, int *path_index_of_min);

#endif /* BINARY_HEAP.H */
