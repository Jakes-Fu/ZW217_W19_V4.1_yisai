/*****************************************************************************
** File Name:      mmieng_id.h                                                *
** Author:                                                                   *
** Date:           2006/05/17                                                *
** Copyright:      2006 Spreadrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe eng win and control id    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/10/09     songbin.zeng      Create                                   *
******************************************************************************/
#ifdef ENGTD_SUPPORT
#ifndef _MMIENGTD_ID_H_
#define _MMIENGTD_ID_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "mmi_module.h"

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

#define WIN_ID_DEF(win_id)          win_id
// window ID
typedef enum
{
    MMIENGTD_WIN_ID_START = (MMI_MODULE_ENGTD << 16),

#include "mmiengtd_id.def"

    MMIENGTD_MAX_WIN_ID
}MMIENGTD_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIENGTD_CTRL_ID_ID_START = MMIENGTD_MAX_WIN_ID, //(MMI_MODULE_ENGTD << 16),

    MMIENGTD_MAINMENU_CTRL_ID,   
    
   MMIENG_NETWORKSET_FORM_CTRL_ID,
   MMIENG_NETWORKSET_FORM1_CTRL_ID,
   MMIENG_NETWORKSET_FORM2_CTRL_ID,
   MMIENG_NETWORKSET_LABEL1_CTRL_ID,
   MMIENG_NETWORKSET_LABEL2_CTRL_ID,
   MMIENG_NETWORKSET_EDIT1_CTRL_ID,
   MMIENG_NETWORKSET_EDIT2_CTRL_ID,
    MMIENG_EDITBOX3_CTRL_ID,
    MMIENG_EDITBOX4_CTRL_ID,
    MMIENG_EDITBOX5_CTRL_ID,
    MMIENG_LOCKCELL_LABEL_CTRL_ID,
    MMIENG_LOCKCELL_EDITBOX_CTRL_ID,
    MMIENG_ASNSELECT_LISTBOX_CTRL_ID,
    MMIENG_DRXCYCLE_EDITBOX_CTRL_ID,
    MMIENG_UART0USEDBY_LISTBOX_CTRL_ID,
    MMIENG_APN_SETTING_CTRL_ID,
    MMIENG_VTMENU_CTRL_ID,
     MMIENG_DSPSET_FORM_CTRL_ID,
     MMIENG_DSPSET_FORM1_CTRL_ID,
     MMIENG_DSPSET_FORM2_CTRL_ID,
     MMIENG_DSPSET_FORM3_CTRL_ID,
     MMIENG_DSPSET_FORM4_CTRL_ID,
     MMIENG_DSPSET_FORM5_CTRL_ID,
     MMIENG_DSPSET_LABEL1_CTRL_ID,
     MMIENG_DSPSET_LABEL2_CTRL_ID,
     MMIENG_DSPSET_LABEL3_CTRL_ID,
     MMIENG_DSPSET_LABEL4_CTRL_ID,
     MMIENG_DSPSET_LABEL5_CTRL_ID,
     MMIENG_DSPSET_EDIT1_CTRL_ID,
     MMIENG_DSPSET_EDIT2_CTRL_ID,
     MMIENG_DSPSET_EDIT3_CTRL_ID,
     MMIENG_DSPSET_EDIT4_CTRL_ID,
     MMIENG_DSPSET_EDIT5_CTRL_ID,     
     
    MMIENG_DSP_SETTING_LISTBOX_CTRL_ID,   
    MMIENG_DSP_LISTBOX1_CTRL_ID,
    MMIENG_DSP_LISTBOX3_CTRL_ID,
    MMIENG_DSP_LISTBOX4_CTRL_ID,
    MMIENG_DSP_LISTBOX5_CTRL_ID,
    MMIENG_DSP_LISTBOX7_CTRL_ID,
    MMIENG_EDITBOX7_CTRL_ID,    
    MMIENG_ROAMPARAM_FORM_CTRL_ID,
    MMIENG_ROAMPARAM_FORM1_CTRL_ID,
    MMIENG_ROAMPARAM_FORM2_CTRL_ID,
    MMIENG_ROAMPARAM_FORM3_CTRL_ID,
    MMIENG_ROAMPARAM_LABEL1_CTRL_ID,
    MMIENG_ROAMPARAM_LABEL2_CTRL_ID,
    MMIENG_ROAMPARAM_LABEL3_CTRL_ID,
    MMIENG_ROAMPARAM_EDITBOX1_CTRL_ID,
    MMIENG_ROAMPARAM_EDITBOX2_CTRL_ID,
    MMIENG_ROAMPARAM_EDITBOX3_CTRL_ID,
    MMIENG_PICH_EDITBOX_CTRL_ID,
    MMIENG_PSBITRATE_LISTBOX_CTRL_ID,
    MMIENG_UART0BAUDRATE_LISTBOX_CTRL_ID,
    MMIENG_UART1BAUDRATE_LISTBOX_CTRL_ID,
    MMIENG_ARMLOGOUTPATHSELECT_LISTBOX_CTRL_ID,
   MMIENG_FALLBACK_LISTBOX_CTRL_ID,
   MMIENG_ANSWERFAST_LISTBOX_CTRL_ID,
   MMIENG_REQUESTMODE_LISTBOX_CTRL_ID,
   MMIENG_ADAPTERLAYER_LISTBOX_CTRL_ID,
   MMIENG_VTVIDEOSETTING_LISTBOX_CTRL_ID,
   MMIENG_L1DSPPARAM_SET_MENU_CTRL_ID,
   MMIENG_DSPPARAM_LOG_LISTBOX_CTRL_ID,
   MMIENG_DSPPARAM_FORM_CTRL_ID,
   MMIENG_DSPPARAM_FORM1_CTRL_ID,
   MMIENG_DSPPARAM_FORM2_CTRL_ID,
   MMIENG_DSPPARAM_FORM3_CTRL_ID,
   MMIENG_DSPPARAM_FORM4_CTRL_ID,
   MMIENG_DSPPARAM_LABEL1_CTRL_ID,
   MMIENG_DSPPARAM_LABEL2_CTRL_ID,
   MMIENG_DSPPARAM_LABEL3_CTRL_ID,
   MMIENG_DSPPARAM_LABEL4_CTRL_ID, 
   MMIENG_DSPPARAM_EDITBOX1_CTRL_ID,
   MMIENG_DSPPARAM_EDITBOX2_CTRL_ID,
   MMIENG_DSPPARAM_EDITBOX3_CTRL_ID,
   MMIENG_DSPPARAM_EDITBOX4_CTRL_ID,
   //MMIENG_UPLMN_EDITBOX_CTRL_ID,
   MMIENG_UPLMN_TEXTBOX_CTRL_ID,
   //MMIENG_UPLMN_LISTBOX_CTRL_ID,
   MMIENG_UPLMN_FORM_CTRL_ID,
   MMIENG_UPLMN_FORM1_CTRL_ID,
   MMIENG_UPLMN_FORM2_CTRL_ID,
   MMIENG_UPLMN_FORM3_CTRL_ID,
   MMIENG_UPLMN_LABEL1_CTRL_ID,
   MMIENG_UPLMN_LABEL2_CTRL_ID,
   MMIENG_UPLMN_LABEL3_CTRL_ID,
   MMIENG_UPLMN_EDITBOX1_CTRL_ID,
   MMIENG_UPLMN_EDITBOX2_CTRL_ID,
   MMIENG_UPLMN_EDITBOX3_CTRL_ID,
   
   //@yongwei.he add for dyna eng mode 2007-05-18
   MMIENG_DYNA_MENU_CTRL_ID,
   MMIENG_DYNA_ITEM_LIST_LISTBOX_CTRL_ID,
   MMIENG_DYNA_PARAM_VALUE_TEXTBOX_CTRL_ID,
   MMIENG_DYNA_VALUE_LIST_LISTBOX_CTRL_ID,
   MMIENG_DYNA_VALUE_EDITBOX_CTRL_ID,

   MMIENG_UART0MIXMODE_LISTBOX_CTRL_ID,//@zhaohui,cr106076
   MMIENG_SUPPORT_GSMSIM_LISTBOX_CTRL_ID,//@zhaohui,cr109170     
    MMIENGTD_MAX_CTRL_ID
}MMIENGTD_CONTROL_ID_E;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

#endif// ENGTD_SUPPORT

