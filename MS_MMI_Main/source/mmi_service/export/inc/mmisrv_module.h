/*****************************************************************************
** File Name:      mmisrv_module.h                                           *
** Author:         bin.ji                                                          *
** Date:           07/2012                                                   *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe mmi service module                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef _MMISRV_MODULE_H_
#define _MMISRV_MODULE_H_

#include "sci_types.h"

#ifdef __cplusplus
extern   "C" {
#endif

#define MMISRV_MAX_MODU_NUM                      64  

typedef enum MMISRV_MODULE_TYPE_E
{
	MMISRV_MODULE_SAMPLE,
    /*
    MMISRV_MODULE_COMMON,
    MMISRV_MODULE_FONT,
    MMISRV_MODULE_IM,
    MMISRV_MODULE_TP,
    MMISRV_MODULE_MMIBRW,
    MMISRV_MODULE_MMS,
    MMISRV_MODULE_SMS,
    MMISRV_MODULE_FM,
    MMISRV_MODULE_EBOOK,
    MMISRV_MODULE_MPEG4,
    MMISRV_MODULE_MULTIM,
    MMISRV_MODULE_ALARM,
    MMISRV_MODULE_GAME_MANAGER,
    MMISRV_MODULE_SET,
    MMISRV_MODULE_MP3,
    MMISRV_MODULE_CC,
    MMISRV_MODULE_PB,
    MMISRV_MODULE_STK,
    MMISRV_MODULE_ENG,
    MMISRV_MODULE_ENGTD,
    MMISRV_MODULE_RECORD,
    MMISRV_MODULE_ACC,
    MMISRV_MODULE_JAVA,
    MMISRV_MODULE_BT,
    MMISRV_MODULE_GAME_LINK,
    MMISRV_MODULE_GAME_BOXMAN, 
    MMISRV_MODULE_CONNECTION,
    MMISRV_MODULE_FMM,
    MMISRV_MODULE_OCR,
    MMISRV_MODULE_VT,
    MMISRV_MODULE_STREAMING,
    MMISRV_MODULE_GPS,
    MMISRV_MODULE_CAMERA,
    MMISRV_MODULE_KURO,    
    MMISRV_MODULE_PIC,
    MMISRV_MODULE_MTV,
    MMISRV_MODULE_DYNA_APPLIST,
    */
    
    MMISRV_MODULE_MAX_NUM
} MMISRV_MODULE_TYPE_E;


/*****************************************************************************/
//  Description : initial mmisrv modudle
//  Global resource dependence : 
//  Author:bin.ji
//  Modify:
//  Note: 所有新增模块的初始化都需要放在这里
/*****************************************************************************/
PUBLIC void MMISRV_InitMMIModule(void);

#ifdef __cplusplus
}
#endif

#endif // _MMISRV_MODULE_H_


