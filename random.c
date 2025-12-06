#include "stdlib.h"
#include "stdio.h"
#include "random.h"

/* el hasta esta incluido */
int getRandom(int desde,int hasta){
    return rand()%(hasta-desde+1)+desde;
}

void mostrarVec(int* vec,int vecTam){
    int i = 0;
    for (i = 0; i<vecTam ; i++){
        printf("\nPOS{%d},VAL{%d}\n",i,vec[i]);
    }
}

int buscar(int numero,int* vec,int vecIterator){
    int i = 0;
    for (i = 0; i<=vecIterator ; i++){
        /*printf("\n es %d = %d? = %s \n",numero,vec[i],numero==vec[i] ? "true" : "false");*/
        if (vec[i]==numero){
            return TRUE;
        }
    }
    return FALSE;
}

int* getVecRandoms(int desde,int hasta,int TAM){
    int *vec = (int*)malloc(TAM*sizeof(int));
    int i = 0;
    int resultado = FALSE;

    while(i<TAM){
        int numRandom = getRandom(desde,hasta);
        resultado = buscar(numRandom,vec,i);
        while(resultado){
            numRandom = getRandom(desde,hasta);
            resultado = buscar(numRandom,vec,i);
        }
        vec[i] = numRandom;
        i++;
    }
    return vec;
}

int getRandomNotIn(int *vec,int tam,int from,int to){
    int notin = 0;
    int i = 0;
    int rand = getRandom(from,to);
    while (!notin){
        for (i=0;i<tam;i++){
            if (rand == vec[i]){
                rand = getRandom(from,to);
                break;
            }
            if (i==tam-1){
                notin = 1;
            }
        }
    }
    return rand;
}
