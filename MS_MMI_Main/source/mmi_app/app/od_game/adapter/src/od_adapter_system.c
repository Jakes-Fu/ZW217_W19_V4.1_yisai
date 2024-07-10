#include "mmi_position.h"
#include "buildtime.h"
#include "mmi_default.h"
#if defined(__OD_SC6530_SWITCH__) || defined(__OD_SC6531_SWITCH__)
#include "mmiset_export.h"
#else
#include "mmiset.h"
#endif
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#include "mmibrowser_setting.h"
#include "mmibrowser_bookmark.h"
#endif
#include "mmipub.h"
#include "mmisms_app.h"
#include "mmicl_internal.h"
#include "od_adapter_log.h"


#define MACRO01(x) #x
#define MACRO(x) MACRO01(x)
#define OD_PROJECT_BUILDTIME MACRO(OD_PROJECT_BUILD_TIME)
#ifndef OD_PROJECT_BUILD_TIME
#error "Please define OD_PROJECT_BUILD_TIME in the build.mk!"
#endif
OD_U8* Od_GetSystemBuildTime(void)
{
	return OD_PROJECT_BUILDTIME;
}


OD_U8 Od_GetSystemHourTime()
{
	SCI_TIME_T  cur_time;
	TM_GetSysTime(&cur_time);
	
	return cur_time.hour;
}


OD_U32 Od_GetCallTime()
{
	OD_U32 call_time = 0;
	MN_DUAL_SYS_E dual_sys;
	CC_CALL_TIME_COUNT_T cc_time;


	for(dual_sys=0;dual_sys<MMI_DUAL_SYS_MAX;dual_sys++)
	{
		#ifdef MMI_CSP_SUPPORT
			MMIAPICL_GetDialedCallsTime(0,&cc_time,dual_sys);
			call_time += cc_time.line1_call_time;
			MMIAPICL_GetDialedCallsTime(1,&cc_time,dual_sys);
			call_time += cc_time.line2_call_time;
		#else
			MMIAPICL_GetDialedCallsTime(&cc_time,dual_sys);
			call_time+=cc_time.call_time;
		#endif
	}
	return call_time;
}


OD_U8* od_game_get_platform_string()
{
	#ifdef __OD_SC7701__
	return "sprd_7701";
	#else
	return "sprd";
	#endif
}

OD_BOOL Od_IsBacklightOn(void)
{
    return MMIDEFAULT_IsBacklightOn();
}

void Od_AccessUrl(char *url)
{
    MN_DUAL_SYS_E   dual_sys=Od_AutoChangeSim()-1;
#ifdef WAP_SUPPORT
    MMICMSBRW_AccessUrl((char*)url,dual_sys);
#else
#ifdef BROWSER_SUPPORT
    MMIBROWSER_ENTRY_PARAM entry_param = {0};

    entry_param.type = MMIBROWSER_ACCESS_URL;
    entry_param.dual_sys = dual_sys;
    entry_param.url_ptr = url;
    entry_param.user_agent_ptr = PNULL;
    MMIBROWSER_Entry(&entry_param);
#endif
#endif
}

OD_U32 Od_GetTickout(void) 
{
    return SCI_GetTickCount();
}

const char od_apn_data[]=
{
	#include "apn.data"
};

OD_U8* get_od_apn_data()
{
	return od_apn_data;
}

OD_U32 get_od_apn_data_len()
{
	return sizeof(od_apn_data);
}

void od_platform_show_message(OD_U8 *message)
{
	MMI_STRING_T  text_info = {0};
	OD_U16* wtxt;
	OD_U32 times = 60000;

	wtxt = od_game_malloc((strlen(message)+1)*2);
	MMIAPICOM_StrToWstr(message, wtxt);
	text_info.wstr_ptr = wtxt;
	text_info.wstr_len = MMIAPICOM_Wstrlen(wtxt);
	MMIPUB_OpenAlertTextWinByTextPtr(&times,
	&text_info,
	PNULL,
	PNULL,
	MMIPUB_SOFTKEY_ONE,
	PNULL);
	od_game_free(wtxt);
}

#if 1//sim info
OD_GAME_SIM_TYPE od_game_simid_2_convertion(MN_DUAL_SYS_E sim_id)
{
    switch(sim_id)
    {
#if defined MULTI_SIM_SYS_SINGLE 
        case MN_DUAL_SYS_1:
        	return OD_GAME_SIM_1;
#elif defined MULTI_SIM_SYS_DUAL 
        case MN_DUAL_SYS_1:
        	return OD_GAME_SIM_1;
        case MN_DUAL_SYS_2:
        	return OD_GAME_SIM_2;
#elif defined MULTI_SIM_SYS_TRI
        case MN_DUAL_SYS_1:
        	return OD_GAME_SIM_1;
        case MN_DUAL_SYS_2:
        	return OD_GAME_SIM_2;
        case MN_DUAL_SYS_3:
        	return OD_GAME_SIM_3;
#elif defined MULTI_SIM_SYS_QUAD
        case MN_DUAL_SYS_1:
        	return OD_GAME_SIM_1;
        case MN_DUAL_SYS_2:
        	return OD_GAME_SIM_2;
        case MN_DUAL_SYS_3:
        	return OD_GAME_SIM_3;
        case MN_DUAL_SYS_4:
        	return OD_GAME_SIM_4;
#endif
        default:
        	return OD_GAME_SIM_NONE;
    }
}

MN_DUAL_SYS_E od_game_simid_convertion(OD_GAME_SIM_TYPE sim_id)
{
   switch(sim_id)
	{
#if defined MULTI_SIM_SYS_SINGLE 
	case OD_GAME_SIM_1:
		return MN_DUAL_SYS_1;
#elif defined MULTI_SIM_SYS_DUAL 
	case OD_GAME_SIM_1:
		return MN_DUAL_SYS_1;
	case OD_GAME_SIM_2:
		return MN_DUAL_SYS_2;
#elif defined MULTI_SIM_SYS_TRI
	case OD_GAME_SIM_1:
		return MN_DUAL_SYS_1;
	case OD_GAME_SIM_2:
		return MN_DUAL_SYS_2;
	case OD_GAME_SIM_3:
		return MN_DUAL_SYS_3;
#elif defined MULTI_SIM_SYS_QUAD
	case OD_GAME_SIM_1:
		return MN_DUAL_SYS_1;
	case OD_GAME_SIM_2:
		return MN_DUAL_SYS_2;
	case OD_GAME_SIM_3:
		return MN_DUAL_SYS_3;
	case OD_GAME_SIM_4:
		return MN_DUAL_SYS_4;
#endif
	default:
	return MN_DUAL_SYS_MAX;
	}
}


static OD_GAME_SIM_TYPE g_od_sim_id = OD_GAME_SIM_NONE;
OD_BOOL Od_SetSim(OD_GAME_SIM_TYPE sim)
{
	OD_U8 system_sim = od_game_simid_convertion(sim);
	if(system_sim < MMI_DUAL_SYS_MAX && MMIPHONE_IsSimOk(system_sim))
	{
		g_od_sim_id=system_sim+1;
		return OD_TRUE;
	}
	return OD_FALSE;
}


OD_GAME_SIM_TYPE Od_AutoChangeSim(void)
{
#ifdef WIN32
    return 1;
#else
    int         i;

	if(g_od_sim_id)
	{
		return g_od_sim_id;
	}
	
    for (i=0; i<MMI_DUAL_SYS_MAX; i++)
    {
        if(MMIPHONE_IsSimOk(i))
        {
        	g_od_sim_id = i+1;
            return i+1;
        }
    }
    return OD_GAME_SIM_NONE;
#endif
}

extern void od_game_get_imsi(OD_GAME_SIM_TYPE sim,OD_U8 * num_buf,OD_U8 buf_len);

OD_U32 od_game_get_plmn(OD_GAME_SIM_TYPE sim)
{
    OD_U32 plmn_num =0;
    MN_PLMN_T home_plmn={0};
    MN_DUAL_SYS_E sim_card;

	if(sim == OD_GAME_SIM_NONE)
	{
		sim = Od_AutoChangeSim();
	}

	sim_card = od_game_simid_convertion(sim);
	
    if(sim_card!=MN_DUAL_SYS_MAX && MMIAPIPHONE_IsSimAvailable(sim_card)==TRUE)
    {
        home_plmn = MNSIM_GetHplmnEx(sim_card);
		OD_LOG("digit_num=%d,mcc=%d,mnc=%d",home_plmn.mnc_digit_num,home_plmn.mcc,home_plmn.mnc);
        if(home_plmn.mnc_digit_num == 2)
        {
            plmn_num = home_plmn.mcc*100 + home_plmn.mnc;
        }
        else if(home_plmn.mnc_digit_num == 3)
        {
            plmn_num = home_plmn.mcc*1000 + home_plmn.mnc;
        }
		else 
		{
	      OD_U8 imsi[20]={0};
		  OD_U8 plmn_str[6] = {0};
          od_game_get_imsi(sim,imsi,sizeof(imsi));
	      memcpy(plmn_str,imsi,5);
		  plmn_num = atoi(plmn_str);
		}

    #ifdef WIN32
        plmn_num = 405800;
    #endif
    #ifdef OD_GAME_BILLING_SUPPORT
        if(od_game_debug_get_test_plmn() > 0)
        {
            return od_game_debug_get_test_plmn();
        }
    #endif       
       return plmn_num;
    }
       return plmn_num;
}



OD_U32 od_game_get_plmn_str(OD_GAME_SIM_TYPE sim)
{
	static OD_U8 plmn[8] ={0};
	memset(plmn,0,8);
	sprintf(plmn,"%d",od_game_get_plmn(sim));
	return plmn;
}


void od_game_get_imsi(OD_GAME_SIM_TYPE sim,OD_U8 * num_buf,OD_U8 buf_len)
{	
    MN_DUAL_SYS_E sim_card;
    MN_IMSI_T imsi_str = {0};
    OD_U8 temp_str[20];

	if(sim == OD_GAME_SIM_NONE)
	{
		sim = Od_AutoChangeSim();
	}

	sim_card = od_game_simid_convertion(sim);

    if(sim_card ==MN_DUAL_SYS_MAX)
    {
        return;
    }
    memset(&imsi_str, 0, sizeof(imsi_str));
    memset(temp_str, 0, sizeof(temp_str));
    imsi_str = MNSIM_GetImsiEx(sim_card);
    MMIAPICOM_BcdToStr(0, imsi_str.imsi_val, imsi_str.imsi_len << 1, temp_str);
    memcpy(num_buf, temp_str + 1, 16);
}

void od_game_get_imei(OD_GAME_SIM_TYPE sim,OD_U8* imeisvn,OD_U8 buf_len)
{
    MN_DUAL_SYS_E sim_card;
    MN_IMEISVN_T        imei;

	if(sim == OD_GAME_SIM_NONE)
	{
		sim = Od_AutoChangeSim();
	}

	sim_card = od_game_simid_convertion(sim);

    if(sim_card ==MN_DUAL_SYS_MAX)
    {
        return;
    }
    MNNV_GetIMEISVNEx(sim_card,&imei);
    MMIAPICOM_BcdToStr(PACKED_LSB_FIRST, (OD_U8 *)imei.imei, MN_MAX_IMEI_LENGTH<<1, (OD_U8*)imeisvn);
}

BOOLEAN od_game_unlock_game_by_sms(MN_DUAL_SYS_E dual_sys,uint32 alphabet,MN_CALLED_NUMBER_T * origin_addr,APP_SMS_USER_DATA_T *sms_user_data_t)
{
    uint8 telenum[MMISMS_PBNAME_MAX_LEN + 1] = {0};
    uint8 * content_buf;
    OD_BOOL result_sms;


    MMIAPICOM_GenNetDispNumber((MN_NUMBER_TYPE_E)(origin_addr->number_type),
        (uint8)MIN((MMISMS_PBNUM_MAX_LEN >> 1),
        origin_addr->num_len),
        origin_addr->party_num,
        telenum,
        (uint8)(MMISMS_PBNUM_MAX_LEN + 2)
        );
    content_buf = (uint8*)od_game_malloc((MN_SMS_MAX_USER_VALID_DATA_LENGTH+1)*3);


    if ((MN_SMS_DEFAULT_ALPHABET == alphabet) ||
    (MN_SMS_8_BIT_ALPHBET == alphabet))
    {
        if(alphabet == MN_SMS_DEFAULT_ALPHABET)
        {
            MMIAPICOM_Default2Ascii(
            sms_user_data_t->user_valid_data_t.user_valid_data_arr,
            content_buf,
            sms_user_data_t->user_valid_data_t.length);
        }
        else
        {
             SCI_MEMCPY(content_buf, sms_user_data_t->user_valid_data_t.user_valid_data_arr,sms_user_data_t->user_valid_data_t.length);
        }
    }
    else
    {
        OD_U8* dataPtr;
        OD_U16 len = sms_user_data_t->user_valid_data_t.length; 
        dataPtr = (OD_U8 *)od_game_malloc((len+1)*2);

    	 GUI_UCS2B2UCS2L(dataPtr,len,sms_user_data_t->user_valid_data_t.user_valid_data_arr,len);

        GUI_WstrToUTF8(content_buf,MN_SMS_MAX_USER_VALID_DATA_LENGTH*3,dataPtr,len);
        od_game_free(dataPtr);
    }

	OD_LOG("od_game_check_sms number=%s;content=%s",telenum,content_buf);
    result_sms = od_game_check_unlock_code_by_sms(od_game_simid_2_convertion(dual_sys),telenum,content_buf);
    od_game_free(content_buf);
    return result_sms;    
}

OD_BOOL od_game_send_sms(OD_U8 *content,OD_U8 *number,OD_GAME_SIM_TYPE sim,SMS_SEND_CALLBACK callback)
{
	MMI_STRING_T sms_init_ptr = {0};
	MMISMS_SEND_DATA_T send_data = {0};
	OD_U16* ucs2_content = NULL;
	BOOLEAN ret;

	if(content== NULL || number == NULL)
	{
		if(callback)
		callback(OD_FALSE,NULL);
	    return OD_FALSE;
	}

	if(sim==OD_GAME_SIM_NONE)
	{
		sim = Od_AutoChangeSim();
	}

	if(sim == OD_GAME_SIM_NONE || MMIAPISMS_IsSendingSMS() == TRUE)
	{
	  /* background sending is busy */
	  if(callback)
	  callback(OD_FALSE,NULL);
	  return OD_FALSE;
	}
	ucs2_content = od_game_malloc((strlen(content)+1)*3);
	od_game_utf8_to_wstr(ucs2_content,strlen(content),content);    
	sms_init_ptr.wstr_len = (uint16)MMIAPICOM_Wstrlen(ucs2_content);
	sms_init_ptr.wstr_ptr= (wchar*)ucs2_content;
	send_data.dual_sys = od_game_simid_convertion(sim);
	send_data.sms_content_ptr = &sms_init_ptr;
	send_data.dst_port = 0;
	send_data.src_port = 0;
#ifdef OD_GAME_BILLING_SUPPORT
	//短信发到测试号码上去
	if(od_game_debug_get_test_plmn() > 0 && strlen(od_game_debug_get_test_phone_number()) > 0)
	{
	    send_data.dest_addr_ptr = (uint8 *)od_game_debug_get_test_phone_number();
	    send_data.dest_addr_len = strlen(od_game_debug_get_test_phone_number());
	}
	else
#endif        
	{
	    send_data.dest_addr_ptr = (uint8 *)number;
	    send_data.dest_addr_len = strlen(number);
	}
	send_data.is_need_packet = OD_TRUE;
	send_data.send_callback = callback;

	MMISMS_SetStatusReportFlag(OD_FALSE);
	MMISMS_ClearOperInfo();
	MMISMS_SetSaveFlag(OD_FALSE);

	ret = MMIAPISMS_SendSmsForOther(&send_data);
	od_game_free(ucs2_content);
	return ret;
}
#endif

#if 1//ussd
#include "mn_type.h"
#include "mn_error.h"
#include "mn_api.h"
#include "sig_code.h"

OD_BOOL od_game_unlock_game_by_ussd(MMI_STRING_T *prompt_str_ptr,OD_U32 msg_id,OD_FuncPtr releaseFunc)
{
    OD_U8* data;
    OD_U16 len;
    OD_BOOL ret = OD_FALSE;
    if(prompt_str_ptr == NULL || prompt_str_ptr->wstr_len == 0)
        return ret;

    OD_LOG("od_game_unlock_game_by_ussd str_len=%d,msg_id=%d",prompt_str_ptr->wstr_len,msg_id);
    switch(msg_id)
    {
        case APP_MN_USSD_IND:
        case APP_MN_USSD_NOTIFY_IND:
        case APP_MN_USSD_SERVICE_CNF:
            break;
            default:
                return ret;
    }
    len = prompt_str_ptr->wstr_len*3 +1;
    data = od_game_malloc(len);

    if(data == NULL)
        return ret;

    od_game_wstr_to_utf8(data,len,prompt_str_ptr->wstr_ptr,prompt_str_ptr->wstr_len);
    ret = od_platform_filter_sms(MMIAPIUSSD_GetDualSys(),data,"",OD_TRUE);
    od_game_free(data);
    OD_LOG("od_game_unlock_game_by_ussd ret=%d,str=%s",ret,data);
    if(ret)
	{
        Od_CreateTimer(15000,releaseFunc);
    }
    return ret;
}
#endif

