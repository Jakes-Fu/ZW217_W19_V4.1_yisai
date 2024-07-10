/******************************************************************************
 ** File Name:      uplink_adp.h                                           *
 ** Author:         shujing.dong                                              *
 ** DATE:           06/28/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incoporated. All Rights Reserved.        *
 ** Description:    audio codec to finish recording data from device in arm   *
 **                 and translating it to dsp to net                          *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 06/28/2010     shujing.dong     Create.                                   *
 ******************************************************************************/
  
#ifndef _UPLINK_ADP_H
#define _UPLINK_ADP_H
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
 **  set pcm data flow direction after bt device to dsp                                          **
 **  b_if_to_net:     0:  dsp data flow not to net                            ** 
 **			          1:  dsp data flow to net                                **
 **  b_if_to_speaker: 0:  dsp data flow not to spreaker                       ** 
 **			          1:  dsp data flow to speaker                            **
 **  note:                                                                    **
 **		1.the extend operations as below should be executed before playing.   **
 **     2.normal BT call, this extend operations can do or not, 
 **        value(hBtToDspPlay,1,0)
 **     3.CALL in BT_DIALING state, this extend operations should be executed 
 **        and the value(hBtToDspPlay,0,1)
 **  ANSI2UINT16("SET_UPLINKIFTONET");
 **---------------------------------------------------------------------------**/
//ANSI2UINT16("SET_UPLINKIFTONET");
//argc: 
#define UPLINK_DATA_TO_COUNT 3
//argv: structure of
typedef struct
{
	HAUDIO  haudiohandle;
	BOOLEAN  b_if_to_net; //SCI_TRUE:to net, SCI_FALSE:not to net
	BOOLEAN  b_if_to_speaker; //SCI_TRUE:to speaker, SCI_FALSE:not to speaker
}UPLINK_DATA_TO_T;

/*
sample code:
1.create voice audio handle:hBtToDspPlay by  AUDIO_CreateNormalHandle;

2.
UPLINK_DATA_TO_T uplink_param = {hBtToDspPlay, 0, 1};//if bt_dialing call
UPLINK_DATA_TO_T uplink_param = {hBtToDspPlay, 1, 0};//if normal bt_call

AUDIO_ExeCodecExtOpe(
hBtToDspPlay,
ANSI2UINT16("SET_UPLINKIFTONET "),
UPLINK_DATA_TO_COUNT,
& uplink_param,
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
PUBLIC  void UPLINK_RegCallbackToGetPcmData(RECORD_GETPCMDATA callback);    

/*****************************************************************************/
//  Description: This Function is used to register codec to audio codec to    
//               finish recording data from device in arm  and translating 
//               it to dsp to net.
//  Author:      shujing.dong   
//  Note:           
/*****************************************************************************/
PUBLIC HAUDIOCODEC UPLINK_RegCodecPlugger(
    HCOLLISION hCollision,
    AUDIO_CODEC_EXT_CFGINFO_T * ptCodecExtCfgInfo
    );

#if defined(BT_ARM_NXP_ENABLE) && defined(NXP_LVVE_SUPPORT)
/*****************************************************************************/
//! \fn PUBLIC void UPLINK_ChangeNVConfig(
//!    AUDMOD_DEVMOD_T *ptAudMode,
//!    void *para
//!    )
//! \return AUDIO_RESULT_E
//! \brief  Description: This function is used to set if data to net/spek.
//! \author Author:
//! \note   Note: None. 
/*****************************************************************************/
PUBLIC void  UPLINK_ChangeNVConfig(
    AUDMOD_DEVMOD_T *ptAudMode,
    void *para
 );
#endif
PUBLIC void UPLINK_WriteDataToNXP(void);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/    
#ifdef __cplusplus
}
#endif

#endif 

