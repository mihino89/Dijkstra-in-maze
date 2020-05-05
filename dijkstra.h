#include "graph.h"
#include "binary_heap.h"

MAZE *init_starting_vertex(MAZE *maze, int index);

MAZE *set_known_root_node(MAZE *maze, int index);
MAZE *change_path_cost(MAZE *maze, PATH_NODE *path_node, HEAP *heap, int root_path_cost);
MAZE *find_and_update_neighboor(MAZE *maze, PATH_NODE *current_path_node, HEAP *heap, int cost);
MAZE *dijkstra(MAZE *maze);