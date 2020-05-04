#include "binary_heap.h"

HEAP *init_heap(int num_of_nodes){
    HEAP *heap = (HEAP *)malloc(sizeof(HEAP));

    heap->heap_node = (HEAP_NODE *)malloc(num_of_nodes * sizeof(HEAP_NODE));

    return heap;
}

HEAP_NODE *init_heap_node(int index, int path_cost){
    HEAP_NODE *heap_node = (HEAP_NODE *)malloc(sizeof(HEAP_NODE));

    heap_node->index = index;
    heap_node->value = path_cost;

    return heap_node;
}

void swap_heap_nodes(HEAP_NODE *h1, HEAP_NODE *h2){
    HEAP_NODE *temp = h1;
    h1 = h2;
    h2 = temp;
}