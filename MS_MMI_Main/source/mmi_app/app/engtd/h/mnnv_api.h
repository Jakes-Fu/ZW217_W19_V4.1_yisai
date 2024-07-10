#if !defined (_MNNV_API_H)
#define       _MNNV_API_H

/******************************************************************************
 ** File Name:      mnnv_api.h                                                *
 ** Author:         qingfeng.dong                                               *
 ** DATE:           09/15/2005                                                *
 ** Copyright:      2002 Spreadtrum, Incoporated. All Rights Reserved.         *
 *                                                                            *
 * DESCRIPTION      this header file include nv apis declaration,             *
 *                  not include APIs for MMI                                  *
 *                                                                            *
 *                                                                            *
 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 09/15/2005    qingfeng.dong      Create.                                   *
 ******************************************************************************/


/*****************************************************************************/
//   Macro and data strcut define
/*****************************************************************************/

//qingfeng.dong 2005-09-13, add for engineer mode functions, 
//such as IMSI, freq list and SIM status setting.
#define MN_MAX_ENG_IMSI_ARR_LEN    9 

typedef struct 
{
    uint8 len;
    uint8 val[MN_MAX_ENG_IMSI_ARR_LEN];
    BOOLEAN isused;
} MN_ENG_IMSI_T;

typedef struct
{
    uint8 len;
    uint16 val;
}MN_ENG_FREQ_INFO_T;

typedef struct 
{
    MN_ENG_FREQ_INFO_T info[4];
    BOOLEAN isused;
} MN_ENG_FREQ_T;

typedef struct 
{
    uint8 isSimulate; //0x00 use sim simulate, 0x01 use sim card
} MN_ENG_SIM_STATUS_T;

typedef struct 
{
/*
*0x00: used by Arm Ref
*0x01: used by Dsp1
*0x02: used by CY ps
*/
    uint8 usedBy; 
} MN_ENG_UART1_T;

typedef struct 
{
    uint8 isSleep; //0x01 enable sleep
} MN_ENG_SLEEP_STATUS_T;

typedef struct 
{
    uint8 param_id; //0-127
} MN_ENG_PARAM_ID_T;

typedef struct 
{
    uint8 isSimLogOpen; //0x01 enable usim log
} MN_ENG_SIM_LOG_STATUS_T;

//Structure used to save all engineer phone settings in NV.
typedef struct
{
    MN_ENG_IMSI_T imsi;
    MN_ENG_FREQ_T freq;
    MN_ENG_SIM_STATUS_T sim_status;
    MN_ENG_UART1_T uart1;
    MN_ENG_SLEEP_STATUS_T sleep_status;
    MN_ENG_SIM_LOG_STATUS_T sim_log_status;
    MN_ENG_PARAM_ID_T param_id;
    uint8   asn_version_selection;
    uint8   drx_cycle;
    /*
    *Keep the struct size stable by changing the size of the reserved array!
    *And by this means, the NV struct can be more stable, needn't to change so frequently.
    */
    uint8 reserved[97]; 
}MN_ENG_NV_SETTING_T;

/*****************************************************************************/
// 	Description : initialize NV, this wil initial some setting used by Protocol Stack,
//                      and should be called before PS start!
//	Global resource dependence : none
//  Author:       qingfeng.dong
//	Note:
/*****************************************************************************/
extern void DMNV_PrevInit(
              void
              );
/*****************************************************************************/
// 	Description : initialize NV, it will be called when MN is initialized
//	Global resource dependence : none
//  Author:       qingfeng.dong
//	Note:
/*****************************************************************************/
extern void DMNV_Init(
              void
              );

/*****************************************************************************/
// 	Description : Init param id from nv
//	Global resource dependence : none
//  Author:    bo.chen
//	Note:
/*****************************************************************************/
BOOLEAN DMNV_InitParamID(
			void
			);
/*****************************************************************************/
// 	Description : Set param id to NV
//	Global resource dependence : none
//  Author:  bo.chen
//	Note:
/*****************************************************************************/
BOOLEAN DMNV_SetParamID(
		        MN_ENG_PARAM_ID_T param_id
			);

/*****************************************************************************/
// 	Description : Init IMSI with the value saved in NV!
//	Global resource dependence : none
//  Author:       qingfeng.dong
//	Note:
/*****************************************************************************/
extern BOOLEAN DMNV_InitIMSI(
			void
			);
/*****************************************************************************/
// 	Description : Set IMSI value to NV, be careful that the array format should be identical to 
//                       global value g_imsi!
//	Global resource dependence : none
//  Author:       qingfeng.dong
//	Note:
/*****************************************************************************/
extern BOOLEAN DMNV_SetIMSI(
		        MN_ENG_IMSI_T imsi
			);

/*****************************************************************************/
// 	Description : Init frequency point list with value saved in NV
//	Global resource dependence : none
//  Author:       qingfeng.dong
//	Note:
/*****************************************************************************/
extern BOOLEAN DMNV_InitFreqList(
			void
			);
/*****************************************************************************/
// 	Description : Set frequency point list into NV
//	Global resource dependence : none
//  Author:       qingfeng.dong
//	Note:
/*****************************************************************************/
extern BOOLEAN DMNV_SetFreqList(
			MN_ENG_FREQ_T freq
			);

/*****************************************************************************/
// 	Description : Init SIM status with the value saved in NV!
//	Global resource dependence : none
//  Author:       qingfeng.dong
//	Note:
/*****************************************************************************/
extern BOOLEAN DMNV_InitSimSimulate(
			void
			);


/*****************************************************************************/
// 	Description : Set SIM status value to NV.
//	Global resource dependence : none
//  Author:       qingfeng.dong
//	Note:
/*****************************************************************************/
extern BOOLEAN DMNV_SetSimSimulate(
		        BOOLEAN isSimulate
			);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author:       qingfeng.dong
//	Note:
/*****************************************************************************/
extern BOOLEAN DMNV_InitUart1Used(
			void
			);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author:       qingfeng.dong
//	Note: uart1flag:
//          0x00                  ARM Ref(use our ATC & TCP/IP)
//          0x01                  CY ps
//          0x02                  DSP
//          0xxx                  undefine
/*****************************************************************************/
extern BOOLEAN DMNV_SetUart1Used(
		        uint8 uart1flag 
			);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author:       qingfeng.dong
//	Note:
/*****************************************************************************/
extern BOOLEAN DMNV_InitDeepSleep(
			void
			);
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author:       qingfeng.dong
//	Note:
/*****************************************************************************/
extern BOOLEAN DMNV_SetDeepSleep(
		        BOOLEAN isOn 
			);
/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author:       qingfeng.dong
//	Note:
/*****************************************************************************/
extern BOOLEAN DMNV_InitUsimLogStatus(
			void
			);

/*****************************************************************************/
// 	Description : 
//	Global resource dependence : none
//  Author:       qingfeng.dong
//	Note:
/*****************************************************************************/
extern BOOLEAN DMNV_SetRfLoop(
		        BOOLEAN isOn 
			);
/*****************************************************************************/
// 	Description : set the flag to control the version of asn.1
//	Global resource dependence : none
//  Author:       bo.chen
//	Note:   0: asn.1 v490; 0: asn.1 v4h0; 2: asn.1 v4h0&UpPCH shifting
/*****************************************************************************/
BOOLEAN DMNV_SetASNVersionSelection(
		        uint8 asn_version_selection 
			);

/*****************************************************************************/
// 	Description : get the flag to control the version of asn.1
//	Global resource dependence : none
//  Author:  bo.chen
//	Note:   0: asn.1 v490; 0: asn.1 v4h0; 2: asn.1 v4h0&UpPCH shifting
/*****************************************************************************/
uint8 DMNV_GetASNVersionSelection(
		    void
			);

/*****************************************************************************/
// 	Description : init asn version selection after we read nv(fail or success)
//	Global resource dependence : none
//  Author: bo.chen
//	Note:
/*****************************************************************************/
BOOLEAN DMNV_InitASNVersionSelection(
			void
			);
/*****************************************************************************/
// 	Description : init drx cycle len
//	Global resource dependence : none
//  Author: bo.chen
//	Note:
/*****************************************************************************/
BOOLEAN DMNV_InitDRXCycleLen(
			void
			);
	
#endif /* _MNNV_API_H */
