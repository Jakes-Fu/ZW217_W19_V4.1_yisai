/**********************************************************************************
** File Name:      mmimsd_internal.h                                             	*
** Author:         murphy.xie                                            					*
** Date:           02/07/2012                                           					*
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.    					*
** Description:    This file declare the function about msd internal interface    *
******************************************************************************************
******************************************************************************************
**                        Edit History                                  *
** ---------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                          *
** 02/07/2012     murphy.xie       Create.                              *
*************************************************************************/
#if defined(MMI_MSD_SUPPORT)
#ifndef _MMI_MSD_INTERNAL_H_
#define _MMI_MSD_INTERNAL_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include    "guianim.h"
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
    
/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/ 
typedef enum
{
    MMIMSD_RESULT_ERROR,		//函数调用错误
    MMIMSD_RESULT_SUCCESS //函数调用成功
}MMIMSD_RESULT_E; 

typedef enum
{
    MMIMSD_CLOSE,		//关闭防盗功能
    MMIMSD_SETUP		//开启防盗功能
}MMIMSD_FUNC_E;

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_SetInfoToNv(MMIMSD_NV_INFO_T *msd_info_ptr);
/*****************************************************************************/
// 	Description : shut the phone when the tolerance arrive max.
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_MsdCheckIfPhoneStart(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
BOOLEAN MMIMSD_UpdatePsw(wchar *new_psw);

/*****************************************************************************/
// 	Description : 
//	Global resour1ce dependence :                                
//      Author: Phoebe.chen
//	Note: mode is 1 ,means active msd, else deactive msd
/*****************************************************************************/
void MMIMSD_MsdSetNewInfo(uint8* tel_num, 
					  uint8 tel_len,
                      MMIMSD_FUNC_E mode);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
BOOLEAN MMIMSD_IsPswValid(uint8* psw);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
BOOLEAN MMIMSD_TestPsw(uint8* new_psw);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
MMIMSD_RESULT_E MMIMSD_MsdSetUp(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_OpenInputPswWin(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
BOOLEAN MMIMSD_IsPswNotNull(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
BOOLEAN MMIMSD_IsPswRight(MMI_STRING_T *psw_string);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
BOOLEAN MMIMSD_IsSimLegal(void);

/*****************************************************************************/
// 	Description :
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_OpenInitInputPswWin(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_OpenShutOffWin (void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_ResetSendMsgCount(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_ClearSmsFlag(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
BOOLEAN MMIMSD_IfPswWrongTooMany(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_PswWrongPlus(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_ResetSendMsgCount(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
BOOLEAN MMIMSD_IsSimAvailable(MN_DUAL_SYS_E   dual_sys);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_SetIllegalUseFlag(BOOLEAN is_illegal);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
BOOLEAN MMIMSD_IfIllegalUse(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_SendMsgSuccessCallback(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_SendMsgFailCallback(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_ResetErrorInputPswCount(void);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :                                
//      Author: Phoebe.chen
//	Note:
/*****************************************************************************/
void MMIMSD_SendMsdMsg(void);
#endif
#endif //#if defined(MMI_MSD_SUPPORT)

