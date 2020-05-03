#include <stdio.h>
#include <stdlib.h> 
#include "popolvar.h"


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
    maze->princess = NULL;

    return maze;
}

PATH_NODE *init_path_node(int id, int path_node_cost, int pos_y, int pos_x){
    PATH_NODE *path_node = (PATH_NODE *)malloc(sizeof(PATH_NODE));
    path_node->id = id;
    path_node->cost = path_node_cost;
    path_node->position.x = pos_x;
    path_node->position.y = pos_y;
    path_node->next = NULL;

    return path_node;
}

int check_node_neighbors(MAZE *maze, char **map, int x, int y){
    if(x < 0 || x >= maze->width || y < 0 || y >= maze->height)
        return FALSE;
    
    if(map[y][x] == FOREST_PATH || map[y][x] == PRINC || map[y][x] == DRAG)
        return 1;

    if(map[y][x] == SLOW_WAY)
        return 2;

    return FALSE;
}

PATH *actualize_path(PATH *path, PATH_NODE *check_node){
    int i = 0;

    while(path[i].path_root != NULL){
        if(path[i].path_root->position.x == check_node->position.x && path[i].path_root->position.y == check_node->position.y)
            return path;
        i++;
    }

    printf("actualize_path - pridavam do pola path novy ROOT PATH NODE: [%d, %d]\n", check_node->position.y, check_node->position.x);
    path[i].path_root = check_node;
    path[i].path_root->next = NULL;

    return path;
}

PATH_NODE *find_node_in_path(PATH *path, int x, int y){
    int i = 0;

    while(path[i].path_root != NULL){
        if(path[i].path_root->position.x == x && path[i].path_root->position.y == y)
            return path[i].path_root;

        i++;
    }

    return NULL;
}

PATH *create_edge(PATH *path, PATH_NODE *src, PATH_NODE *dst){
    int i = 0, counter = 0;
    PATH_NODE *current;

    /* prechadzam pole path az nakoniec (verticalne) */
    while(path[i].path_root != NULL){

        /* prechadzam pole path_root az nakoniec (horizontalne) */
        if(path[i].path_root->position.x == src->position.x && path[i].path_root->position.y == src->position.y){
            current = path[i].path_root;

            while(current->next != NULL )
                current = current->next;
            
            current->next = dst;
            dst->next = NULL;
        }

        i++;
    }

    return path;
}

MAZE *create_vertex(MAZE *maze, PATH_NODE *new_node, char **map, int index){
    PATH_NODE *neighboor_node;
    int neighboor_value, x, y, i=0;

    /* prejdi pole path a pozri sa ci uz nie je nahodou v poli path (adjance list) dany path_node ako root, ak nie pridaj ho na prvy volny index v path */    
    maze->path = actualize_path(maze->path, new_node);

    // while (maze->path[i].path_root != NULL){
    //     printf(" [%d, %d] ", maze->path[i].path_root->position.y,maze->path[i].path_root->position.x);
    //     i++;
    // }
    // printf("\n");

    x = new_node->position.x;
    y = new_node->position.y;

    /* este nieje v poli path ako root->path */
    neighboor_value = check_node_neighbors(maze, map, x + 1, y);
    if (neighboor_value > FALSE){
        printf("mam suseda na pravo od seba\n");
        neighboor_node = init_path_node(y * 10 + (x + 1), neighboor_value, y, (x + 1));
        maze->path = create_edge(maze->path, new_node, neighboor_node);
    }

    /* uz by mal byt v poli path ako root->path */
    neighboor_value = check_node_neighbors(maze, map, x - 1, y);
    if (neighboor_value > FALSE){
        printf("mam suseda na lavo od seba\n");
        neighboor_node = init_path_node(y * 10 + (x - 1), neighboor_value, y, (x - 1));
        maze->path = create_edge(maze->path, new_node, neighboor_node);
    }

    /* este nieje v poli path ako root->path */
    neighboor_value = check_node_neighbors(maze, map, x, y + 1);
    if (neighboor_value > FALSE){
        printf("mam suseda pod sebou odo mna [%d, %d]\n", y+1, x);
        neighboor_node = init_path_node((y + 1) * 10 + x, neighboor_value, (y + 1), x);
        maze->path = create_edge(maze->path, new_node, neighboor_node);
    }

    /* uz by mal byt v poli path ako root->path */
    neighboor_value = check_node_neighbors(maze, map, x, y - 1);
    if (neighboor_value > FALSE){
        printf("mam suseda nadomnou odo mna\n");
        neighboor_node = init_path_node((y - 1) * 10 + x, neighboor_value, (y - 1), x);
        maze->path = create_edge(maze->path, new_node, neighboor_node);
    }
    
    return maze;
}

MAZE *load_maze(char **mapa, MAZE *maze){
    PATH_NODE *new_node;
    int y, x, num_of_nodes = 0;

    for (y = 0; y < maze->height; y++){
        for (x = 0; x < maze->width; x++){
            if(mapa[y][x] == FOREST_PATH){
                new_node = init_path_node(y * 10 + x, FOREST_PATH_VALUE, y, x);
                maze = create_vertex(maze, new_node, mapa, num_of_nodes);
                num_of_nodes++;
            }
            else if(mapa[y][x] == SLOW_WAY){

            }
        }
    }

    return maze;
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty){
    int i = 0;
    PATH_NODE *current = NULL;

    MAZE *maze = init_maze(m, n);
    maze = load_maze(mapa, maze);

    printf("----- Konecny vypis grafu!! -----\n");
    while (maze->path[i].path_root != NULL){
        current = maze->path[i].path_root;
        while(current->next != NULL ){
            printf(" [%d, %d] ->", current->position.y, current->position.x);
            current = current->next;
        }
        printf(" [%d, %d]\n", current->position.y, current->position.x);

        i++;
    }

    return (int *)2;
}
