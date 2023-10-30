// gcc -fopenmp single.c -o single
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void work1() { printf("W1\n"); }
void work2() { printf("W2\n"); }

int main() {
  #pragma omp parallel num_threads(5)
  {
    #pragma omp single
    {
      printf("Invocando work1() %d\n", omp_get_thread_num());
      work1();
    }
    #pragma omp single
      printf("Fin work1() %d\n", omp_get_thread_num());
    #pragma omp single
    {
      printf("Invocando work2() %d\n", omp_get_thread_num());
      work2();
    }
    #pragma omp single
      printf("Fin work2()  %d\n", omp_get_thread_num());
  }
}
