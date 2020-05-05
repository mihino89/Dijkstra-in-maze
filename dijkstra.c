#include "dijkstra.h"
#include "binary_heap.c"

MAZE *init_starting_vertex(MAZE *maze, int index){
    maze->path[index].cost = FOREST_PATH_VALUE;
    maze->path[index].path_root->known = TRUE;

    return maze;
}

MAZE *set_known_root_node(MAZE *maze, int index){
    maze->path[index].path_root->known = TRUE;

    return maze;
}

/* najdem node z LL ako root node a updatujem mu paht cost */
MAZE *change_path_cost(MAZE *maze, PATH_NODE *path_node, HEAP *heap, int root_path_cost){
    for (int i = 0; i < maze->nodes_num; i++){
        if(maze->path[i].path_root->id == path_node->id){
            if(maze->path[i].path_root->known == FALSE && path_node->cost + root_path_cost < maze->path[i].cost){
                maze->path[i].cost = path_node->cost + root_path_cost;
                // printf("cost do haldy: %d\n", maze->path[i].cost);
                heap = insert_heap_node(heap, i, path_node->cost + root_path_cost);
                // tu vloz do binarnej haldy
            }
            return maze;
        }
    }

    printf("change_path_cost - Asi je niekde chyba, nenasiel som match!\n");
    return maze;
}

/* Prejde linked list od pathroota */
MAZE *find_and_update_neighboor(MAZE *maze, PATH_NODE *current_path_node, HEAP *heap, int cost){

    while(current_path_node != NULL){
        // printf("current vertex: [%d, %d]\n", current_path_node->position.y, current_path_node->position.x);
        maze = change_path_cost(maze, current_path_node, heap, cost);
        current_path_node = current_path_node->next;
    }

    return maze;
}

MAZE *dijkstra(MAZE *maze){
    PATH_NODE *current;
    HEAP *heap;
    int *index = (int *)malloc(sizeof(int));

    heap = init_heap(maze->nodes_num);
    maze = init_starting_vertex(maze, 0);

    maze = find_and_update_neighboor(maze, maze->path[0].path_root->next, heap, maze->path[0].cost);
    // print_heap(heap);

    /* vyberam kym je nieco v halde */
    while((heap = push_and_pop_min(heap, index)) != NULL){
        // printf("index: %d\n", *index);
        maze = set_known_root_node(maze, *index);
        maze = find_and_update_neighboor(maze, maze->path[*index].path_root->next, heap, maze->path[*index].cost);
        // printf("\n");
        // print_heap(heap);
    }

    return maze;
}