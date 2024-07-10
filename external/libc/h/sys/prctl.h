/**
 * Copyright (C) 2021 Alibaba.inc, All rights reserved.
 *
 * @file   httpdns.c
 * @brief  
 * @author gaopan.gpr@alibaba-inc.com
 * @date   2021/01/15
 */

#ifndef PRCTL_H
#define PRCTL_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define PR_SET_NAME 0x00000000

int prctl(int option, ...);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PRCTL_H */