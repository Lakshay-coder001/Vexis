#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../include/cli.h"
#include "../include/vex_error.h"


int main(int argc, char* argv[]){
    // step1: parsing of cl command 
    vexis_cli_options options = {0};
    vexis_err_t err = vexis_cli_parse(argc , argv, &options);
    if(err != VEXIS_SUCC){
        fprintf(stderr, "Vexis Error: %s\n",print_vexis_err(err));
        return err;
    } //this will handle all the errors

    switch (options.cmd) {
        case VEXIS_CMD_HELP:
            return 0;
        case VEXIS_CMD_VERSION:
            return 0;
        case VEXIS_CMD_ENCRYPT:
        // here need to do the encrypt logic with crypto and all
        // if malloc fail for .vxs file then give VEXIS_ERR_GEN
            printf("Encrypting..... %s\n", options.input_path);
            return 0;
        case VEXIS_CMD_DECRYPT:
        // same here need to write the decrypt logic
            printf("Decrypting..... %s -> %s\n", options.input_path, options.output_path);
            return 0;
        default:
            fprintf(stderr, "Vexis Error: %s\n", print_vexis_err(VEXIS_ERR_UNK));
            return VEXIS_ERR_UNK;
    }

    return 0;
}