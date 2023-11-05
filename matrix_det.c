#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

float det(int N, float* matrix);
float* minor(int N, int ix, int jx, float* matrix);
void print_matrix(float* matrix, int N);

int main() {
  int N = 11; // 3 x 3 square matrix
  float sum = 0, sec = 0;
  double start, end;
  clock_t s, e;

  float* mat = calloc(N * N, sizeof(float));
  for (int i = 0; i < N* N; i++) {
    mat[i] = rand() % 100;
  }
  
  //float mat[25] = {0,1,0,-2,1, 1,0,3,1,1 ,  1,-1,1,1,1, 2,2,1,0,1, 3,1,1,1,2};
  start = omp_get_wtime();
  #pragma omp parallel num_threads(11)
  {
    #pragma omp for reduction(+ : sum)
    for (int i = 0; i < N; i++){
      sum += mat[i] * pow(-1, i) * det(N - 1, minor(N, 0, i, mat));
      printf("h: %d res: %f\n", omp_get_thread_num(), sum);
    }
  }
  end = omp_get_wtime();
  printf("Parallelll %f %f\n", sum, end - start);

  start = omp_get_wtime();
  sec = det(N, mat);
  end = omp_get_wtime();
  printf("Secuential %f %f\n", sec, end - start);

}

float det(int N, float* matrix){
  if(N == 2)
    return matrix[0] * matrix[3] - matrix[1] * matrix[2];
  else{
    float n = 0;
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