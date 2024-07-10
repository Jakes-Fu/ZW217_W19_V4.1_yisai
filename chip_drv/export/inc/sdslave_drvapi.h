/*******************************************************************************
 ** File Name:      sdslave_drvapi.h                                           *
 ** Author:         andy.chen                                                  *
 ** Date:           12/30/2010                                                 *
 ** Copyright:      Copyright 2001-xxxx by Spreadtrum Communications,Inc.      *
 *                  All Rights Reserved.                                       *
 **                 This software is supplied under the terms of a license     *
 **                 agreement or non-disclosure agreement with Spreadtrum.     *
 **                 Passing on and copying of this document,and communication  *
 **                 of its contents is not permitted without prior written     *
 **                 authorization.                                             *
 ** Description:    This is the header file of defines signals of the sim      *
 **                 application layer                                          *
 *******************************************************************************

 *******************************************************************************
 **                        Edit History                                        *
 ** ---------------------------------------------------------------------------*
 ** Date           Name             Description                                *
 **----------------------------------------------------------------------------*
 ** 2010.12        andy.chen         Create                                    *
 ** 20xx.xx        xxxx.xxxx         update                                    *
 ******************************************************************************/
#ifndef __SDSLAVE_DRVAPI_H__
#define __SDSLAVE_DRVAPI_H__

/*******************************************************************************
 **                        Dependencies                                        *
 ******************************************************************************/

/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/
#ifdef   __cplusplus
extern   "C"
{
#endif

/*******************************************************************************
 **                        Mcaro Definitions                                   *
 ******************************************************************************/
// IOCTL CMD
#define SDSLAVE_IOCTL_CMD_RST       1001
#define SDSLAVE_IOCTL_CMD_SENDINT   1002
/*******************************************************************************
 **                        Structures Definitions                              *
 ******************************************************************************/
struct SDSLAVE_HAL_TAG;
typedef struct SDSLAVE_HAL_TAG *SDSLAVE_HAL_HANDLE;

/*******************************************************************************
 **                        data prototype                                      *
 ******************************************************************************/

/*******************************************************************************
 **                        Function Definitions                                *
 ******************************************************************************/
PUBLIC SDSLAVE_HAL_HANDLE SDSLAVE_HAL_Open (void);
PUBLIC uint32 SDSLAVE_HAL_Close (SDSLAVE_HAL_HANDLE handle);
PUBLIC uint32 SDSLAVE_HAL_Read (SDSLAVE_HAL_HANDLE handle, uint8 *pBuf, uint32 len);
PUBLIC uint32 SDSLAVE_HAL_Write (SDSLAVE_HAL_HANDLE handle, uint8 *pBuf, uint32 len);
PUBLIC uint32 SDSLAVE_HAL_IOctl (SDSLAVE_HAL_HANDLE handle, uint32 cmd, void *arg);

/*******************************************************************************
 **                        Compiler Flag                                       *
 ******************************************************************************/
#ifdef   __cplusplus
}
#endif

#endif
