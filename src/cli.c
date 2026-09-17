#include<stdio.h>
#include<string.h>
#include "../include/options.h"
#include "../include/vex_error.h"
#include "../include/config.h"
#include "../include/cli.h"

void vexis_cli_print_help(){
    printf("VEXIS - A Cryptographic Encryption Tool\n");
    printf("USAGE: \n");
    printf("./vexis <action> <filename> --<options>\n");
    printf("ACTIONS: \n");
    printf(" 1. encrypt\n");
    printf(" 2. decrypt\n");
    printf("OPTIONS: \n");
    printf(" 1. --help or -h\n");
    printf(" 2. --version or -v\n");
}


void vexis_cli_print_version(){
    printf(VEXIS_VERSION_STRING);
}

vexis_err_t vexis_cli_parse(int argc, char *argv[]){
    if (argc < 2) {
        return VEXIS_ERR_GEN;
    }

    if (argc == 2) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            vexis_cli_print_help();
            return VEXIS_SUCC;
        }
        if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
            vexis_cli_print_version();
            return VEXIS_SUCC;
        }
    }

    if (argc >= 3) {
        if (strcmp(argv[argc - 1], "-h") == 0 || strcmp(argv[argc - 1], "--help") == 0) {
            vexis_cli_print_help();
            return VEXIS_SUCC;
        }
        if (strcmp(argv[argc - 1], "-v") == 0 || strcmp(argv[argc - 1], "--version") == 0) {
            vexis_cli_print_version();
            return VEXIS_SUCC;
        }
    }

    if (argc != 3) {
        return VEXIS_ERR_UNK;
    }
    if (strcmp(argv[1], "encrypt") != 0 && strcmp(argv[1], "decrypt") != 0){
        return VEXIS_ERR_UNK;
    }
    //here handling the file opening case
    FILE *input = fopen(argv[2], "rb");
    if(input == NULL){
        return VEXIS_ERR_IFILE;
    }
    if(fclose(input) != 0){
        return VEXIS_ERR_IFILE;
    }

    return VEXIS_SUCC;
}