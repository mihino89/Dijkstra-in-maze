#include <stdio.h>
#include <stdlib.h> 
#include "graph.h"


PATH *init_path(int maze_width, int maze_height){
    PATH *path = (PATH *)malloc((maze_width + maze_height) * sizeof(PATH));
    path->cost = 0;

    for (int i = 0; i < maze_width + maze_height; i++)
        path->path_root = NULL;
    
    return path;
}

MAZE *init_maze(int maze_width, int maze_height){
    MAZE *maze = (MAZE *)malloc(sizeof(MAZE));
    maze->width = maze_width;
    maze->height = maze_height;

    maze->path = init_path(maze_width, maze_height);
    maze->dragon = NULL;

    return maze;
}

PATH_NODE *init_path_node(int id, int path_node_cost, int pos_y, int pos_x, int is_princess){
    PATH_NODE *path_node = (PATH_NODE *)malloc(sizeof(PATH_NODE));
    path_node->id = id;
    path_node->cost = path_node_cost;
    path_node->is_princess = is_princess;
    path_node->position.x = pos_x;
    path_node->position.y = pos_y;
    path_node->next = NULL;

    return path_node;
}


/* prida novy node do pola ako path root */
MAZE *actualize_path(MAZE *maze, PATH_NODE *check_node, int index){
    maze->path[index].path_root = check_node;
    maze->path[index].path_root->next = NULL;

    return maze;
}

int check_node_neighbors(MAZE *maze, char **map, int x, int y){
    if(x < 0 || x >= maze->width || y < 0 || y >= maze->height)
        return FALSE;
    
    if(map[y][x] == FOREST_PATH || map[y][x] == DRAG)
        return FOREST_PATH_VALUE;

    if(map[y][x] == SLOW_WAY)
        return SLOW_PATH_VALUE;

    if(map[y][x] == PRINC){
        return PRINCESS_HELPER;
    }

    return FALSE;
}

MAZE *create_edge(MAZE *maze, PATH_NODE *src, PATH_NODE *dst, int index){
    int i = 0;
    PATH_NODE *current = maze->path[index].path_root;

    /* prechadzam pole path az nakoniec (verticalne) */
    while(current->next != NULL )
        current = current->next;
    
    current->next = dst;

    // if(maze->path[10].path_root != NULL){
    //     printf("maze->path[10].path_root != NULL !!!!!!!!!! [%d, %d]\n", maze->path[10].path_root->position.y, maze->path[10].path_root->position.x);
    // }

    return maze;

}

MAZE *add_neighboor_to_path(MAZE *maze, PATH_NODE *new_node, int neighboor_value, int index, int y, int x){
    if(neighboor_value <= FALSE){
        return maze;
    }
    
    PATH_NODE *neighboor_node;

    if(neighboor_value == PRINCESS_HELPER)
        neighboor_node = init_path_node(y * 10 + x, neighboor_value-SLOW_PATH_VALUE, y, x, TRUE);

    else
        neighboor_node = init_path_node(y * 10 + x, neighboor_value, y, x, FALSE);
    
    maze = create_edge(maze, new_node, neighboor_node, index);
    return maze;
}

MAZE *create_vertex(MAZE *maze, PATH_NODE *new_node, char **map, int index){
    PATH_NODE *neighboor_node;
    PATH_NODE *current = NULL;
    int neighboor_value, x, y;

    /* pridaj novy path root na prvy volny index v path */  
    maze = actualize_path(maze, new_node, index);
    // if(index ==  10){
    //     printf("BUG - path[0] = [%d, %d] path[0]->next = [%d, %d]\n", maze->path[0].path_root->position.y, maze->path[0].path_root->position.x, maze->path[0].path_root->next->position.y, maze->path[0].path_root->next->position.x);
    //     printf("BUG - new node path[%d] = [%d, %d]\n", index, new_node->position.y, new_node->position.x);
    //     if(maze->path[index].path_root == NULL){
    //         printf("haha som NULL\n");
    //     } else{
    //         printf("path[%d] = [%d, %d]\n",index, maze->path[index].path_root->position.y, maze->path[index].path_root->position.y);
    //     }
    //     // for (int i = 0; i <= index; i++){
    //     //     printf("path[%d] = [%d, %d]\n",i, maze->path[i].path_root->position.y, maze->path[i].path_root->position.x);
    //     // }

    //     maze->path[index].path_root = new_node;
    //     printf("BUG - path[0] = [%d, %d] path[0]->next = [%d, %d]\n\n", maze->path[0].path_root->position.y, maze->path[0].path_root->position.x, maze->path[0].path_root->next->position.y, maze->path[0].path_root->next->position.x);
    //     maze->path[index].path_root->next = NULL;
    // } else {
    //     maze = actualize_path(maze, new_node, index);
    // }

    x = new_node->position.x;
    y = new_node->position.y;

    /* smer - Vychod */
    neighboor_value = check_node_neighbors(maze, map, x + 1, y);
    maze = add_neighboor_to_path(maze, new_node, neighboor_value, index, y, (x + 1));

    /* smer - Zapad */
    neighboor_value = check_node_neighbors(maze, map, x - 1, y);
    maze = add_neighboor_to_path(maze, new_node, neighboor_value, index, y, (x - 1));
  
    /* smer - Sever */
    neighboor_value = check_node_neighbors(maze, map, x, y + 1);
    maze = add_neighboor_to_path(maze, new_node, neighboor_value, index, (y + 1), x);
   
    /* smer - Juh */
    neighboor_value = check_node_neighbors(maze, map, x, y - 1);
    maze = add_neighboor_to_path(maze, new_node, neighboor_value, index, (y - 1), x);

    // printf("----- Priebezny vypis grafu!! -----\n");
    // for (int i = 0; i <= index; i++){
    //     printf("i:%d ", i);
    //     current = maze->path[i].path_root;
    //     while(current->next != NULL ){
    //         printf(" [%d, %d] ->", current->position.y, current->position.x);
    //         current = current->next;
    //     }
    //     printf(" [%d, %d]\n", current->position.y, current->position.x);
    // }

    return maze;
}

MAZE *load_maze(char **mapa, MAZE *maze){
    PATH_NODE *new_node;
    int y, x, num_of_nodes = 0, num_of_princess = 0;

    for (y = 0; y < maze->height; y++){
        for (x = 0; x < maze->width; x++){
            if (mapa[y][x] == FOREST_PATH || mapa[y][x] == SLOW_WAY || mapa[y][x] == PRINC || mapa[y][x] == DRAG){

                if(mapa[y][x] == FOREST_PATH)
                    new_node = init_path_node(y * 10 + x, FOREST_PATH_VALUE, y, x, FALSE);
                
                else if(mapa[y][x] == PRINC)
                    new_node = init_path_node(y * 10 + x, FOREST_PATH_VALUE, y, x, TRUE);
                
                else
                    new_node = init_path_node(y * 10 + x, SLOW_PATH_VALUE, y, x, FALSE);

                maze = create_vertex(maze, new_node, mapa, num_of_nodes);

                num_of_nodes++;
            }
        }
    }
    maze->nodes_num = num_of_nodes;
    // printf("num of nodes: %d\n", num_of_nodes);

    return maze;
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty){
    int i = 0;
    PATH_NODE *current = NULL;

    MAZE *maze = init_maze(m, n);
    maze = load_maze(mapa, maze);

    // printf("num of nodes: [%d, %d]\n", maze->path[0].path_root->next->position.y, maze->path[0].path_root->next->position.x);

    printf("----- Konecny vypis grafu!! -----\n");
    for (i = 0; i < maze->nodes_num; i++){
        printf("i:%d ", i);
        current = maze->path[i].path_root;
        while(current->next != NULL ){
            printf(" [%d, %d] ->", current->position.y, current->position.x);
            current = current->next;
        }
        printf(" [%d, %d]\n", current->position.y, current->position.x);
    }

    return (int *)2;
}
