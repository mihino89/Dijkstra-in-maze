#include <stdio.h>
#include <stdlib.h> 

#ifndef GRAPH_H
#define GRAPH_H

#define FOREST_PATH 'C'
#define SLOW_WAY 'H'
#define NO_WAY 'N'
#define DRAG 'D'
#define PRINC 'P'

#define TRUE	1
#define FALSE	0

#define FOREST_PATH_VALUE 1
#define SLOW_PATH_VALUE   2
#define PRINCESS_HELPER   3

#define MAX_NUM_OF_PRINCESS 5
#define INFINITY        10000

typedef struct coordinates_{
    int x;
    int y;
} COORDINATES;


/* root in adjacency list */
typedef struct path_node{
    int id;
    int cost;
    COORDINATES position;
    struct path_node *next;
} PATH_NODE;


typedef struct dragones_{
    int t;
    int index;
    COORDINATES position;
} DRAGON;


/* Path in adjacency list */
typedef struct path{
    int cost;
    int src_path_root;
    int known;
    PATH_NODE *path_root;
} PATH;


typedef struct princess_rescue{
    int t;
    int *princess_indexes;
} PRINCESS_RESCUE;


typedef struct maze{
    int width;
    int height;
    short princess_num;
    int nodes_num;
    int *princess_index_arr;
    DRAGON *dragon;
    PRINCESS_RESCUE *princess_rescue;
    PATH *path;
} MAZE;

PATH *init_path(int maze_width, int maze_height);
MAZE *init_maze(int maze_width, int maze_height);
PATH_NODE *init_path_node(int id, int path_node_cost, int pos_y, int pos_x);
DRAGON *init_dragon(int t, int index, int pos_y, int pos_x);

MAZE *actualize_path(MAZE *maze, PATH_NODE *check_node, int index);
int check_node_neighbors(MAZE *maze, char **map, int x, int y);
MAZE *add_neighboor_to_path(MAZE *maze, PATH_NODE *new_node, int neighboor_value, int index, int y, int x);

MAZE *create_edge(MAZE *maze, PATH_NODE *src, PATH_NODE *dst, int index);
MAZE *create_vertex(MAZE *maze, PATH_NODE *new_node, char **map, int index);
MAZE *load_maze(MAZE *maze, char **mapa, int t);

#endif /* GRAPH_H */

int **zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty);
