#include<stdio.h>
#include<string.h>
#include "../include/options.h"
#include "../include/vex_error.h"
#include "../include/config.h"
#include "../include/cli.h"

void vexis_cli_print_help(){
    printf("VEXIS - A Cryptographic Encryption Tool\n");
    printf("USAGE: \n");
    printf("  ./vexis encrypt <filename>\n");
    printf("  ./vexis decrypt <filename.vxs> -o <output_file>\n");
    printf("ACTIONS: \n");
    printf(" 1. encrypt\n");
    printf(" 2. decrypt\n");
    printf("OPTIONS: \n");
    printf(" 1. --help or -h\n");
    printf(" 2. --version or -v\n");
    printf(" 3. -o <output_file> (required for decrypt)\n");
}


void vexis_cli_print_version(){
    printf(VEXIS_VERSION_STRING);
}

vexis_err_t vexis_cli_parse(int argc, char *argv[], vexis_cli_options *options){
    if(!options){
        return VEXIS_ERR_GEN;
    }
    options->cmd = VEXIS_CMD_EMP;
    options->input_path = NULL;
    options->output_path = NULL;

    if (argc < 2) {
        return VEXIS_ERR_GEN;
    }

    if (argc == 2) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            options->cmd = VEXIS_CMD_HELP;
            vexis_cli_print_help();
            return VEXIS_SUCC;
        }
        if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
            options->cmd = VEXIS_CMD_VERSION;
            vexis_cli_print_version();
            return VEXIS_SUCC;
        }
        return VEXIS_ERR_UNK;
    }

    if (strcmp(argv[1], "encrypt") == 0){
        if (argc != 3) {
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
        options->cmd = VEXIS_CMD_ENCRYPT;
        options->input_path = argv[2];
        return VEXIS_SUCC;
    }

    // decrypt is parsed here
    if (strcmp(argv[1], "decrypt") == 0){
        if (argc != 5) {
            return VEXIS_ERR_UNK;
        }
        if (strcmp(argv[3], "-o") != 0) {
            return VEXIS_ERR_UNK;
        }
        if (argv[4] == NULL || argv[4][0] == '\0') {
            return VEXIS_ERR_UNK;
        }
        if (strcmp(argv[2], argv[4]) == 0){ // case where in and out path same
            return VEXIS_ERR_UNK;
        }
        FILE *input = fopen(argv[2], "rb");
        if(input == NULL){
            return VEXIS_ERR_IFILE;
        }
        if(fclose(input) != 0){
            return VEXIS_ERR_IFILE;
        }

        options->cmd = VEXIS_CMD_DECRYPT;
        options->input_path = argv[2];
        options->output_path = argv[4];
        return VEXIS_SUCC;
    }

    return VEXIS_ERR_UNK;
}