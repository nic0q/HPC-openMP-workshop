// gcc -fopenmp hello_openmp.c -o hello_openmp
#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int val1 = 5;
  int ntasks = 3;
  // Creación de múltiples hebras (pool), se comunican a traves de variables compartidas
  #pragma omp parallel num_threads(ntasks) shared(val1)
  {
    printf("antes -> %d | %d\n", val1, omp_get_thread_num());
    if(omp_get_thread_num() == 0){
      printf("IF\n");val1 = 10;
    }
    else{printf("ELSE\n");val1 = 400;}
    #pragma omp barrier // Sincronzación automática
    printf("despues -> %d\n", val1);
  } 
  printf("finalizacion -> %d\n", val1);
  return 1;
}