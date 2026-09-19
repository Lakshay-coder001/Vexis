#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/output.h"
#include "../include/config.h"

vexis_err_t vexis_build_enc_output(const char *input_path, char **out_path) {
    size_t in_len;
    size_t ext_len;
    size_t total;
    char *out;

    if (out_path == NULL) {
        return VEXIS_ERR_GEN;
    }
    *out_path = NULL;

    if (input_path == NULL || input_path[0] == '\0') {
        return VEXIS_ERR_UNK;
    }

    in_len = strlen(input_path);
    ext_len = strlen(VEXIS_EXT);
    total = in_len + ext_len + 1;
    if (total <= in_len || total <= ext_len){
        return VEXIS_ERR_GEN;
    }

    out = (char *)malloc(total);
    if (out == NULL) {
        return VEXIS_ERR_GEN;
    }

    snprintf(out, total, "%s%s", input_path, VEXIS_EXT);
    *out_path = out;
    return VEXIS_SUCC;
}
