#ifndef VEXIS_CLI_H
#define VEXIS_CLI_H
#include "vex_error.h"
#include <stdbool.h>
#include <stddef.h>

typedef enum {
    VEXIS_CMD_EMP = 0,
    VEXIS_CMD_ENCRYPT,
    VEXIS_CMD_DECRYPT,
    VEXIS_CMD_HELP,
    VEXIS_CMD_VERSION
} vexis_cmd_type;


void vexis_cli_print_help(void);

void vexis_cli_print_version(void);

vexis_err_t vexis_cli_parse(int argc, char *argv[]);

#endif