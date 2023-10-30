// gcc -fopenmp tasks.c -o tasks
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  int t = 5;
  int c = 3;
  FILE* file = fopen("hltau_test.csv", "rb");
  char buffer[256]; // string de largo 256
  #pragma omp parallel shared(file)
  {
    #pragma omp single
    {
      for (int i=0;i < t; i++){
        float* arr = calloc(16 * c, sizeof(float));
        #pragma omp task untied
          while(feof(file) == 0) {
            #pragma omp critical
            for (int j = 0; j < c; j++) {
              if(feof(file) != 0)     // Si se llegó al final del archivo mientras se esta leyendo lineas del chunk, se sale del ciclo
                break;
            fgets(buffer,sizeof(buffer),file);
            printf("thread: %d vis: %s",omp_get_thread_num(),  buffer);
            }
            printf("Procesar thread: %d | %d\n", omp_get_thread_num(), feof(file));
            // recorrer el arreglo de vis 6 * c
          }
      }
    }
  }
}