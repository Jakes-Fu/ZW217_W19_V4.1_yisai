/***************************************************************************
** File Name:      mmiwww_export.h                                                                               *
** Author:                                                                                                                    *
** Date:           15/06/2009                                                                                          *
** Copyright:      2004 Spreadtrum, Incorporated. All Rights Reserved.                             *
** Description:    This file is used to www browser internal define                                      *
****************************************************************************
**                         Important Edit History                                                                       *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                                                  *
** 06/2009        jiaoyou.wu    Create
****************************************************************************/
#ifndef  _MMIWWW_EXPORT_H_    
#define  _MMIWWW_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mn_type.h"
#include "mmibrowser_export.h"
#include "mmidl_export.h"//cr240847

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern "C"
    {
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#ifdef VM_SUPPORT 
#define MMIWWW_MEMORY_SIZE (24 * 1024 * 1024)
#else
#define MMIWWW_MEMORY_SIZE (18 * 1024 * 1024 - 512 * 1024)
#endif

#define MMIAPIWWW_StopWWW() MMIAPIWWW_StopWWWEx(FALSE)
#define MMIAPIWWW_MinimizeWWW() MMIAPIWWW_StopWWWEx(TRUE)

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
#if 0
typedef struct 
{
    wchar                   file_name[MMIFILE_FULL_PATH_MAX_LEN + 1];
    MMIBROWSER_MIME_TYPE_E  mime_type;
    char*                   url_ptr;
    void *                  user_data;
    BOOLEAN                 download_result;
}MMIBROWSER_MIME_CALLBACK_PARAM_T;/* Urge: Modify MMIDL_MIME_CALLBACK_PARAM_T simultaneously */
#else
typedef  MMIDL_MIME_CALLBACK_PARAM_T MMIBROWSER_MIME_CALLBACK_PARAM_T;
#endif
//modified for cr240847 end

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Start WWW browser
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWWW_StartWWW(const MMIBROWSER_ENTRY_PARAM * entry_param_ptr);

 /*****************************************************************************/
//  Description : Stop WWW browser
//  Global resource dependence : 
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWWW_StopWWWEx(BOOLEAN is_minimize);

 /*****************************************************************************/
//  Description : Init WWW
//  Global resource dependence : 
//  Author:jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWWW_InitWWW(void);

/*****************************************************************************/
// 	Description : register mmiwww module nv len and max item
//	Global resource dependence : none
//  Author: kun.yu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIWWW_InitModule( void );

/*****************************************************************************/
//  Description : handle www timer id
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWWW_HandleWWWTimer(uint8 timer_id, uint32 param);

/*****************************************************************************/
//  Description : Get www net setting index 
//  Global resource dependence : none
//  Author: jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC uint8 MMIAPIWWW_GetNetSettingIndex(MN_DUAL_SYS_E dual_sys);

/*****************************************************************************/
//  Description : init browser
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWWW_UninitApp(void);

/*****************************************************************************/
//  Description : check if browser is inited
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWWW_IsRunning(void);

/*****************************************************************************/
//  Description : check if browser is the top focused win
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWWW_IsActive(void);

/*****************************************************************************/
//  Description : the entry of browser 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIAPIWWW_Entry(const MMIBROWSER_ENTRY_PARAM * entry_param_ptr);

/*****************************************************************************/
//  Description : close browser 
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ERROR_E MMIAPIWWW_Close(void);

/*****************************************************************************/
//  Description : set net setting to default
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWWW_ResetFactorySetting(void);

/*****************************************************************************/
//  Description : reg mime func
//  Global resource dependence : 
//  Author: li.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIWWW_RegMimeFunc(MMIBROWSER_MIME_DOWNLOAD_PARAM_T* param_ptr);

/*****************************************************************************/
//  Description : Get browser Version 
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note:
/*****************************************************************************/
PUBLIC char* MMIAPIWWW_GetVersion(void);

/*****************************************************************************/
//  Description : Delete completed download task
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWWW_DeleteCompletedDownloadTask(uint32 task_index);

/*****************************************************************************/
//  Description : Check availability of Netfront Timer
//  Global resource dependence : 
//  Author: Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIWWW_CheckTimerDisable(void);

/*****************************************************************************/
//  Description : Set default bookmark
//  Global resource dependence : none
//  Author: dave.ruan
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIWWW_SetDefaultBookmartk(void);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    Start of Atest  Function  Declare                                                              */
/* Description:  Hanlde at cmd for browser auto test cases                                                     */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

//to do

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                                    End of Atest  Function  Declare                                                                  */
/* Description:  Hanlde at cmd for browser auto test cases                                                     */
/* Author: Shuyan.hu                                                                                                          */
/* Date: 30/12/2010                                                                                                           */ 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#ifdef BROWSER_SUPPORT_NETFRONT

/*****************************************************************************/
//     Description : insert string to sms edit
//    Global resource dependence : none
//  Author:
//    Note: 
/*****************************************************************************/
BOOLEAN MMIAPIWWW_InsertURL(                                            //RETURN:
                             wchar *wstr_ptr,
							 uint16 wstr_len
                             );
#endif
  
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    				   *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define 	MMIWWW_GetNetSettingIndex	   		MMIAPIWWW_GetNetSettingIndex

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif


