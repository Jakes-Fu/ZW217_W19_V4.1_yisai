/****************************************************************************************
** File Name:       layer1_aac.h                                                        *   
** Author:          Yu Guang Yao                                                        *
** DATE:            01/30/2007                                                          *
** CopyRight        2002 SpreadTrum Communications CORP. All Rights Reserved.           *
** Description:     This header file is to handle aac operations for Interface between  *
**                  MCU and DSP on 2k Shared Dual-Port Memory.                          *       
**                                                                                      *
*****************************************************************************************
**                  Edit History                                                        *   
** -------------------------------------------------------------------------------------*
**                                                                                      *
** DATE:            NAME                DESCIPTION                                      *
** 01/30/2007       Yu Guang Yao        Create.                                         *
*****************************************************************************************/

#ifndef _LAYER1_SBC_H
#define _LAYER1_SBC_H


/**-------------------------------------------------------------------------------------*
 **                         Dependencies                                                *
 **-------------------------------------------------------------------------------------*/
#include "sci_types.h"
 
 
/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/ 

typedef void (*SBC_CONFIG_ENCODER_IND_CALLBACK_PFUNC)(
  uint16    frame_size
  );

typedef void (*SBC_ENCODE_FRAME_IND_CALLBACK_PFUNC)(
  uint8* frame_addr,
  uint16 frame_len,
  uint16 frame_count, 
  uint16 result
  );

void L1_SBC_Init(void);

void L1_SBC_Uninit(void);

BOOLEAN L1_SBC_Config_Encoder  (uint8  channel_mode, 
                             uint8  alloc_method,
                             uint16 sample_freq,
                             uint8  nrof_blocks, 
                             uint8  nrof_subbands, 
                             uint8  bitpool,
                             SBC_CONFIG_ENCODER_IND_CALLBACK_PFUNC callback_func_ptr);
                             

BOOLEAN L1_SBC_Encode_Frame (int16*  ps_left_pcm,
                          int16*  ps_right_pcm,
                          uint16  us_pcm_len,
                          uint8*  puc_frame,
                          uint16  us_frame_len,
                          uint8   bitpool,
                          uint16   us_frame_size,
                          SBC_ENCODE_FRAME_IND_CALLBACK_PFUNC callback_func_ptr);

PUBLIC void LAYER1_TRANPCM_Config(uint16 samplerate);
                          
#if defined(SBC_UNIT_TEST)                          

    void L1_SBC_Test(void);
    
#endif


/**---------------------------------------------------------------------------*
 **                         Overlay Feature Support                           *
 **---------------------------------------------------------------------------*/

//#define SBC_OVERLAY_SUPPORT    1
//#if defined(SBC_OVERLAY_SUPPORT)

/**---------------------------------------------------------------------------*
 **                         Macro definition                                  *
 **---------------------------------------------------------------------------*/

// paramter for Overlay_Load_Code
#define      SBC_CODE               1
#define      AUDIO_CODE             2
#define	  GPS_CODE			3


// constant for OVERLAY_CONTEXT_T
#define      SBC_IN_MEMORY          1
#define      AUDIO_IN_MEMORY        2
#define 	   GPS_IN_MEMORY		3

#define      AUDIO_USED             1
#define      AUDIO_UNUSED           2

// sbc encode time with ms unit
#define      SBC_ENCODE_TIME_MS     6

// ARM DSP Switch Memory Address
#define      ARM_OVERLAY_ADDR       (uint16 *) 0x40010000

/**---------------------------------------------------------------------------*
 **                         Type  definition                                  *
 **---------------------------------------------------------------------------*/

typedef struct {
    uint16             mem_state;
    uint16             audio_usage;
    uint16*            overlay_code_base_ptr;
    uint16             overlay_mutex;                // 0: some one can used the overlay region 
} OVERLAY_CONTEXT_T;



/**---------------------------------------------------------------------------*
 **                         Function Prototypes                               *
 **---------------------------------------------------------------------------*/ 
#if defined (OVERLAY_GLOBAL)
    #define OVERLAY_EXTERN
#else
    #define OVERLAY_EXTERN extern
#endif

OVERLAY_EXTERN OVERLAY_CONTEXT_T overlay_context;

void Overlay_Load_Code(uint8 t_code_partition );

//#endif // SBC_OVERLAY_SUPPORT


#endif
