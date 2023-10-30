// gcc -fopenmp sections.c -o sections
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  int sum = 0;
  #pragma omp parallel num_threads(3) reduction(+:sum)
  {
    #pragma omp sections
    {
      #pragma omp section
      {
        sum = 1;
        printf("Hebra %d, en section 0\n", omp_get_thread_num());
      }
      #pragma omp section
      {
        sum = 2;
        printf("Hebra %d, en section 1\n", omp_get_thread_num());
      }
      #pragma omp section
      {
        sum = 3;
        printf("Hebra %d, en section 2\n", omp_get_thread_num());
      }
    }
  }
  printf("total sum = %d\n", sum);
}