/*****************************************************************************
** File Name:      mmiap_album.h                                             *
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
#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT

#ifndef _MMIAP_ALBUM_H_
#define _MMIAP_ALBUM_H_

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmimp3_export.h"
#include "mmiapwin_common.h"

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

/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/

typedef struct
{
    BOOLEAN is_tp_start;
    BOOLEAN is_glide_move;

    GUI_POINT_T           glide_tp_start_point;
    GUI_POINT_T           glide_tp_prev_point;

    MMIMP3_ALBUM_INFO_T   album_info[MMIAP_DIRECT_MAX];

} MMIAP_ALBUM_MNG_INFO_T;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 获取ID3文本描述信息并转换为wchar
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIMP3_GetAlbumWcharDescInfo(
    MMISRVAUD_ID3_TAG_T     *tag_info_ptr, //in
    MMIMP3_ALBUM_INFO_T *album_ptr     //out
);

/*****************************************************************************/
//  Description : 获取ID3文本描述信息并转换为wchar
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetAlbumImgDataFromFile(
    wchar  *file_name_ptr,    //in
    uint32 apic_offset,       //in
    uint32 apic_len,          //in
    uint8  *buf_ptr,          //out
    uint32 buf_len            //in
);

/*****************************************************************************/
//  Description : 获取当前mp3专辑信息，包括歌手、专辑图片数据等.
//  Global resource dependence :
//  Author:xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMP3_GetCurMp3AlbumInfo(
    MMIMP3_ALBUM_INFO_T *album_ptr  //in/out
);

/*****************************************************************************/
//  Description : 根据文件路径获取专辑信息
//  Global resource dependence :
//  Author: xin.li
//  Note: aoke modify
/*****************************************************************************/
PUBLIC BOOLEAN  MMIMP3_GetAlbumInfo(
    const wchar * full_path_ptr,
    uint16 full_path_len,
    MMIMP3_ALBUM_INFO_T *album_ptr  //out
);

/*****************************************************************************/
//  Description : 获取全部的专辑信息
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_GetAllAlbumInfo(void);

/*****************************************************************************/
//  Description : 更新全部的专辑信息
//  Global resource dependence :
//  Author: paul.huang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_UpdateAllAlbumInfo(BOOLEAN is_prev);

/*****************************************************************************/
//  Description : 清空全部的专辑信息
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_ReleaseAllAlbumInfo(void);

/*****************************************************************************/
//  Description : 清空专辑封面信息，释放内存
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_ReleaseAllAlbumImgInfo(void);

/*****************************************************************************/
//  Description : get appointed album info ptr
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC MMIMP3_ALBUM_INFO_T*  GetAppointedAlbumInfoPtr(
    MMIAP_DIRECT_E  which_album //in
);

/*****************************************************************************/
//  Description : handle album image glide tp press up msg
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_HandleAlbumImgGlideTpUp
(
    MMIAP_MAIN_WIN_INFO_T *win_ptr, //in
    DPARAM param
);

/*****************************************************************************/
//  Description : handle album image glide tp press move msg
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_HandleAlbumImgGlideTpMove
(
    MMIAP_MAIN_WIN_INFO_T *win_ptr, //in
    DPARAM param
);

/*****************************************************************************/
//  Description : handle album image glide tp press down msg
//  Global resource dependence :
//  Author: xin.li
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_HandleAlbumImgGlideTpDown
(
    MMIAP_MAIN_WIN_INFO_T *win_ptr, //in
    DPARAM param //in
);

/*****************************************************************************/
//  Description : set album tp start
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetAlbumTpStart(
    BOOLEAN is_start //in
);

/*****************************************************************************/
//  Description : get album tp start
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsAlbumTpStart(
    void
);

/*****************************************************************************/
//  Description : get album glide move
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_IsAlbumGlideMove(
    void
);

/*****************************************************************************/
//  Description : set album glide move
//  Global resource dependence :
//  Author:robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_SetAlbumGlideMove(
    BOOLEAN is_move //in
);

/*****************************************************************************/
//  Description : init album mng info
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAP_InitAlbumMngInfo(
    void
);

/*****************************************************************************/
//  Description : destroy album mng ino
//  Global resource dependence :
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAP_DestroyAlbumMngInfo(
    void
);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif

#endif //#ifdef MMI_AUDIOPLAYER_ALBUM_SUPPORT

