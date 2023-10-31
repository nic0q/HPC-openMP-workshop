/*

    Programa tutorial para distintas directivas de OpenMP.
    El objetivo de este programa es encontrar el menor valor dentro de un arreglo
    de manera paralela con el uso de OpenMP.

        Entrada : No requiere entrada.
        Salida : Imprime por pantalla el menor valor del arreglo

*/

//Inclusión de ficheros de cabecera
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/*****
    
    Compilación!!!!
    gcc -o nombre_ejecutable menor.c -fopenmp

*****/

//Número de hebras
#define H 3
//Tamaño del vector
#define N 9

int main(int argc, char const *argv[])
{
    //Variable donde se almacenará el id de la hebra
    int tid;
    //Entrada del problema
    int valores[N] = {9,4,1,3,7,8,5,2,6};
    //Variable donde se almacenará el menor valor encontrado por cada hebra.
    int minimo;
    //Variable donde se almacenará el menor valor encotrado entre todas las hebras.
    int minimo_global = valores[0];
    //Variable de iteración
    int i;
    //Seteo número de hebras
    omp_set_num_threads(H);
    //Se define una sección paralela. Las variables indicadas en shared() son variables 
    //  compartidas es decir, todas las hebras acceden y modifican la misma dirección de 
    //  memoria (heap). Las variables en private() son variables privadas que sólo son
    //  accedidas por la hebra que las utiliza (stack).
    #pragma omp parallel shared(valores,minimo_global) private(i,minimo)
    {
        minimo = valores[0];
        //Se indica la directiva para que el for se realice de manera paralela.
        //  Nuevamente, esto ocurre de manera transparante para el programador.
        #pragma omp for
        for(i = 0 ; i < N ; i++)
        {
            //Se busca el valor mínimo en los valores del vector
            if(minimo > valores[i])
            {
                minimo = valores[i];
            }
        }

        printf("minimo : %i\n", minimo);

        //Es necesario que todas las hebras hayan procesado su trozo del vector
        //  antes de poder procesar el menor de todo el vector. Por esto,
        //  se incluye una barrera de sincronización; gracias a esta, todas las 
        //  hebras se sincronizarán desde esta directiva.
        #pragma omp barrier

        //Cada hebra intentará actualizar el valor global de la variable.
        //  En muchos casos se pueden producir condiciones de carrera, es decir,
        //  el resultado final de la operación dependerá del orden en el que se ejecuten
        //  las hebras. Por esto, se incluye la directiva critical, la cual indica 
        //  a las hebras ejecutar la sección de código indicada sólo una a la vez.
        #pragma omp critical
        {
            if(minimo < minimo_global)
            {
                minimo_global = minimo;
            }
        }

        //Notar que es necesario una barrera para que la ejecución del código sea la correcta.
        //  Se busca encontrar el menor valor dentro de un arreglo de manera paralela.
        //  Sin esta barrera la primera hebra que actualice el valor de 'minimo_global' podrá
        //  imprimir el valor de 'minimo_global', pero no podemos asumir que las demás hebras
        //  han terminado de ejecutar la sección crítica, por tanto, es necesaria una barrera
        //  para obtener el verdadero valor del mínimo global.
        //  Verifiquen las diferencias al ejecutar con la siguiente linea comentada y sin comentar
        //#pragma omp barrier

        //Solo una hebra ejecuta esta acción. No indica cuál será.
        #pragma omp single
        printf("El menor valor encontrado es: %i\n", minimo_global);
        
    }
    return 0;
}