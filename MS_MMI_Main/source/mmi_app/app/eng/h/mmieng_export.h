/****************************************************************************
** File Name:      mmieng_export.h							                *
** Author:         allen								                    *
** Date:           2012.02.03												*
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    									               			*
*****************************************************************************
**                         Important Edit History                        	*
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             	*
** 08/2004      allen               Create
** 02/2012      xiyuan.ma           Edit
****************************************************************************/
#ifndef  _MMIENG_EXPORT_H_    
#define  _MMIENG_EXPORT_H_   

#include "mmiparse_export.h"

//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmieng_main.h"
#ifdef CMMB_SUPPORT
#include "mmimtv_main.h"
#include "mmimbbms_main.h"
#include "mbbms_service.h"
#endif
#include "adpcmrecord_adp.h"
#ifdef BLUETOOTH_SUPPORT
#include "bt_abs.h"
#endif
#ifdef __cplusplus
    extern "C"
    {
#endif

/**--------------------------------------------------------------------------*
			                         MACRO DEFINITION
 **--------------------------------------------------------------------------*/
#define MMIENG_MAX_URL_LEN		512
#define MMIENG_MENU_ITEM_HEIGHT (55)
#define MMIENG_MENU_ITEM_FONT   (SONG_FONT_25)

//@songbin.zeng,2006-08-08
typedef enum
{
	MMIENG_SENSOR_MODE_DEFAULT = 0,
	MMIENG_SENSOR_MODE_RGB, 
	MMIENG_SENSOR_MODE_YUV,
	MMIENG_SENSOR_MODE_MAX
} MMIENG_SENSOR_MODE_E;

//@songbin.zeng,2006-12-05
typedef enum
{
	MMIENG_DV_FRAME_FRE_MIN, 
	MMIENG_DV_FRAME_FRE_7HZ    = 7,
	MMIENG_DV_FRAME_FRE_10HZ   = 10,
	MMIENG_DV_FRAME_FRE_15HZ   = 15, 
	MMIENG_DV_FRAME_FRE_20HZ   = 20, 
	MMIENG_DV_FRAME_FRE_25HZ   = 25, 
	MMIENG_DV_FRAME_FRE_30HZ   = 30, 
	MMIENG_DV_FRAME_FRE_MAX 
} MMIENG_DV_FRAME_FRE_E;

//@songbin.zeng,2006-12-05
typedef enum
{
    MMIENG_IDLE_DIAL_NUM_MIN,
    MMIENG_IDLE_DIAL_NUM_UI_TEST1,
    MMIENG_IDLE_DIAL_NUM_UI_TEST2,
    MMIENG_IDLE_DIAL_NUM_PRODUCT,
    MMIENG_IDLE_DIAL_NUM_PHONE_INFO,
    MMIENG_IDLE_DIAL_NUM_SET_MAX_POWER,
    MMIENG_IDLE_DIAL_NUM_PRODUCT_SN,
    MMIENG_IDLE_DIAL_NUM_SHOW_NET,
    MMIENG_IDLE_DIAL_NUM_MENU,
    MMIENG_IDLE_DIAL_NUM_SHOW_VERSION1,
    MMIENG_IDLE_DIAL_NUM_SHOW_VERSION2,
    MMIENG_IDLE_DIAL_NUM_CHIP_TEST,
    MMIENG_IDLE_DIAL_NUM_CMMB_TEST,
    MMIENG_IDLE_DIAL_NUM_MONKEY_TEST_AUTOSET,
    MMIENG_IDLE_DIAL_NUM_OTA_ENTER,
    #ifdef TF_LOAD_SUPPORT
    MMIENG_IDLE_DIAL_NUM_TFLOAD_ENTER,
    #endif        
    MMIENG_IDLE_DIAL_NUM_NV_COUNTING,
#ifdef MEDIA_DEMO_SUPPORT
    MMIENG_IDLE_DIAL_NUM_AUDIO_DEMO,
    MMIENG_IDLE_DIAL_NUM_VIDEO_DEMO,
    MMIENG_IDLE_DIAL_NUM_MEDIA_DEMO,//for video demos
#endif
#ifdef MET_MEX_SUPPORT    
	MMIENG_IDLE_DIAL_NUM_MET_MEX,
#endif

#ifdef AUTODEMO_SUPPORT
    MMI_AUTODEMO_WINDOW,
#endif

#ifdef MCARE_V31_SUPPORT
	MMIENG_IDLE_DIAL_MCARE_TEST,
#endif

#ifdef ZDT_ZFB_SUPPORT // 支付宝快捷键重置
    MMIENG_ZFB_KEY_RESET_1,
    MMIENG_ZFB_KEY_RESET_2,
    MMIENG_ZFB_KEY_RESET_ALL,
#endif
#if defined(ZDT_SHIPPING_MODE_CMD)//shipping mode
    MMIENG_SHIPPING_MODE_TEST,
#endif

    MMIENG_IMEI_WRITE,
    MMIENG_OPEN_UDISK,
#if defined(ZDT_UNBIND_APP_CMD)//#指令解除绑定APP
    MMIENG_UNBIND_APP_CMD,
#endif

    MMIENG_IDLE_DIAL_NUM_MAX
} MMIENG_IDLE_DIAL_NUM_TYPE_E;

typedef enum
{
    MMIENG_CHIP_TEST_SETTING_ON = 0,
    MMIENG_CHIP_TEST_SETTING_OFF, 
    MMIENG_CHIP_TEST_SETTING_MAX
} MMIENG_CHIP_TEST_SETTING_E;

typedef enum
{
    MMIENG_IQ_MODE_A,
    MMIENG_IQ_MODE_B,
    MMIENG_IQ_MODE_C,
/*[START] Jerry Liang for IQ MODE 2008/07/31*/
    MMIENG_IQ_MODE_D,
/*[END] Jerry Liang for IQ MODE 2008/07/31*/    
    MMIENG_IQ_MODE_CUSTOMIZED,
    MMIENG_IQ_MODE_MAX
}MMIENG_IQ_MODE_E;

typedef struct
{
    BOOLEAN        b_status;//是否开启IQ模式
    BOOLEAN        b_is_start;//是否已经启动IQ数据
    BOOLEAN        b_power_on_start;//是否开机时启动
    uint16         cur_mode;//当前选择的模式
    RECORD_PARAM_T record_param[MMIENG_IQ_MODE_MAX];
}MMIENG_IQ_MODE_T;

typedef struct
{
    uint8					user_agent[MMIENG_MAX_URL_LEN + 1];	
    uint8					uaprofile[MMIENG_MAX_URL_LEN + 1];//默认设置的索引
}MMIENG_UA_T;//ua的信息

typedef enum
{
    MMIENG_SCREEN_LOG_AVALID_MEM,
    MMIENG_SCREEN_LOG_LCD,
    MMIENG_SCREEN_LOG_APP,
    MMIENG_SCREEN_LOG_LAYER_NUM,
	MMIENG_SCREEN_LOG_MAX
} MMIENG_SCREEN_LOG_E;

#define MMIENG_SCREEN_DEBUG_NONE             0
#define MMIENG_SCREEN_DEBUG_STRING          (MMIENG_SCREEN_DEBUG_NONE+1)
#define MMIENG_SCREEN_DEBUG_RECT            (MMIENG_SCREEN_DEBUG_NONE+2)
#define MMIENG_SCREEN_DEBUG_MAX             (MMIENG_SCREEN_DEBUG_NONE+3)
typedef uint8 MMIENG_SCREEN_DEBUG_T;

typedef enum
{
    MMIENG_LTE_PS_MODE_2,
    MMIENG_LTE_CS_PS_MODE_1,
    MMIENG_LTE_CS_PS_MODE_2,
    MMIENG_LTE_PS_MODE_1,
    MMIENG_LTE_OPERATION_MODE_MAX
} MMIENG_LTE_MODE_E;
#if defined ENG_BAND_FREQ_SUPPORT
#define LTE_BAND_MAX     20    //the max support LTE band num
#define LTE_BAND_NUM     18    //the LTE band already support
#define GSM_BAND_MAX      4
#define ENG_FREQ_MAX_NUM  9    //Freq max number
typedef struct
{
    uint16 LTE_band[LTE_BAND_MAX];
    BOOLEAN GSM_band[GSM_BAND_MAX];
    uint32 Freq_list[ENG_FREQ_MAX_NUM];
    uint32 GSM_Freq[ENG_FREQ_MAX_NUM];
}MMIENG_BAND_FREQ_T;
#endif

#ifdef ENG_SUPPORT
/********************************************************************************
 NAME:			MMIAPIENG_SaveLastTime
 DESCRIPTION:	initialize the application 's message handler
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.28
********************************************************************************/
PUBLIC void MMIAPIENG_SaveLastTime(void);

/********************************************************************************
 NAME:			MMIAPIENG_AppInit
 DESCRIPTION:	initialize the application 's message handler
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.28
********************************************************************************/
PUBLIC void MMIAPIENG_AppInit(void);

/*****************************************************************************/
// 	Description : register module nv len and max item
//	Global resource dependence : none
//  Author: songbin.zeng
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_RegNv(void);

/********************************************************************************
 NAME:			MMIAPIENG_RedkeyIsEnableInIdle
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.09.27
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_RedkeyIsEnableInIdle(void);

/********************************************************************************
 NAME:			MMIAPIENG_StartEngineerMenu
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.23
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_StartEngineerMenu(void);
// MMI_IMEI_EDIT_FUNC 
PUBLIC BOOLEAN MMIAPIENG_StartIMEIEdit(void);

/********************************************************************************
 NAME:			MMIAPIENG_GetSensorMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.08.08
********************************************************************************/
PUBLIC MMIENG_SENSOR_MODE_E MMIAPIENG_GetSensorMode(void);

/********************************************************************************
 NAME:			MMIAPIENG_SetSensorMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.08.08
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_SetSensorMode(MMIENG_SENSOR_MODE_E sensor_mode);

/********************************************************************************
 NAME:			MMIAPIENG_VibrateWinGetFocus
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_VibrateWinGetFocus(void);

/********************************************************************************
 NAME:			MMIAPIENG_IsFocusInLoopBack
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		
 DATE:			
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsFocusInLoopBack(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreateShowOutVersionWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateShowOutVersionWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreateProductWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateProductWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreatePhoneInfoWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreatePhoneInfoWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_SetMaxPowerOff
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.09.27
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_SetMaxPowerOff(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreateProductSNWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
BOOLEAN MMIAPIENG_CreateProductSNWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreateShowNetWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateShowNetWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreateShowGprsTestWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		
 DATE:			2020.7.14
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateShowGprsTestWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_BandSelectCnf
 DESCRIPTION:	
 PARAM IN:		None 
 PARAM OUT:		None
 AUTHOR:		songbin.zeng
 DATE:			2006.11.16
********************************************************************************/
PUBLIC void MMIAPIENG_BandSelectCnf(DPARAM  param);

/*****************************************************************************/
// 	Description : to append one item to listbox style by self
//	Global resource dependence : 
//  Author:songbin.zeng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_AppendListItemStyleBySelf(									
                                                MMI_CTRL_ID_T        ctrl_id,
                                                MMI_TEXT_ID_T        text_id,
                                                uint16                      item_pos,
                                                MMI_IMAGE_ID_T      left_icon_id,
                                                MMI_IMAGE_ID_T      right_icon_id,
                                                MMI_TEXT_ID_T        left_softkey_id,
                                                MMI_TEXT_ID_T        mid_softkey_id,
                                                MMI_TEXT_ID_T        right_softkey_id,
                                                BOOLEAN                 is_item_sel
                                                );

/********************************************************************************
 NAME:			MMIAPIENG_CreateRFWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.11.22
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateRFWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_SetDVFrameFre
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_SetDVFrameFre(MMIENG_DV_FRAME_FRE_E dv_frame_frequency);

/********************************************************************************
 NAME:			MMIAPIENG_GetDVFrameFre
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC MMIENG_DV_FRAME_FRE_E MMIAPIENG_GetDVFrameFre(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreateWinDVFrameFre
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateWinDVFrameFre(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreateChipTestWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		ryan.xu
 DATE:			2007.7.31
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateChipTestWin(void);

#ifdef CMMB_SUPPORT
/********************************************************************************
 NAME:			MMIAPIENG_CreateCMMBTestWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.07.23
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateCMMBTestWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreateCMMBTestWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.07.23
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateCMMBChannelStatusWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_InitMTV
 DESCRIPTION:	MTV初始化
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.09.17
********************************************************************************/
PUBLIC void MMIAPIENG_InitMTV(void);

/********************************************************************************
 NAME:			MMIAPIENG_ReleaseMTV
 DESCRIPTION:	MTV释放
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.09.17
********************************************************************************/
PUBLIC void MMIAPIENG_ReleaseMTV(void);

/********************************************************************************
 NAME:			MMIAPIENG_ReleaseMTV
 DESCRIPTION:	MTV停止流程
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2010-12-30
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_StopMTVFlow(void);

/********************************************************************************
 NAME:			MMIAPIENG_PlayMTVService
 DESCRIPTION:	直播
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.09.17
********************************************************************************/
PUBLIC void MMIAPIENG_PlayMTVService(uint32 net_id, uint32 service_id);

/********************************************************************************
 NAME:			MMIAPIENG_MTVRFTest
 DESCRIPTION:	RF测试(用于无信号时的工厂测试)
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.09.17
********************************************************************************/
PUBLIC void MMIAPIENG_MTVRFTest(uint32 net_frequency);

/********************************************************************************
 NAME:			MMIAPIENG_GetMTVFlowInfo
 DESCRIPTION:	获取状态信息
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.09.17
********************************************************************************/
PUBLIC MMIMBBMS_FLOW_INFO_T* MMIAPIENG_GetMTVFlowInfo(void);

/********************************************************************************
 NAME:			MMIAPIENG_GetPlayResult
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.09.17
********************************************************************************/
PUBLIC MBBMS_SERVICE_RESULT_E MMIAPIENG_GetPlayResult(void);

/********************************************************************************
 NAME:			MMIAPIENG_GetMTVChannelDisplayMode
 DESCRIPTION:	获取Channel信息显示模式
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: xin.li
 DATE:2009.09.17
********************************************************************************/
PUBLIC MMIENG_MTV_CHANNEL_DISP_MODE_E MMIAPIENG_GetMTVChannelDisplayMode(void);
#endif//CMMB_SUPPORT

/********************************************************************************
 NAME:			MMIAPIENG_GetChipTestMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		ryan.xu
 DATE:			2007.08.08
********************************************************************************/
PUBLIC MMIENG_CHIP_TEST_SETTING_E MMIAPIENG_GetChipTestMode(void);

/********************************************************************************
 NAME:			MMIAPIENG_SetChipTestMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		ryan.xu
 DATE:			2006.08.08
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_SetChipTestMode(MMIENG_CHIP_TEST_SETTING_E test_mode);

/*****************************************************************************/
// 	Description : start chip test
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: 
/*****************************************************************************/
PUBLIC void MMIENGAPI_StartChipTest(void);

/********************************************************************************
 NAME:			MMIAPIENG_GetCallForwardQuery
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		liqing.peng
 DATE:			2007.07.06
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetCallForwardQuery(void);

/********************************************************************************
 NAME:			MMIAPIENG_GetCallForwardQuery
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		liqing.peng
 DATE:			2007.07.06
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetAutoAnswerCallSet(void);

/********************************************************************************
 NAME:			MMIAPIENG_SetCallForwardQuery
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		liqing.peng
 DATE:			2007.07.06
********************************************************************************/
PUBLIC void MMIAPIENG_SetAutoAnswerCallSet(
                                           BOOLEAN   is_on
                                           );

/********************************************************************************
 NAME:			MMIAPIENG_GetIsCardLogOn
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		bin.ji
 DATE:			
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIsCardLogOn(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIsBrowserLogOn 
//	Global resource dependence : none
//  Author: sally.he
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIsBrowserLogOn(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIsScreenLogOn 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMIENG_SCREEN_DEBUG_T MMIAPIENG_GetIsScreenLogOn(void);

/*****************************************************************************/
// 	Description : get user wre url change 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIsBrowserChangeURL(void);

/*****************************************************************************/
// 	Description : set user wre url change 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetIsBrowserChangeURL(BOOLEAN is_URL_change);

#ifdef MMI_ENG_WIFI_SUPPORT
/********************************************************************************
 NAME:          MMIAPIENG_GetIsFlightModeWifi
 DESCRIPTION:   get flight mode wifi
 PARAM IN:
 PARAM OUT:     flight mode of wifi
 AUTHOR:        zhenpeng.yao
 DATE:          2020.11.25
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIsFlightModeWifi(void);
#endif

/*****************************************************************************/
// 	Description : MMIAPIENG_SetIsScreenLogOn 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetIsScreenLogOn(MMIENG_SCREEN_DEBUG_T type);

/*****************************************************************************/
// 	Description : MMIAPIENG_SetScreenTag 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetScreenTag( const char* tag, MMIENG_SCREEN_LOG_E type, uint32 ticks );

/*****************************************************************************/
// 	Description : MMIAPIENG_AppendScreenTicks 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
//void MMIAPIENG_AppendScreenTicks( MMIENG_SCREEN_LOG_E type );

/*****************************************************************************/
// 	Description : MMIAPIENG_PrintScreenLog 
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_PrintScreenLog( void );

/*****************************************************************************/
// 	Description : display debug rect
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_DisplayDebugRect(
                                       GUI_RECT_T update_rect     //in:
                                       );

/*****************************************************************************/
// 	Description : clear debug rect
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_ClearDebugRect( void );

/*****************************************************************************/
// 	Description : is debug layer
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsDebugLayer( 
                                      GUI_LCD_DEV_INFO const *dev_info_ptr
                                      );

/********************************************************************************
 NAME:			MMIAPIENG_StartupAdjustCardLogOn
 DESCRIPTION:	开机调整CardLog状态
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		bin.ji
 DATE:			2007.10.11
********************************************************************************/
PUBLIC void MMIAPIENG_StartupAdjustCardLogOn(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_SetIQMode 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetIQMode(MMIENG_IQ_MODE_T *iq_mode);

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIQMode 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC MMIENG_IQ_MODE_T MMIAPIENG_GetIQMode(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIQModeStatus 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIQModeStatus(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIQDataStatus 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIQDataStatus(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIQPowerOnStartStatus 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIQPowerOnStartStatus(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_OpenIQDataWin
//	Global resource dependence : 
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenIQDataWin(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_StartIQDataAcquire
//	Global resource dependence : 
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_StartIQData(void);

/*****************************************************************************/
//  Description : 停止录音（通话录音）
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIENG_StopENGRecord(void);

/*****************************************************************************/
//  Description : MMIENG_DeletRecordFile
//  Global resource dependence : none
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIENG_DeletRecordFile(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_StopQData
//	Global resource dependence : 
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_StopQData(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_AlertIQMode
//	Global resource dependence : 
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_AlertIQMode(void);

/*****************************************************************************/
// 	Description : Open IQ Data Query Win
//	Global resource dependence :                                
//      Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_OpenHandleIQDataQueryWin(BOOLEAN is_succeed);
/*****************************************************************************/
// 	Description : is arm log on when power on
//	Global resource dependence :                                
//      Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsUSBLOGON(void);

/*****************************************************************************/
// 	Description : is eng band select cmd
//	Global resource dependence : 
//  Author: jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsBandSelectWinOpen(void);

/********************************************************************************
//MS00216980 cheney
 NAME:			BOOLEAN MMIAPIENG_GetCMCCTestFlag
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		cheney.chen
 DATE:			2010.12.09
********************************************************************************/
PUBLIC  BOOLEAN MMIAPIENG_GetCMCCTestFlag(void);

/********************************************************************************
//MS00216980 cheney
 NAME:			BOOLEAN MMIAPIENG_SetCMCCTestFlag
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		cheney.chen
 DATE:			2010.12.09
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_SetCMCCTestFlag(BOOLEAN *pTestFlag);

#else  //ENG_SUPPORT
/********************************************************************************
 NAME:			MMIAPIENG_AppInit
 DESCRIPTION:	initialize the application 's message handler
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.28
********************************************************************************/
PUBLIC void MMIAPIENG_AppInit(void);

/********************************************************************************
 NAME:			MMIAPIENG_RedkeyIsEnableInIdle
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.09.27
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_RedkeyIsEnableInIdle(void);

/********************************************************************************
 NAME:			MMIAPIENG_GetSensorMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.08.08
********************************************************************************/
PUBLIC MMIENG_SENSOR_MODE_E MMIAPIENG_GetSensorMode(void);

/********************************************************************************
 NAME:			MMIAPIENG_VibrateWinGetFocus
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_VibrateWinGetFocus(void);

/********************************************************************************
 NAME:			MMIAPIENG_IsFocusInLoopBack
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		
 DATE:			
********************************************************************************/
//PUBLIC BOOLEAN MMIAPIENG_IsFocusInLoopBack(void);

/********************************************************************************
 NAME:			MMIAPIENG_BandSelectCnf
 DESCRIPTION:	
 PARAM IN:		None 
 PARAM OUT:		None
 AUTHOR:		songbin.zeng
 DATE:			2006.11.16
********************************************************************************/
PUBLIC void MMIAPIENG_BandSelectCnf(DPARAM  param);

/********************************************************************************
 NAME:			MMIAPIENG_GetDVFrameFre
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC MMIENG_DV_FRAME_FRE_E MMIAPIENG_GetDVFrameFre(void);

/********************************************************************************
 NAME:			MMIAPIENG_DoIdleDialNumOpt
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC void MMIAPIENG_DoIdleDialNumOpt(
                                       MMIENG_IDLE_DIAL_NUM_TYPE_E opt_type
                                       );

/********************************************************************************
 NAME:			MMIAPIENG_ParseIdleDialNumString
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_ParseIdleDialNumString(
                                                uint8* num_str_ptr, 
                                                uint8 num_str_len,
                                                MMIENG_IDLE_DIAL_NUM_TYPE_E *opt_type_ptr,//OUT
                                                parsCtrlCodeE *ctrl_code_ptr//OUT
                                                );

/********************************************************************************
 NAME:			MMIAPIENG_GetChipTestMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		ryan.xu
 DATE:			2007.08.08
********************************************************************************/
PUBLIC MMIENG_CHIP_TEST_SETTING_E MMIAPIENG_GetChipTestMode(void);

/*****************************************************************************/
// 	Description : start chip test
//	Global resource dependence : 
//  Author: ryan.xu
//	Note: 
/*****************************************************************************/
PUBLIC void MMIENGAPI_StartChipTest(void);

/********************************************************************************
 NAME:			MMIAPIENG_GetCallForwardQuery
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		liqing.peng
 DATE:			2007.07.06
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetCallForwardQuery(void);

/********************************************************************************
 NAME:			MMIAPIENG_GetCallForwardQuery
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		liqing.peng
 DATE:			2007.07.06
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetAutoAnswerCallSet(void);

/********************************************************************************
 NAME:			MMIAPIENG_GetIsCardLogOn
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		bin.ji
 DATE:			
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIsCardLogOn(void);

/********************************************************************************
 NAME:			MMIAPIENG_StartupAdjustCardLogOn
 DESCRIPTION:	开机调整CardLog状态
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		bin.ji
 DATE:			2007.10.11
********************************************************************************/
PUBLIC void MMIAPIENG_StartupAdjustCardLogOn(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIQMode 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC MMIENG_IQ_MODE_T MMIAPIENG_GetIQMode(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_GetIQModeStatus 
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetIQModeStatus(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_OpenIQDataWin
//	Global resource dependence : 
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenIQDataWin(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_AlertIQMode
//	Global resource dependence : 
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_AlertIQMode(void);

/*****************************************************************************/
// 	Description : Open IQ Data Query Win
//	Global resource dependence :                                
//      Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_OpenHandleIQDataQueryWin(BOOLEAN is_succeed);

/*****************************************************************************/
// 	Description : is arm log on when power on
//	Global resource dependence :                                
//      Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsUSBLOGON(void);

/*****************************************************************************/
// 	Description : is eng band select cmd
//	Global resource dependence : 
//  Author: jian.ma
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsBandSelectWinOpen(void);

/********************************************************************************
//MS00216980 cheney
 NAME:			BOOLEAN MMIAPIENG_GetCMCCTestFlag
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		cheney.chen
 DATE:			2010.12.09
********************************************************************************/
PUBLIC  BOOLEAN MMIAPIENG_GetCMCCTestFlag(void);
#endif//ENG_SUPPORT

/********************************************************************************
 NAME:			GetWapMmsSasTestMode
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		hui.zhao
 DATE:			2009.07.27
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetWapMmsSasTestMode(void);

/********************************************************************************
 NAME:			MMIAPIENG_CreateShowOutVersionWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateShowOutVersionWin(void);

/********************************************************************************
 NAME:			MMIAPIENG_StartUITest
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.23
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_StartUITest(void);

/********************************************************************************
 NAME:			MMIAPIENG_DoIdleDialNumOpt
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC void MMIAPIENG_DoIdleDialNumOpt(
                                       MMIENG_IDLE_DIAL_NUM_TYPE_E opt_type
                                       );


/********************************************************************************
 NAME:			MMIAPIENG_ParseIdleDialNumString
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.12.05
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_ParseIdleDialNumString(
                                                uint8* num_str_ptr, 
                                                uint8 num_str_len,
                                                MMIENG_IDLE_DIAL_NUM_TYPE_E *opt_type_ptr,//OUT
                                                parsCtrlCodeE *ctrl_code_ptr//OUT
                                                );

/********************************************************************************
 NAME:			MMIAPIENG_ParseIdleSetLanguageTypeNumString
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhaohui
 DATE:			2009.2.25
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_ParseIdleSetLanguageTypeNumString(
                                                           uint8* num_str_ptr, 
                                                           uint8 num_str_len,
                                                           MMISET_LANGUAGE_TYPE_E *language_type_ptr
                                                           );

/********************************************************************************
 NAME:			MMIAPIENG_DoIdleSetLanguageTypeNumOpt
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhaoghui
 DATE:			2009.2.25
********************************************************************************/
PUBLIC void MMIAPIENG_DoIdleSetLanguageTypeNumOpt(
                                                  MMISET_LANGUAGE_TYPE_E language_type
                                                  );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_InitModule(void);

/********************************************************************************
 NAME:			MMIAPIENG_StartEngineerMenu
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		allen
 DATE:			2004.09.23
********************************************************************************/
PUBLIC void MMIAPIENG_SetPktOutPut(void);

/*****************************************************************************/
//  Description : MMIAPIENG_GetUa,when eng ua is open, return the eng ua info
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetUa(MMIENG_UA_T *ua_ptr);

/*****************************************************************************/
//  Description : MMIAPIENG_SetUa
//  Global resource dependence : 
//  Author: fen.xie
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_SetUa(MMIENG_UA_T *ua_ptr);



/********************************************************************************
 NAME:			MMIAPIENG_GetCallBarring
 DESCRIPTION:	MS00186848
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		fen.xie
 DATE:			2010.07.06
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetCallBarring(void);

/********************************************************************************
 NAME:			MMIAPIENG_SetCallBarring
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		fen.xie
 DATE:			2010.07.06
********************************************************************************/
PUBLIC void MMIAPIENG_SetCallBarring(BOOLEAN is_on);

/*****************************************************************************/
// 	Description : restore factory setting of ENG
//	Global resource dependence : 
//  Author: fen.xie
//	Note:   
/*****************************************************************************/
PUBLIC void MMIAPIENG_RestoreFactorySetting(void);


#ifdef TF_LOAD_SUPPORT
/********************************************************************************
 NAME:			MMIAPIENG_CreateTFloadWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR: tao.feng
 DATE: 2010.12.16
********************************************************************************/
BOOLEAN MMIAPIENG_CreateTFloadWin(void);
#endif
/********************************************************************************
 NAME:       MMIAPIENG_CreateOTAWin
 DESCRIPTION:
 PARAM IN:
 PARAM OUT:
 AUTHOR: xiuyun.wang
 DATE: 2020.06.13
********************************************************************************/
BOOLEAN MMIAPIENG_CreateOTAWin(void);

#ifdef ENG_SUPPORT
/*****************************************************************************/
// 	Description : open cardlog win
//	Global resource dependence : 
//  Author: bin.ji
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenCardLogWin(void);

/*****************************************************************************/
// 	Description : open IQ Mode win
//	Global resource dependence : 
//  Author: wancan.you
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenIQModeOnOffWin(void);

#ifdef MBBMS_SUPPORT
/*****************************************************************************/
// 	Description : is smd counter enable
//	Global resource dependence : 
//  Author: lisa.lin
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsSMDCounterEnable(void);

/*****************************************************************************/
// 	Description : is interface testing eable
//	Global resource dependence : 
//  Author: xin.li
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsInterfaceTestingEnable(void);
#endif //MBBMS_SUPPORT

#ifdef CMMB_SUPPORT
/*****************************************************************************/
// 	Description : is smd counter enable
//	Global resource dependence : 
//  Author: lisa.lin
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsAudioServiceEnable(void);
#endif //CMMB_SUPPORT

/*****************************************************************************/
// 	Description : MMIAPIENG_OpenBrowserLogWin
//	Global resource dependence : 
//  Author: li.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenBrowserLogWin(void);

/*****************************************************************************/
//  Description : browser change url by wre
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenBrowserChangeURLWin(void);

/*****************************************************************************/
//  Description : open screenlog win
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenScreenLogWin(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_OpenUserAgentWin
//	Global resource dependence : 
//  Author: li.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenUserAgentWin(void);

/*****************************************************************************/
// 	Description : MMIAPIENG_OpenUaProfileWin
//	Global resource dependence : 
//  Author: li.li
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenUaProfileWin(void);

#endif//ENG_SUPPORT

#ifdef MMI_AUTOTEST_SUPPORT
/*****************************************************************************/
//  Description : open auto test set win
//  Global resource dependence : 
//  Author: hongbo.lan
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenAutoTestSetWin(uint8 set_id);
#endif

#ifdef MMI_ENG_WIFI_SUPPORT
/*****************************************************************************/
//  Description : open WIFI performance  win
//  Global resource dependence : 
//  Author: George.Liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenWlanPerformanceModeWin(void);

/*****************************************************************************/
//  Description : open WIFI iperf  win
//  Global resource dependence : 
//  Author: YING.XU
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenWifiIperfWin(void);


/*****************************************************************************/
//  Description : open WIFI radio frequency win
//  Global resource dependence : 
//  Author: George.Liu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_OpenWlanRadioFrequencyWin(void);
#endif


/********************************************************************************
 NAME:			MMIAPIENG_RunUITestNextStep
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:
 DATE:			
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_RunUITestNextStep(void);

/********************************************************************************
 NAME:			MMIAPIENG_SaveTestResult
 DESCRIPTION:	api of MMIAPIENG_SaveTestResult
 AUTHOR:		ying.xu
 DATE:			2010.8.10
********************************************************************************/
PUBLIC uint32 MMIAPIENG_SaveTestResult(uint32 win_id, uint32 result);

/*****************************************************************************/
//  Description :get power key flag
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetPowerKeyFlag(void);

/*****************************************************************************/
//  Description :set power key flag
//  Global resource dependence : 
//  Author:Jiaoyou.wu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetPowerKeyFlag(BOOLEAN is_enable);

#ifdef MMI_ENG_WIFI_SUPPORT
/********************************************************************************
NAME:          MMIAPIENG_IsWifiUITest
DESCRIPTION:   is wifi ui test
AUTHOR:        jian.ma
DATE:    
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_IsWifiUITest(void);
#endif

/********************************************************************************
 NAME:			MMIAPIENG_CreateShowOutVersionWin
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_CreateShowVersionWin(void);

/********************************************************************************
 NAME:			MMIENGSET_AppendListItemByTextIdExt
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		songbin.zeng
 DATE:			2006.10.11
********************************************************************************/
PUBLIC void MMIENGSET_AppendListItemByTextIdExt(                                    
                                                MMI_TEXT_ID_T        text_id,
                                                MMI_TEXT_ID_T        left_softkey_id,
                                                MMI_TEXT_ID_T        middle_softkey_id,
                                                MMI_TEXT_ID_T        right_softkey_id,
                                                MMI_CTRL_ID_T        ctrl_id,
                                                GUIITEM_STYLE_E      item_style
                                                );
/********************************************************************************
 NAME:			MMIAPIENG_MainWinEnter
 DESCRIPTION:	access point
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		chunjuan.liang 
 DATE:			2020.4.4
********************************************************************************/
PUBLIC void MMIAPIENG_MainWinEnter(uint8 timer_id, uint32 param);
#ifdef BLUETOOTH_SUPPORT
/********************************************************************************
 NAME:			MMIENG_UITestBTIsOpenWin
 DESCRIPTION:	BT win is open
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		chunjuan.liang 
 DATE:			2020.4.10
********************************************************************************/
PUBLIC BOOLEAN MMIENG_UITestBTIsOpenWin(void);
/********************************************************************************
 NAME:			MMIENG_UITestBT_ON_CNF
 DESCRIPTION:	recieve msg
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		chunjuan.liang 
 DATE:			2020.4.10
********************************************************************************/
PUBLIC void MMIENG_UITestBT_ON_CNF(BT_MSG_T *msg_body_ptr,uint16 msg_id);
#endif
/********************************************************************************
 NAME:			MMIENG_log
 DESCRIPTION:	recieve msg
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		chunjuan.liang 
 DATE:			2020.4.10
********************************************************************************/
PUBLIC void MMIENG_OpenSwitchLogWin(uint32 menu_id);
/********************************************************************************
 NAME:			MMIENG_log
 DESCRIPTION:	recieve msg
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		chunjuan.liang 
 DATE:			2020.4.10
********************************************************************************/
PUBLIC void MMIENG_OpenLogToPCWin(uint32 menu_id);

/********************************************************************************
 NAME:			MMIENG_OpenUartLogWin
 DESCRIPTION:	open window to choose the status of uart log
 PARAM IN:		None
 PARAM OUT:		None
 AUTHOR:		xiaotong.su
 DATE:			2021.5.24
********************************************************************************/
PUBLIC void MMIENG_OpenUartLogWin();

/****************************************************************************/
//  Description : Eng Get Log Uart Is Open
//  Global resource dependence :
//  Author:xiaotong.su
//  Note:uart log是否open，返回值为TRUE,uart log打开，返回值为FALSE,uart log关闭
/****************************************************************************/
PUBLIC BOOLEAN MMIENG_GetLogUartIsOpen(void);

/****************************************************************************/
//  Description : Eng Set Log Uart Is Open
//  Global resource dependence :
//  Author:xiaotong.su
//  Note:uart log是否open，返回值为TRUE,uart log打开，返回值为FALSE,uart log关闭
/****************************************************************************/
PUBLIC void MMIENG_SetLogUartIsOpen(BOOLEAN is_open);
/****************************************************************************/
//  Description : Eng init uart port,it is for sensor hub or it is for uart log
//                default is sensor hub
//  Global resource dependence :
//  Author:changli.jiang
//  Note:
/****************************************************************************/
PUBLIC void MMIENG_InitUartPort(void);

/*****************************************************************************/
//  Description :set Register Ps Service
//  Parameter: [In] is_enable: True: register PS service
//                             False:not register PS service
//             [Out] None
//             [Return] None
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIENG_SetRegisterPsServiceEnable( BOOLEAN is_enable );

/*****************************************************************************/
//  Description :Get Register Ps Service enable
//  Parameter: [In] None
//             [Out] None
//             [Return] is_enable: True: register PS service
//                                 False:not register PS service
//  Author: tingting.zheng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetRegisterPsServiceEnable( void );

/********************************************************************************
 NAME:          MMIENGBT_HandleBLEMsgProcess
 AUTHOR:        chunjuan.liang
 DATE:          2020.6.22
********************************************************************************/
#ifdef BT_BLE_SUPPORT
PUBLIC MMI_RESULT_E MMIENGBT_HandleBLEMsgProcess(uint16 msg_id, const BT_MSG_T *msg_body_ptr);
#endif
/*****************************************************************************/
//  Description :MMIAPIENG_GetRole
//  Parameter: [In] None
//             [Out] None
//             [Return] 1: phone
//                      0:dialer
//  Author: chunjuan.liang
//  Note:
/*****************************************************************************/
PUBLIC uint8  MMIAPIENG_GetRole( void );

PUBLIC BOOLEAN MMIAPIENG_GetMemCheckFlag(void);

PUBLIC void MMIENG_QrCode_Enter(void);

PUBLIC BOOLEAN MMIENGFacotry_IsTestMode(void);

PUBLIC BOOLEAN MMIENG_IsMonkeyTestMode(void);

#ifdef ADULT_WATCH_SUPPORT 
/*****************************************************************************/
//  Description : get bbm view visible
//  Parameter: [In] None
//             [Out] BOOLEAN
//             [Return] None
//  Author: chunjuan.liang
//  Note:TRUE:visible;FALSE:not visible
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetBBMViewVisible(void);
/*****************************************************************************/
//  Description : get call forward enable
//  Parameter: [In] None
//             [Out] BOOLEAN
//             [Return] None
//  Author: chunjuan.liang
//  Note:TRUE:enable;FALSE:disable
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIENG_GetCallForwardEnable(void);
#endif

PUBLIC void GprsTest_SetUsedSim(int simId);

PUBLIC BOOLEAN GprsTest_RunPdpActive(void);

PUBLIC void GprsTest_RunPdpDeActive(void);

#ifdef __cplusplus
    }
#endif
#endif
