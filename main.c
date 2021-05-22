#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
/* #include <time.h> */
/* #include <omp.h> */

void dot(int size, double* m, double* v, double* d) {
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

  dot(size, A, x, r);

  for (i=0; i<size; i++) {
    r[i] = b[i] - r[i];
  }
  // maybe use memcpy instead of memmove
  memmove(p, r, size*sizeof(double));

  for (i=0; i<size; i++) {
    rs_old += r[i] * r[i];
  }

  for (int k=0; k<size; k++) {
    dot(size, A, p, Ap);

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


int main(int argc, char * argv[]){
  // height, width, filw
  /* if (argc != 2) { */
  /*   printf("Incorrect argument number!\n"); */
  /*   return 0; */
  /* } */
  int size = 2;

  double* A = (double*)malloc(size*size*sizeof(double));
  double* b = (double*)malloc(size*sizeof(double));
  A[0] = 4;
  A[1] = 1;
  A[2] = 1;
  A[3] = 3;
  b[0] = 1;
  b[1] = 2;
  double* x = (double*)malloc(size*sizeof(double));
  conjugate(A, b, size, x);

  printf("X: [ ");
  for (int i=0; i<size; i++)
    printf("%lf ", x[i]);
  printf("]\n");
  
  return 0;
}
