#include "dijkstra.h"
#include "binary_heap.c"

MAZE *init_starting_vertex(MAZE *maze, int index){
    maze->path[index].cost = FOREST_PATH_VALUE;
    maze->path[index].known = TRUE;
    maze->path[index].src_path_root = index;

    return maze;
}

MAZE *set_known_root_node(MAZE *maze, int index){
    maze->path[index].known = TRUE;

    return maze;
}

/* najdem node z LL ako root node a updatujem mu paht cost */
MAZE *change_path_cost(MAZE *maze, PATH_NODE *path_node, HEAP *heap, int root_path_cost, int root_path_index){
    for (int i = 0; i < maze->nodes_num; i++){

        /* najdem pozadovany path node v path (verticalnom smere) */ 
        if(maze->path[i].path_root->id == path_node->id){

            /* Ak dany path root este nie je objaveny a jeho path cost je vyssia ako aktualna */ 
            if(maze->path[i].known == FALSE && path_node->cost + root_path_cost < maze->path[i].cost){
                maze->path[i].cost = path_node->cost + root_path_cost;
                maze->path[i].src_path_root = root_path_index;
                heap = insert_heap_node(heap, i, path_node->cost + root_path_cost);
            }

            return maze;
        }
    }

    printf("change_path_cost - Asi je niekde chyba, nenasiel som match!\n");
    return maze;
}

/* Prejde linked list od pathroota horizontalne */
MAZE *find_and_update_neighboor(MAZE *maze, PATH_NODE *current_path_node, HEAP *heap, int root_path_cost, int root_path_index){

    while(current_path_node != NULL){
        maze = change_path_cost(maze, current_path_node, heap, root_path_cost, root_path_index);
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

    maze = find_and_update_neighboor(maze, maze->path[0].path_root->next, heap, maze->path[0].cost, 0);

    /* vyberam kym je nieco v halde */
    while((heap = push_and_pop_min(heap, index)) != NULL){
        // printf("index: %d\n", *index);
        maze = set_known_root_node(maze, *index);
        maze = find_and_update_neighboor(maze, maze->path[*index].path_root->next, heap, maze->path[*index].cost, *index);
        // printf("\n");
        // print_heap(heap);
    }

    return maze;
}