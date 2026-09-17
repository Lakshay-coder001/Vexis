#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../include/cli.h"
#include "../include/vex_error.h"


int main(int argc, char* argv[]){
    // step1: parsing of cl command 
    vexis_err_t err = vexis_cli_parse(argc , argv);
    if(err != VEXIS_SUCC){
        fprintf(stderr, "Vexis Error: %s\n",print_vexis_err(err));
        return err;
    } //this will handle all the errors

    
    return 0;
}