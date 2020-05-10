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
        f=fopen("./testing_maps/mapa_10x10.txt","r");
        break;
    case 1:
        f=fopen("./testing_maps/mapa_4x4_variant1P.txt","r");
        break;
    case 2:
        f=fopen("./testing_maps/mapa_5x4.txt","r");
        break;
    case 3:
        f=fopen("./testing_maps/mapa_6x6.txt","r");
        break;
    case 4:
        f=fopen("./testing_maps/mapa_7x100.txt","r");
        break;
    case 5:
        f=fopen("./testing_maps/mapa_7x100_variant4P.txt","r");
        break;
    case 6:
        f=fopen("./testing_maps/mapa_20x20_variant1P.txt","r");
        break;
    case 7:
        f=fopen("./testing_maps/mapa_20x20.txt","r");
        break;
    case 8:
        f=fopen("./testing_maps/mapa_21x21.txt","r");
        break;
    case 9:
        f=fopen("./testing_maps/mapa_21x21_variant4P.txt","r");
        break;
    case 10:
        f=fopen("./testing_maps/mapa_25x50.txt","r");
        break;
    case 11:
        f=fopen("./testing_maps/mapa_25x50_variant5P.txt","r");
        break;
    case 12:
        f=fopen("./testing_maps/mapa_15x100.txt","r");
        break;
    case 13:
        f=fopen("./testing_maps/mapa_15x100_variant4P.txt","r");
        break;
    case 14:
        f=fopen("./testing_maps/mapa_30x100.txt","r");
        break;
    case 15:
        f=fopen("./testing_maps/mapa_30x100_variant5P.txt","r");
        break;
    case 16:
        f=fopen("./testing_maps/mapa_50x50.txt","r");
        break;
    case 17:
        f=fopen("./testing_maps/mapa_50x50_variant4P.txt","r");
        break;
    case 18:
        f=fopen("./testing_maps/mapa_100x100_variant3P.txt","r");
        break;
    case 19:
        f=fopen("./testing_maps/mapa_100x100.txt","r");
        break;
    case 20:
        f=fopen("./testing_maps/mapa_1x1.txt","r");
        break;
    case 21:
        f=fopen("./testing_maps/mapa_1x1_variant_drak.txt","r");
        break;
    case 22:
        f=fopen("./testing_maps/mapa_1x2.txt","r");
        break;
    case 23:
        f=fopen("./testing_maps/mapa_1x3.txt","r");
        break;
    case 24:
        f=fopen("./testing_maps/mapa_2x4.txt","r");
        break;
    case 25:
        f=fopen("./testing_maps/mapa_2x7.txt","r");
        break;
    case 26:
        f=fopen("./testing_maps/mapa_15x15.txt","r");
        break;
    case 27:
        f=fopen("./testing_maps/mapa_7x8.txt","r");
        break;
    case 28:
        f=fopen("./testing_maps/mapa_5x5_nepriechod.txt","r");
        break;
    case 29:
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
        printf("%d  %d\n", path[2 * i + 1], path[2 * i]);
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
    printf("\t- 0. Test ukazkovy 10x10 - 3P\n");
    printf("\t- 1. Test maly 4x4 - 1P\n");
    printf("\t- 2. Test maly 5x4 (najlacnejsia cesta rozhodnutie)- 3P\n");
    printf("\t- 3. Test maly 6x6 - 5P\n");
    printf("\t- 4. Test maly 7x100 - 2P\n");
    printf("\t- 5. Test maly 7x100 - 4P\n");
    printf("\t- 6. Test stredny 20x20 - 1P\n");
    printf("\t- 7. Test stredny 20x20 - 5P\n");
    printf("\t- 8. Test stredny 21x21 - 2P\n");
    printf("\t- 9. Test stredny 21x21 - 4P\n");
    printf("\t- 10. Test stredny 25x50 - 3P\n");
    printf("\t- 11. Test stredny 25x50 - 5P\n");
    printf("\t- 12. Test stredny 15x100 - 1P\n");
    printf("\t- 13. Test stredny 15x100 - 4P\n");
    printf("\t- 14. Test velky 30x100 - 3P\n");
    printf("\t- 15. Test velky 30x100 - 5P\n");
    printf("\t- 16. Test velky 50x50 - 2P\n");
    printf("\t- 17. Test velky 50x50 - 4P\n");
    printf("\t- 18. Test velky 100x100 - 3P\n");
    printf("\t- 19. Test velky 100x100 - 5P\n");
    printf("\t- 20. Test krajnych pripadov 1x1 - ziadna princezna ani drak\n");
    printf("\t- 21. Test krajnych pripadov 1x1 - ziadna princezna iba drak\n");
    printf("\t- 22. Test krajnych pripadov 1x2 - jedna princezna a drak\n");
    printf("\t- 23. Test krajnych pripadov 1x3 - drak a zablokovana princezna verticalne\n");
    printf("\t- 24. Test krajnych pripadov 2x4 - jedna princezna a zablokovany drak\n");
    printf("\t- 25. Test krajnych pripadov 2x7 - v bludisku sa nenachadza ziadna princezna\n");
    printf("\t- 26. Test krajnych pripadov 15x15 - 2 princezne a drak zablokovane horizontalne\n");
    printf("\t- 27. Test krajnych pripadov 7x8 - 4 princezne a ziadny drak\n");
    printf("\t- 28. Test krajnych pripadov 5x5 - nepriechodna sachovnica k princeznej\n");
    printf("\t- 29. Nechcem uz testovat\n");

    printf("Test cislo: ");
    scanf("%d", &choice);

    switch (choice){
    case 0:
        y = x = 10;
        t = 12;
        break;
    case 1:
        y = x = 4;
        t = 6;
        break;
    case 2:
        y = 5;
        x = 4;
        t = 3;
        break;
    case 3:
        y = x = 6;
        t = 17;
        break;
    case 4:
        y = 7;
        x = 100;
        t = 27;
        break;
    case 5:
        y = 7;
        x = 100;
        t = 27;
        break;
    case 6:
        y = x = 20;
        t = 95;
        break;
    case 7:
        y = x = 20;
        t = 95;
        break;
    case 8:
        y = x = 21;
        t = 55;
        break;
    case 9:
        y = x = 21;
        t = 55;
        break;
    case 10:
        y = 25;
        x = 50;
        t = 65;
        break;
    case 11:
        y = 25;
        x = 50;
        t = 65;
        break;
    case 12:
        y = 15;
        x = 100;
        t = 28;
        break;
    case 13:
        y = 15;
        x = 100;
        t = 28;
        break;
    case 14:
        y = 30;
        x = 100;
        t = 28;
        break;
    case 15:
        y = 30;
        x = 100;
        t = 28;
        break;
    case 16:
        y = x = 50;
        t = 67;
        break;
    case 17:
        y = x = 50;
        t = 67;
        break;
    case 18:
        y = x = 100;
        t = 28;
        break;
    case 19:
        y = x = 100;
        t = 28;
        break;
    case 20:
        y = x = 1;
        t = 0;
        break;
    case 21:
        y = x = 1;
        t = 1;
        break;
    case 22:
        y = 1;
        x = 2;
        t = 1;
        break;
    case 23:
        y = 1;
        x = 3;
        t = 1;
        break;
    case 24:
        y = 2;
        x = 4;
        t = 5;
        break;
    case 25:
        y = 2;
        x = 7;
        t = 10;
        break;
    case 26:
        y = x = t = 15;
        break;
    case 27:
        y = 7;
        x = 8;
        t = 10;
        break;
    case 28:
        y = 5;
        x = 5;
        t = 10;
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