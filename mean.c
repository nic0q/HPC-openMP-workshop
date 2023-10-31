// gcc -fopenmp mean.c -o mean
#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
  int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int N = 10; // n° elements
  int acum = 0;
  #pragma omp parallel
  {
    #pragma omp for reduction(+:acum)// Se divide automáticamente el trabajo, reduction
    for(int i = 0; i < N; i++){
      acum += arr[i];
    }
  }
  //printf("XXXX %.3f\n", float convertion of (* 1.) acum / N);
  printf("Suma %.2f\n", acum * 1./ N);
}

