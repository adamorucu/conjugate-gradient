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

void
util_monotonic_time(struct timeval *ts)
{
	if (gettimeofday(ts, 0) != 0) {
                perror("gettimeofday failed");
                abort();
        }
}

double
util_time_diff(struct timeval *ts_start, struct timeval *ts_stop)
{
	return ((double)ts_stop->tv_sec-ts_start->tv_sec) + ((double)ts_stop->tv_usec-ts_start->tv_usec) * 1E-6;
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

/* void mult2(const int size, const double* __restrict m, const double* __restrict v, double* __restrict d) { */
/*   /1* double temp; *1/ */
/* #pragma omp parallel for */
/*   int split = 2; */
/*   for (int a=0; a<split; a++){ */
/*     for (int i=0; i<size; i++) { */
/*       double temp = 0.0; */
/*       for (int j=0; j<size; j++) { */
/*         temp += m[i*size + j] * v[j]; */
/*       } */
/*       d[i] = temp; */
/*     } */
/*   } */
/* } */

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

void conjugate(const double* __restrict A, const double* __restrict b, const int size, double* __restrict x) {
  for (int z=0; z<size; z++)
    x[z] = 1.0;
  struct timeval ts_start, ts_stop;
  double runtime_rest=0.0, runtime_mult=0.0;

  double* r = (double*)malloc(size*sizeof(double));
  double* p = (double*)malloc(size*sizeof(double));
  double* Ap = (double*)malloc(size*sizeof(double));
  double rs_old = 0.0;
  double one = 1.0;
  double zero = 0.0;
  int oneint = 1;
  double rs_new;
  int i;
  double alpha;
	double rs;

  mult(size, A, x, r);
  /* dgemv_("n", &size, &size, &one, A, &size, x, &oneint, &zero, r, &oneint); */

/* #pragma omp parallel for */
  for (i=0; i<size; i++) {
    r[i] = b[i] - r[i];
  }
  // maybe use memcpy instead of memmove
  memmove(p, r, size*sizeof(double));

  for (i=0; i<size; i++) {
    rs_old += r[i] * r[i];
  }

  for (int k=0; k<size; k++) {
    util_monotonic_time(&ts_start);
    mult(size, A, p, Ap);
    /* dgemv_("n", &size, &size, &one, A, &size, p, &oneint, &zero, Ap, &oneint); */

    util_monotonic_time(&ts_stop);
    runtime_mult += util_time_diff(&ts_start, &ts_stop);
    util_monotonic_time(&ts_start);

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
    if (sqrt(rs_new) < 1e-8){
      printf("exited\n");
      break;
    }

    rs = rs_new / rs_old;
/* #pragma omp parallel for */
    for (i=0; i<size; i++) {
      p[i] = r[i] + rs * p[i];
    }
    rs_old = rs_new;
    util_monotonic_time(&ts_stop);
    runtime_rest += util_time_diff(&ts_start, &ts_stop);
  }
  
  /* printf("K is: %d\n", k); */
  printf("Time spent on mult function %.2f s\n", runtime_mult);
  printf("Time spent on rest of the function %.2f s\n", runtime_rest);
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

	double one = get_wall_seconds();
  conjugate(A, b, size, x);
  printf("Total time: %.2f s\n", get_wall_seconds() - one);

  write_result("result.txt", size, x);
  free(A);
  free(b);
  free(x);
  
  return 0;
}
