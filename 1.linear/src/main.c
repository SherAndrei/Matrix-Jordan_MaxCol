/*
    Метод Гаусса с выбором максимального элемента по столбцу
    решения системы линейных уравнений
*/
#define _GNU_SOURCE
#include <sys/sysinfo.h>
#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "error.h"
#include "print.h"
#include "fill.h"
#include "solve.h"

static inline double* alloc_matrix(const int size) {
    return (double*) malloc (size * sizeof(double));
}

static inline void free_matrix(double* matrix) {
    free(matrix);
}

int main(int argc, const char* argv[]) {
    int errcode = 0;
    int n, m, r, s, i;
    // Solving AX = B
    // V1, V2, V3 вспомогательные матрицы
    double *A  = NULL, *B  = NULL, *X  = NULL,
           *V1 = NULL, *V2 = NULL, *V3 = NULL;
    double res;
    double t_resid, t_solving;
    // Чтобы зафиксировать выделение памяти только на одном процессоре
    cpu_set_t cpu;
    int nprocs = 0;
    nprocs = get_nprocs();
    CPU_ZERO(&cpu);
    CPU_SET(nprocs - 1, &cpu);
    sched_setaffinity(getpid(), sizeof(cpu), &cpu);

    printf(" Usage: [");
    for (i = 1; i < argc - 1; i++)
        printf("%s, ", argv[i]);
    printf("%s]\n", argv[argc - 1]);

    if (!((argc == 5 || argc == 6) &&
        (sscanf(argv[1], "%d", &n) == 1) &&
        (sscanf(argv[2], "%d", &m) == 1) &&
        (sscanf(argv[3], "%d", &r) == 1) &&
        (sscanf(argv[4], "%d", &s) == 1) &&
        (n >= 0)           &&
        (m >  0 && m <= n) &&
        (r >= 0 && r <= n)))
            return error(1);

    // выделяем память под матрицу и под правый вектор
    A  = alloc_matrix(n * n), B  = alloc_matrix(n * 1), X  = alloc_matrix(n * 1);
    V1 = alloc_matrix(m * m), V2 = alloc_matrix(m * m), V3 = alloc_matrix(m * m);
    if (A  == NULL || B  == NULL || X  == NULL ||
        V1 == NULL || V2 == NULL || V3 == NULL) {
        free_matrix(A),  free_matrix(B),  free_matrix(X);
        free_matrix(V1), free_matrix(V2), free_matrix(V3);
        return error(2);
    }

    if (s == 0 && argc == 6)
        fill(A, n, m, 0, argv[5], &errcode);
    else if ((s > 0 && s < 5) && argc == 5)
        fill(A, n, m, s, NULL, NULL);
    else
        errcode = 1;

    if (errcode > 0) {
        free_matrix(A),  free_matrix(B),  free_matrix(X);
        free_matrix(V1), free_matrix(V2), free_matrix(V3);
        return error(errcode);
    }

    fill_right_part(A, B, n, m);

    if (r > 0) {
        print_matrix(A, n, n, m, r);
        print_matrix(B, n, 1, m, r);
    }

    t_solving = clock();
    errcode = solve(n, m, A, B, X, V1, V2, V3);
    t_solving = (clock() - t_solving) / CLOCKS_PER_SEC;
    if (errcode < 0) {
        free_matrix(A),  free_matrix(B),  free_matrix(X);
        free_matrix(V1), free_matrix(V2), free_matrix(V3);
        return error(5);
    }
    if (r > 0) {
        printf(" Result:\n");
        print_matrix(X, n, 1, m, r);
    }
    if (s == 0)
        fill(A, n, m, 0, argv[5], &errcode);
    else
        fill(A, n, m, s, NULL, NULL);
    if (errcode > 0) {
        free_matrix(A),  free_matrix(B),  free_matrix(X);
        free_matrix(V1), free_matrix(V2), free_matrix(V3);
        return error(errcode);
    }

    fill_right_part(A, B, n, m);

    t_resid = clock();
    res = residual(A, B, X, n, m);
    t_resid = (clock() - t_resid) / CLOCKS_PER_SEC;
    printf(" Difference: %10.3e\n", difference(X, n));
    printf(" Time computing residual: %6.3f sec\n\n", t_resid);
    printf("%s : residual = %e elapsed = %.2f for s = %d n = %d m = %d\n",
            argv[0], res, t_solving,
            s, n, m);

    free_matrix(A),  free_matrix(B),  free_matrix(X);
    free_matrix(V1), free_matrix(V2), free_matrix(V3);
    return 0;
}
