#include "mmi_app_wre_boot_trc.h"
#ifdef WRE_SUPPORT
/******************************************************************************
 ** File Name:      adaptor_image.c                                          *
 ** Author:         RiancyZhang                                              *
 ** DATE:           30/12/2010                                                *
 ** Copyright:      2010 Spreadtrum, Incorporated. All Rights Reserved.       *
 ** Description:    This file wraps image-layer for mocor os				       *
 **																              *
 ******************************************************************************/
#include "mmiwre_image.h"
#include "img_dec_interface.h"
#include "dal_img.h"
#include "mmk_type.h"
#include "guiimg.h"
#include "guires.h"
// extern IMG_RES_ERROR_E ALPHA_RES_GetImageInfo(IMG_RES_SRC_T  *img_src_ptr, IMG_RES_INFO_T *img_info_ptr);
// extern int ARGB_Decoder_AlphaEx(uint16* src_ptr, uint16* rgb_ptr,uint8* alpha_ptr);
//////////////////////////////////////////////////////////////////////
/*=============================================================
 函数名称 : GetImageInfo
 功能 : 获取通过硬件解码的数据.
 参数:
		unsigned long dwType[IN]---图形类型
		BOOL bIsFile[IN] --- 文件标志
		                     TRUE: 文件
							 FALSE: 数据
		LPTSTR  lpData[IN] --- 数据/文件全路径
		DWORD dwDataLen[IN] --- 数据长度
        int* pnw[IN/OUT] --- 限定宽度/实际解码后的宽度.
		int* pnh[IN/OUT] --- 限定高度/实际解码后的高度.
 返回 : >0: 解码成功
        0: 解码失败
=============================================================*/
unsigned long GetImageInfo(unsigned long dwType,unsigned char bIsFile,unsigned short* lpData,unsigned long dwDataLen,int* pnw,int* pnh)
{
	uint32  img_handle = 0;
    IMG_DEC_SRC_T   dec_src = {0};
	IMG_DEC_INFO_T      dec_info = {0}; /*lint !e64*/
	
	int w,h;
	
	if(!pnw || !pnh || !lpData) return 0;
// 	//如果是私有格式的png
// 	if(dwType == PT_WPNG)
// 	{
// 		IMG_RES_SRC_T img_src={0x00};
// 		IMG_RES_INFO_T img_info={0x00};
// 		
// 
// 		img_src.imgstream_ptr = (uint8*)lpData;
// 		img_src.imgstreamsize = dwDataLen;
// 		
// 		if(IMG_RES_SUCCESS != ALPHA_RES_GetImageInfo(&img_src,&img_info))
// 		{
// 			return 0;
// 		}
// 		
// 		*pnw=img_info.width;
// 		*pnh=img_info.height;
// 
// 		return 1;
// 	}

	//区域宽/高
	if(*pnw==0 || *pnh==0) return 0;

	//文件/数据
	if(!bIsFile)
	{
		dec_src.src_ptr       = (unsigned char*)lpData;
        dec_src.src_file_size = dwDataLen;
	}
	else
	{
		dec_src.file_name_ptr = lpData;
	}
	//获取图像信息
	if(IMG_DEC_RET_SUCCESS != IMG_DEC_GetInfo(&dec_src,&dec_info))
	{
		//SCI_TraceLow:"IMG_DEC_GetInfo fail!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_IMAGE_78_112_2_18_3_10_40_59,(uint8*)"");
		return 0;
	}
	
	if(dwType == PT_JPG)
	{
		if(dec_info.img_type != IMG_DEC_TYPE_JPEG)
		{
			//SCI_TRACE_LOW:"dec_info.img_type %d != %d!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_IMAGE_86_112_2_18_3_10_40_60,(uint8*)"dd",dec_info.img_type,dwType);
			*pnw=0;
			*pnh=0;
			return 0;
		}

		w = (*pnw > dec_info.img_detail_info.jpeg_info.img_width)?dec_info.img_detail_info.jpeg_info.img_width:*pnw;
		h = (*pnh > dec_info.img_detail_info.jpeg_info.img_heigth)?dec_info.img_detail_info.jpeg_info.img_heigth:*pnh;

		//w>>=1;w<<=1;
		//h>>=1;h<<=1;

	}
	else if(dwType == PT_PNG)
	{
		if(dec_info.img_type != IMG_DEC_TYPE_PNG)
		{
			//SCI_TRACE_LOW:"dec_info.img_type %d != %d!"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_IMAGE_103_112_2_18_3_10_40_61,(uint8*)"dd",dec_info.img_type,dwType);
			*pnw=0;
			*pnh=0;
			return 0;
		}

		w = (*pnw > dec_info.img_detail_info.png_info.img_width)?dec_info.img_detail_info.png_info.img_width:*pnw;
		h = (*pnh > dec_info.img_detail_info.png_info.img_height)?dec_info.img_detail_info.png_info.img_height:*pnh;
	}
	else
	{
		//SCI_TRACE_LOW:"dec_info.img_type %d != %d!"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_IMAGE_114_112_2_18_3_10_40_62,(uint8*)"dd",dec_info.img_type,dwType);
		*pnw=0;
		*pnh=0;
		return 0;
	}
	//准备解码图像信息
	IMG_DEC_Create(&dec_src,&img_handle);
	
	*pnw=w;
	*pnh=h;

	return (unsigned long)img_handle;
}
/*=============================================================
 函数名称 : GetImageData
 功能 : 获取通过硬件解码的数据.
 参数:
		unsigned long dwType[IN] --- 图像类型 PT_JPG/PT_WPNG/PT_PNG
		unsigned long handle[IN] --- 图形句柄
		unsigned char* pszRGBData[IN/OUT] --- 解码空间
		unsigned char* pszAlphaData[IN/OUT]--- alpha表
		unsigned long dwDataLen[IN] --- 空间大小
        int* pnw[IN/OUT] --- 限定宽度/实际解码后的宽度.
		int* pnh[IN/OUT] --- 限定高度/实际解码后的高度.
 返回 : 1: 解码成功
        0: 解码失败
=============================================================*/

unsigned char GetImageData(unsigned long dwType,unsigned long handle,unsigned char* pszRGBData,unsigned char* pszAlphaData,unsigned long dwDataLen,int* pnw,int* pnh)
{
    IMG_DEC_FRAME_IN_PARAM_T    dec_in = {0};
    IMG_DEC_FRAME_OUT_PARAM_T   dec_out = {0};

	int w,h;
	
// 	//如果是私有格式的png
// 	if(dwType == PT_WPNG)
// 	{
// 		IMG_RES_ERROR_E nret		= 0;
// 		nret = ARGB_Decoder_AlphaEx((unsigned short *)handle,(unsigned short *)pszRGBData,(unsigned char *)pszAlphaData);
// 		return (nret != 0)?1:0;
// 
// 	}

	//区域宽/高
	if(*pnw==0 || *pnh==0) 
	{
		IMG_DEC_Destroy(handle);
		return 0;
	}

	//文件/数据
	w = *pnw ;
	h = *pnh ;
	/*
	w>>=1;w<<=1;
	h>>=1;h<<=1;
	*/
	//set decode handle
    dec_in.handle = handle;
    //set decode target
    dec_in.is_dec_thumbnail = 0;
    dec_in.target_ptr       = pszRGBData;
    dec_in.target_buf_size  = dwDataLen;
	dec_in.alpha_buf_ptr    = pszAlphaData;
	dec_in.alpha_buf_size   =w*h;
    dec_in.target_width     = w;
    dec_in.target_height    = h;
    dec_in.img_rect.left    = 0;
    dec_in.img_rect.top     = 0;
    dec_in.img_rect.right   = w-1;
    dec_in.img_rect.bottom  = h-1;
	dec_in.data_format = IMG_DEC_RGB565;
	dec_in.target_buf_width=dec_in.target_buf_width=(unsigned short)(dwDataLen/(uint32)h);	// changed by yifei
	dec_in.target_buf_height=(unsigned short)h;
  	//图形解码
	if(IMG_DEC_RET_SUCCESS != IMG_DEC_GetFrame(&dec_in,&dec_out))
	{
		IMG_DEC_Destroy(handle);
		return 0;
	}
#if 0
    {
        SFS_HANDLE hOutFile;
        uint32 len=0;
        hOutFile = SFS_CreateFile(L"e:\\norm.raw", SFS_MODE_CREATE_ALWAYS|SFS_MODE_WRITE, 0, 0);
        if(SCI_NULL != hOutFile)
        {
            SFS_WriteFile(hOutFile,(const void*)pszRGBData,w*h*2,&len,0);
        }
        SFS_CloseFile(hOutFile);
    }
#endif
	*pnw=dec_out.real_width;
	*pnh=dec_out.real_height;

	IMG_DEC_Destroy(handle);

   
	return (dwType != PT_PNG)?1:(2|dec_out.is_process_alpha);
}
/*=============================================================
 函数名称 : _ScratchImage
 功能 : 把图片按照给定的宽高缩放
 参数:
		unsigned short* lpszName[IN] --- 图像文件全路径
		int cxDesired[IN] --- 要求缩放的宽
		int cyDesired[IN] --- 要求缩放的高
		unsigned char* lpszData[OUT]--- 缩后的图像文件数据
		unsigned long *unDataLen[IN/OUT] --- 缩后的图像文件数据大小
		int nPriority [IN]          --- 图片压缩级别
 返回 : 1: 成功
        0: 失败
=============================================================*/
unsigned char _ScratchImage(unsigned short* lpszName,int cxDesired,int cyDesired,unsigned char* lpszData, unsigned long* unDataLen,int nPriority)
{
    IMG_DEC_FRAME_IN_PARAM_T    in_param = {0};
    IMG_DEC_FRAME_OUT_PARAM_T   out_param = {0};
    IMG_DEC_SRC_T               img_dec_src = {0};
    uint32                      handle = 0;
	
    if (PNULL == lpszData || PNULL == lpszName)
    {
        //SCI_TRACE_LOW:"ScratchImage: NULL == lpszData || PNULL == lpszName"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_IMAGE_237_112_2_18_3_10_41_63,(uint8*)"");
        return FALSE;
    }
	//SCI_TRACE_LOW:">>>_ScratchImage0=%08x, ptr=%08x, cyDesired=%d, nPriority=%d"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_IMAGE_240_112_2_18_3_10_41_64,(uint8*)"dddd",*unDataLen, unDataLen, cyDesired, nPriority);
    //img_dec_src.src_file_size = file_len;
    img_dec_src.file_name_ptr = lpszName;
	
    if (IMG_DEC_RET_SUCCESS == IMG_DEC_Create(&img_dec_src,&handle))
    {
        in_param.callback = PNULL;
        in_param.handle = handle;
        in_param.data_format = IMG_DEC_JPEG;
        
        in_param.target_ptr = lpszData;
        in_param.target_buf_size = *unDataLen;
        in_param.target_width = cxDesired;
        in_param.target_height = cyDesired;
        in_param.write_data = PNULL;
        in_param.is_dec_thumbnail = FALSE;
		in_param.target_buf_width=cxDesired;
		in_param.target_buf_height=cyDesired;

		switch (nPriority)
		{
		case PICTURE_PRIORITY_LOWEST: //poor
			in_param.quality = JINF_QUALITY_LOW;
			break;
		case PICTURE_PRIORITY_BELOW_NORMAL: //normal
			in_param.quality = JINF_QUALITY_MIDDLE_LOW;
			break;
		case PICTURE_PRIORITY_NORMAL://good
			in_param.quality = JINF_QUALITY_MIDDLE;
			break;
		case PICTURE_PRIORITY_ABOVE_NORMAL://excellent
			in_param.quality = JINF_QUALITY_MIDDLE_HIGH;
			break;
		case PICTURE_PRIORITY_HIGHEST://outstanding
		default:
			in_param.quality = JINF_QUALITY_HIGH;	
			break;
		}
    }
    else
    {
        return FALSE;
    }
	
    if (IMG_DEC_RET_SUCCESS != IMG_DEC_GetFrame(&in_param,&out_param))
    {
        return FALSE;
    }
    
    if (IMG_DEC_RET_SUCCESS != IMG_DEC_Destroy(handle))
    {
        return FALSE;
    }
    *unDataLen=out_param.img_size;
    //SCI_TRACE_LOW:">>>_ScratchImage1=%d,%d"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_IMAGE_292_112_2_18_3_10_41_65,(uint8*)"dd",*unDataLen,out_param.img_size);

    return TRUE;
}

extern BOOLEAN MMIAPIFMM_SetPictureToWallpaper(
                                                  BOOLEAN      is_pre_wallpaper,   //是否是恢复前一张墙纸
                                                  const unsigned short  *full_path_ptr,
                                                  unsigned short       full_path_len
                                           );

#define MMIFMM_FILE_FULL_PATH_MAX_LEN 255
typedef struct _tagAPP_WALLPAPER_SET_PARAM
{
    const unsigned short  full_path_ptr[MMIFMM_FILE_FULL_PATH_MAX_LEN];
    unsigned short  full_path_len;
}APP_WALLPAPER_SET_PARAM, *PAPP_WALLPAPER_SET_PARAM;
BOOLEAN _SetPictureToWallpaper(void* param)
{
    const unsigned short  *full_path_ptr;
    unsigned short  full_path_len;
    if (param == NULL)
    {
        return FALSE;
    }
    full_path_ptr = ((APP_WALLPAPER_SET_PARAM*)param)->full_path_ptr;
    full_path_len = ((APP_WALLPAPER_SET_PARAM*)param)->full_path_len;
    return MMIAPIFMM_SetPictureToWallpaper(FALSE,full_path_ptr,full_path_len);
}
/*=============================================================
 函数名称 : MMIWRE_DisplayImage
 功能 : 把图片按照给定的区域显示
 参数:
		const unsigned short* lpszData[IN]  --- 图像文件全路径/未解码数据BUFFER
		unsigned long unDataLen[IN]         --- 0/未解码数据字节长度
		const GUI_RECT_T* lpRC[IN]          --- 显示区域
        unsigned long dwType[IN]            --- 图片类型
        const GUI_LCD_DEV_INFO *pdevInfo[IN]--- 设备信息
 返回 : TRUE: 成功
        FALSE: 失败
=============================================================*/
PUBLIC BOOLEAN MMIWRE_DisplayImage(const unsigned short* lpszData, unsigned long unDataLen,GUI_RECT_T* lpRC,unsigned long dwType,const GUI_LCD_DEV_INFO *pdevInfo)
{
    uint8*   pszRGBData=NULL;
    uint32  img_handle = 0;
    IMG_DEC_SRC_T   dec_src = {0};
    IMG_DEC_INFO_T      dec_info = {0}; /*lint !e64*/
    IMG_DEC_FRAME_IN_PARAM_T    dec_in = {0};
    IMG_DEC_FRAME_OUT_PARAM_T   dec_out = {0};
    GUIIMG_BITMAP_T  bitmap_handle = {0};
    GUI_POINT_T                 dis_point = {0};
    int w,h;
    //文件/数据
    if(0 != unDataLen)
    {
        dec_src.src_ptr       = (unsigned char*)lpszData;
        dec_src.src_file_size = unDataLen;
    }
    else
    {
        dec_src.file_name_ptr = lpszData;
    }

    //获取图像信息
    if(IMG_DEC_RET_SUCCESS != IMG_DEC_GetInfo(&dec_src,&dec_info))
    {
        //SCI_TraceLow:"IMG_DEC_GetInfo fail!"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWRE_IMAGE_357_112_2_18_3_10_41_66,(uint8*)"");
        return FALSE;
    }

    switch(dec_info.img_type)
    {
    case IMG_DEC_TYPE_BMP:
        w=dec_info.img_detail_info.bmp_info.img_width;
        h=dec_info.img_detail_info.bmp_info.img_height;
        break;
    case IMG_DEC_TYPE_PNG:
        w=dec_info.img_detail_info.png_info.img_width;
        h=dec_info.img_detail_info.png_info.img_height;
        break;
    case IMG_DEC_TYPE_JPEG:
        w=dec_info.img_detail_info.jpeg_info.img_width;
        h=dec_info.img_detail_info.jpeg_info.img_heigth;
        break;
    case IMG_DEC_TYPE_GIF:
        w=dec_info.img_detail_info.gif_info.img_width;
        h=dec_info.img_detail_info.gif_info.img_height;
        break;
    case IMG_DEC_TYPE_WBMP:
        w=dec_info.img_detail_info.wbmp_info.img_width;
        h=dec_info.img_detail_info.wbmp_info.img_height;
        break;
    default:
        return FALSE;

    }

    pszRGBData=(uint8*)SCI_ALLOC_APP(w*h*2);
    if(NULL == pszRGBData)
    {
        return FALSE;
    }

    if (IMG_DEC_RET_SUCCESS != IMG_DEC_Create(&dec_src,&img_handle))
    {
        SCI_FREE(pszRGBData);
        return FALSE;
    }

    dec_in.handle = img_handle;
    //set decode target
    dec_in.is_dec_thumbnail = 0;
    dec_in.target_ptr       = pszRGBData;
    dec_in.target_buf_size  = w*h*2;
    dec_in.target_width     = w;
    dec_in.target_height    = h;
    dec_in.img_rect.left    = 0;
    dec_in.img_rect.top     = 0;
    dec_in.img_rect.right   = w-1;
    dec_in.img_rect.bottom  = h-1;
    dec_in.data_format = IMG_DEC_RGB565;

    //图形解码
    if(IMG_DEC_RET_SUCCESS != IMG_DEC_GetFrame(&dec_in,&dec_out))
    {
        IMG_DEC_Destroy(img_handle);
        SCI_FREE(pszRGBData);
        return FALSE;
    }

    IMG_DEC_Destroy(img_handle);

   
    bitmap_handle.bit_ptr = pszRGBData;
    bitmap_handle.img_width  = dec_out.real_width;
    bitmap_handle.img_height = dec_out.real_height;
    
    dis_point.x=lpRC->left;
    dis_point.y=lpRC->top;
    //IMG_EnableTransparentColor(TRUE);//decode osd

    GUIIMG_DisplayBmp(FALSE, lpRC, &dis_point, &bitmap_handle,pdevInfo);
    
    //IMG_EnableTransparentColor(FALSE);

    SCI_FREE(pszRGBData);
    return TRUE;
}

PUBLIC BOOLEAN MMIWRE_DisplayImage_Ex(const unsigned short* lpszData, unsigned long unDataLen,GUI_RECT_T* lpRC,const GUI_LCD_DEV_INFO *pdevInfo)
{
    GUIIMG_BITMAP_T  bitmap_handle = {0};
    GUI_POINT_T                 dis_point = {0};
  

    if (lpszData == NULL)
    {
        return FALSE;
    }

   
    bitmap_handle.bit_ptr = lpszData;
    bitmap_handle.img_width  = unDataLen & 0xffff;
    bitmap_handle.img_height = (unDataLen >> 16);
    
    dis_point.x=lpRC->left;
    dis_point.y=lpRC->top;
    //IMG_EnableTransparentColor(TRUE);//decode osd

    GUIIMG_DisplayBmp(FALSE, lpRC, &dis_point, &bitmap_handle,pdevInfo);
    
    //IMG_EnableTransparentColor(FALSE);

    return TRUE;
}
#endif
