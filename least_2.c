// gcc -fopenmp least.c -o least
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  int N = 10;
  int arr[10] = {1,2,3,4,-2,-42,-45,100,290,0};
  int min_global = arr[0];
  int min_local = arr[0];
  double start;
  double end;
  start = omp_get_wtime();
  #pragma omp parallel firstprivate(min_local) shared(min_global)
  {

    #pragma omp for
    for(int i = 1; i < N; i++){
      if(arr[i] < min_local)
        min_local = arr[i];
    }
    #pragma omp barrier

    #pragma omp critical
    if (min_local < min_global)
      min_global = min_local;
    /* Barrier
    es necesario usar un barrier dado que alguna hebra puede ejecutar el single
    y este no será el valor mínimo.
    #pragma omp barrier
    #pragma omp single
    printf("menor valor %d\n", min_global);  
    */
  }
  end = omp_get_wtime();
  printf("Tiempo usado = %f sec.\n", end-start);
  printf("menor valor %d\n", min_global);
  return 0;
}