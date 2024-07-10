/*******************************************************************************
 ** FileName:    img_proc.h
 ** Author:        Tim.zhu
 ** Date:          2011/12/26
 ** CopyRight:   2011, Spreatrum, Incoporated, All right reserved
 ** Description:
 *******************************************************************************

 *******************************************************************************
 **                        Edit History
 ** -------------------------------------------------------------------------- *
 ** DATE              NAME                   DESCRIPTION
 ** 2011/12/26     Tim.zhu                 Created
 *******************************************************************************/

#ifndef IMG_PROC_H
#define IMG_PROC_H
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_api.h"
#include "sci_types.h"
#include "img_ref_types.h"

/**---------------------------------------------------------------------------*
 **                         Debugging Flag                                    *
 **---------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef   __cplusplus
extern   "C" 
{
#endif

/**---------------------------------------------------------------------------*
 **                         Macro define                                      *
 **---------------------------------------------------------------------------*/
 // IMG_PROC_PROCESS_T proc_mode
#define IMG_PROC_PROCESS_EB (0x01<<0)
#define IMG_PROC_VIDW_BEFORE0_EB (0x01<<1)
#define IMG_PROC_VIDW_BEFORE1_EB (0x01<<2)
#define IMG_PROC_VIDW_AFTER_EB (0x01<<3)
#define IMG_PROC_HISTORY_EB (0x01<<4)
#define IMG_PROC_FIRST (0x01<<5)
#define IMG_PROC_END (0x01<<6)

#define IMG_PROC_UPDATE_ALG (0x01<<7)
#define IMG_PROC_UPDATE_SRC (0x01<<8)
#define IMG_PROC_UPDATE_DST (0x01<<9)
#define IMG_PROC_RTN_NOTICE (0x01<<10)

#define IMG_PROC_VIDW_BEFORE_NUM 0x02
#define IMG_PROC_ZERO 0x00
#define IMG_PROC_ONE 0x01

#define IMGPROC_TRACE(_x_)   SCI_TraceLow _x_
#define IMG_RETURN_IF_FAIL(exp, warning) do{if(exp) {IMGPROC_TRACE(warning); return exp;}}while(0)
#define IMG_TRACE_IF_FAIL(exp, warning) do{if(exp) {IMGPROC_TRACE(warning);}}while(0)

typedef uint32 IMGPROC_HANDLE;
typedef uint32 (*imgproc_callback)(void* param_ptr);

typedef enum
{
    IMG_PROC_SUCCESS=0x00,
    IMG_PROC_PARAM_ERROR,
    IMG_PROC_MEMORY_NOT_ENOUGH,
    IMG_PROC_T_FLASH_NOT_ENOUGH,
    IMG_PROC_OPERATE_ERROR,
    IMG_PROC_ALLOCA_ERROR,
    IMG_PROC_FREE_ERROR,
    IMG_PROC_NO_HANDLE_ERROR,
    IMG_PROC_FORMAT_ERROR,
    IMG_PROC_REG_FUN_NULL_ERROR,
    IMG_PROC_PARAM_NULL_ERROR,
    IMG_PROC_ERROR,
    IMG_PROC_RETURN_MAX=0xff
}IMG_PROC_RETURN_VALUE_E;

typedef enum
{
    IMG_PROC_RGB565=0x0001,
    IMG_PROC_RGB888=0x0002,
    IMG_PROC_ARGB888=0x0004,
    IMG_PROC_ARGB565=0x0008,
    IMG_PROC_ARGB555=0x0010,
    IMG_PROC_COMPRESSED_ARGB=0x0020,
    IMG_PROC_JPG=0x0040,
    IMG_PROC_YUV422=0x0080,
    IMG_PROC_YUV420=0x0100,
    IMG_PROC_YUV400=0x0200,
    IMG_PROC_YUV422_3FRAME=0x0400,
    IMG_PROC_YUV420_3FRAME=0x0800,
    IMG_PROC_FORMAT_MAX=0x0fffffff
}IMG_PROC_DATA_FORMAT_E;

typedef enum
{
    IMG_PROC_NORMAL=0x00,
    IMG_PROC_VIEW,
    //camera 360
    IMG_PROC_LOMO, // need 9 type
    IMG_PROC_HDR,
    IMG_PROC_EFFECT_ENHANCEMENT, // need 9 type
    IMG_PROC_BLACK_AND_WHITE,
    IMG_PROC_ART_OF_BLACKWHITE,
    IMG_PROC_DREAM_EFFEC,
    IMG_PROC_DREAM_OF_1839,
    IMG_PROC_COLOR_TO_DRAWING,
    IMG_PROC_JAPANESE_STYLE,
    IMG_PROC_RETRO,
    IMG_PROC_NIGHT_ENHANCEMENT,
    //magic camera
    IMG_PROC_EXPOSURE,
    IMG_PROC_BRIGHTNESS,
    IMG_PROC_CONTRAST,
    IMG_PROC_SATURATION,
    IMG_PROC_HUE,
    //delight camera
    IMG_PROC_BLACK_WHITE_DRAWING,
    IMG_PROC_COLOURED_DRAWING,
    IMG_PROC_INFRARED,
    IMG_PROC_SYMMETYR,
    IMG_PROC_FOUR_COLOR,
    IMG_PROC_FISHEYE,
    IMG_PROC_REMOVE_COLOR,
    IMG_PROC_MOTION_AXIS,
    IMG_PROC_SCENCE_SKETCH,
    IMG_PROC_GHOST,
    //ext add
    IMG_PROC_FACE_DECT,
    IMG_PROC_EMBOSS,
    IMG_PROC_HISTOGRAM,
    IMG_PROC_NEGATIVE,
    IMG_PROC_EXPOSAL,
    IMG_PROC_MOSAIC,
    IMG_PROC_NINONE,
    IMG_PROC_PANORAMA,
    IMG_PROC_SCALE,
    IMG_PROC_ROTATION,
    IMG_PROC_XRAY,
    IMG_PROC_PHOTO_FRAME,

    IMG_PROC_CMD_MAX
} IMG_PROC_ALG_CMD_E;

typedef enum
{
    IMG_PROC_LOMO_MODE_ALPHA,
    IMG_PROC_LOMO_MODE_IMAGE,
    IMG_PROC_LOMO_MODE_FULL
}IMG_PROC_LOMO_MODE_E;

typedef enum
{
    IMG_PROC_PENCILSKETCH_MODE_NORMAL,
    IMG_PROC_PENCILSKETCH_MODE_PERFECT
    
}IMG_PROC_PENCILSKETCH_MODE_E;

typedef enum
{
    SUBFRM_LEFT_TOP= 0,
    SUBFRM_TOP,
    SUBFRM_RIGHT_TOP,
    SUBFRM_LEFT,
    SUBFRM_RIGHT,
    SUBFRM_LEFT_BTM,
    SUBFRM_BTM,
    SUBFRM_RIGHT_BTM,
    SUBFRAME_POS_ID_MAX
}SUBFRAME_POS_ID_E;

typedef enum
{
    FRAME_SIMPLE,
    FRAME_MASK,
    FRAME_MIXED,	     
    FRAME_STYLE_MAX       
}FRAME_STYLE_E;

typedef enum
{
    FRAME_CALLING_PREVIEW = 0x00,
    FRAME_CALLING_CAPTURE,
    FRAME_CALLING_MAX
}FRAME_CALLING_MODE_E;

typedef enum
{
    SIMPLE_MODE_ONE = 0x00,
    SIMPLE_MODE_MAX    
}FRAME_SIMPLE_MODE_E;

/**---------------------------------------------------------------------------*
 **                         Data Structure definition                         *
 **---------------------------------------------------------------------------*/
typedef struct _img_proc_point_tag
{
    int16 	x;
    int16 	y;
}IMG_PROC_POINT_T, *IMG_PROC_POINT_T_PTR;

typedef struct _img_proc_size_tag
{
    uint16 w;
    uint16 h;
}IMG_PROC_SIZE_T, *IMG_PROC_SIZE_T_PTR;

typedef struct _img_proc_rect_tag
{
    uint16 x;
    uint16 y;
    uint16 w;
    uint16 h;
}IMG_PROC_RECT_T, *IMG_PROC_RECT_T_PTR;

typedef struct _img_proc_data_addr_tag
{
    uint8* chn0;
    uint8* chn1;
    uint8* chn2;
    uint32 chn0_len;
    uint32 chn1_len;
    uint32 chn2_len;
}IMG_PROC_DATA_ADDR_T, *IMG_PROC_DATA_ADDR_T_PTR;

typedef struct  _img_proc_data_t
{
    IMG_PROC_DATA_FORMAT_E format;
    IMG_PROC_SIZE_T size;
    IMG_PROC_DATA_ADDR_T addr;
    uint16* name_ptr;
}IMG_PROC_DATA_T, *IMG_PROC_DATA_T_PTR; 

typedef struct  _img_proc_dst_t
{
    IMG_PROC_RECT_T rect;
    IMG_PROC_DATA_T data;
    imgproc_callback ctrl_callback;
}IMG_PROC_DST_T, *IMG_PROC_DST_T_PTR;

typedef struct
{
    IMG_PROC_DATA_T proc_back_data;
    IMG_PROC_DST_T proc_img;
    IMG_PROC_DST_T view_before[IMG_PROC_VIDW_BEFORE_NUM];
    IMG_PROC_DST_T view_after;
}IMG_PROC_TYPE_T, *IMG_PROC_TYPE_T_PTR;

typedef struct
{
    uint32 rtn_mode;
    IMG_PROC_DATA_T dst;
    IMG_PROC_DATA_T view_before;
    IMG_PROC_DATA_T view_after;
}IMG_PROC_FRAME_RTN_T, *IMG_PROC_FRAME_RTN_T_PTR;

typedef struct
{
    uint32 mode;
    IMG_PROC_DATA_T proc_img;
    IMG_PROC_DATA_T view_before[IMG_PROC_VIDW_BEFORE_NUM];
    IMG_PROC_DATA_T view_after;
}IMG_PROC_DATA_RTN_T, *IMG_PROC_DATA_RTN_T_PTR;

typedef struct
{
    uint32 mode;
    IMG_PROC_DATA_T data;
    void* alg_ptr;
}IMG_PROC_RTN_T, *IMG_PROC_RTN_T_PTR;

typedef struct
{
    int32 inum;
    IMG_PROC_FRAME_RTN_T param;
    imgproc_callback callback;
}IMG_PROC_FRAME_T, *IMG_PROC_FRAME_T_PTR;

typedef struct
{
    uint32 param;
}IMG_PROC_INIT_T, *IMG_PROC_INIT_T_PTR;

typedef struct
{
    uint32 param;
    uint16* temp_directory;
    uint32 temp_directory_len;
}IMG_PROC_OPEN_T, *IMG_PROC_OPEN_T_PTR;

typedef struct
{
    uint32 param;
}IMG_PROC_CLOSE_T, *IMG_PROC_CLOSE_T_PTR;

typedef struct
{
    uint32 param;
}IMG_PROC_CAPABILITY_T, *IMG_PROC_CAPABILITY_T_PTR;

typedef struct
{
    IMG_PROC_ALG_CMD_E cmd;
}IMG_PROC_ABORT_T, *IMG_PROC_ABORT_T_PTR;

typedef struct
{
    uint32 param;
}IMG_PROC_CREATE_T, *IMG_PROC_CREATE_T_PTR;

typedef struct
{
    IMG_PROC_DATA_RTN_T data_rtn;
    void* alg_rtn_ptr;
}IMG_PROC_PROCESS_RTN_T, *IMG_PROC_PROCESS_RTN_T_PTR;

typedef struct
{
    IMG_PROC_ALG_CMD_E cmd;
    IMG_PROC_DATA_T_PTR src_img_ptr;
    uint32 src_img_num;
    uint32 proc_mode;
    IMG_PROC_DST_T proc_img;
    IMG_PROC_DST_T view_before[IMG_PROC_VIDW_BEFORE_NUM];
    IMG_PROC_DST_T view_after;
    void *proc_param_ptr;
}IMG_PROC_PROCESS_T, *IMG_PROC_PROCESS_T_PTR;

typedef struct
{
    IMG_PROC_ALG_CMD_E cmd;
    void* alg_ptr;
    IMG_PROC_SIZE_T size;
    IMG_PROC_DATA_FORMAT_E input_format;
    IMG_PROC_DATA_ADDR_T input_addr;
    IMG_PROC_DATA_FORMAT_E output_format;
    IMG_PROC_DATA_ADDR_T output_addr;
}IMG_PROC_PROCESS_ICON_T, *IMG_PROC_PROCESS_ICON_T_PTR;

/////////ALG/////////
typedef struct _img_scale_param_t
{
    IMG_PROC_DATA_ADDR_T input_addr;
    IMG_PROC_SIZE_T input_size;
    IMG_PROC_RECT_T input_rect;
    IMG_PROC_DATA_FORMAT_E input_format;

    IMG_PROC_DATA_FORMAT_E output_format;
    IMG_PROC_DATA_ADDR_T output_addr;
    IMG_PROC_SIZE_T output_size;
}IMG_ALG_SCALE_PARAM_T, *IMG_ALG_SCALE_PARAM_T_PTR;

typedef struct _img_proc_ninone_tag
{
    uint32 all_img_num;
    uint32 h_img_num;
} IMG_PROC_NINONE_T, *IMG_PROC_NINONE_T_PTR;

typedef struct
{
	int32 num;                                           // the number of dect rect
	IMG_PROC_RECT_T rect[4];   // the face rect of dect from src image
	int8  smile_score[4];                             // the value of simle 0-100 [Tim.zhu modify]
} IMG_PROC_FACE_DECT_RTN_T, *IMG_PROC_FACE_DECT_RTN_T_PTR;

typedef struct _img_proc_hdr_tag
{
    uint32 level; // hdr level 0: low hdr 1: high hdr
} IMG_PROC_HDR_T, *IMG_PROC_HDR_T_PTR;

typedef struct img_proc_fisheye_tag
{
    uint32 distort_level;
    uint32 back_color; // x_rgb888
}IMG_PROC_FISHEYE_T;

typedef struct
{
    IMG_PROC_LOMO_MODE_E mode;
    uint32 width;
    uint32 height;
    uint32 *alpha_buf_ptr; //ARGB888
} IMG_PROC_LOMO_PARAM_T;

typedef struct
{
    uint32 param;
    IMG_PROC_PENCILSKETCH_MODE_E mode;
}IMG_PROC_PENCILSKETCH_T, *IMG_PROC_PENCILSKETCH_T_PTR;

// frame start
typedef struct frame_sliceinfo_tag
{
    IMG_PROC_DATA_ADDR_T data_addr;
    uint16 slice_h;
    uint16 slice_w;
    //uint16 offsets;
    IMG_PROC_DATA_FORMAT_E img_data_format; 
    
}FRAME_SLICEINFO_T, *FRAME_SLICEINFO_PTR;

//simple frame info
typedef struct frame_simple_tag
{
    FRAME_SIMPLE_MODE_E mode_type;
    
}FRAME_FILEINFO_SIMPLE_T, *FRAME_FILEINFO_SIMPLE_PTR;

//mask properties
typedef struct frame_file_mask_tag
{
    //uint16 frmCompression;               //compression
    uint32 frmImageSize;                 //pixel data size
    uint16 frmWidth;                                          
    uint16 frmHeight;
    uint32 clr_val;
    IMG_PROC_DATA_ADDR_T data_chn;
    IMG_PROC_DATA_FORMAT_E data_format;
    
}FRAME_FILEINFO_MASK_T, *FRAME_FILEINFO_MASK_PTR;

//sub-frame info
typedef struct frame_file_subframe_info_tag
{
    IMG_PROC_RECT_T subfrm_rect;
//    REF_IMG_SUBFRAME_STYLE_E frm_style; 
    SUBFRAME_POS_ID_E pos_id;
    
}FRAME_FILE_SUBFRAME_T, *FRAME_FILE_SUBFRAME_PTR;

//mixed frame properties
typedef struct frame_file_mixed_tag
{
    uint16 width;                   //frame image's width
    uint16 height;                  //frame image's height
    uint16 subfrm_num;              //the number of sub-frame
    uint16 reserved1;
    IMG_PROC_DATA_ADDR_T img_data_chn;
    void* psubfrm_info;
    IMG_PROC_DATA_FORMAT_E data_format;
    
}FRAME_FILEINFO_MIXED_T, *FRAME_FILEINFO_MIXED_PTR;

//frame resource information
typedef struct 
{
    void* frm_res_info_ptr;
    FRAME_STYLE_E frm_style;     
    IMGREF_BUF_T  temp_buff;
    FRAME_CALLING_MODE_E calling_mode;
}IMG_PROC_IMGFRAME_T;
// frame end

/******************************************************************************
// Description: img process ioctrl
// Author: Tim.zhu
// Note: none
******************************************************************************/
/******************************************************************************
// Description: img proc fun
******************************************************************************/
PUBLIC uint32 IMGPROC_Init(IMG_PROC_INIT_T_PTR param_ptr);

PUBLIC uint32 IMGPROC_Open(IMG_PROC_OPEN_T_PTR param_ptr); 

PUBLIC uint32 IMGPROC_Close(IMG_PROC_CLOSE_T_PTR param_ptr); 

PUBLIC IMGPROC_HANDLE IMGPROC_CreateHandle(IMG_PROC_CREATE_T_PTR param_ptr); 

PUBLIC uint32 IMGPROC_DestroyHandle(IMGPROC_HANDLE handle);

PUBLIC uint32 IMGPROC_Capability(IMG_PROC_CAPABILITY_T_PTR param_ptr, void* rtn_param_ptr);

PUBLIC uint32 IMGPROC_DoProcess(IMGPROC_HANDLE handle, IMG_PROC_PROCESS_T_PTR param_ptr, IMG_PROC_PROCESS_RTN_T_PTR rtn_param_ptr);

PUBLIC uint32 IMGPROC_DoProcessIcon(IMGPROC_HANDLE handle, IMG_PROC_PROCESS_ICON_T_PTR param_ptr, IMG_PROC_DATA_T_PTR rtn_param_ptr);

PUBLIC uint32 IMGPROC_AbortProcess(IMGPROC_HANDLE handle, IMG_PROC_ABORT_T_PTR param_ptr, void* rtn_param_ptr);

PUBLIC uint32 IMGPROC_GetProcessedFrame(IMGPROC_HANDLE handle, IMG_PROC_FRAME_T_PTR param_ptr, IMG_PROC_FRAME_RTN_T_PTR rtn_param_ptr);

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /*IMG_PROC_H*/
