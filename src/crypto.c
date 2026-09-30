#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/crypto.h"
#include "../include/config.h"
#include "../include/sha256.h"
#ifdef _WIN32
#include <windows.h>
#include <wincrypt.h>
#endif

void vexis_random_salt(unsigned char* salt , size_t len){
    size_t i;
    #ifdef _WIN32 //window compatibility
        HCRYPTPROV prov = 0;
        if(CryptAcquireContext(&prov, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT)){
            if(CryptGenRandom(prov,(DWORD)len, salt)){
                CryptReleaseContext(prov , 0);
                return ;
            }
            CryptReleaseContext(prov , 0);
        }

    #else //linux compatibility
        FILE *f = fopen("/dev/urandom", "rb");
        if(f != NULL){
            if(fread(salt , 1, len , f) == len){
                fclose(f);
                return;
            }
            fclose(f);
        }
    #endif
        srand((unsigned)time(NULL)^ (unsigned)clock());
        for(int i = 0 ; i<len ; i++){
            salt[i] = (unsigned char)(rand() & 0xFF);
        }
}

void vexis_verifier_compute(const char* password, size_t pwd_len, const  unsigned char* salt, unsigned char out[32]){
    vexis_sha256_ctx ctx;
    vexis_sha256_init(&ctx);
    vexis_sha256_update(&ctx, (const uint8_t)* VEXIS_MAGIC, VEXIS_MAGIC_LEN);
    vexis_sha256_update(&ctx, salt , 16);
    vexis_sha256_update(&ctx , (const uint8_t)* password, pwd_len);

    vexis_sha256_final(&ctx, out);
    memset(&ctx, 0, sizeof(ctx));
}


static void keystream_block(const unsigned char verifier[32], uint64_t counter,unsigned char ks[32]){
    uint8_t cntr[8];
    int i ;
    vexis_sha256_ctx ctx;

    for(i = 0 ; i<8 ; i++){
        cntr[i] = (uint8_t)(counter >>(i*8)); //this is little endian conversion
    }

    vexis_sha256_init(&ctx);
    vexis_sha256_update(&ctx, verifier, VEXIS_HASH_LEN);
    vexis_sha256_update(&ctx, cntr, 8);
    vexis_sha256_final(&ctx, ks);
    memset(&ctx, 0, sizeof(ctx));
}


static vexis_err_t crypt_body(FILE *fin, FILE *fout,const unsigned char verifier[32]){
    static const size_t chunk = 4096;
    unsigned char *buf = malloc(chunk);
    unsigned char ks[32];
    uint64_t block = 0;
    size_t key_pos = 0;
    size_t n, i;

    if(buf == NULL){
        return VEXIS_ERR_GEN;
    }

    keystream_block(verifier, block , ks);

    for(;;){
        n = fread(buf , 1, chunk , fin);
        if(n == 0){
            if(ferror(fin)){
                memset(ks, 0 ,sizeof(ks));
                free(buf);
                return VEXIS_ERR_IFILE;
            }
            break;
        }
        for(int i = 0 ; i<n  ; i++){
            if(key_pos == 32){
                key_pos = 0;
                block++;
                keystream_block(verifier, block , ks);
            }
            buf[i] ^= ks[key_pos];
            key_pos++;
        }

        if(fwrite(buf , 1 , n , fout) != n){
            memset(ks, 0, sizeof(ks));
            free(buf);
            return VEXIS_ERR_OFILE;
        }

        if(n<chunk && feof(fin)){
            break;
        }   
    }
    memset(ks, 0, sizeof(ks));
    free(buf);
    return VEXIS_SUCC;
}


vexis_err_t vexis_encrypt_file(const char* in_path, const char* out_path, const char* password){
    FILE *fin = NULL;
    FILE *fout = NULL;
    unsigned char salt[VEXIS_SALT_LEN];
    unsigned char verifier[VEXIS_HASH_LEN];
    vexis_err_t err;

    if (in_path == NULL || out_path == NULL || password == NULL) {
        return VEXIS_ERR_GEN;
    }
    fin = fopen(in_path , "rb");
    fout = fopen(out_path , "wb");
    if(fin == NULL){
        return VEXIS_ERR_IFILE;
    }
    if(fout == NULL){
        fclose(fin);
        return VEXIS_ERR_OFILE;
    }

    vexis_random_salt(salt, sizeof(salt));

    vexis_verifier_compute(password, strlen(password), salt, verifier);

    //now to write magic salt and verifier in file header
    if(fwrite(VEXIS_MAGIC , 1 ,VEXIS_MAGIC_LEN, fout) != VEXIS_MAGIC_LEN  || fwrite(salt, 1, sizeof(salt), fout) != sizeof(salt)   || fwrite(verifier , 1 , sizeof(verifier), fout) != sizeof(verifier)){
        err = VEXIS_ERR_OFILE;
        memset(salt, 0 , sizeof(salt));
        memset(verifier , 0, sizeof(verifier));
        if (fin != NULL) {
        fclose(fin);
        }
        if (fout != NULL) {
            if (fclose(fout) != 0 && err == VEXIS_SUCC) {
                err = VEXIS_ERR_OFILE;
            }
        }
        return err;
    }

    //now encrypt the body
    err = crypt_body(fin, fout, verifier);
    memset(verifier, 0, sizeof(verifier));
    memset(salt, 0, sizeof(salt));
    if (fin != NULL) {
        fclose(fin);
    }
    if (fout != NULL) {
        if (fclose(fout) != 0 && err == VEXIS_SUCC) {
            err = VEXIS_ERR_OFILE;
        }
    }
    return err;
}


static int calc_equal(const unsigned char *a, const unsigned char *b, size_t len) {
    unsigned char diff = 0;
    size_t i;
    for (i = 0; i < len; i++) {
        diff |= (unsigned char)(a[i] ^ b[i]);
    }
    return diff == 0;
}

vexis_err_t vexis_decrypt_file(const char *in_path, const char *out_path,const char *password){
    FILE *fin = NULL;
    FILE *fout = NULL;
    unsigned char magic[VEXIS_MAGIC_LEN];
    unsigned char salt[VEXIS_SALT_LEN];
    unsigned char stored[VEXIS_HASH_LEN];
    unsigned char calc[VEXIS_HASH_LEN];
    vexis_err_t err;

    if(in_path == NULL || out_path == NULL || password == NULL){
        return VEXIS_ERR_GEN;
    }

    fin = fopen(in_path, "rb");
    if(fin == NULL){
        err = VEXIS_ERR_IFILE;
        return err;
    }
    
    if(fread(magic, 1, VEXIS_MAGIC_LEN ,fin) != VEXIS_MAGIC_LEN || fread(salt, 1, VEXIS_SALT_LEN, fin) != VEXIS_SALT_LEN || fread(stored, 1, VEXIS_HASH_LEN , fin) != VEXIS_HASH_LEN){
        fclose(fin);
        err = VEXIS_ERR_CRYP;
        return err;
    }

    if(memcmp(magic , VEXIS_MAGIC , VEXIS_MAGIC_LEN) != 0){
        fclose(fin);
        memset(salt, 0, sizeof(salt));
        memset(stored, 0, sizeof(stored));
        err = VEXIS_ERR_CRYP;
        return err;
    }

    vexis_verifier_compute(password, strlen(password), salt, calc);

    if(!calc_equal(calc, stored , sizeof(stored))){
        fclose(fin);
        memset(calc, 0, sizeof(calc));
        memset(stored, 0, sizeof(stored));
        memset(salt, 0, sizeof(salt));
        return VEXIS_ERR_PWD;
    }

    fout = fopen(out_path , "wb");
    if(fout == NULL){
        fclose(fin);
        memset(calc, 0, sizeof(calc));
        memset(stored, 0, sizeof(stored));
        memset(salt, 0, sizeof(salt));
        return VEXIS_ERR_OFILE;
    }

    err = crypt_body(fin, fout , calc);
    memset(calc, 0, sizeof(calc));
    memset(stored, 0, sizeof(stored));
    memset(salt, 0, sizeof(salt));

    fclose(fin);
    if(fclose(fout) != 0 && err == VEXIS_SUCC){
        err = VEXIS_ERR_OFILE;
    }
    return err;
    
}