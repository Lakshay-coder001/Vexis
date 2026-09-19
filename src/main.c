#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../include/cli.h"
#include "../include/vex_error.h"
#include "../include/config.h"
#include "../include/password.h"
#include "../include/output.h"

static vexis_err_t vexis_run_encrypt(vexis_cli_options *options){
    char password[VEXIS_PWD_MAX];
    char *enc_path = NULL;
    FILE *out = NULL;
    vexis_err_t err;

    if (options == NULL || options->input_path == NULL) {
        return VEXIS_ERR_UNK;
    }

    err = vexis_password_prompt_confirm(password, sizeof(password));
    if (err != VEXIS_SUCC) {
        fprintf(stderr, "Vexis Error: %s\n", print_vexis_err(err));
        return err;
    }

    err = vexis_build_enc_output(options->input_path, &enc_path);
    if (err != VEXIS_SUCC) {
        vexis_password_wipe(password, sizeof(password));
        fprintf(stderr, "Vexis Error: %s\n", print_vexis_err(err));
        return err;
    }

    out = fopen(enc_path, "wb");
    if (out == NULL) {
        vexis_password_wipe(password, sizeof(password));
        fprintf(stderr, "Vexis Error: %s\n", print_vexis_err(VEXIS_ERR_OFILE));
        free(enc_path);
        return VEXIS_ERR_OFILE;
    }
    if (fclose(out) != 0) {
        vexis_password_wipe(password, sizeof(password));
        fprintf(stderr, "Vexis Error: %s\n", print_vexis_err(VEXIS_ERR_OFILE));
        free(enc_path);
        return VEXIS_ERR_OFILE;
    }

    printf("Encryption successful: %s\n", enc_path);

    // here should store the password with the file mapped to it

    free(enc_path);
    return VEXIS_SUCC;
}


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
            return vexis_run_encrypt(&options);
        case VEXIS_CMD_DECRYPT:
            return 0;
        default:
            fprintf(stderr, "Vexis Error: %s\n", print_vexis_err(VEXIS_ERR_UNK));
            return VEXIS_ERR_UNK;
    }

    return 0;
}