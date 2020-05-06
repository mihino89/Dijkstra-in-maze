#include "graph.h"
#include "binary_heap.h"

void init_starting_vertex(MAZE *maze, int index);

void set_known_root_node(MAZE *maze, int index);
void change_path_cost(MAZE *maze, PATH_NODE *path_node, HEAP *heap, int root_path_cost, int root_path_index);
void find_and_update_neighboor(MAZE *maze, PATH_NODE *current_path_node, HEAP *heap, int root_path_cost, int root_path_index);
MAZE *dijkstra(MAZE *maze, int starting_index);