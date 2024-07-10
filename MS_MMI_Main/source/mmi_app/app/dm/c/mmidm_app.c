/*************************************************************************
 ** File Name:      mmidm_app.c                                          *
 ** Author:         jian.ma                                              *
 ** Date:           2009/07/02                                           *
 ** Copyright:      2009 Spreadtrum, Incorporated. All Rights Reserved.  *
 ** Description:    This file defines the function about dm function     *
 *************************************************************************
 *************************************************************************
 **                        Edit History                                  *
 ** ---------------------------------------------------------------------*
 ** DATE           NAME             DESCRIPTION                          *
 ** 2009/07/02     jian.ma          Create.                              *
*************************************************************************/
#define _MMIDM_APP_C_
#include "mmi_app_dm_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif
#if (defined (DM_SUPPORT) || defined (DM_ONLY_SELFREG_SUPPORT))
/**---------------------------------------------------------------------------*
**                         DEPENDENCIES                                      *
**---------------------------------------------------------------------------*/
#include "mmidm_app.h"
#include "mmi_appmsg.h"
#include "mmi_nv.h"
#include "mmifmm_export.h"
#include "mmisms_export.h"
#include "mmidm_nv.h"
#include "mmidm_config.h"
#include "mmiphone_export.h"
#include "version.h"
#include "mmk_timer.h"
#ifndef DM_ONLY_SELFREG_SUPPORT
#include "mmidm_internal.h"
//#include "fota.h"
#endif /*DM_ONLY_SELFREG_SUPPORT*/
#include "mmidm_text.h"
#include "mmipub.h"
#include "mmi_data.h"


#include "mmiconnection_export.h"
#include "mmicc_export.h"
#include "mmicl_export.h"
#include "mmiudisk_export.h"
//#include "mmiset_func.h"
#ifdef JAVA_SUPPORT
#include "mmijava_export.h"
#endif
#include "mmipim_export.h"
//#include "mmiset_wintab.h"
#ifdef BROWSER_SUPPORT
#include "mmibrowser_export.h"
#endif
#include "mmipub.h"
#include "mmimms_export.h"
#if defined (NANDBOOT_SUPPORT) && !defined(WIN32)
#include "mn_api.h"
#include "mmipb_export.h"
#endif
#include "mmicl_internal.h"
#include "mmicc_nv.h"

#ifdef MMI_STREAMING_PLAYER_SUPPORT
#include "mmivp_export.h"
#endif
//#include "Mmieng.h"
#include "mmieng_export.h"
#ifdef DM_SUPPORT_DFRESH
#include"mmidm_parsexml.h"
#endif
#include "mmi_ring.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define DM_PUSH_HEADER_LENGTH               6              //DM wap push header length
#define DM_SR_REPLY_LEN                     23      //define dm self register sms reply length
#define DM_IMEI_BCD_MAX_LEN                 8       //define dm device imei bcd max length
#define DM_PACKED_LSB_FIRST                 0
#define DM_SR_CONTENT_MAX_LEN               256     //define dm self register sms max length      
#define DM_DEVICE_ID_MAX_LEN                64      //define dm device id max length 
#define SELF_REGISTRATION_OFF               0
#define SELF_REGISTRATION_ON                1
#define DM_CMCC_MCC_NUM                     460
#define DM_CMCC_MNC_NUM_00                  00
#define DM_CMCC_MNC_NUM_02                  02
#define DM_CMCC_MNC_NUM_07                  07
#define DM_MAX_READ_FILE_SIZE               (20*1024)
#define DM_MAX_BUFFER_LENGTH                DM_MAX_READ_FILE_SIZE

#define DM_IMEI_START_POS                   5

#define DM_MAN_INFO_EXT1                    "LG"
#define DM_MOD_INFO_EXT1                    "GW880"
#define DM_VER_INFO_EXT1                    "OPhone 1.5"
#define DM_MAN_INFO_EXT2                    "Newpostcom"
#define DM_MOD_INFO_EXT2                    "N330"
#define DM_VER_INFO_EXT2                    "N330_s3b_V1.2.1"
#define DM_IMEI_NUM_EXT                     "IMEI:001010623456291"
#ifdef DM_SUPPORT_DFRESH
#define DM_LOCK_CODE                        "20"
#define DM_UNLOCK_CODE                      "30" 
#endif
//#ifdef WIN32
////#define MMIDM_DEVICE_SYS_NAME               MMIFILE_DEVICE_UDISK
////#define MMIDM_DEVICE_SYS_NAME_LEN           MMIFILE_DEVICE_UDISK_LEN
//#define PACH_NAME                           L"\\dm\\"
//#define PACH_NAME_PATCH                     L"\\dm"
// #else
//#define MMIDM_DEVICE_SYS_NAME               MMIFILE_DEVICE_SYS_NAME
//#define MMIDM_DEVICE_SYS_NAME_LEN           MMIFILE_DEVICE_SYS_NAME_LEN
#define PACH_NAME                           L"dm"
//#define PACH_NAME_PATCH                     L"dm"
// #endif
#if defined CHIP_ENDIAN_LITTLE
#define MMIDM_JAVA_DMAP                     (0x50414d44)
#else
#define MMIDM_JAVA_DMAP                     (0x444D4150)
#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct  
{
    char man[DM_MAX_ID_STR];
    char mode[DM_MAX_ID_STR];
    char version[DM_MAX_ID_STR];
    char imei[DM_MAX_ID_STR];
    BOOLEAN IdasdecType;
}MMIDM_SERVER_TYPE_T;

typedef struct  
{
    BOOLEAN debug_mode;  //dm调试模式
    BOOLEAN log_switch;  //库lob开关
    BOOLEAN fota_switch; //FOTA开关
    BOOLEAN tree_switch; //DM tree 管理开关
    BOOLEAN dm_reg_switch; //DM 自注册开关
    uint32  reg_times;     //DM 注册次数
}MMIDM_DEBUG_INFO_T;

typedef struct  
{
    char            apn_info[MMIDM_APN_MAX_LEN];
    char            sr_number[MMIDM_SRNUM_MAX_LEN];
    char            sr_port[MMIDM_SRPORT_MAX_LEN];
    char            server_url[MMIDM_GATEWAY_IP_MAX_LEN];
    char            device_type_info[DM_MAX_ID_STR];
    char            oem_info[DM_MAX_ID_STR];
    char            srv_addr[MMIDM_GATEWAY_IP_MAX_LEN];
    char*           firmwareversion;
    char*           hardwareversion;
    BOOLEAN         handset_lock;
    BOOLEAN         is_download_finish;
    BOOLEAN         is_java_app_install;
    MMIDM_DM_STATE  dm_state;
    MMIDM_DM_STATE  dl_state;
    DM_SESSION_TYPE session_type;
    FOTA_UPDATA_STATE_E  fota_state;
    MMIDM_TEST_SERVER_E  server_type;
    MMIDM_SELF_REG_E     sr_ready;
    MN_DUAL_SYS_E   dm_sim;    
}MMIDM_INFO_T;


LOCAL MMIDM_SERVER_TYPE_T s_server_type_tab[]=
{
    {DM_DEFAULT_MAN,         DM_DEFAULT_MOD,          DM_DEFAULT_VER,           DM_DEFAULT_IMEI,    TRUE    },
    {DM_MAN_INFO_EXT1,      DM_MOD_INFO_EXT1,       DM_VER_INFO_EXT2,       DM_IMEI_NUM_EXT,    FALSE   },
    {DM_MAN_INFO_EXT2,      DM_MOD_INFO_EXT2,       DM_VER_INFO_EXT2,       DM_IMEI_NUM_EXT,    TRUE    },
    //0
};


/*---------------------------------------------------------------------------*
**                            GLOBAL FUCNTION IMPLEMENT                       *
**---------------------------------------------------------------------------*/
//const wchar  g_mmidm_dir_wstr[] = { 's','y','s','t','e','m','\\','d', 'm', 0 };
const wchar  g_mmidm_dir_wstr[] = { 'D', 'm', 0 };
/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
#define MMI_GET_BE_FROM_LE_UINT32(X)\
            ((X) & 0x000000FF) << 24 |\
            ((X) & 0x0000FF00) << 8 |\
            ((X) & 0x00FF0000) >> 8 |\
            ((X) & 0xFF000000) >> 24;

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/
LOCAL  MMIDM_DEBUG_INFO_T      s_dm_debug_info={FALSE,FALSE,FALSE,FALSE,FALSE,0};
LOCAL  MMIDM_INFO_T            s_dm_info = {0};
LOCAL  BOOLEAN s_is_sms_ready   = FALSE;
LOCAL  BOOLEAN s_is_net_ready[MN_DUAL_SYS_MAX]   = {0};
//LOCAL  MN_PHONE_PLMN_STATUS_E  s_plmn_status = PLMN_NO_SERVICE;
#ifndef DM_ONLY_SELFREG_SUPPORT
LOCAL  uint8 s_fota_send_report_times = 0;
LOCAL  int s_scomo_app_id[DM_SCOMO_APPID_MAX_NUMBER]={0};
LOCAL  BOOLEAN                 s_is_dm_run = FALSE; //mmi task state
LOCAL  MN_SMS_USER_VALID_DATA_T s_user_valid_data_t ={0};
LOCAL  BOOLEAN  s_dm_need_restart = FALSE;
LOCAL  char     s_port_num[10]= {0};
#ifdef MMI_STREAMING_PLAYER_SUPPORT
LOCAL  uint8    s_st_agent_ip[VP_MAX_IP_LENGTH+1];
#endif
LOCAL  BOOLEAN s_is_need_handle_delay_event = FALSE;
#endif /*DM_ONLY_SELFREG_SUPPORT*/
LOCAL  char s_string_arr[] ="";
/**--------------------------------------------------------------------------*
**                          LOCAL FUNCTION DECLARE                           *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Discription: if the sms is dm wap push
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/ 
LOCAL BOOLEAN IsDmWapPush(APP_SMS_USER_DATA_T  *sms_user_data);

/*****************************************************************************/
// Description :获得特服号
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetDmServerSmscNumber(char *dmSrvID);

/*****************************************************************************/
// Description :判断接收到的短信是否是来自特服号回复的自注册短信，若是则进入自注册回复短信内容判断
//          若否，则进入正常的处理流程
//params:address ----------  接收到短信的发送者号码,如发送者都是手机号是13581596634,那么address = +8613581596634 
//       text    ----------  接收到的短信的内容
//return :TRUE ---------  来自特服号的自注册回复短信
//      FALSE---------  一般的短信
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN DevRecvSRHandle(MN_DUAL_SYS_E dual_sys, char *addr_ptr,char *message_ptr);

/*****************************************************************************/
// Description :判断接收到的自注册回复短信是否注册成功,若成功，则将当前SIM卡的IMSI号存储到文件
//          lastImsi.bin中去，若否，则丢弃该短信
//param: SRSMS -----  接到的自注册回复短信内容
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN DevRecvSelfRegisterSMS(MN_DUAL_SYS_E dual_sys, char * srsms_ptr);
/*****************************************************************************/
//  Description : get device id
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
LOCAL uint32 GetdeviceId(char *devid_ptr, uint32 buf_len);

/*****************************************************************************/
//  Description : This function retrieves the current IMSI from SIM
//  Global resource dependence : none
//  return:String that holds the name of the IMSI
//  Note:during Integration,You must obtain this IMSI from the SIM card of Device
/*****************************************************************************/
LOCAL BOOLEAN GetCurrentIMSI(MN_DUAL_SYS_E     dual_sys, MN_IMSI_T *imsi_ptr);

/*****************************************************************************/
//  Description : This function store the current IMSI as the last IMSI .
//  Global resource dependence : none
//  return:String that holds the name of the IMSI
// Notes:  1)during Integration , decide a fixed position to store the last IMSI
//    this function should call by after the Self-Registration is successed.
//          the position should can't access by anyone easily!!
//          2)please ensure  cover last IMSI and current IMSI is new one!!!
/*****************************************************************************/
LOCAL BOOLEAN SaveCurrentIMSI(MN_IMSI_T *imsi_ptr);

/*****************************************************************************/
//  Description : This function retrieves the last IMSI of SIM from specified position.
//  Global resource dependence : none
//  return:String that holds the name of the IMSI
// Notes:  during Integration , decide a fixed position to store the last IMSI
//      the position should can't access by anyone easily!! 
/*****************************************************************************/
LOCAL BOOLEAN GetLastIMSI(MN_IMSI_T *imsi_ptr);

/*****************************************************************************/
//  Description : init device param
//  Global resource dependence : 
//  Author:yi.su
//  Note:2007.01.08
/*****************************************************************************/
LOCAL void DM_ParamInit(void);

/*****************************************************************************/
// Description : 获得特服号的端口号
// Global resource dependence:
// Author:yi.su
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetServePortNumber(uint32  *port_ptr);

/*****************************************************************************/
// Description : Get if is cmcc card
// Global resource dependence:
// Author:yi.su
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsCmccSimCard(MN_DUAL_SYS_E     dual_sys);

/******************************************************************************
Description: This function is used for set self-Registration Switch.
The callback functions for MVA main process to call.
Parameters:
 appRef   - Application Reference that will be passed to the wrappers.
Return: 0/1 
Notes: 
  1:if self_Registration switch on ,then check self_Registation 
Flag to decide whether to  initiate the Self_Registration.
  0:If the switch is off ,then MVA omits the Self_Registation process.      
******************************************************************************/
LOCAL uint32 IsNeedSelfRegister(MN_DUAL_SYS_E     dual_sys);

/*****************************************************************************/
//  Description : 向带端口的特服号发送自注册的短信，首先判断是否中移动的卡，若否，那么不
//          进行自注册，若是，那么获得特服号，端口号，然后将自注册短信发送出去
//params:   SR_Msg ----  自注册短信内容
//         iSR_MsgLen--- 自注册短信长度
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SendSelfRegisterSms(
                                  MN_DUAL_SYS_E dual_sys,
                                  char     *msg_ptr, 
                                  uint32    msg_len
                                  );

/*****************************************************************************/
//  Description : init imsi and imei
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
LOCAL void InitImeiAndImsiInfo(void);

#ifndef DM_ONLY_SELFREG_SUPPORT
/*****************************************************************************/
//  Description : load file to Image
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN LoadFileToImage(void);

/*****************************************************************************/
//  Description : create dm tree xml file
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateDmTreeXmlFile(BOOLEAN is_create_new);

/*****************************************************************************/
//  Description : init DM file location
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void CreateDmDirectory(void);

/*****************************************************************************/
//  Description : parse the file name
//  Global resource dependence : none
//  Author:rui xu
//  Note:
/*****************************************************************************/
LOCAL void  ParseFilename(wchar * filepath_ptr,wchar * * filename_ptr);


/*****************************************************************************/
//  Description : extract the java
//  Global resource dependence : none
//  Author:rui xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ExtractAppFileAD(
                                      wchar * filepath_ptr, //in
                                      uint32    full_path_len,
                                      MMIDM_EXT_FILE_INFO_T *extract_ptr //out  
                                      );

/*****************************************************************************/
//  Description : is scomo app 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsScomoApp(int suiteID);

/*****************************************************************************/
//  Discription: This function get net setting index
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
LOCAL uint8 MMIDM_GetNetSettingIndex(void);

/*****************************************************************************/
//  Discription: This function get mms setting index
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
LOCAL uint8 MMIDM_GetMmsSettingIndex(void);

/*****************************************************************************/
//  Description : get connet info
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
LOCAL MMICONNECTION_LINKSETTING_DETAIL_T * MMIDM_GetConnectInfo(uint32 index);
#endif /*DM_ONLY_SELFREG_SUPPORT*/

/*****************************************************************************/
//  Description : dm send sms callback function
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DmSMSSendCnfCallback(BOOLEAN is_succss, DPARAM parm);

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
 /*****************************************************************************/
// 	Description : set the sim dm will use
//	Global resource dependence : none
//  Author: maryxiao
//	Note
/*****************************************************************************/ 

PUBLIC void MMIDM_SetMainSim(MN_DUAL_SYS_E sim)
{
    SCI_TRACE_LOW("MMIDM==> MMIDM_SetMainSim sim=%d", sim);  
#ifndef DM_ONLY_SELFREG_SUPPORT
     MMIDM_DelDmFolderFiles();
     MMIDM_CleanResumeNv();    
#endif
     s_dm_info.dm_sim = sim;
     MMINV_WRITE(MMINV_DM_MAIN_SIM,&s_dm_info.dm_sim);
}
/*****************************************************************************/
// 	Description : get the sim dm will use
//	Global resource dependence : none
//  Author: maryxiao
//	Note
/*****************************************************************************/  
PUBLIC MN_DUAL_SYS_E MMIDM_GetMainSim(void)
{
    return s_dm_info.dm_sim;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: jian.ma
//  Note
/*****************************************************************************/
PUBLIC void MMIDM_Init(void)
{
    DM_ParamInit();
    #ifndef DM_ONLY_SELFREG_SUPPORT
    CreateDmDirectory();
    CreateDmTreeXmlFile(FALSE);
    #endif /*DM_ONLY_SELFREG_SUPPORT*/
}

/*****************************************************************************/
//  Description : clean dm data
//  Global resource dependence : 
//  Author: mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_CleanDmData(void)
{
    MN_IMSI_T last_imsi = {0};
    BOOL       is_download_finish = FALSE;
    FOTA_UPDATA_STATE_E fota_state = DM_FOTA_UPDATA_NONE;
    MMIDM_RESUME_TYPE resume_type = RESUME_TYPE_MAX;
    MMIDM_BROKEN_TYPE broken_type = DM_BROKEN_TYPE_NONE;
    MMIDM_PL_ALERT_TYPE alert_type = DM_PL_ALERT_NONE;

    
    MMINV_WRITE(MMINV_DM_IMSI_INFO,&last_imsi);
    MMINV_WRITE(MMINV_DM_DOWNLOAD_FINISH, &is_download_finish);
    MMINV_WRITE(MMINV_DM_FOTA_STATE,&fota_state);


    MMINV_WRITE(MMINV_DM_RESUME_TYPE,&resume_type);
    MMINV_WRITE(MMINV_DM_BROKEN_TYPE,&broken_type);
    MMINV_WRITE(MMINV_DM_ALERT_TYPE,&alert_type);
}
/*****************************************************************************/
//  Discription: if the sms is dm wap push
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/ 
LOCAL BOOLEAN IsDmWapPush(APP_SMS_USER_DATA_T  *sms_user_data)
{    
    BOOLEAN     is_dm = FALSE;
    uint8       dm_char_flag [1] = {0x87};
    uint8       push_header_info[DM_PUSH_HEADER_LENGTH] = {0x05,0x04,0x0b,0x84,0xc0,0x02}; 
//    int32       i=0;
#ifdef DM_SUPPORT_DFRESH 
    uint32      session_id = 0; 
    uint32      alert_id = 0;
    BOOL        ret = FALSE;  
    char buf[6]= {0};    
#endif
    if (PNULL == sms_user_data)
    {
        return is_dm;
    }
    //SCI_TRACE_LOW:"MMIDM==> IsDmWapPush content[4]= 0x%x,content[5]= 0x%x,content[6]= 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_458_112_2_18_2_13_45_0,(uint8*)"ddd", sms_user_data->user_valid_data_t.user_valid_data_arr[4],sms_user_data->user_valid_data_t.user_valid_data_arr[5],sms_user_data->user_valid_data_t.user_valid_data_arr[6]);

    if (!memcmp(sms_user_data->user_data_head_t.user_header_arr,push_header_info,DM_PUSH_HEADER_LENGTH))
    {
        //equal,is dm wap push
        if (!memcmp(sms_user_data->user_valid_data_t.user_valid_data_arr+5,dm_char_flag,1))
        {
            is_dm = TRUE;
#ifdef DM_SUPPORT_DFRESH
            if(!MMIDM_IsDmRun())
            {    
                sprintf(buf, "%02x%02x",sms_user_data->user_valid_data_t.user_valid_data_arr[DM_SESSIONID_1], sms_user_data->user_valid_data_t.user_valid_data_arr[DM_SESSIONID_2]);
                session_id = MMIDM_atoIU32(buf, 32, &ret);
                //SCI_TRACE_LOW:"MMIDM==>IsDmWapPush session_id buf %s"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_476_112_2_18_2_13_45_2,(uint8*)"s",buf);
                
                sprintf(buf, "%02x",sms_user_data->user_valid_data_t.user_valid_data_arr[DM_ALERT_ID]);
                alert_id = MMIDM_atoIU32(buf, 32, &ret);
                alert_id = (alert_id>>4)&0x03;
                //SCI_TRACE_LOW:"MMIDM==>IsDmWapPush alert_id  %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_481_112_2_18_2_13_45_3,(uint8*)"d",alert_id);
                NIA_Plugin_handleAlert(alert_id,0,PNULL,0,PNULL);
                MMIDM_setSessionId(session_id);
            }
#endif
        }
    }
    return(is_dm);
}

/*****************************************************************************/
//  Discription: handle sms MT ind
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/ 
PUBLIC  BOOLEAN MMIDM_HandleSmsMtInd(
                                        MN_DUAL_SYS_E dual_sys,
                                        uint8   *number_ptr,
                                        uint16   num_len,
                                        APP_SMS_USER_DATA_T  sms_user_data
                                       )
{
    uint8               tele_num[(MN_MAX_ADDR_BCD_LEN<<1) + 1] = {0};
    uint8               party_len = 0;  
    BOOLEAN             status = 0;
    BOOLEAN             result = FALSE;
    BOOLEAN             sms_is_wappush = FALSE;

    party_len = num_len<< 1;
    MMIAPICOM_BcdToStr(PACKED_LSB_FIRST,number_ptr,party_len,(char*)tele_num);
     
    //SCI_TRACE_LOW:"MMIDM==> MMIAPIDM_HandleSmsMtInd:"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_511_112_2_18_2_13_45_4,(uint8*)"");
    //process the reply of self register sms
    result = DevRecvSRHandle(dual_sys, (char *)tele_num, (char *)sms_user_data.user_valid_data_t.user_valid_data_arr);

    if(result)       
    {  
        #ifndef DM_ONLY_SELFREG_SUPPORT    
        MMIDM_SendFotaReport();
        #endif
        //SCI_TRACE_LOW:"MMIDM==> reply of self register sms,and write IMSI to NV success!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_520_112_2_18_2_13_45_5,(uint8*)"");
        return TRUE;
    }
    
    //process DM  server wap push
    sms_is_wappush = IsDmWapPush(&sms_user_data);
    //SCI_TRACE_LOW:"MMIDM==> sms_is_wappush = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_526_112_2_18_2_13_45_6,(uint8*)"d",sms_is_wappush);

    if(sms_is_wappush)
    {
        #ifndef DM_ONLY_SELFREG_SUPPORT
        if (MMIAPIUDISK_UdiskIsRun()) //U盘使用中
        {
            return TRUE;
        }
        if (!MMIDM_CheckIsDlSpaceEnouge())
        {
            MMIPUB_OpenAlertWarningWin(TXT_COMM_NO_SPACE);
            return TRUE;
        }
        if (MMIDM_IsDebugMode()) 
        {               
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DM_PUSH,TXT_NULL,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
            
        }
        if (DM_SELF_REG_SUCC == MMIDM_GetIsSrReady() &&!MMIDM_IsDmRun())
        {
            //SCI_TRACE_LOW:"MMIDM==> data[5]=0x%x,data[6]=0x%x,data[7]=0x%x"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_547_112_2_18_2_13_45_7,(uint8*)"ddd",sms_user_data.user_valid_data_t.user_valid_data_arr[5],sms_user_data.user_valid_data_t.user_valid_data_arr[6],sms_user_data.user_valid_data_t.user_valid_data_arr[7]);

            //SCI_TRACE_LOW:"MMIDM==> Start signal dm_ is_start len = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_549_112_2_18_2_13_45_8,(uint8*)"d",sms_user_data.user_valid_data_t.length);
            


            if (MMIDM_IsNeedDelayDMSession())
            {
                MMIDM_SetIsNeedRestart(TRUE);
                MMIDM_CreateDelayTimer(0);
                MMIDM_StopDmRunCheckTimer();
                s_dm_info.session_type = DM_SESSION_SERVER;
                status = TRUE;
            }
            else
            {
                status = MMIDM_StartDm(DM_SESSION_SERVER,
                        (char*)&sms_user_data.user_valid_data_t.user_valid_data_arr[6],
                        sms_user_data.user_valid_data_t.length-6);
                //SCI_TRACE_LOW:"MMIDM==> Start signal dm status = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_566_112_2_18_2_13_45_9,(uint8*)"d",status);
            }
            if (status)
            {
                SCI_MEMSET(&s_user_valid_data_t,0x00,sizeof(MN_SMS_USER_VALID_DATA_T));
                SCI_MEMCPY(&s_user_valid_data_t,&sms_user_data.user_valid_data_t,sizeof(MN_SMS_USER_VALID_DATA_T));
            }
            return TRUE;
        }
        #else  /*DM_ONLY_SELFREG_SUPPORT*/
        return TRUE;        
        #endif /*DM_ONLY_SELFREG_SUPPORT*/
    }

    return FALSE;
}

/*****************************************************************************/
//  Description :handle network change
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_HandleNetChange(MN_DUAL_SYS_E dual_sys)
{
    if(dual_sys != MMIDM_GetMainSim())
    {
        SCI_TRACE_LOW("MMIDM==>MMIDM_HandleNetChange: dual_sys = %d,DMMainSim=%d", dual_sys,MMIDM_GetMainSim());
        return;
    }
    
    if (MMIDM_GetRegTimes() < 1 &&DM_SELF_REG_NONE == MMIDM_GetIsSrReady())
    {   
        if (dual_sys < MN_DUAL_SYS_MAX)
        {
            SCI_TRACE_LOW("MMIDM==> MMIDM_HandleNetChange: dual_sys = %d",  dual_sys);
            s_is_net_ready[dual_sys] = TRUE;
            MMIDM_SendSelfRegisterSMS(dual_sys);
        }
    }
    #ifndef DM_ONLY_SELFREG_SUPPORT
    else
    {
        if( 0 != MMIDM_GetNoSigTimerId()&&!MMIDM_IsNeedDelayDMSession())
        {
            if(MMIDM_ReStartDmSession())
            {
                MMIDM_CloseNoSigTimer();
            }
        }
    }
    #endif /*DM_ONLY_SELFREG_SUPPORT*/
}

/*****************************************************************************/
// Description :判断接收到的短信是否是来自特服号回复的自注册短信，若是则进入自注册回复短信内容判断
//          若否，则进入正常的处理流程
//params:address ----------  接收到短信的发送者号码,如发送者都是手机号是13581596634,那么address = +8613581596634 
//       text    ----------  接收到的短信的内容
//return :TRUE ---------  来自特服号的自注册回复短信
//      FALSE---------  一般的短信
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN DevRecvSRHandle(MN_DUAL_SYS_E dual_sys, char *addr_ptr,char *message_ptr)
{
    BOOLEAN     ret = FALSE;
    char       server_id[MMIDM_SRNUM_MAX_LEN] = {0};
    char       replay_sms[DM_SR_REPLY_LEN] = {0};
    char       *sms_ptr = PNULL;

    if (PNULL == addr_ptr || PNULL == message_ptr)
    {
        return ret;
    }
    //SCI_TRACE_LOW:"MMIDM==> DevRecvSRHandle: msg = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_626_112_2_18_2_13_46_10,(uint8*)"s",message_ptr);

    //Get dm-server sms center number
    GetDmServerSmscNumber(server_id); 

    //SCI_TRACE_LOW:"MMIDM==> DevRecvSRHandle server_id=%s\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_631_112_2_18_2_13_46_11,(uint8*)"s", server_id);
    
    if(strstr(addr_ptr, server_id))
    {
        sms_ptr = strchr((char *)message_ptr, 'I');

        if(sms_ptr == NULL || memcmp(sms_ptr, "",1) == 0)
        {
            //SCI_TRACE_LOW:"MMIDM==> DevRecvSRHandle---wrong SMS text received\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_639_112_2_18_2_13_46_12,(uint8*)"");
            return ret;
        }
        //get reply sms content
        SCI_MEMSET(replay_sms, 0, sizeof(replay_sms));
        MMI_MEMCPY(replay_sms,DM_SR_REPLY_LEN,sms_ptr, sizeof(replay_sms) - 1,sizeof(replay_sms) - 1);
        if(DevRecvSelfRegisterSMS(dual_sys, replay_sms))
        {
            ret = TRUE;
        }           
    }
    return ret;
}

/*****************************************************************************/
// Description :获得特服号
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetDmServerSmscNumber(char *dmSrvID)
{
    /* get it from carrier*/
    MN_RETURN_RESULT_E  return_value = MN_RETURN_SUCCESS;
    char                srsr_number[MMIDM_SRNUM_MAX_LEN] = {0};

    MMINV_READ(MMINV_DM_SRNUM, &srsr_number, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        SCI_TRACE_LOW("[mmidm] GetDmServerSmscNumber return_value %d", return_value);
        return FALSE;
    }

    //SCI_TRACE_LOW:"MMIDM==> ----------GetDmServerSmscNumber  dmSrvID= %s---------\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_665_112_2_18_2_13_46_13,(uint8*)"s",dmSrvID);
    strcpy(dmSrvID, srsr_number);

    //SCI_TRACE_LOW:"MMIDM==> ----------GetDmServerSmscNumber  dmSrvID= %s---------\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_668_112_2_18_2_13_46_14,(uint8*)"s",dmSrvID);
    return TRUE;
}
/*****************************************************************************/
// Description :判断接收到的自注册回复短信是否注册成功,若成功，则将当前SIM卡的IMSI号存储到文件
//          lastImsi.bin中去，若否，则丢弃该短信
//param: SRSMS -----  接到的自注册回复短信内容
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN DevRecvSelfRegisterSMS(MN_DUAL_SYS_E dual_sys, char * srsms_ptr)
{
    BOOLEAN     ret = TRUE;
    char       srsms_ok[DM_DEVICE_ID_MAX_LEN]={0};
    char       srsms_error[DM_DEVICE_ID_MAX_LEN]={0};
    char       dev_imei[DM_DEVICE_ID_MAX_LEN] = {0};
    MN_IMSI_T   cur_imsi = {0};

    if (PNULL == srsms_ptr)
    {
        return FALSE;
    }
    GetdeviceId(dev_imei,DM_DEVICE_ID_MAX_LEN);
    strcpy(srsms_ok, dev_imei);
    strcat(srsms_ok, "/1");
    strcat(srsms_error, dev_imei);
    strcat(srsms_error, "/0");
    //SCI_TRACE_LOW:"MMIDM==> --DevRecvSelfRegisterSMS--SRSMS=%s, SRSMSOK=%s--\n,SRSMSERROR=%s--\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_692_112_2_18_2_13_46_15,(uint8*)"sss",srsms_ptr, srsms_ok, srsms_error);
    
    if(0 == strcmp(srsms_ptr,srsms_ok))
    {
        //self register successful,save the imsi info   
        GetCurrentIMSI(dual_sys, &cur_imsi);
        SaveCurrentIMSI(&cur_imsi);
        MMIDM_SetMainSim(dual_sys);
        MMIDM_SetIsSrReady(DM_SELF_REG_SUCC);
        if (MMIDM_IsDebugMode()) 
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DM_SMS_REG,STXT_FINISH,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
        }
        //SCI_TRACE_LOW:"MMIDM==> ---------the device register ok--------\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_704_112_2_18_2_13_46_16,(uint8*)"");
    }
    else if(0 == strcmp(srsms_ptr,srsms_error))
    {
        MMIDM_SetIsSrReady(DM_SELF_REG_FAIL);
        if (MMIDM_IsDebugMode()) 
        {
            MMIPUB_OpenAlertWinByTextId(PNULL,TXT_DM_SMS_REG,TXT_ERROR,IMAGE_PUBWIN_SUCCESS,PNULL,PNULL,MMIPUB_SOFTKEY_NONE,PNULL);
        }
        //SCI_TRACE_LOW:"MMIDM==> ----------failed to register the device---\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_713_112_2_18_2_13_46_17,(uint8*)"");
    }
    else
    {
        ret = FALSE;
    }
    return ret;
}

/*****************************************************************************/
//  Description : get device id
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
LOCAL uint32 GetdeviceId(char *devid_ptr, uint32 buf_len)
{
    if(PNULL == devid_ptr)
    {
        SCI_TRACE_LOW("[MMIDM] GetdeviceId PNULL == devid_ptr");
        return FALSE;
    }
    strcpy(devid_ptr, s_server_type_tab[s_dm_info.server_type].imei);
    return TRUE;
}
/*****************************************************************************/
//  Description : This function retrieves the current IMSI from SIM
//  Global resource dependence : none
//  return:String that holds the name of the IMSI
//  Note:during Integration,You must obtain this IMSI from the SIM card of Device
/*****************************************************************************/
LOCAL BOOLEAN GetCurrentIMSI(MN_DUAL_SYS_E     dual_sys, MN_IMSI_T *imsi_ptr)
{
    MN_IMSI_T           imsi_str = {0};
       
    if(!imsi_ptr)
    {   
        return FALSE;
    }
    else
    {
        //get current sim imsi info
        imsi_str = MNSIM_GetImsiEx(dual_sys);
        SCI_MEMCPY(imsi_ptr,&imsi_str,sizeof(imsi_str));
        return TRUE;
    }
}

/*****************************************************************************/
//  Description : This function store the current IMSI as the last IMSI .
//  Global resource dependence : none
//  return:String that holds the name of the IMSI
// Notes:  1)during Integration , decide a fixed position to store the last IMSI
//    this function should call by after the Self-Registration is successed.
//          the position should can't access by anyone easily!!
//          2)please ensure  cover last IMSI and current IMSI is new one!!!
/*****************************************************************************/
LOCAL BOOLEAN SaveCurrentIMSI(MN_IMSI_T *imsi_ptr)
{
    MN_IMSI_T last_imsi ={0};

    //get last imsi info
    GetLastIMSI(&last_imsi);

    if(0 != GUI_StringCompare(last_imsi.imsi_val,last_imsi.imsi_len, imsi_ptr->imsi_val, imsi_ptr->imsi_len))
    {
        //different sim-card,should save the imsi info
        MMINV_WRITE(MMINV_DM_IMSI_INFO,imsi_ptr);
        SCI_TRACE_BUF("MMIDM:====SaveCurrentIMSI==\n",imsi_ptr->imsi_val,imsi_ptr->imsi_len);
    }
    return TRUE;        
}

/*****************************************************************************/
//  Description : This function retrieves the last IMSI of SIM from specified position.
//  Global resource dependence : none
//  return:String that holds the name of the IMSI
// Notes:  during Integration , decide a fixed position to store the last IMSI
//      the position should can't access by anyone easily!! 
/*****************************************************************************/
LOCAL BOOLEAN GetLastIMSI(MN_IMSI_T *imsi_ptr)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    if(PNULL == imsi_ptr)
    {
        return FALSE;
    }
    else
    {  
        //get IMSI info from nv     
        MMINV_READ(MMINV_DM_IMSI_INFO,imsi_ptr,return_value);
        return TRUE;
    }
}
/*****************************************************************************/
//  Description : get is self register ready
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC MMIDM_SELF_REG_E  MMIDM_GetIsSrReady(void)
{
    //SCI_TRACE_LOW:"MMIDM==> get is sr ready = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_801_112_2_18_2_13_46_18,(uint8*)"d",s_dm_info.sr_ready);
    return(s_dm_info.sr_ready);
}

/*****************************************************************************/
//  Description : Set is self register ready
//  Global resource dependence : none
//  Author:kelly.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SetIsSrReady(MMIDM_SELF_REG_E  is_ready)
{
    //SCI_TRACE_LOW:"MMIDM==> set is sr ready = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_811_112_2_18_2_13_46_19,(uint8*)"d",is_ready);
    s_dm_info.sr_ready = is_ready;
}


/*****************************************************************************/
//  Description : get test server type
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC  MMIDM_TEST_SERVER_E  MMIDM_GetTestServerType(void)
{
    //SCI_TRACE_LOW:"MMIDM==> get test server type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_822_112_2_18_2_13_46_20,(uint8*)"d",s_dm_info.server_type);
    return(s_dm_info.server_type);
}

/*****************************************************************************/
//  Description : set test server type
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC  void  MMIDM_SetTestServerType( MMIDM_TEST_SERVER_E  type)
{
    //SCI_TRACE_LOW:"MMIDM==> set test server type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_832_112_2_18_2_13_46_21,(uint8*)"d",type);
    s_dm_info.server_type = type;
    MMINV_WRITE(MMINV_DM_SERVER_TYPE,&s_dm_info.server_type);
}

/*****************************************************************************/
//  Description : 判断当前SIM卡跟IMEI号是否已经注册，若是，那么就不用再次发送自注册短信，若否，
//          则发送如下格式的自注册短信:
//          IMEI:IMEI number/manufacture/mode/sw version
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_SendSelfRegisterSMS(MN_DUAL_SYS_E     dual_sys)
{
    char           sr_sms_arr[DM_SR_CONTENT_MAX_LEN] = {0};
    char           dev_imei[DM_DEVICE_ID_MAX_LEN] = {0};
    char           *devman_ptr = PNULL;
    char           *devver_ptr = PNULL;
    char           *devmode_ptr = PNULL;
    BOOLEAN         result = FALSE;
    if (!MMIDM_GetDmRefSwitchState())
    {
        return result;
    }
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_SendSelfRegisterSMS : sms=%d,net=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_855_112_2_18_2_13_46_22,(uint8*)"dd",s_is_sms_ready,s_is_net_ready);

    if (s_is_sms_ready && s_is_net_ready[dual_sys])
    {
        //SCI_TRACE_LOW:"MMIDM==> -------MMIDM_SendSelfRegisterSMS--------\n"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_859_112_2_18_2_13_46_23,(uint8*)"");
        if (!MMIAPIPHONE_GetSimExistedStatus(dual_sys))
        {
            //SCI_TRACE_LOW:"MMIDM==> --------SIM CARD NOT exist-------\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_862_112_2_18_2_13_46_24,(uint8*)"");
            return result;
        }
        if (SIM_STATUS_OK != MMIAPIPHONE_GetSimStatus(dual_sys))
        {
            //SCI_TRACE_LOW:"MMIDM==> --------SIM CARD NOT ready-------\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_867_112_2_18_2_13_46_25,(uint8*)"");
            return result;
        }
        if(!IsCmccSimCard(dual_sys))
        {
            //SCI_TRACE_LOW:"MMIDM==> --------NOT CMCC SIM CARD--------\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_872_112_2_18_2_13_46_26,(uint8*)"");
            return result;
        }

        if(!IsNeedSelfRegister(dual_sys))
        {
            if (DM_SELF_REG_SUCC != MMIDM_GetIsSrReady())
            {
                MMIDM_SetIsSrReady(DM_SELF_REG_SUCC);
                s_dm_debug_info.reg_times++;
                #ifndef DM_ONLY_SELFREG_SUPPORT
                MMIDM_HandeDmOption();
                #endif /*DM_ONLY_SELFREG_SUPPORT*/
            }
            //SCI_TRACE_LOW:"MMIDM==>--------the device have registered--------\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_885_112_2_18_2_13_46_27,(uint8*)"");
            result = TRUE;
            return result;
        }
        else if (DM_SELF_REG_FAIL == MMIDM_GetIsSrReady())
        {
            //SCI_TRACE_LOW:"MMIDM==>--------the device have registered fail-----\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_891_112_2_18_2_13_46_28,(uint8*)"");
            result = TRUE;
            return result;
        }
        else
        {
 
            //Get self register sms content
            GetdeviceId(dev_imei,DM_DEVICE_ID_MAX_LEN);

            strcpy(sr_sms_arr, dev_imei);
            //SCI_TRACE_LOW:"MMIDM==> ----------SRSMS devIMEI = %s---------\n"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_902_112_2_18_2_13_46_29,(uint8*)"s",sr_sms_arr);
            strcat(sr_sms_arr, "/");

            devman_ptr = MMIDM_GetMan();
            strcat(sr_sms_arr, devman_ptr);

            strcat(sr_sms_arr, "/");
            devmode_ptr = MMIDM_GetModel();

            strcat(sr_sms_arr, devmode_ptr);
            strcat(sr_sms_arr, "/");

            devver_ptr = MMIDM_GetSoftwareVersion();
            strcat(sr_sms_arr, devver_ptr);

            //send self register sms
            result =SendSelfRegisterSms(dual_sys,sr_sms_arr,strlen(sr_sms_arr));
            MMIDM_SetMainSim(dual_sys);
        }   
        return result;
    } 
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_SendSelfRegisterSMS fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_924_112_2_18_2_13_46_30,(uint8*)"");
        return FALSE;
    }
}

/*****************************************************************************/
//  Description : dm send sms callback function
//  Global resource dependence : 
//  Author:minghu.mao
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN DmSMSSendCnfCallback(BOOLEAN is_succss, DPARAM parm)
{//dm 不需要做任何处理，直接结束流程
    BOOLEAN result = TRUE;

    return result;
}

/*****************************************************************************/
//  Description : 向带端口的特服号发送自注册的短信，首先判断是否中移动的卡，若否，那么不
//          进行自注册，若是，那么获得特服号，端口号，然后将自注册短信发送出去
//  params: SR_Msg ----  自注册短信内容
//         iSR_MsgLen--- 自注册短信长度
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SendSelfRegisterSms(
                                  MN_DUAL_SYS_E dual_sys,
                                  char     *msg_ptr, 
                                  uint32    msg_len
                                  )
{
    MMI_STRING_T    message_content = {0};
    char            server_id[MMIDM_SRNUM_MAX_LEN] = {0};
    uint32          port_id = 0;
    BOOLEAN         result = FALSE;
    wchar           *sms_content = PNULL; 
    uint16          len =0;
    MMISMS_SEND_DATA_T send_data = {0};

    //SCI_TRACE_LOW:"MMIDM==> ----------SendSelfRegisterSms ---------\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_960_112_2_18_2_13_46_31,(uint8*)"");
    //get dm server number
    GetDmServerSmscNumber(server_id);   
    //SCI_TRACE_LOW:"MMIDM==>SR_Msg = %s \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_963_112_2_18_2_13_46_32,(uint8*)"s",msg_ptr);
    GetServePortNumber(&port_id);
    len = (msg_len+1)*sizeof(wchar);
    sms_content = SCI_ALLOCA(len);
    if (PNULL == sms_content)
    {
        return FALSE;
    }
    SCI_MEMSET(sms_content,0x00,len);
    MMI_STRNTOWSTR(sms_content,len,(uint8*)msg_ptr,msg_len,MIN(len,msg_len));
    
    message_content.wstr_len = MMIAPICOM_Wstrlen(sms_content);
    message_content.wstr_ptr = sms_content; /*lint !e64*/

    send_data.sms_content_ptr = &message_content;
    send_data.dst_port = port_id;
    send_data.src_port = port_id;
    send_data.dest_addr_ptr = (uint8 *)server_id;
    send_data.dest_addr_len = strlen(server_id);
    send_data.dual_sys = dual_sys;
    send_data.is_need_packet = FALSE;
    send_data.send_callback = DmSMSSendCnfCallback;

    //SCI_TRACE_LOW:"MMIDM==> SendSelfRegisterSms length = %d \n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_986_112_2_18_2_13_46_33,(uint8*)"d",message_content.wstr_len);
    
    result = MMIAPISMS_SendSmsForOther(&send_data);

    s_dm_debug_info.reg_times ++; //send dm message success

    //SCI_TRACE_LOW:"MMIDM==> MMISMS_AppSendSmsForDm reg_times = %d send result =%d\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_992_112_2_18_2_13_46_34,(uint8*)"dd",s_dm_debug_info.reg_times,result);
    SCI_FREE(sms_content);
    return result;
}

/*****************************************************************************/
//  Description : init imsi and imei
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
LOCAL void InitImeiAndImsiInfo(void)
{
    MN_IMSI_T imsi = {0};
    char    imei_nv[DM_MAX_ID_STR] = {0};
    char    imei [DM_IMEI_BCD_MAX_LEN] = {0};
    char    imei_str[DM_IMEI_BCD_MAX_LEN<<1] = {0};
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    //init imsi
    MMINV_READ(MMINV_DM_IMSI_INFO,&imsi,return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_DM_IMSI_INFO, &imsi);
    }
    //init imei
    if(MNNV_GetIMEIEx(MMIDM_GetMainSim(),imei)) /*lint !e64*/
    {
        MMIAPICOM_BcdToStr(DM_PACKED_LSB_FIRST,(uint8*)imei,DM_IMEI_BCD_MAX_LEN<<1,(char*)imei_str);
        SCI_MEMSET(&s_server_type_tab[s_dm_info.server_type].imei[DM_IMEI_START_POS],0,(DM_MAX_ID_STR-DM_IMEI_START_POS));
        strcpy(&s_server_type_tab[s_dm_info.server_type].imei[DM_IMEI_START_POS], &imei_str[1]);
    }
    MMINV_READ(MMINV_DM_IMEI_SET,&imei_nv,return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        if(0 != memcmp(imei_nv, &s_server_type_tab[s_dm_info.server_type].imei[DM_IMEI_START_POS],DM_MAX_ID_STR-DM_IMEI_START_POS))
        {
            MMINV_WRITE(MMINV_DM_IMEI_SET, &s_server_type_tab[s_dm_info.server_type].imei[DM_IMEI_START_POS]);
            SCI_MEMSET(&imsi,0,sizeof(MN_IMSI_T));
            MMINV_WRITE(MMINV_DM_IMSI_INFO, &imsi);
        }
    }
}
/*****************************************************************************/
//  Description : init device param
//  Global resource dependence : 
//  Author:yi.su
//  Note:2007.01.08
/*****************************************************************************/
LOCAL void DM_ParamInit(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMIDM_RESUME_TYPE  resume_type = RESUME_TYPE_MAX;
    MMIDM_BROKEN_TYPE  broken_type = DM_BROKEN_TYPE_NONE;
    BOOLEAN test_flag = FALSE;
    MMIDM_PL_ALERT_TYPE  alert_type = DM_PL_ALERT_NONE;
    //SCI_TRACE_LOW:"MMIDM==>  in Param_init() enter"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1044_112_2_18_2_13_46_35,(uint8*)"");
    test_flag = MMIAPIENG_GetCMCCTestFlag();
    //init cmwap server url
    MMINV_READ(MMINV_DM_GATEWAY_IP, s_dm_info.server_url, return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMCPY(s_dm_info.server_url,DM_GATEWAY_IP_NUMBER,sizeof(DM_GATEWAY_IP_NUMBER));
        MMINV_WRITE(MMINV_DM_GATEWAY_IP, s_dm_info.server_url);
    }
    //init dm sr reg port num
    MMINV_READ(MMINV_DM_SRPORT, s_dm_info.sr_port, return_value);
    //MS00216980 cheney
    if((MN_RETURN_SUCCESS != return_value)||
		(test_flag && (strcmp(s_dm_info.sr_port, DM_SELF_REGISTER_PORT_NUMBER_TEST) != 0))||
		(!test_flag && (strcmp(s_dm_info.sr_port, DM_SELF_REGISTER_PORT_NUMBER) !=0 )))
    {
	if(test_flag)
	{
		SCI_MEMCPY(s_dm_info.sr_port,DM_SELF_REGISTER_PORT_NUMBER_TEST,sizeof(DM_SELF_REGISTER_PORT_NUMBER_TEST));
	}
	else
	{
		SCI_MEMCPY(s_dm_info.sr_port,DM_SELF_REGISTER_PORT_NUMBER,sizeof(DM_SELF_REGISTER_PORT_NUMBER));
	}
        MMINV_WRITE(MMINV_DM_SRPORT, s_dm_info.sr_port);
    }
    //init apn cmwap
    MMINV_READ(MMINV_DM_APN, s_dm_info.apn_info, return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMCPY(s_dm_info.apn_info,DM_APN_CMWAP_INFO,sizeof(DM_APN_CMWAP_INFO));
        MMINV_WRITE(MMINV_DM_APN, s_dm_info.apn_info);
    }
    //init sr reg nember 1065840409
    MMINV_READ(MMINV_DM_SRNUM, s_dm_info.sr_number, return_value);
    //MS00216980 cheney
   if((MN_RETURN_SUCCESS != return_value)||
		(test_flag && (strcmp(s_dm_info.sr_number, DM_SELF_REGISTER_NUMBER_TEST) != 0))||
		(!test_flag && (strcmp(s_dm_info.sr_number, DM_SELF_REGISTER_NUMBER) !=0 )))
    {
	if(MMIAPIENG_GetCMCCTestFlag())
	{
		SCI_MEMCPY(s_dm_info.sr_number,DM_SELF_REGISTER_NUMBER_TEST,sizeof(DM_SELF_REGISTER_NUMBER_TEST));
	}
	else
	{
		SCI_MEMCPY(s_dm_info.sr_number,DM_SELF_REGISTER_NUMBER,sizeof(DM_SELF_REGISTER_NUMBER));   
	}
        MMINV_WRITE(MMINV_DM_SRNUM, s_dm_info.sr_number);
    }

    MMINV_READ(MMINV_DM_SERVER_TYPE, &s_dm_info.server_type, return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        s_dm_info.server_type = DM_SPREADTRUM;
        MMINV_WRITE(MMINV_DM_SERVER_TYPE,&s_dm_info.server_type);
    }
    //init dm mod info "8801h"
    MMINV_READ(MMINV_DM_MON_SET, s_server_type_tab[s_dm_info.server_type].mode, return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_DM_MON_SET, s_server_type_tab[s_dm_info.server_type].mode);
    }
    //init software version
    {
#ifndef WIN32
        char* temp_ptr = VERSION_GetInfo(PLATFORM_VERSION);/*lint !e605*/
#else
        char temp_ptr[]= "Platform Version: MOCOR_W09.43_Debug";
#endif
        uint32 len1 = SCI_STRLEN((char*)temp_ptr);
        uint8  offset = SCI_STRLEN("Platform Version: ");
        uint32 len = MIN((len1-offset),(DM_MAX_ID_STR-1));

        SCI_MEMSET(s_server_type_tab[s_dm_info.server_type].version, 0, sizeof(s_server_type_tab[s_dm_info.server_type].version));
        SCI_MEMCPY(s_server_type_tab[s_dm_info.server_type].version,temp_ptr+offset,len);
    }
    //init man "spreadtrum"
    MMINV_READ(MMINV_DM_MAN_SET, s_server_type_tab[s_dm_info.server_type].man, return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_DM_MAN_SET,s_server_type_tab[s_dm_info.server_type].man);
    }

    InitImeiAndImsiInfo();

    
    #ifndef DM_ONLY_SELFREG_SUPPORT
    //init dm  handset lock
    MMINV_READ(MMINV_DM_HANDSET_LOCK, &s_dm_info.handset_lock, return_value);
    //SCI_TRACE_LOW:"MMIDM==> in Param_init() enter lock=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1133_112_2_18_2_13_47_36,(uint8*)"d",s_dm_info.handset_lock);
    if(MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_DM_HANDSET_LOCK, &s_dm_info.handset_lock);
    }
    //init dm download finish flag
    MMINV_READ(MMINV_DM_DOWNLOAD_FINISH, &s_dm_info.is_download_finish, return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        s_dm_info.is_download_finish = FALSE;
        MMINV_WRITE(MMINV_DM_DOWNLOAD_FINISH, &s_dm_info.is_download_finish);
    }
    
    MMINV_READ(MMINV_DM_FOTA_STATE, &s_dm_info.fota_state, return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        s_dm_info.fota_state = DM_FOTA_UPDATA_NONE;
        MMINV_WRITE(MMINV_DM_FOTA_STATE,&s_dm_info.fota_state);
    }

    MMINV_READ(MMINV_DM_APPID_TAB, s_scomo_app_id, return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        SCI_MEMSET(s_scomo_app_id,0,sizeof(s_scomo_app_id));
        MMINV_WRITE(MMINV_DM_APPID_TAB,s_scomo_app_id);
    }
    MMINV_READ(MMINV_DM_RESUME_TYPE, &resume_type, return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        resume_type = RESUME_TYPE_MAX;
        MMINV_WRITE(MMINV_DM_RESUME_TYPE,&resume_type);
    }
    #endif /*DM_ONLY_SELFREG_SUPPORT*/

    MMINV_READ(MMINV_DM_MAIN_SIM, &s_dm_info.dm_sim, return_value);//MARY FOR MURI SIM
    if(MN_RETURN_SUCCESS != return_value)
    {
        s_dm_info.dm_sim = MN_DUAL_SYS_1;//MMIAPISET_GetActiveSim();//need supply
        MMINV_WRITE(MMINV_DM_MAIN_SIM,&s_dm_info.dm_sim);
    }    
    
    SCI_MEMCPY(s_dm_info.device_type_info,DM_DEVICE_TYPE_INFO_STR,sizeof(DM_DEVICE_TYPE_INFO_STR));
    SCI_MEMCPY(s_dm_info.oem_info,DM_OEM_INFO_STR,sizeof(DM_OEM_INFO_STR));
    //MS00216980 cheney
    if(MMIAPIENG_GetCMCCTestFlag())
    {
	   SCI_MEMCPY(s_dm_info.srv_addr,DM_SRV_ADDR_URL_TEST,sizeof(DM_SRV_ADDR_URL_TEST));
    }
    else
    {
	   SCI_MEMCPY(s_dm_info.srv_addr,DM_SRV_ADDR_URL,sizeof(DM_SRV_ADDR_URL));
    }
    s_dm_info.firmwareversion = DM_FIRMWARE_VERSION;
    s_dm_info.hardwareversion = DM_HARDWARE_VERSION;
    s_dm_info.dm_state = DM_NONE;
    s_dm_info.fota_state = DM_FOTA_UPDATA_NONE;
    s_dm_info.is_java_app_install = FALSE;
    s_dm_info.dl_state = DM_NONE;
    s_dm_debug_info.log_switch = FALSE;
    s_dm_debug_info.fota_switch = TRUE;
    s_dm_debug_info.tree_switch = TRUE;
    s_dm_debug_info.reg_times = 0; 

    MMINV_READ(MMINV_DM_REG_SWITCH, &s_dm_debug_info.dm_reg_switch, return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
#ifndef TDPS_UEIT_ARM_L1SIM 
        s_dm_debug_info.dm_reg_switch = TRUE;
#else
        s_dm_debug_info.dm_reg_switch = FALSE;
#endif
        MMINV_WRITE(MMINV_DM_REG_SWITCH,&s_dm_debug_info.dm_reg_switch);
    }
    
    #ifndef DM_ONLY_SELFREG_SUPPORT
    MMINV_READ(MMINV_DM_BROKEN_TYPE, &broken_type, return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        broken_type = DM_BROKEN_TYPE_NONE;
        MMINV_WRITE(MMINV_DM_BROKEN_TYPE,&broken_type);
    }
    #endif /*DM_ONLY_SELFREG_SUPPORT*/

    MMINV_READ(MMINV_DM_ALERT_TYPE, &alert_type, return_value);//MARYXIAO
    if(MN_RETURN_SUCCESS != return_value)
    {
        alert_type = DM_PL_ALERT_NONE;
        MMINV_WRITE(MMINV_DM_ALERT_TYPE,&alert_type);
    }
    //SCI_TRACE_LOW:"MMIDM==> DM_ParamInit  alert_type %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1222_112_2_18_2_13_47_37,(uint8*)"d", alert_type);
    //SCI_TRACE_LOW:"MMIDM==>  in devParam_init() end"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1223_112_2_18_2_13_47_38,(uint8*)"");
}

/*****************************************************************************/
// Description : set debug mode
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetDebugMode(BOOLEAN mode)
{
    s_dm_debug_info.debug_mode = mode;
}
/*****************************************************************************/
// Description :DM Is debug mode 
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsDebugMode(void)
{
    //SCI_TRACE_LOW:"MMIDM==> s_dm_debug_info.debug_mode = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1240_112_2_18_2_13_47_39,(uint8*)"d",s_dm_debug_info.debug_mode);
    return s_dm_debug_info.debug_mode;
}

/*****************************************************************************/
// Description :DM get reg times
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC uint32 MMIDM_GetRegTimes(void)
{
    //SCI_TRACE_LOW:"MMIDM==> s_dm_debug_info.reg_times = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1250_112_2_18_2_13_47_40,(uint8*)"d",s_dm_debug_info.reg_times);
    return s_dm_debug_info.reg_times;
}

/*****************************************************************************/
// Description : set dm reg switch state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetDmRegSwitchState(BOOLEAN mode)
{
    s_dm_debug_info.dm_reg_switch = mode;
    MMINV_WRITE(MMINV_DM_REG_SWITCH,&s_dm_debug_info.dm_reg_switch);
}
/*****************************************************************************/
// Description :DM dm reg switch state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetDmRefSwitchState(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    MMINV_READ(MMINV_DM_REG_SWITCH, &s_dm_debug_info.dm_reg_switch, return_value);
    //SCI_TRACE_LOW:"MMIDM==> s_dm_debug_info.dm_reg_switch = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1271_112_2_18_2_13_47_41,(uint8*)"d",s_dm_debug_info.dm_reg_switch);
    return s_dm_debug_info.dm_reg_switch;
}

/*****************************************************************************/
// Description : 获得特服号的端口号
// Global resource dependence:
// Author:yi.su
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN GetServePortNumber(uint32  *port_ptr) 
{
    MN_RETURN_RESULT_E  return_value =MN_RETURN_SUCCESS;
    char               srsr_port[MMIDM_SRPORT_MAX_LEN] = {0};

    if(PNULL == port_ptr)
        return FALSE;

    //SCI_TRACE_LOW:"MMIDM==>--enter into GetServePortNumber!!---\n"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1284_112_2_18_2_13_47_42,(uint8*)"");
    MMINV_READ(MMINV_DM_SRPORT, &srsr_port, return_value);
    if(MN_RETURN_SUCCESS == return_value)
    {
        *port_ptr = (uint16)atoi(srsr_port);
    }
    return TRUE;
}
/*****************************************************************************/
// Description : Get if is cmcc card
// Global resource dependence:
// Author:yi.su
// Note: 
/*****************************************************************************/
LOCAL BOOLEAN IsCmccSimCard(MN_DUAL_SYS_E     dual_sys)
{
    BOOLEAN                     is_cmcc = FALSE;
    PHONE_SERVICE_STATUS_T      service_status= {0};   

    //get single card mnc value
    MMIAPIPHONE_GetCurServiceStatus(dual_sys, &service_status);
    if( (DM_CMCC_MCC_NUM == service_status.plmn.mcc) &&
        ((DM_CMCC_MNC_NUM_00 == service_status.plmn.mnc )
        ||(DM_CMCC_MNC_NUM_02 == service_status.plmn.mnc )
        ||(DM_CMCC_MNC_NUM_07 == service_status.plmn.mnc ))
        )
    {
        is_cmcc = TRUE;
    }
    else
    {
        is_cmcc = FALSE;
    }
        
    return(is_cmcc);
}

/******************************************************************************
Description: This function is used for set self-Registration Switch.
The callback functions for MVA main process to call.
Parameters:
 appRef   - Application Reference that will be passed to the wrappers.
Return: 0/1
Notes: 
  1:if self_Registration switch on ,then MVA must check self_Registation 
Flag to decide whether to  initiate the Self_Registration.
  0:If the switch is off ,then MVA omits the Self_Registation process.      
******************************************************************************/
LOCAL uint32 IsNeedSelfRegister(MN_DUAL_SYS_E     dual_sys)
{ 
    MN_IMSI_T last_imsi ={0};
    MN_IMSI_T curr_imsi ={0};
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;
    
    SCI_TRACE_LOW("MMIDM==>--------DM_MVA IsNeedSelfRegister--------\n");
    GetLastIMSI(&last_imsi);

    SCI_TRACE_BUF("MMIDM==> DM LastIMSI = ",last_imsi.imsi_val,last_imsi.imsi_len);
    //长度为0 肯定需要注册
    if (0 == last_imsi.imsi_len)
    {
        SCI_TRACE_LOW("MMIDM==>: sr switch on last imsi=%d", last_imsi.imsi_len);
        return SELF_REGISTRATION_ON;    
    }
    //只要有一张卡注册 成功过就不需要再注册
    for (i = MN_DUAL_SYS_1; i < MN_DUAL_SYS_MAX; i++)
    {
        GetCurrentIMSI(i, &curr_imsi);
        SCI_TRACE_BUF("MMIDM==> DM CurrIMSI = ",curr_imsi.imsi_val,curr_imsi.imsi_len);
        if (0 == memcmp((char*)last_imsi.imsi_val, (char*)curr_imsi.imsi_val, MN_MAX_IMSI_ARR_LEN))
        {
            SCI_TRACE_LOW("MMIDM==>: sr switch off");
            if (MMIDM_GetMainSim() != i)
            {//有可能SIM 卡调换 位置，需要重新设置main sim
                MMIDM_SetMainSim(i);
            }
            return SELF_REGISTRATION_OFF;
        }
        SCI_MEMSET(&curr_imsi, 0, sizeof(MN_IMSI_T));
    }
    SCI_TRACE_LOW("MMIDM==>: sr switch on");
    return SELF_REGISTRATION_ON;    
}

/*****************************************************************************/
// Description : This function retrieves the device Manufacturer from
//          Non-volatile memory.
// return String that holds the name of the Manufacturer. 
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC  char *MMIDM_GetMan(void)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetMan %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1361_112_2_18_2_13_47_46,(uint8*)"s", s_server_type_tab[s_dm_info.server_type].man);
    return s_server_type_tab[s_dm_info.server_type].man;
}

/*****************************************************************************/
// Description : This function retrieves the device Model from Non-volatile memory.
// return String that holds the name of the device Model.  
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC  char *MMIDM_GetModel(void)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetModel %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1371_112_2_18_2_13_47_47,(uint8*)"s", s_server_type_tab[s_dm_info.server_type].mode);
    return s_server_type_tab[s_dm_info.server_type].mode;
}

/*****************************************************************************/
// Description : This function retrieves the DM client version from Non-volatile memory
// return String that holds the DM version. 
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC char *MMIDM_GetDMVersion(void)
{
    return DM_VERSION;
}

/*****************************************************************************/
// Description : This function retrieves the software version information
//          from Non-volatile memory.
// return String that holds the software version information.  
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC  char * MMIDM_GetSoftwareVersion(void)
{
    //SCI_TRACE_LOW:"MMIDM==> version %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1391_112_2_18_2_13_47_48,(uint8*)"s", s_server_type_tab[s_dm_info.server_type].version);
    return s_server_type_tab[s_dm_info.server_type].version;
}

/*****************************************************************************/
// 	Description : set sms ready
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIDM_SetSmsReady(BOOLEAN is_ready)
{
    s_is_sms_ready = is_ready;
}
/*****************************************************************************/
// 	Description : set net ready
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIDM_SetNetReady(BOOLEAN is_ready)
{
    MN_DUAL_SYS_E i = MN_DUAL_SYS_1;

    //工程模式设置--只要SIM 卡存在就置标记位
    for (i = MN_DUAL_SYS_1; i < MN_DUAL_SYS_MAX; i++)
    {
        if (MMIAPIPHONE_GetSimExistedStatus(i))
        {
            s_is_net_ready[i] = is_ready;
        }
    }
}

/*****************************************************************************/
//  Description : set dm para info
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SetDmParaInfo(MMIDM_DEBUG_TYPE_E type,char* string,uint32 len)
{
    int str_len=0;
    str_len = strlen(string);
    switch(type)
    {
    case DM_APN_SET:
        SCI_MEMSET(s_dm_info.apn_info,0,MMIDM_APN_MAX_LEN);
        SCI_MEMCPY(s_dm_info.apn_info,string,MIN(MMIDM_APN_MAX_LEN,str_len));
        MMINV_WRITE(MMINV_DM_APN, s_dm_info.apn_info);
        break;
    case DM_MON_SET:
        SCI_MEMSET(s_server_type_tab[s_dm_info.server_type].mode,0,DM_MAX_ID_STR);
        SCI_MEMCPY(s_server_type_tab[s_dm_info.server_type].mode,string,MIN(DM_MAX_ID_STR,str_len));
        break;
    case DM_DEV_VERSION_SET:
        SCI_MEMSET(s_server_type_tab[s_dm_info.server_type].version,0,DM_MAX_ID_STR);
        SCI_MEMCPY(s_server_type_tab[s_dm_info.server_type].version,string,MIN(DM_MAX_ID_STR,str_len));
        break;
    case DM_MAN_SET:
        SCI_MEMSET(s_server_type_tab[s_dm_info.server_type].man,0,DM_MAX_ID_STR);
        SCI_MEMCPY(s_server_type_tab[s_dm_info.server_type].man,string,MIN(DM_MAX_ID_STR,str_len));
        break;
    case DM_IMEI_SET:
        SCI_MEMSET(&s_server_type_tab[s_dm_info.server_type].imei[DM_IMEI_START_POS],0,(DM_MAX_ID_STR-DM_IMEI_START_POS));
        SCI_MEMCPY(&s_server_type_tab[s_dm_info.server_type].imei[DM_IMEI_START_POS],string,MIN((DM_MAX_ID_STR-DM_IMEI_START_POS),str_len));
        MMINV_WRITE(MMINV_DM_IMEI_SET, &s_server_type_tab[s_dm_info.server_type].imei[DM_IMEI_START_POS]);
        break;
    case DM_SRV_ADDR:
        SCI_MEMSET(s_dm_info.srv_addr,0,MMIDM_GATEWAY_IP_MAX_LEN);
        SCI_MEMCPY(s_dm_info.srv_addr,string,MIN(MMIDM_GATEWAY_IP_MAX_LEN,str_len));
        break;
    case DM_SELF_REG_NUM:
        SCI_MEMSET(s_dm_info.sr_number,0,MMIDM_SRNUM_MAX_LEN);
        SCI_MEMCPY(s_dm_info.sr_number,string,MIN(MMIDM_SRNUM_MAX_LEN,str_len));
        MMINV_WRITE(MMINV_DM_SRNUM, s_dm_info.sr_number);
        break;
    case DM_SELF_REGPORT_NUM:
        SCI_MEMSET(s_dm_info.sr_port,0,MMIDM_SRPORT_MAX_LEN);
        SCI_MEMCPY(s_dm_info.sr_port,string,MIN(MMIDM_SRPORT_MAX_LEN,str_len));
        MMINV_WRITE(MMINV_DM_SRPORT, s_dm_info.sr_port);
        break;
    default:
        break;
    }
}


/*****************************************************************************/
//  Description : Get dm para info
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_GetDmParaInfo(MMIDM_DEBUG_TYPE_E type,char* string)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    switch(type)
    {
    case DM_APN_SET:
        strcpy(string,s_dm_info.apn_info);
        break;
    case DM_MON_SET:
        strcpy(string,s_server_type_tab[s_dm_info.server_type].mode);
        break;
    case DM_DEV_VERSION_SET:
        strcpy(string,s_server_type_tab[s_dm_info.server_type].version);
        break;
    case DM_MAN_SET:
        strcpy(string,s_server_type_tab[s_dm_info.server_type].man);
        break;
    case DM_IMEI_SET:
        strcpy(string,&s_server_type_tab[s_dm_info.server_type].imei[DM_IMEI_START_POS]);
        break;
    case DM_SRV_ADDR:
        strcpy(string,s_dm_info.srv_addr);
        break;
    case DM_SELF_REG_NUM:
        MMINV_READ(MMINV_DM_SRNUM, s_dm_info.sr_number, return_value);
        strcpy(string,s_dm_info.sr_number);
        break;
    case DM_SELF_REGPORT_NUM:
        MMINV_READ(MMINV_DM_SRPORT, s_dm_info.sr_port, return_value);
        strcpy(string,s_dm_info.sr_port);
        break;
    default:
        break;
    }
}


#ifndef DM_ONLY_SELFREG_SUPPORT
/*****************************************************************************/
//  Description : init DM file location
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL void CreateDmDirectory(void)
{
    uint16              full_path_arr[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};//sd:/dm/..., udisk:/dm/...
    uint16              full_path_len = MMIFILE_FULL_PATH_MAX_LEN;

    MMIAPIFMM_CombineFullPath(
                            (uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_SYSTEM), 
                            MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SYSTEM),
                            (uint16 *)MMIDM_PATH_NAME, 
                            MMIDM_PATH_NAME_LEN,
                            PNULL, 
                            PNULL, //in bytes
                            full_path_arr,      //[out]
                            &full_path_len      //[out]
                            );

    MMIAPIFMM_CreateDir(full_path_arr,full_path_len);  
}

/*****************************************************************************/
//  Description : get file full path
//  Global resource dependence : none
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetFileFullPath(
                                     wchar *file_name,      //in
                                     uint16 file_name_len,  //in
                                     wchar* full_path,      //out
                                     uint16* full_path_len  //out
                                     )
{
    BOOLEAN             result = FALSE;

    result = MMIAPIFMM_CombineFullPath(
                            (uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_SYSTEM),
                            MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SYSTEM),
                            (uint16 *)MMIDM_PATH_NAME, 
                            MMIDM_PATH_NAME_LEN,
                            file_name, 
                            file_name_len, //in bytes
                            full_path,      //[out]
                            full_path_len   //[out]
                            );
   return result;
}

/*****************************************************************************/
//  Discription: This function is dm run
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIDM_IsDmRun(void)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_IsDmRun = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1562_112_2_18_2_13_48_49,(uint8*)"d",s_is_dm_run);
    return (s_is_dm_run);
}

/*****************************************************************************/
//  Discription: This function is to get dm state
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC  MMIDM_DM_STATE  MMIDM_GetDmState(void)
{
    //SCI_TRACE_LOW:"MMIDM==> get dm state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1573_112_2_18_2_13_48_50,(uint8*)"d",s_dm_info.dm_state);
    return (s_dm_info.dm_state);
}

/*****************************************************************************/
//  Discription: This function is to set dm state
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void  MMIDM_SetDmState(MMIDM_DM_STATE state)
{
    //SCI_TRACE_LOW:"MMIDM==> set dm state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1584_112_2_18_2_13_48_51,(uint8*)"d",state);
    s_dm_info.dm_state = state;
}

/*****************************************************************************/
//  Discription: This function is to get dm session type
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC  DM_SESSION_TYPE  MMIDM_GetDmSessionType(void)
{
    //SCI_TRACE_LOW:"MMIDM==> get dm session type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1595_112_2_18_2_13_48_52,(uint8*)"d",s_dm_info.session_type);
    return (s_dm_info.session_type);
}

/*****************************************************************************/
//  Discription: send signal to DM Task
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC  BOOLEAN MMIDM_SendSigToDmTask(
                                      uint32    event_id, 
                                      uint32    task_id,
                                      MMIDM_SESSION_T *session_info
                                      )
{
    BOOLEAN     action_result = TRUE;
    DM_SIGNAL_T* signal_ptr = PNULL;
    if (0 == task_id)
    {
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_SendSigToDmTask task_id=0"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1614_112_2_18_2_13_48_53,(uint8*)"");
        return FALSE;
    }
    
    signal_ptr = (DM_SIGNAL_T*)SCI_ALLOCA(sizeof(DM_SIGNAL_T));
    if (PNULL == signal_ptr)
    {
        //SCI_TRACE_LOW:"MMIDM==> PNULL == signal_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1621_112_2_18_2_13_48_54,(uint8*)"");
        return FALSE;
    }
    SCI_MEMSET(signal_ptr, 0, sizeof(DM_SIGNAL_T));

    signal_ptr->SignalSize = sizeof(DM_SIGNAL_T);
    signal_ptr->Sender = SCI_IdentifyThread();
    signal_ptr->SignalCode = event_id;
    if (PNULL!= session_info)
    {
        signal_ptr->content.type = session_info->type;

        if (0 != session_info->msg_size)
        {
            signal_ptr->content.msg_body = SCI_ALLOCA(session_info->msg_size +1);
            if (PNULL==signal_ptr->content.msg_body)
            {
                //SCI_TRACE_LOW:"MMIDM==> PNULL=signal_ptr->content.msg_body"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1638_112_2_18_2_13_48_55,(uint8*)"");
                return FALSE;
            }
            SCI_MEMSET(signal_ptr->content.msg_body,0x00,(session_info->msg_size+1));
            if(PNULL!=session_info->message_buffer)
            {
                SCI_MEMCPY(signal_ptr->content.msg_body,session_info->message_buffer,session_info->msg_size);
            }
            signal_ptr->content.msg_size = session_info->msg_size;
        }
    }
    //SCI_TRACE_LOW:"MMIDM==> SendSigToDmTask! event id =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1649_112_2_18_2_13_48_56,(uint8*)"d",event_id);
    
    SCI_SendSignal((xSignalHeader)signal_ptr,task_id);

    return(action_result);
}

/*****************************************************************************/
//  Discription: This function check is wap active
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsWapActive(void)
{
#ifdef BROWSER_SUPPORT
    return MMIAPIBROWSER_IsActive();
#else
    return FALSE;
#endif
}

/*****************************************************************************/
//  Discription: This function get net setting index
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
LOCAL uint8 MMIDM_GetNetSettingIndex(void)
{
    uint8 index = 0;
#ifdef BROWSER_SUPPORT
    index = MMIAPIBROWSER_GetNetSettingIndex(MMIDM_GetMainSim());
#endif
    return index;
}
/*****************************************************************************/
//  Discription: This function get mms setting index
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
LOCAL uint8 MMIDM_GetMmsSettingIndex(void)
{
    return MMIAPIMMS_GetNetSettingIndex(MMIDM_GetMainSim());
}

/*****************************************************************************/
//  Discription: This function restart dm session
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_ReStartDmSession(void)
{
    BOOLEAN status = FALSE;
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_ReStartDmSession s_dm_need_restart = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1701_112_2_18_2_13_48_57,(uint8*)"d",s_dm_need_restart);
    if(!MMIDM_IsDmRun() && s_dm_need_restart)
    {
        MMIDM_RESUME_TYPE  resume_type = MMIDM_GetResumeType();
        MMIDM_BROKEN_TYPE  broken_type = MMIDM_GetBrokemType();
        switch(resume_type)
        {
        case RESUME_TYPE_FUMO:
            if (DM_BROKEN_TYPE_DM == broken_type)
            {
                if (DM_SESSION_SERVER == s_dm_info.session_type &&s_user_valid_data_t.length!=0)
                {
                    status = MMIDM_StartDm(DM_SESSION_SERVER,(char*)&s_user_valid_data_t.user_valid_data_arr[6],s_user_valid_data_t.length-6);
                }
            }
            else
            {
                status = MMIDM_StartDm(DM_SESSION_FUMO_RESUME,PNULL,0);
            }
            break;
        case RESUME_TYPE_SCOMO:
            if (DM_BROKEN_TYPE_DM == broken_type)
            {
                if (DM_SESSION_SERVER == s_dm_info.session_type &&s_user_valid_data_t.length!=0)
                {
                    status = MMIDM_StartDm(DM_SESSION_SERVER,(char*)&s_user_valid_data_t.user_valid_data_arr[6],s_user_valid_data_t.length-6);
                }
            }
            else
            {
                status = MMIDM_StartDm(DM_SESSION_SCOMO_RESUME,PNULL,0);
            }
            break;
        default:
            if (DM_SESSION_SERVER == s_dm_info.session_type &&s_user_valid_data_t.length!=0)
            {
                status = MMIDM_StartDm(DM_SESSION_SERVER,(char*)&s_user_valid_data_t.user_valid_data_arr[6],s_user_valid_data_t.length-6);
            }
            else if (DM_SESSION_CLIENT == s_dm_info.session_type)
            { 
                status = MMIDM_StartDm(DM_SESSION_CLIENT,PNULL,0);
            }
            break;
        } 
        MMIDM_SetIsNeedRestart(FALSE);
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_ReStartDmSession status = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1746_112_2_18_2_13_48_58,(uint8*)"d",status);
    }

    return status;
}

/*****************************************************************************/
//  Discription: This function reset dm push info
//  Global resource dependence: None
//  Author: jian.ma
//  Note : 
//  Return:
/*****************************************************************************/
PUBLIC void MMIDM_ReSetDmPushInfo(void)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_ReSetDmPushInfo "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1759_112_2_18_2_13_48_59,(uint8*)"");
    s_dm_info.session_type = DM_SESSION_NONE;
    SCI_MEMSET(&s_user_valid_data_t,0x00,sizeof(MN_SMS_USER_VALID_DATA_T));
    MMIDM_SetIsNeedRestart(FALSE);
}

/*****************************************************************************/
//  Description :handle dm rx change
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_HandDmRxChange(uint8 rx_level)
{
    SCI_TRACE_LOW("MMIDM==> MMIDM_HandDmRxChange  rx_level=%d s_dm_need_restart=%d",rx_level,s_dm_need_restart);
    /*+++ cr251729*/
	if (0 == rx_level)
    {
        if(MMIDM_IsDmRun() 
            && !MMIDM_GetIsNeedRestart()
            && 0 == MMIDM_GetNoSigTimerId() 
            && 0 == MMIDM_GetBrokenTimerId())
        {
            SCI_TRACE_LOW("MMIDM==> MMIDM_HandDmRxChange: Create No Signal Timer");
            MMIDM_CreateNoSigTimer();
        }
    }
    else
    {
        if(0 != MMIDM_GetNoSigTimerId())
        {
            if(MMIDM_IsDmRun()&&!MMIDM_GetIsNeedRestart())
            {
                SCI_TRACE_LOW("MMIDM==> MMIDM_HandDmRxChange: Send DM_TASK_DM_BROKEN");
                MMIDM_SendSigToDmTask(DM_TASK_DM_BROKEN,MMIDM_GetDmTaskID(),PNULL);
                MMIDM_SetIsNeedRestart(TRUE);
                MMIDM_CreateBrokenSessionTimer();
            }
            else
            {
                MN_PHONE_PLMN_STATUS_E status = MMIAPIPHONE_GetPlmnStatus(MMIDM_GetMainSim());
                if( MMIDM_GetIsNeedRestart()
                    && 0 != MMIDM_GetNoSigTimerId()
                    && (PLMN_NORMAL_GSM_ONLY == status 
                    || PLMN_NORMAL_GSM_GPRS_BOTH == status
                    || PLMN_NORMAL_GPRS_ONLY == status )
                    && !MMIDM_IsNeedDelayDMSession())
                {
                    if(MMIDM_ReStartDmSession())
                    {
                        SCI_TRACE_LOW("MMIDM==> MMIDM_HandDmRxChange: Restart DM");
                        MMIDM_CloseNoSigTimer();
                        MMIDM_CloseBrokenSessionTimer();
                    }
                }
            }
        }
    }
	/*--- cr251729*/
}

/*****************************************************************************/
//  Description :set is need restart flag
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SetIsNeedRestart(BOOLEAN is_need_restart)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_SetIsNeedRestart  is_need_restart=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1806_112_2_18_2_13_48_61,(uint8*)"d",is_need_restart);
    s_dm_need_restart = is_need_restart;
}

/*****************************************************************************/
//  Description :get need restart flag
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetIsNeedRestart(void)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetIsNeedRestart  s_dm_need_restart=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1816_112_2_18_2_13_48_62,(uint8*)"d",s_dm_need_restart);
    return s_dm_need_restart;
}


/*****************************************************************************/
//  Description : create dm tree xml file
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN CreateDmTreeXmlFile(BOOLEAN is_create_new)
{
    MMIRES_DATA_INFO_T  file_info = {0}; /*lint !e64*/
    uint32              file_len = 0;
    wchar*              filename_ptr = PNULL;
    wchar               full_path_arr[MMIFILE_FULL_PATH_MAX_LEN+ 1] = {0};
    uint16              full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    BOOLEAN             result = FALSE;
    SFS_HANDLE          sfs_handle = NULL;
    //uint32              file_size = 0;
    SFS_ERROR_E         error = SFS_NO_ERROR;
    uint32              write_size = 0;
    
    MMI_GetDataInfo(D_DM_TREE_FILE, &file_info);
    file_len = strlen(DM_CLIENT_PL_FILENAME_TREE);
    filename_ptr = SCI_ALLOCAZ(sizeof(wchar)*(file_len+1));
    if (PNULL == filename_ptr)
    {
        return result;
    }

    MMIAPICOM_StrToWstr((uint8*)DM_CLIENT_PL_FILENAME_TREE,filename_ptr);
    
    if(MMIDM_GetFileFullPath(filename_ptr,(uint16)MMIAPICOM_Wstrlen(filename_ptr),full_path_arr,&full_path_len))
    {
        if(MMIAPIFMM_IsFileExist(full_path_arr,full_path_len))
        {
            if (is_create_new)
            {
                error = MMIAPIFMM_DeleteFile(full_path_arr,PNULL);
                //SCI_TRACE_LOW:"MMIDM==> CreateDmTreeXmlFile MMIAPIFMM_DeleteFile error=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1856_112_2_18_2_13_48_63,(uint8*)"d",error);
            }
            else
            {
                SCI_FREE(filename_ptr);
                return TRUE;
            }
        }
        //open fumo file
        sfs_handle = MMIAPIFMM_CreateFile(full_path_arr,SFS_MODE_READ|SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS,PNULL,PNULL);
        if (0 != sfs_handle)
        {
            error = MMIAPIFMM_WriteFile(
                sfs_handle,
                file_info.data_ptr,
                file_info.data_len,
                &write_size,
                NULL
                );
            if ( error == SFS_NO_ERROR && write_size == file_info.data_len)
            {
                //SCI_TRACE_LOW:"MMIDM==> tree.xml WriteFile succ!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1877_112_2_18_2_13_48_64,(uint8*)"");
                result = TRUE;
            }
            else
            {
                //SCI_TRACE_LOW:"MMIDM==> tree.xml WriteFile fail!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1882_112_2_18_2_13_48_65,(uint8*)"");
                result = FALSE;
            }
            MMIAPIFMM_CloseFile(sfs_handle);
        }

    }
    SCI_FREE(filename_ptr);
    return result;
}

/*****************************************************************************/
//  Description : start dm session
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_StartDm(DM_SESSION_TYPE type,char* msg_body, uint32 msg_size)
{
    MMIDM_SESSION_T     dm_session_info ={0};
    BOOLEAN             result = FALSE;

    CreateDmDirectory();

    CreateDmTreeXmlFile(FALSE);

    MMIDM_CreateTask();

    dm_session_info.message_buffer = msg_body;
    dm_session_info.msg_size = msg_size;
    dm_session_info.type = type;
    s_dm_info.session_type = type;
#ifdef DM_SUPPORT_REDBENCH
    if(!MMIDM_IsDmRun())
#elif defined(DM_SUPPORT_DFRESH)
    VDM_Comm_PL_HTTP_init_dfresh(); //mary
        MMIDM_clearDmXmlData();
#endif
    { 
        s_is_dm_run = TRUE;
        MMIDM_CreateDmRunCheckTimer();
#ifdef DM_SUPPORT_DFRESH
        MMIDM_SetDmState(DM_START);
#endif
        result = MMIDM_SendSigToDmTask(DM_TASK_START_MESSAGE,MMIDM_GetDmTaskID(),&dm_session_info);
    }
   // else
   // {
   //     MMIDM_CloseWaitingWin();
   //     MMIDM_SendSigToDmTask(DM_TASK_DM_EXIT,MMIDM_GetDmTaskID(),PNULL);
    //    MMIPUB_OpenAlertWarningWin(TXT_DM_SERVICE_RUN);
    //}
    if (!result)
    {
       s_dm_info.session_type = DM_SESSION_NONE; 
    }
    return result;
}

/*****************************************************************************/
//  Description : exit dm session
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_ExitDM(void)
{
    if (MMIDM_IsDmRun())
    {
        MMIDM_SessionCancel();
    }
    if (MMIDM_PDP_STATUS_NONE !=MMIDM_GetPdpActiveStatus())
    {
        MMIDM_DeactivePdpContext();
    }
#ifdef DM_SUPPORT_DFRESH    
    s_is_dm_run = FALSE;//mary111
#endif
    return TRUE;
}

/*****************************************************************************/
// Description : set nv down load finish flag
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetNVDownloadFinish(BOOLEAN is_finish)
{
    s_dm_info.is_download_finish = is_finish;
    MMINV_WRITE(MMINV_DM_DOWNLOAD_FINISH, &s_dm_info.is_download_finish);
}
/*****************************************************************************/
// Description : get nv down load finish flag
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetNVDownloadFinish(void)
{
    //SCI_TRACE_LOW:"MMIDM==> s_dm_info.is_download_finish = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1973_112_2_18_2_13_49_66,(uint8*)"d",s_dm_info.is_download_finish);
    return s_dm_info.is_download_finish;
}
/*****************************************************************************/
// Description : set nv handset lock state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetNVHandsetLock(BOOLEAN state)
{
    s_dm_info.handset_lock = state;
    MMINV_WRITE(MMINV_DM_HANDSET_LOCK, &s_dm_info.handset_lock);
}

/*****************************************************************************/
// Description :get  handset lock state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetHandsetLock(void)
{
    //SCI_TRACE_LOW:"MMIDM==> s_dm_info.handset_lock = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_1992_112_2_18_2_13_49_67,(uint8*)"d",s_dm_info.handset_lock);
    return s_dm_info.handset_lock;
}

/*****************************************************************************/
// Description : set log state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetLogState(BOOLEAN mode)
{
    s_dm_debug_info.log_switch = mode;
}
/*****************************************************************************/
// Description :DM get log state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetLogState(void)
{
    //SCI_TRACE_LOW:"MMIDM==> s_dm_debug_info.log_switch = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2010_112_2_18_2_13_49_68,(uint8*)"d",s_dm_debug_info.log_switch);
    return s_dm_debug_info.log_switch;
}

/*****************************************************************************/
// Description : set fota state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetFotaState(BOOLEAN mode)
{
    s_dm_debug_info.fota_switch = mode;
}
/*****************************************************************************/
// Description :DM get fota state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetFotaState(void)
{
    //SCI_TRACE_LOW:"MMIDM==> s_dm_debug_info.fota_switch = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2028_112_2_18_2_13_49_69,(uint8*)"d",s_dm_debug_info.fota_switch);
    return s_dm_debug_info.fota_switch;
}
/*****************************************************************************/
// Description : set tree state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetTreeState(BOOLEAN mode)
{
    s_dm_debug_info.tree_switch = mode;
}
/*****************************************************************************/
// Description :DM get tree state
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetTreeState(void)
{
    //SCI_TRACE_LOW:"MMIDM==> s_dm_debug_info.tree_switch = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2045_112_2_18_2_13_49_70,(uint8*)"d",s_dm_debug_info.tree_switch);
    return s_dm_debug_info.tree_switch;
}

/*****************************************************************************/
// Description :get id asdec type
// Global resource dependence:
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetIdasdecType(void)
{
    //SCI_TRACE_LOW:"MMIDM==> idasdec_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2055_112_2_18_2_13_49_71,(uint8*)"d",s_server_type_tab[s_dm_info.server_type].IdasdecType);
    return s_server_type_tab[s_dm_info.server_type].IdasdecType;
}

/*****************************************************************************/
// Description : This function returns the regional language. 
// return String that holds the language the handset is configured. 
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC char *MMIDM_GetLanguage(void)
{
    return DM_LANG_US_EN;
}

/*****************************************************************************/
// Description : This function retrieves the device type from Non-volatile memory.
//          Possible values are PDA, pager, or phone. 
// return String that holds the device type. 
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC char *MMIDM_GetDeviceType(void)
{
    return s_dm_info.device_type_info;
}

/*****************************************************************************/
// Description : This function retrieves the OEM identifier from Non-volatile memory.
// return String that holds the OEM identifier. 
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC char *MMIDM_GetOEM(void)
{
    return s_dm_info.oem_info;
}

/*****************************************************************************/
// Description : This function retrieves the firmware version information from
//          Non-volatile memory.
// return String that holds the firmware version information. 
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC char * MMIDM_GetFirmwareVersion(void)
{
    return s_dm_info.firmwareversion;
}


/*****************************************************************************/
// Description : This function retrieves the device Hardware version information
//          from Non-volatile memory.
// return String that holds the Hardware version information. 
// Author:jian.ma
// Note: 
/*****************************************************************************/
PUBLIC char *MMIDM_GetHardwareVersion(void)
{
    return s_dm_info.hardwareversion;
}

/*****************************************************************************/
//  Description : get device imei str
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC  char* DM_GetDevImeiInfo(void)
{
    return(s_server_type_tab[s_dm_info.server_type].imei);
}

/*****************************************************************************/
//  Description : get dm profile
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC char* MMIDM_GetDmProfile(void)
{
    //SCI_TRACE_LOW:"MMIDM==>MMIDM_GetDmProfile"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2123_112_2_18_2_13_49_72,(uint8*)"");
    return s_dm_info.apn_info;
}

/*****************************************************************************/
//  Description : set dm profile
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_SetDmProfile( char* info,uint32 length)
{
    BOOLEAN  result = FALSE;
    uint16 len = 0;

    if (MMIDM_APN_MAX_LEN > length && PNULL !=info)
    {
        SCI_MEMSET(s_dm_info.apn_info,0,MMIDM_APN_MAX_LEN);
        len = MIN(MMIDM_APN_MAX_LEN,length);
        SCI_MEMCPY(s_dm_info.apn_info,info,len);
        result = TRUE;
    }
    else
    {
        SCI_MEMSET(s_dm_info.apn_info,0,MMIDM_APN_MAX_LEN);
        result = TRUE;
    }
    //SCI_TRACE_LOW:"MMIDM==>MMIDM_SetDmProfile length = %d result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2148_112_2_18_2_13_49_73,(uint8*)"dd",length,result);
    
    return result;
}
/*****************************************************************************/
//  Description : get connet info
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
LOCAL MMICONNECTION_LINKSETTING_DETAIL_T * MMIDM_GetConnectInfo(uint32 index)
{
    MMICONNECTION_LINKSETTING_DETAIL_T *connect_datail_ptr = PNULL;
    uint32  max_num = MMIAPICONNECTION_GetLinkSettingNum(MMIDM_GetMainSim());

    if (max_num > index)
    {
        connect_datail_ptr =  MMIAPICONNECTION_GetLinkSettingItemByIndex(MMIDM_GetMainSim(),index);
    }
    
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetConnectInfo max_num = %d mmi_cmnet_connect_index =%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2166_112_2_18_2_13_49_74,(uint8*)"dd",max_num,index);

    return connect_datail_ptr;
}
/*****************************************************************************/
//  Description : get cmnet/cmwap info
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC char* MMIDM_GetGPRSInfo(DM_WAP_INFO_TYPE type,DM_CON_INFO_TYPE con_type)
{
    char* string = s_string_arr;
    MMICONNECTION_LINKSETTING_DETAIL_T *connect_datail_ptr = PNULL;
    uint32  mmi_cmnet_connect_index = 0;

    if (DM_CMNET == con_type)
    {
     mmi_cmnet_connect_index = 2;
    }
    else
    {
        mmi_cmnet_connect_index = 0;
    }
    connect_datail_ptr =  MMIDM_GetConnectInfo(mmi_cmnet_connect_index);
    
    //SCI_TRACE_LOW:"MMIDM==>MMIDM_GetGPRSInfo type = %d con_type=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2190_112_2_18_2_13_49_75,(uint8*)"dd",type,con_type);

    if(PNULL == connect_datail_ptr)
    {
        //SCI_TRACE_LOW:"MMIDM==>MMIDM_GetGPRSInfo type PNULL == connect_datail_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2194_112_2_18_2_13_49_76,(uint8*)"");
        return PNULL;
    }
    switch(type)
    {
        case DM_WAP_APN:
            if (0 != connect_datail_ptr->apn_len && strlen((char*)connect_datail_ptr->apn))
            {
                string = (char*)connect_datail_ptr->apn;
            }
            break;  
        case DM_WAP_USER_NAME:
            if (0 != connect_datail_ptr->username_len && strlen((char*)connect_datail_ptr->username))
            {
                string = (char*)connect_datail_ptr->username;
            }
            break;
        case DM_WAP_PW:
            if (0 != connect_datail_ptr->password_len && strlen((char*)connect_datail_ptr->password))
            {
                string = (char*)connect_datail_ptr->password;
            }
            break;
        case DM_WAP_PORT_NUM:
            if (0 != connect_datail_ptr->port)
            {
                SCI_MEMSET(s_port_num,0,sizeof(s_port_num));
                MMIDM_IU32toa(connect_datail_ptr->port,s_port_num,DM_RADIX_TYPE);
                string = s_port_num;
            }
            break;    
        case DM_WAP_HOME_PAGE:    
            if (0 != connect_datail_ptr->homepage_len && strlen((char*)connect_datail_ptr->homepage))
            {
                string = (char*)connect_datail_ptr->homepage;
            }
            break;
        case DM_WAP_PROXY_ADDR:
            if (0 != connect_datail_ptr->gateway_len && strlen((char*)connect_datail_ptr->gateway))
            {
                char gateway[]="0.0.0.0";
                if(0 != memcmp(gateway, (char*)connect_datail_ptr->gateway,connect_datail_ptr->gateway_len))
                {
                    string = (char*)connect_datail_ptr->gateway;
                }
            }
            break;
        default:
            //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetGPRSInfo type=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2242_112_2_18_2_13_49_77,(uint8*)"d",type);
            break;
    }
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetGPRSInfo string = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2245_112_2_18_2_13_49_78,(uint8*)"s",string);
    return string;
}

/*****************************************************************************/
//  Description : set cmnet/cmwap apn info
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_SetGprsInfo(DM_WAP_INFO_TYPE type,char* info,uint32 length,DM_CON_INFO_TYPE con_type)
{
    MMICONNECTION_LINKSETTING_DETAIL_T connection_datail={0};
    MMICONNECTION_LINKSETTING_DETAIL_T* connect_datail_ptr = PNULL;
    uint16 len = 0;
    uint32  mmi_cmnet_connect_index = 0;

    if (DM_CMNET == con_type)
    {
        mmi_cmnet_connect_index = 2;
    }
    else
    {
        mmi_cmnet_connect_index = 0;
    }
    connect_datail_ptr =  MMIDM_GetConnectInfo(mmi_cmnet_connect_index);
    if (PNULL == connect_datail_ptr)
    {
        //SCI_TRACE_LOW:"MMIDM==>MMIDM_SetGprsInfo type PNULL == connect_datail_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2271_112_2_18_2_13_49_79,(uint8*)"");
        return FALSE;    
    }
    SCI_MEMCPY(&connection_datail,connect_datail_ptr,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));

    //SCI_TRACE_LOW:"MMIDM==>MMIDM_SetGprsInfo type = %d con_type = %d,len=%d,info=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2276_112_2_18_2_13_49_80,(uint8*)"ddds",type,con_type,length,info);

    switch(type)
    {
        case DM_WAP_APN:
            SCI_MEMSET(connection_datail.apn,0,MMICONNECTION_MAX_APN_LEN);
            len = MIN(MMICONNECTION_MAX_APN_LEN,length);
            connection_datail.apn_len = len;
            if(0!=len && PNULL!=info)
            {
                SCI_MEMCPY(connection_datail.apn,info,len);
            }
            break; 
        case DM_WAP_USER_NAME:
            SCI_MEMSET(connection_datail.username,0,MMICONNECTION_MAX_USERNAME_LEN);
            len = MIN(MMICONNECTION_MAX_USERNAME_LEN,length);
            connection_datail.username_len = len;
            if(0!=len && PNULL!=info)
            {
                SCI_MEMCPY(connection_datail.username,info,len);
            }
            break;
        case DM_WAP_PW:
            SCI_MEMSET(connection_datail.password,0,MMICONNECTION_MAX_PASSWORD_LEN);
            len = MIN(MMICONNECTION_MAX_PASSWORD_LEN,length);
            connection_datail.password_len = len;
            if(0!=len && PNULL!=info)
            {
                SCI_MEMCPY(connection_datail.password,info,len);
            }
            break;
        case DM_WAP_PORT_NUM:
            if(PNULL!=info)
            {
                connection_datail.port = atoi(info);
            }
            else
            {
                connection_datail.port= 0;
            }
            break;    
        case DM_WAP_HOME_PAGE: 
            SCI_MEMSET(connection_datail.homepage,0,MMICONNECTION_HOMEPAGE_MAX_URL_LEN);
            len = MIN(MMICONNECTION_HOMEPAGE_MAX_URL_LEN,length);
            connection_datail.homepage_len = len;
            if(0!=len && PNULL!=info)
            {
                SCI_MEMCPY(connection_datail.homepage,info,len);
            }
            break;
        case DM_WAP_PROXY_ADDR:
            SCI_MEMSET(connection_datail.gateway,0,MMICONNECTION_MAX_GATEWAY_LEN);
            len = MIN(MMICONNECTION_MAX_GATEWAY_LEN,length);
            connection_datail.gateway_len = len;
            if(0!=len && PNULL!=info)
            {
                SCI_MEMCPY(connection_datail.gateway,info,len);
            }
            break;
        default:
            //SCI_TRACE_LOW:"MMIDM_SetGprsInfo type=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2336_112_2_18_2_13_49_81,(uint8*)"d",type);
            break;
    }

    return MMIAPICONNECTION_ReplaceItem(MMIDM_GetMainSim(),mmi_cmnet_connect_index,&connection_datail);

}
/*****************************************************************************/
//  Description : get Get dm server url
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC char* MMIDM_GetServerUrl(void)
{
    return s_dm_info.server_url;
}

/*****************************************************************************/
//  Description : get wap info
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC char* DM_GetWAPInfo(DM_WAP_INFO_TYPE type)
{

    char* string = s_string_arr;
    uint32 index = MMIDM_GetNetSettingIndex();
    MMICONNECTION_LINKSETTING_DETAIL_T *connect_datail_ptr = PNULL;

    if (MMIAPICONNECTION_GetLinkSettingNum(MMIDM_GetMainSim()) <= index)
    {
        return PNULL;
    }
    connect_datail_ptr =  MMIAPICONNECTION_GetLinkSettingItemByIndex(MMIDM_GetMainSim(), index);

    //SCI_TRACE_LOW:"MMIDM==>DM_GetWAPInfo type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2369_112_2_18_2_13_49_82,(uint8*)"d",type);

    if(PNULL == connect_datail_ptr)
    {
        //SCI_TRACE_LOW:"MMIDM==>DM_GetWAPInfo type PNULL == connect_datail_ptr"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2373_112_2_18_2_13_49_83,(uint8*)"");
        return PNULL;
    }
    switch(type)
    {
        case DM_WAP_APN:
            if (0 != connect_datail_ptr->apn_len && strlen((char*)connect_datail_ptr->apn))
            {
                string = (char*)connect_datail_ptr->apn;
            }
            break;  
        case DM_WAP_USER_NAME:
            if (0 != connect_datail_ptr->username_len && strlen((char*)connect_datail_ptr->username))
            {
                string = (char*)connect_datail_ptr->username;
            }
            break;
        case DM_WAP_PW:
            if (0 != connect_datail_ptr->password_len && strlen((char*)connect_datail_ptr->password))
            {
                string = (char*)connect_datail_ptr->password;
            }
            break;
        case DM_WAP_PORT_NUM:
            SCI_MEMSET(s_port_num,0,sizeof(s_port_num));
            MMIDM_IU32toa(connect_datail_ptr->port,s_port_num,DM_RADIX_TYPE);
            string = s_port_num;
            break;    
        case DM_WAP_HOME_PAGE:    
            if (0 != connect_datail_ptr->homepage_len && strlen((char*)connect_datail_ptr->homepage))
            {
                string = (char*)connect_datail_ptr->homepage;
            }
            break;
        case DM_WAP_PROXY_ADDR:
            if (0 != connect_datail_ptr->gateway_len && strlen((char*)connect_datail_ptr->gateway))
            {
                string = (char*)connect_datail_ptr->gateway;
            }
            break;
        default:
            //SCI_TRACE_LOW:"MMIDM==> DM_GetWAPInfo type=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2414_112_2_18_2_13_50_84,(uint8*)"d",type);
            break;
    }
    //SCI_TRACE_LOW:"MMIDM==> DM_GetWAPInfo string = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2417_112_2_18_2_13_50_85,(uint8*)"s",string);
    return string;
}

/*****************************************************************************/
//  Description : set wap info
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN DM_SetWAPInfo(DM_WAP_INFO_TYPE type, char* info,uint32 length)
{
    MMICONNECTION_LINKSETTING_DETAIL_T connection_datail={0};
    MMICONNECTION_LINKSETTING_DETAIL_T *connect_datail_ptr = PNULL;
    uint16  len = 0;

    //SCI_TRACE_LOW:"MMIDM==>DM_SetWAPInfo type = %d info = %s length=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2431_112_2_18_2_13_50_86,(uint8*)"dsd",type,info ,length);

    connect_datail_ptr =  MMIAPICONNECTION_GetLinkSettingItemByIndex(MMIDM_GetMainSim(), MMIDM_GetNetSettingIndex());
    SCI_MEMCPY(&connection_datail,connect_datail_ptr,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));

    switch(type)
    {
        case DM_WAP_APN:
            SCI_MEMSET(connection_datail.apn,0,MMICONNECTION_MAX_APN_LEN);
            len = MIN(MMICONNECTION_MAX_APN_LEN,length);
            connection_datail.apn_len = len;
            if(0!=len && PNULL!=info)
            {
                SCI_MEMCPY(connection_datail.apn,info,len);
            }
            break; 
        case DM_WAP_USER_NAME:
            SCI_MEMSET(connection_datail.username,0,MMICONNECTION_MAX_USERNAME_LEN);
            len = MIN(MMICONNECTION_MAX_USERNAME_LEN,length);
            connection_datail.username_len = len;
            if(0!=len && PNULL!=info)
            {
                SCI_MEMCPY(connection_datail.username,info,len);
            }
            break;
        case DM_WAP_PW:
            SCI_MEMSET(connection_datail.password,0,MMICONNECTION_MAX_PASSWORD_LEN);
            len = MIN(MMICONNECTION_MAX_PASSWORD_LEN,length);
            connection_datail.password_len = len;
            if(0!=len && PNULL!=info)
            {
                SCI_MEMCPY(connection_datail.password,info,len);
            }
            break;
        case DM_WAP_PORT_NUM:
            if(PNULL!=info)
            {
                connection_datail.port = atoi(info);
            }
            else
            {
                connection_datail.port = 0;
            }
            break;    
        case DM_WAP_HOME_PAGE:  
            SCI_MEMSET(connection_datail.homepage,0,MMICONNECTION_HOMEPAGE_MAX_URL_LEN);
            len = MIN(MMICONNECTION_HOMEPAGE_MAX_URL_LEN,length);
            connection_datail.homepage_len = len;
            if(0!=len && PNULL!=info)
            {
                SCI_MEMCPY(connection_datail.homepage,info,len);
            }
            break;
        case DM_WAP_PROXY_ADDR:
            SCI_MEMSET(connection_datail.gateway,0,MMICONNECTION_MAX_GATEWAY_LEN);
            len = MIN(MMICONNECTION_MAX_GATEWAY_LEN,length);
            connection_datail.gateway_len = len;
            if(0!=len && PNULL!=info)
            {
                SCI_MEMCPY(connection_datail.gateway,info,len);
            }
            break;
        default:
            //SCI_TRACE_LOW:"MMIDM==> DM_SetWAPInfo type=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2494_112_2_18_2_13_50_87,(uint8*)"d",type);
            break;
    }

    return MMIAPICONNECTION_ReplaceItem(MMIDM_GetMainSim(), MMIDM_GetNetSettingIndex(),&connection_datail);
}
/*****************************************************************************/
//  Description : get MMSC Info
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC char* MMIDM_GetMMSCInfo(DM_MMS_INFO_TYPE type)
{    
    char* string = s_string_arr;
    
    MMICONNECTION_LINKSETTING_DETAIL_T *connect_datail_ptr = PNULL;
    connect_datail_ptr =  MMIAPICONNECTION_GetLinkSettingItemByIndex(MMIDM_GetMainSim(), MMIDM_GetMmsSettingIndex());

    //SCI_TRACE_LOW:"MMIDM==>MMIDM_GetMMSCInfo type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2511_112_2_18_2_13_50_88,(uint8*)"d",type);
    if(PNULL == connect_datail_ptr)
    {
        return PNULL;
    }
    switch(type) 
    {
    case DM_MMS_MMSC:
        if (strlen((char*)connect_datail_ptr->homepage))
        {
            string = (char*)connect_datail_ptr->homepage;
        }
        break;
    case DM_MMS_CONN_PROFILE:
        if (strlen((char*)connect_datail_ptr->apn))
        {
            string = (char*)connect_datail_ptr->apn;
        }
        break;
    default:
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetMMSCInfo type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2531_112_2_18_2_13_50_89,(uint8*)"d",type);
        break;
    }
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetMMSCInfo string = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2534_112_2_18_2_13_50_90,(uint8*)"s",string);

    return string;
}

/*****************************************************************************/
//  Description : set mmsc info
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_SetMMSCInfo(DM_MMS_INFO_TYPE type,char* info,uint32 length)
{    
    MMICONNECTION_LINKSETTING_DETAIL_T connection_datail={0};
    MMICONNECTION_LINKSETTING_DETAIL_T* connect_datail_ptr = PNULL;
    uint16      len = 0;

    connect_datail_ptr =  MMIAPICONNECTION_GetLinkSettingItemByIndex(MMIDM_GetMainSim(),MMIDM_GetMmsSettingIndex());

    SCI_MEMCPY(&connection_datail,connect_datail_ptr,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));

    //SCI_TRACE_LOW:"MMIDM==>MMIDM_SetMMSCInfo type = %d len=%d,info=%s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2553_112_2_18_2_13_50_91,(uint8*)"dds",type,length,info);

    switch(type) 
    {
    case DM_MMS_MMSC:
        SCI_MEMSET(connection_datail.homepage,0,MMICONNECTION_HOMEPAGE_MAX_URL_LEN);
        len = MIN(MMICONNECTION_HOMEPAGE_MAX_URL_LEN,length);
        connection_datail.homepage_len = len;
        if(0!=len && PNULL!=info)
        {
            SCI_MEMCPY(connection_datail.homepage,info,len);
        }
        break;
    case DM_MMS_CONN_PROFILE:
        SCI_MEMSET(connection_datail.apn,0,MMICONNECTION_MAX_APN_LEN);
        len = MIN(MMICONNECTION_MAX_APN_LEN,length);
        connection_datail.apn_len = len;
        if(0!=len && PNULL!=info)
        {
            SCI_MEMCPY(connection_datail.apn,info,len);
        }
        break;
    default:
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_SetMMSCInfo type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2576_112_2_18_2_13_50_92,(uint8*)"d",type);
        break;
    }
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_SetMMSCInfo string = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2579_112_2_18_2_13_50_93,(uint8*)"s",info);
    return MMIAPICONNECTION_ReplaceItem(MMIDM_GetMainSim(), MMIDM_GetMmsSettingIndex(),&connection_datail);
}
/*****************************************************************************/
//  Description : get PIM info 
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_GetPIMInfo(DM_PIM_INFO_TYPE type, char *str_ptr, uint16 max_count)
{
    //char* string = s_string_arr;
    char* string = PNULL;/*lint !e831*/
    uint16  str_len = 0;
#ifdef PIM_SUPPORT
    server_setting_info_type  server_setting={0};

    //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetPIMInfo type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2592_112_2_18_2_13_50_94,(uint8*)"d",type);
    MMIAPIPIM_GetServerConfig(&server_setting);

    switch(type) 
    {
    case DM_PIM_ADDR:
        if (strlen(server_setting.addr))
        {
            string = server_setting.addr;
        }
        break;
    case DM_PIM_ADDRESSBOOKURL:
        if (strlen(server_setting.pbdb))
        {
            string = server_setting.pbdb;
        }
        break;
    case DM_PIM_CALENDARURL:
        if (strlen(server_setting.cddb))
        {
            string = server_setting.cddb;
        }
        break;
    default:
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetPIMInfo type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2616_112_2_18_2_13_50_95,(uint8*)"d",type);
        break;
    }
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetPIMInfo string = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2619_112_2_18_2_13_50_96,(uint8*)"s",string);
#endif

    if (PNULL != string)/*lint !e831 !e774*/
    {
        str_len = (uint16)SCI_STRLEN(string);
        str_len = MIN(max_count, str_len);
        SCI_MEMCPY(str_ptr, string, str_len);
        SCI_TRACE_LOW("MMIDM==> MMIDM_GetPIMInfo string = %s",string);
    }
    else
    {
        SCI_TRACE_LOW("MMIDM==> MMIDM_GetPIMInfo string == PNULL");
    }
}

/*****************************************************************************/
//  Description : set PIM info 
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_SetPIMInfo(DM_PIM_INFO_TYPE type,char* info,uint32 length)
{
#ifdef PIM_SUPPORT
    server_setting_info_type  server_setting={0};
    uint32 len = 0;

    //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetPIMInfo type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2634_112_2_18_2_13_50_97,(uint8*)"d",type);
    MMIAPIPIM_GetServerConfig(&server_setting);
    switch(type) 
    {
    case DM_PIM_ADDR:
        SCI_MEMSET(server_setting.addr,0,MMIPIM_MAX_URL_LEN);
        len = MIN(MMIPIM_MAX_URL_LEN,length);
        if(0!=len && PNULL!=info)
        {
            SCI_MEMCPY(server_setting.addr,info,len);
        }
        break;
    case DM_PIM_ADDRESSBOOKURL:
        SCI_MEMSET(server_setting.pbdb,0,MMIPIM_MAX_DATABASE_LEN);
        len = MIN(MMIPIM_MAX_DATABASE_LEN,length);
        if(0!=len && PNULL!=info)
        {
            SCI_MEMCPY(server_setting.pbdb,info,len);
        }
        break;
    case DM_PIM_CALENDARURL:
        SCI_MEMSET(server_setting.cddb,0,MMIPIM_MAX_DATABASE_LEN);
        len = MIN(MMIPIM_MAX_DATABASE_LEN,length);
        if(0!=len && PNULL!=info)
        {
            SCI_MEMCPY(server_setting.cddb,info,len);
        }
        break;
    default:
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_SetPIMInfo type=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2663_112_2_18_2_13_50_98,(uint8*)"d",type);
        break;
     }

    MMIAPIPIM_SetServerConfig(&server_setting);
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_SetPIMInfo string = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2668_112_2_18_2_13_50_99,(uint8*)"s",info);
#endif
    return TRUE;
}
/*****************************************************************************/
//  Description : get streaming info 
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC char* MMIDM_GetStreamingInfo(DM_STREAMING_INFO_TYPE type)
{
    char* string = s_string_arr;
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    VP_NV_SETTING_T streaming_info = {0};
    uint16  index = 0;
    MMICONNECTION_LINKSETTING_DETAIL_T* connect_datail_ptr = PNULL;
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetStreamingInfo type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2683_112_2_18_2_13_50_100,(uint8*)"d",type);
    streaming_info = MMIAPIVP_GetNVSettingInfo();
    switch(type) 
    {
        case DM_ST_NAME:
            string = "Streaming";
            break;
        case DM_ST_APN:
            index = streaming_info.net_index[MMIDM_GetMainSim()];
            connect_datail_ptr =  MMIAPICONNECTION_GetLinkSettingItemByIndex(MMIDM_GetMainSim(),index);
            if (strlen((char*)connect_datail_ptr->apn))
            {
                string = (char*)connect_datail_ptr->apn;
            }
            break;
        case DM_ST_NETINFO:
            string = "EGPRS,10,100";
            break;
        case DM_ST_PROXY:
            if (strlen((char*)streaming_info.agent_ip))
            {
                SCI_MEMSET(s_st_agent_ip,0,VP_MAX_IP_LENGTH);
                SCI_MEMCPY(s_st_agent_ip,streaming_info.agent_ip,VP_MAX_IP_LENGTH);
                string = (char*)s_st_agent_ip;
            }
            break;
        case DM_ST_PROXYPORT:
            SCI_MEMSET(s_port_num,0,sizeof(s_port_num));
            MMIDM_IU32toa(streaming_info.agent_port,s_port_num,DM_RADIX_TYPE);
            string = s_port_num;
            break;
        case DM_ST_MIN_UDP_PORT:
            SCI_MEMSET(s_port_num,0,sizeof(s_port_num));
            MMIDM_IU32toa(streaming_info.lowest_port,s_port_num,DM_RADIX_TYPE);
            string = s_port_num;
            break;
        case DM_ST_MAX_UDP_PORT:
            SCI_MEMSET(s_port_num,0,sizeof(s_port_num));
            MMIDM_IU32toa(streaming_info.highest_port,s_port_num,DM_RADIX_TYPE);
            string = s_port_num;
            break;
        default:
            //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetStreamingInfo type=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2725_112_2_18_2_13_50_101,(uint8*)"d",type);
            break;
    }
#endif
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetStreamingInfo string = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2729_112_2_18_2_13_50_102,(uint8*)"s",string);
    return string;
}

/*****************************************************************************/
//  Description : set streaming info 
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_SetStreamingInfo(DM_STREAMING_INFO_TYPE type,char* info,uint32 length)
{
    uint32 len = 0;
#ifdef MMI_STREAMING_PLAYER_SUPPORT
    VP_NV_SETTING_T streaming_info = {0};
    MMICONNECTION_LINKSETTING_DETAIL_T* connect_datail_ptr = PNULL;
    MMICONNECTION_LINKSETTING_DETAIL_T connection_datail= {0};
    uint16  index = 0;

    //SCI_TRACE_LOW:"MMIDM==> MMIDM_SetStreamingInfo type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2746_112_2_18_2_13_50_103,(uint8*)"d",type);

    streaming_info = MMIAPIVP_GetNVSettingInfo();
    switch(type) 
    {
        case DM_ST_NAME:
            //no support
            break;
        case DM_ST_PROXY:
            SCI_MEMSET(streaming_info.agent_ip,0,VP_MAX_IP_LENGTH);
            len = MIN(VP_MAX_IP_LENGTH,length);
            streaming_info.agent_ip_length = len;
            if(0!=len && PNULL!=info)
            {
                SCI_MEMCPY(streaming_info.agent_ip,info,len); 
            }
            break;
        case DM_ST_PROXYPORT:
            if(PNULL!=info)
            {
                streaming_info.agent_port = atoi(info);
            }
            else
            {
                streaming_info.agent_port  = 0;
            }
            break;
        case DM_ST_APN:
            index = streaming_info.net_index[MMIDM_GetMainSim()];
            connect_datail_ptr =  MMIAPICONNECTION_GetLinkSettingItemByIndex(MMIDM_GetMainSim(),index);
            SCI_MEMCPY(&connection_datail,connect_datail_ptr,sizeof(MMICONNECTION_LINKSETTING_DETAIL_T));
            SCI_MEMSET(connection_datail.apn,0,MMICONNECTION_MAX_APN_LEN);
            len = MIN(MMICONNECTION_MAX_APN_LEN,length);
            connection_datail.apn_len = len;
            if(0!=len && PNULL!=info)
            {
                SCI_MEMCPY(connection_datail.apn,info,len);
            }
            //SCI_TRACE_LOW:"MMIDM==> MMIDM_SetStreamingInfo string = %s"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2784_112_2_18_2_13_50_104,(uint8*)"s",info);
            return MMIAPICONNECTION_ReplaceItem(MMIDM_GetMainSim(), index,&connection_datail);

        case DM_ST_NETINFO:
            //no support
            break;
        case DM_ST_MIN_UDP_PORT:
            if(PNULL!=info)
            {
                streaming_info.lowest_port = atoi(info);
            }
            else
            {
                streaming_info.lowest_port = 0;
            }
            break;
        case DM_ST_MAX_UDP_PORT:
            if(PNULL!=info)
            {
                streaming_info.highest_port = atoi(info);
            }
            else
            {
                streaming_info.highest_port = 0;
            }
            break;
        default:
            //SCI_TRACE_LOW:"MMIDM==> MMIDM_SetStreamingInfo type=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2811_112_2_18_2_13_50_105,(uint8*)"d",type);
            break;
     }
    
        MMIAPIVP_SetNVSettingInfo(streaming_info);
#endif
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_SetStreamingInfo string = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2817_112_2_18_2_13_50_106,(uint8*)"s",info);
    return TRUE;
}

/*****************************************************************************/
//  Description : get srvadd URL
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC char* MMIDM_GetSrvAddURL(void)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetSrvAddURL srv_addr = %s"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2827_112_2_18_2_13_50_107,(uint8*)"s",s_dm_info.srv_addr);
    return s_dm_info.srv_addr;
}


#ifdef DM_SUPPORT_DFRESH
/*****************************************************************************/
//  Description : get LOCK STATUS CODE
//  Global resource dependence : none
//  Author: mary.xiao 
//  Note:
/*****************************************************************************/
PUBLIC char* MMIDM_GetLockStatusCode(void)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetLockStatusCode status"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2839_112_2_18_2_13_50_108,(uint8*)"");
    if(MMIDM_GetHandsetLock())
    {
        return DM_LOCK_CODE;
    }
    else
    {
        return DM_UNLOCK_CODE;
    }
}
#endif
/*****************************************************************************/
//  Description : handle dm message
//  Global resource dependence : none
//  Author:jian.ma 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_HandleDmMessage(DPARAM     sig_data_ptr)
{
    MmiAppDMMsgS *dm_sig = PNULL;
    BOOLEAN result = TRUE;

    if (PNULL == sig_data_ptr)
    {
        return FALSE;
    }

    dm_sig = (MmiAppDMMsgS*)sig_data_ptr;

    //SCI_TRACE_LOW:"MMIDM==> MMIDM_HandleDmMessage 0x%x"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2866_112_2_18_2_13_50_109,(uint8*)"d",dm_sig->event_id);

    switch(dm_sig->event_id) 
    {
        case MSG_DM_DWNLD_PROGRESS:
            MMIDM_ProgressUpdateCallBack((MMIDM_DOWNLOAD_PROCESS_T*)dm_sig->param);
            break;
        case MSG_DM_LOCK_DEVICE:
            MMIDM_ServerLockCallBack();
            break;
        case MSG_DM_UNLOCK_DEVICE:
            MMIDM_ServerUnlockCallBack();
            break;
        case MSG_DM_SESSION_CHANGE:
            MMIDM_SessionChangeCallBack((MMIDM_SESSION_CHANGE_T*)dm_sig->param);
            break;
        case MSG_DM_NOTIFY_ALERT:
            MMIDM_NotifyAlertCallBack((MMIDM_NOTIFY_ALERT_T*)dm_sig->param);
            break;
        case MSG_DM_SESSION_EXIT:
            //SCI_TRACE_LOW:"MMIDM==> MSG_DM_SESSION_EXIT "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2886_112_2_18_2_13_50_110,(uint8*)"");
            s_is_dm_run = FALSE;
            MMIDM_StopDmRunCheckTimer();
            MMIDM_CloseAllWin();
            if (MMIDM_IsNeedHandleDelayEvent())
            {
                MMIDM_ReStartDmSession();
                MMIDM_SetNeedHandleDelayEvent(FALSE);
            }
            break;
        default:
            //SCI_TRACE_LOW:"MMIDM==> MMIDM_HandleDmMessage dm_sig->event_id=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2897_112_2_18_2_13_51_111,(uint8*)"d",dm_sig->event_id);
            break;
    }

    return result;
}


/*****************************************************************************/
//  Description : reset dm factory setting
//  Global resource dependence : none
//  Author:jianhui.luo 
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_ResetFactorySetting(void)
{    
    //int32 i = 0;
    MMICL_CALL_ARRAY_BCDINFO_T *call_ptr = PNULL;
    uint32 missed_call_num = 0;

    //SCI_TRACE_LOW:"MMIDM==> MMIDM_ResetFactorySetting "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2915_112_2_18_2_13_51_112,(uint8*)"");
    //MMISET_CleanUserData();
    if (MMIAPIFMM_GetDeviceStatus(MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK)))
    {
        MMIAPIFMM_FormatDevice((uint16 *)MMIAPIFMM_GetDevicePath(MMI_DEVICE_UDISK), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_UDISK), MMIFILE_FORMAT_AUTO);
    }
    
    MMIAPIMMS_DelUserInfo();
    
#ifdef BROWSER_SUPPORT
    MMIAPIBROWSER_CleanUserInfo();
#endif
    MMINV_WRITE(MMI_CC_MISSED_CALL_NUM, &missed_call_num);

    MMIAPICL_ClearCalllogData();
    MMIAPIPB_DeleteAllPBInfo();
    

#ifndef WIN32
    MMISMS_NV_DelAllSmsItem(MMINV_MAX_SMS_RECORDS);
    EFS_NvitemDelete(NV_SMS_MESSAGE_ITEM,NV_SMS_MESSAGE_ITEM_LAST_ENTRY); /*lint !e718*/ //delete sms in phone
    EFS_NvitemDelete(NV_PHONE_BOOK_MAIN,NV_PHONE_BOOK_LND_LAST_ENTRY);/*lint !e718*/  //delete phonebook information in phone
#endif
#endif
    //MMIAPISET_SetNeedResetDateTime(TRUE);
    MMIAPISMS_SetDefaultPresetSms(TRUE);
#ifdef PIM_SUPPORT
    MMIPB_UpdateAllPimStatus(PB_NO_MODIFY);
#endif

}


/*****************************************************************************/
//  Description : Get dev free space
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIDM_GetDevFreeSpace(void)
{
    uint32       free_space_low  = 0;
    uint32       free_space_high = 0;
    uint32       free_size = 0;

    MMIAPIFMM_GetDeviceFreeSpace(MMIAPIFMM_GetDevicePath(MMI_DEVICE_SYSTEM), MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SYSTEM),&free_space_high, &free_space_low); 
    if (0 == free_space_high)
    {
        free_size = free_space_low;
    }
    else
    {
        free_size = DM_CLIENT_PL_MAX_DL_PACKAGE_SIZE;
    }

    //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetDevFreeSpace free size = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_2982_112_2_18_2_13_51_114,(uint8*)"d",free_size);
    return free_size;
}

/*****************************************************************************/
//  Description : check dl space enouge
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIDM_CheckIsDlSpaceEnouge(void)
{
    BOOLEAN result = FALSE;
    if (DM_CLIENT_PL_MAX_DL_PACKAGE_SIZE <= MMIDM_GetDevFreeSpace())
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_CheckDlSpaceEnouge result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3001_112_2_18_2_13_51_115,(uint8*)"d",result);
    return result;
}

/*****************************************************************************/
//  Description : install update package
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_InstallUpdatePackage(void)
{
    BOOLEAN result = FALSE;

    result = LoadFileToImage();
    if (result)
    {
       MMIDM_SetNVDownloadFinish(FALSE);
       MMIDM_DeleteAllResumeFile();
       //SCI_TRACE_LOW:"MMIDM==> MMIDM_InstallUpdatePackage succ!"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3018_112_2_18_2_13_51_116,(uint8*)"");
    }
    else
    {
       //SCI_TRACE_LOW:"MMIDM==> MMIDM_InstallUpdatePackage fail!"
       SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3022_112_2_18_2_13_51_117,(uint8*)"");
    }

    return result;
}

/*****************************************************************************/
//  Description : check update package exist
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_CheckUpdateFile(void)
{
    BOOLEAN                 result = FALSE;
    wchar                   *filename_ptr = PNULL;
    wchar                   full_path_arr[MMIFILE_FULL_PATH_MAX_LEN+ 1] = {0};
    uint16                  full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    uint32                       str_len = (uint32)strlen(DM_FUMO_UPDATE_FILE)+1;

    filename_ptr = SCI_ALLOCAZ(str_len * sizeof(wchar));
    if (PNULL == filename_ptr)
    {
        return result;
    }
    SCI_MEMSET(filename_ptr,0,(str_len * sizeof(wchar)));

    //SCI_TRACE_LOW:"MMIDM==> MMIDM_CheckUpdateFile "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3047_112_2_18_2_13_51_118,(uint8*)"");
    
    MMIAPICOM_StrToWstr((uint8*)DM_FUMO_UPDATE_FILE,filename_ptr);
    
    if (MMIDM_GetFileFullPath(filename_ptr,(uint16)MMIAPICOM_Wstrlen(filename_ptr),full_path_arr,&full_path_len))
    {
        if(MMIAPIFMM_IsFileExist(full_path_arr,full_path_len))
        {
            result = TRUE;
        }
    }
    SCI_FREE(filename_ptr);
    return result;
}
/*****************************************************************************/
//  Description : load file to Image
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN LoadFileToImage(void)
{
    BOOLEAN                 result = FALSE;

    wchar                   *filename_ptr = PNULL;
    wchar                   full_path_arr[MMIFILE_FULL_PATH_MAX_LEN+ 1] = {0};
    uint16                  full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    SFS_HANDLE              sfs_handle = NULL;
    uint32                  to_read_size = 0;
    uint32                  bytes_read = 0;
    static uint32            s_offset = 0;
    uint8                   * buffer = PNULL;
    DM_FOTA_ERROR_E            fota_error = DM_FOTA_ERR_NONE;
    uint32                  fota_size = 0;
    uint32                  file_size = 0;
    SFS_ERROR_E             error = SFS_NO_ERROR;
   uint32                       str_len= 0;
    
    if (MMIDM_GetFotaState())
    {
        filename_ptr = SCI_ALLOCA((strlen(DM_FUMO_UPDATE_FILE)+1) * sizeof(wchar));
        if(PNULL == filename_ptr)
        {
            SCI_TRACE_LOW("[mmidm] LoadFileToImage PNULL == filename_ptr");
            return FALSE;
        }
        SCI_MEMSET(filename_ptr, 0X0, (strlen(DM_FUMO_UPDATE_FILE)+1) * sizeof(wchar));/*lint !e666*/
	str_len = (uint32)strlen(DM_FUMO_UPDATE_FILE)+1;
        SCI_MEMSET(filename_ptr,0,(str_len * sizeof(wchar)));

        //SCI_TRACE_LOW:"MMIDM==> LoadFileToImage start"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3089_112_2_18_2_13_51_119,(uint8*)"");
        MMIAPICOM_StrToWstr((uint8*)DM_FUMO_UPDATE_FILE,filename_ptr);
    
        if (!MMIDM_GetFileFullPath(filename_ptr,(uint16)MMIAPICOM_Wstrlen(filename_ptr),full_path_arr,&full_path_len))
        {
            return FALSE;
        }

        if(MMIAPIFMM_IsFileExist(full_path_arr,full_path_len))
        {
            //SCI_TRACE_LOW:"MMIDM==> LoadFileToImage MMIAPIFMM_IsFileExist"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3099_112_2_18_2_13_51_120,(uint8*)"");
            //open fumo file
            sfs_handle = MMIAPIFMM_CreateFile(full_path_arr,SFS_MODE_READ| SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS,PNULL,PNULL);
            //open Fota image area
            fota_error = MMIDM_FOTA_ImageArea_Open();
            //SCI_TRACE_LOW:"MMIDM==> LoadFileToImage sfs_handle=%d,fota_error=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3104_112_2_18_2_13_51_121,(uint8*)"dd",sfs_handle,fota_error);
            if (0 == sfs_handle ||DM_FOTA_ERR_NONE != fota_error)
            {
                SCI_FREE(filename_ptr);
                MMIDM_FOTA_ImageArea_Close();
                return FALSE;
            }
            SFS_GetFileSize(sfs_handle, &file_size);
            
            //SCI_TRACE_LOW:"MMIDM==> SFS_GetFileSize file_size=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3113_112_2_18_2_13_51_122,(uint8*)"d",file_size);
            if (MMIDM_FOTA_GetSpaceSize()<file_size)
            {
                //SCI_TRACE_LOW:"MMIDM==> LoadFileToImage FOTA_GetSpaceSize()=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3116_112_2_18_2_13_51_123,(uint8*)"d",MMIDM_FOTA_GetSpaceSize());
                SCI_FREE(filename_ptr);
                MMIDM_FOTA_ImageArea_Close();
                return FALSE;
            }

            if(!MMIDM_FOTA_SetUpdateFlag(DM_FOTA_OP_SET_UPDATE_FLAG))
            {
                //SCI_TRACE_LOW:"MMIDM==> LoadFileToImage FOTA_SetUpdateFlag fail!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3124_112_2_18_2_13_51_124,(uint8*)"");
                SCI_FREE(filename_ptr);
                MMIDM_FOTA_ImageArea_Close();
                return FALSE;
            }
            // alloc read buffer
            buffer = SCI_ALLOC_APPZ(sizeof(uint8)*(DM_MAX_READ_FILE_SIZE));
        
            to_read_size = file_size > DM_MAX_READ_FILE_SIZE ? DM_MAX_READ_FILE_SIZE:file_size;  
        
            do{
                SCI_MEMSET(buffer,0x00,(sizeof(uint8)*(DM_MAX_READ_FILE_SIZE)));

                //set read file pointer
                if(SFS_ERROR_NONE == SFS_SetFilePointer(sfs_handle,(int64)s_offset, SFS_SEEK_BEGIN))
                {
                    //read file to buffer
                    error = MMIAPIFMM_ReadFile(sfs_handle, buffer, to_read_size, &bytes_read, NULL);
                    //SCI_TRACE_LOW:"MMIDM==> LoadFileToImage to_read_size=%d,bytes_read= %d,error=%d"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3142_112_2_18_2_13_51_125,(uint8*)"ddd",to_read_size,bytes_read,error);
                
                    if(SFS_NO_ERROR == error)
                    {
                        //writer buffer to image area
                        fota_size = MMIDM_FOTA_ImageArea_Write(buffer,to_read_size);

                        //SCI_TRACE_LOW:"MMIDM==> FOTA_ImageArea_Write fota_size =%d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3149_112_2_18_2_13_51_126,(uint8*)"d",fota_size);
                    
                        if (fota_size != bytes_read || 0 == fota_size)
                        {
                            //SCI_TRACE_LOW:"MMIDM==> FOTA_ImageArea_Write fail fota_size=%d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3153_112_2_18_2_13_51_127,(uint8*)"d",fota_size);
                            result = FALSE;
                            break;
                        }
                        else
                        {
                            result = TRUE;
                        }
                    }
                    else
                    {
                        //SCI_TRACE_LOW:"MMIDM==> MMIAPIFMM_ReadFile fail!"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3164_112_2_18_2_13_51_128,(uint8*)"");
                        result = FALSE;
                        break;
                    }
                }
                else
                {
                    //SCI_TRACE_LOW:"MMIDM==> SFS_SetFilePointer fail!"
                    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3171_112_2_18_2_13_51_129,(uint8*)"");
                    result = FALSE;
                    break;
                }
                s_offset += to_read_size;
                to_read_size = (file_size-s_offset)>DM_MAX_READ_FILE_SIZE?DM_MAX_READ_FILE_SIZE:(file_size-s_offset);
                //SCI_TRACE_LOW:"MMIDM==> loop s_offset =%d,to_read_size=%d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3177_112_2_18_2_13_51_130,(uint8*)"dd",s_offset,to_read_size);
            }while( 0 != to_read_size );
        
            if (result)
            {
                result = MMIDM_FOTA_SetUpdateFlag(DM_FOTA_OP_SET_UPDATE_FLAG);
            }
            else
            {
                MMIDM_FOTA_ImageArea_Erase();
            }
            MMIDM_FOTA_ImageArea_Close();
            MMIDM_SetFotaUpdateState(DM_FOTA_UPDATA_NONE);
            SCI_FREE(buffer);
            MMIAPIFMM_CloseFile(sfs_handle);
        }
        //SCI_TRACE_LOW:"MMIDM==> result = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3193_112_2_18_2_13_51_131,(uint8*)"d",result);
        SCI_FREE(filename_ptr);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> LoadFileToImage fota not switch on!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3198_112_2_18_2_13_51_132,(uint8*)"");
    }

    return result;
}

/*****************************************************************************/
//  Description : get fota update state
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC FOTA_UPDATA_STATE_E MMIDM_GetFotaUpdateState(void)
{
    return s_dm_info.fota_state;
}


/*****************************************************************************/
//  Description : set fota update state
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SetFotaUpdateState(FOTA_UPDATA_STATE_E state)
{
    //SCI_TRACE_LOW:"MMIDM==> fota state = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3220_112_2_18_2_13_51_133,(uint8*)"d",state);
    
    s_dm_info.fota_state = state;
    MMINV_WRITE(MMINV_DM_FOTA_STATE,&s_dm_info.fota_state);

    if (DM_FOTA_UPDATA_FAIL == state)
    {
        s_fota_send_report_times++;
        if (s_fota_send_report_times >= 3)//after tree times send fail reset none
        {
            //SCI_TRACE_LOW:"MMIDM==> fota state DM_FOTA_UPDATA_DONE"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3230_112_2_18_2_13_51_134,(uint8*)"");
            s_fota_send_report_times = 0;
            s_dm_info.fota_state = DM_FOTA_UPDATA_DONE;
            MMINV_WRITE(MMINV_DM_FOTA_STATE,&s_dm_info.fota_state);/*lint !e747*/
        }
        else
        {
            MMIDM_CreateFotaReportTimer();
        }
    }
}

/*****************************************************************************/
//  Description : SendFotaReport
//  Global resource dependence : none
//  Author: jian.ma
//  Note
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_SendFotaReport(void)
{
    BOOLEAN  ret = FALSE;
    static uint16 try_times = 0;        
    MN_PHONE_PLMN_STATUS_E status = MMIAPIPHONE_GetPlmnStatus(MMIDM_GetMainSim());
    DM_FOTA_RESULT_E  result = MMIDM_GetFotaUpdateResult();
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_SendFotaReport PLMN_status=%d,fota result = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3252_112_2_18_2_13_51_135,(uint8*)"dd",status,result);
    
    if ((DM_FOTA_UPDATA_NONE == MMIDM_GetFotaUpdateState()||DM_FOTA_UPDATA_FAIL == MMIDM_GetFotaUpdateState())
        && DM_FOTA_NONE != result 
        )
    {
        if (PLMN_NO_SERVICE == status)
        {
            if(try_times<3)
            {
                MMIDM_CreateFotaReportTimer();
                try_times++;
            }
        }
        else
        {
            //SCI_TRACE_LOW:"MMIDM==> MMIDM_SendFotaReport "
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3268_112_2_18_2_13_51_136,(uint8*)"");
            ret = MMIDM_StartDm(DM_SESSION_FOTA,PNULL,(uint32)result);
            if (ret)
            {
                MMIDM_SetFotaUpdateState(DM_FOTA_UPDATA_START);
            }
            //SCI_TRACE_LOW:"MMIDM==> MMIDM_SendFotaReport =%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3274_112_2_18_2_13_51_137,(uint8*)"d",ret);
        }
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> plmn status=%d,fota result=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3279_112_2_18_2_13_51_138,(uint8*)"dd",status,result);
    }
    return ret;
}

/*****************************************************************************/
//  Description : Function used to extract app file 
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_ExtractAPPFile(MMIDM_EXT_FILE_INFO_T *file_info)
{
    MMIFILE_ERROR_E     error = SFS_NO_ERROR;
    BOOLEAN             result = FALSE;
    wchar               source_full_path[MMIFILE_FULL_PATH_MAX_LEN+ 1] = {0};
    uint16              source_full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar*              source_filename_ptr = PNULL;   
     uint32                    str_len = (uint32)strlen(DM_SCOMO_UPDATE_FILE)+1;

    source_filename_ptr = SCI_ALLOCAZ(str_len * sizeof(wchar));
    if (PNULL == source_filename_ptr)
    {
        return result;
    }

    MMIAPICOM_StrToWstr((uint8*)DM_SCOMO_UPDATE_FILE,source_filename_ptr);

    if(MMIDM_GetFileFullPath(source_filename_ptr,(uint16)MMIAPICOM_Wstrlen(source_filename_ptr),source_full_path,&source_full_path_len))
    {
        result = ExtractAppFileAD(source_full_path,source_full_path_len,file_info);
        error = MMIAPIFMM_DeleteFile(source_full_path, NULL);
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_ExtractAPPFile MMIAPIFMM_DeleteFile error= %d result= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3310_112_2_18_2_13_51_139,(uint8*)"dd",error,result);
    }
                            
    SCI_FREE(source_filename_ptr);

    return result;
}

/*****************************************************************************/
//  Description : create app file 
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
LOCAL BOOLEAN CreateAppFile( 
                            MMIFILE_HANDLE read_handle,
                            MMIFILE_HANDLE write_handle,
                            uint32         offset,
                            uint32         file_size,
                            uint8*         buffer,
                            uint32         max_buffer_len
                            )
{
    uint32 read_size = 0;
    uint32 bytes_read = 0;
    SFS_ERROR_E error = SFS_NO_ERROR;
    uint32 writelen = 0;
    uint32 size = file_size;
    if (0== read_handle || 0==write_handle || PNULL==buffer)
    {
        return FALSE;
    }

    SFS_SetFilePointer(read_handle, (int64)offset, SFS_SEEK_BEGIN);
    read_size = file_size < DM_MAX_BUFFER_LENGTH ?file_size: DM_MAX_BUFFER_LENGTH;

    do{
        SCI_MEMSET(buffer,0,max_buffer_len);
        error = MMIAPIFMM_ReadFile(read_handle, buffer, read_size, &bytes_read, NULL);
        //SCI_TRACE_LOW:"MMIDM==> 2 read_size = %d,bytes_read=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3347_112_2_18_2_13_51_140,(uint8*)"dd",read_size,bytes_read);
        if (SFS_NO_ERROR != error)   
        {
            //SCI_TRACE_LOW:"MMIDM==> read sfspacket_handle fail 1 error=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3350_112_2_18_2_13_51_141,(uint8*)"d",error);
            return FALSE;
        }
        error = MMIAPIFMM_WriteFile(write_handle, buffer, bytes_read, &writelen, NULL);
        //SCI_TRACE_LOW:"MMIDM==> 3 bytes_read = %d,writelen=%d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3354_112_2_18_2_13_51_142,(uint8*)"dd",bytes_read,writelen);
        if( SFS_NO_ERROR != error)
        {
            //SCI_TRACE_LOW:"MMIDM==> write sfsjad_handle fail 1 !error %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3357_112_2_18_2_13_51_143,(uint8*)"d",error);
            return FALSE;
        }

        if (size<=read_size)
        {
            read_size = 0;
        }
        else
        {
            size -= read_size;
            read_size = (size<DM_MAX_BUFFER_LENGTH)?size:DM_MAX_BUFFER_LENGTH;
        }
    }while(read_size!=0);

    return TRUE;
}

/*****************************************************************************/
//  Description : wchar 类型数组高低对调--无需借助缓存
//  Global resource dependence : none
//  Author: Taiping Lai
//  Note:  len: wchar 的个数，而非字节数
/*****************************************************************************/
LOCAL void WcharHighLowExchange(wchar * src ,  //IN & OUT
                                                                  uint16 len        //IN
                                                                  )//@CR253990 2011.07.15
{
    uint16 i = 0;
    
    SCI_TRACE_LOW("MMIDM==> WcharHighLowExchange len=%d", len);
    
    if (PNULL == src || 0 == len)
    {
        return;
    }
    
    for (i = 0; i < len; i++)
    {
        src[i] = ((((src[i] ) & 0x00FF) << 8) + (((src[i] ) & 0xFF00) >> 8));
    }
}

/*****************************************************************************/
//  Description : extract the java
//  Global resource dependence : none
//  Author:rui xu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN ExtractAppFileAD(
                                      wchar * filepath_ptr, //in
                                      uint32 full_path_len, //in
                                      MMIDM_EXT_FILE_INFO_T *extract_ptr //out  
                                      )
{
    wchar *file_ptr = PNULL;
    wchar filename[MMIFILE_FULL_PATH_MAX_LEN+1]={0};
    wchar name[MMIFILE_FULL_PATH_MAX_LEN+1]={0};
    SFS_HANDLE              sfsjad_handle = NULL;
    SFS_HANDLE              sfsjar_handle = NULL;
    SFS_HANDLE              sfspacket_handle = NULL;
    MMIDM_APP_PAKEAGE_OLD_T app_java_old = {0};
    MMIDM_APP_PAKEAGE_T     app_java = {0};
    //uint32                  to_read_size = 0;
    uint32                  bytes_read = 0;
    uint8*                  buffer = PNULL;
    uint32                  jad_size = 0;
    uint32                  jar_size = 0;
    uint16                  offset = 0;
    SFS_ERROR_E             error = SFS_NO_ERROR;

    uint32                  file_size = 0;
    uint32                  file_head = 0;
    uint32                  file_all_size = 0;
    BOOLEAN                 is_old_version = FALSE;
    uint32                  len =0;
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_ExtractAppFile"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3405_112_2_18_2_13_52_144,(uint8*)"");
    
    if(MMIAPIFMM_IsFileExist(filepath_ptr,full_path_len))
    {  
        SCI_MEMSET(extract_ptr,0,sizeof(MMIDM_EXT_FILE_INFO_T));
        
        if(0==(sfspacket_handle = MMIAPIFMM_CreateFile(filepath_ptr,SFS_MODE_READ| SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS,PNULL,PNULL)))
        {
            //SCI_TRACE_LOW:"MMIDM==> create sfspacket_handle fail !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3413_112_2_18_2_13_52_145,(uint8*)"");
            return FALSE;
        }
        SFS_SetFilePointer(sfspacket_handle, (int64)sizeof(uint32), SFS_SEEK_BEGIN);
        if((error = MMIAPIFMM_ReadFile(sfspacket_handle, &file_head, sizeof(uint32), &bytes_read, NULL))!=SFS_NO_ERROR)
        {
            MMIAPIFMM_CloseFile(sfspacket_handle);
            //SCI_TRACE_LOW:"MMIDM==> read sfspacket_handle fail !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3420_112_2_18_2_13_52_146,(uint8*)"");
            return FALSE;
        }
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_ExtractAppFile file_head=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3423_112_2_18_2_13_52_147,(uint8*)"d",file_head);
        SFS_SetFilePointer(sfspacket_handle, 0, SFS_SEEK_BEGIN);/*lint !e747*/
        if (MMIDM_JAVA_DMAP == file_head)
        {
            if((error = MMIAPIFMM_ReadFile(sfspacket_handle, &app_java, sizeof(MMIDM_APP_PAKEAGE_T), &bytes_read, NULL))!=SFS_NO_ERROR)
            {
                MMIAPIFMM_CloseFile(sfspacket_handle);
                //SCI_TRACE_LOW:"MMIDM==> read sfspacket_handle fail !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3430_112_2_18_2_13_52_148,(uint8*)"");
                return FALSE;
            }
#if defined WIN32 || defined CHIP_ENDIAN_LITTLE //DMAppPackage打包的是大端，需要转化为小端
            WcharHighLowExchange((wchar *)&app_java.package_version, 1);
            WcharHighLowExchange(app_java.name, MMIAPICOM_Wstrlen((const wchar *)app_java.name));
            WcharHighLowExchange(app_java.version, MMIAPICOM_Wstrlen((const wchar *)app_java.version));
            WcharHighLowExchange(app_java.provider, MMIAPICOM_Wstrlen((const wchar *)app_java.provider));
#endif
            //SCI_TRACE_LOW:"MMIDM==> MMIDM_ExtractAppFile version=%d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3433_112_2_18_2_13_52_149,(uint8*)"d",app_java.package_version);
        }
        else
        {
            is_old_version = TRUE;
            if((error = MMIAPIFMM_ReadFile(sfspacket_handle, &app_java_old, sizeof(MMIDM_APP_PAKEAGE_OLD_T), &bytes_read, NULL))!=SFS_NO_ERROR)
            {
                MMIAPIFMM_CloseFile(sfspacket_handle);
                //SCI_TRACE_LOW:"MMIDM==> read sfspacket_handle fail !"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3441_112_2_18_2_13_52_150,(uint8*)"");
                return FALSE;
            }
        }

        SFS_GetFileSize(sfspacket_handle, &file_size);
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_ExtractAppFile file_size=0x%x"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3447_112_2_18_2_13_52_151,(uint8*)"d",file_size);
        if (is_old_version)
        {
#if !defined WIN32 && !defined CHIP_ENDIAN_LITTLE
        app_java_old.all_size= MMI_GET_BE_FROM_LE_UINT32(app_java_old.all_size);
        app_java_old.jad_size = MMI_GET_BE_FROM_LE_UINT32(app_java_old.jad_size);
#endif
        file_all_size =app_java_old.all_size;
        jad_size = app_java_old.jad_size;
        }
        else
        {
#if !defined WIN32 && !defined CHIP_ENDIAN_LITTLE
            app_java.all_size= MMI_GET_BE_FROM_LE_UINT32(app_java.all_size);
#else
            //DMAppPackage打包的是大端，需要转化为小端
            app_java.jad_size= MMI_GET_BE_FROM_LE_UINT32(app_java.jad_size);//@CR253990 2011.07.15
#endif
            file_all_size =app_java.all_size;
            jad_size = app_java.jad_size;
        }
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_ExtractAppFile file_all_size=%d,jad size= %d,is_old_version %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3465_112_2_18_2_13_52_152,(uint8*)"ddd",file_all_size,jad_size,is_old_version);
        if (file_size!=file_all_size)
        {
            uint32 temp_file_all_size = MMI_GET_BE_FROM_LE_UINT32(file_all_size); 

            if (temp_file_all_size == file_size)
            {//自动校正大小端异常处理
                SCI_TRACE_LOW("MMIDM==> ExtractAppFileAD AUTO ADJUST ###");
                file_all_size = temp_file_all_size;
            }
            else
            {
                MMIAPIFMM_CloseFile(sfspacket_handle);
                return FALSE;
            }
        }
        MMIDM_CleanInstallFile();
        if (!is_old_version)
        {
            MMI_WSTRNCPY(name,MMIFILE_FULL_PATH_MAX_LEN,app_java.name,DM_STRING_LENGTH,DM_STRING_LENGTH);
        }
        else
        {
            MMIAPICOM_StrToWstr(app_java_old.name,name);
        }
        //create jad file 
        MMIAPIFMM_CombineFullPathEx(filename, MMIFILE_FULL_PATH_MAX_LEN, MMI_DEVICE_SYSTEM, PACH_NAME, PNULL);
        //MMIAPICOM_Wstrcpy(filename, MMIAPIFMM_GetDevicePath(MMI_DEVICE_SYSTEM));
        //MMIAPICOM_Wstrcat(filename, PACH_NAME);
	  len=MMIAPICOM_Wstrlen(name);
        MMIAPICOM_Wstrncat(filename, name,MIN(len,MMIFILE_FULL_PATH_MAX_LEN));/*lint !e64*/
        MMIAPICOM_Wstrcat(filename,L".jad");
        if(0==(sfsjad_handle = MMIAPIFMM_CreateFile(filename,SFS_MODE_READ| SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS|SFS_MODE_APPEND,PNULL,PNULL)))
        {
            //SCI_TRACE_LOW:"MMIDM==> create sfsjad_handle fail !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3489_112_2_18_2_13_52_153,(uint8*)"");
            return FALSE;
        }
        //create jar file
        SCI_MEMSET(filename,0,sizeof(filename));
        MMIAPIFMM_CombineFullPathEx(filename, MMIFILE_FULL_PATH_MAX_LEN, MMI_DEVICE_SYSTEM, PACH_NAME, PNULL);
        //MMIAPICOM_Wstrcpy(filename, MMIAPIFMM_GetDevicePath(MMI_DEVICE_SYSTEM));
        //MMIAPICOM_Wstrcat(filename, PACH_NAME);
	 len=MMIAPICOM_Wstrlen(name);
        MMIAPICOM_Wstrncat(filename, name,min(len,MMIFILE_FULL_PATH_MAX_LEN));
        MMIAPICOM_Wstrcat(filename,L".jar");
        if(0==(sfsjar_handle = MMIAPIFMM_CreateFile(filename,SFS_MODE_READ| SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS|SFS_MODE_APPEND,PNULL,PNULL)))
        {
            MMIFILE_CloseFile(sfspacket_handle);
            MMIFILE_CloseFile(sfsjad_handle);
            //SCI_TRACE_LOW:"MMIDM==> create sfsjar_handle fail !"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3502_112_2_18_2_13_52_154,(uint8*)"");
            return FALSE;
        }

        offset = (is_old_version==TRUE)? sizeof(app_java_old):sizeof(app_java);

        buffer = SCI_ALLOC_APPZ(DM_MAX_BUFFER_LENGTH);

        if(!CreateAppFile(sfspacket_handle,sfsjad_handle,offset,jad_size,buffer,DM_MAX_BUFFER_LENGTH))
        {
            MMIAPIFMM_CloseFile(sfspacket_handle);
            MMIAPIFMM_CloseFile(sfsjad_handle);
            MMIAPIFMM_CloseFile(sfsjar_handle);
            SCI_FREE(buffer);
            return FALSE;
        }

        offset += jad_size;
        //SCI_TRACE_LOW:"MMIDM==> app_java.all_size %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3520_112_2_18_2_13_52_155,(uint8*)"d",file_all_size);
        jar_size = file_all_size - offset;

        if(!CreateAppFile(sfspacket_handle,sfsjar_handle,offset,jar_size,buffer,DM_MAX_BUFFER_LENGTH))
        {
            MMIAPIFMM_CloseFile(sfspacket_handle);
            MMIAPIFMM_CloseFile(sfsjad_handle);
            MMIAPIFMM_CloseFile(sfsjar_handle);
            SCI_FREE(buffer);
            return FALSE;
        }

        SCI_FREE(buffer);
        MMIAPIFMM_CloseFile(sfspacket_handle);
        MMIAPIFMM_CloseFile(sfsjad_handle);
        MMIAPIFMM_CloseFile(sfsjar_handle);
        MMIAPIFMM_CombineFullPathEx(extract_ptr->path, MMIFILE_FULL_PATH_MAX_LEN, MMI_DEVICE_SYSTEM, PACH_NAME, PNULL);
        MMIAPICOM_Wstrcpy(extract_ptr->path, MMIAPIFMM_GetDevicePath(MMI_DEVICE_SYSTEM));
        extract_ptr->path_len =MMIAPICOM_Wstrlen(extract_ptr->path);
        ParseFilename(filename, &file_ptr);
        MMIAPICOM_Wstrcpy(extract_ptr->name,file_ptr);
        extract_ptr->name_len =MMIAPICOM_Wstrlen(extract_ptr->name);
//        if (is_old_version)
//        {
//            MMIDM_SetDPHeader(&app_java_old,is_old_version);
//        }
//        else
//        {
//            MMIDM_SetDPHeader(&app_java,is_old_version);
//        }
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_ExtractAppFile end"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3550_112_2_18_2_13_52_156,(uint8*)"");
        return TRUE;
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> File is not exist !"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3555_112_2_18_2_13_52_157,(uint8*)"");
        return FALSE;
    }
}



/*****************************************************************************/
//  Description : parse the file name
//  Global resource dependence : none
//  Author:rui xu
//  Note:
/*****************************************************************************/
LOCAL void  ParseFilename(wchar * filepath_ptr,wchar * * filename_ptr)
{
    wchar * offer=filepath_ptr;
    *filename_ptr=offer;
    while(*offer!='\0')
    { 
        if(*offer++=='\\')
        {
            *filename_ptr=offer;
        }
    }
}
/*****************************************************************************/
//  Description : is dm app install
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_GetIsDmAppInstall(void)
{
    return s_dm_info.is_java_app_install;
}


/*****************************************************************************/
//  Description : set dm app install
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_SetDmAppInstall(BOOLEAN is_install)
{
    s_dm_info.is_java_app_install = is_install;
}

/*****************************************************************************/
//  Description : scomo install
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_ScomoInstall(BOOLEAN isBackgroup)
{
    MMIDM_EXT_FILE_INFO_T file_info={0};
    
    if(MMIDM_ExtractAPPFile(&file_info))
    {
        MMIDM_SetDmAppInstall(TRUE);

#ifndef WIN32
		#ifdef JAVA_SUPPORT
        MMIAPIJAVA_InstallFromFilesystemForDM(file_info.path,file_info.path_len,file_info.name,file_info.name_len);
#else
        if (!isBackgroup)//backgroup 模式都不提示
        {
            MMIPUB_OpenAlertFailWin(TXT_DM_NOT_SUPPORT_INSTALL);
        }
        MMIDM_ScomoInstallCallback(1,PNULL,0,PNULL,0);
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_ScomoInstall not support java!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3615_112_2_18_2_13_52_158,(uint8*)"");
		#endif
#else
        
        MMIDM_ScomoInstallCallback(1,PNULL,0,PNULL,0);
#endif
    }
    else
    {
        if (!isBackgroup)//backgroup 模式都不提示
        {
            MMIPUB_OpenAlertFailWin(TXT_DM_FILE_PARSE_FIAL); 
        }
        //SCI_TRACE_LOW:"MMIDM==> MMIDM_ScomoInstall fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3625_112_2_18_2_13_52_159,(uint8*)"");
    }
}


/*****************************************************************************/
//  Description : get dl state
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC MMIDM_DM_STATE MMIDM_GetDlState(void)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetDlState state = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3636_112_2_18_2_13_52_160,(uint8*)"d",s_dm_info.dl_state);
    return s_dm_info.dl_state;
}

/*****************************************************************************/
//  Description : set dl state
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetDlState(MMIDM_DM_STATE state)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_SetDlState state = %d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3646_112_2_18_2_13_52_161,(uint8*)"d",state);
    s_dm_info.dl_state = state;
}
/*****************************************************************************/
//  Description : dl suspend
//  Global resource dependence : none
//  Author:jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsNeedDelayDMSession(void) 
{
    BOOLEAN result = FALSE;

    if ((MMIDM_IsWapActive()&& MMI_GMMREG_RAT_GPRS ==MMIAPIPHONE_GetTDOrGsm(s_dm_info.dm_sim))
       ||(MMIAPICC_IsInState(CC_IN_CALL_STATE)&& MMI_GMMREG_RAT_GPRS ==MMIAPIPHONE_GetTDOrGsm(s_dm_info.dm_sim))
       )
    {
        result =TRUE;
    }
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_IsNeedDelayDMSession result= %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3663_112_2_18_2_13_52_162,(uint8*)"d",result);
    return result;
}
/*****************************************************************************/
//  Description : open dm alert win
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_ScomoInstallCallback(int suiteID,
                                       uint16*version,
                                       uint16 version_len,
                                       uint16* name,
                                       uint16 name_len)
{
#ifdef DM_SUPPORT_REDBENCH
    BOOLEAN  result = FALSE;
    MMIDM_APP_PAKEAGE_T app_java = {0};
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_ScomoInstallCallback = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3678_112_2_18_2_13_52_163,(uint8*)"d",suiteID);
    
    if (!MMIDM_IsDmRun()&&PNULL!=version&&PNULL!=name)
    {
        MMI_WSTRNCPY(app_java.version,DM_STRING_LENGTH,version,version_len,MIN(DM_STRING_LENGTH,version_len));
        MMI_WSTRNCPY(app_java.name,DM_STRING_LENGTH,name,name_len,MIN(DM_STRING_LENGTH,name_len));
        MMIDM_SetDPHeader(&app_java,FALSE);
        result = MMIDM_StartDm(DM_SESSION_ADD,PNULL,suiteID);
        //SCI_TRACE_LOW:"MMIDM==> install scomo dm result= %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3686_112_2_18_2_13_52_164,(uint8*)"d",result);
    }
    else
    {
        //SCI_TRACE_LOW:"MMIDM==> install scomo dm fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3690_112_2_18_2_13_52_165,(uint8*)"");
    }
#else
    SCI_TRACE_LOW("MMIDM==> MMIDM_ScomoInstallCallback = %d",suiteID);
#endif
}

/*****************************************************************************/
//  Description : init scomo id to tab
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_InitScomoIdToTab(void)
{
    SCI_MEMSET(s_scomo_app_id,0,sizeof(s_scomo_app_id));
    return TRUE;
}
/*****************************************************************************/
//  Description : add scomo id to tab
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_AddScomoIdToTab(int id)
{
    uint32 i = 0;
    BOOLEAN result = FALSE;
    
    for (i =0;i<DM_SCOMO_APPID_MAX_NUMBER;i++)
    {
        if(s_scomo_app_id[i]==id)
        {
            result = TRUE;
            break;
        }
    }
    if (!result)
    {
        for (i =0;i<DM_SCOMO_APPID_MAX_NUMBER;i++)
        {
            if(s_scomo_app_id[i]==0)
            {
                s_scomo_app_id[i] = id;
                result = TRUE;
                break;
            }
        }
    }
    if (result)
    {
        MMINV_WRITE(MMINV_DM_APPID_TAB,s_scomo_app_id);
    }
    //SCI_TRACE_LOW:"MMIDM==> AddScomoIdToTab result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3736_112_2_18_2_13_52_166,(uint8*)"d",result);
    return result;
}

/*****************************************************************************/
//  Description : del scomo id to tab
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_DelScomoIdToTab(int id)
{
    uint32 i = 0;
    BOOLEAN result = FALSE;

    for (i =0;i<DM_SCOMO_APPID_MAX_NUMBER;i++)
    {
        if(s_scomo_app_id[i]== id)
        {
            s_scomo_app_id[i] = 0;
            result = TRUE;
        }
    }
    if(result)
    {
        MMINV_WRITE(MMINV_DM_APPID_TAB,s_scomo_app_id);
    }

    return result;
}
/*****************************************************************************/
//  Description : is scomo app 
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN IsScomoApp(int suiteID)
{
    uint32 i = 0;
    BOOLEAN result = FALSE;
    int scomo_app_id [DM_SCOMO_APPID_MAX_NUMBER]={0};
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    
    MMINV_READ(MMINV_DM_APPID_TAB, scomo_app_id, return_value);
    for (i =0;i<DM_SCOMO_APPID_MAX_NUMBER;i++)
    {
        if(scomo_app_id[i]== suiteID)
        {
            result = TRUE;
            break;
        }
    }
    //SCI_TRACE_LOW:"MMIDM==> IsScomoApp result=%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3784_112_2_18_2_13_52_167,(uint8*)"d",result);
    return result;
}
/*****************************************************************************/
//  Description : java remove callback
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_ScomoRemoveCallback(int suiteID)
{
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_ScomoRemoveCallback = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3793_112_2_18_2_13_52_168,(uint8*)"d",suiteID);
#ifdef DM_SUPPORT_REDBENCH
    if (!IsScomoApp(abs(suiteID)))
    {   
        return;
    }
    if (MMIDM_GetTreeState())
    {
        if (!MMIDM_IsDmRun())
        {
            uint32  id = abs(suiteID);
            if (!MMIDM_StartDm(DM_SESSION_REMOVE,PNULL,id))
            {
                //SCI_TRACE_LOW:"MMIDM==> remove scomo fail!"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3806_112_2_18_2_13_52_169,(uint8*)"");
            }
        }
    }
#endif
}
/*****************************************************************************/
//  Description : clean install file
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIDM_CleanInstallFile(void)
{
    wchar filter[]=L"*.jar|*.jad";
    //wchar full_path[] = PACH_NAME_PATCH;
    SFS_ERROR_E     result = SFS_NO_ERROR;
    SFS_HANDLE      file_handle = NULL;
    SFS_FIND_DATA_T find_data = {0};
    wchar filename[MMIFILE_FULL_PATH_MAX_LEN+1]={0};

    //SCI_TRACE_LOW:"MMIDM==> MMIDM_CleanInstallFile "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3824_112_2_18_2_13_52_170,(uint8*)"");
   
    MMIAPIFMM_CombineFullPathEx(filename, MMIFILE_FULL_PATH_MAX_LEN, MMI_DEVICE_SYSTEM, PACH_NAME, PNULL);
    //MMIAPICOM_Wstrcpy(filename, MMIAPIFMM_GetDevicePath(MMI_DEVICE_SYSTEM));
    //MMIAPICOM_Wstrcat(filename, PACH_NAME_PATCH);
    result = SFS_FindFirstFileCtrl(filename,filter,NULL,&file_handle,&find_data,PNULL);
    if (NULL != file_handle&&SFS_NO_ERROR == result)
    {
        do
        {
            //uint16 len = MMIAPICOM_Wstrlen(find_data.name);
            SCI_MEMSET(filename,0,sizeof(filename));
            if(MMIAPICOM_Wstrlen(find_data.name) + MMIAPICOM_Wstrlen(PACH_NAME) + MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SYSTEM) + 4 <= MMIFILE_FULL_PATH_MAX_LEN)
            {
                MMIAPIFMM_CombineFullPathEx(filename, MMIFILE_FULL_PATH_MAX_LEN, MMI_DEVICE_SYSTEM, PACH_NAME, find_data.name);
                //MMI_WSTRNCPY(filename,MMIFILE_FULL_PATH_MAX_LEN,PACH_NAME,sizeof(PACH_NAME),sizeof(PACH_NAME));
                //MMI_WSTRNCPY(filename+6,MMIFILE_FULL_PATH_MAX_LEN-6,
                //  find_data.name,len,len);
                result = MMIAPIFMM_DeleteFile(filename,PNULL);
                //SCI_TRACE_LOW:"MMIDM==> error = %d"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3843_112_2_18_2_13_52_171,(uint8*)"d",result);
            }

        }
        while (SFS_NO_ERROR == SFS_FindNextFile(file_handle, &find_data)); 
    }
    if (NULL != file_handle)
    {
        SFS_FindClose(file_handle);
    }
}
/*****************************************************************************/
//  Description : delete dm folder files 
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_DelDmFolderFiles(void)
{
    wchar path[] = L"c:";
    wchar filename[] = L"dm";
    SFS_ERROR_E error = SFS_NO_ERROR;

   error = MMIAPIFMM_DeleteTree(path,filename);
   //SCI_TRACE_LOW:"MMIDM==> MMIDM_DelDmFolderFiles error=%d"
   SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3865_112_2_18_2_13_53_172,(uint8*)"d",error);
}

/*****************************************************************************/
//  Description : clean resume nv
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC  void MMIDM_CleanResumeNv(void)
{
    MMIDM_RESUME_TYPE  resume_type = RESUME_TYPE_MAX;
    MMINV_WRITE(MMINV_DM_RESUME_TYPE,&resume_type);
}

/*****************************************************************************/
//  Description : get resume type
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC MMIDM_RESUME_TYPE MMIDM_GetResumeType(void)
{
    MMIDM_RESUME_TYPE  resume_type = RESUME_TYPE_MAX;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_DM_RESUME_TYPE, &resume_type, return_value);

    //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetResumeType type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3889_112_2_18_2_13_53_173,(uint8*)"d",resume_type);
    return resume_type;
}
/*****************************************************************************/
//  Description : get uimode
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC E_VDM_NIA_UIMode_t MMIDM_GetUiMode(void)
{
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    E_VDM_NIA_UIMode_t ui_mode = E_VDM_NIA_UIMode_NotSpecified;

    MMINV_READ(MMINV_DM_UI_MODE, &ui_mode, return_value);
    if(MN_RETURN_SUCCESS != return_value)
    {
        ui_mode = E_VDM_NIA_UIMode_NotSpecified;
        MMINV_WRITE(MMINV_DM_UI_MODE,&ui_mode);
    }

    return ui_mode;
}
/*****************************************************************************/
//  Description : set uimode
//  Global resource dependence : 
//  Author:mary.xiao
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetUiMode(E_VDM_NIA_UIMode_t ui_mode)
{
    MMINV_WRITE(MMINV_DM_UI_MODE, &ui_mode);
}

/*****************************************************************************/
//  Description : Set dm broken type
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_SetBrokenType(MMIDM_BROKEN_TYPE type)
{
    if( type < DM_BROKEN_TYPE_MAX)
    {
        MMINV_WRITE(MMINV_DM_BROKEN_TYPE,&type);
    }
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_SetBrokenType type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3929_112_2_18_2_13_53_174,(uint8*)"d",type);
}

/*****************************************************************************/
//  Description : Get dm broken type
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC MMIDM_BROKEN_TYPE MMIDM_GetBrokemType(void)
{
    MMIDM_BROKEN_TYPE  broken_type = DM_BROKEN_TYPE_NONE;
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    MMINV_READ(MMINV_DM_BROKEN_TYPE, &broken_type, return_value);
    //SCI_TRACE_LOW:"MMIDM==> MMIDM_GetBrokemType broken_type = %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDM_APP_3942_112_2_18_2_13_53_175,(uint8*)"d",broken_type);
    return broken_type;
}
/*****************************************************************************/
// 	Description : is need handle delay event
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsNeedHandleDelayEvent(void)
{
    return s_is_need_handle_delay_event;
}

/*****************************************************************************/
// 	Description : set need handle delay event
//	Global resource dependence : none
//  Author:jian.ma
//	Note:
/*****************************************************************************/
PUBLIC void MMIDM_SetNeedHandleDelayEvent(BOOLEAN is_need_handle)
{
    s_is_need_handle_delay_event = is_need_handle;
}

/*****************************************************************************/
// 	Description : Checking whether scomo file is exist
//	Global resource dependence : none
//  Author:qing.yu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDM_IsScomoFileExist(void)
{
    BOOLEAN result = FALSE;
    wchar   source_full_path[MMIFILE_FULL_PATH_MAX_LEN+ 1] = {0};
    uint16  source_full_path_len = MMIFILE_FULL_PATH_MAX_LEN;
    wchar*  source_filename_ptr = PNULL;   
    uint32  str_len = (uint32)strlen(DM_SCOMO_UPDATE_FILE)+1;

    source_filename_ptr = SCI_ALLOC_APP(str_len * sizeof(wchar));
    if (PNULL == source_filename_ptr)
    {
        return result;
    }
    SCI_MEMSET(source_filename_ptr,0,(str_len* sizeof(wchar)));

    MMIAPICOM_StrToWstr((uint8*)DM_SCOMO_UPDATE_FILE,source_filename_ptr);

    if(MMIDM_GetFileFullPath(source_filename_ptr,(uint16)MMIAPICOM_Wstrlen(source_filename_ptr),source_full_path,&source_full_path_len))
    {
        result = MMIFILE_IsFileExist(source_full_path,source_full_path_len);
    }
                            
    SCI_FREE(source_filename_ptr);

    return result;
}

#endif /*DM_ONLY_SELFREG_SUPPORT*/

#if 0
/*****************************************************************************/
//  Description : create java app pak
//  Global resource dependence : 
//  Author:jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC void MMIDM_CreateJavaAppPak(void)
{
    SFS_HANDLE              sfs_handle1 = NULL;
    SFS_HANDLE              sfs_handle2 = NULL;
    SFS_HANDLE              sfs_handle3 = NULL;
    uint32                  file_size_jar = 0;
    uint32                  file_size_jad = 0;
    SFS_ERROR_E             error = SFS_NO_ERROR;
    uint16                  java_jar[]=L"d:\\dm\\MSNV2.0.0.jar";
    uint16                  java_jad[]=L"d:\\dm\\MSNV2.0.0.jad";
    uint16                  java[]=L"d:\\dm\\MSNV2.0.0.dat";
    MMIDM_APP_PAKEAGE_OLD_T     app_java = {0};
    uint16                  writelen = 0;
    uint16                  offset = 0;
    uint32                  bytes_read = 0;
    uint8                   * buffer = PNULL;
    SCI_MEMCPY(app_java.name,"MSNV2.0.0",sizeof("MSNV2.0.0"));
    SCI_MEMCPY(app_java.version,"8801h_09",sizeof("8801h_09")); 
    SCI_MEMCPY(app_java.provider,"spreadtrum",sizeof("spreadtrum")); 

    sfs_handle1 = MMIAPIFMM_CreateFile(java_jad,SFS_MODE_READ| SFS_MODE_WRITE | SFS_MODE_OPEN_ALWAYS,PNULL,PNULL);
    SFS_GetFileSize(sfs_handle1, &file_size_jad);
    app_java.all_size += file_size_jad;
    app_java.jad_size = file_size_jad;
    sfs_handle2 = MMIAPIFMM_CreateFile(java_jar,SFS_MODE_READ| SFS_MODE_WRITE | SFS_MODE_OPEN_ALWAYS,PNULL,PNULL);
    SFS_GetFileSize(sfs_handle2, &file_size_jar);
    app_java.all_size += file_size_jar;
    app_java.all_size += sizeof(app_java);

    sfs_handle3 = MMIAPIFMM_CreateFile(java,SFS_MODE_READ| SFS_MODE_WRITE|SFS_MODE_OPEN_ALWAYS,PNULL,PNULL);

    error = MMIAPIFMM_WriteFile(
                sfs_handle3, 
                &app_java, 
                sizeof(app_java), 
                &writelen, 
                NULL
                );
    offset = sizeof(app_java);
    
    SFS_SetFilePointer(sfs_handle3, offset, SFS_SEEK_BEGIN);

    buffer = SCI_ALLOC_APP(sizeof(uint8)*(file_size_jad));
    SCI_MEMSET(buffer,0,(sizeof(uint8)*(file_size_jad)));
    error = MMIAPIFMM_ReadFile(sfs_handle1, buffer, file_size_jad, &bytes_read, NULL);

    error = MMIAPIFMM_WriteFile(
        sfs_handle3, 
        buffer, 
        bytes_read, 
        &writelen, 
        NULL
        );

    SCI_FREE(buffer);
    offset +=file_size_jad;
    buffer = SCI_ALLOC_APP(sizeof(uint8)*(file_size_jar));
    SCI_MEMSET(buffer,0,(sizeof(uint8)*(file_size_jar)));
    SFS_SetFilePointer(sfs_handle3, offset, SFS_SEEK_BEGIN);
    error = MMIAPIFMM_ReadFile(sfs_handle2, buffer, file_size_jar, &bytes_read, NULL);

    error = MMIAPIFMM_WriteFile(
        sfs_handle3, 
        buffer, 
        bytes_read, 
        &writelen, 
        NULL
        );

    SCI_FREE(buffer);
    MMIAPIFMM_CloseFile(sfs_handle1);
    MMIAPIFMM_CloseFile(sfs_handle2);
    MMIAPIFMM_CloseFile(sfs_handle3);
}

#endif


