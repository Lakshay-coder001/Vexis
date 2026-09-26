#ifndef VEXIS_CRYPTO_H
#define VEXIS_CRYPTO_H

#include <stddef.h>
#include "vex_error.h"

// File layout: MAGIC(5) "VXS22" | SALT(16) | VERIFIER(32) | encrypted txt
// verfiier = SHA256(MAGIC || SALT || password_bytes, no NUL).
void vexis_random_salt(unsigned char *salt , size_t len);

// verifier = SHA256(magic || salt || pwd). pwd_len = strlen(password)
void vexis_verifier_compute(const char* password, size_t pwd_len, const  unsigned char* salt, unsigned char out[32]);


//creates hash at time of encryption
vexis_err_t vexis_encrypt_file(const char* in_path, const char* out_path, const char* password);


//recomputes hash and verfies and gives decrypted text
vexis_err_t vexis_decrypt_file(const char* in_path, const char* out_path, const char* password);

#endif
