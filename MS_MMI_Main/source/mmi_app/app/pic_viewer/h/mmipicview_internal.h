/*****************************************************************************
** File Name:      mmipicview_internal.h                                     *
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
#ifdef PIC_VIEWER_SUPPORT
#ifndef _MMIPICVIEW_INTERNAL_H_
#define _MMIPICVIEW_INTERNAL_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmipicview_export.h"
#include "mmifilearray_export.h"
#include "mmimultim_text.h"

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
#define MMIPICVIEW_SUFFIX_MAX_LEN           10  //include dot

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
//picture type
typedef enum
{
#ifdef BMP_DEC_SUPPORT
    MMIPICVIEW_PIC_TYPE_BMP,
    MMIPICVIEW_PIC_TYPE_WBMP,
#endif
#ifdef GIF_DEC_SUPPORT
    MMIPICVIEW_PIC_TYPE_GIF,
#endif
#ifdef PNG_DEC_SUPPORT
    MMIPICVIEW_PIC_TYPE_PNG,
#endif
	MMIPICVIEW_PIC_TYPE_JPG,
    MMIPICVIEW_PIC_TYPE_JPEG,
#ifdef DRM_SUPPORT
    MMIPICVIEW_PIC_TYPE_DM,
    MMIPICVIEW_PIC_TYPE_DCF,
#endif
    MMIPICVIEW_PIC_TYPE_MAX
} MMIPICVIEW_PIC_TYPE_E;

typedef enum
{
    PICVIEW_SOURCE_NONE,
    PICVIEW_SOURCE_ALL_LOCAL,
    PICVIEW_SOURCE_CAPTURED,
    PICVIEW_SOURCE_VIDEO,
    PICVIEW_SOURCE_CLOUD_THUMBNAIL,
    PICVIEW_SOURCE_CLOUD_DOWNLOADED,    
    PICVIEW_SOURCE_CLOUD_UPLOAD,      
    PICVIEW_SOURCE_TYPE_MAX
}PIC_SOURCE_TYPE;

//slide play time
typedef enum
{
    MMIPICVIEW_TIME_2S,     //2s
    MMIPICVIEW_TIME_3S,     //3s
    MMIPICVIEW_TIME_6S,     //6s
    MMIPICVIEW_TIME_10S,    //10s
    MMIPICVIEW_TIME_16S,    //16s
    MMIPICVIEW_TIME_20S,    //2s
    MMIPICVIEW_TIME_TYPE_MAX
} MMIPICVIEW_TIME_TYPE_E;

//view type
typedef enum
{
    MMIPICVIEW_VIEW_LIST,       //list
    MMIPICVIEW_VIEW_ICONLIST,   //icon list
    MMIPICVIEW_VIEW_MAX
} MMIPICVIEW_VIEW_TYPE_E;

typedef enum
{
    MMIPICVIEW_PIC_INIT,  //
    MMIPICVIEW_PIC_DECODING,  //pic is decoding
    MMIPICVIEW_PIC_DECODED,   // pic is decoded 
    MMIPICVIEW_PIC_MIN_ZOOM_OUT, //min zoom out
    MMIPICVIEW_PIC_MAX_ZOOM_IN, //max zoom in
    MMIPICVIEW_PIC_STATE_MAX
}MMIPICVIEW_PIC_STATE_E;

//#ifdef PIC_PREVIEW_U200_SUPPORT
typedef enum
{
    MMIPICVIEW_PDA_SETTING_SPLIT_1,
    MMIPICVIEW_PDA_SETTING_CHANGE_FOLDER,//MMIPICVIEW_PDA_SETTING_VIEW,
    MMIPICVIEW_PDA_SETTING_SPLIT_2,
    MMIPICVIEW_PDA_SETTING_EFFECT,
    MMIPICVIEW_PDA_SETTING_TIME,

    MMIPICVIEW_PDA_SETTING_MAX
}MMIPICVIEW_PDA_SETTING_TYPE_E;

//#endif

typedef enum
{
    MMIPICVIEW_PICOPE_POINT,//point to open file
    MMIPICVIEW_PICOPE_GLID,//glide to open file
	
    MMIPICVIEW_PICOPE_MAX	
}MMIPICVIEW_PICOPE_TYPE_E;

typedef enum
{
    MMIPICVIEW_START_LOAD_SIG,
    MMIPICVIEW_STOP_LOAD_SIG,
    MMIPICVIEW_LOAD_SIG_MAX
}MMIPICVIEW_LOAD_SIG_TYPE_E;

typedef struct
{
    SIGNAL_VARS
    void * data_ptr;
}MMIPICVIEW_LOAD_SIG_T;
/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
// 	Description : set slide effect
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SetSlideEffect(
                                      MMIPICVIEW_EFFECT_TYPE_E  slide_effect
                                      );

/*****************************************************************************/
// 	Description : get slide time
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC MMIPICVIEW_TIME_TYPE_E MMIPICVIEW_GetSlideTime(void);

/*****************************************************************************/
// 	Description : set slide time
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SetSlideTime(
                                    MMIPICVIEW_TIME_TYPE_E  slide_time
                                    );

/*****************************************************************************/
// 	Description : get view type
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC MMIPICVIEW_VIEW_TYPE_E MMIPICVIEW_GetViewType(void);

/*****************************************************************************/
// 	Description : set view type
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_SetViewType(
                                      MMIPICVIEW_VIEW_TYPE_E    view_type
                                      );

/*****************************************************************************/
//  Description : get all picture files number
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPICVIEW_GetAllPicNum(void);

/*****************************************************************************/
//  Description : create all picture file array
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_CreateAllPicArray(void);

/*****************************************************************************/
//  Description : destroy all picture file array
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_DestroyAllPicArray(void);
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : create picture files lib array include sub-folder 
//  Global resource dependence : 
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_CreatePicLibArray(void);

/*****************************************************************************/
//  Description : destroy  picture files lib array
//  Global resource dependence : 
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_DestroyPicLibArray(void);

/*****************************************************************************/
//  Description : get picture files  array of lib
//  Global resource dependence : 
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY MMIPICVIEW_GetPicLibArray(void);

/*****************************************************************************/
//  Description : initl picture file array form pic lib
//  Global resource dependence : 
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_InitPicArrayFromPicLib(uint32 cur_index_folder);
/*****************************************************************************/
//  Description : release picture file array from pic lib
//  Global resource dependence : 
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPICVIEW_ReleasePicArrayFromPicLib(void);

/*****************************************************************************/
//  Description : get all picture files number in pic lib
//  Global resource dependence : 
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPICVIEW_GetAllPicNumInLib(void);
#endif
#if 0//norflashcut_pic
/*****************************************************************************/
//  Description : get picture files number in cur Folder
//  Global resource dependence : 
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPICVIEW_GetAllPicNumInCurFolder(void);

#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : get all picture files number in pic lib
//  Global resource dependence :
//  Author:  
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPICVIEW_GetCurFolderIndex(void);//NEWMS00145771
#endif
/*****************************************************************************/
//  Description : get picture file info by index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_GetPicFileInfo(
                                         uint32             pic_index,      //in
                                         FILEARRAY_DATA_T   *file_info_ptr  //in/out
                                         );

/*****************************************************************************/
//  Description : delete picture file info by index from array
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_DeletePicFileInfo(
                                            uint32  pic_index   //in
                                            );

/*****************************************************************************/
//  Description : delete picture file info by index from array
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_ModifyPicFileInfo(
                                            uint32                  pic_index,  //in
                                            const FILEARRAY_DATA_T  *data_ptr   //in
                                            );

/*****************************************************************************/
//  Description : post update list msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_PostUpdateMsg(
                                     uint16 index
                                     );

/*****************************************************************************/
//  Description : get current index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPICVIEW_GetCurIndex(
                                     MMI_CTRL_ID_T  ctrl_id
                                     );

/*****************************************************************************/
//  Description : set current index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SetCurIndex(
                                   MMI_CTRL_ID_T    ctrl_id,
                                   uint16           cur_index
                                   );

/*****************************************************************************/
//  Description : set max selected number
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_SetMaxSelected(
                                         MMI_CTRL_ID_T  ctrl_id,
                                         uint16         selected_max_num
                                         );

/*****************************************************************************/
//  Description : get list markable
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_GetListMarkable(
                                          MMI_CTRL_ID_T ctrl_id
                                          );

/*****************************************************************************/
//  Description : set list markable
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_SetListMarkable(
                                          MMI_CTRL_ID_T ctrl_id,
                                          BOOLEAN       is_markable
                                          );

/*****************************************************************************/
//  Description : item is select
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_IsSelectItem(
                                       MMI_CTRL_ID_T    ctrl_id,
                                       uint16           index
                                       );

/*****************************************************************************/
//  Description : get selected item index
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIPICVIEW_GetSelectItemIndex(//selected number
                                            MMI_CTRL_ID_T   ctrl_id,    //in:
                                            uint16          *index_ptr, //in/out:may PNULL
                                            uint16		    array_len   //in:
                                            );

/*****************************************************************************/
//  Description : set selected item
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_SetSelectedItem(
                                          MMI_CTRL_ID_T ctrl_id,
                                          uint16        index,
                                          BOOLEAN       is_selected
                                          );

/*****************************************************************************/
//  Description : set selected all item or not
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_SetAllSelected(
                                         MMI_CTRL_ID_T  ctrl_id,
                                         BOOLEAN        is_selected
                                         );

/*****************************************************************************/
//  Description : set selected all item or not
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_RemoveItem(
                                     MMI_CTRL_ID_T  ctrl_id,
                                     uint16         index
                                     );
#if defined(DRM_SUPPORT)//norflashcut_pic
/*****************************************************************************/
//  Description : modify item icon
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_ModifyItemIcon(
                                         MMI_CTRL_ID_T  ctrl_id,
                                         uint16         index
                                         );
#endif

/*****************************************************************************/
//  Description : modify item text
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_ModifyItemText(
                                         MMI_CTRL_ID_T  ctrl_id,
                                         uint16         index
                                         );

/*****************************************************************************/
//  Description : handle picture glide tp down msg
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_IsTpOsdSoftkey(
                                         GUI_POINT_T    *point_ptr
                                         );

/*****************************************************************************/
//  Description : modify name,modify current preview anim
//  Global resource dependence : 
//  Author: Jassmine
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPICVIEW_ModifyName(
                                  uint16    item_index  //from 0
                                  );

/*****************************************************************************/
// 	Description : picture slide play
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SliePlay(
								MMI_CTRL_ID_T	ctrl_id
								);

#ifdef PICVIEW_VIEW_ZOOM
/*****************************************************************************/
//  Description : zoom picture
//  Global resource dependence : none
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_Zoom(
                            wchar   *full_path_ptr, //in:
                            uint16  full_path_len   //in:
                            );
#endif
#ifdef DRM_SUPPORT
/*****************************************************************************/
//  Description : is drm file by suffix name
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_IsDrmFile(
                                    wchar   *file_name_ptr, //in:
                                    uint16  file_name_len   //in:
                                    );

/*****************************************************************************/
//  Description : is drm file has right
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_IsDrmRight(
                                     wchar      *file_name_ptr, //in:
                                     uint16     file_name_len   //in:
                                     );

/*****************************************************************************/
//  Description : is preview drm picture file
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_IsPreview(
                                    BOOLEAN     is_prompt,  //in:
                                    uint32      item_index  //in:
                                    );

/*****************************************************************************/
//  Description : is send drm picture file
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_IsSend(
                                 BOOLEAN            is_sms,
                                 FILEARRAY_DATA_T   *file_data_ptr
                                 );

/*****************************************************************************/
//  Description : is delete drm picture file
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_IsDeleteDrm(
                                      MMI_CTRL_ID_T		ctrl_id    //in:
                                      );

/*****************************************************************************/
//  Description : is drm picture file set wallpaper
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_IsSetWallpaper(
                                         uint32     item_index  //in:
                                         );

/*****************************************************************************/
//  Description : add drm file detail
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_AddDrmDetail(
                                    wchar           *file_name_ptr, //in:
                                    uint16          file_name_len,  //in:
									MMI_WIN_ID_T	win_id,			//in:
                                    MMI_CTRL_ID_T   ctrl_id         //in:
                                    );

/*****************************************************************************/
//  Description : set if download drm rights file 
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SetIsDownloadRightsFile(BOOLEAN is_down);

/*****************************************************************************/
//  Description : get if download drm rights file 
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_GetIsDownloadRightsFile(void);
#endif

#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
//  Description : open pic list longok menu win
//  Global resource dependence : 
//  Author: xin.li
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPICVIEW_OpenPicListLongokOptWin(void);
#endif //PIC_PREVIEW_U200_SUPPORT

/*****************************************************************************/
//  Description : is picture file error and note the 0 size of file
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_isFileError(
                                      MMI_CTRL_ID_T   ctrl_id,
                                      uint32      	  item_index,  //in:
                                      MMIPICVIEW_PICOPE_TYPE_E  ope_type
                                      );
                                      
/*****************************************************************************/
//  Description : handle picture list mark option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void HandlePicListMarked(
                               MMI_CTRL_ID_T  list_ctrl_id
                               );

/*****************************************************************************/
//  Description : handle picture list mark all option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void HandlePicListMarkedAll(
                                  MMI_CTRL_ID_T  list_ctrl_id
                                  );

/*****************************************************************************/
//  Description : handle picture list mark all cancel option
//  Global resource dependence : none
//  Author: Jassmine.Meng
//  Note:
/*****************************************************************************/
PUBLIC void HandlePicListUnmarkedAll(
                                    MMI_CTRL_ID_T  list_ctrl_id
                                    );
PUBLIC MMI_RESULT_E HandlePicPreviewDelQueryWin(
                                               MMI_WIN_ID_T     win_id, 
                                               MMI_MESSAGE_ID_E msg_id,
                                               DPARAM           param
                                               );
                                    
#ifdef PIC_PREVIEW_U200_SUPPORT
/*****************************************************************************/
// 	Description : set pic list buttons visible
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPICVIEW_DestroyMarkMenuButtons(MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : set pic list buttons visible
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SetButtonsVisible(BOOLEAN is_visible);

#endif

/*****************************************************************************/
// 	Description : set if the list win is empty
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SetListEmpty(BOOLEAN is_empty);

/*****************************************************************************/
//  Description : release the memory of anim controls
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_ReleaseAnimMemory(void);

/*****************************************************************************/
//  Description : slide callback
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_RefreshAnimDisplay(uint32	item_index);

/*****************************************************************************/
// 	Description : get if the list win is open form DCamera
//	Global resource dependence :
//  Author: yaye.jiang
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_GetifListFromDc(void);

/*****************************************************************************/
// 	Description : get photo path of special udisk
//	Global resource dependence : none
//  Author: yaye.jiang
//	Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_GetFixeddiskPhotoPath(const wchar* dev_ptr, uint16 dev_len, MMIPICVIEW_DEFAULT_PATH_T* search_path);

/*****************************************************************************/
//  Description : Search all the Photo path of all disks 
//  Global resource dependence :
//  Author:  
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_SearchAllPhotos(MMI_WIN_ID_T win_id, MMIFMM_FILTER_T* file_filter, MMIFILE_DEVICE_E* fmm_dev);

PUBLIC void PicviewListAllEmptyShow( MMI_WIN_ID_T   win_id);
PUBLIC BOOLEAN MMIPICVIEW_IsVideoSuffix( FILEARRAY_DATA_T *fileinfo);

#ifdef CMCC_UI_STYLE
PUBLIC MMIPICVIEW_PATH_TYPE_E MMIPICVIEW_GetPathType(void);
#endif

PUBLIC void MMIPICVIEW_GetFixedFolderPhotoPath(const wchar* dev_ptr, uint16 dev_len, PIC_SOURCE_TYPE type,MMIPICVIEW_DEFAULT_PATH_T* search_path);
PUBLIC void MMIAPIPICVIEW_SetShowFolderPath(PIC_SOURCE_TYPE type);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif
#endif //#ifdef PIC_VIEWER_SUPPORT
