/******************************************************************************
 ** File Name:      admm.h                                              
 ** Author:         shujing.dong                                              
 ** DATE:           12/30/2005                                                
 ** Copyright:      2005 Spreatrum, Incoporated. All Rights Reserved.         
 ** Description:    This file defines the basic operation interfaces  
 **                  and data structure of audio device mode manager.                     
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       
 **  -----------------------------------------------------------------------  
 ** DATE           NAME             DESCRIPTION                               
 ** 11/15/2005     shujing.dong     Create.                                   
 ******************************************************************************/

/*! \file admm.h
*  \author Shujing.Dong 
*  \date Dec 30, 2005
*  \brief This file defines the basic operation interfaces and data structure 
*          of audio device mode manager.
*/ 

#ifndef _ADMM_H
#define _ADMM_H
/**---------------------------------------------------------------------------**
 **                         Dependencies                                      **
 **---------------------------------------------------------------------------**/
#include "sci_types.h"
#include "arm_reg.h"

/**---------------------------------------------------------------------------**
 **                        Debugging Flag                                     **
 **---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
**                         Compiler Flag                                      **
**----------------------------------------------------------------------------**/
#ifdef __cplusplus
extern   "C"
{
#endif
/**---------------------------------------------------------------------------**
 **                         MACRO Definations                                 **
 **---------------------------------------------------------------------------**/
 #ifdef WIN32
 #define PACKED
 #else
#define PACKED __packed
#endif
#define AUDMOD_NAME_MAX_LEN (16)

#define AUDMOD_MAX_VOL (9)
#define AUDMOD_MIN_VOL (1)

/*Channel Num definition.*/
#define AUDMOD_CHANNEL_NONE 0
#define AUDMOD_CHANNEL_1 BIT_0
#define AUDMOD_CHANNEL_2 BIT_1

/*ARM Device Control MASK*/
#define AUDMOD_DA_MASK (0x0000000F)
#define AUDMOD_DA_MASK_OFFSET (0)
#define AUDMOD_DA_OUTPUT_MASK (0x00000010)  // AUX or PRI Output
#define AUDMOD_STEREO_MASK (0x00000020)
#define AUDMOD_DA_HSOUT_MASK (0x00000040)   // Headset Output
#define AUDMOD_PRI_PA_MASK (0x00000100)
#define AUDMOD_AUX_PA_MASK (0x00000200)
#define AUDMOD_HEADL_PA_MASK (0x00000400)
#define AUDMOD_HEADR_PA_MASK (0x00000800)
#define AUDMOD_DA_POLARITY_MASK (0x0000F000)
#define AUDMOD_DA_POLARITY_MASK_OFFSET (12)

#define AUDMOD_AD_MASK (0x0000000F)
#define AUDMOD_AD_MASK_OFFSET (0)
#define AUDMOD_AD_INPUT_MASK (0x000000F0)
#define AUDMOD_AD_INPUT_MASK_OFFSET (4)
#define AUDMOD_AD_PA_MASK (0x00000F00)
#define AUDMOD_AD_PA_MASK_OFFSET (8)
#define AUDMOD_AD_POLARITY_MASK (0x0000F000)
#define AUDMOD_AD_POLARITY_MASK_OFFSET (12)

/*Mute type*/
#define AUDMOD_MUTE_DISABLE (0)
#define AUDMOD_MUTE_ENABLE (1)

/*SPKOFF type*/
#define AUDMOD_SPKOFF_DISABLE (0)
#define AUDMOD_SPKOFF_ENABLE (1)

/*Dolphin voiceband device control parameter */
#define AUDMOD_AUDIO_M         (0)
#define AUDMOD_AUDIO_DOLPHIN   (1)

#define AUDMOD_AUDIO_CHANGE_ENABLE      (0)
#define AUDMOD_AUDIO_CHANGE_DISABLE        (1)

#define AUDMOD_AUDIO_EDITION_OFFSET  (14)
#define AUDMOD_AUDIO_CHANGE_ENABLE_OFFSET  (15)
#define AUDMOD_OUTPUT_DOLPHIN_MASK   (0x00000010)  // AUX or PRI Output
#define AUDMOD_STEREO_DOLPHIN_MASK   (0x00000004)  // Mono or Stereo
#define AUDMOD_HSOUT_DOLPHIN_MASK    (0x00000008)  // Headset or Speraker/Receiver
#define AUDMOD_DA_DOLPHIN_MASK       (0x00000080)  // ADC off/on

/**---------------------------------------------------------------------------**
 **                         Data Structures                                   **
 **---------------------------------------------------------------------------**/
typedef enum
{
    AUDMOD_NO_ERROR = 0x0,
    AUDMOD_ALLOC_ERROR = BIT_0, /*<!alloc error.*/
    AUDMOD_INPUT_PARA_ERROR = BIT_1, /*<!input parameter error.*/
    AUDMOD_MODE_NO_EXIST = BIT_3,  /*<!the mode with this name is not exist.*/
    
    AUDMOD_ERROR    
}AUDMOD_RESULT_E;

PACKED typedef struct 
{
    /*Audio System Variables*/
    uint16 dl_DA_device_internal;//uint32 dl_DA_device;
    uint16 dl_DA_device_external;    
    
    uint16 ul_AD_device_internal;//uint32 ul_AD_device;
    uint16 ul_AD_device_external; 
       
    uint16 sidetone_switch;
    uint16 aec_switch;
    uint16 volume_mode; 
   
    uint16 dl_PGA_gain_internal;//uint32 dl_PGA_gain;
    uint16 dl_PGA_gain_external;    
    
    uint16 ul_PGA_gain_internal;//uint32 ul_PGA_gain;
    uint16 ul_PGA_gain_external; 
    
    uint16 Samplerate; 

    /*Audio Adjusting Variables*/
    uint16 dl_DP_gain;
    uint16 dl_DP_attenu;
    uint16 dl_EQ_bass_alpha;
    uint16 dl_EQ_bass_beta;
    uint16 dl_EQ_bass_gama;
    uint16 dl_EQ_bass_gain;
    uint16 dl_EQ_mid_alpha;
    uint16 dl_EQ_mid_beta;
    uint16 dl_EQ_mid_gama;
    uint16 dl_EQ_mid_gain;     
    uint16 dl_EQ_treble_alpha;
    uint16 dl_EQ_treble_beta;
    uint16 dl_EQ_treble_gama;
    uint16 dl_EQ_treble_gain;
    uint16 digital_sidetone_gain;
    uint16 ul_DP_gain;
    uint16 ul_DP_attenu;
    uint16 ul_EQ_bass_alpha;
    uint16 ul_EQ_bass_beta;
    uint16 ul_EQ_bass_gama;
    uint16 ul_EQ_bass_gain;
    uint16 ul_EQ_treble_alpha;
    uint16 ul_EQ_treble_beta;
    uint16 ul_EQ_treble_gama;
    uint16 ul_EQ_treble_gain;

    /*Audio Module Switch*/
    uint16 dl_POP_switch;
    uint16 dl_AGC_switch;
    uint16 dl_EQ_AGC_switch;
    uint16 DP_switch;
    uint16 dl_EQ_switch;
    uint16 Dl_agc_Rsv0;
    uint16 Dl_agc_Rsv1;
    uint16 ul_EQ_switch;
    uint16 dl_DA_pop_switch;

    /*AEC Controller Parameter*/
    uint16 ul_FIR_HPF_enable;
    uint16 aec_enable;
    uint16 pdelay;
    uint16 dl_ref_HPF_enable;
    uint16 decor_filter_enable;
    uint16 fir_taps;
    uint16 Aec_frozen;
    uint16 coeff_frozen;
    uint16 DT_dect_threshold;    
    uint16 Dt_noise_floor;
    uint16 step_size;
    uint16 coeff_norm_shift;
    uint16 SA_frozen;
    uint16 send_attenu_in_dt;
    uint16 send_attenu_in_rv;
    uint16 send_threshold;
    uint16 r2dt_threshold;
    uint16 s2dt_threshold;
    uint16 recv_threshold;
    uint16 Bn40;
    uint16 Sa_AR;

    /*Noise Restrain Parameter*/
    uint16 ng_select;     
    uint16 alpha_distor;
    uint16 beta_distor ; 
    uint16 ul_ng_p1k_wPyy_a;    
    uint16 ul_ng_p1k_wPyy_n ;   
    uint16 ul_ng_p1k_holdc;     
    uint16 ul_ng_p1k_ATT  ;     
    uint16 ul_ng_c1k_wPyy_a ;   
    uint16 ul_ng_c1k_wPyy_n ;   
    uint16 ul_ng_c1k_holdc ;    
    uint16 ul_ng_c1k_ATT;       
    uint16 dl_ng_p1k_wPyy_a ;   
    uint16 dl_ng_p1k_wPyy_n ;   
    uint16 dl_ng_p1k_holdc;     
    uint16 dl_ng_p1k_ATT  ;     
    uint16 dl_ng_c1k_wPyy_a ;   
    uint16 dl_ng_c1k_wPyy_n ;   
    uint16 dl_ng_c1k_holdc  ;   
    uint16 dl_ng_c1k_ATT    ;   
    uint16 DA_limit;
    uint16 reserved;
    
    /*FIR Parameter*/
    uint16 extend[142];
    
    /* Arm volume gain Parameter */
    uint16 armVolume[10];
    
    /* Dsp volume gain Parameter */
    uint16 dspVolume[10];

}AUDIO_STRUCT_T;


typedef struct 
{
	uint8 ucADChannel; //Input select  	
	#define VB_AD_MIC_INPUT	0 //MIC input 
	#define VB_AD_AUX_INPUT	1 //Auxiliary input
	//for rocky
	#define VB_AD_MIC1_INPUT    0 //Mic 1 input
 	#define VB_AD_MIC2_INPUT    1  //Mic 2 input
	
	uint8 ucDAChannel; //Output select 	
	#define VB_DA_PRI_OUTPUT 	0 //Earphone output
	#define VB_DA_AUX_OUTPUT 	1 //Auxiliary output
	
	uint8 ucDA0Reverse; //DA0 Reverse Enable bit
	#define AUD_DA0_REVERSE_EN      1
	#define AUD_DA0_NON_REVERSE   0

	uint8 ucDA1Reverse;  //DA1 Reverse Eanble bit
	#define AUD_DA1_REVERSE_EN      1
	#define AUD_DA1_NON_REVERSE   0

	uint8 ucStereoEn; //Stereo output enable
	#define VB_AUD_STEREO		      1
	#define VB_AUD_NON_STEREO	      0
	
	uint8 ucSpkAmpEn; //speaker amp enable
	#define AUD_SPK_AMP_EN		      1 
	#define AUD_SPK_NON_AMP	      0

	uint8 ucHeadsetAmpEn; //speaker amp enable
	#define AUD_HEADSET_AMP_EN     1 
	#define AUD_HEADSET_NON_AMP  0

    uint8 ucMixerInput;
    #define AUD_DAC_MIXER_INPUT  0
    #define AUD_LINEIN_MIX_INPUT 1    
}AUDMOD_DEVICECTL_T;


typedef struct AUDMOD_Devmode_struct
{
    uint8 ucModeName[AUDMOD_NAME_MAX_LEN];	// Device mode name.
    AUDIO_STRUCT_T tAudioStruct;	// Audio structure(88 half-words)    
}AUDMOD_DEVMOD_T;

typedef struct AUDMOD_Devmode_node_struct
{
    AUDMOD_DEVMOD_T tAudioDevMode;	// Device Mode Information

    struct AUDMOD_Devmode_node_struct* ptNext;	// next element    
    struct AUDMOD_Devmode_node_struct* ptPre; // previous element
}AUDMOD_DEVMODNODE_T;

typedef void (*AUDMOD_SETDACHANNEL_PFUNC)(uint32 uiChannel);
typedef void (*AUDMOD_SETOUTPUTMODE_PFUNC)(BOOLEAN bAuxon);
typedef void (*AUDMOD_SETSTEREO_PFUNC)(BOOLEAN bStereoOn);
typedef void (*AUDMOD_SETHEADSETOUT_PFUNC)(BOOLEAN bEnable);
typedef void (*AUDMOD_SETPRIPA_PFUNC)(BOOLEAN bEnable);
typedef void (*AUDMOD_SETAUXPA_PFUNC)(BOOLEAN bEnable);
typedef void (*AUDMOD_SETHEADLPA_PFUNC)(BOOLEAN bEnable);
typedef void (*AUDMOD_SETHEADRPA_PFUNC)(BOOLEAN bEnable);
typedef void (*AUDMOD_SETDADATAPOLARITY_PFUNC)(uint32 uiPolarity);
typedef void (*AUDMOD_SETADCHANNEL_PFUNC)(uint32 uiChannel);
typedef void (*AUDMOD_SETINPUTMODE_PFUNC)(uint32 uiStatus);
typedef void (*AUDMOD_SETADPA_PFUNC)(uint32 uiStatus);
typedef void (*AUDMOD_SETADDATAPOLARITY_PFUNC)(uint32 uiStatus);
typedef void (* AUDMOD_SETVOLUME_PFUNC)(uint32 uiVol, uint32 uiBase);   // uiBase value is peer to 4 level volume.
typedef void (* AUDMOD_SETMUTE_PFUNC)(uint32 uiType);
typedef void (* AUDMOD_SETSPKOFF_PFUNC)(uint32 uiType);
typedef void (* AUDMOD_SPAREOPE_PFUNC)(AUDMOD_DEVMOD_T* ptDevMode);

typedef struct AUDMOD_DevInfo_struct
{
    AUDMOD_SETDACHANNEL_PFUNC pSetDAChannel;
    AUDMOD_SETOUTPUTMODE_PFUNC pSetOutputMode;
    AUDMOD_SETSTEREO_PFUNC pSetStereo;
    AUDMOD_SETHEADSETOUT_PFUNC pSetHeadsetOut;
    AUDMOD_SETPRIPA_PFUNC pSetPRIPA;
    AUDMOD_SETAUXPA_PFUNC pSetAUXPA;
    AUDMOD_SETHEADLPA_PFUNC pSetHeadLPA;
    AUDMOD_SETHEADRPA_PFUNC pSetHeadRPA;
    AUDMOD_SETDADATAPOLARITY_PFUNC pSetDADataPolarity;
    AUDMOD_SETADCHANNEL_PFUNC pSetADChannel;
    AUDMOD_SETINPUTMODE_PFUNC pSetInputMode;
    AUDMOD_SETADPA_PFUNC pSetADPA;
    AUDMOD_SETADDATAPOLARITY_PFUNC pSetADDataPolarity;
    AUDMOD_SETVOLUME_PFUNC pSetVolume;
    AUDMOD_SETMUTE_PFUNC pSetMute;
    AUDMOD_SETSPKOFF_PFUNC pSetSPKOff;
    AUDMOD_SPAREOPE_PFUNC pSpareOpe;
}AUDMOD_DEVINFO_T;


/**---------------------------------------------------------------------------**
 **                         Global Variables                                  **
 **---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
 **                         Constant Variables                                **
 **---------------------------------------------------------------------------**/

/**---------------------------------------------------------------------------**
 **                         Function Prototypes                               **
 **---------------------------------------------------------------------------**/
/**---------------------------------------------------------------------------**
 **                         Function Define                                   **
 **---------------------------------------------------------------------------**/
/*****************************************************************************/
//! \fn PUBLIC AUDIO_RESULT_E AUDMOD_InitDevModeManager(void) 
//! \param void No Parameter
//! \return Operation results. \sa AUDMOD_RESULT_E
//! \brief  Description:  This function is to init Device Mode Manager.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDMOD_RESULT_E AUDMOD_InitDevModeManager(void);

/*****************************************************************************/
//! \fn PUBLIC AUDMOD_RESULT_E AUDMOD_SetDevMode(
//!    const uint8* pucModeName
//!    DA_DEVICE_T *ptDeviceCtrl, 
//!    )
//! \param pucModeName Device mode's name.
//! \param ptDeviceCtrl, corresponding to device control param.
//! \return Operation results. \sa AUDMOD_RESULT_E
//! \brief  Description:  This function is to set current device mode.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDMOD_RESULT_E AUDMOD_SetDevMode(
    const uint8* pucModeName, 
    AUDMOD_DEVICECTL_T *ptDeviceCtrl
);

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDMOD_GetDevModeNumber(void)
//! \param void No parameter.
//! \return The total number of registered device modes.
//! \brief  Description:  This function is to get the number of device mode in the current 
//!                             device mode manager.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC uint32 AUDMOD_GetDevModeNumber(void);

/*****************************************************************************/
//! \fn PUBLIC AUDMOD_DEVMOD_T* AUDMOD_GetFirstDevMode(void)
//! \param void No parameter.
//! \return The first device mode saved in device mode manager.
//! \brief  Description:  This function is to get the first device mode in the current 
//!                             device mode manager.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDMOD_DEVMOD_T* AUDMOD_GetFirstDevMode(void);

/*****************************************************************************/
//! \fn PUBLIC AUDMOD_DEVMOD_T* AUDMOD_GetNextDevMode(
//!    AUDMOD_DEVMOD_T* ptPreDevMode
//!    )
//! \param ptPreDevMode Previous device mode pointer.
//! \return The device mode after ptPreDevMode.
//! \brief  Description:  This function is to get the address of info structure 
//!                         of next device mode
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDMOD_DEVMOD_T* AUDMOD_GetNextDevMode(
    AUDMOD_DEVMOD_T* ptPreDevMode
    );

/*****************************************************************************/
//! \fn PUBLIC AUDMOD_RESULT_E AUDMOD_AddDevMode(
//!    AUDMOD_DEVMOD_T* ptDevMode
//!    )
//! \param ptDevMode Added device mode.
//! \return Operation results. \sa AUDMOD_RESULT_E
//! \brief  Description:  This function is to add one device mode into device mode manager.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDMOD_RESULT_E AUDMOD_AddDevMode(
    AUDMOD_DEVMOD_T* ptDevMode
    );

/*****************************************************************************/
//! \fn PUBLIC AUDMOD_RESULT_E AUDMOD_RemoveDevMode(
//!    const uint8* pucModeName
//!    )
//! \param pucModeName Device mode name which is needed to remove from device mode 
//!                                 manager.
//! \return Operation results. \sa AUDMOD_RESULT_E
//! \brief  Description:  This function is to remove one device mode from device mode manager.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDMOD_RESULT_E AUDMOD_RemoveDevMode(
    const uint8* pucModeName
    );

/*****************************************************************************/
//! \fn PUBLIC AUDMOD_RESULT_E AUDMOD_RegDevice(
//!    AUDMOD_DEVINFO_T* ptDevInfo
//!    )
//! \param ptDevInfo Audio Device Infor.
//! \return Operation results. \sa AUDMOD_RESULT_E
//! \brief  Description:  This function is to register audio device into device mode manager.
//!                             The device infor includes all device operation interface.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDMOD_RESULT_E AUDMOD_RegDevice(
    AUDMOD_DEVINFO_T* ptDevInfo
    );

/*****************************************************************************/
//! \fn PUBLIC AUDMOD_DEVMOD_T* AUDMOD_GetCurrentDevMode(void)
//! \param void No parameter.
//! \return Current device mode structure pointer.
//! \brief  Description:  This function is to get current device mode.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDMOD_DEVMOD_T* AUDMOD_GetCurrentDevMode(void);

/*****************************************************************************/
//! \fn PUBLIC AUDMOD_DEVMOD_T* AUDMOD_GetDevMode(const uint8* pucModeName)
//! \param pucModeName Device mode name searched.
//! \return  device mode structure pointer.
//! \brief  Description:  This function is to get  device mode through mode name.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDMOD_DEVMOD_T* AUDMOD_GetDevMode(
    const uint8* pucModeName
    );

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDMOD_GetDAChannelSetting(void)
//! \param void No parameter.
//! \return DA Channel Number.
//! \brief  Description:  This function is to get current DA channel setting.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC uint32 AUDMOD_GetDAChannelSetting(void);

/*****************************************************************************/
//! \fn PUBLIC BOOLEAN AUDMOD_GetOutputModeSetting(void)
//! \param void No parameter.
//! \return DA output port. SCI_TRUE - AUX, SCI_FALSE - PRI.
//! \brief  Description:  This function is to get current DA output port setting.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC BOOLEAN AUDMOD_GetOutputModeSetting(void);

/*****************************************************************************/
//! \fn PUBLIC BOOLEAN AUDMOD_GetStereoSetting(void)
//! \param void No parameter.
//! \return SCI_TRUE - Stereo, SCI_FALSE - Mono.
//! \brief  Description:  This function is to get current stereo setting.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC BOOLEAN AUDMOD_GetStereoSetting(void);

/*****************************************************************************/
//! \fn PUBLIC BOOLEAN AUDMOD_GetHeadsetOutSetting(void)
//! \param void No parameter.
//! \return SCI_TRUE - Headset output on, SCI_FALSE - Headset output off.
//! \brief  Description:  This function is to get current headset port setting.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC BOOLEAN AUDMOD_GetHeadsetOutSetting(void);

/*****************************************************************************/
//! \fn PUBLIC BOOLEAN AUDMOD_GetPriPASetting(void)
//! \param void No parameter.
//! \return SCI_TRUE - PRI PA on, SCI_FALSE - PRI PA off.
//! \brief  Description:  This function is to get current PRI PA setting.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC BOOLEAN AUDMOD_GetPriPASetting(void);

/*****************************************************************************/
//! \fn PUBLIC BOOLEAN AUDMOD_GetAuxPASetting(void)
//! \param void No parameter.
//! \return SCI_TRUE - AUX PA on, SCI_FALSE - AUX PA off.
//! \brief  Description:  This function is to get current AUX PA setting.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC BOOLEAN AUDMOD_GetAuxPASetting(void);

/*****************************************************************************/
//! \fn PUBLIC BOOLEAN AUDMOD_GetHeadLPASetting(void)
//! \param void No parameter.
//! \return SCI_TRUE - Headset left channel PA on, SCI_FALSE - HS Left channel PA off.
//! \brief  Description:  This function is to get current headset left channel PA setting.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC BOOLEAN AUDMOD_GetHeadLPASetting(void);

/*****************************************************************************/
//! \fn PUBLIC BOOLEAN AUDMOD_GetHeadRPASetting(void)
//! \param void No parameter.
//! \return SCI_TRUE - Headset right channel PA on, SCI_FALSE - HS right channel PA off.
//! \brief  Description:  This function is to get current headset right channel PA setting.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC BOOLEAN AUDMOD_GetHeadRPASetting(void);

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDMOD_GetDADataPolaritySetting(void)
//! \param void No parameter.
//! \return DA polarity setting.
//! \brief  Description:  This function is to get current DA polarity setting.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC uint32 AUDMOD_GetDADataPolaritySetting(void);

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDMOD_GetADChannelSetting(void)
//! \param void No parameter.
//! \return AD Channel.
//! \brief  Description:  This function is to get current AD channel setting.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC uint32 AUDMOD_GetADChannelSetting(void);

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDMOD_GetInputModeSetting(void)
//! \param void No parameter.
//! \return AD Input mode. AUX or PRI
//! \brief  Description:  This function is to get current AD input mode setting.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC uint32 AUDMOD_GetInputModeSetting(void);

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDMOD_GetADPASetting(void)
//! \param void No parameter.
//! \return AD PA setting.
//! \brief  Description:  This function is to get current AD PA setting.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC uint32 AUDMOD_GetADPASetting(void);

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDMOD_GetADDataPolaritySetting(void)
//! \param void No parameter.
//! \return AD data polarity setting.
//! \brief  Description:  This function is to get current AD data polarity setting.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC uint32 AUDMOD_GetADDataPolaritySetting(void);

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDMOD_GetVolumeSetting(void)
//! \param void No parameter.
//! \return volume value.
//! \brief  Description:  This function is to get current volume setting.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC uint32 AUDMOD_GetVolumeSetting(void);

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDMOD_GetDLBasePGASetting(void)
//! \param void No parameter.
//! \return Down link base PGA value.
//! \brief  Description:  This function is to get current downlink base PGA value.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC uint32 AUDMOD_GetDLBasePGASetting(void);

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDMOD_GetULBasePGASetting(void)
//! \param void No parameter.
//! \return Up link base PGA value.
//! \brief  Description:  This function is to get current uplink base PGA value.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC uint32 AUDMOD_GetULBasePGASetting(void);

/*****************************************************************************/
//! \fn PUBLIC  AUDMOD_RESULT_E AUDMOD_GetModeParam(
//!    const uint8* pucModeName, 
//!    AUDMOD_DEVMOD_T* ptDeviceMode
//!    )
//! \param pucModeName Mode Name.
//! \param ptDeviceMode [OUT] Address which the mode parameter output.
//! \return Operation resules. \sa AUDMOD_RESULT_E
//! \brief  Description:  This function is to get the named device mode information.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC  AUDMOD_RESULT_E AUDMOD_GetModeParam(
    const uint8* pucModeName, 
    AUDMOD_DEVMOD_T* ptDeviceMode
    );

/*****************************************************************************/
//! \fn PUBLIC AUDMOD_RESULT_E AUDMOD_SetModeParam(
//!    const uint8* pucModeName, 
//!    AUDMOD_DEVMOD_T* ptDeviceMode
//!    )
//! \param pucModeName Mode Name.
//! \param ptDeviceMode [IN] Address which the mode parameter saved.
//! \return Operation resules. \sa AUDMOD_RESULT_E
//! \brief  Description:  This function is to set the named device mode information.
//! \author Author:  Benjamin.Wang
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC AUDMOD_RESULT_E AUDMOD_SetModeParam(
    const uint8* pucModeName, 
    AUDMOD_DEVMOD_T* ptDeviceMode
    );

//for dolphin voiceband.
/*****************************************************************************/
//! \fn PUBLIC BOOLEAN AUDMOD_GetOutputModeSetting_Dolphin(void)
//! \param void No parameter.
//! \return DA output port. SCI_TRUE - AUX, SCI_FALSE - PRI.
//! \brief  Description:  This function is to get current DA output port setting.
//! \author Author:  Johnson sun
//! \note   Note: used for dolphin voiceband. 
/*****************************************************************************/
PUBLIC BOOLEAN AUDMOD_GetOutputModeSetting_Dolphin(void);

/*****************************************************************************/
//! \fn PUBLIC BOOLEAN AUDMOD_GetHeadsetOutSetting_Dolphin(void)
//! \param void No parameter.
//! \return SCI_TRUE - Headset output on, SCI_FALSE - Headset output off.
//! \brief  Description:  This function is to get current headset port setting.
//! \author Author:  Johnson sun
//! \note   Note:Nothing.  
/*****************************************************************************/
PUBLIC BOOLEAN AUDMOD_GetHeadsetOutSetting_Dolphin(void);

/**---------------------------------------------------------------------------**
 **                         Compiler Flag                                     **
 **---------------------------------------------------------------------------**/ 
#ifdef __cplusplus
}
#endif

#endif  // _ADMM_H

// End of admm.h
