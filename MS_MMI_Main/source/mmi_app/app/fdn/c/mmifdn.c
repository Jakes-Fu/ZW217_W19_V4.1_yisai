/******************************************************************************
** File Name:      mmifdn.c                                    *
** Author:                                                                    *
** Date:           09/07/2009                                                 *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.        *
** Description:    
*******************************************************************************
**                         Important Edit History                             *
** -------------------------------------------------------------------------- *
** DATE           NAME             DESCRIPTION                                *
** 09/2009        wancan.you        Create                                    *
******************************************************************************/
#define _MMIFDN_C_  

/**--------------------------------------------------------------------------*
 **                         Dependencies                                     *
 **--------------------------------------------------------------------------*/
#include "mmi_app_fdn_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmk_app.h"
#include "mmifdn_export.h"
#include "mmipb_export.h"
#include "mmi_text.h"
#include "mn_type.h"
#ifdef FDN_SUPPORT
#include "sig_code.h"
#endif
#include "mmi_id.h"
#include "mmipub.h"
#include "mmipb_export.h"
#include "mmipb_id.h"
#include "mmiudisk_export.h"
#include "mmi_appmsg.h"
#include "mmi_default.h"
#ifdef MMI_IDLE_FDN_ICON_SUPPORT
#include "mmiidle_statusbar.h"
#endif
#include "os_api.h"

#ifdef FDN_SUPPORT
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
MMI_APPLICATION_T g_mmifdn_app;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL MMIFDN_CONTEXT_T s_fdn_context[MMI_DUAL_SYS_MAX];

LOCAL MN_DUAL_SYS_E s_fdn_dual_sys;

LOCAL BOOLEAN s_is_rereadphonebook = FALSE;
/**--------------------------------------------------------------------------*
 **                         LOCAL FUNCTION DECLARE                           *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : Handle FDN Message
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFDNPsMsg(PWND app_ptr, uint16 msg_id, DPARAM param);

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_FDN_SERVICE_IND
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFDNServiceCnf(DPARAM param);

/*****************************************************************************/
//Description : deal with signal of APP_MN_FDN_CAPABILITY_IND
//Global resource dependence : 
//Author: CBK
//Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFDNCapabilityCnf(DPARAM param);

/*****************************************************************************/
// 	Description : handle APP_MN_FDN_UPDATE_CNF
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFDNUpdateCnf(DPARAM param);

/**--------------------------------------------------------------------------*
 **                         Constant Variables                               *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : set reread phonebook state
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFDN_SetReReadPhoneBookState(BOOLEAN is_reread)
{
    s_is_rereadphonebook = is_reread;
}

  /*****************************************************************************/
//  Description : get reread phonebook state
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFDN_GetReReadPhoneBookState(void)
{
    return s_is_rereadphonebook;
}

/*****************************************************************************/
//  Description : set operate fdn SIM id
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFDN_SetCurSIM(MN_DUAL_SYS_E dual_sys)
{
    s_fdn_dual_sys = dual_sys;
}

 /*****************************************************************************/
//  Description : get operate fdn SIM id
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIFDN_GetCurSIM(void)
{
    return s_fdn_dual_sys;
}
/*****************************************************************************/
//  Description : FDN Initial
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFDN_Init(void)
{
    SCI_MEMSET(&s_fdn_context, 0x00, (sizeof(MMIFDN_CONTEXT_T) * MMI_DUAL_SYS_MAX));

    g_mmifdn_app.ProcessMsg = HandleFDNPsMsg;
}

/*****************************************************************************/
// 	Description : Handle FDN Message
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFDNPsMsg(PWND app_ptr, uint16 msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

    switch (msg_id)
    {
    case APP_MN_FDN_SERVICE_IND:
        //SCI_TRACE_LOW:"HandleFDNPsMsg:receive APP_MN_FDN_SERVICE_IND"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFDN_144_112_2_18_2_19_14_0,(uint8*)"");
        result = HandleFDNServiceCnf(param);
        break;
#ifdef MMI_COMPILE_ERROR
    /* fix compile error, modify by zhigang.peng ,2019-07-19 */
    case APP_MN_FDN_CAPABILITY_IND:
        SCI_TRACE_LOW("HandleFDNPsMsg:receive APP_MN_FDN_CAPABILITY_IND");
        result = HandleFDNCapabilityCnf(param);
        break;
#endif
    case APP_MN_FDN_UPDATE_CNF:
        result = HandleFDNUpdateCnf(param);
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

    return result;
}

/*****************************************************************************/
// 	Description : deal with signal of APP_MN_FDN_SERVICE_IND
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFDNServiceCnf(DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    APP_MN_FDN_SERVICE_IND_T *fdn_service_cnf = (APP_MN_FDN_SERVICE_IND_T*)param;//this signal indicate fdn enable/disable to MMI
    BOOLEAN is_fdn_enable = FALSE;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    if(PNULL == fdn_service_cnf)
    {
        //SCI_TRACE_LOW:"HandleFDNServiceCnf  fdn_service_cnf is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFDN_173_112_2_18_2_19_14_1,(uint8*)"");
        return MMI_RESULT_FALSE;
    }
    is_fdn_enable = fdn_service_cnf->is_fdn_enable;
    dual_sys = fdn_service_cnf->dual_sys;
    
    //SCI_TRACE_LOW:"HandleFDNServiceCnf:is_fdn_enable=%d, dual_sys=%d, oper_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFDN_179_112_2_18_2_19_14_2,(uint8*)"ddd", is_fdn_enable, dual_sys, s_fdn_context[dual_sys].oper_type);

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"HandleFDNServiceCnf:error dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFDN_183_112_2_18_2_19_14_3,(uint8*)"d", dual_sys);
        return MMI_RESULT_FALSE;
    }    
    if (is_fdn_enable)
    {
        MMIAPIFDN_InitFDNInfo(dual_sys);

        s_fdn_context[dual_sys].is_fdn_enable = TRUE;

        if (MMIFDN_ENABLE_OPER == s_fdn_context[dual_sys].oper_type)
        {
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin( TXT_COMPLETE);
#else
            MMIPUB_OpenAlertSuccessWin( TXT_SUCCESS);	
#endif
            /*2G中从关闭状态到开启fdn成功，需释放ADN电话本*/
            if (!s_fdn_context[dual_sys].is_usim)
            {
                // 初始化NV中的电话记录,因为需要对PB记录重新排序
                MMIAPIPB_ResortPB();//PB相关全局变量重新初始化，以及对NV排序列表重新初始化
            }
        }
        else if (MMIFDN_DISABLE_OPER == s_fdn_context[dual_sys].oper_type)
        {
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertWarningWin( TXT_COMPLETE);
#endif
        }
        else
        {
            //do nothing
        }
    }
    else
    {
        s_fdn_context[dual_sys].is_fdn_enable = FALSE;

        if (MMIFDN_ENABLE_OPER == s_fdn_context[dual_sys].oper_type)
        {
            MMIPUB_OpenAlertWarningWin( TXT_ERROR);
        }
        else if  (MMIFDN_DISABLE_OPER == s_fdn_context[dual_sys].oper_type)
        {
#ifdef MMI_GUI_STYLE_TYPICAL
            MMIPUB_OpenAlertSuccessWin( TXT_COMPLETE);
#else
            MMIPUB_OpenAlertSuccessWin( TXT_SUCCESS);
#endif

            /*2G中从开启状态到关闭fdn成功，需要重新读取ADN电话本*/
            if (!s_fdn_context[dual_sys].is_usim)
            {
//#ifndef WIN32
#ifdef MMI_DUALMODE_ENABLE      //与双模USIM卡有关
                //PHONEBOOK_ReReadPhoneBook();
                MMIAPIPB_ResortPB();
#else
                s_is_rereadphonebook = TRUE;
                PHONEBOOK_ReReadPhoneBookEx(dual_sys);
                MMIAPIPB_ReinitPB(dual_sys);
#endif
//#endif
            }
        }
        else
        {
            //do nothing
        }
    }

    MMK_CloseWin(FDN_PIN2_WIN_ID);
    // 关闭等待窗口
    MMK_CloseWin(PHONE_PIN_WAIT_WIN_ID);

#ifdef MMI_IDLE_FDN_ICON_SUPPORT
    MAIN_SetFDNState(TRUE);
#endif

    return (result);
}

/*****************************************************************************/
//Description : deal with signal of APP_MN_FDN_CAPABILITY_IND
//Global resource dependence : 
//Author: CBK
//Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFDNCapabilityCnf(DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;    
    APP_MN_FDN_CAPABILITY_IND_T* fdn_capablt_cnf = (APP_MN_FDN_CAPABILITY_IND_T*)param;

    if(PNULL == fdn_capablt_cnf)
    {
        SCI_TRACE_LOW("HandleFDNCapabilityCnf  fdn_service_cnf is NULL");        
        return MMI_RESULT_FALSE;
    }

#if 0
    SCI_TRACE_LOW("HandleFDNCapabilityCnf: dual_sys=:%d, is_fdn_enable=:%d, is_fdn_exist=%d",
        fdn_capablt_cnf->dual_sys, fdn_capablt_cnf->is_fdn_enable, fdn_capablt_cnf->is_fdn_exist);
    
    if(fdn_capablt_cnf->dual_sys >= MMI_DUAL_SYS_MAX)
    {
        return MMI_RESULT_FALSE;
    }    
    s_fdn_context[fdn_capablt_cnf->dual_sys].is_fdn_exist = fdn_capablt_cnf->is_fdn_exist;
#endif
    return result;
}

/*****************************************************************************/
// 	Description : handle APP_MN_FDN_UPDATE_CNF
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleFDNUpdateCnf(DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    BOOLEAN is_done = TRUE;
    MMIPB_ERROR_E pb_error = MMIPB_ERROR_ERROR;
    MMI_TEXT_ID_T text_id = TXT_NULL;
    MMI_IMAGE_ID_T image_id = IMAGE_NULL;
    //MMIPB_ALERT_INFO_T alert_info_t = {0};
    APP_MN_FDN_UPDATE_CNF_T *fdn_update_cnf_ptr = (APP_MN_FDN_UPDATE_CNF_T *)param;
    MN_FDN_OPERATE_TYPE_E operate_type = MN_FDN_UPDATE;
    BOOLEAN is_ok = FALSE;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    
    if(PNULL == fdn_update_cnf_ptr)
    {
        //SCI_TRACE_LOW:"HandleFDNUpdateCnf  fdn_update_cnf_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFDN_279_112_2_18_2_19_15_4,(uint8*)"");
        return MMI_RESULT_FALSE;
    }

    dual_sys = fdn_update_cnf_ptr->dual_sys;
    operate_type = fdn_update_cnf_ptr->operate_type;
    is_ok = fdn_update_cnf_ptr->is_ok;
    s_fdn_context[dual_sys].entry_id = fdn_update_cnf_ptr->entry_id;
    //SCI_TRACE_LOW:"HandleFDNUpdateCnf operate_type=%d, dual_sys=%d, is_ok=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFDN_287_112_2_18_2_19_15_5,(uint8*)"ddd", operate_type, dual_sys, is_ok);

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"HandleFDNUpdateCnf:error dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFDN_291_112_2_18_2_19_15_6,(uint8*)"d", dual_sys);
        return MMI_RESULT_FALSE;
    }
    switch (operate_type)
    {
    case MN_FDN_UPDATE:
        if (is_ok)
        {
            text_id = TXT_COMPLETE;
            image_id = IMAGE_PUBWIN_SUCCESS;
            pb_error = MMIPB_ERROR_SUCCESS;
            if(!s_fdn_context[dual_sys].entry_state[s_fdn_context[dual_sys].entry_id - 1])
            {
                //新增
                s_fdn_context[dual_sys].entry_state[s_fdn_context[dual_sys].entry_id - 1] = TRUE;
                s_fdn_context[dual_sys].fdn_info.used_record_num++;
            }
        }
        else
        {
            text_id  = TXT_ERROR;
            image_id = IMAGE_PUBWIN_WARNING;
            pb_error = MMIPB_ERROR_ERROR;            
        }
        MMIAPIPB_CmdConfirm(pb_error);
        break;
        
    case MN_FDN_ERASE:
        if (is_ok)
        {
            //成功删除，改变相应标志位
            s_fdn_context[dual_sys].entry_state[s_fdn_context[dual_sys].entry_id - 1] = FALSE;//相应标志位置零  
            s_fdn_context[dual_sys].fdn_info.used_record_num--;

            text_id = TXT_COMPLETE;
            image_id = IMAGE_PUBWIN_SUCCESS;
            pb_error = MMIPB_ERROR_SUCCESS;
        }
        else
        {
            text_id  = TXT_ERROR;
            image_id = IMAGE_PUBWIN_WARNING;
            pb_error = MMIPB_ERROR_ERROR;
        }
        MMIAPIPB_CmdConfirm(pb_error);
        break;
        
    default:
        is_done = FALSE;
        SCI_TRACE_LOW("HandleFDNUpdateCnf operate_type = %d", operate_type);
        break;
    }
    
    if (is_done)
    {
        if (!MMIAPIUdisk_VUARTIsRun())
        {
#ifndef MMI_GUI_STYLE_TYPICAL
            if (IMAGE_PUBWIN_SUCCESS != image_id)
#endif
            {
                MMIPUB_OpenAlertWinByTextId(PNULL,text_id,TXT_NULL,image_id,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
        }

        MMIAPIPB_UpdateSIMDNList();

        //turn on the back light
        MMIDEFAULT_SetBackLight(TRUE);
        MMK_CloseWin(MMIPB_WAITING_ALERT_WIN_ID);
    }
    return (result);
}

/*****************************************************************************/
// 	Description : Handle FDN Message
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIFDN_HandleCallReadyInd(DPARAM param)
{
    APP_MN_CALL_READY_IND_T *call_ready_param_ptr = 0;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;
    BOOLEAN is_fdn_enable = FALSE;
    BOOLEAN is_adn_enable = FALSE;
    BOOLEAN is_usim = FALSE;

    call_ready_param_ptr = (APP_MN_CALL_READY_IND_T *)param;

    if(PNULL == call_ready_param_ptr)
    {
        //SCI_TRACE_LOW:"MMIAPIFDN_HandleCallReadyInd  call_ready_param_ptr is NULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFDN_392_112_2_18_2_19_15_7,(uint8*)"");
        return;
    }

    is_fdn_enable = call_ready_param_ptr->fdn_is_enable;
#ifdef MMI_DUALMODE_ENABLE
    is_adn_enable = call_ready_param_ptr->adn_is_enable;
    is_usim = call_ready_param_ptr->is_usim;
#endif
    dual_sys = call_ready_param_ptr->dual_sys;

    //SCI_TRACE_LOW:"MMIAPIFDN_HandleCallReadyInd dual_sys=%d,is_fdn_enable=%d,is_adn_enable=%d,is_usim=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFDN_404_112_2_18_2_19_15_8,(uint8*)"dddd",dual_sys, is_fdn_enable, is_adn_enable, is_usim);

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPIFDN_HandleCallReadyInd:error dual_sys=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFDN_408_112_2_18_2_19_15_9,(uint8*)"d", dual_sys);
        return;
    }

    s_fdn_context[dual_sys].is_fdn_enable = is_fdn_enable;
    s_fdn_context[dual_sys].is_adn_enable = is_adn_enable;
    s_fdn_context[dual_sys].is_usim = is_usim;

    MMIAPIFDN_InitFDNInfo(dual_sys);
#ifdef MMI_IDLE_FDN_ICON_SUPPORT    
    MAIN_SetFDNState(TRUE);
#endif
}

/*****************************************************************************/
// 	Description : Initial FDN Info From PB
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIFDN_InitFDNInfo(MN_DUAL_SYS_E dual_sys)
{
    uint16 i = 0;
    MMIPB_CONTACT_T *contact_info = PNULL;  
#if 0    
     PB_GROUP_LIST_E pb_sim_group_ids[] = {
         PB_GROUP_SIM_1,         //SIM卡1
#ifndef MMI_MULTI_SIM_SYS_SINGLE
         PB_GROUP_SIM_2,             //SIM卡2
#if defined(MMI_MULTI_SIM_SYS_TRI)||defined(MMI_MULTI_SIM_SYS_QUAD)
         PB_GROUP_SIM_3,             //SIM卡3
#endif
#if defined(MMI_MULTI_SIM_SYS_QUAD)
         PB_GROUP_SIM_4,             //SIM卡4
#endif
#endif
        };   
#endif    

    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPIFDN_InitFDNInfo  dual_sys is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFDN_444_112_2_18_2_19_15_10,(uint8*)"");
        return;
    }

    s_fdn_context[dual_sys].fdn_info = MMIPB_GetContactStorage(dual_sys, PHONEBOOK_FDN_STORAGE);

    //SCI_TRACE_MID:"MMIAPIFDN_InitFDNInfo is_valid=%d,max=%d, used=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFDN_451_112_2_18_2_19_15_11,(uint8*)"ddd",s_fdn_context[dual_sys].fdn_info.is_valid, s_fdn_context[dual_sys].fdn_info.max_record_num, s_fdn_context[dual_sys].fdn_info.used_record_num);

    //init fdn context without check if pb is valid
    {
        if (PNULL != s_fdn_context[dual_sys].entry_state)
        {
            SCI_FREE(s_fdn_context[dual_sys].entry_state);
            s_fdn_context[dual_sys].entry_state= PNULL;
        }
    
        //为全局变量s_fdn_context[dual_sys].entry_state分配空间
        s_fdn_context[dual_sys].entry_state = SCI_ALLOCAZ(s_fdn_context[dual_sys].fdn_info.max_record_num);
        if(PNULL == s_fdn_context[dual_sys].entry_state)
        {
            //SCI_TRACE_LOW:"MMIAPIFDN_InitFDNInfo entry state alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFDN_465_112_2_18_2_19_15_12,(uint8*)"");
            return;            
        }
        //SCI_MEMSET(s_fdn_context[dual_sys].entry_state, 0, s_fdn_context[dual_sys].fdn_info.max_record_num);

        contact_info = (MMIPB_CONTACT_T *)SCI_ALLOCAZ(sizeof(MMIPB_CONTACT_T));
        if(PNULL == contact_info)
        {
            //SCI_TRACE_LOW:"MMIAPIFDN_InitFDNInfo MMIPB_CONTACT_T alloc failed"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFDN_473_112_2_18_2_19_15_13,(uint8*)"");
            return;            
        }
        //SCI_MEMSET(contact_info, 0x00, sizeof(MMIPB_CONTACT_T));
        //按照entry_id读出所有的记录
        for (i = 1; i <= s_fdn_context[dual_sys].fdn_info.max_record_num; i++)
        {		
            uint16 storage_id = 0;
            uint16  entry_id = i;
           // fdn_entry_ptr->group_id = pb_sim_group_ids[dual_sys];

            /*若读取当前entry_id的记录有值，则返回MN_RETURN_SUCCESS*/
            storage_id = (((PHONEBOOK_FDN_STORAGE<<8)&0XFF00) | (dual_sys+MMIPB_STORAGE_SIM1));
            if (MMIPB_ERROR_SUCCESS ==  MMIAPIPB_GetContactInfo(entry_id, storage_id, contact_info))
            {
                s_fdn_context[dual_sys].entry_state[entry_id - 1]  = TRUE;//相应的标志位赋值
            }
            else
            {
                s_fdn_context[dual_sys].entry_state[entry_id - 1]  = FALSE;//相应的标志位赋值
            }
        } 
        SCI_FREE(contact_info);
    }
}

/*****************************************************************************/
// 	Description : Get FDN Context
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC MMIFDN_CONTEXT_T * MMIAPIFDN_GetFDNContext(MN_DUAL_SYS_E dual_sys)
{
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPIFDN_GetFDNContext  dual_sys is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFDN_507_112_2_18_2_19_15_14,(uint8*)"");
        dual_sys = MN_DUAL_SYS_1;
    }

    return &s_fdn_context[dual_sys];

}

/*****************************************************************************/
//  Description : 判断是否是2G fdn开启
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFDN_Is2GFdnEnable(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN result = FALSE;

    if(dual_sys < MMI_DUAL_SYS_MAX)
    {
        if ((s_fdn_context[dual_sys].is_fdn_enable) && 
            (!s_fdn_context[dual_sys].is_usim))
        {
            result = TRUE;
        }
    }

    return result;
}

/*****************************************************************************/
//  Description : if fdn enable
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFDN_IsFdnExist(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN result = FALSE;

    if(dual_sys < MMI_DUAL_SYS_MAX)
    {
        if ((s_fdn_context[dual_sys].is_fdn_exist))
        {
            result = TRUE;
        }
    }
    return result;
}    

#else
  /*****************************************************************************/
//  Description : set reread phonebook state
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFDN_SetReReadPhoneBookState(BOOLEAN is_reread)
{

}

  /*****************************************************************************/
//  Description : get reread phonebook state
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFDN_GetReReadPhoneBookState(void)
{
    return FALSE;
}

 /*****************************************************************************/
//  Description : set operate fdn SIM id
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
 PUBLIC void MMIAPIFDN_SetCurSIM(MN_DUAL_SYS_E dual_sys)
{

}

 /*****************************************************************************/
//  Description : get operate fdn SIM id
//  Global resource dependence : 
//  Author: kun.yu
//  Note:
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E MMIAPIFDN_GetCurSIM(void)
{
    return MN_DUAL_SYS_1;
}

/*****************************************************************************/
//  Description : FDN Initial
//  Global resource dependence : 
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIFDN_Init(void)
{

}

/*****************************************************************************/
// 	Description : Handle FDN Message
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIFDN_HandleCallReadyInd(DPARAM param)
{

}

/*****************************************************************************/
// 	Description : Initial FDN Info From PB
//	Global resource dependence : none
//  Author: zhaohui
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIFDN_InitFDNInfo(MN_DUAL_SYS_E dual_sys)
{

}

LOCAL MMIFDN_CONTEXT_T s_fdn_context[MMI_DUAL_SYS_MAX];

/*****************************************************************************/
// 	Description : Get FDN Context
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC MMIFDN_CONTEXT_T * MMIAPIFDN_GetFDNContext(MN_DUAL_SYS_E dual_sys)
{
    if(dual_sys >= MMI_DUAL_SYS_MAX)
    {
        //SCI_TRACE_LOW:"MMIAPIFDN_GetFDNContext  dual_sys is error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIFDN_612_112_2_18_2_19_15_15,(uint8*)"");
        dual_sys = MN_DUAL_SYS_1;
    }

    return &s_fdn_context[dual_sys];
}

/*****************************************************************************/
//  Description : 判断是否是2G fdn开启
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIFDN_Is2GFdnEnable(MN_DUAL_SYS_E dual_sys)
{
    BOOLEAN result = FALSE;

    if(dual_sys < MMI_DUAL_SYS_MAX)
    {
        if ((s_fdn_context[dual_sys].is_fdn_enable) && 
            (!s_fdn_context[dual_sys].is_usim))
        {
            result = TRUE;
        }
    }

    return result;
}


#endif //#ifdef FDN_SUPPORT

