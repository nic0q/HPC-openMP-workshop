#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

float det(int N, float* matrix);
float* minor(int N, int ix, int jx, float* matrix);
void print_matrix(float* matrix, int N);

int main() {
  int N = 3; // 3 x 3 square matrix
  float sum = 0;
  float mat_1[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  float* mat_2 = calloc(N * N, sizeof(float));
  float* minor_m = minor(N, 0, 0, mat_1);

  float hola[9] = {2,-3,1,2,0,-1,1,4,5};

  float t = 10, w = N / t;
  
  #pragma omp parallel num_threads(10)
  {
    float local_sum = 0;
    float k = omp_get_thread_num();
    printf("hebra: %f %f\n", k, k * w);
    for (float j = k * w; j < k * w + w - 1; j++)
    {
      printf("NUCNA");
      local_sum += det(N, hola);
    }

    #pragma omp barrier
    #pragma omp critical
    sum += local_sum;
  }
  
  float dete = det(3, hola);

  printf("%.2f\n", dete);

  printf("sum %f", sum);
}

float det(int N, float* matrix){
  if(N == 2)
    return matrix[0] * matrix[3] - matrix[1] * matrix[2];
  else{
    int n = 0;
    for(int j = 0; j < N; j++){
      n += matrix[j] * pow(-1, j) * det(N - 1, minor(N, 0, j, matrix));
    }
    return n;
  }
}

float* minor(int N, int ix, int jx, float* matrix){
  float* minor = calloc((N - 1) * (N - 1), sizeof(float));
  int index = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      if (i != ix && j != jx){
        minor[index] = matrix[i * N + j];
        index++;
      }
    }
  }
  return minor;
}

void print_matrix(float* matrix, int N){
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%f ", matrix[i * N + j]);
    }
    printf("\n");
  }  
}