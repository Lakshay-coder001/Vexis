#include "include/options.h"
#include "include/vex_error.h"
#include "include/config.h"
#include "include/cli.h"

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