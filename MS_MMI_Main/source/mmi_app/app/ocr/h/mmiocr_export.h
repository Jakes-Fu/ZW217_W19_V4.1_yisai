/*****************************************************************************
** File Name:     mmiocr_export.h                                                   *
** Author:          jian.ma                                                          *
** Date:           6/2007                                                *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe t9 text input               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 6/2007      jian.ma            Create
******************************************************************************/

#ifndef _MMIOCR_EXPORT_H_
#define _MMIOCR_EXPORT_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
//#include "mmipb_mnapi.h"
#include "mmipb_adapt.h"

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
#define OCR_SIGNAL_GROUP	0xdf
#define MMIOCR_MAX_SIZE			(2*1024*1024)
/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
// ocr signal types
typedef enum{
	OCR_LAYOUT = (OCR_SIGNAL_GROUP<<8) | 1,
	OCR_RECOG,
	OCR_MAX = (OCR_SIGNAL_GROUP<<8) |0xff
}OCR_MSG_E;

typedef struct _MMIOCR_INFO_ENTRY_T
{
    MMIPB_NAME_T			entry_name;                         // 姓名
    uint8					personal_number[MMIPB_BCD_NUMBER_MAX_LEN];   //手机
    uint8					company_number[MMIPB_BCD_NUMBER_MAX_LEN];  //公司电话
	uint8					home_number[MMIPB_BCD_NUMBER_MAX_LEN];//住宅电话
	MMIPB_MAIL_T			mail;                               // 邮箱地址
	MMIPB_CUSTOM_TEXT_T		custom_text;						// 自定义文字
    MMIPB_TITLE_T           title;                              // 职务
    MMIPB_ORG_T             org;                                // 公司名称
    MMIPB_WORK_ADR_T        work_adr;                           // 工作地址
} MMIOCR_INFO_ENTRY_T;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         INTERFACE TO GUI                                      *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : open preview window
//	Global resource dependence : none
//  Author: jian.ma 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOCR_OpenPreviewWin(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: Annie.an
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIOCR_Init(void);


/*****************************************************************************/
// 	Description : handle the timer msg of OCR window
//	Global resource dependence : none
//  Author: jian.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOCR_HandleProcessTimer (uint8 timer_id,  uint32 param );

/*****************************************************************************/
// 	Description : close ocr preview win
//	Global resource dependence : 
//  Author: jian.ma
//	Note:	
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIOCR_ClosePreviewWinOpen(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
