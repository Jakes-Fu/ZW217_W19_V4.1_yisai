/*****************************************************************************
** File Name:      atest_pb.h                                                *
** Author:          shuyan.hu                                                         *
** Date:           06/12/2010                                                *
** Copyright:      2001 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    This file is used to describe pb win and control id       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
tony      Create                                    *
******************************************************************************/

#ifndef _ATEST_PB_H
#define _ATEST_PB_H 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"
#include "stdio.h"
#include "atest_signal.h"


#ifdef WIN32
/**--------------------------------------------------------------------*
 **                         Compiler Flag                              *
 **--------------------------------------------------------------------*/

#define  MAX_FILE_LEN       255
#define  MAX_LEN_T          400
#define  MAX_CHARACTER_NUM    280
#define  MAX_CHINESE_NUM      20500
#define  MAX_ARR_CHARACTER_NUM  10
#define MAX_PB_PATH 260



/**-----------------------------------------------------------------------*
 **                                Function Prototypes                    *
 **-----------------------------------------------------------------------*/
PUBLIC void Atest_PbGetConfigPath(ATEST_APP_DATA_T *pb_signal_ptr);
PUBLIC void atest_mmipbcode();
PUBLIC void atest_mmipb_fl();
PUBLIC void atest_generatecases();
PUBLIC void Atest_PbGetPathandNum(ATEST_APP_DATA_T *pb_signal_ptr);


#endif//WIN32

#endif //_ATEST_PB_H
