#ifndef COPY_KERNEL_IMG_H
#define COPY_KERNEL_IMG_H

#include "sci_types.h"

#define VOLUME_NUMBER       0
#ifdef VECTOR_TABLE_ADDR_HIGH
/* address define according to kernel region base in scf file*/
#define SDRAM_BASE_ADDR     0x400000
#else
#define SDRAM_BASE_ADDR     0x0
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/******************************************************************************
 * copy_kernel_img
 *
 * Return values
 *      0 - The operation failed.
 *      1 - OK.
 ******************************************************************************/
BOOLEAN LoadImg(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* COPY_KERNEL_IMG_H */