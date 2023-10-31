// gcc -fopenmp tasks.c -o tasks
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
const float SPEED_OF_LIGHT = 299792458;
float arcsec_to_rad(float deg);
void write_file(char* archive_name, float* pixels, int size);

int main(void) {
  int t = 5;
  int c = 3;
  int deltaX = 0.003;
  int cont = 0;
  int N = 2048;
  int deltaU = 1 / (N * arcsec_to_rad(deltaX));  // to radians
  int deltaV = deltaU;
  FILE* file = fopen("hltau_completo_uv.csv", "rb");
  char buffer[256]; // string de largo 256 chars
  float* absfr = calloc(N * N, sizeof(float));
  float* absfi = calloc(N * N, sizeof(float));
  float* abswt = calloc(N * N, sizeof(float));
  float* fr = calloc(N * N, sizeof(float));
  float* fi = calloc(N * N, sizeof(float));
  float* wt = calloc(N * N, sizeof(float));
  #pragma omp parallel shared(file, cont,absfr, absfi, abswt)
  {
    #pragma omp single
    {
    for (int i=0;i < t; i++){
      #pragma omp task untied firstprivate(fr, fi, wt)
      {
      while(feof(file) == 0) {
        float* vis = calloc(6 * c, sizeof(float));
        #pragma omp critical
        for (int j = 0; j < c; j++) {
          if(feof(file) != 0)     // Si se llegó al final del archivo mientras se esta leyendo lineas del chunk, se sale del ciclo
            break;
          fgets(buffer,sizeof(buffer),file);
          int row = j * 6;
          sscanf(buffer, "%f,%f,%*f,%f,%f,%f,%f,%*f", &vis[row], &vis[row+1], &vis[row+2], &vis[row+3], &vis[row+4], &vis[row+5]);
          // printf("thread: %d | %s",omp_get_thread_num(),  buffer);
          cont++;
        }
        // printf("Thread: %d | %d cont %d\n", omp_get_thread_num(), feof(file), cont);
        if(vis[0] != 0.0)
        for(int a = 0; a < c; a++){
          int row = a * 6;
          float vr = vis[row + 2];
          float vi = vis[row + 3];
          float wk = vis[row + 4];
          float fq = vis[row + 5];
          float fqspeed = fq / SPEED_OF_LIGHT;
          float uk = vis[row] * fqspeed;
          float vk = vis[row + 1] * fqspeed;
          int ik = round(uk / deltaU) + (N / 2);  // i,j coordinate
          int jk = round(vk / deltaV) + (N / 2);
          int index = ik * N + jk;
          // printf("ANTES:FR[%d]=%f, value:%f \n",index, fr[index], wk * vr);
          fr[index] += wk * vr;  // acumulate in matrix fr, fi, wt
          // printf("DESPUES %f\n", fr[index]);
          fi[index] += wk * vi;
          wt[index] += wk;
        }
      } // while archivo
      for (int j = 0; j < N * N; j++) {  // Acumulate in main matrix's fr, fi, wt
      absfr[j] += fr[j];
      absfi[j] += fi[j];
      abswt[j] += wt[j];
      }
      
  }
    } // for tasks

    }
  }
  for (int i = 0; i < N * N; i++) {  // Acumulate in main matrix's fr, fi, wt
    if(abswt[i]!=0){
      absfr[i] = absfr[i] / abswt[i];
      absfi[i] = absfi[i] / abswt[i];
    }
  }
  write_file("datosgrideadosr.raw", absfr, N * N);
  write_file("datosgrideadosi.raw", absfi, N * N);
}
float arcsec_to_rad(float deg){  // arcseconds to radians
  return deg * M_PI / (180 * 3600);
}
void write_file(char* archive_name, float* data, int dim) {
  FILE* file;
  file = fopen(archive_name, "wb");

  size_t elements_written = fwrite(data, sizeof(float), dim, file);

  if (elements_written == dim) {
    printf("All elements were written successfully.\n");
  } else {
    printf("There was an error while writing the elements.\n");
  }

  fclose(file);
}