#include <stdio.h>
#include <stdlib.h> 

typedef struct heap_node{
    int index;
    int value;
} HEAP_NODE;

typedef struct heap{
    HEAP_NODE *heap_node;
} HEAP;