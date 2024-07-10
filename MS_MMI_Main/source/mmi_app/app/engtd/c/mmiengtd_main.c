/*****************************************************************************
** File Name:      mmieng_main.c                                                  *
** Author:                                                                   *
** Date:           08/2004                                                   *
** Copyright:      2003 Spreadtrum, Incorporated. All Rights Reserved.         *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 8/2004         Allen

******************************************************************************/
#define _MMIENG_MAIN_C_

#include "mmi_app_engtd_trc.h"
#ifdef ENGTD_SUPPORT
/**--------------------------------------------------------------------------*
		                        Include Files      
 **--------------------------------------------------------------------------*/


#include "mmk_app.h"
//#include "mmieng.h"
#include "mmieng_export.h"
#include "mmieng_win.h"
//#include "mmieng_uitestwin.h"
#include "window_parse.h"
#include "mmi_image.h"
#include "mmi_text.h"
#include "guilcd.h"
#include "guicommon.h"
#include "guitext.h"
#ifndef _WIN32
//#include "ref_param.h"
#include "deep_sleep.h"
#include "sio.h"
//#include "Layer1_engineering.h"
#include "sci_log.h"
#endif
//#include "sci_log.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_signal.h"
#include "mmi_signal_ext.h"
#include "ref_engineering.h"
#include "tb_hal.h"//@cr36254 jian.ma
//#include "mmiengtd_nv.h"
#include "mmiengtd_id.h"
//#include "mmiengtd_menutable.h"
//#include "mmiengtd_main.h"
#include "mmi_nv.h"
#include "mmi_appmsg.h"
#include "mmiengtd_internal.h"
/**--------------------------------------------------------------------------*
			                         MACRO DEFINITION
 **--------------------------------------------------------------------------*/

#ifdef WIN32
#define DSP_Assert_Reset(x)
#endif

/**--------------------------------------------------------------------------*
			                         STATIC DEFINITION
 **--------------------------------------------------------------------------*/

#ifdef WIN32

ERR_MNPHONE_CODE_E MNPHONE_SelectMOSMSService(                   // return whether success or failure
			 MNSMS_SERVICE_E sel_service      		// the sc addrress struct
			 )
{
    return 0;/*lint !e64*/
}
PUBLIC void MNGPRS_SetExtendQos(void){};
PUBLIC void DMNV_SetSimSimulate(void){};
PUBLIC void DMNV_SetUart1Used(void){};
PUBLIC void DMNV_SetDeepSleep(void){};
PUBLIC void DMNV_SetRfLoop(void){};
PUBLIC void DMNV_SetDRXCycleLen(void){};
PUBLIC void DMNV_GetDRXCycleLen(void){};
PUBLIC void REFPARAM_GetLogOutPath(void){};
PUBLIC void REFPARAM_SetLogOutPath(void){};
PUBLIC void TD_MNEM_SetCommandTypeAndParam(void){};
PUBLIC void DMNV_SetParamID(void){};
PUBLIC void DMNV_SetIMSI(void){};
PUBLIC void DMNV_SetFreqList(void){};

uint32 v_glo_pdpDataFlag;   /*lint !e552 */
//uint32 v_tgl_csDataFlag;    /*lint !e552 */
//extern uint32 g_cell_Para_Id;

#else
uint32 v_glo_pdpDataFlag;   /*lint !e552 */
//uint32 v_tgl_csDataFlag;    /*lint !e552 */
//extern uint32 g_cell_Para_Id;

#endif
/**--------------------------------------------------------------------------*
			                         TYPE AND CONSTANT
 **--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
			                         LOCAL FUNCTION DECLARE
 **--------------------------------------------------------------------------*/


//LOCAL void SetPSDataCtrlFlag(BOOLEAN b_flag);

//@xiefen add begin 
//LOCAL void SetServiceType(MN_PHONE_USER_SERVICE_SELECT_E service_type);

/********************************************************************************
 NAME:			SetServiceType
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		bo.chen
 DATE:			20060211
********************************************************************************/
//LOCAL void MMIENGTD_SetSMSSendPath(uint32 sms_service);

//LOCAL void MMIENGTD_SetGPRSAttach(void);

//LOCAL void MMIENGTD_SetGPRSDetach(void);

//LOCAL void MMIENGTD_SetPsBitRate(uint16 kBitRate);

//LOCAL void MMIENGTD_SetCS64Flag(BOOLEAN b_flag);

//LOCAL void MMIENGTD_SetSimSimulate(BOOLEAN isOn);

//LOCAL void MMIENGTD_SetUart1UsedFlag(uint8 uart1flag);

//LOCAL void MMIENGTD_SetDeepSleep(BOOLEAN isOn);

//LOCAL void MMIENGTD_SetRfLoop(BOOLEAN isOn);

//LOCAL void MMIENGTD_SetFDNFlag(BOOLEAN isOn);

//LOCAL void MMIENGTD_SetBDNFlag(BOOLEAN isOn);
//@xiefen add end 

/**--------------------------------------------------------------------------*
			                         GLOBAL DEFINITION
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
			                         EXTERNAL DECLARE
 **--------------------------------------------------------------------------*/


//extern void DSP_Assert_Reset(int16 dsp_id);


/*===============================================================================
								IMPLEMENTATION
===============================================================================*/

/********************************************************************************
 NAME:			SetServiceType
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		bo.chen
 DATE:			20060211
********************************************************************************/
PUBLIC void MMIAPIENGTD_SetSMSSendPath(uint32 sms_service)
{
	MMI_STRING_T text1 = {0}; 
	MMI_STRING_T text2 = {0}; 
	MMI_STRING_T text3 = {0}; 
	
	text1.wstr_ptr = L"Set SMS Service Type";
	text1.wstr_len = 16;
	text2.wstr_ptr = L"to";
	text2.wstr_len = 2;

	switch(sms_service)
	{
		case 1:
			text3.wstr_ptr = L"CS Only";
			text3.wstr_len = 7;
			break;
			
		case 0:
			text3.wstr_ptr = L"PS Only";
			text3.wstr_len = 7;			
			break;
			
		case 3:
			text3.wstr_ptr = L"CS preferred";
			text3.wstr_len = 12;			
			break;
			
		case 2:
			text3.wstr_ptr = L"PS preferred";
			text3.wstr_len = 12;			
			break;
			
		default:
			return;
			
	}
/*	+cr90072		*/
//@yongwei.he
	//MNPHONE_SelectMOSMSService(sms_service);
#ifndef WIN32
	MNSMS_SetServiceForMOSms(sms_service);
#endif
/*	-cr90072		*/	
	MMIEng_SetResultWinText(text1, text2, text3);
	MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);	
}

//@xiefen add begin 
/********************************************************************************
 NAME:			SetServiceType
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		Bill Dong
 DATE:			2005.05.08
********************************************************************************/
PUBLIC void MMIAPIENGTD_SetServiceType(MN_PHONE_USER_SERVICE_SELECT_E service_type)
{
	MMI_STRING_T text1 = {0}; 
	MMI_STRING_T text2 = {0}; 
	MMI_STRING_T text3 = {0}; 
	
	text1.wstr_ptr = L"Set Service Type";
	text1.wstr_len= 16;
	text2.wstr_ptr = L"to";
	text2.wstr_len = 2;

	switch(service_type)
	{
		case MN_PHONE_USER_SELECT_GSM_ONLY:
			text3.wstr_ptr = L"GSM Only";
			text3.wstr_len = 8;
			break;

		case MN_PHONE_USER_SELECT_GPRS_ONLY:
			text3.wstr_ptr = L"GPRS Only";
			text3.wstr_len = 9;			
			break;

		case MN_PHONE_USER_SELECT_GSM_GPRS_BOTH:
			text3.wstr_ptr = L"GSM and GPRS";
			text3.wstr_len = 12;			
			break;
		default:
			return;
	
	}
	
	#ifndef _WIN32
	MNPHONE_SetServiceTypeEx(MN_DUAL_SYS_1, service_type);//%#discuss
	#endif
	MMIEng_SetResultWinText(text1, text2, text3);
	MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);	
}
/********************************************************************************
 NAME:			SetGPRSAttach
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		Bill Dong
 DATE:			2005.05.08
********************************************************************************/
PUBLIC void MMIAPIENGTD_SetGPRSAttach(void)
{
	MMI_STRING_T text1 = {0}; 
	MMI_STRING_T text2 = {0}; 
	MMI_STRING_T text3 = {0}; 
	
	text1.wstr_ptr = L"Set GPRS";
	text1.wstr_len = 8;
	text2.wstr_ptr = L"to";
	text2.wstr_len = 2;
	text3.wstr_ptr = L"Attach";
	text3.wstr_len = 6;
	#ifndef _WIN32
	MNPHONE_GprsAttachEx(MN_DUAL_SYS_1, MN_PHONE_GPRS_ATTACH);//%#discuss
	#endif
	MMIEng_SetResultWinText(text1, text2, text3);
	MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);
}

/********************************************************************************
 NAME:			SetGPRSDetach
 DESCRIPTION:	
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		Bill Dong
 DATE:			2005.05.08
********************************************************************************/
PUBLIC void MMIAPIENGTD_SetGPRSDetach(void)
{
	MMI_STRING_T text1 = {0}; 
	MMI_STRING_T text2 = {0}; 
	MMI_STRING_T text3 = {0}; 
	
	text1.wstr_ptr = L"Set GPRS";
	text1.wstr_len = 8;
	text2.wstr_ptr = L"to";
	text2.wstr_len = 2;
	text3.wstr_ptr = L"Detach";
	text3.wstr_len = 6;
	#ifndef _WIN32
	MNPHONE_GprsDetachEx(MN_DUAL_SYS_1,MN_PHONE_GPRS_DETACH);
	#endif
	MMIEng_SetResultWinText(text1, text2, text3);
	MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);
}
#if 0
/********************************************************************************
 NAME:			SetPsBitRate
 DESCRIPTION:	Set Protocol Stack packet service bit rate.
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		qingfeng.dong
 DATE:			2005.09.06
********************************************************************************/
PUBLIC void MMIAPIENGTD_SetPsBitRate(uint16 kUpBitRate,uint16 kDownBitRate)//@zhaohui modify,cr108979
{
    MN_GPRS_TOTAL_QOS_T ext_qos={0};
    //MMI_STRING_T text1 = {0, FALSE, PNULL}; 
    //MMI_STRING_T text2 = {0, FALSE, PNULL}; 
    //MMI_STRING_T text3 = {0, FALSE, PNULL}; 

    /*+CR105931*/
    //BOOLEAN item_setted[13]={0,0,0,0,0,1,1,1,1,0,0,0,0};
    /*-CR105931*/
    
	//SCI_TRACE_LOW:"[mmiengtd_main.c] kUpBitRate is %d,kDownBitRate is %d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIENGTD_MAIN_320_112_2_18_2_18_35_12,(uint8*)"dd",kUpBitRate,kDownBitRate);
    if(0==kDownBitRate)
    {
       ext_qos.guaranteed_bit_rate_for_downlink = MN_GPRS_GUARANTD_BIT_RATE_UPLINK7;/*lint !e64*/
	ext_qos.max_bit_rate_for_downlink=MN_GPRS_GUARANTD_BIT_RATE_UPLINK7;/*lint !e64*/
    }
    else if(kDownBitRate<=63)
    {
        ext_qos.guaranteed_bit_rate_for_downlink = kDownBitRate;/*lint !e64*/
	 ext_qos.max_bit_rate_for_downlink=kDownBitRate;/*lint !e64*/
    }
    else if(kDownBitRate<=576)
    {
        ext_qos.guaranteed_bit_rate_for_downlink=(MN_GPRS_GUARANTD_BIT_RATE_UPLINK3+(kDownBitRate-MN_GPRS_GUARANTD_BIT_RATE_UPLINK3)/8);/*lint !e64*/
	 ext_qos.max_bit_rate_for_downlink=(MN_GPRS_GUARANTD_BIT_RATE_UPLINK3+(kDownBitRate-MN_GPRS_GUARANTD_BIT_RATE_UPLINK3)/8);/*lint !e64*/
    }
    else if(kDownBitRate<=8640)
    {
        ext_qos.guaranteed_bit_rate_for_downlink=(MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK5+(kDownBitRate-MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK5)/8);/*lint !e64*/
	 ext_qos.max_bit_rate_for_downlink=(MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK5+(kDownBitRate-MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK5)/8);/*lint !e64*/
    }
    else
    {
         ext_qos.guaranteed_bit_rate_for_downlink=MN_GPRS_NW2MS_GUARANTD_BIT_RATE_DOWNLINK_RESERVED;
	  ext_qos.max_bit_rate_for_downlink=MN_GPRS_NW2MS_GUARANTD_BIT_RATE_DOWNLINK_RESERVED;/*lint !e64*/
      
    }
    if(0==kUpBitRate)
    {
       ext_qos.guaranteed_bit_rate_for_uplink = MN_GPRS_GUARANTD_BIT_RATE_UPLINK7;
	ext_qos.max_bit_rate_for_uplink=MN_GPRS_GUARANTD_BIT_RATE_UPLINK7;/*lint !e64*/
    }
    else if(kUpBitRate<=63)
    {
        ext_qos.guaranteed_bit_rate_for_uplink = kDownBitRate;/*lint !e64*/
	 ext_qos.max_bit_rate_for_uplink=kDownBitRate;/*lint !e64*/
    }
    else if(kUpBitRate<=576)
    {
        ext_qos.guaranteed_bit_rate_for_uplink=(MN_GPRS_GUARANTD_BIT_RATE_UPLINK3+(kDownBitRate-MN_GPRS_GUARANTD_BIT_RATE_UPLINK3)/8);/*lint !e64*/
	 ext_qos.max_bit_rate_for_uplink=(MN_GPRS_GUARANTD_BIT_RATE_UPLINK3+(kDownBitRate-MN_GPRS_GUARANTD_BIT_RATE_UPLINK3)/8);/*lint !e64*/
    }
    else if(kUpBitRate<=8640)
    {
        ext_qos.guaranteed_bit_rate_for_uplink=(MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK5+(kDownBitRate-MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK5)/8);/*lint !e64*/
	 ext_qos.max_bit_rate_for_uplink=(MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK5+(kDownBitRate-MN_GPRS_GUARANTD_BIT_RATE_DOWNLINK5)/8);/*lint !e64*/
    }
    else
    {
         ext_qos.guaranteed_bit_rate_for_uplink=MN_GPRS_NW2MS_GUARANTD_BIT_RATE_DOWNLINK_RESERVED;/*lint !e64*/
	  ext_qos.max_bit_rate_for_uplink=MN_GPRS_NW2MS_GUARANTD_BIT_RATE_DOWNLINK_RESERVED;/*lint !e64*/
      
    }	
  
   // ext_qos.pdp_id = 1;
    ext_qos.traffic_class = MN_GPRS_TRAFFIC_CLASS_INTERACTIVE;

    //	
    //text1.str_ptr = (uint8*)"Set Ps(UL/DL)";
    //text1.length = 13;
    //text2.str_ptr = (uint8*)"to";
    //text2.length = 2;
/*    switch(kBitRate)
    {
        case 64:
            //text3.str_ptr = (uint8*)"64k/64k";
            //text3.length = 7;
		
		    ext_qos.guaranteed_bt_ul = 64;
		    ext_qos.max_bt_ul = 64;
            break;
        case 128:
            //text3.str_ptr = (uint8*)"64k/128k";
            //text3.length = 8;

		    ext_qos.guaranteed_bt_ul = 64;
		    ext_qos.max_bt_ul = 64;
            break;
        case 384:
            //text3.str_ptr = (uint8*)"64k/384k";            
            //text3.length = 8;

		    ext_qos.guaranteed_bt_ul = 64;
		    ext_qos.max_bt_ul = 64;
            break;
		case 8:
			//text3.str_ptr = (uint8 *)"8k/8k";
			//text3.length = 5;

		    ext_qos.guaranteed_bt_ul = 8;
		    ext_qos.max_bt_ul = 8;
			break;
		case 16:
			//text3.str_ptr = (uint8 *)"16k/16k";
			//text3.length = 7;

		    ext_qos.guaranteed_bt_ul = 16;
		    ext_qos.max_bt_ul = 16;
			break;
		case 32:
			//text3.str_ptr = (uint8 *)"16k/32k";
			//text3.length = 7;

		    ext_qos.guaranteed_bt_ul = 16;
		    ext_qos.max_bt_ul = 16;
			break;
        default:
            break;
    }*/

    /*+CR105931*/
    MNGPRS_SetExtendQosEx(MN_DUAL_SYS_1, 1, ext_qos, DOWNLOAD_E);
    //(&ext_qos,DOWNLOAD_E,item_setted);
    /*-CR105931*/
    //MMIEng_SetResultWinText(text1, text2, text3);
    //MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL);    
}

PUBLIC void MMIAPIENGTD_SetPSDataCtrlFlag(BOOLEAN b_flag)
{
    MMI_STRING_T text1 = {0}; 
    MMI_STRING_T text2 = {0}; 
    MMI_STRING_T text3 = {0}; 

    text1.wstr_ptr = L"PS Data Setting";
    text1.wstr_len = 15;
    text2.wstr_ptr = L"PS Data is";
    text2.wstr_len = 10;
    if(b_flag)
    {
        text3.wstr_ptr = L"Allow";
        text3.wstr_len = 5;       
    }
    else
    {
        text3.wstr_ptr = L"Forbidden";
        text3.wstr_len = 9;       
    }
    
	v_glo_pdpDataFlag = b_flag;
	
    MMIEng_SetResultWinText(text1, text2, text3);
    MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL); 
    
	return;
}
#endif
PUBLIC void MMIAPIENGTD_SetCS64Flag(BOOLEAN b_flag)
{
    MMI_STRING_T text1 = {0}; 
    MMI_STRING_T text2 = {0}; 
    MMI_STRING_T text3 = {0}; 

    text1.wstr_ptr = L"CS64 Setting";
    text1.wstr_len = 12;
    text2.wstr_ptr = L"CS64 Flag";
    text2.wstr_len = 8;
    if(b_flag)
    {
        text3.wstr_ptr = L"On";
        text3.wstr_len = 2;       
    }
    else
    {
        text3.wstr_ptr = L"Off";
        text3.wstr_len = 3;       
    }
 
	//v_tgl_csDataFlag = b_flag;
    
    MMIEng_SetResultWinText(text1, text2, text3);
    MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL); 
    
	return;
}

/********************************************************************************
 NAME:			SetSimSimulate
 DESCRIPTION:	Set Sim card simulate on or off.
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		qingfeng.dong
 DATE:			2005.09.13
********************************************************************************/
PUBLIC void MMIAPIENGTD_SetSimSimulate(BOOLEAN isOn)
{
    MMI_STRING_T text1 = {0}; 
    MMI_STRING_T text2 = {0}; 
    MMI_STRING_T text3 = {0}; 

    text1.wstr_ptr = L"Set Sim card";
    text1.wstr_len = 12;
    text2.wstr_ptr = L"Simulate";
    text2.wstr_len = 8;
    if(isOn)
    {
        text3.wstr_ptr = L"On";
        text3.wstr_len = 2;       
    }
    else
    {
        text3.wstr_ptr = L"Off";
        text3.wstr_len = 3;       
    }
//    DMNV_SetSimSimulate(isOn);
    MMIEng_SetResultWinText(text1, text2, text3);
    MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL); 
}
/********************************************************************************
 NAME:			SetUart1UsedFlag
 DESCRIPTION:	Set flag to indicate if uart1 is reserved for CY using, Dsp1 using or ARM Ref using.
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		qingfeng.dong
 DATE:			2005.10.17
********************************************************************************/
PUBLIC void MMIAPIENGTD_SetUart1UsedFlag(uint8 uart1flag)
{
    uint8 num = 0;
    MMI_STRING_T text1 = {0}; 
    MMI_STRING_T text2 = {0}; 
    MMI_STRING_T text3 = {0}; 

    text1.wstr_ptr = L"Uart1 will be";
    text1.wstr_len = 13;
    text2.wstr_ptr = L"used by";
    text2.wstr_len = 7;
    switch(uart1flag)
    {   
        case 0x00:
            text3.wstr_ptr = L"default";
            text3.wstr_len = 7;  
            num = 0;     
            break;
        case 0x02:
            text3.wstr_ptr = L"Dsp1";
            text3.wstr_len = 4;       
            num = 3;
            break;
        case 0x01:
            text3.wstr_ptr = L"PS";
            text3.wstr_len = 2;                   
            num = 2;
            break;
        default:
            break;
    }
    /*
         uart0  uart1
    0:  armlog   bt(default)
    1:   ps       dsp
    2:   armlog   ps
    3:   armlog   dsplog
    */
    //REFPARAM_SetUartMode(num);
    
    MMIEng_SetResultWinText(text1, text2, text3);
    MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL); 
}
/********************************************************************************
 NAME:			SetDeepSleep
 DESCRIPTION:	Set flag to indicate if Deep Sleep is enable.
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		qingfeng.dong
 DATE:			2005.10.17
********************************************************************************/
PUBLIC void MMIAPIENGTD_SetDeepSleep(BOOLEAN isOn)
{
    MMI_STRING_T text1 = {0}; 
    MMI_STRING_T text2 = {0}; 
    MMI_STRING_T text3 = {0}; 

    text1.wstr_ptr = L"Enable Deep";
    text1.wstr_len = 11;
    text2.wstr_ptr = L"Sleep";
    text2.wstr_len = 5;
    if(isOn)
    {
        text3.wstr_ptr = L"On";
        text3.wstr_len = 2;       
    }
    else
    {
        text3.wstr_ptr = L"Off";
        text3.wstr_len = 3;       
    }
//    DMNV_SetDeepSleep(isOn);
    MMIEng_SetResultWinText(text1, text2, text3);
    MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL); 
}
/********************************************************************************
 NAME:			SetRfLoop
 DESCRIPTION:	Set flag to indicate if Rf loop is enable.
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		qingfeng.dong
 DATE:			2005.10.25
********************************************************************************/
PUBLIC void MMIAPIENGTD_SetRfLoop(BOOLEAN isOn)
{
    MMI_STRING_T text1 = {0}; 
    MMI_STRING_T text2 = {0}; 
    MMI_STRING_T text3 = {0}; 

    text1.wstr_ptr = L"Enable RF";
    text1.wstr_len = 9;
    text2.wstr_ptr = L"Loop";
    text2.wstr_len = 4;
    if(isOn)
    {
        text3.wstr_ptr = L"On";
        text3.wstr_len = 2;       
    }
    else
    {
        text3.wstr_ptr = L"Off";
        text3.wstr_len = 3;       
    }
//    DMNV_SetRfLoop(isOn);
    MMIEng_SetResultWinText(text1, text2, text3);
    MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL); 
}

/********************************************************************************
 NAME:			SetFDNFlag
 DESCRIPTION:	Set flag to indicate if FDN is enable.
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		qingfeng.dong
 DATE:			2005.10.27
********************************************************************************/
PUBLIC void MMIAPIENGTD_SetFDNFlag(BOOLEAN isOn)
{
    MMI_STRING_T text1 = {0}; 
    MMI_STRING_T text2 = {0}; 
    MMI_STRING_T text3 = {0}; 

    text1.wstr_ptr = L"FDN";
    text1.wstr_len = 3;
    text2.wstr_ptr = L"Setting";
    text2.wstr_len = 7;
    if(isOn)
    {
        text3.wstr_ptr = L"On";
        text3.wstr_len = 2;       
    }
    else
    {
        text3.wstr_ptr = L"Off";
        text3.wstr_len = 3;       
    }

    //DMNV_SetFDNFlag(isOn);
    MMIEng_SetResultWinText(text1, text2, text3);
    MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL); 
}
/********************************************************************************
 NAME:			SetBDNFlag
 DESCRIPTION:	Set flag to indicate if BDN is enable.
 PARAM IN:		None
 PARAM OUT:		
 AUTHOR:		qingfeng.dong
 DATE:			2005.10.27
********************************************************************************/
PUBLIC void MMIAPIENGTD_SetBDNFlag(BOOLEAN isOn)
{
    MMI_STRING_T text1 = {0}; 
    MMI_STRING_T text2 = {0}; 
    MMI_STRING_T text3 = {0}; 

    text1.wstr_ptr = L"BDN";
    text1.wstr_len = 3;
    text2.wstr_ptr = L"Setting";
    text2.wstr_len = 7;
    if(isOn)
    {
        text3.wstr_ptr = L"On";
        text3.wstr_len = 2;       
    }
    else
    {
        text3.wstr_ptr = L"Off";
        text3.wstr_len = 3;       
    }
    //DMNV_SetBDNFlag(isOn);
    MMIEng_SetResultWinText(text1, text2, text3);
    MMK_CreateWin((uint32*)MMIENG_SHOW3RESULT_WIN_TAB, PNULL); 
}
//@xiefen add end 

/*****************************************************************************/
// 	Description : intial mmieng module  
//	Global resource dependence : none
//  Author: jibin
//	Note:
/*****************************************************************************/
/*
PUBLIC void MMIENG_InitModule(void)
{
    MMIENG_RegMenuGroup();  //³õÊ¼»¯mmieng menu
}
*/
#endif// ENGTD_SUPPORT