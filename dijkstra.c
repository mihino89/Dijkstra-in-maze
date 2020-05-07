#include "dijkstra.h"
#include "binary_heap.c"

void init_starting_vertex(MAZE *maze, int index){
    maze->path[index].cost = FOREST_PATH_VALUE;
    maze->path[index].known = TRUE;
    maze->path[index].source_path->index_of_src_path_root = index;
}

void set_known_root_node(MAZE *maze, int index){
    maze->path[index].known = TRUE;
}

/* najdem node z LL ako root node a updatujem mu paht cost */
void change_path_cost(MAZE *maze, PATH_NODE *path_node, HEAP *heap, int root_path_cost, int root_path_index){
    for (int i = 0; i < maze->nodes_num; i++){

        /* najdem pozadovany path node v path (verticalnom smere) */ 
        if(maze->path[i].path_root->id == path_node->id){

            /* Ak dany path root este nie je objaveny a jeho path cost je vyssia ako aktualna */ 
            if(maze->path[i].known == FALSE && path_node->cost + root_path_cost < maze->path[i].cost){
                maze->path[i].cost = path_node->cost + root_path_cost;
                maze->path[i].source_path->index_of_src_path_root = root_path_index;
                heap = insert_heap_node(heap, i, path_node->cost + root_path_cost);
            }

            return;
        }
    }

    printf("change_path_cost - Asi je niekde chyba, nenasiel som match!\n");
}

/* Prejde linked list od pathroota horizontalne */
void find_and_update_neighboor(MAZE *maze, PATH_NODE *current_path_node, HEAP *heap, int root_path_cost, int root_path_index){

    while(current_path_node != NULL){
        change_path_cost(maze, current_path_node, heap, root_path_cost, root_path_index);
        current_path_node = current_path_node->next;
    }
}


void check_and_update_src_paths(MAZE *maze, int starting_index){
    int index_counter;

    for (int i = 0; i < maze->nodes_num; i++){
        if (maze->path[i].known == FALSE){
            printf("nasiel som neobjaveny node, skontroluj to nieje princezna!\n");
        } else{
            /* pridam do pola src_path source nodes */
            maze->path[i].source_path->num_of_src_path_nodes++;

            if (maze->path[i].path_root->id != maze->path[starting_index].path_root->id){
                index_counter = maze->path[i].source_path->index_of_src_path_root;

                while (maze->path[index_counter].path_root->id != maze->path[starting_index].path_root->id){
                    maze->path[i].source_path->num_of_src_path_nodes++;
                    index_counter = maze->path[index_counter].source_path->index_of_src_path_root;
                }
                
                maze->path[i].source_path->num_of_src_path_nodes++;
            }            
        }
    }
}

MAZE *dijkstra(MAZE *maze, int starting_index){
    PATH_NODE *current;
    HEAP *heap;
    int *index = (int *)malloc(sizeof(int));

    heap = init_heap(maze->nodes_num);
    init_starting_vertex(maze, starting_index);

    find_and_update_neighboor(maze, maze->path[starting_index].path_root->next, heap, maze->path[starting_index].cost, starting_index);

    /* vyberam kym je nieco v halde */
    while((heap = push_and_pop_min(heap, index)) != NULL){
        set_known_root_node(maze, *index);
        find_and_update_neighboor(maze, maze->path[*index].path_root->next, heap, maze->path[*index].cost, *index);
    }

    check_and_update_src_paths(maze, starting_index);

    return maze;
}