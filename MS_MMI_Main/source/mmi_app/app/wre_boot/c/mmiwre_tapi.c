#include "mmi_app_wre_boot_trc.h"
#ifdef WRE_SUPPORT

#include "sci_types.h"
#include "mn_type.h"
#include "mn_api.h"
#include "mmisms_export.h"
#include "mmi_custom_define.h"
#include "mmicc_export.h"
#include "mmiphone_export.h"
#include "mmiwre_tapi.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
//#include "mmiidle_dial.h"
#include "mmisms_read.h"
#include "mmisms_save.h"
#include "mmisms_send.h"
#include "mmisms_app.h"
#include "mmk_app.h"
#include "gui_ucs2b_converter.h"
#include "mmiwre_adaptor.h"
//#include "mmiidle_dial.h"
#include "mmisd_export.h"
#include "mmiwre.h"
#include "mmiwre_other.h"
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : ConvertBcdToDigitalStr
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/
LOCAL void ConvertBcdToDigitalStr(uint8 length, uint8 *bcd_ptr, uint8 *digital_ptr)
{
    int           i;
    uint8         temp;
    
    // get the first digital
    temp = (uint8)((*bcd_ptr >> 4) &0x0f);
    if (temp >= 0x0a)
    {
        *digital_ptr = (uint8)((temp - 0x0a) + 'A');
    }
    else
    {
        *digital_ptr = (uint8)(temp + '0');
    }
    
    bcd_ptr++;
    digital_ptr++;
    for (i=0; i<(length - 1); i++)
    {
        temp = *bcd_ptr;
        temp &= 0x0f;
        if (temp >= 0x0a)
        {
            *digital_ptr = (uint8)((temp - 0x0a) + 'A');
        }
        else
        {
            *digital_ptr = (uint8)(temp + '0');
        }
        digital_ptr++;
        temp = *bcd_ptr;
        temp = (uint8)((temp & 0xf0) >> 4);
        if ((temp == 0x0f) && (i == (length -1)))
        {
            *digital_ptr = '\0';  
            return;
        }
        else if (temp>=0x0a)
        {
            *digital_ptr = (uint8)((temp - 0x0a) + 'A');
        }
        else
        {
            *digital_ptr = (uint8)(temp + '0');
        }
        digital_ptr++;
        bcd_ptr++;
    }
    *digital_ptr = '\0';
}

/*****************************************************************************/
//  Description : GetSimNumSupported
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

uint32 GetSimNumSupported()
{
	return MMI_DUAL_SYS_MAX;
}

/*****************************************************************************/
//  Description : Adapt_GetSimInfo
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

BOOL Adapt_GetSimInfo(void *pList,BOOL imsineeded)
{
    uint8                temp_str[20];
    MN_IMSI_T	         imsi_str;
	uint32               num ,i;
	PADAPT_IMSI_LIST_T         pImsiList = (PADAPT_IMSI_LIST_T)pList;

       if (pList == NULL)
       {
             return FALSE;
       }
	   
	num = GetSimNumSupported();

	for(i=0;i<num;i++){
		
		if(FALSE == imsineeded)
		{
			pImsiList->simExisted[i] = MMIAPIPHONE_GetSimExistedStatus(i)?TRUE:FALSE;
		}
		else
		{
			imsi_str = MNSIM_GetImsiEx(i);
			
			if(MN_MAX_IMSI_ARR_LEN >= imsi_str.imsi_len)
			{
				ConvertBcdToDigitalStr(imsi_str.imsi_len, imsi_str.imsi_val, temp_str);
				sprintf((char*)(&pImsiList->imsiList[i]), "%s", temp_str);
				
				pImsiList->simExisted[i] = TRUE;
			}
			else
			{
				pImsiList->simExisted[i] = FALSE;
			}
		}
	}

	pImsiList->SlotNum = num;

    return TRUE;
}

#define WRE_MMISMS_OFFSERT_YEAR    1998

/*****************************************************************************/
//  Description : 判断是否为 UCS2 字符
//  Global resource dependence : none
//  Author: ZhangJuntao
//  Note:
/*****************************************************************************/
PUBLIC  BOOLEAN WREAPICOM_IsUcs2Char(uint16 ch)
{
    return (ch & 0xFF80) != 0;
}

static  BOOLEAN  g_is_wre_sms_sending = FALSE;

/*****************************************************************************/
//  Description : Adapt_GetWREStatus
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

PUBLIC uint8    Adapt_GetWREStatus(void)
{
    return (g_is_wre_sms_sending);
}

/*****************************************************************************/
//  Description : MMISMS_GetWREStatus
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

//这两个接口最好对MMI开放
BOOLEAN    MMISMS_GetWREStatus(void)
{
    return (g_is_wre_sms_sending);
}

/*****************************************************************************/
//  Description : MMISMS_SetWREStatus
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

PUBLIC void MMISMS_SetWREStatus(BOOLEAN is_on)
{
    g_is_wre_sms_sending = is_on;
}

/*****************************************************************************/
//  Description : MMIWRE_NotifyResultResult
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void MMIWRE_NotifyResultResult(BOOLEAN sendresult)
{
      WBOOT_WRE_SendSmsCallback((uint8)sendresult);
	  MMISMS_SetWREStatus(FALSE);
}

/*****************************************************************************/
//  Description : wre send sms callback function
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN WreSMSSendCnfCallback(BOOLEAN is_succss, DPARAM parm)
{//dm 不需要做任何处理，直接结束流程
    BOOLEAN result = TRUE;

    MMIWRE_NotifyResultResult(is_succss);
    return result;
}

/*****************************************************************************/
//  Description : Adapt_APPSendSms
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

//短信发送接口
PUBLIC  BOOLEAN Adapt_APPSendSms(                                //RETURN:
                                       uint32 dual_sys,
                                       void    * content_ptr,    //IN:
                                       uint8            *dest_addr_ptr,    //IN:
                                       uint8            dest_addr_len    //IN:
                                       )
{
//changed by minghu.mao for sms backsend
    MMISMS_SEND_DATA_T send_data = {0};

    if (PNULL == content_ptr || PNULL == dest_addr_ptr)
    {
        return FALSE;
    }

    send_data.dual_sys = dual_sys;
    send_data.sms_content_ptr = (MMI_STRING_T *)content_ptr;
    send_data.dst_port = 0;
    send_data.src_port = 0;
    send_data.dest_addr_ptr = dest_addr_ptr;
    send_data.dest_addr_len = strlen((char *)dest_addr_ptr);    // changed by yifei
    send_data.is_need_packet = FALSE;
    send_data.send_callback = WreSMSSendCnfCallback;

    MMISMS_SetWREStatus(TRUE);

    if (!MMIAPISMS_SendSmsForOther(&send_data))
    {
        //SCI_TRACE_LOW:"mmiwre Adapt_APPSendSms send fail"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_TAPI_239_112_2_18_3_11_2_236,(uint8*)"");
        MMISMS_SetWREStatus(FALSE);
        return FALSE;
    }
    return TRUE;
}

/*****************************************************************************/
//  Description : GetTransmitterDispString
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

LOCAL void GetTransmitterDispString(
                                    MN_SMS_ALPHABET_TYPE_E    alphabet,            //IN:
                                    MN_CALLED_NUMBER_T        *orginal_addr_ptr,    //IN:
                                    MMI_STRING_T            *string_ptr            //OUT:
                                    )
{
    uint8 tele_num[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    uint16 num_len = 0;
    wchar pb_name[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    MMI_STRING_T num_name = {0};
    BOOLEAN find_result = FALSE;
    
    /*
    SCI_ASSERT(PNULL != orginal_addr_ptr);
    SCI_ASSERT(PNULL != string_ptr);
    */
    if((PNULL == orginal_addr_ptr) || (PNULL == string_ptr))
    {
        return;
    }
    
    num_name.wstr_len  = 0;
    num_name.wstr_ptr = pb_name;
    
    if (orginal_addr_ptr->num_len > 0)
    {
        //indicate the phonebook info of the address is not saved.
        //SCI_TRACE_LOW:"MMISMS: GetTransmitterDispString number_type = %d, number_plan = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_TAPI_279_112_2_18_3_11_2_237,(uint8*)"dd",orginal_addr_ptr->number_type, orginal_addr_ptr->number_plan);
        
        num_len = MMIAPICOM_GenNetDispNumber((MN_NUMBER_TYPE_E)(orginal_addr_ptr->number_type),
                                                                    (uint8)MIN((MMISMS_PBNUM_MAX_LEN >> 1),
                                                                    orginal_addr_ptr->num_len),
                                                                    orginal_addr_ptr->party_num,
                                                                    tele_num,
                                                                    (uint8)(MMISMS_PBNUM_MAX_LEN + 2)
                                                                    );

        find_result = MMIAPISMS_GetNameByNumberFromPB(tele_num,&num_name, MMISMS_PBNAME_MAX_LEN);    
        
        if ((!find_result) 
            || (0 == num_name.wstr_len))
        {
            MMI_STRNTOWSTR(num_name.wstr_ptr,
                                        MMISMS_PBNAME_MAX_LEN,
                                        (const uint8 *)tele_num,
                                        MMISMS_PBNAME_MAX_LEN,
                                        strlen((char *)tele_num));

            num_name.wstr_len = strlen((char *)tele_num);
        }
    }
    else
    {
        num_name.wstr_len  = 0;
    }
    
    string_ptr->wstr_len  = num_name.wstr_len;

    MMI_WSTRNCPY(
        string_ptr->wstr_ptr,
        MMISMS_PBNAME_MAX_LEN,
        num_name.wstr_ptr,
        MMISMS_PBNAME_MAX_LEN,
        string_ptr->wstr_len
        );
}

/*****************************************************************************/
//  Description : WRESMS_RecvSms_CallBack
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

//仅仅过滤特定号码的特别短信,主要用户搜狐计费通路
BOOLEAN  WRESMS_RecvSms_CallBack(MN_DUAL_SYS_E dual_sys,uint32 alphabet,MN_CALLED_NUMBER_T * origin_addr,APP_SMS_USER_DATA_T *sms_user_data_t)
{
    uint8 telenum[MMISMS_PBNAME_MAX_LEN + 1] = {0};
	uint8 * ascii_vaild;//[MN_SMS_MAX_USER_VALID_DATA_LENGTH] = {0};


    if(FALSE == WBOOT_IsWRERunning())
		return FALSE;

	MMIAPICOM_GenNetDispNumber((MN_NUMBER_TYPE_E)(origin_addr->number_type),
		(uint8)MIN((MMISMS_PBNUM_MAX_LEN >> 1),
		origin_addr->num_len),
		origin_addr->party_num,
		telenum,
		(uint8)(MMISMS_PBNUM_MAX_LEN + 2)
		);


	/*
	//判断条件在于，要求严格区分掉WRE所关心的短信及彩信通知!
	//获得号码和内容
    MMIAPICOM_GenNetDispNumber(origin_addr->number_type,
								(origin_addr->num_len>20)?20:origin_addr->num_len,
								origin_addr->party_num,
								telenum,20);
	*/

    
	ascii_vaild = NULL;
	if ((MN_SMS_DEFAULT_ALPHABET == alphabet) ||
            (MN_SMS_8_BIT_ALPHBET == alphabet))
	{
		BOOLEAN  ret ;
		uint16 content_len ;
        uint8 * temp;
		ascii_vaild = SCI_ALLOC(MN_SMS_MAX_USER_VALID_DATA_LENGTH*2+2);
		if(!ascii_vaild)
		{
			return FALSE;
		}
		content_len = MMIAPICOM_Default2Ascii(
			sms_user_data_t->user_valid_data_t.user_valid_data_arr,
			ascii_vaild,
			sms_user_data_t->user_valid_data_t.length
			);
		//sms_user_data_t->user_valid_data_t.length = content_len;

        temp = SCI_ALLOC(MN_SMS_MAX_USER_VALID_DATA_LENGTH*2+2);
        if(!temp)
        {
            SCI_FREE(ascii_vaild);
            return FALSE;
		}
			
        SCI_MEMCPY(temp, ascii_vaild, content_len);
	

        MMI_STRNTOWSTR((wchar *)ascii_vaild, (size_t)MN_SMS_MAX_USER_VALID_DATA_LENGTH,
                        (const uint8 *)temp,
                        (size_t)MN_SMS_MAX_USER_VALID_DATA_LENGTH, (size_t)sms_user_data_t->user_valid_data_t.length);

        SCI_FREE(temp);

        ret = WBOOT_WRE_SmsReceiveCallBack(dual_sys,(char *)telenum, (uint16 *)ascii_vaild,content_len);

        SCI_FREE(ascii_vaild);
        return ret;

	}
	else
	{
		return WBOOT_WRE_SmsReceiveCallBack(dual_sys, (char *)telenum,
			(uint16 *)sms_user_data_t->user_valid_data_t.user_valid_data_arr,
			sms_user_data_t->user_valid_data_t.length);
	}
}

/*****************************************************************************/
//  Description : Adapt_MakeCall
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

//电话拨打
BOOL Adapt_MakeCall(uint32 simNo,uint32 calltype,char * tele_num)
{
	uint16 num_len ; 
	CC_CALL_SIM_TYPE_E call_type = CC_CALL_SIM_MAX;
    CC_RESULT_E cc_result  = CC_RESULT_SUCCESS;

	if(NULL == tele_num)
		return FALSE;

	if(FALSE == MMIAPIPHONE_GetSimExistedStatus(simNo))
	{
		return FALSE;
	}
	

	num_len = strlen(tele_num);

	if(calltype == 0)
	{
		if(simNo == 0)
			call_type = CC_SIM1_CALL;
		else if (simNo == 1)
			call_type = CC_SIM2_CALL;
#if defined(MMI_MULTI_SIM_SYS_TRI) || defined(MMI_MULTI_SIM_SYS_QUAD)		
		else if (simNo == 2)
			call_type = CC_SIM3_CALL;
#if defined(MMI_MULTI_SIM_SYS_QUAD)		
		else if (simNo == 3)
			call_type = CC_SIM4_CALL;
#endif
#endif
		else
			call_type = CC_CALL_SIM_MAX;
	}

	cc_result = MMIAPICC_MakeCall(simNo,(uint8 *)tele_num,num_len,PNULL, PNULL, call_type,CC_CALL_NORMAL_CALL,PNULL);

	if(cc_result == CC_RESULT_SUCCESS)
		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************/
//  Description : Adapt_GetImei
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

//获得IMEI号
BOOL Adapt_GetImei(uint32 simNo,char *imeiStr)
{
    BOOLEAN            ret;
    //  uint8               temp_str[20];
    MN_IMEI_T           imei_arr; 
	uint32               num ;

	if(imeiStr == NULL)
		return FALSE;

	num = GetSimNumSupported();
	if(simNo >= num)
		return FALSE;

	ret = MNNV_GetIMEIEx(simNo, imei_arr);
	if(ret == FALSE )
		return FALSE;
	
	ConvertBcdToDigitalStr(MN_MAX_IMEI_LENGTH, imei_arr, (uint8 *)imeiStr);
	

	return TRUE;
}

/*****************************************************************************/
//  Description : Adapt_GetImsi
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

BOOL Adapt_GetImsi(uint32 simNo,char *imsiStr)
{

    MN_IMSI_T	         imsi_str;
	uint32               num ;

	if(imsiStr == NULL)
		return FALSE;

	num = GetSimNumSupported();

	if(simNo >= num)
		return FALSE;

	imsi_str = MNSIM_GetImsiEx(simNo);
			
	if(MN_MAX_IMSI_ARR_LEN >= imsi_str.imsi_len)
	{
		ConvertBcdToDigitalStr(imsi_str.imsi_len, imsi_str.imsi_val, (uint8 *)imsiStr);

		return TRUE;
		
	}
    return FALSE;
}

WRE_PHONE_PLMN_T  wre_phone_plmn[MMI_DUAL_SYS_MAX] = {0};

/*****************************************************************************/
//  Description : Adapt_GetNetWorkInfo
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

//获得当前网络状态
WRE_PHONE_PLMN_T * Adapt_GetNetWorkInfo(uint32 simNo)
{

	if(simNo < MMI_DUAL_SYS_MAX)
	{
		BOOLEAN  bRet = MMIAPIPHONE_GetIsPsReady(simNo);
		if(bRet)
			return &wre_phone_plmn[simNo];
		else
		{
			SCI_MEMSET(&wre_phone_plmn[simNo],0,sizeof(WRE_PHONE_PLMN_T));
			return &wre_phone_plmn[simNo];
		}
	}
	else
	{
		return NULL;
	}
}


/*****************************************************************************/
//  Description : WRE_NETWORK_IND_Handle
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

//嵌入到MOCOR
PUBLIC void WRE_NETWORK_IND_Handle(DPARAM param)
{
	APP_MN_NETWORK_STATUS_IND_T status_ind = *((APP_MN_NETWORK_STATUS_IND_T *)param);
    
	uint32  simNo = status_ind.dual_sys;

	//保存到全局变量中
	wre_phone_plmn[simNo].plmn_status   = (ADAPT_MN_PHONE_PLMN_STATUS_E)status_ind.plmn_status;
	wre_phone_plmn[simNo].rat           = (ADAPT_MN_GMMREG_RAT_E)status_ind.rat;
	wre_phone_plmn[simNo].mcc           = status_ind.mcc;
	wre_phone_plmn[simNo].mnc_digit_num = status_ind.mnc_digit_num;
	wre_phone_plmn[simNo].mnc           = status_ind.mnc;
	wre_phone_plmn[simNo].cell_id       = status_ind.cell_id;
	wre_phone_plmn[simNo].lac           = status_ind.lac;
}


/*****************************************************************************/
//  Description : Adapt_IsSmsReady
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

BOOLEAN Adapt_IsSmsReady(uint32 simNo)
{
	if(simNo < MMI_DUAL_SYS_MAX)
	{
		return MMISMS_IsSMSReady(simNo);
	}
	else
	{
		return FALSE;
	}
}


/*****************************************************************************/
//  Description : Adapt_EntryBrowser
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

uint32 Adapt_EntryBrowser(uint32 simNo,char * urlink_ptr,BOOLEAN  is_use_wifi)
{
#ifdef BROWSER_SUPPORT
    MMIBROWSER_ENTRY_PARAM entry_param = {0};    /*lint !e64*/
    //char dcd_ua[MMIDCD_MAX_UA_LEN + 1] = {0};
#ifdef MMIWRE_MEM_CONFLICT_SOLUTION
    SCI_TRACE_LOW("Adapt_EntryBrowser, urlink = %s", urlink_ptr);
    if(wretomocorapp.AppType == WRETOMORCOR_NONE)
    {
        uint16 len = 0;

        if((urlink_ptr != PNULL) && ((len = SCI_STRLEN(urlink_ptr)) > 0))
        {
            wretomocorapp.AppType = WRETOMORCOR_BROWSER;
            WreToMocorApp_Realse();
            wretomocorapp.lpParam = (unsigned short *)SCI_ALLOCA(len + 1);
            memset((char *)wretomocorapp.lpParam, 0, (len + 1));
            SCI_STRCPY((char *)wretomocorapp.lpParam, urlink_ptr);
            MMIWRE_CloseWin();
            SCI_TRACE_LOW("Adapt_EntryBrowser, malloc memory len = %d", len);
            return TRUE;
        }
    }
#endif
    //strcat(dcd_ua, DCD_DEFAULT_UA);
    entry_param.user_agent_ptr = PNULL; 
    entry_param.type = MMIBROWSER_ACCESS_URL;
    entry_param.dual_sys = MN_DUAL_SYS_MAX;
    entry_param.url_ptr = urlink_ptr;
    entry_param.user_agent_ptr = NULL;
    entry_param.is_connection_customized = FALSE;
    //entry_param.connection_index = 1;
    //entry_param.is_use_wifi = is_use_wifi;
    
    MMIAPIBROWSER_Entry(&entry_param); 
#ifdef MMIWRE_MEM_CONFLICT_SOLUTION
    WreToMocorApp_Realse();
#endif
#endif
   return TRUE;
}


/*****************************************************************************/
//  Description : Adapt_GetSmsCenterAddr
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

int Adapt_GetSmsCenterAddr(uint32 simNo,char * telenum)
{
	MN_CALLED_NUMBER_T tp_sc_addr;
	ERR_MNSMS_CODE_E   error;
    uint32             num = 0;

	num = GetSimNumSupported();
	if(simNo >= num)
		return FALSE;

	error = MNSMS_GetScAddrEx(simNo,&tp_sc_addr); 

	if(ERR_MNSMS_NONE == error)
	{
		MMIAPICOM_GenNetDispNumber((MN_NUMBER_TYPE_E)(tp_sc_addr.number_type),
			(uint8)MIN((MMISMS_PBNUM_MAX_LEN >> 1),
			tp_sc_addr.num_len),
			tp_sc_addr.party_num,
			(uint8 *)telenum,
			(uint8)(MMISMS_PBNUM_MAX_LEN + 2)
			);

		return TRUE;
	}
	else
		return FALSE;

}


/*****************************************************************************/
//  Description : Adapt_APPWriteNewSMS
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void Adapt_APPWriteNewSMS(
	                       uint32    dual_sys,           //IN:
                               uint16        *sms_init_ptr,        //IN:
                               uint8            *dest_addr_ptr,        //IN:
                               uint8            dest_addr_len        //IN:
                               )  //add by qingjun.yu 2011.06.01
{
	  MMI_STRING_T  sms_init = {0};
	  sms_init.wstr_ptr = sms_init_ptr;
	  sms_init.wstr_len = MMIAPICOM_Wstrlen(sms_init_ptr);
       
	  MMIAPISMS_WriteNewMessage(dual_sys, &sms_init, dest_addr_ptr, dest_addr_len);
}

extern void MMIAPIIDLE_EditDialWin(MMI_MESSAGE_ID_E msg_id, BOOLEAN is_dial_key, char *init_tele, BOOLEAN is_dial_str);


/*****************************************************************************/
//  Description : Adapt_AppEditCall
//  Global resource dependence : 
//  Author:wei.zhou
//  Note:
/*****************************************************************************/

void Adapt_AppEditCall(char * tele_num)   //add by qingjun.yu 2011.06.01
{
      if (PNULL != tele_num)
      	{
      	      MMIAPIIDLE_EditDialWin(MSG_APP_OK, FALSE,  tele_num, TRUE);
      	}
}

/*********************************************************************
 *  Adapt_GetFlightMode
 *
 * 获取手机配置类信息,如屏幕、键盘类型等
 *
 * 参数
 * 无
 * 返回值
 *  TRUE--- 飞行模式状态
 *  FALSE-- 正常模式状态
*/
BOOL Adapt_GetFlightMode(void)
{
	return MMIAPISET_GetFlyMode();
}

/*****************************************************************************/
//  Description : Adapt_IsSDExistEx
//  Global resource dependence : 
//  Author: liang.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Adapt_IsSDExist(void)
{
    return MMISD_IsSDExistEx();
}

#endif
