/*************************************************************************
** File Name:      mmimsd_export.h                                             *
** Author:         Phoebe Chen                                            *
** Date:           11/04/2004                                           *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.    *
** Description:    This file declare the function about nv             *
*************************************************************************
*************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 11/04/2004     Phoebe Chen        Create.                              *
*************************************************************************/
#if defined(MMI_MSD_SUPPORT)
#ifndef _MMI_MSD_EXPORT_H_
#define _MMI_MSD_EXPORT_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include	"guianimate.h"
#include "guianim.h"
#include    "sci_types.h"
#include    "mmk_type.h"

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
#define MMIMSD_MAX_TEL_MUM				20
#define MMIMSD_MAX_MSG_COUNT			15
#define MMIMSD_PSW_LEN					6
#define MMIMSD_TRY_TIME_MASK			0x0F
#define MMIMSD_OPEN_MASK				0xF0
#define MMIMSD_HOUR_MAX_COUNT			24	
#define	MMIMSD_MAX_SMS_TXT_LEN			160
#define	MMIMSD_MAX_SMS_DETAIL_LEN		160
#define	MMIMSD_MAX_PSW_TRY_COUNT		3
    
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/ 
typedef struct
{
    wchar psw[MMIMSD_PSW_LEN + 2];          //  default is *, valid value is from 0 to 9
    MN_IMSI_T msd_imsi[MMI_DUAL_SYS_MAX];
    uint8 relate_tel[MMIMSD_MAX_TEL_MUM];    // BCD code
    uint8 relate_tel_len;
    uint8 count;  		// SMS sending count, default is 0//告警的次数，就是发短消息的次数
    uint8 open;  		//  0xFX : opened,  0x0X: closed,  low 4 bits: for error ct// ct is err input psw times;  //THIS SERVICE STAR-up OR NOT
}MMIMSD_NV_INFO_T;

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIAPIMSD_Init(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//  Author: zhaohui
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIMSD_SetFactoryValue(void);//@zhaohui,cr95392
/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
BOOLEAN MMIAPIMSD_IsMsdStartup(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIAPIMSD_OpenSetupInputPswWin (void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIAPIMSD_SimLegalityCheck(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIAPIMSD_HourCheck(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIAPIMSD_MinuteCheck(void);

/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIAPIMSD_OpenCloseInputPswWin(void);

/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIAPIMSD_ModPswInputPswWin(void);
/**---------------------------------------------------------------------------*
 **                    Old Interface  Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIMSD_IsMsdStartup				        MMIAPIMSD_IsMsdStartup
#define MMIMSD_Init				                MMIAPIMSD_Init
#define MMIMSD_OpenSetupInputPswWin				MMIAPIMSD_OpenSetupInputPswWin
#define MMIMSD_SimLegalityCheck				    MMIAPIMSD_SimLegalityCheck

#define MMIMSD_HourCheck				            MMIAPIMSD_HourCheck
#define MMIMSD_MinuteCheck				        MMIAPIMSD_MinuteCheck
#define MMIMSD_OpenCloseInputPswWin				MMIAPIMSD_OpenCloseInputPswWin
#define MMIMSD_ModPswInputPswWin				    MMIAPIMSD_ModPswInputPswWin

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

#ifdef   __cplusplus
    }
#endif
    
#endif //_MMI_MSD_EXPORT_H_
#endif //#if defined(MMI_MSD_SUPPORT)

