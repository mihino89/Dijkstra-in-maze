#include "dijkstra.h"

MAZE *init_starting_vertex(MAZE *maze, int index){
    maze->path[index].cost = 0;
    maze->path[index].path_root->known = TRUE;

    return maze;
}

MAZE *set_known_root_node(MAZE *maze, int index){
    maze->path[index].path_root->known = TRUE;

    return maze;
}

/* funkcia vrati index najmensieho costu pathu */
int smallest_cost_in_path(MAZE *maze){
    int min = INFINITY, index = -1;

    for (int i = 0; i < maze->nodes_num; i++){
        if (maze->path[i].cost < min && maze->path[i].path_root->known != TRUE){
            index = i;
            min = maze->path[i].cost;
        }
    }

    return index;
}

/* najdem node z LL ako root node a updatujem mu paht cost */
MAZE *change_path_cost(MAZE *maze, PATH_NODE *path_node, int root_path_cost){
    for (int i = 0; i < maze->nodes_num; i++){
        if(maze->path[i].path_root->id == path_node->id){
            if(maze->path[i].path_root->known == FALSE && path_node->cost + root_path_cost < maze->path[i].cost)
                maze->path[i].cost = path_node->cost + root_path_cost;
            return maze;
        }
    }

    printf("change_path_cost - Asi je niekde chyba, nenasiel som match!\n");
    return maze;
}

/* Prejde linked list od pathroota */
MAZE *find_and_update_neighboor(MAZE *maze, PATH_NODE *current_path_node, int cost){

    while(current_path_node != NULL){
        printf("current vertex: [%d, %d]\n", current_path_node->position.y, current_path_node->position.x);
        maze = change_path_cost(maze, current_path_node, cost);
        current_path_node = current_path_node->next;
    }

    return maze;
}

MAZE *dijkstra(MAZE *maze){
    PATH_NODE *current;

    maze = init_starting_vertex(maze, 0);
    maze = find_and_update_neighboor(maze, maze->path[0].path_root->next, maze->path[0].cost);

    int index;

    while((index = smallest_cost_in_path(maze)) != -1){
        printf("index: %d\n", index);
        maze = set_known_root_node(maze, index);
        maze = find_and_update_neighboor(maze, maze->path[index].path_root->next, maze->path[index].cost);
        // break;
    }

    // while(current != NULL){
    //     printf("current vertex: [%d, %d]\n", current->position.y, current->position.x);
    //     maze = change_path_cost(maze, current);
    //     current = current->next;
    // }

    return maze;
}