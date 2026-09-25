#ifndef VEXIS_CONFIG_H
#define VEXIS_CONFIG_H

#include <stddef.h>
#include <stdint.h>

#define VEXIS_VERSION_STRING "1.0.0"
#define VEXIS_PROGRAM_NAME "vexis"
#define VEXIS_EXT ".vxs"
#define VEXIS_PWD_MAX 256


#define VEXIS_MAGIC "VXS22"
#define VEXIS_MAGIC_LEN 5
#define VEXIS_SALT_LEN 16
#define VEXIS_HASH_LEN 32
#define VEXIS_HEADER_LEN (VEXIS_MAGIC_LEN + VEXIS_SALT_LEN + VEXIS_HASH_LEN)


#endif