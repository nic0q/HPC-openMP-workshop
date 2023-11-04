#include <stdio.h>
#include <omp.h>

int main(){
  int arr[14] = {1,2,3,4,5,6,7,8,-1,100,99,101,203,202};
  int N = 14, max = arr[0];
  int h = 10;
  #pragma omp parallel num_threads(h)
  {
    int local_max = max;
    #pragma omp for
      for (int i = 1; i < N; i++)
        if(arr[i] > local_max)
          local_max = arr[i];
    #pragma omp critical
      if(local_max > max)
        max = local_max;
  }
  printf("Max %d\n", max);  
}