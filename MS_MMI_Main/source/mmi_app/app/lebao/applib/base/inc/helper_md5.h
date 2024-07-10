//#pragma once
#ifndef __HELPER_MD5_H_
#define __HELPER_MD5_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "port_cfg.h"

#if defined(_MSC_VER)  &&  _MSC_VER >= 1400
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#else
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
#endif

void helper_md5(const uint8_t *initial_msg, size_t initial_len, uint8_t *digest);

#ifdef __cplusplus
}
#endif
#endif /* __MD5_H_ */

