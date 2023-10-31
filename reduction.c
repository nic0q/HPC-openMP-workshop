/*

    Programa tutorial para distintas directivas de OpenMP.
    El objetivo de este programa es realizar la sumatoria de los primeros N números enteros,
    donde N es un valor de entrada por terminal

        Entrada : N -> Límite de la sumatoria
        Salida : Imprime por pantalla la sumatoria de los primeros N números

*/

//Inclusión de ficheros de cabecera
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

//Número de hebras
#define H 3

/*****
    
    Compilación!!!
    gcc -o nombre_ejecutable reduction.c -fopenmp

    Ejecución!!!
    ./nombre_ejecutable valor_de_N

*****/

int main(int argc, char const *argv[])
{
    //Variable donde se almacenará el id de la hebra
    int tid;
    //Tamaño del problema. Se obtiene como entrada desde el terminal
    int N = atoi(argv[1]);
    //Variable de iteración
    int i;
    //Variable del resultado
    int sum = 0;
    //Seteo número de hebras
    omp_set_num_threads(H);
    //Se define una sección paralela.
    #pragma omp parallel
    {
    	//Se ejecuta el for en paralelo. La directiva reduction() indica que se debe actualizar el valor de suma
    	//	otorgando exclusión mutua a la sección crítica presente con operación suma sobre esa variable.
        //La directiva nowait indica a la hebra que cuando termine el for no necesita esperar por el resto
        #pragma omp for nowait private(i) reduction(+:sum) 
		for (i = 1; i <= N; i++)
		{
		   sum += i;
		}

        //Se define una barrera de sincronización.
		#pragma omp barrier

		//Sólo el thread principal ejecuta esta acción
		#pragma omp master
		printf("Valor de la suma de los los primeros %i valores es %i\n",N,sum);
    }
    return 0;
}
