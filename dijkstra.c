#include "dijkstra.h"
#include "binary_heap.c"


void init_starting_vertex(MAZE *maze, int index){
    maze->path[index].cost = 0;
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
                
                /**
                 * Ak je predchadzajuci cost infinity, to znamena ze v halde este nie je zaznam toho cost
                 * ak nie je infinity zanmena z v halde uz je zaznam a preto ho treba zmenit aby nevznikali duplikaty
                */
                heap = maze->path[i].cost == INFINITY ? insert_heap_node(heap, i, path_node->cost + root_path_cost)
                                                      : change_for_cheaper_cost(heap, i, path_node->cost + root_path_cost);
                
                /* updatujem ho na lacnejsi cost aj v grafe */
                maze->path[i].cost = path_node->cost + root_path_cost;

                /* zmenim src node v grafe */
                maze->path[i].source_path->index_of_src_path_root = root_path_index;
            }

            return;
        }
    }

    printf("change_path_cost - Asi je niekde chyba, nenasiel som match!\n");
}


/* Prejde linked list od pathroota horizontalne */
void find_and_update_neighboor(MAZE *maze, HEAP *heap, int index){

    /* prvy sused od path root-a*/
    PATH_NODE *current = maze->path[index].path_root->next;

    /* path cost momentalny */
    int root_path_cost = maze->path[index].cost;

    /* prechadzam vsetkych susedov od path-root-a */
    while(current != NULL){
        change_path_cost(maze, current, heap, root_path_cost, index);
        current = current->next;
    }
}


int check_and_update_src_paths(MAZE *maze, int starting_index){
    int index_counter, i, j;

    for (i = 0; i < maze->nodes_num; i++){
        if (maze->path[i].known == FALSE){
            for (j = 0; j < maze->princess_num; j++){

                /* Kontrola ci sa jedna o princeznu - ak ano tak je zle a neviem sa k nim dostat */
                if(i == maze->princess_index_arr[j]){
                    printf("Zle je, princezna zabednena! %d %d\n", i, maze->princess_index_arr[j]);
                    return 0;
                }
            }

            /* Kontrola ci sa jedna o draka - ak ano tak je zle a neviem sa k nim dostat */
            if(i == maze->dragon->index){
                printf("Zle je, drak zabedneny! %d %d\n", i, maze->dragon->index);
                return 0;
            }
        } else{
            
            /* pripocitam najdeny node ako source */
            maze->path[i].source_path->num_of_src_path_nodes++;
            
            /**
             * ak dany pathroot nie je zaciatok cesty tak mas dany path root node source node
             * tak pokracuj az kym nepridem na node zaciatku cesty
            */
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

    return 1;
}

int dijkstra(MAZE *maze, int starting_index){
    PATH_NODE *current;
    HEAP *heap;
    int *index;
   
    index = (int *)malloc(sizeof(int));

    heap = init_heap(maze->nodes_num);
    init_starting_vertex(maze, starting_index);

    find_and_update_neighboor(maze, heap, starting_index);

    /* vyberam kym je nieco v halde */
    while((heap = push_and_pop_min(heap, index)) != NULL){

        /* funkcia push and pop min mi cez premennu index updatuje index min cost-u cesty a ten oznacim ako known true */
        set_known_root_node(maze, *index);

        /* najdem susedov path roota ktory ma priebezny najmensi cost */
        find_and_update_neighboor(maze, heap, *index);
    }

    if(check_and_update_src_paths(maze, starting_index) == 0){
        maze = NULL;
        return 0;
    }

    return 1;
}