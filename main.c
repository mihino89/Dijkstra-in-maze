#include "graph.c"

int main(int argc, char const *argv[]){
    int *postupnost = (int *)malloc(16 * sizeof(int));
    char **testing;
    testing = (char **)malloc(4 * sizeof(char *));

    /* BUG */
    // char mapa[4][4]={{'C','N','P','N'},
    //                  {'C','C','H','D'},
    //                  {'N','C','N','H'},
    //                  {'N','P','H','C'}};

    char mapa[4][4]={{'C','N','P','N'},
                     {'C','C','H','D'},
                     {'N','C','N','H'},
                     {'N','P','H','N'}};

    for (int i = 0; i < 4;  i++){
        testing[i] = (char *)malloc(4 * sizeof(char));
    }

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++)
            testing[i][j] = mapa[i][j];
    }

        postupnost = zachran_princezne(testing, 4, 4, 6, 0);
    return 0;
}
