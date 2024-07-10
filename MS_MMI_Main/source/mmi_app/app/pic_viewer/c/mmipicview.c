/*****************************************************************************
** File Name:      mmipicview.c                                              *
** Author:                                                                   *
** Date:           04/20/2004                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2008      Jassmine              Creat
******************************************************************************/

#define _MMIPICVIEW_C_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_pic_viewer_trc.h"
#ifdef WIN32
#include "std_header.h"
#endif

#ifdef PIC_VIEWER_SUPPORT
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmipub.h"
#include "mmipicview_id.h"
#include "mmipicview_nv.h"
#include "mmifilearray_export.h"
#include "mmifmm_export.h"
#include "mmi_appmsg.h"
//#include "mmipicview_text.h"
#include "window_parse.h"
#include "mmipicview_internal.h"
#include "mmisd_export.h"
#include "mmipicview_menutable.h"
#include "mmipicview_position.h"
#include "mmi_nv.h"

#ifdef BAIDU_DRIVE_SUPPORT
#include "mmibaidu_export.h"
#endif

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

LOCAL FILEARRAY     s_mmipicview_pic_array = PNULL; //current path all picture files array
#ifndef MMI_PICVIEW_MINI_SUPPORT
LOCAL FILEARRAY     s_mmipicview_pic_lib      = PNULL; //picture files array include sub-folder
LOCAL uint32            cur_folder_index              = 0;
LOCAL BOOLEAN       is_array_from_lib                       = FALSE;
#ifndef PIC_PREVIEW_U200_SUPPORT
LOCAL uint32            s_cur_index              = 0;
#endif
#endif

LOCAL MMIPICVIEW_EFFECT_TYPE_E  s_mmipicview_slide_effect = MMIPICVIEW_EFFECT_ORDER;
LOCAL MMIPICVIEW_TIME_TYPE_E    s_mmipicview_slide_time = MMIPICVIEW_TIME_2S;
LOCAL MMIPICVIEW_VIEW_TYPE_E    s_mmipicview_view_type = MMIPICVIEW_VIEW_ICONLIST;

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : get default path
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void GetDefaultPath(
    MMIPICVIEW_DEFAULT_PATH_T* path_ptr
);

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
    //register menu
    MMIPICVIEW_RegMenuGroup();

    //register nv
    MMIPICVIEW_RegNv();

    //register win id name
    MMIPICVIEW_RegWinIdNameArr();
}

/*****************************************************************************/
//  Description : init pic viewer nv
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_Init(void)
{
    MN_RETURN_RESULT_E          return_value = MN_RETURN_FAILURE;
    MMIPICVIEW_TIME_TYPE_E      slide_time = MMIPICVIEW_TIME_2S;
    MMIPICVIEW_EFFECT_TYPE_E    slide_effect = MMIPICVIEW_EFFECT_ORDER;
    MMIPICVIEW_VIEW_TYPE_E      view_type = MMIPICVIEW_VIEW_ICONLIST;
    MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};

    //init slide effect
    MMINV_READ(MMINV_PICVIEW_SLIDE_EFFECT, &slide_effect, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_PICVIEW_SLIDE_EFFECT, &slide_effect);
    }
    s_mmipicview_slide_effect = slide_effect;

    //init slide time
    MMINV_READ(MMINV_PICVIEW_SLIDE_TIME, &slide_time, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_PICVIEW_SLIDE_TIME, &slide_time);
    }
    s_mmipicview_slide_time = slide_time;

    //init view type
    MMINV_READ(MMINV_PICVIEW_VIEW_TYPE, &view_type, return_value);
    if (MN_RETURN_SUCCESS != return_value)
    {
        MMINV_WRITE(MMINV_PICVIEW_VIEW_TYPE, &view_type);
    }
    s_mmipicview_view_type = view_type;

    //init default path
    MMIAPIPICVIEW_GetDefaultPath(&default_path);
}

/*****************************************************************************/
//  Description : resset pic viewer factory settings
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPIPICVIEW_ResetFactorySettings(void)
{
    MMIPICVIEW_TIME_TYPE_E      slide_time = MMIPICVIEW_TIME_2S;
    MMIPICVIEW_EFFECT_TYPE_E    slide_effect = MMIPICVIEW_EFFECT_ORDER;
    MMIPICVIEW_DEFAULT_PATH_T   default_path = {0};

    //reset slide effect
    MMIPICVIEW_SetSlideEffect(slide_effect);

    //reset slide time
    MMIPICVIEW_SetSlideTime(slide_time);

    //reset default path
    GetDefaultPath(&default_path);
    MMIAPIPICVIEW_SetDefaultPath(&default_path);
}

/*****************************************************************************/
// 	Description : get slide effect
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC MMIPICVIEW_EFFECT_TYPE_E MMIAPIPICVIEW_GetSlideEffect(void)
{
    return (s_mmipicview_slide_effect);
}

/*****************************************************************************/
// 	Description : get slide effect
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SetSlideEffect(
    MMIPICVIEW_EFFECT_TYPE_E  slide_effect
)
{
    if (slide_effect != s_mmipicview_slide_effect)
    {
        s_mmipicview_slide_effect = slide_effect;

        //write slide effect
        MMINV_WRITE(MMINV_PICVIEW_SLIDE_EFFECT, &slide_effect);
    }
}

/*****************************************************************************/
// 	Description : get slide time
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC MMIPICVIEW_TIME_TYPE_E MMIPICVIEW_GetSlideTime(void)
{
    return (s_mmipicview_slide_time);
}

/*****************************************************************************/
// 	Description : set slide time
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_SetSlideTime(
    MMIPICVIEW_TIME_TYPE_E  slide_time
)
{
    if ((MMIPICVIEW_TIME_2S > slide_time)
            || (MMIPICVIEW_TIME_TYPE_MAX <= slide_time)
       )
    {
        //SCI_TRACE_LOW:"MMIPICVIEW MMIPICVIEW_SetSlideTime slide_time=%d, overflow !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_200_112_2_18_2_46_18_0,(uint8*)"d", slide_time);
        return ;
    }

    if (slide_time != s_mmipicview_slide_time)
    {
        s_mmipicview_slide_time = slide_time;

        //write slide effect
        MMINV_WRITE(MMINV_PICVIEW_SLIDE_TIME, &slide_time);
    }
}

/*****************************************************************************/
// 	Description : get view type
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC MMIPICVIEW_VIEW_TYPE_E MMIPICVIEW_GetViewType(void)
{
    return (s_mmipicview_view_type);
}

/*****************************************************************************/
// 	Description : set view type
//	Global resource dependence : none
//  Author: Jassmine.Meng
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_SetViewType(
    MMIPICVIEW_VIEW_TYPE_E    view_type
)
{
    BOOLEAN     result = FALSE;

    if ((MMIPICVIEW_VIEW_LIST > view_type)
            || (MMIPICVIEW_VIEW_MAX <= view_type)
       )
    {
        result = FALSE;
        //SCI_TRACE_LOW:"MMIPICVIEW MMIPICVIEW_SetViewType view_type=%d, overflow!!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_237_112_2_18_2_46_18_1,(uint8*)"d", view_type);
        return result;
    }

    if (view_type != s_mmipicview_view_type)
    {
        s_mmipicview_view_type = view_type;

        //write view type
        MMINV_WRITE(MMINV_PICVIEW_VIEW_TYPE, &view_type);

        result = TRUE;
    }

    return (result);
}

/*****************************************************************************/
// 	Description : get default path
//	Global resource dependence : none
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
LOCAL void GetDefaultPath(
    MMIPICVIEW_DEFAULT_PATH_T* path_ptr
)
{
    MMIFILE_DEVICE_E fmm_dev =  0;
//    BOOLEAN      is_sd   = MMIAPISD_IsSDExist(&fmm_dev);
    const wchar* dev_ptr = PNULL;//is_sd ? ((MMI_DEVICE_SDCARD == fmm_dev) ? MMIFILE_DEVICE_SDCARD : MMIAPIFMM_GetDevicePath(MMI_DEVICE_SDCARD_1)) : MMIFILE_DEVICE_UDISK;
    uint16       dev_len = 0;//(uint16)(is_sd ? ((MMI_DEVICE_SDCARD == fmm_dev) ? MMIFILE_DEVICE_SDCARD_LEN : MMIAPIFMM_GetDevicePathLen(MMI_DEVICE_SDCARD_1)) : MMIFILE_DEVICE_UDISK_LEN);

    if (PNULL == path_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW GetDefaultPath path_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_273_112_2_18_2_46_18_2,(uint8*)"");
        return ;
    }
   fmm_dev = MMIAPIFMM_GetDefaultDisk();

    dev_ptr = MMIAPIFMM_GetDevicePath(fmm_dev);
    dev_len = MMIAPIFMM_GetDevicePathLen(fmm_dev);
    path_ptr->path_len = MMIFMM_PATHNAME_LEN;

#ifdef CMCC_UI_STYLE
    MMIAPIFMM_CombineFullPath(dev_ptr, dev_len,
                            MMIMULTIM_DIR_PHOTOS, (uint16)MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PHOTOS),
                            PNULL, 0,
                            path_ptr->path, &path_ptr->path_len);
#else
    MMIAPIFMM_CombineFullPath(dev_ptr, dev_len,
                            MMIMULTIM_DIR_PICTURE, (uint16)MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PICTURE),
                            PNULL, 0,
                            path_ptr->path, &path_ptr->path_len);
#endif
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
    //MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;
    GetDefaultPath(path_ptr);
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
    MN_RETURN_RESULT_E return_value = MN_RETURN_FAILURE;

    if (PNULL == path_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW MMIAPIPICVIEW_GetDefaultPath path_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_323_112_2_18_2_46_18_3,(uint8*)"");
        return ;
    }

    MMINV_READ(MMINV_PICVIEW_DEFAULT_PATH, path_ptr, return_value);

    if ((MN_RETURN_SUCCESS != return_value) ||
            (!MMIAPIFMM_IsFolderExist(path_ptr->path, path_ptr->path_len)))
    {
        GetDefaultPath(path_ptr);

        MMIAPIPICVIEW_SetDefaultPath(path_ptr);
    }
}

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
    if (PNULL == path_ptr)
    {
        //SCI_TRACE_LOW:"MMIPICVIEW MMIAPIPICVIEW_SetDefaultPath path_ptr is PNULL !!!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_348_112_2_18_2_46_18_4,(uint8*)"");
        return ;
    }

    MMINV_WRITE(MMINV_PICVIEW_DEFAULT_PATH, (MMIPICVIEW_DEFAULT_PATH_T *)path_ptr);
}

/*****************************************************************************/
//  Description : get all picture files number
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPICVIEW_GetAllPicNum(void)
{
	uint32  pic_num=0 , i= 0 , ret_val=0;
	FILEARRAY_DATA_T        file_info = {0};	

#ifndef MMI_PICVIEW_MINI_SUPPORT
    if (TRUE ==  is_array_from_lib)
    {
        if (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetLastFilePos(s_mmipicview_pic_lib, cur_folder_index))
        {
            pic_num = 1 + MMIAPIFILEARRAY_GetLastFilePos(s_mmipicview_pic_lib, cur_folder_index) - MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index);
        } 
    }
    else
#endif
    {
        pic_num = MMIAPIFILEARRAY_GetArraySize(s_mmipicview_pic_array);
    }

	SCI_TRACE_LOW("[MMIPIC]: MMIPICVIEW_GetAllPicNum count %d" , pic_num);

	ret_val = pic_num;
#ifdef BAIDU_DRIVE_SUPPORT
	//remove some specific folder's pic 

	for(i=0 ; i<pic_num ; i++)
	{
		SCI_MEMSET(&file_info , 0x00 , sizeof(FILEARRAY_DATA_T));
		
		MMIPICVIEW_GetPicFileInfo(i,&file_info);
		if(MMISRV_BAIDUDRV_is_in_BaiduFolder (PICVIEW_SOURCE_CLOUD_DOWNLOADED , file_info.filename , file_info.name_len))
		{
			ret_val--;
		}
	}

#endif

	SCI_TRACE_LOW("[MMIPIC]: MMIPICVIEW_GetAllPicNum ret_val %d" , ret_val);

    return (ret_val);
}

/*****************************************************************************/
//  Description : create all picture file array
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_CreateAllPicArray(void)
{
#ifndef MMI_PICVIEW_MINI_SUPPORT
    if (TRUE !=  is_array_from_lib)
#endif
    {
        if(PNULL == s_mmipicview_pic_array)
        {
            s_mmipicview_pic_array = MMIAPIFILEARRAY_Create();
        }
    }
}

/*****************************************************************************/
//  Description : destroy all picture file array
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_DestroyAllPicArray(void)
{
#ifndef MMI_PICVIEW_MINI_SUPPORT
    if (TRUE !=  is_array_from_lib)
#endif
    {
    	MMIAPIFMM_SearchFileStop();
        MMIAPIFILEARRAY_Destroy(&s_mmipicview_pic_array);
    }
#ifndef MMI_PICVIEW_MINI_SUPPORT
    else
    {
        is_array_from_lib = FALSE;
    }
#endif
}
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : check the array is  from lib or not
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_IsPicArrayFromArrayLib(void)   
{
  return   is_array_from_lib;
}

#endif
/*****************************************************************************/
//  Description : get all picture file array
//  Global resource dependence :
//  Author: James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY MMIAPIPICVIEW_GetAllPicArray(void)
{
    return s_mmipicview_pic_array;
}

/*****************************************************************************/
//  Description : get cur picture file array
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY MMIAPIPICVIEW_GetCurPicArray(void)
{
#ifndef MMI_PICVIEW_MINI_SUPPORT
    if (TRUE ==  is_array_from_lib)
    {
        return s_mmipicview_pic_lib;
    }
    else
#endif
    {
        return s_mmipicview_pic_array;
    }
}

/*****************************************************************************/
//  Description : get the current preview  picture file info by index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIAPIPICVIEW_GetFirstPos(FILEARRAY filearray)
{
#ifndef MMI_PICVIEW_MINI_SUPPORT
    if (TRUE ==  is_array_from_lib)
    {
        return MMIAPIFILEARRAY_GetFirstFilePos(filearray, cur_folder_index);
    }
    else
#endif
    {
        return 0;
    }
}

/*****************************************************************************/
//  Description : get the current preview  picture file info by index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_GetCurPicFileInfo(BOOLEAN  is_piclib,FILEARRAY_DATA_T   *file_info_ptr)
{
    BOOLEAN     result = FALSE;

#ifndef MMI_PICVIEW_MINI_SUPPORT
    if(is_piclib)
    {
       if (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index))
        {
           result = MMIAPIFILEARRAY_Read(s_mmipicview_pic_lib, (MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index)), file_info_ptr);
        }
    }
    else
#endif
    {
        result = MMIAPIFILEARRAY_Read(s_mmipicview_pic_array, 0, file_info_ptr);
    }

    //SCI_TRACE_LOW:"MMIPICVIEW_GetPicFileInfo:file_size is %d,name len %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_446_112_2_18_2_46_18_5,(uint8*)"dd", file_info_ptr->size, file_info_ptr->name_len);
    if (0 == file_info_ptr->name_len)
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : add  picture file to pic array
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_AddFileToPicArray(FILEARRAY   file_array,     
                                                                    const wchar *fullpathname)
{
    SFS_FIND_DATA_T   find_data={0};
    FILEARRAY_DATA_T    f_array_data={0};
    MMIFILE_HANDLE         handle_result =   0;   
    BOOLEAN result=FALSE;
    
    if(NULL == file_array
        || fullpathname==PNULL)
    {
        return FALSE;
    }
    handle_result=MMIAPIFMM_FindFirstFile(fullpathname, MMIAPICOM_Wstrlen(fullpathname), &find_data);
    if(SFS_INVALID_HANDLE != handle_result)
    {
        MMIAPICOM_Wstrcpy(f_array_data.filename, fullpathname);
        f_array_data.name_len=MMIAPICOM_Wstrlen(fullpathname);

        f_array_data.type = FILEARRAY_TYPE_FILE;
        f_array_data.size = find_data.length;
        f_array_data.modify_date.year = find_data.modify_Date.year;
        f_array_data.modify_date.mon = find_data.modify_Date.mon;
        f_array_data.modify_date.mday = find_data.modify_Date.mday;

        f_array_data.modify_time.hour = find_data.modify_time.hour;
        f_array_data.modify_time.min = find_data.modify_time.min;
        f_array_data.modify_time.sec = find_data.modify_time.sec;
        result = MMIAPIFILEARRAY_Add(file_array, &f_array_data);
    }
    MMIAPIFMM_FindClose(handle_result);
    return result;
       
}




/*****************************************************************************/
//  Description : get picture file info by index
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_GetPicFileInfo(
    uint32             pic_index,      //in
    FILEARRAY_DATA_T   *file_info_ptr  //in/out
)
{
    BOOLEAN     result = FALSE;
    
    if(PNULL == file_info_ptr)
    {
        return FALSE;
    }
#ifndef MMI_PICVIEW_MINI_SUPPORT
    if (TRUE ==  is_array_from_lib)
    {
        if  (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index))
        {
            result = MMIAPIFILEARRAY_Read(s_mmipicview_pic_lib, (pic_index + MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index)), file_info_ptr);
        }
    }
    else
#endif
    {
        result = MMIAPIFILEARRAY_Read(s_mmipicview_pic_array, pic_index, file_info_ptr);
    }
    //SCI_TRACE_LOW:"MMIPICVIEW_GetPicFileInfo:file_size is %d,name len %d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIPICVIEW_523_112_2_18_2_46_18_6,(uint8*)"dd", file_info_ptr->size, file_info_ptr->name_len);
    if (0 == file_info_ptr->name_len)
    {
        result = FALSE;
    }

    return (result);
}

/*****************************************************************************/
//  Description : delete picture file info by index from array
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_DeletePicFileInfo(
    uint32  pic_index   //in
)
{
    BOOLEAN     result = FALSE;
#ifndef MMI_PICVIEW_MINI_SUPPORT
    if (TRUE ==  is_array_from_lib)
    {
         if  (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index))
        {
                result = MMIAPIFILEARRAY_Delete(s_mmipicview_pic_lib, (pic_index + MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index)));
        }
    }
    else
#endif
    {
        result = MMIAPIFILEARRAY_Delete(s_mmipicview_pic_array, pic_index);
    }

    return (result);
}

/*****************************************************************************/
//  Description : delete picture file info by index from array
//  Global resource dependence :
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_ModifyPicFileInfo(
    uint32                  pic_index,  //in
    const FILEARRAY_DATA_T  *data_ptr   //in
)
{
    BOOLEAN     result = FALSE;
    
    if(PNULL == data_ptr)
    {
        return FALSE;
    }
#ifndef MMI_PICVIEW_MINI_SUPPORT
    if (TRUE ==  is_array_from_lib)
    {
            if  (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index))
            {
                result = MMIAPIFILEARRAY_Modify(s_mmipicview_pic_lib, \
                                                       (pic_index + MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index)),\
                                                        (FILEARRAY_DATA_T    *)data_ptr\
                                                        );
            }
    }
    else
#endif
    {
        result = MMIAPIFILEARRAY_Modify(s_mmipicview_pic_array, pic_index, (FILEARRAY_DATA_T *)data_ptr);
    }

    return (result);
}

/*****************************************************************************/
//  Description : check the default path
//  Global resource dependence :
//  Author: haiwu.chen
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPIPICVIEW_IsDefaultPath(MMI_STRING_T string)
{
    MMIPICVIEW_DEFAULT_PATH_T  default_path = {0};

    if (0 == string.wstr_len)
    {
        return FALSE;
    }

    //get default path
    MMIAPIPICVIEW_GetDefaultPath(&default_path);

    if (default_path.path_len != string.wstr_len)
    {
        return FALSE;
    }

    if (0 != MMIAPICOM_Wstrncmp(default_path.path, string.wstr_ptr, default_path.path_len))
    {
        return FALSE;
    }

    return TRUE;
}
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : create picture files lib array include sub-folder
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_CreatePicLibArray(void)
{
    if(PNULL == s_mmipicview_pic_lib)
    {
        s_mmipicview_pic_lib = MMIAPIFILEARRAY_Create();
    }
}

/*****************************************************************************/
//  Description : destroy  picture files lib array
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_DestroyPicLibArray(void)
{
	MMIAPIFMM_SearchFileStop();
    MMIAPIFILEARRAY_Destroy(&s_mmipicview_pic_lib);
}

/*****************************************************************************/
//  Description : get picture files  array of total pic lib
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC FILEARRAY MMIPICVIEW_GetPicLibArray(void)
{
    return s_mmipicview_pic_lib;
}
/*****************************************************************************/
//  Description : Create picture file array from pic lib
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_InitPicArrayFromPicLib(uint32 cur_index_folder)
{
    cur_folder_index            = cur_index_folder;
#ifndef PIC_PREVIEW_U200_SUPPORT
	s_cur_index					= cur_index_folder + 1;
#endif
    is_array_from_lib          = TRUE;
    return;
}
/*****************************************************************************/
//  Description : release picture file array from pic lib
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPICVIEW_ReleasePicArrayFromPicLib(void)
{
    is_array_from_lib   = FALSE;
#ifdef PIC_PREVIEW_U200_SUPPORT
    return cur_folder_index;
#else
	return s_cur_index;
#endif
}
/*****************************************************************************/
//  Description : get all picture files number in pic lib
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPICVIEW_GetAllPicNumInLib(void)
{
    uint32  pic_num = 0;

    pic_num = MMIAPIFILEARRAY_GetArraySize(s_mmipicview_pic_lib);

    return (pic_num);
}
#endif
#if 0//norflashcut_pic
/*****************************************************************************/
//  Description : get picture files number in cur Folder
//  Global resource dependence :
//  Author: kevin.lou
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPICVIEW_GetAllPicNumInCurFolder(void)
{
    uint32  pic_num = 0;
    //FILEARRAY_DATA_T  data_t;

    //MMIAPIFILEARRAY_ReadFolder(s_mmipicview_pic_lib, cur_folder_index, &data_t);
    if (TRUE ==  is_array_from_lib)
    {
        if (MMIFMM_INVALID_INDEX != MMIAPIFILEARRAY_GetLastFilePos(s_mmipicview_pic_lib, cur_folder_index))
        {
            pic_num = 1 + MMIAPIFILEARRAY_GetLastFilePos(s_mmipicview_pic_lib, cur_folder_index) - MMIAPIFILEARRAY_GetFirstFilePos(s_mmipicview_pic_lib, cur_folder_index);
        } 
    }

    
    return (pic_num);
}
#endif
#ifndef MMI_PICVIEW_MINI_SUPPORT
/*****************************************************************************/
//  Description : get all picture files number in pic lib
//  Global resource dependence :
//  Author:  
//  Note:
/*****************************************************************************/
PUBLIC uint32 MMIPICVIEW_GetCurFolderIndex(void)//NEWMS00145771
{
#ifdef PIC_PREVIEW_U200_SUPPORT
    return cur_folder_index;
#else
	return s_cur_index;
#endif
}
#endif
/*****************************************************************************/
// 	Description : get photo path of special udisk
//	Global resource dependence : none
//  Author: yaye.jiang
//	Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_GetFixeddiskPhotoPath(const wchar* dev_ptr, uint16 dev_len, MMIPICVIEW_DEFAULT_PATH_T* search_path)
{
    SCI_MEMSET(search_path->path, 0, (MMIFMM_PATHNAME_LEN+1) * sizeof(wchar));
    search_path->path_len = MMIFMM_PATHNAME_LEN;

#ifdef CMCC_UI_STYLE
    MMIAPIFMM_CombineFullPath(dev_ptr, dev_len,
                            MMIMULTIM_DIR_PHOTOS, (uint16)MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PHOTOS),
                            PNULL, 0,
                            search_path->path, &search_path->path_len);
#else
    MMIAPIFMM_CombineFullPath(dev_ptr, dev_len,
                            MMIMULTIM_DIR_PICTURE, (uint16)MMIAPICOM_Wstrlen(MMIMULTIM_DIR_PICTURE),
                            PNULL, 0,
                            search_path->path, &search_path->path_len);
#endif
}

/*****************************************************************************/
// 	Description : get photo path of special folder
//	Global resource dependence : none
//  Author: zhuyongjing 
//	Note:
/*****************************************************************************/
PUBLIC void MMIPICVIEW_GetFixedFolderPhotoPath(const wchar* dev_ptr, uint16 dev_len, PIC_SOURCE_TYPE type,MMIPICVIEW_DEFAULT_PATH_T* search_path)
{
	wchar *w_folder_path_ptr = PNULL;

	SCI_MEMSET(search_path->path, 0, (MMIFMM_PATHNAME_LEN+1) * sizeof(wchar));
	search_path->path_len = MMIFMM_PATHNAME_LEN;

	switch(type)
	{
#ifdef BAIDU_DRIVE_SUPPORT
		case PICVIEW_SOURCE_CLOUD_THUMBNAIL :
			w_folder_path_ptr = MMIMULTIM_DIR_PIC_BAIDUDRV_THUMBNAIL;
			break;
			
		case PICVIEW_SOURCE_CLOUD_UPLOAD:
			w_folder_path_ptr = MMIMULTIM_DIR_PIC_BAIDUDRV_UPLOAD;
			break;

		case PICVIEW_SOURCE_CLOUD_DOWNLOADED:
			w_folder_path_ptr = MMIMULTIM_DIR_PIC_BAIDUDRV_DOWNLOAD;
			break;
#endif
;
		case PICVIEW_SOURCE_VIDEO:
			w_folder_path_ptr = MMIMULTIM_DIR_MOVIE;
			break;
	}

	if(PNULL != w_folder_path_ptr)
	{
	        MMIAPIFMM_CombineFullPath(dev_ptr, dev_len,
	                            w_folder_path_ptr, (uint16)MMIAPICOM_Wstrlen(w_folder_path_ptr),
	                            PNULL, 0,
	                            search_path->path, &search_path->path_len);
	}
	else{
        SCI_TRACE_LOW("MMIPICVIEW_GetFixedFolderPhotoPath err type=%d",type);
    	}

}
/*****************************************************************************/
//  Description : Search all the Photo path of all disks 
//  Global resource dependence :
//  Author:  
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIPICVIEW_SearchAllPhotos(MMI_WIN_ID_T win_id, MMIFMM_FILTER_T* file_filter, MMIFILE_DEVICE_E* fmm_dev)
{
	BOOLEAN						return_val=FALSE;
	MMIFILE_DEVICE_E*			search_dev  = fmm_dev;
	MMIPICVIEW_DEFAULT_PATH_T 	search_path = {0};
	wchar* 						dev_ptr = PNULL;
    uint16 						dev_len = 0;
	

	if(PNULL == fmm_dev)
	{
		SCI_TRACE_LOW("MMIPICVIEW_SearchAllPhotos &searching_device = pnull !!");
		return return_val;
	}
    for(; *search_dev < MMI_DEVICE_NUM; (*search_dev)++)
    {
    	dev_ptr = MMIAPIFMM_GetDevicePath(*search_dev);
		dev_len = MMIAPIFMM_GetDevicePathLen(*search_dev);

        if(MMIAPIFMM_GetDeviceStatus(dev_ptr, dev_len))
        {
			MMIPICVIEW_GetFixeddiskPhotoPath(dev_ptr, dev_len, &search_path);

			MMIAPIFMM_SearchFileInPathAndSort(search_path.path,
		                        search_path.path_len,
		                        file_filter,
		                        FALSE,
		                        FUNC_FIND_FILE,
		                        MMIAPIPICVIEW_GetAllPicArray(),
		                        win_id,
		                        MSG_PICVIEWER_LOAD_CONTINUE,
		                        FILEARRAY_SORT_TIME_CREATE_DESCEND);
			(*search_dev)++;
			return_val=TRUE;
			break;
        }
	}

    return return_val;
}
#endif //#ifdef PIC_VIEWER_SUPPORT
