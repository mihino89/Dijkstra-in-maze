#include "graph.c"

int main(int argc, char const *argv[]){
    int i = 0;
    int **postupnost;
    int *dlzka_cesty = (int *)malloc(sizeof(int));
    char **testing = (char **)malloc(4 * sizeof(char *));

    *dlzka_cesty = 0;

    int n = 10, m = 10, t = 12;
    char **mapa_test = (char **)malloc(n * sizeof(char *));

    /* Test map 1 */
    char mapa[4][4]={{'C','N','P','N'},
                     {'C','C','H','D'},
                     {'N','C','N','H'},
                     {'N','P','H','C'}};
    
    for (i = 0; i < 4;  i++)
        testing[i] = (char *)malloc(4 * sizeof(char));
    
    for (i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++)
            testing[i][j] = mapa[i][j];
    }

    postupnost = zachran_princezne(testing, 4, 4, 6, dlzka_cesty);

    printf("\n\n---- Konecna postupnost ku drakovi -----\n");
    printf("Najkratsia cesta s casom: %d\n", *dlzka_cesty);

    for (i = 0; i < INFINITY; i++){
        if(postupnost[i][0] == 0  && postupnost[i][1] == 0){
            printf("[%d, %d]\n", postupnost[i][0], postupnost[i][1]);
            break;
        }

        printf("[%d, %d]\n", postupnost[i][0], postupnost[i][1]);
    }

    printf("\n\n");

    /* Test map 2 */
    mapa_test[0]="CCHCNHCCHN";
    mapa_test[1]="NNCCCHHCCC";
    mapa_test[2]="DNCCNNHHHC";
    mapa_test[3]="CHHHCCCCCC";
    mapa_test[4]="CCCCCNHHHH";
    mapa_test[5]="PCHCCCNNNN";
    mapa_test[6]="NNNNNHCCCC";
    mapa_test[7]="CCCCCPCCCC";
    mapa_test[8]="CCCNNHHHHH";
    mapa_test[9]="HHHPCCCCCC";

    postupnost = zachran_princezne(mapa_test, n, m, t, dlzka_cesty);

    printf("\n\n---- Konecna postupnost ku drakovi -----\n");
    printf("Najkratsia cesta s casom: %d\n", *dlzka_cesty);

    for (i = 0; i < INFINITY; i++){
        if(postupnost[i][0] == 0  && postupnost[i][1] == 0){
            printf("[%d, %d]\n", postupnost[i][0], postupnost[i][1]);
            break;
        }

        printf("[%d, %d]\n", postupnost[i][1], postupnost[i][0]);
    }
    return 0;
}
