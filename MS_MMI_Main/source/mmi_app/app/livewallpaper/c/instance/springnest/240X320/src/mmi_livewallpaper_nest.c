/*****************************************************************************
** File Name:      mmi_livewallpaper_NEST.c                                    *
** Author:         Arvin.wu                                                       *
** Date:           18/06/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe runninglight wallpaer       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2011       Arvin.wu          Modify                                  *
******************************************************************************/
#ifndef _MMI_LIVEWALLPAPER_NEST_C_
#define _MMI_LIVEWALLPAPER_NEST_C_

#ifdef LIVE_WALLPAPER_SUPPORT_NEST

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmilivewallpaper_export.h"

#include "mmi_text.h"
#include "mmi_image.h"
#include "mmiset_text.h"
#include "mmiset_image.h"
#include "mmi_livewallpaper_text.h"
#include "mmi_livewallpaper_image.h"
#include "graphics_3d_rotate.h"
#include "Nest_PeachGarden_3D.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define BLOCK_MEM_POOL_SIZE_NEST (700 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;

typedef struct
{
    MMI_IMAGE_ID_T  img_id;
    IMGREF_SIZE_T size;
    uint8*    buf_ptr;
    BOOLEAN is_bg;
}NEST_RESOURCE_INFO_T;


/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL OBJECT565_T* s_pRes_springnest = PNULL;
LOCAL NEST_RESOURCE_INFO_T* s_pImg_res = PNULL;
LOCAL PEACHFLOWER_RES_T s_petal_info = {0};
LOCAL COLOR16* s_pBG_Res = PNULL;
LOCAL int32 * s_calc_buf_ptr = PNULL;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                                                         */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 静态数据初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN NEST_Init(void);
/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN NEST_Construct(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN NEST_DrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : 算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void NEST_Calculate(LW_CAL_PARAM *lw_cal_param);
/*****************************************************************************/
//  Description : 释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void NEST_Destruct(void);
/*****************************************************************************/
//  Description : 交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN NEST_HandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 NEST_GetType(void);
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void NEST_GetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr);
/*****************************************************************************/
//  Description : 获得动态壁纸源数据宽高
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void NEST_GetWidthAndHeight(uint16 *width_ptr,uint16 *height_ptr);
/*****************************************************************************/
//  Description : 获得动态壁纸选项列表
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL OPTIONLIST *NEST_GetOptionList(void);
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void NEST55Calc(int32 frame, int32 frame_total, COLOR16* pBG, IMGREF_SIZE_T bg_window);
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_nest_Instance =
{
    NEST_Init,
    NEST_Construct,
    NEST_DrawBg,
    NEST_Calculate,
    NEST_Destruct,
    NEST_HandleEvent,
    PNULL,
    NEST_GetType,
    NEST_GetThumbNailInfo,
    NEST_GetWidthAndHeight,
    PNULL,
    PNULL,
    NEST_GetOptionList
};
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                                                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 静态数据初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN NEST_Init(void)
{
    
    return TRUE;
}

/*****************************************************************************/
//  Description : 创建layer
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN NEST_CreateLayer(MMI_WIN_ID_T win_id,NEST_RESOURCE_INFO_T *img_info)
{
    UILAYER_CREATE_T create_info = {0}; 
    GUI_POINT_T display_point = {0,0};
    UILAYER_INFO_T layer_info = {0};
    BOOLEAN result = TRUE;
    GUI_LCD_DEV_INFO layer_handle = {0,UILAYER_NULL_HANDLE};
    uint8*  buf_temp = PNULL;
    uint8 size = 2;
    
    /*先创建一个层*/
    create_info.lcd_id = GUI_MAIN_LCD_ID;
    create_info.owner_handle = (MMI_HANDLE_T)win_id;
    create_info.offset_x = 0;
    create_info.offset_y = 0;
    create_info.width = img_info->size.w;
    create_info.height = img_info->size.h;
    create_info.is_bg_layer = img_info->is_bg;
    create_info.is_static_layer = FALSE; 
    create_info.func_ptr = MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(); 
    
    if(UILAYER_RESULT_SUCCESS != UILAYER_CreateLayer(
    					&create_info,
    					&layer_handle
    					))
    {
        return FALSE;
    }
	
    if(!UILAYER_IsLayerActive(&layer_handle))
    	{
    		UILAYER_RELEASELAYER(&layer_handle);
		return FALSE;
	}
	
	
    // 设置color key
    UILAYER_SetLayerColorKey(&layer_handle, TRUE, UILAYER_TRANSPARENT_COLOR);

    // 使用color清除层
    UILAYER_Clear(&layer_handle);
	
	    //把图片画到层上
	    GUIRES_DisplayImg(&display_point,
	    				PNULL,
	    				PNULL,
	    				win_id,
	    				img_info->img_id,
	    				(const GUI_LCD_DEV_INFO*)&layer_handle);
UILAYER_ChangeLayerRGB565ToARGB5515(&layer_handle,TRUE,TRUE);

    //获取图片的信息
    UILAYER_GetLayerInfo(&layer_handle, &layer_info);
    
    img_info->size.w = layer_info.layer_width;
    img_info->size.h = layer_info.layer_height;
    
    //获取图片buf
    buf_temp= UILAYER_GetLayerBufferPtr(&layer_handle);  
	
	if(PNULL == buf_temp)
	{
		UILAYER_RELEASELAYER(&layer_handle);
		return FALSE;
	}
	
    if(img_info->is_bg)
    {
        size = 2;
    }
    else
    {
        size = 4;
    }
    SCI_MEMCPY(img_info->buf_ptr,buf_temp,img_info->size.w*img_info->size.h*size);

    UILAYER_RELEASELAYER(&layer_handle);
    return result;
}
/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN NEST_Construct(MMI_WIN_ID_T win_id)
{
    uint32 met_num = Nest_GetMeterialNum();
    int i = 0,j=0;
     wchar file_full_path[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};  
    wchar fName[MMIFILE_FULL_PATH_MAX_LEN + 1] = {0};
     const wchar path[]= L"e:\\photos\\"; 
	 const wchar format[]= L".bmp"; 
	  uint16 cat_len = 0;
	  int32 allocSize = 0;
	  
    s_pRes_springnest = (OBJECT565_T*)MMILIVEWALLPAPER_ALLOC(met_num*sizeof(OBJECT565_T));

    if(PNULL == s_pRes_springnest)
    {
        return FALSE;
    }
    s_pImg_res = (NEST_RESOURCE_INFO_T*)MMILIVEWALLPAPER_ALLOC(met_num*sizeof(NEST_RESOURCE_INFO_T));

    if(PNULL == s_pImg_res)
    {
        NEST_Destruct();
        return FALSE;
    }

    for(i=0;i<met_num;i++)
    {
        s_pImg_res[i].img_id = IMAGE_LIVEWALLPAPER_SPRINGNEST_0 + i;
        GUIRES_GetImgWidthHeight((uint16*)&s_pImg_res[i].size.w, (uint16*)&s_pImg_res[i].size.h,s_pImg_res[i].img_id, win_id);

        s_pImg_res[i].is_bg = TRUE;
        s_pImg_res[i].buf_ptr = (uint8*)MMILIVEWALLPAPER_ALLOC(s_pImg_res[i].size.w*s_pImg_res[i].size.h*sizeof(uint16));

        if(PNULL == s_pImg_res[i].buf_ptr)
        {
            NEST_Destruct();
            return FALSE;
        }

		 if(FALSE == NEST_CreateLayer(win_id,&(s_pImg_res[i])))
	        {
	            NEST_Destruct();
	            return FALSE;
	        }
	   

        
        s_pRes_springnest[i].srcARGB_Map = s_pImg_res[i].buf_ptr;
        s_pRes_springnest[i].up_window = s_pImg_res[i].size;
    }

    s_petal_info.Petal[0] = s_pRes_springnest[55];
    s_petal_info.Petal[1] = s_pRes_springnest[57];
    s_petal_info.Petal[2] = s_pRes_springnest[56];
    SetPeachGardenResOrder(s_pRes_springnest);
	allocSize = (GetPeachGardenFaceNum()+PEACHFLOWER_NUM*PEACHFLOWER_PETAL_NUM)*sizeof(int32)*2;
    s_calc_buf_ptr = (int32*)MMILIVEWALLPAPER_ALLOC(allocSize);
	memset(s_calc_buf_ptr,0,allocSize);
    return TRUE;
}

/*****************************************************************************/
//  Description : 准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN NEST_DrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    BOOLEAN b_result = FALSE;
    GUI_POINT_T display_point = {0,0};

    

    if(PNULL == dev_info_ptr)
    {
        return FALSE;
    }
    else
    {
	UILAYER_ChangeLayerARGB555ToRGB565(dev_info_ptr,FALSE);
	
        b_result = GUIRES_DisplayImg(&display_point, 
                                            PNULL, 
                                            PNULL, 
                                            win_id, 
                                            IMAGE_LIVEWALLPAPER_SPRINGNEST_BG, 
                                            dev_info_ptr);
    }
    s_pBG_Res = (COLOR16*)UILAYER_GetLayerBufferPtr(dev_info_ptr);

	UILAYER_ChangeLayerRGB565ToARGB5515(dev_info_ptr,TRUE,FALSE);
    return b_result;
    
}

/*****************************************************************************/
//  Description : 算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void NEST_Calculate(LW_CAL_PARAM *lw_cal_param)
{
    int32 Gapframe = 1;
    int32 GaptotalFrame  = 22;
    int32 KeyFrame = 0;
    COLOR16* BG_ResBuf = s_pBG_Res;
    OBJECT565_T* PeachGardenlambertRes = s_pRes_springnest;
    PEACHFLOWER_RES_T* PeachFlowerRes = &s_petal_info;
    IMGREF_SIZE_T pWindowSize  = {0}; 
    COLOR16* TargetBuf = PNULL;
	int32 Up_Page_total_move = 0;


		static int32 rot_i = 0;
		int32 GapFrametotal = 0, totalFrame = 0,frame = 0;

		int32 i = 0, j = 0;
        static int32 cur_time = 0;
        int32 time = 0;

        int32 TimeGap = 2;
		

        SCI_TRACE_LOW("[NEST] ONE FRAME %d",SCI_GetTickCount()-cur_time);
		
        cur_time = SCI_GetTickCount();

 	time = SCI_GetTickCount();
	
if(PNULL==s_calc_buf_ptr||PNULL == lw_cal_param)
{
	return;
}
#if 1
Up_Page_total_move = (lw_cal_param->fg_pos_info.rc.right - lw_cal_param->fg_pos_info.rc.left + 1-MMI_MAINSCREEN_WIDTH);

pWindowSize.w = lw_cal_param->dest_buf_info.w;
pWindowSize.h = lw_cal_param->dest_buf_info.h;
TargetBuf = lw_cal_param->dest_buf_info.buf_ptr;

if( lw_cal_param->fg_pos_info.x<0)
{
	KeyFrame = 0;
	Gapframe = 0;
}
else
{
	KeyFrame = lw_cal_param->fg_pos_info.x/pWindowSize.w;

	GaptotalFrame = Nest_GetTOTALGapFRAME();
	Gapframe = (lw_cal_param->fg_pos_info.x%pWindowSize.w)*GaptotalFrame/pWindowSize.w;
}

if(KeyFrame>(Nest_GetTOTALKeyFRAME()-2))
{
	KeyFrame = (Nest_GetTOTALKeyFRAME()-2);
	Gapframe = GaptotalFrame-1;
}

if(MMI_MAINSCREEN_WIDTH == (lw_cal_param->fg_pos_info.rc.right - lw_cal_param->fg_pos_info.rc.left + 1))
{
	KeyFrame = (Nest_GetTOTALKeyFRAME()-2);
	Gapframe = GaptotalFrame-1;
}
#else
		GapFrametotal = Nest_GetTOTALGapFRAME();
			
		totalFrame = 0;
		i = Nest_GetTOTALKeyFRAME()-1;
		while(i>0)
		{			
			totalFrame += (GapFrametotal-2);
			i--;
		}
		totalFrame += Nest_GetTOTALKeyFRAME();

		rot_i+=TimeGap;
		if(rot_i>=totalFrame+41)
			rot_i = TimeGap;//totalFrame+21;//  1;//totalFrame+21;

		frame = rot_i;//rot_i;//ValFromDlg_1;

		i = 0;
		j = 0;
		while(1)
		{
			if(i>=Nest_GetTOTALKeyFRAME()-1)
			{
				KeyFrame = Nest_GetTOTALKeyFRAME()-2;
				GaptotalFrame = GapFrametotal-1;
				Gapframe = GaptotalFrame;
				break;
			}
			j += GapFrametotal-2;
			if(frame<=j)
			{
				KeyFrame = i;
				GaptotalFrame = GapFrametotal-1;
				Gapframe = frame-(j-GapFrametotal+2);
				break;
			}
			i++;
			j++;
		}

		/*GaptotalFrame = GapFrameNum[KeyFrame]+1;
		Gapframe = ValFromDlg_1;*/
		if(Gapframe>GaptotalFrame)
		{
			Gapframe = GaptotalFrame;
		}
		if(Gapframe<0)
		{
			Gapframe = 0;
		}
		if(GaptotalFrame<=0)
		{
			GaptotalFrame = 1;
		}
		{
			static int32 waitInGap = 0;
			static int32 waitCopy = 0;
			if(TimeGap > Gapframe&&0 >= waitInGap)
			{
				waitInGap = 40;//20;//0;
				waitCopy = rot_i;
			}
			else if(TimeGap > Gapframe && 0 < waitInGap)
			{
				waitInGap--; 
			}
			else
			{
				waitInGap = 0;
			}			

			if(waitInGap>0)
			{
				rot_i = waitCopy-TimeGap;
			}
		}

		

    pWindowSize.w = lw_cal_param->dest_buf_info.w;
    pWindowSize.h = lw_cal_param->dest_buf_info.h;
    TargetBuf = lw_cal_param->dest_buf_info.buf_ptr;
SCI_TRACE_LOW("[NEST] calc FREE FRAME %d",SCI_GetTickCount()-time);


#endif

    time = SCI_GetTickCount();
    PeachGardenCalc(Gapframe, 
						GaptotalFrame, 
						KeyFrame, 
						BG_ResBuf, 
						PeachGardenlambertRes,
						PeachFlowerRes,
						&pWindowSize, 
						TargetBuf,
						s_calc_buf_ptr);
						
	SCI_TRACE_LOW("[NEST] calc FRAME %d",SCI_GetTickCount()-time);

	/*{
		int32 i = 0, j = 0;
		for(j = 0; j < pWindowSize.h; j++)
		{
		for(i = 0; i < pWindowSize.w; i++)
		{
		TargetBuf[j*pWindowSize.w+i] = ((TargetBuf[j*pWindowSize.w+i]<<1)&0xffc0)+(TargetBuf[j*pWindowSize.w+i]&0x1f);
		}
		}
	}*/

	
}
/*****************************************************************************/
//  Description : 释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void NEST_Destruct(void)
{
    uint32 met_num = Nest_GetMeterialNum();
    int i = 0;
	
	if(PNULL != s_pImg_res)
	{
	    for(i=0;i<met_num;i++)
	    {
	        MMILIVEWALLPAPER_FREE(s_pImg_res[i].buf_ptr);
	    }  

	}

	if(PNULL != s_pRes_springnest)
	{
	    for(i=0;i<met_num;i++)
	    {
		s_pRes_springnest[i].srcARGB_Map = PNULL;
	    }
	}

    MMILIVEWALLPAPER_FREE(s_pImg_res);
    MMILIVEWALLPAPER_FREE(s_pRes_springnest);

	MMILIVEWALLPAPER_FREE(s_calc_buf_ptr);
	
}

/*****************************************************************************/
//  Description : 交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN NEST_HandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    return FALSE;
}
/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 NEST_GetType(void)
{
    return MMI_LIVE_WALLPAPER_TYPE_NEST;
}
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void NEST_GetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr)
{
    if(thumbnail_info_ptr)
    {
        thumbnail_info_ptr->is_image_file = FALSE;
        //thumbnail_info_ptr->image_id = IMAGE_WALLPAPER_SPRING_THUMBNAIL;

        thumbnail_info_ptr->is_not_string = FALSE;
        thumbnail_info_ptr->text_id = TXT_SET_WALLPAPER_NEST;
        thumbnail_info_ptr->instance_mem_pool_size = BLOCK_MEM_POOL_SIZE_NEST;
    }
}
/*****************************************************************************/
//  Description : 获得动态壁纸源数据宽高
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void NEST_GetWidthAndHeight(uint16 *width_ptr,uint16 *height_ptr)
{
    GUIRES_GetImgWidthHeight(width_ptr, height_ptr,IMAGE_LIVEWALLPAPER_SPRINGNEST_BG, MMK_GetFocusWinId());
}
/*****************************************************************************/
//  Description : 获得动态壁纸选项列表
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL OPTIONLIST *NEST_GetOptionList(void)
{
    return (OPTIONLIST*)PNULL;
}

#endif// LIVE_WALLPAPER_SUPPORT_NEST


#endif //_MMI_LIVEWALLPAPER_NEST_C_

