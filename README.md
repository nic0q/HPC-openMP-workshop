## Multiplicación matricial paralela

### Algoritmo secuencial
```c
double start_s, end_s;
int N = 512;
int* mat_1 = calloc(N * N, sizeof(int));
int* mat_2 = calloc(N * N, sizeof(int));
int* mat_3 = calloc(N * N, sizeof(int));
int* mat_4 = calloc(N * N, sizeof(int));
```
```c
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
```
### Multiplicación matricial paralela
```c
double start_p, end_p;
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
```
#### Resultados tiempo(s)
Tamaño/Algoritmo | Secuencial | Paralelo
--- | --- | --- | 
32x32 | 0.0010 | 0.0013
64x64 | 0.0011 | 0.0013
128x128 | 0.0078 | 0.0029
256x256 | 0.057 | 0.016

- Se puede apreciar que si se colocan matrices pequeñas el tiempo paralelo es mayor dado que la parte paralela es insignificante y se demora más en la comunicación entre hebras.
Sin embargo con matrices grandes se obtiene excelentes resultados:

## Determinante de una matríz paralelo

### Minor matriz
![image](https://github.com/nic0q/openMP-HPC-workshop/assets/91075814/cdb010ad-1fc1-4258-a549-2b730758142c)
- M|0,0|, M|0,1|, M|0,2| son minors

### Función para obtener el minor
```c
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
```
### Función recursiva determinante
```c
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
```

### Paralelización
Cada hebra trabaja con recursividad para calcular el determinante de la matríz que le tocó pero no se crean nuevas hebras en niveles más profundos de la recursividad.
```c
#pragma omp parallel
  {
    #pragma omp for reduction(+ : sum)
    for (int i = 0; i < N; i++){
      sum += mat[i] * pow(-1, i) * det(N - 1, minor(N, 0, i, mat));
    }
  }
```
#### Resultados tiempo(s)
Tamaño/Algoritmo | Secuencial | Paralelo
--- | --- | --- | 
6  | 0.0010 | 0.0013
7  | 0.0011 | 0.0013
8  | 0.0078 | 0.0029
9  | 0.057 | 0.016
10 | 0.057 | 0.016
11 | 0.057 | 0.016

## Scope / Alcance
Algo que no sabía de c hasta que tuve que ocupar "{" "}" para hacer un scope o bloque es que si uno tiene una claúsula "if" o "for" sin "{" "}" es tomada solametne para la línea de abajo.
```c
  for(int i = 0; i < 3; i++)
    printf("😎\n");
    printf("🎶\n");
```
```
😎
😎
😎
🎶
```

```c
  for(int i = 0; i < 3; i++){
    printf("😎\n");
    printf("🎶\n");
  }
```

```
😎
🎶
😎
🎶
😎
🎶
```

### Lo mismo aplica para OpenMP
```c
#pragma omp parallel
  #pragma omp single
```
Es equivalente a
```c
#pragma omp parallel
  {
  #pragma omp single
    {
    }
  }
```
A veces es redundante ocupar llaves, por lo que entendiendo esto se puede tener un código más legible

## Variable pública vs Privada (for loop)
#### Pública
```c
  int k;
  #pragma omp parallel shared(k)
  for(k = 0; k < 3; k++){
    printf("for shared %d t: %d\n", k, omp_get_thread_num());
  }
```
Output
```
for shared 0 t: 0
for shared 0 t: 2
for shared 2 t: 2
for shared 0 t: 1
for shared 0 t: 3
for shared 0 t: 6
for shared 0 t: 4
for shared 1 t: 0
for shared 0 t: 7
for shared 0 t: 5
```
Lo que está ocurriendo es una **condición de carrera** dado que al ser una variable compartida por todas las hebras, todas sobreescriben su valor de manera caótica, por lo que el ciclo se detiene cuando alguna toma el valor de k = 2 y lo suma, hasta que k = 3.
#### Privada
```c
  #pragma omp parallel
  for(int k = 0; k < 3; k++){
    printf("for priv %d t: %d\n", k, omp_get_thread_num());
  }
```
```
for priv 0 t: 4
for priv 1 t: 4
for priv 2 t: 4
for priv 0 t: 5
for priv 1 t: 5
for priv 2 t: 5
for priv 0 t: 0
for priv 1 t: 0
for priv 2 t: 0
for priv 0 t: 7
for priv 1 t: 7
for priv 2 t: 7
for priv 0 t: 1
for priv 1 t: 1
for priv 2 t: 1
for priv 0 t: 6
for priv 1 t: 6
for priv 2 t: 6
for priv 0 t: 2
for priv 1 t: 2
for priv 2 t: 2
for priv 0 t: 3
for priv 1 t: 3
for priv 2 t: 3
```
En este caso se puede observar que cada hebra tiene su propia copia de la variable k, por lo que debe cada una debe hacer el ciclo for es decir que si se crean 4 hebras cada una imprimirá 3 veces, serían 12 líneas, en este caso se crean las hebras 0,1,2,3,4,5,6,7 (8 hebras) por lo que se imprimen 24 líneas.

## Single
```c
#pragma omp single
```
El código ejecutado es solo ejecutado por 1 hebra (cualquiera).

## ¿Cómo acumular variables?
En vez de hacer critical/atomic o un lock, si se coloca **reduction (+:ac) se acumula el valor de la variable deseada automáticamente
```c
#pragma omp for reduction(+:ac)
```
## Acumulación paralela (Barrera implícita en for y wait/nowait)
```c
  int arr[5] = {1, 2, 3, 4, 5};
  int N = 5; // n° elements
  int ac = 0;
  int i = 0;
  #pragma omp parallel
  {
    #pragma omp for reduction(+:ac)
    for(i = 0; i < N; i++){
      ac += arr[i];
      printf("for %d\n", i);
    }
    #pragma omp single
    printf("Sum: %d\n", ac);
  }
```
```
for 4
for 3
for 0
for 1
for 2
Sum: 15
```
**#pragma omp for** tiene implícita una barrera al final de la ejecución por lo que si se realia un un **#pragma omp single** al final se obtendrá el resultado sincronizado por todas las hebras, un **#pragma omp barrier** sería redundante

## wait vs no wait
Si se coloca nowait, no se sincroniza al finalizar el for, por lo que se obtiene cualquier resultado **(Condición de carrera)**
```c
#pragma omp for reduction(+:ac) nowait
```

```
for 3
for 2
for 4
Sum: 0
for 1
for 0
```


## Tarea
Bloque independiente de código que será ejecutado cuando se pueda, utilizado para dividir el trabajo a elementos no estructurados como listas enlazadas, árboles.

### Creación de múltiples tareas
En este caso se crean 10 tareas y cada una imprime la hebra a la que está unida/ligada.
```c
  #pragma omp parallel
  #pragma omp single nowait
  {
    for (int i = 0; i < 10; i++) {
      #pragma omp task
      printf("Thread no. %d\n", omp_get_thread_num());
    }
  }
  return 0;
```

```
Thread no. 3
Thread no. 1
Thread no. 3
Thread no. 4
Thread no. 2
Thread no. 6
Thread no. 5
Thread no. 7
Thread no. 1
Thread no. 0
```

512x512 | 0.420 | 0.105
1024x1024 | 3.969 | 1.376
