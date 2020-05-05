#include <stdio.h>
#include <stdlib.h> 

typedef struct heap_node{
    int index;
    int value;
} HEAP_NODE;

typedef struct heap{
    int heap_act_size;
    int heap_max_size;
    HEAP_NODE *heap_node;
} HEAP;