#ifndef VEXIS_SHA256_H
#define VEXIS_SHA256_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint32_t state[8];
    uint64_t bitlen;
    uint8_t data[64];
    size_t datalen;
} vexis_sha256_ctx;

void vexis_sha256_init(vexis_sha256_ctx *ctx); // this initialises the the context whole

void vexis_sha256_update(vexis_sha256_ctx *ctx, const uint8_t *data, size_t len); // this fills the input in the context
void vexis_sha256_final(vexis_sha256_ctx *ctx, uint8_t hash[32]); //this creates the hash for the input password'


void vexis_sha256(const uint8_t *data, size_t len, uint8_t hash[32]); // this just calls all the above in order

#endif
