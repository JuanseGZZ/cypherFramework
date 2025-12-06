#ifndef nrandoms
#define nrandoms

#define FALSE 0
#define TRUE 1

int buscar(int numero,int* vec,int vecTam);
void mostrarVec(int* vec,int vecTam);
int getRandom(int desde,int hasta);
int* getVecRandoms(int desde,int hasta,int TAM);
int getRandomNotIn(int *vec,int tam,int from,int to);

#endif
