/*******************************************************************************
 ** FileName:    img_alg.h
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

#ifndef IMG_ALG_H
#define IMG_ALG_H
/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
//#include "sci_types.h"
#include "img_proc.h"
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
// YUV to RGB
//r = y + 1.402 * (v - 128);
#define YUV_TO_R(_y, _v) ((int32)(_y) + ((359 * ((int32)(_v) - 128)) >> 8))
//g =y - 0.34414 * (u - 128) - 0.71414 * (v - 128);
#define YUV_TO_G(_y, _u, _v) ((int32)(_y) - ((88 * ((int32)(_u) - 128) + 183 * ((int32)(_v) - 128)) >> 8))
//b = y + 1.772 * (u - 128);
#define YUV_TO_B(_y, _u) ((int32)(_y) + ((454 * ((int32)(_u) - 128)) >> 8))

// RGB to YUV
//Y = 0.299 * r + 0.587 * g + 0.114 * b;
#define RGB_TO_Y(_r, _g, _b)    (int32)((77 * (_r) + 150 * (_g) + 29 * (_b)) >> 8)
//U = 128 - 0.1687 * r - 0.3313 * g + 0.5 * b 
#define RGB_TO_U(_r, _g, _b)    (int32)(128 + ((128 * (_b) - 43 * (_r) - 85 * (_g)) >> 8))
//V = 128 + 0.5 * r - 0.4187 * g - 0.0813 * b 
#define RGB_TO_V(_r, _g, _b)    (int32)(128  + ((128 * (_r) - 107 * (_g) - 21 * (_b)) >> 8))

#define RGB565_GET_R(_color) (((_color) >> 8) & 0xf8)
#define RGB565_GET_G(_color) (((_color) >> 3) & 0xfc)
#define RGB565_GET_B(_color) (((_color) << 3) & 0xf8)

#define RGB565_SET_R(_color, _r) ((_color) | (((_r) << 8) & 0xf800))
#define RGB565_SET_G(_color, _g) ((_color) | (((_g) << 3) & 0x07e0))
#define RGB565_SET_B(_color, _b) ((_color) | (((_b) >> 3) & 0x001f))

#define MAKE_ARGB(_a, _r, _g, _b) ((((_a) << 24) & 0xff000000) |  (((_r) << 16) & 0xff0000) | (((_g) << 8) & 0xff00) | (((_b)) & 0xff))
#define MAKE_RGB565(_r, _g, _b) ((((_r) << 8) & 0xf800) | ((((_g) << 3)) & 0x7e0) | ((((_b) >> 3)) & 0x1f))
#define CHECK_RANGE_UINT8(_x) ((uint8)((_x) < 0 ? 0 : (_x) > 255 ? 255 : (_x)))
#define CLIP(x, bottom, top) ((x) < bottom ? bottom : (x) > top ? top :(x))


#define DATA_CLIP(input, bottom, top)	{if (input>top) input = top; if (input < bottom) input = bottom;}
#define ABS(x) (((x)>0)? (x) : -(x))
#define IMG_MIN_VALUE 0x00
#define IMG_MID_VALUE 0x80
#define IMG_MAX_VALUE 0xff
#define Y_MID_VALUE 0x80
#define UV_MID_VALUE 0x80

/**---------------------------------------------------------------------------*
 **                         Data Structure definition                         *
 **---------------------------------------------------------------------------*/
#define IMG_ALG_MAX_IMG_NUM 32

// start 
#define IMG_ALG_CALLING_ACTIVE (0x01<<0)
#define IMG_ALG_CALLING_PASSIVITY (0x01<<1)

 // read/write file calling mode
#define IMG_ALG_CALLING_BUF (0x00<<0)
#define IMG_ALG_CALLING_ID (0x01<<0)

// init/update/start end rtn cmd
#define IMG_ALG_START (0x01<<0)
#define IMG_ALG_COMPLETE (0x01<<1)
#define IMG_ALG_SAVE_FOR_NEXT (0x01<<2)
#define IMG_ALG_CLEAN_UPDATE (0x01<<3)

// update cmd
#define IMG_ALG_UPDATE_ALG (0x01<<0)
#define IMG_ALG_UPDATE_SRC (0x01<<1)
#define IMG_ALG_UPDATE_DST (0x01<<2)

typedef enum
{
    IMG_ALG_RGB565=0x0001,
    IMG_ALG_RGB888=0x0002,
    IMG_ALG_ARGB888=0x0004,
    IMG_ALG_ARGB565=0x0008,
    IMG_ALG_ARGB555=0x0010,
    IMG_ALG_COMPRESSED_ARGB=0x0020,
    IMG_ALG_JPG=0x0040,
    IMG_ALG_YUV422=0x0080,
    IMG_ALG_YUV420=0x0100,
    IMG_ALG_YUV400=0x0200,
    IMG_ALG_YUV422_3FRAME=0x0400,
    IMG_ALG_YUV420_3FRAME=0x0800,
    IMG_ALG_FORMAT_MAX=0x0fffffff
}IMG_ALG_DATA_FORMAT_E;

typedef struct _img_alg_size_tag
{
    uint16 w;
    uint16 h;
}IMG_ALG_SIZE_T, *IMG_ALG_SIZE_T_PTR;

typedef struct _img_alg_rect_tag
{
    uint16 x;
    uint16 y;
    uint16 w;
    uint16 h;
}IMG_ALG_RECT_T, *IMG_ALG_RECT_T_PTR;

typedef struct _img_alg_data_addr_tag
{
    uint8* chn0;
    uint8* chn1;
    uint8* chn2;
    uint32 chn0_len;
    uint32 chn1_len;
    uint32 chn2_len;
}IMG_ALG_DATA_ADDR_T, *IMG_ALG_DATA_ADDR_T_PTR;

typedef struct _img_alg_rtn_data_t
{
    IMG_ALG_DATA_FORMAT_E format;
    IMG_ALG_DATA_ADDR_T addr;
    IMG_ALG_SIZE_T size;
}IMG_ALG_RTN_DATA_T, *IMG_ALG_RTN_DATA_T_PTR; 

typedef struct _img_alg_read_data_tag
{
    IMG_ALG_DATA_FORMAT_E format;
    uint32 read_id;
    IMG_ALG_SIZE_T src_size;
    IMG_ALG_RECT_T src_rect;
    IMG_ALG_DATA_ADDR_T dst;
}IMG_ALG_READ_DATA_T, *IMG_ALG_READ_DATA_T_PTR;

typedef struct _img_alg_write_data_tag
{
    IMG_ALG_DATA_FORMAT_E format;
    IMG_ALG_DATA_ADDR_T src;
    IMG_ALG_SIZE_T src_size;
    uint32 write_id;
    IMG_ALG_SIZE_T dst_size;
    IMG_ALG_RECT_T dst_rect;
}IMG_ALG_WRITE_DATA_T, *IMG_ALG_WRITE_DATA_T_PTR;

typedef struct _img_alg_init_data_tag
{
    uint32 calling_mode;
    IMG_ALG_DATA_FORMAT_E format;
    IMG_ALG_DATA_ADDR_T addr;
    uint32 read_id;
    IMG_ALG_SIZE_T size;
    IMG_ALG_RECT_T rect;
}IMG_ALG_INIT_DATA_T, *IMG_ALG_INIT_DATA_T_PTR; 

typedef struct _img_alg_init_src_tag
{
    IMG_ALG_INIT_DATA_T_PTR img_ptr;// src image struct array pointer
    uint32 img_num;    
    uint32 (*read_data)(IMG_ALG_READ_DATA_T param_ptr, IMG_ALG_DATA_ADDR_T_PTR rtn_param_ptr);
}IMG_ALG_INIT_SRC_T, *IMG_ALG_INIT_SRC_T_PTR;

typedef struct _img_alg_init_dst_tag
{
    uint32 calling_mode;
    IMG_ALG_DATA_FORMAT_E format;
    IMG_ALG_SIZE_T size;
    IMG_ALG_DATA_ADDR_T addr;
    uint32 write_id;
    uint32 (*write_data)(IMG_ALG_WRITE_DATA_T_PTR param_ptr);
}IMG_ALG_INIT_DST_T, *IMG_ALG_INIT_DST_T_PTR;

typedef struct _img_alg_init_tag
{
	IMG_ALG_INIT_SRC_T src;
	IMG_ALG_INIT_DST_T dst;
	uint32 calling_mode;
	void* param_ptr; // alg param
}IMG_ALG_INIT_T, *IMG_ALG_INIT_T_PTR;

typedef struct _img_alg_init_rtn_tag
{
	uint32 param;
}IMG_ALG_INIT_RTN_T, *IMG_ALG_INIT_RTN_T_PTR;

typedef struct _img_alg_get_info_tag
{
    uint32 param;
}IMG_ALG_GET_INFO_T, *IMG_ALG_GET_INFO_T_PTR;

typedef struct _img_alg_info_rtn_tag
{
    uint32 param;
}IMG_ALG_INFO_RTN_T, *IMG_ALG_INFO_RTN_T_PTR;

typedef struct _img_alg_update_tag
{
    uint32 cmd;
    IMG_ALG_INIT_DST_T dst;
    IMG_ALG_INIT_DATA_T_PTR src_img_ptr;// src image struct array pointer
    uint32 src_img_num;
    uint32 (*read_data)(IMG_ALG_READ_DATA_T param_ptr, IMG_ALG_DATA_ADDR_T_PTR rtn_param_ptr);
    void* param_ptr; // alg param
    //uint32 param; //file change jpg/improcess to alg use
}IMG_ALG_UPDATE_T, *IMG_ALG_UPDATE_T_PTR;

typedef struct _img_alg_update_rtn_tag
{
    uint32 param;//IMG_ALG_START
}IMG_ALG_UPDATE_RTN_T, *IMG_ALG_UPDATE_RTN_T_PTR;

typedef struct _img_alg_start_tag
{
    IMG_ALG_DATA_FORMAT_E src_format;
    IMG_ALG_SIZE_T src_size;
    IMG_ALG_RECT_T src_rect;
    IMG_ALG_DATA_ADDR_T src_addr;
    IMG_ALG_DATA_FORMAT_E dst_format;
    IMG_ALG_SIZE_T dst_size;
    IMG_ALG_DATA_ADDR_T dst_addr;
}IMG_ALG_START_T, *IMG_ALG_START_T_PTR;

typedef struct _img_alg_start_rtn_tag
{
    IMG_ALG_DATA_FORMAT_E format;
    IMG_ALG_SIZE_T size;
    IMG_ALG_DATA_ADDR_T addr;
    void* alg_rtn_ptr;
    uint32 param;
}IMG_ALG_START_RTN_T, *IMG_ALG_START_RTN_T_PTR;

typedef struct _img_alg_ioctl_func_tab_tag
{
    uint32 (*init)(IMG_ALG_INIT_T_PTR param_ptr, uint32* id, IMG_ALG_INIT_RTN_T_PTR rtn_param_ptr);
    uint32 (*update)(uint32 id, IMG_ALG_UPDATE_T_PTR param_ptr, IMG_ALG_UPDATE_RTN_T_PTR rtn_param_ptr);
    uint32 (*get_info)(uint32 id, IMG_ALG_GET_INFO_T_PTR param_ptr, IMG_ALG_INFO_RTN_T_PTR rtn_param_ptr);
    uint32 (*start)(uint32 id, IMG_ALG_START_T_PTR param_ptr, IMG_ALG_START_RTN_T_PTR end_param_ptr,void* rtn_param_ptr);
    uint32 (*destroy)(uint32 id);
}IMG_ALG_IOCTL_FUNC_TAB_T, *IMG_ALG_IOCTL_FUNC_TAB_T_PTR;

typedef struct _img_alg_info_tag
{
	uint32* support_format_ptr;
	IMG_ALG_IOCTL_FUNC_TAB_T_PTR ioctl_ptr;
}IMG_ALG_INFO_T, *IMG_ALG_INFO_T_PTR;

typedef struct _img_alg_exp_info_tag
{
	IMG_PROC_ALG_CMD_E	cmd;
	IMG_ALG_INFO_T_PTR alg_info_ptr;
}IMG_ALG_EXP_INFO_T, *IMG_ALG_EXP_INFO_T_PTR;

typedef struct _img_proc_ioctl_func_tab_tag
{
    uint32 (*setdecoderdstbuf)(IMGPROC_HANDLE handle);
    uint32 (*setprocimgdstbuf)(IMGPROC_HANDLE handle);
}IMG_PROC_IOCTL_FUNC_TAB_T, *IMG_PROC_IOCTL_FUNC_TAB_T_PTR;

/******************************************************************************
// Description: img process get register function tab pointer
// Author: Tim.zhu
// Note: none
******************************************************************************/
/******************************************************************************
// Description: img proc fun
******************************************************************************/
PUBLIC IMG_ALG_EXP_INFO_T_PTR IMG_ALG_GetAlgExpInfo(void);
PUBLIC IMG_PROC_IOCTL_FUNC_TAB_T_PTR IMG_PROC_GetIoCtlFun(void);
PUBLIC uint8* IMGPROC_ALLOCA(uint32 size);
PUBLIC uint32 IMGPROC_FREE(uint8* buf_ptr);
PUBLIC uint32 IMGPROC_GetPixelbyte(IMG_ALG_DATA_FORMAT_E format, uint32 frame_num, uint32* pixel_byte, uint32* h_div, uint32* v_div);
PUBLIC uint32 IMGPROC_GetFrameNum(IMG_ALG_DATA_FORMAT_E format, uint32* frame_num);
PUBLIC uint32 IMGPROC_GetChnLen(IMG_PROC_DATA_T_PTR data_ptr);
PUBLIC void IMGPROC_ScaleDown_Y_UV_422(IMG_ALG_SCALE_PARAM_T *scale_info_ptr);
PUBLIC uint32 IMGPROC_SetProcImgDstBuf(IMGPROC_HANDLE handle);
PUBLIC uint32 IMGPROC_SetProcImgDstFs(IMGPROC_HANDLE handle);
PUBLIC uint32 IMGPROC_BufEncoder(IMGPROC_HANDLE handle);
PUBLIC int32 IMGPROC_Sqrt(int32 t);

PUBLIC void IMGPROC_YUV2RGB(IMG_PROC_DATA_FORMAT_E rgb_format, uint8 *rgb_ptr, uint8 * y_ptr, uint8 * uv_ptr, uint32 w,uint32 h);
PUBLIC void IMGPROC_RGB2YUV(IMG_PROC_DATA_FORMAT_E rgb_format, uint8 *rgb_ptr, uint8 * y_ptr, uint8 * uv_ptr, uint32 w,uint32 h);
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /*IMG_ALG_H*/
