/*****************************************************************************
** File Name:      mmiap_main.h                                              *
** Author:                                                                   *
** Date:           28/08/2012                                                *
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to audio player                         *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2012       robert.wang       Create                                    *
******************************************************************************/
#ifndef _MMIAP_MAIN_H_
#define _MMIAP_MAIN_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "mmi_signal.h"
#include "mmisrv.h"
#include "mmisrvmgr.h"

#include "mmisrvfmm_export.h"
#include "mmisrvap_export.h"

#include "mmiap_list.h"

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
typedef uint32 MMIAP_WIN_FLAG_T;
#define MMIAPWIN_FLAG_FUNC_LYRIC     0x00000001 /*更新歌词*/
#define MMIAPWIN_FLAG_FUNC_PLAY      0x00000002 /*更新播放内容*/
#define MMIAPWIN_FLAG_FUNC_SPECTRUM  0x00000004 /*频谱*/
#define MMIAPWIN_FLAG_FUNC_MUSICMARK 0x00000008 /*音乐书签*/

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

typedef enum
{
    MMIAP_WIN_TYPE_VIRTUAL, //virtual win
    MMIAP_WIN_TYPE_WIDGET, //widget win
    MMIAP_WIN_TYPE_MAIN,// ap main win
    MMIAP_WIN_TYPE_IDLE, //platform main idle win
    MMIAP_WIN_TYPE_MAX
}MMIAP_WIN_TYPE_E;

/*
* win callback 返回值
*/
typedef enum
{
    MMIAP_WIN_CALLBACK_RET_CONTINUE, //继续流程
    MMIAP_WIN_CALLBACK_RET_BREAK, //中止流程，
    MMIAP_WIN_CALLBACK_RET_DONE_BY_CB, //callback 中已经处理
    MMIAP_WIN_CALLBACK_RET_MAX
}MMIAP_WIN_CALLBACK_RET_E; 

typedef struct
{
   uint32 *win_ptr; //win 数据
   MMIAP_WIN_FLAG_T win_flag;
   void *param_ptr;
   uint32 size_of_param;
}MMIAP_WIN_CALLBACK_PARAM_T;

typedef MMIAP_WIN_CALLBACK_RET_E (*MMIAP_WIN_CALLBACK) (MMIAP_WIN_CALLBACK_PARAM_T *param_ptr, uint32 size_of_param);


typedef struct
{
    MMIAP_WIN_TYPE_E win_type; //win type
    uint32 *win_ptr; //win 数据
    MMIAP_WIN_CALLBACK win_callback_func;
}MMIAP_WIN_REQ_PARAM_T;



typedef struct
{
    uint32 *win_ptr; //win 数据
    MMIAP_WIN_CALLBACK win_callback_func;
    MMIAP_WIN_FLAG_T win_update_flag;
}MMIAP_WIN_INFO_T;


typedef enum
{
    MMIMP3_OPEN_INFO_SINGLE_FILE,
    MMIMP3_OPEN_INFO_FMM_DATA,
    MMIMP3_OPEN_INFO_FMM_ARRAY,
    MMIMP3_OPEN_INFO_MAX
} MMIMP3_OPEN_INFO_TYPE_E;

typedef struct
{
    MMIFMM_DATA_INFO_T  *file_info_ptr;
    uint16              file_num;
    uint16              file_index;
    uint16              *dir_path_ptr;
    uint16              dir_path_len;
} MMIMP3_FMM_INFO_T;  //仅仅用于内部参数传递

typedef struct
{
    MMIMP3_OPEN_INFO_TYPE_E open_type;
    union
    {
        MMIMP3_FILE_INFO_T file_data_info;
        MMIMP3_FMM_INFO_T fmm_data_info;
    } file_info_u;
}MMIAP_OPEN_PLAYER_PARAM_T;

typedef struct
{
    MMIMP3_OPEN_INFO_TYPE_E open_type;
    union
    {
        MMIMP3_FILE_INFO_T file_data_info;
        MMIMP3_FMM_INFO_T fmm_data_info;
    } file_info_u;
    BOOLEAN is_need_init_player;
} MMIAP_OPEN_PLAYER_INFO_T;  //仅仅用于内部参数传递

typedef struct
{
    MMIAP_WIN_INFO_T wins_info[MMIAP_WIN_TYPE_MAX];

    BOOLEAN is_play_file;
    MMIAP_OPEN_PLAYER_INFO_T *open_player_info_ptr; //for 
    
    MMIFMM_FILTER_T music_filter;    
} MMIAP_INFO_T;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : reg win info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_RegWinInfo
(
    MMIAP_WIN_REQ_PARAM_T *param_ptr //out
);

/*****************************************************************************/
//  Description : destroy ap info
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DestroyAPInfo(void);

/*****************************************************************************/
//  Description : do win callback
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIAP_WIN_CALLBACK_RET_E MMIAP_DoWinCallback
(
    MMIAP_WIN_TYPE_E which_win, //in
    MMIAP_WIN_FLAG_T flag,//in
    void* param_ptr, //in
    uint32 size_of_param //in
);

/*****************************************************************************/
//  Description : init player info
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_InitPlayerInfo(
    MMIAP_OPEN_PLAYER_PARAM_T *open_param_ptr //in
);

/*****************************************************************************/
//  Description : set need init player flag
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetNeedInitPlayer(
    BOOLEAN is_need //in
);

/*****************************************************************************/
//  Description : get need init player flag
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsNeedInitPlayer(
    void
);

/*****************************************************************************/
//  Description : get open player info param
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_GetOpenPlayerParam(
    MMIAP_OPEN_PLAYER_PARAM_T *open_param_ptr //out
);

/*****************************************************************************/
//  Description : set bg start flag
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetBgStartFlag(BOOLEAN is_bg_start);

/*****************************************************************************/
//  Description : bg start or not
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsBgStart(void);

/*****************************************************************************/
//  Description : get music filter pointer
//  Global resource dependence :  none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIFMM_FILTER_T* MMIAP_GetMusicFilterPtr(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

