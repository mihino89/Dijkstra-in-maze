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


void print_map(char **map, int y, int x){
    int i, j;

    for (i = 0; i < y; i++){
        for (j = 0; j < x; j++){
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}


char **load_map(int y, int x, int map_id){
    FILE *f;
    int i, j;
    char **map;

    switch (map_id){
    case 0:
        f=fopen("./testing_maps/mapa_4x4.txt","r");
        break;
    case 1:
        f=fopen("./testing_maps/mapa_10x10.txt","r");
        break;
    case 2:
        f=fopen("./testing_maps/mapa_100x100.txt","r");
        break;
    case 15:
        return NULL;
    default:
        printf("Nenasiel som subor mapy\n");
        return NULL;
    }

    map = (char**)malloc(y * sizeof(char*));

    for(i=0; i < y; i++){
        map[i] = (char *)malloc(x * sizeof(char));
        for (j = 0; j < x; j++){
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


void dijkstra_testing_env(char **map, int y, int x, int t){
    int *path;
    int *dlzka_cesty = (int *)malloc(sizeof(int));

    if((path = zachran_princezne(map, y, x, t, dlzka_cesty)) == NULL)
        return;

    print_path(path, *dlzka_cesty);

    free(dlzka_cesty);
    free(path);
}

void testing_enviroment(){
    int choice, t, y, x;
    char **map;

    printf("======= Vitajte v testovacom prostredi algoritmu dijkstra, minimalnej haldy a heap algoritmu! =======\n");
    printf("Prosim vyberte si test (cislo 0-1):\n");
    printf("\t- 0. Zakladny test 4x4\n");
    printf("\t- 1. Zakladny test 10x10\n");
    printf("\t- 2. Zakladny test 100x100 - Drak nenajdeny\n");
    printf("\t- 15. Nechcem uz testovat\n");

    printf("Test cislo: ");
    scanf("%d", &choice);

    switch (choice){
    case 0:
        y = x = 4;
        t = 6;
        break;
    case 1:
        y = x = 10;
        t = 12;
        break;
    case 2:
        y = x = 100;
        t = 1200;
        break;
    default:
        break;
    }

    map = load_map(y, x, choice);
    if(map != NULL){
        print_map(map, y, x);

        dijkstra_testing_env(map, y, x, t);
        free_map(map, y);
    }
}

int main(int argc, char const *argv[]){
    testing_enviroment();

    return 0;
}
