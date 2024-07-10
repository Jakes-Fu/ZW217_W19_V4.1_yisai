/****************************************************************************
** File Name:      mmimrapp.h                                               *
** Author:                                                                  *
** Date:           05/15/2006                                               *
** Copyright:      2003 Spreatrum, Incoporated. All Rights Reserved.        *
** Description:    this file is use to describe the mr app module functions *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 03/2006        		       Create
** 
****************************************************************************/
#ifndef  _MMIMRAPP_H_    
#define  _MMIMRAPP_H_  

 /**--------------------------------------------------------------------------*
 **                         Include Files                                     *
 **---------------------------------------------------------------------------*/
#include "mmidisplay_data.h"
#include "mmiset_export.h"
#include "mn_type.h"
#include "mmk_type.h"
#include "block_mem_def.h"
#include "guistatusbar.h"
#include "mmipdp_export.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef __cplusplus
   extern   "C"
   {
#endif

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

#define PORT_BASE_NILE//porting base mocor (new rocky, nile,L etc..)

#define EXTPCM //�Ƿ���nes ��������
 
 /*Ӧ����Ļsize*/
/*modify by zack@20120803 start*/ 
#if defined(MAINLCD_LOGIC_ANGLE_90) || defined(MAINLCD_LOGIC_ANGLE_270)
#define MR_SCREEN_WIDTH 						(MMI_MAINSCREEN_HEIGHT)
#define MR_SCREEN_HEIGHT 						(MMI_MAINSCREEN_WIDTH)
#else
#define MR_SCREEN_WIDTH 						(MMI_MAINSCREEN_WIDTH)
#define MR_SCREEN_HEIGHT 						(MMI_MAINSCREEN_HEIGHT)
#endif
/*modify by zack@20120803 end*/ 

/*
  Description: �����������ڴ�ֵ��С����λΪ�ֽ�,ϵͳ��С�ڴ�Ϊ650K���Ӳ�ͬ�������

  68�ȸ߶˻������ڴ������2M���ϣ���������¸�����Ļ�ֱ��������ʵ���ֻ��ڴ�ʣ������¾�����������
*/
#if defined(PLATFORM_SC6800H)
	#define __MRAPP_CFG_VAR_VMMEM_SIZE__ (4*1024*1024)
#else
#if defined(MAINLCD_SIZE_320X480)
	#define __MRAPP_CFG_VAR_VMMEM_SIZE__ (1.5*1024*1024)
#elif defined(MAINLCD_SIZE_240X400)  
	#define __MRAPP_CFG_VAR_VMMEM_SIZE__ (1024*1024)
#else
	#define __MRAPP_CFG_VAR_VMMEM_SIZE__ (800*1024)
#endif
#endif

#define   __MR_CFG_VMMEM_CUSTOM__        //�Ƿ�֧��MRP�Զ����ڴ��С

#ifdef __MR_CFG_VMMEM_CUSTOM__
#define __MRAPP_CFG_VAR_VMMEM_MAX_SIZE__ (2.5*1024*1024)
#endif

/*
  Description: SPR ����汾�ţ�16���Ʊ�ʾ����Ҫ����ǰ���0x��
  	��ֲǰ�����ȷ�Ϻÿͻ�Ŀ�깤�̵Ļ��߰汾�ţ�����ȷ�������º��ֵ
*/
#define __MR_CFG_VAR_SPR_VERSION__		(0x12A1251)
#define SPR_VERSION		                 __MR_CFG_VAR_SPR_VERSION__        

/*
  Description: �Ƿ�֧����չ�����ֶ�
*/
//#define __MR_CFG_FEATURE_HANDSETID_EX__		

//:TODO: ����ع�ע���漸��Ӧ��ѡ��ĺ꣬ȷ����ʹ�õ�Ӧ����رն�Ӧ�ĺ꣬�Խ�Լ����ռ�
 /*Ӧ��ѡ�񿪹�*/

#define __MRAPP_OVERSEA__  //����汾

#ifdef __MRAPP_OVERSEA__
//#define MRAPP_MRPSTORE_OPEN	  //MRP Store
#define MRAPP_SKYBUDDY_OPEN   //skybuddy
//#define MRAPP_MSN_OPEN		    //MSN
//#define MRAPP_FACEBOOK_OPEN     //Facebook
//#define MRAPP_TWITTER_OPEN      //Twitter
//#define MRAPP_SKYPE_OPEN        //Skype
//#define MRAPP_YAHOO_OPEN        //Yahoo Message
#else
//#define __MRAPP_DOWNLOAD__    //��������
#define MRAPP_NETGAME_OPEN	//ð������
//#define MRAPP_QIPAI_OPEN	    //ð������
//#define MRAPP_DDZ_OPEN	    //������
#define MRAPP_COOKIE_OPEN		//����
#define MRAPP_EBOOK_OPEN	    //ð�����
#define MRAPP_MSN_OPEN		    //MSN
#define MRAPP_FETION_OPEN	    //����
#define MRAPP_SDKQQ_OPEN	    //QQ
#define MRAPP_KAIXIN_OPEN	    //������
//#define MRAPP_FARM_OPEN       //ð��ũ��
#define MRAPP_CLOCK_OPEN	    //����ʱ��
//#define MRAPP_TIANQI_OPEN     //��������
//#define MRAPP_TOOL_OPEN       //ʵ�ù���
#define MRAPP_XLWB_OPEN       //����΢��
//#define MRAPP_SIMUGAME_OPEN   //NESģ����
#endif

/*�Ƿ�̻�Ӧ�ã������ѡ�񽫶�Ӧ��mrpԤ��T��,�����������ѯfae*/

//#define SDKQQ_FIRMED
//#define MSN_FIRMED
//#define SKYREAD_FIRMED
//#define FETION_FIRMED
//#define KAIXIN_FIRMED
//#define SKYQQ_FIRMED
//#define FACEBOOK_FIRMED
//#define YAHOO_FIRMED
//#define SKYPE_FIRMED
//#define TWITTER_FIRMED
//#define NES_FIRMED

#ifndef TOUCHPANEL_TYPE_NONE //modify by zack@20100627 for 6600L
#define MR_HANDSET_IS_SUPPORT_TOUCHPANEL//�Ƿ�֧�ִ�����
#endif

#ifdef MMI_WIFI_SUPPORT
//#define __MRAPP_WIFI_SUPPORT__     //�Ƿ�֧��WIFI
#endif
//#define __MRAPP_MOTION_SUPPORT__   //�Ƿ�֧�ִ�����
//#define __MR_CFG_LICENSE_MANAGER__   //�Ƿ�֧�ֱ���Ӧ�ð�
#define __MR_CFG_LOCALAPP_MANAGER__  //�Ƿ�֧�ֱ���Ӧ����ڰ�
#ifdef MMI_RECORD_SUPPORT
#define __MR_RECORD_SUPPORT__       //�Ƿ�֧��¼��
#endif
#ifdef CAP_TP_SUPPORT
#define __MR_CFG_MULTITP_SUPPORT__   //�Ƿ�֧�ֶ�㴥��
#endif

/*ƽ̨�����������ֵ*/
#define MMI_AUD_VOLUME_MAX 					MMISET_VOL_MAX

/*ȷ�����µ�ֵ���龰ģʽ�еĶ�Ӧitem��λ��ֵ��ͬ(��������ģʽֵ)*/
#define MSG_RING_TYPE_VIBRATE 				1//��Ϣ��ʾ����
#define VOLTAGE_WARN_TYPE_NOTRING 			1//��ѹ���棭��
#define FLIP_RING_TYPE_NOTRING 				0//����������
#define CALL_RING_TYPE_VIBRATE 				1//������ʾ����
#define ALARM_RING_TYPE_VIBRATE 			1//������ʾ����

/*��������size��ע��:ƽ̨������size�ı��ʱ����Ҫ�޸���
��Ķ��壬����ʹ������ֵΪ��Чֵ*/
#define MRAPP_FONT_SIZE_SMALL 				MMI_DEFAULT_SMALL_FONT
#define MRAPP_FONT_SIZE_MID 				MMI_DEFAULT_NORMAL_FONT
#define MRAPP_FONT_SIZE_BIG 				MMI_DEFAULT_BIG_FONT

#define MRAPP_FONT_UI_SIZE 					MMI_DEFAULT_TEXT_FONT//ƽ̨ͳһUI(��˵�)�������С

/*mrp�ļ��洢�ĸ�Ŀ¼*/
#define MMIMRAPP_ROOT_DIR		"\x6d\x00\x79\x00\x74\x00\x68\x00\x72\x00\x6f\x00\x61\x00\x64\x00\x00\x00"//mythroad


/*nesģ������rom�ļ����Ŀ¼��ȫ��,����:xxx/xxx, ע:ǰ�治Ҫ��'/', Ӧ�û��Զ����U�̸���忨 */
//#define MMIMRAPP_NES_DIR_NAME			"\x00\x67\x00\x61\x00\x6d\x00\x65\x00\x73\x00\x00"//"games"		
#define MMIMRAPP_NES_DIR_NAME			"games"		

/*����ID*/
#define MRAPP_MANUFACTORY 			"skysple"    //���̺�,length limited to 7byte

#if(MR_SCREEN_WIDTH == 240 && MR_SCREEN_HEIGHT == 320)
#define MRAPP_HANDSET 					"t240320"   //�ֻ��ͺ�,length limited to 7byte
#elif(MR_SCREEN_WIDTH == 240 && MR_SCREEN_HEIGHT == 400)
#define MRAPP_HANDSET 					"t240400"   //�ֻ��ͺ�,length limited to 7byte
#elif(MR_SCREEN_WIDTH == 320 && MR_SCREEN_HEIGHT == 480)
#define MRAPP_HANDSET 					"t320480"   //�ֻ��ͺ�,length limited to 7byte
#elif(MR_SCREEN_WIDTH == 176 && MR_SCREEN_HEIGHT == 220)
#define MRAPP_HANDSET 					"t176220"   //�ֻ��ͺ�,length limited to 7byte
#elif(MR_SCREEN_WIDTH == 220 && MR_SCREEN_HEIGHT == 176)
#define MRAPP_HANDSET 					"t220176"   //�ֻ��ͺ�,length limited to 7byte
#else
#error "Unknown screen size, please call SKY-MOBI."
#endif

/*
  Description: �ֻ���չ����ID������ֶλ�ͻ����ֶ����ӣ������ַ����ĳ���С��34�ֽ�
  	�������Ƿ������m900��չ������handset1��ô���ظ�Ӧ�õĽ���m900-handset1

  Note: Ҫ֧�ִ˹���������__MR_CFG_FEATURE_HANDSETID_EX__��
*/
#define __MR_CFG_VAR_HANDSETID_EX__					"handset1"  // length < 26
#define __MR_CFG_VAR_HANDSETID_EX_LENGHT_MAX__	    34 // 8 + 26
#define SKY_VIDEOPLAYER "Sky videoplayer"
/*Ӧ����ڲ���*/
typedef enum
 {
	MRAPP_COOKIE    = 800001, //����
	MRAPP_MRPSTORE  = 810010, //MRPStore
	MRAPP_SKYBUDDY  = 200004, //SkyBuddy
	MRAPP_APPLIST   = 9000,   //applist
	MRAPP_NETGAME   = 400101, //ð������
    MRAPP_QIPAI     = 1008,   //ð������
    MRAPP_DDZ       = 390028, //������
	MRAPP_EBOOK     = 2908,  //����
	MRAPP_MPMUSIC   = 2913,  //ð�����ֺ�
	MRAPP_SDKQQ     = 203103,//SDKQQ
    MRAPP_MSN       = 203102, //msn
    MRAPP_CLOCK     = 6008,  //����ʱ��
    MRAPP_SKYQQ     = 203106,//SKYQQ
    MRAPP_FETION    = 6005,  //����
    MRAPP_KAIXIN    = 6020,  //������
    MRAPP_FARM      = 391129, //ð��ũ��
    MRAPP_TOOL      = 800000, //ð�ݹ���
    MRAPP_ENDICT    = 391122, //Ӣ���ֵ�
    MRAPP_CHDICT    = 391014, //�»��ֵ�
	MRAPP_XLWB      = 391049, //����΢��
	MRAPP_TIANQI    = 2919,   //��������
    MRAPP_FACEBOOK  = 810002, //Facebook
    MRAPP_SKYPE     = 391022,  //Skype
    MRAPP_YAHOO     = 810000,  //Yahoo message
    MRAPP_TWITTER   = 810006,  //Twitter
#ifdef MRAPP_SIMUGAME_OPEN
    MRAPP_NESGAME   = 9107,
#endif
#ifdef SKY_CAT_SUPPORT
    MRAPP_TALKCAT   = 2924,
#endif
	MRAPP_TEST      = 0xFFFF,
	
	MRAPP_MAX = 127//MAX ID byte
 }MRAPP_APPENTRY_E;


#ifdef __MR_CFG_LICENSE_MANAGER__
/**
 * \brief ����Ӧ��Ȩ��У�������Ϣ
 */
typedef struct {
	void (*mr_lapp_license_register_f)(void);
}mr_lapp_license_entry_t;


#define MR_LAPP_LICENSE_MANAGER_BEGIN()	const mr_lapp_license_entry_t g_mr_lapp_license_entries[] = {
#define MR_LAPP_LICENSE_MANAGER_END()	{0}}
#define MR_LAPP_LICENSE_ENTRY(appname, reg)	{reg},
#endif

#ifdef __MR_CFG_LOCALAPP_MANAGER__
/**
 * \brief ����Ӧ����ں���
 */
typedef int32 (*startSkyLapp)(uint8* param);
#endif
 
/**--------------------------------------------------------------------------*
 **                         STRUCT                               *
 **--------------------------------------------------------------------------*/

typedef BOOLEAN (*MMIMRAPP_WAIT2DOPDPACT_FUNC)(uint32 data);

#define MRAPP_MD_MAXNUM 1//NOTE: THIS VALUE PLS EQUAL TO E_MR_MD_MAX
typedef enum
{
	E_MR_MD_GPRS = 0, //GPRS
	E_MR_MD_MAX
}e_mrapp_phone_mod;


/**--------------------------------------------------------------------------*
 **                         FUNCTINS                                         *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : get and check to proess some ps event msg, as :the pdp activate and deactivate event
//	Global resource dependence : 
//  Author: qgp
//  Note : 
/*****************************************************************************/
MMI_RESULT_E MMIMRAPP_handlePsMsg(MMIPDP_CNF_INFO_T *msg_ptr);

/*****************************************************************************/
// 	Description : ���ע��mrģ��ʹ���е� pdp
//	Global resource dependence : 
//  Author: qgp
//	Note: PARAM - WaitFunc, �ȴ�pdpע�����������ĺ���
//   	  return- TRUE, pdpע���У���Ҫ�ȴ���FALSE��pdp���С�
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_waitToDeactiveMrappPdp(MMIMRAPP_WAIT2DOPDPACT_FUNC WaitFunc, uint32 data);

/*****************************************************************************/
// 	Description : bl mem Cross proc cb
//	Global resource dependence : 
//  Author: qgp
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMRAPP_BLMemCb(BLOCK_MEM_ID_E block_id, BLOCK_MEM_RELATION_STATUS_E relation_info);

PUBLIC void MMIMRAPP_getApnAccountRet(int32 report);

#ifdef MRAPP_CLOCK_OPEN
/*****************************************************************************/
//  Description : netclock timer callback fuction
//  Global resource dependence : 
//  Author: zzh
//  Note:
/*****************************************************************************/
void MMIMRAPP_StartNetClock(void);
#endif

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
 
#ifdef   __cplusplus
   }
#endif

#endif/*_MMIMRAPP_H_*/
