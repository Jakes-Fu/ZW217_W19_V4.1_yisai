

/*****************************************************************************
** File Name:      mmi_livewallpaper_galaxy.c                                    *
** Author:         Gaily.Wang                                                *
** Date:           18/06/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe runninglight wallpaer       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 06/2011       Gaily.Wang          Create                                  *
** 08/2011       Arvin.wu          Modify                                  *
******************************************************************************/
#ifndef _MMI_LIVEWALLPAPER_LITTLECAT_C_
#define _MMI_LIVEWALLPAPER_LITTLECAT_C_

#ifdef LIVE_WALLPAPER_SUPPORT_LITTLECAT

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmilivewallpaper_export.h"

#include "mmk_timer.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmiset_text.h"
#include "mmiset_image.h"
#include "mmi_livewallpaper_image.h"
#include "mmi_livewallpaper_text.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define MMI_LITTLECAT_ROW_MAX_NUM 10
#define MMI_LITTLECAT_COL_MAX_NUM 8
#define MMI_LITTLECAT_JUMP_INTERVAL 20
#define MMI_LITTLECAT_DANCE_INTERVAL 20
#define MMI_LITTLECAT_JUMP_CYCLE_INTERVAL 4000
#define MMI_LITTLECAT_PIC_WIDTH 80
#define MMI_LITTLECAT_PIC_HEIGHT 80
#define BLOCK_MEM_POOL_SIZE_LITTLECAT (500 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;

#define SCR_SIZE    ((MMI_MAINSCREEN_HEIGHT*MMI_MAINSCREEN_WIDTH)*sizeof(uint16))

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

typedef struct
{
	uint16 dance_frame_index;
	uint16 jump_frame_index;
	BOOL is_select;
}MMI_LITTLECAT_WALLPAPER_INFO_T;

typedef enum
{
    MMI_LITTLECAT_IDLE,
    MMI_LITTLECAT_DANCING,
    MMI_LITTLECAT_JUMPING,
    MMI_LITTLECAT_STATE_MAX
}MMI_LITTLECAT_STATE_E;
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL MMI_LITTLECAT_STATE_E s_littlecat_state = {0};


uint32* s_littlecat_buf_ptr = PNULL;
uint32* s_littlecat_dance1_buf_ptr = PNULL;
uint32* s_littlecat_dance2_buf_ptr = PNULL;
uint32* s_littlecat_dance3_buf_ptr = PNULL;
uint32* s_littlecat_dance4_buf_ptr = PNULL;
uint32* s_littlecat_dance5_buf_ptr = PNULL;
uint32* s_littlecat_dance6_buf_ptr = PNULL;
uint32* s_littlecat_dance7_buf_ptr = PNULL;
uint32* s_littlecat_dance8_buf_ptr = PNULL;
uint32* s_littlecat_dance9_buf_ptr = PNULL;
uint32* s_littlecat_dance10_buf_ptr = PNULL;
uint32* s_littlecat_jump1_buf_ptr = PNULL;
uint32* s_littlecat_jump2_buf_ptr = PNULL;
uint32* s_littlecat_jump3_buf_ptr = PNULL;
uint32* s_littlecat_jump4_buf_ptr = PNULL;
uint32* s_littlecat_jump5_buf_ptr = PNULL;
uint32* s_littlecat_jump6_buf_ptr = PNULL;
uint32* s_littlecat_jump7_buf_ptr = PNULL;

//LOCAL uint8 s_littlecat_jump_timer = 0;
//LOCAL uint8 s_littlecat_dance_timer = 0;
LOCAL uint8 s_littlecat_cycle_jump_timer = 0;
LOCAL uint8 s_littlecat_rand_col = 0;
LOCAL uint8 s_littlecat_rand_row = 0;
LOCAL uint8 s_littlecat_arr_col_num = 0;
LOCAL uint8 s_littlecat_arr_row_num = 0;
LOCAL uint8 s_littlecat_dance_frame_num = 0;
LOCAL uint8 s_littlecat_jump_frame_num = 0;
LOCAL uint16 s_littlecat_pic_width = 0;
LOCAL uint16 s_littlecat_pic_height = 0;
    
LOCAL GUI_LCD_DEV_INFO s_littlecat_lcd_dev_info = {0,0};

LOCAL const MMI_IMAGE_ID_T s_littlecat_anim_dance_arry[]={
	IMAGE_LITTLECAT_DANCE_01,IMAGE_LITTLECAT_DANCE_02,IMAGE_LITTLECAT_DANCE_03,
	IMAGE_LITTLECAT_DANCE_04,IMAGE_LITTLECAT_DANCE_05,IMAGE_LITTLECAT_DANCE_06,
	IMAGE_LITTLECAT_DANCE_07,IMAGE_LITTLECAT_DANCE_08,IMAGE_LITTLECAT_DANCE_09,
	IMAGE_LITTLECAT_DANCE_10,
	IMAGE_LITTLECAT_DANCE_01,IMAGE_LITTLECAT_DANCE_02,IMAGE_LITTLECAT_DANCE_03,
	IMAGE_LITTLECAT_DANCE_04,IMAGE_LITTLECAT_DANCE_05,IMAGE_LITTLECAT_DANCE_06,
	IMAGE_LITTLECAT_DANCE_07,IMAGE_LITTLECAT_DANCE_08,IMAGE_LITTLECAT_DANCE_09,
	IMAGE_LITTLECAT_DANCE_10,
	IMAGE_LITTLECAT_DANCE_01,IMAGE_LITTLECAT_DANCE_02,IMAGE_LITTLECAT_DANCE_03,
	IMAGE_LITTLECAT_DANCE_04,IMAGE_LITTLECAT_DANCE_05,IMAGE_LITTLECAT_DANCE_06,
	IMAGE_LITTLECAT_DANCE_07,IMAGE_LITTLECAT_DANCE_08,IMAGE_LITTLECAT_DANCE_09,
	IMAGE_LITTLECAT_DANCE_10,
	IMAGE_LITTLECAT_DANCE_01,IMAGE_LITTLECAT_DANCE_02,IMAGE_LITTLECAT_DANCE_03,
	IMAGE_LITTLECAT_DANCE_04,IMAGE_LITTLECAT_DANCE_05,IMAGE_LITTLECAT_DANCE_06,
	IMAGE_LITTLECAT_DANCE_07,IMAGE_LITTLECAT_DANCE_08,IMAGE_LITTLECAT_DANCE_09,
	IMAGE_LITTLECAT_DANCE_10,
	IMAGE_LITTLECAT_DANCE_11,
	};
LOCAL const MMI_IMAGE_ID_T s_littlecat_anim_jump_arry[]={IMAGE_LITTLECAT_JUMP_01,IMAGE_LITTLECAT_JUMP_02,IMAGE_LITTLECAT_JUMP_03,IMAGE_LITTLECAT_JUMP_04,
	IMAGE_LITTLECAT_JUMP_05,IMAGE_LITTLECAT_JUMP_06,IMAGE_LITTLECAT_JUMP_07,};

MMI_LITTLECAT_WALLPAPER_INFO_T s_littlecat_bg_array[MMI_LITTLECAT_ROW_MAX_NUM][MMI_LITTLECAT_COL_MAX_NUM] ={
{{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE}},
{{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE}},
{{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE}},
{{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE}},
{{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE}},
{{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE}},
{{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE}},
{{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE}},
{{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE}},
{{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE},{0,0,FALSE}},
};



/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                                                         */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description :小猫静态数据初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LittleCatInit(void);
/*****************************************************************************/
//  Description : 小猫动态数据初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LittleCatConstruct(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 小猫准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LittleCatDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : 小猫刷新算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LittleCatCalculate(LW_CAL_PARAM *lw_cal_param);
/*****************************************************************************/
//  Description : 小猫动态数据释放释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void LittleCatDestruct(void);
/*****************************************************************************/
//  Description : 取得当前帧的数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/

LOCAL COLOR32* GetLittleCatDancingFrameBuf(uint16 frame_index);
/*****************************************************************************/
//  Description :取得小猫动画帧数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL COLOR32* GetLittleCatJumpingFrameBuf(uint16 frame_index);
/*****************************************************************************/
//  Description : 星系交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LittleCatHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/*****************************************************************************/
//  Description : Handle tp down msg on the littlecat wallpaper
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_HandleLittleCatTPDownMsg(MMI_WIN_ID_T win_id,GUI_POINT_T tp_point);
/*****************************************************************************/
//  Description : free all the buffer allocat by the littlecat wallpaper 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void FreeAllBufferofLittleCat(void);

/*****************************************************************************/
//  Description : stop the littlecat wallpaper timer
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_StopAllLittleCatTimer(void);
/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 LittleCatGetType(void);
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LittleCatGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_littlecat_Instance =
{
    LittleCatInit,
    LittleCatConstruct,
    LittleCatDrawBg,
    LittleCatCalculate,
    LittleCatDestruct,
    LittleCatHandleEvent,
    PNULL,
    LittleCatGetType,
    LittleCatGetThumbNailInfo,
    PNULL,
    PNULL,
    PNULL,
    PNULL
};
/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                                                              */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 小猫 刷新
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void LittleCatUpdate(COLOR16 *target_buf_ptr, uint16 scr_width, uint16 scr_height)
{
}
/*****************************************************************************/
//  Description : 小猫初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LittleCatInit(void)
{
    BOOLEAN b_result = TRUE;
    GUI_RECT_T lcd_rect = {0};

    MMK_SetWinAngle(MMK_GetFocusWinId(), LCD_ANGLE_0, FALSE );  //keep the collision dynamic wallpaper's lcd angle as 0 whenever the actual lcd angle is ;


     
    GUIRES_GetImgWidthHeight(&s_littlecat_pic_width, &s_littlecat_pic_height,
        IMAGE_LITTLECAT_JUMP_01,MAIN_IDLE_WIN_ID);
    if(0==s_littlecat_pic_width||0==s_littlecat_pic_height)
    {
        s_littlecat_pic_width = MMI_LITTLECAT_PIC_WIDTH;
        s_littlecat_pic_height = MMI_LITTLECAT_PIC_HEIGHT;
    }
    lcd_rect = MMITHEME_GetFullScreenRect();
    s_littlecat_dance_frame_num = sizeof(s_littlecat_anim_dance_arry)/(sizeof(s_littlecat_anim_dance_arry[0]));
    s_littlecat_jump_frame_num = sizeof(s_littlecat_anim_jump_arry)/(sizeof(s_littlecat_anim_jump_arry[0]));

    s_littlecat_arr_row_num = (lcd_rect.bottom-lcd_rect.top+1)/s_littlecat_pic_height;//MMI_LITTLECAT_PIC_HEIGHT;
    s_littlecat_arr_col_num = (lcd_rect.right-lcd_rect.left+1)/s_littlecat_pic_width;//MMI_LITTLECAT_PIC_WIDTH;
   
    if(s_littlecat_arr_col_num>MMI_LITTLECAT_COL_MAX_NUM)
    {
        s_littlecat_arr_col_num = MMI_LITTLECAT_COL_MAX_NUM;
    }
    if(s_littlecat_arr_row_num>MMI_LITTLECAT_ROW_MAX_NUM)
    {
        s_littlecat_arr_row_num = MMI_LITTLECAT_ROW_MAX_NUM;
    }
    s_littlecat_state = MMI_LITTLECAT_IDLE;

    return b_result;
}

/*****************************************************************************/
//  Description : 小猫初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LittleCatConstruct(MMI_WIN_ID_T win_id)
{
    BOOLEAN b_result = FALSE;
    UILAYER_CREATE_T create_info = {0};   
//    GUI_RECT_T lcd_rect = MMITHEME_GetFullScreenRect();
UILAYER_INFO_T layer_info = {0};
    GUI_POINT_T dis_point = {0,0};
    UILAYER_COPYBUF_T           copy_buf = {0};
    UILAYER_COPYBUF_PARAM_T     copy_param = {0};

    create_info.lcd_id          = MAIN_LCD_ID;
    create_info.owner_handle    = win_id;
    create_info.offset_x        = 0;
    create_info.offset_y        = 0;
    create_info.width           = s_littlecat_pic_width;
    create_info.height          = s_littlecat_pic_height;
    create_info.is_bg_layer     = FALSE;
    create_info.is_static_layer = FALSE;   
    create_info.func_ptr = MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(); 
    UILAYER_CreateLayer(&create_info,&s_littlecat_lcd_dev_info);

    s_littlecat_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*s_littlecat_pic_height*s_littlecat_pic_width);
    if(PNULL == s_littlecat_buf_ptr)
    {
        FreeAllBufferofLittleCat();
        return FALSE;
    }
    s_littlecat_dance1_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*s_littlecat_pic_height*s_littlecat_pic_width);
    if(PNULL == s_littlecat_dance1_buf_ptr)
    {
        FreeAllBufferofLittleCat();
        return FALSE;
    }
    s_littlecat_dance2_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*s_littlecat_pic_height*s_littlecat_pic_width);
    if(PNULL == s_littlecat_dance2_buf_ptr)
    {
        FreeAllBufferofLittleCat();
        return FALSE;
    }
    s_littlecat_dance3_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*s_littlecat_pic_height*s_littlecat_pic_width);
    if(PNULL == s_littlecat_dance3_buf_ptr)
    {
        FreeAllBufferofLittleCat();
        return FALSE;
    }
    s_littlecat_dance4_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*s_littlecat_pic_height*s_littlecat_pic_width);
    if(PNULL == s_littlecat_dance4_buf_ptr)
    {
        FreeAllBufferofLittleCat();
        return FALSE;
    }
    s_littlecat_dance5_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*s_littlecat_pic_height*s_littlecat_pic_width);
    if(PNULL == s_littlecat_dance5_buf_ptr)
    {
        FreeAllBufferofLittleCat();
        return FALSE;
    }
    s_littlecat_dance6_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*s_littlecat_pic_height*s_littlecat_pic_width);
    if(PNULL == s_littlecat_dance6_buf_ptr)
    {
        FreeAllBufferofLittleCat();
        return FALSE;
    }
    s_littlecat_dance7_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*s_littlecat_pic_height*s_littlecat_pic_width);
    if(PNULL == s_littlecat_dance7_buf_ptr)
    {
        FreeAllBufferofLittleCat();
        return FALSE;
    }
    s_littlecat_dance8_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*s_littlecat_pic_height*s_littlecat_pic_width);
    if(PNULL == s_littlecat_dance8_buf_ptr)
    {
        FreeAllBufferofLittleCat();
        return FALSE;
    }
    s_littlecat_dance9_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*s_littlecat_pic_height*s_littlecat_pic_width);
    if(PNULL == s_littlecat_dance9_buf_ptr)
    {
        FreeAllBufferofLittleCat();
        return FALSE;
    }
    s_littlecat_dance10_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*s_littlecat_pic_height*s_littlecat_pic_width);
    if(PNULL == s_littlecat_dance10_buf_ptr)
    {
        FreeAllBufferofLittleCat();
        return FALSE;
    }

    s_littlecat_jump1_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*s_littlecat_pic_height*s_littlecat_pic_width);
    if(PNULL == s_littlecat_jump1_buf_ptr)
    {
        FreeAllBufferofLittleCat();
        return FALSE;
    }
    s_littlecat_jump2_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*s_littlecat_pic_height*s_littlecat_pic_width);
    if(PNULL == s_littlecat_jump2_buf_ptr)
    {
        FreeAllBufferofLittleCat();
        return FALSE;
    }
    s_littlecat_jump3_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*s_littlecat_pic_height*s_littlecat_pic_width);
    if(PNULL == s_littlecat_jump3_buf_ptr)
    {
        FreeAllBufferofLittleCat();
        return FALSE;
    }
    s_littlecat_jump4_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*s_littlecat_pic_height*s_littlecat_pic_width);
    if(PNULL == s_littlecat_jump4_buf_ptr)
    {
        FreeAllBufferofLittleCat();
        return FALSE;
    }
    s_littlecat_jump5_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*s_littlecat_pic_height*s_littlecat_pic_width);
    if(PNULL == s_littlecat_jump5_buf_ptr)
    {
        FreeAllBufferofLittleCat();
        return FALSE;
    }
    s_littlecat_jump6_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*s_littlecat_pic_height*s_littlecat_pic_width);
    if(PNULL == s_littlecat_jump6_buf_ptr)
    {
        FreeAllBufferofLittleCat();
        return FALSE;
    }
    s_littlecat_jump7_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*s_littlecat_pic_height*s_littlecat_pic_width);
    if(PNULL == s_littlecat_jump7_buf_ptr)
    {
        FreeAllBufferofLittleCat();
        return FALSE;
    }
    
    UILAYER_GetLayerInfo(&s_littlecat_lcd_dev_info, &layer_info);
    copy_param.rect.left   = 0;
    copy_param.rect.top    = 0;
    copy_param.rect.right  = (int16)(layer_info.layer_width - 1);
    copy_param.rect.bottom = (int16)(layer_info.layer_height - 1);

    copy_buf.data_type = DATA_TYPE_ARGB888;
    copy_buf.width     = layer_info.layer_width;
    copy_buf.height    = layer_info.layer_height;       

    UILAYER_Clear(&s_littlecat_lcd_dev_info);
    GUIRES_DisplayImg(&dis_point,
                    PNULL,PNULL,win_id,IMAGE_WALLPAPER_LITTLECAT,
						&s_littlecat_lcd_dev_info);
    
    copy_buf.buf_ptr   = (uint8*)s_littlecat_buf_ptr;
    UILAYER_CopyLayerToBuffer(&s_littlecat_lcd_dev_info,&copy_param,&copy_buf);
//    SCI_MEMCPY(s_littlecat_buf_ptr, layer_info.layer_buf_ptr,4*layer_info.layer_width*layer_info.layer_height);

    UILAYER_Clear(&s_littlecat_lcd_dev_info);
    GUIRES_DisplayImg(&dis_point,
                    PNULL,PNULL,win_id,IMAGE_LITTLECAT_DANCE_01,
						&s_littlecat_lcd_dev_info);
    copy_buf.buf_ptr   = (uint8*)s_littlecat_dance1_buf_ptr;
    UILAYER_CopyLayerToBuffer(&s_littlecat_lcd_dev_info,&copy_param,&copy_buf);
//    SCI_MEMCPY(s_littlecat_dance1_buf_ptr, layer_info.layer_buf_ptr,4*layer_info.layer_width*layer_info.layer_height);

    UILAYER_Clear(&s_littlecat_lcd_dev_info);
    GUIRES_DisplayImg(&dis_point,
                    PNULL,PNULL,win_id,IMAGE_LITTLECAT_DANCE_02,
						&s_littlecat_lcd_dev_info);
    copy_buf.buf_ptr   = (uint8*)s_littlecat_dance2_buf_ptr;
    UILAYER_CopyLayerToBuffer(&s_littlecat_lcd_dev_info,&copy_param,&copy_buf);
//    SCI_MEMCPY(s_littlecat_dance2_buf_ptr, layer_info.layer_buf_ptr,4*layer_info.layer_width*layer_info.layer_height);
    GUIRES_DisplayImg(&dis_point,
                    PNULL,PNULL,win_id,IMAGE_LITTLECAT_DANCE_03,
						&s_littlecat_lcd_dev_info);
    copy_buf.buf_ptr   = (uint8*)s_littlecat_dance3_buf_ptr;
    UILAYER_CopyLayerToBuffer(&s_littlecat_lcd_dev_info,&copy_param,&copy_buf);
//    SCI_MEMCPY(s_littlecat_dance3_buf_ptr, layer_info.layer_buf_ptr,4*layer_info.layer_width*layer_info.layer_height);

    UILAYER_Clear(&s_littlecat_lcd_dev_info);
    GUIRES_DisplayImg(&dis_point,
                    PNULL,PNULL,win_id,IMAGE_LITTLECAT_DANCE_04,
						&s_littlecat_lcd_dev_info);
    copy_buf.buf_ptr   = (uint8*)s_littlecat_dance4_buf_ptr;
    UILAYER_CopyLayerToBuffer(&s_littlecat_lcd_dev_info,&copy_param,&copy_buf);
//    SCI_MEMCPY(s_littlecat_dance4_buf_ptr, layer_info.layer_buf_ptr,4*layer_info.layer_width*layer_info.layer_height);

    UILAYER_Clear(&s_littlecat_lcd_dev_info);
    GUIRES_DisplayImg(&dis_point,
                    PNULL,PNULL,win_id,IMAGE_LITTLECAT_DANCE_05,
						&s_littlecat_lcd_dev_info);
    copy_buf.buf_ptr   = (uint8*)s_littlecat_dance5_buf_ptr;
    UILAYER_CopyLayerToBuffer(&s_littlecat_lcd_dev_info,&copy_param,&copy_buf);
//    SCI_MEMCPY(s_littlecat_dance5_buf_ptr, layer_info.layer_buf_ptr,4*layer_info.layer_width*layer_info.layer_height);

    UILAYER_Clear(&s_littlecat_lcd_dev_info);
    GUIRES_DisplayImg(&dis_point,
                    PNULL,PNULL,win_id,IMAGE_LITTLECAT_DANCE_06,
						&s_littlecat_lcd_dev_info);
    copy_buf.buf_ptr   = (uint8*)s_littlecat_dance6_buf_ptr;
    UILAYER_CopyLayerToBuffer(&s_littlecat_lcd_dev_info,&copy_param,&copy_buf);
//    SCI_MEMCPY(s_littlecat_dance6_buf_ptr, layer_info.layer_buf_ptr,4*layer_info.layer_width*layer_info.layer_height);

    UILAYER_Clear(&s_littlecat_lcd_dev_info);
    GUIRES_DisplayImg(&dis_point,
                    PNULL,PNULL,win_id,IMAGE_LITTLECAT_DANCE_07,
						&s_littlecat_lcd_dev_info);
    copy_buf.buf_ptr   = (uint8*)s_littlecat_dance7_buf_ptr;
    UILAYER_CopyLayerToBuffer(&s_littlecat_lcd_dev_info,&copy_param,&copy_buf);
//    SCI_MEMCPY(s_littlecat_dance7_buf_ptr, layer_info.layer_buf_ptr,4*layer_info.layer_width*layer_info.layer_height);

    UILAYER_Clear(&s_littlecat_lcd_dev_info);
    GUIRES_DisplayImg(&dis_point,
                    PNULL,PNULL,win_id,IMAGE_LITTLECAT_DANCE_08,
						&s_littlecat_lcd_dev_info);
    copy_buf.buf_ptr   = (uint8*)s_littlecat_dance8_buf_ptr;
    UILAYER_CopyLayerToBuffer(&s_littlecat_lcd_dev_info,&copy_param,&copy_buf);
//    SCI_MEMCPY(s_littlecat_dance8_buf_ptr, layer_info.layer_buf_ptr,4*layer_info.layer_width*layer_info.layer_height);

    UILAYER_Clear(&s_littlecat_lcd_dev_info);
    GUIRES_DisplayImg(&dis_point,
                    PNULL,PNULL,win_id,IMAGE_LITTLECAT_DANCE_09,
						&s_littlecat_lcd_dev_info);
    copy_buf.buf_ptr   = (uint8*)s_littlecat_dance9_buf_ptr;
    UILAYER_CopyLayerToBuffer(&s_littlecat_lcd_dev_info,&copy_param,&copy_buf);
//    SCI_MEMCPY(s_littlecat_dance9_buf_ptr, layer_info.layer_buf_ptr,4*layer_info.layer_width*layer_info.layer_height);

    UILAYER_Clear(&s_littlecat_lcd_dev_info);
    GUIRES_DisplayImg(&dis_point,
                    PNULL,PNULL,win_id,IMAGE_LITTLECAT_DANCE_10,
						&s_littlecat_lcd_dev_info);
    copy_buf.buf_ptr   = (uint8*)s_littlecat_dance10_buf_ptr;
    UILAYER_CopyLayerToBuffer(&s_littlecat_lcd_dev_info,&copy_param,&copy_buf);
//    SCI_MEMCPY(s_littlecat_dance10_buf_ptr, layer_info.layer_buf_ptr,4*layer_info.layer_width*layer_info.layer_height);

    UILAYER_Clear(&s_littlecat_lcd_dev_info);
    GUIRES_DisplayImg(&dis_point,
                    PNULL,PNULL,win_id,IMAGE_LITTLECAT_JUMP_01,
						&s_littlecat_lcd_dev_info);
    copy_buf.buf_ptr   = (uint8*)s_littlecat_jump1_buf_ptr;
    UILAYER_CopyLayerToBuffer(&s_littlecat_lcd_dev_info,&copy_param,&copy_buf);
//    SCI_MEMCPY(s_littlecat_jump1_buf_ptr, layer_info.layer_buf_ptr,4*layer_info.layer_width*layer_info.layer_height);

    UILAYER_Clear(&s_littlecat_lcd_dev_info);
    GUIRES_DisplayImg(&dis_point,
                    PNULL,PNULL,win_id,IMAGE_LITTLECAT_JUMP_02,
						&s_littlecat_lcd_dev_info);
    copy_buf.buf_ptr   = (uint8*)s_littlecat_jump2_buf_ptr;
    UILAYER_CopyLayerToBuffer(&s_littlecat_lcd_dev_info,&copy_param,&copy_buf);
//    SCI_MEMCPY(s_littlecat_jump2_buf_ptr, layer_info.layer_buf_ptr,4*layer_info.layer_width*layer_info.layer_height);

    UILAYER_Clear(&s_littlecat_lcd_dev_info);
    GUIRES_DisplayImg(&dis_point,
                    PNULL,PNULL,win_id,IMAGE_LITTLECAT_JUMP_03,
						&s_littlecat_lcd_dev_info);
    copy_buf.buf_ptr   = (uint8*)s_littlecat_jump3_buf_ptr;
    UILAYER_CopyLayerToBuffer(&s_littlecat_lcd_dev_info,&copy_param,&copy_buf);
//    SCI_MEMCPY(s_littlecat_jump3_buf_ptr, layer_info.layer_buf_ptr,4*layer_info.layer_width*layer_info.layer_height);

    UILAYER_Clear(&s_littlecat_lcd_dev_info);
    GUIRES_DisplayImg(&dis_point,
                    PNULL,PNULL,win_id,IMAGE_LITTLECAT_JUMP_04,
						&s_littlecat_lcd_dev_info);
    copy_buf.buf_ptr   = (uint8*)s_littlecat_jump4_buf_ptr;
    UILAYER_CopyLayerToBuffer(&s_littlecat_lcd_dev_info,&copy_param,&copy_buf);
//    SCI_MEMCPY(s_littlecat_jump4_buf_ptr, layer_info.layer_buf_ptr,4*layer_info.layer_width*layer_info.layer_height);

    UILAYER_Clear(&s_littlecat_lcd_dev_info);
    GUIRES_DisplayImg(&dis_point,
                    PNULL,PNULL,win_id,IMAGE_LITTLECAT_JUMP_05,
						&s_littlecat_lcd_dev_info);
    copy_buf.buf_ptr   = (uint8*)s_littlecat_jump5_buf_ptr;
    UILAYER_CopyLayerToBuffer(&s_littlecat_lcd_dev_info,&copy_param,&copy_buf);
//    SCI_MEMCPY(s_littlecat_jump5_buf_ptr, layer_info.layer_buf_ptr,4*layer_info.layer_width*layer_info.layer_height);

    UILAYER_Clear(&s_littlecat_lcd_dev_info);
    GUIRES_DisplayImg(&dis_point,
                    PNULL,PNULL,win_id,IMAGE_LITTLECAT_JUMP_06,
						&s_littlecat_lcd_dev_info);
    copy_buf.buf_ptr   = (uint8*)s_littlecat_jump6_buf_ptr;
    UILAYER_CopyLayerToBuffer(&s_littlecat_lcd_dev_info,&copy_param,&copy_buf);
//    SCI_MEMCPY(s_littlecat_jump6_buf_ptr, layer_info.layer_buf_ptr,4*layer_info.layer_width*layer_info.layer_height);

    UILAYER_Clear(&s_littlecat_lcd_dev_info);
    GUIRES_DisplayImg(&dis_point,
                    PNULL,PNULL,win_id,IMAGE_LITTLECAT_JUMP_07,
						&s_littlecat_lcd_dev_info);
    copy_buf.buf_ptr   = (uint8*)s_littlecat_jump7_buf_ptr;
    UILAYER_CopyLayerToBuffer(&s_littlecat_lcd_dev_info,&copy_param,&copy_buf);
//    SCI_MEMCPY(s_littlecat_jump7_buf_ptr, layer_info.layer_buf_ptr,4*layer_info.layer_width*layer_info.layer_height);

    UILAYER_RELEASELAYER(&s_littlecat_lcd_dev_info);   /*lint !e506 !e774*/
//     UILAYER_ReleaseLayer(&s_littlecat_lcd_dev_info);
     
    if(0==s_littlecat_cycle_jump_timer)
    {
        s_littlecat_cycle_jump_timer = MMK_CreateWinTimer(MAIN_IDLE_WIN_ID, MMI_LITTLECAT_JUMP_CYCLE_INTERVAL,FALSE);
    }

   b_result = TRUE;
   return b_result;
}

/*****************************************************************************/
//  Description :小猫准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LittleCatDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    BOOLEAN b_result = FALSE;
    uint16 ncol = 0;
    uint16 nrow = 0;
	
    GUI_POINT_T dis_point = {0,0};
    GUI_RECT_T lcd_rect = {0};
 lcd_rect = MMITHEME_GetFullScreenRect();

        if (PNULL == dev_info_ptr)
        {
            return FALSE;
        }
        LCD_FillRect(dev_info_ptr,lcd_rect ,MMI_WHITE_COLOR);
                  
        for(nrow=0;nrow<s_littlecat_arr_row_num;nrow++)		
        {
            for(ncol = 0;ncol<s_littlecat_arr_col_num;ncol++)
            {
                dis_point.x = ncol*s_littlecat_pic_width;//MMI_LITTLECAT_PIC_WIDTH;
                dis_point.y = nrow*s_littlecat_pic_height;//MMI_LITTLECAT_PIC_HEIGHT;

                b_result = GUIRES_DisplayImg(&dis_point,
                                                                        PNULL,
                                                                        PNULL,
                                                                        win_id,
                                                                        IMAGE_WALLPAPER_LITTLECAT,
                                                                        dev_info_ptr);
            }
        }	     
       return b_result;
}
/*****************************************************************************/
//  Description :取得小猫动画帧数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL COLOR32* GetLittleCatDancingFrameBuf(uint16 frame_index)
{
    COLOR32* buf_ptf = PNULL;
    switch(frame_index)
    {
                        case 0:
                        case 10:
                        case 20: 
                        case 30:
                //        case 40:    
                            buf_ptf = s_littlecat_dance1_buf_ptr;
                            break;
                        case 1:
                        case 11:
                        case 21:
                        case 31:
                        case 41:      
                            buf_ptf = s_littlecat_dance2_buf_ptr;
                            break;  
                        case 2:
                        case 12:
                        case 22:
                        case 32:
                        case 42:      
                            buf_ptf = s_littlecat_dance3_buf_ptr;
                            break; 
                        case 3:
                        case 13:
                        case 23:
                        case 33:
                        case 43:      
                            buf_ptf = s_littlecat_dance4_buf_ptr;
                            break;
                        case 4:
                        case 14:
                        case 24:
                        case 34:
                        case 44:      
                            buf_ptf = s_littlecat_dance5_buf_ptr;
                            break;
                        case 5:
                        case 15:
                        case 25:
                        case 35:
                        case 45:      
                            buf_ptf = s_littlecat_dance6_buf_ptr;
                            break;     
                         case 6:
                        case 16:
                        case 26:
                        case 36:
                        case 46:      
                            buf_ptf = s_littlecat_dance7_buf_ptr;
                            break;  
                        case 7:
                        case 17:
                        case 27:
                        case 37:
                        case 47:      
                            buf_ptf = s_littlecat_dance8_buf_ptr;
                            break; 
                        case 8:
                        case 18:
                        case 28:
                        case 38:
                        case 48:      
                            buf_ptf = s_littlecat_dance9_buf_ptr;
                            break;     
                         case 9:
                        case 19:
                        case 29:
                        case 39:
                        case 49:      
                            buf_ptf = s_littlecat_dance10_buf_ptr;
                            break;  
                         
                        default:
                            buf_ptf = s_littlecat_buf_ptr;
                            break;
    }
    return buf_ptf;
}
/*****************************************************************************/
//  Description :取得小猫动画帧数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL COLOR32* GetLittleCatJumpingFrameBuf(uint16 frame_index)
{
    COLOR32* buf_ptf = PNULL;
    switch(frame_index)
    {
                        case 0:
                        case 10:
                        case 20: 
                        case 30:
                        case 40:    
                            buf_ptf = s_littlecat_jump1_buf_ptr;
                            break;
                        case 1:
                        case 11:
                        case 21:
                        case 31:
                        case 41:      
                            buf_ptf = s_littlecat_jump2_buf_ptr;
                            break;  
                        case 2:
                        case 12:
                        case 22:
                        case 32:
                        case 42:      
                            buf_ptf = s_littlecat_jump3_buf_ptr;
                            break; 
                        case 3:
                        case 13:
                        case 23:
                        case 33:
                        case 43:      
                            buf_ptf = s_littlecat_jump4_buf_ptr;
                            break;
                        case 4:
                        case 14:
                        case 24:
                        case 34:
                        case 44:      
                            buf_ptf = s_littlecat_jump5_buf_ptr;
                            break;
                        case 5:
                        case 15:
                        case 25:
                        case 35:
                        case 45:      
                            buf_ptf = s_littlecat_jump6_buf_ptr;
                            break;     
                         case 6:
                        case 16:
                        case 26:
                        case 36:
                        case 46:      
                            buf_ptf = s_littlecat_jump7_buf_ptr;
                            break;  
                 
                        default:
                            buf_ptf = s_littlecat_jump7_buf_ptr;
                            break;
    }
    return buf_ptf;
}
/*****************************************************************************/
//  Description :小猫算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LittleCatCalculate(LW_CAL_PARAM *lw_cal_param)
{
    IMGREF_SIZE_T bg_window = {0};
    IMGREF_SIZE_T up_window = {0};
    FIX16_POINT_T offset = {0};
    uint16 ncol = 0;
    uint16 nrow = 0;
    COLOR32* s_image_buf_ptr = s_littlecat_buf_ptr;
    BOOL should_change_state = TRUE;
    uint8 white_color = (uint8)MMI_WHITE_COLOR;
    //GUI_RECT_T lcd_rect = MMITHEME_GetFullScreenRect();
    

    if(PNULL == lw_cal_param || PNULL == lw_cal_param->dest_buf_info.buf_ptr || PNULL == s_image_buf_ptr)
    {
        return;
    }
    
    bg_window.w = lw_cal_param->src_buf_info.w;
    bg_window.h = lw_cal_param->src_buf_info.h;
    up_window.w = s_littlecat_pic_width;
    up_window.h = s_littlecat_pic_height;

    SCI_MEMSET(lw_cal_param->dest_buf_info.buf_ptr,white_color,SCR_SIZE);
    //LCD_FillRect(target_buf_ptr,lcd_rect,MMI_WHITE_COLOR);
    switch(s_littlecat_state)    
    {
        case MMI_LITTLECAT_IDLE:
        for(nrow = 0;nrow < s_littlecat_arr_row_num;nrow++)
        {
            for(ncol = 0;ncol < s_littlecat_arr_col_num;ncol++)
            {
                offset.x = ncol*s_littlecat_pic_width;
                offset.y = nrow*s_littlecat_pic_height;
                S2D_Blending(bg_window, lw_cal_param->dest_buf_info.buf_ptr, up_window,  offset, s_littlecat_buf_ptr, lw_cal_param->dest_buf_info.buf_ptr);
            }
        }
        break;
        
        case MMI_LITTLECAT_DANCING:
        for(nrow = 0;nrow < s_littlecat_arr_row_num;nrow++)
        {
            for(ncol = 0;ncol < s_littlecat_arr_col_num;ncol++)
            {
                offset.x = ncol*s_littlecat_pic_width;
                offset.y = nrow*s_littlecat_pic_height;
                    
                if(s_littlecat_bg_array[nrow][ncol].is_select)
                {
                    
                    s_image_buf_ptr = GetLittleCatDancingFrameBuf(s_littlecat_bg_array[nrow][ncol].dance_frame_index);
                    if(PNULL == s_image_buf_ptr)
                    {
                        return;
                    }
                    s_littlecat_bg_array[nrow][ncol].dance_frame_index++;
                    if(s_littlecat_bg_array[nrow][ncol].dance_frame_index >= s_littlecat_dance_frame_num)
                        {
                            s_littlecat_bg_array[nrow][ncol].is_select = FALSE;
                            s_littlecat_bg_array[nrow][ncol].dance_frame_index = 0;
                        }
                 }
                else
                {
                    s_image_buf_ptr = s_littlecat_buf_ptr;
                }
                S2D_Blending(bg_window, lw_cal_param->dest_buf_info.buf_ptr, up_window,  offset,s_image_buf_ptr,lw_cal_param->dest_buf_info.buf_ptr);
                
            }
        }
        for(nrow = 0;nrow < s_littlecat_arr_row_num;nrow++)
        {
            for(ncol = 0;ncol < s_littlecat_arr_col_num;ncol++)
            {
                if(s_littlecat_bg_array[nrow][ncol].is_select)
                    {
                        should_change_state = FALSE;
                        break;
                    }
            }
        }
        if(should_change_state)
            {
                s_littlecat_state = MMI_LITTLECAT_IDLE;
                s_littlecat_cycle_jump_timer = MMK_CreateWinTimer(MAIN_IDLE_WIN_ID, MMI_LITTLECAT_JUMP_CYCLE_INTERVAL,FALSE);
            }
        break;
        case MMI_LITTLECAT_JUMPING:
        for(nrow = 0;nrow < s_littlecat_arr_row_num;nrow++)
        {
            for(ncol = 0;ncol < s_littlecat_arr_col_num;ncol++)
            {
                
                    offset.x = ncol*s_littlecat_pic_width;
                    offset.y = nrow*s_littlecat_pic_height;
                    if(s_littlecat_rand_row == nrow && s_littlecat_rand_col == ncol)
                    {
                        s_image_buf_ptr = GetLittleCatJumpingFrameBuf(
                            s_littlecat_bg_array[nrow][ncol].jump_frame_index
                             );
                        
                        s_littlecat_bg_array[nrow][ncol].jump_frame_index++;
                        if(s_littlecat_bg_array[nrow][ncol].jump_frame_index >= s_littlecat_jump_frame_num)
                        {
                          //  s_littlecat_bg_array[nrow][ncol].is_select = FALSE;
                            s_littlecat_bg_array[nrow][ncol].jump_frame_index = 0;
                            s_littlecat_state = MMI_LITTLECAT_IDLE;
                        s_littlecat_cycle_jump_timer = MMK_CreateWinTimer(MAIN_IDLE_WIN_ID, MMI_LITTLECAT_JUMP_CYCLE_INTERVAL,FALSE);
                        }
                   }
                    else
                        {
                        
                            s_image_buf_ptr = s_littlecat_buf_ptr;
                        }
			if(PNULL == s_image_buf_ptr)
                        {
                            s_littlecat_state = MMI_LITTLECAT_IDLE;
                            return;
                        }
                    S2D_Blending(bg_window, lw_cal_param->dest_buf_info.buf_ptr, up_window,  offset,s_image_buf_ptr,lw_cal_param->dest_buf_info.buf_ptr);
                 
                 }
           }
        break;
        default:
            break;
}
   
 //   SCI_MEMCPY((void*)(target_buf_ptr),(void*)(s_littlecat_bg_buf_ptr), SCR_SIZE);// (void*)(source_buf_ptr), SCR_SIZE);
 //   MMITHEME_StoreUpdateRect(&s_littlecat_lcd_dev_info,lcd_rect);
   
}
/*****************************************************************************/
//  Description : 小猫释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void LittleCatDestruct(void)
{
    uint16 ncol = 0;
    uint16 nrow = 0;
    s_littlecat_state = MMI_LITTLECAT_IDLE;
    
    for(nrow=0;nrow<s_littlecat_arr_row_num;nrow++)		
    {
        for(ncol = 0;ncol<s_littlecat_arr_col_num;ncol++)
        {
            s_littlecat_bg_array[nrow][ncol].is_select = FALSE;
            s_littlecat_bg_array[nrow][ncol].dance_frame_index = 0;
            s_littlecat_bg_array[nrow][ncol].jump_frame_index = 0;
        }
    }  
    MMIIDLE_StopAllLittleCatTimer();
    FreeAllBufferofLittleCat();
}

/*****************************************************************************/
//  Description : 小猫交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LittleCatHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
	//uint16 ncol = 0;
	//uint16 nrow = 0;
	//MMI_IMAGE_ID_T image_id = 0;
	//BOOL should_stop_jump_timer = TRUE;
	//BOOL should_stop_dance_time = TRUE;
	
	//GUI_RECT_T pic_rect = {0};
	uint8* win_timer_id=PNULL;
	//GUI_POINT_T dis_point = {0,0};
	
	switch(msg_id)
	{
	case MSG_OPEN_WINDOW:
        
		break;
	case MSG_FULL_PAINT:
		break;

	case MSG_TIMER:
        win_timer_id = (uint8*)param;
        if(s_littlecat_cycle_jump_timer == (*win_timer_id))
        {
            MMK_StopTimer(s_littlecat_cycle_jump_timer);
            s_littlecat_cycle_jump_timer= 0;
            if(MMI_LITTLECAT_IDLE == s_littlecat_state)
            {
                s_littlecat_state = MMI_LITTLECAT_JUMPING;
                s_littlecat_rand_row = rand()%(s_littlecat_arr_row_num);
                s_littlecat_rand_col = rand()%(s_littlecat_arr_col_num);
                s_littlecat_bg_array[s_littlecat_rand_row][s_littlecat_rand_col].jump_frame_index = 0;
            }
        }
        else if(MMI_LITTLECAT_IDLE ==s_littlecat_state)
        {
            if(0 == s_littlecat_cycle_jump_timer)
            {
		    s_littlecat_cycle_jump_timer = MMK_CreateWinTimer(win_id, MMI_LITTLECAT_JUMP_CYCLE_INTERVAL,FALSE);
            }
        }
		break;
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
        case MSG_TP_PRESS_DOWN:
        case MSG_TP_PRESS_MOVE:
        {
            GUI_POINT_T tp_point = {0};
            tp_point.x = MMK_GET_TP_X(param);
            tp_point.y = MMK_GET_TP_Y(param);
            MMIIDLE_HandleLittleCatTPDownMsg(win_id,tp_point);
        }
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
        default:
            break;
	}
    return TRUE;
}

/*****************************************************************************/
//  Description : Handle tp down msg on the littlecat wallpaper
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_HandleLittleCatTPDownMsg(MMI_HANDLE_T win_id,GUI_POINT_T tp_point)
{
    uint16 ncol = 0;
    uint16 nrow = 0;
    
    if(s_littlecat_pic_height>0&&s_littlecat_pic_width>0)
    {
        nrow = tp_point.y/s_littlecat_pic_height;
        ncol = tp_point.x/s_littlecat_pic_width;
    }
    else
    {
        nrow = tp_point.y/MMI_LITTLECAT_PIC_HEIGHT;
        ncol = tp_point.x/MMI_LITTLECAT_PIC_WIDTH;
    }
    if(nrow>s_littlecat_arr_row_num)
    {
        return;
    }
    else if(ncol>s_littlecat_arr_col_num)
    {
        return;
    }
   if(MMI_LITTLECAT_JUMPING!=s_littlecat_state)
    {
        s_littlecat_state = MMI_LITTLECAT_DANCING;
        s_littlecat_bg_array[nrow][ncol].is_select = TRUE;
        s_littlecat_bg_array[nrow][ncol].dance_frame_index = 0;
        if(s_littlecat_cycle_jump_timer>0)
        {
            MMK_StopTimer(s_littlecat_cycle_jump_timer);
            s_littlecat_cycle_jump_timer = 0;
        }
    }
}
/*****************************************************************************/
//  Description : free all the buffer allocat by the littlecat wallpaper 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void FreeAllBufferofLittleCat(void)
{
    if(PNULL!= s_littlecat_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_littlecat_buf_ptr);//MMILIVEWALLPAPER_FREE
    }
    if(PNULL!=s_littlecat_dance1_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_littlecat_dance1_buf_ptr);
    }
    if(PNULL != s_littlecat_dance2_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_littlecat_dance2_buf_ptr);
    }
    if(PNULL != s_littlecat_dance3_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_littlecat_dance3_buf_ptr);
    }
    if(PNULL!=s_littlecat_dance4_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_littlecat_dance4_buf_ptr);
    }
    if(PNULL!= s_littlecat_dance5_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_littlecat_dance5_buf_ptr);
    }
     if(PNULL!= s_littlecat_dance6_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_littlecat_dance6_buf_ptr);
    }
     if(PNULL!= s_littlecat_dance7_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_littlecat_dance7_buf_ptr);
    }
     if(PNULL!= s_littlecat_dance8_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_littlecat_dance8_buf_ptr);
    }
     if(PNULL!= s_littlecat_dance9_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_littlecat_dance9_buf_ptr);
    }
     if(PNULL!= s_littlecat_dance10_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_littlecat_dance10_buf_ptr);
    }

     if(PNULL!=s_littlecat_jump1_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_littlecat_jump1_buf_ptr);
    }
     if(PNULL!=s_littlecat_jump2_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_littlecat_jump2_buf_ptr);
    }
     if(PNULL!=s_littlecat_jump3_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_littlecat_jump3_buf_ptr);
    }
     if(PNULL!=s_littlecat_jump4_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_littlecat_jump4_buf_ptr);
    }
     if(PNULL!=s_littlecat_jump5_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_littlecat_jump5_buf_ptr);
    }
     if(PNULL!=s_littlecat_jump6_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_littlecat_jump6_buf_ptr);
    }
     if(PNULL!=s_littlecat_jump7_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_littlecat_jump7_buf_ptr);
    }
}
/*****************************************************************************/
//  Description : stop the littlecat wallpaper timer
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_StopAllLittleCatTimer(void)
{
	if(s_littlecat_cycle_jump_timer>0)
	{
		MMK_StopTimer(s_littlecat_cycle_jump_timer);
		s_littlecat_cycle_jump_timer = 0;
	}
}

/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 LittleCatGetType(void)
{
    return MMI_LIVE_WALLPAPER_TYPE_LITTLECAT;
}
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LittleCatGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr)
{
    if(thumbnail_info_ptr)
    {
        thumbnail_info_ptr->is_image_file = FALSE;
        thumbnail_info_ptr->image_id = IMAGE_WALLPAPER_LITTLECAT_THUMBNAIL;

        thumbnail_info_ptr->is_not_string = FALSE;
        thumbnail_info_ptr->text_id = TXT_SET_WALLPAPER_LITTLECAT;
        thumbnail_info_ptr->instance_mem_pool_size = BLOCK_MEM_POOL_SIZE_LITTLECAT;
    }
}
#endif// LIVE_WALLPAPER_SUPPORT_LITTLECAT


#endif //_MMI_LIVEWALLPAPER_LITTLECAT_C_




/*Edit by script, ignore 2 case. Thu Apr 26 19:00:58 2012*/ //IGNORE9527
