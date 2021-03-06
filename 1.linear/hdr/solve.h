#pragma once

int    solve(const int n, const int m,
             double* A, double* b, double* X,
             double* V1, double* V2, double* V3);
double residual(double* A, double* B, double* X, const int n, const int m);
double difference(const double* answer, const int dim);
