#ifndef VEXIS_OUTPUT_H
#define VEXIS_OUTPUT_H

#include "vex_error.h"

// Build "<input>.vxs" (VEXIS_EXT) into freshly malloc'd memory.
// Caller must free(*out_path). NULL/empty input -> VEXIS_ERR_UNK,
// malloc failure -> VEXIS_ERR_GEN.
vexis_err_t vexis_build_enc_output(const char *input_path, char **out_path);

#endif
