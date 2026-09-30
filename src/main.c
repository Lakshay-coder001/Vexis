#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../include/cli.h"
#include "../include/vex_error.h"
#include "../include/config.h"
#include "../include/password.h"
#include "../include/output.h"
#include "../include/crypto.h"

static vexis_err_t vexis_run_encrypt(vexis_cli_options *options){
    char password[VEXIS_PWD_MAX];
    char *enc_path = NULL;
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

    err = vexis_encrypt_file(options->input_path, enc_path, password);
    vexis_password_wipe(password, sizeof(password));
    if (err != VEXIS_SUCC) {
        fprintf(stderr, "Vexis Error: %s\n", print_vexis_err(err));
        free(enc_path);
        return err;
    }

    printf("Encryption successful: %s\n", enc_path);

    free(enc_path);
    return VEXIS_SUCC;
}

static vexis_err_t vexis_run_decrypt(vexis_cli_options *options){
    char password[VEXIS_PWD_MAX];
    vexis_err_t err;

    if (options == NULL || options->input_path == NULL || options->output_path == NULL) {
        return VEXIS_ERR_UNK;
    }

    err = vexis_password_prompt("Enter password: ", password, sizeof(password));
    if (err != VEXIS_SUCC) {
        fprintf(stderr, "Vexis Error: %s\n", print_vexis_err(err));
        return err;
    }

    err = vexis_decrypt_file(options->input_path, options->output_path, password);
    vexis_password_wipe(password, sizeof(password));
    if (err != VEXIS_SUCC) {
        fprintf(stderr, "Vexis Error: %s\n", print_vexis_err(err));
        return err;
    }

    printf("Decryption successful: %s\n", options->output_path);
    return VEXIS_SUCC;
}


int main(int argc, char* argv[]){
    // step1: parsing of cl command 
    vexis_cli_options options = {0};
    vexis_err_t err = vexis_cli_parse(argc , argv, &options);
    if(err != VEXIS_SUCC){
        fprintf(stderr, "Vexis Error: %s\n",print_vexis_err(err));
        return err;
    }

    switch (options.cmd) {
        case VEXIS_CMD_HELP:
            return 0;
        case VEXIS_CMD_VERSION:
            return 0;
        case VEXIS_CMD_ENCRYPT:
            return vexis_run_encrypt(&options);
        case VEXIS_CMD_DECRYPT:
            return vexis_run_decrypt(&options);
        default:
            fprintf(stderr, "Vexis Error: %s\n", print_vexis_err(VEXIS_ERR_UNK));
            return VEXIS_ERR_UNK;
    }

    return 0;
}