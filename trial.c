#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "cli.c"


// void print_help(const char* prg_name){
//     printf("Vexis - A cryptographic encryption tool\n");
//     printf("Usage: \n");
//     printf("./vexis <action> <filename> --<options>\n");
//     printf("Actions: \n");
//     printf(" 1. encrypt\n");
//     printf(" 2. decrypt\n");
//     printf("Options: \n");
//     printf(" 1. --help or -h\n");
// }


// void print_version(const char* prg_name){
//     printf("vexis 1.0.0\n");
// }

int main(int argc, char* argv[]){
    if(argc < 2){
        printf("Output: Error occured: No action specified. Try again!\n");
        vexis_cli_print_help();
        return 1;
    }

    if(argc == 2 && (strcmp(argv[1],"-h") == 0 || strcmp(argv[1],"--help") == 0)){
        vexis_cli_print_help();
        return 0;
    }

    if(argc == 2 && (strcmp(argv[1],"-v") == 0 || strcmp(argv[1],"--version") == 0)){
        vexis_cli_print_version();
        return 0;
    }

    if(strcmp(argv[3],"-h") == 0 || strcmp(argv[3],"--help") == 0){
        vexis_cli_print_help();
        return 0;
    }

    return 0;
}