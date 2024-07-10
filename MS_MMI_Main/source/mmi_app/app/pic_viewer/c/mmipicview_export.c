/*****************************************************************************
** File Name:      mmipicview_export.c                                       *
** Author:                                                                   *
** Date:           15/04/2012                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmipicview_export file               *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008       Jassmine           Create
******************************************************************************/
#ifndef PIC_VIEWER_SUPPORT
#define _MMIPICVIEW_EXPORT_C_ 

#include "mmipicview_export.h"
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init pic viewer module
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_InitModule(void)
{
}

/*****************************************************************************/
//  Description : init pic viewer nv
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_Init(void)
{
}

/*****************************************************************************/
//  Description : resset pic viewer factory settings
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_ResetFactorySettings(void)
{
}

/*****************************************************************************/
// 	Description : get slide effect
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC MMIPICVIEW_EFFECT_TYPE_E MMIAPIPICVIEW_GetSlideEffect(void)
{
	return MMIPICVIEW_EFFECT_ORDER;
}

/*****************************************************************************/
//  Description : open pic viewer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenPicViewer(void)
{
}

/*****************************************************************************/
//  Description : open pic viewer in photos directory
//  Global resource dependence : 
//  Author: kevinlou
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenMyPhotos(void)
{
}

/*****************************************************************************/
//  Description : close pic viewer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_ClosePicViewer(void)
{
}

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
                                                 )
{
	return MMI_RESULT_FALSE;
}
#ifdef PIC_EDITOR_SUPPORT    //norflashcut_pic     
//add for NEWMS00108303
/*****************************************************************************/
//  Description : get editpic index
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIPICVIEW_GetAndResetEditPicIndex(void)
{
	return 0;
}
//add for NEWMS00108303
#endif

/*****************************************************************************/
// 	Description : picture viewer is delete file
//	Global resource dependence : 
//  Author: Jassmine
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_IsDeleteFiles(void)
{
	return FALSE;
}

/*****************************************************************************/
// 	Description : stop delete file
//	Global resource dependence : 
//  Author: 
//	Note: 
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_StopDeleteFiles(void)
{
}

/*****************************************************************************/
//  Description : update picture list
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_ReloadFile(void)
{
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_IsPicFolderWinOpen(void)
{
	return FALSE;
}

/*****************************************************************************/
// 	Description : get default path
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_GetDefaultPath( 
                                      MMIPICVIEW_DEFAULT_PATH_T* path_ptr
                                      )
{
}

/*****************************************************************************/
// 	Description : get my photos pic path
//	Global resource dependence : none
//  Author: kevin.lou
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_GetMyPhotosPath( 
                                      MMIPICVIEW_DEFAULT_PATH_T* path_ptr
                                      )
{
}

/*****************************************************************************/
// 	Description : show pic list in my photos path
//	Global resource dependence : none
//    Author: kevin.lou
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_EnterMyPhotos(void )
{
}

#ifdef PIC_PREVIEW_U200_SUPPORT  
/*****************************************************************************/
//  Discription: delete cur preview file
//  Global resource dependence: none 
//  Author: yaye.jiang
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_DeletePreviewFile(MMI_CTRL_ID_T list_ctrl_id)
{
}

/*****************************************************************************/
//  Discription: open set win
//  Global resource dependence: none 
//  Author: yaye.jiang
//  Note:
//
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenSetWin(void)
{
}
#endif

/*****************************************************************************/
// 	Description : set default path
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_SetDefaultPath(
                                      const MMIPICVIEW_DEFAULT_PATH_T* path_ptr
                                      )
{
}

/*****************************************************************************/
//  Description : check the default path
//  Global resource dependence : 
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_IsDefaultPath(MMI_STRING_T string)
{
	return FALSE;
}

#ifdef CMCC_UI_STYLE
/*****************************************************************************/
//  Description : open pic viewer according to type
//  Global resource dependence : 
//  Author: hongying wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_OpenPicViewerExt(MMIPICVIEW_PATH_TYPE_E type)
{
}
#endif

/*****************************************************************************/
//  Description : get the current preview  picture file info by index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_GetCurPicFileInfo( BOOLEAN  is_piclib,FILEARRAY_DATA_T   *file_info_ptr)
{
	return FALSE;
}

/*****************************************************************************/
//  Description : check the array is  from lib or not
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_IsPicArrayFromArrayLib(void)
{
	return FALSE;
}

/*****************************************************************************/
//  Description : 按照文件全路径，在filearray数组中增加一个数据
//  Global resource dependence : 
//  Author: liqing.peng
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_AddFileToPicArray(FILEARRAY   file_array,     //in, filearray值,
                                                                    const wchar *fullpathname)//in,添加的文件全路径 
{
	return FALSE;
}

/*****************************************************************************/
//  Description : get all picture file array
//  Global resource dependence : 
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY MMIAPIPICVIEW_GetAllPicArray(void)
{
	return PNULL;
}

/*****************************************************************************/
//  Description : get cur picture file array
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY MMIAPIPICVIEW_GetCurPicArray(void)
{
    return PNULL;
}

/*****************************************************************************/
//  Description : get the current preview  picture file info by index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIPICVIEW_GetFirstPos(FILEARRAY filearray)
{
    return 0;
}
#endif

