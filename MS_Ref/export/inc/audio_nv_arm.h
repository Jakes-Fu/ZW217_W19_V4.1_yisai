/******************************************************************************
 ** File Name:      audio_nv_arm.h
 ** Author:         shujing.dong
 ** DATE:           01/17/2011
 ** Copyright:      2011 Spreatrum, Incoporated. All Rights Reserved.
 ** Description:    This file defines the basic operation interfaces and data
 **                 structure of audio device mode manager controlled by arm.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History
 **  -----------------------------------------------------------------------
 ** DATE           NAME             DESCRIPTION
 ** 01/17/2011     shujing.dong     Create.
 ******************************************************************************/

/*! \file audio_nv_arm.h
*  \author Shujing.Dong
*  \date Jan 17, 2011
*  \brief This file defines the basic operation interfaces and data structure
*          of audio device mode manager controlled by arm.
*/

#ifndef _AUDIO_NV_ARM_H
#define _AUDIO_NV_ARM_H
/**---------------------------------------------------------------------------**
 **                         Dependencies                                      **
 **---------------------------------------------------------------------------**/
#include "sci_types.h"
#include "audio_api.h"
#include "admm_III.h"
#include "CVS_frame_recd.h"

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
#define AUDIO_NV_ARM_MODE_NAME_MAX_LEN (16)
#define AUDIO_ARM_VOLUME_LEVEL (31)
#define AUDIO_NV_ARM_APP_PARA_RESERVE (9)
#define AUDIO_NV_ARM_PARA_RESERVE (64)
/**---------------------------------------------------------------------------**
 **                         Data Structures                                   **
 **---------------------------------------------------------------------------**/
typedef enum
{
    AUDIO_DEVICE_MODE_PARAM_PLAY,
    AUDIO_DEVICE_MODE_PARAM_RECORD,
    AUDIO_DEVICE_MODE_PARAM_RECORD_LINEIN,
    AUDIO_DEVICE_MODE_PARA_RESERVE0,
    AUDIO_DEVICE_MODE_PARAM_RESERVE1,
    AUDIO_DEVICE_MODE_PARAM_RESERVE2,
    AUDIO_DEVICE_MODE_PARAM_RESERVE3,
    AUDIO_DEVICE_MODE_PARAM_RESERVE4,
    AUDIO_DEVICE_MODE_PARAM_MAX
}AUDIO_DEVICE_MODE_PARAM_TYPE_E;

typedef enum
{
    AUDIO_AGC_INPUG_GAIN_MP3,   //agc type mp3
    AUDIO_AGC_INPUG_GAIN_MIDI,  //agc type midi
    AUDIO_AGC_INPUG_GAIN_AMR,   //agc type amr
    AUDIO_AGC_INPUG_GAIN_FM,
    AUDIO_AGC_INPUG_GAIN_RESERVE0,
    AUDIO_AGC_INPUG_GAIN_RESERVE1,
    AUDIO_AGC_INPUG_GAIN_RESERVE2,
    AUDIO_AGC_INPUG_GAIN_RESERVE3,
    AUDIO_AGC_INPUG_GAIN_MAX
}AUDIO_AGC_INPUG_GAIN_TYPE_E;

typedef enum
{
    AUDIO_NV_ARM_NO_ERROR = 0x0,
    AUDIO_NV_ARM_ALLOC_ERROR = BIT_0, /*<!alloc error.*/
    AUDIO_NV_ARM_INPUT_PARA_ERROR = BIT_1, /*<!input parameter error.*/
    AUDIO_NV_ARM_MODE_NO_EXIST = BIT_3,  /*<!the mode with this name is not exist.*/

    AUDIO_NV_ARM_ERROR
}AUDIO_NV_ARM_RESULT_E;

typedef enum
{
        NV_AUDIO_ARM_HANDSET_E,
        NV_AUDIO_ARM_HANDSFREE_E,
        NV_AUDIO_ARM_HEADSET_E,
        NV_AUDIO_ARM_EARFREE_E,
        NV_AUDIO_ARM_BTHS_E,
        NV_AUDIO_ARM_MAX
}NV_AUDIO_ARM_E_;
typedef uint32 NV_AUDIO_ARM_E;

typedef struct Audio_Nv_Arm_Device_Path_Struct
{
	uint16 valid_dev_set_count;
	//AUDMOD_DEVICECTL_T bit0-bit7:
	//ucADChannel&0x1,
	//ucDAChannel&0x1,
	//ucDA0Reverse&0x1,
	//ucDA1Reverse&0x1,
	//ucStereoEn&0x1,
	//ucSpkAmpEn&0x1,
	//ucSpkAmpEn&0x1,
	//ucMixerInput&0x1
	uint16 reserve;//shujing add for align
	uint16 dev_set[AUDIO_DEVICE_MODE_PARAM_MAX];
}AUDIO_NV_ARM_DEVICE_PATH_T;

typedef struct Audio_Nv_Arm_App_Config_Info_Struct
{
	uint16 eq_switch;
	uint16 agc_input_gain[AUDIO_AGC_INPUG_GAIN_MAX];
	uint16 valid_volume_level_count;
	uint32 arm_volume[AUDIO_ARM_VOLUME_LEVEL];
	uint16 reserve[AUDIO_NV_ARM_APP_PARA_RESERVE];
}AUDIO_NV_ARM_APP_CONFIG_INFO_T;//include eq_switch/agc_input_gain/arm_volume_table

typedef struct Audio_Nv_Arm_App_Set_Struct
{
	uint16 valid_app_set_count;
	uint16 valid_agc_input_gain_count;
	uint16 aud_proc_exp_control[2];//switch of agc/lcf/eq_select--original dsp audio nv:extend2[110]
	AUDIO_NV_ARM_APP_CONFIG_INFO_T app_config_info[AUDIO_ARM_APP_TYPE_MAX];
}AUDIO_NV_ARM_APP_SET_T;

typedef struct Audio_Nv_Arm_Mode_Struct
{
	AUDIO_NV_ARM_DEVICE_PATH_T dev_path_set;
	AUDIO_NV_ARM_APP_SET_T app_config_info_set;
	uint16 midi_opt;
    uint16 aud_dev;
    uint16 reserve[AUDIO_NV_ARM_PARA_RESERVE];
    CVSI_CTRL_PARAM_recd cvs_ctrl_param_rcd;
}AUDIO_NV_ARM_MODE_STRUCT_T;

typedef struct Audio_Nv_Arm_Mode_Info_struct
{
    uint8 ucModeName[AUDIO_NV_ARM_MODE_NAME_MAX_LEN];	//node name.
    AUDIO_NV_ARM_MODE_STRUCT_T tAudioNvArmModeStruct;	// Audio structure
}AUDIO_NV_ARM_MODE_INFO_T;
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
//! \fn PUBLIC AUDIO_NV_ARM_RESULT_E AUDIONVARM_InitModeManager(void)
//! \param void No Parameter
//! \return Operation results. \sa AUDIO_NV_ARM_RESULT_E
//! \brief  Description:  This function is to init Mode ARM Manager.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.
/*****************************************************************************/
PUBLIC AUDIO_NV_ARM_RESULT_E AUDIONVARM_InitModeManager(void);

/*****************************************************************************/
//! \fn PUBLIC uint32 AUDIONVARM_GetModeNumber(void)
//! \param void No parameter.
//! \return The total number of registered modes.
//! \brief  Description:  This function is to get the number of mode in the
//!                       current mode manager.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.
/*****************************************************************************/
PUBLIC uint32 AUDIONVARM_GetModeNumber(void);

/*****************************************************************************/
//! \fn PUBLIC AUDIO_NV_ARM_RESULT_E AUDIONVARM_AddMode(
//!    AUDIO_NV_ARM_MODE_INFO_T* ptMode
//!    )
//! \param ptMode Added mode.
//! \return Operation results. \sa AUDIO_NV_ARM_RESULT_E
//! \brief  Description:  This function is to add one mode into mode manager.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.
/*****************************************************************************/
PUBLIC AUDIO_NV_ARM_RESULT_E AUDIONVARM_AddMode(
    AUDIO_NV_ARM_MODE_INFO_T* ptMode
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_NV_ARM_RESULT_E AUDIONVARM_RemoveMode(
//!    const char* pucModeName
//!    )
//! \param pucModeName mode name which is needed to remove from this mode manager.
//! \return Operation results. \sa AUDIO_NV_ARM_RESULT_E
//! \brief  Description:  This function is to remove one mode from mode manager.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.
/*****************************************************************************/
PUBLIC AUDIO_NV_ARM_RESULT_E AUDIONVARM_RemoveMode(
    const char* pucModeName
    );

/*****************************************************************************/
//! \fn PUBLIC  AUDIO_NV_ARM_RESULT_E AUDIONVARM_GetModeParam(
//!    const char* pucModeName,
//!    AUDIO_NV_ARM_MODE_INFO_T* ptMode
//!    )
//! \param pucModeName Mode Name.
//! \param ptMode [OUT] Address which the mode parameter output.
//! \return Operation resules. \sa AUDIO_NV_ARM_RESULT_E
//! \brief  Description:  This function is to get the named mode information
//!                       from linked list managed by Device Mode Manager.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.
/*****************************************************************************/
PUBLIC  AUDIO_NV_ARM_RESULT_E AUDIONVARM_GetModeParam(
    const char* pucModeName,
    AUDIO_NV_ARM_MODE_INFO_T* ptMode
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_NV_ARM_RESULT_E AUDIONVARM_SetModeParam(
//!    const char* pucModeName,
//!    AUDIO_NV_ARM_MODE_INFO_T* ptMode
//!    )
//! \param pucModeName Mode Name.
//! \param ptMode [IN] Address which the new mode parameter saved.
//! \return Operation resules. \sa AUDIO_NV_ARM_RESULT_E
//! \brief  Description: This function is used to set mode parameters to the
//! \          appointed device mode managed by Mode Manager.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.
/*****************************************************************************/
PUBLIC AUDIO_NV_ARM_RESULT_E AUDIONVARM_SetModeParam(
    const char* pucModeName,
    AUDIO_NV_ARM_MODE_INFO_T* ptMode
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_NV_ARM_RESULT_E AUDIONVARM_GetAudioModeName(
//!    uint32 index
//!    )
//! \param index  arm nv index.
//! \return mode name. \sa char*
//! \brief  Description:  This function is to get the mode name by index of arm nv list in ram.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.
/*****************************************************************************/
PUBLIC char *AUDIONVARM_GetAudioModeName (
	uint32 index
	);

/*****************************************************************************/
//! \fn PUBLIC AUDIO_NV_ARM_RESULT_E AUDIONVARM_GetAudioModeNameById(
//!    NV_AUDIO_ARM_E arm_nv_id
//!    )
//! \param arm_nv_id arm nv id.
//! \return mode name. \sa char*
//! \brief  Description:  This function is to get the mode name by arm nv id.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.
/*****************************************************************************/
PUBLIC char *AUDIONVARM_GetAudioModeNameById (
	NV_AUDIO_ARM_E arm_nv_id
	);

/*****************************************************************************/
//! \fn PUBLIC AUDIO_DEVICE_MODE_TYPE_E AUDIONVARM_GetAudioModeType(
//!    char *mode_name_ptr
//!    )
//! \param mode name. \sa char*
//! \return nv_audio_e Mode type.
//! \brief  Description:  This function is to get the mode type by mode name.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.
/*****************************************************************************/
PUBLIC AUDIO_DEVICE_MODE_TYPE_E AUDIONVARM_GetAudioModeType(
	char *mode_name_ptr
	);

/*****************************************************************************/
//! \fn PUBLIC  AUDIO_NV_ARM_RESULT_E AUDIONVARM_GetAudioModeDevCtrInfo(
//!    const char* pucModeName,
//!    AUDIO_DEVICE_MODE_PARAM_TYPE_E eModeParaType,
//!    AUDMOD_DEVICECTL_T *ptDevCtrInfo
//!    )
//! \param pucModeName mode name, its max length is not more than 16 bytes..
//! \param eModeParaType Mode para type.
//! \param ptDevCtrInfo [OUT] Address to save the device basic control info
//!                        in the named mode.
//! \return Operation resules. \sa AUDIO_NV_ARM_RESULT_E
//! \brief  Description:  This function is to get the device control info from
//!                       the named mode information.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.
/*****************************************************************************/
PUBLIC AUDIO_NV_ARM_RESULT_E AUDIONVARM_GetAudioModeDevCtrInfo (
	const char* pucModeName,
	AUDIO_DEVICE_MODE_PARAM_TYPE_E eModeParaType,
	AUDMOD_DEVICECTL_T *ptDevCtrInfo
	);

/*****************************************************************************/
//! \fn PUBLIC  AUDIO_NV_ARM_RESULT_E AUDIONVARM_ReadModeParamFromFlash(
//!    const char* pucModeName,
//!    AUDIO_NV_ARM_MODE_INFO_T* ptMode
//!    )
//! \param pucModeName device mode name, its max length is not more than 16 bytes.
//! \param ptMode [OUT] Address which the mode parameter output.
//! \return Operation resules. \sa AUDIO_NV_ARM_RESULT_E
//! \brief  Description:  This function is to get the named mode information
//!                       from flash.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.
/*****************************************************************************/
PUBLIC  AUDIO_NV_ARM_RESULT_E AUDIONVARM_ReadModeParamFromFlash(
    const char* pucModeName,
    AUDIO_NV_ARM_MODE_INFO_T* ptMode
    );

/*****************************************************************************/
//! \fn PUBLIC AUDIO_NV_ARM_RESULT_E AUDIONVARM_WriteModeParamToFlash(
//!    const char* pucModeName,
//!    AUDIO_NV_ARM_MODE_INFO_T* ptMode
//!    )
//! \param pucModeName mode name, its max length is not more than 16 bytes.
//! \param ptMode [IN] Address which the new mode parameter saved.
//! \return Operation resules. \sa AUDIO_NV_ARM_RESULT_E
//! \brief  Description:  This function is to set the named mode information
//!                       to flash.
//! \author Author:  Shujing.Dong
//! \note   Note:Nothing.
/*****************************************************************************/
PUBLIC AUDIO_NV_ARM_RESULT_E AUDIONVARM_WriteModeParamToFlash(
	const char* pucModeName,
    AUDIO_NV_ARM_MODE_INFO_T* ptMode
    );
PUBLIC char * _ARMVB_ArmvbNv_GetName(AUDIO_DEVICE_MODE_TYPE_E aud_dev_mode);
/**---------------------------------------------------------------------------**
 **                         Compiler Flag                                     **
 **---------------------------------------------------------------------------**/
#ifdef __cplusplus
}
#endif

#endif  // _ADMM_H

// End of admm.h

