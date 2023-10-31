/*

	Este programa indica cómo se establece un bloque paralelo en OpenMp.

		Entrada : No requiere entrada.
		Salida : Imprime por pantalla un hola mundo por cada hebra generada.

*/

//Inclusión de ficheros de cabecera
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/*****
    
    Compilación!!!!
    gcc -o nombre_ejecutable simple.c -fopenmp

*****/

//Número de hebras
#define H 8

int main(int argc, char const *argv[])
{
	//Variable donde se almacenará el id de la hebra
	int tid;
	//Se asigna la cantidad de hebras a utilizar. Notar que estas son hebras lógicas y no
	//	guardan relación necesariamente con las hebras a nivel físico.
	omp_set_num_threads(H);
	//Se define una sección paralela. De manera transparente para el progamador se paraleliza
	//	bajo la estrategia fork-join el código indicado. Notar que se trabaja con notación
	//	de bloques.
	#pragma omp parallel
	{
		//Se obtiene el identificador de la hebra (tid). Notar que este identificador
		//	no guarda relación con la dirección real de la heba y es sólo un identicador
		//	para el programador [0,H-1], donde H es la cantidad de hebras a utilizar.
		tid = omp_get_thread_num();
		printf("Hola mundo :D! desde hebra = %i\n",tid);
	}
	return 0;
}