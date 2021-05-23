#include <stdio.h>
#include <stdlib.h>

void read_data(const char* filename, const int size, double* A, double* b) {
  FILE* file = fopen(filename, "r");
  int num=0;
  fscanf(file, "%d", &num);
  int i=0;
  while(!feof(file)) {
    if (i<size*size){
      A[i] = num;
      printf("A[%d] = %d\n", i, num);
    }
    else{
      b[i-size*size] = num;
      printf("b[%d] = %d\n", i, num);
    }
    fscanf(file, "%d", &num);
    i++;
  }
  fclose(file);
}

int main(int argc, char* argv[]) {
  int size = 3;
  double* A = (double*)malloc(size*size*sizeof(double));
  double* b = (double*)malloc(size*sizeof(double));
  read_data("data/size3.txt", size, A, b);

  printf("A\n");
  for (int i=0; i<size; i++)
    for (int j=0; j<size; j++)
      printf("%lf ", A[i*size+j]);
    printf("\n");

  printf("b\n");
  for (int j=0; j<size; j++)
    printf("%lf ", b[j]);
  return 0;
}
