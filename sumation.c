// gcc -fopenmp sumation.c -o sumation
#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
  int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int N = 10; // n° elements
  int a = 0;
  // Equally functional
  /*
    #pragma omp parallel
  {
    #pragma omp for reduction(+:a) // Se divide automáticamente el trabajo, reduction
    for(int i = 0; i < N; i++){
      a += arr[i];
    }
    #pragma omp single
      printf("Sum: %d\n", a);
  }
  */
  #pragma omp parallel reduction(+:a)
  {
    #pragma omp for // Se divide automáticamente el trabajo, reduction
    for(int i = 0; i < N; i++){
      a = arr[i];
    }
    #pragma omp single
      printf("Sum: %d\n", a);
  }
}