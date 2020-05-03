#include <stdio.h>

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
    PATH_NODE *path_node;
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


int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty);