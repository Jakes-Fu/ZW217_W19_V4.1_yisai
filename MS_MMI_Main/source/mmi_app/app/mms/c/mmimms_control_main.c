/****************************************************************************
** File Name:      mmimms_control_main.c                                   *
** Author:         aoke.hu                                                 *
** Date:           04/13/2009                                              *
** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 04/2009        aoke.hu          create
** 
****************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "mmi_app_mms_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#include "mmisd_export.h"
#include "mmimms_control_main.h"
#include "mmimms_file.h"
#include "mmimms_setting.h"
#include "dal_time.h"
#include "ffs.h"
#include "mmimms_setting.h"
#include "gui_ucs2b_converter.h"
//#include "freq_cfg.h"
#ifdef CSC_XML_SUPPORT
#include "mmimms_export.h"
#endif
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
#include "mmiplmn_export.h"
#endif
/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT    							  *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL MMSCONTROL_CALLBACK s_control_callback = PNULL;
#ifndef WIN32
LOCAL uint32 s_mmimms_chng_freq_req_handler = CHNG_FREQ_REQ_HANDLER_NULL;
#endif
#ifdef CSC_XML_SUPPORT
LOCAL uint32 s_mmimms_mms_limit = 0;
#endif

MMI_APPLICATION_T g_mmimms_app = {0};
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : 处理MMS LIB的HTTP消息
//	Global resource dependence : 
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpMsg(
                                 PWND                app_ptr, 
                                 uint16              msg_id, 
                                 DPARAM              param
                                 );
                                 
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description: get the prefer disk
//	Global resource dependence:
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIMMS_GetPreferDisk(void)
{
#ifdef CMCC_UI_SYTLE
    return MMIMMS_Setting_GetPreferDisk();
#else
    return MMIAPIFMM_GetValidateDevice(MMI_DEVICE_SDCARD);
#endif
}

/*****************************************************************************/
// 	Description: Check space is enough or not
//	Global resource dependence:
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_IsEnoughSpace(uint32 size)
{
    return MMIAPIFMM_IsEnoughSpace(MMIFMM_STORE_TYPE_PREFER, MMIMMS_GetPreferDisk(), size, PNULL);
}

/*****************************************************************************/
//  Description : 获取彩信已占用的总大小total_size_ptr和能使用的空间大小all_space_ptr
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMMS_GetTotalSizeAndAllSize(uint32 *total_size_ptr, 
                                            uint32 *all_space_ptr)
{
    uint32 free_high_word = 0;
    uint32 free_low_word = 0;
    //uint32 free_high_word_sd = 0;
    //uint32 free_low_word_sd = 0;
    //wchar*	device_name_ptr = PNULL;
    //uint16	device_name_len = 0;
    uint32  all_size = 0;
	uint32  all_high_size = 0;
    uint32 max_size = 0;
    MMIFILE_DEVICE_E dev  = MMI_DEVICE_UDISK;
    uint32           free_space = 0;
    
    if(PNULL == total_size_ptr || PNULL == all_space_ptr)
    {
        return FALSE;
    }
    *total_size_ptr = MMIMMS_GetTotalSize();

    max_size = sizeof(MMIMMS_LIST_FILE_HEAD_T)+MMIMMS_MAX_MMS_NUM*sizeof(MMISMS_ORDER_INFO_T);
    max_size += MMIMMS_MAX_MMS_NUM * MMIMMS_MMS_LIMIT_SIZE;

    //calculate the free space
    do
    {
        if(MMIAPIFMM_GetDeviceTypeStatus(dev))
        {
            if(MMIAPIFMM_GetDeviceFreeSpace(
                                        MMIAPIFMM_GetDevicePath(dev), 
                                        MMIAPIFMM_GetDevicePathLen(dev), 
                                        &free_high_word,
                                        &free_low_word))
            {
                free_space += free_low_word;//用低位
				all_high_size += free_high_word;
            }
        }
    }while(++dev < MMI_DEVICE_NUM);
    
    all_size = free_space + *total_size_ptr;
	    
    if(0 ==all_high_size && all_size < max_size)
    {
        //SCI_TRACE_LOW:"[MMIMMS]:MMIMMS_GetTotalSizeAndAllSize all_size=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_CONTROL_MAIN_136_112_2_18_2_37_31_0,(uint8*)"d",all_size);
        *all_space_ptr = all_size;
    }
    else
    {
        *all_space_ptr = max_size;
    }

    return TRUE;
}

/*****************************************************************************/
// 	Description: get prefer device with enough space
//	Global resource dependence:
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC MMIFILE_DEVICE_E MMIMMS_GetDeviceForSave(uint32 size)
{
    MMIFILE_DEVICE_E device    = MMIMMS_Setting_GetPreferDisk();
    
    if(SFS_ERROR_NONE != MMIAPIFMM_GetSuitableFileDev(device, size, &device))
    {
        device = MMI_DEVICE_NUM;
    }
    
    return device;
}

/*****************************************************************************/
// 	Description: 返回 1970年1月1日零点以来到指定时间经过的秒数
//	Global resource dependence:
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_Tm2SecondFrom1970( 
                      uint32 tm_sec, 
                      uint32 tm_min, 
                      uint32 tm_hour,
                      uint32 tm_mday,
                      uint32 tm_mon, 
                      uint32 tm_year)
{
    uint32 second_common = 0;

    second_common = MMIAPICOM_Tm2Second(tm_sec, tm_min, tm_hour,
                      tm_mday, tm_mon, tm_year);
    
    /* 取到的秒数为1980年以来的秒数，还需要加上70~80的秒数 */
    return (uint32)(second_common + MMIMMS_SECOND_1970_TO_1980);
}

/*****************************************************************************/
// 	Description: get the seconds from appointed time to 1970年1月1日零点
//	Global resource dependence:
//  Author: kelly.li
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMSAdapter_Tm2SecondFrom1970( 
                      uint32 tm_sec,    //[IN]
                      uint32 tm_min,    //[IN]
                      uint32 tm_hour,   //[IN]
                      uint32 tm_mday,   //[IN]
                      uint32 tm_mon,    //[IN]
                      uint32 tm_year    //[IN]
                      )
{
    return (MMIMMS_Tm2SecondFrom1970(tm_sec,tm_min,tm_hour,tm_mday,tm_mon,tm_year));
}

/*****************************************************************************/
// 	Description : change gb to wstr
//	Global resource dependence : none
//  Author: kelly.li
//  Return: the number of UCS2 code that have been converted.
//	Note: 
/*****************************************************************************/
PUBLIC uint32 MMSAdapter_GBToWstr(					
                                   uint16 *wstr_ptr, 	//destination: string pointer to UCS2 string.
                                   const uint8 *gb_ptr, //source: string pointer to GB string
                                   uint32 len			//the length to convert from GB to UCS2.
                                   )
{
    return (GUI_GBToWstr((wchar*)wstr_ptr,gb_ptr,len));
}

/*****************************************************************************/
// 	Description : other code to wstr
//	Global resource dependence : 
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMSAdapter_OtherCodeToWstr(//ucs2b len
                                  uint16 *wstr_ptr,//out
                                  uint32 wstr_len,//in
                                  MMS_TEXT_ENCODE_TYPE_E code_type,//in
                                  const uint8 *code_ptr,//in
                                  uint32 code_len//in
                                  )
{
    return (GUI_OtherCodeToWstr((wchar*)wstr_ptr, wstr_len, code_type, code_ptr, code_len));
}

/*****************************************************************************/
// 	Description : judge the type of text file
//	Global resource dependence : none
//  Author: MINGHU.MAO
//  Return: judge the type of text by content
//	Note: 
/*****************************************************************************/
PUBLIC MMS_TEXT_ENCODE_TYPE_E MMSAdapter_GetTextCharType(					
                                                         uint8 *code_ptr, //source: content
                                                         uint32 code_len			//the length to judge
                                                         )
{
    return (MMS_TEXT_ENCODE_TYPE_E)GUI_GetCodeType(code_ptr,code_len);
}

/*****************************************************************************/
// 	Description: 返回 1970年1月1日零点以来经过的秒数
//	Global resource dependence:
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetCurrentTime( void )
{
    SCI_DATE_T	sys_date = {0};
    SCI_TIME_T	sys_time = {0};
    uint32      second = 0;
    
    
    TM_GetSysDate(&sys_date);
    TM_GetSysTime(&sys_time);

    /* modified by plum peng for cr116442 */
    second = MMIMMS_Tm2SecondFrom1970( sys_time.sec, 
        sys_time.min, 
        sys_time.hour,
        sys_date.mday,
        sys_date.mon, 
        sys_date.year);
    
    /*1970年1月1日零点到现在经过的秒数*/ 
    return ( second);
}

/*****************************************************************************/
// 	Description: 生成随机文件名
//	Global resource dependence:
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIMMS_GenRandomName(uint8 *name_ptr,uint16 name_maxlen)
{
    uint8       temp_buf[MMIMMS_FILE_NAME_LEN]={0};
    uint16      random = 0;
    uint16      len=0;   
    
    srand(SCI_GetTickCount());
    random = (uint16)rand();
    _snprintf((char *)temp_buf,(MMIMMS_FILE_NAME_LEN-1),"%ld",random);
    len=strlen((char *)temp_buf);
    if(len>name_maxlen)
    {
        len=name_maxlen;
    }
    SCI_MEMCPY(name_ptr,temp_buf,len);

    return len;
}


/*****************************************************************************/
//  Description : MMS control init
//  Global resource dependence : none
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_ControlInit(MMSCONTROL_CALLBACK callback)
{
    s_control_callback = callback;
    g_mmimms_app.ProcessMsg = HandleHttpMsg;

#ifndef WIN32
    if (CHNG_FREQ_REQ_HANDLER_NULL == s_mmimms_chng_freq_req_handler)
    {
        s_mmimms_chng_freq_req_handler = CHNG_FREQ_GetReqHandler("mmimms");
        if (PNULL == s_mmimms_chng_freq_req_handler)
        {
            //SCI_TRACE_LOW:"MMIMMS_ControlInit CHNG_FREQ_REQ_HANDLER_NULL == s_mmimms_chng_freq_req_handler"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_CONTROL_MAIN_292_112_2_18_2_37_31_1,(uint8*)"");
        }        
    }   
#endif
}

/*****************************************************************************/
// 	Description: send msg from control to ui
//	Global resource dependence:
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
PUBLIC void MMIMMS_ControlToUIMsg(
                                  uint16   	    msg_id,
                                  DPARAM        param
                                  )
{
    if(s_control_callback)
    {
        s_control_callback(msg_id,param);
    }
}

/*****************************************************************************/
// 	Description : 处理MMS LIB的HTTP消息
//	Global resource dependence : 
//  Author: aoke.hu
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleHttpMsg(
                                 PWND                app_ptr, 
                                 uint16              msg_id, 
                                 DPARAM              param
                                 )
{
    void* original_param=(void*)((uint32)param);

    if( MMS_HandleNetMsg(original_param))
    {
        return MMI_RESULT_TRUE;
    }
    else
    {
        return MMI_RESULT_FALSE;
    }
}

/*****************************************************************************/
//  Description : Set systerm frequency for mmi mms
//  Global resource dependence :                     
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIMMS_SetFreq(void)
{
#ifndef WIN32
    if (CHNG_FREQ_REQ_HANDLER_NULL != s_mmimms_chng_freq_req_handler)
    {
        CHNG_FREQ_SetArmClk(s_mmimms_chng_freq_req_handler, (CHNG_FREQ_INDEX_E)FREQ_INDEX_WWW);       
    }
#endif
    //SCI_TRACE_LOW:"\"\"  MMIMMS_SetFreq   \"#"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_CONTROL_MAIN_347_112_2_18_2_37_31_2,(uint8*)"");
    
    return 1;
}
 
/*****************************************************************************/
//  Description : Get systerm frequency for mmi mms
//  Global resource dependence :                     
//  Author: wancan.you
//  Note: 
/*****************************************************************************/ 
PUBLIC uint32 MMIMMS_RestoreFreq(void)
{
#ifndef WIN32
     if (CHNG_FREQ_REQ_HANDLER_NULL != s_mmimms_chng_freq_req_handler)
    {
        CHNG_FREQ_RestoreARMClk(s_mmimms_chng_freq_req_handler);
    }       
#endif
    //SCI_TRACE_LOW:"\"\"  MMIMMS_SetFreq   \"#"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMMS_CONTROL_MAIN_364_112_2_18_2_37_31_3,(uint8*)"");

    return 1;
}

#ifdef CSC_XML_SUPPORT
/*****************************************************************************/
//  Description : Set MMS Limit Size
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC void MMIMMS_SetMMSLimitSize(uint32 mms_limit)
{
    s_mmimms_mms_limit = mms_limit;
}
#endif

/*****************************************************************************/
//  Description : Get MMS Limit Size
//  Global resource dependence : none
//  Author: wancan.you
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIMMS_GetMMSLimitSize(void)
{
    uint32 mms_limit_size = NULL;
#ifdef MMI_PLMN_LOAD_DEFAULT_SUPPORT
    uint32 i =0;

    for (i = 0; i < MMI_DUAL_SYS_MAX; i++)
    {
        mms_limit_size = MMIAPIPLMN_GetLocalMMSLimit(i);

        if (NULL != mms_limit_size)
        {
            break;
        }
    }

    if (NULL == mms_limit_size || mms_limit_size > MMIMMS_MAX_ONE_MMS_SIZE)
#endif
    {
#ifdef CSC_XML_SUPPORT
        if (10*1024 < s_mmimms_mms_limit && s_mmimms_mms_limit < MMIMMS_MAX_ONE_MMS_SIZE)
        {
            mms_limit_size = s_mmimms_mms_limit;
        }
        else
#endif
        {
            mms_limit_size = MMIMMS_MAX_ONE_MMS_SIZE;
        }
    }

    return mms_limit_size;
}

