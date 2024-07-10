/***************************************************************************
** File Name:      mmismscb_api.c                                          *
** Author:                                                                 *
** Date:           04/2006                                                   *
** Copyright:      2006 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to define the api function.           *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2006          Tracy Zhang       Modularize
****************************************************************************/
#include "mmi_app_sms_trc.h"
#if defined(MMI_SMSCB_SUPPORT)

/**------------------------------------------------------------------------*
**                         Include Files                                   *
**-------------------------------------------------------------------------*/
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmisms_export.h"
#include "mmismscb_control.h"
#include "mmi_common.h"
#include "mmi_default.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiphone_export.h"
#include "guitext.h"
#include "guimsgbox.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_textfun.h"
#ifdef MMI_KEY_LOCK_SUPPORT
#include "mmikl_export.h"
#endif
#include "mmk_timer.h"
#include "mmipub.h"
#include "mmisms_text.h"
#include "mmisms_image.h"
#include "mmisms_id.h"
#include "mmisms_nv.h"
//#include "mmisms_menutable.h"
//#include "mmismsapp_wintab.h"
#include "mmi_appmsg.h"
#include "gui_ucs2b_converter.h"
#include "guiedit.h"
#ifdef MMI_PDA_SUPPORT
#include "guictrl_api.h"
#include "guibutton.h"
#endif

#include "guitab.h"
#include "guirichtext.h"
//#ifdef SCREENSAVER_SUPPORT
//#include "mmiss_export.h"
//#endif

#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
#include "mmiidle_statusbar.h"
#endif

#ifdef PDA_UI_DROPDOWN_WIN
#include "mmidropdownwin_export.h"
#endif
#include "mmi_pubmenuwin.h"
#include "mmi_signal_ext.h"
#include "mmismscb_wintab.h"
#include "mmisms_internal.h"

#ifdef MMI_ETWS_SUPPORT
#include "mmi_ring.h"
#include "mmicc_export.h"
#include "window_parse.h"
#include "mmicc_id.h"
#include "mmirecord_id.h"
#include "mmidc_camera_id.h"
#include "mmicc_internal.h"
#endif

/******************************************************************************************************************/
//
//
//                        小区广播模块内部宏定义
//
//
/******************************************************************************************************************/
#ifdef PDA_UI_DROPDOWN_WIN
#define MMI_DISPLAY_TIME_LEN    9
#endif

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
#ifdef MMI_ETWS_SUPPORT
MMISMS_CB_MESSAGE_T s_cb_msg = {0};            //小区广播消息数组
#else
LOCAL MMISMS_CB_MESSAGE_T s_cb_msg = {0};            //小区广播消息数组
#endif
LOCAL MN_DUAL_SYS_E s_dual_card_sys = MN_DUAL_SYS_1;
LOCAL BOOLEAN s_is_alert_type_msg = FALSE; //check if the message is alert type. added by Tong Yongli

LOCAL MN_CB_ALERT_MSG_T s_cb_all_alert_info[MN_DUAL_SYS_MAX] = {0};    //记录ALERT所有信息

#ifdef MMI_ETWS_SUPPORT
LOCAL uint16 s_cur_display_cb_msg_index = 0;
LOCAL uint8 s_repeated_period_time_id = 0;
LOCAL uint8 s_vibrator_period_time_id = 0;
LOCAL uint16 s_etws_alert_num  = 0;
LOCAL uint16 s_repeated_channel = 0;
LOCAL    MMI_STRING_T etws_prompt_str = {0};
LOCAL MMI_STRING_T etws_title_str = {0};
#endif
const MN_CB_MSG_ID_T g_cb_alert_id[MMI_CB_ALERT_NUM] =
{
    4370,
};

extern BOOLEAN cell_broadcast;
LOCAL BOOLEAN s_is_etws_prim_msg = FALSE;

#ifdef MMI_ETWS_SUPPORT
LOCAL BOOLEAN s_is_etws_alert_msg = FALSE;	// <<3GPP 23.041 9.4.1.2.1>> Emergency User Alert
LOCAL BOOLEAN s_is_etws_need_popup = FALSE;	// <<3GPP 23.041 9.4.1.2.1>> Popup
LOCAL BOOLEAN s_is_need_enable = FALSE;
LOCAL BOOLEAN s_is_personal = FALSE;
LOCAL BOOLEAN s_is_need_torch = FALSE;
LOCAL BOOLEAN s_is_need_redisplay = FALSE; // display etws prompt with dismiss button after call
LOCAL BOOLEAN s_is_emer_call = FALSE; // make emergency call
LOCAL MN_DUAL_SYS_E s_etws_card_sys = MN_DUAL_SYS_1;
LOCAL BOOLEAN s_is_etws_display = FALSE;
LOCAL MN_DUAL_SYS_E s_etws_delay_show_dual_sim_context;
LOCAL uint16 s_etws_vibrate_type_context = 0;
LOCAL uint16 s_etws_alert_period_context = 0;
LOCAL uint16 s_etws_is_contiued_context = 0;
LOCAL BOOLEAN s_etws_redisplay_is_need_vibrate = FALSE;

#define SMSCB_ALERT_VIBRATE_FOREVER 0xFF

CMAS_CHANNEL_INFO_T s_cmas_pws_channel_info[] =
{
    {  911, 0, 0, TXT_CMAS_PWS_911_TITLE,  {0},  0, 1,  1,  1, 4,  0,  0, 0, 10 },
    {  919, 0, 0, TXT_CMAS_PWS_919_TITLE,  {0},  0, 1,  1,  1, 4,  0,  0, 0, 10 },

    { 4370, 0, 0, TXT_CMAS_PWS_4370_TITLE, {0},  0, 1,  0,  0, 1,  1,  1, 0, 10 },
    { 4371, 0, 0, TXT_CMAS_PWS_4371_TITLE, {0},  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4372, 0, 0, TXT_CMAS_PWS_4372_TITLE, {0},  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4373, 0, 0, TXT_CMAS_PWS_4373_TITLE, {0},  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4374, 0, 0, TXT_CMAS_PWS_4374_TITLE, {0},  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4375, 0, 0, TXT_CMAS_PWS_4375_TITLE, {0},  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4376, 0, 0, TXT_CMAS_PWS_4376_TITLE, {0},  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4377, 0, 0, TXT_CMAS_PWS_4377_TITLE, {0},  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4378, 0, 0, TXT_CMAS_PWS_4378_TITLE, {0},  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4379, 0, 0, TXT_CMAS_PWS_4379_TITLE, {0},  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4380, 0, 0, TXT_CMAS_PWS_4380_TITLE, {0},  0, 0,  1,  1, 5,  1,  1, 0, 10 },

    { 4383, 0, 0, TXT_CMAS_PWS_4383_TITLE, {0},  0, 1,  0,  1, 1,  1,  1, 0, 10 },
    { 4384, 0, 0, TXT_CMAS_PWS_4384_TITLE, {0},  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4385, 0, 0, TXT_CMAS_PWS_4385_TITLE, {0},  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4386, 0, 0, TXT_CMAS_PWS_4386_TITLE, {0},  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4387, 0, 0, TXT_CMAS_PWS_4387_TITLE, {0},  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4388, 0, 0, TXT_CMAS_PWS_4388_TITLE, {0},  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4389, 0, 0, TXT_CMAS_PWS_4389_TITLE, {0},  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4390, 0, 0, TXT_CMAS_PWS_4390_TITLE, {0},  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4391, 0, 0, TXT_CMAS_PWS_4391_TITLE, {0},  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4392, 0, 0, TXT_CMAS_PWS_4392_TITLE, {0},  0, 1,  1,  1, 2,  1,  1, 0, 10 },
    { 4393, 0, 0, TXT_CMAS_PWS_4393_TITLE, {0},  0, 0,  1,  1, 5,  1,  1, 0, 10 },

    { 0 }
};


CMAS_CONTROL_LIST_T s_cmas_spec_list[] =
{
    {466,   s_cmas_pws_channel_info},
    {0}
};
#endif

extern BOOLEAN cell_broadcast;

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//  Description : Get Current Channel table according the MCC
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC CMAS_CHANNEL_INFO_T* GetCMASChannelTable( MN_DUAL_SYS_E dual_sys)
{
    CMAS_CHANNEL_INFO_T           *current_channel_table_ptr = NULL;
    uint32 i = 0;
    MN_PLMN_T plmn_info = {0};
#ifndef MMI_MULTI_SIM_SYS_SINGLE
    uint32 sim_ok_array[MMI_DUAL_SYS_MAX] = {0,};
    uint32 sim_ok_num = 0;
#endif

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        SCI_MEMSET(sim_ok_array, 0x00, sizeof(sim_ok_array));

        for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
        {
            if(MMIAPIPHONE_IsSimOk((MN_DUAL_SYS_E)i))
            {
                sim_ok_array[sim_ok_num] = (MN_DUAL_SYS_E)i;
                sim_ok_num ++;
            }
        }

        dual_sys = (MN_DUAL_SYS_E)(sim_ok_array[0]);   //else use the ok sim. or if no ok, user the sim1.(sim_ok have inited with 0. )
    }
#else
    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        dual_sys = MN_DUAL_SYS_1;
    }
#endif

    SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));

    MMIAPIPHONE_GetCurPLMN(&plmn_info, dual_sys);

    SCI_TRACE_LOW("[MMISMSCB] GetCMASChannelTable MMIAPIPHONE_GetCurPLMN plmn_info.mcc = %d", plmn_info.mcc);

    if( plmn_info.mcc < 200 )
    {
        MMIAPIPHONE_GetSimHPLMN(&plmn_info, dual_sys);

        SCI_TRACE_LOW("[MMISMSCB] GetCMASChannelTable MMIAPIPHONE_GetSimHPLMN plmn_info.mcc = %d", plmn_info.mcc);
    }
    i = 0;

    SCI_TRACE_LOW("[MMISMSCB] GetCMASChannelTable plmn_info.mcc = %d", plmn_info.mcc);

    do
    {
        if(plmn_info.mcc == s_cmas_spec_list[i].mcc)
        {
            SCI_TRACE_LOW("[MMISMSCB] GetCMASChannelTable table index i = %d", i);
            current_channel_table_ptr = s_cmas_spec_list[i].channel_info;
            return current_channel_table_ptr;
        }
        i ++;
    }
    while(0 != s_cmas_spec_list[i].mcc);

    current_channel_table_ptr = s_cmas_spec_list[0].channel_info;
    return current_channel_table_ptr;

    return NULL;
}

/*****************************************************************************/
//  Description : Get Current Channel info according the channel ID
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC CMAS_CHANNEL_INFO_T* GetCMASChannelInfo( MN_DUAL_SYS_E dual_sys, uint16 msg_id)
{
    uint8 i = 0;
    CMAS_CHANNEL_INFO_T* current_channel_table = GetCMASChannelTable(dual_sys);

    SCI_TRACE_LOW("[MMISMSCB] GetCMASChannelInfo, dual_sys = %d, msg_id = %d", dual_sys, msg_id);

    if( NULL == current_channel_table )
    {
        SCI_TRACE_LOW("[MMISMSCB] GetCMASChannelInfo, current_channel_table = NULL");
        return NULL;
    }

    do
    {
        if( msg_id == current_channel_table[i].channel_id)
        {
            return &current_channel_table[i];
        }
        i++;
    }
    while(0 != current_channel_table[i].channel_id);

    return NULL;
}
#endif

/******************************************************************************************************************/
//
//
//                        小区广播control模块处理函数声明
//
//
/******************************************************************************************************************/
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         AddNewMsgToCBMsgList
//  功能描述:        将收到的新小区广播加入到广播收件箱列表中去
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void AddNewMsgToCBMsgList(
                                APP_MN_SMSCB_MSG_IND_T *cb_msg_ptr,    //IN: 广播信息信号指针
                                BOOLEAN is_read
                                );

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         InitCBSoftConfChannel
//  功能描述:        根据配置化软件判断需要配置的NV Channels. NV_MS_CONFIG_TELE/CELL_BROADCAST_CHANNEL
//  全局变量使用:    cell_broadcast_channel
//  返回值:
//  作者:            Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MMISMS_CB_CHANNEL_T InitCBSoftConfChannel(void);

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         AddNewMarcoMsgToCBMsgList
//  功能描述:        将收到的新小区广播加入到广播收件箱列表中去
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void AddNewMarcoMsgToCBMsgList(
                                APP_MN_SMSCB_MACRO_MSG_IND_T *cb_msg_ptr    //IN: 广播信息信号指针
                                );
#if defined (MODEM_SUPPORT_SPRD)
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         AddNewLteWarningMsgToCBMsgList
//  功能描述:        将收到的新小区广播加入到广播收件箱列表中去
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void AddNewLteWarningMsgToCBMsgList(
                                APP_MN_LTE_WARNING_IND_T *cb_msg_ptr  //IN: 广播信息信号指针
                                );
#endif

/**------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                             *
**-------------------------------------------------------------------------*/

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         AddNewMsgToCBMsgList
//  功能描述:        将收到的新小区广播加入到广播收件箱列表中去
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void AddNewMsgToCBMsgList(
                                APP_MN_SMSCB_MSG_IND_T *cb_msg_ptr,    //IN: 广播信息信号指针
                                BOOLEAN is_read
                                )
{
    MMISMS_CB_MESSAGE_ITEM_T *cur_msg_ptr = PNULL;
    MMISMS_CB_MESSAGE_ITEM_T *temp_msg_ptr = PNULL;
    MMISMS_CB_MESSAGE_ITEM_T *last_msg_ptr = PNULL;
    uint16 msg_real_len = 0;
#ifdef MMI_ETWS_SUPPORT
    MMISMS_CB_MESSAGE_ARRAY_T msg_arr_ptr = {0};
    MMISMS_CB_MESSAGE_ARRAY_T* cb_message_array = PNULL;

    MMISMS_CB_MESSAGE_ARRAY_T* temp_message_array = PNULL;
    uint16 index = 0;//bug1941699
    uint8 cb_num = 0;
    uint8 read_count = 0;
#endif

    if (PNULL == cb_msg_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:AddNewMsgToCBMsgList cb_msg_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2140_112_2_18_2_55_33_889,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"SMSCB: AddNewMsgToCBMsgList"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2143_112_2_18_2_55_33_890,(uint8*)"");
    
    cur_msg_ptr = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
    if (PNULL == cur_msg_ptr)
    {
        //SCI_TRACE_LOW:"SMSCB: cur_msg_ptr alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2148_112_2_18_2_55_33_891,(uint8*)"");
        return;
    }
    //SCI_MEMSET(cur_msg_ptr, 0 , sizeof(MMISMS_CB_MESSAGE_ITEM_T));

#ifdef MMI_ETWS_SUPPORT
    s_cur_display_cb_msg_index = 0;
#endif
    // 先给相应的广播数组项赋值并设为未读，然后处理链表
    cur_msg_ptr->msg_id  = cb_msg_ptr->msg_id;
    cur_msg_ptr->is_read = FALSE;
    cur_msg_ptr->dual_sys = cb_msg_ptr->dual_sys;
    //将收到的广播消息的内容存入消息内容结构数组中去
    if (cb_msg_ptr->dcs.alphabet_type == MN_SMS_UCS2_ALPHABET)
    {
        msg_real_len = MMIAPICOM_Wstrlen((const wchar*)cb_msg_ptr->msg_arr) * sizeof(wchar);
    } 
    else
    {
        msg_real_len = strlen((char *)cb_msg_ptr->msg_arr);
    }
    ConvertCBMsgContextToMsgItem(
        cb_msg_ptr->msg_arr,
        MIN(cb_msg_ptr->msg_len, msg_real_len),
        cb_msg_ptr->dcs.alphabet_type,
        FALSE,
        cur_msg_ptr
        );
    //获得接受到广播时的系统日期和时间
    TM_GetSysTime(&cur_msg_ptr->time);
    TM_GetSysDate(&cur_msg_ptr->date);

#ifdef MMI_ETWS_SUPPORT
    if( PNULL == s_cb_msg.msg_ptr )
    {
        SCI_TRACE_LOW("MMISMSCB: s_cb_msg.msg_ptr is PNULL");
        s_cb_msg.num = MMISMS_NV_LoadSmsCBMessageListNum();
        read_count = 0;

        if( s_cb_msg.num > 0 )
        {
            temp_msg_ptr = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
            if (PNULL == temp_msg_ptr)
            {
                SCI_TRACE_LOW("MMISMSCB:alloc temp_msg_ptr failed");
                return;
            }
            SCI_MEMSET(temp_msg_ptr , 0, sizeof(MMISMS_CB_MESSAGE_ITEM_T));
            s_cb_msg.msg_ptr = temp_msg_ptr;

            cb_message_array = (MMISMS_CB_MESSAGE_ARRAY_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
            if (PNULL == cb_message_array)
            {
                SCI_TRACE_LOW("MMISMSCB:alloc cb_message_array failed");
                return;
            }
            SCI_MEMSET(cb_message_array , 0, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
            if( TRUE == MMISMS_NV_ReadSmsCBItem(index, cb_message_array) )
            {
                SCI_MEMCPY(temp_msg_ptr, cb_message_array, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
                read_count ++;
            }
            else
            {
                SCI_TRACE_LOW("MMISMSCB: MMISMS_NV_ReadSmsCBItem FAIL!!");
            }

            for( index = 1; index < s_cb_msg.num; index ++)
            {
                last_msg_ptr = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
                if (PNULL == last_msg_ptr)
                {
                    SCI_TRACE_LOW("MMISMSCB:alloc last_msg_ptr failed");
                    return;
                }
                SCI_MEMSET(last_msg_ptr , 0, sizeof(MMISMS_CB_MESSAGE_ITEM_T));
                if( TRUE == MMISMS_NV_ReadSmsCBItem(index, cb_message_array) )
                {
                    SCI_MEMCPY(last_msg_ptr, cb_message_array, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
                    temp_msg_ptr->msg_next_ptr = last_msg_ptr;
                    temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
                }
                else
                {
                    SCI_TRACE_LOW("MMISMSCB: MMISMS_NV_ReadSmsCBItem FAIL!!");
                }
            }
        }
    }
#endif

    //收件箱的广播消息总数加一
    s_cb_msg.num++;
    if (PNULL == s_cb_msg.msg_ptr)
    {
        s_cb_msg.msg_ptr = cur_msg_ptr;
    }
    else
    {//结构调整，以前是最新的放在最后，调整后最新的放在最前面        
        cur_msg_ptr->msg_next_ptr = s_cb_msg.msg_ptr;
        s_cb_msg.msg_ptr = cur_msg_ptr;
        
        if (s_cb_msg.num > MMI_MAX_CB_MSG_NUM)
        {//如果大于最大条数，销毁最后一个节点，把最新的插入到头节点。
            s_cb_msg.num = MMI_MAX_CB_MSG_NUM;
            temp_msg_ptr = s_cb_msg.msg_ptr;
            while (PNULL != temp_msg_ptr->msg_next_ptr)
            {//找到最后一个节点，然后销毁空间，倒数第二个节点next要置空
                last_msg_ptr = temp_msg_ptr;
                temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
            }
            if (PNULL != last_msg_ptr)
            {
                last_msg_ptr->msg_next_ptr = PNULL;
            }            
            SCI_FREE(temp_msg_ptr);
            temp_msg_ptr = PNULL;
        }
    }

#ifdef MMI_ETWS_SUPPORT
    temp_msg_ptr = s_cb_msg.msg_ptr;
    index = 0;
    cb_message_array = (MMISMS_CB_MESSAGE_ARRAY_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
    if ( PNULL == cb_message_array)
    {
        SCI_TRACE_LOW("MMISMSCB: cb_message_array = PNULL!!");
        return ;
    }
    SCI_MEMSET(cb_message_array , 0, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));

    MMISMSNV_SaveSmsCBMessageListNum(s_cb_msg.num);

    while( (PNULL != temp_msg_ptr) && ( index < s_cb_msg.num))
    {
        SCI_MEMCPY(cb_message_array, temp_msg_ptr, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
        MMIAPISMS_NV_WriteSmsCBItem(TRUE, index, cb_message_array);
        //MMISMSNV_SaveSmsCBMessageList(cb_message_array, index);                  //保存语言列表到 NV 中去
        temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
        index ++;
    }

    if( PNULL != cb_message_array)
    {
        SCI_FREE(cb_message_array);
        cb_message_array = PNULL;
    }
#endif

}
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         InitCBSoftConfChannel
//  功能描述:        根据配置化软件判断需要配置的NV Channels. NV_MS_CONFIG_TELE/CELL_BROADCAST_CHANNEL
//  全局变量使用:    cell_broadcast_channel
//  返回值:
//  作者:            Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
LOCAL MMISMS_CB_CHANNEL_T InitCBSoftConfChannel(void)
{
    uint8 i = 0;
    MMISMS_CB_CHANNEL_T return_channel_list = {0};
    MN_CB_MSG_ID_T cell_broadcast_channel[5] = {0};
    MN_RETURN_RESULT_E ret_value = MN_RETURN_FAILURE;
    //ret_value = MMIMMS_GetNDTNvSetting(TELE_NV_CELL_BROADCAST_CHANNEL,&return_channel_list);
    ret_value = MMI_ReadNVItem(MMINV_SMSCB_CELL_BROADCAST_CHANNEL,&cell_broadcast_channel);
    if (MN_RETURN_SUCCESS != ret_value)
    {
        return_channel_list.num = 0;
    }
    else
    {
        return_channel_list.num = 0;
        for(i = 0; i < 5; i++)
        {
            if(0 != cell_broadcast_channel[i])
            {
                return_channel_list.channel_arr[i].msg_id = cell_broadcast_channel[i];
                return_channel_list.channel_arr[i].is_enabled = TRUE;
                return_channel_list.num++;
            }
            else
            {
                 break;
            }
        }
    }
    return return_channel_list;
}


#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         InitCB_ETWSChannel
//  功能描述:
//  全局变量使用:    cell_broadcast_channel
//  返回值:
//  作者:
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_CHANNEL_T InitCB_ETWSChannel(void)
{
    uint16 i = 0;
    MMISMS_CB_CHANNEL_T etws_channel_list = {0};
    CMAS_CHANNEL_INFO_T* current_channel_table = PNULL;
    MN_DUAL_SYS_E dual_sys = MMI_DUAL_SYS_MAX;
    MMI_STRING_T etws_title = {0};

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    uint32 sim_ok_array[MMI_DUAL_SYS_MAX] = {0,};
    uint32 sim_ok_num = 0;
#endif
    SCI_TRACE_LOW("[MMISMSCB] InitCB_ETWSChannel");

#ifndef MMI_MULTI_SIM_SYS_SINGLE
    SCI_MEMSET(sim_ok_array, 0x00, sizeof(sim_ok_array));

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        if(MMIAPIPHONE_IsSimOk((MN_DUAL_SYS_E)i))
        {
            sim_ok_array[sim_ok_num] = (MN_DUAL_SYS_E)i;
            sim_ok_num ++;
        }
    }

    if (dual_sys >= MMI_DUAL_SYS_MAX)
    {
        dual_sys = (MN_DUAL_SYS_E)(sim_ok_array[0]);   //else use the ok sim. or if no ok, user the sim1.(sim_ok have inited with 0. )
    }
#else
    dual_sys = MN_DUAL_SYS_1;
#endif

    current_channel_table = GetCMASChannelTable(dual_sys);

    if( NULL == current_channel_table )
        return etws_channel_list;

    i = 0;
    do
    {
        etws_channel_list.channel_arr[i].msg_id = current_channel_table[i].channel_id;
        etws_channel_list.channel_arr[i].is_enabled = current_channel_table[i].is_default_enabled;

        if( 0 != current_channel_table[i].string_id )
        {
            MMI_GetLabelTextByLang(current_channel_table[i].string_id , &etws_title);
            etws_channel_list.channel_arr[i].name.wstr_len = (uint16)MIN(MMI_CB_CHANNEL_NAME_LEN, etws_title.wstr_len);
            MMIAPICOM_Wstrncpy(etws_channel_list.channel_arr[i].name.wstr, etws_title.wstr_ptr, etws_title.wstr_len) ;
        }
        else
        {
            etws_channel_list.channel_arr[i].name.wstr_len = (uint16)MIN(MMI_CB_CHANNEL_NAME_LEN, MMIAPICOM_Wstrlen(current_channel_table[i].title_id));
            MMIAPICOM_Wstrncpy(etws_channel_list.channel_arr[i].name.wstr, current_channel_table[i].title_id, etws_channel_list.channel_arr[i].name.wstr_len) ;
        }

        i++;
    }
    while(0 != current_channel_table[i].channel_id);
    etws_channel_list.num = i;

    MMIAPISMSCB_SetETWSNum(etws_channel_list.num);

    return etws_channel_list;
}
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         AddNewMarcoMsgToCBMsgList
//  功能描述:        将收到的新小区广播加入到广播收件箱列表中去
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void AddNewMarcoMsgToCBMsgList(
                                APP_MN_SMSCB_MACRO_MSG_IND_T *cb_msg_ptr    //IN: 广播信息信号指针
                                )
{
    MMISMS_CB_MESSAGE_ITEM_T *cur_msg_ptr = PNULL;
    MMISMS_CB_MESSAGE_ITEM_T *temp_msg_ptr = PNULL;
    MMISMS_CB_MESSAGE_ITEM_T *last_msg_ptr = PNULL;
    uint16 msg_real_len = 0;
#ifdef MMI_ETWS_SUPPORT
    MMISMS_CB_MESSAGE_ARRAY_T msg_arr_ptr = {0};
    MMISMS_CB_MESSAGE_ARRAY_T* cb_message_array = PNULL;

    MMISMS_CB_MESSAGE_ARRAY_T* temp_message_array = PNULL;
    uint16 index = 0;//bug1941699
    uint8 read_count = 0;
#endif
    
    if (PNULL == cb_msg_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:AddNewMarcoMsgToCBMsgList cb_msg_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2220_112_2_18_2_55_33_892,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"SMSCB: AddNewMarcoMsgToCBMsgList"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2223_112_2_18_2_55_33_893,(uint8*)"");
    
    cur_msg_ptr = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
    if (PNULL == cur_msg_ptr)
    {
        //SCI_TRACE_LOW:"SMSCB: cur_msg_ptr alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2228_112_2_18_2_55_33_894,(uint8*)"");
        return;
    }
    //SCI_MEMSET(cur_msg_ptr, 0 , sizeof(MMISMS_CB_MESSAGE_ITEM_T));

#ifdef MMI_ETWS_SUPPORT
    s_cur_display_cb_msg_index = 0;
#endif
    // 先给相应的广播数组项赋值并设为未读，然后处理链表
    cur_msg_ptr->msg_id  = cb_msg_ptr->msg_id;
    cur_msg_ptr->is_read = FALSE;
    cur_msg_ptr->dual_sys = cb_msg_ptr->dual_sys;
    //将收到的广播消息的内容存入消息内容结构数组中去
    if (cb_msg_ptr->dcs.alphabet_type == MN_SMS_UCS2_ALPHABET)
    {
        msg_real_len = MMIAPICOM_Wstrlen((const wchar*)cb_msg_ptr->msg_arr) * sizeof(wchar);
    } 
    else
    {
        msg_real_len = strlen((char *)cb_msg_ptr->msg_arr);
    }
    ConvertCBMsgContextToMsgItem(
        cb_msg_ptr->msg_arr,
        MIN(cb_msg_ptr->msg_len, msg_real_len),
        cb_msg_ptr->dcs.alphabet_type,
        TRUE,
        cur_msg_ptr
        );
    //获得接受到广播时的系统日期和时间
    TM_GetSysTime(&cur_msg_ptr->time);
    TM_GetSysDate(&cur_msg_ptr->date);

#ifdef MMI_ETWS_SUPPORT
    if( PNULL == s_cb_msg.msg_ptr ) 
    {
        SCI_TRACE_LOW("MMISMSCB: s_cb_msg.msg_ptr is PNULL");
        s_cb_msg.num = MMISMS_NV_LoadSmsCBMessageListNum();
        read_count = 0;

        if( s_cb_msg.num > 0 )
        {
            temp_msg_ptr = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
            if (PNULL == temp_msg_ptr)
            {
                SCI_TRACE_LOW("MMISMSCB:alloc temp_msg_ptr failed");
                return;
            }
            SCI_MEMSET(temp_msg_ptr , 0, sizeof(MMISMS_CB_MESSAGE_ITEM_T));
            s_cb_msg.msg_ptr = temp_msg_ptr;

            cb_message_array = (MMISMS_CB_MESSAGE_ARRAY_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
            if (PNULL == cb_message_array)
            {
                SCI_TRACE_LOW("MMISMSCB:alloc cb_message_array failed");
                return;
            }
            SCI_MEMSET(cb_message_array , 0, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
            if( TRUE == MMISMS_NV_ReadSmsCBItem(index, cb_message_array) )
            {
                SCI_MEMCPY(temp_msg_ptr, cb_message_array, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
                read_count ++;
            }
            else
            {
                SCI_TRACE_LOW("MMISMSCB: MMISMS_NV_ReadSmsCBItem FAIL!!");
            }

            for( index = 1; index < s_cb_msg.num; index ++)
            {
                last_msg_ptr = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
                if (PNULL == last_msg_ptr)
                {
                    SCI_TRACE_LOW("MMISMSCB:alloc last_msg_ptr failed");
                    return;
                }
                SCI_MEMSET(last_msg_ptr , 0, sizeof(MMISMS_CB_MESSAGE_ITEM_T));
                if( TRUE == MMISMS_NV_ReadSmsCBItem(index, cb_message_array) )
                {
                    SCI_MEMCPY(last_msg_ptr, cb_message_array, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
                    temp_msg_ptr->msg_next_ptr = last_msg_ptr;
                    temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
                }
                else

                {
                    SCI_TRACE_LOW("MMISMSCB: MMISMS_NV_ReadSmsCBItem FAIL!!");
                }
            }
        }
    }
#endif

    //收件箱的广播消息总数加一
    s_cb_msg.num++;
    if (PNULL == s_cb_msg.msg_ptr)
    {
        s_cb_msg.msg_ptr = cur_msg_ptr;
    }
    else
    {//结构调整，以前是最新的放在最后，调整后最新的放在最前面        
        cur_msg_ptr->msg_next_ptr = s_cb_msg.msg_ptr;
        s_cb_msg.msg_ptr = cur_msg_ptr;
        
        if (s_cb_msg.num > MMI_MAX_CB_MSG_NUM)
        {//如果大于最大条数，销毁最后一个节点，把最新的插入到头节点。
            s_cb_msg.num = MMI_MAX_CB_MSG_NUM;
            temp_msg_ptr = s_cb_msg.msg_ptr;
            while (PNULL != temp_msg_ptr->msg_next_ptr)
            {//找到最后一个节点，然后销毁空间，倒数第二个节点next要置空
                last_msg_ptr = temp_msg_ptr;
                temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
            }
            if (PNULL != last_msg_ptr)
            {
                last_msg_ptr->msg_next_ptr = PNULL;
            }            
            SCI_FREE(temp_msg_ptr);
            temp_msg_ptr = PNULL;
        }
    }

#ifdef MMI_ETWS_SUPPORT
    temp_msg_ptr = s_cb_msg.msg_ptr;
    index = 0;
    cb_message_array = (MMISMS_CB_MESSAGE_ARRAY_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
    if ( PNULL == cb_message_array)
    {
        SCI_TRACE_LOW("MMISMSCB: cb_message_array = PNULL!!");
        return ;
    }
    SCI_MEMSET(cb_message_array , 0, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));

    MMISMSNV_SaveSmsCBMessageListNum(s_cb_msg.num);

    while( (PNULL != temp_msg_ptr) && ( index < s_cb_msg.num))
    {
        SCI_MEMCPY(cb_message_array, temp_msg_ptr, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
        MMIAPISMS_NV_WriteSmsCBItem(TRUE, index, cb_message_array);
        temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
        index ++;
    }

    if( PNULL != cb_message_array)
    {
        SCI_FREE(cb_message_array);
        cb_message_array = PNULL;
    }
#endif

}

#if defined (MODEM_SUPPORT_SPRD)
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         AddNewLteWarningMsgToCBMsgList
//  功能描述:        将收到的新小区广播加入到广播收件箱列表中去
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
LOCAL void AddNewLteWarningMsgToCBMsgList(
                                APP_MN_LTE_WARNING_IND_T *cb_msg_ptr   //IN: 广播信息信号指针
                                )
{
    MMISMS_CB_MESSAGE_ITEM_T *cur_msg_ptr = PNULL;
    MMISMS_CB_MESSAGE_ITEM_T *temp_msg_ptr = PNULL;
    MMISMS_CB_MESSAGE_ITEM_T *last_msg_ptr = PNULL;
    uint16 msg_real_len = 0;
#ifdef MMI_ETWS_SUPPORT
    MMISMS_CB_MESSAGE_ARRAY_T msg_arr_ptr = {0};
    MMISMS_CB_MESSAGE_ARRAY_T* cb_message_array = PNULL;
    MMISMS_CB_MESSAGE_ARRAY_T* temp_message_array = PNULL;
    uint16 index = 0;//bug1941699
    uint8 read_count = 0;
#endif

    if (PNULL == cb_msg_ptr)
    {
        SCI_TRACE_LOW("MMISMS:AddNewLteWarningMsgToCBMsgList cb_msg_ptr = PNULL");
        return;
    }
   SCI_TRACE_LOW("SMSCB: AddNewLteWarningMsgToCBMsgList");

    cur_msg_ptr = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
    if (PNULL == cur_msg_ptr)
    {
        //SCI_TRACE_LOW:"SMSCB: cur_msg_ptr alloc failed"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2228_112_2_18_2_55_33_894,(uint8*)"");
        return;
    }
    //SCI_MEMSET(cur_msg_ptr, 0 , sizeof(MMISMS_CB_MESSAGE_ITEM_T));

#ifdef MMI_ETWS_SUPPORT
    s_cur_display_cb_msg_index = 0;
#endif
    // 先给相应的广播数组项赋值并设为未读，然后处理链表
    cur_msg_ptr->msg_id  = cb_msg_ptr->msg_id;
    cur_msg_ptr->is_read = FALSE;
    cur_msg_ptr->dual_sys = cb_msg_ptr->dual_sys;
    //将收到的广播消息的内容存入消息内容结构数组中去
    if (cb_msg_ptr->dcs.alphabet_type == MN_SMS_UCS2_ALPHABET)
    {
        msg_real_len = MMIAPICOM_Wstrlen((const wchar*)cb_msg_ptr->msg_ptr) * sizeof(wchar);
    }
    else
    {
        msg_real_len = cb_msg_ptr->msg_len;
    }
    SCI_TRACE_LOW("MMISMSCB:AddNewLteWarningMsgToCBMsgList,msg_real_len:%d\n",msg_real_len);
    ConvertCBMsgContextToMsgItem(
        cb_msg_ptr->msg_ptr,
        msg_real_len,
        cb_msg_ptr->dcs.alphabet_type,
        TRUE,
        cur_msg_ptr
        );
    //获得接受到广播时的系统日期和时间
    TM_GetSysTime(&cur_msg_ptr->time);
    TM_GetSysDate(&cur_msg_ptr->date);

#ifdef MMI_ETWS_SUPPORT
    if( PNULL == s_cb_msg.msg_ptr )
    {
        SCI_TRACE_LOW("MMISMSCB: s_cb_msg.msg_ptr is PNULL");
        s_cb_msg.num = MMISMS_NV_LoadSmsCBMessageListNum();
        read_count = 0;

        if( s_cb_msg.num > 0 )
        {
            temp_msg_ptr = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
            if (PNULL == temp_msg_ptr)
            {
                SCI_TRACE_LOW("MMISMSCB:alloc temp_msg_ptr failed");
                return;
            }
            SCI_MEMSET(temp_msg_ptr , 0, sizeof(MMISMS_CB_MESSAGE_ITEM_T));
            s_cb_msg.msg_ptr = temp_msg_ptr;

            cb_message_array = (MMISMS_CB_MESSAGE_ARRAY_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
            if (PNULL == cb_message_array)
            {
                SCI_TRACE_LOW("MMISMSCB:alloc cb_message_array failed");
                return;
            }
            SCI_MEMSET(cb_message_array , 0, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));

            if( TRUE == MMISMS_NV_ReadSmsCBItem(index, cb_message_array) )
            {
                SCI_MEMCPY(temp_msg_ptr, cb_message_array, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
                read_count ++;
            }
            else
            {
                SCI_TRACE_LOW("MMISMSCB: MMISMS_NV_ReadSmsCBItem FAIL!!");
            }

            for( index = 1; index < s_cb_msg.num; index ++)
            {
                last_msg_ptr = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
                if (PNULL == last_msg_ptr)
                {
                    SCI_TRACE_LOW("MMISMSCB:alloc last_msg_ptr failed");
                    return;
                }
                SCI_MEMSET(last_msg_ptr , 0, sizeof(MMISMS_CB_MESSAGE_ITEM_T));
                if( TRUE == MMISMS_NV_ReadSmsCBItem(index, cb_message_array) )
                {
                    SCI_MEMCPY(last_msg_ptr, cb_message_array, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
                    temp_msg_ptr->msg_next_ptr = last_msg_ptr;
                    temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
                }
                else
                {
                    SCI_TRACE_LOW("MMISMSCB: MMISMS_NV_ReadSmsCBItem FAIL!!");
                }
            }
        }
    }
#endif

    //收件箱的广播消息总数加一
    s_cb_msg.num++;
    if (PNULL == s_cb_msg.msg_ptr)
    {
        s_cb_msg.msg_ptr = cur_msg_ptr;
    }
    else
    {//结构调整，以前是最新的放在最后，调整后最新的放在最前面
        cur_msg_ptr->msg_next_ptr = s_cb_msg.msg_ptr;
        s_cb_msg.msg_ptr = cur_msg_ptr;

        if (s_cb_msg.num > MMI_MAX_CB_MSG_NUM)
        {//如果大于最大条数，销毁最后一个节点，把最新的插入到头节点。
            s_cb_msg.num = MMI_MAX_CB_MSG_NUM;
            temp_msg_ptr = s_cb_msg.msg_ptr;
            while (PNULL != temp_msg_ptr->msg_next_ptr)
            {//找到最后一个节点，然后销毁空间，倒数第二个节点next要置空
                last_msg_ptr = temp_msg_ptr;
                temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
            }
            if (PNULL != last_msg_ptr)
            {
                last_msg_ptr->msg_next_ptr = PNULL;
            }
            SCI_FREE(temp_msg_ptr);
            temp_msg_ptr = PNULL;
        }
    }

#ifdef MMI_ETWS_SUPPORT
    temp_msg_ptr = s_cb_msg.msg_ptr;
    index = 0;
    cb_message_array = (MMISMS_CB_MESSAGE_ARRAY_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
    if ( PNULL == cb_message_array)
    {
        SCI_TRACE_LOW("MMISMSCB: cb_message_array = PNULL!!");
        return ;
    }
    SCI_MEMSET(cb_message_array , 0, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));

    MMISMSNV_SaveSmsCBMessageListNum(s_cb_msg.num);

    while( (PNULL != temp_msg_ptr) && ( index < s_cb_msg.num))
    {
        SCI_MEMCPY(cb_message_array, temp_msg_ptr, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
        MMIAPISMS_NV_WriteSmsCBItem(TRUE, index, cb_message_array);
        temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
        index ++;
    }

    if( PNULL != cb_message_array)
    {
        SCI_FREE(cb_message_array);
        cb_message_array = PNULL;
    }
#endif

}
#endif

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_IsCBDefaultEnable
//  功能描述:        根据配置化软件判断是否打开CB功能NV_MS_CONFIG_TELE/CELL_BROADCAST
//  全局变量使用:    cell_broadcast
//  返回值:          TRUE 默认打开CB，FALSE 默认关闭CB
//  作者:            Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_CONFIG_T MMISMSCB_GetCBDefaultConfig(void)
{
    MMISMS_CB_CONFIG_T cb_config = FALSE;
    MN_RETURN_RESULT_E ret_value = MN_RETURN_FAILURE;
    ret_value = MMI_ReadNVItem(MMINV_SMSCB_CELL_BROADCAST,&cb_config);
    SCI_TRACE_LOW("[MMISMSCB] MMISMSCB_GetCBDefaultConfig ret_value = %d, cb_config = %d.",ret_value,cb_config);

    return cb_config;
}

#else /* MMI_ETWS_SUPPORT */
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMSCB_IsCBDefaultEnable
//  功能描述:        根据配置化软件判断是否打开CB功能NV_MS_CONFIG_TELE/CELL_BROADCAST
//  全局变量使用:    cell_broadcast
//  返回值:          TRUE 默认打开CB，FALSE 默认关闭CB
//  作者:            Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC BOOLEAN MMISMSCB_IsCBDefaultEnable(void)
{
    BOOLEAN bEnabled = FALSE;
    MN_RETURN_RESULT_E ret_value = MN_RETURN_FAILURE;
    ret_value = MMI_ReadNVItem(MMINV_SMSCB_CELL_BROADCAST,&bEnabled);
    SCI_TRACE_LOW("[MMISMSCB] MMISMSCB_IsCBDefaultEnable ret_value = %d, bEnabled = %d.",ret_value,bEnabled);
    if (MN_RETURN_SUCCESS != ret_value)
    {
        bEnabled = TRUE;
    }

    return bEnabled;
}
#endif /* MMI_ETWS_SUPPORT */

//----------------------------------------------------------------------------------------------------------------
//
//  函数名                 ：MMIAPISMSCB_GetCBMsgType
//  功能描述           :
//  全局变量使用:    s_is_alert_type_msg
//  返回值                 :    TRUE    : Alert Message
//                                          FALSE  : Normal Message
//  作者                       :    Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC BOOLEAN MMIAPISMSCB_GetCBMsgType(void)
{
    return s_is_alert_type_msg;
}

/*****************************************************************************/
//  Description : get g_cb_alert_id[index]
//  Global resource dependence : none
//  Author: CBK
//  Note:
/*****************************************************************************/
PUBLIC MN_CB_MSG_ID_T MMIAPISMSCB_GetCBAlertID(uint8 index)
 {
     return g_cb_alert_id[index];
 }
//----------------------------------------------------------------------------------------------------------------
//
//  函数名                 ：MMIAPISMSCB_GetCBAllAlertInfo
//  功能描述           :
//  全局变量使用:    s_cb_all_alert_info
//  返回值                 :
//  作者                       :    Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MN_CB_ALERT_MSG_T * MMIAPISMSCB_GetCBAllAlertInfo(MN_DUAL_SYS_E dual_sys)
{
    return  &s_cb_all_alert_info[dual_sys];
}

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description :
//    Global resource dependence :
//  Author:Candice.Chen
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMSCB_GetETWSNum(void)
{
    return s_etws_alert_num;
}

/*****************************************************************************/
//     Description :
//    Global resource dependence :
//  Author:Candice.Chen
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetETWSNum(uint16 num)
{
    s_etws_alert_num = num;
}
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  函数名                 ：MMIAPISMSCB_InitCBAlertInfo
//  功能描述           :
//  全局变量使用:    s_cb_alert_msg
//  返回值                 :    void
//  作者                       :    Tong Yongli
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMIAPISMSCB_InitCBAlertInfo(MN_DUAL_SYS_E dual_sys)
{
    uint8 i = 0;
    MMISMS_CB_CHANNEL_T channel_list = {0};
#ifdef MMI_ETWS_SUPPORT
    MMISMS_CB_CHANNEL_T etws_channel_list = {0};
#endif

    s_cb_all_alert_info[dual_sys].emegence_num = 0;
#ifdef MMI_ETWS_SUPPORT
    etws_channel_list = InitCB_ETWSChannel();
#endif
    channel_list = InitCBSoftConfChannel();
#ifdef MMI_ETWS_SUPPORT
    // ETWS
    s_cb_all_alert_info[dual_sys].emegence_num = etws_channel_list.num;
    for(i = 0; i < s_cb_all_alert_info[dual_sys].emegence_num; i++)
    {
        s_cb_all_alert_info[dual_sys].cb_alert_content[i].type= MMI_CB_ETWS_ID;
        s_cb_all_alert_info[dual_sys].cb_alert_content[i].cb_alert_id = etws_channel_list.channel_arr[i].msg_id;
        s_cb_all_alert_info[dual_sys].cb_alert_content[i].name.wstr_len = etws_channel_list.channel_arr[i].name.wstr_len;
        MMIAPICOM_Wstrncpy(s_cb_all_alert_info[dual_sys].cb_alert_content[i].name.wstr, etws_channel_list.channel_arr[i].name.wstr, s_cb_all_alert_info[dual_sys].cb_alert_content[i].name.wstr_len ) ;
        s_cb_all_alert_info[dual_sys].cb_alert_content[i].is_enabled = etws_channel_list.channel_arr[i].is_enabled;
    }
    MMIAPISMSCB_SetETWSNum(s_cb_all_alert_info[dual_sys].emegence_num);

    //Default emergency
    if(0 == s_cb_all_alert_info[dual_sys].emegence_num)
    {
        for(i = 0; i < MMI_CB_ALERT_NUM; i++)
        {
            s_cb_all_alert_info[dual_sys].cb_alert_content[i].type = MMI_CB_EMERGENCE_ID;
            s_cb_all_alert_info[dual_sys].cb_alert_content[i].cb_alert_id = g_cb_alert_id[i];
            s_cb_all_alert_info[dual_sys].emegence_num++;
        }
    }
#else
    for(i = 0; i < MMI_CB_ALERT_NUM; i++)
    {
        s_cb_all_alert_info[dual_sys].cb_alert_content[i].type = MMI_CB_EMERGENCE_ID;
        s_cb_all_alert_info[dual_sys].cb_alert_content[i].cb_alert_id = g_cb_alert_id[i];
        s_cb_all_alert_info[dual_sys].emegence_num++;
    }
#endif

    s_cb_all_alert_info[dual_sys].soft_conf_num = channel_list.num;
    for(i = 0; i < s_cb_all_alert_info[dual_sys].soft_conf_num; i++)
    {
        s_cb_all_alert_info[dual_sys].soft_conf_id[i] = channel_list.channel_arr[i].msg_id;
    }

}

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         AddNewMsgNodeToCBMsgList
//  功能描述:
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            CBK
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void AddNewMsgNodeToCBMsgList(MMISMS_CB_MESSAGE_ITEM_T* cur_msg_ptr)
{
    MMISMS_CB_MESSAGE_ITEM_T *temp_msg_ptr = PNULL;
    MMISMS_CB_MESSAGE_ITEM_T *last_msg_ptr = PNULL;

    //收件箱的广播消息总数加一
    s_cb_msg.num++;
    if (PNULL == s_cb_msg.msg_ptr)
    {
        s_cb_msg.msg_ptr = cur_msg_ptr;
    }
    else
    {//结构调整，以前是最新的放在最后，调整后最新的放在最前面
        cur_msg_ptr->msg_next_ptr = s_cb_msg.msg_ptr;
        s_cb_msg.msg_ptr = cur_msg_ptr;

        if (s_cb_msg.num > MMI_MAX_CB_MSG_NUM)
        {//如果大于最大条数，销毁最后一个节点，把最新的插入到头节点。
            s_cb_msg.num = MMI_MAX_CB_MSG_NUM;
            temp_msg_ptr = s_cb_msg.msg_ptr;
            while (PNULL != temp_msg_ptr->msg_next_ptr)
            {//找到最后一个节点，然后销毁空间，倒数第二个节点next要置空
                last_msg_ptr = temp_msg_ptr;
                temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
            }
            if (PNULL != last_msg_ptr)
            {
                last_msg_ptr->msg_next_ptr = PNULL;
            }
            SCI_FREE(temp_msg_ptr);
            temp_msg_ptr = PNULL;
        }
    }
}

#ifdef MMI_ETWS_SUPPORT
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_Get_CB_Message_Num
//  功能描述:
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:           Candice
//
//----------------------------------------------------------------------------------------------------------------
 PUBLIC void MMISMS_Set_CB_Message_Num( uint16 num )
{
    if( num <= MMINV_MAX_SMSCB_RECORDS )
    {
        s_cb_msg.num = num;
    }
}

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_Get_CB_Message_Ptr
//  功能描述:
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC MMISMS_CB_MESSAGE_T *MMISMS_Get_CB_Message_Parent_Ptr( void )
{
    return &s_cb_msg;
}
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_Get_CB_Message_Num
//  功能描述:
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            jixin.xu
//
//----------------------------------------------------------------------------------------------------------------
 PUBLIC uint16 MMISMS_Get_CB_Message_Num( void )
{
    return s_cb_msg.num;
}

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_Get_CB_Message_Ptr
//  功能描述:
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            jixin.xu
//
//----------------------------------------------------------------------------------------------------------------
 PUBLIC MMISMS_CB_MESSAGE_ITEM_T *MMISMS_Get_CB_Message_Ptr( void )
{
    return s_cb_msg.msg_ptr;
}

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMISMS_DeleteMsgFromCBMsgList
//  功能描述:        从小区广播收件箱列表中删除一条记录
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            liqing.peng
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC void MMISMS_DeleteMsgFromCBMsgList(
                                  uint16      index
                                  )
{
    uint32 i = 0;
    MMISMS_CB_MESSAGE_ITEM_T *cur_msg_ptr = PNULL;
    MMISMS_CB_MESSAGE_ITEM_T *temp_msg_ptr = PNULL;
#ifdef MMI_ETWS_SUPPORT
    MMISMS_CB_MESSAGE_ARRAY_T* cb_message_array = PNULL;
    MMISMS_CB_MESSAGE_ARRAY_T* temp_message_array = PNULL;
    uint8 cb_num = 0;
    uint8 read_count = 0;
#endif

    //SCI_TRACE_LOW:"SMSCB: DeleteMsgFromCBMsgList num = d%, index = d%"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2297_112_2_18_2_55_33_895,(uint8*)"", s_cb_msg.num, index);
    if (0 == s_cb_msg.num || MMI_MAX_CB_MSG_NUM < index)
    {
        return;
    }

#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
    //读取最新一条的时候，需要停止滚动
    cur_msg_ptr = s_cb_msg.msg_ptr;

    if (0 == index && !cur_msg_ptr->is_read)
    {
        GUISTBSCROLLMSG_SetIsValid(GUISTBSCROLLMSG_SCROLL_ITEM_NEW_SMSCB, FALSE);
    }
#endif

    if (1 == s_cb_msg.num)
    {
        temp_msg_ptr = s_cb_msg.msg_ptr;
        SCI_FREE(temp_msg_ptr);
        temp_msg_ptr = PNULL;
        s_cb_msg.msg_ptr = PNULL;

#ifdef MMI_ETWS_SUPPORT
        MMISMS_NV_DelAllSmsCBItem();
#endif
    }
    else
    {
        if (0 == index)
        {//删除第一个节点
            temp_msg_ptr = s_cb_msg.msg_ptr;
            s_cb_msg.msg_ptr = temp_msg_ptr->msg_next_ptr;
            SCI_FREE(temp_msg_ptr);
            temp_msg_ptr = PNULL;
        }
        else
        {
            temp_msg_ptr = s_cb_msg.msg_ptr;
            //查找到需要删除的节点
            for (i=0; i < index; i++)
            {
                cur_msg_ptr = temp_msg_ptr;
                temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
            }
            if (PNULL != cur_msg_ptr)
            {
                cur_msg_ptr->msg_next_ptr = temp_msg_ptr->msg_next_ptr;
            }
            SCI_FREE(temp_msg_ptr);
            temp_msg_ptr = PNULL;
        }
    }
    s_cb_msg.num--;

#ifdef MMI_ETWS_SUPPORT
    temp_msg_ptr = s_cb_msg.msg_ptr;

    index = 0;
    cb_message_array = (MMISMS_CB_MESSAGE_ARRAY_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
    if ( PNULL == cb_message_array)
    {
        return ;
    }
    SCI_MEMSET(cb_message_array , 0, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));

    MMISMSNV_SaveSmsCBMessageListNum(s_cb_msg.num);

    while( (PNULL != temp_msg_ptr) && ( index < s_cb_msg.num))
    {
        SCI_MEMCPY(cb_message_array, temp_msg_ptr, sizeof(MMISMS_CB_MESSAGE_ARRAY_T));
        MMIAPISMS_NV_WriteSmsCBItem(TRUE, index, cb_message_array);
        temp_msg_ptr = temp_msg_ptr->msg_next_ptr;
        index ++;
    }

    if( s_cb_msg.num < MMINV_MAX_SMSCB_RECORDS)
    {
        MMISMS_NV_DelSmsCBItem(s_cb_msg.num, MMINV_MAX_SMSCB_RECORDS-1);
    }

    if( PNULL != cb_message_array)
    {
        SCI_FREE(cb_message_array);
        cb_message_array = PNULL;
    }
#endif

    //SCI_TRACE_LOW:"SMSCB: DeleteMsgFromCBMsgList num1 = d%"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2337_112_2_18_2_55_33_896,(uint8*)"", s_cb_msg.num);
}

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         IsCBMsgListEmpty
//  功能描述:        判断广播收件箱是否为空
//  全局变量使用:    s_cb_msg
//  返回值:          TRUE 为空，FALSE 不为空
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
PUBLIC BOOLEAN MMISMS_IsCBMsgListEmpty(void)
{
#ifdef MMI_ETWS_SUPPORT
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
    MMISMS_CB_MESSAGE_ARRAY_T* cb_message_array = PNULL;
    uint8 cb_num = 0;
#endif

    if(0 == s_cb_msg.num)
    {
#ifdef MMI_ETWS_SUPPORT
        cb_num = MMISMS_NV_LoadSmsCBMessageListNum();
        if( cb_num > 0)
        {
            return FALSE;
        }
        else
        {
            return TRUE;
        }
#else
        return TRUE;
#endif
    }
    else
    {
        return FALSE;
    }
}

#ifdef PDA_UI_DROPDOWN_WIN
/*****************************************************************************/
//  Description : Get Unread SMS CB Number
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMSCB_GetUnreadSMSCBNum(void)
{
    uint16 unread_num = 0;
    MMISMS_CB_MESSAGE_ITEM_T *cur_msg_ptr = PNULL;

    if (0 == s_cb_msg.num)
    {
        return unread_num;
    }

    cur_msg_ptr = s_cb_msg.msg_ptr;

    while (PNULL != cur_msg_ptr)
    {
        if (!cur_msg_ptr->is_read)
        {
            unread_num++;
        }

        cur_msg_ptr = cur_msg_ptr->msg_next_ptr;
    }

    return unread_num;
}

/*****************************************************************************/
//  Description : Get Unread SMS CB Channel
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetUnreadSMSCBChannel(wchar *name_ptr, uint16 max_len)
{
    MMISMS_CB_MESSAGE_ITEM_T *cur_msg_ptr = PNULL;
    MMISMS_CB_CHANNEL_T channel_list = {0};
    MMISMS_CB_CHANNEL_NAME_T channel_name = {0};
    uint8 channel_str[MMI_CB_CHANNEL_NAME_LEN] = {0};
    uint16 channel_len = 0;

    if (0 == s_cb_msg.num || PNULL == s_cb_msg.msg_ptr || PNULL == name_ptr || 0 == max_len)
    {
        return FALSE;
    }

    cur_msg_ptr = s_cb_msg.msg_ptr;

    while(PNULL != cur_msg_ptr)
    {
        if (!cur_msg_ptr->is_read)
        {
            break;
        }
        else
        {
            cur_msg_ptr = cur_msg_ptr->msg_next_ptr;
        }
    }

    if (PNULL == cur_msg_ptr)
    {
        return FALSE;
    }

    if (cur_msg_ptr->dual_sys < MMI_DUAL_SYS_MAX)
    {
        channel_list = MMISMS_NV_LoadSmsCBChannelList(cur_msg_ptr->dual_sys);
    }

    channel_name = MMISMS_GetCBChannelNameByMsgId(&channel_list, cur_msg_ptr->msg_id);

    if (0 == channel_name.wstr_len)
    {
        channel_len = sprintf((char *)channel_str, "%d", cur_msg_ptr->msg_id);

        MMI_STRNTOWSTR(channel_name.wstr,
                                MMI_CB_CHANNEL_NAME_LEN,
                                (uint8 *)channel_str,
                                MMI_CB_CHANNEL_NAME_LEN,
                                channel_len
                                );

        channel_name.wstr_len = channel_len;
    }

    channel_name.wstr_len = MIN(max_len, channel_name.wstr_len);

    MMI_WSTRNCPY(name_ptr, channel_name.wstr_len, channel_name.wstr, channel_name.wstr_len, channel_name.wstr_len);

    return TRUE;
}

/*****************************************************************************/
//  Description : Get Unread SMS CB Time
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetUnreadSMSCBTime(wchar *time_ptr, uint16 max_len)
{
    MMISMS_CB_MESSAGE_ITEM_T *cur_msg_ptr = PNULL;
    uint8 time[MMI_DISPLAY_TIME_LEN + 1] = {0};
    uint16 time_len = 0;

    if (0 == s_cb_msg.num || PNULL == s_cb_msg.msg_ptr || PNULL == time_ptr || 0 == max_len)
    {
        return FALSE;
    }

    cur_msg_ptr = s_cb_msg.msg_ptr;

    while(PNULL != cur_msg_ptr)
    {
        if (!cur_msg_ptr->is_read)
        {
            break;
        }
        else
        {
            cur_msg_ptr = cur_msg_ptr->msg_next_ptr;
        }
    }

    if (PNULL == cur_msg_ptr)
    {
        return FALSE;
    }

    MMIAPISET_FormatTimeStrByTime(cur_msg_ptr->time.hour, cur_msg_ptr->time.min, time, MMI_DISPLAY_TIME_LEN);

    time_len = strlen((char *)time);

    time_len = MIN(max_len, time_len);

    MMI_STRNTOWSTR(time_ptr, time_len, time, time_len, time_len);

    return TRUE;
}
#endif

#ifdef MMI_STATUSBAR_SCROLLMSG_SUPPORT
/*****************************************************************************/
//  Description : 添加滚动信息
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetScrollItemNewSMSCB(void)
{
    GUISTBSCROLLMSG_SCROLL_ITEM_T scroll_item = {0};
    MMI_STRING_T msg_arr[3] = {0};
    wchar colon_str[]=L": \n";
    uint32 total_len = 0;
    MMI_STRING_T smscb_name = {0};
    MMISMS_CB_MESSAGE_ITEM_T *cur_msg_ptr = PNULL;

    if (0 == s_cb_msg.num || PNULL == s_cb_msg.msg_ptr)
    {
        return;
    }

    scroll_item.is_valid = TRUE;
    scroll_item.type = GUISTBSCROLLMSG_SCROLL_ITEM_NEW_SMSCB;

    //get smscb name
    MMI_GetLabelTextByLang(TXT_SMS_CB_RECEIVE_NEW, &smscb_name);

    //icon
    scroll_item.icon_id = IMAGE_IDLE_TITLE_ICON_MESSAGE;

    cur_msg_ptr = s_cb_msg.msg_ptr;

    msg_arr[0] = smscb_name;
    msg_arr[1].wstr_ptr = colon_str;
    msg_arr[1].wstr_len = MMIAPICOM_Wstrlen(colon_str);
    msg_arr[2].wstr_ptr = cur_msg_ptr->context;
    msg_arr[2].wstr_len = cur_msg_ptr->len;

    total_len = msg_arr[0].wstr_len+msg_arr[1].wstr_len+msg_arr[2].wstr_len;
    scroll_item.string_info.wstr_ptr = SCI_ALLOCAZ((total_len + 1)*sizeof(wchar));
    scroll_item.string_info.wstr_len = total_len;
    MMIAPICOM_ConnectSomeString(msg_arr, 3, &scroll_item.string_info);

    GUISTBSCROLLMSG_AppendScrollItem(&scroll_item);

    if (PNULL != scroll_item.string_info.wstr_ptr)
    {
        SCI_FREE(scroll_item.string_info.wstr_ptr);
    }
}
#endif


#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//  Description : handle counted time timer over msg
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_HandleETWSVibrateTimer(uint8 timer_id, uint32 param)
{
    BOOLEAN is_continued = (BOOLEAN) param;

    if( 0 == is_continued )
    {
        MMIAPISET_StopSOSVibrate();

        MMK_StopTimer(s_vibrator_period_time_id);
        s_vibrator_period_time_id = 0;
    }

#ifdef CAMERA_SUPPORT
    if (MMK_IsOpenWin(MMIDC_MAIN_WIN_ID))
    {
        s_is_etws_display = TRUE;
    }
#endif

    MMIDEFAULT_TurnOnBackLight();
#ifdef SCREENSAVER_SUPPORT
    if (MMK_IsFocusWin(MMIIDLE_SCREENSAVER_WIN_ID))
    {
        MMIAPIIDLESS_CloseScreenSaver();
    }
#endif
    s_is_need_enable = TRUE;
    if(s_is_etws_prim_msg)
    {
         s_is_etws_prim_msg = FALSE;
    }

    if( MMICC_IsExistIncommingCall())
    {
        MMIAPICC_PlayNewCallRing(MMIAPICC_GetCallDualSys());
    }
}

/*****************************************************************************/
//  Description : handle counted time timer over msg
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_IsETWSAlert(void)
{
    if( 0 == s_vibrator_period_time_id  )
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

/*****************************************************************************/
//  Description : judge ETWS alert status
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_IsCMASChannelAlert(void)
{
    MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
    CMAS_CHANNEL_INFO_T* current_channel_table = GetCMASChannelTable(dual_sys);

    if( NULL == current_channel_table )
    {
        SCI_TRACE_LOW("[MMISMSCB] GetCMASChannelInfo, current_channel_table = NULL");
        return FALSE;
    }

    if( 0 != s_vibrator_period_time_id || SMSCB_ALERT_VIBRATE_FOREVER == current_channel_table->alert_period)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_StopETWSVibrateTimer(void)
{
    MMIAPISET_StopSOSVibrate();

    MMK_StopTimer(s_vibrator_period_time_id);
    s_vibrator_period_time_id = 0;
}


/*****************************************************************************/
//  Description : stop timer
//  Global resource dependence : none
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void MMISMSCB_HandleETWSRepeatedTimer(uint8 timer_id, uint32 param)
{
    s_repeated_channel = 0;

    MMK_StopTimer(s_repeated_period_time_id);
    s_repeated_period_time_id = 0;
}
#endif

void smscb_receivedata_print(APP_MN_SMSCB_MSG_IND_T *cb_msg_ptr)
{
    SCI_TraceLow("smscb_receivedata_print:msg_code=%d,display_mode=%d,dual_sys=%d,msg_len=%d,msg_id=%d",\
    cb_msg_ptr->msg_code,cb_msg_ptr->display_mode,cb_msg_ptr->dual_sys,cb_msg_ptr->msg_len,cb_msg_ptr->msg_id);

    SCI_TraceLow("smscb_receivedata_print:alphabet_type=%d,class_is_present=%d,language_id=%d,language_id_present=%d,sms_class=%d",\
    cb_msg_ptr->dcs.alphabet_type,cb_msg_ptr->dcs.class_is_present,cb_msg_ptr->dcs.language_id,cb_msg_ptr->dcs.language_id_present,cb_msg_ptr->dcs.sms_class);

    if(cb_msg_ptr->msg_len > 0)
    {
        SCI_TraceBuf("SMSCB: smscb_receivedata_print, receive data content: ",cb_msg_ptr->msg_arr,cb_msg_ptr->msg_len);
    }
}

#if defined (MODEM_SUPPORT_SPRD)
void smscb_prim_receivedata_print(APP_MN_ETWS_PRIM_IND_T *cb_msg_ptr)
{
    SCI_TraceLow("smscb_prim_receivedata_print: display_mode=%d, alert_mode=%d,  warning_type= %d, dual_sys=%d,msg_len=%d,msg_id=%d",\
    cb_msg_ptr->display_mode, cb_msg_ptr->alert_mode, cb_msg_ptr->warning_type, cb_msg_ptr->dual_sys,cb_msg_ptr->msg_len,cb_msg_ptr->msg_id);
}
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMIAPISMSCB_HandleCBMsgInd
//  功能描述:        收到新小区广播的处理函数
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
void MMIAPISMSCB_HandleCBMsgInd(
                             DPARAM        param            //IN: 系统回调参数

                             )
{
    APP_MN_SMSCB_MSG_IND_T *cb_msg_ptr = (APP_MN_SMSCB_MSG_IND_T *)param;
#ifdef MMI_ETWS_SUPPORT
    CMAS_CHANNEL_INFO_T* current_channel_info = NULL;
    MMI_STRING_T prompt_content_str = {0};
    MMI_STRING_T prompt_str = {0};
    MMISMS_CB_MESSAGE_ITEM_T* cb_msg_item = {0};
    uint32 msg_real_len = 0;
    uint8 ring_vol=0;
    uint16 etws_alert_num = 0;
    wchar time_str[CB_MSG_TIME_MAX_LEN + 1] = {0};
    uint16 time_len = 0;
    uint16 etws_context_len = 0;
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
    MMI_STRING_T etws_title = {0};
    uint16 etws_title_len = 0;
#endif

    if (PNULL == cb_msg_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMIAPISMSCB_HandleCBMsgInd cb_msg_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2492_112_2_18_2_55_33_899,(uint8*)"");
        return;
    }

    smscb_receivedata_print(cb_msg_ptr);

#ifdef MMI_ETWS_SUPPORT
    etws_alert_num = MMIAPISMSCB_GetETWSNum();

    if( etws_alert_num > 0)
    {
        MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
        MN_CB_ALERT_MSG_T *cb_msg_id_ptr = PNULL;
        MMISMS_CB_CHANNEL_T etws_channel_list = {0};

        etws_channel_list = InitCB_ETWSChannel();
    }

    current_channel_info = GetCMASChannelInfo(cb_msg_ptr->dual_sys, cb_msg_ptr->msg_id);

    if( NULL != current_channel_info)
    {
        cb_msg_item = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
        if (PNULL == cb_msg_item)
        {
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleCBMsgInd, allocate cb_msg_item buffer failed");
            return;
        }

        SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleCBMsgInd, ETWS received");
        // add cb to msg list
        AddNewMsgToCBMsgList(cb_msg_ptr, FALSE);

        // ignore secondary channel cb
        if( 1 == current_channel_info->primary_channel)
        {
            s_repeated_channel = current_channel_info->secondary_channel;
            s_repeated_period_time_id = MMK_CreateTimerCallback(MMI_5MINUTES,  MMISMSCB_HandleETWSRepeatedTimer, NULL, FALSE);
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleCBMsgInd, current_channel_info->primary_channel = %d", current_channel_info->primary_channel);
        }
        else  if(( 1 == current_channel_info->secondary_channel )  &&  (s_repeated_channel == cb_msg_ptr->msg_id)  )
        {
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleCBMsgInd, current_channel_info->secondary_channel = %d, cb_msg_ptr->msg_id = %d", current_channel_info->secondary_channel, cb_msg_ptr->msg_id);
            return;
        }
        else
        {
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleCBMsgInd, neither primary channel nor secondary channel");
        }

        // init popup string buffer
        etws_context_len = MMI_CB_ETWS_TITLE_NAME_MAX_LEN + 1 \
            +CB_MSG_TIME_MAX_LEN + 1\
            + MN_CB_MSG_LEN + 1;
        etws_title_len = MMI_CB_ETWS_TITLE_NAME_MAX_LEN + 1;

        prompt_content_str.wstr_ptr = SCI_ALLOCAZ(MN_CB_MSG_LEN * sizeof(wchar));
        if (PNULL == prompt_content_str.wstr_ptr)
        {
            SCI_TRACE_LOW("MMISMSCB:alloc prompt_content_str.wstr_ptr failed");
            return;
        }
        SCI_MEMSET(prompt_content_str.wstr_ptr , 0, MN_CB_MSG_LEN* sizeof(wchar));
        prompt_content_str.wstr_len = 0;

        prompt_str.wstr_ptr = SCI_ALLOCAZ(etws_context_len * sizeof(wchar));
        if (PNULL == prompt_str.wstr_ptr)
        {
            SCI_TRACE_LOW("MMISMSCB:alloc prompt_str.wstr_ptr failed");
            return;
        }
        SCI_MEMSET(prompt_str.wstr_ptr , 0, etws_context_len* sizeof(wchar));
        prompt_str.wstr_len = 0;

        if( PNULL != etws_prompt_str.wstr_ptr )
        {
            SCI_FREE(etws_prompt_str.wstr_ptr);
        }
        etws_prompt_str.wstr_ptr = SCI_ALLOCAZ(etws_context_len * sizeof(wchar));
        if (PNULL == etws_prompt_str.wstr_ptr)
        {
            SCI_TRACE_LOW("MMISMSCB:alloc etws_prompt_str.wstr_ptr failed");
            return;
        }
        SCI_MEMSET(etws_prompt_str.wstr_ptr , 0, etws_context_len* sizeof(wchar));
        etws_prompt_str.wstr_len = etws_context_len;

        // Title
        if( PNULL != etws_title_str.wstr_ptr )
        {
            SCI_FREE(etws_title_str.wstr_ptr);
        }
        etws_title_str.wstr_ptr = SCI_ALLOCAZ(etws_title_len * sizeof(wchar));
        if (PNULL == etws_title_str.wstr_ptr)
        {
            SCI_TRACE_LOW("MMISMSCB:alloc etws_title_str.wstr_ptr failed");
            return;
        }
        SCI_MEMSET(etws_title_str.wstr_ptr , 0, etws_title_len* sizeof(wchar));
        etws_title_str.wstr_len = 0;

        SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));
        plmn_info = MNPHONE_GetCurrentPLMNInfoEx(cb_msg_ptr->dual_sys);

        if( 0 != current_channel_info->string_id )
        {
             MMI_GetLabelTextByLang(current_channel_info->string_id , &etws_title);
             MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, etws_title.wstr_ptr, etws_title.wstr_len) ;
             MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, L"\n");
             prompt_str.wstr_len = etws_title.wstr_len+1;
        }
        else
        {
             MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, current_channel_info->title_id, MMIAPICOM_Wstrlen(current_channel_info->title_id)) ;
             MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, L"\n");
             prompt_str.wstr_len = MMIAPICOM_Wstrlen(current_channel_info->title_id) +1;
        }

        MMIAPICOM_Wstrncpy(etws_title_str.wstr_ptr, prompt_str.wstr_ptr, prompt_str.wstr_len );
        etws_title_str.wstr_len = prompt_str.wstr_len;

        SCI_MEMSET(prompt_str.wstr_ptr , 0, etws_context_len* sizeof(wchar));
        prompt_str.wstr_len = 0;

        //Content
        MMI_WSTRNCPY(prompt_str.wstr_ptr+prompt_str.wstr_len,
            MN_CB_MSG_LEN,
            s_cb_msg.msg_ptr->context,
            s_cb_msg.msg_ptr->len,
            s_cb_msg.msg_ptr->len);

        prompt_str.wstr_len += s_cb_msg.msg_ptr->len;

        MMIAPICOM_Wstrncpy(etws_prompt_str.wstr_ptr, prompt_str.wstr_ptr, prompt_str.wstr_len );
        etws_prompt_str.wstr_len = prompt_str.wstr_len;

        if (MMK_IsOpenWin(MMISMS_CB_MSG_LIST_WIN_ID))
        {
            uint16 cur_index = 0;

            cur_index = GUILIST_GetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID);
            //刷新当前小区广播收件箱列表
            GUILIST_RemoveAllItems(MMISMS_CB_MSG_LISTBOX_CTRL_ID);
            MMISMS_UpdateCBMsgList(MMISMS_CB_MSG_LISTBOX_CTRL_ID);

            if( (cur_index + 1 ) < MMI_MAX_CB_MSG_NUM )
            {
                GUILIST_SetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID, cur_index + 1);
            }
            else
            {
                GUILIST_SetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID, cur_index );
            }

            if (MMK_IsFocusWin(MMISMS_CB_MSG_LIST_WIN_ID))
            {
                MMK_SendMsg(MMISMS_CB_MSG_LIST_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
        }

        // alert
        if( current_channel_info->alert_level > 0)
        {
            s_is_etws_alert_msg = TRUE;
        }

        if(current_channel_info->alert_tone_type)
        {
            s_is_etws_alert_msg = TRUE;
            s_is_etws_need_popup = TRUE;
        }

        SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleCBMsgInd, vibrate_type=%d,mcc=%d ",current_channel_info->vibrate_type,plmn_info.mcc);

        s_etws_vibrate_type_context = current_channel_info->vibrate_type;
        // vibrate
        if(current_channel_info->vibrate_type)
        {
            // Display ETWS prompt
            MMIDEFAULT_TurnOnBackLight();

            s_etws_delay_show_dual_sim_context = cb_msg_ptr->dual_sys;

            if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
            {
                MMIAPISMSCB_StopETWSVibrateTimer();
                MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
            }

            if( MMICC_IsExistIncommingCall())
            {
                MMIAPICC_StopRingOrVibrate();
            }


            if(CB_PWS_MCC == plmn_info.mcc)
            {
                s_is_need_enable = TRUE;
                s_is_personal = TRUE;
                s_is_need_torch = FALSE;
            }
            else
            {
                s_is_need_enable = FALSE;
                s_is_personal = FALSE;
                s_is_need_torch = FALSE;
            }
            s_etws_card_sys = cb_msg_ptr->dual_sys;

            MMIAPISMSCB_SetEMCallStatus(FALSE);
            MMIAPISMSCB_SetIsNeedRedisplay(FALSE);

            if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
            {
                MMIAPISMSCB_StopETWSVibrateTimer();
                MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
            }

    // stop recording
#ifdef MMI_RECORD_SUPPORT
            if( MMIAPIRECORD_IsRecording() )
            {
                 MMK_SendMsg(MMIRECORD_MAINPLAY_WIN_ID, MSG_APP_OK, PNULL);
            }
#endif
#ifdef CAMERA_SUPPORT
            if (MMK_IsOpenWin(MMIDC_MAIN_WIN_ID))
            {
                 s_is_etws_display = TRUE;
            }
            MMIAPIDC_Interrupt();
#endif

            s_cur_display_cb_msg_index = 0;

            MMISMS_ShowNewETWSPrompt(&etws_prompt_str, &etws_title_str, s_is_need_enable);
            MMIAPISET_SetSOSVibrate(s_etws_card_sys , s_is_personal, s_is_need_torch);

            // stop vibration and alert after timeout
            switch(current_channel_info->alert_period)
            {
                case 5:
                    s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_5SECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                break;

                case 30:
                    s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_30SECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                break;
			case SMSCB_ALERT_VIBRATE_FOREVER:
				break;
                case 10:
                default:
                    s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_10500MSECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                break;
            }
        }
        else
        {
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleCBMsgInd normal ringtone and vibration");

            // Display ETWS prompt
            MMIDEFAULT_TurnOnBackLight();
            if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
            {
                 MMIAPISMSCB_StopETWSVibrateTimer();
                 MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
            }

            MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
            MMISMS_SetNewSMSRingFlag(TRUE);
            MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());

            s_is_need_enable = TRUE;
            MMISMS_ShowNewETWSPrompt(&etws_prompt_str, &etws_title_str, s_is_need_enable);
        }


    if( PNULL != prompt_content_str.wstr_ptr)
    {
        SCI_FREE(prompt_content_str.wstr_ptr);
        prompt_content_str.wstr_ptr = PNULL;
    }

    if( PNULL != prompt_str.wstr_ptr)
    {
        SCI_FREE(prompt_str.wstr_ptr);
        prompt_str.wstr_ptr= PNULL;
    }

        return ;
    }
#endif

    s_is_alert_type_msg = ((cb_msg_ptr->msg_code) >> 9) & 0x1;
    //SCI_TRACE_LOW:"SMSCB: MMIAPISMSCB_HandleCBMsgInd dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2495_112_2_18_2_55_33_900,(uint8*)"d", cb_msg_ptr->dual_sys);
#ifdef MMI_ETWS_SUPPORT
    s_is_etws_alert_msg = ((cb_msg_ptr->msg_code) >> 9) & 0x01;
    s_is_etws_need_popup = ((cb_msg_ptr->msg_code) >> 8) & 0x01;
#endif

    if (((1 == cb_msg_ptr->dcs.class_is_present)
        && (MN_SMS_CLASS_0 == cb_msg_ptr->dcs.sms_class)
#ifdef MMI_ETWS_SUPPORT
        && (!s_is_etws_alert_msg)
#endif
        )
        || ((MN_CB_DISPLAY_IMMEDIATE == cb_msg_ptr->display_mode)
#ifdef MMI_ETWS_SUPPORT
        && (!s_is_etws_alert_msg)
#endif
        )
#ifdef MMI_ETWS_SUPPORT
        || (s_is_etws_need_popup && s_is_etws_alert_msg)
#endif
    )
    {
        //若收到的广播信息为 CLASS0 级且要立即显示则提取消息的有关信息
        //调用窗口立即显示有关内容
        //先关闭阅读窗口再打开保证了若此时在阅读窗口下也能刷新新的信息内容
        MMK_CloseWin(MMISMS_CB_CLASS0_READ_WIN_ID);
        //MMK_CreateWin((uint32*)MMISMS_CB_CLASS0_READ_WIN_TAB ,PNULL);

#ifdef MMI_ETWS_SUPPORT
        if(!s_is_etws_alert_msg)
        {
            MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
            MMISMS_SetNewSMSRingFlag(TRUE);
            MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());
        }
        else
        {
            MMIAPISET_PlayRingById(TRUE, 1, 1, MMISET_RING_TYPE_ALARM, PNULL);
            MMIAPISET_SetVibratorEx(TRUE, MMISET_RING_TYPE_MSG, TRUE, MMISMSCB_VIBRATE_INTERVAL);
        }
#else
        MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
        MMISMS_SetNewSMSRingFlag(TRUE);
        MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());
        if(s_is_alert_type_msg)
        {
            MMIAPISET_PlayRingById(TRUE, 1,1,MMISET_RING_TYPE_ALARM,PNULL);
            MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,TRUE,MMISMSCB_VIBRATE_INTERVAL);
	 }
#endif

        MMISMSCB_OpenCLASS0Win();

        MMISMSCB_DirectDisplayNewCBMsg(cb_msg_ptr);
    }
    else
    {
        //若收到的广播信息为普通级且要保存到广播消息收件箱列表中去并显示提示窗口
        //添加新的广播消息内容到广播消息收件箱列表中去
        AddNewMsgToCBMsgList(cb_msg_ptr, FALSE);

        //若在小区广播收件箱列表窗口中则要刷新列表
        if (MMK_IsOpenWin(MMISMS_CB_MSG_LIST_WIN_ID))
        {
            uint16 cur_index = 0;

            cur_index = GUILIST_GetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID);;

            //刷新当前小区广播收件箱列表
            GUILIST_RemoveAllItems(MMISMS_CB_MSG_LISTBOX_CTRL_ID);
            MMISMS_UpdateCBMsgList(MMISMS_CB_MSG_LISTBOX_CTRL_ID);

            GUILIST_SetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID, cur_index + 1);

            if (MMK_IsFocusWin(MMISMS_CB_MSG_LIST_WIN_ID))
            {
                MMK_SendMsg(MMISMS_CB_MSG_LIST_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
        }

#ifdef MMI_ETWS_SUPPORT
        if(!s_is_etws_alert_msg)
        {
            MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
            MMISMS_SetNewSMSRingFlag(TRUE);
        }
        else
        {
            MMIAPISET_PlayRingById(TRUE, 1, 1, MMISET_RING_TYPE_ALARM, PNULL);
            MMIAPISET_SetVibratorEx(TRUE, MMISET_RING_TYPE_MSG, TRUE, MMISMSCB_VIBRATE_INTERVAL);
        }
#else
        MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
        MMISMS_SetNewSMSRingFlag(TRUE);
#endif

        MMISMS_ShowNewSMSCBPrompt();
        MMIDEFAULT_TurnOnBackLight();
    }
}

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMIAPISMSCB_HandleMarcoCBMsgInd
//  功能描述:        收到新小区广播的处理函数
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
void macro_smscb_receivedata_print(APP_MN_SMSCB_MACRO_MSG_IND_T *cb_msg_ptr)
{
    SCI_TraceLow("macro_smscb_receivedata_print:msg_code=%d,display_mode=%d,dual_sys=%d,msg_len=%d,msg_id=%d",\
    cb_msg_ptr->msg_code,cb_msg_ptr->display_mode,cb_msg_ptr->dual_sys,cb_msg_ptr->msg_len,cb_msg_ptr->msg_id);

    SCI_TraceLow("macro_smscb_receivedata_print:alphabet_type=%d,class_is_present=%d,language_id=%d,language_id_present=%d,sms_class=%d",\
    cb_msg_ptr->dcs.alphabet_type,cb_msg_ptr->dcs.class_is_present,cb_msg_ptr->dcs.language_id,cb_msg_ptr->dcs.language_id_present,cb_msg_ptr->dcs.sms_class);

    if(cb_msg_ptr->msg_len > 0)
    {
        SCI_TraceBuf("SMSCB: macro_smscb_receivedata_print, receive data content: ",cb_msg_ptr->msg_arr,cb_msg_ptr->msg_len);
    }
}

#if defined (MODEM_SUPPORT_SPRD)
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         macro_lte_warning_receivedata_print
//  功能描述:        收到新小区广播的处理函数
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
void macro_lte_warning_receivedata_print(APP_MN_LTE_WARNING_IND_T *cb_msg_ptr)
{
    SCI_TRACE_LOW("macro_lte_warning_receivedata_print:display_mode=%d,dual_sys=%d,msg_len=%d,msg_id=%d, msg_ptr=%p",\
    cb_msg_ptr->display_mode,cb_msg_ptr->dual_sys,cb_msg_ptr->msg_len,cb_msg_ptr->msg_id, cb_msg_ptr->msg_ptr);

    SCI_TRACE_LOW("macro_lte_warning_receivedata_print:alphabet_type=%d,class_is_present=%d,language_id=%d,language_id_present=%d,sms_class=%d",\
    cb_msg_ptr->dcs.alphabet_type,cb_msg_ptr->dcs.class_is_present,cb_msg_ptr->dcs.language_id,cb_msg_ptr->dcs.language_id_present,cb_msg_ptr->dcs.sms_class);

    if(cb_msg_ptr->msg_len > 0)
    {
        SCI_TraceBuf("SMSCB: macro_lte_warning_receivedata_print, receive data content: ",cb_msg_ptr->msg_ptr,cb_msg_ptr->msg_len);
    }
}
#endif

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMIAPISMSCB_HandleMarcoCBMsgInd
//  功能描述:        收到新小区广播的处理函数
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
void MMIAPISMSCB_HandleMarcoCBMsgInd(
                             DPARAM        param            //IN: 系统回调参数
                             )
{
    APP_MN_SMSCB_MACRO_MSG_IND_T *cb_msg_ptr = (APP_MN_SMSCB_MACRO_MSG_IND_T *)param;
#ifdef MMI_ETWS_SUPPORT
    CMAS_CHANNEL_INFO_T* current_channel_info = NULL;
    MMI_STRING_T prompt_content_str = {0};
    MMI_STRING_T prompt_str = {0};
    MMISMS_CB_MESSAGE_ITEM_T* cb_msg_item = {0};
    uint32 msg_real_len = 0;
    uint8 ring_vol=0;
    uint16 etws_alert_num = 0;
    wchar time_str[CB_MSG_TIME_MAX_LEN + 1] = {0};
    uint16 time_len = 0;
    uint16 etws_context_len = 0;
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
    MMI_STRING_T etws_title = {0};
    uint16 etws_title_len = 0;
#endif

    if (PNULL == cb_msg_ptr)
    {
        //SCI_TRACE_LOW:"MMISMS:MMIAPISMSCB_HandleMarcoCBMsgInd cb_msg_ptr = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2557_112_2_18_2_55_33_901,(uint8*)"");
        return;
    }
    //SCI_TRACE_LOW:"SMSCB: MMISMSCB_HandleMacroCBMsgInd dual_sys = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMISMSCB_WINTAB_2560_112_2_18_2_55_33_902,(uint8*)"d", cb_msg_ptr->dual_sys);

    macro_smscb_receivedata_print(cb_msg_ptr);

#ifdef MMI_ETWS_SUPPORT
    etws_alert_num = MMIAPISMSCB_GetETWSNum();

    if( etws_alert_num > 0)
    {
        MN_CB_ALERT_MSG_T *cb_msg_id_ptr = PNULL;
        MMISMS_CB_CHANNEL_T etws_channel_list = {0};

        etws_channel_list = InitCB_ETWSChannel();
    }

    current_channel_info = GetCMASChannelInfo(cb_msg_ptr->dual_sys, cb_msg_ptr->msg_id);

    if( NULL != current_channel_info)
    {
        cb_msg_item = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
        if (PNULL == cb_msg_item)
        {
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleMarcoCBMsgInd, allocate cb_msg_item buffer failed");
            return;
        }

        SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleMarcoCBMsgInd, ETWS received");

    // stop recording
#ifdef MMI_RECORD_SUPPORT
        if( MMIAPIRECORD_IsRecording() )
        {
            MMK_SendMsg(MMIRECORD_MAINPLAY_WIN_ID, MSG_APP_OK, PNULL);
        }
#endif
#ifdef CAMERA_SUPPORT
        if (MMK_IsOpenWin(MMIDC_MAIN_WIN_ID))
        {
            s_is_etws_display = TRUE;
        }
        MMIAPIDC_Interrupt();
#endif

        // add cb to msg list
        AddNewMarcoMsgToCBMsgList(cb_msg_ptr);

        // ignore secondary channel cb
        if( 1 == current_channel_info->primary_channel)
        {
            s_repeated_channel = current_channel_info->secondary_channel;
            s_repeated_period_time_id = MMK_CreateTimerCallback(MMI_5MINUTES,  MMISMSCB_HandleETWSRepeatedTimer, NULL, FALSE);
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleMarcoCBMsgInd, current_channel_info->primary_channel = %d", current_channel_info->primary_channel);
        }
        else  if(( 1 == current_channel_info->secondary_channel )  &&  (s_repeated_channel == cb_msg_ptr->msg_id)  )
        {
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleMarcoCBMsgInd, current_channel_info->secondary_channel = %d, cb_msg_ptr->msg_id = %d", current_channel_info->secondary_channel, cb_msg_ptr->msg_id);
            return;
        }
        else
        {
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleMarcoCBMsgInd, neither primary channel nor secondary channel");
        }

        // init popup string buffer
        etws_context_len = MMI_CB_ETWS_TITLE_NAME_MAX_LEN + 1 \
            +CB_MSG_TIME_MAX_LEN + 1\
            + MN_CB_MACRO_MSG_LEN + 1;
        etws_title_len = MMI_CB_ETWS_TITLE_NAME_MAX_LEN + 1;

        prompt_content_str.wstr_ptr = SCI_ALLOCAZ(MN_CB_MACRO_MSG_LEN * sizeof(wchar));
        if (PNULL == prompt_content_str.wstr_ptr)
        {
            SCI_TRACE_LOW("MMISMSCB:alloc prompt_content_str.wstr_ptr failed");
            return;
        }
        SCI_MEMSET(prompt_content_str.wstr_ptr , 0, MN_CB_MACRO_MSG_LEN * sizeof(wchar));
        prompt_content_str.wstr_len = 0;

        prompt_str.wstr_ptr = SCI_ALLOCAZ(etws_context_len * sizeof(wchar));
        if (PNULL == prompt_str.wstr_ptr)
        {
            SCI_TRACE_LOW("MMISMSCB:alloc prompt_str.wstr_ptr failed");
            return;
        }
        SCI_MEMSET(prompt_str.wstr_ptr , 0, etws_context_len * sizeof(wchar));
        prompt_str.wstr_len = 0;

        if( PNULL != etws_prompt_str.wstr_ptr )
        {
            SCI_FREE(etws_prompt_str.wstr_ptr);
        }
        etws_prompt_str.wstr_ptr = SCI_ALLOCAZ(etws_context_len * sizeof(wchar));
        if (PNULL == etws_prompt_str.wstr_ptr)
        {
            SCI_TRACE_LOW("MMISMSCB:alloc etws_prompt_str.wstr_ptr failed");
            return;
        }
        SCI_MEMSET(etws_prompt_str.wstr_ptr , 0, etws_context_len * sizeof(wchar));
        etws_prompt_str.wstr_len = etws_context_len;

        if( PNULL != etws_title_str.wstr_ptr )
        {
            SCI_FREE(etws_title_str.wstr_ptr);
        }
        etws_title_str.wstr_ptr = SCI_ALLOCAZ(etws_title_len * sizeof(wchar));
        if (PNULL == etws_title_str.wstr_ptr)
        {
            SCI_TRACE_LOW("MMISMSCB:alloc etws_title_str.wstr_ptr failed");
            return;
        }
        SCI_MEMSET(etws_title_str.wstr_ptr , 0, etws_title_len* sizeof(wchar));
        etws_title_str.wstr_len = 0;

        // Title
        SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));
        plmn_info = MNPHONE_GetCurrentPLMNInfoEx(cb_msg_ptr->dual_sys);
        if( 0 != current_channel_info->string_id )
        {
            MMI_GetLabelTextByLang(current_channel_info->string_id , &etws_title);
            MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, etws_title.wstr_ptr, etws_title.wstr_len) ;
            MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, L"\n");
            prompt_str.wstr_len = etws_title.wstr_len+1;
        }
        else
        {
            MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, current_channel_info->title_id, MMIAPICOM_Wstrlen(current_channel_info->title_id)) ;
            MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, L"\n");
            prompt_str.wstr_len = MMIAPICOM_Wstrlen(current_channel_info->title_id) +1;
        }

        MMIAPICOM_Wstrncpy(etws_title_str.wstr_ptr, prompt_str.wstr_ptr, prompt_str.wstr_len );
        etws_title_str.wstr_len = prompt_str.wstr_len;

        SCI_MEMSET(prompt_str.wstr_ptr , 0, etws_context_len* sizeof(wchar));
        prompt_str.wstr_len = 0;


        //Content
        MMI_WSTRNCPY(prompt_str.wstr_ptr+prompt_str.wstr_len,
            MN_CB_MACRO_MSG_LEN,
            s_cb_msg.msg_ptr->context,
            s_cb_msg.msg_ptr->len,
            s_cb_msg.msg_ptr->len);

        prompt_str.wstr_len += s_cb_msg.msg_ptr->len;

        MMIAPICOM_Wstrncpy(etws_prompt_str.wstr_ptr, prompt_str.wstr_ptr, prompt_str.wstr_len );
        etws_prompt_str.wstr_len = prompt_str.wstr_len;

        if (MMK_IsOpenWin(MMISMS_CB_MSG_LIST_WIN_ID))
        {
            uint16 cur_index = 0;

            cur_index = GUILIST_GetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID);

            //刷新当前小区广播收件箱列表
            GUILIST_RemoveAllItems(MMISMS_CB_MSG_LISTBOX_CTRL_ID);
            MMISMS_UpdateCBMsgList(MMISMS_CB_MSG_LISTBOX_CTRL_ID);

            if( (cur_index + 1 ) < MMI_MAX_CB_MSG_NUM )
            {
                GUILIST_SetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID, cur_index + 1);
            }
            else
            {
                GUILIST_SetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID, cur_index );
            }

            if (MMK_IsFocusWin(MMISMS_CB_MSG_LIST_WIN_ID))
            {
                MMK_SendMsg(MMISMS_CB_MSG_LIST_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
        }

        // alert
        if( current_channel_info->alert_level > 0)
        {
            s_is_etws_alert_msg = TRUE;
        }

        if(current_channel_info->alert_tone_type)
        {
            s_is_etws_alert_msg = TRUE;
            s_is_etws_need_popup = TRUE;
        }

        // vibrate
        if(current_channel_info->vibrate_type)
        {
            // Display ETWS prompt
            MMIDEFAULT_TurnOnBackLight();

            if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
            {
                MMIAPISMSCB_StopETWSVibrateTimer();
                MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
            }

            if( MMICC_IsExistIncommingCall())
            {
                MMIAPICC_StopRingOrVibrate();
            }

            if(CB_PWS_MCC == plmn_info.mcc)
            {
                s_is_need_enable = TRUE;
                s_is_personal = TRUE;
                s_is_need_torch = FALSE;
            }
            else
            {
                s_is_need_enable = FALSE;
                s_is_personal = FALSE;
                s_is_need_torch = FALSE;
            }
            s_etws_card_sys = cb_msg_ptr->dual_sys;

            MMIAPISMSCB_SetEMCallStatus(FALSE);
            MMIAPISMSCB_SetIsNeedRedisplay(FALSE);

            if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
            {
                MMIAPISMSCB_StopETWSVibrateTimer();
                MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
            }
            s_cur_display_cb_msg_index = 0;

            MMISMS_ShowNewETWSPrompt(&etws_prompt_str, &etws_title_str, s_is_need_enable);
            MMIAPISET_SetSOSVibrate(s_etws_card_sys , s_is_personal, s_is_need_torch);

            // stop vibration and alert after timeout
            switch(current_channel_info->alert_period)
            {
            case 5:
                s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_5SECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                break;

            case 30:
                s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_30SECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                break;
	     case SMSCB_ALERT_VIBRATE_FOREVER:
		  break;
            case 10:
            default:
                s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_10500MSECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                break;
            }
        }
        else
        {

            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleMarcoCBMsgInd, normal ringtone and vibration");

            // Display ETWS prompt
            MMIDEFAULT_TurnOnBackLight();
            if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
            {
                MMIAPISMSCB_StopETWSVibrateTimer();
                MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
            }

   // stop recording
#ifdef MMI_RECORD_SUPPORT
            if( MMIAPIRECORD_IsRecording() )
            {
                 MMK_SendMsg(MMIRECORD_MAINPLAY_WIN_ID, MSG_APP_OK, PNULL);
            }
#endif
#ifdef CAMERA_SUPPORT
            if (MMK_IsOpenWin(MMIDC_MAIN_WIN_ID))
            {
                s_is_etws_display = TRUE;
            }
            MMIAPIDC_Interrupt();
#endif

            MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
            MMISMS_SetNewSMSRingFlag(TRUE);
            MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());

            MMIAPISMSCB_SetEMCallStatus(FALSE);
            MMIAPISMSCB_SetIsNeedRedisplay(FALSE);

            s_is_need_enable = TRUE;
            MMISMS_ShowNewETWSPrompt(&etws_prompt_str, &etws_title_str, s_is_need_enable);
        }

        if( PNULL != prompt_content_str.wstr_ptr)
        {
            SCI_FREE(prompt_content_str.wstr_ptr);
            prompt_content_str.wstr_ptr = PNULL;
        }

        if( PNULL != prompt_str.wstr_ptr)
        {
            SCI_FREE(prompt_str.wstr_ptr);
            prompt_str.wstr_ptr= PNULL;
        }

        return ;
    }
#endif

#ifdef MMI_ETWS_SUPPORT
    s_is_etws_alert_msg = ((cb_msg_ptr->msg_code) >> 9) & 0x01;
    s_is_etws_need_popup = ((cb_msg_ptr->msg_code) >> 8) & 0x01;
#endif

    if (((1 == cb_msg_ptr->dcs.class_is_present)
        && (MN_SMS_CLASS_0 == cb_msg_ptr->dcs.sms_class)
#ifdef MMI_ETWS_SUPPORT
        && (!s_is_etws_alert_msg)
#endif
        )
        || ((MN_CB_DISPLAY_IMMEDIATE == cb_msg_ptr->display_mode)
#ifdef MMI_ETWS_SUPPORT
        && (!s_is_etws_alert_msg)
#endif
        )
#ifdef MMI_ETWS_SUPPORT
        || (s_is_etws_need_popup && s_is_etws_alert_msg)
#endif
        )
    {
        //若收到的广播信息为 CLASS0 级且要立即显示则提取消息的有关信息
        //调用窗口立即显示有关内容
        //先关闭阅读窗口再打开保证了若此时在阅读窗口下也能刷新新的信息内容
        MMK_CloseWin(MMISMS_CB_CLASS0_READ_WIN_ID);
        //MMK_CreateWin((uint32*)MMISMS_CB_CLASS0_READ_WIN_TAB ,PNULL);

#ifdef MMI_ETWS_SUPPORT
        if(!s_is_etws_alert_msg)
        {
            MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
            MMISMS_SetNewSMSRingFlag(TRUE);
            MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());
        }
        else
        {
            MMIAPISET_PlayRingById(TRUE, 1, 1, MMISET_RING_TYPE_ALARM, PNULL);
            MMIAPISET_SetVibratorEx(TRUE, MMISET_RING_TYPE_MSG, TRUE, MMISMSCB_VIBRATE_INTERVAL);
        }
#else
        MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
        MMISMS_SetNewSMSRingFlag(TRUE);
        MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());
        if(s_is_alert_type_msg)
        {
            MMIAPISET_PlayRingById(TRUE, 1,1,MMISET_RING_TYPE_ALARM,PNULL);
            MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,TRUE,MMISMSCB_VIBRATE_INTERVAL);
        }
#endif

        MMISMSCB_OpenCLASS0Win();
        MMISMSCB_DirectDisplayNewMarcoCBMsg(cb_msg_ptr);
    }
    else
    {
        //若收到的广播信息为普通级且要保存到广播消息收件箱列表中去并显示提示窗口
        //添加新的广播消息内容到广播消息收件箱列表中去
        AddNewMarcoMsgToCBMsgList(cb_msg_ptr);

        //若在小区广播收件箱列表窗口中则要刷新列表
        if (MMK_IsOpenWin(MMISMS_CB_MSG_LIST_WIN_ID))
        {
            uint16 cur_index = 0;

            cur_index = GUILIST_GetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID);;

            //刷新当前小区广播收件箱列表
            GUILIST_RemoveAllItems(MMISMS_CB_MSG_LISTBOX_CTRL_ID);
            MMISMS_UpdateCBMsgList(MMISMS_CB_MSG_LISTBOX_CTRL_ID);

            GUILIST_SetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID, cur_index + 1);

            if (MMK_IsFocusWin(MMISMS_CB_MSG_LIST_WIN_ID))
            {
                MMK_SendMsg(MMISMS_CB_MSG_LIST_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
        }

#ifdef MMI_ETWS_SUPPORT
        if(!s_is_etws_alert_msg)
        {
            MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
            MMISMS_SetNewSMSRingFlag(TRUE);
        }
        else
        {
            MMIAPISET_PlayRingById(TRUE, 1, 1, MMISET_RING_TYPE_ALARM, PNULL);
            MMIAPISET_SetVibratorEx(TRUE, MMISET_RING_TYPE_MSG, TRUE, MMISMSCB_VIBRATE_INTERVAL);
        }
#else
        MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
        MMISMS_SetNewSMSRingFlag(TRUE);
#endif

        MMISMS_ShowNewSMSCBPrompt();
        MMIDEFAULT_TurnOnBackLight();
    }
}

#if defined (MODEM_SUPPORT_SPRD)
//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMIAPISMSCB_HandleLteWarningMsgInd
//  功能描述:        收到新小区广播的处理函数
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
void MMIAPISMSCB_HandleLteWarningMsgInd(
                             DPARAM        param            //IN: 系统回调参数
                             )
{
    APP_MN_LTE_WARNING_IND_T *cb_msg_ptr = (APP_MN_LTE_WARNING_IND_T *)param;
#ifdef MMI_ETWS_SUPPORT
    CMAS_CHANNEL_INFO_T* current_channel_info = NULL;
    MMI_STRING_T prompt_content_str = {0};
    MMI_STRING_T prompt_str = {0};
    MMISMS_CB_MESSAGE_ITEM_T* cb_msg_item = {0};
    uint32 msg_real_len = 0;
    uint8 ring_vol=0;
    uint16 etws_alert_num = 0;
    wchar time_str[CB_MSG_TIME_MAX_LEN + 1] = {0};
    uint16 time_len = 0;
    uint16 etws_context_len = 0;
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
    MMI_STRING_T etws_title = {0};
    uint16 etws_title_len = 0;
#endif

    if (PNULL == cb_msg_ptr)
    {
        SCI_TRACE_LOW("MMISMS:MMIAPISMSCB_HandleLteWarningMsgInd cb_msg_ptr = PNULL");
        return;
    }
    SCI_TRACE_LOW("SMSCB: MMIAPISMSCB_HandleLteWarningMsgInd dual_sys = %d", cb_msg_ptr->dual_sys);

    macro_lte_warning_receivedata_print(cb_msg_ptr);
#ifdef MMI_ETWS_SUPPORT
    etws_alert_num = MMIAPISMSCB_GetETWSNum();

    if( etws_alert_num > 0)
    {
        MN_CB_ALERT_MSG_T *cb_msg_id_ptr = PNULL;
        MMISMS_CB_CHANNEL_T etws_channel_list = {0};

        etws_channel_list = InitCB_ETWSChannel();
    }

    current_channel_info = GetCMASChannelInfo(cb_msg_ptr->dual_sys, cb_msg_ptr->msg_id);

    if( NULL != current_channel_info)
    {
        cb_msg_item = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
        if (PNULL == cb_msg_item)
        {
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleLteWarningMsgInd, allocate cb_msg_item buffer failed");
            return;
        }

        SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleLteWarningMsgInd, ETWS received");

    // stop recording
#ifdef MMI_RECORD_SUPPORT
        if( MMIAPIRECORD_IsRecording() )
        {
            MMK_SendMsg(MMIRECORD_MAINPLAY_WIN_ID, MSG_APP_OK, PNULL);
        }
#endif
#ifdef CAMERA_SUPPORT
    if (MMK_IsOpenWin(MMIDC_MAIN_WIN_ID))
    {
        s_is_etws_display = TRUE;
    }
    MMIAPIDC_Interrupt();
#endif

        // add cb to msg list
        AddNewLteWarningMsgToCBMsgList(cb_msg_ptr);

        // ignore secondary channel cb
        if( 1 == current_channel_info->primary_channel)
        {
            s_repeated_channel = current_channel_info->secondary_channel;
            s_repeated_period_time_id = MMK_CreateTimerCallback(MMI_5MINUTES,  MMISMSCB_HandleETWSRepeatedTimer, NULL, FALSE);
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleLteWarningMsgInd, current_channel_info->primary_channel = %d", current_channel_info->primary_channel);
        }
        else  if(( 1 == current_channel_info->secondary_channel )  &&  (s_repeated_channel == cb_msg_ptr->msg_id)  )
        {
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleLteWarningMsgInd, current_channel_info->secondary_channel = %d, cb_msg_ptr->msg_id = %d", current_channel_info->secondary_channel, cb_msg_ptr->msg_id);
            return;
        }
        else
        {
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleLteWarningMsgInd, neither primary channel nor secondary channel");
        }

        // init popup string buffer
        etws_context_len = MMI_CB_ETWS_TITLE_NAME_MAX_LEN + 1 \
            +CB_MSG_TIME_MAX_LEN + 1\
            + MN_CB_MACRO_MSG_LEN + 1;
        etws_title_len = MMI_CB_ETWS_TITLE_NAME_MAX_LEN + 1;

        prompt_content_str.wstr_ptr = SCI_ALLOCAZ(MN_CB_MACRO_MSG_LEN * sizeof(wchar));
        if (PNULL == prompt_content_str.wstr_ptr)
        {
            SCI_TRACE_LOW("MMISMSCB:alloc prompt_content_str.wstr_ptr failed");
            return;
        }
        SCI_MEMSET(prompt_content_str.wstr_ptr , 0, MN_CB_MACRO_MSG_LEN * sizeof(wchar));
        prompt_content_str.wstr_len = 0;

        prompt_str.wstr_ptr = SCI_ALLOCAZ(etws_context_len * sizeof(wchar));
        if (PNULL == prompt_str.wstr_ptr)
        {
            SCI_TRACE_LOW("MMISMSCB:alloc prompt_str.wstr_ptr failed");
            return;
        }
        SCI_MEMSET(prompt_str.wstr_ptr , 0, etws_context_len * sizeof(wchar));
        prompt_str.wstr_len = 0;

        if( PNULL != etws_prompt_str.wstr_ptr )
        {
            SCI_FREE(etws_prompt_str.wstr_ptr);
        }
        etws_prompt_str.wstr_ptr = SCI_ALLOCAZ(etws_context_len * sizeof(wchar));
        if (PNULL == etws_prompt_str.wstr_ptr)
        {
            SCI_TRACE_LOW("MMISMSCB:alloc etws_prompt_str.wstr_ptr failed");
            return;
        }
        SCI_MEMSET(etws_prompt_str.wstr_ptr , 0, etws_context_len * sizeof(wchar));
        etws_prompt_str.wstr_len = etws_context_len;

        if( PNULL != etws_title_str.wstr_ptr )
        {
            SCI_FREE(etws_title_str.wstr_ptr);
        }
        etws_title_str.wstr_ptr = SCI_ALLOCAZ(etws_title_len * sizeof(wchar));
        if (PNULL == etws_title_str.wstr_ptr)
        {
            SCI_TRACE_LOW("MMISMSCB:alloc etws_title_str.wstr_ptr failed");
            return;
        }
        SCI_MEMSET(etws_title_str.wstr_ptr , 0, etws_title_len* sizeof(wchar));
        etws_title_str.wstr_len = 0;

        // Title
        SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));
        plmn_info = MNPHONE_GetCurrentPLMNInfoEx(cb_msg_ptr->dual_sys);
        if( 0 != current_channel_info->string_id )
        {
            MMI_GetLabelTextByLang(current_channel_info->string_id , &etws_title);
            MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, etws_title.wstr_ptr, etws_title.wstr_len) ;
            MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, L"\n");
            prompt_str.wstr_len = etws_title.wstr_len+1;
        }
        else
        {
            MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, current_channel_info->title_id, MMIAPICOM_Wstrlen(current_channel_info->title_id)) ;
            MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, L"\n");
            prompt_str.wstr_len = MMIAPICOM_Wstrlen(current_channel_info->title_id) +1;
        }

        MMIAPICOM_Wstrncpy(etws_title_str.wstr_ptr, prompt_str.wstr_ptr, prompt_str.wstr_len );
        etws_title_str.wstr_len = prompt_str.wstr_len;

        SCI_MEMSET(prompt_str.wstr_ptr , 0, etws_context_len* sizeof(wchar));
        prompt_str.wstr_len = 0;

        //Content
        MMI_WSTRNCPY(prompt_str.wstr_ptr+prompt_str.wstr_len,
            MN_CB_MACRO_MSG_LEN,
            s_cb_msg.msg_ptr->context,
            s_cb_msg.msg_ptr->len,
            s_cb_msg.msg_ptr->len);

        prompt_str.wstr_len += s_cb_msg.msg_ptr->len;

        MMIAPICOM_Wstrncpy(etws_prompt_str.wstr_ptr, prompt_str.wstr_ptr, prompt_str.wstr_len );
        etws_prompt_str.wstr_len = prompt_str.wstr_len;

        if (MMK_IsOpenWin(MMISMS_CB_MSG_LIST_WIN_ID))
        {
            uint16 cur_index = 0;

            cur_index = GUILIST_GetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID);;

            //刷新当前小区广播收件箱列表
            GUILIST_RemoveAllItems(MMISMS_CB_MSG_LISTBOX_CTRL_ID);
            MMISMS_UpdateCBMsgList(MMISMS_CB_MSG_LISTBOX_CTRL_ID);

            if( (cur_index + 1 ) < MMI_MAX_CB_MSG_NUM )
                GUILIST_SetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID, cur_index + 1);
            else
                GUILIST_SetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID, cur_index );

            if (MMK_IsFocusWin(MMISMS_CB_MSG_LIST_WIN_ID))
            {
                MMK_SendMsg(MMISMS_CB_MSG_LIST_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
        }

        // alert
        if( current_channel_info->alert_level > 0)
            s_is_etws_alert_msg = TRUE;

        if(current_channel_info->alert_tone_type)
        {
            s_is_etws_alert_msg = TRUE;
            s_is_etws_need_popup = TRUE;
        }

        // vibrate
        if(current_channel_info->vibrate_type)
        {
            // Display ETWS prompt
            MMIDEFAULT_TurnOnBackLight();

            if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
            {
                MMIAPISMSCB_StopETWSVibrateTimer();
                MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
            }

            if( MMICC_IsExistIncommingCall())
            {
                MMIAPICC_StopRingOrVibrate();
            }

            if(CB_PWS_MCC == plmn_info.mcc)
            {
                s_is_need_enable = TRUE;
                s_is_personal = TRUE;
                s_is_need_torch = FALSE;
            }
            else
            {
                s_is_need_enable = FALSE;
                s_is_personal = FALSE;
                s_is_need_torch = FALSE;
            }
            s_etws_card_sys = cb_msg_ptr->dual_sys;

            MMIAPISMSCB_SetEMCallStatus(FALSE);
            MMIAPISMSCB_SetIsNeedRedisplay(FALSE);

            if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
            {
                MMIAPISMSCB_StopETWSVibrateTimer();
                MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
            }
            s_cur_display_cb_msg_index = 0;

            MMISMS_ShowNewETWSPrompt(&etws_prompt_str, &etws_title_str, s_is_need_enable);
            MMIAPISET_SetSOSVibrate(s_etws_card_sys , s_is_personal, s_is_need_torch);

            // stop vibration and alert after timeout
            switch(current_channel_info->alert_period)
            {
            case 5:
                s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_5SECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                break;

            case 30:
                s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_30SECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                break;

            case SMSCB_ALERT_VIBRATE_FOREVER:
		break;

            case 10:
            default:
                s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_10500MSECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                break;
            }
        }
        else
        {

            SCI_TRACE_LOW("[MMISMSCB] HandleLteWarningMsgInd, normal ringtone and vibration");

            // Display ETWS prompt
            MMIDEFAULT_TurnOnBackLight();
            if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
            {
                MMIAPISMSCB_StopETWSVibrateTimer();
                MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
            }

   // stop recording
#ifdef MMI_RECORD_SUPPORT
            if( MMIAPIRECORD_IsRecording() )
            {
                 MMK_SendMsg(MMIRECORD_MAINPLAY_WIN_ID, MSG_APP_OK, PNULL);
            }
#endif
#ifdef CAMERA_SUPPORT
            if (MMK_IsOpenWin(MMIDC_MAIN_WIN_ID))
            {
                s_is_etws_display = TRUE;
            }
            MMIAPIDC_Interrupt();
#endif

            MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
            MMISMS_SetNewSMSRingFlag(TRUE);
            MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());

            MMIAPISMSCB_SetEMCallStatus(FALSE);
            MMIAPISMSCB_SetIsNeedRedisplay(FALSE);

            s_is_need_enable = TRUE;
            MMISMS_ShowNewETWSPrompt(&etws_prompt_str, &etws_title_str, s_is_need_enable);
        }

        if( PNULL != prompt_content_str.wstr_ptr)
        {
            SCI_FREE(prompt_content_str.wstr_ptr);
            prompt_content_str.wstr_ptr = PNULL;
        }

        if( PNULL != prompt_str.wstr_ptr)
        {
            SCI_FREE(prompt_str.wstr_ptr);
            prompt_str.wstr_ptr= PNULL;
        }


    //SCI_FREE(temp_msg_ptr);
        SCI_FREE(cb_msg_ptr->msg_ptr);
            return ;
    }
#endif

#ifdef MMI_ETWS_SUPPORT
    //s_is_etws_alert_msg = ((cb_msg_ptr->msg_code) >> 9) & 0x01;
    //s_is_etws_need_popup = ((cb_msg_ptr->msg_code) >> 8) & 0x01;
#endif
    
    if (((1 == cb_msg_ptr->dcs.class_is_present)
        && (MN_SMS_CLASS_0 == cb_msg_ptr->dcs.sms_class)
#ifdef MMI_ETWS_SUPPORT
        && (!s_is_etws_alert_msg)
#endif
        )
        || ((MN_CB_DISPLAY_IMMEDIATE == cb_msg_ptr->display_mode)
#ifdef MMI_ETWS_SUPPORT
        && (!s_is_etws_alert_msg)
#endif
        )
#ifdef MMI_ETWS_SUPPORT
        || (s_is_etws_need_popup && s_is_etws_alert_msg)
#endif
        )
    {
        //若收到的广播信息为 CLASS0 级且要立即显示则提取消息的有关信息
        //调用窗口立即显示有关内容
        //先关闭阅读窗口再打开保证了若此时在阅读窗口下也能刷新新的信息内容
        MMK_CloseWin(MMISMS_CB_CLASS0_READ_WIN_ID);
        //MMK_CreateWin((uint32*)MMISMS_CB_CLASS0_READ_WIN_TAB ,PNULL);

        SCI_TRACE_LOW("[MMISMSCB] HandleLteWarningMsgInd, CLASS0 message received");
#ifdef MMI_ETWS_SUPPORT
        if(!s_is_etws_alert_msg)
        {
            MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
            MMISMS_SetNewSMSRingFlag(TRUE);
            MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());
        }
        else
        {
            MMIAPISET_PlayRingById(TRUE, 1, 1, MMISET_RING_TYPE_ALARM, PNULL);
            MMIAPISET_SetVibratorEx(TRUE, MMISET_RING_TYPE_MSG, TRUE, MMISMSCB_VIBRATE_INTERVAL);
        }
#else
        MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
        MMISMS_SetNewSMSRingFlag(TRUE);
        MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());
        if(s_is_alert_type_msg)
        {
            MMIAPISET_PlayRingById(TRUE, 1,1,MMISET_RING_TYPE_ALARM,PNULL);
            MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,TRUE,MMISMSCB_VIBRATE_INTERVAL);
	 }
#endif

        MMISMSCB_OpenCLASS0Win();
        MMISMSCB_DirectDisplayNewLteWarningMsg(cb_msg_ptr);
    }
    else
    {
        SCI_TRACE_LOW("[MMISMSCB] HandleLteWarningMsgInd, normal message received");
        //若收到的广播信息为普通级且要保存到广播消息收件箱列表中去并显示提示窗口
        //添加新的广播消息内容到广播消息收件箱列表中去
        AddNewLteWarningMsgToCBMsgList(cb_msg_ptr);
        
        //若在小区广播收件箱列表窗口中则要刷新列表
        if (MMK_IsOpenWin(MMISMS_CB_MSG_LIST_WIN_ID))
        {
            uint16 cur_index = 0;

            cur_index = GUILIST_GetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID);;

            //刷新当前小区广播收件箱列表
            GUILIST_RemoveAllItems(MMISMS_CB_MSG_LISTBOX_CTRL_ID);
            MMISMS_UpdateCBMsgList(MMISMS_CB_MSG_LISTBOX_CTRL_ID);

            GUILIST_SetCurItemIndex(MMISMS_CB_MSG_LISTBOX_CTRL_ID, cur_index + 1);

            if (MMK_IsFocusWin(MMISMS_CB_MSG_LIST_WIN_ID))
            {
                MMK_SendMsg(MMISMS_CB_MSG_LIST_WIN_ID, MSG_FULL_PAINT, PNULL);
            }
        }

#ifdef MMI_ETWS_SUPPORT
    if(!s_is_etws_alert_msg)
    {
        MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
        MMISMS_SetNewSMSRingFlag(TRUE);
    }
    else
    {
        MMIAPISET_PlayRingById(TRUE, 1, 1, MMISET_RING_TYPE_ALARM, PNULL);
        MMIAPISET_SetVibratorEx(TRUE, MMISET_RING_TYPE_MSG, TRUE, MMISMSCB_VIBRATE_INTERVAL);
    }
#else
        MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
        MMISMS_SetNewSMSRingFlag(TRUE);
#endif

        MMISMS_ShowNewSMSCBPrompt();
        MMIDEFAULT_TurnOnBackLight();
    }
    //SCI_FREE(temp_msg_ptr);
    SCI_FREE(cb_msg_ptr->msg_ptr);
}

//----------------------------------------------------------------------------------------------------------------
//
//  函数名：         MMIAPISMSCB_HandleCBMsgInd
//  功能描述:        收到新小区广播的处理函数
//  全局变量使用:    s_cb_msg
//  返回值:          无
//  作者:            肖凡
//
//----------------------------------------------------------------------------------------------------------------
void MMIAPISMSCB_HandleETWSPrimMsgInd(
                             DPARAM        param            //IN: 系统回调参数

                             )
{
    APP_MN_ETWS_PRIM_IND_T *cb_msg_ptr = (APP_MN_ETWS_PRIM_IND_T *)param;
#ifdef MMI_ETWS_SUPPORT
    CMAS_CHANNEL_INFO_T* current_channel_info = NULL;
    MMI_STRING_T prompt_content_str = {0};
    MMI_STRING_T prompt_str = {0};
    MMISMS_CB_MESSAGE_ITEM_T* cb_msg_item = {0};
    uint32 msg_real_len = 0;
    uint8 ring_vol=0;
    uint16 etws_alert_num = 0;
    wchar time_str[CB_MSG_TIME_MAX_LEN + 1] = {0};
    uint16 time_len = 0;
    uint16 etws_context_len = 0;
    MN_PHONE_CURRENT_PLMN_INFO_T  plmn_info = {0};
    MMI_STRING_T etws_title = {0};
    uint16 etws_title_len = 0;
#endif
    
    if (PNULL == cb_msg_ptr)
    {
        SCI_TRACE_LOW("MMISMS:MMIAPISMSCB_HandleETWSPrimMsgInd cb_msg_ptr = PNULL");
	 s_is_etws_prim_msg = FALSE;
        return;
    }    

    s_is_etws_prim_msg = TRUE;

    smscb_prim_receivedata_print(cb_msg_ptr);

#ifdef MMI_ETWS_SUPPORT
    etws_alert_num = MMIAPISMSCB_GetETWSNum();

    if( etws_alert_num > 0)
    {
        MN_DUAL_SYS_E dual_sys = MMISMSCB_GetSelectCard();
        MN_CB_ALERT_MSG_T *cb_msg_id_ptr = PNULL;
        MMISMS_CB_CHANNEL_T etws_channel_list = {0};

        etws_channel_list = InitCB_ETWSChannel();
    }

    current_channel_info = GetCMASChannelInfo(cb_msg_ptr->dual_sys, cb_msg_ptr->msg_id);

    if( NULL != current_channel_info)
    {
        cb_msg_item = (MMISMS_CB_MESSAGE_ITEM_T *)SCI_ALLOCAZ(sizeof(MMISMS_CB_MESSAGE_ITEM_T));
        if (PNULL == cb_msg_item)
        {
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleETWSPrimMsgInd, allocate cb_msg_item buffer failed");
            return;
        }

        SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleETWSPrimMsgInd, ETWS received");
        // add cb to msg list
        //AddNewMsgToCBMsgList(cb_msg_ptr, FALSE);

        // ignore secondary channel cb
        if( 1 == current_channel_info->primary_channel)
        {
            s_repeated_channel = current_channel_info->secondary_channel;
            s_repeated_period_time_id = MMK_CreateTimerCallback(MMI_5MINUTES,  MMISMSCB_HandleETWSRepeatedTimer, NULL, FALSE);
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleETWSPrimMsgInd, current_channel_info->primary_channel = %d", current_channel_info->primary_channel);
        }
        else  if(( 1 == current_channel_info->secondary_channel )  &&  (s_repeated_channel == cb_msg_ptr->msg_id)  )
        {
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleETWSPrimMsgInd, current_channel_info->secondary_channel = %d, cb_msg_ptr->msg_id = %d", current_channel_info->secondary_channel, cb_msg_ptr->msg_id);
            return;
        }
        else
        {
            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleETWSPrimMsgInd, neither primary channel nor secondary channel");
        }

        // init popup string buffer
        etws_context_len = MMI_CB_ETWS_TITLE_NAME_MAX_LEN + 1;
        etws_title_len = MMI_CB_ETWS_TITLE_NAME_MAX_LEN + 1;

        prompt_content_str.wstr_ptr = SCI_ALLOCAZ(MN_CB_MSG_LEN * sizeof(wchar));
        if (PNULL == prompt_content_str.wstr_ptr)
        {
            SCI_TRACE_LOW("MMISMSCB:alloc prompt_content_str.wstr_ptr failed");
            return;
        }
        SCI_MEMSET(prompt_content_str.wstr_ptr , 0, MN_CB_MSG_LEN* sizeof(wchar));
        prompt_content_str.wstr_len = 0;

        prompt_str.wstr_ptr = SCI_ALLOCAZ(etws_context_len * sizeof(wchar));
        if (PNULL == prompt_str.wstr_ptr)
        {
            SCI_TRACE_LOW("MMISMSCB:alloc prompt_str.wstr_ptr failed");
            return;
        }
        SCI_MEMSET(prompt_str.wstr_ptr , 0, etws_context_len* sizeof(wchar));
        prompt_str.wstr_len = 0;

        if( PNULL != etws_prompt_str.wstr_ptr )
        {
            SCI_FREE(etws_prompt_str.wstr_ptr);
        }
        etws_prompt_str.wstr_ptr = SCI_ALLOCAZ(etws_context_len * sizeof(wchar));
        if (PNULL == etws_prompt_str.wstr_ptr)
        {
            SCI_TRACE_LOW("MMISMSCB:alloc etws_prompt_str.wstr_ptr failed");
            return;
        }
        SCI_MEMSET(etws_prompt_str.wstr_ptr , 0, etws_context_len* sizeof(wchar));
        etws_prompt_str.wstr_len = etws_context_len;

        // Title
        if( PNULL != etws_title_str.wstr_ptr )
        {
            SCI_FREE(etws_title_str.wstr_ptr);
        }
        etws_title_str.wstr_ptr = SCI_ALLOCAZ(etws_title_len * sizeof(wchar));
        if (PNULL == etws_title_str.wstr_ptr)
        {
            SCI_TRACE_LOW("MMISMSCB:alloc etws_title_str.wstr_ptr failed");
            return;
        }
        SCI_MEMSET(etws_title_str.wstr_ptr , 0, etws_title_len* sizeof(wchar));
        etws_title_str.wstr_len = 0;

        SCI_MEMSET(&plmn_info, 0x00, sizeof(plmn_info));
        plmn_info = MNPHONE_GetCurrentPLMNInfoEx(cb_msg_ptr->dual_sys);

        if( 0 != current_channel_info->string_id )
        {
             MMI_GetLabelTextByLang(current_channel_info->string_id , &etws_title);
             MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, etws_title.wstr_ptr, etws_title.wstr_len) ;
             MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, L"\n");
             prompt_str.wstr_len = etws_title.wstr_len+1;
        }
        else
        {
             MMIAPICOM_Wstrncpy(prompt_str.wstr_ptr, current_channel_info->title_id, MMIAPICOM_Wstrlen(current_channel_info->title_id)) ;
             MMIAPICOM_Wstrcat(prompt_str.wstr_ptr, L"\n");
             prompt_str.wstr_len = MMIAPICOM_Wstrlen(current_channel_info->title_id) +1;
        }

        MMIAPICOM_Wstrncpy(etws_title_str.wstr_ptr, prompt_str.wstr_ptr, prompt_str.wstr_len );
        etws_title_str.wstr_len = prompt_str.wstr_len;

        SCI_MEMSET(prompt_str.wstr_ptr , 0, etws_context_len* sizeof(wchar));
        prompt_str.wstr_len = 0;

        // alert
        if( current_channel_info->alert_level > 0)
            s_is_etws_alert_msg = TRUE;

        if(current_channel_info->alert_tone_type)
        {
            s_is_etws_alert_msg = TRUE;
            s_is_etws_need_popup = TRUE;
        }

        SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleETWSPrimMsgInd, vibrate_type=%d,mcc=%d ",current_channel_info->vibrate_type,plmn_info.mcc);

        s_etws_vibrate_type_context = current_channel_info->vibrate_type;
        // vibrate
        if(current_channel_info->vibrate_type)
        {
            // Display ETWS prompt
            MMIDEFAULT_TurnOnBackLight();

            s_etws_delay_show_dual_sim_context = cb_msg_ptr->dual_sys;

            if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
            {
                MMIAPISMSCB_StopETWSVibrateTimer();
                MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
            }

            if( MMICC_IsExistIncommingCall())
            {
                MMIAPICC_StopRingOrVibrate();
            }

            s_etws_card_sys = cb_msg_ptr->dual_sys;

            MMIAPISMSCB_SetEMCallStatus(FALSE);
            MMIAPISMSCB_SetIsNeedRedisplay(FALSE);

            if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
            {
                MMIAPISMSCB_StopETWSVibrateTimer();
                MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
            }

    // stop recording
#ifdef MMI_RECORD_SUPPORT
            if( MMIAPIRECORD_IsRecording() )
            {
                 MMK_SendMsg(MMIRECORD_MAINPLAY_WIN_ID, MSG_APP_OK, PNULL);
            }
#endif
#ifdef CAMERA_SUPPORT
            if (MMK_IsOpenWin(MMIDC_MAIN_WIN_ID))
            {
                 s_is_etws_display = TRUE;
            }
            MMIAPIDC_Interrupt();
#endif

            s_cur_display_cb_msg_index = 0;

            MMISMS_ShowETWSPrimaryPrompt(&etws_prompt_str, &etws_title_str, s_is_need_enable);
            MMIAPISET_SetSOSVibrate(s_etws_card_sys , s_is_personal, s_is_need_torch);

            // stop vibration and alert after timeout
            switch(current_channel_info->alert_period)
            {
                case 5:
                    s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_5SECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                break;

                case 30:
                    s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_30SECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                break;
			case SMSCB_ALERT_VIBRATE_FOREVER:
				break;
                case 10:
                default:
                    s_vibrator_period_time_id = MMK_CreateTimerCallback(MMI_10500MSECONDS,  MMIAPISMSCB_HandleETWSVibrateTimer, current_channel_info->is_continued, FALSE);
                break;
            }
        }
        else
        {

            SCI_TRACE_LOW("[MMISMSCB] MMIAPISMSCB_HandleETWSPrimMsgInd normal ringtone and vibration");

            // Display ETWS prompt
            MMIDEFAULT_TurnOnBackLight();
            if(MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
            {
                 MMIAPISMSCB_StopETWSVibrateTimer();
                 MMK_CloseWin(MMISMS_ETWS_NEWMSG_WIN_ID);
            }

            MMISMS_SetSimIdOfNewMsg(cb_msg_ptr->dual_sys);
            MMISMS_SetNewSMSRingFlag(TRUE);
            MMISMS_PlayNewSMSRing(MMISMS_GetSimIdOfNewMsg());

            s_is_need_enable = TRUE;
            MMISMS_ShowETWSPrimaryPrompt(&etws_prompt_str, &etws_title_str, s_is_need_enable);
        }


    if( PNULL != prompt_content_str.wstr_ptr)
    {
        SCI_FREE(prompt_content_str.wstr_ptr);
        prompt_content_str.wstr_ptr = PNULL;
    }

    if( PNULL != prompt_str.wstr_ptr)
    {
        SCI_FREE(prompt_str.wstr_ptr);
        prompt_str.wstr_ptr= PNULL;
    }

        return ;
    }
#endif

}
#endif

/*****************************************************************************/
//  Description : set setting card
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMISMSCB_SetSelectCard(MN_DUAL_SYS_E dual_sys)
{
    if (dual_sys < MMI_DUAL_SYS_MAX)
    {
        s_dual_card_sys = dual_sys;
    }
    else
    {
        s_dual_card_sys = MN_DUAL_SYS_1;
    }
}

/*****************************************************************************/
//  Description : get setting card
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC MN_DUAL_SYS_E  MMISMSCB_GetSelectCard( void )
{
    return s_dual_card_sys;
}

/***************************************************************************/
//     Description : 初始化小区广播接口函数(在手机初始化时调用)
//    Global resource dependence : none
//  Author:肖凡
//    Note:
/***************************************************************************/
PUBLIC MN_RETURN_RESULT_E MMIAPISMSCB_Init(MN_DUAL_SYS_E dual_sys)
{
    return MMISMSCB_Init(dual_sys);
}

/*****************************************************************************/
//     Description : open SMSCB main menu window
//    Global resource dependence :
//  Author: Tracy Zhang
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_OpenMainMenu(void)
{
    MMISMSCB_OpenMainMenu();
}

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description : open SMSCB main menu window      
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_RestoreFactorySetting(void)
{
    MMISMS_CB_CONFIG_T default_cb_config = FALSE;
    MN_DUAL_SYS_E dual_sys = MN_DUAL_SYS_1;

    default_cb_config = MMISMSCB_GetCBDefaultConfig();

    for(dual_sys = MN_DUAL_SYS_1; dual_sys < MN_DUAL_SYS_MAX; dual_sys++)
    {
        //save important channel
        MMISMSCB_SaveCBEmergenceChannel(dual_sys);
        SCI_TRACE_LOW("MMIAPISMSCB_RestoreFactorySetting --> dual_sys = %d, default_cb_config = %d.", dual_sys, default_cb_config);
        MMISMSCB_SwitchSmsCBStatus(default_cb_config);
    }
}

#else /* MMI_ETWS_SUPPORT */
/*****************************************************************************/
//     Description : open SMSCB main menu window      
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_RestoreFactorySetting(void)
{
    MMISMSCB_SwitchSmsCBStatus(FALSE); //恢复出厂设置，关闭小区广播。    
}
#endif /* MMI_ETWS_SUPPORT */

/*****************************************************************************/
//  Description : open SMSCB setting window      
//  Global resource dependence : 
//  Author: fengming.huang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_OpenSettingWin(void)
{
    MMISMSCB_OpenSettingWin();
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_OpenCBList
//    Global resource dependence : 
//  Author: Tracy Zhang
//    Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_OpenCBList(void)
{
    MMISMSCB_OpenCBList();

    return TRUE;
}

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description : MMIAPISMSCB_GetETWSAlertFlag
//    Global resource dependence :
//  Author: Yongli Tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetETWSAlertFlag(void)
{
    return s_is_etws_alert_msg;
}
#endif

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
//  Description : Set SMS CB Status
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SwitchSmsCBStatus(BOOLEAN cb_enabled)
{
    MMISMSCB_SwitchSmsCBStatus(cb_enabled);
}
#endif

#ifdef MMI_ETWS_SUPPORT
/*****************************************************************************/
//     Description : to show the new SMSCB prompt
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_ShowNewETWSPrompt(void)
{
    // prompt
    if(  TRUE == s_is_need_redisplay )
    {
        MMIAPISMSCB_SetEMCallStatus(FALSE);
        MMIAPISMSCB_SetIsNeedRedisplay(FALSE);
        if( FALSE == MMK_IsOpenWin( MMISMS_ETWS_NEWMSG_WIN_ID))
        {
            MMISMS_ShowNewETWSPrompt(&etws_prompt_str, &etws_title_str, s_is_need_enable);
        }
        // ring and vibrate
        if( (TRUE == MMIAPISMSCB_IsCMASChannelAlert() )
            && ( FALSE == MMIAPISET_IsSOSVibrate()) )
        {
            MMIAPISET_SetSOSVibrate(s_etws_card_sys , s_is_personal, s_is_need_torch);
        }
    }
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_SetEMCallStatus
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetEMCallStatus(BOOLEAN is_emer_call)
{
    s_is_emer_call = is_emer_call;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetEMCallStatus
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetEMCallStatus(void)
{
    return s_is_emer_call;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetIsNeedEnable
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetIsNeedEnable(void)
{
    return s_is_need_enable;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetIsNeedEnable
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetIsNeedEnable(BOOLEAN is_need_enable)
{
     s_is_need_enable = is_need_enable;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetIsNeedRedisplay
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetIsNeedRedisplay(void)
{
    return s_is_need_redisplay;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetIsNeedEnable
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetIsNeedRedisplay(BOOLEAN is_need_redisplay)
{
     s_is_need_redisplay = is_need_redisplay;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetIsETWSDisplay
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISMSCB_GetIsETWSDisplay(void)
{
   SCI_TRACE_LOW("MMIAPISMSCB_GetIsETWSDisplay, s_is_etws_display = %d", s_is_etws_display);
    return s_is_etws_display;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_SetIsETWSDisplay
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetIsETWSDisplay(BOOLEAN is_etws_display)
{
     s_is_etws_display = is_etws_display;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetCurDispCBIndex
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMSCB_GetCurDispCBIndex(void)
{
   SCI_TRACE_LOW("MMIAPISMSCB_GetCurDispIndex, s_cur_display_cb_msg_index = %d", s_cur_display_cb_msg_index);
    return s_cur_display_cb_msg_index;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_SetCurDispCBIndex
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC void MMIAPISMSCB_SetCurDispCBIndex(uint16 index)
{
     s_cur_display_cb_msg_index = index;
}

/*****************************************************************************/
//     Description : MMIAPISMSCB_GetCurDispCBIndex
//    Global resource dependence :
//  Author:
//    Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPISMSCB_GetSimIndex(void)
{
    SCI_TRACE_LOW("MMIAPISMSCB_GetSimIndex, s_etws_card_sys = %d", s_etws_card_sys);
    return s_etws_card_sys;
}

#endif

#endif//#if defined(MMI_SMSCB_SUPPORT)

