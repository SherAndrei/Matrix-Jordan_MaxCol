#include <stdio.h>
#include "error.h"

int error(const int error) {
    switch (error) {
    case 1: {
        printf("Usage: ./a.out n m r s (filename)\n"
                "\t n - matrix dim:       \t1, 2 ...  \n"
                "\t m - block dim:        \t1, 2 ... n\n"
                "\t r - printing value:   \t0, 1 ... n\n"
                "\t s - number of formula:\t0, 1 ... 4\n"
                "\t if s == 0 input file's name\n");
        break;
    }
    case 2: {
        printf("Malloc error!\n");
        break;
    }
    case 3: {
        printf("Error opening file!\n");
        break;
    }
    case 4: {
        printf("Incorrect input or not enough data!\n");
        break;
    }
    case 5: {
        printf("Algotithm is inaplicable!\n");
        break;
    }
    default:
        return 0;
    }
    return error;
}
