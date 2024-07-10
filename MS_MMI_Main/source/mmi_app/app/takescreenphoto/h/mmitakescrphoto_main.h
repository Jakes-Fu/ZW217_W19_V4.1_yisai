/*****************************************************************************
** File Name:      mmitakescrphoto_main.h                                           *
** Author:                                                                   *
** Date:           04/14/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe mmi file manage             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**                                   *
******************************************************************************/


#ifndef _MMI_TAKESCRPHOTO_MAIN_H_
#define _MMI_TAKESCRPHOTO_MAIN_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif



/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Type Definition                                  *
 **--------------------------------------------------------------------------*/

typedef enum
{
    MMITSCRPHOTO_ERROR_NONE,
    MMITSCRPHOTO_SDCARD_INVALID,
    MMITSCRPHOTO_SDCARD_NO_EXIST,
    MMITSCRPHOTO_SDCARD_NO_SPACE,
    MMITSCRPHOTO_SDCARD_USING,
    MMITSCRPHOTO_ERROR_MAX
}MMITSCRPHOTO_ERROR_STATUS_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//     Description : 输出lcd_buffer以及图层数据
//    Global resource dependence : 
//  Author:   bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMITSCRPHOTO_TakePhoto(void);	

/*,@CRMS00239652 renwei modify begin*/					   
/*****************************************************************************/
//  Description : check sd card memory status
//  Global resource dependence : 
//  Author:   renwei
//  Note:
/*****************************************************************************/
PUBLIC MMITSCRPHOTO_ERROR_STATUS_E MMITSCRPHOTO_PreCheck(void);


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif


#endif //_MMI_CAPTURESCREEN_H_
