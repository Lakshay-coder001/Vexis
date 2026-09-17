#ifndef VEXIS_ERR_H
#define VEXIS_ERR_H
#include<stdio.h>

typedef enum{
    VEXIS_SUCC = 0,
    VEXIS_ERR_GEN = 1,
    VEXIS_ERR_IFILE = 2,
    VEXIS_ERR_OFILE = 3,
    VEXIS_ERR_PWD = 4,
    VEXIS_ERR_CRYP = 5,
    VEXIS_ERR_UNK = 6
} vexis_err_t;

const char *print_vexis_err(vexis_err_t err);

#endif