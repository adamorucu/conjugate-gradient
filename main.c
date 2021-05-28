#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <omp.h>

void dgemv_(const char *trans, const int *m,
    const int *n, const double *alpha, const double *A,
    const int *lda, const double *X, const int *incx,
    const double *beta, const double *Y, const int *incy);

static double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

// Used to read input data from a file
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

// Writes the resulting vector to a file
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

// Matrix-vector multiplication
// m: matrix, v: vector, d: output vector
void mult(const int size, const double* __restrict m, const double* __restrict v, double* __restrict d) {
#pragma omp parallel for
  for (int i=0; i<size; i++) {
     double temp = 0.0;
    for (int j=0; j<size; j++) {
      temp += m[i*size + j] * v[j];
    }
    d[i] = temp;
  }
}

// Performs conjugate gradient method
// x: initial guess for x, A: size x size, b: size
void conjugate(const double* __restrict A, const double* __restrict b, const int size, double* __restrict x) {
  double* r = (double*)malloc(size*sizeof(double));
  double* p = (double*)malloc(size*sizeof(double));
  double* Ap = (double*)malloc(size*sizeof(double));
  double rs_old = 0.0;
  double one = 1.0, zero = 0.0;
  double rs_new, rs, alpha;
  int oneint = 1;
  int i;

  mult(size, A, x, r);
  /* dgemv_("n", &size, &size, &one, A, &size, x, &oneint, &zero, r, &oneint); */

  for (i=0; i<size; i++) {
    r[i] = b[i] - r[i];
  }

  memmove(p, r, size*sizeof(double));

  for (i=0; i<size; i++) {
    rs_old += r[i] * r[i];
  }

  for (int k=0; k<size; k++) {
    mult(size, A, p, Ap);
    /* dgemv_("n", &size, &size, &one, A, &size, p, &oneint, &zero, Ap, &oneint); */

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

    if (sqrt(rs_new) < 1e-8){
      printf("exited\n");
      break;
    }

    rs = rs_new / rs_old;

    for (i=0; i<size; i++) {
      p[i] = r[i] + rs * p[i];
    }
    rs_old = rs_new;
  }
  
  free(r);
  free(p);
  free(Ap);
}


int main(int argc, char * argv[]){
  if (argc != 4) {
    printf("Incorrect argument number!\n");
    printf("Please input size filename nthreads\n");
    return 0;
  }
  const short int size = atoi(argv[1]);
  const char* filename = argv[2];
  const short int nthreads = atoi(argv[3]);
  omp_set_num_threads(nthreads);

  double* A = (double*)malloc(size*size*sizeof(double));
  double* b = (double*)malloc(size*sizeof(double));
  read_data(filename, size, A, b);

  double* x = (double*)malloc(size*sizeof(double));
  // initial guess for x
  for (int z=0; z<size; z++)
    x[z] = 1.0;

	double one = get_wall_seconds();
  conjugate(A, b, size, x);
  printf("Total time: %.2f s\n", get_wall_seconds() - one);

  write_result("result.txt", size, x);
  free(A);
  free(b);
  free(x);
  
  return 0;
}
