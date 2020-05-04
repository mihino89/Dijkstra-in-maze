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
    int known;
    int is_princess;
    COORDINATES position;
    struct path_node *next;
} PATH_NODE;


typedef struct dragones_{
    int t;
    PATH_NODE path_node;
} DRAGON;


/* Path in adjacency list */
typedef struct path{
    int cost;
    PATH_NODE *path_root;
} PATH;


typedef struct maze{
    int width;
    int height;
    short princess_num;
    int nodes_num;
    DRAGON *dragon;
    // PATH_NODE *princess;
    PATH *path;
} MAZE;

PATH *init_path(int maze_width, int maze_height);
MAZE *init_maze(int maze_width, int maze_height);
PATH_NODE *init_path_node(int id, int path_node_cost, int pos_y, int pos_x, int is_princess);
DRAGON *init_dragon(int id, int pos_y, int pos_x, int t);

MAZE *actualize_path(MAZE *maze, PATH_NODE *check_node, int index);
int check_node_neighbors(MAZE *maze, char **map, int x, int y);
MAZE *add_neighboor_to_path(MAZE *maze, PATH_NODE *new_node, int neighboor_value, int index, int y, int x);

MAZE *create_edge(MAZE *maze, PATH_NODE *src, PATH_NODE *dst, int index);
MAZE *create_vertex(MAZE *maze, PATH_NODE *new_node, char **map, int index);
MAZE *load_maze(MAZE *maze, char **mapa, int t);

#endif /* GRAPH_H */

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty);
