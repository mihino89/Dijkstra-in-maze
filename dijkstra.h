#include "graph.h"
#include "binary_heap.h"

void init_starting_vertex(MAZE *maze, int index);

void set_known_root_node(MAZE *maze, int index);
void change_path_cost(MAZE *maze, PATH_NODE *path_node, HEAP *heap, int root_path_cost, int root_path_index);
void find_and_update_neighboor(MAZE *maze, HEAP *heap, int index);
int check_and_update_src_paths(MAZE *maze, int starting_index);
int dijkstra(MAZE *maze, int starting_index);