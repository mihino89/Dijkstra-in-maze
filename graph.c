#include "graph.h"
#include "dijkstra.c"


SOURCE_PATH *init_source_path(int size){
    SOURCE_PATH *source_path = (SOURCE_PATH *)malloc(sizeof(SOURCE_PATH));

    source_path->index_of_src_path_root = -1;
    source_path->num_of_src_path_nodes = 0;

    return source_path;
}

PATH *init_path(int maze_width, int maze_height){
    PATH *path = (PATH *)malloc((maze_width * maze_height) * sizeof(PATH));

    for (int i = 0; i < maze_width * maze_height; i++){
        path[i].path_root = NULL;
        path[i].cost = INFINITY;
        path[i].known = FALSE;
        path[i].source_path = init_source_path(maze_width * maze_height);
    }

    return path;
}


int *init_princess_index_arr(){
    int *arr = (int *)malloc(MAX_NUM_OF_PRINCESS * sizeof(int));

    for (int i = 0; i < MAX_NUM_OF_PRINCESS; i++)
        arr[i] = -1;

    return arr;
}

MAZE *init_maze(int maze_width, int maze_height){
    MAZE *maze = (MAZE *)malloc(sizeof(MAZE));
    maze->width = maze_width;
    maze->height = maze_height;

    maze->princess_num = maze->nodes_num = 0;
    maze->princess_index_arr = init_princess_index_arr();

    maze->path = init_path(maze_width, maze_height);
    maze->dragon = NULL;

    return maze;
}


void re_init_maze(MAZE *maze){
    for (int i = 0; i < maze->nodes_num; i++){
        maze->path[i].known = FALSE;
        maze->path[i].cost = INFINITY;
        maze->path[i].source_path->index_of_src_path_root = -1;
        maze->path[i].source_path->num_of_src_path_nodes = 0;
    }
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


DRAGON *init_dragon(int t, int index, int pos_y, int pos_x){
    DRAGON *init_dragon = (DRAGON *)malloc(sizeof(DRAGON));

    init_dragon->t = t;
    init_dragon->index = index;
    init_dragon->position.y = pos_y;
    init_dragon->position.x = pos_x;

    return init_dragon;
}


void init_princess_rescue_permutations(MAZE *maze, int permutations){
    maze->princess_rescue = (PRINCESS_RESCUE *)malloc(permutations * sizeof(PRINCESS_RESCUE));
}


MAZE *init_princess_rescue(MAZE *maze, int arr[], int size, int index){
    maze->princess_rescue[index].permutation_of_princess_indexes = (int *)malloc(size * sizeof(int));
    maze->princess_rescue[index].rescue_path_of_princess_permutation = (int **)malloc((maze->width * maze->height * 4) * sizeof(int*));
    maze->princess_rescue[index].num_princess_rescue_path = 0;

    for (int i = 0; i < size; i++)
        maze->princess_rescue[index].permutation_of_princess_indexes[i] = arr[i];
    
    maze->princess_rescue[index].t = 0;

    return maze;
}

/* prida novy node do pola ako path root */
MAZE *actualize_path(MAZE *maze, PATH_NODE *check_node, int index){
    maze->path[index].path_root = check_node;
    maze->path[index].path_root->next = NULL;

    return maze;
}


int check_node_neighbors(MAZE *maze, char **map, int x, int y){
    if(x < 0 || x >= maze->width || y < 0 || y >= maze->height)
        return FALSE;
    
    if(map[y][x] == FOREST_PATH || map[y][x] == DRAG)
        return FOREST_PATH_VALUE;

    if(map[y][x] == SLOW_WAY)
        return SLOW_PATH_VALUE;

    if(map[y][x] == PRINC){
        return PRINCESS_HELPER;
    }

    return FALSE;
}


MAZE *create_edge(MAZE *maze, PATH_NODE *src, PATH_NODE *dst, int index){
    int i = 0;
    PATH_NODE *current = maze->path[index].path_root;

    /* prechadzam pole path az nakoniec (verticalne) */
    while(current->next != NULL )
        current = current->next;
    
    current->next = dst;

    return maze;

}


MAZE *add_neighboor_to_path(MAZE *maze, PATH_NODE *new_node, int neighboor_value, int index, int y, int x){
    if(neighboor_value <= FALSE){
        return maze;
    }
    
    PATH_NODE *neighboor_node;

    if(neighboor_value == PRINCESS_HELPER)
        neighboor_node = init_path_node(y * 10 + x, neighboor_value-SLOW_PATH_VALUE, y, x);

    else
        neighboor_node = init_path_node(y * 10 + x, neighboor_value, y, x);
    
    maze = create_edge(maze, new_node, neighboor_node, index);
    return maze;
}


MAZE *create_vertex(MAZE *maze, PATH_NODE *new_node, char **map, int index){
    PATH_NODE *neighboor_node;
    PATH_NODE *current = NULL;
    int neighboor_value, x, y;

    /* pridaj novy path root na prvy volny index v path */  
    maze = actualize_path(maze, new_node, index);

    x = new_node->position.x;
    y = new_node->position.y;

    /* smer - Vychod */
    neighboor_value = check_node_neighbors(maze, map, x + 1, y);
    maze = add_neighboor_to_path(maze, new_node, neighboor_value, index, y, (x + 1));

    /* smer - Zapad */
    neighboor_value = check_node_neighbors(maze, map, x - 1, y);
    maze = add_neighboor_to_path(maze, new_node, neighboor_value, index, y, (x - 1));
  
    /* smer - Sever */
    neighboor_value = check_node_neighbors(maze, map, x, y + 1);
    maze = add_neighboor_to_path(maze, new_node, neighboor_value, index, (y + 1), x);
   
    /* smer - Juh */
    neighboor_value = check_node_neighbors(maze, map, x, y - 1);
    maze = add_neighboor_to_path(maze, new_node, neighboor_value, index, (y - 1), x);

    return maze;
}


MAZE *load_maze(MAZE *maze, char **mapa, int t){
    PATH_NODE *new_node;
    DRAGON *drak;
    int y, x;

    for (y = 0; y < maze->height; y++){
        for (x = 0; x < maze->width; x++){
            if (mapa[y][x] == FOREST_PATH || mapa[y][x] == SLOW_WAY || mapa[y][x] == PRINC || mapa[y][x] == DRAG){

                if(mapa[y][x] == FOREST_PATH)
                    new_node = init_path_node(y * 10 + x, FOREST_PATH_VALUE, y, x);
                
                else if(mapa[y][x] == PRINC){
                    new_node = init_path_node(y * 10 + x, FOREST_PATH_VALUE, y, x);
                    maze->princess_index_arr[maze->princess_num++] = maze->nodes_num;
                }
                else if(mapa[y][x] == DRAG){
                    drak = init_dragon(t, maze->nodes_num, y, x);
                    maze->dragon = drak;
                    new_node = init_path_node(y * 10 + x, FOREST_PATH_VALUE, y, x);
                }
                else
                    new_node = init_path_node(y * 10 + x, SLOW_PATH_VALUE, y, x);

                maze = create_vertex(maze, new_node, mapa, maze->nodes_num);

                maze->nodes_num++;
            }
        }
    }

    return maze;
}


int **kil_the_dragon(MAZE *maze, int *dlzka_cesty){
    int indexer = maze->total_path_lengt = maze->path[maze->dragon->index].source_path->num_of_src_path_nodes;

    int path_node_index = maze->dragon->index, counter = 0;
    int **dragon_path = (int **)malloc((maze->width * maze->height) * sizeof(int *));
    int dragon_index;

    PATH_NODE *root_path_node = maze->path[path_node_index].path_root;
    *dlzka_cesty = maze->path[path_node_index].cost;

    while(--indexer >= 0){
        dragon_path[indexer] = (int *)malloc(2 * sizeof(int));
        dragon_path[indexer][0] = root_path_node->position.y;
        dragon_path[indexer][1] = root_path_node->position.x;

        // printf("pocet policok potrebnych k drakovi: %d %d %d\n", indexer, dragon_path[indexer][0], dragon_path[indexer][1]);

        path_node_index = maze->path[path_node_index].source_path->index_of_src_path_root;
        root_path_node = maze->path[path_node_index].path_root;
    }

    // dragon_path[counter] = (int *)malloc(2 * sizeof(int));
    // dragon_path[counter][0] = maze->path[0].path_root->position.y;
    // dragon_path[counter][1] = maze->path[0].path_root->position.x;

    return dragon_path;
}


void actualize_princess_rescue_path(MAZE *maze, int starting_index, int ending_index, int princess_rescue_index){
    int indexer = maze->path[ending_index].source_path->num_of_src_path_nodes;
    int index_rescue_perm;
    // printf("pocet policok k princeznej doteraz: %d potrebne pre tento usek dalsich: %d\n", maze->princess_rescue[princess_rescue_index].num_princess_rescue_path, maze->path[ending_index].source_path->num_of_src_path_nodes);
    PATH_NODE *root_path_node = maze->path[ending_index].path_root;

    int i = maze->princess_rescue[princess_rescue_index].num_princess_rescue_path;

    while (root_path_node->id != maze->path[starting_index].path_root->id){
        index_rescue_perm = indexer + i - 2;

        maze->princess_rescue[princess_rescue_index].rescue_path_of_princess_permutation[index_rescue_perm] = (int *)malloc(2 * sizeof(int));
        maze->princess_rescue[princess_rescue_index].rescue_path_of_princess_permutation[index_rescue_perm][0] = root_path_node->position.y;
        maze->princess_rescue[princess_rescue_index].rescue_path_of_princess_permutation[index_rescue_perm][1] = root_path_node->position.x;

        ending_index = maze->path[ending_index].source_path->index_of_src_path_root;
        root_path_node = maze->path[ending_index].path_root;
        maze->princess_rescue[princess_rescue_index].num_princess_rescue_path++;
        i--;
    }

    // index_rescue_perm = indexer + i - 1;
    // // printf("ukladam na index: %d\n", index_rescue_perm);

    // maze->princess_rescue[princess_rescue_index].rescue_path_of_princess_permutation[index_rescue_perm] = (int *)malloc(2 * sizeof(int));
    // maze->princess_rescue[princess_rescue_index].rescue_path_of_princess_permutation[index_rescue_perm][0] = root_path_node->position.y;
    // maze->princess_rescue[princess_rescue_index].rescue_path_of_princess_permutation[index_rescue_perm][1] = root_path_node->position.x;
    // maze->princess_rescue[princess_rescue_index].num_princess_rescue_path++;
}


/* function  return all posible permutations of princess number in maze (count factorial) */
void get_number_of_princess_permutations(int *princess_num){
    int loop_max = *princess_num;
    for (int i = 2; i < loop_max; i++)
        *princess_num *= i;
}


void swappiness(int *a, int *b){
    int temp = *b;
    *b = *a;
    *a = temp;
}

// Generating permutation using Heap Algorithm - zdroj: https://www.geeksforgeeks.org/heaps-algorithm-for-generating-permutations/
void heapPermutation(MAZE *maze, int a[], int size, int index) {
    int static je = 0;
    // if size becomes 1 then prints the obtained
    // permutation
    if (size == 1) {
        maze = init_princess_rescue(maze, a, maze->princess_num, je++);
        return; 
    } 
  
    for (int i=0; i<size; i++) { 
        heapPermutation(maze, a, size-1, index); 
  
        // if size is odd, swap first and last 
        // element 
        if (size%2==1) 
            swappiness(&a[0], &a[size-1]); 
  
        // If size is even, swap ith and last 
        // element 
        else
            swappiness(&a[i], &a[size-1]); 
    } 
} 


void print_graph(MAZE *maze){
    int i;
    PATH_NODE *current = NULL;

    printf("----- Vypis grafu!! -----\n");
    printf("Number of nodes in path: %d \n", maze->nodes_num);
    printf("Number of princess: %d \n", maze->princess_num);
    printf("Drak coordinates: [%d, %d] and his index in path: %d\n\n", maze->dragon->position.y, maze->dragon->position.x, maze->dragon->index);

    for (i = 0; i < maze->nodes_num; i++){
        printf("i:%d id: %d path cost: %d known: %d src path node: %d source nodes %d,  ", i, maze->path[i].path_root->id, maze->path[i].cost, maze->path[i].known, maze->path[i].source_path->index_of_src_path_root, maze->path[i].source_path->num_of_src_path_nodes);
        
        current = maze->path[i].path_root;
        while(current->next != NULL ){
            printf(" [%d, %d] ->", current->position.y, current->position.x);
            current = current->next;
        }
        printf(" [%d, %d]\n", current->position.y, current->position.x);
    }
}


/* function printing all posible permutations of finding princess, moreover their cost and path */
void print_princess_rescue_permutation(MAZE *maze, int factorial) {
    printf("\n----- Permutacie postupu hladania princezien -----\n");
    for (int i = 0; i < factorial; i++){
        printf("perm %d cost %d: ", i, maze->princess_rescue[i].t);
        for (int j = 0; j < maze->princess_num; j++)
            printf("%d ", maze->princess_rescue[i].permutation_of_princess_indexes[j]);
        printf("perm path: %d", maze->princess_rescue[i].num_princess_rescue_path);
        for (int j = 0; j < maze->princess_rescue[i].num_princess_rescue_path - 1; j++){
            printf("[%d, %d] ", maze->princess_rescue[i].rescue_path_of_princess_permutation[j][0], maze->princess_rescue[i].rescue_path_of_princess_permutation[j][1]);
        }
        printf("\n");
    }
    printf("\n");
}


void print_final_path(MAZE *maze, int *final_array){
    printf("\n----- TOTAL FINAL ARRAY -----\n");
    for (int d = 0; d <= maze->total_path_lengt; d++)
        printf("[%d, %d]\n", final_array[2*d+1], final_array[2*d]);
    printf("\n");

}


/* funkcia vrati index najlacnejsej cesty spomedzi permutacii */
int cheapest_princess_rescue_path(MAZE *maze, int fatorial, int *dlzka_cesty){
    int min, min_index, i;

    min = maze->princess_rescue[0].t;
    min_index = 0;

    for (int i = 1; i < fatorial; i++){
        if(maze->princess_rescue[i].t < min){
            min = maze->princess_rescue[i].t;
            min_index = i;
        }
    }
    *dlzka_cesty += maze->princess_rescue[min_index].t;

    return min_index;
}


void free_dragon_path(MAZE *maze, int **dragon_path){

    for (int i = 0; i < (maze->width * maze->height); i++){
        for (int j = 0; j < 2; j++)
            free(&dragon_path[i][j]);
        
    }
}


int *create_and_connect_final_path(MAZE *maze, int **path, int index_of_cheapest_path){
    int *final_array = (int *)malloc((2*maze->total_path_lengt) + 1 * sizeof(int));
    
    for (int d = 0; d < maze->total_path_lengt; d++){
        if(d <= (maze->total_path_lengt - maze->princess_rescue[index_of_cheapest_path].num_princess_rescue_path)){
            final_array[2 * d] = path[d][0];
            final_array[2 * d + 1] = path[d][1];
        } else {
            final_array[2 * d] = maze->princess_rescue[index_of_cheapest_path].rescue_path_of_princess_permutation
            [d - (maze->total_path_lengt - maze->princess_rescue[index_of_cheapest_path].num_princess_rescue_path + 1)][0];
            final_array[2 * d + 1] = maze->princess_rescue[index_of_cheapest_path].rescue_path_of_princess_permutation
            [d - (maze->total_path_lengt - maze->princess_rescue[index_of_cheapest_path].num_princess_rescue_path + 1)][1];
        }
    }
    final_array[2*maze->total_path_lengt] = -1;

    return final_array;
}


void free_maze(MAZE *maze, int factorial){
    PATH_NODE *current, *previous;

    free(maze->princess_index_arr);
    free(maze->dragon);

    for (int i = 0; i < factorial; i++){
        free(maze->princess_rescue[i].permutation_of_princess_indexes);
        for (int j = 0; j < (maze->width * maze->height * 4); j++)
            free(maze->princess_rescue[i].rescue_path_of_princess_permutation[j]);
        
        free(maze->princess_rescue[i].rescue_path_of_princess_permutation);
    }

    for (int i = 0; i < (maze->width * maze->height); i++){
        current = maze->path[i].path_root != NULL ? maze->path[i].path_root->next : maze->path[i].path_root;

        while(current != NULL){
            previous = current;
            current = current->next;
            free(previous);
        }

        free(current);
        free(maze->path[i].source_path);
    }
    free(maze->path);

    free(maze->princess_rescue);
    free(maze);
}


int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty){
    int starting_index, factorial, index_of_cheapest_path;
    int *final_array;
    int **path;

    starting_index = 0;

    MAZE *maze = init_maze(m, n);
    maze = load_maze(maze, mapa, t);

    maze = dijkstra(maze, starting_index);
    path = kil_the_dragon(maze, dlzka_cesty);

    /**
     * kontrola cas t najdenej cesty, ak je mensi rovny ako t pokracuj v zachrane princezien
     * a teda znamena to, ze som stihol zachranit princeznu v cas limite
    */
    if(*dlzka_cesty > t)
        return NULL;

    /**
     * starting index je index draka, zisti permutacie a vykonaj a porovnaj ich
    */    
    starting_index = maze->dragon->index;
    factorial = maze->princess_num;

    /* Vypocet moznych roznych permutacii navstivenia princezien (faktorial z poctu princezien) */
    get_number_of_princess_permutations(&factorial);
    init_princess_rescue_permutations(maze, factorial);

    /* Vytvorenie roznych permutacii navstivenia princezien podla algoritmu Heap Algorithm (ciastocne prebrany - zdroj pri fn)*/
    heapPermutation(maze, maze->princess_index_arr, maze->princess_num, 0);

    /* prechadzam polia moznych permutacii hladania princezien */ 
    for (int m = 0; m < factorial; m++){
        
        /* prechadzam jednotlivo danu permutaciu */
        for (int n = 0; n < maze->princess_num; n++){

            /* Precistujem bludisko a pripravujem na novu dijsktru */
            re_init_maze(maze);

            /* Prva cesta je vzdy od draka k prvej permutacii princeznej */
            maze = dijkstra(maze, starting_index);

            /* aktualizujem cestu permutacie */
            actualize_princess_rescue_path(maze, starting_index, maze->princess_rescue[m].permutation_of_princess_indexes[n], m);

            /* nastavim novy start index na dalsiu dijkstru */
            starting_index = maze->princess_rescue[m].permutation_of_princess_indexes[n];
            
            /* zapocitam cas potrebny na najdenie hladanej princeznej */
            maze->princess_rescue[m].t += maze->path[starting_index].cost;
        }

        /* start index znovu nastavim na index draka v grafe */
        starting_index = maze->dragon->index;
    }

    /* Najde najlacnejsiu cestu spomedzi permutacii */
    index_of_cheapest_path = cheapest_princess_rescue_path(maze, factorial, dlzka_cesty);

    /* K zatial docastnej casovej narocnosti cesty k drakovi pripocitava casovu narocnost hladania princezien */
    maze->total_path_lengt += (maze->princess_rescue[index_of_cheapest_path].num_princess_rescue_path - 1);

    /* Spoji pole pre cestu k drakovi a hladania princezien */
    final_array = create_and_connect_final_path(maze, path, index_of_cheapest_path);

    /* functions for printing purposes and checks */
    // print_graph(maze);
    // print_final_path(maze, final_array);
    // print_princess_rescue_permutation(maze, factorial);

    free_maze(maze, factorial);
    return final_array;
}
