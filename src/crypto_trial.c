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
            return ;
        }

    #else //linux compatibility
        File *f = fopen("/dev/urandom", "rb");
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
        fcolse(fin);
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
}