/*****************************************************************************
** File Name:               mmiap_musicmark.h                                *
** Author:                                                                   *
** Date:           2012/7/19                                                 *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe ap control                  *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME              DESCRIPTION                              *
** 2012/7/19      aoke.hu           Create                                   *
******************************************************************************/
#ifdef MMIAP_MUSICMARK_SUPPORT
#ifndef _MMIAP_MUSICMARK_H_
#define _MMIAP_MUSICMARK_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmisrvfmm_export.h"
/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif
    
/*---------------------------------------------------------------------------*/
/*                          MACRO DEFINITION                                 */
/*---------------------------------------------------------------------------*/
//音乐书签总数
#define MMIMP3_MUSIC_MARK_MAX_NUM   50
//音乐书签名称长度
#if defined(MAINLCD_SIZE_240X320) || defined(MAINLCD_SIZE_240X400)
#define MMIMP3_MARK_NAME_MAX_LEN    (13)
#elif defined(MAINLCD_SIZE_320X480)
#define MMIMP3_MARK_NAME_MAX_LEN    (17)
#else
#define MMIMP3_MARK_NAME_MAX_LEN    (13)
#endif

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/ 

typedef struct
{
    wchar         mark_name[MMIMP3_MARK_NAME_MAX_LEN+1];
    uint16        mark_name_len;
    wchar         music_file_name[FMM_SEARCH_FILENAME_MAXLEN+1];
    uint16        music_file_name_len;
    wchar         list_name[FMM_SEARCH_FILENAME_MAXLEN+1];
    uint16        list_name_len;
    uint32        mark_offset_in_time;
    uint32        mark_offset_in_byte;
    uint32        music_total_time;
    uint32        mark_create_time; //reserved
} MMIMP3_MUSIC_MARK_DETAIL_INFO_T;

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :save and free 
//  Global resource dependence :
//  Author:xingdong.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_SaveMusicMark(void);

/*****************************************************************************/
//  Description : Get the total number of music mark.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC uint32 MMIMP3_GetMusicMarkNum(void);

/*****************************************************************************/
//  Description : Get music mark detail info.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetMusicMarkInfo(
                    uint32 index,                                       //[IN]
                    MMIMP3_MUSIC_MARK_DETAIL_INFO_T *mark_info_ptr      //[OUT]
                    );

/*****************************************************************************/
//  Description : load all music mark brief info.
//  Global resource dependence : s_music_mark_info_ptr
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_LoadAllMusicMarkInfo(void);


/*****************************************************************************/
//  Description : Add a new music mark.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC MMIFILE_ERROR_E MMIMP3_AddMusicMark(uint16 *cur_list_selection_ptr);

/*****************************************************************************/
//  Description : Delete music mark.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_DelMusicMark(
    uint16      *mark_index_ptr,        //[IN]
    uint16      index_num,           //[IN]
	uint16      *new_list_selection_ptr
);

/*****************************************************************************/
//  Description : Update music mark info.
//  Global resource dependence : 
//  Author:xingdong.li
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_UpdateMusicMarkName(
                    uint32              mark_index,         //[IN]mark index
                    MMI_STRING_T        mark_name_str   
                    );

/*****************************************************************************/
//  Description : resize music mark buffer size
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_ResizeMusicMarkBufferSize(uint32 buffer_size);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif
#endif
#endif//MMIAP_MUSICMARK_SUPPORT

