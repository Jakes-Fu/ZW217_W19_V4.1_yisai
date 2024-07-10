/*****************************************************************************
** File Name:      mmipicview_export.h                                       *
** Author:                                                                   *
** Date:           24/03/2006                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmimp3_internal func                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008       Jassmine           Create
******************************************************************************/

#ifndef _MMIPICVIEW_H_
#define _MMIPICVIEW_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmifmm_export.h"
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
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//slide play effect
typedef enum
{
    MMIPICVIEW_EFFECT_ORDER,    //以下情况顺序播放
    MMIPICVIEW_EFFECT_ENLARGE,  //盒装展开
    MMIPICVIEW_EFFECT_GRID,     //棋盘展开
    MMIPICVIEW_EFFECT_LEFTWARD, //向左展开
    MMIPICVIEW_EFFECT_RIGHTWARD,//向右展开
    MMIPICVIEW_EFFECT_UPWARD,   //向上展开
    MMIPICVIEW_EFFECT_DOWNWARD, //向下展开
#ifdef EFFECT_RIPPLE_SUPPORT    
    MMIPICVIEW_EFFECT_RIPPLE,   //ripple  
#endif    
    MMIPICVIEW_EFFECT_NONE,	    //无效果
    MMIPICVIEW_EFFECT_TYPE_MAX
} MMIPICVIEW_EFFECT_TYPE_E;

typedef struct
{
    wchar  path[MMIFMM_PATHNAME_LEN + 1];
    uint16 path_len;
}MMIPICVIEW_DEFAULT_PATH_T;

#ifdef CMCC_UI_STYLE
typedef enum
{
    MMIPICVIEW_PHOTO,	    //照片
    MMIPICVIEW_DOWNLOAD_PIC,	    //下载图片
    MMIPICVIEW_TYPE_MAX  
} MMIPICVIEW_PATH_TYPE_E;
#endif

typedef enum
{
    MMIPICVIEW_FROM_NONE,
    MMIPICVIEW_PHOTOS_FROM_WALLPAPER,
    MMIPICVIEW_PHOTOS_FROM_MMS,
    MMIPICVIEW_VIDEOS_FROM_MMS,
    MMIPICVIEW_PHOTOS_FROM_EMAIL,
    MMIPICVIEW_VIDEOS_FROM_EMAIL,
    MMIPICVIEW_PHOTOS_FROM_PB,
    MMIPICVIEW_PHOTOS_FROM_CAMERA,
    MMIPICVIEW_FROM_MAX
} MMIPICVIEW_FROM_TYPE_E;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMIPICVIEW_HandleDelQueryWin		MMIAPIPICVIEW_HandleDelQueryWin
#define MMIPICVIEW_GetDefaultPath			MMIAPIPICVIEW_GetDefaultPath
#define MMIPICVIEW_GetMyPhotosPath			MMIAPIPICVIEW_GetMyPhotosPath
#define MMIPICVIEW_EnterMyPhotos			MMIAPIPICVIEW_EnterMyPhotos
#define MMIPICVIEW_SetDefaultPath			MMIAPIPICVIEW_SetDefaultPath
#define MMIPICVIEW_IsDefaultPath			MMIAPIPICVIEW_IsDefaultPath
#define MMIPICVIEW_DeleteOrderInTime		MMIAPIPICVIEW_DeleteOrderInTime
#define MMIPICVIEW_GetCurPicFileInfo		MMIAPIPICVIEW_GetCurPicFileInfo
#ifndef MMI_PICVIEW_MINI_SUPPORT
#define MMIPICVIEW_IsPicArrayFromArrayLib   MMIAPIPICVIEW_IsPicArrayFromArrayLib
#endif
#define MMIPICVIEW_AddFileToPicArray		MMIAPIPICVIEW_AddFileToPicArray
#define MMIPICVIEW_GetAllPicArray			MMIAPIPICVIEW_GetAllPicArray
#define MMIPICVIEW_GetCurPicArray			MMIAPIPICVIEW_GetCurPicArray
#define MMIPICVIEW_GetFirstPos  			MMIAPIPICVIEW_GetFirstPos

#ifdef PIC_EDITOR_SUPPORT//norflashcut_pic 
#define MMIPICVIEW_GetAndResetEditPicIndex  MMIAPIPICVIEW_GetAndResetEditPicIndex
#endif

#ifdef PIC_PREVIEW_U200_SUPPORT 
#define MMIPICVIEW_DeletePreviewFile        MMIAPIPICVIEW_DeletePreviewFile
#define MMIPICVIEW_OpenSetWin               MMIAPIPICVIEW_OpenSetWin
#endif

#endif

/*****************************************************************************/
//  Description : init pic viewer module
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_InitModule(void);

/*****************************************************************************/
//  Description : init pic viewer nv
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_Init(void);

/*****************************************************************************/
//  Description : resset pic viewer factory settings
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_ResetFactorySettings(void);

/*****************************************************************************/
// 	Description : get slide effect
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC MMIPICVIEW_EFFECT_TYPE_E MMIAPIPICVIEW_GetSlideEffect(void);

/*****************************************************************************/
//  Description : open pic viewer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenPicViewer(void);

/*****************************************************************************/
//  Description : open pic viewer in photos directory
//  Global resource dependence : 
//  Author: kevinlou
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenMyPhotos(void);

/*****************************************************************************/
//  Description : close pic viewer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_ClosePicViewer(void);

/*****************************************************************************/
//  Description : handle picture list delete msg
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIAPIPICVIEW_HandleDelQueryWin(
                                                 MMI_WIN_ID_T        win_id, 
                                                 MMI_MESSAGE_ID_E    msg_id,
                                                 DPARAM              param
                                                 );
#ifdef PIC_EDITOR_SUPPORT    //norflashcut_pic     
//add for NEWMS00108303
/*****************************************************************************/
//  Description : get editpic index
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIPICVIEW_GetAndResetEditPicIndex(void);
//add for NEWMS00108303
#endif

/*****************************************************************************/
// 	Description : picture viewer is delete file
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_IsDeleteFiles(void);

/*****************************************************************************/
// 	Description : stop delete file
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_StopDeleteFiles(void);

/*****************************************************************************/
//  Description : update picture list
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_ReloadFile(void);

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_IsPicFolderWinOpen(void);

/*****************************************************************************/
// 	Description : get default path
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_GetDefaultPath( 
                                      MMIPICVIEW_DEFAULT_PATH_T* path_ptr
                                      );
/*****************************************************************************/
// 	Description : get my photos pic path
//	Global resource dependence : none
//  Author: kevin.lou
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_GetMyPhotosPath( 
                                      MMIPICVIEW_DEFAULT_PATH_T* path_ptr
                                      );

/*****************************************************************************/
// 	Description : show pic list in my photos path
//	Global resource dependence : none
//    Author: kevin.lou
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_EnterMyPhotos(void );

PUBLIC void MMIPICVIEW_SetEnterType(MMIPICVIEW_FROM_TYPE_E type);
PUBLIC MMIPICVIEW_FROM_TYPE_E MMIPICVIEW_GetEnterType(void);

#ifdef PIC_PREVIEW_U200_SUPPORT  
/*****************************************************************************/
//  Discription: delete cur preview file
//  Global resource dependence: none 
//  Author: yaye.jiang
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_DeletePreviewFile(MMI_CTRL_ID_T list_ctrl_id);

/*****************************************************************************/
//  Discription: open set win
//  Global resource dependence: none 
//  Author: yaye.jiang
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenSetWin(void);
#endif

/*****************************************************************************/
// 	Description : set default path
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_SetDefaultPath(
                                      const MMIPICVIEW_DEFAULT_PATH_T* path_ptr
                                      );

/*****************************************************************************/
//  Description : check the default path
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_IsDefaultPath(MMI_STRING_T string);

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : open pic viewer according to type
//  Global resource dependence : 
//  Author: hongying wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenPicViewerExt(MMIPICVIEW_PATH_TYPE_E type);
#endif

/*****************************************************************************/
// 	Description : refresh the iconlist groups orded in time
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_DeleteOrderInTime(MMI_CTRL_ID_T ctrl_id);

/*****************************************************************************/
//  Description : get the current preview  picture file info by index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_GetCurPicFileInfo( BOOLEAN  is_piclib,FILEARRAY_DATA_T   *file_info_ptr);
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : check the array is  from lib or not
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_IsPicArrayFromArrayLib(void);

#endif
/*****************************************************************************/
//  Description : 按照文件全路径，在filearray数组中增加一个数据
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_AddFileToPicArray(FILEARRAY   file_array,     //in, filearray值,
                                                                    const wchar *fullpathname);//in,添加的文件全路径 

/*****************************************************************************/
//  Description : get all picture file array
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY MMIAPIPICVIEW_GetAllPicArray(void);																	

/*****************************************************************************/
//  Description : get cur picture file array
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY MMIAPIPICVIEW_GetCurPicArray(void);

/*****************************************************************************/
//  Description : get the current preview  picture file info by index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIPICVIEW_GetFirstPos(FILEARRAY filearray);

#ifdef BAIDU_DRIVE_SUPPORT
/*****************************************************************************/
//  Description : Create one drive Auto upload text dialog window
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_CreateOneDriveConnectTextDialogWin(void);

/*****************************************************************************/
//  Description : Create one drive Auto upload text dialog window
//  Global resource dependence : 
//  Author: 
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPICVIEW_OpenPicViewerWin(void);
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
