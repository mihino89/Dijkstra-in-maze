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


void print_graph(MAZE *maze){
    int i;
    PATH_NODE *current = NULL;

    printf("----- Vypis grafu!! -----\n");
    printf("Number of nodes in path: %d \n", maze->nodes_num);
    printf("Number of princess: %d \n", maze->princess_num);
    printf("Drak coordinates: [%d, %d] and his index in path: %d\n\n", maze->dragon->position.x, maze->dragon->position.y, maze->dragon->index);


    for (i = 0; i < maze->princess_num; i++){
        printf("Princezna %d: [%d, %d]\n",i, maze->path[maze->princess_index_arr[i]].path_root->position.x, maze->path[maze->princess_index_arr[i]].path_root->position.y);
    }

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


void print_final_rescue_princess_path(MAZE *maze, int index_of_cheapest_path){
    int index_first_princess = maze->princess_rescue[index_of_cheapest_path].permutation_of_princess_indexes[0];

    printf("Cesta od draka [%d, %d], k prvej princezne [%d, %d]\n", 
        maze->path[maze->dragon->index].path_root->position.x, 
        maze->path[maze->dragon->index].path_root->position.y, 
        maze->path[index_first_princess].path_root->position.x, 
        maze->path[index_first_princess].path_root->position.y
    );

    for (int i = 1; i < maze->princess_num; i++){
        printf("Cesta od %d. princezny [%d, %d] ", 
            i,
            maze->path[index_first_princess].path_root->position.x, 
            maze->path[index_first_princess].path_root->position.y
        );
        index_first_princess = maze->princess_rescue[index_of_cheapest_path].permutation_of_princess_indexes[i];
        printf("k %d. princezne [%d, %d]\n", 
            i+1,
            maze->path[index_first_princess].path_root->position.x, 
            maze->path[index_first_princess].path_root->position.y
        );
    }
}


/* function printing all posible permutations of finding princess, moreover their cost and path */
void print_princess_rescue_permutation(MAZE *maze, int factorial){
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


/* fn to free two dimensional array in dragon path */
void free_dragon_path(MAZE *maze, int **dragon_path){
    int i;

    for (i = 0; i < (maze->width * maze->height); i++)
        free(dragon_path[i]);
    free(dragon_path);
}


/* Funkcia sa spusti na konci programu a uvolnim pamat pre struct maze */
void free_maze(MAZE *maze, int factorial, int *total_time){
    PATH_NODE *current, *previous;
    int i;

    free(maze->princess_index_arr);
    free(maze->dragon);

    for (i = 0; i < factorial; i++){
        free(maze->princess_rescue[i].permutation_of_princess_indexes);
        for (int j = 0; j < (maze->width * maze->height * 4); j++)
            free(maze->princess_rescue[i].rescue_path_of_princess_permutation[j]);
        
        free(maze->princess_rescue[i].rescue_path_of_princess_permutation);
    }

    for (i = 0; i < (maze->width * maze->height); i++){
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
    free(total_time);

    free(maze->princess_rescue);
    free(maze);
}


/* function  return all posible permutations of princess number in maze (count factorial) */
void get_number_of_princess_permutations(int *princess_num){
    int i, loop_max = *princess_num;
    for (i = 2; i < loop_max; i++)
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


/* prida novy node do pola ako path root */
MAZE *actualize_path(MAZE *maze, PATH_NODE *check_node, int index){
    maze->path[index].path_root = check_node;
    maze->path[index].path_root->next = NULL;

    return maze;
}


/* Funkcia sa pozrie ci existuje cesta, ak ano vrati hodnotu cesty */
int check_node_neighbors(MAZE *maze, char **map, int x, int y){
    if(x < 0 || x >= maze->width || y < 0 || y >= maze->height)
        return FALSE;
    
    if(map[y][x] == FOREST_PATH || map[y][x] == DRAG || map[y][x] == PRINC)
        return FOREST_PATH_VALUE;

    if(map[y][x] == SLOW_WAY)
        return SLOW_PATH_VALUE;

    return FALSE;
}


/* Funkcia, ktora prepoji path root v poli so susednym nodeom */
MAZE *create_edge(MAZE *maze, PATH_NODE *src, PATH_NODE *dst, int index){
    PATH_NODE *current = maze->path[index].path_root;

    /* prechadzam pole path az nakoniec (verticalne) */
    while(current->next != NULL )
        current = current->next;
    
    current->next = dst;

    return maze;
}


/* Funkcia inituje suseda a vytvori hranu medzi nim a src (susednym) nodeom */
MAZE *add_neighboor_to_path(MAZE *maze, PATH_NODE *new_node, int neighboor_value, int index, int y, int x){
    if(neighboor_value <= FALSE){
        return maze;
    }
    
    PATH_NODE *neighboor_node;

    neighboor_node = init_path_node((y * 100) + x, neighboor_value, y, x);
    
    maze = create_edge(maze, new_node, neighboor_node, index);
    return maze;
}


/* Fn na vytvorenie noveho vrcholu s kontrolou ci existuju jeho susedia */
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


/* Fn for loading maze as a struct in the begin of the program/algorithm */
MAZE *load_maze(MAZE *maze, char **mapa, int t){
    PATH_NODE *new_node;
    DRAGON *drak;
    int y, x;

    for (y = 0; y < maze->height; y++){
        for (x = 0; x < maze->width; x++){
            if (mapa[y][x] == FOREST_PATH || mapa[y][x] == SLOW_WAY || mapa[y][x] == PRINC || mapa[y][x] == DRAG){
                
                new_node = init_path_node(y * 100 + x, mapa[y][x] == SLOW_WAY ? SLOW_PATH_VALUE : FOREST_PATH_VALUE, y, x);
                
                /* Saving index of the princess */
                if(mapa[y][x] == PRINC){
                    maze->princess_index_arr[maze->princess_num++] = maze->nodes_num;
                }

                /* Saving dragon as a struct */
                else if(mapa[y][x] == DRAG){
                    drak = init_dragon(t, maze->nodes_num, y, x);
                    maze->dragon = drak;
                }

                maze = create_vertex(maze, new_node, mapa, maze->nodes_num);
                maze->nodes_num++;
            }
        }
    }

    return maze;
}


/* Fn for order coordinates of cheapest path to dragon from dijsktra algorithm */
int **kil_the_dragon(MAZE *maze, int *total_time){
    int num_of_src_nodes, src_node_index;
    int **dragon_path;

    dragon_path = (int **)malloc((maze->width * maze->height) * sizeof(int *));

    /**
     * num of src nodes = pocet krokov ktore potrebuje urobit od src node k destination node
     * aktualizacia poctu krokov ktore je potrebne spravit pre dosiahnutie cieloveho node-u
    */
    num_of_src_nodes = maze->total_path_lengt = maze->path[maze->dragon->index].source_path->num_of_src_path_nodes;

    /* src index na zaciatku bude index draka v grafe */
    src_node_index = maze->dragon->index;
    PATH_NODE *root_path_node = maze->path[src_node_index].path_root;

    /* k celkovemu casu pripocitam hodnotu prveho policka v mape, ktore nie je zapocitane v dst node coste */
    *total_time = maze->path[src_node_index].cost + maze->path[0].path_root->cost;
    printf("\nDrak je na suradnici [%d, %d], dlzka cesty k nemu je: %d s casom : %d\n", maze->path[maze->dragon->index].path_root->position.x, maze->path[maze->dragon->index].path_root->position.y, maze->path[maze->dragon->index].source_path->num_of_src_path_nodes, *total_time);

    while(--num_of_src_nodes >= 0){
        dragon_path[num_of_src_nodes] = (int *)malloc(2 * sizeof(int));
        dragon_path[num_of_src_nodes][0] = root_path_node->position.y;
        dragon_path[num_of_src_nodes][1] = root_path_node->position.x;
        
        /* Najdem index src node-u a zmenim root_path_node na src node */
        src_node_index = maze->path[src_node_index].source_path->index_of_src_path_root;
        root_path_node = maze->path[src_node_index].path_root;
    }

    return dragon_path;
}


/* Fn for order coordinates of cheapest path to princess from dijsktra algorithm */
void actualize_princess_rescue_path(MAZE *maze, int starting_index, int ending_index, int princess_rescue_index){

    /* pocet krokov ktore potrebuje urobit od src node k destination node */
    int num_of_src_nodes = maze->path[ending_index].source_path->num_of_src_path_nodes;
    int act_index;

    /* destination node */
    PATH_NODE *root_path_node = maze->path[ending_index].path_root;

    /* aktualny celkovy index v poli rescue path of princess - urcuje kolko krokov som potreboval doteraz spravit pre zahranu princezien */
    int total_steps_in_rescue_princess_path = maze->princess_rescue[princess_rescue_index].num_princess_rescue_path;
    // printf("pocet policok k princeznej doteraz: %d potrebne pre tento usek dalsich: %d, id: %d\n", total_steps_in_rescue_princess_path, num_of_src_nodes, root_path_node->id);

    /* 
    * Skacem na src node, kym sa nedostanem na zaciatocny node, kde moja cesta zacala 
    * postupne cestu ukladam do pola avsak v preusporiadanom poradi src->dst
    */
    while (root_path_node->id != maze->path[starting_index].path_root->id){
        act_index = num_of_src_nodes + total_steps_in_rescue_princess_path - 2;

        maze->princess_rescue[princess_rescue_index].rescue_path_of_princess_permutation[act_index] = (int *)malloc(2 * sizeof(int));
        maze->princess_rescue[princess_rescue_index].rescue_path_of_princess_permutation[act_index][0] = root_path_node->position.y;
        maze->princess_rescue[princess_rescue_index].rescue_path_of_princess_permutation[act_index][1] = root_path_node->position.x;

        ending_index = maze->path[ending_index].source_path->index_of_src_path_root;
        root_path_node = maze->path[ending_index].path_root;
        maze->princess_rescue[princess_rescue_index].num_princess_rescue_path++;
        num_of_src_nodes--;
    }
}


/* funkcia vrati index najlacnejsej cesty spomedzi permutacii */
int cheapest_princess_rescue_path(MAZE *maze, int fatorial, int *total_time){
    int min, min_index, i;

    min = maze->princess_rescue[0].t;
    min_index = 0;

    for (i = 1; i < fatorial; i++){
        if(maze->princess_rescue[i].t < min){
            min = maze->princess_rescue[i].t;
            min_index = i;
        }
    }
    *total_time += maze->princess_rescue[min_index].t;

    return min_index;
}


/**
 * funkcia spoji najvyhodnejsiu cestu k drakovi a navstivenia vsetkych princezien a spravne ich zoradi
 */
int *create_and_connect_final_path(MAZE *maze, int **path, int index_of_cheapest_path){
    int *final_array = (int *)malloc((2*maze->total_path_lengt) + 1 * sizeof(int));
    int i, length_of_cheapest_path_princess_rescue;

    length_of_cheapest_path_princess_rescue = maze->princess_rescue[index_of_cheapest_path].num_princess_rescue_path;
    // printf("length of cheapest path: %d\n", length_of_cheapest_path_princess_rescue);

    for (i = 0; i < maze->total_path_lengt; i++){
        /* Overenie ci index patri ceste k drakovi (if) alebo k ceste k princeznam (else) */
        if(i < (maze->total_path_lengt - length_of_cheapest_path_princess_rescue)){
            final_array[2 * i] = path[i][0];
            final_array[2 * i + 1] = path[i][1];
        } else {
            final_array[2 * i] = maze->princess_rescue[index_of_cheapest_path].rescue_path_of_princess_permutation
            [i - (maze->total_path_lengt - length_of_cheapest_path_princess_rescue)][0];
            final_array[2 * i + 1] = maze->princess_rescue[index_of_cheapest_path].rescue_path_of_princess_permutation
            [i - (maze->total_path_lengt - length_of_cheapest_path_princess_rescue)][1];
            // printf("[%d, %d]\n",final_array[2 * i], final_array[2 * i + 1]);
        }
    }

    /* Uvolnenie path k drakovi */
    free_dragon_path(maze, path);

    return final_array;
}


int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty){
    int starting_index, factorial, index_of_cheapest_path;
    int *final_array, *total_time;
    int **path;

    total_time = (int *)malloc(sizeof(int));
    starting_index = *total_time = *dlzka_cesty = 0;

    MAZE *maze = init_maze(m, n);
    maze = load_maze(maze, mapa, t);

    /* Kontrola ci sa v bludisku nachadzaja aspon jedna princezna */
    if(maze->princess_num == 0 || maze->dragon == NULL){
        printf("V bludisko sa nenachadza minimalne jedna princezna alebo drak\n");
        return NULL;
    }

    /* Ak maze == NULL znamena ze k drakovi nevedie cesta */
    if(dijkstra(maze, starting_index) == 0){
        // print_graph(maze);
        printf("K drakovi nevedie cesta\n");
        return NULL;
    }

    // print_graph(maze);

    path = kil_the_dragon(maze, total_time);

    /* kontrola ak najdena cesta je dlhsia ako t tak som existuje kratsia cesta alebo t je nedosiahnutelne */
    if(*total_time > t){
       printf("Sorry, nedokazal som najst kratsiu cestu ako je t (%d)\n", t);
       return NULL;
    }

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
    // print_graph(maze);

    /* prechadzam polia moznych permutacii hladania princezien */ 
    for (int m = 0; m < factorial; m++){
        // printf("m: %d\n", m);

        /* prechadzam jednotlivo danu permutaciu */
        for (int n = 0; n < maze->princess_num; n++){

            /* Precistujem bludisko a pripravujem na novu dijsktru */
            re_init_maze(maze);       

            /* kontrola ci cesta k princeznej je pristupna */
            if(dijkstra(maze, starting_index) == 0){
                printf("Neviem sa dostat k princeznej\n");
                *dlzka_cesty = 0;
                return NULL;
            }

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
    index_of_cheapest_path = cheapest_princess_rescue_path(maze, factorial, total_time);

    // printf("total length:  %d\n", maze->total_path_lengt);
    /* K zatial docastnej casovej narocnosti cesty k drakovi pripocitava casovu narocnost hladania princezien */
    maze->total_path_lengt += maze->princess_rescue[index_of_cheapest_path].num_princess_rescue_path;

    /* For printing best permutation */
    print_final_rescue_princess_path(maze, index_of_cheapest_path);
    /* For printing all possible permutation */
    // print_princess_rescue_permutation(maze, factorial);

    /* Spoji pole pre cestu k drakovi a hladania princezien */
    final_array = create_and_connect_final_path(maze, path, index_of_cheapest_path);

    /* Nastavenie dlzky cesty */
    *dlzka_cesty = maze->total_path_lengt;


    /* functions for printing purposes and checks */
    // print_graph(maze);
    // print_princess_rescue_permutation(maze, factorial);
    printf("Najkratsia cesta s casom: %d\n", *total_time);

    free_maze(maze, factorial, total_time);
    return final_array;
}
