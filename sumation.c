// gcc -fopenmp sumation.c -o sumation
#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void) {
  int arr[5] = {1, 2, 3, 4, 5};
  int N = 5; // n° elements
  int ac = 0;
  int i = 0;
  #pragma omp parallel
  {
    #pragma omp for reduction(+:ac)
    for(i = 0; i < N; i++){
      ac += arr[i];
      printf("for %d\n", i);
    }
    #pragma omp single
    printf("Sum: %d\n", ac);
  }
}