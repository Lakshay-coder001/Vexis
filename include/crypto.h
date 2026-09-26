#ifndef VEXIS_CRYPTO_H
#define VEXIS_CRYPTO_H

#include <stddef.h>
#include "vex_error.h"

void vexis_random_salt(unsigned char *salt , size_t len);

vexis_err_t vexis_encrypt_file(const char* in_path, const char* out_path, const char* password);

vexis_err_t vexis_decrypt_file(const char* in_path, const char* out_path, const char* password);

#endif
