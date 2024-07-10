/*****************************************************************************
** File Name:      xxx_mmi_module.h                                           *
** Author:                                                                   *
** Date:           07/2007                                                   *
** Copyright:      2007 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe call log                    *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** Create by Spreadtrum Resource Editor tool                                 *
******************************************************************************/

#ifndef _MMI_MODULE_H_
#define _MMI_MODULE_H_

#include "sci_types.h"

#ifdef __cplusplus
extern   "C" {
#endif

#define MMI_WIN_ID_NAME_MAX_LENGTH 50
#define MMI_SYS_RES_FLAG            (0x1 << 30)
///////////////////////////////////////////////////////////////////////////////
// 模块资源信息
///////////////////////////////////////////////////////////////////////////////

#define MACRO_RES_PRJ_TYPE      MACRO_RES_PRJ_MODULE

#include "macro_res_prj_def.h"

typedef enum MMI_MODULE_TYPE_E
{
    /*
    MMI_MODULE_COMMON,
    MMI_MODULE_FONT,
    MMI_MODULE_IM,
    MMI_MODULE_TP,
    MMI_MODULE_MMIBRW,
    MMI_MODULE_MMS,
    MMI_MODULE_SMS,
    MMI_MODULE_FM,
    MMI_MODULE_EBOOK,
    MMI_MODULE_MPEG4,
    MMI_MODULE_MULTIM,
    MMI_MODULE_ALARM,
    MMI_MODULE_GAME_MANAGER,
    MMI_MODULE_SET,
    MMI_MODULE_MP3,
    MMI_MODULE_CC,
    MMI_MODULE_PB,
    MMI_MODULE_STK,
    MMI_MODULE_ENG,
    MMI_MODULE_ENGTD,
    MMI_MODULE_RECORD,
    MMI_MODULE_ACC,
    MMI_MODULE_JAVA,
    MMI_MODULE_BT,
    MMI_MODULE_GAME_LINK,
    MMI_MODULE_GAME_BOXMAN, 
    MMI_MODULE_CONNECTION,
    MMI_MODULE_FMM,
    MMI_MODULE_OCR,
    MMI_MODULE_VT,
    MMI_MODULE_STREAMING,
    MMI_MODULE_GPS,
    MMI_MODULE_CAMERA,
    MMI_MODULE_KURO,    
    MMI_MODULE_PIC,
    MMI_MODULE_MTV,
    MMI_MODULE_DYNA_APPLIST,
    */
    #include "mmi_res_prj_def.h"
    MMI_MODULE_GPRS_TEST,
    MMI_MODULE_GPS,
    MMI_MODULE_IPERF_TEST,
    MMI_MODULE_MAX_NUM
} MMI_MODULE_TYPE_E;

#undef MACRO_RES_PRJ_TYPE

///////////////////////////////////////////////////////////////////////////////
// 动画资源的信息
///////////////////////////////////////////////////////////////////////////////

typedef enum _ANIM_TYPE_E
{
    AT_COMPRESS_STREAM = 0,
    AT_COMPRESS_MJPEG,      // 采用Motion JPEG 压缩
    ANIM_CMP_BMP_ARGB,      // motion argb
    ANIM_CMP_PNG_ARGB
} ANIM_TYPE_E;

typedef struct _ANIM_FLAG_T
{
    uint16 nWidth;
    uint16 nHeight;
    uint16 AnimType;
    uint16 nFrameNum;
    uint32 nDataSize;
} ANIM_FLAG_T, * PANIM_FLAG_T;

typedef struct _MMI_ANIM_DATA_T
{
    PANIM_FLAG_T pFlag;
    uint32     * pAnimData;
} MMI_ANIM_DATA_T, * PMMI_ANIM_DATA_T;


///////////////////////////////////////////////////////////////////////////////
// 图片资源的信息
///////////////////////////////////////////////////////////////////////////////

// 图片的类型
typedef enum _IMG_TYPE_E
{
    IMG_CMP_BMP_16_565_RAW = 0,     // 输入16、24位BMP,输出16位565格式,未压缩，标准格式
    IMG_CMP_BMP_16_565_RLE,         // 输入16、24位BMP,输出16位565格式，行压缩，展讯私有格式
    IMG_CMP_BMP_JPG,                // 输入16、24位BMP,输出JPG,标准格式
    IMG_CMP_BMP_SJPG,               // 输入16、24位BMP,输出JPG,展讯私有格式(去头)
    IMG_CMP_BMP_32_SCI,             // 输入32位BMP，输出32位ARGB,展讯私有格式    
    IMG_CMP_GIF_SCI,                // 输入GIF，输出GIF，展讯私有格式
    IMG_CMP_JPG_STD,                // 输入JPG，输出JPG, 转换为标准格式(去掉了不必要的信息)，输出格式同“IMG_CMP_BMP_JPG”
    IMG_CMP_JPG_SCI,                // 输入JPG，输出JPG，展讯私有格式（去头），输出格式同“IMG_CMP_BMP_SJPG”
    IMG_CMP_PNG_ARGB,               // 输入PNG，输出ARGB，展讯私有格式，输出格式同“IMG_CMP_BMP_32_SCI”
    IMG_CMP_BMP_8_SCI,              // 输入8位BMP，输出8位BMP,展讯私有格式
    IMG_CMP_PNG_SABM,               // 输入PNG，输出32位ARGB,展讯私有格式, 用于图像的平铺和拉伸
    IMG_CMP_BMP_CK,                 // 支持其他平台带color key的bmp图片, 不受平台全局的color key影响
    IMG_CMP_BMP_32_ARGB888,         // 输入16、24位BMP,输出32位ARGB888格式
    IMG_CMP_PNG_ARGB888,            // 输入PNG，输出32位ARGB888格式
    IMG_CMP_BMP_32_ARGB_RLE,
    IMG_CMP_PNG_ARGB_RLE,        //RLE
    
     IMG_CMP_BMP_32_ARGB_RLE_AQ,
    IMG_CMP_PNG_ARGB_RLE_AQ,

    IMG_CMP_PNG_ARGB888_RLE, 
    IMG_CMP_BMP_32_ARGB888_RLE, 
    
    IMG_CMP_PNG_SABM_RLE,
    IMG_CMP_PNG_SABM888_RLE,
     
    IMG_CMP_USER,                //用户自定义格式
    IMG_CMP_RAW = 0xFF              // 输入任何图片文件，输出原文件，不作任何处理
} IMG_TYPE_E;

typedef enum _IMG_JPG_QULITY_E
{
    JPG_Q40 = 0,
    JPG_Q50,
    JPG_Q70,
    JPG_Q85,
    JPG_Q95
}IMG_JPG_QULITY_E;

//abm的定义放在guires_img.c中, flag用于资源工具解析, 可以在RES_ADD_IMG中使用
//如 RES_ADD_IMG(IMAGE_TITLE_BAR,"\\MMI_RES_##\\IMAG\\Title\\IMAGE_TITLE_BAR.png",IMG_CMP_PNG_SABM | SABM_TILE,0)
//sabm flag
#define SABM_TILE 		    (0x1)    //平铺
#define SABM_HOR_STRETCH  	(0x2)    //水平拉伸
#define SABM_VER_STRETCH  	(0x4)    //垂直拉伸
#define SABM_CEN_STRETCH  	(0x8)    //中间拉伸

typedef struct _IMG_FLAG_T
{
    uint16 nWidth;
    uint16 nHeight;
    uint16 nType;
    uint16 nOtherInfo;
    uint32 nSize;
} IMG_FLAG_T, * PIMG_FLAG_T;

typedef struct _MMI_IMG_DATA_T
{
    PIMG_FLAG_T pFlag;       // 图片的附件标识，如长、宽、类型等信息
    uint32    * pImgData;
} MMI_IMG_DATA_T, * PMMI_IMG_DATA_T;


///////////////////////////////////////////////////////////////////////////////
// 文字资源的信息
///////////////////////////////////////////////////////////////////////////////

typedef enum _TEXT_TYPE_E
{
    TT_ENGLISH              = 0,
    TT_SIMPLECHINESE        = 1,
    TT_TRADITIONALCHINESE   = 2,
    TT_ARABIC               = 3,
    TT_FRENCH               = 4,
    TT_HINDI                = 5,
    TT_HUNGARIAN            = 6,
    TT_INDONESIAN           = 7,
    TT_MALAY                = 8,
    TT_PORTUGUESE           = 9,
    TT_RUSSIAN              = 10,
    TT_SPANISH              = 11,
    TT_TAGALOG              = 12,
    TT_THAI                 = 13,
    TT_VIETNAMESE           = 14,
    TT_MAX_NUM
} TEXT_TYPE_E;

typedef struct _KSP_TEXT_T
{
    char *  pTxtTable;
    uint16  nbText;
} KSP_TEXT_T, * PKSP_TEXT_T;


#ifdef __cplusplus
}
#endif

#endif // _MMI_MODULE_H_


