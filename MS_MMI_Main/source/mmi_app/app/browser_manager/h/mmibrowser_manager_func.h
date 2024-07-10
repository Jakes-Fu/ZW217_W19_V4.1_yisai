/*****************************************************************************
** File Name:      mmibrowser_manager_func.h                                 *
** Author:          fen.xie                                                  *
** Date:           01/11/2011                                                *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe the browser window table    *
*****************************************************************************/
#ifndef MMIBROWSER_MANAGER_FUNC_H
#define MMIBROWSER_MANAGER_FUNC_H

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h" 
#include "mmi_module.h"
#include "mmi_position.h"
#include "mmidisplay_data.h"
#include "mmi_signal_ext.h"
#include "mmibrowser_export.h"

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

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct _MMIBM_CC_INFO_T_
{
    unsigned char * phone_num_ptr;
    unsigned int cc_option;
} MMIBM_CC_INFO_T;

typedef enum
{
    MMIBM_CC_OPTION_VOICE = 0x0001,
    MMIBM_CC_OPTION_IP = 0x0001 << 1,
    MMIBM_CC_OPTION_VIDEO = 0x0001 << 2,
    MMIBM_CC_OPTION_MAX
}MMIBM_CC_OPTION_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DECLARE                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : init browser manager module
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBM_InitModule(void);

/*****************************************************************************/
//  Description : create the entry param for browser manager
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBM_CreateEntryParam(const MMIBROWSER_ENTRY_PARAM entry_param);

/*****************************************************************************/
//  Description : destory the entry param for browser manager
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBM_DestoryEntryParam(void);

/*****************************************************************************/
//  Description : get the entry param for browser manager
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_ENTRY_PARAM* MMIBM_GetEntryParam(void);

/*****************************************************************************/
//  Description : get the default browser type setting
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC MMIBROWSER_BROWSER_TYPE_E MMIBM_GetDefaultBrowser(void);

/*****************************************************************************/
//  Description : set the default browser type setting
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBM_SetDefaultBrowser(MMIBROWSER_BROWSER_TYPE_E default_browser_type);

/*****************************************************************************/
//  Description : reset factory setting of browser manager
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBM_ResetFactorySetting(void);

/*****************************************************************************/
//  Description : handle app message in browser manager
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBM_HandleAppMsg(MMI_MESSAGE_ID_E msg_id,void* param);

/*****************************************************************************/
//  Description : reg mime func
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIBM_RegMimeFunc(MMIBROWSER_MIME_DOWNLOAD_PARAM_T* param_ptr);

/*****************************************************************************/
//  Description : handle download callback
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC void MMIBM_MimeDownlaodCallBack(MMIBROWSER_MIME_CALLBACK_PARAM_T* param_ptr);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
