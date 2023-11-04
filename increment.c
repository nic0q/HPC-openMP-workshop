#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*Escriba un código en openMP donde 5 hebras incrementan en paralelo una variable entera
compartida a, inicializada en cero, y tal que se imprima una sola vez el valor resultante, antes
de salir del bloque paralelo*/
int main(void) {
  int a = 0;
  int n = 5;
  #pragma omp parallel num_threads(n) reduction(+ : a)
    #pragma omp for
      for(int i = 0; i < n; i++){
        a++;
      }
    #pragma single
    printf("%d\n", a);
}