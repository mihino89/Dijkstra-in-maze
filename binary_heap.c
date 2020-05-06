#include "binary_heap.h"


HEAP *init_heap(int num_of_nodes){
    HEAP *heap = (HEAP *)malloc(sizeof(HEAP));

    heap->heap_node = (HEAP_NODE *)malloc(num_of_nodes * sizeof(HEAP_NODE));
    heap->heap_max_size = num_of_nodes;
    heap->heap_act_size = 0;

    return heap;
}


HEAP_NODE *init_heap_node(int index, int path_cost){
    HEAP_NODE *heap_node = (HEAP_NODE *)malloc(sizeof(HEAP_NODE));

    heap_node->index = index;
    heap_node->value = path_cost;

    return heap_node;
}


void print_heap(HEAP *heap){
    printf("----- Printing heap -----\n");
    printf("Number of heap nodes: %d and min is: %d\n", heap->heap_act_size, heap->heap_node[0].value);

    for (int i = 0; i < heap->heap_act_size; i++)
        printf("%d ", heap->heap_node[i].value);
    
    printf("\n\n");
}


void swap_heap_nodes(HEAP_NODE *h1, HEAP_NODE *h2){
    HEAP_NODE temp = *h1;
    *h1 = *h2;
    *h2 = temp;
}


HEAP_NODE *find_parent(HEAP *heap, int index){
    return &heap->heap_node[((index - 1) / 2)];
}


HEAP *clean_in_heap(HEAP *heap, int index_heap_node_need_clean){
    HEAP_NODE *parent;

    while(index_heap_node_need_clean > 0 && (parent = find_parent(heap, index_heap_node_need_clean))->value > heap->heap_node[index_heap_node_need_clean].value){
        // printf("funguje: 1. value: %d 1. index: %d, 2. value: %d 2. index: %d\n", heap->heap_node[index_heap_node_need_clean].value, index_heap_node_need_clean, heap->heap_node[((index_heap_node_need_clean - 1) / 2)].value, ((index_heap_node_need_clean - 1) / 2));
        swap_heap_nodes(&heap->heap_node[index_heap_node_need_clean], &heap->heap_node[((index_heap_node_need_clean - 1) / 2)]);
        // printf("funguje: 1. value: %d 1. index: %d, 2. value: %d 2. index: %d\n", heap->heap_node[index_heap_node_need_clean].value, index_heap_node_need_clean, heap->heap_node[((index_heap_node_need_clean - 1) / 2)].value, ((index_heap_node_need_clean - 1) / 2));
        index_heap_node_need_clean = (index_heap_node_need_clean - 1) / 2;
    }

    return heap;
}


HEAP *insert_heap_node(HEAP *heap, int index, int path_cost){
    HEAP_NODE *heap_node = init_heap_node(index, path_cost);

    /* resolution statement */
    if(heap->heap_act_size >= heap->heap_max_size){
        printf("No success insert to heap - ochrana proti preteceniu\n");
        return heap;
    }

    int act_index = heap->heap_act_size;

    /* vlozim dany heap_node do pola */
    heap->heap_node[heap->heap_act_size] = *heap_node;
    heap = clean_in_heap(heap, act_index);
    heap->heap_act_size++;

    return heap;
}


int get_path_index_of_min(HEAP *heap){
    return heap->heap_node[0].index;
}


HEAP *change_value_heap_node(HEAP *heap, int index, int new_value){
    heap->heap_node[index].value = new_value;

    heap = clean_in_heap(heap, index);
    return heap;
}


void heapifyTtB(HEAP *heap, int index){
    HEAP_NODE *left_child, *right_child;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    left_child = &heap->heap_node[left];
    right_child = &heap->heap_node[right];

    // printf("left: %d left_child val: %d right: %d right child val: %d parent index: %d parent val: %d\n", left, left_child->value, right, right_child->value, index, heap->heap_node[index].value);

    if((left < heap->heap_act_size && heap->heap_node[index].value > left_child->value) && (right < index && heap->heap_node[index].value > right_child->value)){
        // printf("HAHAHAHHA\n");
        swap_heap_nodes(&heap->heap_node[index], left_child->value < right_child->value ? &heap->heap_node[left] : &heap->heap_node[right]);
        heapifyTtB(heap, left_child->value < right_child->value
                             ? left_child->value
                             : right_child->value);
    }
    if(left < heap->heap_act_size && heap->heap_node[index].value > left_child->value){
        // printf("HAHAHAHHA2\n");
        swap_heap_nodes(&heap->heap_node[index], &heap->heap_node[left]);
        heapifyTtB(heap, left_child->value);
    }
    if(right < heap->heap_act_size && heap->heap_node[index].value > right_child->value){
        // printf("HAHAHAHHA3\n");
        swap_heap_nodes(&heap->heap_node[index], &heap->heap_node[right]);
        heapifyTtB(heap, right_child->value);
    }
}


HEAP *push_and_pop_min(HEAP *heap, int *path_index_of_min){
    *path_index_of_min = get_path_index_of_min(heap);

    if(heap->heap_act_size <= 0){
        free(heap);
        return NULL;
    }

    if (heap->heap_act_size == 1){
        heap->heap_act_size--;
        return heap;
    }

    heap->heap_act_size--;
    heap->heap_node[0] = heap->heap_node[heap->heap_act_size];
    heapifyTtB(heap, 0);

    return heap;
}