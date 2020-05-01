#include <stdio.h>

#define FOREST_PATH 'C'
#define SLOW_WAY 'H'
#define NO_WAY 'N'
#define DRAG 'D'
#define PRINC 'P'

#define TRUE	1
#define FALSE	0

typedef struct coordinates_{
    int x;
    int y;
} COORDINATES;


typedef struct dragones_{
    COORDINATES *position;
} DRAGON;


typedef struct princess_{
    COORDINATES *position;
} PRINCESS;


/* root in adjacency list */
typedef struct path_node{
    int id;
    int cost;
    int known;
    COORDINATES *position;
    struct path_node *next;
} PATH_NODE;


/* Path in adjacency list */
typedef struct path{
    int cost;
    PATH_NODE *path_root;
} PATH;


typedef struct maze{
    int width;
    int height;
    short princess_num;
    DRAGON *dragon;
    PRINCESS *princess;
    PATH *path;
} MAZE;


int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty);