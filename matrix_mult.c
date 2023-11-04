#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void print_matrix(int* matrix, int N);

int main() {
  double start_s, end_s, start_p, end_p;
  int N = 256, sum; // 3 x 3 square matrix
  // int mat_1[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  // int mat_2[9] = {1, 2, 1, 2, 4, 6, 7, 2, 5};
  int* mat_1 = calloc(N * N, sizeof(int));
  int* mat_2 = calloc(N * N, sizeof(int));
  int* mat_3 = calloc(N * N, sizeof(int));
  int* mat_4 = calloc(N * N, sizeof(int));

  start_s = omp_get_wtime();
  for (int i = 0; i < N; i++) {   // O(N**3)
    for (int j = 0; j < N; j++) {
      sum = 0;
      for (int k = 0; k < N; k++) {
        sum += mat_1[i * N + k] * mat_2[k * N + j];
      }
      mat_3[i * N + j] = sum;
    }
  }
  end_s = omp_get_wtime();
  // print_matrix(mat_3, N);
  printf("Secuential Time: %.4f[s]\n", end_s - start_s);

  start_p = omp_get_wtime();
  #pragma omp parallel for
    for (int i = 0; i < N; i++) { 
      for (int j = 0; j < N; j++) {
        int sum = 0;
        for (int k = 0; k < N; k++) {
          sum += mat_1[i*N+k]*mat_2[k*N+j];
        }
        mat_4[i*N+j ] = sum;
      }
    }
  end_p = omp_get_wtime();
  // print_matrix(mat_4, N);
  printf("Parallel Time: %.4f[s]\n", end_p - start_p);
}

void print_matrix(int* matrix, int N){
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%d ", matrix[i * N + j]);
    }
    printf("\n");
  }  
}