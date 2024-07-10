/****************************************************************************************
** File Name:       layer1_aac.h                                                        *   
** Author:          Yu Guang Yao                                                        *
** DATE:            01/30/2007                                                          *
** CopyRight        2002 SpreadTrum Communications CORP. All Rights Reserved.           *
** Description:     This header file is to handle all operations for Interface between  *
**                  MCU and DSP on 2k Shared Dual-Port Memory.                          *       
**                  More detailed Info. could be seen in Doc.MCU and DSP interface Spec *
*****************************************************************************************
**                  Edit History                                                        *   
** -------------------------------------------------------------------------------------*
**                                                                                      *
** DATE:            NAME                DESCIPTION                                      *
** 01/30/2007       Yu Guang Yao        Create.                                         *
*****************************************************************************************/
#ifndef _LAYER1_AUDIO_CODEC_H
#define _LAYER1_AUDIO_CODEC_H

#include "sci_types.h"
#include "layer1_audio.h"
/**-------------------------------------------------------------------------------------*
 **                         Dependencies                                                *
 **-------------------------------------------------------------------------------------*/
typedef void (*DSP_CODEC_IND_CALLBACK_PFUNC)(
    uint16 parameter0,
	uint16 parameter1,
	uint16 parameter2,
	uint16 parameter3
);

typedef struct dsp_codec_src_data_struct
{
    Audio_Sub_Cmd_E e_sub_command;
    uint8* uc_data_src_ptr;   //pointer of input data
    uint32 ui_data_len;   //length of input data unit:byte
    uint32 ui_arm_dest_address;//dest address input data put, if it is zero, layer1 will use the default address
    uint16 us_dsp_src_address;
    uint16 us_dsp_dest_address;
    DSP_CODEC_IND_CALLBACK_PFUNC callback_func_ptr;//pointer of callback which 
                  //is used to inform codec the message that decode header is over
}DSP_CODEC_SRC_DATA_T;

typedef struct dsp_codec_data_struct
{
    uint16 * us_data_dest_address;
    uint32 ui_data_src_address;
    uint32 ui_data_length; //unit:bytes
}DSP_CODEC_DATA_T;

typedef struct dsp_codec_data_lr_struct
{
    uint16 * us_data_dest_address_l;
    uint16 * us_data_dest_address_r;    
    uint32 ui_data_src_address;
    uint32 ui_sample_count; 
}DSP_CODEC_DATA_LR_T;

//define the address map between arm and dsp
typedef struct
{
    uint32 ui_arm_address_base;
    uint16 us_dsp_address_base;
    uint16 ui_arm_buffer_len;//bytes.
}AUDIO_CODEC_ARM_DSP_ADDRESS_MAP_T;

/*****************************************************************************/
// 	Description : copy frame data for dsp access and send command to dsp to 
//                init or encode/decode.
//	Global resource dependence : none
//  Author:        shujing.dong
//	Note:    
/*****************************************************************************/
PUBLIC void LAYER1_AUDIO_CODEC_Data(//return none
    DSP_CODEC_SRC_DATA_T * data_info
    );

/*****************************************************************************/
// 	Description : send command to dsp to stop decoding
//	Global resource dependence : none
//  Author:        shujing.dong
//	Note:    
/*****************************************************************************/
PUBLIC void LAYER1_AUDIO_CODEC_Brake(
    Audio_Sub_Cmd_E e_sub_command
    );

/*****************************************************************************/
// 	Description : change the controlment of the memory and copy data to dest 
//                address. The data maybe is from arm to dsp or is from dsp to arm.
//                And the data is not only maybe src data need to process but also
//                the info of src data.
//	Global resource dependence : none
//  Author:        shujing.dong
//	Note:    
/*****************************************************************************/
PUBLIC void LAYER1_AUDIO_CODEC_CopyData(
    DSP_CODEC_DATA_T *data_ptr
    );
    
/*****************************************************************************/
// 	Description : change the controlment of the memory and copy data to dest address
//                according to the data placement way:LRLRLR... 
//	Global resource dependence : none
//  Author:        shujing.dong
//	Note:    
/*****************************************************************************/
PUBLIC void LAYER1_AUDIO_CODEC_CopyLRData(
    DSP_CODEC_DATA_LR_T *data_ptr
    );
    
/*****************************************************************************/
// 	Description : get base address of arm and dsp. 
//	Global resource dependence : none
//  Author:        shujing.dong
//	Note:    
/*****************************************************************************/
PUBLIC void LAYER1_AUDIO_CODEC_GetAddressBase(
    AUDIO_CODEC_ARM_DSP_ADDRESS_MAP_T * const address_base_ptr
    );
    
#endif

