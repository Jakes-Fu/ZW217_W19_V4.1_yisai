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
// ģ����Դ��Ϣ
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
// ������Դ����Ϣ
///////////////////////////////////////////////////////////////////////////////

typedef enum _ANIM_TYPE_E
{
    AT_COMPRESS_STREAM = 0,
    AT_COMPRESS_MJPEG,      // ����Motion JPEG ѹ��
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
// ͼƬ��Դ����Ϣ
///////////////////////////////////////////////////////////////////////////////

// ͼƬ������
typedef enum _IMG_TYPE_E
{
    IMG_CMP_BMP_16_565_RAW = 0,     // ����16��24λBMP,���16λ565��ʽ,δѹ������׼��ʽ
    IMG_CMP_BMP_16_565_RLE,         // ����16��24λBMP,���16λ565��ʽ����ѹ����չѶ˽�и�ʽ
    IMG_CMP_BMP_JPG,                // ����16��24λBMP,���JPG,��׼��ʽ
    IMG_CMP_BMP_SJPG,               // ����16��24λBMP,���JPG,չѶ˽�и�ʽ(ȥͷ)
    IMG_CMP_BMP_32_SCI,             // ����32λBMP�����32λARGB,չѶ˽�и�ʽ    
    IMG_CMP_GIF_SCI,                // ����GIF�����GIF��չѶ˽�и�ʽ
    IMG_CMP_JPG_STD,                // ����JPG�����JPG, ת��Ϊ��׼��ʽ(ȥ���˲���Ҫ����Ϣ)�������ʽͬ��IMG_CMP_BMP_JPG��
    IMG_CMP_JPG_SCI,                // ����JPG�����JPG��չѶ˽�и�ʽ��ȥͷ���������ʽͬ��IMG_CMP_BMP_SJPG��
    IMG_CMP_PNG_ARGB,               // ����PNG�����ARGB��չѶ˽�и�ʽ�������ʽͬ��IMG_CMP_BMP_32_SCI��
    IMG_CMP_BMP_8_SCI,              // ����8λBMP�����8λBMP,չѶ˽�и�ʽ
    IMG_CMP_PNG_SABM,               // ����PNG�����32λARGB,չѶ˽�и�ʽ, ����ͼ���ƽ�̺�����
    IMG_CMP_BMP_CK,                 // ֧������ƽ̨��color key��bmpͼƬ, ����ƽ̨ȫ�ֵ�color keyӰ��
    IMG_CMP_BMP_32_ARGB888,         // ����16��24λBMP,���32λARGB888��ʽ
    IMG_CMP_PNG_ARGB888,            // ����PNG�����32λARGB888��ʽ
    IMG_CMP_BMP_32_ARGB_RLE,
    IMG_CMP_PNG_ARGB_RLE,        //RLE
    
     IMG_CMP_BMP_32_ARGB_RLE_AQ,
    IMG_CMP_PNG_ARGB_RLE_AQ,

    IMG_CMP_PNG_ARGB888_RLE, 
    IMG_CMP_BMP_32_ARGB888_RLE, 
    
    IMG_CMP_PNG_SABM_RLE,
    IMG_CMP_PNG_SABM888_RLE,
     
    IMG_CMP_USER,                //�û��Զ����ʽ
    IMG_CMP_RAW = 0xFF              // �����κ�ͼƬ�ļ������ԭ�ļ��������κδ���
} IMG_TYPE_E;

typedef enum _IMG_JPG_QULITY_E
{
    JPG_Q40 = 0,
    JPG_Q50,
    JPG_Q70,
    JPG_Q85,
    JPG_Q95
}IMG_JPG_QULITY_E;

//abm�Ķ������guires_img.c��, flag������Դ���߽���, ������RES_ADD_IMG��ʹ��
//�� RES_ADD_IMG(IMAGE_TITLE_BAR,"\\MMI_RES_##\\IMAG\\Title\\IMAGE_TITLE_BAR.png",IMG_CMP_PNG_SABM | SABM_TILE,0)
//sabm flag
#define SABM_TILE 		    (0x1)    //ƽ��
#define SABM_HOR_STRETCH  	(0x2)    //ˮƽ����
#define SABM_VER_STRETCH  	(0x4)    //��ֱ����
#define SABM_CEN_STRETCH  	(0x8)    //�м�����

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
    PIMG_FLAG_T pFlag;       // ͼƬ�ĸ�����ʶ���糤�������͵���Ϣ
    uint32    * pImgData;
} MMI_IMG_DATA_T, * PMMI_IMG_DATA_T;


///////////////////////////////////////////////////////////////////////////////
// ������Դ����Ϣ
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


