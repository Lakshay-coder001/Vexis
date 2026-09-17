#include<stdio.h>
#include<stdlib.h>
#include "cli.c"

const char *print_vexis_err(const char* prg_name, vexis_err_t err){
    switch(err){
        case VEXIS_SUCC:
            return "SUCCESS";
        case VEXIS_ERR_GEN:
            return "General Failure";
        case VEXIS_ERR_IFILE:
            return "Input File Error";
        case VEXIS_ERR_OFILE:
            return "Output File Error";
        case VEXIS_ERR_PWD:
            return "Password Error";
        case VEXIS_ERR_CRYP:
            return "Cryptographic Error";
        case VEXIS_ERR_UNK:
            return "Unsupported File or Params";
        default:
            return "Unknown Error";
    }
}