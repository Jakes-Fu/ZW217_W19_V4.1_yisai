

#ifndef HELIX_UTILS_H
#define HELIX_UTILS_H

#include "helix_config.h"

#ifdef __cplusplus
extern "C" {
#endif  /* #ifdef __cplusplus */


/*
 * General support macros
 *
 */

#define HX_MAX(a, b)  (((a) > (b)) ? (a) : (b))
#define HX_MIN(a, b)  (((a) < (b)) ? (a) : (b))

/*
 * Macro: IS_BIG_ENDIAN()
 *
 * Endianness detection macro.
 * Requires local test_int = 0xFF for correct operation.
 * Set ARCH_IS_BIG_ENDIAN in helix_config.h for best performance.
 * 
 */

#if !defined(ARCH_IS_BIG_ENDIAN)
#define IS_BIG_ENDIAN(test_int) ((*((char*)&test_int)) == 0)
#else
#define IS_BIG_ENDIAN(test_int) ARCH_IS_BIG_ENDIAN
#endif

/*
 * Macro: HX_GET_MAJOR_VERSION()
 *
 * Given the encoded product version,
 * returns the major version number.
 */
#define HX_GET_MAJOR_VERSION(prodVer)   ((prodVer >> 28) & 0xF)

/*
 * Macro: HX_GET_MINOR_VERSION()
 *
 * Given the encoded product version,
 * returns the minor version number.
 */
#define HX_GET_MINOR_VERSION(prodVer)   ((prodVer >> 20) & 0xFF)

/*
 * Macro: HX_ENCODE_PROD_VERSION()
 *
 * Given the major version, minor version,
 * release number, and build number,
 * returns the encoded product version.
 */
#define HX_ENCODE_PROD_VERSION(major,minor,release,build)   \
            ((ULONG32)((ULONG32)major << 28) | ((ULONG32)minor << 20) | \
            ((ULONG32)release << 12) | (ULONG32)build)

#define HX_GET_PRIVATE_FIELD(ulversion)(ulversion & (UINT32)0xFF)

#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus */


#endif /* HELIX_UTILS_H */

