/*****************************************************************************
** File Name:      mmi_live_wallpaper.c                                    *
** Author:         kyle.jin                                                *
** Date:           21/07/2011                                                *
** Copyright:      2011 Spreadtrum, Incorporated. All Rights Reserved.       *
** Description:    This file is used to describe runninglight wallpaer       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
**  07/2011   kyle.jin            Create                                  *
** 08/2011       Arvin.wu          Modify                                  *
******************************************************************************/

#ifndef _MMI_LIVEWALLPAPER_SWEETTREE_C_
#define _MMI_LIVEWALLPAPER_SWEETTREE_C_

#include "mmi_app_livewallpaper_trc.h"
#ifdef LIVE_WALLPAPER_SUPPORT_SWEETTREE

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmilivewallpaper_export.h"

#include "mmi_text.h"
#include "mmi_image.h"
#include "mmi_livewallpaper_text.h"
#include "mmi_livewallpaper_image.h"
#include "graphics_3d_rotate.h"
/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define   GIRL_LEG_TOTAL_FRAM_NUM 8
#define GIRL_SWING_ANGLE_NUM 20
#define   GIRL_SWING_TOTAL_FRAM_NUM (4*GIRL_SWING_ANGLE_NUM-2)
#define GIRL_SWING_STRONG_LEVEL_NUM 3
#define GIRL_SWING_TPDOWN_VALID_WINDOW  80
#define GIRL_SWING_LINE_OFFSET  106//58

#define BIRD_FLY_ANIM_FRAME_NUM 20
#define BIRD_TOUCH_ANIM_FRAME_NUM 14
#define BIRD_GO_NEST_FRAME_NUM 3

#define SQUIRREL_RUN_ANIM_FRAME_NUM 8
#define SQUIRREL_TOUCH_ANIM_FRAME_NUM 12

#define BUTTERFLY_FLY_ANIM_FRAME_NUM 20
#define SWEETTREE_MUSHROOM_NUM 4
#define SWEETTREE_CLOUD_NUM 3

#define RABBIT_RUNAWAY_FRAME_NUM 8
#define RABBIT_STAND_PIC_WIDTH 30
#define RABBIT_STAND_PIC_HEIGHT 63
#define RABBIT_RUN_PIC_WIDTH 63
#define RABBIT_RUN_PIC_HEIGHT 33
#define SWEETTREE_NEST_PIC_WIDTH 42
#define SWEETTREE_NEST_PIC_HEIGHT 40

#define SWEETTREE_CAVE_PIC_WIDTH 25
#define SWEETTREE_CAVE_PIC_HEIGHT 38
#define SWEETTREE_GIRL_PIC_WIDTH 192//110
#define SWEETTREE_GIRL_PIC_HEIGHT 423//242
#define SWEETTREE_BUTTERFLY_PIC_WIDTH 20
#define SWEETTREE_BUTTERFLY_PIC_HEIGHT 22

#define ROTATION_BUFFER_SIZE (8192)    //2048*4
#define BLOCK_MEM_POOL_SIZE_SWEETTREE (500 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

typedef enum
{
    MMI_SWEETTREE_RABBIT_IDLE,
    MMI_SWEETTREE_RABBIT_RUNING_AWAY,
    MMI_SWEETTREE_RABBIT_RUNAWAY,
    MMI_SWEETTREE_RABBIT_RUNING_BACK,
    MMI_SWEETTREE_RABBIT_TOUCHED,
    MMI_SWEETTREE_RABBIT_MAX
}MMI_SWEETTREE_RABBIT_STATE_E;


typedef enum
{
MMI_SWEETTREE_SQUIRREL_IN_CAVE,
MMI_SWEETTREE_SQUIRREL_SWAYING,
MMI_SWEETTREE_SQUIRREL_SWAYED,
MMI_SWEETTREE_SQUIRREL_RUNING_AWAY,
MMI_SWEETTREE_SQUIRREL_RUNAWAY,
MMI_SWEETTREE_SQUIRREL_RUNING_BACK,
MMI_SWEETTREE_SQUIRREL_TOUCHED,
MMI_SWEETTREE_SQUIRREL_STATE_MAX
}MMI_SWEETTREE_SQUIRREL_STATE_E;
typedef enum
{
MMI_SWEETTREE_BIRD_IN_CAVE,
MMI_SWEETTREE_BIRD_SWAYING,
MMI_SWEETTREE_BIRD_SWAYED,
MMI_SWEETTREE_BIRD_FLYING_AWAY,
MMI_SWEETTREE_BIRD_FLYAWAY,
MMI_SWEETTREE_BIRD_FLYING_BACK,
MMI_SWEETTREE_BIRD_FLYBACK,
MMI_SWEETTREE_BIRD_TURN_RIGHT,
MMI_SWEETTREE_BIRD_GO_NEST,
MMI_SWEETTREE_BIRD_THOUCHED,
MMI_SWEETTREE_BIRD_MAX
}MMI_SWEETTREE_BIRD_STATE_E;

typedef enum
{
    MMI_SWEETTREE_BUTTERFLY_IDLE,
    MMI_SWEETTREE_BUTTERFLY_SWAYING,
    MMI_SWEETTREE_BUTTERFLY_SWAYED,
    MMI_SWEETTREE_BUTTERFLY_FLYING_AWAY,
    MMI_SWEETTREE_BUTTERFLY_FLYAWAY,
    MMI_SWEETTREE_BUTTERFLY_FLYING_BACK,
    MMI_SWEETTREE_BUTTERFLY_FLYBACK,
    MMI_SWEETTREE_BUTTERFLY_STATE_MAX
}MMI_SWEETTREE_BUTTERFLY_STATE_E;
typedef struct
{
    uint16 girl_leg_frame_index ;
    uint16 girl_swing_frame_index;
    uint16 girl_strong_reduce_cycel;
    uint16 bird_frame_index;
    uint16 squirrel_frame_index;
    uint16 butterfly_frame_index;
    uint16 rabbit_frame_index;
    
    uint16 river_pic_x_offset;
    uint16 river_pic_y_offset;
    uint16 smalltree_pic_x_offset;
    uint16 smalltree_pic_y_offset;
    
    uint16 bigtree_pic_x_offset;
    uint16 bigtree_pic_y_offset;
    uint16 grass_pic_x_offset;
    uint16 grass_pic_y_offset;
     uint16 strong_level;

    MMI_SWEETTREE_BIRD_STATE_E bird_state;
    MMI_SWEETTREE_SQUIRREL_STATE_E squirrel_state;
    MMI_SWEETTREE_BUTTERFLY_STATE_E butterfly_state;
    MMI_SWEETTREE_RABBIT_STATE_E rabbit_state;
    BOOL bling_left;
    BOOL have_relocate_position_of_element;
    BOOL should_add_stronglevel;
    BOOL should_sway_leg;
}MMI_SWEETTREE_WALLPAPER_INFO_T;

/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                 *
**--------------------------------------------------------------------------*/
LOCAL GUI_LCD_DEV_INFO s_sweettree_dancinggirl_lcd_dev = {0,0};
LOCAL GUI_LCD_DEV_INFO s_sweettree_bird_lcd_dev = {0,0};
LOCAL GUI_LCD_DEV_INFO s_bird_nest_lcd_dev = {0,0};
LOCAL GUI_LCD_DEV_INFO s_sweettree_butterfly_lcd_dev = {0,0};
LOCAL GUI_LCD_DEV_INFO s_sweettree_bigtree_lcd_dev = {0,0};
LOCAL GUI_LCD_DEV_INFO s_sweettree_squirrel_lcd_dev = {0,0};
LOCAL GUI_LCD_DEV_INFO s_sweettree_rabbit_lcd_dev = {0,0};

LOCAL COLOR16 *clean_bg_buf_ptr;
LOCAL COLOR16 *rgb565_buf_ptr;

LOCAL COLOR32 *girl1_buf_ptr;
LOCAL COLOR32 *girl2_buf_ptr;
LOCAL COLOR32 *girl3_buf_ptr;
LOCAL COLOR32 *girl4_buf_ptr;
LOCAL FIX16_POINT_T *girl_rotation_buf_ptr;

LOCAL COLOR32 *s_nest1_buf_ptr = PNULL;
LOCAL COLOR32 *s_nest2_buf_ptr = PNULL;
LOCAL COLOR32 *s_nest3_buf_ptr = PNULL;
LOCAL COLOR32 *s_nest4_buf_ptr = PNULL;
LOCAL COLOR32 *s_nest_left_buf_ptr = PNULL;

 
LOCAL COLOR32 *s_bird_fly1_buf_ptr = PNULL;
LOCAL COLOR32 *s_bird_fly2_buf_ptr = PNULL;
LOCAL COLOR32 *s_bird_fly3_buf_ptr = PNULL;
LOCAL COLOR32 *s_bird_fly4_buf_ptr = PNULL;


LOCAL COLOR32 *s_squirrel_head1_buf_ptr = PNULL;
LOCAL COLOR32 *s_squirrel_head2_buf_ptr = PNULL;
LOCAL COLOR32 *s_squirrel_head3_buf_ptr = PNULL;
LOCAL COLOR32 *s_squirrel_tail1_buf_ptr = PNULL;
LOCAL COLOR32 *s_squirrel_tail2_buf_ptr = PNULL;
LOCAL COLOR32 *s_squirrel_tail3_buf_ptr = PNULL;
LOCAL COLOR32 *s_squirrel_run1_buf_ptr = PNULL;
LOCAL COLOR32 *s_squirrel_run2_buf_ptr = PNULL;
LOCAL COLOR32 *s_squirrel_run3_buf_ptr = PNULL;

LOCAL COLOR32 *s_butterfly1_buf_ptr = PNULL;
LOCAL COLOR32 *s_butterfly2_buf_ptr = PNULL;
LOCAL COLOR32 *s_butterfly3_buf_ptr = PNULL;
LOCAL COLOR32 *s_butterfly4_buf_ptr = PNULL;

LOCAL COLOR32 *s_rabbit_stand2_buf_ptr = PNULL;
LOCAL COLOR32 *s_rabbit_stand3_buf_ptr = PNULL;
LOCAL COLOR32 *s_rabbit_stand4_buf_ptr = PNULL;
LOCAL COLOR32 *s_rabbit_stand5_buf_ptr = PNULL;

LOCAL COLOR32* s_rabbit_run2_buf_ptr = PNULL;
LOCAL COLOR32* s_rabbit_run3_buf_ptr = PNULL;

uint16 *s_girl_swing_angle_array1 = PNULL;
uint16 *s_girl_swing_angle_array2 = PNULL;
uint16 *s_girl_swing_angle_array3 = PNULL;
//uint16 *s_girl_swing_angle_array4 = PNULL;

LOCAL uint32 s_sweettree_old_tick = 0;
LOCAL uint32 s_bird_old_tick = 0;
LOCAL uint32 s_squirrel_old_tick = 0;

LOCAL uint16 s_lcd_width;
LOCAL uint16 s_lcd_height;
LOCAL uint8 s_sweettree_timer = 0;

LOCAL uint16 s_girl_pic_width=0;
LOCAL uint16 s_girl_pic_height=0;
LOCAL uint16 s_girl_dis_height = 0;

LOCAL uint16 s_river_pic_width=0;
LOCAL uint16 s_river_pic_height=0;
LOCAL uint16 s_smalltree_pic_width=0;
LOCAL uint16 s_smalltree_pic_height=0;
LOCAL uint16 s_bigtree_pic_width=0;
LOCAL uint16 s_bigtree_pic_height=0;
LOCAL uint16 s_grass_pic_width=0;
LOCAL uint16 s_grass_pic_height=0;

LOCAL uint16 s_rabbit_run_pic_width = 0;
LOCAL uint16 s_rabbit_run_pic_height = 0;
LOCAL uint16 s_nest_pic_width=0;
LOCAL uint16 s_nest_pic_height=0;

LOCAL uint16 s_squirrel_cave_pic_width=0;
LOCAL uint16 s_squirrel_cave_pic_height=0;
uint16 s_rabbit_stand_pic_width = 0;
uint16 s_rabbit_stand_pic_height = 0;
LOCAL uint16 s_butterfly_pic_width=0;
LOCAL uint16 s_butterfly_pic_height=0;

LOCAL uint16 s_butterfly_sway_anim_frame_num = 50;

LOCAL uint16 s_bird_sway_anim_frame_num = 50;


LOCAL uint16 s_squirrel_sway_anim_frame_num = 40;


LOCAL GUI_RECT_T s_girl_swing_tp_valid_rect = {0};
/*
LOCAL const uint16 s_girl_swing_level5[]={0, 35, 71, 106, 141, 175, 207, 237, 265, 290, 314, 335, 354, 364,374, 380, 385, 393, 403, 402, 409, 416, 422, 427, 431, 436, 439, 442, 445, 448};  
LOCAL const uint16 s_girl_swing_level4[]={0, 28, 57, 85, 113, 140, 165, 189, 212, 232, 251, 268, 283, 297, 309, 320, 330, 339, 346, 353, 359, 365, 369, 373, 377, 380, 383, 386, 388, 390};    
LOCAL const uint16 s_girl_swing_level3[]={0, 20,43, 70, 100, 130, 140, 161, 193, 214, 234, 252, 260, 266, 272, 277, 281, 285, 288, 291, 293, 295, 297, 298, 300, 301, 302, 303, 303,304};
LOCAL const uint16 s_girl_swing_level2[]={0, 17, 35, 53, 70, 87, 103, 118, 132, 145, 157, 167, 177, 185, 193, 200, 206, 211, 216, 221, 224, 228, 231, 233, 235, 238, 239, 241, 242, 244};
LOCAL const uint16 s_girl_swing_level1[]={0, 14, 28,42, 56,70, 82, 94, 106, 116, 125, 134, 141, 148, 154, 160, 165, 169, 173, 176 , 179, 182, 184, 186, 188, 190, 191, 193, 194, 195};
*/
LOCAL const uint16 s_girl_swing_level3[]={0, 40, 80, 116, 152, 182, 212, 236, 260,280,300, 318, 336, 350, 360, 368, 374, 378, 380, 381};  
//LOCAL const uint16 s_girl_swing_level3[]={0, 30, 60, 88, 116, 142, 168, 192, 216, 238, 260, 280, 296, 310, 320, 328, 334, 338, 340, 341};    
LOCAL const uint16 s_girl_swing_level2[]={0, 24,48, 70, 92, 114, 134, 154, 172, 190, 206, 222, 236, 250,  262, 272, 280, 286, 290, 292};
LOCAL const uint16 s_girl_swing_level1[]={0, 16, 32,47, 62,76, 86, 100, 113, 126, 138, 140, 151, 162, 172 , 180, 186,190, 193, 195};

LOCAL GUI_POINT_T s_dancing_girl_coordinate = {135,0};//the coordinate of bird nest of the bigtree image
GUI_POINT_T s_cloud_coordinate[SWEETTREE_CLOUD_NUM] ={
 {30,100},  {130,90}, {210,140}, //{330,110}, {430,190},{470,170},
};

LOCAL GUI_POINT_T s_squirrel_cave_coordinate ={240,292};// {300,286};//the coordinate of squirrel cave of the bigtree image
LOCAL GUI_POINT_T s_bird_nest_coordinate = {260,158};//{310,158};//the coordinate of bird nest of the bigtree image
LOCAL GUI_POINT_T s_rose_coordinate = {155,302};//{225,302};//the coordinate of bird nest of the bigtree image
LOCAL GUI_POINT_T s_mushroom_coordinate[SWEETTREE_MUSHROOM_NUM] = {
{92,416},{122,396},{142,412},{162,412}, 
    //   {112,416},{132,396},{162,412},{182,412},
 };

LOCAL GUI_POINT_T s_bird_flyaway_coordinate[BIRD_FLY_ANIM_FRAME_NUM] = {
{240,180},{235,185},{230,180},{225,180},{220,175},{210,170},{205,170},{200,172},{195,170},
    {190,160},{180,165},{170,160},{160,165},{140,165},
{120,170},{100,170},{80,165},{60,160},{40,160}, {20,155},  
 };
LOCAL GUI_POINT_T s_bird_flyback_coordinate[BIRD_FLY_ANIM_FRAME_NUM] = {
{390,170},{390,170},{315,170},{310,170},{305,188}, {300,170},{295,188},{290,180},{285,188},
    {280,189},    {275,188},{270,190},
{265,188}, {260,189},{255,188},{250,180},{245,188},{240,189},{235,188},{230,188},
 };
LOCAL GUI_POINT_T s_bird_go_nest_coordinate[BIRD_GO_NEST_FRAME_NUM]={
    {270,170},{260,158},{260,158},
};

LOCAL GUI_POINT_T s_squirrel_runaway_coordinate[SQUIRREL_RUN_ANIM_FRAME_NUM] = {
    {220,366},{200,370},{180,380},{150,390},{110,405},{80,410},{50,415},{4,420},
 };
LOCAL GUI_POINT_T s_squirrel_runback_coordinate[SQUIRREL_RUN_ANIM_FRAME_NUM] = {
{500,363},  {300,363}, {290,360},{278,340},{260,335},{250,336},{245,300},{240,293},
 };

LOCAL GUI_POINT_T s_butterfly_sway_coordinate = {170,325};//the coordinate of bird nest of the bigtree image
LOCAL GUI_POINT_T s_butterfly_flyaway_coordinate[BUTTERFLY_FLY_ANIM_FRAME_NUM] = {
 {180,313},{185,315}, {190,313}, {195,312},{200,313},{205,310},{210,310},{220,300},{225,306},{230,300},
 {235,305},{240,310}, {250,310}, {260,305},{270,310},{280,315},{290,305},{300,310},{305,305},{310,310},
 };
LOCAL GUI_POINT_T s_butterfly_flyback_coordinate[BUTTERFLY_FLY_ANIM_FRAME_NUM] = {
{10,325},{20,320},{30,315}, {40,320}, {50,310},{60,325}, {70,325}, {80,325},{90,325},{95,320},  
{105,320},{110,325}, {115,320}, {120,340},{130,320},{140,330},{150,320},{155,339},{160,330},{165,320},
 };
LOCAL MMI_SWEETTREE_WALLPAPER_INFO_T s_sweettree_wallpaper_param={0};


LOCAL GUI_POINT_T s_rabbit_stand_coordinate = {60,380};

LOCAL GUI_POINT_T s_rabbit_runaway_coordinate[RABBIT_RUNAWAY_FRAME_NUM] = {
 {70,385},{90,385},{105,385},{120,385},{155,385},{205,395},{245,395},{310,395},
};

LOCAL  GUI_POINT_T s_rabbit_runback_coordinate[RABBIT_RUNAWAY_FRAME_NUM] = {
 {-70,355},{-90,355},{-105,355},{-120,355},{-25,350},{-10,355},{-5,350},{5,350}, 
};

LOCAL const MMI_IMAGE_ID_T s_cloud_anim[SWEETTREE_CLOUD_NUM] ={
IMAGE_WALLPAPER_SWEETTREE_CLOUD03,IMAGE_WALLPAPER_SWEETTREE_CLOUD04,
    IMAGE_WALLPAPER_SWEETTREE_CLOUD05,
};


/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : 甜蜜树刷新
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void SweetTreeUpdate(COLOR16 *target_buf_ptr,uint16 scr_width, uint16 scr_height);

/*****************************************************************************/
//  Description : 甜蜜树互动判断
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SweetTreeHandleTpPressDown(GUI_POINT_T tp_point);
/*****************************************************************************/
//  Description : 小蜜蜂初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SweetTreeInit(void);
/*****************************************************************************/
//  Description : change the girl swing strong level
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_ChangeGirlSwayStrongeLevel(BOOL add);
/*****************************************************************************/
//  Description : re_calculate the coordinate of birdnest an so on because of the bigtree
//relative posiontion of the grass
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_CalculateOffsetofBirstNestAndSoOn();
/*****************************************************************************/
//  Description :取得 甜蜜树bigtree buffer
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIDLE_GetBigTreeBGImageofSweetTree(MMI_HANDLE_T win_id);
/*****************************************************************************/
//  Description : get layer infor of bird 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIDLE_GetRabbitRunImageofSweetTree(MMI_HANDLE_T win_id);
/*****************************************************************************/
//  Description : get layer infor of bird nest
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIDLE_GetBirdAndNestImageSweetTree(MMI_HANDLE_T win_id);

/*****************************************************************************/
//  Description : Display dancing girl of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIDLE_GetDancingGirlImageofSweetTree(MMI_HANDLE_T win_id);
/*****************************************************************************/
//  Description : Display the Squirrel cave of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIDLE_GetRabbitIMGofSweetTree(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Description : Display the Squirrel cave of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIDLE_GetSquirrelIMGofSweetTree(MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Description : get layer infor of butterfly 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIDLE_GetButterflyImageofSweetTree(MMI_HANDLE_T win_id);

/*****************************************************************************/
//  Description : 甜蜜树构造函数
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SweetTreeConstruct(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 甜蜜树准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SweetTreeDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr);
/*****************************************************************************/
//  Description : Display the butterfly of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_DisplayButterflyofSweetTree();
 /*****************************************************************************/
//  Description : Display the standing rabbit of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void DisplayStandRabbitOfSweetTree();
/*****************************************************************************/
//  Description : play the girl sway leg animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlayGirlSwayLegAnim();
/*****************************************************************************/
//  Description : play butterfly sway animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlayButterflySwayAnimofSweetTree();
/*****************************************************************************/
//  Description : play butterfly flyaway animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void  MMIIDLE_PlayButterflyFlyAwayAnimofSweetTree();
/*****************************************************************************/
//  Description : play bird flyaback animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void  MMIIDLE_PlayBirdFlyBackAnimofSweetTree();
/*****************************************************************************/
//  Description : play bird flyaway animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void  MMIIDLE_PlayBirdGoNestAnimofSweetTree();
/*****************************************************************************/
//  Description : Display the squirrel of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_DisplayBirdofSweetTree();
/*****************************************************************************/
//  Description : Display the squirrel of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_DisplayBirdNestofSweetTree();
/*****************************************************************************/
//  Description : play butterfly sway animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlayBirdSwayAnimofSweetTree();
/*****************************************************************************/
//  Description : Display the squirrel of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_DisplaySquirrelofSweetTree();
/*****************************************************************************/
//  Description : play squirrel runback animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlaySquirrelRunBackAnimofSweetTree();
/*****************************************************************************/
//  Description : play the squirrel cave animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlaySquirrelCaveAnimofSweetTree();
/*****************************************************************************/
//  Description : play squirrel sway animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlaySquirrelSwayAnimofSweetTree();
/*****************************************************************************/
//  Description : play squirrel runaway animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlaySquirrelRunAwayAnimofSweetTree();
/*****************************************************************************/
//  Description : play butterfly flyaway animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void  MMIIDLE_PlayBirdFlyAwayAnimofSweetTree();
/*****************************************************************************/
//  Description : play the bird nest animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlayBirdTouchedAnimofSweetTree();
/*****************************************************************************/
//  Description : play rabbit running away animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlayRabbitRunningAwayAnimofSweetTree();
/*****************************************************************************/
//  Description : play rabbit running back animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlayRabbitRunningBackAnimofSweetTree();
/*****************************************************************************/
//  Description : Calc the TP msg Vavlid Rect Of GirlSwing strong level
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void CalcTPVavlidRectOfGirlSwing(int16 angle);
    /*****************************************************************************/
//  Description : 甜蜜树小女孩图片旋转
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_GirlSwing(COLOR16 * dst,COLOR32 * front,FIX16_POINT_T * rota_buffer,IMGREF_SIZE_T window_size,IMGREF_SIZE_T img_size,int frame,int total_frame,int level);
/*****************************************************************************/
//  Description : play butterfly flyback animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlayButterflyFlyBackAnimofSweetTree();
/*****************************************************************************/
//  Description : play butterfly flyback animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlayRabbitTouchedAnimofSweetTree();

/*****************************************************************************/
//  Description : 小蜜蜂算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SweetTreeCalculate(LW_CAL_PARAM *lw_cal_param);
/*****************************************************************************/
//  Description : Rest some Parameters Of SweetTree when Lose Focus
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_RestParamOfSweetTreeLoseFocus();
/*****************************************************************************/
//  Description : 甜蜜树内存释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁申请的内存
/*****************************************************************************/
LOCAL void FreeAllBufferOfSweetTree(void);
/*****************************************************************************/
//  Description : 甜蜜树释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void SweetTreeDestruct(void);
/*****************************************************************************/
//  Description : 甜蜜树交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SweetTreeHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 SweetTreeGetType(void);
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SweetTreeGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_sweettree_Instance =
{
    SweetTreeInit,
    SweetTreeConstruct,
    SweetTreeDrawBg,
    SweetTreeCalculate,
    SweetTreeDestruct,
    SweetTreeHandleEvent,
    PNULL,
    SweetTreeGetType,
    SweetTreeGetThumbNailInfo,
    PNULL,
    PNULL,
    PNULL,
    PNULL
};

/*---------------------------------------------------------------------------*/
LOCAL const float _sin_0_90_table[91] = 
{
    0,
    0.017, 0.034, 0.052, 0.069, 0.087, 0.104, 0.121, 0.139, 0.156, 0.173,
    0.190, 0.207, 0.224, 0.241, 0.258, 0.275, 0.292, 0.309, 0.325, 0.342,
    0.358, 0.374, 0.390, 0.406, 0.422, 0.438, 0.453, 0.469, 0.484, 0.499,
    0.515, 0.529, 0.544, 0.559, 0.573, 0.587, 0.601, 0.615, 0.629, 0.642,
    0.656, 0.669, 0.681, 0.694, 0.707, 0.719, 0.731, 0.743, 0.754, 0.766,
    0.777, 0.788, 0.798, 0.809, 0.819, 0.829, 0.838, 0.848, 0.857, 0.866,
    0.874, 0.882, 0.891, 0.898, 0.906, 0.913, 0.920, 0.927, 0.933, 0.939,
    0.945, 0.951, 0.956, 0.961, 0.965, 0.970, 0.974, 0.978, 0.981, 0.984,
    0.987, 0.990, 0.992, 0.994, 0.996, 0.997, 0.998, 0.999, 0.999, 1.000,
};
/******************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:   input:  angle
//          output: sin(angle) * PRCISION
/*****************************************************************************/
LOCAL float SIN_S(int16 angle)
{
    angle %= 360;

    if(angle <= 90)
    {
        return _sin_0_90_table[angle];
    }
    else if (angle <= 180)
    {
        return _sin_0_90_table[180 - angle];
    }
    else if (angle <= 270)
    {
        return -_sin_0_90_table[angle - 180];
    }
    else if (angle <= 360)
    {
        return -_sin_0_90_table[360 - angle];
    }
    else 
    {
        return 0;
    }
}
/******************************************************************************/
//  Description : 
//  Global resource dependence : none
//  Author: 
//  Note:   input:  angle
//          output: sin(angle) * PRCISION
/*****************************************************************************/
LOCAL float COS_S(int16 angle)
{
    angle %= 360;

    if (angle <= 90)
    {
        return _sin_0_90_table[90 - angle];
    }
    else if (angle <= 180)
    {
        return -_sin_0_90_table[angle - 90];
    }
    else if (angle <= 270)
    {
        return -_sin_0_90_table[270 - angle];
    }
    else if (angle <= 360)
    {
        return _sin_0_90_table[angle - 270];
    }
    else
    {
        return 0;
    }
    
}

/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                           */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 甜蜜树刷新
//  Global resource dependence : 
//  Author:Gaily.Wang
//  Note:
/*****************************************************************************/
LOCAL void SweetTreeUpdate(COLOR16 *target_buf_ptr,uint16 scr_width, uint16 scr_height)
{
}

/*****************************************************************************/
//  Description : re_calculate the coordinate of birdnest an so on because of the bigtree
//relative posiontion of the grass
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_CalculateOffsetofBirstNestAndSoOn()
{
    uint16 offset = 0;
    uint16 index = 0;
    if(s_sweettree_wallpaper_param.have_relocate_position_of_element)
    {
        return;
    }
    else
    {//in order to avoid when enter wallpaper setting select and unselect and reselect,recalculate for severel times
        s_sweettree_wallpaper_param.have_relocate_position_of_element = TRUE;
    }
    if(s_bigtree_pic_height>s_lcd_height)
    {
        offset  = s_bigtree_pic_height - s_lcd_height +s_grass_pic_height/2;
    }
    else
    {
        offset  = s_grass_pic_height/2;
     }
    s_dancing_girl_coordinate.y -=offset;
    if(s_dancing_girl_coordinate.y<0)
    {
        s_dancing_girl_coordinate.y = 0;
    }
    s_bird_nest_coordinate.y -=offset;
    s_squirrel_cave_coordinate.y -= offset;
    s_rose_coordinate.y -= offset;
    s_butterfly_sway_coordinate.y -= offset;

     s_rabbit_stand_coordinate.y -= offset;
     
    for(index = 0;index<BUTTERFLY_FLY_ANIM_FRAME_NUM;index++)
    {
       s_butterfly_flyaway_coordinate[index].y -= offset;
        s_butterfly_flyback_coordinate[index].y -= offset;
    }
    for(index = 0;index<BIRD_FLY_ANIM_FRAME_NUM;index++)
    {
        s_bird_flyaway_coordinate[index].y -= offset;
        s_bird_flyback_coordinate[index].y -= offset;
       
    }
    for(index = 0;index<BIRD_GO_NEST_FRAME_NUM;index++)
    {
        s_bird_go_nest_coordinate[index].y -= offset;
    }
    for(index = 0;index<SWEETTREE_MUSHROOM_NUM;index++)
    {
        s_mushroom_coordinate[index].y -= offset;
    }

    for(index = 0;index<SQUIRREL_RUN_ANIM_FRAME_NUM;index++)
    {
        s_squirrel_runaway_coordinate[index].y -= offset;
        s_squirrel_runback_coordinate[index].y -= offset;
    }

//     for(index = 0;index<RABBIT_RUNAWAY_FRAME_NUM;index++)
//    {
 //       s_rabbit_runaway_coordinate[index].y -= offset;
 //       s_rabbit_runback_coordinate[index].y -= offset;
 //   }
}

/*****************************************************************************/
//  Description : 甜蜜树初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SweetTreeInit(void)
{
    BOOLEAN b_result = TRUE;
    GUI_RECT_T lcd_rect = {0};
    
    lcd_rect = MMITHEME_GetFullScreenRect();
    
    s_lcd_width= (lcd_rect.right-lcd_rect.left+1);
    s_lcd_height = (lcd_rect.bottom-lcd_rect.top+1);
    
    MMK_SetWinAngle(MMK_GetFocusWinId(), LCD_ANGLE_0, FALSE ); 
    
    s_girl_swing_angle_array1 = (uint16*)&s_girl_swing_level1[0];
    s_girl_swing_angle_array2 = (uint16*)&s_girl_swing_level2[0];
    s_girl_swing_angle_array3 = (uint16*)&s_girl_swing_level3[0];
   // s_girl_swing_angle_array4 = &s_girl_swing_level4[0];
    
    s_sweettree_wallpaper_param.girl_leg_frame_index = 1;
    s_sweettree_wallpaper_param.girl_swing_frame_index = 1;
    s_sweettree_wallpaper_param.bird_frame_index = 0;
    s_sweettree_wallpaper_param.squirrel_frame_index = 0;
    s_sweettree_wallpaper_param.butterfly_frame_index = 0;
    s_sweettree_wallpaper_param.rabbit_frame_index = 0;
    
    s_sweettree_wallpaper_param.strong_level = 1;
    s_sweettree_wallpaper_param.should_add_stronglevel = FALSE;

    s_sweettree_wallpaper_param.squirrel_state = MMI_SWEETTREE_SQUIRREL_IN_CAVE;
    s_sweettree_wallpaper_param.bird_state = MMI_SWEETTREE_BIRD_IN_CAVE;
    s_sweettree_wallpaper_param.butterfly_state = MMI_SWEETTREE_BUTTERFLY_IDLE;
    s_sweettree_wallpaper_param.rabbit_state = MMI_SWEETTREE_RABBIT_IDLE;
        
   GUIRES_GetImgWidthHeight(&s_girl_pic_width,&s_girl_pic_height, IMAGE_WALLPAPER_SWEETTREE_GIRL01,MAIN_IDLE_WIN_ID);
    if(0 == s_girl_pic_width||0 == s_girl_pic_height)
    {
        s_girl_pic_width = SWEETTREE_GIRL_PIC_WIDTH;
        s_girl_pic_height = SWEETTREE_GIRL_PIC_HEIGHT;
    }
    if(s_girl_pic_height>240)
    {
        s_girl_dis_height = (2*s_girl_pic_height)/3;
    }
    else
    {
        s_girl_dis_height = s_girl_pic_height;
    }
    GUIRES_GetImgWidthHeight(&s_rabbit_run_pic_width,&s_rabbit_run_pic_height, IMAGE_SWEETTREE_RABBIT_RUN02,MAIN_IDLE_WIN_ID);
    if(0 == s_rabbit_run_pic_width||0 == s_rabbit_run_pic_height)
    {
        s_rabbit_run_pic_width= RABBIT_RUN_PIC_WIDTH;
        s_rabbit_run_pic_height= RABBIT_RUN_PIC_HEIGHT;
    }
    
   GUIRES_GetImgWidthHeight(&s_nest_pic_width,&s_nest_pic_height, IMAGE_WALLPAPER_SWEETTREE_BIRDNEST01,MAIN_IDLE_WIN_ID);
    if(0 == s_nest_pic_width||0 == s_nest_pic_height)
    {
        s_nest_pic_width = SWEETTREE_NEST_PIC_WIDTH;
        s_nest_pic_height= SWEETTREE_NEST_PIC_HEIGHT;
    }
    
    GUIRES_GetImgWidthHeight(&s_rabbit_stand_pic_width,&s_rabbit_stand_pic_height, IMAGE_SWEETTREE_RABBIT_STAND02,MAIN_IDLE_WIN_ID);
    if(0 == s_rabbit_stand_pic_width||0 == s_rabbit_stand_pic_height)
    {
        s_rabbit_stand_pic_width = RABBIT_STAND_PIC_WIDTH;
        s_rabbit_stand_pic_height = RABBIT_STAND_PIC_HEIGHT;
    }

    GUIRES_GetImgWidthHeight(&s_squirrel_cave_pic_width,&s_squirrel_cave_pic_height, IMAGE_SWEETTREE_SQUIRREL_TAIL01,MAIN_IDLE_WIN_ID);
    if(0 == s_squirrel_cave_pic_width||0 == s_squirrel_cave_pic_height)
    {
        s_squirrel_cave_pic_width = SWEETTREE_CAVE_PIC_WIDTH;
        s_squirrel_cave_pic_height = SWEETTREE_CAVE_PIC_HEIGHT;
    }
    
    GUIRES_GetImgWidthHeight(&s_butterfly_pic_width,&s_butterfly_pic_height, IMAGE_WALLPAPER_SWEETTREE_BUTTERFLY01,MAIN_IDLE_WIN_ID);
    if(0 == s_butterfly_pic_width||0 == s_butterfly_pic_height)
    {
        s_butterfly_pic_width = SWEETTREE_BUTTERFLY_PIC_WIDTH;
        s_butterfly_pic_height= SWEETTREE_BUTTERFLY_PIC_HEIGHT;
    }

    
    GUIRES_GetImgWidthHeight(&s_river_pic_width,&s_river_pic_height, IMAGE_WALLPAPER_SWEETTREE_RIVER,MAIN_IDLE_WIN_ID);
    s_sweettree_wallpaper_param.river_pic_x_offset = s_river_pic_width - s_lcd_width;
    if(s_lcd_height > s_river_pic_height)
    {
        s_sweettree_wallpaper_param.river_pic_y_offset = s_lcd_height- s_river_pic_height;
    }
    else
     {
        s_sweettree_wallpaper_param.river_pic_y_offset = 0;
     }

    GUIRES_GetImgWidthHeight(&s_grass_pic_width,&s_grass_pic_height, IMAGE_WALLPAPER_SWEETTREE_GRASS,MAIN_IDLE_WIN_ID);
    s_sweettree_wallpaper_param.grass_pic_x_offset = s_grass_pic_width - s_lcd_width;
    #if 1//(s_lcd_height > s_grass_pic_height)
    {
        s_sweettree_wallpaper_param.grass_pic_y_offset = s_lcd_height - s_grass_pic_height+s_rabbit_stand_pic_width/2;
    }
    #else
    {
        s_sweettree_wallpaper_param.grass_pic_y_offset = 0;
    }
    #endif
    
    GUIRES_GetImgWidthHeight(&s_smalltree_pic_width,&s_smalltree_pic_height, IMAGE_WALLPAPER_SWEETTREE_SMALLTREE,MAIN_IDLE_WIN_ID);
    s_sweettree_wallpaper_param.smalltree_pic_x_offset = s_lcd_width - s_smalltree_pic_width;
    if(s_lcd_height > s_smalltree_pic_height)
    {
        s_sweettree_wallpaper_param.smalltree_pic_y_offset = s_lcd_height- s_smalltree_pic_height-s_grass_pic_height/2;
    }
    else
    {
        s_sweettree_wallpaper_param.smalltree_pic_y_offset = 0;
    }
    
    GUIRES_GetImgWidthHeight(&s_bigtree_pic_width,&s_bigtree_pic_height, IMAGE_WALLPAPER_SWEETTREE_BIGTREE,MAIN_IDLE_WIN_ID);
    s_sweettree_wallpaper_param.bigtree_pic_x_offset = s_bigtree_pic_width - s_lcd_width;

    s_sweettree_wallpaper_param.bigtree_pic_y_offset = 0;


    MMIIDLE_CalculateOffsetofBirstNestAndSoOn();

    s_sweettree_wallpaper_param.bling_left = FALSE;
    
    return b_result;
}


/*****************************************************************************/
//  Description :取得 甜蜜树bigtree buffer
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIDLE_GetBigTreeBGImageofSweetTree(MMI_HANDLE_T win_id)
{
    BOOLEAN b_result = TRUE;
    GUI_RECT_T lcd_rect = {0};
    GUI_RECT_T image_rect = {0};
    GUI_POINT_T dis_point = {0,0};
    uint8 *layer_buf_ptr = PNULL; 
    
    UILAYER_CREATE_T create_infor = {0};
    uint16 index = 0;
    
    dis_point.x = 0;
    lcd_rect = MMITHEME_GetFullScreenRect();

    create_infor.lcd_id = MAIN_LCD_ID;
    create_infor.owner_handle = win_id;
    create_infor.is_bg_layer = TRUE;
    create_infor.is_static_layer = FALSE;
    create_infor.offset_x = 0;//lcd_rect.left;
    create_infor.offset_y =  0;//lcd_rect.top;
    create_infor.width = s_lcd_width;
    create_infor.height = s_lcd_height;
    create_info.func_ptr = MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(); 
    if(UILAYER_RESULT_SUCCESS != UILAYER_CreateLayer(&create_infor, &s_sweettree_bigtree_lcd_dev))
    {
        return FALSE;
    }

    layer_buf_ptr = UILAYER_GetLayerBufferPtr(&s_sweettree_bigtree_lcd_dev);
    if(PNULL == layer_buf_ptr)
    {
        UILAYER_ReleaseLayer(&s_sweettree_bigtree_lcd_dev);
        return FALSE;
    }

    // 使用color清除层
	UILAYER_Clear(&s_sweettree_bigtree_lcd_dev);
    //display background on main block id
         
        GUIRES_DisplayImg(&dis_point,PNULL,
            PNULL,win_id, IMAGE_WALLPAPER_SWEETTREE_BG,
                &s_sweettree_bigtree_lcd_dev
            );
       //display cloud
       for(index = 0;index<SWEETTREE_CLOUD_NUM;index++)
    {
        dis_point.x = s_cloud_coordinate[index].x;
        dis_point.y = s_cloud_coordinate[index].y;
        
        GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,
            s_cloud_anim[index],//IMAGE_WALLPAPER_SWEETTREE_CLOUD01, 
            &s_sweettree_bigtree_lcd_dev
            );
    }
                        //display back river
        dis_point.x = 0;                
        dis_point.y = s_sweettree_wallpaper_param.river_pic_y_offset;
            
		GUIRES_DisplayImg(&dis_point,PNULL,
		PNULL,win_id,IMAGE_WALLPAPER_SWEETTREE_RIVER,
		&s_sweettree_bigtree_lcd_dev
		);
        //display smalltree
		dis_point.y = s_sweettree_wallpaper_param.smalltree_pic_y_offset;
        
        GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,
		IMAGE_WALLPAPER_SWEETTREE_SMALLTREE,
		&s_sweettree_bigtree_lcd_dev
		);
        
		//display bigtree
		dis_point.y = s_sweettree_wallpaper_param.bigtree_pic_y_offset;
        
            image_rect.left = 0;
           image_rect.top = s_grass_pic_height/2;
            image_rect.right = lcd_rect.right;
           image_rect.bottom = s_bigtree_pic_height;
            
            GUIRES_DisplayImg(&dis_point,PNULL,&image_rect,win_id,
		IMAGE_WALLPAPER_SWEETTREE_BIGTREE,
		&s_sweettree_bigtree_lcd_dev
		);
            //display big nest
//    dis_point.x = s_bird_nest_coordinate.x ;
//    dis_point.y = s_bird_nest_coordinate.y ;

 //       GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,
//        IMAGE_WALLPAPER_SWEETTREE_BIRDNEST01,&s_sweettree_bigtree_lcd_dev);

  //display rose
         dis_point.x = s_rose_coordinate.x;
        dis_point.y = s_rose_coordinate.y;
        GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,
		IMAGE_WALLPAPER_SWEETTREE_ROSE,
		&s_sweettree_bigtree_lcd_dev
		);
         //display mushroom
    for(index = 0;index<SWEETTREE_MUSHROOM_NUM;index++)
    {
        dis_point.x = s_mushroom_coordinate[index].x ;
            dis_point.y = s_mushroom_coordinate[index].y;
        GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,
            (IMAGE_SWEETTREE_MUSHROOM_PINK+index),&s_sweettree_bigtree_lcd_dev);
    } 
        //display grass
        dis_point.x = 0;
		dis_point.y = s_sweettree_wallpaper_param.grass_pic_y_offset;

		GUIRES_DisplayImg(&dis_point,PNULL,&lcd_rect,win_id,
		IMAGE_WALLPAPER_SWEETTREE_GRASS,
		&s_sweettree_bigtree_lcd_dev
		);
    
 
   //     SCI_MEMCPY( bigtree_buf_ptr,layer_info.layer_buf_ptr,(layer_info.layer_height*layer_info.layer_width*2));
           
        SCI_MEMCPY(clean_bg_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height*2));

        UILAYER_ReleaseLayer(&s_sweettree_bigtree_lcd_dev);

    b_result = TRUE;
    return b_result;
}
/*****************************************************************************/
//  Description : Display dancing girl of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIDLE_GetDancingGirlImageofSweetTree(MMI_HANDLE_T win_id)
{
    BOOLEAN b_result = FALSE;
    GUI_POINT_T dis_point = {0,0};
    GUI_RECT_T image_rect = {0};
    UILAYER_CREATE_T create_infor = {0};
    uint8 *layer_buf_ptr = PNULL; 

    create_infor.lcd_id = MAIN_LCD_ID;
    create_infor.owner_handle = win_id;
    create_infor.is_bg_layer = FALSE;
    create_infor.is_static_layer = FALSE;
    create_infor.offset_x = 0;// lcd_rect.left;
    create_infor.offset_y = 0;//lcd_rect.top;
    create_infor.width = s_girl_pic_width;
    create_infor.height = s_girl_dis_height;
    create_info.func_ptr = MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(); 
    
    if(UILAYER_RESULT_SUCCESS != UILAYER_CreateLayer(&create_infor, &s_sweettree_dancinggirl_lcd_dev))
    {
        return FALSE;
    }

    layer_buf_ptr = UILAYER_GetLayerBufferPtr(&s_sweettree_dancinggirl_lcd_dev);
    if(PNULL == layer_buf_ptr)
    {
        UILAYER_ReleaseLayer(&s_sweettree_dancinggirl_lcd_dev);
        return FALSE;
    }

	// 使用color清除层
    UILAYER_Clear(&s_sweettree_dancinggirl_lcd_dev);

    image_rect.left = 0;
    image_rect.right = s_girl_pic_width;
    image_rect.top = s_girl_pic_height - create_infor.height;
    image_rect.bottom = s_girl_pic_height;
    GUIRES_DisplayImg(&dis_point,PNULL,&image_rect, win_id,
            IMAGE_WALLPAPER_SWEETTREE_GIRL01,&s_sweettree_dancinggirl_lcd_dev);

     SCI_MEMCPY( girl1_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_Clear(&s_sweettree_dancinggirl_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,&image_rect, win_id,
            IMAGE_WALLPAPER_SWEETTREE_GIRL02,&s_sweettree_dancinggirl_lcd_dev);

     SCI_MEMCPY( girl2_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_Clear(&s_sweettree_dancinggirl_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,&image_rect, win_id,
            IMAGE_WALLPAPER_SWEETTREE_GIRL03,&s_sweettree_dancinggirl_lcd_dev);

     SCI_MEMCPY( girl3_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

UILAYER_Clear(&s_sweettree_dancinggirl_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,&image_rect, win_id,
            IMAGE_WALLPAPER_SWEETTREE_GIRL04,&s_sweettree_dancinggirl_lcd_dev);

     SCI_MEMCPY( girl4_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);


    UILAYER_ReleaseLayer(&s_sweettree_dancinggirl_lcd_dev);
    b_result = TRUE;
    return b_result;
 }
/*****************************************************************************/
//  Description : get layer infor of bird 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIDLE_GetRabbitRunImageofSweetTree(MMI_HANDLE_T win_id)
{
    uint8 *layer_buf_ptr = PNULL;
    GUI_POINT_T dis_point = {0,0};
    UILAYER_CREATE_T create_infor = {0};
//   GUI_RECT_T lcd_rect = {0};

 //   lcd_rect = MMITHEME_GetFullScreenRect();
    create_infor.lcd_id = MAIN_LCD_ID;
    create_infor.owner_handle = win_id;
    create_infor.is_bg_layer = FALSE;
    create_infor.is_static_layer = FALSE;
    create_infor.offset_x = 0;//lcd_rect.left;
    create_infor.offset_y = 0;//lcd_rect.top;
    create_infor.width = s_rabbit_run_pic_width;
    create_infor.height = s_rabbit_run_pic_height;
    create_info.func_ptr = MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(); 
    if(UILAYER_RESULT_SUCCESS != UILAYER_CreateLayer(&create_infor, &s_sweettree_rabbit_lcd_dev))
    {
        return FALSE;
    }

    // 设置color key
 //   UILAYER_SetLayerColorKey(&s_sweettree_bird_lcd_dev, TRUE, UILAYER_TRANSPARENT_COLOR);
    layer_buf_ptr = UILAYER_GetLayerBufferPtr(&s_sweettree_rabbit_lcd_dev);
    if(PNULL == layer_buf_ptr)
    {
        UILAYER_ReleaseLayer(&s_sweettree_rabbit_lcd_dev);
        return FALSE;
    }
    
	// 使用color清除层
    UILAYER_Clear(&s_sweettree_rabbit_lcd_dev);
    
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL, win_id,
            IMAGE_SWEETTREE_RABBIT_RUN02,&s_sweettree_rabbit_lcd_dev);

    SCI_MEMCPY(s_rabbit_run2_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_Clear(&s_sweettree_rabbit_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL, win_id,
            IMAGE_SWEETTREE_RABBIT_RUN03,&s_sweettree_rabbit_lcd_dev);
    SCI_MEMCPY(s_rabbit_run3_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

   
    UILAYER_ReleaseLayer(&s_sweettree_rabbit_lcd_dev);
    return TRUE;
 }


/*****************************************************************************/
//  Description : get layer infor of bird nest
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIDLE_GetBirdAndNestImageSweetTree(MMI_HANDLE_T win_id)
{
    uint8 *layer_buf_ptr = PNULL;
    GUI_POINT_T dis_point = {0,0};
   
    UILAYER_CREATE_T create_infor = {0};
 
    create_infor.lcd_id = MAIN_LCD_ID;
    create_infor.owner_handle = win_id;
    create_infor.is_bg_layer = FALSE;
    create_infor.is_static_layer = FALSE;
    create_infor.offset_x = 0;//lcd_rect.left;
    create_infor.offset_y = 0;//lcd_rect.top;
    create_infor.width = s_nest_pic_width;
    create_infor.height = s_nest_pic_height;
    create_info.func_ptr = MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(); 
    if(UILAYER_RESULT_SUCCESS != UILAYER_CreateLayer(&create_infor, &s_bird_nest_lcd_dev))
    {
        return FALSE;
    }

    layer_buf_ptr = UILAYER_GetLayerBufferPtr(&s_bird_nest_lcd_dev);
    if(PNULL == layer_buf_ptr)
    {
        UILAYER_ReleaseLayer(&s_bird_nest_lcd_dev);
        return FALSE;
    }
    
    UILAYER_Clear(&s_bird_nest_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL, win_id,
            IMAGE_WALLPAPER_SWEETTREE_BIRDNEST01,&s_bird_nest_lcd_dev);
    SCI_MEMCPY(s_nest1_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_Clear(&s_bird_nest_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL, win_id,
            IMAGE_WALLPAPER_SWEETTREE_BIRDNEST02,&s_bird_nest_lcd_dev);
    SCI_MEMCPY(s_nest2_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_Clear(&s_bird_nest_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL, win_id,
            IMAGE_WALLPAPER_SWEETTREE_BIRDNEST03,&s_bird_nest_lcd_dev);
    SCI_MEMCPY(s_nest3_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_Clear(&s_bird_nest_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL, win_id,
            IMAGE_WALLPAPER_SWEETTREE_BIRDNEST04,&s_bird_nest_lcd_dev);
    SCI_MEMCPY(s_nest4_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_Clear(&s_bird_nest_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL, win_id,
            IMAGE_WALLPAPER_SWEETTREE_BIRDNEST_LEFT,&s_bird_nest_lcd_dev);
    SCI_MEMCPY(s_nest_left_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_Clear(&s_bird_nest_lcd_dev);
    
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL, win_id,
            IMAGE_SWEETTREE_BIRDFLY01,&s_bird_nest_lcd_dev);

    SCI_MEMCPY(s_bird_fly1_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_Clear(&s_bird_nest_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL, win_id,
            IMAGE_SWEETTREE_BIRDFLY02,&s_bird_nest_lcd_dev);
    SCI_MEMCPY(s_bird_fly2_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_Clear(&s_bird_nest_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL, win_id,
            IMAGE_SWEETTREE_BIRDFLY03,&s_bird_nest_lcd_dev);
    SCI_MEMCPY(s_bird_fly3_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_Clear(&s_bird_nest_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL, win_id,
            IMAGE_SWEETTREE_BIRDFLY04,&s_bird_nest_lcd_dev);
    SCI_MEMCPY(s_bird_fly4_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_ReleaseLayer(&s_bird_nest_lcd_dev);
    return TRUE;
 }

/*****************************************************************************/
//  Description : Display the Squirrel cave of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIDLE_GetSquirrelIMGofSweetTree(MMI_WIN_ID_T win_id)
{
    uint8 *layer_buf_ptr = PNULL;
    GUI_POINT_T dis_point = {0,0};
    
    UILAYER_CREATE_T create_infor = {0};
    
    create_infor.lcd_id = MAIN_LCD_ID;
    create_infor.owner_handle = win_id;
    create_infor.is_bg_layer = FALSE;
    create_infor.is_static_layer = FALSE;
    create_infor.offset_x = 0;
    create_infor.offset_y = 0;
    create_infor.width = s_nest_pic_width;
    create_infor.height = s_nest_pic_height;
    create_info.func_ptr = MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(); 
    if(UILAYER_RESULT_SUCCESS != UILAYER_CreateLayer(&create_infor, &s_sweettree_squirrel_lcd_dev))
    {
        return FALSE;
    }
    // 设置color key
 //   UILAYER_SetLayerColorKey(&s_sweettree_squirrel_lcd_dev, TRUE, UILAYER_TRANSPARENT_COLOR);

    layer_buf_ptr = UILAYER_GetLayerBufferPtr(&s_sweettree_squirrel_lcd_dev);
    if(PNULL == layer_buf_ptr)
    {
        UILAYER_ReleaseLayer(&s_sweettree_squirrel_lcd_dev);
        return FALSE;
    }
    
    // 使用color清除层
    UILAYER_Clear(&s_sweettree_squirrel_lcd_dev);

    GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_SWEETTREE_SQUIRREL_HEAD01,
            &s_sweettree_squirrel_lcd_dev);
    SCI_MEMCPY(s_squirrel_head1_buf_ptr,layer_buf_ptr,create_infor.width*create_infor.height*4);

    UILAYER_Clear(&s_sweettree_squirrel_lcd_dev);
GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_SWEETTREE_SQUIRREL_HEAD02,
        &s_sweettree_squirrel_lcd_dev);
    SCI_MEMCPY(s_squirrel_head2_buf_ptr,layer_buf_ptr,create_infor.width*create_infor.height*4);

    UILAYER_Clear(&s_sweettree_squirrel_lcd_dev);
GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_SWEETTREE_SQUIRREL_HEAD03,
        &s_sweettree_squirrel_lcd_dev);
    SCI_MEMCPY(s_squirrel_head3_buf_ptr,layer_buf_ptr,create_infor.width*create_infor.height*4);

    UILAYER_Clear(&s_sweettree_squirrel_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_SWEETTREE_SQUIRREL_TAIL01,
            &s_sweettree_squirrel_lcd_dev);
    SCI_MEMCPY(s_squirrel_tail1_buf_ptr,layer_buf_ptr,create_infor.width*create_infor.height*4);

    UILAYER_Clear(&s_sweettree_squirrel_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_SWEETTREE_SQUIRREL_TAIL02,
            &s_sweettree_squirrel_lcd_dev);
    SCI_MEMCPY(s_squirrel_tail2_buf_ptr,layer_buf_ptr,create_infor.width*create_infor.height*4);

    UILAYER_Clear(&s_sweettree_squirrel_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_SWEETTREE_SQUIRREL_TAIL03,
            &s_sweettree_squirrel_lcd_dev);
    SCI_MEMCPY(s_squirrel_tail3_buf_ptr,layer_buf_ptr,create_infor.width*create_infor.height*4);

    
    UILAYER_Clear(&s_sweettree_squirrel_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_SWEETTREE_SQUIRREL_RUN01,
            &s_sweettree_squirrel_lcd_dev);
    SCI_MEMCPY(s_squirrel_run1_buf_ptr,layer_buf_ptr,create_infor.width*create_infor.height*4);

    UILAYER_Clear(&s_sweettree_squirrel_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_SWEETTREE_SQUIRREL_RUN02,
            &s_sweettree_squirrel_lcd_dev);
    SCI_MEMCPY(s_squirrel_run2_buf_ptr,layer_buf_ptr,create_infor.width*create_infor.height*4);

    UILAYER_Clear(&s_sweettree_squirrel_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL,win_id,IMAGE_SWEETTREE_SQUIRREL_RUN03,
        &s_sweettree_squirrel_lcd_dev);
    SCI_MEMCPY(s_squirrel_run3_buf_ptr,layer_buf_ptr,create_infor.width*create_infor.height*4);


    UILAYER_ReleaseLayer(&s_sweettree_squirrel_lcd_dev);
    return TRUE;
}

/*****************************************************************************/
//  Description : Display the Squirrel cave of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIDLE_GetRabbitIMGofSweetTree(MMI_WIN_ID_T win_id)
{
    GUI_POINT_T dis_point = {0,0};
    uint8 *layer_buf_ptr = PNULL;
    UILAYER_CREATE_T create_infor = {0};

    
    create_infor.lcd_id = MAIN_LCD_ID;
    create_infor.owner_handle = win_id;
    create_infor.is_bg_layer = FALSE;
    create_infor.is_static_layer = FALSE;
    create_infor.offset_x = 0;
    create_infor.offset_y =  0;
    create_infor.width = s_rabbit_stand_pic_width;
    create_infor.height = s_rabbit_stand_pic_height;
    create_info.func_ptr = MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(); 
    if(UILAYER_RESULT_SUCCESS != UILAYER_CreateLayer(&create_infor, &s_sweettree_rabbit_lcd_dev))
    {
        return FALSE;
    }
   
    layer_buf_ptr = UILAYER_GetLayerBufferPtr(&s_sweettree_rabbit_lcd_dev);
    if(PNULL == layer_buf_ptr)
    {
        UILAYER_ReleaseLayer(&s_sweettree_rabbit_lcd_dev);
        return FALSE;
    }

    // 使用color清除层
    UILAYER_Clear(&s_sweettree_rabbit_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL, win_id,
            IMAGE_SWEETTREE_RABBIT_STAND02,&s_sweettree_rabbit_lcd_dev);

    SCI_MEMCPY(s_rabbit_stand2_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_Clear(&s_sweettree_rabbit_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL, win_id,
            IMAGE_SWEETTREE_RABBIT_STAND03,&s_sweettree_rabbit_lcd_dev);

    SCI_MEMCPY(s_rabbit_stand3_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_Clear(&s_sweettree_rabbit_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL, win_id,
            IMAGE_SWEETTREE_RABBIT_STAND04,&s_sweettree_rabbit_lcd_dev);

    SCI_MEMCPY(s_rabbit_stand4_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_Clear(&s_sweettree_rabbit_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL, win_id,
            IMAGE_SWEETTREE_RABBIT_STAND05,&s_sweettree_rabbit_lcd_dev);

    SCI_MEMCPY(s_rabbit_stand5_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_ReleaseLayer(&s_sweettree_rabbit_lcd_dev);
    return TRUE;
}

/*****************************************************************************/
//  Description : get layer infor of butterfly 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIDLE_GetButterflyImageofSweetTree(MMI_HANDLE_T win_id)
{
    uint8 *layer_buf_ptr = PNULL;
    GUI_POINT_T dis_point = {0};
    UILAYER_CREATE_T create_infor = {0};
   
    create_infor.lcd_id = MAIN_LCD_ID;
    create_infor.owner_handle = win_id;
    create_infor.is_bg_layer = FALSE;
    create_infor.is_static_layer = FALSE;
    create_infor.offset_x = 0;
    create_infor.offset_y = 0;
    create_infor.width = s_butterfly_pic_width;
    create_infor.height = s_butterfly_pic_height;
    create_info.func_ptr = MMIAPILIVEWALLPAPER_UilayerMemPoolFunc(); 
    if(UILAYER_RESULT_SUCCESS != UILAYER_CreateLayer(&create_infor, &s_sweettree_butterfly_lcd_dev))
    {
        return FALSE;
    }

    layer_buf_ptr = UILAYER_GetLayerBufferPtr(&s_sweettree_butterfly_lcd_dev);
    if(PNULL == layer_buf_ptr)
    {
        UILAYER_ReleaseLayer(&s_sweettree_butterfly_lcd_dev);
        return FALSE;
    }

    // 设置color key
   // UILAYER_SetLayerColorKey(&s_sweettree_butterfly_lcd_dev, TRUE, UILAYER_TRANSPARENT_COLOR);

	// 使用color清除层
    UILAYER_Clear(&s_sweettree_butterfly_lcd_dev);

    GUIRES_DisplayImg(&dis_point,PNULL,PNULL, win_id,
            IMAGE_WALLPAPER_SWEETTREE_BUTTERFLY01,&s_sweettree_butterfly_lcd_dev);

    SCI_MEMCPY(s_butterfly1_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_Clear(&s_sweettree_butterfly_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL, win_id,
            IMAGE_WALLPAPER_SWEETTREE_BUTTERFLY02,&s_sweettree_butterfly_lcd_dev);
    SCI_MEMCPY(s_butterfly2_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_Clear(&s_sweettree_butterfly_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL, win_id,
            IMAGE_WALLPAPER_SWEETTREE_BUTTERFLY03,&s_sweettree_butterfly_lcd_dev);
    SCI_MEMCPY(s_butterfly3_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_Clear(&s_sweettree_butterfly_lcd_dev);
    GUIRES_DisplayImg(&dis_point,PNULL,PNULL, win_id,
            IMAGE_WALLPAPER_SWEETTREE_BUTTERFLY04,&s_sweettree_butterfly_lcd_dev);
    SCI_MEMCPY(s_butterfly4_buf_ptr,layer_buf_ptr,(create_infor.width*create_infor.height)*4);

    UILAYER_ReleaseLayer(&s_sweettree_butterfly_lcd_dev);
    return TRUE;
 }


/*****************************************************************************/
//  Description : 甜蜜树构造函数
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SweetTreeConstruct(MMI_WIN_ID_T win_id)
{
    BOOLEAN b_result = FALSE;
    //SCI_TRACE_LOW:"SweetTreeConstruct()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_1369_112_2_18_2_32_30_38,(uint8*)"");
    
	clean_bg_buf_ptr = MMILIVEWALLPAPER_ALLOC(2*(s_lcd_width)*(s_lcd_height));
    if(PNULL == clean_bg_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }

    girl1_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_girl_pic_width)*(s_girl_dis_height));
    if(PNULL == girl1_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    girl2_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_girl_pic_width)*(s_girl_dis_height));
    if(PNULL == girl2_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    girl3_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_girl_pic_width)*(s_girl_dis_height));
    if(PNULL == girl3_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    girl4_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_girl_pic_width)*(s_girl_dis_height));
    if(PNULL == girl4_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }

    
    girl_rotation_buf_ptr = MMILIVEWALLPAPER_ALLOC(ROTATION_BUFFER_SIZE);
    if(PNULL == girl_rotation_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }

      s_rabbit_run2_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_rabbit_run_pic_width)*s_rabbit_run_pic_height);
    if(PNULL == s_rabbit_run2_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    s_rabbit_run3_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_rabbit_run_pic_width)*s_rabbit_run_pic_height);
    if(PNULL == s_rabbit_run3_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    
   s_bird_fly1_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_nest_pic_width)*s_nest_pic_height);
    if(PNULL == s_bird_fly1_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    s_bird_fly2_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_nest_pic_width)*s_nest_pic_height);
    if(PNULL == s_bird_fly2_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    s_bird_fly3_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_nest_pic_width)*s_nest_pic_height);
    if(PNULL == s_bird_fly3_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    s_bird_fly4_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_nest_pic_width)*s_nest_pic_height);
    if(PNULL == s_bird_fly4_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }

    
    s_nest1_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_nest_pic_width)*s_nest_pic_height);
    if(PNULL == s_nest1_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    s_nest2_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_nest_pic_width)*s_nest_pic_height);
    if(PNULL == s_nest2_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    s_nest3_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_nest_pic_width)*s_nest_pic_height);
    if(PNULL == s_nest3_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    s_nest4_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_nest_pic_width)*s_nest_pic_height);
    if(PNULL == s_nest4_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    s_nest_left_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_nest_pic_width)*s_nest_pic_height);
    if(PNULL == s_nest_left_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
   
    
    s_squirrel_head1_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_nest_pic_width)*s_nest_pic_height);
    if(PNULL == s_squirrel_head1_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    s_squirrel_head2_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_nest_pic_width)*s_nest_pic_height);
    if(PNULL == s_squirrel_head2_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    s_squirrel_head3_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_nest_pic_width)*s_nest_pic_height);
    if(PNULL == s_squirrel_head3_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    s_squirrel_tail1_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_nest_pic_width)*s_nest_pic_height);
    if(PNULL == s_squirrel_tail1_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    s_squirrel_tail2_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_nest_pic_width)*s_nest_pic_height);
    if(PNULL == s_squirrel_tail2_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
     s_squirrel_tail3_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_nest_pic_width)*s_nest_pic_height);
    if(PNULL == s_squirrel_tail3_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
     s_squirrel_run1_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_nest_pic_width)*s_nest_pic_height);
    if(PNULL == s_squirrel_run1_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
     s_squirrel_run2_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_nest_pic_width)*s_nest_pic_height);
    if(PNULL == s_squirrel_run2_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
     s_squirrel_run3_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_nest_pic_width)*s_nest_pic_height);
    if(PNULL == s_squirrel_run3_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    
    s_butterfly1_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_butterfly_pic_width)*(s_butterfly_pic_height));
    if(PNULL ==s_butterfly1_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    s_butterfly2_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_butterfly_pic_width)*(s_butterfly_pic_height));
    if(PNULL ==s_butterfly2_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    s_butterfly3_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_butterfly_pic_width)*(s_butterfly_pic_height));
    if(PNULL ==s_butterfly3_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    s_butterfly4_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_butterfly_pic_width)*(s_butterfly_pic_height));
    if(PNULL ==s_butterfly4_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }

    s_rabbit_stand2_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_rabbit_stand_pic_width)*(s_rabbit_stand_pic_height));
    if(PNULL ==s_rabbit_stand2_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    s_rabbit_stand3_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_rabbit_stand_pic_width)*(s_rabbit_stand_pic_height));
    if(PNULL ==s_rabbit_stand3_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    s_rabbit_stand4_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_rabbit_stand_pic_width)*(s_rabbit_stand_pic_height));
    if(PNULL ==s_rabbit_stand4_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }
    s_rabbit_stand5_buf_ptr = MMILIVEWALLPAPER_ALLOC(4*(s_rabbit_stand_pic_width)*(s_rabbit_stand_pic_height));
    if(PNULL ==s_rabbit_stand5_buf_ptr)
    {
        FreeAllBufferOfSweetTree();
        return FALSE;
    }

    
    if(!MMIIDLE_GetSquirrelIMGofSweetTree(win_id))
    {
          return FALSE;  
    }
        if(!MMIIDLE_GetBigTreeBGImageofSweetTree(win_id))
    {
          return FALSE;  
    }
        if(!MMIIDLE_GetDancingGirlImageofSweetTree(win_id))
    {
          return FALSE;  
    }
        if(!MMIIDLE_GetButterflyImageofSweetTree(win_id))
    {
          return FALSE;  
    }
        if(!MMIIDLE_GetRabbitIMGofSweetTree(win_id))
    {
          return FALSE;  
    }
        if(!MMIIDLE_GetBirdAndNestImageSweetTree(win_id))
    {
          return FALSE;  
    }
        if(!MMIIDLE_GetRabbitRunImageofSweetTree(win_id))
    {
          return FALSE;  
    }
    s_sweettree_old_tick = SCI_GetTickCount();
    s_bird_old_tick = SCI_GetTickCount();
    s_squirrel_old_tick = SCI_GetTickCount();
    s_sweettree_wallpaper_param.girl_strong_reduce_cycel = 0;
    
    b_result = TRUE;
    return b_result;
}

/*****************************************************************************/
//  Description :甜蜜树准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SweetTreeDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    BOOLEAN b_result = FALSE;
    GUI_POINT_T dis_point = {0,0};
    GUI_RECT_T lcd_rect = MMITHEME_GetFullScreenRect();
    GUI_RECT_T image_rect = {0};
    uint16 index = 0;

    //SCI_TRACE_LOW:"SweetTreeDrawBg()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_1613_112_2_18_2_32_31_39,(uint8*)"");
        
    image_rect = lcd_rect;
    image_rect.bottom = lcd_rect.bottom-s_grass_pic_height;
           
    if (PNULL != dev_info_ptr)
    {
        b_result = GUIRES_DisplayImg(&dis_point,
                                                            PNULL,
                                                            &image_rect,
                                                            win_id, 
                                                            IMAGE_WALLPAPER_SWEETTREE_BG,
                                                            dev_info_ptr);
    }
   
        for(index = 0;index<SWEETTREE_CLOUD_NUM;index++)
        {
            dis_point.x = s_cloud_coordinate[index].x;
            dis_point.y = s_cloud_coordinate[index].y;
        
        if (b_result)
        {
            b_result = GUIRES_DisplayImg(&dis_point,
                                                                PNULL,
                                                                PNULL,
                                                                win_id,
            s_cloud_anim[index],dev_info_ptr);
        }
    }

         //display back river
        dis_point.x = 0;                
        dis_point.y = s_sweettree_wallpaper_param.river_pic_y_offset;

    if (b_result)
    {
        b_result = GUIRES_DisplayImg(&dis_point,
                                                            PNULL,
                                                            PNULL,
                                                            win_id,
                                            	        IMAGE_WALLPAPER_SWEETTREE_RIVER,
                                            	        dev_info_ptr);
    }
        //display smalltree
         dis_point.x = 0;                
         dis_point.y = s_sweettree_wallpaper_param.smalltree_pic_y_offset;

    if (b_result)
    {
        b_result = GUIRES_DisplayImg(&dis_point,
                                                            PNULL,
                                                            PNULL,
                                                            win_id,
		IMAGE_WALLPAPER_SWEETTREE_SMALLTREE,
                                            		dev_info_ptr);
    }
        
		//display bigtree
		dis_point.x = 0;                
            dis_point.y = s_sweettree_wallpaper_param.bigtree_pic_y_offset;

                image_rect.left = 0;
           image_rect.top = s_grass_pic_height/2;
            image_rect.right = lcd_rect.right;
           image_rect.bottom = s_bigtree_pic_height;
            
    if (b_result)
    {
        b_result = GUIRES_DisplayImg(&dis_point,
                                                            PNULL,
                                                            &image_rect,
                                                            win_id,
                                            		IMAGE_WALLPAPER_SWEETTREE_BIGTREE,
                                            		dev_info_ptr);
    }
            //display big nest
    dis_point.x = s_bird_nest_coordinate.x;
    dis_point.y = s_bird_nest_coordinate.y;

    if (b_result)
    {
        b_result = GUIRES_DisplayImg(&dis_point,
                                                            PNULL,
                                                            PNULL,
                                                            win_id,
                                                            IMAGE_WALLPAPER_SWEETTREE_BIRDNEST01,
                                                            dev_info_ptr);
        }
        //display rabbit
        dis_point.x = s_rabbit_stand_coordinate.x;
    dis_point.y = s_rabbit_stand_coordinate.y;

    if (b_result)
    {
        b_result = GUIRES_DisplayImg(&dis_point,
                                                            PNULL,
                                                            PNULL,
                                                            win_id,
                                                            IMAGE_SWEETTREE_RABBIT_STAND02,
                                                            dev_info_ptr);
        }
            //display squirrel cave
           dis_point.x = s_squirrel_cave_coordinate.x;
            dis_point.y = s_squirrel_cave_coordinate.y;//- s_sweettree_wallpaper_param.bigtree_pic_y_offset;

    if (b_result)
    {
        b_result = GUIRES_DisplayImg(&dis_point,
                                                            PNULL,
                                                            PNULL,
                                                            win_id,
                                                            IMAGE_SWEETTREE_SQUIRREL_TAIL03,
                                                            dev_info_ptr);
    }
        dis_point.x = s_rose_coordinate.x;
        dis_point.y = s_rose_coordinate.y;
    if (b_result)
    {
        b_result = GUIRES_DisplayImg(&dis_point,
                                                            PNULL,
                                                            PNULL,
                                                            win_id,
		IMAGE_WALLPAPER_SWEETTREE_ROSE,
                                            		dev_info_ptr);
    }
        
        for(index = 0;index<SWEETTREE_MUSHROOM_NUM;index++)
        {
            dis_point.x = s_mushroom_coordinate[index].x;
            dis_point.y = s_mushroom_coordinate[index].y;
        
        if (b_result)
        {
            b_result = GUIRES_DisplayImg(&dis_point,
                                                            PNULL,
                                                            PNULL,
                                                            win_id,
                                                            (IMAGE_SWEETTREE_MUSHROOM_PINK+index),
                                                            dev_info_ptr);
        }
    }
        //display grass
            dis_point.x = 0;
		dis_point.y = s_sweettree_wallpaper_param.grass_pic_y_offset;

       //     image_rect.left = s_sweettree_wallpaper_param.scene_frame_index*s_grass_a_frame_pix;
    //        image_rect.top = 0;
      //      image_rect.right = image_rect.left+lcd_rect.right;
     //       image_rect.bottom = s_grass_pic_height;
            
    if (b_result)
    {
        b_result = GUIRES_DisplayImg(&dis_point,
                                                            PNULL,
                                                            PNULL,
                                                            win_id,
		IMAGE_WALLPAPER_SWEETTREE_GRASS,
                                            		dev_info_ptr);
    }

    return b_result;
}


/*****************************************************************************/
//  Description : change the girl swing strong level
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_ChangeGirlSwayStrongeLevel(BOOL add)
{
    //SCI_TRACE_LOW:"MMIIDLE_ChangeGirlSwayStrongeLevel()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_1718_112_2_18_2_32_31_40,(uint8*)"");
    if(add)
    {
        if(s_sweettree_wallpaper_param.strong_level< GIRL_SWING_STRONG_LEVEL_NUM)
        {
            s_sweettree_wallpaper_param.strong_level++;
        }
    }
    else
    {
        if(s_sweettree_wallpaper_param.strong_level>1)
        {
            s_sweettree_wallpaper_param.strong_level--;
        }
    }
}
/*****************************************************************************/
//  Description : change the bird state
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_ChangeBirdState()
{
    //SCI_TRACE_LOW:"MMIIDLE_ChangeBirdState()"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_1740_112_2_18_2_32_31_41,(uint8*)"");
    switch(s_sweettree_wallpaper_param.bird_state)
    {
        case MMI_SWEETTREE_BIRD_IN_CAVE:
        case MMI_SWEETTREE_BIRD_SWAYED:
        case MMI_SWEETTREE_BIRD_FLYAWAY:
        case MMI_SWEETTREE_BIRD_FLYBACK:
        case MMI_SWEETTREE_BIRD_TURN_RIGHT:
            s_sweettree_wallpaper_param.bird_state++;
     break;
      case MMI_SWEETTREE_BIRD_THOUCHED:
        break;
   
    default:
        s_sweettree_wallpaper_param.bird_state = MMI_SWEETTREE_BIRD_IN_CAVE;
               break;
    }    
}

/*****************************************************************************/
//  Description : Display the squirrel state
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_ChangeSquirrelState()
{
    //SCI_TRACE_LOW:"MMIIDLE_ChangeSquirrelState"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_1765_112_2_18_2_32_31_42,(uint8*)"");
    switch(s_sweettree_wallpaper_param.squirrel_state)
    {
        case MMI_SWEETTREE_SQUIRREL_IN_CAVE:
        case MMI_SWEETTREE_SQUIRREL_SWAYED:
            case MMI_SWEETTREE_SQUIRREL_RUNAWAY:
             s_sweettree_wallpaper_param.squirrel_state++;
        break;
     case MMI_SWEETTREE_SQUIRREL_TOUCHED:
        break;
        default:
            s_sweettree_wallpaper_param.squirrel_state = MMI_SWEETTREE_SQUIRREL_IN_CAVE;
        break;
    }    
}

/*****************************************************************************/
//  Description : change the butterfly state 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_ChangeButterflyState()
{
    //SCI_TRACE_LOW:"MMIIDLE_ChangeButterflyState"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_1787_112_2_18_2_32_31_43,(uint8*)"");
    switch(s_sweettree_wallpaper_param.butterfly_state)
    {
        case MMI_SWEETTREE_BUTTERFLY_IDLE:
        case MMI_SWEETTREE_BUTTERFLY_SWAYED:
        case MMI_SWEETTREE_BUTTERFLY_FLYAWAY:
            s_sweettree_wallpaper_param.butterfly_state++;
        break;
        case MMI_SWEETTREE_BUTTERFLY_FLYBACK:
            s_sweettree_wallpaper_param.butterfly_state = MMI_SWEETTREE_BUTTERFLY_IDLE;
        break;
        default:
            s_sweettree_wallpaper_param.butterfly_state = MMI_SWEETTREE_BUTTERFLY_IDLE;
        break;
    }    
}

/*****************************************************************************/
//  Description : change the butterfly state 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_ChangeRabbitState()
{
    //SCI_TRACE_LOW:"MMIIDLE_ChangeRabbitState"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_1810_112_2_18_2_32_31_44,(uint8*)"");
    switch(s_sweettree_wallpaper_param.rabbit_state)
    {
        case MMI_SWEETTREE_RABBIT_IDLE:
        case MMI_SWEETTREE_RABBIT_RUNAWAY:
            s_sweettree_wallpaper_param.rabbit_state++;
        break;
        
        default:
            s_sweettree_wallpaper_param.rabbit_state = MMI_SWEETTREE_RABBIT_IDLE;
        break;
    }    
}
LOCAL void CalcTPVavlidRectOfGirlSwing(int16 angle)
{
    BOOL negtive_tag = FALSE;
    int16 temp = 0;

    //SCI_TRACE_LOW:"CalcTPVavlidRectOfGirlSwing"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_1828_112_2_18_2_32_31_45,(uint8*)"");
    
    angle = angle/10;
    if(angle<0)
    {
        negtive_tag = TRUE;
        angle = -angle;
    }
    temp = (int16)((SIN_S(angle))*s_girl_dis_height);
    if(negtive_tag)
        {
            temp = -temp;
        }
    s_girl_swing_tp_valid_rect.left = s_dancing_girl_coordinate.x -temp-GIRL_SWING_LINE_OFFSET;
    s_girl_swing_tp_valid_rect.right = s_girl_swing_tp_valid_rect.left + GIRL_SWING_TPDOWN_VALID_WINDOW;
    temp = (int16)((COS_S(angle))*s_girl_dis_height) ;
    s_girl_swing_tp_valid_rect.top = temp - GIRL_SWING_TPDOWN_VALID_WINDOW;
    s_girl_swing_tp_valid_rect.bottom = temp ;
    
}
/*****************************************************************************/
//  Description : rotate the girl_swing picture 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_GirlSwing(COLOR16 * dst,COLOR32 * front,FIX16_POINT_T * rota_buffer,IMGREF_SIZE_T window_size,IMGREF_SIZE_T img_size,int frame,int total_frame,int level)
{    
    int16 angle = 0;
    const uint16 *angle_array = PNULL;
    IMGREF_POINT_T offset_window = {0};
    IMGREF_POINT_T offset_img = {0};

    //SCI_TRACE_LOW:"enter MMIIDLE_GirlSwing"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_1859_112_2_18_2_32_31_46,(uint8*)"");
    
    offset_window.x=s_dancing_girl_coordinate.x;//window_size.w/2;
    offset_window.y=s_dancing_girl_coordinate.y;//window_size.h/6;

    offset_img.x=GIRL_SWING_LINE_OFFSET;
    offset_img.y=0;

    switch(level)
    {     
        case 1:
        angle_array=s_girl_swing_angle_array1;
        break;
        case 2:
        angle_array=s_girl_swing_angle_array2;
        break;
        case 3:
        angle_array=s_girl_swing_angle_array3;
        break;
    //    case 4:
    //    angle_array=s_girl_swing_angle_array4;
    //    break;
       default:
        angle_array=s_girl_swing_angle_array1;
        break;

        }
	
    if(frame<=GIRL_SWING_ANGLE_NUM)
    {
	   angle=angle_array[frame-1];
    }
    else if ((frame>GIRL_SWING_ANGLE_NUM)&&(frame<=(2*GIRL_SWING_ANGLE_NUM-1)))
    {
		   angle=angle_array[39-frame];
    }
    else if ((frame>=(2*GIRL_SWING_ANGLE_NUM))&&(frame<=(3*GIRL_SWING_ANGLE_NUM-1)))
    {
		   angle=-angle_array[frame-40];
    }
    else if ((frame>=(3*GIRL_SWING_ANGLE_NUM))&&(frame<=GIRL_SWING_TOTAL_FRAM_NUM))
    {
        angle=-angle_array[GIRL_SWING_TOTAL_FRAM_NUM-frame];
    }
    
    
    CalcTPVavlidRectOfGirlSwing(angle);
    S2d_rotation_slim(window_size,img_size,offset_window,offset_img,angle,front,dst,4096,rota_buffer);

    //SCI_TRACE_LOW:"exit MMIIDLE_GirlSwing"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_1908_112_2_18_2_32_32_47,(uint8*)"");
}
/*****************************************************************************/
//  Description : Display the squirrel of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_DisplayBirdofSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
	IMGREF_SIZE_T lcd_size = {0};

      //SCI_TRACE_LOW:"MMIIDLE_DisplayBirdofSweetTree"
      SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_1920_112_2_18_2_32_32_48,(uint8*)"");
      
    lcd_size.w = s_lcd_width;
    lcd_size.h = s_lcd_height;   
                 //blend butterfly    
    offset.x = s_bird_flyback_coordinate[BIRD_FLY_ANIM_FRAME_NUM-1].x;
    offset.y = s_bird_flyback_coordinate[BIRD_FLY_ANIM_FRAME_NUM-1].y;
    layer_size.w = s_nest_pic_width;
    layer_size.h = s_nest_pic_height;
    if(MMI_SWEETTREE_BIRD_FLYBACK == s_sweettree_wallpaper_param.bird_state)
    {
        S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
            offset,s_bird_fly3_buf_ptr,rgb565_buf_ptr
            );
        }
    else
        {
            S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
            offset,s_bird_fly4_buf_ptr,rgb565_buf_ptr
            );
        }
}
/*****************************************************************************/
//  Description : Display the squirrel of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_DisplayBirdNestofSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
    IMGREF_SIZE_T lcd_size = {0};

     //SCI_TRACE_LOW:"MMIIDLE_DisplayBirdNestofSweetTree"
     SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_1952_112_2_18_2_32_32_49,(uint8*)"");
     
    lcd_size.w = s_lcd_width;
    lcd_size.h = s_lcd_height;   
  
                 //blend butterfly    
    offset.x = s_bird_nest_coordinate.x;
    offset.y = s_bird_nest_coordinate.y;
    layer_size.w = s_nest_pic_width;
    layer_size.h = s_nest_pic_height;
    S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
            offset,s_nest1_buf_ptr,rgb565_buf_ptr
            );
}
/*****************************************************************************/
//  Description : Display the squirrel of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_DisplaySquirrelofSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
	IMGREF_SIZE_T lcd_size = {0};
    
    //SCI_TRACE_LOW:"MMIIDLE_DisplaySquirrelofSweetTree"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_1976_112_2_18_2_32_32_50,(uint8*)"");
    
    lcd_size.w = s_lcd_width;
    lcd_size.h = s_lcd_height;   
  
                 //blend butterfly    
    offset.x = s_squirrel_cave_coordinate.x;
    offset.y = s_squirrel_cave_coordinate.y;
    layer_size.w = s_nest_pic_width;
    layer_size.h = s_nest_pic_height;
    S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
            offset,s_squirrel_tail3_buf_ptr,rgb565_buf_ptr
            );
}

 /*****************************************************************************/
//  Description : Display the standing rabbit of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void DisplayStandRabbitOfSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
    IMGREF_SIZE_T lcd_size = {0};

    //SCI_TRACE_LOW:"DisplayStandRabbitOfSweetTree"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2002_112_2_18_2_32_32_51,(uint8*)"");
        
    lcd_size.w = s_lcd_width;
    lcd_size.h = s_lcd_height;   
  
    //blend butterfly    
    offset.x = s_rabbit_stand_coordinate.x;
    offset.y = s_rabbit_stand_coordinate.y;
    layer_size.w = s_rabbit_stand_pic_width;
    layer_size.h = s_rabbit_stand_pic_height;
    S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
        offset,s_rabbit_stand2_buf_ptr,rgb565_buf_ptr
        );
}
 /*****************************************************************************/
//  Description : Display the butterfly of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_DisplayButterflyofSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
	IMGREF_SIZE_T lcd_size = {0};
    GUI_RECT_T lcd_rect = MMITHEME_GetFullScreenRect();

    //SCI_TRACE_LOW:"MMIIDLE_DisplayButterflyofSweetTree"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2028_112_2_18_2_32_32_52,(uint8*)"");
     
    lcd_size.w = lcd_rect.right - lcd_rect.left + 1;
    lcd_size.h = lcd_rect.bottom - lcd_rect.top+1;   
  
                 //blend butterfly    
    offset.x = s_butterfly_sway_coordinate.x;
    offset.y = s_butterfly_sway_coordinate.y;
    layer_size.w = s_butterfly_pic_width;
    layer_size.h = s_butterfly_pic_height;
    S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
            offset,s_butterfly3_buf_ptr,rgb565_buf_ptr
            );
}


/*****************************************************************************/
//  Description : play the bird nest animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlayBirdTouchedAnimofSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
IMGREF_SIZE_T lcd_size = {0};
    
    //SCI_TRACE_LOW:"MMIIDLE_PlayBirdTouchedAnimofSweetTree"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2054_112_2_18_2_32_32_53,(uint8*)"");
    
    lcd_size.w = s_lcd_width;//lcd_rect.right - lcd_rect.left + 1;
    lcd_size.h = s_lcd_height;//lcd_rect.bottom - lcd_rect.top+1;   
    offset.x = s_bird_nest_coordinate.x;
    offset.y = s_bird_nest_coordinate.y;
    layer_size.w = s_nest_pic_width;
    layer_size.h = s_nest_pic_height;

    switch(s_sweettree_wallpaper_param.bird_frame_index)
    {
        case 0:
        case 1:
        case 2:
        case 12:   
        case 13:       
        case 14:
            S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_nest2_buf_ptr, rgb565_buf_ptr);
        break;
        case 3:
        case 4:
        case 5:
        case 9:
        case 10: 
        case 11:       
           S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_nest3_buf_ptr, rgb565_buf_ptr);
        break;
        case 6:
        case 7:
        case 8:
            S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_nest4_buf_ptr, rgb565_buf_ptr);
        break;
       default:
            S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_nest1_buf_ptr, rgb565_buf_ptr);
        break;
        }
        
        s_sweettree_wallpaper_param.bird_frame_index++;
        if( s_sweettree_wallpaper_param.bird_frame_index >= BIRD_TOUCH_ANIM_FRAME_NUM)
        {
            s_bird_old_tick = SCI_GetTickCount();
            s_sweettree_wallpaper_param.bird_frame_index  = 0;
            s_sweettree_wallpaper_param.bird_state = MMI_SWEETTREE_BIRD_IN_CAVE;
        }
}

/*****************************************************************************/
//  Description : play butterfly sway animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlayBirdSwayAnimofSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
    uint16 frame_tag = 0;
    IMGREF_SIZE_T lcd_size = {0};
    
    //SCI_TRACE_LOW:"MMIIDLE_PlayBirdSwayAnimofSweetTree"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2115_112_2_18_2_32_32_54,(uint8*)"");
    
    lcd_size.w = s_lcd_width;//lcd_rect.right - lcd_rect.left + 1;
    lcd_size.h = s_lcd_height;//lcd_rect.bottom - lcd_rect.top+1;   
    offset.x = s_bird_nest_coordinate.x;
    offset.y = s_bird_nest_coordinate.y;
    layer_size.w = s_nest_pic_width;
    layer_size.h = s_nest_pic_height;
        frame_tag = s_sweettree_wallpaper_param.bird_frame_index%2;
        if(0== frame_tag)
        {
            S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_nest_left_buf_ptr, rgb565_buf_ptr);
            }
        else // if(2==frame_tag)
            {
            S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_nest1_buf_ptr, rgb565_buf_ptr);
            }
        s_sweettree_wallpaper_param.bird_frame_index++;
        if( s_sweettree_wallpaper_param.bird_frame_index >= s_bird_sway_anim_frame_num)
        {
            s_bird_old_tick = SCI_GetTickCount();
            s_sweettree_wallpaper_param.bird_frame_index  = 0;
            s_sweettree_wallpaper_param.bird_state = MMI_SWEETTREE_BIRD_SWAYED;
        }
}
/*****************************************************************************/
//  Description : play bird flyaway animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void  MMIIDLE_PlayBirdGoNestAnimofSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
    IMGREF_SIZE_T lcd_size = {0};

    uint16 index = s_sweettree_wallpaper_param.bird_frame_index/3;
    
    //SCI_TRACE_LOW:"MMIIDLE_PlayBirdGoNestAnimofSweetTree"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2154_112_2_18_2_32_32_55,(uint8*)"");
    
    lcd_size.w = s_lcd_width;
    lcd_size.h = s_lcd_height;   
  
    offset.x = s_bird_go_nest_coordinate[index].x;
    offset.y = s_bird_go_nest_coordinate[index].y;
    layer_size.w = s_nest_pic_width;
    layer_size.h = s_nest_pic_height;
	
    if(0== index)
    {
        S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
            offset,s_bird_fly3_buf_ptr,rgb565_buf_ptr);
    }
    else  if(1== index)
    {
        S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_nest4_buf_ptr,rgb565_buf_ptr);
    }
    else
    {
         S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_nest3_buf_ptr,rgb565_buf_ptr);
    }
    s_sweettree_wallpaper_param.bird_frame_index++;
    if( s_sweettree_wallpaper_param.bird_frame_index >= (3*BIRD_GO_NEST_FRAME_NUM))
    {
        s_bird_old_tick = SCI_GetTickCount();
        s_sweettree_wallpaper_param.bird_frame_index = 0;
        s_sweettree_wallpaper_param.bird_state = MMI_SWEETTREE_BIRD_IN_CAVE;
    }
}
/*****************************************************************************/
//  Description : play bird flyaway animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void  MMIIDLE_PlayBirdFlyAwayAnimofSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
    uint16 frame_tag = 0;
    IMGREF_SIZE_T lcd_size = {0};
    
    //SCI_TRACE_LOW:"MMIIDLE_PlayBirdFlyAwayAnimofSweetTree"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2198_112_2_18_2_32_32_56,(uint8*)"");
    
    lcd_size.w = s_lcd_width;
    lcd_size.h = s_lcd_height;   
  
    offset.x = s_bird_flyaway_coordinate[s_sweettree_wallpaper_param.bird_frame_index].x;
    offset.y = s_bird_flyaway_coordinate[s_sweettree_wallpaper_param.bird_frame_index].y;
    layer_size.w = s_nest_pic_width;
    layer_size.h = s_nest_pic_height;
	
    frame_tag = s_sweettree_wallpaper_param.bird_frame_index%2;
    if(0== frame_tag)
    {
        S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
            offset,s_bird_fly1_buf_ptr,rgb565_buf_ptr);
    }
    else//  if(2==frame_tag)
    {
        S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_bird_fly2_buf_ptr,rgb565_buf_ptr);
    }
    s_sweettree_wallpaper_param.bird_frame_index++;
    if( s_sweettree_wallpaper_param.bird_frame_index >= BIRD_FLY_ANIM_FRAME_NUM)
    {
        s_bird_old_tick = SCI_GetTickCount();
            s_sweettree_wallpaper_param.bird_frame_index = 0;
        s_sweettree_wallpaper_param.bird_state = MMI_SWEETTREE_BIRD_FLYAWAY;
    }
}
/*****************************************************************************/
//  Description : play bird flyaback animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void  MMIIDLE_PlayBirdFlyBackAnimofSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
    uint16 frame_tag = 0;
    IMGREF_SIZE_T lcd_size = {0};
    
    //SCI_TRACE_LOW:"MMIIDLE_PlayBirdFlyBackAnimofSweetTree"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2238_112_2_18_2_32_32_57,(uint8*)"");
    
    lcd_size.w = s_lcd_width;
    lcd_size.h = s_lcd_height;   
  
    offset.x = s_bird_flyback_coordinate[s_sweettree_wallpaper_param.bird_frame_index].x;
    offset.y = s_bird_flyback_coordinate[s_sweettree_wallpaper_param.bird_frame_index].y;
    layer_size.w = s_nest_pic_width;
    layer_size.h = s_nest_pic_height;
	
    frame_tag = s_sweettree_wallpaper_param.bird_frame_index%2;
    if(0== frame_tag)
    {
        S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
            offset,s_bird_fly1_buf_ptr,rgb565_buf_ptr);
    }
    else//  if(2==frame_tag)
    {
        S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_bird_fly2_buf_ptr,rgb565_buf_ptr);
    }
    s_sweettree_wallpaper_param.bird_frame_index++;
    if( s_sweettree_wallpaper_param.bird_frame_index >= BIRD_FLY_ANIM_FRAME_NUM)
    {
        s_bird_old_tick = SCI_GetTickCount();
        s_sweettree_wallpaper_param.bird_frame_index = 0;
        s_sweettree_wallpaper_param.bird_state = MMI_SWEETTREE_BIRD_FLYBACK;
    }
}
/*****************************************************************************/
//  Description : play butterfly sway animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlayButterflySwayAnimofSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
    uint16 frame_tag = 0;
    IMGREF_SIZE_T lcd_size = {0};
    
    //SCI_TRACE_LOW:"MMIIDLE_PlayButterflySwayAnimofSweetTree"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2278_112_2_18_2_32_32_58,(uint8*)"");
    
    lcd_size.w = s_lcd_width;//lcd_rect.right - lcd_rect.left + 1;
    lcd_size.h = s_lcd_height;//lcd_rect.bottom - lcd_rect.top+1;   
    offset.x = s_butterfly_sway_coordinate.x;
    offset.y = s_butterfly_sway_coordinate.y;
    layer_size.w = s_butterfly_pic_width;
    layer_size.h = s_butterfly_pic_height;
    frame_tag = s_sweettree_wallpaper_param.butterfly_frame_index%2;
    if(0== frame_tag)
    {
        S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_butterfly4_buf_ptr, rgb565_buf_ptr);
    }
    else // if(2==frame_tag)
    {
        S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_butterfly3_buf_ptr, rgb565_buf_ptr);
    }
    s_sweettree_wallpaper_param.butterfly_frame_index++;
    if( s_sweettree_wallpaper_param.butterfly_frame_index >= s_butterfly_sway_anim_frame_num)
    {
        s_sweettree_wallpaper_param.butterfly_frame_index  = 0;
        s_sweettree_wallpaper_param.butterfly_state = MMI_SWEETTREE_BUTTERFLY_SWAYED;
    }
}

/*****************************************************************************/
//  Description : play butterfly flyaway animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void  MMIIDLE_PlayButterflyFlyAwayAnimofSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
    uint16 frame_tag = 0;
    IMGREF_SIZE_T lcd_size = {0};
    

    //SCI_TRACE_LOW:"MMIIDLE_PlayButterflyFlyAwayAnimofSweetTree"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2317_112_2_18_2_32_32_59,(uint8*)"");
    
    lcd_size.w = s_lcd_width;
    lcd_size.h = s_lcd_height;  
  
    offset.x = s_butterfly_flyaway_coordinate[s_sweettree_wallpaper_param.butterfly_frame_index].x;
    offset.y = s_butterfly_flyaway_coordinate[s_sweettree_wallpaper_param.butterfly_frame_index].y;
    layer_size.w = s_butterfly_pic_width;
    layer_size.h = s_butterfly_pic_height;
	
    frame_tag = s_sweettree_wallpaper_param.butterfly_frame_index%2;
    if(0== frame_tag)
    {
        S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
            offset,s_butterfly1_buf_ptr,rgb565_buf_ptr);
    }
    else//  if(2==frame_tag)
    {
        S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_butterfly2_buf_ptr,rgb565_buf_ptr);
    }
    s_sweettree_wallpaper_param.butterfly_frame_index++;
    if( s_sweettree_wallpaper_param.butterfly_frame_index >= BUTTERFLY_FLY_ANIM_FRAME_NUM)
    {
        s_sweettree_old_tick = SCI_GetTickCount();
        s_sweettree_wallpaper_param.butterfly_frame_index = 0;
        s_sweettree_wallpaper_param.butterfly_state = MMI_SWEETTREE_BUTTERFLY_FLYAWAY;
    }
}


/*****************************************************************************/
//  Description : play the squirrel cave animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlaySquirrelCaveAnimofSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
    IMGREF_SIZE_T lcd_size = {0};
    
    //SCI_TRACE_LOW:"MMIIDLE_PlaySquirrelCaveAnimofSweetTree"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2358_112_2_18_2_32_32_60,(uint8*)"");
    
    lcd_size.w = s_lcd_width;
    lcd_size.h = s_lcd_height;   
    offset.x = s_squirrel_cave_coordinate.x;
    offset.y = s_squirrel_cave_coordinate.y;
    layer_size.w = s_nest_pic_width;
    layer_size.h = s_nest_pic_height;

   s_sweettree_wallpaper_param.squirrel_frame_index++;
        switch(s_sweettree_wallpaper_param.squirrel_frame_index)
        {
            case 1:
            case 2:
            case 10:
            case 11:    
            S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_squirrel_head1_buf_ptr,rgb565_buf_ptr);
            break;
            case 3:
            case 4:
            case 8:
            case 9:    
                S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                    offset,s_squirrel_head2_buf_ptr,rgb565_buf_ptr);
            break;
   
            case 5:
            case 6:
            case 7:    
                S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                    offset,s_squirrel_head3_buf_ptr,rgb565_buf_ptr);
            break;
            default:    
                S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                    offset,s_squirrel_tail3_buf_ptr,rgb565_buf_ptr);
            break;
            
            }
        
        if( s_sweettree_wallpaper_param.squirrel_frame_index >= SQUIRREL_TOUCH_ANIM_FRAME_NUM)
        {
            s_squirrel_old_tick = SCI_GetTickCount();
            s_sweettree_wallpaper_param.squirrel_frame_index  = 0;
            s_sweettree_wallpaper_param.squirrel_state = MMI_SWEETTREE_SQUIRREL_IN_CAVE;
        }
}
/*****************************************************************************/
//  Description : play squirrel sway animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlaySquirrelSwayAnimofSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
    uint16 frame_tag = 0;
    IMGREF_SIZE_T lcd_size = {0};

    //SCI_TRACE_LOW:"MMIIDLE_PlaySquirrelSwayAnimofSweetTree"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2416_112_2_18_2_32_33_61,(uint8*)"");
    
    lcd_size.w = s_lcd_width;//lcd_rect.right - lcd_rect.left + 1;
    lcd_size.h = s_lcd_height;//lcd_rect.bottom - lcd_rect.top+1;   
    offset.x = s_squirrel_cave_coordinate.x;
    offset.y = s_squirrel_cave_coordinate.y;
    layer_size.w = s_nest_pic_width;
    layer_size.h = s_nest_pic_height;
    
    frame_tag = s_sweettree_wallpaper_param.squirrel_frame_index%2;
    if(0==frame_tag)
    {
        S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_squirrel_tail2_buf_ptr,rgb565_buf_ptr);
    }
    else
    {
        S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_squirrel_tail3_buf_ptr, rgb565_buf_ptr);
    }

        s_sweettree_wallpaper_param.squirrel_frame_index++;
   
        if( s_sweettree_wallpaper_param.squirrel_frame_index >= s_squirrel_sway_anim_frame_num)
        {
            s_squirrel_old_tick = SCI_GetTickCount();
            s_sweettree_wallpaper_param.squirrel_frame_index  = 0;
            s_sweettree_wallpaper_param.squirrel_state = MMI_SWEETTREE_SQUIRREL_SWAYED;
        }
}
/*****************************************************************************/
//  Description : play squirrel runaway animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlaySquirrelRunAwayAnimofSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
    IMGREF_SIZE_T lcd_size = {0};

    //SCI_TRACE_LOW:"MMIIDLE_PlaySquirrelRunAwayAnimofSweetTree"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2456_112_2_18_2_32_33_62,(uint8*)"");
    
    lcd_size.w = s_lcd_width;//lcd_rect.right - lcd_rect.left + 1;
    lcd_size.h = s_lcd_height;//lcd_rect.bottom - lcd_rect.top+1;   
    offset.x = s_squirrel_runaway_coordinate[s_sweettree_wallpaper_param.squirrel_frame_index].x;
    offset.y = s_squirrel_runaway_coordinate[s_sweettree_wallpaper_param.squirrel_frame_index].y;
    layer_size.w = s_nest_pic_width;
    layer_size.h = s_nest_pic_height;
    
   s_sweettree_wallpaper_param.squirrel_frame_index++;
        switch(s_sweettree_wallpaper_param.squirrel_frame_index)
        {
            case 1:
            case 4:
            case 7:    
            S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_squirrel_run1_buf_ptr,rgb565_buf_ptr);
            break;
            case 2:
            case 5:
            case 8:
                S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_squirrel_run2_buf_ptr,rgb565_buf_ptr);
            break;
            case 3:
            case 6:
            case 9:
                S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_squirrel_run3_buf_ptr, rgb565_buf_ptr);
            break;
       
            default:
                break;
            }
        
        if( s_sweettree_wallpaper_param.squirrel_frame_index >= SQUIRREL_RUN_ANIM_FRAME_NUM)
        {
            s_squirrel_old_tick = SCI_GetTickCount();
            s_sweettree_wallpaper_param.squirrel_frame_index  = 0;
            s_sweettree_wallpaper_param.squirrel_state = MMI_SWEETTREE_SQUIRREL_RUNAWAY;
        }
}


/*****************************************************************************/
//  Description : play squirrel runback animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlaySquirrelRunBackAnimofSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
    IMGREF_SIZE_T lcd_size = {0};

    //SCI_TRACE_LOW:"MMIIDLE_PlaySquirrelRunBackAnimofSweetTree"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2510_112_2_18_2_32_33_63,(uint8*)"");
    
    lcd_size.w = s_lcd_width;//lcd_rect.right - lcd_rect.left + 1;
    lcd_size.h = s_lcd_height;//lcd_rect.bottom - lcd_rect.top+1;   
    offset.x = s_squirrel_runback_coordinate[s_sweettree_wallpaper_param.squirrel_frame_index].x;
    offset.y = s_squirrel_runback_coordinate[s_sweettree_wallpaper_param.squirrel_frame_index].y;
    layer_size.w = s_nest_pic_width;
    layer_size.h = s_nest_pic_height;
 
        switch(s_sweettree_wallpaper_param.squirrel_frame_index)
        {
            case 0:
            case 1:
            case 4:
            S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_squirrel_run1_buf_ptr,rgb565_buf_ptr);
            break;
            case 2:
            case 5:
            case 7:
                S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_squirrel_run2_buf_ptr, rgb565_buf_ptr);
            break;
            case 3:
            case 6:
            case 8:
                S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_squirrel_run3_buf_ptr, rgb565_buf_ptr);
            break;
        default:
                S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_squirrel_tail3_buf_ptr, rgb565_buf_ptr);
                break;

            }
        s_sweettree_wallpaper_param.squirrel_frame_index++;
        if( s_sweettree_wallpaper_param.squirrel_frame_index >= SQUIRREL_RUN_ANIM_FRAME_NUM)
        {
            s_squirrel_old_tick = SCI_GetTickCount();
             s_sweettree_wallpaper_param.squirrel_frame_index  = 0;
            s_sweettree_wallpaper_param.squirrel_state = MMI_SWEETTREE_SQUIRREL_IN_CAVE;
        }
}

/*****************************************************************************/
//  Description : play butterfly flyback animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlayButterflyFlyBackAnimofSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
    uint16 frame_tag = 0;
    IMGREF_SIZE_T lcd_size = {0};

    //SCI_TRACE_LOW:"MMIIDLE_PlayButterflyFlyBackAnimofSweetTree"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2565_112_2_18_2_32_33_64,(uint8*)"");
    
    lcd_size.w = s_lcd_width;
    lcd_size.h = s_lcd_height;   
  
    offset.x = s_butterfly_flyback_coordinate[s_sweettree_wallpaper_param.butterfly_frame_index].x;
    offset.y = s_butterfly_flyback_coordinate[s_sweettree_wallpaper_param.butterfly_frame_index].y;
    layer_size.w = s_butterfly_pic_width;
    layer_size.h = s_butterfly_pic_height;

    frame_tag = s_sweettree_wallpaper_param.butterfly_frame_index%2;
    if(0== frame_tag)
    {
        S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,offset,s_butterfly1_buf_ptr,rgb565_buf_ptr);
    }
    else//  if(2==frame_tag)
    {
        S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_butterfly2_buf_ptr,rgb565_buf_ptr);
    }
    s_sweettree_wallpaper_param.butterfly_frame_index++;
    if( s_sweettree_wallpaper_param.butterfly_frame_index >= BUTTERFLY_FLY_ANIM_FRAME_NUM)
    {
        s_sweettree_old_tick = SCI_GetTickCount();
        s_sweettree_wallpaper_param.butterfly_frame_index = 0;
        s_sweettree_wallpaper_param.butterfly_state = MMI_SWEETTREE_BUTTERFLY_FLYBACK;
    }
}

/*****************************************************************************/
//  Description : play rabbit running away animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlayRabbitRunningAwayAnimofSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
    uint16 frame_tag = 0;      
    IMGREF_SIZE_T lcd_size = {0};

    //SCI_TRACE_LOW:"MMIIDLE_PlayRabbitRunningAwayAnimofSweetTree"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2605_112_2_18_2_32_33_65,(uint8*)"");
        
    lcd_size.w = s_lcd_width;
    lcd_size.h = s_lcd_height;   
  
    offset.x = s_rabbit_runaway_coordinate[s_sweettree_wallpaper_param.rabbit_frame_index].x;
    offset.y = s_rabbit_runaway_coordinate[s_sweettree_wallpaper_param.rabbit_frame_index].y;
    layer_size.w = s_rabbit_run_pic_width;
    layer_size.h = s_rabbit_run_pic_height;
    frame_tag = s_sweettree_wallpaper_param.rabbit_frame_index%2;
    if(0 == frame_tag)
    {
       S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_rabbit_run2_buf_ptr,rgb565_buf_ptr);
    }
    else
    {
    S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_rabbit_run3_buf_ptr,rgb565_buf_ptr);
    } 
      
    s_sweettree_wallpaper_param.rabbit_frame_index++;
    if( s_sweettree_wallpaper_param.rabbit_frame_index >= RABBIT_RUNAWAY_FRAME_NUM)
    {
        s_sweettree_old_tick = SCI_GetTickCount();
        s_sweettree_wallpaper_param.rabbit_frame_index = 0;
        s_sweettree_wallpaper_param.rabbit_state = MMI_SWEETTREE_RABBIT_RUNAWAY;
    }
}

/*****************************************************************************/
//  Description : play rabbit running back animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlayRabbitRunningBackAnimofSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
    uint16 frame_tag = 0;      
	IMGREF_SIZE_T lcd_size = {0};

    //SCI_TRACE_LOW:"MMIIDLE_PlayRabbitRunningBackAnimofSweetTree"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2646_112_2_18_2_32_33_66,(uint8*)"");
    
    lcd_size.w = s_lcd_width;
    lcd_size.h = s_lcd_height;   
  
    offset.x = s_rabbit_runback_coordinate[s_sweettree_wallpaper_param.rabbit_frame_index].x;
    offset.y = s_rabbit_runback_coordinate[s_sweettree_wallpaper_param.rabbit_frame_index].y;
    layer_size.w = s_rabbit_run_pic_width;
    layer_size.h = s_rabbit_run_pic_height;
    frame_tag = s_sweettree_wallpaper_param.rabbit_frame_index%2;
    if(0 == frame_tag)
    {
       S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_rabbit_run2_buf_ptr,rgb565_buf_ptr);
    }
    else
    {
    S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_rabbit_run3_buf_ptr,rgb565_buf_ptr);
    } 
      
    s_sweettree_wallpaper_param.rabbit_frame_index++;
    if( s_sweettree_wallpaper_param.rabbit_frame_index >= RABBIT_RUNAWAY_FRAME_NUM)
    {
        s_sweettree_old_tick = SCI_GetTickCount();
        s_sweettree_wallpaper_param.rabbit_frame_index = 0;
        s_sweettree_wallpaper_param.rabbit_state = MMI_SWEETTREE_RABBIT_IDLE;
    }
}

/*****************************************************************************/
//  Description : play rabbit touched animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlayRabbitTouchedAnimofSweetTree()
{
    IMGREF_SIZE_T layer_size = {0};
    FIX16_POINT_T offset = {0};
    IMGREF_SIZE_T lcd_size = {0};

    //SCI_TRACE_LOW:"MMIIDLE_PlayRabbitTouchedAnimofSweetTree"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2686_112_2_18_2_32_33_67,(uint8*)"");
    
    lcd_size.w = s_lcd_width;
    lcd_size.h = s_lcd_height;   
  
    offset.x = s_rabbit_stand_coordinate.x;
    offset.y = s_rabbit_stand_coordinate.y;
    layer_size.w = s_rabbit_stand_pic_width;
    layer_size.h = s_rabbit_stand_pic_height;

    switch(s_sweettree_wallpaper_param.rabbit_frame_index)
    {
        case 0:
        case 1:
        S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_rabbit_stand3_buf_ptr,rgb565_buf_ptr);
        break;
        case 2:
        case 3:
        S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_rabbit_stand4_buf_ptr,rgb565_buf_ptr);
        break;
        case 4:
        case 5:
        S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_rabbit_stand3_buf_ptr,rgb565_buf_ptr);
        break;
        case 6:
        case 7:
        S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_rabbit_stand3_buf_ptr,rgb565_buf_ptr);
        break;
        default:
           S2D_Blending(lcd_size,rgb565_buf_ptr,layer_size,
                offset,s_rabbit_stand2_buf_ptr,rgb565_buf_ptr);
        break;
    }
       
    s_sweettree_wallpaper_param.rabbit_frame_index++;
    if( s_sweettree_wallpaper_param.rabbit_frame_index >= RABBIT_RUNAWAY_FRAME_NUM)
    {
        s_sweettree_old_tick = SCI_GetTickCount();
        s_sweettree_wallpaper_param.rabbit_frame_index = 0;
        s_sweettree_wallpaper_param.rabbit_state = MMI_SWEETTREE_RABBIT_IDLE;
    }
}
/*****************************************************************************/
//  Description : play the girl sway leg animation of SweetTree 
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_PlayGirlSwayLegAnim()
{
    IMGREF_SIZE_T lcd_size = {0};
    
    IMGREF_SIZE_T layer_size = {0};

    //SCI_TRACE_LOW:"MMIIDLE_PlayGirlSwayLegAnim"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2742_112_2_18_2_32_33_68,(uint8*)"");
    
    lcd_size.w = s_lcd_width;
    lcd_size.h = s_lcd_height;
    layer_size.w = s_girl_pic_width;
    layer_size.h = s_girl_dis_height;
    switch(s_sweettree_wallpaper_param.girl_leg_frame_index)
    {
        case 1:
        case 7:   
        MMIIDLE_GirlSwing(rgb565_buf_ptr,girl2_buf_ptr, girl_rotation_buf_ptr,
                lcd_size, layer_size,s_sweettree_wallpaper_param.girl_swing_frame_index,
                GIRL_SWING_TOTAL_FRAM_NUM,s_sweettree_wallpaper_param.strong_level
                    );
        break;

        case 2:
        case 6:   
        MMIIDLE_GirlSwing(rgb565_buf_ptr,girl3_buf_ptr, girl_rotation_buf_ptr,
                lcd_size, layer_size,s_sweettree_wallpaper_param.girl_swing_frame_index,
                GIRL_SWING_TOTAL_FRAM_NUM,s_sweettree_wallpaper_param.strong_level
                    );
        break;
        case 3:
        case 5:   
        MMIIDLE_GirlSwing(rgb565_buf_ptr,girl4_buf_ptr, girl_rotation_buf_ptr,
                lcd_size, layer_size,s_sweettree_wallpaper_param.girl_swing_frame_index,
                GIRL_SWING_TOTAL_FRAM_NUM,s_sweettree_wallpaper_param.strong_level
                    );
        break;
        case 4:
            MMIIDLE_GirlSwing(rgb565_buf_ptr,girl1_buf_ptr, girl_rotation_buf_ptr,
                lcd_size, layer_size,s_sweettree_wallpaper_param.girl_swing_frame_index,
                GIRL_SWING_TOTAL_FRAM_NUM,s_sweettree_wallpaper_param.strong_level
                    );
        break;
        default:   
            MMIIDLE_GirlSwing(rgb565_buf_ptr,girl1_buf_ptr, girl_rotation_buf_ptr,
                lcd_size, layer_size,s_sweettree_wallpaper_param.girl_swing_frame_index,
                GIRL_SWING_TOTAL_FRAM_NUM,s_sweettree_wallpaper_param.strong_level
                    );
        break;
    }
    s_sweettree_wallpaper_param.girl_leg_frame_index++;
    if(s_sweettree_wallpaper_param.girl_leg_frame_index >= GIRL_LEG_TOTAL_FRAM_NUM)
    {
        s_sweettree_wallpaper_param.should_sway_leg = FALSE;
        s_sweettree_wallpaper_param.girl_leg_frame_index = 1;
    }
}
/*****************************************************************************/
//  Description : 甜蜜树算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SweetTreeCalculate(LW_CAL_PARAM *lw_cal_param)
{
 
    IMGREF_SIZE_T lcd_size = {0};
    uint32 tick_p = 0;
    uint32 new_tick = 0;
    
    IMGREF_SIZE_T layer_size = {0};

    //SCI_TRACE_LOW:"SweetTreeCalculate"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2805_112_2_18_2_32_33_69,(uint8*)"");
    
    lcd_size.w = s_lcd_width;//lcd_rect.right - lcd_rect.left + 1;
    lcd_size.h = s_lcd_height;//lcd_rect.bottom - lcd_rect.top+1;   

   rgb565_buf_ptr = lw_cal_param->dest_buf_info.buf_ptr;
    SCI_MEMCPY(rgb565_buf_ptr,clean_bg_buf_ptr,lcd_size.w*lcd_size.h*2);
        
    new_tick = SCI_GetTickCount();
    tick_p = new_tick - s_bird_old_tick;
    if(tick_p>17000)
    {
        s_bird_old_tick = new_tick;
        MMIIDLE_ChangeBirdState();
    }
    tick_p = new_tick - s_squirrel_old_tick;
    if(tick_p>20000)
    {
        s_squirrel_old_tick = new_tick;
        MMIIDLE_ChangeSquirrelState();
    }
    tick_p = new_tick - s_sweettree_old_tick;
        if(tick_p >=14000)
        {
            s_sweettree_old_tick = new_tick;
            
            MMIIDLE_ChangeRabbitState();
            MMIIDLE_ChangeButterflyState();
        }

        
        s_sweettree_wallpaper_param.girl_swing_frame_index++;
        if (s_sweettree_wallpaper_param.girl_swing_frame_index >= GIRL_SWING_TOTAL_FRAM_NUM )
        {
            s_sweettree_wallpaper_param.girl_swing_frame_index = 1;
            if(s_sweettree_wallpaper_param.should_add_stronglevel)
            {
                
                MMIIDLE_ChangeGirlSwayStrongeLevel(TRUE);
                s_sweettree_wallpaper_param.should_add_stronglevel = FALSE;
                s_sweettree_wallpaper_param.girl_strong_reduce_cycel = 0;
            }
            else 
            {
                s_sweettree_wallpaper_param.girl_strong_reduce_cycel++;
                if(s_sweettree_wallpaper_param.girl_strong_reduce_cycel>=2)
                {
                    MMIIDLE_ChangeGirlSwayStrongeLevel(FALSE);
                    s_sweettree_wallpaper_param.girl_strong_reduce_cycel = 0;
                 }
            }
        }
         
        switch(s_sweettree_wallpaper_param.bird_state)
        {
            case MMI_SWEETTREE_BIRD_IN_CAVE:
            case MMI_SWEETTREE_BIRD_SWAYED:
            case MMI_SWEETTREE_BIRD_FLYAWAY:
                 MMIIDLE_DisplayBirdNestofSweetTree();
                break;
            case MMI_SWEETTREE_BIRD_SWAYING:
                MMIIDLE_PlayBirdSwayAnimofSweetTree();
                break;
            case MMI_SWEETTREE_BIRD_THOUCHED:
                MMIIDLE_PlayBirdTouchedAnimofSweetTree();
                break;
            case MMI_SWEETTREE_BIRD_FLYING_AWAY:
                MMIIDLE_PlayBirdFlyAwayAnimofSweetTree();
                MMIIDLE_DisplayBirdNestofSweetTree();
                break;
            case MMI_SWEETTREE_BIRD_FLYING_BACK:
                MMIIDLE_PlayBirdFlyBackAnimofSweetTree();
                MMIIDLE_DisplayBirdNestofSweetTree();
                break;
            case MMI_SWEETTREE_BIRD_FLYBACK:
                case MMI_SWEETTREE_BIRD_TURN_RIGHT:
                MMIIDLE_DisplayBirdofSweetTree();
                MMIIDLE_DisplayBirdNestofSweetTree();
                break;
                 case MMI_SWEETTREE_BIRD_GO_NEST:
                MMIIDLE_DisplayBirdNestofSweetTree();
                MMIIDLE_PlayBirdGoNestAnimofSweetTree();
                break;
 
                default:
                    break;
        }

        switch(s_sweettree_wallpaper_param.butterfly_state)
        {
            case MMI_SWEETTREE_BUTTERFLY_IDLE:
            case MMI_SWEETTREE_BUTTERFLY_SWAYED:
                case MMI_SWEETTREE_BUTTERFLY_FLYBACK:
                MMIIDLE_DisplayButterflyofSweetTree();
                    break;

            case MMI_SWEETTREE_BUTTERFLY_SWAYING:
            MMIIDLE_PlayButterflySwayAnimofSweetTree();
            break;
            case MMI_SWEETTREE_BUTTERFLY_FLYING_AWAY:
                MMIIDLE_PlayButterflyFlyAwayAnimofSweetTree();
            break;
            case MMI_SWEETTREE_BUTTERFLY_FLYING_BACK:
                MMIIDLE_PlayButterflyFlyBackAnimofSweetTree();
            break;
            default:
                    break;
        }

        switch(s_sweettree_wallpaper_param.squirrel_state)
        {
            case MMI_SWEETTREE_SQUIRREL_IN_CAVE:
            case MMI_SWEETTREE_SQUIRREL_SWAYED:  
            MMIIDLE_DisplaySquirrelofSweetTree();
            break;
            case MMI_SWEETTREE_SQUIRREL_TOUCHED:
                MMIIDLE_PlaySquirrelCaveAnimofSweetTree();
                break;

            case MMI_SWEETTREE_SQUIRREL_SWAYING:
                MMIIDLE_PlaySquirrelSwayAnimofSweetTree();
                break;
             case MMI_SWEETTREE_SQUIRREL_RUNING_AWAY:
                MMIIDLE_PlaySquirrelRunAwayAnimofSweetTree();
                break;
             case MMI_SWEETTREE_SQUIRREL_RUNING_BACK:
                MMIIDLE_PlaySquirrelRunBackAnimofSweetTree();
				break;
             default:
                        break;
            }

        switch(s_sweettree_wallpaper_param.rabbit_state)
        {
            case MMI_SWEETTREE_RABBIT_IDLE:
            DisplayStandRabbitOfSweetTree();
            break;
            case MMI_SWEETTREE_RABBIT_TOUCHED:
                MMIIDLE_PlayRabbitTouchedAnimofSweetTree();
                break;
            case MMI_SWEETTREE_RABBIT_RUNING_AWAY:
                MMIIDLE_PlayRabbitRunningAwayAnimofSweetTree();
                break;
            case MMI_SWEETTREE_RABBIT_RUNING_BACK:
                MMIIDLE_PlayRabbitRunningBackAnimofSweetTree();
                break;  
                default:
                    break;
        }

        if(s_sweettree_wallpaper_param.should_sway_leg)
        {
            MMIIDLE_PlayGirlSwayLegAnim();
        }
        else
        {
            layer_size.w = s_girl_pic_width;
            layer_size.h = s_girl_dis_height;
        
            MMIIDLE_GirlSwing(rgb565_buf_ptr,girl1_buf_ptr, girl_rotation_buf_ptr,
                lcd_size, layer_size,s_sweettree_wallpaper_param.girl_swing_frame_index,
                GIRL_SWING_TOTAL_FRAM_NUM,s_sweettree_wallpaper_param.strong_level
                    );
        }
  //      SCI_MEMCPY(target_buf_ptr,rgb565_buf_ptr,lcd_size.w*lcd_size.h*2);
}

/*****************************************************************************/
//  Description : 甜蜜树互动判断
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SweetTreeHandleTpPressDown(GUI_POINT_T tp_point)
{
    //SCI_TRACE_LOW:"SweetTreeHandleTpPressDown"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_2978_112_2_18_2_32_34_70,(uint8*)"");
    
    if(tp_point.x>=s_bird_nest_coordinate.x&&
        ((tp_point.x - s_bird_nest_coordinate.x)<s_nest_pic_width)&&
        tp_point.y>=s_bird_nest_coordinate.y&&
        ((tp_point.y - s_bird_nest_coordinate.y)<s_nest_pic_height))
        {
            if(MMI_SWEETTREE_BIRD_IN_CAVE == s_sweettree_wallpaper_param.bird_state||
                MMI_SWEETTREE_BIRD_SWAYED== s_sweettree_wallpaper_param.bird_state)
            {
                s_sweettree_wallpaper_param.bird_state = MMI_SWEETTREE_BIRD_THOUCHED;
             }
        }
    else if(tp_point.x>=s_squirrel_cave_coordinate.x&&
        ((tp_point.x - s_squirrel_cave_coordinate.x)<s_squirrel_cave_pic_width)&&
        tp_point.y>=s_squirrel_cave_coordinate.y&&
        ((tp_point.y - s_squirrel_cave_coordinate.y)<s_squirrel_cave_pic_height))
        {
            if(MMI_SWEETTREE_SQUIRREL_IN_CAVE == s_sweettree_wallpaper_param.squirrel_state||
                MMI_SWEETTREE_SQUIRREL_SWAYED == s_sweettree_wallpaper_param.squirrel_state)
            {
                s_sweettree_wallpaper_param.squirrel_state = MMI_SWEETTREE_SQUIRREL_TOUCHED;
            }
        }
     else if(tp_point.x>=s_rabbit_stand_coordinate.x&&
        ((tp_point.x - s_rabbit_stand_coordinate.x)<s_rabbit_stand_pic_width)&&
        tp_point.y>=s_rabbit_stand_coordinate.y&&
        ((tp_point.y - s_rabbit_stand_coordinate.y)<s_rabbit_stand_pic_height))
        {
            if(MMI_SWEETTREE_RABBIT_IDLE == s_sweettree_wallpaper_param.rabbit_state)
            {
                s_sweettree_wallpaper_param.rabbit_state = MMI_SWEETTREE_RABBIT_TOUCHED;
            }
        }
     else if(tp_point.x>s_girl_swing_tp_valid_rect.left&&
        tp_point.x<s_girl_swing_tp_valid_rect.right&&
        tp_point.y>s_girl_swing_tp_valid_rect.top&&
        tp_point.y<s_girl_swing_tp_valid_rect.bottom)
        {
            s_sweettree_wallpaper_param.should_sway_leg = TRUE;
            
            if(tp_point.x > s_dancing_girl_coordinate.x)
            {//when dancing to the right,add the stronglevel
                s_sweettree_wallpaper_param.should_add_stronglevel = TRUE;
            }
        }
}

/*****************************************************************************/
//  Description : Rest some Parameters Of SweetTree when Lose Focus
//  Global resource dependence : 
//  Author: xiaohua.liu
//  Note:
/*****************************************************************************/
LOCAL void MMIIDLE_RestParamOfSweetTreeLoseFocus()
{
    //SCI_TRACE_LOW:"MMIIDLE_RestParamOfSweetTreeLoseFocus"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_3032_112_2_18_2_32_34_71,(uint8*)"");
 //   s_sweettree_wallpaper_param.girl_swing_frame_index = 0;
    
    s_sweettree_wallpaper_param.bird_frame_index = 0;

    s_sweettree_wallpaper_param.squirrel_frame_index = 0;
    s_sweettree_wallpaper_param.butterfly_frame_index = 0;
    s_sweettree_wallpaper_param.rabbit_frame_index = 0;
    
    s_sweettree_wallpaper_param.squirrel_state = MMI_SWEETTREE_SQUIRREL_IN_CAVE;
    s_sweettree_wallpaper_param.bird_state = MMI_SWEETTREE_BIRD_IN_CAVE;
    s_sweettree_wallpaper_param.butterfly_state = MMI_SWEETTREE_BUTTERFLY_IDLE;
    s_sweettree_wallpaper_param.rabbit_state = MMI_SWEETTREE_RABBIT_IDLE;

}


/*****************************************************************************/
//  Description : 甜蜜树内存释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁申请的内存
/*****************************************************************************/
LOCAL void FreeAllBufferOfSweetTree()
{
    //SCI_TRACE_LOW:"FreeAllBufferOfSweetTree"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_3055_112_2_18_2_32_34_72,(uint8*)"");
    
    if(PNULL != clean_bg_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(clean_bg_buf_ptr);
    }
    

    rgb565_buf_ptr = PNULL;
  //  if(PNULL != bigtree_buf_ptr)
  //  {
  //      MMILIVEWALLPAPER_FREE( bigtree_buf_ptr);
 //   }
    
    if(PNULL != girl1_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE( girl1_buf_ptr);
    }
    if(PNULL != girl2_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE( girl2_buf_ptr);
    }
    if(PNULL != girl3_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE( girl3_buf_ptr);
    }
    if(PNULL != girl4_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE( girl4_buf_ptr);
    }
    if(PNULL !=  girl_rotation_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE( girl_rotation_buf_ptr);
    }

    
    if(PNULL != s_nest1_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_nest1_buf_ptr);
    }
    
    if(PNULL != s_nest2_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_nest2_buf_ptr);
    }
    
    if(PNULL != s_nest3_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_nest3_buf_ptr);
    }
    
    if(PNULL != s_nest4_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_nest4_buf_ptr);
    }
    
    if(PNULL != s_nest_left_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_nest_left_buf_ptr);
    }
    
    if(PNULL != s_bird_fly1_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_bird_fly1_buf_ptr);
    }
    
    if(PNULL != s_bird_fly2_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_bird_fly2_buf_ptr);
    }
    
    if(PNULL != s_bird_fly3_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_bird_fly3_buf_ptr);
    }
    if(PNULL != s_bird_fly4_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_bird_fly4_buf_ptr);
    }
    
    if(PNULL != s_squirrel_head1_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_squirrel_head1_buf_ptr);
    }

    if(PNULL != s_squirrel_head2_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_squirrel_head2_buf_ptr);
    }
    
    if(PNULL != s_squirrel_head3_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_squirrel_head3_buf_ptr);
    }
    
    if(PNULL != s_squirrel_tail1_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_squirrel_tail1_buf_ptr);
    }
    
    
    if(PNULL != s_squirrel_tail2_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_squirrel_tail2_buf_ptr);
    }
     
    if(PNULL != s_squirrel_tail3_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_squirrel_tail3_buf_ptr);
    }
     
    if(PNULL != s_squirrel_run1_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_squirrel_run1_buf_ptr);
    }
    
    if(PNULL != s_squirrel_run2_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_squirrel_run2_buf_ptr);
    }
     
    if(PNULL != s_squirrel_run3_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_squirrel_run3_buf_ptr);
    }
    
    
    if(PNULL != s_butterfly1_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_butterfly1_buf_ptr);
    }
   
    if(PNULL != s_butterfly2_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_butterfly2_buf_ptr);
    }
    
    if(PNULL != s_butterfly3_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_butterfly3_buf_ptr);
    }
    
    if(PNULL != s_butterfly4_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_butterfly4_buf_ptr);
    }

    if(PNULL != s_rabbit_stand2_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_rabbit_stand2_buf_ptr);
    }
    if(PNULL != s_rabbit_stand3_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_rabbit_stand3_buf_ptr);
    }
    if(PNULL != s_rabbit_stand4_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_rabbit_stand4_buf_ptr);
    }
    if(PNULL != s_rabbit_stand5_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_rabbit_stand5_buf_ptr);
    }


    

    if(PNULL != s_rabbit_run2_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_rabbit_run2_buf_ptr);
    }
    if(PNULL != s_rabbit_run3_buf_ptr)
    {
        MMILIVEWALLPAPER_FREE(s_rabbit_run3_buf_ptr);
    }

   
}

/*****************************************************************************/
//  Description : 甜蜜树释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁申请的内存
/*****************************************************************************/
LOCAL void SweetTreeDestruct(void)
{
    //SCI_TRACE_LOW:"SweetTreeDestruct"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_3240_112_2_18_2_32_34_73,(uint8*)"");
    
    //MMIIDLE_RestParamOfSweetTreeLoseFocus();
    FreeAllBufferOfSweetTree();
}
/*****************************************************************************/
//  Description : 甜蜜树交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN SweetTreeHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    GUI_POINT_T   point       = {0};
    
    //SCI_TRACE_LOW:"SweetTreeHandleEvent"
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMI_LIVEWALLPAPER_SWEETTREE_3253_112_2_18_2_32_34_74,(uint8*)"");
    
    switch(msg_id)
    {
    case MSG_TP_PRESS_DOWN:
        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);
        SweetTreeHandleTpPressDown(point);
        break;

    default:
        return FALSE;
    }
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
    
    return TRUE;
}
/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 SweetTreeGetType(void)
{
    return MMI_LIVE_WALLPAPER_TYPE_SWEETTREE;
}
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void SweetTreeGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr)
{
    if(thumbnail_info_ptr)
    {
        thumbnail_info_ptr->is_image_file = FALSE;
        thumbnail_info_ptr->image_id = IMAGE_WALLPAPER_SWEETTREE_THUMBNAIL;

        thumbnail_info_ptr->is_not_string = FALSE;
        thumbnail_info_ptr->text_id = TXT_SET_WALLPAPER_SWEETTREE;
        thumbnail_info_ptr->instance_mem_pool_size = BLOCK_MEM_POOL_SIZE_SWEETTREE;
    }
}
#endif

#endif /*_MMI_LIVEWALLPAPER_SWEETTREE_C_*/


/*Edit by script, ignore 1 case. Thu Apr 26 19:00:58 2012*/ //IGNORE9527
