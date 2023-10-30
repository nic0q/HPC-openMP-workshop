// gcc -fopenmp least.c -o least
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  int N = 10;
  int arr[10] = {1,2,3,4,-2,-42,-45,100,290,0};
  int min = arr[0];
  double start;
  double end;
  start = omp_get_wtime();
  #pragma omp parallel shared(min)
  {
    #pragma omp for
    for(int i = 1; i < N; i++){
      /*Deben ser critical al, el for dividirse el trabajo entre X hebras,
      cualquier puede tomar algun valor relativo (consistencia relajada),
      por lo tanto nada asegura que el valor menor que tenga cada hebra
      sea el menor absoluto
      */
      #pragma omp critical
      if(arr[i] < min)
        min = arr[i];
    }
    #pragma omp single
    printf("menor valor %d\n", min);
  }
  end = omp_get_wtime();
  printf("Tiempo usado = %f sec.\n", end-start);
  return 0;
}