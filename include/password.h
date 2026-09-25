#ifndef VEXIS_PASSWORD_H
#define VEXIS_PASSWORD_H

#include <stddef.h>
#include "vex_error.h"

vexis_err_t vexis_password_prompt(const char *prompt, char *out, size_t out_size);

// confirms password 
vexis_err_t vexis_password_prompt_confirm(char *out, size_t out_size);

// cleans the passowrd when anything is done in-between
void vexis_password_wipe(char *buf, size_t len);

#endif
