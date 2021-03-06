#pragma once

// a size: av * ah
// b size: bv * bh
static inline int multiply(const double* const a, const int av, const int ah,
                           const double* const b, const int bv, const int bh,
                           double* const c) {
    int r = 0, t = 0, q, temp_r, temp_t;
    // для вышеописанной логики
    double c00, c01, c02, c10, c11, c12, c20, c21, c22;
    if (ah != bv)
        return -1;
    // Зануляем c
    for (r = 0; r < av; r++)
        for (t = 0; t < bh; t++)
            c[r * bh + t] = 0.;

    for (r = 0; r + 2 < av; r += 3)
        for (t = 0; t + 2 < bh; t += 3) {
            c00 = 0., c01 = 0., c02 = 0.;
            c10 = 0., c11 = 0., c12 = 0.;
            c20 = 0., c21 = 0., c22 = 0.;
            for (q = 0; q < ah; q++) {
                c00 += a[(r + 0) * ah + q] * b[q * bh + (t + 0)];
                c01 += a[(r + 0) * ah + q] * b[q * bh + (t + 1)];
                c02 += a[(r + 0) * ah + q] * b[q * bh + (t + 2)];
                c10 += a[(r + 1) * ah + q] * b[q * bh + (t + 0)];
                c11 += a[(r + 1) * ah + q] * b[q * bh + (t + 1)];
                c12 += a[(r + 1) * ah + q] * b[q * bh + (t + 2)];
                c20 += a[(r + 2) * ah + q] * b[q * bh + (t + 0)];
                c21 += a[(r + 2) * ah + q] * b[q * bh + (t + 1)];
                c22 += a[(r + 2) * ah + q] * b[q * bh + (t + 2)];
            }
            c[(r + 0) * bh + (t + 0)] += c00;
            c[(r + 0) * bh + (t + 1)] += c01;
            c[(r + 0) * bh + (t + 2)] += c02;
            c[(r + 1) * bh + (t + 0)] += c10;
            c[(r + 1) * bh + (t + 1)] += c11;
            c[(r + 1) * bh + (t + 2)] += c12;
            c[(r + 2) * bh + (t + 0)] += c20;
            c[(r + 2) * bh + (t + 1)] += c21;
            c[(r + 2) * bh + (t + 2)] += c22;
        }
    temp_t = t;
    temp_r = r;
    // если не получилось разделить четно, то
    // повторяем процесс для последнего столбца и строчки
    // так, как делали раньше
    if ((av - temp_r) == 2) {
        for (t = 0; t + 1 < bh; t += 2) {
            c00 = 0., c01 = 0.;
            c10 = 0., c11 = 0.;
            for (q = 0; q < ah; q++) {
                c00 += a[(temp_r + 0) * ah + q] * b[q * bh + (t + 0)];
                c01 += a[(temp_r + 0) * ah + q] * b[q * bh + (t + 1)];
                c10 += a[(temp_r + 1) * ah + q] * b[q * bh + (t + 0)];
                c11 += a[(temp_r + 1) * ah + q] * b[q * bh + (t + 1)];
            }
            c[(temp_r + 0) * bh + (t + 0)] = c00;
            c[(temp_r + 0) * bh + (t + 1)] = c01;
            c[(temp_r + 1) * bh + (t + 0)] = c10;
            c[(temp_r + 1) * bh + (t + 1)] = c11;
        }
        if (t < bh) {
            for (r = temp_r; r < av; r++)
                for (t = bh & ~1; t < bh; t++) {
                    c00 = 0.;
                    for (q = 0; q < ah; q++) {
                        c00 += a[(r + 0) * ah + q] * b[q * bh + (t + 0)];
                    }
                    c[(r + 0) * bh + (t + 0)] = c00;
                }
        }
    }

    if ((bh - temp_t) == 2) {
        for (r = 0; r + 1 < av; r += 2) {
            c00 = 0., c01 = 0.;
            c10 = 0., c11 = 0.;
            for (q = 0; q < ah; q++) {
                c00 += a[(r + 0) * ah + q] * b[q * bh + (temp_t + 0)];
                c01 += a[(r + 0) * ah + q] * b[q * bh + (temp_t + 1)];
                c10 += a[(r + 1) * ah + q] * b[q * bh + (temp_t + 0)];
                c11 += a[(r + 1) * ah + q] * b[q * bh + (temp_t + 1)];
            }
            c[(r + 0) * bh + (temp_t + 0)] = c00;
            c[(r + 0) * bh + (temp_t + 1)] = c01;
            c[(r + 1) * bh + (temp_t + 0)] = c10;
            c[(r + 1) * bh + (temp_t + 1)] = c11;
        }
        for (r = av & ~1; r < av; r++) {
            for (t = temp_t; t < bh; t++) {
                c00 = 0.;
                for (q = 0; q < ah; q++) {
                    c00 += a[(r + 0) * ah + q] * b[q * bh + (t + 0)];
                }
                c[(r + 0) * bh + (t + 0)] = c00;
            }
        }
    }

    if (av - temp_r == 1) {
        for (t = 0; t < bh; t++) {
            c00 = 0.;
            for (q = 0; q < ah; q++)
                c00 += a[temp_r * ah + q] * b[q * bh + t];
            c[temp_r * bh + t] = c00;
        }
    }
    if (bh - temp_t == 1) {
        for (r = 0; r < av; r++) {
            c00 = 0.;
            for (q = 0; q < ah; q++)
                c00 += a[r * ah + q] * b[q * bh + temp_t];
            c[r * bh + temp_t] = c00;
        }
    }
    return 0;
}

// c -= a * b
static inline int multiply_and_extract(const double* const a, const int av, const int ah,
                                       const double* const b, const int bv, const int bh,
                                       double* const c) {
    int r = 0, t = 0, q, temp_r, temp_t;
    double c00, c01, c02, c10, c11, c12, c20, c21, c22;
    if (ah != bv)
        return -1;

    for (r = 0; r + 2 < av; r += 3)
        for (t = 0; t + 2 < bh; t += 3) {
            c00 = 0., c01 = 0., c02 = 0.;
            c10 = 0., c11 = 0., c12 = 0.;
            c20 = 0., c21 = 0., c22 = 0.;
            for (q = 0; q < ah; q++) {
                c00 += a[(r + 0) * ah + q] * b[q * bh + (t + 0)];
                c01 += a[(r + 0) * ah + q] * b[q * bh + (t + 1)];
                c02 += a[(r + 0) * ah + q] * b[q * bh + (t + 2)];
                c10 += a[(r + 1) * ah + q] * b[q * bh + (t + 0)];
                c11 += a[(r + 1) * ah + q] * b[q * bh + (t + 1)];
                c12 += a[(r + 1) * ah + q] * b[q * bh + (t + 2)];
                c20 += a[(r + 2) * ah + q] * b[q * bh + (t + 0)];
                c21 += a[(r + 2) * ah + q] * b[q * bh + (t + 1)];
                c22 += a[(r + 2) * ah + q] * b[q * bh + (t + 2)];
            }
            c[(r + 0) * bh + (t + 0)] -= c00;
            c[(r + 0) * bh + (t + 1)] -= c01;
            c[(r + 0) * bh + (t + 2)] -= c02;
            c[(r + 1) * bh + (t + 0)] -= c10;
            c[(r + 1) * bh + (t + 1)] -= c11;
            c[(r + 1) * bh + (t + 2)] -= c12;
            c[(r + 2) * bh + (t + 0)] -= c20;
            c[(r + 2) * bh + (t + 1)] -= c21;
            c[(r + 2) * bh + (t + 2)] -= c22;
        }
    temp_r = r;
    temp_t = t;
    if (temp_r < av) {
        for (; r < av; r++) {
            for (t = 0; t + 2 < bh; t += 3) {
                c00 = 0., c01 = 0., c02 = 0.;
                for (q = 0; q < ah; q++) {
                    c00 += a[(r + 0) * ah + q] * b[q * bh + (t + 0)];
                    c01 += a[(r + 0) * ah + q] * b[q * bh + (t + 1)];
                    c02 += a[(r + 0) * ah + q] * b[q * bh + (t + 2)];
                }
                c[(r + 0) * bh + (t + 0)] -= c00;
                c[(r + 0) * bh + (t + 1)] -= c01;
                c[(r + 0) * bh + (t + 2)] -= c02;
            }
        }

        if (t < bh) {
            for (r = temp_r; r < av; r++) {
                for (t = temp_t; t < bh; t++) {
                    c00 = 0.;
                    for (q = 0; q < ah; q++) {
                        c00 += a[(r + 0) * ah + q] * b[q * bh + (t + 0)];
                    }
                    c[(r + 0) * bh + (t + 0)] -= c00;
                }
            }
        }
    }
    if (temp_t < bh) {
        for (r = 0; r + 2 < av; r += 3)
            for (t = temp_t; t < bh; t++) {
                c00 = 0., c10 = 0., c20 = 0.;
                for (q = 0; q < ah; q++) {
                    c00 += a[(r + 0) * ah + q] * b[q * bh + (t + 0)];
                    c10 += a[(r + 1) * ah + q] * b[q * bh + (t + 0)];
                    c20 += a[(r + 2) * ah + q] * b[q * bh + (t + 0)];
                }
                c[(r + 0) * bh + (t + 0)] -= c00;
                c[(r + 1) * bh + (t + 0)] -= c10;
                c[(r + 2) * bh + (t + 0)] -= c20;
            }
    }
    return 0;
}
