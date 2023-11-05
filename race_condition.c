#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int yo, vecino, flag[2], buffer[2], result = 0;
  #pragma omp parallel private(yo, vecino) shared(buffer, flag, result) num_threads(2)
  {
    yo = omp_get_thread_num();  // quien soy yo
    buffer[yo] = 5 * yo;  // produzco un item
    vecino = (yo == 0 ? 1 : 0);
    #pragma omp barrier
    result = buffer[vecino] * buffer[yo];
  }
  printf("RES %d\n", result);
}
