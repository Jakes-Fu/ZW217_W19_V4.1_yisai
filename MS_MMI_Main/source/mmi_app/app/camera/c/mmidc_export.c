/*****************************************************************************
** File Name:      mmipic_edit_export.c                                      *             *
** Author:                                                                   *
** Date:           04/02/2010                                                *
** Copyright:      2008 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe pic editor                  *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2010       robert.wang         Create                                  *
******************************************************************************/


/**--------------------------------------------------------------------------*
**                         Include Files                                     *
**---------------------------------------------------------------------------*/
//#include "std_header.h"
//#include "sci_types.h"
//#include "os_api.h"
#include "mmi_app_camera_trc.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#ifndef CAMERA_SUPPORT
#include "mmidc_internal.h"
#endif
#include "mmidc_export.h"
#include "mmk_app.h"


#ifndef CAMERA_SUPPORT
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                 *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                          TYPE AND CONSTANT                                *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                  *
**---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                 *
**---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
**                        LOCAL FUNCTION DECLARE                             *
**---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                               *
**---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : open preview window
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_OpenForChipTest(void)
{
    return FALSE;
}

/*****************************************************************************/
// 	Description : init camera module
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_InitModule(void)
{
    return ;
}
                  
/*****************************************************************************/
// 	Description : open preview window
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_OpenPhotoWin(void)
{	
    return FALSE;
}

/*****************************************************************************/
// 	Description : open preview window
//	Global resource dependence : none
//  Author: 
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_OpenVideoWin(void)
{	
    return FALSE;
}

/*****************************************************************************/
// 	Description : get camera is open
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_IsOpened(void)
{
    return FALSE;
}

/*****************************************************************************/
// 	Description : interrupt camera
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_Interrupt(void)
{
    return ;
}

/*****************************************************************************/
// 	Description : close camera
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_Exit(void)
{
    return ;
}

/*****************************************************************************/
// 	Description : reset setting value by factory
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_Setting_SetFactoryValue(void)
{
    return ;
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_AllocSettingMemory(void)
{
    return ;
}

/*****************************************************************************/
// 	Description : init menu and icon
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC int32 MMIAPIDC_InitOSDMenuIcon(void)
{
    return 0;
}

/*****************************************************************************/
// 	Description : init and get icon handle
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC DC_ICONS_OP* MMIAPIDC_InitIcon(void)
{
    return PNULL;
}

/*****************************************************************************/
// 	Description : init menu and icon
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_FreeGUI(void)
{
    return ;
}

/*****************************************************************************/
// 	Description : save dc settings before exit from dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_FreeSettingMemory(void)
{
    return ;
}

/*****************************************************************************/
// 	Description : get change folder state
//	Global resource dependence : none
//  Author: robert.wang
//   Date: 2009-9-8
//	Note:  查看图片管理中浏览默认路径下的图片
//     应允许浏览其他文件夹的图片，以此来区别
//    相关联的函数 mmipicview_wintab.c   
//    HandleLoadAllPicWinMsg() ,HandlePiclistOptWinMsg()
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_GetChangeFolderState(void)    
{
	return FALSE;
}

/*****************************************************************************/
// 	Description : set change folder state
//	Global resource dependence : none
//  Author: robert.wang
//   Date: 2009-9-8
//	Note:  查看图片管理中浏览默认路径下的图片
//     应允许浏览其他文件夹的图片，以此来区别
//    相关联的函数 mmipicview_wintab.c   
//    HandleLoadAllPicWinMsg() ,HandlePiclistOptWinMsg()
/*****************************************************************************/
PUBLIC void MMIAPIDC_SetChangeFolderState(BOOLEAN is_changed)    
{
	return ;
}

/*****************************************************************************/
// 	Description : called by reference after snapshot
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC wchar* MMIAPIDC_GetPhotoSavePath(void)
{
	return PNULL;
}

/*****************************************************************************/
// 	Description : get current save photo index
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIDC_GetSavePhotoIndex(
                                      uint32        pic_num,    //in
                                      FILEARRAY     pic_array   //in
                                      )
{
    return (0);
}

/*****************************************************************************/
// 	Description : dc engineer mode
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseStoreWin(void)
{
    return ;
}

/*****************************************************************************/
//  Description : get dc modul sensor type by id
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC DCAMERA_IMG_SENSOR_TYPE_E  MMIAPIDC_GetDCModulSensorTypeById(int32 id)
{
    return 0;
}

/*****************************************************************************/
//  Description : get dc modul sensor type
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC DCAMERA_IMG_SENSOR_TYPE_E  MMIAPIDC_GetDCModulSensorType(void)
{
    return 0;
}

/*****************************************************************************/
// 	Description : mmidcsrv open dc main window
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDCSRV_OpenMainWin(MMIDCSRV_WORK_PARAM_T *work_param_ptr /*in*/
                                             )
{
    return FALSE;
}

/*****************************************************************************/
//  Description : init sensor
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_InitSensor(BOOLEAN is_force_init,
                                         DCAMERA_CAM_PARAM_T *sensor_param_ptr /*out*/
                                         )
{
    return FALSE;
}

/*****************************************************************************/
// 	Description : init camera module parameter
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPIDC_Init(void)
{
    ;
}

/*****************************************************************************/
// 	Description : get used lcd for camera
//	Global resource dependence : none
//  Author: jesse.yu
//	Note:
/*****************************************************************************/
PUBLIC uint32 MMIDC_GetLcdID(void)
{
	return 0;
}
#endif //end NOT CAMERA_SUPPORT

/*****************************************************************************/
// 	Description : create capture photo name
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIDC_GetFileIDString(char* file_name, const char* prefix, uint32 id, uint32 id_max, const char* file_type)
{
    uint16 id_len = 0;
    uint16 id_max_len = 0;
    char string_max[20] = {0};
    uint32 i = 0;
 
    if(id >= id_max)
    {
        //SCI_TRACE_LOW:"[MMIDC] MMIAPIDC_GetFileIDString id >= id_max"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_EXPORT_243_112_2_18_2_8_28_30,(uint8*)"");
        id = id_max - 100;
    }

    id_len = sprintf(file_name, "%d", id);
    id_max_len = sprintf(string_max, "%d", id_max);
    for(i = 0; i < id_len; i++)
    {
        string_max[id_max_len - id_len + i] = file_name[i];
    }
    string_max[0] = '_';
    string_max[id_max_len] = '.';
    id_len = sprintf(file_name, "%s%s%s", prefix, string_max, file_type);
    return id_len;
}

/*****************************************************************************/
//  Description : combine path according to root, dir, name
//  Global resource dependence :                                
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIAPIDC_CombinePathName(wchar* path_name_ptr, uint16 path_name_len, const wchar* file_path,
                                    const wchar* file_name)
{
    uint16 file_path_len = 0;

    if (PNULL == path_name_ptr)
    {
        //SCI_TRACE_LOW:"[MMIDC] MMIAPIDC_CombinePathName path = PNULL"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIDC_EXPORT_270_112_2_18_2_8_28_31,(uint8*)"");
        return 0;
    }

    SCI_MEMSET(path_name_ptr, 0, (path_name_len * sizeof(wchar)));
    MMIAPICOM_Wstrcpy(path_name_ptr, file_path);
    file_path_len = MMIAPICOM_Wstrlen(file_path);
    *(path_name_ptr + file_path_len) = '\\';
    MMIAPICOM_Wstrcpy(path_name_ptr + file_path_len + 1, file_name);
    return (uint16)MMIAPICOM_Wstrlen(path_name_ptr);
}

#if !defined(MMIDC_F_WORK_MODE) 
/*****************************************************************************/
//  Description : start dc work mode
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC MMIDC_WORK_ERROR_E MMIAPIDC_StartWork(MMIDC_WORK_PARAM_T *work_param_ptr /*in*/
                                                    )
{
    return MMIDC_WORK_ERROR;
}
#endif

#if !defined(MMIDC_F_U_CAMERA)
/*****************************************************************************/
//  Description : open u camera window
//  Global resource dependence : none
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIDC_OpenUCameraPhotoWin(void)
{
    return FALSE;
}

#endif


