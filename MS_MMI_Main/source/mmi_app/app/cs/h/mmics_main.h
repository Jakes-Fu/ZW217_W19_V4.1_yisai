
/***************************************************************************
** File Name:      mmidcdmain.h                                                *
** Author:                                                                 *
** Date:           20/09/2007                                              *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 09/2007        violent.wei      Create
****************************************************************************/
#ifndef  _MMICS_MAIN_H_    
#define  _MMICS_MAIN_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mn_type.h"
#include "dal_time.h"

#ifdef CMCC_UI_STYLE
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif


#define CS_DEBUG_ENABLE
#define MODULE_CS    0x22

#define MMICS_MAX_DETAIL_STR_LEN              1536

#define MMICS_TEXT_MAX_LEN			50		    // 日程表的最大长度


#ifdef CS_DEBUG_ENABLE
#define CS_TRACE_LOW( _format_string )   SCI_TRACE_LOW _format_string
#else
#define CS_TRACE_LOW( _format_string )  
#endif

typedef void(*CS_SigDispatch)(uint16 sig_code, uint16 para);

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/               

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/



typedef struct _MMICS_SETTING_INFO_T  
{
    uint16   name_string[MMICS_TEXT_MAX_LEN];								
    uint16   callnum_string[MMICS_TEXT_MAX_LEN];
    uint16   hotline_string[MMICS_TEXT_MAX_LEN];
} MMICS_SETTING_INFO_T;



/**--------------------------------------------------------------------------*
 **                         PUBLIC FUNCTION DECLARE                          *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description :
//	Global resource dependence : none
//  Author:violent.wei
//	Note:
/*****************************************************************************/
PUBLIC void MMICS_RegMenuGroup(void);
/*****************************************************************************/
// 	Description :
//	Global resource dependence : 
//  Author:violent.wei
//	Note: 
/*****************************************************************************/
PUBLIC void MMICS_RegNv(void);

/****************************************************************************/
//  Description: 
//  Global resource dependence
//  Auther: 
//  Note:
/****************************************************************************/
PUBLIC MMICS_SETTING_INFO_T*  MMICS_GeSettingInfo(void);

/****************************************************************************/
//  Description: 设置信号分发函数
//  Global resource dependence
//  Auther: jun.hu
//  Note: 2009/8/26
/****************************************************************************/
PUBLIC void  MMICS_SetSigDispatchFun(CS_SigDispatch cs_sig);
/****************************************************************************/
//  Description: 获取信号分发函数
//  Global resource dependence
//  Auther: jun.hu
//  Note: 2009/8/26
/****************************************************************************/
PUBLIC CS_SigDispatch  MMICS_GetSigDispatchFun(void);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/





/*****************************************************************************/
//  Description :  dcd send sig to mmi
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
PUBLIC void MMICS_Sendsig(uint16 signal_code,uint16 para);

/*****************************************************************************/
// 	Description : write setting info to nv
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/
PUBLIC void MMICS_WriteSettingToNv(void);

/*****************************************************************************/
// 	Description : read setting info from nv
//	Global resource dependence : none
//  Author:byte.guo
//	Note:
/*****************************************************************************/
PUBLIC void MMICS_SetSettingDefault(void);
/****************************************************************************
   Description : DCD task init handler
   Global resource dependence : 
   Author:Zhuo.Wang
   Note: 
            first set some param
            parser default xml file in fs

 ****************************************************************************/
PUBLIC void MMICS_InitStart(void);

#ifdef __cplusplus
    }
#endif

#endif /*#ifdef MMI_ENABLE_DCD*/   
#endif /*#ifdef _MMICS_MAIN_H_*/ 
