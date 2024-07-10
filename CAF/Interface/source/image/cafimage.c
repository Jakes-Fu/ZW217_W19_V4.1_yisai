#ifdef DYNAMIC_MODULE_SUPPORT
/****************************************************************************
** File Name:      cafimage.c                                              *
** Author:         Andrew.Zhang                                            *
** Date:           03/02/2009                                              *
** Copyright:      2008 Spreatrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2009        Andrew.Zhang     Create
** 
****************************************************************************/


/**---------------------------------------------------------------------------*
**                         Dependencies                                      *
**---------------------------------------------------------------------------*/
#include "caf.h" 
#include "cafmodule.h"
#include "cafmodule_gen.h"
#include "cafimage.h"
#include "caffilesys.h"
#include "sci_types.h"
#include "os_api.h"
#include "mmk_app.h"
#include "mmi_theme.h"
#include "mmipub.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "guianim.h"
#include "guires.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
//最大图像文件尺寸定义
#define MAX_FILE_SIZE	(128 * 1024)


/**--------------------------------------------------------------------------*
**                         STURCT DEFINITION                                *
**--------------------------------------------------------------------------*/
typedef struct
{
    CAF_VTBL_T(IIMAGE_T)  *vtbl_ptr; // 函数列表
    CAF_GUID_T	guid;
    uint32	ref_num;
    int16   width;	//显示的宽度
    int16   heigth;	//显示的高度
	CAF_RESOURCE_INFO_T		resouce_info;
}CAFIMAGE_T;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/

//转换CAF文件路径到平台文件路径

LOCAL uint32 CAFIMAGE_Draw( IIMAGE_T *me_ptr, int32 x, int32 y );
LOCAL uint32 CAFIMAGE_GetInfo( IIMAGE_T *me_ptr, CAF_IMAGE_INFO_T * image_info_ptr );
LOCAL uint32 CAFIMAGE_SetParm( IIMAGE_T *me_ptr, int32 parm, int32 p1, int32 p2 );
LOCAL CAF_RESULT_E CAFIMAGE_ClassNew( 
                                   CAF_GUID_T guid, 
                                   void**     object_pptr 
                                   );
LOCAL CAF_RESULT_E CAFIMAGE_QueryInterface( 
                                         IIMAGE_T *me_ptr, 
                                         CAF_GUID_T guid, 
                                         void** ojbect_pptr 
                                         );
LOCAL uint32 CAFIMAGE_AddRef(
                           IIMAGE_T * me_ptr 
                           );
LOCAL uint32 CAFIMAGE_Release(
                            IIMAGE_T * me_ptr
                            );

LOCAL uint32 ReadFileData(IIMAGE_T *me_ptr);

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/
// functions table
LOCAL const CAF_VTBL_T(IIMAGE_T) s_image_vtbl =
{
    CAFIMAGE_QueryInterface,
    CAFIMAGE_AddRef,
    CAFIMAGE_Release,
    CAFIMAGE_Draw,
    CAFIMAGE_GetInfo,
    CAFIMAGE_SetParm
};

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                          functions definition                             */
/*---------------------------------------------------------------------------*/
LOCAL const CAF_STATIC_CLASS_T s_image_class_list[] = 
{
    { SPRD_GUID_IMAGE,      CAFIMAGE_ClassNew, 0 },
};

const CAF_STATIC_CLASS_INFO_T g_image_static_class_info = 
{ 
    (CAF_STATIC_CLASS_T*)s_image_class_list, 
        sizeof(s_image_class_list) / sizeof(s_image_class_list[0])
};

LOCAL CAF_RESULT_E CAFIMAGE_ClassNew( 
                                   CAF_GUID_T guid, 
                                   void**     object_pptr 
                                   )
{
    CAF_RESULT_E    result  = CAF_RESULT_NOT_SUPPORT;
    CAFIMAGE_T   *image_ptr = PNULL;
    
    if( SPRD_GUID_IMAGE == guid )
    {       
        image_ptr = SCI_ALLOC_APP(sizeof(CAFIMAGE_T));
        
        SCI_MEMSET(image_ptr, 0, sizeof(CAFIMAGE_T));
        image_ptr->vtbl_ptr = (CAF_VTBL_T(IIMAGE_T)*)&s_image_vtbl;
        
        image_ptr->guid       = guid;
        image_ptr->ref_num    = 1;
        
        result = CAF_RESULT_SUCCEEDED;
    }
    
    *object_pptr = image_ptr;
    
    return result;
}

LOCAL CAF_RESULT_E CAFIMAGE_QueryInterface( 
                                         IIMAGE_T *me_ptr, 
                                         CAF_GUID_T guid, 
                                         void** ojbect_pptr 
                                         )
{
    CAF_RESULT_E result = CAF_RESULT_FAILED;
    
    CAFIMAGE_T *image_ptr = (CAFIMAGE_T*)me_ptr;
    
    if ( IUNKNOWN_ID == guid ||  image_ptr->guid == guid )
    {
        *ojbect_pptr = image_ptr;
        
        result = CAF_RESULT_SUCCEEDED;
    }
    
    if( result == CAF_RESULT_SUCCEEDED )
    {
        IUNKNOWN_AddRef( *ojbect_pptr );
    }
    
    return result;
}

LOCAL uint32 CAFIMAGE_AddRef(
                           IIMAGE_T * me_ptr 
                           )
{
    return ++(((CAFIMAGE_T *)me_ptr)->ref_num);
}

LOCAL uint32 CAFIMAGE_Release(
                            IIMAGE_T * me_ptr
                            )
{   
    CAFIMAGE_T * image_ptr = (CAFIMAGE_T *)me_ptr;
    
    if ( --image_ptr->ref_num )
    {
        return( image_ptr->ref_num );
    }
    else
    {
		if(image_ptr->resouce_info.res_source == CAF_RESOURCE_SOURCE_FILE) { //图像来自单独的文件
			if(image_ptr->resouce_info.res_ptr != NULL) {
				SCI_FREE(image_ptr->resouce_info.res_ptr);
				image_ptr->resouce_info.res_ptr = NULL;
			}
			if(image_ptr->resouce_info.resinfo.resfile_ptr != NULL) {
				SCI_FREE(image_ptr->resouce_info.resinfo.resfile_ptr);
				image_ptr->resouce_info.resinfo.resfile_ptr = NULL;
			}						
		}
        SCI_FREE( image_ptr );   
        
        return 0;
    }
}

/*****************************************************************************/
//  Description:  draw image on the screen
//  Author: Andrew.Zhang
//  Param
//       me_ptr:IIMAGE_T object instance pointer
//       x:start point x
//       y:start point y
//  Return:
//       TRUE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL uint32 CAFIMAGE_Draw( IIMAGE_T *me_ptr, int32 x, int32 y )
{
	uint32 result = FALSE;
	GUI_LCD_DEV_INFO lcd_dev = {0};
	GUI_RECT_T image_rect = {0};
	GUI_RECT_T display_rect = {0};
	GUI_POINT_T point = {0};
	CAFIMAGE_T *image_ptr = (CAFIMAGE_T *)me_ptr;

	if(image_ptr->resouce_info.res_source == CAF_RESOURCE_SOURCE_FILE) { //图像来自单独的文件
		result = ReadFileData((IIMAGE_T *)image_ptr);
		if(result == FALSE)
		{
			return result;
		}
	}
	else if(image_ptr->resouce_info.res_source == CAF_RESOURCE_SOURCE_ID) { //图像来自资源包
		if (MMITHEME_CheckImageID(image_ptr->resouce_info.resinfo.res_id))
		{
			image_ptr->resouce_info.res_ptr = (void *)MMI_GetLabelImage(image_ptr->resouce_info.resinfo.res_id, image_ptr->resouce_info.handle, &image_ptr->resouce_info.res_size);
		}
	}
	else
	{
	}


    //set lcd dev
	//以后最好能用相关函数来赋值
    lcd_dev.lcd_id   = GUI_MAIN_LCD_ID;
    lcd_dev.block_id = GUI_BLOCK_MAIN;

	//设置显示相关参数
	point.x = (int16)x;
	point.y = (int16)y;

	display_rect.left = (int16)x;
	display_rect.top = (int16)y;

	if((image_ptr->width <= 0) || (image_ptr->heigth <= 0)) {
		CAF_IMAGE_INFO_T image_info;
		CAFIMAGE_GetInfo( (IIMAGE_T *)image_ptr, (CAF_IMAGE_INFO_T *)&image_info);
		display_rect.right = (int16)(display_rect.left + image_info.width);
		display_rect.bottom = (int16)(display_rect.top + image_info.heigth);
	}
	else
	{
		display_rect.right = (int16)(display_rect.left + image_ptr->width);
		display_rect.bottom = (int16)(display_rect.top + image_ptr->heigth);
	}

	image_rect.top = 0;
	image_rect.left = 0;
	image_rect.right = (int16)(display_rect.right - display_rect.left);
	image_rect.bottom = (int16)(display_rect.bottom - display_rect.top);

#if 0
	if(image_type == GUIANIM_TYPE_IMAGE_JPG) {
		//DisplayStaticJpg((GUI_LCD_DEV_INFO *)&lcd_dev, image_ptr->resouce_info.res_ptr, point, image_ptr->resouce_info.res_size, (GUI_RECT_T *)&image_rect, TRUE);
		result = TRUE;
	}
	else if(image_type == GUIANIM_TYPE_IMAGE_BMP)
	{
		//LCD_DisplayCompressedBmpInRect((GUI_LCD_DEV_INFO *)&lcd_dev, image_ptr->resouce_info.res_ptr, (GUI_RECT_T *)&image_rect, (GUI_RECT_T *)&display_rect);
		result = TRUE;
	}
	else
	{
	}
#endif

	result = GUIRES_DisplayImgByPtr(image_ptr->resouce_info.res_ptr, image_ptr->resouce_info.res_size, &point, (GUI_RECT_T *)&display_rect, (GUI_RECT_T *)&image_rect, (GUI_LCD_DEV_INFO *)&lcd_dev );

	return result;
}

/*****************************************************************************/
//  Description:  get image information
//  Author: Andrew.Zhang
//  Param
//       me_ptr:IIMAGE_T object instance pointer
//       image_info_ptr:image information pointer
//  Return:
//       TRUE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL uint32 CAFIMAGE_GetInfo( IIMAGE_T *me_ptr, CAF_IMAGE_INFO_T * image_info_ptr )
{
	uint32 result = FALSE;
	//uint16 img_width = 0;
	//uint16 img_heigth = 0;

	CAFIMAGE_T *image_ptr = (CAFIMAGE_T *)me_ptr;

	if(image_ptr->resouce_info.res_source == CAF_RESOURCE_SOURCE_FILE) { //图像来自单独的文件
		result = ReadFileData((IIMAGE_T *)image_ptr);
		if(result == FALSE)
		{
			return result;
		}
	}
	else if(image_ptr->resouce_info.res_source == CAF_RESOURCE_SOURCE_ID) { //图像来自资源包
		if (MMITHEME_CheckImageID(image_ptr->resouce_info.resinfo.res_id))
		{
			image_ptr->resouce_info.res_ptr = (void *)MMI_GetLabelImage(image_ptr->resouce_info.resinfo.res_id, image_ptr->resouce_info.handle, &(image_ptr->resouce_info.res_size));
		}
	}
	else
	{
	}

	//result = GUIANIM_GetImageInfoByData(image_ptr->resouce_info.res_ptr,&img_width,&img_heigth,image_ptr->resouce_info.res_size, GUIANIM_TYPE_UNKNOW);
	
    {
        GUIRES_IMG_SRC_T img_src = {0};
        GUIRES_IMG_INFO_T img_info = {0};

        img_src.data_ptr = (uint8 *)image_ptr->resouce_info.res_ptr;
        img_src.data_size = image_ptr->resouce_info.res_size;
    
        result = GUIRES_GetImgInfo(&img_src, &img_info);
        
        if(result == TRUE) {
		    image_info_ptr->width = img_info.width;
		    image_info_ptr->heigth = img_info.height;
	    }
    }

	return result;
}

/*****************************************************************************/
//  Description:  set image param
//  Author: Andrew.Zhang
//  Param
//       me_ptr:IIMAGE_T object instance pointer
//       parm:param type
//       p1:value have been defined by parm
//       p2:value have been defined by parm
//  Return:
//       TRUE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL uint32 CAFIMAGE_SetParm( IIMAGE_T *me_ptr, int32 parm, int32 p1, int32 p2 )
{
	CAFIMAGE_T *image_ptr = (CAFIMAGE_T *)me_ptr;
	
	switch(parm)
	{
	case CAFIMAGE_PARM_SIZE:
		image_ptr->width = (uint16)p1;
		image_ptr->heigth= (uint16)p2;
		break;
	case CAFIMAGE_PARM_IMAGE:
		//只有文件方式才需要释放这两个BUFFER
		if(image_ptr->resouce_info.res_source == CAF_RESOURCE_SOURCE_FILE) { //图像来自单独的文件
			if(image_ptr->resouce_info.res_ptr != NULL) {
				SCI_FREE(image_ptr->resouce_info.res_ptr);
				image_ptr->resouce_info.res_ptr = NULL;
			}
			if(image_ptr->resouce_info.resinfo.resfile_ptr != NULL) {
				SCI_FREE(image_ptr->resouce_info.resinfo.resfile_ptr);
				image_ptr->resouce_info.resinfo.resfile_ptr = NULL;
			}			
		}
		//image_ptr->resouce_info = *((CAF_RESOURCE_INFO_T *)p1);
		image_ptr->resouce_info.res_source = ((CAF_RESOURCE_INFO_T *)p1)->res_source;
		image_ptr->resouce_info.res_type = ((CAF_RESOURCE_INFO_T *)p1)->res_type;
		image_ptr->resouce_info.res_size = ((CAF_RESOURCE_INFO_T *)p1)->res_size;
		image_ptr->resouce_info.handle = ((CAF_RESOURCE_INFO_T *)p1)->handle;

		//copy filename
		if(image_ptr->resouce_info.res_source == CAF_RESOURCE_SOURCE_FILE) { //图像来自单独的文件
			image_ptr->resouce_info.resinfo.resfile_ptr = SCI_ALLOC_APP(IFILE_MAX_PATH*2+4);
			SCI_ASSERT(image_ptr->resouce_info.resinfo.resfile_ptr != NULL);

			SCI_ASSERT(TransferFileNameFromCAFToSys(image_ptr->resouce_info.resinfo.resfile_ptr, ((CAF_RESOURCE_INFO_T *)p1)->resinfo.resfile_ptr) == TRUE);
			image_ptr->resouce_info.res_ptr = NULL;
		}
		else if(image_ptr->resouce_info.res_source == CAF_RESOURCE_SOURCE_ID) { //图像来自资源文件
			image_ptr->resouce_info.resinfo.res_id = ((CAF_RESOURCE_INFO_T *)p1)->resinfo.res_id;
			image_ptr->resouce_info.res_ptr = NULL;
		}
		else if(image_ptr->resouce_info.res_source == CAF_RESOURCE_SOURCE_BUFFER) { //图像来自BUFFER
			image_ptr->resouce_info.resinfo.resfile_ptr = NULL;
			image_ptr->resouce_info.res_ptr = ((CAF_RESOURCE_INFO_T *)p1)->res_ptr;
		}
		else
		{
			SCI_ASSERT(0);
		}

		break;
	default:
		break;
	}

	return TRUE;
}

/*****************************************************************************/
//  Description:  read resource from file
//  Author: Andrew.Zhang
//  Param
//       me_ptr:IIMAGE_T object instance pointer
//  Return:
//       TRUE: Success
//       Other: Fail
//  Note: 
/*****************************************************************************/
LOCAL uint32 ReadFileData(IIMAGE_T *me_ptr)
{
	SFS_HANDLE handle = NULL;
	uint32 size = 0;
	uint32 bytes_read = 0;
	uint32 result = FALSE;
	CAFIMAGE_T *image_ptr = (CAFIMAGE_T *)me_ptr;

	if(image_ptr->resouce_info.res_source == CAF_RESOURCE_SOURCE_FILE) { //图像来自单独的文件
		if(image_ptr->resouce_info.res_ptr == NULL) {
			//读入文件
			handle = SFS_CreateFile(image_ptr->resouce_info.resinfo.resfile_ptr, SFS_MODE_READ | SFS_MODE_WRITE | SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655 */
			if(handle == NULL)
			{
				return result;
			}
			SFS_GetFileSize(handle, &size);
			if((size > MAX_FILE_SIZE) || (size <= 0)) {		/*lint !e775 */
				SFS_CloseFile(handle);
				return result;
			}
			image_ptr->resouce_info.res_ptr = SCI_ALLOC_APP(size + 4);
			SCI_ASSERT(image_ptr->resouce_info.res_ptr != NULL);
			SFS_SetFilePointer(handle, (__int64)0, SFS_SEEK_BEGIN);
			SFS_ReadFile(handle, (void *)image_ptr->resouce_info.res_ptr, size, (uint32 *)&bytes_read, NULL);
			SCI_ASSERT(bytes_read == size);
			image_ptr->resouce_info.res_size = size;
			SFS_CloseFile(handle);
			result = TRUE;
		}
		else
		{
			result = TRUE;
		}
	}

	return result;
}
#endif