/******************************************************************************
** File Name:      mmi_autotest.h                                             *
** Author:                                                                    *
** Date:           13/05/2004                                                 *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.          *
** Description:    This file is used to describe rtos and mmi interface       *
*******************************************************************************
**                         Important Edit History                             *
** ---------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                *
** 13/05/2004       jibin          Create									  *
******************************************************************************/

/*!
 *  \addtogroup mmk_osbridge_group_label
 *  @{
 */


/*! 
 *  \file 	mmi_osbridge.h
 *  \author	bin.ji
 *  \date	13/05/2004
 *  \brief 	This file is used to describe rtos and mmi interface
 */

/*!
 * mmi_osbridge.h会被mmk_app.c包含, 如果用户修改需要重新编译mmk_app.c, @liqing.peng, 08-06-26
 */


#ifndef MMI_AUTOTEST_H
#define MMI_AUTOTEST_H

#include "sci_types.h"
#include "mmi_default.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         Module Head Define                                 */
/*      we can add our module head flag here                                  */
/*----------------------------------------------------------------------------*/ 
//BT Module
#define MMI_BT_TEST_CHAR     "BLUETOOTH:"
#define MMI_BT_TEST_WCHAR    L"BLUETOOTH:"
//FMM Module
#define MMI_FMM_TEST_CHAR    "FMM:"
#define MMI_FMM_TEST_WCHAR   L"FMM:"		
        
typedef enum
{
    MMI_AUTO_TEST_ALL_SET,// @CR247145 renwei add
    MMI_AUTO_TEST_IMAGE,
    MMI_AUTO_TEST_TEXT,
    MMI_AUTO_TEST_WIN_MSG,
    MMI_AUTO_TEST_TP,
    MMI_AUTO_TEST_KEY,
    MMI_AUTO_TEST_MSG,
    MMI_AUTO_TEST_SOFTKEY,
    MMI_AUTO_TEST_TITLE,
    MMI_AUTO_TEST_POINTER_SHAPE,
    MMI_AUTO_TEST_BACKLIGHT,    
    MMI_AUTO_TEST_MAX
}MMI_AUTO_TEST_TYPE_E;

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description:    Handle autotest information
//	Global resource dependence:
//  Author:         bin.ji
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_TraceAutoTestOfImage(
                                    MMI_IMAGE_ID_T         img_id             //in:image id
                                    );

/*****************************************************************************/
//  Description:    Handle autotest information
//	Global resource dependence:
//  Author:         bin.ji
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_TraceAutoTestOfText(
                                    wchar *text_ptr,
                                    uint16 len
                                    );

/*****************************************************************************/
// 	Description : trace auto test open win
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMI_TraceAutoTestOfWinMessage(
                                MMI_WIN_ID_T win_id,
                                MMI_MESSAGE_ID_E msg_id
                                );

/*****************************************************************************/
//  Description:    Handle autotest information
//	Global resource dependence:
//  Author:         bin.ji
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_TraceAutoTestOfTpPress(
                                       uint32 x,
                                       uint32 y,
                                       uint16 gesture_type,
                                       uint16 signalCode
                                       );

/*****************************************************************************/
//  Description:    Handle autotest information
//	Global resource dependence:
//  Author:         bin.ji
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_TraceAutoTestOfKeyPress(
                                       uint16 key_type,
                                       uint32 key_code
                                       );

/*****************************************************************************/
//  Description:    Handle autotest information
//	Global resource dependence:
//  Author:         bin.ji
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_TraceAutoTestOfMessage(
                                       MMI_TEXT_ID_T text_id
                                       );

/*****************************************************************************/
//  Description : notify pointer shape callback
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIWWW_Netfront_NotifyPointerStatesProc(int in_x,int in_y,int in_shape,int in_show, int in_nf);

/*****************************************************************************/
// 	Description : trace auto test Notify Title
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIWWW_TraceAutoTestNotifySoftkey(MMI_TEXT_ID_T left_text_id, MMI_TEXT_ID_T middle_text_id, MMI_TEXT_ID_T right_text_id);

/*****************************************************************************/
// 	Description : trace auto test Notify Title
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIWWW_TraceAutoTestNotifyTiTle(MMI_STRING_T *in_title);


/*****************************************************************************/
//  Description:    Handle autotest string
//	Global resource dependence:
//  Author:        
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_Wchar_AutoTest_Trace(
                                    wchar *trace_head_ptr,
                                    uint16 trace_head_len,							        
                                    wchar *trace_body_ptr,
                                    uint16 trace_body_len
                                    );

/*****************************************************************************/
//  Description:    Handle autotest string
//	Global resource dependence:
//  Author:        
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_Char_AutoTest_Trace(
                                    char *trace_head_ptr,
                                    uint16 trace_head_len,							        
                                    char *trace_body_ptr,
                                    uint16 trace_body_len
                                    );

/*****************************************************************************/
//  Description:    Handle autotest information
//	Global resource dependence:
//  Author:         
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_TraceAutoBackLightStatus(MMI_BACKLIGHT_STATUS_E backlight_status);

/*****************************************************************************/
//  Description:    get auto test setting items value.
//	Global resource dependence:
//  Author:         
//	Note:
/*****************************************************************************/    
PUBLIC BOOLEAN MMI_GetAtSetValue(uint8 at_type);

/*****************************************************************************/
//  Description:    set auto test setting items value.
//	Global resource dependence:
//  Author:         
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_SetAtValue(uint8 at_type, BOOLEAN is_on);


/*, @CR247145 renwei add begin*/
/*****************************************************************************/
//  Description:    set all auto test items value.
//	Global resource dependence:
//  Author:     renwei     
//	Note:
/*****************************************************************************/    
PUBLIC void MMI_SetAllAtValue(BOOLEAN is_on);
/*, @CR247145 renwei add end*/

/*****************************************************************************/
//  Description:    Handle autotest information
//	Global resource dependence:
//  Author:         bin.ji
//	Note:
/*****************************************************************************/    
PUBLIC int MMI_TraceAutoTest( // None
                             const uint8 *src_ptr,   // Pointer of the input message. 
                             uint16 src_len          // Size of the source buffer in uint8.
                             );

/*****************************************************************************/
//  Description:    handle autotest module
//	Global resource dependence:
//  Author:         James.Zhang
//	Note:
/*****************************************************************************/   
PUBLIC BOOLEAN MMI_HandleAutoTestModule(
                                        const char* module_str
                                        );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/ 
#ifdef __cplusplus   
    }
#endif

#endif
/*! @} */ 
