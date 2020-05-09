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
    case 3:
        f=fopen("./testing_maps/mapa_5x4.txt","r");
        break;
    case 4:
        f=fopen("./testing_maps/mapa_2x7.txt","r");
        break;
    case 5:
        f=fopen("./testing_maps/mapa_5x15.txt","r");
        break;
    case 6:
        f=fopen("./testing_maps/mapa_3x3.txt","r");
        break;
    case 7:
        f=fopen("./testing_maps/mapa_5x5.txt","r");
        break;
    case 8:
        f=fopen("./testing_maps/mapa_20x20.txt","r");
        break;
    case 9:
        f=fopen("./testing_maps/mapa_7x100.txt","r");
        break;
    case 10:
        f=fopen("./testing_maps/mapa_15x100.txt","r");
        break;
    case 11:
        f=fopen("./testing_maps/mapa_30x100.txt","r");
        break;
    case 12:
        f=fopen("./testing_maps/mapa_25x50.txt","r");
        break;
    case 13:
        f=fopen("./testing_maps/mapa_50x50.txt","r");
        break;
    case 14:
        f=fopen("./testing_maps/mapa_6x6.txt","r");
        break;
    case 15:
        f=fopen("./testing_maps/mapa_7x7.txt","r");
        break;
    case 16:
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
    for (i = 0; i < dlzka_cesty; i++){
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
    // free(path);
}

void testing_enviroment(){
    int choice, t, y, x;
    char **map;

    printf("======= Vitajte v testovacom prostredi algoritmu dijkstra, minimalnej haldy a heap algoritmu! =======\n");
    printf("Prosim vyberte si test (cislo 0-1):\n");
    printf("\t- 0. Test 4x4\n");
    printf("\t- 1. Test 10x10\n");
    printf("\t- 2. Test 100x100 - NEFUNGUJE!\n");
    printf("\t- 3. Test na hladanie optimalnej cesty zachrany princezien 5x4\n");
    printf("\t- 4. Test v bludisku sa nenachadza ziadna princezna 2x7\n");
    printf("\t- 5. Test (cesta k drakovi cez princeznu) 5x15\n");
    printf("\t- 6. Test (cesta k drakovi cez princeznu) 3x3\n");
    printf("\t- 7. Test (cesta k drakovi cez princeznu (4 P)) 5x5\n");
    printf("\t- 8. The real maze! 20x20\n");
    printf("\t- 9. Test 7x100\n");
    printf("\t- 10. Test 15x100\n");
    printf("\t- 11. Test 30x100\n");
    printf("\t- 12. Test 25x50 - \n");
    printf("\t- 13. Zabednena princezna 50x50 - Bug\n");
    printf("\t- 14. Test 6X6 - zabedneny drak\n");
    printf("\t- 15. Test 7X7 - zabednena princezna\n");
    printf("\t- 16. Nechcem uz testovat\n");

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
    case 3:
        y = 5;
        x = 4;
        t = 11;
        break;
    case 4:
        y = 2;
        x = 7;
        t = 10;
        break;
    case 5:
        y = 5;
        x = 15;
        t = 100;
        break;
    case 6:
        y = x = 3;
        t = 8;
        break;
    case 7:
        y = x = 5;
        t = 74;
        break;
    case 8:
        y = x = 20;
        t = 95;
        break;
    case 9:
        y = 7;
        x = 100;
        t = 28;
        break;
    case 10:
        y = 15;
        x = 100;
        t = 28;
        break;
    case 11:
        y = 30;
        x = 100;
        t = 28;
        break;
    case 12:
        y = 25;
        x = 50;
        t = 28;
        break;
    case 13:
        y = x = 50;
        t = 300;
        break;
    case 14:
        y = x = 6;
        t = 300;
        break;
    case 15:
        y = x = 7;
        t = 300;
        break;
    default:
        break;
    }

    map = load_map(y, x, choice);
    if(map != NULL){
        // print_map(map, y, x);

        dijkstra_testing_env(map, y, x, t);
        free_map(map, y);
    }
}

int main(int argc, char const *argv[]){
    testing_enviroment();

    return 0;
}


/**
 * TODO -- skusit 100x100 na jednej princeznej...
*/