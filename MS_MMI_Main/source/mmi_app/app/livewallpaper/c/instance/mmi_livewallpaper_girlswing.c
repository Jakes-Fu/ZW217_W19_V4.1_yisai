

/*****************************************************************************
** File Name:      mmi_livewallpaper_girlswing.c                                    *
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
#ifndef _MMI_LIVEWALLPAPER_GIRLSWING_C_
#define _MMI_LIVEWALLPAPER_GIRLSWING_C_

#include "mmi_app_livewallpaper_trc.h"
#ifdef LIVE_WALLPAPER_SUPPORT_GIRLSWING

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/

#include "mmilivewallpaper_export.h"
#include "mmi_text.h"
#include "mmi_image.h"
#include "mmiset_text.h"
#include "mmiset_image.h"
#include "mmi_livewallpaper_image.h"
#include "mmi_livewallpaper_text.h"
#include "graphics_3d_rotate.h"
/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
#define FNUM 10
#define ONUM 5
#define G_NUM 4
#define OKASHI_NUM 4
#define R_MOVE_NUM 4
#define R_OUT_NUM 4
#define R_STOP_NUM 2
#define RISU_NUM 9
#define CHO_NUM 2
#define SUBAKO_NUM 4
#ifdef MAINLCD_SIZE_320X480
#define UP_WIN_X               48
#define UP_WIN_Y               155
#define DOWN_WIN_X          48
#define DOWN_WIN_Y          166
#define UP_IMAGE_X            47
#define UP_IMAGE_Y            3
#define DOWN_IMAGE_X       41
#define DOWN_IMAGE_Y       3
#define BALLON_PNG_X 102
#define BALLON_PNG_Y    85
#define CHO_PNG_X    212
#define CHO_PNG_Y    319
#define BALLON_PNG_G_X   133
#define BALLON_PNG_G_Y    95
#define GIRL_PNG_X      77
#define GIRL_PNG_Y      110
#define ROPE_LOCATION    77  
#define RISU03_PNG_X      234  
#define RISU03_PNG_Y 	273
#define RISU04_PNG_X      231   
#define RISU04_PNG_Y      273
#define RISU05_PNG_X      230   
#define RISU05_PNG_Y      273
#define RISU06_PNG_X      230   
#define RISU06_PNG_Y      274
#define RISU07_PNG_X      230   
#define RISU07_PNG_Y      277
#define RISU08_PNG_X      226   
#define RISU08_PNG_Y      279

#define OKASHI_05_PNG_X       17  
#define OKASHI_04_PNG_X       52  
#define OKASHI_03_PNG_X       99  
#define OKASHI_02_PNG_X      155  
#define OKASHI_05_PNG_Y       89  
#define OKASHI_04_PNG_Y       83  
#define OKASHI_03_PNG_Y      100  
#define OKASHI_02_PNG_Y      101  


#define RABBIT_IN_01_X   13      
#define RABBIT_IN_02_X   17      
#define RABBIT_IN_03_X   27      
#define RABBIT_IN_04_X   53      
#define RABBIT_IN_05_X   67      
#define RABBIT_IN_06_X   88      
#define RABBIT_IN_07_X   123     
#define RABBIT_IN_08_X   108     
#define RABBIT_IN_01_Y   411     
#define RABBIT_IN_02_Y   409     
#define RABBIT_IN_03_Y   407     
#define RABBIT_IN_04_Y   411     
#define RABBIT_IN_05_Y   405     
#define RABBIT_IN_06_Y   403     
#define RABBIT_IN_07_Y   401     
#define RABBIT_IN_08_Y   371     

#define RABBIT_OUT_01_X   108   
#define RABBIT_OUT_02_X   99    
#define RABBIT_OUT_03_X   91    
#define RABBIT_OUT_04_X   59    
#define RABBIT_OUT_05_X   40    
#define RABBIT_OUT_01_Y   400   
#define RABBIT_OUT_02_Y   403   
#define RABBIT_OUT_03_Y   403   
#define RABBIT_OUT_04_Y   405   
#define RABBIT_OUT_05_Y   405   

#define SUBAKO_X 229
#define SUBAKO_Y 167

#define BLOCK_MEM_POOL_SIZE_GIRLSWING (570 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;

#endif

#ifdef  MAINLCD_SIZE_240X400
#define UP_WIN_X               45
#define UP_WIN_Y               130
#define DOWN_WIN_X          45
#define DOWN_WIN_Y          140
#define UP_IMAGE_X            44
#define UP_IMAGE_Y     0
#define DOWN_IMAGE_X       40 
#define DOWN_IMAGE_Y   0
#define CHO_PNG_X    176
#define CHO_PNG_Y    270
#define BALLON_PNG_X    86
#define BALLON_PNG_Y    71
#define BALLON_PNG_G_X   109
#define BALLON_PNG_G_Y    81
#define GIRL_PNG_X      64
#define GIRL_PNG_Y      91
#define ROPE_LOCATION    64 
#define RISU03_PNG_X    195
#define RISU04_PNG_X    191
#define RISU05_PNG_X    189
#define RISU06_PNG_X    188
#define RISU07_PNG_X    190
#define RISU08_PNG_X    186
#define RISU03_PNG_Y	219
#define RISU04_PNG_Y    220
#define RISU05_PNG_Y    220
#define RISU06_PNG_Y    221
#define RISU07_PNG_Y    224
#define RISU08_PNG_Y    226

#define OKASHI_05_PNG_X       14 
#define OKASHI_04_PNG_X       43 
#define OKASHI_03_PNG_X       82 
#define OKASHI_02_PNG_X      129 
#define OKASHI_05_PNG_Y      74  
#define OKASHI_04_PNG_Y      69  
#define OKASHI_03_PNG_Y      83  
#define OKASHI_02_PNG_Y      84  


#define RABBIT_IN_01_X   25        
#define RABBIT_IN_02_X   26        
#define RABBIT_IN_03_X   42        
#define RABBIT_IN_04_X   72        
#define RABBIT_IN_05_X   74        
#define RABBIT_IN_06_X   88        
#define RABBIT_IN_07_X   118       
#define RABBIT_IN_08_X   68        
#define RABBIT_IN_01_Y    349      
#define RABBIT_IN_02_Y    349      
#define RABBIT_IN_03_Y    348      
#define RABBIT_IN_04_Y    347      
#define RABBIT_IN_05_Y    341      
#define RABBIT_IN_06_Y    340      
#define RABBIT_IN_07_Y    339      
#define RABBIT_IN_08_Y   313  

#define RABBIT_OUT_01_X   99   
#define RABBIT_OUT_02_X   91   
#define RABBIT_OUT_03_X   76   
#define RABBIT_OUT_04_X   49   
#define RABBIT_OUT_05_X   32   
#define RABBIT_OUT_01_Y   342  
#define RABBIT_OUT_02_Y   345  
#define RABBIT_OUT_03_Y   344  
#define RABBIT_OUT_04_Y   343  
#define RABBIT_OUT_05_Y   344  

#define SUBAKO_X 191
#define SUBAKO_Y 139

#define BLOCK_MEM_POOL_SIZE_GIRLSWING (450 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;

#endif

#ifdef  MAINLCD_SIZE_240X320
#define UP_WIN_X               38
#define UP_WIN_Y               118
#define DOWN_WIN_X          39
#define DOWN_WIN_Y          127
#define UP_IMAGE_X            35
#define UP_IMAGE_Y            0
#define DOWN_IMAGE_X       32
#define DOWN_IMAGE_Y       0
#define CHO_PNG_X 156
#define CHO_PNG_Y    211
#define BALLON_PNG_X    103
#define BALLON_PNG_Y    39
#define BALLON_PNG_G_X   99
#define BALLON_PNG_G_Y    45
#define GIRL_PNG_X      57
#define GIRL_PNG_Y      52
#define ROPE_LOCATION    58  
#define RISU03_PNG_X          175
#define RISU04_PNG_X          173
#define RISU05_PNG_X          172
#define RISU06_PNG_X          172
#define RISU07_PNG_X          172
#define RISU08_PNG_X          169
#define RISU03_PNG_Y	    174
#define RISU04_PNG_Y          174
#define RISU05_PNG_Y          174
#define RISU06_PNG_Y          176
#define RISU07_PNG_Y          176
#define RISU08_PNG_Y          179

#define OKASHI_05_PNG_X      12 
#define OKASHI_04_PNG_X      39 
#define OKASHI_03_PNG_X      74 
#define OKASHI_02_PNG_X     116 
#define OKASHI_05_PNG_Y      40 
#define OKASHI_04_PNG_Y      35 
#define OKASHI_03_PNG_Y      51 
#define OKASHI_02_PNG_Y      49 


#define RABBIT_IN_01_X   16  
#define RABBIT_IN_02_X   22  
#define RABBIT_IN_03_X   37  
#define RABBIT_IN_04_X   62  
#define RABBIT_IN_05_X   61  
#define RABBIT_IN_06_X   70  
#define RABBIT_IN_07_X   94  
#define RABBIT_IN_08_X   59  
#define RABBIT_IN_01_Y   278 
#define RABBIT_IN_02_Y   278 
#define RABBIT_IN_03_Y   278 
#define RABBIT_IN_04_Y   278 
#define RABBIT_IN_05_Y   273 
#define RABBIT_IN_06_Y   272 
#define RABBIT_IN_07_Y   272 
#define RABBIT_IN_08_Y   242 


#define RABBIT_OUT_01_X   74  
#define RABBIT_OUT_02_X   70  
#define RABBIT_OUT_03_X   56  
#define RABBIT_OUT_04_X   33  
#define RABBIT_OUT_05_X   26  
#define RABBIT_OUT_01_Y   267 
#define RABBIT_OUT_02_Y   272 
#define RABBIT_OUT_03_Y   273 
#define RABBIT_OUT_04_Y   273 
#define RABBIT_OUT_05_Y   273 

#define SUBAKO_X 171
#define SUBAKO_Y 95

#define BLOCK_MEM_POOL_SIZE_GIRLSWING (400 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;

#endif

#ifdef  MAINLCD_SIZE_176X220
#define UP_WIN_X               38
#define UP_WIN_Y               118
#define DOWN_WIN_X          39
#define DOWN_WIN_Y          127
#define UP_IMAGE_X            35
#define UP_IMAGE_Y            0
#define DOWN_IMAGE_X       32
#define DOWN_IMAGE_Y       0
#define CHO_PNG_X 156
#define CHO_PNG_Y    211
#define BALLON_PNG_X    103
#define BALLON_PNG_Y    39
#define BALLON_PNG_G_X   99
#define BALLON_PNG_G_Y    45
#define GIRL_PNG_X      57
#define GIRL_PNG_Y      52
#define ROPE_LOCATION    58  
#define RISU03_PNG_X          175
#define RISU04_PNG_X          173
#define RISU05_PNG_X          172
#define RISU06_PNG_X          172
#define RISU07_PNG_X          172
#define RISU08_PNG_X          169
#define RISU03_PNG_Y	    174
#define RISU04_PNG_Y          174
#define RISU05_PNG_Y          174
#define RISU06_PNG_Y          176
#define RISU07_PNG_Y          176
#define RISU08_PNG_Y          179

#define OKASHI_05_PNG_X      12 
#define OKASHI_04_PNG_X      39 
#define OKASHI_03_PNG_X      74 
#define OKASHI_02_PNG_X     116 
#define OKASHI_05_PNG_Y      40 
#define OKASHI_04_PNG_Y      35 
#define OKASHI_03_PNG_Y      51 
#define OKASHI_02_PNG_Y      49 


#define RABBIT_IN_01_X   16  
#define RABBIT_IN_02_X   22  
#define RABBIT_IN_03_X   37  
#define RABBIT_IN_04_X   62  
#define RABBIT_IN_05_X   61  
#define RABBIT_IN_06_X   70  
#define RABBIT_IN_07_X   94  
#define RABBIT_IN_08_X   59  
#define RABBIT_IN_01_Y   278 
#define RABBIT_IN_02_Y   278 
#define RABBIT_IN_03_Y   278 
#define RABBIT_IN_04_Y   278 
#define RABBIT_IN_05_Y   273 
#define RABBIT_IN_06_Y   272 
#define RABBIT_IN_07_Y   272 
#define RABBIT_IN_08_Y   242 


#define RABBIT_OUT_01_X   74  
#define RABBIT_OUT_02_X   70  
#define RABBIT_OUT_03_X   56  
#define RABBIT_OUT_04_X   33  
#define RABBIT_OUT_05_X   26  
#define RABBIT_OUT_01_Y   267 
#define RABBIT_OUT_02_Y   272 
#define RABBIT_OUT_03_Y   273 
#define RABBIT_OUT_04_Y   273 
#define RABBIT_OUT_05_Y   273 

#define SUBAKO_X 171
#define SUBAKO_Y 95

#define BLOCK_MEM_POOL_SIZE_GIRLSWING (400 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;

#endif

#ifdef  MAINLCD_SIZE_128X160
#define UP_WIN_X               38
#define UP_WIN_Y               118
#define DOWN_WIN_X          39
#define DOWN_WIN_Y          127
#define UP_IMAGE_X            35
#define UP_IMAGE_Y            0
#define DOWN_IMAGE_X       32
#define DOWN_IMAGE_Y       0
#define CHO_PNG_X 156
#define CHO_PNG_Y    211
#define BALLON_PNG_X    103
#define BALLON_PNG_Y    39
#define BALLON_PNG_G_X   99
#define BALLON_PNG_G_Y    45
#define GIRL_PNG_X      57
#define GIRL_PNG_Y      52
#define ROPE_LOCATION    58  
#define RISU03_PNG_X          175
#define RISU04_PNG_X          173
#define RISU05_PNG_X          172
#define RISU06_PNG_X          172
#define RISU07_PNG_X          172
#define RISU08_PNG_X          169
#define RISU03_PNG_Y	    174
#define RISU04_PNG_Y          174
#define RISU05_PNG_Y          174
#define RISU06_PNG_Y          176
#define RISU07_PNG_Y          176
#define RISU08_PNG_Y          179

#define OKASHI_05_PNG_X      12 
#define OKASHI_04_PNG_X      39 
#define OKASHI_03_PNG_X      74 
#define OKASHI_02_PNG_X     116 
#define OKASHI_05_PNG_Y      40 
#define OKASHI_04_PNG_Y      35 
#define OKASHI_03_PNG_Y      51 
#define OKASHI_02_PNG_Y      49 


#define RABBIT_IN_01_X   16  
#define RABBIT_IN_02_X   22  
#define RABBIT_IN_03_X   37  
#define RABBIT_IN_04_X   62  
#define RABBIT_IN_05_X   61  
#define RABBIT_IN_06_X   70  
#define RABBIT_IN_07_X   94  
#define RABBIT_IN_08_X   59  
#define RABBIT_IN_01_Y   278 
#define RABBIT_IN_02_Y   278 
#define RABBIT_IN_03_Y   278 
#define RABBIT_IN_04_Y   278 
#define RABBIT_IN_05_Y   273 
#define RABBIT_IN_06_Y   272 
#define RABBIT_IN_07_Y   272 
#define RABBIT_IN_08_Y   242 


#define RABBIT_OUT_01_X   74  
#define RABBIT_OUT_02_X   70  
#define RABBIT_OUT_03_X   56  
#define RABBIT_OUT_04_X   33  
#define RABBIT_OUT_05_X   26  
#define RABBIT_OUT_01_Y   267 
#define RABBIT_OUT_02_Y   272 
#define RABBIT_OUT_03_Y   273 
#define RABBIT_OUT_04_Y   273 
#define RABBIT_OUT_05_Y   273 

#define SUBAKO_X 171
#define SUBAKO_Y 95

#define BLOCK_MEM_POOL_SIZE_GIRLSWING (400 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;

#endif

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef struct
{
   COLOR32 * buff_ptr;
   IMGREF_SIZE_T ima_size;
   IMGREF_POINT_T ima_location;
   MMI_IMAGE_ID_T id;
   
}IMAGE_INFO_T;

/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL IMGREF_SIZE_T sWindow;
LOCAL int Level1[FNUM]={ 193*65536,188*65536, 182*65536,  173*65536,  160*65536, 141*65536, 116*65536, 82*65536, 42*65536,0};
LOCAL uint16 Level2[ONUM]={  94,  86,   70, 41,  0 };

IMAGE_INFO_T girl_image;
IMAGE_INFO_T okashi_image[OKASHI_NUM];
IMAGE_INFO_T m_rabit_image[R_MOVE_NUM];
IMAGE_INFO_T out_rabit_image[R_OUT_NUM];
IMAGE_INFO_T s_rabit_image[R_STOP_NUM];
IMAGE_INFO_T risu_image[RISU_NUM];
IMAGE_INFO_T balloon_image;
IMAGE_INFO_T candy_image;
IMAGE_INFO_T cho_image[CHO_NUM];

IMAGE_INFO_T subako_image[SUBAKO_NUM];
LOCAL int cur_angle;
LOCAL UILAYER_INFO_T s_girlswing_layer_info ;
LOCAL GUI_LCD_DEV_INFO s_girlswing_layer_handle ;
LOCAL FIX16_POINT_T * Rota_buff ;

FIX16_POINT_T rabit_in_offset[8];
FIX16_POINT_T rabit_out_offset[5];

int m_girl_frame;
int m_okashi_frame[4];
int m_okashi_total_frame;
int m_girl_total_frame;
int in_rabbit_frame;
int out_rabbit_frame;
int shake_rabbit_frame;
int magnify_frame;
int m_risu_frame;
int m_subako_frame;
int shake_risu_frame;
int shake_cho_frame;
IMGREF_POINT_T offset_w,offset_girl;
int rabbit_case;
BOOLEAN Is_magjnify;
IMAGE_INFO_T up_leg_image={0};
IMAGE_INFO_T down_leg_image={0};
COLOR32 * clean_girl_buf=PNULL;
LOCAL int angle_up[4]={  0,  -100,   -200, -320};
LOCAL int angle_down[4]={  0,  100,   200, 320};
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
LOCAL BOOLEAN GirlswingInit(void);
/*****************************************************************************/
//  Description : 初始化
//  Global resource dependence : 
//  Author:Anson.Yin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GirlswingConstruct(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:Anson.Yin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GirlswingDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : 准备背景数据
//  Global resource dependence : 
//  Author:Anson.Yin
//  Note:
/*****************************************************************************/
LOCAL void GirlswingCalculate(LW_CAL_PARAM *lw_cal_param);
/*****************************************************************************/
//  Description : 释放算法
//  Global resource dependence : 
//  Author:Anson.Yin
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void GirlswingDestruct(void);
LOCAL BOOLEAN GirlswingHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
/*****************************************************************************/
//  Description : 交互事件处理
//  Global resource dependence : 
//  Author:Anson.Yin
//  Note:
/*****************************************************************************/
LOCAL void Girl_Swing(COLOR16 * dst,COLOR32 * front,FIX16_POINT_T* rota_buffer,IMGREF_SIZE_T win_size,IMGREF_SIZE_T img_size,IMGREF_POINT_T offset_window,IMGREF_POINT_T offset_img,int cur_frame,int total_frame,int strong_level);
/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

BOOLEAN get_focus(IMGREF_SIZE_T win_size,IMGREF_SIZE_T img,GUI_POINT_T click_p);
LOCAL void Okashi_Swing(COLOR16 * dst,COLOR32 * front,FIX16_POINT_T* rota_buffer,IMGREF_SIZE_T win_size,IMGREF_SIZE_T img_size,IMGREF_POINT_T offset_window,IMGREF_POINT_T offset_img,int cur_frame,int total_frame);
/*****************************************************************************/
//  Description :女孩子荡秋千运动函数
//  Global resource dependence : 
//  Author:Anson.Yin
//  Note:2011.9.10
/*****************************************************************************/
LOCAL void get_wid_h( IMGREF_SIZE_T *  img_size,MMI_IMAGE_ID_T image_id,MMI_HANDLE_T win_id);
LOCAL void MMIIDLE_CreateImageLayer(uint32 *buf_ptr, UILAYER_INFO_T *layer_info_ptr, GUI_LCD_DEV_INFO *layer_handle_ptr, MMI_IMAGE_ID_T image_id, MMI_HANDLE_T win_id);
LOCAL void Rabbit_Move(COLOR16 * bg_buf,COLOR16 * out_buf);
LOCAL void Risu_Move(COLOR16 * bg_buf,COLOR16 * out_buf);
LOCAL BOOLEAN get_animal_focus(IMGREF_SIZE_T win_size, IMAGE_INFO_T img,GUI_POINT_T click_p);
LOCAL BOOLEAN Combine_Pic(int c_angle_up,int c_angle_down,FIX16_POINT_T * Point_Add_ptr);
/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 GirlswingGetType(void);
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void GirlswingGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr);
 #if 1
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_girlswing_Instance =
{
    GirlswingInit,
    GirlswingConstruct,
    GirlswingDrawBg,
    GirlswingCalculate,
    GirlswingDestruct,
    GirlswingHandleEvent,
    PNULL,
    GirlswingGetType,
    GirlswingGetThumbNailInfo,
    PNULL,
    PNULL,
    PNULL,
    PNULL
};


#endif

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                                                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 静态数据初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GirlswingInit(void)
{     
    BOOLEAN b_result = TRUE;
	sWindow.w=MMI_MAINSCREEN_WIDTH;
	sWindow.h=MMI_MAINSCREEN_HEIGHT;
	m_girl_frame=1;
	m_girl_total_frame=10;
	m_okashi_total_frame=5;
	in_rabbit_frame=0;
	shake_rabbit_frame=0;
	magnify_frame=0;
	m_risu_frame=0;
	shake_risu_frame=2;
	m_subako_frame=0;
	shake_cho_frame=0;
	rabbit_case=0;
	m_okashi_frame[0]=rand()%18;
	m_okashi_frame[1]=rand()%18;
	m_okashi_frame[2]=rand()%18;
	m_okashi_frame[3]=rand()%18;
	

		
	girl_image.ima_location.x=GIRL_PNG_X;
	girl_image.ima_location.y=GIRL_PNG_Y;
	
	okashi_image[0].id=IMAGE_WALLPAPER_GIRL_SWING_OKASHI_02;

	okashi_image[0].ima_location.x=OKASHI_02_PNG_X;
	okashi_image[0].ima_location.y=OKASHI_02_PNG_Y;
		
	okashi_image[1].id=IMAGE_WALLPAPER_GIRL_SWING_OKASHI_03;

	okashi_image[1].ima_location.x=OKASHI_03_PNG_X;
	okashi_image[1].ima_location.y=OKASHI_03_PNG_Y;
	
	okashi_image[2].id=IMAGE_WALLPAPER_GIRL_SWING_OKASHI_04;

	okashi_image[2].ima_location.x=OKASHI_04_PNG_X;
	okashi_image[2].ima_location.y=OKASHI_04_PNG_Y;
	
	okashi_image[3].id=IMAGE_WALLPAPER_GIRL_SWING_OKASHI_05;

	okashi_image[3].ima_location.x=OKASHI_05_PNG_X;
	okashi_image[3].ima_location.y=OKASHI_05_PNG_Y;
	
      rabit_in_offset[0].x=RABBIT_IN_01_X;   
	rabit_in_offset[1].x=RABBIT_IN_02_X;   
	rabit_in_offset[2].x=RABBIT_IN_03_X;   
	rabit_in_offset[3].x=RABBIT_IN_04_X;   
	rabit_in_offset[4].x=RABBIT_IN_05_X;   
	rabit_in_offset[5].x=RABBIT_IN_06_X;   
	rabit_in_offset[6].x=RABBIT_IN_07_X;   
	rabit_in_offset[7].x=RABBIT_IN_08_X;   
	rabit_in_offset[0].y=RABBIT_IN_01_Y;   
	rabit_in_offset[1].y=RABBIT_IN_02_Y;   
	rabit_in_offset[2].y=RABBIT_IN_03_Y;   
	rabit_in_offset[3].y=RABBIT_IN_04_Y;   
	rabit_in_offset[4].y=RABBIT_IN_05_Y;   
	rabit_in_offset[5].y=RABBIT_IN_06_Y;   
	rabit_in_offset[6].y=RABBIT_IN_07_Y;   
	rabit_in_offset[7].y=RABBIT_IN_08_Y;   

	risu_image[2].ima_location.x=RISU03_PNG_X;
	risu_image[2].ima_location.y=RISU03_PNG_Y;
	risu_image[3].ima_location.x=RISU04_PNG_X;
	risu_image[3].ima_location.y=RISU04_PNG_Y;
	risu_image[4].ima_location.x=RISU05_PNG_X;
	risu_image[4].ima_location.y=RISU05_PNG_Y;	
	risu_image[5].ima_location.x=RISU06_PNG_X;
	risu_image[5].ima_location.y=RISU06_PNG_Y;	
	risu_image[6].ima_location.x=RISU07_PNG_X;
	risu_image[6].ima_location.y=RISU07_PNG_Y;
	risu_image[7].ima_location.x=RISU08_PNG_X;
	risu_image[7].ima_location.y=RISU08_PNG_Y;	

	rabit_out_offset[0].x=RABBIT_OUT_01_X; 
	rabit_out_offset[1].x=RABBIT_OUT_02_X; 
	rabit_out_offset[2].x=RABBIT_OUT_03_X; 
	rabit_out_offset[3].x=RABBIT_OUT_04_X; 
	rabit_out_offset[4].x=RABBIT_OUT_05_X; 
	rabit_out_offset[0].y=RABBIT_OUT_01_Y; 
	rabit_out_offset[1].y=RABBIT_OUT_02_Y; 
	rabit_out_offset[2].y=RABBIT_OUT_03_Y; 
	rabit_out_offset[3].y=RABBIT_OUT_04_Y; 
	rabit_out_offset[4].y=RABBIT_OUT_05_Y; 

    return b_result;
	
}

/*****************************************************************************/
//  Description : 线性光幕初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GirlswingConstruct(MMI_WIN_ID_T win_id)
{         
    uint16 i=0,y=0,x=0,width=0,height=0;
    BOOLEAN b_result = FALSE;
      
    Rota_buff=MMILIVEWALLPAPER_ALLOC(2048*4);  //SCI_ALLOCA
    //SCI_ASSERT(PNULL != Rota_buff);
    if(PNULL == Rota_buff)
    {
        return b_result;
    }
	  	
	
		girl_image.id=IMAGE_WALLPAPER_GIRL_SWING_01+i;
		get_wid_h(&(girl_image.ima_size),girl_image.id, win_id);
		girl_image.buff_ptr= MMILIVEWALLPAPER_ALLOC(girl_image.ima_size.w*girl_image.ima_size.h*sizeof(COLOR32));
    	if(PNULL == girl_image.buff_ptr)
        {
            return b_result;
        }
           
		MMIIDLE_CreateImageLayer(girl_image.buff_ptr, &s_girlswing_layer_info, &s_girlswing_layer_handle, girl_image.id, win_id);
	      height=girl_image.ima_size.h;
		width=girl_image.ima_size.w;
		for(y=0;y<height;y++)
             	{
             		for(x=0;x<width;x++)
         			{
         				if(girl_image.buff_ptr[y*width+x]<0x08000000)
						{girl_image.buff_ptr[y*width+x]=0;}
         			}
             	}
         // create leg image
        up_leg_image.id=IMAGE_WALLPAPER_GIRL_SWING_LEG_01;
        get_wid_h(&(up_leg_image.ima_size),up_leg_image.id, win_id);
        up_leg_image.buff_ptr= MMILIVEWALLPAPER_ALLOC(up_leg_image.ima_size.w*up_leg_image.ima_size.h*sizeof(COLOR32));
        if(PNULL == up_leg_image.buff_ptr)
        {
        return b_result;
        }

        MMIIDLE_CreateImageLayer(up_leg_image.buff_ptr, &s_girlswing_layer_info, &s_girlswing_layer_handle, up_leg_image.id, win_id);

        down_leg_image.id=IMAGE_WALLPAPER_GIRL_SWING_LEG_02;
        get_wid_h(&(down_leg_image.ima_size),down_leg_image.id, win_id);
        down_leg_image.buff_ptr= MMILIVEWALLPAPER_ALLOC(down_leg_image.ima_size.w*down_leg_image.ima_size.h*sizeof(COLOR32));
        if(PNULL == down_leg_image.buff_ptr)
        {
        return b_result;
        }

        MMIIDLE_CreateImageLayer(down_leg_image.buff_ptr, &s_girlswing_layer_info, &s_girlswing_layer_handle, down_leg_image.id, win_id);
        clean_girl_buf=MMILIVEWALLPAPER_ALLOC(girl_image.ima_size.w*girl_image.ima_size.h*sizeof(COLOR32));
    	if(PNULL == clean_girl_buf)
    	{
            return b_result;
        }
        SCI_MEMCPY(clean_girl_buf, girl_image.buff_ptr, (girl_image.ima_size.w*girl_image.ima_size.h*sizeof(uint32)));
		Combine_Pic(angle_up[0],angle_down[0],Rota_buff);

	for(i=0;i<R_MOVE_NUM;i++)
	{
		
		m_rabit_image[i].id=IMAGE_WALLPAPER_GIRL_SWING_RABBIT_01+i;
		get_wid_h(&(m_rabit_image[i].ima_size),m_rabit_image[i].id, win_id);
		m_rabit_image[i].buff_ptr= MMILIVEWALLPAPER_ALLOC(m_rabit_image[i].ima_size.w*m_rabit_image[i].ima_size.h*sizeof(COLOR32));
    	if(PNULL == m_rabit_image[i].buff_ptr)
        {
            return b_result;
        }
           
		MMIIDLE_CreateImageLayer(m_rabit_image[i].buff_ptr, &s_girlswing_layer_info, &s_girlswing_layer_handle, m_rabit_image[i].id, win_id);
	
	}
	for(i=0;i<R_OUT_NUM;i++)
	{
		
		
		out_rabit_image[i].ima_size.w=m_rabit_image[i].ima_size.w;
		out_rabit_image[i].ima_size.h=m_rabit_image[i].ima_size.h;
		out_rabit_image[i].buff_ptr= MMILIVEWALLPAPER_ALLOC(m_rabit_image[i].ima_size.w*m_rabit_image[i].ima_size.h*sizeof(COLOR32));
    	if(PNULL == m_rabit_image[i].buff_ptr)
        {
            return b_result;
        }
		height=out_rabit_image[i].ima_size.h;
		width=out_rabit_image[i].ima_size.w;
             for(y=0;y<height;y++)
             	{
             		for(x=0;x<width;x++)
         			{
         				out_rabit_image[i].buff_ptr[y*width+x]=m_rabit_image[i].buff_ptr[y*width+width-x-1];
         			}
             	}
		
	
	}
	for(i=0;i<CHO_NUM;i++)
	{
		
		cho_image[i].id=IMAGE_WALLPAPER_GIRL_SWING_CHO_03+i;
		get_wid_h(&(cho_image[i].ima_size),cho_image[i].id, win_id);
		cho_image[i].ima_location.x=CHO_PNG_X;
		cho_image[i].ima_location.y=CHO_PNG_Y;
		cho_image[i].buff_ptr= MMILIVEWALLPAPER_ALLOC(cho_image[i].ima_size.w*cho_image[i].ima_size.h*sizeof(COLOR32));
    	if(PNULL == cho_image[i].buff_ptr)
        {
            return b_result;
        }
           
		MMIIDLE_CreateImageLayer(cho_image[i].buff_ptr, &s_girlswing_layer_info, &s_girlswing_layer_handle, cho_image[i].id, win_id);
	
	}
	for(i=0;i<R_STOP_NUM;i++)
	{
		
		s_rabit_image[i].id=IMAGE_WALLPAPER_GIRL_SWING_RABBIT_09+i;
		get_wid_h(&(s_rabit_image[i].ima_size),s_rabit_image[i].id, win_id);
		s_rabit_image[i].ima_location.x=rabit_in_offset[7].x-s_rabit_image[i].ima_size.w;
		s_rabit_image[i].ima_location.y=rabit_in_offset[7].y;
		s_rabit_image[i].buff_ptr= MMILIVEWALLPAPER_ALLOC(s_rabit_image[i].ima_size.w*s_rabit_image[i].ima_size.h*sizeof(COLOR32));
    	if(PNULL == s_rabit_image[i].buff_ptr)
        {
            return b_result;
        }
           
		MMIIDLE_CreateImageLayer(s_rabit_image[i].buff_ptr, &s_girlswing_layer_info, &s_girlswing_layer_handle, s_rabit_image[i].id, win_id);
	
	}
	
	for(i=0;i<RISU_NUM;i++)
	{
		
		risu_image[i].id=IMAGE_WALLPAPER_GIRL_SWING_RISU_01+i;
		get_wid_h(&(risu_image[i].ima_size),risu_image[i].id, win_id);
		risu_image[i].buff_ptr= MMILIVEWALLPAPER_ALLOC(risu_image[i].ima_size.w*risu_image[i].ima_size.h*sizeof(COLOR32));
    	if(PNULL == risu_image[i].buff_ptr)
        {
            return b_result;
        }
           
		MMIIDLE_CreateImageLayer(risu_image[i].buff_ptr, &s_girlswing_layer_info, &s_girlswing_layer_handle, risu_image[i].id, win_id);
	
	}
	
	for(i=0;i<OKASHI_NUM;i++)
	{
		
		get_wid_h(&(okashi_image[i].ima_size),okashi_image[i].id, win_id);
		okashi_image[i].buff_ptr= MMILIVEWALLPAPER_ALLOC(okashi_image[i].ima_size.w*okashi_image[i].ima_size.h*sizeof(COLOR32));
    	if(PNULL == okashi_image[i].buff_ptr)
        {
            return b_result;
        }
           
		MMIIDLE_CreateImageLayer(okashi_image[i].buff_ptr,&s_girlswing_layer_info, &s_girlswing_layer_handle,okashi_image[i].id, win_id);
	
	}
	for(i=0;i<SUBAKO_NUM;i++)
	{
		subako_image[i].id=IMAGE_WALLPAPER_GIRL_SWING_SUBAKU_01+i;
		get_wid_h(&(subako_image[i].ima_size),subako_image[i].id, win_id);
		subako_image[i].buff_ptr= MMILIVEWALLPAPER_ALLOC(subako_image[i].ima_size.w*subako_image[i].ima_size.h*sizeof(COLOR32));
    	if(PNULL == subako_image[i].buff_ptr)
    	{
            return b_result;
        }
		subako_image[i].ima_location.x=SUBAKO_X;
		subako_image[i].ima_location.y=SUBAKO_Y;
		
	if(subako_image[i].buff_ptr == PNULL)
    	{
            return b_result;
        }
           
		MMIIDLE_CreateImageLayer(subako_image[i].buff_ptr,&s_girlswing_layer_info, &s_girlswing_layer_handle,subako_image[i].id, win_id);
	
	}
	balloon_image.id=IMAGE_WALLPAPER_GIRL_SWING_BALLOON;
	get_wid_h(&(balloon_image.ima_size),balloon_image.id, win_id);
	balloon_image.ima_location.x=BALLON_PNG_X;
	balloon_image.ima_location.y=BALLON_PNG_Y;
	balloon_image.buff_ptr= MMILIVEWALLPAPER_ALLOC(balloon_image.ima_size.w*balloon_image.ima_size.h*sizeof(COLOR32));
    if(PNULL == balloon_image.buff_ptr)
    {
        return b_result;
    }
	MMIIDLE_CreateImageLayer(balloon_image.buff_ptr,&s_girlswing_layer_info, &s_girlswing_layer_handle,balloon_image.id, win_id);
	candy_image.id=IMAGE_WALLPAPER_GIRL_SWING_BALLOON_G;
	get_wid_h(&(candy_image.ima_size),candy_image.id, win_id);
	candy_image.ima_location.x=BALLON_PNG_G_X;
	candy_image.ima_location.y=BALLON_PNG_G_Y;
	candy_image.buff_ptr= MMILIVEWALLPAPER_ALLOC(candy_image.ima_size.w*candy_image.ima_size.h*sizeof(COLOR32));
    if(PNULL == candy_image.buff_ptr)
    {
        return b_result;
    }
	MMIIDLE_CreateImageLayer(candy_image.buff_ptr,&s_girlswing_layer_info, &s_girlswing_layer_handle,candy_image.id, win_id);

    b_result = TRUE;
    return b_result;
}

/*****************************************************************************/
//  Description : 线性光幕准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GirlswingDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    BOOLEAN b_result = FALSE;
	GUI_POINT_T display_point = {0,0};

    if (PNULL != dev_info_ptr)
    {
        b_result = GUIRES_DisplayImg(&display_point, 
                                            PNULL, 
                                            PNULL, 
                                            win_id, 
                                            IMAGE_WALLPAPER_GIRL_SWING_BG, 
                                            dev_info_ptr);
    }
    return b_result;
}

/*****************************************************************************/
//  Description : 线性光幕算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void GirlswingCalculate(LW_CAL_PARAM *lw_cal_param)
{
       int i=0;
	static uint32 cost_time = 0;
	cost_time = SCI_GetTickCount();

    if(PNULL == lw_cal_param || PNULL == lw_cal_param->dest_buf_info.buf_ptr || PNULL == lw_cal_param->src_buf_info.buf_ptr)
    {
        return;
    }
	SCI_MEMCPY(lw_cal_param->dest_buf_info.buf_ptr,lw_cal_param->src_buf_info.buf_ptr,sizeof(uint16)*lw_cal_param->src_buf_info.w*lw_cal_param->src_buf_info.h);
	  

	//moving period
	for(i=0;i<4;i++)
	{	offset_w.x=(okashi_image[i].ima_location.x+okashi_image[i].ima_size.w/2);
		offset_w.y=okashi_image[i].ima_location.y;
		
		offset_girl.x=okashi_image[i].ima_size.w/2;
		offset_girl.y=4;
		Okashi_Swing(lw_cal_param->dest_buf_info.buf_ptr, okashi_image[i].buff_ptr, Rota_buff, sWindow, okashi_image[i].ima_size,offset_w,offset_girl, m_okashi_frame[i], m_okashi_total_frame);
	 }
	if(magnify_frame>=4)
	{
		i=7-magnify_frame;
	}
	else
		{
			i=magnify_frame;
		}
    if(Is_magjnify && (PNULL != girl_image.buff_ptr) && (PNULL != clean_girl_buf))
    {	
        SCI_MEMCPY(girl_image.buff_ptr,clean_girl_buf,  sizeof(COLOR32)*(girl_image.ima_size.w)*(girl_image.ima_size.h));
        Combine_Pic(angle_up[i], angle_down[i],  Rota_buff);
        if(0==magnify_frame)
        {
        	Is_magjnify=FALSE;
        }
    }

	
	offset_w.x=girl_image.ima_location.x+ROPE_LOCATION;
	offset_w.y=girl_image.ima_location.y;
	offset_girl.x=ROPE_LOCATION;
	offset_girl.y=0;
	
	
	Risu_Move(lw_cal_param->dest_buf_info.buf_ptr,lw_cal_param->dest_buf_info.buf_ptr);
	Girl_Swing(lw_cal_param->dest_buf_info.buf_ptr,girl_image.buff_ptr,Rota_buff,sWindow,girl_image.ima_size,offset_w,offset_girl,m_girl_frame,m_girl_total_frame,1);
	Rabbit_Move(lw_cal_param->dest_buf_info.buf_ptr,lw_cal_param->dest_buf_info.buf_ptr);
	 //
	
	m_girl_frame++;
	
	
	if(m_girl_frame>4*m_girl_total_frame-2)
	{
		m_girl_frame=1;
		if(Level1[0]>100*65536)
		{
			for(i=0;i<m_girl_total_frame;i++)
			{
				Level1[i]=Level1[i]*31/32;
			}
		}
	}
	if(magnify_frame>0)
	{
	   magnify_frame--;
	}
	for(i=0;i<4;i++)
	{
		m_okashi_frame[i]++;
		if(m_okashi_frame[i]>4*m_okashi_total_frame-2)
		{
			m_okashi_frame[i]=1;
		}
	}
	cost_time = SCI_GetTickCount() - cost_time;
	//SCI_TRACE_LOW:"YOON, cost_time girl_swing = %d ms"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_GIRLSWING_712_112_2_18_2_32_4_25,(uint8*)"d", cost_time);

}
/*****************************************************************************/
//  Description : 线性光幕释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void GirlswingDestruct(void)
{	int i=0;
	
			MMILIVEWALLPAPER_FREE(girl_image.buff_ptr);//SCI_FREE
			MMILIVEWALLPAPER_FREE(up_leg_image.buff_ptr);
		    MMILIVEWALLPAPER_FREE(down_leg_image.buff_ptr);
		    MMILIVEWALLPAPER_FREE(clean_girl_buf);
	for(i=0;i<OKASHI_NUM;i++)
		{
			
			MMILIVEWALLPAPER_FREE(okashi_image[i].buff_ptr);
			
		}
	for(i=0;i<R_MOVE_NUM;i++)
		{
			
			MMILIVEWALLPAPER_FREE(m_rabit_image[i].buff_ptr);
		}
	for(i=0;i<R_STOP_NUM;i++)
		{
			
			MMILIVEWALLPAPER_FREE(s_rabit_image[i].buff_ptr);
		}
	for(i=0;i<RISU_NUM;i++)
		{
			MMILIVEWALLPAPER_FREE(risu_image[i].buff_ptr);
		}
	for(i=0;i<SUBAKO_NUM;i++)
		{
			MMILIVEWALLPAPER_FREE(subako_image[i].buff_ptr);
		}
	for(i=0;i<R_OUT_NUM;i++)
    {
        MMILIVEWALLPAPER_FREE(out_rabit_image[i].buff_ptr);
    }
	for(i=0;i<CHO_NUM;i++)
		{
			MMILIVEWALLPAPER_FREE(cho_image[i].buff_ptr);
		}
	MMILIVEWALLPAPER_FREE(Rota_buff);
	MMILIVEWALLPAPER_FREE(balloon_image.buff_ptr);
	MMILIVEWALLPAPER_FREE(candy_image.buff_ptr);
}

/*****************************************************************************/
//  Description : 线性光幕释交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN GirlswingHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{   
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
	int i=0;

	switch(msg_id)
	{
	GUI_POINT_T point;
	case MSG_TP_PRESS_DOWN:
	  
	    point.x = MMK_GET_TP_X(param);
	    point.y = MMK_GET_TP_Y(param);
	   if(get_focus(sWindow, girl_image.ima_size, point))
	   	{ 	
				if(Level1[0]<500*65536)
				{
				   for(i=0;i<m_girl_total_frame;i++)
				   	{
				   	  Level1[i]=   Level1[i]*5/4;
				  	}
				}
		    Is_magjnify=TRUE;
		   	magnify_frame=7;
	   	}
	   else if(get_animal_focus(sWindow,risu_image[2],point))
	   	{
	   	   m_risu_frame=36;
	   	}
	   else if(get_animal_focus(sWindow,subako_image[0],point))
	   	{
	   	   m_subako_frame=36;
	   	}
	   else if((get_animal_focus(sWindow, s_rabit_image[1], point))&&(rabbit_case==2))
	   	{
	   	
			rabbit_case=3;
	   	}
	        break;

	default:
	    return FALSE;
	}
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    return TRUE;
}
/*****************************************************************************/
//  Description :图像合并
//  Global resource dependence : 
//  Author:Anson.Yin
//  Note:2011.11.22
/*****************************************************************************/
LOCAL BOOLEAN Combine_Pic(int c_angle_up,int c_angle_down,FIX16_POINT_T * Point_Add_ptr)
{     	
	IMGREF_POINT_T window_mid={0};
	IMGREF_POINT_T img_mid={0};
	window_mid.x=UP_WIN_X;
	window_mid.y=UP_WIN_Y;
	img_mid.x=UP_IMAGE_X;
	img_mid.y=UP_IMAGE_Y;
   	S2d_rotation_alpha_zerosize_ex(girl_image.ima_size, up_leg_image.ima_size,  window_mid, img_mid, c_angle_up,  up_leg_image.buff_ptr, girl_image.buff_ptr, 4096, 2,Point_Add_ptr);

	window_mid.x=DOWN_WIN_X;
	window_mid.y=DOWN_WIN_Y;
	img_mid.x=DOWN_IMAGE_X;
	img_mid.y=DOWN_IMAGE_Y;
	S2d_rotation_alpha_zerosize_ex(girl_image.ima_size, down_leg_image.ima_size,  window_mid, img_mid, c_angle_down,  down_leg_image.buff_ptr, girl_image.buff_ptr, 4096,2, Point_Add_ptr);
	return TRUE;
}

LOCAL BOOLEAN get_animal_focus(IMGREF_SIZE_T win_size, IMAGE_INFO_T img,GUI_POINT_T click_p)
{   
	int x_left=0;
	int x_right=0;
	int y_up=0;
	int y_down=0;

	x_left=img.ima_location.x;
	x_right=x_left+img.ima_size.w;
	y_up=img.ima_location.y;
	y_down=y_up+img.ima_size.h;

	if((click_p.x>x_left)&&(click_p.x<x_right)&&(click_p.y>y_up)&&(click_p.y<y_down))
		{
			 return TRUE;
		}
	return FALSE;
}
BOOLEAN get_focus(IMGREF_SIZE_T win_size,IMGREF_SIZE_T img,GUI_POINT_T click_p)
{    
	int cosData=0,sinData=0;
	IMGREF_POINT_T window_mid;
      IMGREF_POINT_T img_mid;
	IMGREF_POINT_T center_point;
	IMGREF_POINT_T edge_p[4];
	
	window_mid.x=win_size.w/2;
	window_mid.y=win_size.h/6;

	img_mid.x=80;
	img_mid.y=10;
	cosData = s2d_cos_new(cur_angle);
	sinData = s2d_sin_new(cur_angle);
    
	center_point.x=window_mid.x-(((-img_mid.y+img.h*3/4)*sinData)>>16);
	center_point.y=window_mid.y+(((-img_mid.y+img.h*3/4)*cosData)>>16);

	edge_p[0].x =center_point.x-img.w/2;
	edge_p[0].y=center_point.y-img.h/3;

	edge_p[1].x=center_point.x+img.w/3;
	edge_p[1].y=center_point.y-img.h/3;

	edge_p[2].x=center_point.x-img.w/2;
	edge_p[2].y=center_point.y+img.h/3;

	edge_p[3].x=center_point.x+img.w/3;
	edge_p[3].y=center_point.y+img.h/3;

	if((click_p.x>edge_p[0].x)&&(click_p.x<edge_p[1].x)&&(click_p.y>edge_p[0].y)&&(click_p.y<edge_p[2].y))
		{return TRUE;}
	else
		return FALSE;
}
LOCAL void Girl_Swing(COLOR16 * dst,COLOR32 * front,FIX16_POINT_T * rota_buffer,IMGREF_SIZE_T win_size,IMGREF_SIZE_T img_size,IMGREF_POINT_T offset_window,IMGREF_POINT_T offset_img,int cur_frame,int total_frame,int strong_level)
{    
     
	
	if(cur_frame<=total_frame)
	{
		cur_angle=Level1[cur_frame-1]>>16;
		//Is_speed_up=TRUE;
	}
	else if ((cur_frame>total_frame)&&(cur_frame<total_frame*2))
	{
		cur_angle=-(Level1[total_frame*2-1-cur_frame]>>16);
		//Is_speed_up=FALSE;
	}
	else if ((cur_frame>=total_frame*2)&&(cur_frame<=total_frame*3-1))
	{
		cur_angle=-(Level1[cur_frame-total_frame*2]>>16);
		//Is_speed_up=FALSE;
	}
	else if ((cur_frame>=total_frame*3)&&(cur_frame<=total_frame*4-2))
	{
		cur_angle=(Level1[total_frame*4-2-cur_frame]>>16);
		//Is_speed_up=TRUE;
	}

	
	S2d_rotation_slim(	win_size,
						img_size,
						offset_window,
						offset_img,
						cur_angle,
						front,
						dst,
						4096,
						rota_buffer);
}
LOCAL void Okashi_Swing(COLOR16 * dst,COLOR32 * front,FIX16_POINT_T * rota_buffer,IMGREF_SIZE_T win_size,IMGREF_SIZE_T img_size,IMGREF_POINT_T offset_window,IMGREF_POINT_T offset_img,int cur_frame,int total_frame)
{    
     
	
	int angle=0;
	
	   
	if(cur_frame<=total_frame)
	{
	angle=Level2[cur_frame-1];
	}
	else if ((cur_frame>total_frame)&&(cur_frame<total_frame*2))
	{
	   angle=-Level2[total_frame*2-1-cur_frame];
	}
	else if ((cur_frame>=total_frame*2)&&(cur_frame<=total_frame*3-1))
	{
	   angle=-Level2[cur_frame-total_frame*2];
	}
	else if ((cur_frame>=total_frame*3)&&(cur_frame<=total_frame*4-2))
	{
	   angle=Level2[total_frame*4-2-cur_frame];
	}

		
	S2d_rotation_slim(	win_size,
						img_size,
						offset_window,
						offset_img,
						angle,
						front,
						dst,
						4096,
						rota_buffer);
}\
LOCAL void get_wid_h( IMGREF_SIZE_T *  img_size,MMI_IMAGE_ID_T image_id,MMI_HANDLE_T win_id)
{
	UILAYER_CREATE_T create_info = {0}; 
	create_info.lcd_id = GUI_MAIN_LCD_ID;
	create_info.owner_handle = (MMI_HANDLE_T)win_id;
	create_info.offset_x = 0;
	create_info.offset_y = 0;
	GUIRES_GetImgWidthHeight((uint16*)&create_info.width, (uint16*)&create_info.height, image_id, win_id);
	(* img_size).w=create_info.width;
	(* img_size).h=create_info.height;
	
}
LOCAL void Risu_Move(COLOR16 * bg_buf,COLOR16 * out_buf)
{
	COLOR32 * color_buffer=PNULL;
	FIX16_POINT_T c_offset;
	int random=0;
	if(0==m_risu_frame)
	{
		random=rand()%20;
		if(1==random)
		{
		  shake_risu_frame=3;
		}
	}
	if(0==m_risu_frame)
		{
			color_buffer=risu_image[shake_risu_frame].buff_ptr;
			c_offset.x=risu_image[shake_risu_frame].ima_location.x;
			c_offset.y=risu_image[shake_risu_frame].ima_location.y;
			if(shake_risu_frame>2)
				{shake_risu_frame--;}
		}
	else if((m_risu_frame>0)&&(m_risu_frame<=6))
		{
			color_buffer=risu_image[m_risu_frame+1].buff_ptr;
			c_offset.x=risu_image[m_risu_frame+1].ima_location.x;
			c_offset.y=risu_image[m_risu_frame+1].ima_location.y;
				m_risu_frame--;
		}
	else if((m_risu_frame>=31)&&(m_risu_frame<=36))
		{
			color_buffer=risu_image[38-m_risu_frame].buff_ptr;
			c_offset.x=risu_image[38-m_risu_frame].ima_location.x;
			c_offset.y=risu_image[38-m_risu_frame].ima_location.y;
				m_risu_frame--;
		}
	else
		{	color_buffer=risu_image[7].buff_ptr;	
			c_offset.x=risu_image[7].ima_location.x;
			c_offset.y=risu_image[7].ima_location.y;
				m_risu_frame--;
		}
	c_offset.x_dec=0;
	c_offset.y_dec=0;
	S2D_Blending(sWindow, bg_buf, risu_image[2].ima_size, c_offset,  color_buffer,  out_buf);

	if((m_subako_frame>=0)&&(m_subako_frame<=3))
		{
			color_buffer=subako_image[m_subako_frame].buff_ptr;
			c_offset.x=subako_image[m_subako_frame].ima_location.x;
			c_offset.y=subako_image[m_subako_frame].ima_location.y;
				
		}
	else if((m_subako_frame>=33)&&(m_subako_frame<=36))
		{
			color_buffer=subako_image[36-m_subako_frame].buff_ptr;
			c_offset.x=subako_image[36-m_subako_frame].ima_location.x;
			c_offset.y=subako_image[36-m_subako_frame].ima_location.y;
				
		}
	else
		{	color_buffer=subako_image[3].buff_ptr;	
			c_offset.x=subako_image[3].ima_location.x;
			c_offset.y=subako_image[3].ima_location.y;
				
		}
	if(m_subako_frame>0)
		{
			m_subako_frame--;
		}
	S2D_Blending(sWindow, bg_buf, subako_image[2].ima_size, c_offset,  color_buffer,  out_buf);
	random=rand()%10;
		if(1==random)
		{
		  shake_cho_frame=1;
		}
	
	color_buffer=cho_image[shake_cho_frame].buff_ptr;
	c_offset.x_dec=0;
	c_offset.y_dec=0;
	c_offset.x=cho_image[shake_cho_frame].ima_location.x;
	c_offset.y=cho_image[shake_cho_frame].ima_location.y;
	S2D_Blending(sWindow, bg_buf,  cho_image[shake_cho_frame].ima_size,  c_offset,  color_buffer,  out_buf);
	if(shake_cho_frame>0)
		{shake_cho_frame--;}
	

}
LOCAL void Rabbit_Move(COLOR16 * bg_buf,COLOR16 * out_buf)
{	//rabbit move
	int c_in_frame=0;
	int c_out_frame=0;
	int random=0;
	FIX16_POINT_T c_offset={0};
	IMGREF_SIZE_T c_size={0};
	COLOR32 * color_buffer;
	color_buffer=PNULL;

	c_in_frame=in_rabbit_frame/2;
	c_out_frame=out_rabbit_frame/2;
	random=rand()%80+1;

	if((rabbit_case==0)&&(random==1))
	{
		rabbit_case=1;
	}
	
	random=rand()%50+1;
	if((rabbit_case==2)&&(random==2))
	{
		shake_rabbit_frame=1;
	}
	
	switch(rabbit_case)
	{
	
		case 0:
			break;
		case 1:
			
			if(c_in_frame<4)
			{
				color_buffer=m_rabit_image[c_in_frame].buff_ptr;
				c_size.w=m_rabit_image[c_in_frame].ima_size.w;
				c_size.h=m_rabit_image[c_in_frame].ima_size.h;
			}
			else if((c_in_frame<=6)&&(c_in_frame>=4))
			{
				color_buffer=m_rabit_image[c_in_frame-3].buff_ptr;
				c_size.w=m_rabit_image[c_in_frame-3].ima_size.w;
				c_size.h=m_rabit_image[c_in_frame-3].ima_size.h;
			}
			else if(c_in_frame==7)
			{
				color_buffer=s_rabit_image[1].buff_ptr;
				c_size.w=s_rabit_image[1].ima_size.w;
				c_size.h=s_rabit_image[1].ima_size.h;
				
				
			}
			c_offset.x=rabit_in_offset[c_in_frame].x-c_size.w;
			c_offset.y=rabit_in_offset[c_in_frame].y;
			c_offset.x_dec=0;
			c_offset.y_dec=0;
			S2D_Blending(sWindow, bg_buf,  c_size,  c_offset,  color_buffer,  out_buf);

			in_rabbit_frame++;
			if(in_rabbit_frame==16)
			{
				in_rabbit_frame=0;
				shake_rabbit_frame=0;
				rabbit_case=2;
			}
			break;
		case 2:
			color_buffer=s_rabit_image[1-shake_rabbit_frame].buff_ptr;
			c_size.w=s_rabit_image[1-shake_rabbit_frame].ima_size.w;
			c_size.h=s_rabit_image[1-shake_rabbit_frame].ima_size.h;
			c_offset.x=rabit_in_offset[7].x-c_size.w;
			c_offset.y=rabit_in_offset[7].y;
			c_offset.x_dec=0;
			c_offset.y_dec=0;
			S2D_Blending(sWindow, bg_buf,  c_size,  c_offset,  color_buffer,  out_buf);
			if(shake_rabbit_frame>0)
				{shake_rabbit_frame--;}
			break;
		case 3:
			
			if(c_out_frame<4)
			{
				color_buffer=out_rabit_image[c_out_frame].buff_ptr;
				c_size.w=out_rabit_image[c_out_frame].ima_size.w;
				c_size.h=out_rabit_image[c_out_frame].ima_size.h;
				c_offset.x=rabit_out_offset[c_out_frame].x-c_size.w;
				c_offset.y=rabit_out_offset[c_out_frame].y;
			}
			else if(c_out_frame==4)
			{
				color_buffer=out_rabit_image[1].buff_ptr;
				c_size.w=out_rabit_image[1].ima_size.w;
				c_size.h=out_rabit_image[1].ima_size.h;
				c_offset.x=rabit_out_offset[4].x-c_size.w;
				c_offset.y=rabit_out_offset[4].y;
			}
			out_rabbit_frame++;
			if(out_rabbit_frame==10)
			{
				out_rabbit_frame=0;
				rabbit_case=0;
			}
			c_offset.x_dec=0;
			c_offset.y_dec=0;
			S2D_Blending(sWindow, bg_buf,  c_size,  c_offset,  color_buffer,  out_buf);
			break;
		default:
			break;
				
	}
	color_buffer=balloon_image.buff_ptr;
	c_size.h=balloon_image.ima_size.h;
	c_size.w=balloon_image.ima_size.w;
	c_offset.x_dec=0;
	c_offset.y_dec=0;
	c_offset.x=balloon_image.ima_location.x;
	c_offset.y=balloon_image.ima_location.y;
	S2D_Blending(sWindow, bg_buf,  c_size,  c_offset,  color_buffer,  out_buf);
	color_buffer=candy_image.buff_ptr;
	c_size.h=candy_image.ima_size.h;
	c_size.w=candy_image.ima_size.w;
	c_offset.x_dec=0;
	c_offset.y_dec=0;
	c_offset.x=candy_image.ima_location.x;
	c_offset.y=candy_image.ima_location.y;
	S2D_Blending(sWindow, bg_buf,  c_size,  c_offset,  color_buffer,  out_buf);
	 	
}
LOCAL void MMIIDLE_CreateImageLayer(uint32 * buf_ptr,UILAYER_INFO_T *layer_info_ptr, GUI_LCD_DEV_INFO *layer_handle_ptr, MMI_IMAGE_ID_T image_id, MMI_HANDLE_T win_id)
{
       UILAYER_CREATE_T create_info = {0}; 
	GUI_POINT_T      display_point   = {0,0};

        uint8 * wallpaper_layer_buf_ptr = PNULL;

	if((PNULL == buf_ptr) || (PNULL == layer_info_ptr) || (PNULL == layer_handle_ptr) )
        {
            return;
        }
	/*先创建一个层*/
	layer_handle_ptr->lcd_id = 0;
	layer_handle_ptr->block_id = UILAYER_NULL_HANDLE;

	create_info.lcd_id = GUI_MAIN_LCD_ID;
	create_info.owner_handle = (MMI_HANDLE_T)win_id;
	create_info.offset_x = 0;
	create_info.offset_y = 0;
	GUIRES_GetImgWidthHeight((uint16*)&create_info.width, (uint16*)&create_info.height, image_id, win_id);
	
	
	create_info.is_bg_layer = FALSE;
	create_info.is_static_layer = TRUE; 
    create_info.func_ptr = MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(); 
	UILAYER_CreateLayer(
							&create_info,
							layer_handle_ptr
							);	
	// 设置color key
	UILAYER_SetLayerColorKey(layer_handle_ptr, TRUE, UILAYER_TRANSPARENT_COLOR);

	// 使用color清除层
	UILAYER_Clear(layer_handle_ptr);

	//把图片画到层上
	GUIRES_DisplayImg(&display_point,
						PNULL,
						PNULL,
						win_id,
						image_id,
						(const GUI_LCD_DEV_INFO*)layer_handle_ptr);
	//获取图片的信息
	UILAYER_GetLayerInfo(layer_handle_ptr, layer_info_ptr);

        wallpaper_layer_buf_ptr = UILAYER_GetLayerBufferPtr(layer_handle_ptr);   

	//
	if(PNULL == wallpaper_layer_buf_ptr )
        {
            return;
        }
	SCI_MEMCPY(buf_ptr, wallpaper_layer_buf_ptr, create_info.width*create_info.height*4);

	
	//注销层
	UILAYER_ReleaseLayer(layer_handle_ptr);

}
/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 GirlswingGetType(void)
{
    return MMI_LIVE_WALLPAPER_TYPE_GIRLSWING;
}
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void GirlswingGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr)
{
    if(thumbnail_info_ptr)
    {
        thumbnail_info_ptr->is_image_file = FALSE;
        thumbnail_info_ptr->image_id = IMAGE_WALLPAPER_GIRLSWING_THUMBNAIL;

        thumbnail_info_ptr->is_not_string = FALSE;
        thumbnail_info_ptr->text_id = TXT_SET_WALLPAPER_GIRLSWING;
        thumbnail_info_ptr->instance_mem_pool_size = BLOCK_MEM_POOL_SIZE_GIRLSWING;
    }
}
#endif// LIVE_WALLPAPER_SUPPORT_LINEAR


#endif //_MMI_LIVEWALLPAPER_LINEAR_C_


/*Edit by script, ignore 1 case. Thu Apr 26 19:00:58 2012*/ //IGNORE9527
