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