/*****************************************************************************
** File Name:      mmi_custom_define.h                                         *
** Author:                                                                   *
** Date:           12/29/2008                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to store custom define value             *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/29/2008        bin.ji          Create                                 *
******************************************************************************/

#ifndef _MMI_CUSTOM_DEFINE_H_
#define _MMI_CUSTOM_DEFINE_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_font.h"
#ifndef WIN32
#include "buildtime.h"
#endif

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
 
/*-------------------------------font segment-------------------------------------------*/
#if defined MAINLCD_SIZE_128X160
//fixed by hongjun.jia for bugzilla 2461-2464
#define MMI_DEFAULT_BIG_FONT	        SONG_FONT_16//SONG_FONT_14//SONG_FONT_18
#define MMI_DEFAULT_BIG_FONT_SIZE       16//14//18

#define MMI_DEFAULT_NORMAL_FONT	        SONG_FONT_14//SONG_FONT_12//SONG_FONT_16
#define MMI_DEFAULT_NORMAL_FONT_SIZE	14//12//16

#define MMI_DEFAULT_SMALL_FONT          SONG_FONT_12//SONG_FONT_11
#define MMI_DEFAULT_SMALL_FONT_SIZE     12//11

#define WATCH_DEFAULT_BIG_FONT	        SONG_FONT_16
#define WATCH_DEFAULT_BIG_FONT_SIZE       16

#define WATCH_DEFAULT_NORMAL_FONT	        SONG_FONT_14
#define WATCH_DEFAULT_NORMAL_FONT_SIZE	14

#define WATCH_DEFAULT_SMALL_FONT          SONG_FONT_12
#define WATCH_DEFAULT_SMALL_FONT_SIZE     12

#elif defined(MAINLCD_SIZE_240X320)
#define MMI_DEFAULT_BIG_FONT	        DP_FONT_20//SONG_FONT_20
#define MMI_DEFAULT_BIG_FONT_SIZE       20

#define MMI_DEFAULT_NORMAL_FONT	        DP_FONT_18//SONG_FONT_18
#define MMI_DEFAULT_NORMAL_FONT_SIZE	18

#define MMI_DEFAULT_SMALL_FONT          DP_FONT_14//SONG_FONT_14
#define MMI_DEFAULT_SMALL_FONT_SIZE     14

#define WATCH_DEFAULT_BIG_FONT	        DP_FONT_30//SONG_FONT_30
#define WATCH_DEFAULT_BIG_FONT_SIZE       30

#define WATCH_DEFAULT_NORMAL_FONT         DP_FONT_24//   SONG_FONT_24
#define WATCH_DEFAULT_NORMAL_FONT_SIZE	24

#define WATCH_DEFAULT_SMALL_FONT          DP_FONT_16//SONG_FONT_16
#define WATCH_DEFAULT_SMALL_FONT_SIZE     16

#define WATCH_FONT_22       DP_FONT_22
#define WATCH_FONT_16       DP_FONT_16

#ifdef ADULT_WATCH_SUPPORT
#define ADULT_WATCH_DEFAULT_NORMAL_FONT         DP_FONT_20//   SONG_FONT_24
#define ADULT_WATCH_DEFAULT_NORMAL_FONT_SIZE    20

#define ADULT_WATCH_DEFAULT_SMALL_FONT          DP_FONT_16//SONG_FONT_16
#define ADULT_WATCH_DEFAULT_SMALL_FONT_SIZE     16
#endif

#else
#error
#endif

//����1������ɫ
#define MMI_DEFAULT_THEME_FONT_CLOLOR   MMI_GRAY_WHITE_COLOR
#define MMI_THEME1_FONT_COLOR           MMI_BLACK_COLOR

/*-------------------------------setting segment-------------------------------------------*/
//Ĭ�ϲ˵���񣺼�MMIMAINMENU_MATRIX_E�ṹ
#ifdef FLASH_SUPPORT
	#define MMISET_DEFAULT_MENU_STYLE_E	        MMIMAINMENU_MATRIX_E//for defualt style
#else
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
	#ifdef PDA_UI_SUPPORT_MANIMENU_GO			// go�˵���ʱ��Ĭ��Ϊgo�˵�
		#define MMISET_DEFAULT_MENU_STYLE_E	        MMIMAINMENU_GO_SLIDE_E //for defualt style
	#else
		#define MMISET_DEFAULT_MENU_STYLE_E	        MMIMAINMENU_MATRIX_TITLE_E//for defualt style	
	#endif
#else
	#ifdef PDA_UI_SUPPORT_MANIMENU_GO			// go�˵���ʱ��Ĭ��Ϊgo�˵�
		#define MMISET_DEFAULT_MENU_STYLE_E	        MMIMAINMENU_GO_SLIDE_E //for defualt style
    #else
		#define MMISET_DEFAULT_MENU_STYLE_E	        MMIMAINMENU_MATRIX_E  //for defualt style
	#endif
#endif 
#endif
#if defined(PDA_UI_MAINMENU_SUPPORT_SELECT_ANIM)
#define MMISET_DEFAULT_MENU_SELECT_STYLE_E   0//
#endif

//Ĭ��ʱ�ƣ���MMISET_TIME_24HOURS�ṹ
#define MMISET_DEFAULT_TIME_24HOURS         1   //MMISET_TIME_24HOURS
//Ĭ�����ڸ�ʽ����MMISET_DATE_YMD�ṹ
#define MMISET_DEFAULT_DATE_YMD             0   //MMISET_DATE_YMD

//Ĭ�ϳ�ʼ���
#ifdef WIN32
#define MMISET_DEFAULT_SYS_DATE           2010
#else
#define MMISET_DEFAULT_SYS_DATE           PROJECT_BUILD_YEAR
#endif

//Ĭ�ϰ�����
#define MMISET_DEFAULT_KEY_VOL           1

//Ĭ�ϴ�����
#define MMISET_DEFAULT_TKEY_VOL          1

//Ĭ�����ԣ���MMISET_LANGUAGE_SIMP_CHINESE�ṹ
#if defined MMI_PDA_SUPPORT || defined DPHONE_SUPPORT || defined CMCC_UI_STYLE
    #if defined IM_SIMP_CHINESE_SUPPORT
        #ifdef MMI_SIM_LANGUAGE_SUPPORT
            #define MMISET_EDEFAULT_LANGUAGE             2  //����
        #else
            #define MMISET_EDEFAULT_LANGUAGE             1  //����
        #endif
    #else
        #define MMISET_EDEFAULT_LANGUAGE             0  //Ӣ��/Auto
    #endif
#else
    #if defined IM_SIMP_CHINESE_SUPPORT || defined MMI_DISPLAY_SIMP_CHINESE_SUPPORT
        #ifdef MMI_SIM_LANGUAGE_SUPPORT
            #define MMISET_EDEFAULT_LANGUAGE             2  //����
        #else
            #define MMISET_EDEFAULT_LANGUAGE             1  //����
        #endif
    #else
        #define MMISET_EDEFAULT_LANGUAGE             0  //Ӣ��/Auto
    #endif
#endif


//Ĭ�϶Աȶȣ���MMISET_CONTRAST_FIVE�ṹ
#define MMISET_EDEFAULT_CONTRAST            5   //MMISET_CONTRAST_FIVE

//Ĭ�ϰ�����ʱ�䣨��λ�����룩
#define MMISET_EDEFAULT_KEYBOARD_LIGHT_TIME 10000   //MMISET_KEYBOARD_LIGHT_TIME_10

//Ĭ�ϰ��������ͣ���MMISET_KEYBOARD_LIGHT_OPEN�ṹ
#define MMISET_EDEFAULT_KEYBOARD_LIGHT_TYPE 0   //MMISET_KEYBOARD_LIGHT_OPEN

//���ڻ���Ч������MMI_WIN_ANIM_MOVE_SETTING_TYPE_E�ṹ
#define MMISET_EDEFAULT_MOVE_STYLE          1   

//Ĭ��ǽֽ���ͣ���MMISET_WALLPAPER_FIXED_PIC�ṹ
#define MMISET_EDEFAULT_WALLPAPER_TYPE      0   //MMISET_WALLPAPER_FIXED_PIC

//Ĭ��ǽֽid����MMISET_WALLPAPER_DEFAULT_ID�ṹ
#define MMISET_EDEFAULT_WALLPAPER_ID        1   //MMISET_WALLPAPER_DEFAULT_ID
#ifdef SCREENSAVER_SUPPORT
//Ĭ����������״̬��close
#define MMISET_EDEFAULT_SCREENSAVER_OPEN    0  //close
#endif
//Ĭ���������ͣ���MMISET_SCREENSAVER_CLOSED�ṹ
#define MMISET_EDEFAULT_SCREENSAVER_TYPE    0	//MMISET_SCREENSAVER_CLOSED

//Ĭ�Ͽ��ػ���������״̬��open
#define MMISET_EDEFAULT_POWER_ONOFF_ANIM_OPEN  1    //open

//Ĭ�Ͽ��ػ��������ͣ���MMISET_POWER_ON_OFF_ANIM_DEFAULT�ṹ
#define MMISET_EDEFAULT_POWER_ONOFF_ANIM_TYPE  0

//Ĭ�ϱ���ƿ���״̬����MMISET_BACKLIGHT_OPEN�ṹ
#define MMISET_EDEFAULT_BACKLIGHT_OPEN      0

//Ĭ�ϱ����ʱ�䣨��λ�����룩
#ifdef MMI_PDA_SUPPORT
#define MMISET_EDEFAULT_BACKLIGHT_TIME      30000
#else
#define MMISET_EDEFAULT_BACKLIGHT_TIME      15000
#endif
//�˵�����Ĭ�Ͽ���״̬��open
#define MMISET_EDEFAULT_MENU_ANIMATION_OPEN 1   //open

//TVOUTĬ����ʽ����MMISET_PAL_LINE_BY_LINE�ṹ
#define MMISET_EDEFAULTTVOUT_TYPE           0   //MMISET_PAL_LINE_BY_LINE

//Ĭ����������

#define MMISET_DEFAULT_THEME_TYPE           0    //0 or 1

/*-------------------------------env segment-------------------------------------------*/
#if 0// defined MMI_PDA_SUPPORT
#define MMIENVSET_DEFAULT_USER_MAX_NUM      5  //0--5
#endif
/*-------------------------------filemgr segment-------------------------------------------*/

//�ļ��������
#define MMI_FILE_SEARCH_DEPTH_MAX_NUM	2

//�ļ�copy�����С��20k
#define MMI_FILE_COPY_BUFFER_MAX_SIZE	20480
//�ļ�����·����󳤶ȣ�˫�ֽ�Ϊ��λ����Χ��0---255��
#define MMIFMM_FILE_FULL_PATH_MAX_LEN   255  
//�ļ�����󳤶ȣ�˫�ֽ�Ϊ��λ����Χ��0---255��
#define MMIFMM_FILE_FILE_NAME_MAX_LEN   255  

//�ڴ��б������������Χ��200--500��
#ifdef NANDBOOT_SUPPORT
    #define MMIFMM_FILE_MAX_NUM         2000
#else
    #if !defined LOW_MEMORY_SUPPORT
        #define MMIFMM_FILE_MAX_NUM         1000 
    #else
        #define MMIFMM_FILE_MAX_NUM         500
    #endif
        
#endif
//����·������������Χ��40--200��
#ifdef NANDBOOT_SUPPORT
    #define MMIFMM_STORE_PATH_MAX_NUM   200
#else
    #define MMIFMM_STORE_PATH_MAX_NUM   40
#endif
// �ɷ���·�������
#define MMIFMM_PATH_MAX_DEPTH           20  
/*-------------------------------pb segment-------------------------------------------*/
        
// �绰���������ַ����󳤶ȣ���Χ��0 ~40��
#define MMIPB_MAX_MAIL_LEN 	        (uint8)(40)

// �绰�����Զ���ͼƬ,������󳤶ȣ���Χ��0 ~40��
#define MMIPB_MAX_CUSTOM_TEXT_LEN           40//@maggie modify for 6800A PB 20	

// �绰���й�˾������󳤶ȣ���Χ��0 ~20��
#define MMIPB_MAX_TITLE_LEN         20
//�绰����ְ����󳤶ȣ���Χ��0 ~40��
#define MMIPB_MAX_ORG_LEN           40
//��󳤶ȹ�����ַ��󳤶ȣ���Χ��0 ~40��
#define MMIPB_MAX_WORKADR_LEN       50

//�绰����Ⱥ�����Ƶ���󳤶ȣ���Χ0 ~16��
#define MMIPB_MAX_GROUPNAME_LEN     (uint8)(16)

// ���ڶ�������ֻ���һ����¼�ڵ绰����ĸ�������Χ��1 ~5��
#ifdef PB_SUPPORT_LOW_MEMORY
    #define MMIPB_MAX_NV_PHONE_NUM     (uint8)(2) 
#else
    #define MMIPB_MAX_NV_PHONE_NUM     (uint8)(5) 
#endif
//�绰��������λ������Χ��1 ~ 40��
#define MMIPB_NUMBER_MAX_STRING_LEN    40 
//���ٲ������������������ַ�������Χ��0 ~ 11��
#define MMIPB_MUL_SEARCH_MAX_CHAR  11 

#define MMIPB_IM_MAX_LEN            40
#define MMIPB_URL_MAX_LEN           60
#define MMIPB_SNS_NAME_LEN             60
#define MMIPB_SNS_BIRTH_LEN            10
#define MMIPB_SNS_EMAIL_LEN            MMIPB_MAX_MAIL_LEN
#define MMIPB_SNS_PHOTO_PATH_LEN       MMIFMM_FILE_FULL_PATH_MAX_LEN
#define MMIPB_SNS_SIGN_LEN             MMIPB_MAX_CUSTOM_TEXT_LEN
#define MMIPB_SNS_USERID_LEN           20
#define MMIPB_SNS_TYPE_LEN             10
#define MMIPB_SNS_USERID_LEN           20
#define MMIPB_ACTIVE_TEXT_NUM          5
/*-------------------------------sms segment-------------------------------------------*/

// ����ϢNV���洢���� ��ҪNANDBOOT_SUPPORT�궨��򿪣���NV��С�й�ϵ
#if 1 //100  modifyed bao �Ż��ڴ�
  #define MMINV_MAX_SMS_RECORDS    10
#else
#ifdef MMI_SMS_MINI_SUPPORT
  #ifdef MMI_SMS_MAX_NUM_NV_50_ITEMS
  #define MMINV_MAX_SMS_RECORDS    50
  #else
  #define MMINV_MAX_SMS_RECORDS    100
  #endif
#else
#ifdef SMS_SUPPORT_LOW_MEMORY
#ifdef MMI_SMS_MAX_NUM_NV_50_ITEMS
    #define MMINV_MAX_SMS_RECORDS    50
#else
    #define MMINV_MAX_SMS_RECORDS    100
#endif
#elif defined NANDBOOT_SUPPORT
    #define MMINV_MAX_SMS_RECORDS    1000
#else
    #define MMINV_MAX_SMS_RECORDS    200
#endif
#endif

#ifdef CMCC_UI_STYLE
#ifdef MMINV_MAX_SMS_RECORDS
#undef MMINV_MAX_SMS_RECORDS
#endif
#define MMINV_MAX_SMS_RECORDS    1500
#endif
#endif

#ifdef MMI_ETWS_SUPPORT
#define MMINV_MAX_SMSCB_RECORDS    10
#endif

//��������Ŀ
#define MMISMS_FREQUENT_WORD_NUM    10

//��������󳤶�
#define MMISMS_FREQUENT_WORD_LEN    40

//֧������Ⱥ�������������Χ��30-100��
#ifdef SMS_SUPPORT_LOW_MEMORY
#define MMISMS_DEST_ADDR_MAX_NUM     MMINV_MAX_SMS_RECORDS
#else
#define MMISMS_DEST_ADDR_MAX_NUM     100
#endif
//֧�ֳ�����Ϣ�����������Χ��2-6��
#ifdef MMI_DUALMODE_ENABLE
#define MMISMS_SPLIT_MAX_NUM          6
#else
#define MMISMS_SPLIT_MAX_NUM          4
#endif
//֧�ֶ�ȡSIM������Ϣ����(�����ڴ����ƣ��Ӵ����ֵͬʱҲ��Ҫ�Ӵ�const memory)
/*
    �����㷨:���һ�ſ�����ĿС��MMISMS_MAX_SYS_SIM_SMS_NUM/2,
    ��һ�ſ�����Ŀ������ΪMMISMS_MAX_SYS_SIM_SMS_NUM-���������Ŀ;
    �������ȡ�������Ŀ������ΪMMISMS_MAX_SYS_SIM_SMS_NUM/2
*/
#if 1 //100  modifyed bao �Ż��ڴ�
#ifdef MMI_MULTI_SIM_SYS_SINGLE
#define MMISMS_MAX_SYS_SIM_SMS_NUM  10
#else
#define MMISMS_MAX_SYS_SIM_SMS_NUM  (10*MMI_DUAL_SYS_MAX) // 100*N
#endif
#else
#ifdef MMI_SMS_MAX_NUM_SIM_40_ITEMS
#ifdef MMI_MULTI_SIM_SYS_SINGLE
#define MMISMS_MAX_SYS_SIM_SMS_NUM  40
#else
#define MMISMS_MAX_SYS_SIM_SMS_NUM  (40*MMI_DUAL_SYS_MAX) // 100*N
#endif
#else
#ifdef MMI_MULTI_SIM_SYS_SINGLE
#define MMISMS_MAX_SYS_SIM_SMS_NUM  200
#else
#define MMISMS_MAX_SYS_SIM_SMS_NUM  (100*MMI_DUAL_SYS_MAX) // 100*N
#endif
#endif
#endif
//��ȫ����Ĭ������(1-8λ)
#define MMISMS_SECURITYBOX_DEFAULT_PWD "1234"

/*-------------------------------mms segment-------------------------------------------*/
#ifdef NANDBOOT_SUPPORT
//��������Ĳ����ܵ���������������֪ͨ,��Χ:1~200��
#define    MMIMMS_MAX_MMS_NUM                  250  
//MMS��MMS֪ͨ�������������ֵ(��Χ:1~200)    
//#define MMIMMS_MAX_MMS_AND_MMSNOTI_NUM        200    
#else
//��������Ĳ����ܵ���������������֪ͨ,��Χ:1~100��
#define    MMIMMS_MAX_MMS_NUM                 5  //100  modifyed bao �Ż��ڴ�
//MMS��MMS֪ͨ�������������ֵ(��Χ:1~100)
//#define MMIMMS_MAX_MMS_AND_MMSNOTI_NUM        100 
#endif

#ifdef MMI_SMS_MAX_NUM_NV_50_ITEMS
#undef MMIMMS_MAX_MMS_NUM
#define    MMIMMS_MAX_MMS_NUM                 5  //50  modifyed bao �Ż��ڴ�

//����MMS���size(��Χ:10~300k)
#define MMIMMS_MAX_ONE_MMS_SIZE                (100 * 1024)    
#else
//����MMS���size(��Χ:10~300k)
#define MMIMMS_MAX_ONE_MMS_SIZE                (300 * 1024)    
#endif

//�����ռ���������(����CC��BCC��TO��(��Χ:1~30)(�̲ʺ�һ1~100)
#ifdef MMIMMS_SMS_IN_1_SUPPORT
#define MMIMMS_MAX_RECEIVER_NUM                5   //50  modifyed bao �Ż��ڴ�
#else
#define MMIMMS_MAX_RECEIVER_NUM                30   
#endif

//ÿһ����ַ����󳤶�(��Χ:1~40)
#define MMIMMS_MAX_ADDRESS_LEN                40    

//MMS�������󳤶�(��Χ:0~80)
#define MMIMMS_MAX_SUBJECT_LEN                40    
        
//ÿһ֡�����������ݵ���󳤶�(0~6k)
#define MMIMMS_MAX_TEXT_LEN                    (6 * 1024)      

//Ĭ�ϵĵ�ҳ���Ų���ʱ��
#define MMIMMS_DEFAULT_SLIDE_TIMES             10000     

/*-------------------------------user agent segment-------------------------------------------*/
#ifdef DRM_SUPPORT//MS00240308

#if defined(PLATFORM_SC6600L) || defined(PLATFORM_SC6600R)
#define MMI_CUSTOMER_UA_PROFILE_FIELD "http://www.spreadtrum.com/wap/6600L_DORADO_DRM_UAProfile.xml"
#elif defined(PLATFORM_SC6800H) || defined(PLATFORM_SC6530)
#define MMI_CUSTOMER_UA_PROFILE_FIELD    "http://www.spreadtrum.com/wap/6801H_DORADO_DRM_UAProfile.xml" 
#elif defined(PLATFORM_SC8800H) || defined(PLATFORM_SC8800G)
#define MMI_CUSTOMER_UA_PROFILE_FIELD    "http://www.spreadtrum.com/wap/8801H_DORADO_DRM_UAProfile.xml"
#else
#define MMI_CUSTOMER_UA_PROFILE_FIELD    "http://wap.sonyericsson.com/UAprof/K700cR201.xml"  
#endif

#else/*DRM_SUPPORT*/

#if defined(PLATFORM_SC6600L) || defined(PLATFORM_SC6600R)
#define MMI_CUSTOMER_UA_PROFILE_FIELD    "http://www.spreadtrum.com/wap/6600L_DORADO_UAProfile.xml"  
#elif defined(PLATFORM_SC6800H)  || defined(PLATFORM_SC6530)
#define MMI_CUSTOMER_UA_PROFILE_FIELD    "http://www.spreadtrum.com/wap/6801H_DORADO_UAProfile.xml" 
#elif defined(PLATFORM_SC8800H) || defined(PLATFORM_SC8800G)
#define MMI_CUSTOMER_UA_PROFILE_FIELD    "http://www.spreadtrum.com/wap/8801H_240X320_UAProfile.xml"//netfront
//"http://www.spreadtrum.com/wap/8801H_DORADO_UAProfile.xml"
#else
#define MMI_CUSTOMER_UA_PROFILE_FIELD    "http://wap.sonyericsson.com/UAprof/K700cR201.xml"  
#endif

#endif/*DRM_SUPPORT*/

//#ifdef BROWSER_SUPPORT_DORADO
/*the defult UA of dorado browser in platfrom is MMIBROWSER_DEFAULT_DORADO_UA.
    if want to use custom's UA,please open the Macro CUSTOM_DEFINE_UA,and define MMI_DEFAULT_UA in here*/
//#endif

//#define MMI_DEFAULT_UA "HS-N51/1.0 Release/10.25.2009 Browser/NF3.5 Profile/MIDP-2.0 Config/CLDC-1.1"
#define MMI_DEFAULT_UA "HS-T39_TD/1.0 Release/03.03.2011 Threadx/4.0 Mocor/W10 Browser/NF4.0 Profile/MIDP-2.0 Config/CLDC-1.1"

//ua �����벻Ҫ����255,
//#define MMI_DEFAULT_UA "HS-T36_TD/1.0 Release/10.08.2010 Threadx/4.0 Mocor/W10 Browser/NF4.0 Profile/MIDP-2.0 Config/CLDC-1.1"
//note: "_TD" can not missed
//MS00216980 cheney end

/*-------------------------------dorado wap segment-------------------------------------------*/
#ifdef FONT_TYPE_SUPPORT_VECTOR
#ifdef MMI_PDA_SUPPORT
#define MMIBROWSER_BIG_FONT	        SONG_FONT_32
#define MMIBROWSER_NORMAL_FONT	        SONG_FONT_20
#define MMIBROWSER_SMALL_FONT          SONG_FONT_16
#else
#define MMIBROWSER_BIG_FONT	        SONG_FONT_20
#define MMIBROWSER_NORMAL_FONT	        SONG_FONT_16
#define MMIBROWSER_SMALL_FONT          SONG_FONT_14
#endif
#else
#define MMIBROWSER_BIG_FONT	        MMI_DEFAULT_BIG_FONT
#define MMIBROWSER_NORMAL_FONT	        MMI_DEFAULT_NORMAL_FONT
#define MMIBROWSER_SMALL_FONT          MMI_DEFAULT_SMALL_FONT
#endif

/*-------------------------------dorado wap segment-------------------------------------------*/

/*-------------------------------wap segment-------------------------------------------*/
/*
����������ڱ���cache�Ļ���Ĵ�С���ⲿ����Ϣ�����Ҫ����
�����ֵ������ϵͳ�Ĵ洢��Դ��RAM�Ĵ�С������
*/
#define		MMICMSBRW_WAP_CACHE_SIZE					(50*1024)			
/*
��������û�����cookie�Ļ���Ĵ�С���ⲿ����Ϣ�����Ҫ����
�����ȡֵ��Χ������ϵͳ�Ĵ洢��Դ��RAM�Ĵ�С������
*/
#define		MMICMSBRW_WAP_COOKIE_SIZE					(10*1024)			
/*
������У����ڱ�����ҳ���յĻ���Ĵ�С
�����ȡֵ��Χ������ϵͳ��RAM�Ĵ�С������
�ⲿ����Ϣ���û�����ѡ���Ƿ񱣴�ġ�
*/
#define		MMICMSBRW_WAP_SNAPSHOT_SIZE					(60*1024)			
			
/*
������У���ǩ/��ʷ��¼/��ҳ���շֱ� ��������������
������ǩ����ʷ��¼����Ҫ����ģ���ҳ�����û�ѡ�񱣴档
ȡֵ��Χ: 0~255     �����ڴ�ʹ洢��Դ�Ĵ�Сȷ����ֵ������ȡֵ10��20֮��
*/
#ifdef BOOKMARK_SUPPORT_LOW_MEMORY //6432
#define		MMICMSBRW_MAX_ITEM_NUM				5
#else
#define		MMICMSBRW_MAX_ITEM_NUM				20
#endif
/*-------------------------------cc cl segment-------------------------------------------*/

//�Զ��ز������������Ŀ
#define CC_MAX_AUTOCALL_BLACKLIST   10  

//�绰��¼���������
#ifdef MMI_CL_MINI_SUPPORT
    #define MMICL_RECORD_MAX_NUM    40 
#else
    #define MMICL_RECORD_MAX_NUM    80
#endif
//@MS00216980 cheney 
//Be careful!!! Customer must modify the following config for CMCC TEST
//�ͻ������޸�����������Ϣ,���ύ�ƶ�����Ϣ������ȫһ��!!!!!!!
/*
PIM_DEFAULT_MAN
PIM_DEFAULT_MOD
PIM_DEFAULT_OEM
DM_DEFAULT_MAN
DM_DEFAULT_MOD
MMI_DEFAULT_UA
*/
/*-------------------------------Pim segment-------------------------------------------*/
#define  PIM_DEFAULT_MAN                 "hisense"
#define  PIM_DEFAULT_MOD                  "HS-N51"
#define  PIM_DEFAULT_OEM                  "hisense commucation technology,Ltd."

/*-------------------------------DM segment-------------------------------------------*/
#define DM_DEFAULT_MAN                    "spreadtrum"
#define DM_DEFAULT_MOD                    "8501C"

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/  

  
  
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif  


     
