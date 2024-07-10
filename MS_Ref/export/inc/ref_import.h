/******************************************************************************
 ** File Name:      ref_import.h                                                 *
 ** Author:         hanjun.liu                                              *
 ** DATE:           01/29/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE           NAME             DESCRIPTION                               *
 ** 01/29/2004     hanjun.liu     Create.                                   *
 ******************************************************************************/
#ifndef _REF_IMPORT_H
#define _REF_IMPORT_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*******************************************************************/
//	Function name   REF_GetSizeOfClassmark
// 	Discription     Get the size of struct PS_CLASSMARK_T,add it for 
//                  build ref lib.
//  Author:  hanjun.liu
//	Note:           
/*******************************************************************/
PUBLIC uint32 REF_GetSizeOfClassmark(void);

/*******************************************************************/
//	Function name:   REF_GetDspParaSize
// 	Discription:  This function returns size of system_param_dsp_use_T 
//                  build ref lib.
//  Author:  hanjun.liu
//	Note:           
/*******************************************************************/
PUBLIC uint32 REF_GetDspParaSize( void );

/*******************************************************************/
//	Function name:   REF_GetDspParaSize
// 	Discription:  This function returns size of system_param_dsp_use_T 
//                  build ref lib.
//  Author:  hanjun.liu
//	Note:           
/*******************************************************************/
PUBLIC uint32 REF_GetCalibrationParaSize( void );

/*******************************************************************/
//	Function name:   REF_CheckLayer1State
// 	Discription: Return a flag to indication if g_layer1_state ==
//	             L1_STATE_NULL.
//  Author:  hanjun.liu
//	Note:           
/*******************************************************************/
PUBLIC uint8 REF_CheckLayer1State(void);

/********************************************************************
//    DESCRIPTION:Check if g_sim_card_state is SIM_INITIALIZE_STATE or
//                SIM_ACTIVE_STATE.
//    Global resource dependence :
//    Author:hanjun.liu
//    Note:
***********************************************************************/
PUBLIC uint8 REF_CheckSimState(void);

/*****************************************************************************/
//  Description:    Repacket GPRS message which includes pointer., the 
//                  destination packet saved in dest_ptr. If the message with 
//                  no pointer, don't handle it, dest_ptr is still NULL.
//	Global resource dependence: 
//  Author: hanjun.liu
//	Note:
/*****************************************************************************/
PUBLIC void REF_PktGprsMsg(         // None
    uint8     **dest_ptr,       // Destination packet, save packed message.
    uint16    msg_type,         // Message type. Low 8 bit is struct type
    const uint8     *src_ptr,   // Source message
    uint16    *len              // Length of src_ptr when input,
                                // length of dest_ptr when output,
    );
/*******************************************************************/
//	Function name:   REF_CheckLayer1State
// 	Discription: This function returns on/off value for disable/enable the ARM trace.
//  Author:  hanjun.liu
//	Note:           
/*******************************************************************/
PUBLIC BOOLEAN REF_GetARMLogSwitch( void );

/*******************************************************************/
//	Function name:   REF_ShowDspVersion
// 	Discription: This function returns on/off value for disable/enable the ARM trace.
//  Author:  hanjun.liu
//	Note:           
/*******************************************************************/
void REF_ShowDspVersion( void );

/*******************************************************************/
//	Function name:   REF_GetDownloadParamVersion
// 	Discription: Return downloadparam nv version.
//  Author: fancier.fan
//	Note:           
/*******************************************************************/
uint16 REF_GetDownloadParamVersion(void);

/*******************************************************************/
//	Function name:   REF_GetCalibrationParamVersion
// 	Discription: Return calibration nv version.
//  Author: fancier.fan
//	Note:           
/*******************************************************************/
uint16 REF_GetCalibrationParamVersion(void);

/*****************************************************************************/
//  Description:    Send a request to set ARM clock.
//  Author:         hanjun.liu
//	Note:           
/*****************************************************************************/
void REF_SendSetArmClkReq(uint32 clk);

/*******************************************************************/
//	Function name:   REF_ShowDspVersion
// 	Discription: Get the adc calibration from the nvitem
//  Author:  hanjun.liu
//	Note:           
/*******************************************************************/
void REF_GetAdcCalibrationPara( 
							   uint32 battery_calibration[2],
							   uint32 program_calibration[2],
							   uint32 invalid_calibration,
							   uint32 chr_pm_ver_support
							   );


/**----------------------------- ----------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /*End _REF_IMPORT_H*/

