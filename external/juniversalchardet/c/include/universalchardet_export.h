/*****************************************************************************
** File Name:      universalchardet_export.c                                            *
** Author:                                                                   *
** Date:           02/2012                                                   *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe call log                    *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2012       Dafeng xu           Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#ifndef ___UNIVERSALCHARDET_EXPORT_H___
#define ___UNIVERSALCHARDET_EXPORT_H___

#include "universalchardet.h"
#include "os_api.h"

typedef enum{
    CHAR_DET_RESULT_OK = CHARDET_RESULT_OK,
    CHAR_DET_RESULT_MEM_ERR = CHARDET_RESULT_NOMEMORY,
    CHAR_DET_RESULT_INVALID_ENCODING = CHARDET_RESULT_INVALID_DETECTOR
}CHAR_DET_ENCODING_RESULT;

#ifdef __cplusplus
extern "C" {
#endif

PUBLIC CHAR_DET_ENCODING_RESULT CharDetEncoding(const uint8 *pInBuf, uint32 inBufLen, uint8 *pOutBuf, uint32 outBufLen);

#ifdef __cplusplus
};
#endif

#endif
