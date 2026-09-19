#ifndef VEXIS_PASSWORD_H
#define VEXIS_PASSWORD_H

#include <stddef.h>
#include "vex_error.h"

// Single password prompt. Reads one line from stdin, strips CR/LF.
// Empty line or over-long line -> VEXIS_ERR_PWD. Buffer always NUL-terminated.
vexis_err_t vexis_password_prompt(const char *prompt, char *out, size_t out_size);

// Double prompt: "Enter password" + "Confirm password".
// Mismatch/empty -> VEXIS_ERR_PWD. Only `out` keeps the password on success.
vexis_err_t vexis_password_prompt_confirm(char *out, size_t out_size);

// Best-effort wipe (prevents -O2 from optimizing the memset away).
void vexis_password_wipe(char *buf, size_t len);

#endif
