/*! @file guiiconfolder.h
@brief 此文件介绍了icon folder控件的对外接口和数据类型 
@author Jassmine
@version 1.0
@date  04/26/2004 
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note  8/2007       Jassmine              Creat
*******************************************************************************/

/*! @addtogroup iconfolderGrp IconFolder
@brief iconfolder控件
@details 
    -#  图片以文件夹形式显示\n
@{
*******************************************************************************/


#ifndef _GUIICONFOLDER_H_
#define _GUIICONFOLDER_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "ctrliconfolder_export.h"
/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef CTRLICONFOLDER_INIT_DATA_T	GUIICONFOLDER_INIT_DATA_T ;
typedef CTRLICONFOLDER_NOTIFY_T		GUIICONFOLDER_NOTIFY_T;
/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#define GUIICONFOLDER_SetBg		            CTRLICONFOLDER_SetBg
#define GUIICONFOLDER_SetRect               	     CTRLICONFOLDER_SetRect
#define GUIICONFOLDER_SetTotalIcon              CTRLICONFOLDER_SetTotalIcon
#define GUIICONFOLDER_AppendIcon               CTRLICONFOLDER_AppendIcon
#define GUIICONFOLDER_SetSelected		     CTRLICONFOLDER_SetSelected

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
