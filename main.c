#include "graph.c"
#include <stdio.h>
#include <stdlib.h> 


void free_map(char **map, int n){
    int i;

    for (i = 0; i < n; i++){
        free(map[i]);
    }
    free(map);
}


void print_map(char **map, int size){
    for (int i = 0; i < size; i++){
        for (int j = 0; j < size; j++){
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}


char **load_map(int maze_size){
    FILE *f;
    int i, j;
    char **map;

    switch (maze_size){
    case 4:
        f=fopen("./testing_maps/mapa_4x4.txt","r");
        map = (char**)malloc(maze_size * sizeof(char*));
        break;
    
    default:
        break;
    }

    for(i=0; i<maze_size; i++){
        map[i] = (char *)malloc(maze_size * sizeof(char));
        for (j = 0; j< maze_size; j++){
            char policko = fgetc(f);
            if(policko == '\n') 
                policko = fgetc(f);
            map[i][j] = policko;
        }
    }

    return map;
}


void print_path(int *path, int dlzka_cesty){
    int i;

    printf("\n---- Konecna postupnost -----\n");
    printf("Najkratsia cesta s casom: %d\n", dlzka_cesty);

    for (i = 0; i < 25; i++){
        if(path[2*i] == -1  || path[2*i+1] == -1)
            break;
        printf("[%d, %d]\n", path[2 * i + 1], path[2 * i]);
    }
}


void dijkstra_testing_env(char **map, int size, int t){
    int *path;
    int *dlzka_cesty = (int *)malloc(sizeof(int));

    if((path = zachran_princezne(map, size, size, t, dlzka_cesty)) == NULL){
        printf("Sorry, nedokazal som najst kratsiu cestu ako je t (%d)\n", t);
        return;
    }

    print_path(path, *dlzka_cesty);

    free(dlzka_cesty);
    free(path);
}

void testing_enviroment(){
    int choice, size, t;
    char **map;

    printf("======= Vitajte v testovacom prostredi algoritmu dijkstra, minimalnej haldy a heap algoritmu! =======\n");
    printf("Prosim vyberte si test (cislo 0-1):\n");
    printf("\t- 0. Zakladny test 4x4\n");
    printf("\t- 15. Nechcem uz testovat\n");

    printf("Test cislo: ");
    scanf("%d", &choice);

    switch (choice){
    case 0:
        map = load_map(4);
        size = 4;
        t = 6;
        print_map(map, 4);
        /* code */
        break;
    
    default:
        break;
    }

    dijkstra_testing_env(map, size, t);
    free_map(map, size);
}

int main(int argc, char const *argv[]){
    testing_enviroment();
    // int i = 0, t1 = 6;
    // int *postupnost, *postupnost2;
    // int *dlzka_cesty = (int *)malloc(sizeof(int));
    // char **testing = (char **)malloc(4 * sizeof(char *));

    // *dlzka_cesty = 0;

    // int n = 10, m = 10, t = 12;
    // char **mapa_test = (char **)malloc(n * sizeof(char *));

    // /* Test map 2 */
    // mapa_test[0]="CCHCNHCCHN";
    // mapa_test[1]="NNCCCHHCCC";
    // mapa_test[2]="DNCCNNHHHC";
    // mapa_test[3]="CHHHCCCCCC";
    // mapa_test[4]="CCCCCNHHHH";
    // mapa_test[5]="PCHCCCNNNN";
    // mapa_test[6]="NNNNNHCCCC";
    // mapa_test[7]="CCCCCPCCCC";
    // mapa_test[8]="CCCNNHHHHH";
    // mapa_test[9]="HHHPCCCCCC";

    // postupnost = zachran_princezne(mapa_test, n, m, t, dlzka_cesty);

    // printf("\n\n---- Konecna postupnost ku drakovi -----\n");
    // printf("Najkratsia cesta s casom: %d\n", *dlzka_cesty);

    // for (i = 0; i < 25; i++){
    //     if(postupnost[2*i] == -1  || postupnost[2*i+1] == -1)
    //         break;
    //     printf("[%d, %d]\n", postupnost[2 * i + 1], postupnost[2 * i]);
    // }

    // free(postupnost);
    // // free_map(mapa_test, n);

    return 0;
}
