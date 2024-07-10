/****************************************************************************
** File Name:      mmiset_videowallpaper.h                                 *
** Author:         aoke.hu                                                 *
** Date:           08/02/2011                                              *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.     *
** Description:                                                            *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 08/2011        aoke.hu          create
** 
****************************************************************************/

#ifndef _MMISET_VIDEOWALLPAPER_H
#define _MMISET_VIDEOWALLPAPER_H

#ifdef MMI_VIDEOWALLPAPER_SUPPORT
/**---------------------------------------------------------------------------*
 **                         Include Files                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mmifmm_export.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern  "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Macro Declaration    							  *
 **---------------------------------------------------------------------------*/
#if !defined WIN32 && !defined CHIP_ENDIAN_LITTLE
#define MMISET_VIDEO_WALLPAPER_LIST_VER		 "VIDEO_VER.00.02_BE"  //�洢�ļ���ʽ�汾��
#else
#define MMISET_VIDEO_WALLPAPER_LIST_VER		 "VIDEO_VER.00.02_LE"  //�洢�ļ���ʽ�汾��
#endif
#define MMISET_VIDEO_WALLPAPER_LIST_VER_LEN 18
#define MMISET_VIDEO_WALLPAPER_MAX_NUM      10

#if defined(MAINLCD_SIZE_320X480) 
#define VIDEOPLAYER_WALLPAPER_FOR_1300KB_AVAILNUM_IN_IDLESCREEN 
#elif defined(MAINLCD_SIZE_240X400)
#define VIDEOPLAYER_WALLPAPER_FOR_2100KB_AVAILNUM_IN_IDLESCREEN
#elif defined(MAINLCD_SIZE_240X320)
#define VIDEOPLAYER_WALLPAPER_FOR_2400KB_AVAILNUM_IN_IDLESCREEN
#elif defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)
#define VIDEOPLAYER_WALLPAPER_FOR_2900KB_AVAILNUM_IN_IDLESCREEN
#endif

/**---------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                 *
 **---------------------------------------------------------------------------*/
typedef enum
{
    MMISET_VIDEO_WALLPAPER_SWITCH,
    MMISET_VIDEO_WALLPAPER_LIST,
//    MMISET_VIDEO_WALLPAPER_RANDOM,
    MMISET_VIDEO_WALLPAPER_AUDIO_SUPPORT,
    MMISET_VIDEO_WALLPAPER_MAX
}MMISET_VIDEO_WALLPAPER_E;

typedef struct MMISET_VIDEO_LIST_T_tag
{
    wchar           full_path_name[MMIFILE_FULL_PATH_MAX_LEN+1];    //wallpaper name
    uint16          full_path_len;
    uint16          reserved;    
    uint32          file_size;
    struct MMISET_VIDEO_LIST_T_tag  *next_ptr;
}MMISET_VIDEO_LIST_T;

typedef struct MMISET_VIDEO_LIST_HEAD_T_tag
{
    uint8               version[MMISET_VIDEO_WALLPAPER_LIST_VER_LEN+1];
    uint32              total_num;
}MMISET_VIDEO_LIST_HEAD_T;

typedef struct MMISET_VIDEO_WALLPAPER_NV_T_tag
{
    BOOLEAN             is_random;
    BOOLEAN             is_audio_support;
}MMISET_VIDEO_WALLPAPER_NV_T;

typedef struct MMISET_VIDEO_WALLPAPER_LIST_tag
{
    uint32              total_num;
    BOOLEAN             is_invalid[MMISET_VIDEO_WALLPAPER_MAX_NUM];
    MMISET_VIDEO_LIST_T *list_head_ptr;             //����  
}MMISET_VIDEO_WALLPAPER_LIST_T;


/**---------------------------------------------------------------------------*
 **                         Function Declare                                  *
 **---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_IsExistVWP(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_IsExistNextVWP(void);

/*****************************************************************************/
//  Description : ����video wallpaper����������
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISETVWP_OpenMainWin(void);

/*****************************************************************************/
//  Description : VIDEO WALLPAPER����
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISETVWP_Reset(void);

/*****************************************************************************/
//  Description : VIDEO WALLPAPER��ʼ��
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC void MMIAPISETVWP_Init(void);

/*****************************************************************************/
// Description : ��Ƶbuffer���Ƿ��ѿ���
// Global resource dependence : 
// Author:aoke.hu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_IsVideoLayerEnable(void);

/*****************************************************************************/
// Description : �Ƿ������Ƶǽֽ����
// Global resource dependence : 
// Author:aoke.hu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_IsAudioUsing(void);

/*****************************************************************************/
// Description : VIDEO WALLPAPER �Ƿ���
// Global resource dependence : 
// Author:aoke.hu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_IsOpen(void);

/*****************************************************************************/
// Description : �л�����һ��VIDEO WALLPAPER 
// Global resource dependence : 
// Author:aoke.hu
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPISETVWP_SwitchToNext(void);

/*****************************************************************************/
// Description : ֹͣ����VIDEO WALLPAPER 
// Global resource dependence : 
// Author:aoke.hu
// Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_StopVideoWallpaper(void);

/*****************************************************************************/
// Description : 
// Global resource dependence : 
// Author:aoke.hu
// Note: 
/*****************************************************************************/
PUBLIC void MMIAPISETVWP_LcdSwitch(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPISETVWP_HandleVideoWallPaper(MMI_WIN_ID_T win_id, 
                                                      MMI_MESSAGE_ID_E msg_id, 
                                                      DPARAM param);

/*****************************************************************************/
//  Description : ������Ƶǽֽ�б�
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_AddToVideoWallpaperList(wchar *full_name_ptr, 
                                                    uint16 full_name_len, 
                                                    uint32 file_size);

/*****************************************************************************/
//  Description : ResetAllVideoIsValid
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_ResetAllVideoIsValid(void);

/*****************************************************************************/
//  Description : ��ͣ��Ƶ����
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_Pause(void);

/*****************************************************************************/
//  Description : �ָ���Ƶ����
//  Global resource dependence :  none
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_Resume(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: aoke.hu
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISETVWP_SetBgVideoAudioSupport(BOOLEAN is_audio_support);
#ifdef  VIDEOTHUMBNAIL_SUPPORT  
/*****************************************************************************/
//  Description :MMIAPISETVWP_DisplayThumbnail  ��ʾ��Ƶ�Ķ�Ӧ����ͼ
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC  void  MMIAPISETVWP_DisplayThumbnail(void);
#endif
/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  //MMI_VIDEOWALLPAPER_SUPPORT

#endif  // _MMISET_VIDEOWALLPAPER_H


