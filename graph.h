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

#define DRAGON_DIJKSTRA     1
#define PRINCESS_DIJKSTRA   2

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


typedef struct source_path{
    int index_of_src_path_root;
    int num_of_src_path_nodes;
} SOURCE_PATH;


/* Path in adjacency list */
typedef struct path{
    int cost;
    int known;
    SOURCE_PATH *source_path;
    // int *src_path_nodes;
    PATH_NODE *path_root;
} PATH;


typedef struct princess_rescue{
    int t;
    int *permutation_of_princess_indexes;
    int **rescue_path_of_princess_permutation;
    int num_princess_rescue_path;
} PRINCESS_RESCUE;


typedef struct maze{
    int width;
    int height;
    short princess_num;
    int nodes_num;
    int *princess_index_arr;
    int total_path_lengt;
    DRAGON *dragon;
    PRINCESS_RESCUE *princess_rescue;
    PATH *path;
} MAZE;

SOURCE_PATH *init_source_path(int size);
PATH *init_path(int maze_width, int maze_height);
PATH_NODE *init_path_node(int id, int path_node_cost, int pos_y, int pos_x);
DRAGON *init_dragon(int t, int index, int pos_y, int pos_x);

MAZE *init_maze(int maze_width, int maze_height);
void re_init_maze(MAZE *maze);

int *init_princess_index_arr();
void init_princess_rescue_permutations(MAZE *maze, int permutations);
MAZE *init_princess_rescue(MAZE *maze, int arr[], int size, int index);

void print_graph(MAZE *maze);
void print_princess_rescue_permutation(MAZE *maze, int factorial);

void free_dragon_path(MAZE *maze, int **dragon_path);
void free_maze(MAZE *maze, int factorial, int *total_time);

void get_number_of_princess_permutations(int *princess_num);
void heapPermutation(MAZE *maze, int a[], int size, int index);
void swappiness(int *a, int *b);

MAZE *actualize_path(MAZE *maze, PATH_NODE *check_node, int index);
int check_node_neighbors(MAZE *maze, char **map, int x, int y);
MAZE *add_neighboor_to_path(MAZE *maze, PATH_NODE *new_node, int neighboor_value, int index, int y, int x);

MAZE *create_edge(MAZE *maze, PATH_NODE *src, PATH_NODE *dst, int index);
MAZE *create_vertex(MAZE *maze, PATH_NODE *new_node, char **map, int index);
MAZE *load_maze(MAZE *maze, char **mapa, int t);

int **kil_the_dragon(MAZE *maze, int *dlzka_cesty);
void actualize_princess_rescue_path(MAZE *maze, int starting_index, int ending_index, int princess_rescue_index);
int cheapest_princess_rescue_path(MAZE *maze, int fatorial, int *dlzka_cesty);

int *create_and_connect_final_path(MAZE *maze, int **path, int index_of_cheapest_path);
int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty);
#endif /* GRAPH_H */
