#ifdef WRE_SUPPORT

#ifndef _MMIWRE_TAPI_H_
#define _MMIWRE_TAPI_H_

//#include "mn_type.h"
#define WRE_MAX_SIM_NUM 4
#define WRE_MAX_IMEI_LENGTH  20

/*7-bit�������Ͷ���ϢUD����󳤶�*/
#define  WRE_SMS_7BIT_SMS_LEN           160

/*8-bit�������Ͷ���ϢUD����󳤶�*/
#define  WRE_SMS_8BIT_SMS_LEN           140

//IMSI �����б�,�����ǰ�ֻ�Ϊ˫��˫��,��SlotNum Ϊ2,
//���������2�п�,��simExisted[0]ΪFALSE,simExisted[1]ΪTRUE,ͬʱ��ӦimsiList[1]Ϊ��ЧIMSIֵ.
typedef struct _tagADAPT_IMSI_LIST_T
{
	uint32   SlotNum;
	BOOL    simExisted[WRE_MAX_SIM_NUM];	
	char    imsiList[WRE_MAX_SIM_NUM][20];
}ADAPT_IMSI_LIST_T,*PADAPT_IMSI_LIST_T;


//IMSI �����б�,�����ǰ�ֻ�Ϊ˫��˫��,��SlotNum Ϊ2,
//�����1�Ͳ�2���п�,��simExisted[0]ΪTRUE,simExisted[1]ΪTRUE.
typedef struct _tagADAPT_SIM_EXISTED_LIST_T
{
	uint32 SlotNum;
	BOOL   SimExisted[WRE_MAX_SIM_NUM];
}ADAPT_SIM_EXISTED_LIST_T,*PADAPT_SIM_EXISTED_LIST_T;

typedef enum _ADAPT_MN_PLMN_STATUS_E
{
    ADAPT_PLMN_NO_SERVICE = 0 ,          // no service
    ADAPT_PLMN_NORMAL_GSM_ONLY = 0x01 ,     // voice service available
    ADAPT_PLMN_NORMAL_GPRS_ONLY = 0x02 ,    // data service available    
    ADAPT_PLMN_NORMAL_GSM_GPRS_BOTH = 0x03 ,// voice and data service available

    ADAPT_PLMN_NORMAL_CS_ONLY = 0x01 ,     // voice service available
    ADAPT_PLMN_NORMAL_PS_ONLY = 0x02 ,    // data service availabe    
    ADAPT_PLMN_NORMAL_CS_PS_BOTH = 0x03 ,// voice and data service available

    ADAPT_PLMN_EMERGENCY_ONLY = 0x04,       // emergency service available

    ADAPT_PLMN_EMERGENCY_SIM_INVALID = 0x05,        /* emergency; MM in limited service state and
                                               no further PLMN access allowed until power
                                             off or new SIM inserted */
    ADAPT_PLMN_EMERGENCY_GPRS_ONLY = 0x06,      // data service availabe but emergency; MM in limited service state 
    ADAPT_PLMN_EMERGENCY_SIM_INVALID_GPRS_ONLY = 0x07,   /* data service availabe but emergency; MM in limited service state and
                                                  no further PLMN access allowed until power
                                               off or new SIM inserted */

    ADAPT_PLMN_REGISTER_SERVICE = 0x08,    //attaching after camping on   
    ADAPT_PLMN_REGISTER_GPRS_ONLY = 0x09, // data service availabe but attaching;  
    ADAPT_PLMN_FULL_PS_SERVICE = 0x0A       /*full PS service, no cs service*/

//  PLMN_ACCESS_DIFFICULTY,   /* normal; could be in updated or not updated */
//  PLMN_FORBIDDEN,           // emergency; MM in limited service state */
//  PLMN_LA_FORBIDDEN,        /* emergency; MM in limited service state */
//  PLMN_NAT_ROAM_FORBIDDEN,  /* emergency; MM in limited service state */
//  PLMN_ALL_FORBIDDEN,       /* emergency; MM in limited service state and
//                                no further PLMN access allowed until power
//                              off or new SIM inserted */
//  PLMN_MM_CONNECTION_IN_USE /* normal; MM (MT call) conn in progress */
} ADAPT_MN_PHONE_PLMN_STATUS_E;

typedef enum ADAPT_MN_GMMREG_RAT_E

{
    ADAPT_MN_GMMREG_RAT_GPRS = 0,
    ADAPT_MN_GMMREG_RAT_3G = 1,
    ADAPT_MN_GMMREG_RAT_TD = ADAPT_MN_GMMREG_RAT_3G,
    ADAPT_MN_GMMREG_RAT_GPRS_AND_3G,
    ADAPT_MN_GMMREG_RAT_UNKNOWN,
    /* MN_GMMREG_RAT_VDS_GSM is used for virtual dual standby funtion,refer to CR178284.
       In dual mode, ms will be always in gsm when there has none PDP activated and 
       can reselect TD cell when there has PDP activated. 
     */
    ADAPT_MN_GMMREG_RAT_VDS_GSM = 5   
} ADAPT_MN_GMMREG_RAT_E;

typedef struct _tagWRE_PHONE_PLMN_T
{
	ADAPT_MN_PHONE_PLMN_STATUS_E  	plmn_status;        /*!< plmn statue */
	ADAPT_MN_GMMREG_RAT_E             rat;
	uint16						mcc;
	uint16						mnc;
	uint16						mnc_digit_num;
	uint16						cell_id;
    uint16						lac;		
}WRE_PHONE_PLMN_T,*PWRE_PHONE_PLMN_T;

WRE_PHONE_PLMN_T * Adapt_GetNetWorkInfo(uint32 simNo);
BOOLEAN Adapt_IsSmsReady(uint32 simNo);

BOOL Adapt_GetSimInfo(void *pList,BOOL imsineeded);

BOOL Adapt_GetImei(uint32 simNo,char *imeiStr);

BOOL Adapt_GetImsi(uint32 simNo,char *imsiStr);

PUBLIC  BOOLEAN Adapt_APPSendSms(                                //RETURN:
                                       uint32 dual_sys,
                                       void *content,    //IN:
                                       uint8            *dest_addr_ptr,    //IN:
                                       uint8            dest_addr_len    //IN:
                                       );
//�绰����
BOOL Adapt_MakeCall(uint32 simNo,uint32 calltype,char * tele_num);
uint8    Adapt_GetWREStatus(void);

uint32 GetSimNumSupported(void);

//boot -->  kernel
uint8  WBOOT_WRE_SmsReceiveCallBack(uint32 simNo,char * tele_num,uint16 * sms_content,uint16 content_len);
void WBOOT_WRE_SendSmsCallback(uint8 sendResult);

uint32 Adapt_EntryBrowser(uint32 simNo,char * urlink_ptr,BOOLEAN  is_use_wifi);

int Adapt_GetSmsCenterAddr(uint32 simNo,char * telenum);

void Adapt_APPWriteNewSMS(
		                       uint32    dual_sys,           //IN:
                               uint16        *sms_init_ptr,        //IN:
                               uint8            *dest_addr_ptr,        //IN:
                               uint8            dest_addr_len        //IN:
                               );  //add by qingjun.yu 2011.06.01

void Adapt_AppEditCall(char * tele_num);  //add by qingjun.yu 2011.06.01

BOOL Adapt_GetFlightMode(void);     //add by heng.xiao 2011.0727
//BOOLEAN  WRESMS_RecvSms_CallBack(MN_DUAL_SYS_E dual_sys,uint32 alphabet,MN_CALLED_NUMBER_T * origin_addr,APP_SMS_USER_DATA_T *sms_user_data_t);
BOOLEAN Adapt_IsSDExist(void);
#endif //_wre_tapi_adapt_h_
#endif
