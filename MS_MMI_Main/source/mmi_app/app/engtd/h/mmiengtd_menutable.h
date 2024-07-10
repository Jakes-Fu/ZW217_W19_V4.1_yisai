/****************************************************************************
** File Name:      mmiengtd_menutable.h	                                    *
** Author:         allen				                                    *
** Date:           2004.09.09												*
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved. 		*
** Description:    									               			*
*****************************************************************************
**                         Important Edit History                        							*
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             						*
** 09/2004        allen            Create
** 
****************************************************************************/
#ifdef ENGTD_SUPPORT
#ifndef _MMIENGTD_MENUTABLE_H_
#define _MMIENGTD_MENUTABLE_H_

#include "mmi_module.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*-------------------------------------------------------------------------*/
/*                         TYPES/CONSTANTS                                 */
/*-------------------------------------------------------------------------*/

#define TIP_NULL 0

//²Ëµ¥ID
typedef enum
{
    MMIENGTD_MENU_ID_MIN = (MMI_MODULE_ENGTD << 16),


    ID_ENG_CHIP_ID,
    ID_ENG_VTSET,
    ID_ENG_VT_AUDIO,
    ID_ENG_VT_VIDEO,
    ID_ENG_NETINFOLYONE,
    ID_ENG_NETINFOLYTWO,
    ID_ENG_NETINFOLYTHREE,
    ID_ENG_NETINFONAS,
    ID_ENG_LYONETIMESLOT,
    ID_ENG_LYONEDPCH,
    ID_ENG_LYONEBLER,
    ID_ENG_LYONEOTHER,
    ID_ENG_LYTHREESCELL,
    ID_ENG_LYTHREENCELL, 
    ID_ENG_NASMM, 
    ID_ENG_NASGMM,
    ID_ENG_DSP_RESET,
    ID_ENG_DSP_RESET_DSP1,
    ID_ENG_DSP_RESET_DSP2,
    ID_ENG_DSP_RESET_BOTH,	
    ID_ENG_SERVICE_TYPE,
    ID_ENG_GPRS_PROC,
    ID_ENG_PS_BITRATE,
    ID_ENG_CS64_FLAG, 
    ID_ENG_SMS_PATH,   
    ID_ENG_SERVICE_GSM,
    ID_ENG_SERVICE_GPRS,
    ID_ENG_SERVICE_GSM_GPRS,
    ID_ENG_GPRS_PROC_ATTACH,
    ID_ENG_GPRS_PROC_DETACH,
    ID_ENG_PS_BITRATE_8,
    ID_ENG_PS_BITRATE_16,
    ID_ENG_PS_BITRATE_32,
    ID_ENG_PS_BITRATE_64,
    ID_ENG_PS_BITRATE_128,
    ID_ENG_PS_BITRATE_384,
    ID_ENG_CS64_ON,
    ID_ENG_CS64_OFF,
    ID_ENG_SMS_CS,
    ID_ENG_SMS_PS,
    ID_ENG_SMS_CS_PREFER,
    ID_ENG_SMS_PS_PERFER,
    ID_ENG_PSCTRL_FLAG, 
    ID_ENG_PS_CTRL_ON,
    ID_ENG_PS_CTRL_OFF,
    ID_ENG_NETWORK_SETTING,
    ID_ENG_FREQ,
    ID_ENG_IMSI,
    ID_ENG_SIMSIMULATE,
    ID_ENG_SIMSIMULATE_ON,
    ID_ENG_SIMSIMULATE_OFF,
    ID_ENG_UART0_USEDBY,	
    ID_ENG_UART0_MIXMODE,//@zhaohui,cr106076    
    ID_ENG_UART1_USEDBY,
    ID_ENG_UART1_USED_ARMREF,
    ID_ENG_UART1_USED_DSP1,
    ID_ENG_UART1_USED_CYPS,	
    ID_ENG_DEEP_SLEEP,
    ID_ENG_DEEP_SLEEP_ON,
    ID_ENG_DEEP_SLEEP_OFF,
    ID_ENG_RF_LOOP,
    ID_ENG_RF_LOOP_ON,
    ID_ENG_RF_LOOP_OFF,
    ID_ENG_FDN_FLAG, 	
    ID_ENG_FDN_ENABLE,
    ID_ENG_FDN_DISABLE,
    ID_ENG_BDN_FLAG, 	
    ID_ENG_BDN_ENABLE,
    ID_ENG_BDN_DISABLE,
    ID_ENG_APN_SETTING,
    ID_ENG_NETWORK_SETTING_INFO,
    ID_ENG_ASN_SELECT,
    ID_ENG_DRXCYCLE,
    ID_ENG_ARM_LOG_OUT_PATH,
    ID_ENG_DSPSET,
    ID_ENG_UARTBAUDRATE,
    ID_ENG_UART0BAUDRATE,
    ID_ENG_UART1BAUDRATE,
    ID_ENG_PICHSETTING,
    ID_ENG_LOCKCELL,
    ID_ENG_ROAMPARAM, 
    ID_ENG_DINA_ENG_MODE,
    ID_ENG_VT_RECORD_VIDEO_TYPE,
    ID_ENG_VT_ANSWERFAST,
	ID_ENG_VT_REQUESTMODE,
	ID_ENG_VT_ADAPTERLAYER,
    ID_ENG_VT_VIDEO_TYPE,
    ID_ENG_L1_DSP_PARAM,
    ID_ENG_UPLMN_SETTING,
    ID_ENG_SUPPORT_GSM_SIM,//@zhaohui,cr109170       
    ID_ENG_RRM_TEST,
    ID_ENG_RRM_TEST_ON,
    ID_ENG_RRM_TEST_OFF,
    ID_ENG_DSPPARAM_LOG,
    ID_ENG_DSPPARAM,
    MMIENG_MENU_ID_MAX
}MMIENG_MENU_ID_E;

#define MENU_DEF(_TABLEID, _ITEMPTR, _STYLE, _TITLESTR, _TITLE_ICON, _TITLE_NUMICON, _TITLE_BACKGROUND, \
	_ITEMCOUNT) \
     _TABLEID,

typedef enum
{
    MMIENG_MENU_TABEL_MIN = (MMI_MODULE_ENGTD << 16),
    #include "mmiengtd_menutable.def"
    MMIENG_MENU_TABLE_MAX
} MMIENG_MENU_TABEL_E;

#undef MENU_DEF

/*****************************************************************************/
// 	Description : Register eng menu group
//	Global resource dependence : none
//  Author: songbin.zeng
//	Note:
/*****************************************************************************/
PUBLIC void MMIENGTD_RegMenuGroup(void);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
/*-------------------------------------------------------------------------*/

#endif /* _MMIENGTD_MENUTABLE_H_ */

#endif// ENGTD_SUPPORT