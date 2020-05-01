#include <stdio.h>
#include <stdlib.h> 
#include "popolvar.h"


MAZE *init_maze(int maze_width, int maze_height){
    MAZE *maze = (MAZE *)malloc(sizeof(MAZE));
    maze->width = maze_width;
    maze->height = maze_height;

    return maze;
}

PATH *init_path(){
    PATH *path = (PATH *)malloc(sizeof(PATH));
    path->path_root = NULL;
    path->cost = 0;

    return path;
}

PATH_NODE *init_path_node(int id, int path_node_cost, int pos_x, int pos_y){
    PATH_NODE *path_node = (PATH_NODE *)malloc(sizeof(PATH_NODE));
    path_node->id = id;
    path_node->cost = path_node_cost;
    path_node->position->x = pos_x;
    path_node->position->y = pos_y;
    path_node->next = NULL;

    return path_node;
}

void load_maze(char **mapa, MAZE *maze){
    int i, j;

    for (i = 0; i < maze->height; i++){
        for (j = 0; j < maze->width; j++){
            if(mapa[i][j] == PRINC){
                
            }
            else if(mapa[i][j] == FOREST_PATH){

            }
        }
    }
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty){
    MAZE *maze = init_maze(m, n);
    load_maze(mapa, maze);

    return (int *)2;
}
