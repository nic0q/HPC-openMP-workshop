// gcc -fopenmp race_condition.c -o race_condition
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int yo, vecino, flag[2], buffer[2], result = 0;
  #pragma omp parallel private(yo, vecino) shared(buffer, flag, result) num_threads(2)
  {
    yo = omp_get_thread_num();  // qui´en soy yo
    flag[yo] = 0;
    #pragma omp barrier           // sincronizo
    
    buffer[yo] = 5 * (yo + 1);  // produzco un item
    flag[yo] = 1;         // aviso que el dato esta en el buffer

    vecino = (yo == 0 ? 1 : 0);
    while (flag[vecino] == 0);                                    // espero por el item

    result = buffer[vecino] * buffer[yo];  // computo final

  }
  printf("RES %d\n", result);
}
