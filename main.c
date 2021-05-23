#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
/* #include <time.h> */
/* #include <omp.h> */

void mult(const int size, double* m, double* v, double* d) {
  double temp;
  for (int i=0; i<size; i++) {
    temp = 0.0;
    for (int j=0; j<size; j++) {
      temp += m[i*size + j] * v[j];
    }
    d[i] = temp;
  }
}

void conjugate(double* A, double* b, const int size, double* x) {
  /* double* x = (double*)malloc(size*sizeof(double)); */
  memset(x, 1.0, sizeof(x));
  for (int z=0; z<size; z++)
    x[z] = 1.0;

  double* r = (double*)malloc(size*sizeof(double));
  double* p = (double*)malloc(size*sizeof(double));
  double* Ap = (double*)malloc(size*sizeof(double));
  double rs_old = 0.0;
  double rs_new;
  int i;
  double alpha;

  mult(size, A, x, r);

  for (i=0; i<size; i++) {
    r[i] = b[i] - r[i];
  }
  // maybe use memcpy instead of memmove
  memmove(p, r, size*sizeof(double));

  for (i=0; i<size; i++) {
    rs_old += r[i] * r[i];
  }

  for (int k=0; k<size; k++) {
    mult(size, A, p, Ap);

    alpha = 0.0;
    for (i=0; i<size; i++) {
      alpha += p[i] * Ap[i];
    }
    alpha = rs_old / alpha;

    for (i=0; i<size; i++) {
      x[i] += alpha * p[i];
      r[i] -= alpha * Ap[i];
    }

    rs_new = 0.0;
    for (i=0; i<size; i++) {
      rs_new += r[i] * r[i];
    }
    // try out other square root functions
    if (sqrt(rs_new) < 1e-8)
      break;

    double rs = rs_new / rs_old;
    for (i=0; i<size; i++) {
      p[i] = r[i] + rs * p[i];
    }
    rs_old = rs_new;
  }
  
}

void read_data(const char* filename, const int size, double* A, double* b) {
  FILE* file = fopen(filename, "r");
  double num=0.0;
  fscanf(file, "%lf", &num);
  int i=0;
  while(!feof(file)) {
    if (i<size*size){
      A[i] = num;
    }
    else{
      b[i-size*size] = num;
    }
    fscanf(file, "%lf", &num);
    i++;
  }
  fclose(file);
}

void write_result(const char* filename, const int size, double* x) {
  FILE* file;
  file = fopen(filename, "w");
  double num;
  for (int i=0; i<size; i++) {
    num = x[i];
    char buf[50];
    snprintf(buf, 50, "%f\n", num);
    fputs(buf, file);
  }
  fclose(file);
}


int main(int argc, char * argv[]){
  // height, width, filw
  if (argc != 3) {
    printf("Incorrect argument number!\n");
    printf("Please input size filename\n");
    return 0;
  }
  const short int size = atoi(argv[1]);
  const char* filename = argv[2];

  for (int y=0; y<1000; y++) {
  double* A = (double*)malloc(size*size*sizeof(double));
  double* b = (double*)malloc(size*sizeof(double));
  read_data(filename, size, A, b);

  double* x = (double*)malloc(size*sizeof(double));
  conjugate(A, b, size, x);

  free(A);
  free(b);
  free(x);
  }

  /* printf("X: [ "); */
  /* for (int i=0; i<size; i++) */
  /*   printf("%lf ", x[i]); */
  /* printf("]\n"); */

  /* write_result("result.txt", size, x); */
  
  return 0;
}
