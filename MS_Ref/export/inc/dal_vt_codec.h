/******************************************************************************
 ** File Name:      dal_vt_codec.h 
 ** DATE:           12/18/2009
 ** Copyright:      2009 Spreatrum, Incoporated. All Rights Reserved.
 ** Description:    This file defines the marcro, struct, enums and audio/video 
 **                  codec stream API supplied by VT middleware for VT application.
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       
 **  -----------------------------------------------------------------------  
 ** DATE                 DESCRIPTION 
 ** 12/18/2009           Create.
 ******************************************************************************/
 #ifndef _DAL_VT_CODEC_H_
#define _DAL_VT_CODEC_H_


/**---------------------------------------------------------------------------*
 **                         Dependencies           			                  *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif


/**---------------------------------------------------------------------------*
 **                         Macro Declaration                                 *
 **---------------------------------------------------------------------------*/
// TODO : define macor below.


/**---------------------------------------------------------------------------*
 **                      Enum and Structure Declaration                       *
 **---------------------------------------------------------------------------*/
// return type of VT codec.
typedef enum
{
    DVT_CODEC_RET_SUCCESS       = 0,
    DVT_CODEC_RET_NOT_SUPPORTED,
    DVT_CODEC_RET_MEMORY_ERR,
    DVT_CODEC_RET_HARDWARE_ERR,
    DVT_CODEC_RET_ENC_BUF_FULL,
    DVT_CODEC_RET_DEC_BUF_FULL,
    DVT_CODEC_RET_ENC_ERR,
    DVT_CODEC_RET_DEC_ERR,
    DVT_CODEC_RET_INVALID_PARAM,
    DVT_CODEC_RET_INVALID_STATE,
    DVT_CODEC_RET_ERR,
    DVT_CODEC_RET_MAX
}DVT_CODEC_RET_E;

// error type of audio frame data.
typedef enum
{
    DVT_CODEC_A_DATA_GOOD,
    DVT_CODEC_A_DATA_BAD,
    DVT_CODEC_A_DATA_NONE,
    DVT_CODEC_A_DATA_ERR_MAX
}DVT_CODEC_A_DATA_ERR_TYPE_E;


// video codec type.
typedef enum
{
    DVT_CODEC_V_MPEG4   = 0, 
    DVT_CODEC_V_H263    = 1, 
    DVT_CODEC_V_TYPE_MAX
}DVT_CODEC_V_CODEC_E;


typedef enum
{
    DVT_CODEC_V_PROFILE_LEVEL_NONE  =0,
    // for mepg4.
    DVT_CODEC_MPEG4_SIMPLE_LEVEL1   = 1,    // QCIF.64kbps.15fps.
    DVT_CODEC_MPEG4_SIMPLE_LEVEL2   = 2,    // CIF.128kbps.15fps.
    DVT_CODEC_MPEG4_SIMPLE_LEVEL3   = 3,    // CIF.384kbps.30fps.
    //
    DVT_CODEC_MPEG4_MAIN_LEVEL2     = 10,    // CIF.2Mbps.60fps.
    DVT_CODEC_MPEG4_MAIN_LEVEL3     = 11,    // 720x576.15Mbps.30fps.

    // for h.263.
    DVT_CODEC_H263_BASELINE_LEVEL30 = 20   // CIF.384kbps.30fps.
}DVT_CODEC_V_PROFILE_LEVEL_E;


typedef enum
{
    DVT_CODEC_V_PP_NONE,
    DVT_CODEC_V_PP_LEVEL1,
    DVT_CODEC_V_PP_LEVEL2,
    DVT_CODEC_V_PP_LEVEL3,
    DVT_CODEC_V_PP_LEVEL4,
    DVT_CODEC_V_PP_MAX
}DVT_CODEC_V_POST_PROCESS_E;


typedef struct
{
    int32 width;
    int32 height;
}DVT_CODEC_V_SIZE_T;


typedef struct
{
    int32 rate_control_en;
    int32 target_bitrate;
    int32 default_quant;    // [0,31]
}DVT_CODEC_V_RATECONTROL_T;


typedef struct{
    int32 video_packet_en;
    int32 data_patrion_en;
    int32 rvlc_en;
}DVT_CODEC_V_ENC_ERR_TOOL_T;

typedef struct
{
    int32 ac_prediction_en;
    int32 rounding_ctrl_en;
}DVT_CODEC_V_ENC_COMPRESS_TOOL_T;

typedef struct {
    int32 intra_cycle_en;
    int32 intra_cycle;
}DVT_CODEC_V_ENC_INTRA_CYCLE_T;


typedef struct
{
    DVT_CODEC_V_CODEC_E                 codec_type;
    DVT_CODEC_V_PROFILE_LEVEL_E         porfile_level;

    DVT_CODEC_V_SIZE_T                  size;
    DVT_CODEC_V_RATECONTROL_T           rate_ctrl;
    DVT_CODEC_V_ENC_ERR_TOOL_T          error_tool;
    DVT_CODEC_V_ENC_COMPRESS_TOOL_T     compress_tool;
    DVT_CODEC_V_ENC_INTRA_CYCLE_T       intra_cycle;
}DVT_CODEC_V_ENC_OPTION_T;


typedef struct
{
    DVT_CODEC_V_CODEC_E         codec_type;
    DVT_CODEC_V_SIZE_T          raw_size;
    DVT_CODEC_V_POST_PROCESS_E  pp_level;
}DVT_CODEC_V_DEC_OPTION_T;


typedef struct
{
    int32 err_pos_off;  // start postion(offset) of the error section in the stream.
    int32 err_len;      // len of the error section.
}DVT_CODEC_V_STREAM_ERR_T;

typedef struct
{
    uint8                       *puc_packet;
    int32                       i_len;
    int32                       err_pos_count;
    DVT_CODEC_V_STREAM_ERR_T    err_pos_group[3];
}DVT_CODEC_V_DEC_DATA_T;





/**---------------------------------------------------------------------------*
 **                       API Functions Declaration                           *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : intit the video encoder.
//	input       : none
//  output      : DVT_CODEC_RET_E.
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_VideoEncInit(void);

/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_VideoEncRelease(void);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_GetVideoEncAbility(
                                            int32 *p_enc_group_size,
                                            DVT_CODEC_V_ENC_OPTION_T **pp_enc_group
                                            ) ;



/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_GetVideoEncCfg(
                                        DVT_CODEC_V_ENC_OPTION_T *p_enc_cfg);



/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_SetVideoEncCfg(
                                    DVT_CODEC_V_ENC_OPTION_T * p_enc_cfg);





/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_GetVideoEncHeader(
                                                uint32 *p_header, 
                                                uint32 *p_len
                                                );



/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_VideoEncHostDil(
                                                    BOOLEAN is_force_intra,
                                                    uint32  timestamp,
                                                    uint8   *p_packet,
                                                    int32   *p_packet_len,
                                                    BOOLEAN *p_is_frame_enc
                                                    );



/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_VideoDecInit(void);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_VideoDecRelease(void);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_GetVideoDecAbility(
                                        int32 *p_dec_group_size,
                                        DVT_CODEC_V_DEC_OPTION_T **pp_dec_group
                                        ) ;



/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_GetVideoDecConf(
                                        DVT_CODEC_V_DEC_OPTION_T *p_dec_cfg
                                        );


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_SetVideoDecConf(
                                            DVT_CODEC_V_DEC_OPTION_T *p_dec_cfg
                                            ) ;



/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_VideoDecHostDil(
                                                DVT_CODEC_V_DEC_DATA_T *p_data,
                                                BOOLEAN *p_is_frame_decoded,
                                                BOOLEAN *p_is_intra_frmae,
                                                uint32  *p_err_MB_num
                                               );



/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_AmrEncOpen(int i_amr_mode);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_AmrEncClose(void);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_AmrEncSetMode(int i_amr_mode);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_AmrEncFlush(void);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_AmrEncStartCapture (void);

/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC int DVT_CODEC_AmrEncReadAFrame(
                                        uint8* p_buf,
                                        int i_size);



/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_AmrDecOpen(void);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC DVT_CODEC_RET_E DVT_CODEC_AmrDecClose(void);


/*****************************************************************************/
// 	Description :
//	input       :
//  output      :
//	Note        :
//  History     :
/*****************************************************************************/
PUBLIC int DVT_CODEC_AmrDecWriteAFrame(
                                        uint8 *p_data,
                                        int len,
                                        DVT_CODEC_A_DATA_ERR_TYPE_E err_type
                                        );

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif /*_DAL_VT_CODEC_H_*/

