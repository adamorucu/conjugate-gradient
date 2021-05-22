#include <stdio.h>
#include <stdlib.h>
/* #include <math.h> */
/* #include <time.h> */
/* #include <omp.h> */

void transpose(const double* source, double* destination) {
  // can improve with that special trick from labs
  int i, j;
  for (i=0; i<N; i++)
    for (j=0; j<N; j++)
      destination[j*N+i] = source[i*N+j];
}

void mult(int n, int** a, int** b, int** c) {
  int i, j, k;
  for (k=0;k<n;k++) {
    for (i=0;i<n;i++) {
      int x = a[i][k];
      for (j=0;j<N;j++)
        c[i][j] += x * b[k][j];
    }
  }
}


void conjugate(double* A, double* b, int width, int height) {
  double* x = (double*)malloc(height*sizeof(double));
  double* r = (double*)malloc(height*sizeof(double));
  double* rnew = (double*)malloc(height*sizeof(double));
  double* rT = (double*)malloc(height*sizeof(double));
  double* p = (double*)malloc(height*sizeof(double));
  double* pT = (double*)malloc(height*sizeof(double));
  double* Ap = (double*)malloc(height*sizeof(double));
  double* alpha = (double*)malloc(height*sizeof(double));

  memset(x, 0, height*sizeof(double));
  mult(height, A, x, r);
  // maybe use memcpy instead of memmove
  memmove(r, p, sizeof(r));
  transpose(r, rT);
  int rsold = mult();
  // below sizeof double may change depending on b
  for (int i=0;i<(sizeof(b)/sizeof(double));i++){
    mult(height, A, p, Ap);
    transpose(p, pT);
    mult(height, pT, Ap, alpha);
    alpha = rsold / alpha;
    /* multi vec */
    for (int j){
      x[j] += alpha * p[j];
      r[j] += alpha * p[j];
    }
    transpose(r, rT);
    mult(height, rT, r, rnew);
    if (rnew < 1e-8)
      break;

  }
  
}

int main(int argc, char * argv[]){
  // height, width, filw
  if (argc != 2) {
    printf("Incorrect argument number!\n");
    return 0;
  }
  int width = 2;
  int height = 2;

  double* A = (double*)malloc(width*height*sizeof(double));
  double* b = (double*)malloc(bsize*sizeof(double));
  A[0] = 4;
  A[1] = 1;
  A[2] = 1;
  A[3] = 3;
  b[0] = 1;
  b[1] = 2;
  
  return 0;
}
