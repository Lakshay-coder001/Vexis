#ifndef VEXIS_CRYPTO_H
#define VEXIS_CRYPTO_H

#include <stddef.h>
#include "vex_error.h"

void vexis_random_salt(unsigned char *salt , size_t len);

#endif
