/*****************************************************************************
** File Name:      mmivt_extern_simu.h                                                   *
** Author:                                                                   *
** Date:           2006/7/25                                               *
** Copyright:      2006 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:                                                              *
/*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2006/7/25       bin.ji       Create
******************************************************************************/

#ifndef _MMIVT_EXTERN_SIMU_H_
#define _MMIVT_EXTERN_SIMU_H_

#ifdef VT_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define MMI_CALL_TYPE_DATA 2//MN_CALL_TYPE_DATA

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

//the type of image quality
typedef enum
{
    MMIVT_IMAGEQUALITY_HIGH,        //high
    MMIVT_IMAGEQUALITY_NORMAL,   //normal
    MMIVT_MAX_IMAGEQUALITY
} MMIVT_AUDIO_QUALITY_E;
//the type of image rate
typedef enum
{
    MMIVT_IMAGERATE_HIGH,        //high
    MMIVT_IMAGERATE_LOW,   //low
    MMIVT_MAX_IMAGERATE
} MMIVT_VIDEO_RATE_E;
//the type of voice quality
typedef enum
{
    MMIVT_VOICEQUALITY_HIGH,        //high
    MMIVT_VOICEQUALITY_NORMAL,   //normal
    MMIVT_MAX_VOICEQUALITY
} MMIVT_VIDEO_QUALITY_E;

typedef enum
{
    MMIVT_VIDEO_ONLY_H263,
    MMIVT_VIDEO_ONLY_MPG4,    
    MMIVT_VIDEO_PREFER_H263,
    MMIVT_VIDEO_PREFER_MPG4,
    MMIVT_MAX_VIDEO_FORMAT
} MMIVT_VIDEO_FORMAT_E;

typedef enum
{
    MMIVT_H263_SIZE_QCIF,        
    MMIVT_H263_SIZE_SQCIF,   
    MMIVT_MAX_H263_SIZE
} MMIVT_H263_SIZE_E;
//@xiongxiaoyan add 20060330 end

typedef enum
{
    MMIVT_FALLBACK_YES,        //high
    MMIVT_FALLBACK_NO,   //normal
    MMIVT_MAX_FALLBACK
} MMIVT_FALLBACK_E;


typedef enum
{
    MMIVT_RATE_HIGH,        
    MMIVT_RATE_MID,  
    MMIVT_RATE_LOW,  
    MMIVT_MAX_RATE
} MMIVT_RATE_E;

//mmivt setting info
typedef struct
{
    MMIVT_VIDEO_RATE_E  video_rate;
    MMIVT_AUDIO_QUALITY_E audio_quality;
    MMIVT_VIDEO_QUALITY_E video_quality;
    MMIVT_VIDEO_FORMAT_E video_format;
    MMIVT_H263_SIZE_E	h263_size;
    MMIVT_FALLBACK_E	fallback;
    MMIVT_RATE_E		my_rate;	//本方帧率
    MMIVT_RATE_E		remote_rate;//对方
    MMIVT_REPLACEPIC_INFO_T  replacepic_info;//@zhaohui add,无条件替代画面的选择图片的信息
}MMIVT_NVSETTING_INFO_T;      

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/


#endif //#ifdef VT_SUPPORT

#endif
