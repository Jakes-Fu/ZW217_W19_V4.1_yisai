

/*****************************************************************************
** File Name:      mmi_livewallpaper_linear.c                                    *
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
#ifndef _MMI_LIVEWALLPAPER_LINEAR_C_
#define _MMI_LIVEWALLPAPER_LINEAR_C_

#ifdef LIVE_WALLPAPER_SUPPORT_LINEAR

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
#include "graphics_draw.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

#define LINEAR_MOVE_Z_PARAM 40
#define STAR_NUM        (30)

#define STAR_WIDTH (20)
#define HEIGHT_WIDTH_RATIO (100)
#define RADIUS_STAR_HEADER (60)

//#define STAR_BGPIC_W 480
//#define STAR_BGPIC_H 320

#define STAR_WINDOW_W MMI_MAINSCREEN_WIDTH
#define STAR_WINDOW_H MMI_MAINSCREEN_HEIGHT

//x_edge = size.w/2*(10*size.w)/(0-eye_z);
#define STAR_X_EDGE_MIN (-5*STAR_WINDOW_W*STAR_WINDOW_W/(2*STAR_WINDOW_H))
//y_edge = size.h/2*(10*size.w)/(0-eye_z);//(10*size.w) is max z
#define STAR_Y_EDGE_MIN (-5*STAR_WINDOW_W/2)

#define STAR_RANDOM_Z_MAX (10*STAR_WINDOW_W)
#define STAR_RANDOM_Z_MIN (2*STAR_WINDOW_W)

#define SUPPORT_TOUCH_NUM  8
#define DYNAMIC_STAR_DITECTION 4

//(STAR_NUM+DYNAMIC_STAR_DITECTION*SUPPORT_TOUCH_NUM)//(42)
#define STAR_ARR_NUM (STAR_NUM+DYNAMIC_STAR_DITECTION*SUPPORT_TOUCH_NUM)

#define BLOCK_MEM_POOL_SIZE_LILNER (10 * 1024)   // Set a  MEM POOL for different livewallpaper respectively;

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/

typedef struct STAR_LINE_t {
	int16 direction;			//0:+x		1:+y	2:-x	3:-y
	int16 width;				
	COLOR32 color;
	IMGREF_POINT_3D_T start_point;
	IMGREF_POINT_3D_T now_point;
}STAR_LINE_T;
/**--------------------------------------------------------------------------*
 **                         LOCAL DEFINITION                                 *
 **--------------------------------------------------------------------------*/
LOCAL int s_moved_z_axis = 0;
STAR_LINE_T g_aStar[STAR_NUM] = {0};
static STAR_LINE_T *g_dynamic_aStar[SUPPORT_TOUCH_NUM][DYNAMIC_STAR_DITECTION]= {PNULL};
int16 g_current_dynamic_star_num = 0;
LOCAL int32 g_star_x_edge = 0,g_star_y_edge = 0;
LOCAL IMGREF_POINT_3D_T TPMove_offset = {0};
LOCAL BOOLEAN is_tp_move = FALSE;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DECLARE                                 *
 **--------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                                                         */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 线性光幕初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LinerInit(void);
/*****************************************************************************/
//  Description : 线性光幕初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LinerConstruct(MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : 线性光幕准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LinerDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : 线性光幕算法
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LinerCalculate(LW_CAL_PARAM *lw_cal_param);
/*****************************************************************************/
//  Description : 线性光幕释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:销毁临时申请的变量
/*****************************************************************************/
LOCAL void LinerDestruct(void);
/*****************************************************************************/
//  Description : 线性光幕释交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LinerHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);

/*****************************************************************************/
//  Description : 获得动态壁纸类型
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL uint32 LinerGetType(void);
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LinerGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr);
/*****************************************************************************/
//  Description : 获得动态壁纸源数据宽高
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LINEARGetWidthAndHeight(uint16 *width_ptr,uint16 *height_ptr);

/*****************************************************************************/
//  Description : 获得动态壁纸选项列表
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL OPTIONLIST *LINEARGetOptionList(void);


/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
PUBLIC const MMI_LIVE_WALLPAPER_ITEM_INFO_T g_liner_Instance =
{
    LinerInit,
    LinerConstruct,
    LinerDrawBg,
    LinerCalculate,
    LinerDestruct,
    LinerHandleEvent,
    PNULL,
    LinerGetType,
    LinerGetThumbNailInfo,
    LINEARGetWidthAndHeight,
    PNULL,
    PNULL,
    LINEARGetOptionList
};
/*****************************************************************************/
//  Description : 单个像素blending
//  Global resource dependence : 
//  Author:Benny.liu
//  Note:
/*****************************************************************************/
/*_inline void pixel_blending(uint32 alpha,COLOR32 up_color,COLOR32* bg_color)
{
    register uint32 color_temp1 = 0;
    register uint32 color_temp2 = 0;
    register uint32 color_temp3 = 0;
    register uint32 color_temp4 = 0;  
     
	color_temp1 = *bg_color;
	color_temp2 = (color_temp1&0xf81f)*(32-alpha);
	color_temp1 = (color_temp1&0x07e0)*(32-alpha);
	color_temp3 = up_color;
	color_temp4 = (color_temp3&0xf81f)*alpha;
	color_temp3 = (color_temp3&0x07e0)*alpha;

	color_temp1 = ((color_temp1 + color_temp3)>>5)&0x07e0;
	color_temp2 = ((color_temp2 + color_temp4)>>5)&0xf81f;
	*bg_color = color_temp1+color_temp2;
}*/
static void init_star(STAR_LINE_T *pStar,IMGREF_SIZE_T size)
{
	static uint color = 0;

	int32 eye_z  = 0;
	int32 random_temp = 0;


	//int32 pic_bg_w = 480,pic_bg_h = 320;
	//int32 x_edge = 0, y_edge = 0;
	int32 redu = 0;

	eye_z = size.h*(-2);
	pStar->direction = (rand()%4);
	if(0 == eye_z)
		return;

	//x_edge = size.w/2*(10*size.w)/(0-eye_z);
	//y_edge = size.h/2*(10*size.w)/(0-eye_z);//(10*size.w) is max z
	
    pStar->width = STAR_WIDTH;//(((size.w+size.h)/100+(rand()%((size.w+size.h)/200)))/2*2)*20;//30;// 	((size.w+size.h)/100+(rand()%((size.w+size.h)/200)))/2*2;
	switch(pStar->direction)
	{
	case 0://+x
		pStar->start_point.z = (rand()%(STAR_RANDOM_Z_MAX-STAR_RANDOM_Z_MIN))+STAR_RANDOM_Z_MIN;
		pStar->start_point.x = STAR_X_EDGE_MIN -redu+TPMove_offset.x;
		pStar->start_point.y = rand()%(g_star_y_edge-STAR_Y_EDGE_MIN)+STAR_Y_EDGE_MIN+TPMove_offset.y;
		
		pStar->now_point.x = pStar->start_point.x;
		pStar->now_point.y = pStar->start_point.y;
		pStar->now_point.z = pStar->start_point.z;

		break;

	case 1://+y
		pStar->start_point.z = (rand()%(STAR_RANDOM_Z_MAX-STAR_RANDOM_Z_MIN))+STAR_RANDOM_Z_MIN;
		pStar->start_point.x = rand()%(g_star_x_edge-STAR_X_EDGE_MIN) + STAR_X_EDGE_MIN+TPMove_offset.x;
		pStar->start_point.y = STAR_Y_EDGE_MIN - redu+TPMove_offset.y;
		
		pStar->now_point.y = pStar->start_point.y;
		pStar->now_point.x = pStar->start_point.x;
		pStar->now_point.z = pStar->start_point.z;

		break;

	case 2://-x
		pStar->start_point.z = (rand()%(STAR_RANDOM_Z_MAX-STAR_RANDOM_Z_MIN))+STAR_RANDOM_Z_MIN;
		pStar->start_point.x = g_star_x_edge+redu+TPMove_offset.x;
		pStar->start_point.y = rand()%(g_star_y_edge-STAR_Y_EDGE_MIN) + STAR_Y_EDGE_MIN+TPMove_offset.y;
		//pStar->start_point.y = (g_star_y_edge-STAR_Y_EDGE_MIN)/2 + STAR_Y_EDGE_MIN;
		
		pStar->now_point.x = pStar->start_point.x;
		pStar->now_point.y = pStar->start_point.y;
		pStar->now_point.z = pStar->start_point.z;

		break;

	case 3://-y
		pStar->start_point.z = (rand()%(STAR_RANDOM_Z_MAX-STAR_RANDOM_Z_MIN))+STAR_RANDOM_Z_MIN;
		pStar->start_point.x = rand()%(g_star_x_edge-STAR_X_EDGE_MIN) + STAR_X_EDGE_MIN+TPMove_offset.x;
		pStar->start_point.y = g_star_y_edge+redu+TPMove_offset.y;
		
		pStar->now_point.y = pStar->start_point.y;
		pStar->now_point.x = pStar->start_point.x;
		pStar->now_point.z = pStar->start_point.z;

		break;

	default:
		break;
	}
random_temp = 256;//128+127*(STAR_RANDOM_Z_MAX-pStar->start_point.z)/(STAR_RANDOM_Z_MAX-STAR_RANDOM_Z_MIN);
	switch(color%4)
	{
	case 0:
		pStar->color = ((0xf800*random_temp)>>8)&0xf800;
		break;
	case 1:
		pStar->color = ((0x07e0*random_temp)>>8)&0x07e0;//0x07e0;
		break;
	case 2:
		pStar->color = ((0x001f*random_temp)>>8)&0x001f;//0x001f;
		break;
	case 3:
		//pStar->color = 0xffe0;
		pStar->color = (((0x07e0*random_temp)>>8)&0x07e0)+(((0xf800*random_temp)>>8)&0xf800);
		break;

	default:
		//pStar->color = 0xffe0;
		pStar->color = (((0x07e0*random_temp)>>8)&0x07e0)+(((0xf800*random_temp)>>8)&0xf800);
		break;
	}
	color++;
}

static void init_dynamic_star(IMGREF_POINT_T start_point,IMGREF_SIZE_T size)
{

        int i = 0;
        int j = 0;

        IMGREF_3D_POINT_T sEye = {0};

        int32 ini_z = 6*STAR_WINDOW_W;
        int color = rand()%DYNAMIC_STAR_DITECTION;
        int width = ((size.w+size.h)/100+(rand()%((size.w+size.h)/200)))/2*2;

        sEye.x = size.w/2;
	sEye.y = size.h/2;
	sEye.z = (-2)*size.h;

    if(0 == sEye.z)
        return;

        for(i = 0;i<SUPPORT_TOUCH_NUM;i++)
            {
                if(( PNULL == g_dynamic_aStar[i][0])&&( PNULL == g_dynamic_aStar[i][1])&&( PNULL == g_dynamic_aStar[i][2])&&( PNULL == g_dynamic_aStar[i][3]))
                {
                    for( j = DYNAMIC_STAR_DITECTION - 1 ; j>= 0 ; j--)
                    {        

                        if(PNULL != g_dynamic_aStar[i][j])
                        {
                            MMILIVEWALLPAPER_FREE(g_dynamic_aStar[i][j]);//SCI_FREE
                        }
                        g_dynamic_aStar[i][j]=(STAR_LINE_T*)MMILIVEWALLPAPER_ALLOC(sizeof(STAR_LINE_T));//SCI_ALLOCA
                        if(PNULL == g_dynamic_aStar[i][j])
                        {
                            return;
                        }

                        g_dynamic_aStar[i][j]->width = width;
                         g_dynamic_aStar[i][j]->direction = j;

                         g_dynamic_aStar[i][j]->start_point.z = ini_z+TPMove_offset.z;
                            g_dynamic_aStar[i][j]->start_point.x = (start_point.x- sEye.x)*ini_z/(0-sEye.z)+start_point.x+TPMove_offset.x;//(g_dynamic_aStar[i][j]->start_point.z-TPMove_offset.z)
                            g_dynamic_aStar[i][j]->start_point.y = (start_point.y- sEye.y)*ini_z/(0-sEye.z)+start_point.y+TPMove_offset.y;

							
                            g_dynamic_aStar[i][j]->now_point.x = g_dynamic_aStar[i][j]->start_point.x;
                            g_dynamic_aStar[i][j]->now_point.y = g_dynamic_aStar[i][j]->start_point.y;
				g_dynamic_aStar[i][j]->now_point.z = g_dynamic_aStar[i][j]->start_point.z;
                
                        
                        switch((color+j)%DYNAMIC_STAR_DITECTION)
                        {
                            case 0:
                            g_dynamic_aStar[i][j]->color = 0xf800;
                            break;
                            case 1:
                            g_dynamic_aStar[i][j]->color = 0x07e0;
                            break;
                            case 2:
                            g_dynamic_aStar[i][j]->color = 0x001f;
                            break;
                            case 3:
                            g_dynamic_aStar[i][j]->color = 0xffe0;
                            break;

                            default:
                            g_dynamic_aStar[i][j]->color = 0xffe0;
                            break;
                        }
                        g_current_dynamic_star_num++;
                    }

                     break;
                }
            }      

}




/*****************************************************************************/
//  Description: update_randStars
//  Note:        
// 随机运动的星星
//****************************************************************************/

LOCAL void update_randStars(STAR_LINE_T *pStar, COLOR16 *pInOutColor,IMGREF_SIZE_T size)
{
	register uint32 color_temp1 = 0;
	register uint32 color_temp2 = 0;
	uint32 color_temp3 = 0;
	uint32 color_temp4 = 0;
	uint32 color_temp5 = 0;
	uint32 color = 0;

	uint32 alpha;

	COLOR16 *pColor = NULL;

	IMGREF_3D_POINT_T sEye = {0};
	IMGREF_3D_POINT_T in_Point;
	FIX16_POINT_T pos_inPic = {0};
	int32 max_angle = 900,angle_ing = 0;
	IMGREF_SIZE_T wh_inPic = {0};
	int32 i = 0, j = 0, x_st = 0, x_end = 0, y_st = 0, y_end = 0, xh_st = 0, xh_end = 0, yh_st = 0, yh_end = 0,alpha_calc = 0;
	FIX16_POINT_T cen_Poi = {0};
	IMGREF_SIZE_T wh_in2D = {0};
	int32 radius_inUse = 0;
	int32 max_alpha= 160;

	int32 x_temp = 0, y_temp = 0, radius_temp = 0;//优化用的临时参数
	//int32 pic_bg_w = 480, pic_bg_h = 320;

	sEye.x = size.w/2;
	sEye.y = size.h/2;
	sEye.z = (-2)*size.h;

//TP left means pos right, right means left. so minus TPMove_offset
	in_Point.x = pStar->now_point.x - TPMove_offset.x;//pStar->now_point.x;//
	in_Point.y = pStar->now_point.y - TPMove_offset.y;//pStar->now_point.y; //size.h/2;
	in_Point.z = pStar->now_point.z + TPMove_offset.z;//100 + TPMove_offset.z;

	if(in_Point.z <= sEye.z)//物体在眼睛后面(平行)是看不到的
		return;
	wh_inPic.w = (0-sEye.z)*STAR_WIDTH/(in_Point.z - sEye.z);// 0 means the screen//STAR_WIDTH;//
	wh_inPic.h = (0-sEye.z)*STAR_WIDTH*HEIGHT_WIDTH_RATIO/(in_Point.z - sEye.z);//wh_inPic.w*HEIGHT_WIDTH_RATIO;
	radius_inUse = (0-sEye.z)*STAR_WIDTH*RADIUS_STAR_HEADER/(STAR_WIDTH*(in_Point.z - sEye.z));
	if(0 == radius_inUse)
		return;

	s3d_Free_Point_change(in_Point,
						sEye,
						&pos_inPic
						);

	radius_temp = (1<<16)/(radius_inUse*radius_inUse);

	if(0 == pStar->direction)// +x
	{
		wh_in2D.w = wh_inPic.h;
		wh_in2D.h = wh_inPic.w;	
		
		y_st = MAX(pos_inPic.y,0);
		y_end = MIN(pos_inPic.y+wh_in2D.h,size.h-1);
		x_st = MIN(pos_inPic.x,size.w-1);
		x_end = MAX(pos_inPic.x-wh_in2D.w,0);
		

		//if(x_end < size.w)pStar->now_point.x
		if(pStar->now_point.x < (STAR_WIDTH*HEIGHT_WIDTH_RATIO) + g_star_x_edge+ TPMove_offset.x)
		{
			//画头

			cen_Poi.x = pos_inPic.x - (wh_in2D.h>>1);
			cen_Poi.y = pos_inPic.y + (wh_in2D.h>>1);
			xh_st = cen_Poi.x - radius_inUse;
			xh_st = MAX(xh_st,0);
			xh_end = cen_Poi.x + radius_inUse;
			xh_end = MIN(xh_end,size.w-1);

			yh_st = cen_Poi.y - radius_inUse;
			yh_st = MAX(yh_st,0);
			yh_end = cen_Poi.y + radius_inUse;
			yh_end = MIN(yh_end,size.h-1);


			Linear_Head_draw(pStar->color, yh_st, yh_end, xh_st, xh_end,pInOutColor,size,cen_Poi, radius_inUse);
			Linear_Body_R(pStar->color,y_st,y_end,x_st,x_end,pInOutColor,size,pos_inPic, wh_in2D);
		}
		else
		{
			init_star(pStar,size);
		}
	}

	if(1 == pStar->direction)// +y
	{
		wh_in2D.w = wh_inPic.w;
		wh_in2D.h = wh_inPic.h;	
		
		y_st = MIN(pos_inPic.y,size.h-1);
		y_end = MAX(pos_inPic.y-wh_in2D.h,0);
		x_st = MAX(pos_inPic.x,0);
		x_end = MIN(pos_inPic.x+wh_in2D.w,size.w-1);


		//if(y_end < size.h)
		if(pStar->now_point.y < (STAR_WIDTH*HEIGHT_WIDTH_RATIO) + g_star_y_edge+ TPMove_offset.y)
		{
			//画头

			cen_Poi.x = pos_inPic.x + (wh_inPic.w>>1);
			cen_Poi.y = pos_inPic.y - (wh_inPic.w>>1);
			xh_st = cen_Poi.x - radius_inUse;
			xh_st = MAX(xh_st,0);
			xh_end = cen_Poi.x + radius_inUse;
			xh_end = MIN(xh_end,size.w-1);

			yh_st = cen_Poi.y - radius_inUse;
			yh_st = MAX(yh_st,0);
			yh_end = cen_Poi.y + radius_inUse;
			yh_end = MIN(yh_end,size.h-1);
			
			Linear_Head_draw(pStar->color, yh_st, yh_end, xh_st, xh_end,pInOutColor,size,cen_Poi, radius_inUse);
			Linear_Body_D(pStar->color,y_st,y_end,x_st,x_end,pInOutColor,size,pos_inPic, wh_in2D);
		}
		else
		{
			init_star(pStar,size);
		}
	}

	if(2 == pStar->direction)// -x
	{
		wh_in2D.w = wh_inPic.h;
		wh_in2D.h = wh_inPic.w;	
		
		y_st = MAX(pos_inPic.y,0);
		y_end = MIN(pos_inPic.y+wh_in2D.h,size.h-1);
		x_st = MAX(pos_inPic.x,0);
		x_end = MIN(pos_inPic.x+wh_in2D.w,size.w-1);

		//if(x_end > 0)
		if(pStar->now_point.x > (STAR_WIDTH*HEIGHT_WIDTH_RATIO)*(-1)+STAR_X_EDGE_MIN+ TPMove_offset.x)
		{
			//画头

			cen_Poi.x = pos_inPic.x + (wh_inPic.w>>1);
			cen_Poi.y = pos_inPic.y + (wh_inPic.w>>1);
			xh_st = cen_Poi.x - radius_inUse;
			xh_st = MAX(xh_st,0);
			xh_end = cen_Poi.x + radius_inUse;
			xh_end = MIN(xh_end,size.w-1);

			yh_st = cen_Poi.y - radius_inUse;
			yh_st = MAX(yh_st,0);
			yh_end = cen_Poi.y + radius_inUse;
			yh_end = MIN(yh_end,size.h-1);

			Linear_Head_draw(pStar->color, yh_st, yh_end, xh_st, xh_end,pInOutColor,size,cen_Poi, radius_inUse);
                    Linear_Body_L(pStar->color,y_st,y_end,x_st,x_end,pInOutColor,size,pos_inPic, wh_in2D);
		}
		else
		{
			init_star(pStar,size);
		}
	}

	if(3 == pStar->direction)// -y
	{
		wh_in2D.w = wh_inPic.w;
		wh_in2D.h = wh_inPic.h;	
		
		y_st = MAX(pos_inPic.y,0);
		y_end = MIN(pos_inPic.y+wh_in2D.h,size.h-1);
		x_st = MAX(pos_inPic.x,0);
		x_end = MIN(pos_inPic.x+wh_in2D.w,size.w-1);

		//if(y_end > 0)
		if(pStar->now_point.y > (STAR_WIDTH*HEIGHT_WIDTH_RATIO)*(-1)+STAR_Y_EDGE_MIN+ TPMove_offset.y)
		{
			//画头

			cen_Poi.x = pos_inPic.x + (wh_inPic.w>>1);
			cen_Poi.y = pos_inPic.y + (wh_inPic.w>>1);
			xh_st = cen_Poi.x - radius_inUse;
			xh_st = MAX(xh_st,0);
			xh_end = cen_Poi.x + radius_inUse;
			xh_end = MIN(xh_end,size.w-1);

			yh_st = cen_Poi.y - radius_inUse;
			yh_st = MAX(yh_st,0);
			yh_end = cen_Poi.y + radius_inUse;
			yh_end = MIN(yh_end,size.h-1);

			Linear_Head_draw(pStar->color, yh_st, yh_end, xh_st, xh_end,pInOutColor,size,cen_Poi, radius_inUse);
			Linear_Body_U(pStar->color,y_st,y_end,x_st,x_end,pInOutColor,size,pos_inPic, wh_in2D);			
		}
		else
		{
			init_star(pStar,size);
		}
	}
	
	
	switch( pStar->direction )
	{
	case 0:	//+x
		pStar->now_point.x += wh_inPic.w*5;//pStar->width;
		break;
	case 1:	//+y
		pStar->now_point.y += wh_inPic.w*5;//pStar->width;
		break;
	case 2:	//-x
		pStar->now_point.x -= wh_inPic.w*5;//pStar->width;
		break;
	case 3:	//-y
		pStar->now_point.y -= wh_inPic.w*5;//pStar->width;
		break;

	default:
		break;
	}

}



/*****************************************************************************/
//  Description: update_dynamicStars
//  Note:        
// 互动星星的运动过程和随机的星星不一样
//****************************************************************************/

LOCAL void update_dynamicStars(STAR_LINE_T **pStar_addr, COLOR16 *pInOutColor,IMGREF_SIZE_T size)
{
	register uint32 color_temp1 = 0;
	register uint32 color_temp2 = 0;
	uint32 color_temp3 = 0;
	uint32 color_temp4 = 0;
	uint32 color_temp5 = 0;
	uint32 color= 0;

	uint32 alpha;

	COLOR16 *pColor = NULL;

	IMGREF_3D_POINT_T sEye = {0};
	IMGREF_3D_POINT_T in_Point = {0};
	FIX16_POINT_T pos_inPic = {0};
	int32 max_angle = 900,angle_ing = 0;
	IMGREF_SIZE_T wh_inPic = {0};
	int32 i = 0, j = 0, x_st = 0, x_end = 0, y_st = 0, y_end = 0, xh_st = 0, xh_end = 0, yh_st = 0, yh_end = 0,alpha_calc = 0;
	FIX16_POINT_T cen_Poi = {0};
	IMGREF_SIZE_T wh_in2D = {0};
	int32 radius_inUse = 0;
	int32 max_alpha= 160;

	int32 constrain_factor = 0;
	int32 x_temp = 0, y_temp = 0, radius_temp = 0;//优化用的临时参数

	FIX16_POINT_T constrain_point = {0};

	 if(PNULL == (*pStar_addr))
	 	return;

	sEye.x = size.w/2;
	sEye.y = size.h/2;
	sEye.z = (-2)*size.h;
//TP left means pos right, right means left. so minus TPMove_offset
	in_Point.x = (*pStar_addr)->now_point.x - TPMove_offset.x;//pStar->now_point.x;//size.w/2;
	in_Point.y = (*pStar_addr)->now_point.y - TPMove_offset.y;//pStar->now_point.y; //size.h/2;
	in_Point.z = (*pStar_addr)->now_point.z - TPMove_offset.z;//

	if(in_Point.z <= sEye.z)//物体在眼睛后面(平行)是看不到的
		return;
	/*wh_inPic.w = (0-sEye.z)*STAR_WIDTH/(in_Point.z - sEye.z);// 0 means the screen//STAR_WIDTH;//
	wh_inPic.h = (0-sEye.z)*STAR_WIDTH*HEIGHT_WIDTH_RATIO/(in_Point.z - sEye.z);//wh_inPic.w*HEIGHT_WIDTH_RATIO;
	radius_inUse = wh_inPic.w*RADIUS_STAR_HEADER/STAR_WIDTH;*/
	wh_inPic.w = (0-sEye.z)*STAR_WIDTH/(in_Point.z - sEye.z);// 0 means the screen//STAR_WIDTH;//
	wh_inPic.h = (0-sEye.z)*STAR_WIDTH*HEIGHT_WIDTH_RATIO/(in_Point.z - sEye.z);//wh_inPic.w*HEIGHT_WIDTH_RATIO;
	radius_inUse = (0-sEye.z)*STAR_WIDTH*RADIUS_STAR_HEADER/(STAR_WIDTH*(in_Point.z - sEye.z));
	constrain_factor = 0;
	if(0 == radius_inUse)
		return;

	s3d_Free_Point_change(in_Point,
						sEye,
						&pos_inPic
						);

	//计算限制点
	in_Point.x = (*pStar_addr)->start_point.x - TPMove_offset.x;//pStar->now_point.x;//size.w/2;
	in_Point.y = (*pStar_addr)->start_point.y - TPMove_offset.y;//pStar->now_point.y; //size.h/2;
	in_Point.z = (*pStar_addr)->start_point.z - TPMove_offset.z;//

	s3d_Free_Point_change(in_Point,
						sEye,
						&constrain_point
						);

	//constrain_point.x = (*pStar_addr)->start_point.x - TPMove_offset.x;
	//constrain_point.y = (*pStar_addr)->start_point.y - TPMove_offset.y;
	//pos_inPic.x = 

	//pos_inPic.x = (*pStar_addr)->now_point.x;
	//pos_inPic.y = (*pStar_addr)->now_point.y;

	radius_temp = (1<<16)/(radius_inUse*radius_inUse);

	if(0 == (*pStar_addr)->direction)// +x
	{
		wh_in2D.w = wh_inPic.h;
		wh_in2D.h = wh_inPic.w;	

		pos_inPic.x += (wh_inPic.w>>1);
		pos_inPic.y -= (wh_inPic.w>>1);
		
		y_st = MAX(pos_inPic.y,0);
		y_end = MIN(pos_inPic.y+wh_in2D.h,size.h-1);
		x_st = MIN(pos_inPic.x,size.w-1);
		x_end = MAX(pos_inPic.x-wh_in2D.w,0);
		//手点击时候的限制
		x_end = MAX(constrain_point.x-constrain_factor,x_end);
		
		//if(x_end < size.w)//if(x_st >= x_end)
		if((*pStar_addr)->now_point.x < (STAR_WIDTH*HEIGHT_WIDTH_RATIO) + g_star_x_edge+ TPMove_offset.x)
		{
			//画头

			cen_Poi.x = pos_inPic.x - (wh_in2D.h>>1);
			cen_Poi.y = pos_inPic.y + (wh_in2D.h>>1);
			xh_st = cen_Poi.x - radius_inUse;
			xh_st = MAX(xh_st,0);
			xh_end = cen_Poi.x + radius_inUse;
			xh_end = MIN(xh_end,size.w-1);

			yh_st = cen_Poi.y - radius_inUse;
			yh_st = MAX(yh_st,0);
			yh_end = cen_Poi.y + radius_inUse;
			yh_end = MIN(yh_end,size.h-1);

			Linear_Head_draw((*pStar_addr)->color, yh_st, yh_end, xh_st, xh_end,pInOutColor,size,cen_Poi, radius_inUse);
			Linear_Body_R((*pStar_addr)->color,y_st,y_end,x_st,x_end,pInOutColor,size,pos_inPic, wh_in2D);
			
		}
		else
		{
			MMILIVEWALLPAPER_FREE((*pStar_addr));
                    g_current_dynamic_star_num--;
                    return;
		}
	}

	if(1 == (*pStar_addr)->direction)// +y
	{
		wh_in2D.w = wh_inPic.w;
		wh_in2D.h = wh_inPic.h;	

		pos_inPic.x -= (wh_inPic.w>>1);
		pos_inPic.y += (wh_inPic.w>>1);
		
		y_st = MIN(pos_inPic.y,size.h-1);
		y_end = MAX(pos_inPic.y-wh_in2D.h,0);
		x_st = MAX(pos_inPic.x,0);
		x_end = MIN(pos_inPic.x+wh_in2D.w,size.w-1);

		//手点击时候的限制
		y_end = MAX(constrain_point.y-constrain_factor,y_end);


		//if(y_end < size.h)//if(y_st >= y_end)
		if((*pStar_addr)->now_point.y < (STAR_WIDTH*HEIGHT_WIDTH_RATIO) + g_star_y_edge+ TPMove_offset.y)
		{
			//画头

			cen_Poi.x = pos_inPic.x + (wh_inPic.w>>1);
			cen_Poi.y = pos_inPic.y - (wh_inPic.w>>1);
			xh_st = cen_Poi.x - radius_inUse;
			xh_st = MAX(xh_st,0);
			xh_end = cen_Poi.x + radius_inUse;
			xh_end = MIN(xh_end,size.w-1);

			yh_st = cen_Poi.y - radius_inUse;
			yh_st = MAX(yh_st,0);
			yh_end = cen_Poi.y + radius_inUse;
			yh_end = MIN(yh_end,size.h-1);

			Linear_Head_draw((*pStar_addr)->color, yh_st, yh_end, xh_st, xh_end,pInOutColor,size,cen_Poi, radius_inUse);
                    Linear_Body_D((*pStar_addr)->color,y_st,y_end,x_st,x_end,pInOutColor,size,pos_inPic, wh_in2D);
		}
		else
		{
			//init_star((*pStar_addr),size);
			MMILIVEWALLPAPER_FREE((*pStar_addr));
                    //(*pStar_addr) = PNULL;
                    g_current_dynamic_star_num--;
                    return;
		}
	}

	if(2 == (*pStar_addr)->direction)// -x
	{
		wh_in2D.w = wh_inPic.h;
		wh_in2D.h = wh_inPic.w;	

		pos_inPic.x -= (wh_inPic.w>>1);
		pos_inPic.y -= (wh_inPic.w>>1);
		
		y_st = MAX(pos_inPic.y,0);
		y_end = MIN(pos_inPic.y+wh_in2D.h,size.h-1);
		x_st = MAX(pos_inPic.x,0);
		x_end = MIN(pos_inPic.x+wh_in2D.w,size.w-1);

		x_end = MIN(constrain_point.x+constrain_factor,x_end);

		//if(x_end > 0)//if(x_st<=x_end)
		if((*pStar_addr)->now_point.x > (STAR_WIDTH*HEIGHT_WIDTH_RATIO)*(-1)+STAR_X_EDGE_MIN+ TPMove_offset.x)
		{
			//画头

			cen_Poi.x = pos_inPic.x + (wh_inPic.w>>1);
			cen_Poi.y = pos_inPic.y + (wh_inPic.w>>1);
			xh_st = cen_Poi.x - radius_inUse;
			xh_st = MAX(xh_st,0);
			xh_end = cen_Poi.x + radius_inUse;
			xh_end = MIN(xh_end,size.w-1);

			yh_st = cen_Poi.y - radius_inUse;
			yh_st = MAX(yh_st,0);
			yh_end = cen_Poi.y + radius_inUse;
			yh_end = MIN(yh_end,size.h-1);
			
			Linear_Head_draw((*pStar_addr)->color, yh_st, yh_end, xh_st, xh_end,pInOutColor,size,cen_Poi, radius_inUse);
                    Linear_Body_L((*pStar_addr)->color,y_st,y_end,x_st,x_end,pInOutColor,size,pos_inPic, wh_in2D);
		}
		else
		{
			//init_star((*pStar_addr),size);
			MMILIVEWALLPAPER_FREE((*pStar_addr));
                    //(*pStar_addr) = PNULL;
                    g_current_dynamic_star_num--;
                    return;
		}
	}

	if(3 == (*pStar_addr)->direction)// -y
	{
		wh_in2D.w = wh_inPic.w;
		wh_in2D.h = wh_inPic.h;	

		pos_inPic.x -= (wh_inPic.w>>1);
		pos_inPic.y -= (wh_inPic.w>>1);
		
		y_st = MAX(pos_inPic.y,0);
		y_end = MIN(pos_inPic.y+wh_in2D.h,size.h-1);
		x_st = MAX(pos_inPic.x,0);
		x_end = MIN(pos_inPic.x+wh_in2D.w,size.w-1);

		y_end = MIN(constrain_point.y+constrain_factor,y_end);

		//if(y_end > 0)//if(y_st<=y_end)
		if((*pStar_addr)->now_point.y > (STAR_WIDTH*HEIGHT_WIDTH_RATIO)*(-1)+STAR_Y_EDGE_MIN+ TPMove_offset.y)
		{
			//画头

			cen_Poi.x = pos_inPic.x + (wh_inPic.w>>1);
			cen_Poi.y = pos_inPic.y + (wh_inPic.w>>1);
			xh_st = cen_Poi.x - radius_inUse;
			xh_st = MAX(xh_st,0);
			xh_end = cen_Poi.x + radius_inUse;
			xh_end = MIN(xh_end,size.w-1);

			yh_st = cen_Poi.y - radius_inUse;
			yh_st = MAX(yh_st,0);
			yh_end = cen_Poi.y + radius_inUse;
			yh_end = MIN(yh_end,size.h-1);
			
			 Linear_Head_draw((*pStar_addr)->color, yh_st, yh_end, xh_st, xh_end,pInOutColor,size,cen_Poi, radius_inUse);
                    Linear_Body_U((*pStar_addr)->color,y_st,y_end,x_st,x_end,pInOutColor,size,pos_inPic, wh_in2D);
		}
		else
		{
			//init_star((*pStar_addr),size);
			MMILIVEWALLPAPER_FREE((*pStar_addr));
                    //(*pStar_addr) = PNULL;
                    g_current_dynamic_star_num--;
                    return;
		}
	}
	
	
	switch( (*pStar_addr)->direction )
	{
	case 0:	//+x
		(*pStar_addr)->now_point.x += wh_inPic.w*5;
		break;
	case 1:	//+y
		(*pStar_addr)->now_point.y += wh_inPic.w*5;
		break;
	case 2:	//-x
		(*pStar_addr)->now_point.x -= wh_inPic.w*5;
		break;
	case 3:	//-y
		(*pStar_addr)->now_point.y -= wh_inPic.w*5;
		break;

	default:
		break;
	}

}

/*****************************************************************************/
//  Description: S2D_LinearInit
//  Note:        
//****************************************************************************/
PUBLIC void S2D_LinearInit(uint16 scr_width, uint16 scr_height)
{
	int i = 0;
    IMGREF_SIZE_T size = {0};
    
    size.w = scr_width;
    size.h = scr_height;
	for( i = STAR_NUM - 1 ; i >= 0 ; i--)
	{
	    init_star(&g_aStar[i], size);
	}
}

/*****************************************************************************/
//  Description: S2D_Dynamic_LinearCreate
//  Note:        
//****************************************************************************/
PUBLIC void  S2D_Dynamic_LinearCreate(int16 x,int16 y,uint16 scr_width, uint16 scr_height)
{
    IMGREF_SIZE_T size = {0};
    IMGREF_POINT_T start_point = {0};

    
    start_point.x = x;
    start_point.y = y;
    
    size.w = scr_width;
    size.h = scr_height;
    
    if(g_current_dynamic_star_num<=SUPPORT_TOUCH_NUM*DYNAMIC_STAR_DITECTION -DYNAMIC_STAR_DITECTION)
        { 
                init_dynamic_star(start_point,size);
        }
    
}

/*****************************************************************************/
//  Description: S2D_LinearUpdate
//  Note:        
//****************************************************************************/
PUBLIC void S2D_LinearUpdate(LW_CAL_PARAM *lw_cal_param)
 {
    int i = 0;
    int j = 0;
	int32 sort_i = 0;
    IMGREF_SIZE_T size = {0};    
	int32 z_sort[STAR_ARR_NUM] = {0};
	int32 z_sort_index[STAR_ARR_NUM] = {0};
	//int32 min_z = 0x7fffffff;
	BOOLEAN isChanged = SCI_TRUE;
	int32 iLen = STAR_ARR_NUM;
	int32 swap_temp = 0;

	IMGREF_SIZE_T Part_img = {0};
	IMGREF_POINT_T carve_img = {0};
	//int32 delta_z = -640;
	int32 z_max = 0, eye_z = 0;
	//int32 pageNum = 5;
	
	COLOR16 *target_buf_ptr = lw_cal_param->dest_buf_info.buf_ptr;
	uint16* source_buf_ptr = lw_cal_param->src_buf_info.buf_ptr; 
	int32 scr_width = lw_cal_param->src_buf_info.w;
	int32 scr_height = lw_cal_param->src_buf_info.h;

	FIX16_POINT_T ST_offS = {0};// FOR PIC SCALE
	
    int32 page_total_move = 0;
	int32 bg_total_move = 0;
	SCALE_IMAGE_IN_T scale_in = {0};
    SCALE_IMAGE_OUT_T scale_out = {0};

    size.w = lw_cal_param->dest_buf_info.w;
    size.h = lw_cal_param->dest_buf_info.h;

	ST_offS.x = PIC_OFFS_WIDTH;
	ST_offS.y = (scr_height-size.h)/2;
	
    page_total_move = (lw_cal_param->fg_pos_info.rc.right - lw_cal_param->fg_pos_info.rc.left + 1-MMI_MAINSCREEN_WIDTH);
	bg_total_move = scr_width - MMI_MAINSCREEN_WIDTH-2*ST_offS.x;

	if(page_total_move<0)
	{
		return;
	}
	else if(0==page_total_move)
	{
		TPMove_offset.x = 0;
	}
	else
	{
		TPMove_offset.x = lw_cal_param->fg_pos_info.x*bg_total_move/page_total_move;
	}
	if(TPMove_offset.x<0)
		TPMove_offset.x = 0;
	
	TPMove_offset.y = lw_cal_param->fg_pos_info.y;
	TPMove_offset.w = 0;
	TPMove_offset.z = s_moved_z_axis;
	
	eye_z = (-2)*size.h;


	z_max = 10*size.w;


Part_img.w = size.w*(z_max+TPMove_offset.z-eye_z)/(z_max-eye_z);
Part_img.h = size.h*(z_max+TPMove_offset.z-eye_z)/(z_max-eye_z);

carve_img.x = MIN( size.w/2-Part_img.w/2 + TPMove_offset.x+ST_offS.x,scr_width - Part_img.w);
carve_img.x = MAX(carve_img.x,0);

carve_img.y = MIN(size.h/2-Part_img.h/2 + TPMove_offset.y+ST_offS.y,scr_height-Part_img.h);
carve_img.y = MAX(carve_img.y,0);


    scale_in.src_size.w = scr_width;
	scale_in.src_size.h = scr_height;
	scale_in.src_trim_rect.x = carve_img.x & 0xfffc;
	scale_in.src_trim_rect.y = carve_img.y & 0xfffc;
	scale_in.src_trim_rect.w = Part_img.w  & 0xfffc;
	scale_in.src_trim_rect.h = Part_img.h  & 0xfffc;

	scale_in.src_format = IMGREF_FORMAT_RGB565;
	scale_in.src_chn.chn0.ptr = source_buf_ptr;
	scale_in.src_chn.chn0.size = scale_in.src_size.w*scale_in.src_size.h*(sizeof(COLOR16));

	scale_in.target_size.w = size.w;
	scale_in.target_size.h = size.h;
	scale_in.target_format = IMGREF_FORMAT_RGB565;
	scale_in.target_buf.ptr = target_buf_ptr;
	scale_in.target_buf.size = scale_in.target_size.w*scale_in.target_size.h*(sizeof(COLOR16));

	scale_in.scale_mode = SCALE_MODE_DISTORT;
/*	scale_out.output_format = IMGREF_FORMAT_RGB565;
	scale_out.output_size.h = size.h;
	scale_out.output_size.w = size.w;
	scale_out.output_chn.chn0.ptr = target_buf_ptr;
	scale_out.output_chn.chn0.size = scale_out.output_size.h*scale_out.output_size.w*(sizeof(COLOR16));
*/
	if(SCI_SUCCESS != GRAPH_ScaleImage(&scale_in,&scale_out))
	    return;
	//z轴排序
    sort_i = 0;
    for( i = 0; i < STAR_NUM ; i++)
	{
		//update_randStars(&g_aStar[i], target_buf_ptr, size);
		z_sort[sort_i++] = g_aStar[i].start_point.z;
	}

	for(j=0;j<SUPPORT_TOUCH_NUM;j++)
	{
		for( i = DYNAMIC_STAR_DITECTION - 1 ; i >= 0 ; i--)
		{
			if(PNULL != g_dynamic_aStar[j][i])
				z_sort[sort_i++] = g_dynamic_aStar[j][i]->start_point.z;
			else
				z_sort[sort_i++] = 0x7fffffff;
		}
	}

	for( i = 0 ; i < STAR_ARR_NUM ; ++i)
	{
		z_sort_index[i] = i;
	}

	isChanged = SCI_TRUE;
	for( i = 0 ; i < iLen-1 ; i ++)
	{
		if(isChanged)
		{
			isChanged = SCI_FALSE;
			for( j = 1 ; j < iLen - i ; j ++)
			{
				if(z_sort[j-1]>=z_sort[j])
				{
					
				}
				else
				{
					isChanged = SCI_TRUE;
					
					swap_temp = z_sort[j-1];
					z_sort[j-1] = z_sort[j];
					z_sort[j] = swap_temp;

					swap_temp = z_sort_index[j-1];
					z_sort_index[j-1] = z_sort_index[j];
					z_sort_index[j] = swap_temp;
				}
			}
		}
	}

	for(sort_i=0; sort_i<STAR_ARR_NUM; ++sort_i)
	    {
	    	if(z_sort_index[sort_i]<STAR_NUM)
	    	{
	    		i = z_sort_index[sort_i];
	    		update_randStars(&g_aStar[i], target_buf_ptr, size);
	    	}
		else
		{
			i = (z_sort_index[sort_i]-STAR_NUM)%DYNAMIC_STAR_DITECTION;
			j = (z_sort_index[sort_i]-STAR_NUM)/DYNAMIC_STAR_DITECTION;
			update_dynamicStars(&g_dynamic_aStar[j][i], target_buf_ptr, size);
		}
	    		
	    }
    
	//SCI_Free(target_buf_cpy);
	
	
    
}



/*---------------------------------------------------------------------------*/
/*                          FUNCTION DEFINITION                                                              */
/*---------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 线性光幕初始化
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LinerInit(void)
{
    BOOLEAN b_result = TRUE;
	IMGREF_SIZE_T bg_size = {0};	
	
	s_moved_z_axis = 0;
	LINEARGetWidthAndHeight(&bg_size.w,&bg_size.h);
	g_star_x_edge = (5*STAR_WINDOW_W*STAR_WINDOW_W/(2*STAR_WINDOW_H)+bg_size.w);
	g_star_y_edge = (5*STAR_WINDOW_W/2+bg_size.h);
	
	S2D_LinearInit(MMI_MAINSCREEN_WIDTH, MMI_MAINSCREEN_HEIGHT);
    return b_result;
}

/*****************************************************************************/
//  Description : 线性光幕初始化
//  Global resource dependence : 
//  Author:arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LinerConstruct(MMI_WIN_ID_T win_id)
{
    BOOLEAN b_result = TRUE;
    return b_result;
}

/*****************************************************************************/
//  Description : 线性光幕准备背景数据
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LinerDrawBg(MMI_WIN_ID_T win_id,const GUI_LCD_DEV_INFO *dev_info_ptr)
{
    BOOLEAN b_result = FALSE;
    GUI_POINT_T display_point = {0,0};
    if (PNULL != dev_info_ptr)
    {
        b_result = GUIRES_DisplayImg(&display_point, 
		PNULL, 
		PNULL, 
		win_id, 
		IMAGE_WALLPAPER_LINEAR_BG, 
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
LOCAL void LinerCalculate(LW_CAL_PARAM *lw_cal_param)
{

    if(PNULL == lw_cal_param) 
    {
        return;
    }
    //SCI_MEMCPY((GUI_COLOR_T*)lw_cal_param->dest_buf_info.buf_ptr, lw_cal_param->src_buf_info.buf_ptr,MMI_MAINSCREEN_WIDTH*MMI_MAINSCREEN_HEIGHT*sizeof(uint16));
    S2D_LinearUpdate(lw_cal_param);
}
/*****************************************************************************/
//  Description : 线性光幕释放
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LinerDestruct(void)
{	
    int32 i = 0, j = 0;
    for(i = 0;i<SUPPORT_TOUCH_NUM;i++)
    {
        for( j = DYNAMIC_STAR_DITECTION - 1 ; j>= 0 ; j--)
        {             
            if(PNULL != g_dynamic_aStar[i][j])
            {
                MMILIVEWALLPAPER_FREE(g_dynamic_aStar[i][j]);//SCI_FREE
            }
        }
    }
}

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
/*****************************************************************************/
//  Description : 风车手势消息处理
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
LOCAL void Linear_HandleTpGesture(DPARAM param)
{

	if (MMI_GESTURE_ZOOM_IN == MMK_GET_TP_GESTURE(param))//收缩
	{
		s_moved_z_axis += LINEAR_MOVE_Z_PARAM;
	}
	else if(MMI_GESTURE_ZOOM_OUT == MMK_GET_TP_GESTURE(param))//伸展
	{
		s_moved_z_axis -= LINEAR_MOVE_Z_PARAM;
	}
	else
	{

	}
	
	s_moved_z_axis = MIN(s_moved_z_axis,Z_DELTA_MAX);

	s_moved_z_axis = MAX(s_moved_z_axis,Z_ZOOMIN_MAX);
}
#endif

/*****************************************************************************/
//  Description : 线性光幕释交互事件处理
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN LinerHandleEvent(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    GUI_POINT_T   point       = {0};
     
    switch(msg_id)
    {
    	case MSG_TP_PRESS_MOVE:
		is_tp_move = TRUE;
		break;

        case MSG_TP_PRESS_UP:
    		if(is_tp_move)
    		{
    			is_tp_move = FALSE;
    			break;
    		}
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);
#ifndef MMI_RES_ORIENT_LANDSCAPE
	        S2D_Dynamic_LinearCreate(point.x,point.y,MMI_MAINSCREEN_WIDTH,MMI_MAINSCREEN_HEIGHT);
#else
            S2D_Dynamic_LinearCreate(point.x,point.y,MMI_MAINSCREEN_HEIGHT,MMI_MAINSCREEN_WIDTH);
#endif
    			break;

    	case MSG_TP_GESTURE://手势处理 拉近  放缩
    		Linear_HandleTpGesture(param);
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
LOCAL uint32 LinerGetType(void)
{
    return MMI_LIVE_WALLPAPER_TYPE_LINER;
}
/*****************************************************************************/
//  Description : 获得动态壁纸ThumbNail信息
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LinerGetThumbNailInfo(LW_THUMBNAIL_INFO *thumbnail_info_ptr)
{      
    if(thumbnail_info_ptr)
    {
        thumbnail_info_ptr->is_image_file = FALSE;
        thumbnail_info_ptr->image_id = IMAGE_WALLPAPER_LINEAR_THUMBNAIL;

        thumbnail_info_ptr->is_not_string = FALSE;
        thumbnail_info_ptr->text_id = TXT_SET_WALLPAPER_LINEAR;
        thumbnail_info_ptr->instance_mem_pool_size = BLOCK_MEM_POOL_SIZE_LILNER;
    }
}
/*****************************************************************************/
//  Description : 获得动态壁纸源数据宽高
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL void LINEARGetWidthAndHeight(uint16 *width_ptr,uint16 *height_ptr)
{
    if(PNULL == width_ptr ||PNULL == height_ptr )
    {
        return;
    }
	
    GUIRES_GetImgWidthHeight((uint16*)width_ptr, (uint16*)height_ptr,IMAGE_WALLPAPER_LINEAR_BG,MMK_GetFocusWinId());
}
/*****************************************************************************/
//  Description : 获得动态壁纸选项列表
//  Global resource dependence : 
//  Author:Arvin.wu
//  Note:
/*****************************************************************************/
LOCAL OPTIONLIST *LINEARGetOptionList(void)
{
    return (OPTIONLIST*)PNULL;
} 
#endif// LIVE_WALLPAPER_SUPPORT_LINEAR


#endif //_MMI_LIVEWALLPAPER_LINEAR_C_




/*Edit by script, ignore 3 case. Thu Apr 26 19:00:58 2012*/ //IGNORE9527
