/*****************************************************************************
** File Name:      mmiengtd_win.c                                              *
** Author:                                                                   *
** Date:           10/2007                                                   *
** Copyright:      2007 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe td engineering mode                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 10/2007         yongwei.he

******************************************************************************/
#include "mmi_app_engtd_trc.h"
#ifdef ENGTD_SUPPORT
#define _MMIENGTD_WIN_C_

/**--------------------------------------------------------------------------*
		                        Include Files      
 **--------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "mmk_type.h"
#include "mmk_timer.h"
#include "mmipub.h"
#include "window_parse.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "guilcd.h"
#include "guicommon.h"
#include "guitext.h"
#include "guilabel.h"
#include "guifont.h"
#include "tb_dal.h"
#include "mn_api.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guiprgbox.h"
#ifndef WIN32
#include "adc_parameter.h"
#endif
//#include "mmiset.h"
#include "mmiset_export.h"
#ifndef _WIN32
#include "production_test.h"
#endif
#include "guilistbox.h"
#include "mn_type.h"
#include "efs.h"
#include "mmi_textfun.h"
//#include "layer1_engineering.h"
//#include "mmiset.h"

//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmiengtd_win.h"
#include "mmieng_id.h"
#include "mmieng_text.h"
#include "mmiengtd_text.h"
#include "mmiengtd_id.h"
//#include "mmiengtd_main.h"
#include "engineer.h"
#include "mnnv_api.h"
#include "mmiengtd_menutable.h"
#include "mmi_nv.h"
#include "mmiengtd_nv.h"
#include "mmieng_Dynamic_main.h"// @yongwei.he add 2007-04-11
#ifndef WIN32
#include "arm_reg.h"
#else
#include "com_drv.h"
#endif
#ifdef VT_SUPPORT
#ifndef _WIN32
#include "sci_mplapi.h"
#endif
#endif
//#include "mm_ph_context.h"  //@yongwei.he cr94216 uplmn
#include "mmieng_win.h"
#include "guiedit.h"
#include "guiim_base.h"
#include "mmieng_nv.h"
#include "mmidcd_export.h"
//#include "sio_api.h"
#include "guiform.h"
#include "guictrl_api.h"
#ifdef VT_SUPPORT
#include "mmivt_export.h"
#endif
#include "sio.h"     		
//#include "mmieng_main.h"	
#include "ref_param.h"
//#include "L1c_ExportInterface.h"

#include "Mmieng_internal.h"
//#ifndef (PLATFORM_SC8800H)	//dont know the reason that 8800h project can't include this header file
//#include "sci_mplapi.h"   
//#endif
/**--------------------------------------------------------------------------*
			                         MACRO DEFINITION
 **--------------------------------------------------------------------------*/

#define MMIENG_DRXCYCLE_LEN     5
//@xiefen add 
#define ENG_MAX_APN_LEN                   16
//#define MMIENG_MAX_APN_LEN                   16
#define ENG_MAX_PICH_LEN                   3
#define MMIENG_DEFAULT_THRESHOLD  50
#define MMIENG_DEFAULT_PERIODS     10
#define MMIENG_DEFAULT_TIMER       1
#define MMIENG_UART0BAUDRATE_NUM 2
#define MMIENG_UART1BAUDRATE_NUM 2
#define MMIENG_PSBITRATE_SEL_NUM	11//@zhaohui,cr108979
#define MMIENG_3SECOND  3000

/**--------------------------------------------------------------------------*
			                         TYPE AND CONSTANT
 **--------------------------------------------------------------------------*/
//@yongwei.he add for cr94216
#define MN_MAX_PLMN_NUM 20
typedef enum
{   
    SIM_ACCESS_TYPE_UTRAN,
    SIM_ACCESS_TYPE_GSM,
    SIM_ACCESS_TYPE_OTHER
}sim_access_type_enum;

typedef struct _MMI_PLMN_INFO_T
{
	uint8	aucPlmn[3];   
	sim_access_type_enum	 access_type_en;
} MMI_PLMN_INFO_T;

typedef struct _MMI_HPLMN_WACT_T
{
	uint8	plmn_num;
	MMI_PLMN_INFO_T	plmn_info[MN_MAX_PLMN_NUM];
} MMI_HPLMN_WACT_T;

typedef struct
{
    uint32 val;
    sim_access_type_enum type;
}UPLMN_T;

typedef struct
{
uint32 id;
uint32 access_type;
}MMI_UPLMN_DISP_T;
/**--------------------------------------------------------------------------*
			                         STATIC DEFINITION
 **--------------------------------------------------------------------------*/

static MN_PHONE_BAND_TYPE_E s_mmieng_prev_band = MN_PHONE_BAND_GSM;
//@shangke add
	int16 g_threshold = 0;
    uint32 g_periods = 0;
	uint8 g_timer = 0;
    
LOCAL uint8 s_mmiengtd_timer_id = 0;
//LOCAL uint8 s_mmiengtd_up_down_link = 0;    /*lint !e752 !e551 */
LOCAL uint32 s_fallback_stat = 0;
LOCAL uint32 s_answerfast_stat = 0;
LOCAL uint32 s_support_gsmsim_stat = 0;//@zhaohui,cr109170
#ifdef WIN32
#define DSP_Assert_Reset(x)
#endif
LOCAL MMI_UPLMN_DISP_T s_uplmn_disp_arr[MN_MAX_PLMN_NUM] = {0};

LOCAL BOOLEAN s_is_dsppara_log_on = FALSE;//BUG139030

UPLMN_T s_uplmn = {46000, SIM_ACCESS_TYPE_GSM};    
//@yongwei.he add end for cr94216
extern uint8       DMNV_GetDRXCycleLen(void);
extern BOOLEAN     DMNV_SetDRXCycleLen(uint8 drx_cycle_len);

//extern uint32 SIO_GetBaudRate(uint32 port);
extern const MMI_ENG_DISPLAY_T mmi_eng_display;
//extern BOOLEAN REFPARAM_GetMixMode(void);
//extern BOOLEAN REFPARAM_SetMixMode(BOOLEAN on);
extern void TD_MNEM_SetCommandTypeAndParam(uint32 cmd_type,int16 para0, int16 para1,int16 para2,  int16 para3);

ERR_MNGPRS_CODE_E MNGPRS_SetPdpContextAPN( /* return whether success or failure    */
                   MN_DUAL_SYS_E  dual_sys,
                   uint8 *apn_ptr           /* access point name,this depends your network operator*/
        );

#ifdef DM_SUPPORT
//extern PUBLIC void MMIAPIDM_SetReg(BOOLEAN enable);           
#endif
/**--------------------------------------------------------------------------*
			                         LOCAL FUNCTION DECLARE
 **--------------------------------------------------------------------------*/
LOCAL void GetAPNSetting(
						 uint8 *APNcn_ptr
						 );

LOCAL void SetAPNSetting(
						 uint8 *APNcn_ptr
						 );

/********************************************************************************
 NAME:			HandleFallbackWinMsg
 DESCRIPTION:	
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleRecordAudioTypeWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );     
/********************************************************************************
 NAME:			AppendSetFallbackSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL void AppendRecordAudioTypeSelectItem(void);
/********************************************************************************
 NAME:			SaveFallbackSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL void SaveRecordAudioTypeSelectItem(void);
/********************************************************************************
 NAME:			HandleAnswerFastWinMsg
 DESCRIPTION:	
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleAnswerFastWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            ); 
/********************************************************************************
 NAME:			HandleAnswerFastWinMsg
 DESCRIPTION:	
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleRequestModeWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );

/********************************************************************************
 NAME:			HandleAnswerFastWinMsg
 DESCRIPTION:	
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleAdaptLayerWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );

/********************************************************************************
 NAME:			HandleAnswerFastWinMsg
 DESCRIPTION:	
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleVTVideoSetting(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );
/********************************************************************************
 NAME:			AppendAnswerFastSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL void AppendAnswerFastSelectItem(void);
/********************************************************************************
 NAME:			AppendAnswerFastSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL void SetListBoxItems(MMI_WIN_ID_T win_id, 
						   MMI_CTRL_ID_T ctrl_id, 
						   GUIITEM_STYLE_E item_style, 
						   wchar *item_str_array[], 
						   uint16 item_num);

/********************************************************************************
 NAME:			AppendAnswerFastSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL void AppendVTVideoSettingSelectItem(void);
/********************************************************************************
 NAME:			SaveAnswerFastSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL void SaveVTVideoSettingSelectItem(void);
/********************************************************************************
 NAME:			SaveAnswerFastSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL void SaveAnswerFastSelectItem(void);
/********************************************************************************
 NAME:			ClearENGClientBG
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL void ClearENGClientBG(void);

/********************************************************************************
 NAME:			字符串转化为数字
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL uint32 ConvertString2Digit(
                                 uint8 *str_ptr,    //in
                                 uint16 length  //in
                                 );

/********************************************************************************
 NAME:			数字转化为字符串
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL void ConvertDigit2String(
                               uint8 *str_ptr,        //out
                               uint16 *length_ptr,    //out
                               uint32 num //in
                               );

/********************************************************************************
 NAME:			计算数字的幂数
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL uint32 calexp(uint32 num);

//LOCAL void ShowSelectBandResult(BOOLEAN result);

//@xiefen add begin 
LOCAL MMI_RESULT_E EngNetworkSetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);//Brand

LOCAL MMI_RESULT_E EngIMSISetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);//Brand
LOCAL MMI_RESULT_E EngFreqSetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);//Brand
LOCAL MMI_RESULT_E EngShowNetworkInfoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);//@sam.xia

LOCAL int GetNetworkInfoStr(uint8* network_buf, uint8 buf_len);


#ifndef WIN32
//extern PUBLIC  uint32 REFPARAM_GetUartMode(void);
//extern PUBLIC  uint32 REFPARAM_SetUartMode(uint32 sel_num);
#ifndef TD_AS_NOT_PRESENT
extern void DSP_Assert_Reset(int16 dsp_id);
#endif

extern BOOLEAN mmi_write_hplmn_wact_file(int32 multi_sys, MMI_HPLMN_WACT_T *hpmln_wact_ptr);
extern BOOLEAN mmi_read_hplmn_wact_file(int32 multi_sys);
extern BOOLEAN mmi_get_hplmn_wact_file(int32 multi_sys, MMI_HPLMN_WACT_T *hpmln_wact_ptr);

//extern uint32 SIO_SetBaudRate(uint32 port, uint32 baud_rate);
#endif

/********************************************************************************
 NAME:			UART1BaudRateWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E UART1BaudRateWinHandleMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        );

/********************************************************************************
 NAME:			AppendUART1BaudRateItem
 DESCRIPTION:	
 PARAM IN:		list_id - 
 PARAM OUT:		
 AUTHOR:		
 DATE:			
********************************************************************************/
LOCAL void AppendUART1BaudRateItem(void);

/********************************************************************************
 NAME:			SaveUART1BaudRateSelect
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		
 DATE:			
********************************************************************************/
LOCAL void SaveUART1BaudRateSelect(void);

/********************************************************************************
 NAME:			UART0BaudRateWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E UART0BaudRateWinHandleMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        );

/********************************************************************************
 NAME:			AppendUART0BaudRateItem
 DESCRIPTION:	
 PARAM IN:		list_id - 
 PARAM OUT:		
 AUTHOR:		
 DATE:			
********************************************************************************/
LOCAL void AppendUART0BaudRateItem(void);

/********************************************************************************
 NAME:			SaveUART0BaudRateSelect
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		
 DATE:			
********************************************************************************/
LOCAL void SaveUART0BaudRateSelect(void);

LOCAL MMI_RESULT_E EngServiceTypeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL MMI_RESULT_E EngSMSPathWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

WINDOW_TABLE(MMIENG_UART0BAUDRATE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)UART0BaudRateWinHandleMsg),
    WIN_ID(MMIENG_UART0BAUDRATE_WIN_ID),
    WIN_TITLE(TXT_ENG_UART0BAUDRATE),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_UART0BAUDRATE_LISTBOX_CTRL_ID),
    END_WIN
};

WINDOW_TABLE(MMIENG_UART1BAUDRATE_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)UART1BaudRateWinHandleMsg),
    WIN_ID(MMIENG_UART1BAUDRATE_WIN_ID),
    WIN_TITLE(TXT_ENG_UART1BAUDRATE),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_UART1BAUDRATE_LISTBOX_CTRL_ID),
    END_WIN
};

/********************************************************************************
 NAME:			SaveASNSelect
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL void SaveASNSelect(void);

/********************************************************************************
 NAME:			AppendASNSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL void AppendASNSelectItem(void);

/********************************************************************************
 NAME:			EngASNSelectWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E EngASNSelectWinHandleMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );

/********************************************************************************
 NAME:			EngDRXCycleWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E EngDRXCycleWinHandleMsg(
                                           MMI_WIN_ID_T win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM param
                                           );

/*****************************************************************************/
// 	Description : handle message of input apn window of EnG
//	Global resource dependence : none
//  Author: wancan.you
//	Note:11/17/2005
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleApnSettingWinMsg(
											 MMI_WIN_ID_T	win_id, 
											 MMI_MESSAGE_ID_E	msg_id, 
											 DPARAM				param
											 );

/*****************************************************************************/
// 	Description : clear lcd
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void ClearClientLcd(void);
/********************************************************************************
 NAME:			EngShowLayer1MonitorWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		xiongxiaoyan
 DATE:			2006.01.09
********************************************************************************/
LOCAL MMI_RESULT_E EngTimeslotWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/********************************************************************************
 NAME:			EngShowLayer1MonitorWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		xiongxiaoyan
 DATE:			2006.01.09
********************************************************************************/
LOCAL MMI_RESULT_E EngDpchWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/********************************************************************************
 NAME:			EngShowLayer1MonitorWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		xiongxiaoyan
 DATE:			2006.01.09
********************************************************************************/
LOCAL MMI_RESULT_E EngBlerWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/********************************************************************************
 NAME:			EngShowLayer1MonitorWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		xiongxiaoyan
 DATE:			2006.01.09
********************************************************************************/
LOCAL MMI_RESULT_E EngOtherWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/********************************************************************************
 NAME:			EngShowLayer1MonitorWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		xiongxiaoyan
 DATE:			2006.01.09
********************************************************************************/
LOCAL MMI_RESULT_E EngLyTwoInfoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/********************************************************************************
 NAME:			EngScellWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		xiongxiaoyan
 DATE:			2006.01.09
********************************************************************************/
LOCAL MMI_RESULT_E EngScellWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/********************************************************************************
 NAME:			EngNcellWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		xiongxiaoyan
 DATE:			2006.01.09
********************************************************************************/
LOCAL MMI_RESULT_E EngNcellWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/********************************************************************************
 NAME:			EngShowLayer1MonitorWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		xiongxiaoyan
 DATE:			2006.01.09
********************************************************************************/
LOCAL MMI_RESULT_E EngNASMMWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/********************************************************************************
 NAME:			EngShowLayer1MonitorWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		xiongxiaoyan
 DATE:			2006.01.09
********************************************************************************/
LOCAL MMI_RESULT_E EngNASGmmWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

//@xiefen add end 

//@maggie add begin
/********************************************************************************
 NAME:			ENGVTSettingMenuWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		maggie.ren
 DATE:			2006.09.26
********************************************************************************/
//LOCAL MMI_RESULT_E ENGVTSettingMenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/********************************************************************************
 NAME:			EngVTAudioWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		maggie.ren
 DATE:			2006.09.26
********************************************************************************/
LOCAL MMI_RESULT_E EngVTAudioWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/********************************************************************************
 NAME:			EngVTVideoWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		maggie.ren
 DATE:			2006.09.26
********************************************************************************/
LOCAL MMI_RESULT_E EngVTVideoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/********************************************************************************
 NAME:			AppendVTAudioKeyItem
 DESCRIPTION:	
 PARAM IN:		list_id - 
 PARAM OUT:		None
 AUTHOR:		maggie.ren
 DATE:			2006.09.26
********************************************************************************/
LOCAL void AppendVTAudioKeyItem(MMI_CTRL_ID_T list_id);
/********************************************************************************
 NAME:			AppendVTVideoKeyItem
 DESCRIPTION:	
 PARAM IN:		list_id - 
 PARAM OUT:		None
 AUTHOR:		maggie.ren
 DATE:			2006.09.26
********************************************************************************/
LOCAL void AppendVTVideoKeyItem(MMI_CTRL_ID_T list_id);

//@maggie add end

//@maggie for cr58359 begin    EngShowChipIDWinHandleMsg
/********************************************************************************
 NAME:			EngShowChipIDWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		maggie.ren
 DATE:			2006.09.28
********************************************************************************/
LOCAL MMI_RESULT_E EngShowChipIDWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
//@maggie for cr58359 end

//@maggie for cr58510 add begin
/********************************************************************************
 NAME:			EngArmLogOutPathSelectWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		maggie
 DATE:			2006.9.30
********************************************************************************/
LOCAL MMI_RESULT_E EngArmLogOutPathSelectWinHandleMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );

/********************************************************************************
 NAME:			AppendArmLogOutPathSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		maggie
 DATE:			
********************************************************************************/
LOCAL void AppendArmLogOutPathSelectItem(void);
/********************************************************************************
 NAME:			SaveArmLogOutPathSelect
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		maggie
 DATE:			
********************************************************************************/
LOCAL void SaveArmLogOutPathSelect(void);

//@maggie for cr58510 add end

//@maggie add begin
/********************************************************************************
 NAME:			HandleDspSettingWinMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		maggie ren
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleDspSettingWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );

//@maggie begin end

/********************************************************************************
 NAME:			HandleRoamParamWinMsg
 DESCRIPTION:	handle roam param windows
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		vincent shang
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleRoamParamWinMsg(//@shangke
							MMI_WIN_ID_T win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM param
                                           );
/********************************************************************************
 NAME:			HandlePichSettingWinMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandlePichSettingWinMsg(
							MMI_WIN_ID_T win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM param
                                           );
/********************************************************************************
 NAME:			EngUart0UsedByWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E EngUart0UsedByWinHandleMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );

/********************************************************************************
 NAME:			AppendUart0UsedBySelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL void AppendUart0UsedBySelectItem(void);

/********************************************************************************
 NAME:			SaveUart0UsedBySelect
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL void SaveUart0UsedBySelect(void);
/********************************************************************************
 NAME:			HandleL1DSPParamSetWinMsg
 DESCRIPTION:	
 AUTHOR:		jinju.ma
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleL1DSPParamSetWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );
/********************************************************************************
 NAME:			HandleDSPParamLogWinMsg
 DESCRIPTION:	
 AUTHOR:		jinju.ma
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleDSPParamLogWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );
/********************************************************************************
 NAME:			HandleDSPParamWinMsg
 DESCRIPTION:	
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleDSPParamWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );
/********************************************************************************
 NAME:			HandleUPLMNSettingWinMsg
 DESCRIPTION:	
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleUPLMNSettingWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );    
/********************************************************************************
 NAME:			HandleUPLMNEditWinMsg
 DESCRIPTION:	
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleUPLMNEditWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            );       
/********************************************************************************
 NAME:			HandlePSBitrateSelWinMsg
 DESCRIPTION:	
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
//LOCAL MMI_RESULT_E HandlePSBitrateSelWinMsg(
//                                            MMI_WIN_ID_T win_id,
//                                            MMI_MESSAGE_ID_E msg_id,
//                                            DPARAM param
//                                            );    
/********************************************************************************
 NAME:			AppendPSBitrateSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
//LOCAL void AppendPSBitrateSelectItem(void);
/********************************************************************************
 NAME:			SavePSBitrateSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
//LOCAL void SavePSBitrateSelectItem(void);

//@zhaohui,cr106076,start
/********************************************************************************
 NAME:			EngUart0MixModeWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		zhaohui
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E EngUart0MixModeWinHandleMsg(
               			MMI_WIN_ID_T win_id,
                			MMI_MESSAGE_ID_E msg_id,
                			DPARAM param
					);
					
/********************************************************************************
 NAME:			AppendUart0MixModeSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhaohui
 DATE:	
  NOTE:	
********************************************************************************/
LOCAL void AppendUart0MixModeSelectItem(void);

/********************************************************************************
 NAME:			SaveUart0MixModeSelect
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:			zhaohui
 DATE:			
********************************************************************************/
LOCAL void SaveUart0MixModeSelect(void);
//@zhaohui,cr106076,end

/********************************************************************************
 NAME:			AppendListItemByTextBuffer
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhaohui
 DATE:			
********************************************************************************/
void AppendListItemByTextBuffer(GUILIST_ITEM_DATA_T itemdata,
	uint8 *str_ptr,
	uint16 str_len,
	BOOLEAN is_ucs2
);

//@zhaohui,cr109170 start
/********************************************************************************
 NAME:			HandleSuppertGsmSimWinMsg
 DESCRIPTION:	
 AUTHOR:		zhaohui
 DATE:			
********************************************************************************/
//LOCAL MMI_RESULT_E HandleSupportGsmSimWinMsg(
//                                            MMI_WIN_ID_T win_id,
//                                            MMI_MESSAGE_ID_E msg_id,
//                                            DPARAM param
//                                            );
                                            
/********************************************************************************
 NAME:			AppendSupportGsmSimSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhaohui
 DATE:			
********************************************************************************/
//LOCAL void AppendSupportGsmSimSelectItem(void);

/********************************************************************************
 NAME:			SaveSupportGsmSimSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhaohui
 DATE:			
********************************************************************************/
//LOCAL void SaveSupportGsmSimSelectItem(void);   
/********************************************************************************
 NAME:			SaveSupportGsmSimSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhaohui
 DATE:			
********************************************************************************/
//LOCAL void EnableRRMTest(BOOLEAN enable);
//@zhaohui,cr109170 end
/**--------------------------------------------------------------------------*
			                         WINTAB DEFINITION
 **--------------------------------------------------------------------------*/

//@xiefen add begin
//Brand
WINDOW_TABLE(MMIENG_NETWORK_SETTING_WIN_TAB) = 
{
	WIN_FUNC((uint32)EngNetworkSetWinHandleMsg),
	WIN_ID(MMIENG_NETWORKSETTING_WIN_ID),
	WIN_TITLE(TXT_ENG_NETWORKSETTING),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIENG_NETWORKSET_FORM_CTRL_ID),
	
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_NETWORKSET_FORM1_CTRL_ID,MMIENG_NETWORKSET_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_NETWORKSET_LABEL1_CTRL_ID, MMIENG_NETWORKSET_FORM1_CTRL_ID),
	    CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_NETWORKSET_EDIT1_CTRL_ID, MMIENG_NETWORKSET_FORM1_CTRL_ID),	

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_NETWORKSET_FORM2_CTRL_ID,MMIENG_NETWORKSET_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_NETWORKSET_LABEL2_CTRL_ID, MMIENG_NETWORKSET_FORM2_CTRL_ID),
    	CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_NETWORKSET_EDIT2_CTRL_ID, MMIENG_NETWORKSET_FORM2_CTRL_ID),
	
	END_WIN
};

WINDOW_TABLE(MMIENG_LYONETIMESLOT_WIN_TAB) = 
{
	WIN_FUNC((uint32)EngTimeslotWinHandleMsg),
	WIN_ID(MMIENG_TIMESLOT_WIN_ID),
	WIN_TITLE(TXT_ENG_LYONETIMESLOT),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENG_LYONEDPCH_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)EngDpchWinHandleMsg),
	WIN_ID(MMIENG_DPCH_WIN_ID),
	WIN_TITLE(TXT_ENG_LYONEDPCH),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENG_LYONEBLER_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)EngBlerWinHandleMsg),
	WIN_ID(MMIENG_BLER_WIN_ID),
	WIN_TITLE(TXT_ENG_LYONEBLER),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENG_LYONEOTHER_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)EngOtherWinHandleMsg),
	WIN_ID(MMIENG_OTHER_WIN_ID),
	WIN_TITLE(TXT_ENG_LYONEOTHER),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENG_NETINFOLYTWO_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)EngLyTwoInfoWinHandleMsg),
	WIN_ID(MMIENG_LYTWOINFO_WIN_ID),
	WIN_TITLE(TXT_ENG_NETINFO_L2),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENG_LYTHREESCELL_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)EngScellWinHandleMsg),
	WIN_ID(MMIENG_SCELL_WIN_ID),
	WIN_TITLE(TXT_ENG_LYTHREESCELL),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENG_LYTHREENCELL_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)EngNcellWinHandleMsg),
	WIN_ID(MMIENG_NCELL_WIN_ID),
	WIN_TITLE(TXT_ENG_LYTHREENCELL),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENG_NASMM_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)EngNASMMWinHandleMsg),
	WIN_ID(MMIENG_NASMM_WIN_ID),
	WIN_TITLE(TXT_ENG_NASMM),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENG_NASGMM_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)EngNASGmmWinHandleMsg),
	WIN_ID(MMIENG_NASGMM_WIN_ID),
	WIN_TITLE(TXT_ENG_NASGMM),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENG_ASNSELECT_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)EngASNSelectWinHandleMsg),
	WIN_ID(MMIENG_ASNSELECT_WIN_ID),
	WIN_TITLE(TXT_ENG_ASNSELECT),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_ASNSELECT_LISTBOX_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENG_DRXCYCLE_WIN_TAB) = 
{
	WIN_FUNC((uint32)EngDRXCycleWinHandleMsg),
	WIN_ID(MMIENG_DRXCYCLE_WIN_ID),
	WIN_TITLE(TXT_ENG_DRXCYCLE),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),	
	CREATE_EDIT_DIGITAL_CTRL(MMIENG_DRXCYCLE_LEN, MMIENG_DRXCYCLE_EDITBOX_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENG_ENG_IMSI_WIN_TAB) = 
{
	WIN_FUNC((uint32)EngIMSISetWinHandleMsg),
	WIN_ID(MMIENG_ENG_IMSI_WIN_ID),
	WIN_TITLE(TXT_ENG_IMSI),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIENG_FORM_CTRL_ID),	
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_FORM1_CTRL_ID,MMIENG_FORM_CTRL_ID),	
	        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_LABEL1_CTRL_ID, MMIENG_FORM1_CTRL_ID),
	        CHILD_EDIT_DIGITAL_CTRL(TRUE, 18, MMIENG_EDITBOX1_CTRL_ID, MMIENG_FORM1_CTRL_ID),	
	END_WIN
};

WINDOW_TABLE(MMIENG_ENG_FREQ_WIN_TAB) = 
{
	WIN_FUNC((uint32)EngFreqSetWinHandleMsg),
	WIN_ID(MMIENG_ENG_FREQ_WIN_ID),
	WIN_TITLE(TXT_ENG_FREQ),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIENG_FORM_CTRL_ID),	
	CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_LABEL1_CTRL_ID, MMIENG_FORM_CTRL_ID),		
	CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_EDITBOX1_CTRL_ID, MMIENG_FORM_CTRL_ID ),
	CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_EDITBOX2_CTRL_ID, MMIENG_FORM_CTRL_ID ),
	CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_EDITBOX3_CTRL_ID, MMIENG_FORM_CTRL_ID ),
	CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_EDITBOX4_CTRL_ID, MMIENG_FORM_CTRL_ID ),

	END_WIN
};

WINDOW_TABLE(MMIENG_ENG_SIMSIMULATE_WIN_TAB) = 
{
	WIN_FUNC((uint32)EngIMSISetWinHandleMsg),
	WIN_ID(MMIENG_ENG_IMSI_WIN_ID),
	WIN_TITLE(TXT_ENG_SIMSIMULATE),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIENG_FORM_CTRL_ID),	
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_FORM1_CTRL_ID,MMIENG_FORM_CTRL_ID),
	        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_LABEL1_CTRL_ID, MMIENG_FORM1_CTRL_ID),
	        CHILD_EDIT_DIGITAL_CTRL(TRUE, 18, MMIENG_EDITBOX1_CTRL_ID, MMIENG_FORM1_CTRL_ID),	
	END_WIN
};

WINDOW_TABLE(MMIENG_UART0USEDBY_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)EngUart0UsedByWinHandleMsg),
	WIN_ID(MMIENG_UART0USEDBY_WIN_ID),
	WIN_TITLE(TXT_ENG_NETWORKSETTING_INFO),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E,MMIENG_UART0USEDBY_LISTBOX_CTRL_ID),
	END_WIN
};

//@zhaohui,cr106076
WINDOW_TABLE(MMIENGTD_UART0MIXMODE_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)EngUart0MixModeWinHandleMsg),
	WIN_ID(MMIENG_UART0MIXMODE_WIN_ID),
	WIN_TITLE(TXT_ENG_UART0MIXMODE),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_UART0MIXMODE_LISTBOX_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENG_SHOWNETSETINFO_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)EngShowNetworkInfoWinHandleMsg),
	WIN_ID(MMIENG_SHOWNETWORKINFO_WIN_ID),
	WIN_TITLE(TXT_ENG_NETWORKSETTING_INFO),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_TEXT_CTRL(MMIENG_TEXTBOX1_CTRL_ID),
	END_WIN
};

// APN Set
WINDOW_TABLE(MMIENG_APN_SETTING_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleApnSettingWinMsg),    
    WIN_ID(MMIENG_APN_SETTING_WIN_ID),
    WIN_TITLE(TXT_ENG_APN_SETTING),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_TEXT_CTRL(MMIENG_MAX_APN_LEN,MMIENG_APN_SETTING_CTRL_ID),
    END_WIN
};
//@xiefen add end

/*
//@maggie add begin
WINDOW_TABLE(MMIENG_SHOWVT_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)ENGVTSettingMenuWinHandleMsg),
	WIN_ID(MMIENG_VT_SETTING_WIN_ID),
    WIN_TITLE(TXT_ENG_VT_SET),
	CREATE_MENU_CTRL(MENU_ENG_VT, MMIENG_VTMENU_CTRL_ID),
	END_WIN
};

//打开/关闭语音编解码功能
WINDOW_TABLE(MMIENG_VT_AUDIO_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)EngVTAudioWinHandleMsg),
	WIN_ID(MMIENG_VTAUDIO_WIN_ID),
	WIN_TITLE(TXT_ENG_AUDIO),
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
	END_WIN
};
//打开/关闭视频编解码功能
WINDOW_TABLE(MMIENG_VT_VIDEO_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)EngVTVideoWinHandleMsg),
	WIN_ID(MMIENG_VTVIDEO_WIN_ID),
	WIN_TITLE(TXT_ENG_VIDEO),
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_LISTBOX_CTRL_ID),
	END_WIN
};
//@maggie add end
*/

//@maggie for cr58359 begin  MMIENG_SHOWCHIPID_WIN_TAB
WINDOW_TABLE(MMIENG_SHOWCHIPID_WIN_TAB) = 
{
	WIN_FUNC((uint32)EngShowChipIDWinHandleMsg),
	WIN_ID(MMIENG_SHOWCHIPID_WIN_ID),
	WIN_TITLE(TXT_ENG_CHIP_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIENG_FORM_CTRL_ID),
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_FORM1_CTRL_ID,MMIENG_FORM_CTRL_ID),
	        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_LABEL1_CTRL_ID, MMIENG_FORM1_CTRL_ID),
	        CHILD_TEXT_CTRL(TRUE, MMIENG_TEXTBOX1_CTRL_ID, MMIENG_FORM1_CTRL_ID),	
	END_WIN
};
//@maggie for cr58359 end

//@maggie for cr58510 add begin
WINDOW_TABLE(MMIENG_ARMLOGOUTPATHSELECT_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)EngArmLogOutPathSelectWinHandleMsg),
	WIN_ID(MMIENG_ARMLOGOUTPATHSELECT_WIN_ID),
	WIN_TITLE(TXT_ENG_ARM_LOG_OUT_PATH),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_ARMLOGOUTPATHSELECT_LISTBOX_CTRL_ID), 
	END_WIN
};
//@maggie for cr58510 add end

//@maggie add begin
// DSP Setting的菜单窗口
WINDOW_TABLE(MMIENG_DSP_SETTING_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)HandleDspSettingWinMsg),
	WIN_ID(MMIENG_DSP_SETTING_WIN_ID),
	WIN_TITLE(TXT_ENG_DSP_SETTING),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIENG_DSPSET_FORM_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_DSPSET_FORM1_CTRL_ID,MMIENG_DSPSET_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_DSPSET_LABEL1_CTRL_ID, MMIENG_DSPSET_FORM1_CTRL_ID),
	    CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_DSPSET_EDIT1_CTRL_ID, MMIENG_DSPSET_FORM1_CTRL_ID),
        
    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_DSPSET_FORM2_CTRL_ID,MMIENG_DSPSET_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_DSPSET_LABEL2_CTRL_ID, MMIENG_DSPSET_FORM2_CTRL_ID),
	    CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_DSPSET_EDIT2_CTRL_ID, MMIENG_DSPSET_FORM2_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_DSPSET_FORM3_CTRL_ID,MMIENG_DSPSET_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE,MMIENG_DSPSET_LABEL3_CTRL_ID, MMIENG_DSPSET_FORM3_CTRL_ID),
	    CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_DSPSET_EDIT3_CTRL_ID, MMIENG_DSPSET_FORM3_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_DSPSET_FORM4_CTRL_ID,MMIENG_DSPSET_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_DSPSET_LABEL4_CTRL_ID, MMIENG_DSPSET_FORM4_CTRL_ID),
	    CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_DSPSET_EDIT4_CTRL_ID, MMIENG_DSPSET_FORM4_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_DSPSET_FORM5_CTRL_ID,MMIENG_DSPSET_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_DSPSET_LABEL5_CTRL_ID, MMIENG_DSPSET_FORM5_CTRL_ID),	
	    CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_DSPSET_EDIT5_CTRL_ID, MMIENG_DSPSET_FORM5_CTRL_ID),
	END_WIN
};

//@maggie add end

WINDOW_TABLE(MMIENG_ROAM_PARAM_WIN_TAB) = //@shangke
{
	WIN_FUNC((uint32)HandleRoamParamWinMsg),
	WIN_ID(MMIENG_ROAMPARAM_WIN_ID),
	WIN_TITLE(TXT_ENG_ROAMPARAM),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIENG_ROAMPARAM_FORM_CTRL_ID),

	CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_ROAMPARAM_FORM1_CTRL_ID,MMIENG_ROAMPARAM_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_ROAMPARAM_LABEL1_CTRL_ID, MMIENG_ROAMPARAM_FORM1_CTRL_ID),
	    CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_ROAMPARAM_EDITBOX1_CTRL_ID, MMIENG_ROAMPARAM_FORM1_CTRL_ID),	

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_ROAMPARAM_FORM2_CTRL_ID,MMIENG_ROAMPARAM_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_ROAMPARAM_LABEL2_CTRL_ID, MMIENG_ROAMPARAM_FORM2_CTRL_ID),
	    CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_ROAMPARAM_EDITBOX2_CTRL_ID, MMIENG_ROAMPARAM_FORM2_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_ROAMPARAM_FORM3_CTRL_ID,MMIENG_ROAMPARAM_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE,MMIENG_ROAMPARAM_LABEL3_CTRL_ID, MMIENG_ROAMPARAM_FORM3_CTRL_ID),
	    CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_ROAMPARAM_EDITBOX3_CTRL_ID, MMIENG_ROAMPARAM_FORM3_CTRL_ID),
	
	END_WIN
};

WINDOW_TABLE(MMIENGTD_FALLBACK_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)HandleRecordAudioTypeWinMsg),
	WIN_ID(MMIENG_RECORD_AUDIO_TYPE_WIN_ID),
	WIN_TITLE(TXT_ENG_REOCRD_AUDIO_TYPE),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_FALLBACK_LISTBOX_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENGTD_REQUESTMODE_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)HandleRequestModeWinMsg),
	WIN_ID(MMIENGTD_REQUESTMODE_WIN_ID),
	WIN_TITLE(TXT_ENG_REQUEST_MODE),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_REQUESTMODE_LISTBOX_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENGTD_ADAPTERLAYER_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)HandleAdaptLayerWinMsg),
	WIN_ID(MMIENGTD_ADAPTERLAYER_WIN_ID),
	WIN_TITLE(TXT_ENG_ADAPTER_LAYER),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LISTBOX_CTRL(GUILIST_CHECKLIST_E, MMIENG_ADAPTERLAYER_LISTBOX_CTRL_ID),
	END_WIN
};

WINDOW_TABLE(MMIENGTD_ANSWERFAST_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)HandleAnswerFastWinMsg),
	WIN_ID(MMIENG_ANSWERFAST_WIN_ID),
	WIN_TITLE(TXT_ENG_SET_VT_DEFAULT_EP),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_ANSWERFAST_LISTBOX_CTRL_ID),
	END_WIN
};
WINDOW_TABLE(MMIENGTD_VTVideoType_WIN_TAB) = 
{
	//CLEAR_LCD,
	//WIN_PRIO( WIN_ONE_LEVEL ),
	WIN_FUNC((uint32)HandleVTVideoSetting),
	WIN_ID(MMIENG_VTVIDEOTYPE_WIN_ID),
	WIN_TITLE(TXT_ENG_VIDEOTYPE),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_VTVIDEOSETTING_LISTBOX_CTRL_ID),
	END_WIN
};
//add by jinju.ma for cr139030
WINDOW_TABLE(MMIENGTD_L1DSPPARAM_SET_WIN_TAB) = 
{
	WIN_FUNC((uint32)HandleL1DSPParamSetWinMsg),
	WIN_ID(MMIENGTD_L1DSPPARAM_SET_WIN_ID),
	WIN_TITLE(TXT_ENG_L1_DSP_PARAM),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_MENU_CTRL(MENU_MMIENGTD_SET, MMIENG_L1DSPPARAM_SET_MENU_CTRL_ID),
	END_WIN
};	
WINDOW_TABLE(MMIENGTD_DSPPARAM_LOG_WIN_TAB) = 
{
	WIN_FUNC((uint32)HandleDSPParamLogWinMsg),
	WIN_ID(MMIENGTD_DSPPARAM_LOG_WIN_ID),
	WIN_TITLE(TXT_ENG_DSP_PARAM_LOG),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_DSPPARAM_LOG_LISTBOX_CTRL_ID),
	END_WIN
};	
//@yongwei.he add begin for cr93635
WINDOW_TABLE(MMIENGTD_DSPParam_WIN_TAB) = 
{
	WIN_FUNC((uint32)HandleDSPParamWinMsg),
	WIN_ID(MMIENG_DSPPARAM_WIN_ID),
	WIN_TITLE(TXT_ENG_DSP_PARAM),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),

	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIENG_DSPPARAM_FORM_CTRL_ID),

	CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_DSPPARAM_FORM1_CTRL_ID,MMIENG_DSPPARAM_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_DSPPARAM_LABEL1_CTRL_ID, MMIENG_DSPPARAM_FORM1_CTRL_ID),
	    CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_DSPPARAM_EDITBOX1_CTRL_ID, MMIENG_DSPPARAM_FORM1_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_DSPPARAM_FORM2_CTRL_ID,MMIENG_DSPPARAM_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_DSPPARAM_LABEL2_CTRL_ID, MMIENG_DSPPARAM_FORM2_CTRL_ID),
	    CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_DSPPARAM_EDITBOX2_CTRL_ID, MMIENG_DSPPARAM_FORM2_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_DSPPARAM_FORM3_CTRL_ID,MMIENG_DSPPARAM_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE,MMIENG_DSPPARAM_LABEL3_CTRL_ID, MMIENG_DSPPARAM_FORM3_CTRL_ID),
	    CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_DSPPARAM_EDITBOX3_CTRL_ID, MMIENG_DSPPARAM_FORM3_CTRL_ID),

    CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_DSPPARAM_FORM4_CTRL_ID,MMIENG_DSPPARAM_FORM_CTRL_ID),
	    CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_DSPPARAM_LABEL4_CTRL_ID, MMIENG_DSPPARAM_FORM4_CTRL_ID),
	    CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_DSPPARAM_EDITBOX4_CTRL_ID, MMIENG_DSPPARAM_FORM4_CTRL_ID),
	
	END_WIN
};
//@yongwei.he add end for cr93635
/*	+cr90316	*/
//@yongwei.he
//PICH Set
WINDOW_TABLE(MMIENG_PICH_SETTING_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandlePichSettingWinMsg),    
    WIN_ID(MMIENG_PICH_SETTING_WIN_ID),
     WIN_TITLE(TXT_ENG_PICH_SETTING),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    CREATE_EDIT_DIGITAL_CTRL(3, MMIENG_PICH_EDITBOX_CTRL_ID),    
    END_WIN
};
/*	-cr90316	*/
//@yongwei.he add for cr 94216
WINDOW_TABLE(MMIENG_UPLMN_SETTING_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleUPLMNSettingWinMsg),    
    WIN_ID(MMIENG_UPLMN_SETTING_WIN_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
	  WIN_TITLE(TXT_COMM_UPLMN_SETTING),
    CREATE_TEXT_CTRL(MMIENG_UPLMN_TEXTBOX_CTRL_ID),
  
    END_WIN
};
WINDOW_TABLE(MMIENG_UPLMN_EDIT_WIN_TAB) = 
{
    WIN_FUNC((uint32)HandleUPLMNEditWinMsg),    
    WIN_ID(MMIENG_UPLMN_EDIT_WIN_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),
    WIN_TITLE(TXT_COMM_UPLMN_SETTING),
	CREATE_FORM_CTRL(GUIFORM_LAYOUT_ORDER,MMIENG_UPLMN_FORM_CTRL_ID),	
        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_UPLMN_FORM1_CTRL_ID,MMIENG_UPLMN_FORM_CTRL_ID),
	        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_UPLMN_LABEL1_CTRL_ID, MMIENG_UPLMN_FORM1_CTRL_ID),
	        CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_UPLMN_EDITBOX1_CTRL_ID, MMIENG_UPLMN_FORM1_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_UPLMN_FORM2_CTRL_ID,MMIENG_UPLMN_FORM_CTRL_ID),
	        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE, MMIENG_UPLMN_LABEL2_CTRL_ID, MMIENG_UPLMN_FORM2_CTRL_ID),
	        CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_UPLMN_EDITBOX2_CTRL_ID, MMIENG_UPLMN_FORM2_CTRL_ID),

        CHILD_FORM_CTRL(TRUE,GUIFORM_LAYOUT_ORDER,MMIENG_UPLMN_FORM3_CTRL_ID,MMIENG_UPLMN_FORM_CTRL_ID),
	        CHILD_LABEL_CTRL(GUILABEL_ALIGN_LEFT, FALSE,MMIENG_UPLMN_LABEL3_CTRL_ID, MMIENG_UPLMN_FORM3_CTRL_ID),
    	    CHILD_EDIT_DIGITAL_CTRL(TRUE, 5, MMIENG_UPLMN_EDITBOX3_CTRL_ID, MMIENG_UPLMN_FORM3_CTRL_ID),
	
    END_WIN
};
//@yongwei.he add end
//@yongwei.he add begin for cr94781
#if 0
WINDOW_TABLE(MMIENG_PS_BITRATE_SEL_WIN_TAB) = 
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandlePSBitrateSelWinMsg),    
    WIN_ID(MMIENG_PS_BITRATE_SEL_WIN_ID),
	WIN_TITLE(TXT_ENG_PS_BITRATE),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_PSBITRATE_LISTBOX_CTRL_ID),
	WIN_SOFTKEY(TXT_COMMON_OK, TXT_NULL, STXT_RETURN),    
    END_WIN
};
#endif
//@yongwei.he add end for cr94781

/*
WINDOW_TABLE(MMIENG_SUPPORT_GSMSIM_WIN_TAB) = //@zhaohui,cr109170
{
    //CLEAR_LCD,
    //WIN_PRIO( WIN_ONE_LEVEL ),
    WIN_FUNC((uint32)HandleSupportGsmSimWinMsg),    
    WIN_ID(MMIENG_SUPPORT_GSMSIM_WIN_ID),
	WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
	WIN_TITLE(TXT_ENG_SUPPORT_GSM_SIM),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIENG_SUPPORT_GSMSIM_LISTBOX_CTRL_ID),
    
    END_WIN
};
*/
/**--------------------------------------------------------------------------*
			                         EXTERNAL DECLARE
 **--------------------------------------------------------------------------*/

//@xiefen add 4.06
#ifdef _WIN32

uint16 g_freq_num      = 4;

uint16 g_freq_list[10] = {10054,10056,10064,10072,10054,10056,10064,10072,0,0};

uint8 g_imsi[9]       = {0x08,0x49,0x06,0x70,0x00,0x00,0x00,0x00,0x81 };

uint8 g_spss_code = (0 | (1 << 7));

uint8 v_com_uart1Flag = 0;//1;
uint8 v_tgl_sleepFlag = 1;
uint8 v_tgl_rfLoopFlag = 0;


/*===============================================================================
								IMPLEMENTATION
===============================================================================*/

/*****************************************************************************/
// Description : set the flag to control the version of asn.1
//  Global resource dependence : none
//  Author:       bo.chen
//  Note:   0: asn.1 v490; 0: asn.1 v4h0; 2: asn.1 v4h0&UpPCH shifting
/*****************************************************************************/
BOOLEAN DMNV_SetASNVersionSelection(
                                    uint8 asn_version_selection
                                    )
{
    return TRUE;
}

/*****************************************************************************/
// Description : get the flag to control the version of asn.1
//  Global resource dependence : none
//  Author:       bo.chen
//  Note:   0: asn.1 v490; 0: asn.1 v4h0; 2: asn.1 v4h0&UpPCH shifting
/*****************************************************************************/
uint8 DMNV_GetASNVersionSelection(void)
{
    return 0;
}

#else
uint16 g_freq_num      = 4;
//extern uint16 g_freq_num; //@sam.xia
//extern uint16 g_freq_list[10];
extern uint8   g_imsi[9];
//extern uint8 g_spss_code;
uint8 g_spss_code = (0 | (1 << 7));

//extern uint8 v_uart1_used_by_dsp1;
uint8 v_com_uart1Flag = 0;//1;
uint8 v_tgl_sleepFlag = 1;
uint8 v_tgl_rfLoopFlag = 0;
//extern uint8 v_tgl_sleepFlag;
//extern uint8 v_tgl_rfLoopFlag;

/*****************************************************************************/
// Description : set the flag to control the version of asn.1
//  Global resource dependence : none
//  Author:       bo.chen
//  Note:   0: asn.1 v490; 0: asn.1 v4h0; 2: asn.1 v4h0&UpPCH shifting
/*****************************************************************************/
//extern BOOLEAN DMNV_SetASNVersionSelection(
//            uint8 asn_version_selection 
 //           );

/*****************************************************************************/
// Description : get the flag to control the version of asn.1
//  Global resource dependence : none
//  Author:       bo.chen
//  Note:   0: asn.1 v490; 0: asn.1 v4h0; 2: asn.1 v4h0&UpPCH shifting
/*****************************************************************************/
//extern uint8 DMNV_GetASNVersionSelection(
//            void
//            );

#endif

//@xiefen add 4.06


/*===============================================================================
								IMPLEMENTATION
===============================================================================*/
//@xiefen add begin
//extern L1_INFO_T *GetL1Info(void);
//extern L2_INFO_T *GetL2Info(void);
//extern L3_INFO_T *GetL3Info(void);
//extern NAS_INFO_T *GetNasInfo(void);

/********************************************************************************
 NAME:			AppendUart0UsedBySelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL void AppendUart0UsedBySelectItem(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIENG_UART0USEDBY_LISTBOX_CTRL_ID;
    uint16 sel_num = 0;
    MMI_STRING_T kstring[2]  = {0};
	GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
	uint32 i = 0;
    MMI_TEXT_ID_T uart0_version_arr[2] = {
        TXT_ENG_ARMREFDEBUG,TXT_ENG_ARMREFPS};

	item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
	item_t.item_data_ptr = &item_data;
	//set softkey
	item_data.softkey_id[0] = TXT_ENG_CLEAR;
	item_data.softkey_id[1] = TXT_NULL;
	item_data.softkey_id[2] = TXT_ENG_RETURN;
	
	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	GUILIST_SetMaxItem(ctrl_id, 2, FALSE );
	for (i = 0; i < 2; i++)
	{
        //set string context of item
        MMI_GetLabelTextByLang(uart0_version_arr[i], &kstring[i]);

		//AppendListItemByTextBuffer(item_data,kstring.wstr_ptr,kstring.wstr_len,FALSE);
       	item_data.item_content[0].item_data.text_buffer=kstring[i];
		GUILIST_AppendItem(ctrl_id, &item_t);
	}
	
    //get sel_num from MN
#ifndef WIN32
    sel_num = REFPARAM_GetUartMode();
#endif
    
	GUILIST_SetCurItemIndex(ctrl_id, sel_num);
	GUILIST_SetSelectedItem(ctrl_id, sel_num, TRUE);
}

/********************************************************************************
 NAME:			AppendListItemByTextBuffer
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhaohui
 DATE:			
********************************************************************************/
void AppendListItemByTextBuffer(GUILIST_ITEM_DATA_T itemdata,
	uint8 *str_ptr,
	uint16 str_len,
	BOOLEAN is_ucs2
)
{
	itemdata.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	//itemdata.item_content[0].item_data.text_buffer.is_ucs2 = is_ucs2;
	
	itemdata.item_content[0].item_data.text_buffer.wstr_len =MIN( str_len, GUILIST_STRING_MAX_NUM);
	MMI_MEMCPY(itemdata.item_content[0].item_data.text_buffer.wstr_ptr, 
		sizeof(itemdata.item_content[0].item_data.text_buffer.wstr_ptr), 
		str_ptr, 
		itemdata.item_content[0].item_data.text_buffer.wstr_len,
		itemdata.item_content[0].item_data.text_buffer.wstr_len);
}


/********************************************************************************
 NAME:			SaveUart0UsedBySelect
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL void SaveUart0UsedBySelect(void)
{
    uint16 sel_num = 0;
    
    sel_num = GUILIST_GetCurItemIndex(MMIENG_UART0USEDBY_LISTBOX_CTRL_ID);
    //set sel_num to MN
#ifndef WIN32
    REFPARAM_SetUartMode(sel_num);
#endif
    
    // 提示窗口--已完成
    /*
    PUBWIN_OpenPromptAlertWin(
        PUBWIN_SOFTKEY_ONE,
        TXT_COMPLETE,
        IMAGE_PROMPT_ICON_SUCCESS,
        MMI_3SECONDS,
        PNULL
        ); 
        */
    /*MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);*/

}

/********************************************************************************
 NAME:			ClearENGClientBG
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL void ClearENGClientBG(void)
{
    GUI_LCD_DEV_INFO	lcd_dev_info = {0};
    GUI_RECT_T rect = MMITHEME_GetClientRect();

    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);//@xiefen add for cr51412
}

/*****************************************************************************/
// 	Description : clear lcd
//	Global resource dependence : none
//  Author: wancan.you
//	Note:
/*****************************************************************************/
LOCAL void ClearClientLcd(void)
{
    GUI_LCD_DEV_INFO	lcd_dev_info = {0};
    GUI_RECT_T  win_rect = MMITHEME_GetFullScreenRect();
    GUI_COLOR_T back_color = MMI_WINDOW_BACKGROUND_COLOR;
    
    //SCI_TRACE_LOW:"ENG_ONE_CTRL_TOP()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_1690_112_2_18_2_18_40_13,(uint8*)"");

    lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    GUI_FillRect(&lcd_dev_info, win_rect, back_color);
}

/********************************************************************************
 NAME:			GetTimeslotInfoStr
 DESCRIPTION:	
 PARAM IN:		net_buf - 
				buf_len - 
 PARAM OUT:		net info string len
 AUTHOR:		xiongxiaoyan
 DATE:			2004.09.08
********************************************************************************/
LOCAL int GetTimeslotInfoStr(uint8* str_buf, uint16 buf_len)
{	
	TIMESLOT_ISCP_ARR_T timeslotinfo;
	int i;
	char tmp_buf1[51] = {0};
	uint32 offset = 0;
	L1_INFO_T	*L1info = PNULL;
    uint16 length = 0 ;
    L1info="L1info";/*lint !e64*/  //(L1_INFO_T *)GetL1Info();
	
	timeslotinfo = L1info->Timeslot_ISCP;

    if(PNULL == str_buf)
    {
        //SCI_TRACE_LOW:"mmiengtd_win.c GetTimeslotInfoStr str_buf == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_1719_112_2_18_2_18_40_14,(uint8*)"");
        return -1;
    }    
    //SCI_TRACE_LOW:"mmiengtd_win.c GetTimeslotInfoStr buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_1722_112_2_18_2_18_40_15,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }

	
	/*timeslot_iscp_number*/	
	MMI_MEMCPY(str_buf, buf_len, "TS_ISCP_Num:", 12, 12);
	offset = 12;			
	SCI_MEMSET(tmp_buf1, 0, 50);
	sprintf(tmp_buf1, "%d", timeslotinfo.timeslot_iscp_number);
	length = SCI_STRLEN(tmp_buf1) ;
	MMI_MEMCPY(str_buf + offset, buf_len - offset, tmp_buf1, 50, length);
	offset += length ;	
	str_buf[offset] = CR_CHAR;
	offset += 1;
	
	/*timeslot_iscp_arr*/	
	for(i=0;i<7;i++)
	{
	SCI_MEMSET(tmp_buf1, 0, 50);
	sprintf(tmp_buf1, "TS_ISCP[%d]:%d,%d",i, 
		timeslotinfo.timeslot_iscp_arr[i].timeslot,timeslotinfo.timeslot_iscp_arr[i].iscp);
		
	length = SCI_STRLEN(tmp_buf1) ;
	MMI_MEMCPY(str_buf + offset, buf_len - offset, tmp_buf1, 50, length);
	offset += length ;	
	str_buf[offset] = CR_CHAR;
	offset += 1;
	}	
	
	return (offset);
}

/********************************************************************************
 NAME:			EngShowLayer1MonitorWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		xiongxiaoyan
 DATE:			2006.01.09
********************************************************************************/
LOCAL MMI_RESULT_E EngTimeslotWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint8 buf[401] = {0};
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        
        text.wstr_len = GetTimeslotInfoStr(buf, 400);
        text.wstr_ptr = buf;/*lint !e64*/
        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len,FALSE);
        MMK_SetAtvCtrl(MMIENG_TIMESLOT_WIN_ID, MMIENG_TEXTBOX1_CTRL_ID);
        // create neighbour CELL refresh timer
        s_mmiengtd_timer_id = MMK_CreateWinTimer(MMIENG_TIMESLOT_WIN_ID, 1000, TRUE);
        break;
        
    case MSG_FULL_PAINT:
        break;
        
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_TIMESLOT_WIN_ID);
        break;
        
    case MSG_TIMER:
        if (*(uint8*)param == s_mmiengtd_timer_id) 
        {
            text.wstr_len = GetTimeslotInfoStr(buf, 400);
            text.wstr_ptr = buf;/*lint !e64*/
            GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len, TRUE);
            
        }
        else
            result = MMI_RESULT_FALSE;		// need additional process
        break;
        
    case MSG_LOSE_FOCUS:
        MMK_PauseTimer(s_mmiengtd_timer_id);
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_StopTimer(s_mmiengtd_timer_id);
        s_mmiengtd_timer_id = 0;
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}


/********************************************************************************
 NAME:			GetDpchInfoStr
 DESCRIPTION:	
 PARAM IN:		net_buf - 
				buf_len - 
 PARAM OUT:		net info string len
 AUTHOR:		xiongxiaoyan
 DATE:			2004.09.08
********************************************************************************/
LOCAL int GetDpchInfoStr(uint8* str_buf, uint16 buf_len)
{	
	DPCH_RSCP_ARR_T	dpchinfo;
	char tmp_buf1[51] = {0};
	uint32 offset = 0;
	int i;
	uint32 length = 0;
	L1_INFO_T	*L1info ="L1info";/*lint !e64*/// GetL1Info();

	dpchinfo = L1info->DPCH_RSCP;
	
    if(PNULL == str_buf)
    {
        //SCI_TRACE_LOW:"mmiengtd_win.c GetDpchInfoStr str_buf == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_1845_112_2_18_2_18_41_16,(uint8*)"");
        return -1;
    }    
    //SCI_TRACE_LOW:"mmieng_wintd.c GetDpchInfoStr buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_1848_112_2_18_2_18_41_17,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }
    
	/*dpch_rscp_number*/	
	MMI_MEMCPY(str_buf, buf_len, "DPCH_RSCP_Num:", 14, 14);
	offset = 14;
	
	SCI_MEMSET(tmp_buf1, 0, 50);
	sprintf(tmp_buf1, "%d", dpchinfo.dpch_rscp_number);
    length = SCI_STRLEN(tmp_buf1) ;
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 50, length);
	offset += length ;	
	str_buf[offset] = CR_CHAR;
	offset += 1;
	
	/*dpch_rscp_arr*/	
	for(i=0;i<7;i++)
	{
	SCI_MEMSET(tmp_buf1, 0, 50);
	sprintf(tmp_buf1, "DPCH_RSCP[%d]:%d,%d",i, 
		dpchinfo.dpch_rscp_arr[i].timeslot,(signed char)dpchinfo.dpch_rscp_arr[i].dpch_rscp);
    length = SCI_STRLEN(tmp_buf1) ;
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 50, length);
	offset += length ;	
	str_buf[offset] = CR_CHAR;
	offset += 1;
	}	
	
	return (offset);
}



/********************************************************************************
 NAME:			EngShowLayer1MonitorWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		xiongxiaoyan
 DATE:			2006.01.09
********************************************************************************/
LOCAL MMI_RESULT_E EngDpchWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint8 buf[401] = {0};
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        
        text.wstr_len = GetDpchInfoStr(buf, 400);
        text.wstr_ptr = buf;/*lint !e64*/
        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr, text.wstr_len, FALSE);
        MMK_SetAtvCtrl(MMIENG_DPCH_WIN_ID, MMIENG_TEXTBOX1_CTRL_ID);
        // create neighbour CELL refresh timer
        s_mmiengtd_timer_id = MMK_CreateWinTimer(MMIENG_DPCH_WIN_ID, 1000, TRUE);
        break;
        
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_DPCH_WIN_ID);
        break;
        
    case MSG_TIMER:
        if (*(uint8*)param == s_mmiengtd_timer_id) 
        {
            text.wstr_len = GetDpchInfoStr(buf, 400);
            text.wstr_ptr = buf;/*lint !e64*/
            GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len, TRUE);
            
        }
        else
            result = MMI_RESULT_FALSE;		// need additional process
        break;
        
    case MSG_LOSE_FOCUS:
        MMK_PauseTimer(s_mmiengtd_timer_id);
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_StopTimer(s_mmiengtd_timer_id);
        s_mmiengtd_timer_id = 0;
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:			GetDpchInfoStr
 DESCRIPTION:	
 PARAM IN:		net_buf - 
				buf_len - 
 PARAM OUT:		net info string len
 AUTHOR:		xiongxiaoyan
 DATE:			2004.09.08
********************************************************************************/
LOCAL int GetBlerInfoStr(uint8* str_buf, uint16 buf_len)
{	
	BLER_ARR_T	blerinfo;
	char tmp_buf1[11] = {0};
	uint32 offset = 0;
	L1_INFO_T	*L1info ="L1info";/*lint !e64*/// GetL1Info();
    uint32 length = 0;
	blerinfo = L1info->BLER;
	
    if(PNULL == str_buf)
    {
        //SCI_TRACE_LOW:"mmiengtd_win.c GetBlerInfoStr str_buf == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_1966_112_2_18_2_18_41_18,(uint8*)"");
        return -1;
    }    
    //SCI_TRACE_LOW:"mmieng_wintd.c GetBlerInfoStr buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_1969_112_2_18_2_18_41_19,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }
	
	/*tr_bler_number*/	
	MMI_MEMCPY(str_buf, buf_len, "TR_BLER_Num:", 12, 12);
	offset = 12;
	
	SCI_MEMSET(tmp_buf1, 0, 10);
	sprintf(tmp_buf1, "%d", blerinfo.tr_bler_number);
	length = SCI_STRLEN(tmp_buf1);
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 10, length);
	offset += length;	
	str_buf[offset] = CR_CHAR;
	offset += 1;	
	return (offset);
}


/********************************************************************************
 NAME:			EngShowLayer1MonitorWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		xiongxiaoyan
 DATE:			2006.01.09
********************************************************************************/
LOCAL MMI_RESULT_E EngBlerWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint8 buf[30] = {0};
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        
        text.wstr_len = GetBlerInfoStr(buf, 30);
        text.wstr_ptr = buf;/*lint !e64*/
        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len, FALSE);
        MMK_SetAtvCtrl(MMIENG_BLER_WIN_ID, MMIENG_TEXTBOX1_CTRL_ID);
        // create neighbour CELL refresh timer
        s_mmiengtd_timer_id = MMK_CreateWinTimer(MMIENG_BLER_WIN_ID, 1000, TRUE);
        break;
        
    case MSG_FULL_PAINT:
        GUIWIN_SetSoftkeyTextId(MMIENG_BLER_WIN_ID,  (MMI_TEXT_ID_T)TXT_ENG_OK,(MMI_TEXT_ID_T) TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RETURN, FALSE);
        break;
        
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_BLER_WIN_ID);
        break;
        
    case MSG_TIMER:
        if (*(uint8*)param == s_mmiengtd_timer_id) 
        {
            text.wstr_len = GetBlerInfoStr(buf, 30);;
            text.wstr_ptr = buf;/*lint !e64*/
            GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len, TRUE);
            
        }
        else
            result = MMI_RESULT_FALSE;		// need additional process
        break;
        
    case MSG_LOSE_FOCUS:
        MMK_PauseTimer(s_mmiengtd_timer_id);
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_StopTimer(s_mmiengtd_timer_id);
        s_mmiengtd_timer_id = 0;
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:			GetDpchInfoStr
 DESCRIPTION:	
 PARAM IN:		net_buf - 
				buf_len - 
 PARAM OUT:		net info string len
 AUTHOR:		xiongxiaoyan
 DATE:			2004.09.08
********************************************************************************/
LOCAL int GetOtherInfoStr(uint8* str_buf, uint16 buf_len)
{
    char tmp_buf1[11] = {0};
	uint32 offset = 0;
	uint32 length = 0;
	TIMING_ADVANCE_T	tainfo;
	UE_TX_POWER_T		tx_power;
	CCTRCH_SIR_T		cctrch_sir;
	UL_TARGET_T			ul_target_sir;
	L1_INFO_T	*L1info = "L1info";/*lint !e64*/   //GetL1Info();
	
	tainfo = L1info->Timing_Advance;
	tx_power = L1info->UE_TX_Power;
	cctrch_sir = L1info->CCTRCH_SIR;
	ul_target_sir = L1info->Uplink_Target_SIR;	
	
	if(PNULL == str_buf)
    {
        //SCI_TRACE_LOW:"mmiengtd_win.c GetOtherInfoStr str_buf == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_2083_112_2_18_2_18_41_20,(uint8*)"");
        return -1;
    }    
    //SCI_TRACE_LOW:"mmieng_wintd.c GetOtherInfoStr buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_2086_112_2_18_2_18_41_21,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }
	
	/*tainfo*/	
	MMI_MEMCPY(str_buf, buf_len, "TA: ", 4, 4);
	offset = 4;	
	SCI_MEMSET(tmp_buf1, 0, 10);
	sprintf(tmp_buf1, "%d", tainfo.timing_advance);
	length = SCI_STRLEN(tmp_buf1);
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 10, length);
	offset += length;	
	str_buf[offset] = CR_CHAR;
	offset += 1;	
	
	/*tx_power*/	
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, "tx_power:", 9, 9);
	offset += 9;	
	SCI_MEMSET(tmp_buf1, 0, 10);
	sprintf(tmp_buf1, "%d", tx_power.ue_tx_power);
	length = SCI_STRLEN(tmp_buf1);
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 10, length);
	offset += length;	
	str_buf[offset] = CR_CHAR;
	offset += 1;	

	/*cctrch_sir*/	
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, "CCTRCH_SIR:",11, 11);
	offset += 11;	
	SCI_MEMSET(tmp_buf1, 0, 10);
	sprintf(tmp_buf1, "%d", cctrch_sir.cctrch_sir);
	length = SCI_STRLEN(tmp_buf1);
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 10, length);
	offset += length;	
	str_buf[offset] = CR_CHAR;
	offset += 1;
	
	/*ul_target_sir*/	
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, "ul_target_sir:",14, 14);
	offset += 14;	
	SCI_MEMSET(tmp_buf1, 0, 10);
	sprintf(tmp_buf1, "%d", ul_target_sir.ul_target_sir);
	length = SCI_STRLEN(tmp_buf1);
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 10, length);
	offset += length;	
	str_buf[offset] = CR_CHAR;
	offset += 1;	
	return ((int32)offset);
}



/********************************************************************************
 NAME:			EngShowLayer1MonitorWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		xiongxiaoyan
 DATE:			2006.01.09
********************************************************************************/
LOCAL MMI_RESULT_E EngOtherWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint8 buf[101] = {0};
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        
        text.wstr_len = GetOtherInfoStr(buf, 100);
        text.wstr_ptr = buf;/*lint !e64*/
        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr, text.wstr_len, FALSE);
        MMK_SetAtvCtrl(MMIENG_OTHER_WIN_ID, MMIENG_TEXTBOX1_CTRL_ID);
        // create neighbour CELL refresh timer
        s_mmiengtd_timer_id = MMK_CreateWinTimer(MMIENG_OTHER_WIN_ID, 1000, TRUE);
        break;
        
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_OTHER_WIN_ID);
        break;
        
    case MSG_TIMER:
        if (*(uint8*)param == s_mmiengtd_timer_id) 
        {
            text.wstr_len = GetOtherInfoStr(buf, 100);
            text.wstr_ptr = buf;/*lint !e64*/
            GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len, TRUE);
            
        }
        else
            result = MMI_RESULT_FALSE;		// need additional process
        break;
        
    case MSG_LOSE_FOCUS:
        MMK_PauseTimer(s_mmiengtd_timer_id);
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_StopTimer(s_mmiengtd_timer_id);
        s_mmiengtd_timer_id = 0;
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:			GetLyTwoInfoStr
 DESCRIPTION:	
 PARAM IN:		net_buf - 
				buf_len - 
 PARAM OUT:		net info string len
 AUTHOR:		xiongxiaoyan
 DATE:			2004.09.08
********************************************************************************/
LOCAL int GetLyTwoInfoStr(uint8* str_buf, uint16 buf_len)
{	
	char tmp_buf1[11] = {0};
	uint32 offset = 0;
	uint32 length = 0;
	L2_INFO_T *l2info  = "l2info";/*lint !e64*/   //GetL2Info();

	if(PNULL == str_buf)
    {
        //SCI_TRACE_LOW:"mmiengtd_win.c GetLyTwoInfoStr str_buf == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_2220_112_2_18_2_18_41_22,(uint8*)"");
        return -1;
    }    
    //SCI_TRACE_LOW:"mmieng_wintd.c GetLyTwoInfoStr buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_2223_112_2_18_2_18_41_23,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }
    
	/*ly two*/	
	MMI_MEMCPY(str_buf, buf_len, "MAC_CRC_Err:", 12, 12);
	offset = 12;	
	SCI_MEMSET(tmp_buf1, 0, 10);
	sprintf(tmp_buf1, "%d%%", l2info->mac_CRCError_statistic);
	length = SCI_STRLEN(tmp_buf1) ;
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 10, length);
	offset += length;	
	str_buf[offset] = CR_CHAR;
	offset += 1;	

	return ((int32)offset);
}



/********************************************************************************
 NAME:			EngShowLayer1MonitorWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		xiongxiaoyan
 DATE:			2006.01.09
********************************************************************************/
LOCAL MMI_RESULT_E EngLyTwoInfoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint8 buf[101] = {0};
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        text.wstr_len = GetLyTwoInfoStr(buf, 100);
        text.wstr_ptr = buf;/*lint !e64*/
        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len, FALSE);
        MMK_SetAtvCtrl(MMIENG_LYTWOINFO_WIN_ID, MMIENG_TEXTBOX1_CTRL_ID);
        // create neighbour CELL refresh timer
        s_mmiengtd_timer_id = MMK_CreateWinTimer(MMIENG_LYTWOINFO_WIN_ID, 1000, TRUE);
        break;
        
        
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_LYTWOINFO_WIN_ID);
        break;
        
    case MSG_TIMER:
        if (*(uint8*)param == s_mmiengtd_timer_id) 
        {
            text.wstr_len = GetLyTwoInfoStr(buf, 100);
            text.wstr_ptr = buf;/*lint !e64*/
            GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len, TRUE);
            
        }
        else
            result = MMI_RESULT_FALSE;		// need additional process
        break;
        
    case MSG_LOSE_FOCUS:
        MMK_PauseTimer(s_mmiengtd_timer_id);
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_StopTimer(s_mmiengtd_timer_id);
        s_mmiengtd_timer_id = 0;
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}


/********************************************************************************
 NAME:			GetLyTwoInfoStr
 DESCRIPTION:	
 PARAM IN:		net_buf - 
				buf_len - 
 PARAM OUT:		net info string len
 AUTHOR:		xiongxiaoyan
 DATE:			2004.09.08
********************************************************************************/
LOCAL int GetScellInfoStr(uint8* str_buf, uint16 buf_len)
{		
	char tmp_buf1[20] = {0};
	uint32 offset = 0;
	uint32 length = 0 ;
	L3_INFO_T	*L3info = "L3info";/*lint !e64*/  //GetL3Info();

    if(PNULL == str_buf)
    {
        //SCI_TRACE_LOW:"mmiengtd_win.c GetScellInfoStr str_buf == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_2326_112_2_18_2_18_42_24,(uint8*)"");
        return -1;
    }    
    //SCI_TRACE_LOW:"mmieng_wintd.c GetScellInfoStr buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_2329_112_2_18_2_18_42_25,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }	
    
	/*scell_pccpch_rscp*/	
	MMI_MEMCPY(str_buf, buf_len, "pccpch_rscp:", 12, 12);
	offset = 12;	
	SCI_MEMSET(tmp_buf1, 0, 20);
	length = SCI_STRLEN(tmp_buf1) ;
	sprintf(tmp_buf1, "%d", (signed char)(L3info->scell_PCCPCH_RSCP.scell_pccpch_rscp));
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 20, length);
	offset += length ;	
	str_buf[offset] = CR_CHAR;
	offset += 1;	
	
	/*scell_pathloss*/	
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, "pathloss:", 9,9);
	offset += 9;	
	SCI_MEMSET(tmp_buf1, 0, 20);
	sprintf(tmp_buf1, "%d",L3info->scell_pathloss.scell_pathloss);	
	length = SCI_STRLEN(tmp_buf1) ;
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 20, length);
	offset += length ;	
	str_buf[offset] = CR_CHAR;
	offset += 1;	

	/*scell_cell_id*/	
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, "scell_id:",8, 8);
	offset += 8;	
	SCI_MEMSET(tmp_buf1, 0, 20);
	sprintf(tmp_buf1, "0x%x", L3info->scell_cell_id.scell_cell_id);
	length = SCI_STRLEN(tmp_buf1) ;
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 20, length);
	offset += length;	
	str_buf[offset] = CR_CHAR;
	offset += 1;
	
	/*scell_cell_param_id*/	
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, "cell_param_id:",14, 14);
	offset += 14;	
	SCI_MEMSET(tmp_buf1, 0, 20);
	sprintf(tmp_buf1, "%d", L3info->scell_cell_param_id.scell_cell_param_id);
	length = SCI_STRLEN(tmp_buf1) ;
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 20, length);
	offset += length;	
	str_buf[offset] = CR_CHAR;
	offset += 1;	

	/*scell_uarfcn*/	
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, "uarfcn:",7, 7);
	offset += 7;	
	SCI_MEMSET(tmp_buf1, 0, 20);
	sprintf(tmp_buf1, "%d", L3info->scell_UARFCN.scell_uarfcn);	
	length = SCI_STRLEN(tmp_buf1) ;
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 20, length);
	offset += length;	
	str_buf[offset] = CR_CHAR;
	offset += 1;	

	return ((int32)offset);
}

/********************************************************************************
 NAME:			EngScellWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		xiongxiaoyan
 DATE:			2006.01.09
********************************************************************************/
LOCAL MMI_RESULT_E EngScellWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint8 buf[251] = {0};
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        SCI_MEMSET(buf,0x20,250);
        text.wstr_len = GetScellInfoStr(buf, 250);
        text.wstr_ptr = buf;/*lint !e64*/
        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len, FALSE);
        MMK_SetAtvCtrl(MMIENG_SCELL_WIN_ID, MMIENG_TEXTBOX1_CTRL_ID);
        // create neighbour CELL refresh timer
        s_mmiengtd_timer_id = MMK_CreateWinTimer(MMIENG_SCELL_WIN_ID, 1000, TRUE);
        break;
        
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_SCELL_WIN_ID);
        break;
    case MSG_TIMER:
        if (*(uint8*)param == s_mmiengtd_timer_id) 
        {
            text.wstr_len = GetScellInfoStr(buf, 250);
            text.wstr_ptr = buf;/*lint !e64*/
            GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len, TRUE);
            
        }
        else
            result = MMI_RESULT_FALSE;		// need additional process
        break;
        
    case MSG_LOSE_FOCUS:
        MMK_PauseTimer(s_mmiengtd_timer_id);
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_StopTimer(s_mmiengtd_timer_id);
        s_mmiengtd_timer_id = 0;
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:			GetNcellInfoStr
 DESCRIPTION:	
 PARAM IN:		net_buf - 
				buf_len - 
 PARAM OUT:		net info string len
 AUTHOR:		xiongxiaoyan
 DATE:			2006.3.9
 ********************************************************************************/
LOCAL int GetNcellInfoStr(uint8* str_buf, uint16 buf_len)
{		
	uint32 offset = 0;

	char tmp_buf1[31] = {0};
	uint32 length = 0;
	int i;
	L3_INFO_T	*L3info = "L3info";/*lint !e64*/  //GetL3Info();
	
    if(PNULL == str_buf)
    {
        //SCI_TRACE_LOW:"mmiengtd_win.c GetNcellInfoStr str_buf == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_2474_112_2_18_2_18_42_26,(uint8*)"");
        return -1;
    }    
    //SCI_TRACE_LOW:"mmieng_wintd.c GetNcellInfoStr buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_2477_112_2_18_2_18_42_27,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }
    
	/*ncell_num*/	
	MMI_MEMCPY(str_buf, buf_len, "ncell_num:", 10, 10);
	offset = 10;	
	SCI_MEMSET(tmp_buf1, 0, 30);
	sprintf(tmp_buf1, "%d", L3info->ncell_param_list.ncell_num);
    length = SCI_STRLEN(tmp_buf1) ;
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 30, length);
	offset += length;	
	str_buf[offset] = CR_CHAR;
	offset += 1;	
	
	/*ncell_param*/	
	for(i=0;i<6;i++)
	{
	SCI_MEMSET(tmp_buf1, 0, 30);
	sprintf(tmp_buf1, "ncell_param[%d]:",i);
    length = SCI_STRLEN(tmp_buf1) ;
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 30, length);
	offset += length;	
	str_buf[offset] = CR_CHAR;
	offset += 1;
	/*ncello_uarfcn*/
	SCI_MEMSET(tmp_buf1, 0, 30);
	sprintf(tmp_buf1, "uarfcn=%d,",L3info->ncell_param_list.ncell_param[i].ncell_uarfcn);
    length = SCI_STRLEN(tmp_buf1) ;
	MMI_MEMCPY(str_buf + offset, buf_len - offset, tmp_buf1, 30, length);
	offset += length;	
	str_buf[offset] = CR_CHAR;
	offset += 1;
	/*ncello_pccpch_rscp*/
	SCI_MEMSET(tmp_buf1, 0, 30);
	sprintf(tmp_buf1, "pccpch_rscp=%d,",L3info->ncell_param_list.ncell_param[i].ncell_pccpch_rscp);
    length = SCI_STRLEN(tmp_buf1) ;
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 30, length);
	offset += length;	
	str_buf[offset] = CR_CHAR;
	offset += 1;
	/*ncello_pccpch_rscp*/
	SCI_MEMSET(tmp_buf1, 0, 30);
	sprintf(tmp_buf1, "carrier_rssi=%d,",L3info->ncell_param_list.ncell_param[i].ncell_carrier_rssi);
    length = SCI_STRLEN(tmp_buf1) ;
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 30, length);
	offset += length;	
	str_buf[offset] = CR_CHAR;
	offset += 1;
	}	

	return ((int32)offset);
}

/********************************************************************************
 NAME:			EngNcellWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		xiongxiaoyan
 DATE:			2006.01.09
********************************************************************************/
LOCAL MMI_RESULT_E EngNcellWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint8 buf[501] = {0};
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        
        text.wstr_len = GetNcellInfoStr(buf, 500);
        text.wstr_ptr = buf;/*lint !e64*/
        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len, FALSE);
        MMK_SetAtvCtrl(MMIENG_NCELL_WIN_ID, MMIENG_TEXTBOX1_CTRL_ID);
        // create neighbour CELL refresh timer
        s_mmiengtd_timer_id = MMK_CreateWinTimer(MMIENG_NCELL_WIN_ID, 1000, TRUE);
        break;
        
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_NCELL_WIN_ID);
        break;
        
    case MSG_TIMER:
        if (*(uint8*)param == s_mmiengtd_timer_id) 
        {
            text.wstr_len = GetNcellInfoStr(buf, 500);
            text.wstr_ptr = buf;/*lint !e64*/
            GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len, TRUE);
            
        }
        else
            result = MMI_RESULT_FALSE;		// need additional process
        break;
        
    case MSG_LOSE_FOCUS:
        MMK_PauseTimer(s_mmiengtd_timer_id);
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_StopTimer(s_mmiengtd_timer_id);
        s_mmiengtd_timer_id = 0;
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:			GetNASMMInfoStr
 DESCRIPTION:	
 PARAM IN:		net_buf - 
				buf_len - 
 PARAM OUT:		net info string len
 AUTHOR:		xiongxiaoyan
 DATE:			2006.3.9
 ********************************************************************************/
LOCAL int GetNASMMInfoStr(uint8* str_buf, uint8 buf_len)
{	
//	NAS_INFO_T  *nasinfo = GetNasInfo();
	int offset = 0;
	
    if(PNULL == str_buf)
    {
        //SCI_TRACE_LOW:"mmiengtd_win.c GetNASMMInfoStr str_buf == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_2611_112_2_18_2_18_42_28,(uint8*)"");
        return -1;
    }    
    //SCI_TRACE_LOW:"mmieng_wintd.c GetNASMMInfoStr buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_2614_112_2_18_2_18_42_29,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }
    
	return (offset);
}

/********************************************************************************
 NAME:			EngShowLayer1MonitorWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		xiongxiaoyan
 DATE:			2006.01.09
********************************************************************************/
LOCAL MMI_RESULT_E EngNASMMWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint8 buf[151] = {0};
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        text.wstr_len = GetNASMMInfoStr(buf, 150);
        text.wstr_ptr = buf;/*lint !e64*/
        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len, FALSE);
        MMK_SetAtvCtrl(MMIENG_NASMM_WIN_ID, MMIENG_TEXTBOX1_CTRL_ID);
        // create neighbour CELL refresh timer
        s_mmiengtd_timer_id = MMK_CreateWinTimer(MMIENG_NASMM_WIN_ID, 1000, TRUE);
        break;
        
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_NASMM_WIN_ID);
        break;
        
    case MSG_TIMER:
        if (*(uint8*)param == s_mmiengtd_timer_id) 
        {
            text.wstr_len = GetNASMMInfoStr(buf, 150);
            text.wstr_ptr = buf;/*lint !e64*/
            GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len, TRUE);
            
        }
        else
            result = MMI_RESULT_FALSE;		// need additional process
        break;
        
    case MSG_LOSE_FOCUS:
        MMK_PauseTimer(s_mmiengtd_timer_id);
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_StopTimer(s_mmiengtd_timer_id);
        s_mmiengtd_timer_id = 0;
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:			GetNASGMMInfoStr
 DESCRIPTION:	
 PARAM IN:		net_buf - 
				buf_len - 
 PARAM OUT:		net info string len
 AUTHOR:		xiongxiaoyan
 DATE:			2004.09.08
********************************************************************************/
LOCAL int GetNASGMMInfoStr(uint8* str_buf, uint8 buf_len)
{	
	NAS_INFO_T  *nasinfo ="nasinfo";/*lint !e64*/  //GetNasInfo();
	char tmp_buf1[20] = {0};
	uint32 offset = 0;
    uint32 length = 0;
    if(PNULL == str_buf)
    {
        //SCI_TRACE_LOW:"mmiengtd_win.c GetNASGMMInfoStr str_buf == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_2701_112_2_18_2_18_43_30,(uint8*)"");
        return -1;
    }    
    //SCI_TRACE_LOW:"mmieng_wintd.c GetNASGMMInfoStr buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_2704_112_2_18_2_18_43_31,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }
    
	/*rac*/	
	MMI_MEMCPY(str_buf, buf_len, "rac:", 4, 4);
	offset = 4;	
	SCI_MEMSET(tmp_buf1, 0, 20);
	sprintf(tmp_buf1, "0x%02x", nasinfo->gmm_info.rac);
	length = SCI_STRLEN(tmp_buf1) ;
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 19, length);
	offset += length;	
	str_buf[offset] = CR_CHAR;
	offset += 1;
	
	/*nmo*/
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, "nmo:", 4, 4);
	offset += 4;
	SCI_MEMSET(tmp_buf1, 0, 20);
	sprintf(tmp_buf1, "0x%02x", nasinfo->gmm_info.nmo);	
	length = SCI_STRLEN(tmp_buf1) ;
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 19, length);
	offset += length;	
	str_buf[offset] = CR_CHAR;
	offset += 1;

	/*t3302len*/
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, "t3302len:", 9, 9);
	offset += 9;
	SCI_MEMSET(tmp_buf1, 0, 20);
	sprintf(tmp_buf1, "0x%x", nasinfo->gmm_info.t3302Len);
	length = SCI_STRLEN(tmp_buf1) ;
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 19, length);
	offset += length;	
	str_buf[offset] = CR_CHAR;
	offset += 1;

	/*t3312len*/
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, "t3312len:", 9, 9);
	offset += 9;
	SCI_MEMSET(tmp_buf1, 0, 20);
	sprintf(tmp_buf1, "0x%x", nasinfo->gmm_info.t3312Len);
	length = SCI_STRLEN(tmp_buf1) ;
	MMI_MEMCPY(str_buf + offset, (uint32)buf_len - offset, tmp_buf1, 19, length);
	offset += length;	
	str_buf[offset] = CR_CHAR;
	offset += 1;
	
	return ((int32)offset);
}

/********************************************************************************
 NAME:			EngShowLayer1MonitorWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		xiongxiaoyan
 DATE:			2006.01.09
********************************************************************************/
LOCAL MMI_RESULT_E EngNASGmmWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    uint8 buf[151] = {0};
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        text.wstr_len = GetNASGMMInfoStr(buf, 150);
        text.wstr_ptr = buf;/*lint !e64*/
        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len, FALSE);
        MMK_SetAtvCtrl(MMIENG_NASGMM_WIN_ID, MMIENG_TEXTBOX1_CTRL_ID);
        // create neighbour CELL refresh timer
        s_mmiengtd_timer_id = MMK_CreateWinTimer(MMIENG_NASGMM_WIN_ID, 1000, TRUE);
        break;
        
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_NASGMM_WIN_ID);
        break;
        
    case MSG_TIMER:
        if (*(uint8*)param == s_mmiengtd_timer_id) 
        {
            text.wstr_len = GetNASGMMInfoStr(buf, 150);
            text.wstr_ptr = buf;/*lint !e64*/
            GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len, TRUE);
            
        }
        else
            result = MMI_RESULT_FALSE;		// need additional process
        break;
        
    case MSG_LOSE_FOCUS:
        MMK_PauseTimer(s_mmiengtd_timer_id);
        break;
        
    case MSG_GET_FOCUS:
        break;
        
    case MSG_CLOSE_WINDOW:
        MMK_StopTimer(s_mmiengtd_timer_id);
        s_mmiengtd_timer_id = 0;
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:			EngASNSelectWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E EngASNSelectWinHandleMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
	//uint8 buf[151] = {0};
	//MMI_STRING_T text = {0, FALSE, PNULL};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	
	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
            AppendASNSelectItem();
			MMK_SetAtvCtrl(win_id, MMIENG_ASNSELECT_LISTBOX_CTRL_ID);
			break;
			
		case MSG_FULL_PAINT:
			//GUIWIN_SetSoftkeyTextId(MMIENG_NASGMM_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_OK, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RETURN, FALSE);
			break;
			
		case MSG_KEYDOWN_OK:
		case MSG_KEYDOWN_CANCEL:
			MMK_CloseWin(win_id);
			break;

#ifdef FLIP_PHONE_SUPPORT			
		case MSG_KEYDOWN_FLIP:		// close flip
#endif		
		case MSG_KEYDOWN_RED:
			break;
			
		case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
			break;
	
	
	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
            SaveASNSelect();
            MMK_CloseWin(win_id);
			break;
	
		case MSG_LOSE_FOCUS:
			break;
			
		case MSG_GET_FOCUS:
			break;
			
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);
}

/********************************************************************************
 NAME:			EngDRXCycleWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E EngDRXCycleWinHandleMsg(
                                           MMI_WIN_ID_T win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM param
                                           )
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;   
    MMI_CTRL_ID_T ctrl_id = MMIENG_DRXCYCLE_EDITBOX_CTRL_ID;
    MMI_STRING_T kstring = {0};
    uint8 cycle = 56;
    uint8 text_arr[5];
    uint16 text_length = 0;
    	
	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:            
		 {
            		GUI_BOTH_RECT_T rect = MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id));

	    		GUIAPICTRL_SetBothRect(MMK_ConvertIdToHandle(ctrl_id), &rect);	
            }

            cycle = DMNV_GetDRXCycleLen();/*lint !e64*/
            ConvertDigit2String(text_arr, &text_length, cycle);
            GUIEDIT_SetString(ctrl_id, (wchar*)text_arr, text_length);
			MMK_SetAtvCtrl(win_id, ctrl_id);
			break;
			
		case MSG_FULL_PAINT:
            		//ClearENGClientBG();
			break;
			
		case MSG_KEYDOWN_OK:
		case MSG_KEYDOWN_CANCEL:
			MMK_CloseWin(win_id);
			break;
			
	
		case MSG_CTL_CANCEL:
            		MMK_CloseWin(win_id);
			break;
	
		
	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
            GUIEDIT_GetString(ctrl_id, &kstring);
            cycle = ConvertString2Digit((uint8*)kstring.wstr_ptr, kstring.wstr_len);
            DMNV_SetDRXCycleLen(cycle);
            MMK_CloseWin(win_id);
			break;
	
				
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);
}

/********************************************************************************
 NAME:			MMIENG_OpenASNSelectWin
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
PUBLIC void MMIENG_OpenASNSelectWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_ASNSELECT_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:			MMIENG_OpenPICHSelectWin
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
PUBLIC void MMIENG_OpenDRXCycleWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_DRXCYCLE_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:			AppendASNSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL void AppendASNSelectItem(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIENG_ASNSELECT_LISTBOX_CTRL_ID;
    uint16 sel_num = 0;
    MMI_STRING_T kstring [MMIENG_ASNSELECT_NUM] = {0};
	GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
	uint32 i = 0;
    MMI_TEXT_ID_T asn_version_arr[MMIENG_ASNSELECT_NUM] = {
        TXT_ENG_ASN1_490, TXT_ENG_ASN1_4h0, TXT_ENG_UpPCH_SHIFTING};

		item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
	item_t.item_data_ptr = &item_data;		
	//set softkey
	item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_ENG_CLEAR;
	item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
	item_data.softkey_id[2] = (MMI_TEXT_ID_T)TXT_ENG_RETURN;
	
	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	GUILIST_SetMaxItem(ctrl_id, MMIENG_ASNSELECT_NUM, FALSE );
	for (i = 0; i < MMIENG_ASNSELECT_NUM; i++)
	{
        //set string context of item
        MMI_GetLabelTextByLang(asn_version_arr[i], &kstring[i]);
		//AppendListItemByTextBuffer(item_data,kstring.wstr_ptr,kstring.wstr_len,FALSE);
		item_data.item_content[0].item_data.text_buffer=kstring[i];
		GUILIST_AppendItem(ctrl_id, &item_t);
	}
	
    //get sel_num from MN
#ifndef WIN32
    sel_num = DMNV_GetASNVersionSelection();
#endif
    
	GUILIST_SetCurItemIndex(ctrl_id, sel_num);
	GUILIST_SetSelectedItem(ctrl_id, sel_num, TRUE);
}

/********************************************************************************
 NAME:			SaveASNSelect
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL void SaveASNSelect(void)
{
    uint16 sel_num = 0;
    
    sel_num = GUILIST_GetCurItemIndex(MMIENG_ASNSELECT_LISTBOX_CTRL_ID);
    //set sel_num to MN
#ifndef WIN32
    DMNV_SetASNVersionSelection(sel_num);
#endif
    
    // 提示窗口--已完成
   /* MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);*/
}
//@maggie for cr58510 add begin
/********************************************************************************
 NAME:			MMIENG_OpenArmLogOutPathSelectWin
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		maggie	
 DATE:			2006.09.30
********************************************************************************/
PUBLIC void MMIENG_OpenArmLogOutPathSelectWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_ARMLOGOUTPATHSELECT_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:			EngArmLogOutPathSelectWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		maggie
 DATE:			2006.09.30
********************************************************************************/
LOCAL MMI_RESULT_E EngArmLogOutPathSelectWinHandleMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	
	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
            AppendArmLogOutPathSelectItem();
			MMK_SetAtvCtrl(win_id, MMIENG_ARMLOGOUTPATHSELECT_LISTBOX_CTRL_ID);
			break;
			
		case MSG_FULL_PAINT:
			break;
			
		case MSG_KEYDOWN_OK:
		case MSG_KEYDOWN_CANCEL:
			MMK_CloseWin(win_id);
			break;

#ifdef FLIP_PHONE_SUPPORT			
		case MSG_KEYDOWN_FLIP:		// close flip
#endif		
		case MSG_KEYDOWN_RED:
			break;
			
		case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
			break;
	

	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
            SaveArmLogOutPathSelect();
            MMK_CloseWin(win_id);
			break;
	
		case MSG_LOSE_FOCUS:
			break;
			
		case MSG_GET_FOCUS:
			break;
			
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);
}

/********************************************************************************
 NAME:			AppendArmLogOutPathSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		maggie
 DATE:			
********************************************************************************/
LOCAL void AppendArmLogOutPathSelectItem(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIENG_ARMLOGOUTPATHSELECT_LISTBOX_CTRL_ID;
    uint16 sel_num = 0;
    MMI_STRING_T kstring [MMIENG_ARMLOGOUTPATHSELECT_NUM] = {0};
	GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
	uint32 i = 0;
	MMI_TEXT_ID_T arm_log_out_path_arr[MMIENG_ARMLOGOUTPATHSELECT_NUM] = {
        TXT_ENG_OUTPUT_TO_UART, TXT_ENG_OUTPUT_TO_FFS, TXT_ENG_BOTH};

	item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
	item_t.item_data_ptr = &item_data;		
	//set softkey
	item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_ENG_CLEAR;
	item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
	item_data.softkey_id[2] = (MMI_TEXT_ID_T)TXT_ENG_RETURN;
	
	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	GUILIST_SetMaxItem(ctrl_id, MMIENG_ARMLOGOUTPATHSELECT_NUM, FALSE );
	for (i = 0; i < MMIENG_ARMLOGOUTPATHSELECT_NUM; i++)
	{
        //set string context of item
        MMI_GetLabelTextByLang(arm_log_out_path_arr[i], &kstring[i]);
		//AppendListItemByTextBuffer(item_data,kstring.wstr_ptr,kstring.wstr_len,FALSE);
		item_data.item_content[0].item_data.text_buffer=kstring[i];
		GUILIST_AppendItem(ctrl_id, &item_t);
	}
	
    //get sel_num from MN
    sel_num = 0;//REFPARAM_GetLogOutPath();
    
	GUILIST_SetCurItemIndex(ctrl_id, sel_num);
	GUILIST_SetSelectedItem(ctrl_id, sel_num, TRUE);
}

/********************************************************************************
 NAME:			SaveArmLogOutPathSelect
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		maggie
 DATE:			
********************************************************************************/
LOCAL void SaveArmLogOutPathSelect(void)
{
    uint16 sel_num = 0;
    
    sel_num = GUILIST_GetCurItemIndex(MMIENG_ARMLOGOUTPATHSELECT_LISTBOX_CTRL_ID);
    //set sel_num to MN
//    REFPARAM_SetLogOutPath(sel_num);
    
    // 提示窗口--已完成
   /* MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);*/
}
//@maggie for cr58510 add end
/********************************************************************************
 NAME:			UART0BaudRateWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E UART0BaudRateWinHandleMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

//    SCI_TRACE_LOW("UART0BaudRateWinHandleMsg(), msg_id = 0x%x", msg_id);
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            AppendUART0BaudRateItem();
			MMK_SetAtvCtrl(win_id, MMIENG_UART0BAUDRATE_LISTBOX_CTRL_ID);
            break;
	        
        case MSG_CTL_CANCEL:
	        MMK_CloseWin(win_id);
	        break;

	
	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
            SaveUART0BaudRateSelect();
            MMK_CloseWin(win_id);
			break;
            
        default:
	        result = MMI_RESULT_FALSE;
	        break;
	}
	return (result);
}

/********************************************************************************
 NAME:			AppendUART0BaudRateItem
 DESCRIPTION:	
 PARAM IN:		list_id - 
 PARAM OUT:		
 AUTHOR:		
 DATE:	
 NOTE:	uart0改为com_data
********************************************************************************/
LOCAL void AppendUART0BaudRateItem(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIENG_UART0BAUDRATE_LISTBOX_CTRL_ID;
    uint16 sel_num = 0;
    MMI_STRING_T kstring[MMIENG_UART0BAUDRATE_NUM] ={{0},{0}};
	GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
	uint32 i = 0;
	MMI_TEXT_ID_T template_text_arr[MMIENG_UART0BAUDRATE_NUM] = {
        TXT_ENG_BAUD_115200, TXT_ENG_BAUD_460800};
    uint32 baud_rate = 0;

	item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
	item_t.item_data_ptr = &item_data;
	//set softkey
	item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_ENG_CLEAR;
	item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
	item_data.softkey_id[2] = (MMI_TEXT_ID_T)TXT_ENG_RETURN;
	    item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	GUILIST_SetMaxItem(ctrl_id, MMIENG_UART0BAUDRATE_NUM, FALSE );
	for (i = 0; i < MMIENG_UART0BAUDRATE_NUM; i++)
	{
        //set string context of item
        MMI_GetLabelTextByLang(template_text_arr[i], &kstring[i]);
	//AppendListItemByTextBuffer(item_data,kstring.wstr_ptr,kstring.wstr_len,FALSE);
	item_data.item_content[0].item_data.text_buffer=kstring[i];

		GUILIST_AppendItem(ctrl_id, &item_t);
	}
	
    //get sel_num from MN
    //baud_rate = REFPARAM_GetUart0BaudRate();
	//baud_rate = SIO_GetBaudRate(0);
//SCI_PASSERT(0,("%d",COM_DATA));
	
	baud_rate = SIO_GetBaudRate(COM_DATA);
    if (BAUD_115200 == baud_rate)
    {
        sel_num = 0;
    }
    else if (BAUD_460800 == baud_rate)
    {
        sel_num = 1;
    }
    else
    {   
		 sel_num = 1;
        //SCI_PASSERT(0, ("baud_rate = %d", baud_rate));
    }
    
	GUILIST_SetCurItemIndex(ctrl_id, sel_num);
	GUILIST_SetSelectedItem(ctrl_id, sel_num, TRUE);
}
/********************************************************************************
 NAME:			SaveUART0BaudRateSelect
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		
 DATE:		
 NOTE:	uart0改为com_data
********************************************************************************/
LOCAL void SaveUART0BaudRateSelect(void)
{
    uint16 sel_num = 0;
    uint32  baud_rate[2] = {BAUD_115200, BAUD_460800};
    
    sel_num = GUILIST_GetCurItemIndex(MMIENG_UART0BAUDRATE_LISTBOX_CTRL_ID);
    //set sel_num to MN
    //REFPARAM_SetUart0BaudRate(baud_rate[sel_num]);
    //SIO_SetBaudRate(0, baud_rate[sel_num]);
    SIO_SetBaudRate((uint32)COM_DATA, baud_rate[sel_num]);
    // 提示窗口--已完成
    /*MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);*/
}
//@maggie for cr58510 add end

/********************************************************************************
 NAME:			字符串转化为数字
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL uint32 ConvertString2Digit(
                                 uint8 *str_ptr,    //in
                                 uint16 length  //in
                                 )
{
    uint32 num = 0;
    uint32 i = 0;

    for (i = 0; i < length; i++)
    {
        num = (num * 10 + (str_ptr[i] - '0'));
    }
    return num;
}

/********************************************************************************
 NAME:			数字转化为字符串
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL void ConvertDigit2String(
                               uint8 *str_ptr,        //out
                               uint16 *length_ptr,    //out
                               uint32 num //in
                               )
{
    uint32 exp = 0, a = 0, b = 0, c = num, i = 0;
    int32 offset = 0;
    
    exp = calexp(num);
    (*length_ptr) = exp;

    for (i = 0; i < exp; i++)
    {
        a = c / 10;
        b = (c - a * 10);
        offset = exp - 1 - i;
        if(offset>=0 &&offset<*length_ptr)
        {
            str_ptr[exp - 1 - i] = (b + '0');
        }
        else
        {
            break;
        }
        c = a;
    }
}

/********************************************************************************
 NAME:			计算数字的幂数
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL uint32 calexp(uint32 num)
{
    uint32 c = num;
    uint32 i = 1;

    while (c >= 10)
    {
        c = c / 10;
        i++;
    }
    return i;
}

//@maggie add begin
/********************************************************************************
 NAME:			ENGVTSettingMenuWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		maggie.ren
 DATE:			2006.09.26
********************************************************************************/
/*
LOCAL MMI_RESULT_E ENGVTSettingMenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	uint32 group_id = 0;
	uint32 menu_id = 0;

	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
			//GUILIST_SetMaxItem(MMIENG_VT_SETTING_WIN_ID,MMISET_MAX_LANGUAGE, FALSE );
            //MMISET_AppendLanguageListItem(MMIENG_VT_SETTING_WIN_ID);
            MMK_SetAtvCtrl(MMIENG_VT_SETTING_WIN_ID, MMIENG_VTMENU_CTRL_ID);
			break;
			
		case MSG_FULL_PAINT:
			break;

		case MSG_KEYDOWN_FLIP:		// close flip
		case MSG_KEYDOWN_RED:
			MMK_CloseWin(MMIENG_VT_SETTING_WIN_ID);
			break;
		
		case MSG_KEYDOWN_CANCEL:
		case MSG_CTL_CANCEL:
			MMK_CloseWin(MMIENG_VT_SETTING_WIN_ID);
			break;

		case MSG_KEYDOWN_OK:
		case MSG_CTL_OK:
			GUIMENU_GetId(MMIENG_VTMENU_CTRL_ID, &group_id, &menu_id);
			switch (menu_id)
			{
				case ID_ENG_VT_AUDIO:
					MMK_CreateWin((uint32*)MMIENG_VT_AUDIO_WIN_TAB,  PNULL);
					break;
					
				case ID_ENG_VT_VIDEO:
					MMK_CreateWin((uint32*)MMIENG_VT_VIDEO_WIN_TAB, PNULL);
					break;
                
				default:
					break;
			}
			break;

		case MSG_CLOSE_WINDOW:
			break;
		
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);
}
*/

/********************************************************************************
 NAME:			EngVTAudioWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		maggie.ren
 DATE:			2006.09.26
********************************************************************************/
LOCAL MMI_RESULT_E EngVTAudioWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
//	uint32 group_id = 0;
//	uint32 menu_id = 0;
	uint16 index = 0;
	uint16 audio_enabled=1;

	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
			AppendVTAudioKeyItem(MMIENG_LISTBOX_CTRL_ID);
			MMK_SetAtvCtrl(MMIENG_VTAUDIO_WIN_ID, MMIENG_LISTBOX_CTRL_ID);
			break;
			
		case MSG_FULL_PAINT:
			//GUILIST_UpdateListBox(MMIENG_LISTBOX_CTRL_ID);
			break;

#ifdef FLIP_PHONE_SUPPORT
		case MSG_KEYDOWN_FLIP:		
#endif		
		case MSG_KEYDOWN_RED:
			MMK_CloseWin(MMIENG_VTAUDIO_WIN_ID);
			break;
		
		case MSG_KEYDOWN_CANCEL:
		case MSG_CTL_CANCEL:
			MMK_CloseWin(MMIENG_VTAUDIO_WIN_ID);
			break;

		case MSG_KEYDOWN_OK:

	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
			GUILIST_GetSelectedItemIndex(MMIENG_LISTBOX_CTRL_ID, &index, 1);
            if (0 == index) 
			{
				audio_enabled = 1;
			}else
			{	
			    audio_enabled = 0;
            }
//            MPLH324M_EnableAudio(audio_enabled);
			// 成功界面
           /* MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);*/
            MMK_CloseWin(MMIENG_VTAUDIO_WIN_ID);
			break;

		case MSG_CLOSE_WINDOW:
			break;
		
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);
}

/********************************************************************************
 NAME:			EngVTVideoWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		maggie.ren
 DATE:			2006.09.26
********************************************************************************/
LOCAL MMI_RESULT_E EngVTVideoWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
//	uint32 group_id = 0;
//	uint32 menu_id = 0;
	uint16 index = 0;
	uint16 video_enabled=1;

	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
			AppendVTVideoKeyItem(MMIENG_LISTBOX_CTRL_ID);
			MMK_SetAtvCtrl(MMIENG_VTVIDEO_WIN_ID, MMIENG_LISTBOX_CTRL_ID);
			break;
			
		case MSG_FULL_PAINT:
			//GUILIST_UpdateListBox(MMIENG_LISTBOX_CTRL_ID);
			break;

#ifdef FLIP_PHONE_SUPPORT
		case MSG_KEYDOWN_FLIP:		
#endif		
		case MSG_KEYDOWN_RED:
			MMK_CloseWin(MMIENG_VTVIDEO_WIN_ID);
			break;
		
		case MSG_KEYDOWN_CANCEL:
		case MSG_CTL_CANCEL:
			MMK_CloseWin(MMIENG_VTVIDEO_WIN_ID);
			break;

		case MSG_KEYDOWN_OK:
	
	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
			GUILIST_GetSelectedItemIndex(MMIENG_LISTBOX_CTRL_ID, &index, 1);
			if (0 == index) 
			{
				video_enabled = 1;
			}else
			{	
			    video_enabled = 0;
            }
//            MPLH324M_EnableVideo(video_enabled);
			// 成功界面
         /*   MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);*/
            MMK_CloseWin(MMIENG_VTVIDEO_WIN_ID);
			break;

		case MSG_CLOSE_WINDOW:
			break;
		
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);
}


/********************************************************************************
 NAME:			AppendVTAudioKeyItem
 DESCRIPTION:	
 PARAM IN:		list_id - 
 PARAM OUT:		None
 AUTHOR:		maggie.ren
 DATE:			2006.09.26
********************************************************************************/
LOCAL void AppendVTAudioKeyItem(MMI_CTRL_ID_T list_id)
{
	MMI_STRING_T item_str [2];
	GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
//	BOOLEAN is_vt_audio = TRUE;
	 

    //   is_vt_audio=MPLH324M_isAudioEnable();
	
	GUILIST_SetMaxItem(list_id, 2, FALSE );

	item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
	item_t.item_data_ptr = &item_data;
	//set softkey
	item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_ENG_OK;
	item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
	item_data.softkey_id[2] = (MMI_TEXT_ID_T)TXT_ENG_RETURN;
	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	// Yes
	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ENG_YES, &item_str[0]); 
	//AppendListItemByTextBuffer(item_data,item_str.wstr_ptr,item_str.wstr_len,FALSE);
	item_data.item_content[0].item_data.text_buffer=item_str[0];
	GUILIST_AppendItem(list_id, &item_t);
	
	// No
	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ENG_NO, &item_str[1]); 
	item_data.item_content[0].item_data.text_buffer=item_str[1];
	//AppendListItemByTextBuffer(item_data,item_str.wstr_ptr,item_str.wstr_len,FALSE);
	GUILIST_AppendItem(list_id, &item_t);

//	if (is_vt_audio)
//	{
		GUILIST_SetSelectedItem(MMIENG_LISTBOX_CTRL_ID, 0, TRUE);
		GUILIST_SetCurItemIndex(MMIENG_LISTBOX_CTRL_ID, 0);
//	}
//	else
//	{
//		GUILIST_SetSelectedItem(MMIENG_LISTBOX_CTRL_ID, 1, TRUE);
//		GUILIST_SetCurItemIndex(MMIENG_LISTBOX_CTRL_ID, 1);
//	}
}

/********************************************************************************
 NAME:			AppendVTVideoKeyItem
 DESCRIPTION:	
 PARAM IN:		list_id - 
 PARAM OUT:		None
 AUTHOR:		maggie.ren
 DATE:			2006.09.26
********************************************************************************/
LOCAL void AppendVTVideoKeyItem(MMI_CTRL_ID_T list_id)
{
	MMI_STRING_T item_str[2] ;
	GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
	//BOOLEAN is_vt_video = TRUE;
	 

//    is_vt_video=MPLH324M_isVideoEnable();
	
	GUILIST_SetMaxItem(list_id, 2, FALSE );

	item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
	item_t.item_data_ptr = &item_data;
	//set softkey
	item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_ENG_OK;
	item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
	item_data.softkey_id[2] = (MMI_TEXT_ID_T)TXT_ENG_RETURN;
	
	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	// Yes
	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ENG_YES, &item_str[0]); 
	item_data.item_content[0].item_data.text_buffer=item_str[0];
	//AppendListItemByTextBuffer(item_data,item_str.wstr_ptr,item_str.wstr_len,FALSE);
	GUILIST_AppendItem(list_id, &item_t);
	
	// No
	MMI_GetLabelTextByLang((MMI_TEXT_ID_T)TXT_ENG_NO, &item_str[1]); 
	//AppendListItemByTextBuffer(item_data,item_str.wstr_ptr,item_str.wstr_len,FALSE);
	item_data.item_content[0].item_data.text_buffer=item_str[1];
	GUILIST_AppendItem(list_id, &item_t);

	//if (is_vt_video)
//	{
		GUILIST_SetSelectedItem(MMIENG_LISTBOX_CTRL_ID, 0, TRUE);
		GUILIST_SetCurItemIndex(MMIENG_LISTBOX_CTRL_ID, 0);
//	}
//	else
//	{
//		GUILIST_SetSelectedItem(MMIENG_LISTBOX_CTRL_ID, 1, TRUE);
//		GUILIST_SetCurItemIndex(MMIENG_LISTBOX_CTRL_ID, 1);
//	}
}

//@maggie add end

//@maggie for cr58359 begin    
/********************************************************************************
 NAME:			EngShowChipIDWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		maggie
 DATE:			2006.09.28
********************************************************************************/
LOCAL MMI_RESULT_E EngShowChipIDWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    uint32 i = 0;
    int32 length = 0;
    wchar wstr[20] = {0};
    char buffer[20] = {0};
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW: 
        GUIFORM_SetStyle(MMIENG_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);
	{
                GUI_BOTH_RECT_T rect = MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id));
	        GUIAPICTRL_SetBothRect(MMK_ConvertIdToHandle(MMIENG_FORM_CTRL_ID), &rect);	
            }
        text.wstr_ptr = (uint8*)"Chip ID:";/*lint !e64*/
        text.wstr_len = 8;
        GUILABEL_SetBackgroundColor(MMIENG_LABEL1_CTRL_ID, mmi_eng_display.bkgd_color);
        GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, FALSE);
        
#ifndef _WIN32
        i=CHIP_GetHWChipID();	
        length = sprintf((char *)buffer,"%x\n", i );
        MMI_STRNTOWSTR(wstr, 19, (uint8*)buffer, 19, length);
        text.wstr_ptr = wstr;
        text.wstr_len = length;
#else
        text.wstr_ptr = (uint8*)"P2A2";	/*lint !e64*/// this is a example for simulator
        text.wstr_len = 4;
#endif
        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr, text.wstr_len, FALSE);
        break;     
		
	case MSG_FULL_PAINT:
        ClearENGClientBG();
        break;
        
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_SHOWCHIPID_WIN_ID);
        break;
        
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

LOCAL MMI_RESULT_E HandleDspSettingWinMsg(
                                          MMI_WIN_ID_T win_id,
                                          MMI_MESSAGE_ID_E msg_id,
                                          DPARAM param
                                          )
		
{
	MMI_STRING_T text1 = {0};
	MMI_STRING_T text2 = {0};
	MMI_STRING_T text3 = {0};
	MMI_STRING_T text4 = {0};
	MMI_STRING_T text5 = {0};
	MMI_STRING_T net_freq[5] = {{PNULL, 0},{PNULL, 0},{PNULL, 0},{PNULL, 0},{PNULL, 0}};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	MN_ENG_FREQ_T               freq;
	uint16 i;
	uint16 len_sum = 0;
    uint32 param_id[5] = {0};

	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
            GUIFORM_SetStyle(MMIENG_DSPSET_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIENG_DSPSET_FORM2_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIENG_DSPSET_FORM3_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIENG_DSPSET_FORM4_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIENG_DSPSET_FORM5_CTRL_ID,GUIFORM_STYLE_UNIT);

			text1.wstr_ptr = L"cmd: ";
			text1.wstr_len = 5;
			//GUILABEL_SetStyle ( MMIENG_DSPSET_LABEL1_CTRL_ID, GUIEDIT_STYLE_SINGLE);
			GUILABEL_SetText(MMIENG_DSPSET_LABEL1_CTRL_ID, &text1, FALSE);
			
			text2.wstr_ptr = L"param1";
			text2.wstr_len = 6;
			//GUIEDIT_SetStyle(MMIENG_EDITBOX3_CTRL_ID, GUIEDIT_STYLE_SINGLE);
			GUILABEL_SetText(MMIENG_DSPSET_LABEL2_CTRL_ID, &text2, FALSE);
			
			text3.wstr_ptr = L"param2";
			text3.wstr_len = 6;
			//GUIEDIT_SetStyle(MMIENG_EDITBOX4_CTRL_ID, GUIEDIT_STYLE_SINGLE);
			GUILABEL_SetText(MMIENG_DSPSET_LABEL3_CTRL_ID, &text3, FALSE);
			
			text4.wstr_ptr = L"param3";
			text4.wstr_len = 6;
			//GUIEDIT_SetStyle(MMIENG_EDITBOX5_CTRL_ID, GUIEDIT_STYLE_SINGLE);
			GUILABEL_SetText(MMIENG_DSPSET_LABEL4_CTRL_ID, &text4, FALSE);
			
			text5.wstr_ptr = L"param4";
			text5.wstr_len = 6;
			//GUIEDIT_SetStyle(MMIENG_EDITBOX7_CTRL_ID, GUIEDIT_STYLE_SINGLE);
			GUILABEL_SetText(MMIENG_DSPSET_LABEL5_CTRL_ID, &text5, FALSE);
			

			MMK_SetAtvCtrl(MMIENG_DSP_SETTING_WIN_ID,MMIENG_DSPSET_EDIT1_CTRL_ID);
			// first input the up link
			//s_mmiengtd_up_down_link = 0;
			break;	
			
		case MSG_FULL_PAINT:
       	     //ClearENGClientBG();
			break;	
			
		case MSG_CTL_CANCEL:
			MMK_CloseWin(MMIENG_DSP_SETTING_WIN_ID);
			break;
	

	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
			GUIEDIT_GetString(MMIENG_DSPSET_EDIT1_CTRL_ID, &net_freq[0]);
			GUIEDIT_GetString(MMIENG_DSPSET_EDIT2_CTRL_ID, &net_freq[1]);
			GUIEDIT_GetString(MMIENG_DSPSET_EDIT3_CTRL_ID, &net_freq[2]);
			GUIEDIT_GetString(MMIENG_DSPSET_EDIT4_CTRL_ID, &net_freq[3]);
			GUIEDIT_GetString(MMIENG_DSPSET_EDIT5_CTRL_ID, &net_freq[4]);
		
            param_id[0] = ConvertString2Digit((uint8*)net_freq[0].wstr_ptr, net_freq[0].wstr_len);
            param_id[1] = ConvertString2Digit((uint8*)net_freq[1].wstr_ptr, net_freq[1].wstr_len);
            param_id[2] = ConvertString2Digit((uint8*)net_freq[2].wstr_ptr, net_freq[2].wstr_len);
            param_id[3] = ConvertString2Digit((uint8*)net_freq[3].wstr_ptr, net_freq[3].wstr_len);
            param_id[4] = ConvertString2Digit((uint8*)net_freq[4].wstr_ptr, net_freq[4].wstr_len);

			freq.isused = FALSE;
		    TD_MNEM_SetCommandTypeAndParam( 
				       param_id[0],
					   param_id[1],
					   param_id[2],
					   param_id[3],
					   param_id[4]
                       );       

            /*MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE); */  
          

			for (i=0 ; i < 4; i++)
			{
				//check the input 0 or have value
			      if ((0 == net_freq[i].wstr_len) )
			      {
					//check whether have the input before 
					if( len_sum > 0 )
					{
						freq.info[i].len= freq.info[i-1].len;
						freq.info[i].val = freq.info[i-1].val;
					}
			      }
			      else
			     {
					len_sum += net_freq[i].wstr_len;
					freq.info[i].len= (uint8)net_freq[i].wstr_len;
					freq.info[i].val = atoi((char*)net_freq[i].wstr_ptr);

					//check the input reach the 5 bit
					//if (freq.info[i].len < 5)
					//{
					//	freq.info[i].val = freq.info[i].val * 10^(5 - freq.info[i].len);
					//	freq.info[i].len = 5;
					//}					
				}
			}

			if (0 != len_sum)
			{
				freq.isused = TRUE;
			}
#ifndef TD_AS_NOT_PRESENT
			DMNV_SetFreqList(freq);
#endif
		
			MMK_CloseWin(MMIENG_DSP_SETTING_WIN_ID);
			break;
			
		case MSG_KEYDOWN_UP:
			/*
			if (5 == s_mmiengtd_up_down_link) 
				s_mmiengtd_up_down_link = 0 ;
			else
				s_mmiengtd_up_down_link ++ ;		 
				*/
			break;			 
			
		case MSG_KEYDOWN_DOWN:
			/*
			if (0 == s_mmiengtd_up_down_link)
			{
				MMK_SetAtvCtrl(MMIENG_DSP_SETTING_WIN_ID, MMIENG_EDITBOX3_CTRL_ID);
				s_mmiengtd_up_down_link = 1;
			}
			else if  (1 == s_mmiengtd_up_down_link)
			{
				MMK_SetAtvCtrl(MMIENG_DSP_SETTING_WIN_ID, MMIENG_EDITBOX4_CTRL_ID);
				s_mmiengtd_up_down_link = 2;
			}
			else if (2 == s_mmiengtd_up_down_link)
			{
				MMK_SetAtvCtrl(MMIENG_DSP_SETTING_WIN_ID, MMIENG_EDITBOX5_CTRL_ID);
				s_mmiengtd_up_down_link = 3;
			}
			else if (3== s_mmiengtd_up_down_link)
			{
				MMK_SetAtvCtrl(MMIENG_DSP_SETTING_WIN_ID, MMIENG_EDITBOX7_CTRL_ID);
				s_mmiengtd_up_down_link = 4;
			}
			else
			{
				MMK_SetAtvCtrl(MMIENG_DSP_SETTING_WIN_ID, MMIENG_EDITBOX1_CTRL_ID);
				s_mmiengtd_up_down_link = 0;
			}
			
			if (0 == s_mmiengtd_up_down_link) 
				s_mmiengtd_up_down_link = 5 ;
			else
				s_mmiengtd_up_down_link --;		 
				*/
			break;
	
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);
}

	
//@maggie add end

/********************************************************************************
 NAME:			MMIENG_OpenUart0UsedByWin
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
PUBLIC void MMIENG_OpenUart0UsedByWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_UART0USEDBY_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:			EngUart0UsedByWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		jibin
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E EngUart0UsedByWinHandleMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
	//uint8 buf[151] = {0};
	//MMI_STRING_T text = {0, FALSE, PNULL};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	
	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
            AppendUart0UsedBySelectItem();
			MMK_SetAtvCtrl(win_id, MMIENG_UART0USEDBY_LISTBOX_CTRL_ID);
			break;
			
		case MSG_FULL_PAINT:
			//GUIWIN_SetSoftkeyTextId(MMIENG_NASGMM_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_OK, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RETURN, FALSE);
			break;
			
		case MSG_KEYDOWN_OK:
		case MSG_KEYDOWN_CANCEL:
			MMK_CloseWin(win_id);
			break;

#ifdef FLIP_PHONE_SUPPORT			
		case MSG_KEYDOWN_FLIP:		// close flip
#endif		
		case MSG_KEYDOWN_RED:
			break;
			
		case MSG_CTL_CANCEL:
            MMK_CloseWin(win_id);
			break;
	

	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
            SaveUart0UsedBySelect();
            MMK_CloseWin(win_id);
			break;
	
		case MSG_LOSE_FOCUS:
			break;
			
		case MSG_GET_FOCUS:
			break;
			
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);
}

//@zhaohui,cr106076,start
/********************************************************************************
 NAME:			MMIENG_OpenUart0MixModeWin
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		zhaohui
 DATE:			
********************************************************************************/
PUBLIC void MMIENG_OpenUart0MixModeWin()
{
    MMK_CreateWin((uint32*)MMIENGTD_UART0MIXMODE_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:			EngUart0MixModeWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		zhaohui
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E EngUart0MixModeWinHandleMsg(
               			MMI_WIN_ID_T win_id,
                			MMI_MESSAGE_ID_E msg_id,
                			DPARAM param
					)
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	
	switch(msg_id)
	{
		case MSG_OPEN_WINDOW:
			AppendUart0MixModeSelectItem();
			MMK_SetAtvCtrl(win_id, MMIENG_UART0MIXMODE_LISTBOX_CTRL_ID);
			break;
			
	    	case MSG_FULL_PAINT:
	    		break;
	    
	    	case MSG_KEYDOWN_OK:
		case MSG_KEYDOWN_CANCEL:
		case MSG_CTL_CANCEL:
			MMK_CloseWin(win_id);
			break;	
		
	   
	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
            		SaveUart0MixModeSelect();
            		MMK_CloseWin(win_id);
			break;
				
	   	default:
			result = MMI_RESULT_FALSE;
			break;			
	}

	return (result);
}

/********************************************************************************
 NAME:			AppendUart0MixModeSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhaohui
 DATE:	
  NOTE:	
********************************************************************************/
LOCAL void AppendUart0MixModeSelectItem(void)
{
	MMI_CTRL_ID_T ctrl_id = MMIENG_UART0MIXMODE_LISTBOX_CTRL_ID;
	uint16 sel_num = 0;
	MMI_STRING_T kstring [2] = {0};
	GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
	uint32 i = 0;
	MMI_TEXT_ID_T uart0_version_arr[2] = {
	    TXT_ENG_ENABLE,TXT_ENG_DISABLE};

	item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
	item_t.item_data_ptr = &item_data;
	//set softkey
	item_data.softkey_id[0] = TXT_ENG_CLEAR;/*应该是ok*/
	item_data.softkey_id[1] = TXT_NULL;
	item_data.softkey_id[2] = TXT_ENG_RETURN;
    
	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	GUILIST_SetMaxItem(ctrl_id, 2,FALSE);
	for (i = 0; i < 2; i++)
	{
	    	//set string context of item
	    	MMI_GetLabelTextByLang(uart0_version_arr[i], &kstring[i]);
			//AppendListItemByTextBuffer(item_data,kstring.wstr_ptr,kstring.wstr_len,FALSE);
		item_data.item_content[0].item_data.text_buffer=kstring[i];	
			GUILIST_AppendItem(ctrl_id, &item_t);
	}
	
	//get sel_num from NV
#ifndef WIN32
	if (REFPARAM_GetMixMode())
	{
		sel_num = 0;
	}
	else
	{
		sel_num = 1;
	}
#endif

	GUILIST_SetCurItemIndex(ctrl_id, sel_num);
	GUILIST_SetSelectedItem(ctrl_id, sel_num, TRUE);
}

/********************************************************************************
 NAME:			SaveUart0MixModeSelect
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:			zhaohui
 DATE:			
********************************************************************************/
LOCAL void SaveUart0MixModeSelect(void)
{
	uint16 sel_num = 0;
	
	sel_num = GUILIST_GetCurItemIndex(MMIENG_UART0MIXMODE_LISTBOX_CTRL_ID);
	
#ifndef WIN32
	if (0 == sel_num)
	{
		REFPARAM_SetMixMode(TRUE);
	}
	else if (1 == sel_num)
	{
		REFPARAM_SetMixMode(FALSE);
	}	
	else
	{
        //SCI_TRACE_LOW:"mmieng_wintd.c SaveUart0MixModeSelect sel_num is %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_4155_112_2_18_2_18_46_32,(uint8*)"d",sel_num);
	}
#endif

    /*提示窗口，已完成*/
  /*  MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);*/
}
//@zhaohui,cr106076,end

//COM_DATA: 0 COM_DEBUG: 1
LOCAL MMI_RESULT_E HandleRoamParamWinMsg(//@shangke
							MMI_WIN_ID_T win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM param
                                           )

{
	MMI_STRING_T text1 = {0};
	MMI_STRING_T text2 = {0};
	MMI_STRING_T text3 = {0};
	MMI_STRING_T roam_param[3] = {{PNULL, 0},{PNULL, 0},{PNULL, 0}};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	
      uint8 text_arr1[5];
      uint8 text_arr2[5];
      uint8 text_arr3[5];
      	
	uint16 text_length1 = 0;
	uint16 text_length2 = 0;
	uint16 text_length3 = 0;
	GUI_LCD_DEV_INFO	lcd_dev_info = {0};  
    	lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
    	lcd_dev_info.block_id = GUI_BLOCK_MAIN;
    	
	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
            GUIFORM_SetStyle(MMIENG_ROAMPARAM_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIENG_ROAMPARAM_FORM2_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIENG_ROAMPARAM_FORM3_CTRL_ID,GUIFORM_STYLE_UNIT);

	    		{
                GUI_BOTH_RECT_T rect = MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id));
	     	   GUIAPICTRL_SetBothRect(MMK_ConvertIdToHandle(MMIENG_ROAMPARAM_FORM_CTRL_ID), &rect);	
            }  	
				
			// set window 's title
			GUIWIN_SetTitleTextId(MMIENG_ROAMPARAM_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_ROAMPARAM, FALSE);
			//set threshold volume
			text1.wstr_ptr = (uint8*)"Threshold:";/*lint !e64*/
			text1.wstr_len = 10;
			GUILABEL_SetText(MMIENG_ROAMPARAM_LABEL1_CTRL_ID, &text1, FALSE);
			GUIEDIT_SetStyle(MMIENG_ROAMPARAM_EDITBOX1_CTRL_ID, GUIEDIT_STYLE_SINGLE);

			ConvertDigit2String(text_arr1, &text_length1, g_threshold);
			GUIEDIT_SetString(MMIENG_ROAMPARAM_EDITBOX1_CTRL_ID, (wchar*)text_arr1, text_length1);

			//set periods volume
			text2.wstr_ptr = (uint8*)"Periods:";/*lint !e64*/
			text2.wstr_len = 8;
			GUILABEL_SetText(MMIENG_ROAMPARAM_LABEL2_CTRL_ID, &text2, FALSE);
			GUIEDIT_SetStyle(MMIENG_ROAMPARAM_EDITBOX2_CTRL_ID, GUIEDIT_STYLE_SINGLE);

			ConvertDigit2String(text_arr2, &text_length2, g_periods);
            		GUIEDIT_SetString(MMIENG_ROAMPARAM_EDITBOX2_CTRL_ID, (wchar*)text_arr2, text_length2);

			//set minutes volume	
			text3.wstr_ptr = (uint8*)"Timer:";/*lint !e64*/
			text3.wstr_len = 6;
			GUILABEL_SetText(MMIENG_ROAMPARAM_LABEL3_CTRL_ID, &text3, FALSE);
			GUIEDIT_SetStyle(MMIENG_ROAMPARAM_EDITBOX3_CTRL_ID, GUIEDIT_STYLE_SINGLE);

			ConvertDigit2String(text_arr3, &text_length3,g_timer);
            		GUIEDIT_SetString(MMIENG_ROAMPARAM_EDITBOX3_CTRL_ID, (wchar*)text_arr3, text_length3);
	
			MMK_SetAtvCtrl(MMIENG_ROAMPARAM_WIN_ID, MMIENG_ROAMPARAM_EDITBOX1_CTRL_ID);
			// first input the up link
			//s_mmiengtd_up_down_link = 0;
			break;
			
			
		case MSG_CTL_CANCEL:
			MMK_CloseWin(MMIENG_ENG_FREQ_WIN_ID);
			break;
	
		
	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
			GUIEDIT_GetString(MMIENG_ROAMPARAM_EDITBOX1_CTRL_ID, &roam_param[0]);
			GUIEDIT_GetString(MMIENG_ROAMPARAM_EDITBOX2_CTRL_ID, &roam_param[1]);
			GUIEDIT_GetString(MMIENG_ROAMPARAM_EDITBOX3_CTRL_ID, &roam_param[2]);
			g_threshold = ConvertString2Digit((uint8*)roam_param[0].wstr_ptr, roam_param[0].wstr_len);
			g_periods = ConvertString2Digit((uint8*)roam_param[1].wstr_ptr, roam_param[1].wstr_len);
			g_timer = ConvertString2Digit((uint8*)roam_param[2].wstr_ptr, roam_param[2].wstr_len);
		//*******写NV*******
			MMINV_WRITE(MMINV_ENG_THRESHOLD,&g_threshold);
			MMINV_WRITE(MMINV_ENG_PERIODS,&g_periods);
			MMINV_WRITE(MMINV_ENG_TIMER,&g_timer);
		//*******写NV*******
			
           /* MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);   */
			MMK_CloseWin(MMIENG_ROAMPARAM_WIN_ID);
			break;
			
		case MSG_KEYDOWN_UP:
			/*
			if (0 == s_mmiengtd_up_down_link)
			{
				MMK_SetAtvCtrl(MMIENG_ROAMPARAM_WIN_ID, MMIENG_ROAMPARAM_EDITBOX3_CTRL_ID);
				s_mmiengtd_up_down_link = 2;
			}
			else if  (1 == s_mmiengtd_up_down_link)
			{
				MMK_SetAtvCtrl(MMIENG_ROAMPARAM_WIN_ID, MMIENG_ROAMPARAM_EDITBOX1_CTRL_ID);
				s_mmiengtd_up_down_link = 0;
			}
			else
			{
				MMK_SetAtvCtrl(MMIENG_ROAMPARAM_WIN_ID, MMIENG_ROAMPARAM_EDITBOX2_CTRL_ID);
				s_mmiengtd_up_down_link = 1;
			}
			*/
			break;
			
		case MSG_KEYDOWN_DOWN:
			/*
			if (0 == s_mmiengtd_up_down_link)
			{
				MMK_SetAtvCtrl(MMIENG_ROAMPARAM_WIN_ID, MMIENG_ROAMPARAM_EDITBOX2_CTRL_ID);
				s_mmiengtd_up_down_link = 1;
			}
			else if  (1 == s_mmiengtd_up_down_link)
			{
				MMK_SetAtvCtrl(MMIENG_ROAMPARAM_WIN_ID, MMIENG_ROAMPARAM_EDITBOX3_CTRL_ID);
				s_mmiengtd_up_down_link = 2;
			}
			else
			{
				MMK_SetAtvCtrl(MMIENG_ROAMPARAM_WIN_ID, MMIENG_ROAMPARAM_EDITBOX1_CTRL_ID);
				s_mmiengtd_up_down_link = 0;
			}
			*/
			break;	
		
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);

	
}
//*****************************************
/********************************************************************************
 NAME:			MMIENG_InitRoamParam
 DESCRIPTION:	read NV. if failure then write NV
 PARAM IN:		 
 PARAM OUT:		
 AUTHOR:		shangke
 DATE:			2006.12.12
********************************************************************************/
PUBLIC void MMIENG_InitRoamParam(void)
{
	MN_RETURN_RESULT_E      return_value = MN_RETURN_FAILURE;
	MMINV_READ(MMINV_ENG_THRESHOLD,&g_threshold,return_value);
	    if (MN_RETURN_SUCCESS != return_value)
	    {
	        g_threshold = MMIENG_DEFAULT_THRESHOLD;
            MMINV_WRITE(MMINV_ENG_THRESHOLD,&g_threshold);
	    }
	
	MMINV_READ(MMINV_ENG_PERIODS,&g_periods,return_value);
	    if (MN_RETURN_SUCCESS != return_value)
	    {
	        g_periods = MMIENG_DEFAULT_PERIODS;
	        MMINV_WRITE(MMINV_ENG_PERIODS,&g_periods);
	    }
	
	MMINV_READ(MMINV_ENG_TIMER,&g_timer,return_value);
	    if (MN_RETURN_SUCCESS != return_value)
	    { 
	        g_timer = MMIENG_DEFAULT_TIMER;
	        MMINV_WRITE(MMINV_ENG_TIMER,&g_timer);
	    }
	
}
//*****************************************
PUBLIC void MMIENG_OpenRoamParam(void)
{
    BOOLEAN result = FALSE;

	result = MMK_CreateWin((uint32*)MMIENG_ROAM_PARAM_WIN_TAB, PNULL);
}

LOCAL void GetAPNSetting(
						 uint8 *APNcn_ptr
						 )
{
	uint16 nv_return;

	if(PNULL == APNcn_ptr)
    {
        //SCI_TRACE_LOW:"mmiengtd_win.c GetAPNSetting APNcn_ptr == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_4356_112_2_18_2_18_46_33,(uint8*)"");
        return ;
    }    

    MMINV_READ(MMINV_APN_SETTING, APNcn_ptr, nv_return);
    if (MN_RETURN_SUCCESS != nv_return)
    {
		sprintf((char*)APNcn_ptr, "cncwap.sd.cnc");
        MMINV_WRITE(MMINV_APN_SETTING, APNcn_ptr);
    }
}

LOCAL void SetAPNSetting(
						 uint8 *APNcn_ptr
						 )
{
	if(PNULL == APNcn_ptr)
    {
        //SCI_TRACE_LOW:"mmiengtd_win.c SetAPNSetting APNcn_ptr == null"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_4374_112_2_18_2_18_46_34,(uint8*)"");
        return ;
    }  

	MMINV_WRITE(MMINV_APN_SETTING, APNcn_ptr);
}

/********************************************************************************
 NAME:			MMIENG_OpenUART0BaudRateSelectWin
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:			
 DATE:			
********************************************************************************/
PUBLIC void MMIENG_OpenUART0BaudRateSelectWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_UART0BAUDRATE_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:			UART1BaudRateWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E UART1BaudRateWinHandleMsg(
                                        MMI_WIN_ID_T win_id,
                                        MMI_MESSAGE_ID_E msg_id,
                                        DPARAM param
                                        )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;

//    SCI_TRACE_LOW("UART1BaudRateWinHandleMsg(), msg_id = 0x%x", msg_id);
    switch (msg_id) 
    {
        case MSG_OPEN_WINDOW:
            AppendUART1BaudRateItem();
			MMK_SetAtvCtrl(win_id, MMIENG_UART1BAUDRATE_LISTBOX_CTRL_ID);
            break;
	        
        case MSG_CTL_CANCEL:
	        MMK_CloseWin(win_id);
	        break;

		
	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
            SaveUART1BaudRateSelect();
            MMK_CloseWin(win_id);
			break;
            
        default:
	        result = MMI_RESULT_FALSE;
	        break;
	}
	return (result);
}

/********************************************************************************
 NAME:			AppendUART1BaudRateItem
 DESCRIPTION:	
 PARAM IN:		list_id - 
 PARAM OUT:		
 AUTHOR:		
 DATE:	
  NOTE:	uart1改为 com_debug
********************************************************************************/
LOCAL void AppendUART1BaudRateItem(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIENG_UART1BAUDRATE_LISTBOX_CTRL_ID;
    uint16 sel_num = 0;
    MMI_STRING_T kstring [MMIENG_UART1BAUDRATE_NUM] = {0};
	GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
	uint32 i = 0;
	MMI_TEXT_ID_T template_text_arr[MMIENG_UART1BAUDRATE_NUM] = {
        TXT_ENG_BAUD_115200, TXT_ENG_BAUD_460800};
    uint32 baud_rate = 0;

	item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
	item_t.item_data_ptr = &item_data;
	//set softkey
	item_data.softkey_id[0] = (MMI_TEXT_ID_T)TXT_ENG_CLEAR;
	item_data.softkey_id[1] = (MMI_TEXT_ID_T)TXT_NULL;
	item_data.softkey_id[2] = (MMI_TEXT_ID_T)TXT_ENG_RETURN;
	
	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	GUILIST_SetMaxItem(ctrl_id, MMIENG_UART1BAUDRATE_NUM, FALSE );
	for (i = 0; i < MMIENG_UART1BAUDRATE_NUM; i++)
	{
        //set string context of item
        MMI_GetLabelTextByLang(template_text_arr[i], &kstring[i]);
		//AppendListItemByTextBuffer(item_data,kstring.wstr_ptr,kstring.wstr_len,FALSE);
	item_data.item_content[0].item_data.text_buffer=kstring[i];		
		GUILIST_AppendItem(ctrl_id, &item_t);
	}
	
    //get sel_num from MN
    //baud_rate = REFPARAM_GetUart1BaudRate();
	//baud_rate = SIO_GetBaudRate(1);
		baud_rate = SIO_GetBaudRate(COM_DEBUG);
    if (BAUD_115200 == baud_rate)
    {
        sel_num = 0;
    }
    else if (BAUD_460800 == baud_rate)
    {
        sel_num = 1;
    }
    else
    {
        //SCI_PASSERT(0, ("baud_rate = %d", baud_rate));
         sel_num = 1;
    }
    
	GUILIST_SetCurItemIndex(ctrl_id, sel_num);
	GUILIST_SetSelectedItem(ctrl_id, sel_num, TRUE);
}

/********************************************************************************
 NAME:			SaveUART1BaudRateSelect
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		
 DATE:	
 NOTE:	uart1改为 com_debug
********************************************************************************/
LOCAL void SaveUART1BaudRateSelect(void)
{
    uint16 sel_num = 0;
    uint32  baud_rate[2] = {BAUD_115200, BAUD_460800};
    
    sel_num = GUILIST_GetCurItemIndex(MMIENG_UART1BAUDRATE_LISTBOX_CTRL_ID);
    //set sel_num to MN
    //REFPARAM_SetUart1BaudRate(baud_rate[sel_num]);
	//SIO_SetBaudRate(1, baud_rate[sel_num]);
    SIO_SetBaudRate(COM_DEBUG, baud_rate[sel_num]);
    // 提示窗口--已完成
    /*MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);*/
}

/********************************************************************************
 NAME:			MMIENG_OpenUART1BaudRateSelectWin
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:			
 DATE:			
********************************************************************************/
PUBLIC void MMIENG_OpenUART1BaudRateSelectWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_UART1BAUDRATE_WIN_TAB, PNULL);
}

//extern uint8 g_cell_Para_Id;


//@xiefen add begin 
//Brand
LOCAL MMI_RESULT_E EngNetworkSetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	//GUI_BOTH_RECT_T rect = MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id));
	MMI_STRING_T text = {0};
	MMI_STRING_T net_freq = {PNULL, 0};
	MMI_STRING_T net_imsi = {PNULL, 0};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	MN_ENG_IMSI_T               imsi ;
	MN_ENG_FREQ_T               freq;
	int32 i = 0;
 	
	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:	
            GUIFORM_SetStyle(MMIENG_NETWORKSET_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIENG_NETWORKSET_FORM2_CTRL_ID,GUIFORM_STYLE_UNIT);

            {
                GUI_BOTH_RECT_T rect = MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id));

	        	GUIAPICTRL_SetBothRect(MMK_ConvertIdToHandle(MMIENG_NETWORKSET_FORM_CTRL_ID), &rect);	
            }
			text.wstr_ptr = L"Freq:";
			text.wstr_len = 5;
			GUILABEL_SetBackgroundColor(MMIENG_NETWORKSET_LABEL1_CTRL_ID, mmi_eng_display.bkgd_color);
			GUILABEL_SetText(MMIENG_NETWORKSET_LABEL1_CTRL_ID, &text, FALSE);
			text.wstr_ptr = L"Imsi:";
			text.wstr_len = 5;
			GUILABEL_SetBackgroundColor(MMIENG_NETWORKSET_LABEL2_CTRL_ID, mmi_eng_display.bkgd_color);
			GUILABEL_SetText(MMIENG_NETWORKSET_LABEL2_CTRL_ID, &text, FALSE);
			// set window 's title
			
			GUIWIN_SetTitleTextId(MMIENG_NETWORKSETTING_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_NETWORKSETTING, FALSE);
					
			GUIEDIT_SetStyle(MMIENG_NETWORKSET_EDIT1_CTRL_ID, GUIEDIT_STYLE_SINGLE);
			GUIEDIT_SetStyle(MMIENG_NETWORKSET_EDIT1_CTRL_ID, GUIEDIT_STYLE_SINGLE);

			MMK_SetAtvCtrl(MMIENG_NETWORKSETTING_WIN_ID, MMIENG_NETWORKSET_EDIT1_CTRL_ID);
			// first input the up link
			//s_mmiengtd_up_down_link = 0;
			break;

			/*
			case MSG_LCD_SWITCH:			
	    		GUIFORM_SetRect(MMK_ConvertIdToHandle(MMIENG_NETWORKSET_FORM_CTRL_ID),
							&rect);	
			break;
			*/
		case MSG_FULL_PAINT:
           	 ClearENGClientBG();
			GUIWIN_SetSoftkeyTextId(MMIENG_NETWORKSETTING_WIN_ID,  (MMI_TEXT_ID_T)TXT_ENG_OK, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RETURN, FALSE);
			break;			
			
		case MSG_CTL_CANCEL:
			MMK_CloseWin(MMIENG_NETWORKSETTING_WIN_ID);
			break;
	
	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
			GUIEDIT_GetString(MMIENG_NETWORKSET_EDIT1_CTRL_ID, &net_freq);
			GUIEDIT_GetString(MMIENG_NETWORKSET_EDIT2_CTRL_ID, &net_imsi);
			if ((0 == net_freq.wstr_len) && (0 == net_imsi.wstr_len))
			{
				MMK_CloseWin(MMIENG_NETWORKSETTING_WIN_ID);
			}
			else
			{
			     if (0 != net_imsi.wstr_len)
			     {
			     	    //MMINV_READ(MMINV_ENG_PHONE_IMSI,&imsi,return_value);
			    	    //if (MN_RETURN_SUCCESS != return_value)
			    	    {
			     	         imsi.isused = TRUE;
					  imsi.len = (uint8)net_imsi.wstr_len;
    					  for (i = 0; i < MN_MAX_ENG_IMSI_ARR_LEN; i++)
    					  {
    					  	imsi.val[i] =  *net_imsi.wstr_ptr++;
    					  }
			        	  //MMINV_WRITE(MMINV_ENG_PHONE_IMSI,&imsi);
			                //MMINV_READ(MMINV_ENG_PHONE_IMSI,&temp_imsi,return_value);
				     }
			      }
			      else
			     	{
			     		  imsi.isused = FALSE;
			        	  //MMINV_WRITE(MMINV_ENG_PHONE_IMSI,&imsi);
			     	}
			      if (0 != net_freq.wstr_len)
			      {
			     	    //MMINV_READ(MMINV_ENG_PHONE_FREQ,&freq,return_value);
			    	    //if (MN_RETURN_SUCCESS != return_value)
			    	    {
			     	         freq.isused = TRUE;
					  freq.info[0].len = (uint8)net_freq.wstr_len;
					  /*for (i; i < MN_MAX_ENG_FREQ_ARR_LEN; i++)
					  {
					  	freq.val[i] =  (*net_freq.str_ptr++) - 48;
					  }*/
					  freq.info[0].val = atoi((char*)net_freq.wstr_ptr);
			        	  //MMINV_WRITE(MMINV_ENG_PHONE_FREQ,&freq);
			                //MMINV_READ(MMINV_ENG_PHONE_FREQ,&temp_freq,return_value);
			    	    	}
				}
			      else
			     	{
			     		  freq.isused = FALSE;
			        	  //MMINV_WRITE(MMINV_ENG_PHONE_IMSI,&freq);
			     	}
			}
			MMK_CloseWin(MMIENG_NETWORKSETTING_WIN_ID);
			break;
		/*	
		case MSG_KEYDOWN_UP:
		case MSG_KEYDOWN_DOWN:
			if (0 == s_mmiengtd_up_down_link)
			{
				MMK_SetAtvCtrl(MMIENG_NETWORKSETTING_WIN_ID, MMIENG_EDITBOX3_CTRL_ID);
				s_mmiengtd_up_down_link = 1;
			}
			else
			{
				MMK_SetAtvCtrl(MMIENG_NETWORKSETTING_WIN_ID, MMIENG_EDITBOX1_CTRL_ID);
				s_mmiengtd_up_down_link = 0;
			}
			break;
		*/
		
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);
}


//@sam.xia
LOCAL MMI_RESULT_E EngIMSISetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_STRING_T text = {0};
	MMI_STRING_T net_imsi = {PNULL, 0};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	MN_ENG_IMSI_T               imsi ;
   	
	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
            GUIFORM_SetStyle(MMIENG_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);

	       	  {
           		 GUI_BOTH_RECT_T rect = MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id));

	   	 	GUIAPICTRL_SetBothRect(MMK_ConvertIdToHandle(MMIENG_FORM_CTRL_ID), &rect);	
          		  }
		
			text.wstr_ptr = (uint8*)"Imsi:";/*lint !e64*/
			text.wstr_len = 5;
			GUILABEL_SetBackgroundColor(MMIENG_LABEL1_CTRL_ID, mmi_eng_display.bkgd_color);
			GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, FALSE);
			
			GUIWIN_SetTitleTextId(MMIENG_ENG_IMSI_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_IMSI, FALSE);
					
			GUIEDIT_SetStyle(MMIENG_EDITBOX1_CTRL_ID, GUIEDIT_STYLE_SINGLE);

			MMK_SetAtvCtrl(MMIENG_ENG_IMSI_WIN_ID, MMIENG_EDITBOX1_CTRL_ID);
			// first input the up link
			break;

				
		case MSG_FULL_PAINT:
            		ClearENGClientBG();
			break;
			
		case MSG_CTL_CANCEL:
			MMK_CloseWin(MMIENG_ENG_IMSI_WIN_ID);
			break;
	
	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
			GUIEDIT_GetString(MMIENG_EDITBOX1_CTRL_ID, &net_imsi);
			if ((0 != net_imsi.wstr_len))
			{
                            uint8 temp[18];
			     	imsi.isused = TRUE;
                            
                            //SCI_TRACE_LOW:"mmieng_wintd.c EngIMSISetWinHandleMsg net_imsi.wstr_len is %d"
                            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_4723_112_2_18_2_18_47_35,(uint8*)"d",net_imsi.wstr_len);
                            if((uint8)net_imsi.wstr_len==15)
                            {
                                break;
                            }
                            SCI_MEMCPY(temp, "809", 3);
                            SCI_MEMCPY(temp+3, net_imsi.wstr_ptr, 15);
				imsi.len = 18/2;
				MMIAPICOM_StrToBcdLf((char *)temp, 18, imsi.val);
                            //
                            imsi.len = MN_MAX_ENG_IMSI_ARR_LEN;
 //                           DMNV_SetIMSI(imsi);
			}
                    MMK_CloseWin(MMIENG_ENG_IMSI_WIN_ID);                                        
			break;			
		
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);
}


LOCAL MMI_RESULT_E EngFreqSetWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_STRING_T text = {0};
	MMI_STRING_T net_freq[4] = {{PNULL, 0},{PNULL, 0},{PNULL, 0},{PNULL, 0}};
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	MN_ENG_FREQ_T               freq;
	uint16 i;
	uint16 len_sum = 0;
  
	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
			  {
           		 GUI_BOTH_RECT_T rect = MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id));

	   	 	 GUIAPICTRL_SetBothRect(MMK_ConvertIdToHandle(MMIENG_FORM_CTRL_ID), &rect);	
          		  }
		
			text.wstr_ptr = (uint8*)"Freq:";/*lint !e64*/
			text.wstr_len = 5;
			GUILABEL_SetBackgroundColor(MMIENG_LABEL1_CTRL_ID, mmi_eng_display.bkgd_color);
			GUILABEL_SetText(MMIENG_LABEL1_CTRL_ID, &text, FALSE);
			// set window 's title
			
			GUIWIN_SetTitleTextId(MMIENG_ENG_FREQ_WIN_ID, (MMI_TEXT_ID_T)TXT_ENG_IMSI, FALSE);
					
			GUIEDIT_SetStyle(MMIENG_EDITBOX1_CTRL_ID, GUIEDIT_STYLE_SINGLE);
			GUIEDIT_SetStyle(MMIENG_EDITBOX3_CTRL_ID, GUIEDIT_STYLE_SINGLE);
			GUIEDIT_SetStyle(MMIENG_EDITBOX4_CTRL_ID, GUIEDIT_STYLE_SINGLE);
			GUIEDIT_SetStyle(MMIENG_EDITBOX5_CTRL_ID, GUIEDIT_STYLE_SINGLE);

			MMK_SetAtvCtrl(MMIENG_ENG_FREQ_WIN_ID, MMIENG_EDITBOX1_CTRL_ID);
			// first input the up link
			//s_mmiengtd_up_down_link = 0;
			break;
				
		case MSG_FULL_PAINT:
          		  ClearENGClientBG();
			GUIWIN_SetSoftkeyTextId(MMIENG_ENG_FREQ_WIN_ID,  (MMI_TEXT_ID_T)TXT_ENG_OK, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)TXT_ENG_RETURN, FALSE);
			break;
			
		
		case MSG_CTL_CANCEL:
			MMK_CloseWin(MMIENG_ENG_FREQ_WIN_ID);
			break;
	
	
	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
			GUIEDIT_GetString(MMIENG_EDITBOX1_CTRL_ID, &net_freq[0]);
			GUIEDIT_GetString(MMIENG_EDITBOX3_CTRL_ID, &net_freq[1]);
			GUIEDIT_GetString(MMIENG_EDITBOX4_CTRL_ID, &net_freq[2]);
			GUIEDIT_GetString(MMIENG_EDITBOX5_CTRL_ID, &net_freq[3]);
		
			freq.isused = FALSE;
                     for (i=0 ; i < 0X04; i++)
			{
				//check the input 0 or have value
			      if ((0 == net_freq[i].wstr_len) )
			      {
					//check whether have the input before 
					if( len_sum > 0 )
					{
						freq.info[i].len= freq.info[i-1].len;
						freq.info[i].val = freq.info[i-1].val;
					}
			      }
			      else
			     {
					len_sum += net_freq[i].wstr_len;
					freq.info[i].len= (uint8)net_freq[i].wstr_len;
					freq.info[i].val = atoi((char*)net_freq[i].wstr_ptr);

					//check the input reach the 5 bit
					//if (freq.info[i].len < 5)
					//{
					//	freq.info[i].val = freq.info[i].val * 10^(5 - freq.info[i].len);
					//	freq.info[i].len = 5;
					//}					
				}
			}

			if (0 != len_sum)
			{
				freq.isused = TRUE;
			}
//			DMNV_SetFreqList(freq);
			MMK_CloseWin(MMIENG_ENG_FREQ_WIN_ID);
			break;
		/*	
		case MSG_KEYDOWN_UP:
		case MSG_KEYDOWN_DOWN:
			if (0 == s_mmiengtd_up_down_link)
			{
				MMK_SetAtvCtrl(MMIENG_ENG_FREQ_WIN_ID, MMIENG_EDITBOX3_CTRL_ID);
				s_mmiengtd_up_down_link = 1;
			}
			else if  (1 == s_mmiengtd_up_down_link)
			{
				MMK_SetAtvCtrl(MMIENG_ENG_FREQ_WIN_ID, MMIENG_EDITBOX4_CTRL_ID);
				s_mmiengtd_up_down_link = 2;
			}
			else if (2 == s_mmiengtd_up_down_link)
			{
				MMK_SetAtvCtrl(MMIENG_ENG_FREQ_WIN_ID, MMIENG_EDITBOX5_CTRL_ID);
				s_mmiengtd_up_down_link = 3;
			}
			else
			{
				MMK_SetAtvCtrl(MMIENG_ENG_FREQ_WIN_ID, MMIENG_EDITBOX1_CTRL_ID);
				s_mmiengtd_up_down_link = 0;
			}
			break;
			*/		
		
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);
}

/********************************************************************************
 NAME:			EngShowNetworkInfoWinHandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT: 	
 AUTHOR:		sam.xia 
 DATE:			2005.05.20
********************************************************************************/
LOCAL MMI_RESULT_E EngShowNetworkInfoWinHandleMsg(
                                                  MMI_WIN_ID_T win_id, 
                                                  MMI_MESSAGE_ID_E msg_id, 
                                                  DPARAM param)
{
    uint8 buf[251] = {0};
    MMI_STRING_T text = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        text.wstr_len = GetNetworkInfoStr(buf, 250);
        text.wstr_ptr = buf;/*lint !e64*/
        GUITEXT_SetString(MMIENG_TEXTBOX1_CTRL_ID, text.wstr_ptr,text.wstr_len, FALSE);
        MMK_SetAtvCtrl(MMIENG_SHOWNETWORKINFO_WIN_ID, MMIENG_TEXTBOX1_CTRL_ID);
        break;
        
    case MSG_CTL_OK:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(MMIENG_SHOWNETWORKINFO_WIN_ID);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}

/********************************************************************************
 NAME:			GetNetworkInfoStr
 DESCRIPTION:	
 PARAM IN:		phone_buf - 
				buf_len - 
 PARAM OUT:		phone info string len
 AUTHOR:		allen
 DATE:			2004.09.25
********************************************************************************/
LOCAL int GetNetworkInfoStr(uint8* network_buf, uint8 buf_len)
{
    char tmp_buf1[20] = {0};
    uint32 offset = 0;
    uint32 len = 0;
    int i = 0;

    if(PNULL == network_buf)
    {
        //SCI_TRACE_LOW:"mmiengtd_win.c GetNetworkInfoStr str_buf == network_buf"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_4925_112_2_18_2_18_47_36,(uint8*)"");
        return -1;
    }    
    //SCI_TRACE_LOW:"mmieng_wintd.c GetNetworkInfoStr buf_len is %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_4928_112_2_18_2_18_47_37,(uint8*)"d",buf_len);
    if(2 > buf_len)
    {
        return -2;
    }
    
    //////////////
    // Frequency list
    //////////////
    MMI_MEMCPY(network_buf, buf_len-1, "<Frequency>", 11, 11);
    offset = 11;
    network_buf[offset] = CR_CHAR;
    offset += 1;
    for ( i = 0; i < g_freq_num;  i++)
    {
        SCI_MEMSET(tmp_buf1, 0, 5);
//        MMIAPICOM_Int2Str( g_freq_list[i],  (uint8 *)tmp_buf1, 5);
////        itoa( g_freq_list[i],  (uint8 *)tmp_buf1, 5);
        len = SCI_STRLEN(tmp_buf1);
        if(offset + len +1 < (uint32)buf_len - 1)
        {
            MMI_MEMCPY(network_buf + offset, (uint32)buf_len - offset-1, tmp_buf1, len, len);
            offset += len;
            network_buf[offset] = CR_CHAR;
            offset += 1;
        }
    }
    
    //////////////
    // IMSI 
    //////////////
    MMI_MEMCPY(network_buf + offset, (uint32)buf_len - offset-1, "<IMSI:>", 7, 7);
    offset += 7;
    network_buf[offset] = CR_CHAR;    
    offset += 1;
    //
    SCI_MEMSET(tmp_buf1, 0, 20);
    
    MMIAPICOM_BcdLfToStr( g_imsi, 9*2, (char *)tmp_buf1);
    len = SCI_STRLEN(tmp_buf1)-3;//remove "809"
    MMI_MEMCPY(network_buf + offset, (uint32)buf_len - offset-1, tmp_buf1+3, len, len);
    offset += len;
    network_buf[offset] = CR_CHAR;/*lint !e661*/
    offset += 1;

    //////////////
    // SIM Simulate
    //////////////
    MMI_MEMCPY(network_buf + offset, (uint32)buf_len - offset-1, "<SIM Simulate>", 14, 14);
    offset += 14;
    network_buf[offset] = CR_CHAR;    
    offset += 1;
    
   if((g_spss_code >> 7) & 0x01)
  {
       MMI_MEMCPY(network_buf + offset, (uint32)buf_len - offset-1, "Off", 3, 3);/*lint !e669*/
      offset += 3;        
   }
    else
   {
        MMI_MEMCPY(network_buf + offset, (uint32)buf_len - offset-1, "On", 2, 2);/*lint !e669*/
      offset += 2;            
   }
    network_buf[offset] = CR_CHAR;/*lint !e662*//*lint !e661*/
    offset += 1;

    //////////////
    // Uart1 used by
    //////////////
    MMI_MEMCPY(network_buf + offset, (uint32)buf_len - offset-1, "<Uart1 used by>", 15, 15);
    offset += 15;
    network_buf[offset] = CR_CHAR;    
    offset += 1;

    switch(v_com_uart1Flag)
    {   
        case 0x00:
            MMI_MEMCPY(network_buf + offset, (uint32)buf_len - offset-1, "ARM Ref", 7, 7);
            offset += 7;        
            break;
        case 0x02:
            MMI_MEMCPY(network_buf + offset, (uint32)buf_len - offset-1, "Dsp1", 4, 4);
            offset += 4;        
            break;
        case 0x01:
        MMI_MEMCPY(network_buf + offset, (uint32)buf_len - offset-1, "CYPS", 4, 4);
        offset += 4;        
            break;
        default:
            break;
    }   


    network_buf[offset] = CR_CHAR;
    offset += 1;

    //////////////
    // Deep Sleep
    //////////////
    MMI_MEMCPY(network_buf + offset, (uint32)buf_len - offset-1, "<Deep Sleep>", 12, 12);
    offset += 12;
    network_buf[offset] = CR_CHAR;    
    offset += 1;
 
    if(v_tgl_sleepFlag == 0x00)
    {
        MMI_MEMCPY(network_buf + offset, (uint32)buf_len - offset-1, "Off", 3, 3);
        offset += 3;        
    }
    else
    {
        MMI_MEMCPY(network_buf + offset, (uint32)buf_len - offset-1, "On", 2, 2);
        offset += 2;            
    }

    network_buf[offset] = CR_CHAR;
    offset += 1;
    
    //////////////
    // RF Loop
    //////////////
    MMI_MEMCPY(network_buf + offset, (uint32)buf_len - offset-1, "<RF Loop>", 9, 9);
    offset += 9;
    network_buf[offset] = CR_CHAR;    
    offset += 1;
 
    if(v_tgl_rfLoopFlag == 0x00)
    {
        MMI_MEMCPY(network_buf + offset, (uint32)buf_len - offset-1, "Off", 3, 3);
        offset += 3;        
    }
    else
    {
        MMI_MEMCPY(network_buf + offset, (uint32)buf_len - offset-1, "On", 2, 2);
        offset += 2;
    }

    network_buf[offset] = CR_CHAR;
    offset += 1;

    return ((int32)offset);
}

/*****************************************************************************/
// 	Description : handle message of input apn window of EnG
//	Global resource dependence : none
//  Author: wancan.you
//	Note:11/17/2005
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleApnSettingWinMsg(
                                             MMI_WIN_ID_T   win_id, 
                                             MMI_MESSAGE_ID_E   msg_id, 
                                             DPARAM             param
                                             )
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_STRING_T string_info = {0};
    wchar APNcn[MMIENG_MAX_APN_LEN] = {0};// = "cncwap.sd.cnc";
    uint8 APNTemp[MMIENG_MAX_APN_LEN]={0};
    MN_GPRS_PDP_APN_T  apn = {0};    
    
   
    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUI_BOTH_RECT_T rect = MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id));

            GUIAPICTRL_SetBothRect(MMK_ConvertIdToHandle(MMIENG_APN_SETTING_CTRL_ID), &rect);   
        }
        
#ifdef _WIN32
        GetAPNSetting(APNTemp);
#else
        /* FIX Compile error , zhigang.peng 2019-07-15 */
        MNGPRS_GetDailAPN(MN_DUAL_SYS_1,1,&apn);
        SCI_MEMCPY(APNTemp,apn.value_arr, MIN(apn.length,MN_GPRS_MAX_APN_LEN));

#endif
        //set im
        MMI_STRNTOWSTR(APNcn,   MMIENG_MAX_APN_LEN, APNTemp,MMIENG_MAX_APN_LEN,strlen((char*)APNTemp));
        GUIEDIT_SetIm(MMIENG_APN_SETTING_CTRL_ID,GUIIM_TYPE_ABC|GUIIM_TYPE_DIGITAL,GUIIM_TYPE_ABC);/*lint !e655 */

        //Default APN
        GUIEDIT_SetString(MMIENG_APN_SETTING_CTRL_ID,
                                            APNcn,
                                            MMIAPICOM_Wstrlen(APNcn));
        MMK_SetAtvCtrl(
                    MMIENG_APN_SETTING_WIN_ID,
                    MMIENG_APN_SETTING_CTRL_ID);
        break;
 
    case MSG_CTL_OK:
    case MSG_CTL_MIDSK:
    case MSG_APP_WEB:
            //SAVE FDN
            GUIEDIT_GetString(
                                MMIENG_APN_SETTING_CTRL_ID,
                                &string_info);
  
            SCI_MEMSET(APNTemp, 0, MMIENG_MAX_APN_LEN);
            MMI_WSTRNTOSTR(APNTemp,MMIENG_MAX_APN_LEN,string_info.wstr_ptr, string_info.wstr_len,string_info.wstr_len);
            //SCI_MEMCPY(APNcn, string_info.wstr_ptr, string_info.wstr_len);

#ifdef WIN32
           if(TRUE)/*lint !e774 !e506*/
#else

            if(0 == MNGPRS_SetPdpContextAPN(MN_DUAL_SYS_1, APNTemp))
#endif
            {
                //Success
               /* MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);*/
#ifdef WIN32
                SetAPNSetting(APNTemp);
#endif
            }
            else
            {
                //Failure
                MMIPUB_OpenAlertFailWin(TXT_TEMP_FAILURE);
            }

            MMK_CloseWin(win_id);
            break;
    
    case MSG_CTL_CANCEL:        
        MMK_CloseWin(win_id);//返回上一级
        break;

    default:
        //SCI_TRACE_LOW:"HandleApnSettingWinMsgg() msg_id = %d"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_5157_112_2_18_2_18_48_38,(uint8*)"d", msg_id);
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;

}
/********************************************************************************
 NAME:			MMIENGTD_SetFallbackWin
 DESCRIPTION:	
 AUTHOR:		yongwei.he	
 DATE:		07/26/2007	
********************************************************************************/
PUBLIC void MMIENGTD_SetFallbackWin(void)
{
    MMK_CreateWin((uint32*)MMIENGTD_FALLBACK_WIN_TAB, PNULL);
}
/********************************************************************************
 NAME:			MMIENGTD_SetAnswerFastWin
 DESCRIPTION:	
 AUTHOR:		yongwei.he	
 DATE:		07/26/2007	
********************************************************************************/
PUBLIC void MMIENGTD_SetAnswerFastWin(void)
{
    MMK_CreateWin((uint32*)MMIENGTD_ANSWERFAST_WIN_TAB, PNULL);
}
/********************************************************************************
 NAME:			MMIENGTD_SetAnswerFastWin
 DESCRIPTION:	
 AUTHOR:		yongwei.he	
 DATE:		07/26/2007	
********************************************************************************/
PUBLIC void MMIENGTD_SetVTVideoTypeWin(void)
{
    MMK_CreateWin((uint32*)MMIENGTD_VTVideoType_WIN_TAB, PNULL);
}
/********************************************************************************
 NAME:			MMIENGTD_SetL1DSPParamWin
 DESCRIPTION:	
 AUTHOR:		yongwei.he	
 DATE:		08/29/2007	
********************************************************************************/
PUBLIC void MMIENGTD_SetL1DSPParamWin(void)
{
    MMK_CreateWin((uint32*)MMIENGTD_L1DSPPARAM_SET_WIN_TAB, PNULL);
}
/********************************************************************************
 NAME:			HandlePichSettingWinMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
#ifdef _WIN32
uint8 s_pich_rev_mode = 0;
#endif

#ifndef TD_AS_NOT_PRESENT
extern void l1_set_pich_rev_mode(uint8 pich_rev_mode);
extern uint8 l1_get_pich_rev_mode(void);
#endif
LOCAL MMI_RESULT_E HandlePichSettingWinMsg(
							MMI_WIN_ID_T win_id,
                                           MMI_MESSAGE_ID_E msg_id,
                                           DPARAM param
                                           )
{
//	GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
	MMI_STRING_T pich_cycle_str = {PNULL, 0};
	uint16 pich_rev_mode = 0;
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	
   	uint8 buf[4]={0};		
   	
	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:    
		{
                GUI_BOTH_RECT_T rect = MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id));
	        GUIAPICTRL_SetBothRect(MMK_ConvertIdToHandle(MMIENG_PICH_EDITBOX_CTRL_ID), &rect);	
            }
  
#ifndef _WIN32
#ifndef TD_AS_NOT_PRESENT
		pich_rev_mode = (uint16)l1_get_pich_rev_mode();
#endif
#else
		pich_rev_mode = s_pich_rev_mode;
#endif

		pich_cycle_str.wstr_ptr = (wchar *)SCI_ALLOC_APP(4*sizeof(wchar));
		//sprintf((char*)pich_cycle_str.str_ptr,"%3d", pich_rev_mode);
		//pich_cycle_str.str_len = MMIAPICOM_Wstrlen(pich_cycle_str.str_ptr);
		ConvertDigit2String((uint8 *)buf, &pich_cycle_str.wstr_len, pich_rev_mode);
		MMIAPICOM_StrToWstr(buf, pich_cycle_str.wstr_ptr);
		GUIEDIT_SetString(MMIENG_PICH_EDITBOX_CTRL_ID,(wchar *)pich_cycle_str.wstr_ptr, pich_cycle_str.wstr_len);
		SCI_FREE(pich_cycle_str.wstr_ptr);
		MMK_SetAtvCtrl(win_id, MMIENG_PICH_EDITBOX_CTRL_ID);
		break;

	case MSG_FULL_PAINT:
		//LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
		break;
		

	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
		

	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
		GUIEDIT_GetString(MMIENG_PICH_EDITBOX_CTRL_ID, &pich_cycle_str);
		MMIAPICOM_WstrToStr(pich_cycle_str.wstr_ptr, buf);
		if (pich_cycle_str.wstr_len > 0)
		{
			pich_rev_mode = atoi((char *)buf);
			if(255 >= pich_rev_mode)
			{
				#ifndef _WIN32
#ifndef TD_AS_NOT_PRESENT
				l1_set_pich_rev_mode((uint8)pich_rev_mode);     /*lint !e628 !e718 !e746*/
#endif
				#else
				s_pich_rev_mode = pich_rev_mode;
				#endif
				MMK_CloseWin(win_id);
			}
			else
			{
				MMIPUB_OpenAlertWarningWin(TXT_INVAILD_DEFLECTED_NUMBER);
			}
				
		}
		else
		{
			MMK_SetAtvCtrl(win_id, MMIENG_PICH_EDITBOX_CTRL_ID);
		}
		break;
	
	default:
		result = MMI_RESULT_FALSE;
		break;
	}

	return (result);
}
/********************************************************************************
 NAME:			HandlePSBitrateSelWinMsg
 DESCRIPTION:	
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
#if 0
LOCAL MMI_RESULT_E HandlePSBitrateSelWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	
	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
            		AppendPSBitrateSelectItem();
			MMK_SetAtvCtrl(win_id, MMIENG_PSBITRATE_LISTBOX_CTRL_ID);
			break;
			
		case MSG_FULL_PAINT:
			break;
			
		case MSG_KEYDOWN_OK:
		case MSG_KEYDOWN_CANCEL:
			MMK_CloseWin(win_id);
			break;
			
		case MSG_KEYDOWN_FLIP:		// close flip
		case MSG_KEYDOWN_RED:
			break;
			
		case MSG_CTL_CANCEL:
            		MMK_CloseWin(win_id);
			break;
	

	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
            		SavePSBitrateSelectItem();
            		MMK_CloseWin(win_id);
			break;
	
		case MSG_LOSE_FOCUS:
			break;
			
		case MSG_GET_FOCUS:
			break;
			
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);	
}     

#if 1
uint16 s_current_ps_bitrate = 0;
#else
/*+CR105931*/
extern MNGPRS_3G_EXTEND_QOS_SET_T MNGPRS_GetExtendQos(MN_PS_SERVICE_TYPE_E ps_service_type);
/*-CR105931*/
#endif
/********************************************************************************
 NAME:			AppendPSBitrateSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL void AppendPSBitrateSelectItem(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIENG_PSBITRATE_LISTBOX_CTRL_ID;
    uint16 sel_num = 0;
    MMI_STRING_T kstring[MMIENG_PSBITRATE_SEL_NUM];
	GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint32 i = 0;
    /*lint -save -e785 */
    uint16 psbitrate_text_arr[MMIENG_PSBITRATE_SEL_NUM][20] = {L"8k/8k", L"16k/16k", L"16k/32k", L"64k/64k", L"64k/128k", L"64k/384k",
								L"128k/64k", L"128k/128k", L"128k/384k",L"384k/64k", L"384k/128k"};//@zhaohui,cr108979
    /*lint -restore */
#if 0
    /*+CR105931*/
   MNGPRS_3G_EXTEND_QOS_SET_T ext_qos = MNGPRS_GetExtendQos(2);
    /*-CR105931*/
#endif

   item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
	item_t.item_data_ptr = &item_data;   
	//set softkey
	item_data.softkey_id[0] = TXT_ENG_CLEAR;
	item_data.softkey_id[1] = TXT_NULL;
	item_data.softkey_id[2] = TXT_ENG_RETURN;
	
	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	GUILIST_SetMaxItem(ctrl_id, MMIENG_PSBITRATE_SEL_NUM,FALSE);//@zhaohui modify,cr108979
	for (i = 0; i < MMIENG_PSBITRATE_SEL_NUM; i++)
	{
	        //set string context of item
                kstring[i].wstr_ptr = psbitrate_text_arr[i];
                kstring[i].wstr_len = MMIAPICOM_Wstrlen(kstring[i].wstr_ptr);
				//AppendListItemByTextBuffer(item_data,kstring.wstr_ptr,kstring.wstr_len,FALSE);
		item_data.item_content[0].item_data.text_buffer=kstring[i];		
				GUILIST_AppendItem(ctrl_id, &item_t);
	}
	
#if 0
	//@zhaohui,cr108979,start
	switch(ext_qos.max_bt_ul)
	{
	case 8:
				sel_num = 0;
				break;
				
			case 16:
				if (16 == ext_qos.max_bt_dl)
				{
					sel_num = 1;
				}
				else if (32 == ext_qos.max_bt_dl)
				{
					sel_num = 2;
				}
				else
				{
					SCI_ASSERT(0);
				}
				break;
				
			case 64:
				if (64 == ext_qos.max_bt_dl)
				{
					sel_num = 3;
				}
				else if (128 == ext_qos.max_bt_dl)
				{
					sel_num = 4;
				}
				else if (384 == ext_qos.max_bt_dl)
				{
					sel_num = 5;
				}	
				else
				{
					SCI_ASSERT(0);
				}
				break;
				
			case 128:
				if (64 == ext_qos.max_bt_dl)
				{
					sel_num = 6;
				}
				else if (128 == ext_qos.max_bt_dl)
				{
					sel_num = 7;
				}
				else if (384 == ext_qos.max_bt_dl)
				{
					sel_num = 8;
				}	
				else
				{
					SCI_ASSERT(0);
				}
				break;
				
			case 384:
				if (64 == ext_qos.max_bt_dl)
				{
					sel_num = 9;
				}
				else if (128 == ext_qos.max_bt_dl)
				{
					sel_num = 10;
				}
				else
				{
					SCI_ASSERT(0);
				}
				break;

			default:
                          SCI_ASSERT(0);
				break;
				
		}
//@zhaohui,cr108979,end	
#else
		sel_num = s_current_ps_bitrate;
#endif     
	GUILIST_SetCurItemIndex(ctrl_id, sel_num);
	GUILIST_SetSelectedItem(ctrl_id, sel_num, TRUE);
}
#endif  

/********************************************************************************
 NAME:			SavePSBitrateSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
#if 0
LOCAL void SavePSBitrateSelectItem(void)
{
    uint16 sel_num = 0;
    uint16 kUpBitRate = 0;
    uint16 kDownBitRate = 0;
    
    sel_num = GUILIST_GetCurItemIndex(MMIENG_PSBITRATE_LISTBOX_CTRL_ID);
#ifndef _WIN32
                switch(sel_num)//@zhaohui,cr108979
                {
                    case 0:  
				kUpBitRate = 8;
				kDownBitRate	= 8;	
                        break;
                    case 1:     
				kUpBitRate = 16;
				kDownBitRate	= 16;	
                        break;
                    case 2:     
				kUpBitRate = 16;
				kDownBitRate	= 32;	
                        break;
                    case 3:     
				kUpBitRate = 64;
				kDownBitRate	= 64;	
                        break;        
                    case 4:   
				kUpBitRate = 64;
				kDownBitRate	= 128;	
                        break;
                    case 5:    
				kUpBitRate = 64;
				kDownBitRate	= 384;	
                        break;  
                    case 6:     
				kUpBitRate = 128;
				kDownBitRate	= 64;	
                        break;						
                    case 7:  
				kUpBitRate = 128;
				kDownBitRate	= 128;	
                        break;
                    case 8:     
				kUpBitRate = 128;
				kDownBitRate	= 384;	
                        break;
                    case 9:     
				kUpBitRate = 384;
				kDownBitRate	= 64;	
                        break;
                    case 10:     
				kUpBitRate = 384;
				kDownBitRate	= 128;	
                        break;     
                    default: 
                        //SCI_TRACE_LOW:"mmieng_wintd.c SavePSBitrateSelectItem sel_num is %d"
                        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_5555_112_2_18_2_18_49_39,(uint8*)"d",sel_num);
                        break;
                }
                MMIAPIENGTD_SetPsBitRate(kUpBitRate,kDownBitRate);
#else
		s_current_ps_bitrate = sel_num;
#endif
    
    // 提示窗口--已完成
    MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
}
#endif
/********************************************************************************
 NAME:			HandleFallbackWinMsg
 DESCRIPTION:	
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleRecordAudioTypeWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	
	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
            AppendRecordAudioTypeSelectItem();
			MMK_SetAtvCtrl(win_id, MMIENG_FALLBACK_LISTBOX_CTRL_ID);
			break;
			
		case MSG_FULL_PAINT:
			break;
			
		case MSG_KEYDOWN_OK:
		case MSG_KEYDOWN_CANCEL:
			MMK_CloseWin(win_id);
			break;

#ifdef FLIP_PHONE_SUPPORT			
		case MSG_KEYDOWN_FLIP:		// close flip
#endif		
		case MSG_KEYDOWN_RED:
			break;
			
		case MSG_CTL_CANCEL:
            		MMK_CloseWin(win_id);
			break;
	
		
		
	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
            SaveRecordAudioTypeSelectItem();
            		MMK_CloseWin(win_id);
			break;
	
		case MSG_LOSE_FOCUS:
			break;
			
		case MSG_GET_FOCUS:
			break;
			
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);	
} 
/********************************************************************************
 NAME:			AppendFallbackSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL void AppendRecordAudioTypeSelectItem(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIENG_FALLBACK_LISTBOX_CTRL_ID;
    uint16 sel_num = 0;
    //MMI_STRING_T kstring[3] = {0};
	GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint32 i = 0;
    MMI_TEXT_ID_T fallback_text_arr[3] = {TXT_ENG_REOCRD_AUDIO_TYPE_MIXED, TXT_ENG_REOCRD_AUDIO_TYPE_REMOTE, TXT_ENG_REOCRD_AUDIO_TYPE_LOCAL};

	item_t.item_style    = GUIITEM_STYLE_ONE_LINE_TEXT;
	item_t.item_data_ptr = &item_data;	
	//set softkey
	item_data.softkey_id[0] = TXT_ENG_CLEAR;
	item_data.softkey_id[1] = TXT_NULL;
	item_data.softkey_id[2] = TXT_ENG_RETURN;
	
	
	GUILIST_SetMaxItem(ctrl_id, 3, FALSE );
	for (i = 0; i < 3; i++)
	{
	    //set string context of item
	    //MMI_GetLabelTextByLang(fallback_text_arr[i], &kstring[i]);
		//AppendListItemByTextBuffer(item_data,kstring.wstr_ptr,kstring.wstr_len,FALSE);
	    //item_data.item_content[0].item_data.text_buffer=kstring[i];
        item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
        item_data.item_content[0].item_data.text_id = fallback_text_arr[i];
		GUILIST_AppendItem(ctrl_id, &item_t);
	}
	
#ifdef VT_SUPPORT
    sel_num = MMIAPIVT_GetRecordAudioType();
#endif

	GUILIST_SetCurItemIndex(ctrl_id, sel_num);
	GUILIST_SetSelectedItem(ctrl_id, sel_num, TRUE);
}
/********************************************************************************
 NAME:			SaveFallbackSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL void SaveRecordAudioTypeSelectItem(void)
{
    uint16 sel_num = 0;
    
    sel_num = GUILIST_GetCurItemIndex(MMIENG_FALLBACK_LISTBOX_CTRL_ID);
#ifdef VT_SUPPORT
    MMIAPIVT_SetRecordAudioType(sel_num);
#endif
    
    // 提示窗口--已完成
   /* MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);*/
}
/********************************************************************************
 NAME:			HandleAnswerFastWinMsg
 DESCRIPTION:	
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleAnswerFastWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	
	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
            		AppendAnswerFastSelectItem();
			MMK_SetAtvCtrl(win_id, MMIENG_ANSWERFAST_LISTBOX_CTRL_ID);
			break;
			
		case MSG_FULL_PAINT:
			break;
			
		case MSG_KEYDOWN_OK:
		case MSG_KEYDOWN_CANCEL:
			MMK_CloseWin(win_id);
			break;

#ifdef FLIP_PHONE_SUPPORT			
		case MSG_KEYDOWN_FLIP:		// close flip
#endif		
		case MSG_KEYDOWN_RED:
			break;
			
		case MSG_CTL_CANCEL:
            		MMK_CloseWin(win_id);
			break;
	

	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
            		SaveAnswerFastSelectItem();
            		MMK_CloseWin(win_id);
			break;
	
		case MSG_LOSE_FOCUS:
			break;
			
		case MSG_GET_FOCUS:
			break;
			
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);	
}
/********************************************************************************
 NAME:			HandleAnswerFastWinMsg
 DESCRIPTION:	
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleVTVideoSetting(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	
	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
            		AppendVTVideoSettingSelectItem();
			MMK_SetAtvCtrl(win_id, MMIENG_VTVIDEOSETTING_LISTBOX_CTRL_ID);
			break;
			
		case MSG_FULL_PAINT:
			break;
			
		case MSG_KEYDOWN_OK:
		case MSG_KEYDOWN_CANCEL:
			MMK_CloseWin(win_id);
			break;

#ifdef FLIP_PHONE_SUPPORT			
		case MSG_KEYDOWN_FLIP:		// close flip
#endif		
		case MSG_KEYDOWN_RED:
			break;
			
		case MSG_CTL_CANCEL:
            		MMK_CloseWin(win_id);
			break;
	

	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
            		SaveVTVideoSettingSelectItem();
            		MMK_CloseWin(win_id);
			break;
	
		case MSG_LOSE_FOCUS:
			break;
			
		case MSG_GET_FOCUS:
			break;
			
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);	
}
/********************************************************************************
 NAME:			AppendAnswerFastSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL void AppendAnswerFastSelectItem(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIENG_ANSWERFAST_LISTBOX_CTRL_ID;
    uint16 sel_num = 0;
    MMI_STRING_T kstring[2]  = {0};
	GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint32 i = 0;
    MMI_TEXT_ID_T answerfast_text_arr[2] = {TXT_ENG_DISABLE, TXT_ENG_ENABLE};
	item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
	item_t.item_data_ptr = &item_data;
	//set softkey
	item_data.softkey_id[0] = TXT_ENG_CLEAR;
	item_data.softkey_id[1] = TXT_NULL;
	item_data.softkey_id[2] = TXT_ENG_RETURN;
	
	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	GUILIST_SetMaxItem(ctrl_id, 2, FALSE );
	for (i = 0; i < 2; i++)
	{
	        //set string context of item
	        MMI_GetLabelTextByLang(answerfast_text_arr[i], &kstring[i]);
			//AppendListItemByTextBuffer(item_data,kstring.wstr_ptr,kstring.wstr_len,FALSE);
			item_data.item_content[0].item_data.text_buffer=kstring[i];	
			GUILIST_AppendItem(ctrl_id, &item_t);
	}
#ifdef VT_SUPPORT
    sel_num = (uint16)MMIAPIVT_GetIsEPDefault(); 
#endif
	
	GUILIST_SetCurItemIndex(ctrl_id, sel_num);
	GUILIST_SetSelectedItem(ctrl_id, sel_num, TRUE);
}


/********************************************************************************
 NAME:			AppendAnswerFastSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL void SetListBoxItems(MMI_WIN_ID_T win_id, 
						   MMI_CTRL_ID_T ctrl_id, 
						   GUIITEM_STYLE_E item_style, 
						   wchar *item_str_array[], 
						   uint16 item_num)
{
    MMI_STRING_T item_string = {0};
	GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint32 i = 0;
	
	if(PNULL == item_str_array)
	{
		//SCI_TRACE_LOW:"SetRadioListBoxItems item_str_array 0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_5864_112_2_18_2_18_49_40,(uint8*)"d", item_str_array);
		return;
	}
	item_t.item_style    = item_style;
	item_t.item_data_ptr = &item_data;
	//set softkey
	item_data.softkey_id[0] = TXT_COMMON_OK;
	item_data.softkey_id[1] = TXT_NULL;
	item_data.softkey_id[2] = STXT_RETURN;
	
	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	GUILIST_SetMaxItem(ctrl_id, item_num, FALSE );
	for (i = 0; i < item_num; i++)
	{
	        //set string context of item
		item_string.wstr_ptr = item_str_array[i];
		item_string.wstr_len = MMIAPICOM_Wstrlen(item_str_array[i]);
		item_data.item_content[0].item_data.text_buffer = item_string;	
		GUILIST_AppendItem(ctrl_id, &item_t);
	}
}

/********************************************************************************
 NAME:			HandleAnswerFastWinMsg
 DESCRIPTION:	
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleRequestModeWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_REQUESTMODE_LISTBOX_CTRL_ID;
	wchar         *item_str_arr[] = {L"Request Mode On", L"Request Mode Off"};
	uint16       highlight_index = 0;
	int32        request_mode = 0;
	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:
#ifdef VT_SUPPORT
#ifndef WIN32
		request_mode = MPLH324M_isRequestModeEnable(); 
		if(request_mode)
		{
			highlight_index = 0;
		}
		else
		{
			highlight_index = 1;
		}
	    //SCI_TRACE_LOW:"[MMIENG VT]  GET RequestMode  request_mode %d"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_5916_112_2_18_2_18_49_41,(uint8*)"d", request_mode);

#endif
#endif
		SetListBoxItems(win_id,
			ctrl_id,
            GUIITEM_STYLE_ONE_LINE_RADIO,
			item_str_arr,
			ARR_SIZE(item_str_arr));
		//AppendAnswerFastSelectItem();
		if(highlight_index >= ARR_SIZE(item_str_arr))/*lint !e774*/
		{
			highlight_index = 0;
		}
		GUILIST_SetCurItemIndex(ctrl_id, highlight_index);
		GUILIST_SetSelectedItem(ctrl_id, highlight_index, TRUE);
		MMK_SetAtvCtrl(win_id, ctrl_id);
		break;
	case MSG_CTL_CANCEL:
	case MSG_APP_CANCEL:
		MMK_CloseWin(win_id);
		break;
		
		
	case MSG_CTL_OK:
	case MSG_CTL_MIDSK:
		highlight_index =  GUILIST_GetCurItemIndex(ctrl_id);
		if(0 == highlight_index)
		{
			//set request mode on
			request_mode = 1;
		}
		else
		{
			//set request mode off
			request_mode = 0;
		}
#ifdef VT_SUPPORT		
#ifndef WIN32
	    //SCI_TRACE_LOW:"[MMIENG VT]  SET RequestMode request_mode  %d"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_5955_112_2_18_2_18_50_42,(uint8*)"d", request_mode);

		MPLH324M_EnableRequestMode(request_mode);
#endif
#endif
		MMK_CloseWin(win_id);
		break;
		
	default:
		result = MMI_RESULT_FALSE;
		break;
	}
	return (result);	
}

/********************************************************************************
 NAME:			HandleAnswerFastWinMsg
 DESCRIPTION:	
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleAdaptLayerWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T ctrl_id = MMIENG_ADAPTERLAYER_LISTBOX_CTRL_ID;
	wchar         *item_str_arr[] = {L"ADAPTER_LAYER_1", L"ADAPTER_LAYER_2", L"ADAPTER_LAYER_3"};
	GUILIST_ITEM_T const *item_ptr = NULL;
#ifdef VT_SUPPORT	
#ifndef WIN32
	MPLH324M_VIDEO_ADAPTER_LAYER_T video_layer = {0};
#endif
#endif	
//	uint16       highlight_index = 0;
	uint16       item_index = 0;
	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:
		
		SetListBoxItems(win_id,
			ctrl_id,
            GUIITEM_STYLE_ONE_LINE_CHECK,
			item_str_arr,
			ARR_SIZE(item_str_arr));
		GUILIST_SetTextListMarkable(ctrl_id, TRUE);
		GUILIST_SetMaxSelectedItem(ctrl_id, ARR_SIZE(item_str_arr));
		GUILIST_SetOwnSofterKey(ctrl_id,TRUE);
#ifdef VT_SUPPORT
#ifndef WIN32
		MPLH324M_GetVideoAdapterLayer(&video_layer);
	    //SCI_TRACE_LOW:"[MMIENG VT]  GET video_layer.video_al_1 %d, video_layer.video_al_2 %d, video_layer.video_al_3 %d"
	    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_6007_112_2_18_2_18_50_43,(uint8*)"ddd", video_layer.video_al_1,video_layer.video_al_2,video_layer.video_al_3);
		item_index = 0;
		if(video_layer.video_al_1)
		{
			GUILIST_SetSelectedItem(ctrl_id,item_index,TRUE);
		}
        item_index ++;
		if(video_layer.video_al_2)
		{
			GUILIST_SetSelectedItem(ctrl_id,item_index,TRUE);
		}
		item_index ++;
		if(video_layer.video_al_3)
		{
			GUILIST_SetSelectedItem(ctrl_id,item_index,TRUE);
		}
#endif	
#endif
		MMK_SetAtvCtrl(win_id, ctrl_id);
			break;
		case MSG_CTL_CANCEL:
		case MSG_APP_CANCEL:
            MMK_CloseWin(win_id);
			break;
	
        case MSG_CTL_MIDSK:
        case MSG_APP_WEB:
			item_index = GUILIST_GetCurItemIndex(ctrl_id);
			item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id,item_index);
			if(GUILIST_GetItemState(item_ptr,GUIITEM_STATE_SELECTED))
			{
				GUILIST_SetSelectedItem(ctrl_id,item_index,FALSE);
			}
			else
			{
				GUILIST_SetSelectedItem(ctrl_id,item_index,TRUE);
			}
			MMK_SendMsg(win_id, MSG_FULL_PAINT , PNULL);
			break;
		case MSG_CTL_OK:
		case MSG_APP_OK:		
#ifdef VT_SUPPORT			
#ifndef WIN32
			item_index = 0;
			item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id,item_index);
			if(GUILIST_GetItemState(item_ptr,GUIITEM_STATE_SELECTED))
			{
				video_layer.video_al_1 = TRUE;
			}
			item_index ++;
			item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id,item_index);
			if(GUILIST_GetItemState(item_ptr,GUIITEM_STATE_SELECTED))
			{
				video_layer.video_al_2 = TRUE;
			}			
			item_index ++;
			item_ptr = GUILIST_GetItemPtrByIndex(ctrl_id,item_index);
			if(GUILIST_GetItemState(item_ptr,GUIITEM_STATE_SELECTED))
			{
				video_layer.video_al_3 = TRUE;
			}	
		
			//SCI_TRACE_LOW:"[MMIENG VT]  SET video_layer.video_al_1 %d, video_layer.video_al_2 %d, video_layer.video_al_3 %d"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_6069_112_2_18_2_18_50_44,(uint8*)"ddd", video_layer.video_al_1,video_layer.video_al_2,video_layer.video_al_3);
				
			MPLH324M_SetVideoAdapterLayer(&video_layer);
#endif
#endif
			MMK_CloseWin(win_id);
			break;
			
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);	
}
/********************************************************************************
 NAME:			AppendAnswerFastSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/

LOCAL void AppendVTVideoSettingSelectItem(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIENG_VTVIDEOSETTING_LISTBOX_CTRL_ID;
    uint16 sel_num = 0;
    MMI_STRING_T kstring[4] = {0};
	GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint32 i = 0;
    MMI_TEXT_ID_T answerfast_text_arr[4] = {TXT_ENG_H263_P, TXT_ENG_MPEG4_P,TXT_ENG_H263_O,TXT_ENG_MPEG4_O};
	
	item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
	item_t.item_data_ptr = &item_data;
	//set softkey
	item_data.softkey_id[0] = TXT_ENG_CLEAR;
	item_data.softkey_id[1] = TXT_NULL;
	item_data.softkey_id[2] = TXT_ENG_RETURN;
	
	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	GUILIST_SetMaxItem(ctrl_id, 4, FALSE );
	for (i = 0; i < 4; i++)
	{
	        //set string context of item
	        MMI_GetLabelTextByLang(answerfast_text_arr[i], &kstring[i]);
			//AppendListItemByTextBuffer(item_data,kstring.wstr_ptr,kstring.wstr_len,FALSE);
			item_data.item_content[0].item_data.text_buffer=kstring[i];	
			GUILIST_AppendItem(ctrl_id, &item_t);
	}
#ifdef VT_SUPPORT
#ifndef WIN32 
    {
        MPLH324M_VIDEO_FORMAT_E video_format = VIDEO_FORMAT_PREFER_H263;

        video_format = MPLH324M_GetVideoFormat();

        switch(video_format) 
        {
        case VIDEO_FORMAT_ONLY_H263:
            sel_num = 2;
        	break;
        case VIDEO_FORMAT_ONLY_MPEG4:
            sel_num = 3;
        	break;
        case VIDEO_FORMAT_PREFER_H263:
            sel_num = 0;
        	break;
        case VIDEO_FORMAT_PREFER_MPEG4:
            sel_num = 1;
        	break;

        default:
            //do nothing
            break;
        }
        
    }
#endif
#endif //vt support
	GUILIST_SetCurItemIndex(ctrl_id, sel_num);
	GUILIST_SetSelectedItem(ctrl_id, sel_num, TRUE);
}

/********************************************************************************
 NAME:			SaveAnswerFastSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL void SaveVTVideoSettingSelectItem(void)
{
    uint16 sel_num = 0;
#ifdef VT_SUPPORT
#ifndef _WIN32
    MPLH324M_VIDEO_FORMAT_E video_format = VIDEO_FORMAT_PREFER_H263;
    
    sel_num = GUILIST_GetCurItemIndex(MMIENG_VTVIDEOSETTING_LISTBOX_CTRL_ID);

    switch(sel_num) 
    {
    case 0:
        video_format = VIDEO_FORMAT_PREFER_H263;
    	break;
    case 1:
        video_format = VIDEO_FORMAT_PREFER_MPEG4;
    	break;
    case 2:
        video_format = VIDEO_FORMAT_ONLY_H263;
    	break;
    case 3:
        video_format = VIDEO_FORMAT_ONLY_MPEG4;
    	break;
    default:
        //do nothing
        break;
    }
    MPLH324M_SetVideoFormat(video_format);
#endif 
#endif //vt_support
    // 提示窗口--已完成
    /*MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);*/
}
/********************************************************************************
 NAME:			SaveAnswerFastSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL void SaveAnswerFastSelectItem(void)
{
    uint16 sel_num = 0;
    
    sel_num = GUILIST_GetCurItemIndex(MMIENG_ANSWERFAST_LISTBOX_CTRL_ID);
#ifdef VT_SUPPORT
		MMIAPIVT_SetIsEPDefault((BOOLEAN)sel_num); 
#endif
    
    // 提示窗口--已完成
   /* MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);*/
}

/********************************************************************************
 NAME:			HandleDSPParamWinMsg
 DESCRIPTION:	
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
extern void L1_SET_DSPPara(uint16 para0,
                            uint16 para1,
                            uint16 para2, 
                            uint16 para3);

/********************************************************************************
 NAME:			MMIAPIENG_GetIsDspParaLogOn
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		jinju.ma
 DATE:			
********************************************************************************/
PUBLIC BOOLEAN MMIENGTD_GetIsDspParaLogOn(void)
{
    return s_is_dsppara_log_on;
}
/********************************************************************************
 NAME:			HandleL1DSPParamSetWinMsg
 DESCRIPTION:	
 AUTHOR:		jinju.ma
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleL1DSPParamSetWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
	MMI_CTRL_ID_T   ctrl_id =    MMIENG_L1DSPPARAM_SET_MENU_CTRL_ID;   
	uint32 group_id = 0;
	uint32 menu_id = 0;

	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
            MMK_SetAtvCtrl(win_id, ctrl_id);
			break;
			
		case MSG_FULL_PAINT:
			break;

		case MSG_KEYDOWN_FLIP:		// close flip
		case MSG_KEYDOWN_RED:
			MMK_CloseWin(win_id);
			break;
		
		case MSG_KEYDOWN_CANCEL:
		case MSG_CTL_CANCEL:
			MMK_CloseWin(win_id);
			break;


		case MSG_CTL_MIDSK:
		case MSG_APP_OK:
		case MSG_CTL_PENOK:
		case MSG_CTL_OK:
		case MSG_APP_WEB:
			GUIMENU_GetId(ctrl_id, &group_id, &menu_id);
			switch (menu_id)
			{
				case ID_ENG_DSPPARAM_LOG:
					MMK_CreateWin((uint32*)MMIENGTD_DSPPARAM_LOG_WIN_TAB,  PNULL);
					break;
					
				case ID_ENG_DSPPARAM:
					MMK_CreateWin((uint32*)MMIENGTD_DSPParam_WIN_TAB, PNULL);
					break;
                
				default:
					break;
			}
			break;

		case MSG_CLOSE_WINDOW:
			break;
		
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);
}
/********************************************************************************
 NAME:			HandleDSPParamLogWinMsg
 DESCRIPTION:	
 AUTHOR:		jinju.ma
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleDSPParamLogWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
    MMI_RESULT_E    result = MMI_RESULT_TRUE;
    MMI_CTRL_ID_T   ctrl_id = MMIENG_DSPPARAM_LOG_LISTBOX_CTRL_ID;
    uint16   cur_selection = 0;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:

		GUILIST_SetMaxItem( ctrl_id, 2, FALSE );
		MMIAPISET_AppendListItemByTextId(TXT_COMMON_OPEN, TXT_COMMON_OK, ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
		MMIAPISET_AppendListItemByTextId(TXT_COMMON_CLOSE, TXT_COMMON_OK ,ctrl_id,GUIITEM_STYLE_ONE_LINE_RADIO);
        
        if (MMIENGTD_GetIsDspParaLogOn())
        {
            cur_selection = 0;
        }
        else
        {
            cur_selection = 1;
        }
        
        GUILIST_SetSelectedItem(ctrl_id, cur_selection, TRUE);
        GUILIST_SetCurItemIndex(ctrl_id, cur_selection);

        MMK_SetAtvCtrl(win_id,ctrl_id);
        break;
    case MSG_FULL_PAINT:
        break;
        
    case MSG_GET_FOCUS:
        
        break;
        
    case MSG_LOSE_FOCUS:
        
        break;

    case MSG_CTL_MIDSK:
    case MSG_APP_OK:
    case MSG_CTL_PENOK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
		cur_selection = GUILIST_GetCurItemIndex( ctrl_id );		

		if(0 == cur_selection)
		{
			 s_is_dsppara_log_on = TRUE;
		}
		else if(1 == cur_selection)
		{
			  s_is_dsppara_log_on = FALSE;
		}
	
        MMK_CloseWin(win_id);
        break;
        
    case MSG_APP_CANCEL:
    case MSG_CTL_CANCEL:
        
        MMK_CloseWin(win_id);
        
        break;
        
    case MSG_CLOSE_WINDOW:
        
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);
}
/********************************************************************************
 NAME:			HandleDSPParamWinMsg
 DESCRIPTION:	
 AUTHOR:		
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleDSPParamWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
        MMI_STRING_T text1 = {0};
        MMI_STRING_T text2 = {0};
        MMI_STRING_T text3 = {0};
        MMI_STRING_T dsp_param_str[4] = {{PNULL, 0},{PNULL, 0},{PNULL, 0},{PNULL, 0}};
        uint16 dsp_param[4] = {0};
        uint32 i = 0;
        MMI_RESULT_E result = MMI_RESULT_TRUE;

        GUI_LCD_DEV_INFO	lcd_dev_info = {0};
         char buf [10]={0};
        lcd_dev_info.lcd_id = GUI_MAIN_LCD_ID;
        lcd_dev_info.block_id = GUI_BLOCK_MAIN;

        switch (msg_id) 
        {
        case MSG_OPEN_WINDOW:
            GUIFORM_SetStyle(MMIENG_DSPPARAM_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIENG_DSPPARAM_FORM2_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIENG_DSPPARAM_FORM3_CTRL_ID,GUIFORM_STYLE_UNIT);
            GUIFORM_SetStyle(MMIENG_DSPPARAM_FORM4_CTRL_ID,GUIFORM_STYLE_UNIT);

	   {
                GUI_BOTH_RECT_T rect = MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id));
	        GUIAPICTRL_SetBothRect(MMK_ConvertIdToHandle(MMIENG_DSPPARAM_FORM_CTRL_ID), &rect);	
            }
  	
            //set param0
            text1.wstr_ptr = L"param0:";
            text1.wstr_len = 7;
            GUILABEL_SetText(MMIENG_DSPPARAM_LABEL1_CTRL_ID, &text1, FALSE);
		GUIEDIT_SetStyle(MMIENG_DSPPARAM_EDITBOX1_CTRL_ID, GUIEDIT_STYLE_SINGLE);

            //set param1
            text2.wstr_ptr = L"param1:";
            text2.wstr_len = 7;
            GUILABEL_SetText(MMIENG_DSPPARAM_LABEL2_CTRL_ID, &text2, FALSE);
            GUIEDIT_SetStyle(MMIENG_DSPPARAM_EDITBOX2_CTRL_ID, GUIEDIT_STYLE_SINGLE);

            //set param2	
            text3.wstr_ptr = L"param2:";
            text3.wstr_len = 7;
            GUILABEL_SetText(MMIENG_DSPPARAM_LABEL3_CTRL_ID, &text3, FALSE);
            GUIEDIT_SetStyle(MMIENG_DSPPARAM_EDITBOX3_CTRL_ID, GUIEDIT_STYLE_SINGLE);
                
              //set param3	
        	text3.wstr_ptr = L"param3:";
        	text3.wstr_len = 7;
        	GUILABEL_SetText(MMIENG_DSPPARAM_LABEL4_CTRL_ID, &text3, FALSE);
        	GUIEDIT_SetStyle(MMIENG_DSPPARAM_EDITBOX4_CTRL_ID, GUIEDIT_STYLE_SINGLE);
            
            //LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
            MMK_SetAtvCtrl(MMIENG_DSPPARAM_WIN_ID, MMIENG_DSPPARAM_EDITBOX1_CTRL_ID);
            // first input the up link
//            s_mmiengtd_up_down_link = 0;
            break;

	   
        case MSG_FULL_PAINT:
	       //   LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
        	break;
        	   
        case MSG_CTL_CANCEL:
        	MMK_CloseWin(MMIENG_DSPPARAM_WIN_ID);
        	break;

      
	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
                GUIEDIT_GetString(MMIENG_DSPPARAM_EDITBOX1_CTRL_ID, &dsp_param_str[0]);
                GUIEDIT_GetString(MMIENG_DSPPARAM_EDITBOX2_CTRL_ID, &dsp_param_str[1]);
                GUIEDIT_GetString(MMIENG_DSPPARAM_EDITBOX3_CTRL_ID, &dsp_param_str[2]);
                GUIEDIT_GetString(MMIENG_DSPPARAM_EDITBOX4_CTRL_ID, &dsp_param_str[3]);
                for(i = 0; i < 4; i++ )
                {
                    SCI_MEMSET(buf,0,10);
                    MMI_WSTRNTOSTR((uint8*)buf, 10, dsp_param_str[i].wstr_ptr, dsp_param_str[i].wstr_len,MIN(10,dsp_param_str[i].wstr_len));
                    dsp_param[i] = atoi(buf);
                    //dsp_param[i] = ConvertString2Digit((uint8*)dsp_param_str[i].wstr_ptr, dsp_param_str[i].wstr_len);
                }
#ifndef _WIN32         
                L1_SET_DSPPara( dsp_param[0],dsp_param[1],dsp_param[2],  dsp_param[3]);/*lint !e628 !e718 !e748 !e746*/
#endif          
                /*MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);   */
                MMK_CloseWin(MMIENG_DSPPARAM_WIN_ID);
                break;
        	
        case MSG_KEYDOWN_UP:
			/*
            switch(s_mmiengtd_up_down_link)
            {
                case 0:
                    //MMK_SetAtvCtrl(MMIENG_DSPPARAM_WIN_ID, MMIENG_DSPPARAM_EDITBOX4_CTRL_ID);
                    s_mmiengtd_up_down_link = 3;
                    break;

                case 1:
                    //MMK_SetAtvCtrl(MMIENG_DSPPARAM_WIN_ID, MMIENG_DSPPARAM_EDITBOX1_CTRL_ID);
                    s_mmiengtd_up_down_link = 0;
                    break;

                case 2:
                    //MMK_SetAtvCtrl(MMIENG_DSPPARAM_WIN_ID, MMIENG_DSPPARAM_EDITBOX2_CTRL_ID);
                    s_mmiengtd_up_down_link = 1;
                    break;

                case 3:
                    //MMK_SetAtvCtrl(MMIENG_DSPPARAM_WIN_ID, MMIENG_DSPPARAM_EDITBOX3_CTRL_ID);
                    s_mmiengtd_up_down_link = 2;
                    break;

                default:
                    break;
            }*/
            break;
        	
        case MSG_KEYDOWN_DOWN:
			/*
            switch(s_mmiengtd_up_down_link)
            {
                case 0:
                    //MMK_SetAtvCtrl(MMIENG_DSPPARAM_WIN_ID, MMIENG_DSPPARAM_EDITBOX2_CTRL_ID);
                    s_mmiengtd_up_down_link = 1;
                    break;

                case 1:
                    //MMK_SetAtvCtrl(MMIENG_DSPPARAM_WIN_ID, MMIENG_DSPPARAM_EDITBOX3_CTRL_ID);
                    s_mmiengtd_up_down_link = 2;
                    break;

                case 2:
                    //MMK_SetAtvCtrl(MMIENG_DSPPARAM_WIN_ID, MMIENG_DSPPARAM_EDITBOX4_CTRL_ID);
                    s_mmiengtd_up_down_link = 3;
                    break;

                case 3:
                   // MMK_SetAtvCtrl(MMIENG_DSPPARAM_WIN_ID, MMIENG_DSPPARAM_EDITBOX1_CTRL_ID);
                    s_mmiengtd_up_down_link = 0;
                    break;

                default:
                    break;
            }*/
			break;
    
        default:
        	result = MMI_RESULT_FALSE;
        	break;
        }
        return (result);


}
//@yongwei.he add just for uplmn test on win
MMI_HPLMN_WACT_T s_hpmln_wact = {0};
#ifdef WIN32 
BOOLEAN mmi_init_hplmn_wact_file(void)
{
    s_hpmln_wact.plmn_num = 2;
    s_hpmln_wact.plmn_info[0].aucPlmn[0] = 0x46;
    s_hpmln_wact.plmn_info[0].aucPlmn[1] = 0x00;
    s_hpmln_wact.plmn_info[0].aucPlmn[2] = 0x09;
    s_hpmln_wact.plmn_info[0].access_type_en = SIM_ACCESS_TYPE_GSM;
    s_hpmln_wact.plmn_info[1].aucPlmn[0] = 0x46;
    s_hpmln_wact.plmn_info[1].aucPlmn[1] = 0x00;
    s_hpmln_wact.plmn_info[1].aucPlmn[2] = 0x1f;
    s_hpmln_wact.plmn_info[1].access_type_en = SIM_ACCESS_TYPE_UTRAN;
    return TRUE;

}
BOOLEAN mmi_read_hplmn_wact_file(MMI_HPLMN_WACT_T *hpmln_wact_ptr)
{
    uint32 i = 0;
    uint32 j = 0;

    hpmln_wact_ptr->plmn_num = s_hpmln_wact.plmn_num;
    for(i = 0; i< hpmln_wact_ptr->plmn_num; i++)
    {
        for(j = 0; j < 3; j++)
            hpmln_wact_ptr->plmn_info[i].aucPlmn[j] = s_hpmln_wact.plmn_info[i].aucPlmn[j];   
        hpmln_wact_ptr->plmn_info[i].access_type_en = s_hpmln_wact.plmn_info[i].access_type_en;
    }
    return TRUE;

}
BOOLEAN mmi_write_hplmn_wact_file(MMI_HPLMN_WACT_T *hpmln_wact_ptr)
{
    uint32 i = 0;
    uint32 j = 0;

    s_hpmln_wact.plmn_num = hpmln_wact_ptr->plmn_num;
    for(i = 0; i< hpmln_wact_ptr->plmn_num; i++)
    {
        for(j = 0; j < 3; j++)
            s_hpmln_wact.plmn_info[i].aucPlmn[j] = hpmln_wact_ptr->plmn_info[i].aucPlmn[j];   
        s_hpmln_wact.plmn_info[i].access_type_en = hpmln_wact_ptr->plmn_info[i].access_type_en;
    }
    return TRUE;

}
#endif

/*****************************************************************************/
// 	Description : 提示窗口消息处理
//	Global resource dependence : 
//  Author:yongwei.he
//	Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleUPLMNPromptWindow(
                                             MMI_WIN_ID_T win_id,
                                             MMI_MESSAGE_ID_E msg_id,
                                             DPARAM param
                                             )
{
        MMI_RESULT_E recode = MMI_RESULT_TRUE;
        MMI_STRING_T uplmn_txt = {0};
        uint8 uplmn_str_arr[500] = {0};
	 uint8 buf[500]={0};
        MMI_HPLMN_WACT_T *hpmln_wact_ptr = &s_hpmln_wact;
        uint8 aucPlmn_str[7] = {0};
        uint8 access_type_str[10] = {0};
        uint32 i = 0;
        uint32 j = 0;
        uint32 uplmn_id_bit_num = 0;

        switch(msg_id)
        {
        case MSG_CLOSE_WINDOW:
#ifndef WIN32
                //SCI_TRACE_LOW("MMI mmi_get_hplmn_wact_file");
                mmi_get_hplmn_wact_file(MN_DUAL_SYS_1, hpmln_wact_ptr);
#endif
                uplmn_txt.wstr_ptr = uplmn_str_arr;/*lint !e64*/
                //SCI_TRACE_LOW("yongweihe HandleUPLMNPromptWindow, hpmln_wact_ptr->plmn_num = %d", hpmln_wact_ptr->plmn_num);
		  for(i = 0; i< hpmln_wact_ptr->plmn_num; i++)
                {
                    s_uplmn_disp_arr[i].access_type = hpmln_wact_ptr->plmn_info[i].access_type_en;
                    for(j = 0; j < 3; j++)
                    {
                        aucPlmn_str[2*j] = (hpmln_wact_ptr->plmn_info[i].aucPlmn[j] >> 4);
                        aucPlmn_str[2*j + 1] = (hpmln_wact_ptr->plmn_info[i].aucPlmn[j] & 0x0f);
                    }

                    if(0x0f == aucPlmn_str[5])
                    {
                        aucPlmn_str[5] = 0;
                        uplmn_id_bit_num = 5;
                     }
                    else
                    {
                        uplmn_id_bit_num = 6;
                     }
                    s_uplmn_disp_arr[i].id = 0;
                    for(j = 0; j < uplmn_id_bit_num; j++)
                    {
                        s_uplmn_disp_arr[i].id = (s_uplmn_disp_arr[i].id  * 10) + aucPlmn_str[j];
                        aucPlmn_str[j] += '0';                        
                    }
                    if(SIM_ACCESS_TYPE_GSM == hpmln_wact_ptr->plmn_info[i].access_type_en)
                    {
                        SCI_MEMCPY(access_type_str,L"GSM", 4);
                    }
                    else if(SIM_ACCESS_TYPE_UTRAN == hpmln_wact_ptr->plmn_info[i].access_type_en)
                    {
                        SCI_MEMCPY(access_type_str,L"UTRAN", 6);
                    }
                    strcat((char *)buf,(char *)aucPlmn_str);
                    strcat((char *)buf,": ");
                    strcat((char *)buf,(char *)access_type_str);
                    strcat((char *)buf,"\n");
                }
		MMI_STRNTOWSTR(uplmn_txt.wstr_ptr,500,  buf, 500, strlen((char*)buf));
        uplmn_txt.wstr_len = MMIAPICOM_Wstrlen((wchar *)uplmn_txt.wstr_ptr);
		//SCI_TRACE_LOW:"HandleUPLMNPromptWindow uplmn_txt.wstr_len =%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_WIN_6498_112_2_18_2_18_51_45,(uint8*)"d",uplmn_txt.wstr_len );
       //SCI_PASSERT(0,("yongweihe HandleUPLMNPromptWindow: uplmn_txt is %d", uplmn_txt.wstr_len));
        GUITEXT_SetString(MMIENG_UPLMN_TEXTBOX_CTRL_ID, uplmn_txt.wstr_ptr,uplmn_txt.wstr_len, FALSE);
        break;
    default:
        recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
        break;
    }
    return recode;
}
//@yongwei.he add end
/********************************************************************************
 NAME:			HandleUPLMNSettingWinMsg
 DESCRIPTION:	
 AUTHOR:		yongwei.he
 DATE:		2007-9-5	
********************************************************************************/
LOCAL MMI_RESULT_E HandleUPLMNSettingWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
         MMI_STRING_T uplmn_txt = {0};
        uint8 uplmn_str_arr[500] = {0};
        MMI_RESULT_E result = MMI_RESULT_TRUE;
        MMI_HPLMN_WACT_T *hpmln_wact_ptr = &s_hpmln_wact;
 //       GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
        uint8 aucPlmn_str[7] = {0};
        uint8 access_type_str[20] = {0};
	uint8 buf[500]={0};
        uint32 i = 0;
        uint32 j = 0;
        uint32 timeout = 5000;

   	switch (msg_id) 
	{
	case MSG_OPEN_WINDOW:
#ifdef WIN32
        mmi_init_hplmn_wact_file();
#endif
            MMK_SetAtvCtrl(win_id, MMIENG_UPLMN_TEXTBOX_CTRL_ID);
         
            GUIWIN_SetSoftkeyTextId(win_id,  (MMI_TEXT_ID_T)TXT_EDIT, (MMI_TEXT_ID_T)TXT_NULL, (MMI_TEXT_ID_T)STXT_RETURN, FALSE);
#ifdef WIN32                
            mmi_read_hplmn_wact_file(hpmln_wact_ptr);       
#else
            //SCI_TRACE_LOW("MMI mmi_read_hplmn_wact_file");
            mmi_read_hplmn_wact_file(MN_DUAL_SYS_1);  
#endif
            //时间比较长,弹出等待窗口
            MMIPUB_OpenAlertWinByTextId(&timeout,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_NONE, HandleUPLMNPromptWindow );      
            break;
	case MSG_GET_FOCUS:  
	     
            uplmn_txt.wstr_ptr = uplmn_str_arr;/*lint !e64*/
            //SCI_TRACE_LOW("yongweihe HandleUPLMNSettingWinMsg, MSG_GET_FOCUS: hpmln_wact_ptr->plmn_num = %d", hpmln_wact_ptr->plmn_num);            
                for(i = 0; i< hpmln_wact_ptr->plmn_num; i++)
                {
                    for(j = 0; j < 3; j++)
                    {
                        aucPlmn_str[2*j] = (hpmln_wact_ptr->plmn_info[i].aucPlmn[j] >> 4) + '0';
                        aucPlmn_str[2*j + 1] = (hpmln_wact_ptr->plmn_info[i].aucPlmn[j] & 0x0f);
                        if(0x0f != aucPlmn_str[2*j + 1])
                             aucPlmn_str[2*j + 1] += '0';
                    }
                    
                    if(SIM_ACCESS_TYPE_GSM == hpmln_wact_ptr->plmn_info[i].access_type_en)
                    {
                        SCI_MEMCPY(access_type_str,L"GSM", 8);
                    }
                    else if(SIM_ACCESS_TYPE_UTRAN == hpmln_wact_ptr->plmn_info[i].access_type_en)
                    {
                        SCI_MEMCPY(access_type_str,L"UTRAN", 12);
                    }

                    MMIAPICOM_Wstrcat((wchar *)buf,(wchar *)aucPlmn_str);/*lint !e64*/
                    MMIAPICOM_Wstrcat((wchar *)buf,": ");/*lint !e64*/
                    MMIAPICOM_Wstrcat((wchar *)buf,(wchar *)access_type_str);
                    MMIAPICOM_Wstrcat((wchar *)buf,"\n");/*lint !e64*/
                }
	MMI_STRNTOWSTR(uplmn_txt.wstr_ptr,500, (char*)buf,500,strlen((char*)buf));/*lint !e64*/
        uplmn_txt.wstr_len = MMIAPICOM_Wstrlen((wchar *)uplmn_txt.wstr_ptr);
        GUITEXT_SetString(MMIENG_UPLMN_TEXTBOX_CTRL_ID, uplmn_txt.wstr_ptr, uplmn_txt.wstr_len, FALSE);
        break;
		
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
		
	case MSG_CTL_OK:
        MMK_CreateWin((uint32*)MMIENG_UPLMN_EDIT_WIN_TAB, PNULL);
		break;
	
	default:
		result = MMI_RESULT_FALSE;
		break;
	}

	return (result);
}
/********************************************************************************
 NAME:			HandleUPLMNEditWinMsg
 DESCRIPTION:	
 AUTHOR:		yongwei.he
 DATE:			
********************************************************************************/
LOCAL MMI_RESULT_E HandleUPLMNEditWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
      MMI_STRING_T uplmn_str = {PNULL, 0};
//    uint8 aucPlmn_str[7] = {0};
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    MMI_HPLMN_WACT_T *hpmln_wact_ptr = &s_hpmln_wact;
    static uint8 s_focus_pos = 0;
//    GUI_LCD_DEV_INFO	lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    uint32 i = 0;
    MMI_CTRL_ID_T label_ctrl[3] = {MMIENG_UPLMN_LABEL1_CTRL_ID, MMIENG_UPLMN_LABEL2_CTRL_ID, MMIENG_UPLMN_LABEL3_CTRL_ID};
    MMI_CTRL_ID_T editbox_ctrl[3]= {MMIENG_UPLMN_EDITBOX1_CTRL_ID, MMIENG_UPLMN_EDITBOX2_CTRL_ID, MMIENG_UPLMN_EDITBOX3_CTRL_ID};
    MMI_STRING_T text_disp = {0};
    uint16 str_arr[3][60] = {L"index(start from 0):", L"id number:", L"type(0:UTRAN 1:GSM):"}; /*lint !e785*/
    uint8 buf[7]={0};
    uint32 index = 0;
    uint16 uplmn_str_arr[7] = {0};
  
    uplmn_str.wstr_ptr = uplmn_str_arr;
    switch (msg_id) 
    {
	case MSG_OPEN_WINDOW:
        GUIFORM_SetStyle(MMIENG_UPLMN_FORM1_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMIENG_UPLMN_FORM2_CTRL_ID,GUIFORM_STYLE_UNIT);
        GUIFORM_SetStyle(MMIENG_UPLMN_FORM3_CTRL_ID,GUIFORM_STYLE_UNIT);

		  {
                GUI_BOTH_RECT_T rect = MMITHEME_GetWinClientBothRect(MMK_ConvertIdToHandle(win_id));

	        	GUIAPICTRL_SetBothRect(MMK_ConvertIdToHandle(MMIENG_UPLMN_FORM_CTRL_ID), &rect);	
            }
            for(i = 0;i < 3;i++)
            {
                text_disp.wstr_ptr = str_arr[i];
                text_disp.wstr_len = (uint16)MMIAPICOM_Wstrlen((wchar *)text_disp.wstr_ptr);
                GUILABEL_SetText(label_ctrl[i], &text_disp, FALSE);
		 GUIEDIT_SetStyle(editbox_ctrl[i], GUIEDIT_STYLE_SINGLE);
            }
            //uplmn_str.str_ptr = (uint8 *)SCI_ALLOC_APP(7);
            //SCI_MEMSET((char *)uplmn_str.str_ptr, 0, 7);
            ConvertDigit2String((uint8 *)buf, &uplmn_str.wstr_len, 1); //default: the second item
            MMI_STRNTOWSTR(uplmn_str.wstr_ptr,7,  buf,7,strlen((char*)buf));/*lint !e64*/
            GUIEDIT_SetString(MMIENG_UPLMN_EDITBOX1_CTRL_ID,(wchar *)uplmn_str.wstr_ptr, uplmn_str.wstr_len);            
            ConvertDigit2String((uint8 *)buf, &uplmn_str.wstr_len, s_uplmn_disp_arr[1].id);
	        MMI_STRNTOWSTR(uplmn_str.wstr_ptr,7,  buf,7,strlen((char*)buf));
            GUIEDIT_SetString(MMIENG_UPLMN_EDITBOX2_CTRL_ID,(wchar *)uplmn_str.wstr_ptr, uplmn_str.wstr_len);
            ConvertDigit2String((uint8 *)buf, &uplmn_str.wstr_len, s_uplmn_disp_arr[1].access_type);
	         MMI_STRNTOWSTR(uplmn_str.wstr_ptr,7,  buf,7,strlen((char*)buf));
            GUIEDIT_SetString(MMIENG_UPLMN_EDITBOX3_CTRL_ID,(wchar *)uplmn_str.wstr_ptr, uplmn_str.wstr_len);            
            //SCI_FREE((uint8 *)uplmn_str.str_ptr);
            MMK_SetAtvCtrl(win_id, MMIENG_UPLMN_EDITBOX1_CTRL_ID);
            s_focus_pos = 0;
            break;
	
		
	case MSG_FULL_PAINT:
            //LCD_FillRect(&lcd_dev_info, rect, mmi_eng_display.bkgd_color);
            ClearENGClientBG();
            break;		
	/*
	case MSG_APP_DOWN:
				if(0 == s_focus_pos)
		{
			s_focus_pos = 1;
			MMK_SetAtvCtrl(win_id, MMIENG_UPLMN_EDITBOX2_CTRL_ID);

		}
		else if (1 == s_focus_pos)
		{
			s_focus_pos = 2;
			MMK_SetAtvCtrl(win_id, MMIENG_UPLMN_EDITBOX3_CTRL_ID);
		}
		else if(2 == s_focus_pos)
		{
			s_focus_pos = 0;
			MMK_SetAtvCtrl(win_id, MMIENG_UPLMN_EDITBOX1_CTRL_ID);
		}
        MMK_PostMsg(win_id, MSG_FULL_PAINT, NULL, 0);
		break;
		*/
	case MSG_APP_CANCEL:
	case MSG_CTL_CANCEL:
		MMK_CloseWin(win_id);
		break;
		

	case MSG_CTL_OK:
       case MSG_CTL_MIDSK:
            //uplmn_str.str_ptr = (uint8 *)SCI_ALLOC_APP(7);
            //SCI_MEMSET((char *)uplmn_str.str_ptr, 0, 7);
            
            //get index
            GUIEDIT_GetString(MMIENG_UPLMN_EDITBOX1_CTRL_ID, &uplmn_str);
	     MMI_WSTRNTOSTR(buf,7, uplmn_str.wstr_ptr,7,uplmn_str.wstr_len);
            index = ConvertString2Digit((uint8 *)buf, uplmn_str.wstr_len);
            
            //get id
            GUIEDIT_GetString(MMIENG_UPLMN_EDITBOX2_CTRL_ID, &uplmn_str);
		MMI_WSTRNTOSTR( buf,7, uplmn_str.wstr_ptr,7,uplmn_str.wstr_len);	
            s_uplmn_disp_arr[index].id = ConvertString2Digit((uint8 *)buf, uplmn_str.wstr_len);
            hpmln_wact_ptr->plmn_info[index].aucPlmn[0] = (buf[0] - '0') * 0x10 + (buf[1] - '0');
            hpmln_wact_ptr->plmn_info[index].aucPlmn[1] = (buf[2] - '0') * 0x10 + (buf[3] - '0');
            if(6 == uplmn_str.wstr_len)
                hpmln_wact_ptr->plmn_info[index].aucPlmn[2] = (buf[4] - '0') * 0x10 + (buf[5] - '0');
            else
                hpmln_wact_ptr->plmn_info[index].aucPlmn[2] = (buf[4] - '0') * 0x10 + 0x0f;  
            
            //get access type
            GUIEDIT_GetString(MMIENG_UPLMN_EDITBOX3_CTRL_ID, &uplmn_str);
	       MMI_WSTRNTOSTR( buf, 7, uplmn_str.wstr_ptr,7,uplmn_str.wstr_len);
            s_uplmn_disp_arr[index].access_type = ConvertString2Digit((uint8 *)buf, uplmn_str.wstr_len);
            hpmln_wact_ptr->plmn_info[index].access_type_en = (sim_access_type_enum)s_uplmn_disp_arr[index].access_type;
            
            //SCI_FREE((uint8 *)uplmn_str.str_ptr);
#ifdef WIN32 
            mmi_write_hplmn_wact_file(hpmln_wact_ptr);                        
#else
            mmi_write_hplmn_wact_file(MN_DUAL_SYS_1, hpmln_wact_ptr);
#endif
            MMK_CloseWin(win_id);
            break;
	
	default:
		result = MMI_RESULT_FALSE;
		break;
    }

    return (result);
}

/********************************************************************************
 NAME:			OpenUPLMNSettingWin FOR MAINMENU USE, only for cmcc
 DESCRIPTION:	
 AUTHOR:		YING.XU
 DATE:			
********************************************************************************/
PUBLIC void MMIENGTD_OpenUPLMNSettingWin(void)
{
    MMK_CreateWin((uint32*)MMIENG_UPLMN_SETTING_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:			ENG_MainMenuWin_HandleMsg
 DESCRIPTION:	
 PARAM IN:		win_id - 
				msg_id - 
				param - 
 PARAM OUT:		
 AUTHOR:		allen
 DATE:			2004.08.18
********************************************************************************/
PUBLIC MMI_RESULT_E ENGTDMainMenuWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	uint32 group_id = 0;
	uint32 menu_id = 0;

	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
			MMK_SetAtvCtrl(MMIENGTD_MAINMENU_WIN_ID, MMIENGTD_MAINMENU_CTRL_ID);
			GUIWIN_SetSoftkeyTextId(win_id,  TXT_COMMON_OK, TXT_NULL, STXT_RETURN, FALSE);
			break;
			
		case MSG_FULL_PAINT:
			break;

#ifdef FLIP_PHONE_SUPPORT
		case MSG_KEYDOWN_FLIP:		// close flip
#endif		
		case MSG_KEYDOWN_RED:
			MMK_CloseWin(MMIENGTD_MAINMENU_WIN_ID);
			break;
		
		case MSG_KEYDOWN_CANCEL:
		case MSG_CTL_CANCEL:
			MMK_CloseWin(win_id);
			break;
        case MSG_CLOSE_WINDOW:
               break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
		case MSG_KEYDOWN_WEB:
		case MSG_CTL_MIDSK:
		case MSG_KEYDOWN_OK:
		case MSG_CTL_OK:
			GUIMENU_GetId(MMIENGTD_MAINMENU_CTRL_ID, &group_id, &menu_id);
            switch (menu_id)
        {
				case ID_ENG_DSP_RESET_DSP1:
#ifndef TD_AS_NOT_PRESENT
					DSP_Assert_Reset(1);
#endif
					break;
				case ID_ENG_DSP_RESET_DSP2:
#ifndef TD_AS_NOT_PRESENT
					DSP_Assert_Reset(2);
#endif
					break;
				case ID_ENG_DSP_RESET_BOTH:
#ifndef TD_AS_NOT_PRESENT
					DSP_Assert_Reset(0);
#endif
					break;

            
        case ID_ENG_SERVICE_TYPE:
#ifdef ENG_SUPPORT
            MMIENG_OpenListSelectWin((uint32)EngServiceTypeWinHandleMsg);
#endif
            break;
            
            /*	bo.chen 20060211 add sms sending path */
        case ID_ENG_SMS_PATH:
#ifdef ENG_SUPPORT
            MMIENG_OpenListSelectWin((uint32)EngSMSPathWinHandleMsg);
#endif
            break;
            
//        case ID_ENG_PS_BITRATE:
//            MMK_CreateWin((uint32*)MMIENG_PS_BITRATE_SEL_WIN_TAB, PNULL);
//            break;					
            
        case ID_ENG_UART0_USEDBY:
            MMIENG_OpenUart0UsedByWin();
            break;
            
        case ID_ENG_UART0_MIXMODE://@zhaohui,cr106076
            MMIENG_OpenUart0MixModeWin();
            break;                    
            
        case ID_ENG_DRXCYCLE:
            MMIENG_OpenDRXCycleWin();
            break;
            
        case ID_ENG_DSPSET:
            MMK_CreateWin((uint32*)MMIENG_DSP_SETTING_WIN_TAB, PNULL);
            break;
            
        case ID_ENG_PICHSETTING:
            MMK_CreateWin((uint32*)MMIENG_PICH_SETTING_WIN_TAB, PNULL);
            break;
            
            /*
            case ID_ENG_ARM_LOG_OUT_PATH:
            MMIENG_OpenArmLogOutPathSelectWin();
            break;
            */
            // @yongwei.he add 2007-04-11
        case ID_ENG_DINA_ENG_MODE:
            MMIEng_Dynamic_main();
            break;
            
        case ID_ENG_VT_RECORD_VIDEO_TYPE:
            MMIENGTD_SetFallbackWin();
            break;
        case ID_ENG_VT_REQUESTMODE:
            MMK_CreateWin((uint32*)MMIENGTD_REQUESTMODE_WIN_TAB, PNULL);
            break;
        case ID_ENG_VT_ADAPTERLAYER:
            MMK_CreateWin((uint32*)MMIENGTD_ADAPTERLAYER_WIN_TAB, PNULL);
            break;
        case ID_ENG_VT_ANSWERFAST:
            MMIENGTD_SetAnswerFastWin();
            break;
        case ID_ENG_VT_VIDEO_TYPE:
            MMIENGTD_SetVTVideoTypeWin();
            break;					
        case ID_ENG_L1_DSP_PARAM:
            MMIENGTD_SetL1DSPParamWin();
            break;
        case ID_ENG_UPLMN_SETTING:
            MMK_CreateWin((uint32*)MMIENG_UPLMN_SETTING_WIN_TAB, PNULL);
            break;                                        
            
        case ID_ENG_APN_SETTING:
            MMK_CreateWin((uint32*)MMIENG_APN_SETTING_WIN_TAB, PNULL);
            break;   
#if 0            
        case ID_ENG_SUPPORT_GSM_SIM://@zhaohui,cr109170
            MMK_CreateWin((uint32*)MMIENG_SUPPORT_GSMSIM_WIN_TAB, PNULL);
            break;

        case ID_ENG_RRM_TEST_ON:
            EnableRRMTest(TRUE);
            break;
        case ID_ENG_RRM_TEST_OFF:
            EnableRRMTest(FALSE);
            break;
 #endif 
 
        default:
            break;
        }
        break;
        
        default:
			result = MMI_RESULT_FALSE;
            break;
    }
	return (result);
}
//@zhaohui,cr109170 start
/********************************************************************************
 NAME:			HandleSuppertGsmSimWinMsg
 DESCRIPTION:	
 AUTHOR:		zhaohui
 DATE:			
********************************************************************************/
#if 0
LOCAL MMI_RESULT_E HandleSupportGsmSimWinMsg(
                                            MMI_WIN_ID_T win_id,
                                            MMI_MESSAGE_ID_E msg_id,
                                            DPARAM param
                                            )
{
	MMI_RESULT_E result = MMI_RESULT_TRUE;
	
	switch (msg_id) 
	{
		case MSG_OPEN_WINDOW:
            		AppendSupportGsmSimSelectItem();
			MMK_SetAtvCtrl(win_id, MMIENG_SUPPORT_GSMSIM_LISTBOX_CTRL_ID);
			break;
			
		case MSG_FULL_PAINT:
			break;
			
		case MSG_KEYDOWN_OK:
		case MSG_KEYDOWN_CANCEL:
			MMK_CloseWin(win_id);
			break;
			
		case MSG_KEYDOWN_FLIP:		// close flip
		case MSG_KEYDOWN_RED:
			break;
			
		case MSG_CTL_CANCEL:
            		MMK_CloseWin(win_id);
			break;
	
		case MSG_CTL_OK:
            		SaveSupportGsmSimSelectItem();
            		MMK_CloseWin(win_id);
			break;
	
		case MSG_LOSE_FOCUS:
			break;
			
		case MSG_GET_FOCUS:
			break;
			
		default:
			result = MMI_RESULT_FALSE;
			break;
	}
	return (result);	
}   


/********************************************************************************
 NAME:			AppendSupportGsmSimSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhaohui
 DATE:			
********************************************************************************/
LOCAL void AppendSupportGsmSimSelectItem(void)
{
    MMI_CTRL_ID_T ctrl_id = MMIENG_SUPPORT_GSMSIM_LISTBOX_CTRL_ID;
    uint16 sel_num = 0;
    MMI_STRING_T kstring[2];
    GUILIST_ITEM_T      item_t = {0};/*lint !e64*/
	GUILIST_ITEM_DATA_T item_data = {0};/*lint !e64*/
    uint32 i = 0;
    MMI_TEXT_ID_T supportgsmsim_text_arr[2] = {TXT_ENG_ENABLE, TXT_ENG_DISABLE};
    BOOLEAN is_support_gsmsim = FALSE;    
	
    item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
	item_t.item_data_ptr = &item_data;
	//set softkey
	item_data.softkey_id[0] = TXT_ENG_CLEAR;
	item_data.softkey_id[1] = TXT_NULL;
	item_data.softkey_id[2] = TXT_ENG_RETURN;
	
	item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_BUFFER;
	GUILIST_SetMaxItem(ctrl_id, 2, FALSE);
	for (i = 0; i < 2; i++)
	{
	    //set string context of item
	    MMI_GetLabelTextByLang(supportgsmsim_text_arr[i], &kstring[i]);
      //  AppendListItemByTextBuffer(item_data,kstring.wstr_ptr,kstring.wstr_len,FALSE);
      item_data.item_content[0].item_data.text_buffer=kstring[i];	
		GUILIST_AppendItem(ctrl_id, &item_t);
	}
	
#ifndef _WIN32
	is_support_gsmsim = (uint16)MN_GetSupportGsmSimStat(); 
#else
		is_support_gsmsim = s_support_gsmsim_stat;
#endif     

    if (is_support_gsmsim)
    {
        sel_num = 0;
    }
    else
    {
        sel_num = 1;
    }
	GUILIST_SetCurItemIndex(ctrl_id, sel_num);
	GUILIST_SetSelectedItem(ctrl_id, sel_num, TRUE);
}

/********************************************************************************
 NAME:			SaveSupportGsmSimSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhaohui
 DATE:			
********************************************************************************/
LOCAL void SaveSupportGsmSimSelectItem(void)
{
    uint16 sel_num = 0;
    BOOLEAN is_support_gsmsim = FALSE;
    
    sel_num = GUILIST_GetCurItemIndex(MMIENG_SUPPORT_GSMSIM_LISTBOX_CTRL_ID);
    if (!sel_num)
    {
        is_support_gsmsim = 1;
    }
    else
    {
        is_support_gsmsim = 0;
    }
    
#ifndef _WIN32
		MN_SetSupportGsmSimStat(is_support_gsmsim); 
#else
		s_support_gsmsim_stat = is_support_gsmsim;
#endif
    
    // 提示窗口--已完成
    MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
}
//@zhaohui,cr109170 end
#endif

/********************************************************************************
 NAME:			SaveSupportGsmSimSelectItem
 DESCRIPTION:	
 PARAM IN:		
 PARAM OUT:		
 AUTHOR:		zhaohui
 DATE:			
********************************************************************************/
#if 0
LOCAL void EnableRRMTest(BOOLEAN enable)
{
   /* +CR200414 */
   //ITEM_T fta_item={0};
   /* -CR200414 */
   ITEM_T nw_item={0};
   uint32 condition=0;
   BOOLEAN is_on=FALSE;
  
   
   if(enable)
   {
       /* +CR200414 */
       //fta_item.arr_value[0]=1;
       /* -CR200414 */
	nw_item.arr_value[0]=0;
	condition=1;
	is_on=FALSE;
#ifdef DCD_SUPPORT
	MMIAPIDCD_SetDCDEnable(FALSE);
#endif
#ifdef DM_SUPPORT
	MMIAPIDM_SetReg(FALSE);
#endif	   
   }
   else
   {
       /* +CR200414 */
       //fta_item.arr_value[0]=0;
       /* -CR200414 */
	nw_item.arr_value[0]=1;
	condition=0;
	is_on=TRUE;
#ifdef DCD_SUPPORT
	MMIAPIDCD_SetDCDEnable(TRUE);
#endif
#ifdef DM_SUPPORT
	MMIAPIDM_SetReg(TRUE);
#endif
   }
    /* +CR200414 */
    //fta_item.arr_count=1;
    /* -CR200414 */
    nw_item.arr_count=1;
#ifndef PB_SUPPORT_LOW_MEMORY
    MMINV_WRITE(MMIENG_NV_CALLFORWARD_QUERY_ID, &is_on);
 #endif
    

#ifndef _WIN32
    /* +CR200414 */
    //em_set_fta_opt_enable_flag(&fta_item, 0);
    /* -CR200414 */

    em_set_real_nw_opt_enable_flag(&nw_item, 0);

    em_set_wakeup_condition(condition);
#endif

    MMIPUB_OpenAlertSuccessWin(TXT_COMPLETE);
    
}
#endif

LOCAL MMI_RESULT_E EngServiceTypeWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    int32 index = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    int32 list_id = MMIENG_LISTBOX_CTRL_ID ;
    int32 flag = 0; // 0:OFF; 1: ON    
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};   
            GUIWIN_SetTitleTextId(win_id,  TXT_ENG_SERVICE_TYPE,  FALSE);
            GUILIST_SetMaxItem( list_id, 3, FALSE );
            GUILIST_SetOwnSofterKey(list_id,FALSE);
            
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
            item_t.item_data_ptr = &item_data;
            
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;                 
        
            item_data.item_content[0].item_data.text_id = TXT_ENG_GSM_ONLY;    
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_ENG_GPRS_ONLY;    
            GUILIST_AppendItem( list_id, &item_t );                      
            item_data.item_content[0].item_data.text_id = TXT_ENG_GSM_GPRS;    
            GUILIST_AppendItem( list_id, &item_t );        
            
#ifdef MMI_GPRS_SUPPORT
            flag = MNPHONE_GetServiceTypeEx(MN_DUAL_SYS_1);
#endif
            
            GUILIST_SetSelectedItem( list_id,flag , TRUE );
            GUILIST_SetCurItemIndex( list_id, flag);          
            MMK_SetAtvCtrl(win_id, list_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        break;      
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);              
        if(0 <= index && 2>=  index )
        {  
            MNPHONE_SetServiceTypeEx(MN_DUAL_SYS_1,index + 1);
        }   
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);        
}

LOCAL MMI_RESULT_E EngSMSPathWinHandleMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    int32 index = 0;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    int32 list_id = MMIENG_LISTBOX_CTRL_ID ;
    int32 flag = 0; // 0:OFF; 1: ON    
    
    switch (msg_id) 
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_ITEM_T      item_t = {0};
            GUILIST_ITEM_DATA_T item_data = {0};
            
           GUIWIN_SetTitleTextId(win_id, TXT_ENG_SMS_PATH, FALSE);
            
            GUILIST_SetMaxItem( list_id, 4, FALSE );
            GUILIST_SetOwnSofterKey(list_id,FALSE);
            
            item_t.item_style    = GUIITEM_STYLE_ONE_LINE_RADIO;
            item_t.item_data_ptr = &item_data;
            
            item_data.item_content[0].item_data_type    = GUIITEM_DATA_TEXT_ID;    
            
            item_data.item_content[0].item_data.text_id = TXT_ENG_SMS_PS;    
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_ENG_SMS_CS;    
            GUILIST_AppendItem( list_id, &item_t );                      
            item_data.item_content[0].item_data.text_id = TXT_ENG_SMS_PS_PREFER;    
            GUILIST_AppendItem( list_id, &item_t );
            item_data.item_content[0].item_data.text_id = TXT_ENG_SMS_CS_PREFER;    
            GUILIST_AppendItem( list_id, &item_t );              
#ifndef WIN32           
            flag = MNSMS_GetServiceForMOSms();
#endif            
            GUILIST_SetSelectedItem( list_id,flag, TRUE );
            GUILIST_SetCurItemIndex( list_id, flag);          
            MMK_SetAtvCtrl(win_id, list_id);
        }
        break;
        
    case MSG_FULL_PAINT:
        break;      
        
    case MSG_KEYDOWN_CANCEL:
    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;
        
    case MSG_CTL_MIDSK:
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_CTL_PENOK:
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_KEYDOWN_OK:
    case MSG_CTL_OK:
    case MSG_APP_WEB:
        index = GUILIST_GetCurItemIndex(list_id);              
        if(0 <= index && 3>= index)
        {  
#ifndef WIN32               
            MNSMS_SetServiceForMOSms(index);
#endif            
        }     
        MMK_CloseWin(win_id);
        break;
        
    default:
        result = MMI_RESULT_FALSE;
        break;
    }
    return (result);        
}

#endif// ENGTD_SUPPORT


/*Edit by script, ignore 3 case. Fri Apr 27 09:38:50 2012*/ //IGNORE9527
