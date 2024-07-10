/******************************************************************************
 ** File Name:      downlink_adp.h                                             *
 ** Author:         shujing.dong                                              *
 ** DATE:           06/28/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    audio codec to finish recording data from dsp and         *
 **                 translating it to device in arm                           *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/28/2010     shujing.dong     Create.                                   *
 ******************************************************************************/
  
#ifndef _DOWNLINK_ADP_H
#define _DOWNLINK_ADP_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         MACRO Definations                                     *
 **---------------------------------------------------------------------------*/
/**---------------------------------------------------------------------------**
 **  set pcm data flow direction after bt device from dsp                                          **
 **  b_if_from_net: 0:  dsp data flow not from net                            ** 
 **			        1:  dsp data flow from net                                **
 **  b_if_from_mic: 0:  dsp data flow not from mic                            ** 
 **			        1:  dsp data flow from mic                                **
 **  note:                                                                    **
 **		1.the extend operations as below should be executed before playing.   **
 **     2.normal BT call, this extend operations can do or not, 
 **        value(hBtFromDspPlay,1,0)
 **     3.CALL in BT_DIALING state, this extend operations should be executed 
 **        and the value(hBtFromDspPlay,0,1)
 **  ANSI2UINT16("SET_DOWNLINKIFFROMNET");
 **---------------------------------------------------------------------------**/
//ANSI2UINT16("SET_DOWNLINKIFFROMNET");
//argc: 
#define DOWNLINK_SOURCE_FROM_COUNT 3
//argv: structure of
typedef struct
{
	HAUDIO  haudiohandle;
	BOOLEAN  b_if_from_net; //SCI_TRUE:form net, SCI_FALSE:not from net
	BOOLEAN  b_if_from_mic; //SCI_TRUE:from mic, SCI_FALSE:not to mic
}DOWNLINK_SOURCE_FROM_T;

typedef enum
{
    DOWNLINK_FORM_NET_TO_SPK,
    DOWNLINK_FORM_MIC_TO_NET,
    DOWNLINK_MAX
}DOWNLINK_WORK_MODE_E;
/*
sample code:
1.create voice audio handle:hBtFromDspPlay by  AUDIO_CreateNormalHandle;

2.
DOWNLINK_SOURCE_FROM_T downlink_param = {hBtFromDspPlay, 0, 1};//if bt_dialing call
DOWNLINK_SOURCE_FROM_T downlink_param = {hBtFromDspPlay, 1, 0};//if normal bt_call

AUDIO_ExeCodecExtOpe(
hBtFromDspPlay,
ANSI2UINT16("SET_DOWNLINKIFFROMNET"),
UPLINK_DATA_TO_COUNT,
& downlink_param,
&param_result
);
*/
/**---------------------------------------------------------------------------*
 **                         Data Structures                                   *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Global Variables                                  *
 **---------------------------------------------------------------------------*/
 
/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/
PUBLIC  void DOWNLINK_RegCallbackToGetPcmData(RECORD_GETPCMDATA callback);    

/*****************************************************************************/
//  Description:    This Function is used to register codec to record data 
//                  from dsp and translate it to arm to play.
//  Author:         shujing.dong  
//  Note:           
/*****************************************************************************/
PUBLIC HAUDIOCODEC DOWNLINK_RegCodecPlugger(
    HCOLLISION hCollision,
    AUDIO_CODEC_EXT_CFGINFO_T * ptCodecExtCfgInfo
    );
PUBLIC DOWNLINK_WORK_MODE_E  DOWNLINK_GetWorkMode(void);
PUBLIC void DOWNLINK_ReadDataFromNXP(uint16 *puiSrcData);
#if defined(BT_ARM_NXP_ENABLE) && defined(NXP_LVVE_SUPPORT) 
PUBLIC void DOWNLINK_SendVoiceDataToDA(uint16 *data_ptr);
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif 



