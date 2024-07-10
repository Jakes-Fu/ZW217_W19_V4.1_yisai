/*****************************************************************************
** File Name:      mmiwidget_sns.c                                        *
** Author:                                                                   *
** Date:           01/05/2012                                                *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE              NAME             DESCRIPTION                               *
** 01/05/2012        renwei              Creat
******************************************************************************/
#include "mmi_app_widget_trc.h"
#ifdef SNS_SUPPORT
#define _MMIWIDGET_SNS_C_


/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "std_header.h"



#include "window_parse.h"
#include "guilabel.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "mmidisplay_data.h"
#include "mmiwidget_image.h"
#include "mmiwidget_id.h"

#include "guibutton.h"
#include "mmiwidget.h"
#include "guilcd.h"
#include "guires.h"
#include "mmitheme_widget.h"
#include "mmi_appmsg.h"
#include "mmi_menutable.h"
#include "Mmk_app.h"
#include "mmiwidget_position.h"
#include "mmisns_srv.h"
#include "mmipub.h"
#include "mmiwre_export.h"
#include "mmiwre.h"
#include "graphics_3d_rotate.h"
#include "mmi_resource.h"
#include "mmk_tp.h"
#include "mmiwidget_export.h"
#ifdef SNS_PULLING_SUPPORT
#include "mmiaudio_ctrl.h"
#include "mmienvset_export.h"
#include "mmisrvaud_api.h"
#include "mmicc_export.h"
#include "mmiudisk_export.h"
#endif //SNS_PULLING_SUPPORT

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define MMI_SNS_WIDGET_WIN_MAX    3

#define MMI_SNS_WIDGET_LAYER_MAX  2

#define TIME_UPDATE_TIMER         16
//#define MMI_SNS_TEXT_NUM          10
#define MMI_FREAME_NUM            6
#define MMI_DIRECTION_UP          0
#define MMI_DIRECTION_DOWN        1
#define MMI_MOVE_CHANGE_MAX       5
#define MMI_LOGIN_LEN             20  
#define MMI_TOTAL_FRAME           15//
#define MMI_SNS_MOVE_TIME         25//20
#define DISPLAY_NUM 5

#ifdef SNS_PULLING_SUPPORT
#define MMISNS_VIBRA_TIMEROUT     3000
#endif //SNS_PULLING_SUPPORT

/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

typedef struct T_WIDGET_DETAIL_DATA
{
    int            index;                  //索引值
    wchar          name[MMI_NAME_LEN+1];   //姓名
    wchar          date[MMI_DATE_LEN+1];   //时间
    wchar          text[MMI_TEXT_LEN+1];//文本内容
}T_WIDGET_DETAIL_DATA;

typedef struct T_WIDGET_INFO
{
    uint8                 temp;
    uint8                 blogin;
    uint8                 timer_id;                      //刚开机LOAD数据TIMER
    uint8                 move_timer_id;                 //
    int                   g_direction;                   //方向
    int                   curr_num;                      //当前该WIDGET 获取的记录数
    int                   curr_index;                    //WIDGET 列表INDEX
    T_WIDGET_DETAIL_DATA  text_info[MMI_WIDGET_TEXT_NUM];// 记录的数据
}T_WIDGET_INFO;

typedef struct
{
    //uint8               timer_id;
    GUI_POINT_T         cur_point;
    GUI_POINT_T         start_point;
    //GUI_POINT_T         pre_point;
    T_WIDGET_INFO       widget_info[MMI_SNS_MAX];    
    
    GUI_LCD_DEV_INFO    lcd_dev_info[MMI_SNS_WIDGET_LAYER_MAX];
}MMIWIGET_SNS_INFO_T;

typedef struct
{	uint32    *virtue_color_up_ptr; //暂时不要理会
	uint32    *virtue_color_down_ptr;//暂时不要理会
	uint32    virtue_color_width;//暂时不要理会
	uint32    virtue_color_height;//暂时不要理会
 	uint32    *src_current;  //当前显示的图片buffer
	uint32    *src_next;     //下一帧显示的图片buffer
	uint32     src_width;		//显示图片的宽
	uint32     src_height;		//显示图片的高
	uint32  *  dst_ptr;		//结果输出buffer
	uint32     dst_width;		//结果输出buffer的宽
	uint32     dst_height;		//结果输出buffer的高
	uint8*     ext_ptr;            //一个内存空间大小为2048*sizeof(FIX16_POINT_T)
	int32     cur_frame;		//当前帧数
	int32     total_frame;		//所有帧数
	uint8      src_num;		//页面个数暂定为5。
	uint8      focus_num;		//暂定为3。
	BOOLEAN    is_go_up;		//滑动方向，true为上滑，false为下滑。
	BOOLEAN    is_standby;		//暂时不要理会
} HTC_IN_PARAM_T;

#ifdef SNS_PULLING_SUPPORT
typedef enum 
{
    BUTTON_NONE,
    BUTTON_NEW,
    BUTTON_REFRESH,
    BUTTON_SETTING,
    BUTTON_MAX
} WIDGET_BUTTON_TYPE_E;
#endif //SNS_PULLING_SUPPORT

/**--------------------------------------------------------------------------*
**                         EXTERNAL DEFINITION                              *
**--------------------------------------------------------------------------*/
#ifdef SNS_PULLING_SUPPORT
extern BOOLEAN GPIO_CheckHeadsetStatus(void);
#endif //SNS_PULLING_SUPPORT

/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/
LOCAL MMIWIGET_SNS_INFO_T SNSWidgetInfo = {0}; 
LOCAL MMIWIGET_SNS_INFO_T *g_pSNSWidgetInfo = &SNSWidgetInfo; 

LOCAL MMI_TP_STATUS_E     s_tp_status     = MMI_TP_NONE;
LOCAL IMMISns            *pMe             = PNULL;
LOCAL int                 g_direction     = 0;
LOCAL int                 g_widget_id     = 0;
LOCAL BOOLEAN             s_sns_is_slide  = FALSE;
LOCAL uint32              empty_str       = TXT_BLANK;
LOCAL COLOR32 * virtue_color_up_ptr   = PNULL;
LOCAL COLOR32 * virtue_color_down_ptr = PNULL;
LOCAL uint8   * ext_ptr               = PNULL;  
//LOCAL GUI_LCD_DEV_INFO dev_info_ptr_up = {0};
//LOCAL GUI_LCD_DEV_INFO dev_info_ptr_down = {0};
LOCAL HTC_IN_PARAM_T  in_data_ptr     ={0}; 
LOCAL HTC_IN_PARAM_T *g_p_in_data_ptr = &in_data_ptr;
LOCAL UILAYER_COPYBUF_T        bg_layer_buf     = {0};
LOCAL UILAYER_COPYBUF_PARAM_T  bg_layer_param   = {0};
LOCAL UILAYER_INFO_T g_sns_ayer_info = {0};
LOCAL GUI_LCD_DEV_INFO g_sns_layer_handle ;
LOCAL BOOLEAN g_is_rolling = FALSE;
//LOCAL uint16 bg_width = MMIWIDGET_SNS_BG_WIDTH;
//LOCAL uint16 bg_height= MMIWIDGET_SNS_BG_HEIGHT;

//LOCAL uint16 bg2_width = MMIWIDGET_SNS_BG2_WIDTH;
//LOCAL uint16 bg2_height = MMIWIDGET_SNS_BG2_HEIGHT;

LOCAL BOOLEAN bfirst_dection = TRUE;
LOCAL BOOLEAN g_is_go_up     = TRUE;
LOCAL BOOLEAN g_is_reload    = FALSE;
//LOCAL IMGREF_SIZE_T  virtue_size ={MMIWIDGET_SNS_VIRTUE_WIDTH, MMIWIDGET_SNS_VIRTUE_HEIGHT};

#ifdef SNS_PULLING_SUPPORT
LOCAL WIDGET_BUTTON_TYPE_E s_widget_button_type = BUTTON_NONE;
LOCAL MMI_IMAGE_ID_T s_button_id_ary[BUTTON_MAX] = {0};
#endif //SNS_PULLING_SUPPORT
/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/
LOCAL MMI_RESULT_E  HandleWidgetSNSWinMsg(
										   MMI_WIN_ID_T        win_id,        //IN:
										   MMI_MESSAGE_ID_E    msg_id,        //IN:
										   DPARAM            param        //IN:
										   );


LOCAL MMI_RESULT_E MMIWidget_SNSCreateLayer(MMI_WIN_ID_T  win_id, GUI_LCD_DEV_INFO  *lcd_dev_info, int16 width, int16 height);
LOCAL MMI_RESULT_E MMIWidget_SNSCreateRollingBuffer(MMI_WIN_ID_T  win_id);
LOCAL void MMIWidget_SNSSetRillingParam(MMI_WIN_ID_T  win_id);
LOCAL void MMIWidget_SNSReleaseRollingBuffer(void);
LOCAL void ResetWidgetSNS(void);
LOCAL MMI_RESULT_E MMIWidget_GetItemNum(int widgetid, int *pCurrNum);
LOCAL void MMIWidget_DrawReleaseParam(void);
LOCAL MMI_RESULT_E MMIWidget_DrawMoveInitParam(MMI_WIN_ID_T win_id);

#ifdef SNS_PULLING_SUPPORT
LOCAL MMI_RESULT_E HandleSnsWidgetSettingWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);
LOCAL void PlayMsgSound(void);
LOCAL void PlaySoundCallBack(MMISRVAUD_REPORT_RESULT_E result, DPARAM param);
#endif //SNS_PULLING_SUPPORT

/**--------------------------------------------------------------------------*/

PUBLIC const MMIWIDGET_ITEM_INFO_T g_sina_widget =
{
        MMIWIDGET_SNS_SINA_ID, 
		HandleWidgetSNSWinMsg, 
		PNULL,
		ResetWidgetSNS,
		PNULL,		
		WIDGET_SNS_SINA_WIN_ID, 
		IMAGE_WIDGET_SNS_SINA,
		IMAGE_WIDGET_SNS_BG1, 
		TXT_WIDGET_SINA,
		0,
		0,
		TRUE,
};

PUBLIC const MMIWIDGET_ITEM_INFO_T g_facebook_widget =
{
        MMIWIDGET_SNS_FACEBOOK_ID, 
		HandleWidgetSNSWinMsg, 
		PNULL,
		ResetWidgetSNS,
		PNULL,		
		WIDGET_SNS_FACEBOOK_WIN_ID, 
		IMAGE_WIDGET_SNS_FACEBOOK, 
		IMAGE_WIDGET_SNS_BG1, 
		TXT_COMMON_FACEBOOK,
		0,
		0,
		TRUE,
};

PUBLIC const MMIWIDGET_ITEM_INFO_T g_twitter_widget =
{
        MMIWIDGET_SNS_TIWTTER_ID, 
		HandleWidgetSNSWinMsg, 
		PNULL,
		ResetWidgetSNS,
		PNULL,		
		WIDGET_SNS_TIWTTER_WIN_ID, 
		IMAGE_WIDGET_SNS_TWITTER, 
		IMAGE_WIDGET_SNS_BG1, 
		TXT_COMMON_TIWTTER,
		0, 
		0,
		TRUE,
};

#ifdef SNS_PULLING_SUPPORT
WINDOW_TABLE(WIDGET_SNS_SETTING_WIN_TAB) =
{
    WIN_FUNC((uint32)HandleSnsWidgetSettingWinMsg), 
    WIN_ID(WIDGET_SNS_SETTING_WIN_ID),
    WIN_TITLE(TXT_WIDGET_SETTING),
    CREATE_LISTBOX_CTRL(GUILIST_RADIOLIST_E, MMIWIDGET_SNS_SETTING_CTRL_ID),
    END_WIN
};
#endif //SNS_PULLING_SUPPORT

/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

LOCAL uint8 *s_dst_buf_ptr =PNULL;



static IMGREF_POINT_T  Src_position_go_up[DISPLAY_NUM+2]={0};
static IMGREF_POINT_T  Src_position_go_down[DISPLAY_NUM+2]={0};
static int s_alpha_array[DISPLAY_NUM+2]={0,255*3/10,255/2,255,255/2,255*3/10,0};

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIWidget_SNSRolling(HTC_IN_PARAM_T * in_htc_data_ptr)
{	
	int i=0;
	int d_zpara=500,current_zpara=0;
	int z_para_up=0,z_para_down=0;
	
	uint32 cost_time=0;
	int32 SinData=0;
	uint32 *virtue_color_up=PNULL;

	uint32 *virtue_color_down=PNULL;
	int current_alpha_array[DISPLAY_NUM+2]={0};
	int icon_gap[3]={10,17,24};
	IMGREF_POINT_T Center_Position={0};
	IMGREF_POINT_T img_mid={0};
	IMGREF_SIZE_T dst_size={0},src_size={0},vir_size={0};
	IMGREF_POINT_T Src_position_current[DISPLAY_NUM+2]={0};
    IMGREF_POINT_T * Src_position_fix=PNULL;
   
    
	if ( PNULL == in_htc_data_ptr
	    || PNULL == in_htc_data_ptr->src_current
	    || PNULL == in_htc_data_ptr->ext_ptr
	    || PNULL == in_htc_data_ptr->virtue_color_down_ptr
	    || PNULL==in_htc_data_ptr->virtue_color_up_ptr)
	{
	    return;
	}
	if((PNULL==in_htc_data_ptr->src_next)&&(SCI_FALSE==in_htc_data_ptr->is_standby))
		{
			return;
		}
	if(0==in_htc_data_ptr->total_frame)
		{
			return;
		}
    
	dst_size.w=in_htc_data_ptr->dst_width;
	dst_size.h=in_htc_data_ptr->dst_height;
	src_size.w=in_htc_data_ptr->src_width;
	src_size.h=in_htc_data_ptr->src_height;
	vir_size.w=in_htc_data_ptr->virtue_color_width;
	vir_size.h=in_htc_data_ptr->virtue_color_height;
	Center_Position.x=in_htc_data_ptr->dst_width/2;
	Center_Position.y=in_htc_data_ptr->dst_height/2;
	img_mid.x=in_htc_data_ptr->virtue_color_width/2;
	img_mid.y=in_htc_data_ptr->virtue_color_width/2;

    if(in_htc_data_ptr->is_standby)
        {
            in_htc_data_ptr->cur_frame=0;
        }
	if(0==in_htc_data_ptr->cur_frame)
		{	Src_position_go_up[in_htc_data_ptr->focus_num].y=Center_Position.y;
			s_alpha_array[in_htc_data_ptr->focus_num]=255;
			for(i=0;i<DISPLAY_NUM+2;i++)
				{
					Src_position_go_up[i].x=Center_Position.x;
					current_zpara=4096-abs(i-in_htc_data_ptr->focus_num)*d_zpara;
					
					if(i<in_htc_data_ptr->focus_num)
						{	Src_position_go_up[i].y=Center_Position.y-(int)(in_htc_data_ptr->src_height/2)-icon_gap[(int)(in_htc_data_ptr->focus_num)-i-1]+(int)((int)((int)(in_htc_data_ptr->src_height)*current_zpara)/(4096*2));
							
						}
					else if(i>in_htc_data_ptr->focus_num)
						{
							Src_position_go_up[i].y=2*Center_Position.y-Src_position_go_up[DISPLAY_NUM+1-i].y;
						}
				}
			s_alpha_array[0]=0;
			s_alpha_array[DISPLAY_NUM+1]=0;
			for(i=0;i<DISPLAY_NUM+2;i++)
				{
					Src_position_go_down[i].x=Src_position_go_up[DISPLAY_NUM+1-i].x;
					Src_position_go_down[i].y=Src_position_go_up[DISPLAY_NUM+1-i].y;
					
				}
			
		}
	if(in_htc_data_ptr->is_go_up)
		{
			Src_position_fix=Src_position_go_up;
        }
    else
        {
            Src_position_fix=Src_position_go_down;
        }
	if((0==in_htc_data_ptr->total_frame)||(in_htc_data_ptr->cur_frame>in_htc_data_ptr->total_frame))
		{
			 return;
		}
	if(in_htc_data_ptr->is_go_up)
		{	
			virtue_color_up=in_htc_data_ptr->virtue_color_up_ptr;
			virtue_color_down=in_htc_data_ptr->virtue_color_down_ptr;
		}
	else
		{
			virtue_color_up=in_htc_data_ptr->virtue_color_down_ptr;
			virtue_color_down=in_htc_data_ptr->virtue_color_up_ptr;
		}
    if(in_htc_data_ptr->is_standby)
        {   //待机状态
             z_para_up=4096;
        	current_alpha_array[in_htc_data_ptr->focus_num]=s_alpha_array[in_htc_data_ptr->focus_num];
        	//pre_alpha=current_alpha_array[in_htc_data_ptr->focus_num-1];
        	Src_position_current[in_htc_data_ptr->focus_num].x=Src_position_fix[in_htc_data_ptr->focus_num].x;
        	Src_position_current[in_htc_data_ptr->focus_num].y=Src_position_fix[in_htc_data_ptr->focus_num].y;
        	img_mid.x=in_htc_data_ptr->src_width/2;
        	img_mid.y=in_htc_data_ptr->src_height/2;
        	cost_time = SCI_GetTickCount();
            
        	S2d_4_in_one_fast_Out32( dst_size,
        								src_size, 
        								Src_position_current[in_htc_data_ptr->focus_num], 
        								img_mid, 
        								0, 
        								in_htc_data_ptr->src_current,
        								in_htc_data_ptr->dst_ptr, 
        								z_para_up,
        								current_alpha_array[in_htc_data_ptr->focus_num],
        								((FIX16_POINT_T *)(in_htc_data_ptr->ext_ptr)));
        								
        	cost_time = SCI_GetTickCount() - cost_time;
        	//SCI_TRACE_LOW:"HTC_EX, cost_time ONE frame = %d ms"
        	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_SNS_349_112_2_18_3_8_35_230,(uint8*)"d", cost_time);
            Src_position_current[in_htc_data_ptr->focus_num+1].x=Src_position_fix[in_htc_data_ptr->focus_num+1].x;
        	Src_position_current[in_htc_data_ptr->focus_num+1].y=Src_position_fix[in_htc_data_ptr->focus_num+1].y;
        	z_para_down=4096-d_zpara;
        	//z_para_down=current_zpara+d_zpara*in_htc_data_ptr->cur_frame/in_htc_data_ptr->total_frame;;
        	current_alpha_array[in_htc_data_ptr->focus_num+1]=s_alpha_array[in_htc_data_ptr->focus_num+1];
		if(in_htc_data_ptr->is_go_up)
			{	
				//stand_buffer=in_htc_data_ptr->virtue_color_down_ptr;
		        	img_mid.x=(int)(in_htc_data_ptr->virtue_color_width/2);
		        	img_mid.y=in_htc_data_ptr->virtue_color_height-(in_htc_data_ptr->src_height/2);
				
				
			}
		else
			{	
				//stand_buffer=in_htc_data_ptr->virtue_color_up_ptr;
				img_mid.x=in_htc_data_ptr->virtue_color_width/2;
		        	img_mid.y=in_htc_data_ptr->src_height/2;
			}
        	
        	cost_time = SCI_GetTickCount();
        	S2d_Alpha_Blending_Out32_EX( dst_size,
        								vir_size, 
        								Src_position_current[in_htc_data_ptr->focus_num+1], 
        								img_mid, 
        								0, 
        								virtue_color_down,
        								in_htc_data_ptr->dst_ptr, 
        								z_para_down,
        								current_alpha_array[in_htc_data_ptr->focus_num+1],
        								((FIX16_POINT_T *)(in_htc_data_ptr->ext_ptr)));
        	
        }
    else
        {
        	SinData=65536*in_htc_data_ptr->cur_frame/in_htc_data_ptr->total_frame;
        	Src_position_current[in_htc_data_ptr->focus_num+1].x=Src_position_fix[in_htc_data_ptr->focus_num+1].x;
        	Src_position_current[in_htc_data_ptr->focus_num+1].y=Src_position_fix[in_htc_data_ptr->focus_num+1].y+(Src_position_fix[in_htc_data_ptr->focus_num].y-Src_position_fix[in_htc_data_ptr->focus_num+1].y)*in_htc_data_ptr->cur_frame/in_htc_data_ptr->total_frame;
        	current_zpara=4096-d_zpara;
        	z_para_down=current_zpara+d_zpara*in_htc_data_ptr->cur_frame/in_htc_data_ptr->total_frame;;
        	current_alpha_array[in_htc_data_ptr->focus_num+1]=s_alpha_array[in_htc_data_ptr->focus_num+1]+(s_alpha_array[in_htc_data_ptr->focus_num]-s_alpha_array[in_htc_data_ptr->focus_num+1])*in_htc_data_ptr->cur_frame/in_htc_data_ptr->total_frame;
        	//current_alpha_array[in_htc_data_ptr->focus_num+1]=255*SinData/(65536);
        	//pre_alpha=current_alpha_array[in_htc_data_ptr->focus_num];
        	img_mid.x=in_htc_data_ptr->src_width/2;
        	img_mid.y=in_htc_data_ptr->src_height/2;
        	cost_time = SCI_GetTickCount();
        	S2d_4_in_one_fast_Out32( dst_size,
        								src_size, 
        								Src_position_current[in_htc_data_ptr->focus_num+1], 
        								img_mid, 
        								0, 
        								in_htc_data_ptr->src_next,
        								in_htc_data_ptr->dst_ptr, 
        								z_para_down,
        								current_alpha_array[in_htc_data_ptr->focus_num+1],
        								((FIX16_POINT_T *)(in_htc_data_ptr->ext_ptr)));
        	cost_time = SCI_GetTickCount() - cost_time;
        	//SCI_TRACE_LOW:"HTC_SLIM, cost_time ONE frame = %d ms"
        	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_SNS_395_112_2_18_3_8_35_231,(uint8*)"d", cost_time);
        	z_para_up=4096-d_zpara*in_htc_data_ptr->cur_frame/in_htc_data_ptr->total_frame;;
        	current_alpha_array[in_htc_data_ptr->focus_num]=s_alpha_array[in_htc_data_ptr->focus_num]+(s_alpha_array[in_htc_data_ptr->focus_num-1]-s_alpha_array[in_htc_data_ptr->focus_num])*in_htc_data_ptr->cur_frame/in_htc_data_ptr->total_frame;;
        	//pre_alpha=current_alpha_array[in_htc_data_ptr->focus_num-1];
        	Src_position_current[in_htc_data_ptr->focus_num].x=Src_position_fix[in_htc_data_ptr->focus_num].x;
        	Src_position_current[in_htc_data_ptr->focus_num].y=Src_position_fix[in_htc_data_ptr->focus_num].y+(Src_position_fix[in_htc_data_ptr->focus_num-1].y-Src_position_fix[in_htc_data_ptr->focus_num].y)*in_htc_data_ptr->cur_frame/in_htc_data_ptr->total_frame;;
        	
        	cost_time = SCI_GetTickCount();
        	S2d_Alpha_Blending_Out32_EX( dst_size,
        								src_size, 
        								Src_position_current[in_htc_data_ptr->focus_num], 
        								img_mid, 
        								0, 
        								in_htc_data_ptr->src_current,
        								in_htc_data_ptr->dst_ptr, 
        								z_para_up,
        								current_alpha_array[in_htc_data_ptr->focus_num],
        								((FIX16_POINT_T *)(in_htc_data_ptr->ext_ptr)));
        	cost_time = SCI_GetTickCount() - cost_time;
        	//SCI_TRACE_LOW:"HTC_EX, cost_time ONE frame = %d ms"
        	SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_SNS_414_112_2_18_3_8_35_232,(uint8*)"d", cost_time);
        }
	//显示出上部半透明被遮挡区域。
	
    for(i=in_htc_data_ptr->focus_num-1;i>=1;i--)
		{   
			Src_position_current[i].x=Src_position_fix[i].x;
			Src_position_current[i].y=Src_position_fix[i].y+(Src_position_fix[i-1].y-Src_position_fix[i].y)*in_htc_data_ptr->cur_frame/in_htc_data_ptr->total_frame;;
			current_zpara=4096-(in_htc_data_ptr->focus_num-i)*d_zpara-d_zpara*in_htc_data_ptr->cur_frame/in_htc_data_ptr->total_frame;;
			if(1==i)
				{
				current_alpha_array[i]=s_alpha_array[i]+(0-s_alpha_array[i])*in_htc_data_ptr->cur_frame/in_htc_data_ptr->total_frame;;
				//pre_alpha=0;
				}
			else
				{
				current_alpha_array[i]=s_alpha_array[i]+(s_alpha_array[i-1]-s_alpha_array[i])*in_htc_data_ptr->cur_frame/in_htc_data_ptr->total_frame;;
				//pre_alpha=current_alpha_array[i-1];
				}
			
			img_mid.x=in_htc_data_ptr->virtue_color_width/2;
			
			img_mid.y=in_htc_data_ptr->src_height/2;
			if(!in_htc_data_ptr->is_go_up)
				{
					img_mid.y=in_htc_data_ptr->virtue_color_height-(in_htc_data_ptr->src_height/2);
				}
			src_size.w=in_htc_data_ptr->virtue_color_width;
			src_size.h=in_htc_data_ptr->virtue_color_height;
			/*
			img_mid.x=in_data_ptr->src_width/2;
			img_mid.y=in_data_ptr->src_height/2;
			*/
			S2d_Alpha_Blending_Out32_EX( dst_size,
										src_size, 
										Src_position_current[i], 
										img_mid, 
										0, 
										virtue_color_up,
										in_htc_data_ptr->dst_ptr, 
										current_zpara,
										current_alpha_array[i],
										((FIX16_POINT_T *)(in_htc_data_ptr->ext_ptr)));
		}
	for(i=(in_htc_data_ptr->focus_num+2);i<=DISPLAY_NUM+1;i++)
		{
			Src_position_current[i].x=Src_position_fix[i].x;
			Src_position_current[i].y=Src_position_fix[i].y+(Src_position_fix[i-1].y-Src_position_fix[i].y)*in_htc_data_ptr->cur_frame/in_htc_data_ptr->total_frame;;
			
			current_zpara=4096-(i-in_htc_data_ptr->focus_num)*d_zpara;
			current_zpara=current_zpara+d_zpara*in_htc_data_ptr->cur_frame/in_htc_data_ptr->total_frame;;
			if(DISPLAY_NUM+1==i)
				{
				current_alpha_array[i]=s_alpha_array[i]+(s_alpha_array[i-1]-s_alpha_array[i])*in_htc_data_ptr->cur_frame/in_htc_data_ptr->total_frame;;
				//pre_alpha=0;
				}
			else
				{
				current_alpha_array[i]=s_alpha_array[i]+(s_alpha_array[i-1]-s_alpha_array[i])*in_htc_data_ptr->cur_frame/in_htc_data_ptr->total_frame;;
				//pre_alpha=current_alpha_array[i+1];
				}
			
			img_mid.x=in_htc_data_ptr->virtue_color_width/2;
			img_mid.y=in_htc_data_ptr->virtue_color_height-(in_htc_data_ptr->src_height/2);
			if(!in_htc_data_ptr->is_go_up)
				{
					img_mid.y=in_htc_data_ptr->src_height/2;
				}
			src_size.w=in_htc_data_ptr->virtue_color_width;
			src_size.h=in_htc_data_ptr->virtue_color_height;	
				/*
			img_mid.x=in_data_ptr->src_width/2;
			img_mid.y=in_data_ptr->src_height/2;
			*/
			S2d_Alpha_Blending_Out32_EX( dst_size,
										src_size, 
										Src_position_current[i], 
										img_mid, 
										0, 
										virtue_color_down,
										in_htc_data_ptr->dst_ptr, 
										current_zpara,
										current_alpha_array[i],
										((FIX16_POINT_T *)(in_htc_data_ptr->ext_ptr)));
			
		}

	


}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DisplaySNSLogInWindow(MMI_WIN_ID_T  win_id)
{
    GUI_RECT_T           win_rect     = {0};
    GUI_LCD_DEV_INFO     lcd_dev_info = {0};
    GUI_POINT_T          point        = {0};
    GUI_RECT_T           loginfo_rect = MMI_SNS_IMG_LOG_RECT;
    GUI_RECT_T           loginfo_rect1 = MMI_SNS_IMG_LOG_RECT;
    GUISTR_STYLE_T       style_ptr    = {0}; 
    MMI_STRING_T         str_ptr      = {0};
    GUISTR_STATE_T       text_state   = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;
    MMI_IMAGE_ID_T         img_id     = 0;
    
    
    if(WIDGET_SNS_SINA_WIN_ID == win_id)
    {
        img_id = IMAGE_ICON_SINA_BLOG;
    }
    else if(WIDGET_SNS_FACEBOOK_WIN_ID == win_id)
    {
        img_id = IMAGE_WIDGETL_SNS_FACEBOOK;
    }
    else
    {
        img_id = IMAGE_WIDGETL_SNS_TWITTER;
    }
    
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);

    if(0 != lcd_dev_info.block_id)
    {
        UILAYER_ClearRect(&lcd_dev_info, win_rect);
    }
    
	point.x = win_rect.left;
	point.y = win_rect.top;

	GUIRES_DisplayImg(&point,
            		  PNULL,
            		  PNULL,
            		  win_id,
            		  IMAGE_WIDGET_SNS_LOGIN_BG,
            		  &lcd_dev_info); 

    point.x = win_rect.left + MMIWIDGET_SNS_LOG_FLAG_LEFT;
    point.y = win_rect.top + MMIWIDGET_SNS_LOG_FLAG_TOP;

    GUIRES_DisplayImg(&point,
            		  PNULL,
            		  PNULL,
            		  win_id,
            		  img_id,
            		  &lcd_dev_info); 
        
    loginfo_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id, loginfo_rect);
    
    if(MMI_TP_DOWN == s_tp_status)
    {
        //登录区域 这里需要加上高亮条
        //IMG_EnableTransparentColor(TRUE);

        GUIRES_DisplayImg(PNULL,
                &loginfo_rect,
                PNULL,                    
                win_id,
                IMAGE_WIDGET_SNS_BGHLIGHT,
                (const GUI_LCD_DEV_INFO*)&lcd_dev_info);

        //IMG_EnableTransparentColor(FALSE);
        
    }
    
    style_ptr.effect = FONT_EFFECT_REVERSE_EDGE; 
    style_ptr.angle  = ANGLE_0;
    style_ptr.align  = ALIGN_HVMIDDLE;
    style_ptr.font   = SONG_FONT_18;

    MMIRES_GetText(TXT_COMM_LOGIN, win_id, &str_ptr);
            
    loginfo_rect1 = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id, loginfo_rect1);
    
    GUISTR_DrawTextToLCDInRect(
                    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                    &loginfo_rect1,
                    &loginfo_rect1,
                    &str_ptr,	   
                    &style_ptr,
                    text_state,
                    GUISTR_TEXT_DIR_AUTO
                    );

    
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : convert time type
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC MMI_RESULT_E MMIWidget_SNSConvertTime(const wchar *ps_time, wchar *pd_time)
{

    if(PNULL == ps_time || PNULL == pd_time || MMIAPICOM_Wstrlen(ps_time) < 12)
    {
        return MMI_RESULT_FALSE; 
    }

    pd_time[0] = ps_time[4];
    pd_time[1] = ps_time[5];
    
    pd_time[2] = '-';
    
    pd_time[3] = ps_time[6];
    pd_time[4] = ps_time[7];

    pd_time[5] = ' ';
    
    pd_time[6] = ps_time[8];
    pd_time[7] = ps_time[9];

    pd_time[8] = ':';
    
    pd_time[9] = ps_time[10];
    pd_time[10] = ps_time[11];

    
    return MMI_RESULT_TRUE; 
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL void MMIWidget_SNSSetBgLayerParam(MMI_WIN_ID_T  win_id)
{
    GUI_RECT_T           win_rect     = {0};
    GUI_LCD_DEV_INFO     lcd_dev_info = {0};
    
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);

    SCI_MEMSET(&bg_layer_buf, 0 ,sizeof(UILAYER_COPYBUF_T));
    SCI_MEMSET(&bg_layer_param, 0 ,sizeof(UILAYER_COPYBUF_PARAM_T));
    //SCI_MEMSET(s_dst_buf_ptr, 0, 4*MMIWIDGET_SNS_BG_WIDTH * MMIWIDGET_SNS_BG_HEIGHT);

    bg_layer_buf.buf_ptr   = (uint8*)s_dst_buf_ptr;
    bg_layer_buf.data_type = DATA_TYPE_ARGB888;
    bg_layer_buf.width     = win_rect.right - win_rect.left +1;
    bg_layer_buf.height    = win_rect.bottom- win_rect.top +1;
    bg_layer_param.rect    = win_rect;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL void MMIWidget_SetWinTextData(MMI_WIN_ID_T  win_id, int index, DPARAM param, GUI_LCD_DEV_INFO *lcd_dev_info,  T_WIDGET_INFO *widget_info)
{        
    GUISTR_STYLE_T    style_ptr                       = {0}; 
    MMI_STRING_T      str_ptr                         = {0};
    wchar             name[MMI_NAME_LEN +1]           = {0}; 
    wchar             date[MMI_DATE_LEN +1]           = {0};
    wchar             text[MMI_TEXT_LEN + 1]          = {0};
    //wchar             widget_string[MMI_DATE_LEN +1]  = {0};
    //GUI_RECT_T        win_rect                        = {0};
    GUI_RECT_T        name_rect                       = MMI_SNS_IMG_NAME_RECT;
    GUI_RECT_T        text_rect                       = MMI_SNS_IMG_TEXT_RECT;
    GUI_RECT_T        falg_rect                       = MMI_SNS_IMG_FALG_RECT;
    GUI_RECT_T        time_rect                       = MMI_SNS_IMG_TIME_RECT;
    GUISTR_STATE_T    detailtext_state                = GUISTR_STATE_EFFECT|GUISTR_STATE_WORDBREAK;
    GUISTR_STATE_T    text_state                      = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;

    if(WIDGET_SNS_SINA_WIN_ID == win_id)
    {
        MMIRES_GetText(TXT_WIDGET_SINA, win_id, &str_ptr);
    }
    else if(WIDGET_SNS_FACEBOOK_WIN_ID == win_id)
    {
        MMIRES_GetText(TXT_COMMON_FACEBOOK, win_id, &str_ptr);
    }
    else 
    {
        MMIRES_GetText(TXT_COMMON_TIWTTER, win_id, &str_ptr);
    }

    style_ptr.effect   = FONT_EFFECT_REVERSE_EDGE; 
    style_ptr.angle    = ANGLE_0;
    //style_ptr.align    = ALIGN_RIGHT;

    //falg 
    style_ptr.font   = SONG_FONT_16;
    
    //falg_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, falg_rect);

    GUISTR_DrawTextToLCDInRect(
                (const GUI_LCD_DEV_INFO *)lcd_dev_info,				
                &falg_rect,
                &falg_rect,
                &str_ptr,	   
                &style_ptr,
                text_state,
                GUISTR_TEXT_DIR_AUTO
                );

    style_ptr.align    = ALIGN_LEFT;
    if(MMIAPICOM_Wstrlen(widget_info->text_info[index].name) == 0)
    {
        style_ptr.effect = FONT_EFFECT_REVERSE_EDGE; 
        style_ptr.angle  = ANGLE_0;
        style_ptr.align  = ALIGN_HVMIDDLE;
        style_ptr.font   = SONG_FONT_16;

        if(param)
        {
            if(TXT_BLANK == *((uint32 *) param))
            {
                MMIRES_GetText(TXT_BLANK, win_id, &str_ptr);
            }
            else
            {
                MMIRES_GetText(TXT_LOADING, win_id, &str_ptr);
            }
        }
        else    
        {
            MMIRES_GetText(TXT_LOADING, win_id, &str_ptr);
        }
        
        //text_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, text_rect);
        GUISTR_DrawTextToLCDInRect(
                (const GUI_LCD_DEV_INFO *)lcd_dev_info,
                &text_rect,
                &text_rect,
                &str_ptr,	   
                &style_ptr,
                detailtext_state,
                GUISTR_TEXT_DIR_AUTO
                );

        //UILAYER_BltLayerToLayer(&lcd_dev_info, &g_SNSWidgetInfo.lcd_dev_info[0], &win_rect, TRUE);
        return ;
    }

    style_ptr.font     = SONG_FONT_18;
    
    //name
    MMIAPICOM_Wstrcpy(name, widget_info->text_info[index].name);
    
    str_ptr.wstr_ptr = name;
    str_ptr.wstr_len = MMIAPICOM_Wstrlen(name);
    
    //name_rect        = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, name_rect);

    GUISTR_DrawTextToLCDInRect(
            (const GUI_LCD_DEV_INFO *)lcd_dev_info,				
            &name_rect,
            &name_rect,
            &str_ptr,	   
            &style_ptr,
            text_state,
            GUISTR_TEXT_DIR_AUTO
            );

    //time   
    style_ptr.font   = MMI_SNS_TIME_FONT;
    if(MMIAPICOM_Wstrlen(widget_info->text_info[index].date) > 0)
    {
        MMIWidget_SNSConvertTime(widget_info->text_info[index].date, date);
        str_ptr.wstr_ptr = date;
        str_ptr.wstr_len = MMIAPICOM_Wstrlen(date);
    
        //time_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, time_rect);

        GUISTR_DrawTextToLCDInRect(
                (const GUI_LCD_DEV_INFO *)lcd_dev_info,				
                &time_rect,
                &time_rect,
                &str_ptr,	   
                &style_ptr,
                text_state,
                GUISTR_TEXT_DIR_AUTO
                );
    }

    //detail text
    style_ptr.font = SONG_FONT_16;

    if(MMIAPICOM_Wstrlen(widget_info->text_info[index].text) > 0)
    {
        MMIAPICOM_Wstrcpy(text, widget_info->text_info[index].text);

        str_ptr.wstr_ptr = text;
        str_ptr.wstr_len = MMIAPICOM_Wstrlen(text);
    
        //text_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP, win_id, text_rect);
        GUISTR_DrawTextToLCDInRect(
                (const GUI_LCD_DEV_INFO *)lcd_dev_info,				
                &text_rect,
                &text_rect,
                &str_ptr,	   
                &style_ptr,
                detailtext_state,
                GUISTR_TEXT_DIR_AUTO
                );
    }
    
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E DisplaySNSWinow(MMI_WIN_ID_T  win_id, DPARAM param)
{
    GUI_RECT_T           win_rect     = {0};
    GUI_LCD_DEV_INFO     lcd_dev_info = {0};
    GUI_POINT_T          point        = {0};
    int                  widget_index = 0;
    T_WIDGET_INFO        *pwidget_info = PNULL;

    if(WIDGET_SNS_SINA_WIN_ID == win_id)
    {
        widget_index = MMI_SNS_SINA;
    }   
    else if(WIDGET_SNS_FACEBOOK_WIN_ID == win_id)
    {
        widget_index = MMI_SNS_FACEBOOK;
    }
    else
    {
        widget_index = MMI_SNS_TIWTTER;
    }  
    
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
    
    if(UILAYER_IsLayerActive(&g_pSNSWidgetInfo->lcd_dev_info[0])&& UILAYER_GetLayerBufferPtr(&g_pSNSWidgetInfo->lcd_dev_info[0]))
    {   
        if(s_dst_buf_ptr == PNULL)
        {
            return MMI_RESULT_FALSE;
        }
        
        pwidget_info = (T_WIDGET_INFO *)SCI_ALLOCA(sizeof(T_WIDGET_INFO));

        if(PNULL == pwidget_info)
        {
            return MMI_RESULT_FALSE;
        }

        SCI_MEMSET((void *)pwidget_info, 0, sizeof(T_WIDGET_INFO));
        
        SCI_MEMCPY(pwidget_info, &g_pSNSWidgetInfo->widget_info[widget_index], sizeof(T_WIDGET_INFO));
    
        UILAYER_Clear(&g_pSNSWidgetInfo->lcd_dev_info[0]);
        //UILAYER_Clear(&g_pSNSWidgetInfo->lcd_dev_info[1]);
        UILAYER_ClearRect(&lcd_dev_info, win_rect);
        
		point.x = 0;
		point.y = 0;
      
        GUIRES_DisplayImg(&point,
            		  PNULL,
            		  PNULL,
            		  win_id,
            		  IMAGE_WIDGET_SNS_BG2,
            		  &g_pSNSWidgetInfo->lcd_dev_info[0]); 
        #if 0
        GUIRES_DisplayImg(&point,
            		  PNULL,
            		  PNULL,
            		  win_id,
            		  IMAGE_WIDGET_SNS_BG2,
            		  &g_pSNSWidgetInfo->lcd_dev_info[1]);

        #endif

        MMIWidget_SetWinTextData(win_id, pwidget_info->curr_index, param, &g_pSNSWidgetInfo->lcd_dev_info[0], pwidget_info);
        
        g_p_in_data_ptr->is_standby   = SCI_TRUE;
        g_p_in_data_ptr->src_current  = (uint32 *)UILAYER_GetLayerBufferPtr(&g_pSNSWidgetInfo->lcd_dev_info[0]);
       // g_p_in_data_ptr->src_next     = (uint32 *)UILAYER_GetLayerBufferPtr(&g_pSNSWidgetInfo->lcd_dev_info[1]);

        SCI_MEMSET(s_dst_buf_ptr, 0, 4*MMIWIDGET_SNS_BG_WIDTH * MMIWIDGET_SNS_BG_HEIGHT);

        MMIWidget_SNSRolling(g_p_in_data_ptr);

        MMIWidget_SNSSetBgLayerParam(win_id);
        UILAYER_CopyBufferToLayer(&lcd_dev_info, &bg_layer_param, &bg_layer_buf);
#if defined TOUCH_PANEL_SUPPORT
        point.x = win_rect.left + MMIWIDGET_SNS_ADD_LEFT;
        point.y = win_rect.top  + MMIWIDGET_SNS_ADD_TOP;

        if(MMI_TP_DOWN == s_tp_status)
        {
#ifdef SNS_PULLING_SUPPORT
            GUIRES_DisplayImg(&point,
                		  PNULL,
                		  PNULL,
                		  win_id,
                		  s_button_id_ary[BUTTON_NEW],
                		  &lcd_dev_info);

            point.x += MMIWIDGET_SNS_ADD_WIDTH;
            GUIRES_DisplayImg(&point,
                		  PNULL,
                		  PNULL,
                		  win_id,
                		  s_button_id_ary[BUTTON_REFRESH],
                		  &lcd_dev_info);

            point.x += MMIWIDGET_SNS_ADD_WIDTH;
            GUIRES_DisplayImg(&point,
                		  PNULL,
                		  PNULL,
                		  win_id,
                		  s_button_id_ary[BUTTON_SETTING],
                		  &lcd_dev_info);            
#else
            GUIRES_DisplayImg(&point,
                		  PNULL,
                		  PNULL,
                		  win_id,
                		  IMAGE_WIDGET_SNS_ADD_HIGHT,
                		  &lcd_dev_info);
#endif //SNS_PULLING_SUPPORT
        }
        else
        {
            GUIRES_DisplayImg(&point,
                		  PNULL,
                		  PNULL,
                		  win_id,
                		  IMAGE_WIDGET_SNS_ADD,
                		  &lcd_dev_info);

#ifdef SNS_PULLING_SUPPORT
            point.x += MMIWIDGET_SNS_ADD_WIDTH;
            GUIRES_DisplayImg(&point,
                        PNULL,
                        PNULL,
                        win_id,
                        IMAGE_WIDGET_SNS_SYNC,
                        &lcd_dev_info);

            point.x += MMIWIDGET_SNS_ADD_WIDTH;
            GUIRES_DisplayImg(&point,
                        PNULL,
                        PNULL,
                        win_id,
                        IMAGE_WIDGET_SNS_SETTING,
                        &lcd_dev_info);            
#endif //SNS_PULLING_SUPPORT
        }
#endif
        SCI_FREE(pwidget_info);
        

    }
	MMIWidget_DrawReleaseParam();
    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL void MMISNS_CreateImageLayer(uint32 *buf_ptr, UILAYER_INFO_T *layer_info_ptr, GUI_LCD_DEV_INFO *layer_handle_ptr, MMI_IMAGE_ID_T image_id, MMI_HANDLE_T win_id)
{
    UILAYER_CREATE_T create_info    = {0}; 
	GUI_POINT_T      display_point  = {0};
    uint8          *layer_buf_ptr   = PNULL;
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
    layer_buf_ptr = UILAYER_GetLayerBufferPtr(layer_handle_ptr);
	//
	SCI_MEMCPY(buf_ptr, layer_buf_ptr, create_info.width*create_info.height*4);

	
	//注销层
	UILAYER_ReleaseLayer(layer_handle_ptr);

}
/*****************************************************************************/
//  Description :  SNS Create layer
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWidget_SNSCreateLayer(MMI_WIN_ID_T  win_id, GUI_LCD_DEV_INFO  *lcd_dev_info, int16 width, int16 height)
{
    UILAYER_CREATE_T    create_curr_info  = {0};  
    MMI_RESULT_E        recode       = MMI_RESULT_FALSE;
    
    create_curr_info.lcd_id          = GUI_MAIN_LCD_ID;
    create_curr_info.owner_handle    = MMK_GetParentWinHandle(win_id);
    create_curr_info.offset_x        = 0;
    create_curr_info.offset_y        = 0;
    create_curr_info.width           = width;
    create_curr_info.height          = height;
    create_curr_info.is_bg_layer     = FALSE;
    create_curr_info.is_static_layer = FALSE;
    create_curr_info.format          = 0;
        
    if(UILAYER_RESULT_SUCCESS == UILAYER_CreateLayer(&create_curr_info, lcd_dev_info))
    {
        if (UILAYER_IsLayerActive(lcd_dev_info))
        {
            UILAYER_Clear(lcd_dev_info);
        }

        recode = MMI_RESULT_TRUE;
    }

    return recode;
}

/*****************************************************************************/
//  Description :  SNS Create rolling buffer
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWidget_SNSCreateRollingBuffer(MMI_WIN_ID_T  win_id)
{
    int                 i            = 0;
    int                 j            = 0;
    uint32              color        = 0;
    
    s_dst_buf_ptr = (uint8 *)SCI_ALLOCA(4*MMIWIDGET_SNS_BG_WIDTH * MMIWIDGET_SNS_BG_HEIGHT);
    
    if(s_dst_buf_ptr == PNULL)
    {
        return MMI_RESULT_FALSE;
    }

    SCI_MEMSET(s_dst_buf_ptr, 0, 4*MMIWIDGET_SNS_BG_WIDTH * MMIWIDGET_SNS_BG_HEIGHT);

    ext_ptr = SCI_ALLOCA(2048*sizeof(FIX16_POINT_T));
    
    if(ext_ptr == PNULL)
    {
        return MMI_RESULT_FALSE;
    }

    SCI_MEMSET(ext_ptr, 0, 2048*sizeof(FIX16_POINT_T));
   
    virtue_color_up_ptr = SCI_ALLOCA(MMIWIDGET_SNS_VIRTUE_WIDTH*MMIWIDGET_SNS_VIRTUE_HEIGHT*sizeof(COLOR32));

    if(virtue_color_up_ptr == PNULL)
    {
        return MMI_RESULT_FALSE;
    }
    
     
    SCI_MEMSET(virtue_color_up_ptr, 0, MMIWIDGET_SNS_VIRTUE_WIDTH*MMIWIDGET_SNS_VIRTUE_HEIGHT*sizeof(COLOR32));
    
    virtue_color_down_ptr = SCI_ALLOCA(MMIWIDGET_SNS_VIRTUE_WIDTH*MMIWIDGET_SNS_VIRTUE_HEIGHT*sizeof(COLOR32));
    
    if(virtue_color_down_ptr == PNULL)
    {
        return MMI_RESULT_FALSE;
    }
    
    SCI_MEMSET(virtue_color_down_ptr, 0, MMIWIDGET_SNS_VIRTUE_WIDTH*MMIWIDGET_SNS_VIRTUE_HEIGHT*sizeof(COLOR32));

    for(i=0;i<MMIWIDGET_SNS_VIRTUE_HEIGHT;i++)
    {	
        color=255/4+255*3*(MMIWIDGET_SNS_VIRTUE_HEIGHT-i)/(4*MMIWIDGET_SNS_VIRTUE_HEIGHT);

        for(j=0;j<MMIWIDGET_SNS_VIRTUE_WIDTH;j++)
    	{	
            virtue_color_down_ptr[(MMIWIDGET_SNS_VIRTUE_HEIGHT-i-1)*MMIWIDGET_SNS_VIRTUE_WIDTH+j]=(0xff000000)|((color<<16)&0x00ff0000)|((color<<8)&0x0000ff00)|((color)&0x000000ff);
            virtue_color_up_ptr[i*MMIWIDGET_SNS_VIRTUE_WIDTH+j]=(0xff000000)|((color<<16)&0x00ff0000)|((color<<8)&0x0000ff00)|((color)&0x000000ff);
    	}
    }

    return MMI_RESULT_TRUE;

}
/*****************************************************************************/
//  Description :  SNS release  rolling buffer
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL void MMIWidget_SNSReleaseRollingBuffer(void)
{    
    if(s_dst_buf_ptr)
    {
        SCI_FREE(s_dst_buf_ptr);
        s_dst_buf_ptr = PNULL;
    }
    if(ext_ptr)
    {
        SCI_FREE(ext_ptr);
        ext_ptr = PNULL;
    }
    
    if(virtue_color_up_ptr)
    {
        SCI_FREE(virtue_color_up_ptr);
        virtue_color_up_ptr = PNULL;
    }
    if(virtue_color_down_ptr)
    {
        SCI_FREE(virtue_color_down_ptr);
        virtue_color_down_ptr = PNULL;
    }
    
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL void MMIWidget_SNSSetRillingParam(MMI_WIN_ID_T  win_id)
{
    MMISNS_CreateImageLayer(virtue_color_up_ptr, &g_sns_ayer_info, &g_sns_layer_handle, IMAGE_ICON_SINA_VIRTUE_UP, win_id);
    MMISNS_CreateImageLayer(virtue_color_down_ptr, &g_sns_ayer_info, &g_sns_layer_handle, IMAGE_ICON_SINA_VIRTUE_DOWN, win_id);

    g_p_in_data_ptr->src_num               = 5;
    g_p_in_data_ptr->focus_num             = 3;
    g_p_in_data_ptr->cur_frame             = 0;
    g_p_in_data_ptr->total_frame           = MMI_TOTAL_FRAME;
    g_p_in_data_ptr->is_go_up              = FALSE;
    g_p_in_data_ptr->ext_ptr               = ext_ptr;
    g_p_in_data_ptr->virtue_color_width    = MMIWIDGET_SNS_VIRTUE_WIDTH;
    g_p_in_data_ptr->virtue_color_height   = MMIWIDGET_SNS_VIRTUE_HEIGHT;
    g_p_in_data_ptr->virtue_color_up_ptr   = virtue_color_up_ptr;
    g_p_in_data_ptr->virtue_color_down_ptr = virtue_color_down_ptr;
    g_p_in_data_ptr->src_width             = MMIWIDGET_SNS_BG2_WIDTH;
    g_p_in_data_ptr->src_height            = MMIWIDGET_SNS_BG2_HEIGHT;
    g_p_in_data_ptr->dst_ptr               = (uint32 *)s_dst_buf_ptr;
    g_p_in_data_ptr->dst_width             = MMIWIDGET_SNS_BG_WIDTH;
    g_p_in_data_ptr->dst_height            = MMIWIDGET_SNS_BG_HEIGHT;
    
    g_p_in_data_ptr->src_current           = (uint32 *)UILAYER_GetLayerBufferPtr(&g_pSNSWidgetInfo->lcd_dev_info[0]);
    g_p_in_data_ptr->src_next              = (uint32 *)UILAYER_GetLayerBufferPtr(&g_pSNSWidgetInfo->lcd_dev_info[1]);
    
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWidget_SNSCreateWin(MMI_WIN_ID_T  win_id)
{    
#if 0
    //if(PNULL == g_pSNSWidgetInfo)
    {
        //g_pSNSWidgetInfo = (MMIWIGET_SNS_INFO_T *)SCI_ALLOCA(sizeof(MMIWIGET_SNS_INFO_T));

        //if(PNULL == g_pSNSWidgetInfo)
        {
         //   return MMI_RESULT_FALSE;
        }

        SCI_MEMSET(g_pSNSWidgetInfo, 0, sizeof(MMIWIGET_SNS_INFO_T));
    }

   // if(PNULL == g_p_in_data_ptr)
    {
       // g_p_in_data_ptr = (HTC_IN_PARAM_T *)SCI_ALLOCA(sizeof(HTC_IN_PARAM_T));

       // if(PNULL == g_p_in_data_ptr)
        {
          //  return MMI_RESULT_FALSE;
        }

        SCI_MEMSET(g_p_in_data_ptr, 0, sizeof(HTC_IN_PARAM_T));
 
    }
#endif
    if(PNULL == g_pSNSWidgetInfo || PNULL == g_p_in_data_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
    SCI_MEMSET(g_pSNSWidgetInfo, 0, sizeof(MMIWIGET_SNS_INFO_T));
    SCI_MEMSET(g_p_in_data_ptr, 0, sizeof(HTC_IN_PARAM_T));
    
    g_pSNSWidgetInfo->lcd_dev_info[0].block_id = 0xffff;
    g_pSNSWidgetInfo->lcd_dev_info[1].block_id = 0xffff;
    
    return MMI_RESULT_TRUE;
}
#if defined TOUCH_PANEL_SUPPORT 
/*****************************************************************************/
//  Description : handle tp down event
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSNSWidgetTpDown(MMI_WIN_ID_T win_id, DPARAM param )
{
    MMI_RESULT_E      recode         = MMI_RESULT_FALSE;
    GUI_POINT_T       point          = {0};
    GUI_POINT_T       win_point      = {0};
    GUI_RECT_T        rect           = MMI_SNS_IMG_BG_RECT;
    GUI_RECT_T        add_rect       = MMI_SNS_IMG_ADD_RECT;
    GUI_RECT_T        loginfo_rect   = MMI_SNS_IMG_LOG_RECT;
    GUI_RECT_T        win_rect       = {0};
    GUI_LCD_DEV_INFO  lcd_dev_info   = {0};
    int               widget_id      = 0;
#ifdef SNS_PULLING_SUPPORT
    GUI_RECT_T        sync_rect       = MMI_SNS_IMG_SYNC_RECT;
    GUI_RECT_T        setting_rect    = MMI_SNS_IMG_SETTING_RECT;
#endif //SNS_PULLING_SUPPORT    

    if(win_id == WIDGET_SNS_SINA_WIN_ID)
    {
        widget_id = MMI_SNS_SINA;
    }
    else if(win_id == WIDGET_SNS_FACEBOOK_WIN_ID)
    {
        widget_id = MMI_SNS_FACEBOOK;
    }
    else 
    {
        widget_id = MMI_SNS_TIWTTER;
    }
	
    
    point.x   = MMK_GET_TP_X(param);
    point.y   = MMK_GET_TP_Y(param);
    
    win_point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);

    //s_tp_status = MMI_TP_DOWN;

    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id, &lcd_dev_info);
    
    if(g_pSNSWidgetInfo->widget_info[widget_id].blogin)//列表界面
    {
   
        if(GUI_PointIsInRect(win_point, add_rect))//新建
        {
            //对图片设置高亮
            add_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id, add_rect);

            IMG_EnableTransparentColor(TRUE);
            
            GUIRES_DisplayImg(PNULL,
                		  &add_rect,
                		  PNULL,
                		  win_id,
                		  IMAGE_WIDGET_SNS_ADD_HIGHT,
                		  (const GUI_LCD_DEV_INFO*)&lcd_dev_info);

            IMG_EnableTransparentColor(FALSE);
            s_tp_status = MMI_TP_DOWN;

#ifdef SNS_PULLING_SUPPORT
            s_button_id_ary[BUTTON_NEW] = IMAGE_WIDGET_SNS_ADD_HIGHT;
            s_button_id_ary[BUTTON_REFRESH] = IMAGE_WIDGET_SNS_SYNC;
            s_button_id_ary[BUTTON_SETTING] = IMAGE_WIDGET_SNS_SETTING;
#endif //SNS_PULLING_SUPPORT
            
        }
#ifdef SNS_PULLING_SUPPORT
        else if (GUI_PointIsInRect(win_point, sync_rect))
        {
            //对图片设置高亮
            sync_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id, sync_rect);

            IMG_EnableTransparentColor(TRUE);
           
            GUIRES_DisplayImg(PNULL,
                            &sync_rect,
                            PNULL,
                            win_id,
                            IMAGE_WIDGET_SNS_SYNC_HIGHT,
                            (const GUI_LCD_DEV_INFO*)&lcd_dev_info);

            IMG_EnableTransparentColor(FALSE);
            s_tp_status = MMI_TP_DOWN;

            s_button_id_ary[BUTTON_NEW] = IMAGE_WIDGET_SNS_ADD;
            s_button_id_ary[BUTTON_REFRESH] = IMAGE_WIDGET_SNS_SYNC_HIGHT;
            s_button_id_ary[BUTTON_SETTING] = IMAGE_WIDGET_SNS_SETTING;

        }
        else if (GUI_PointIsInRect(win_point, setting_rect))
        {
            //对图片设置高亮
            setting_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id, setting_rect);

            IMG_EnableTransparentColor(TRUE);
           
            GUIRES_DisplayImg(PNULL,
                            &setting_rect,
                            PNULL,
                            win_id,
                            IMAGE_WIDGET_SNS_SETTING_HIGHT,
                            (const GUI_LCD_DEV_INFO*)&lcd_dev_info);

            IMG_EnableTransparentColor(FALSE);
            s_tp_status = MMI_TP_DOWN;

            s_button_id_ary[BUTTON_NEW] = IMAGE_WIDGET_SNS_ADD;
            s_button_id_ary[BUTTON_REFRESH] = IMAGE_WIDGET_SNS_SYNC;
            s_button_id_ary[BUTTON_SETTING] = IMAGE_WIDGET_SNS_SETTING_HIGHT;

        }
#endif //SNS_PULLING_SUPPORT
        else if(GUI_PointIsInRect(win_point, rect))
        {
            SCI_TRACE_LOW("renwei --HandleSNSWidgetTpDown g_pSNSWidgetInfo->start_point.y  =%d", g_pSNSWidgetInfo->start_point.y); 
            g_pSNSWidgetInfo->start_point.x = point.x;
            g_pSNSWidgetInfo->start_point.y = point.y;  
        }

        
        recode = MMI_RESULT_TRUE;
    }
    else  //登录页面
    {
        GUISTR_STYLE_T       style_ptr     = {0}; 
        MMI_STRING_T         str_ptr       = {0};
        GUI_RECT_T           loginfo_rect1 = MMI_SNS_IMG_LOG_RECT;
        GUISTR_STATE_T       text_state    = GUISTR_STATE_ALIGN|GUISTR_STATE_SINGLE_LINE|GUISTR_STATE_ELLIPSIS;

        if(GUI_PointIsInRect(win_point, loginfo_rect))//登录BUTTON
        {            
            loginfo_rect = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id, loginfo_rect);
            //IMG_EnableTransparentColor(TRUE);
        
            //display selected bar
            GUIRES_DisplayImg(PNULL,
                    &loginfo_rect,
                    PNULL,                    
                    win_id,
                    IMAGE_WIDGET_SNS_BGHLIGHT,
                    (const GUI_LCD_DEV_INFO*)&lcd_dev_info);

            //IMG_EnableTransparentColor(FALSE);
            s_tp_status = MMI_TP_DOWN;

            style_ptr.effect = FONT_EFFECT_REVERSE_EDGE; 
            style_ptr.angle  = ANGLE_0;
            style_ptr.align  = ALIGN_HVMIDDLE;
            style_ptr.font   = SONG_FONT_18;

            MMIRES_GetText(TXT_COMM_LOGIN, win_id, &str_ptr);

            loginfo_rect1 = MMI_ConvertWinRect(MMICOM_WINPOS_WIN2DISP,win_id, loginfo_rect1);

            GUISTR_DrawTextToLCDInRect(
                    (const GUI_LCD_DEV_INFO *)&lcd_dev_info,
                    &loginfo_rect1,
                    &loginfo_rect1,
                    &str_ptr,	   
                    &style_ptr,
                    text_state,
                    GUISTR_TEXT_DIR_AUTO
                );

            //请稍候
        }
        recode = MMI_RESULT_FALSE;
        
    }

    return recode;
}

/*****************************************************************************/
//  Description : handle tp move event
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSNSWidgetTpMove(MMI_WIN_ID_T win_id, DPARAM param )
{
    GUI_POINT_T  point        = {0};
    int          widget_id    = 0;
    
	point.x = MMK_GET_TP_X(param);
	point.y = MMK_GET_TP_Y(param);

    if(win_id == WIDGET_SNS_SINA_WIN_ID)
    {
        widget_id = MMI_SNS_SINA;
    }
    else if(win_id == WIDGET_SNS_FACEBOOK_WIN_ID)
    {
        widget_id = MMI_SNS_FACEBOOK;
    }
    else 
    {
        widget_id = MMI_SNS_TIWTTER;
    }
    
    SCI_TRACE_LOW("renwei --HandleSNSWidgetTpMove abs(point.x - g_pSNSWidgetInfo->start_point.x) =%d", abs(point.x - g_pSNSWidgetInfo->start_point.x)); 
    SCI_TRACE_LOW("renwei --HandleSNSWidgetTpMove abs(point.y - g_pSNSWidgetInfo->start_point.y) =%d", abs(point.y - g_pSNSWidgetInfo->start_point.y)); 
    SCI_TRACE_LOW("renwei --HandleSNSWidgetTpMove point.y  =%d", point.y); 
    SCI_TRACE_LOW("renwei --HandleSNSWidgetTpMove g_pSNSWidgetInfo->start_point.y =%d", g_pSNSWidgetInfo->start_point.y); 

    if(g_pSNSWidgetInfo->widget_info[widget_id].blogin 
        &&g_pSNSWidgetInfo->widget_info[widget_id].curr_num > 1
        && abs(point.x - g_pSNSWidgetInfo->start_point.x) < MMIWIDGET_SNS_BG_WIDTH/4
        &&abs(point.y - g_pSNSWidgetInfo->start_point.y) > (int)(MMIWIDGET_SNS_BG2_HEIGHT/MMI_TOTAL_FRAME))
    {

        if(UILAYER_IsEmptyLayerHandle(&g_pSNSWidgetInfo->lcd_dev_info[0]))
        {
            if(MMI_RESULT_FALSE == MMIWidget_DrawMoveInitParam(win_id))
            {
                return MMI_RESULT_FALSE;
            }
        }

        if(0 == g_pSNSWidgetInfo->widget_info[widget_id].move_timer_id)
        {
            g_pSNSWidgetInfo->widget_info[widget_id].move_timer_id = MMK_CreateWinTimer(win_id, MMI_SNS_MOVE_TIME, FALSE); 
        }
        
        SCI_TRACE_LOW("renwei --HandleSNSWidgetTpMove move_timer_id =%d", g_pSNSWidgetInfo->widget_info[widget_id].move_timer_id); 

        return MMI_RESULT_TRUE;
    }
    else if(abs(point.x - g_pSNSWidgetInfo->start_point.x) < MMIWIDGET_SNS_BG_WIDTH/4
        &&abs(point.y - g_pSNSWidgetInfo->start_point.y) > MMI_SNS_MOVE_Y2)
    {
        return MMI_RESULT_TRUE;     
    }
       

    return MMI_RESULT_FALSE;
    
}
#endif
/*****************************************************************************/
//  Description : update widget data
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWidget_UpdateWidgetData(int widget_id)
{

    IMMISNS_CreateObject(&pMe);

    if(pMe)
    {
        int index     = 0;
        int cur_index = 0;

        cur_index = g_pSNSWidgetInfo->widget_info[widget_id].curr_index; 
        index     = g_pSNSWidgetInfo->widget_info[widget_id].text_info[cur_index].index;

        SCI_MEMSET(g_pSNSWidgetInfo->widget_info[widget_id].text_info, 0, sizeof(T_WIDGET_DETAIL_DATA)*MMI_WIDGET_TEXT_NUM);
        IMMISNS_WidgetCmdGetData(pMe, widget_id, index, g_direction, (unsigned long int)g_pSNSWidgetInfo->widget_info[widget_id].text_info);
        MMIWidget_GetItemNum(widget_id, (int *)&g_pSNSWidgetInfo->widget_info[widget_id].curr_num);

        IMMISNS_Release(pMe);
        pMe = PNULL;
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : handle wheterh install app
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
PUBLIC MMI_RESULT_E  HandleWidgetWhetherInstallWinMsg(
                                             MMI_WIN_ID_T		win_id, 	
                                             MMI_MESSAGE_ID_E	msg_id,
                                             DPARAM				param
                                             )
{
    MMI_RESULT_E  recode    = MMI_RESULT_TRUE;
    
    switch(msg_id)
    {
        case MSG_APP_WEB:
        case MSG_APP_OK:
        {
            IMMISNS_CreateObject(&pMe);

            if(pMe)
            {
                IMMISNS_AppInstall(pMe, g_widget_id);
                
                IMMISNS_Release(pMe);
                pMe         = PNULL;
            }
            break;
        }
        case MSG_APP_CANCEL:
            break;
        default:
            recode = MMIPUB_HandleAlertWinMsg(win_id,msg_id,param);
            break;
    }
    
    g_widget_id = 0;
    return recode;
}

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWidget_DrawMoveInitParam(MMI_WIN_ID_T win_id)
{
    MMI_RESULT_E  recode_lay1  = MMI_RESULT_TRUE;
    MMI_RESULT_E  recode_lay2  = MMI_RESULT_TRUE;
    MMI_RESULT_E  recode       = MMI_RESULT_TRUE;

    if(UILAYER_IsEmptyLayerHandle(&g_pSNSWidgetInfo->lcd_dev_info[0]))
    {
        recode_lay1  = MMIWidget_SNSCreateLayer(win_id,&g_pSNSWidgetInfo->lcd_dev_info[0],MMIWIDGET_SNS_BG2_WIDTH, MMIWIDGET_SNS_BG2_HEIGHT);
    }
    
    if(UILAYER_IsEmptyLayerHandle(&g_pSNSWidgetInfo->lcd_dev_info[1]))
    {
        recode_lay2  = MMIWidget_SNSCreateLayer(win_id,&g_pSNSWidgetInfo->lcd_dev_info[1],MMIWIDGET_SNS_BG2_WIDTH, MMIWIDGET_SNS_BG2_HEIGHT);
    }
    
    recode = MMIWidget_SNSCreateRollingBuffer(win_id);

    if(MMI_RESULT_FALSE == recode_lay1 || MMI_RESULT_FALSE == recode_lay2 || MMI_RESULT_FALSE == recode)
    {
        MMIWidget_DrawReleaseParam();
        return MMI_RESULT_FALSE;
    }
    
    MMIWidget_SNSSetBgLayerParam(win_id);
    MMIWidget_SNSSetRillingParam(win_id);

    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWidget_DrawInitParam(MMI_WIN_ID_T win_id)
{
    MMI_RESULT_E  recode_lay1  = MMI_RESULT_TRUE;
    MMI_RESULT_E  recode       = MMI_RESULT_TRUE;

    if(UILAYER_IsEmptyLayerHandle(&g_pSNSWidgetInfo->lcd_dev_info[0]))
    {
        recode_lay1  = MMIWidget_SNSCreateLayer(win_id,&g_pSNSWidgetInfo->lcd_dev_info[0],MMIWIDGET_SNS_BG2_WIDTH, MMIWIDGET_SNS_BG2_HEIGHT);
    }
        
    recode = MMIWidget_SNSCreateRollingBuffer(win_id);

    if(MMI_RESULT_FALSE == recode_lay1 || MMI_RESULT_FALSE == recode)
    {
        MMIWidget_DrawReleaseParam();
        return MMI_RESULT_FALSE;
    }
    
    MMIWidget_SNSSetBgLayerParam(win_id);
    MMIWidget_SNSSetRillingParam(win_id);

    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL void MMIWidget_DrawClearParam(void)
{
    UILAYER_Clear(&g_pSNSWidgetInfo->lcd_dev_info[0]);
    UILAYER_Clear(&g_pSNSWidgetInfo->lcd_dev_info[1]);

    if(s_dst_buf_ptr)
    {
        SCI_MEMSET(s_dst_buf_ptr, 0, 4*MMIWIDGET_SNS_BG_WIDTH * MMIWIDGET_SNS_BG_HEIGHT);
    }

    if(ext_ptr)
    {
        SCI_MEMSET(ext_ptr, 0, 2048*sizeof(FIX16_POINT_T));
    }

    if(virtue_color_up_ptr)
    {
        SCI_MEMSET(virtue_color_up_ptr, 0, MMIWIDGET_SNS_VIRTUE_WIDTH*MMIWIDGET_SNS_VIRTUE_HEIGHT*sizeof(COLOR32));
    }

    if(virtue_color_down_ptr)
    {
        SCI_MEMSET(virtue_color_down_ptr, 0, MMIWIDGET_SNS_VIRTUE_WIDTH*MMIWIDGET_SNS_VIRTUE_HEIGHT*sizeof(COLOR32));
    }
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL void MMIWidget_DrawReleaseParam(void)
{
    if(FALSE == UILAYER_IsEmptyLayerHandle(&g_pSNSWidgetInfo->lcd_dev_info[0]))
    {
        UILAYER_RELEASELAYER(&g_pSNSWidgetInfo->lcd_dev_info[0]); /*lint !e506 !e774*/
    }

    if(FALSE == UILAYER_IsEmptyLayerHandle(&g_pSNSWidgetInfo->lcd_dev_info[1]))
    {
        UILAYER_RELEASELAYER(&g_pSNSWidgetInfo->lcd_dev_info[1]); /*lint !e506 !e774*/
    }
    
    MMIWidget_SNSReleaseRollingBuffer();
    SCI_MEMSET(g_p_in_data_ptr, 0, sizeof(HTC_IN_PARAM_T));
}
#if defined TOUCH_PANEL_SUPPORT 
/*****************************************************************************/
//  Description : handle tp up event
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSNSWidgetTpUp(MMI_WIN_ID_T win_id, DPARAM param )
{
    GUI_POINT_T point         = {0};
    GUI_POINT_T point_bg      = {0};
    GUI_POINT_T win_point     = {0};
    GUI_RECT_T  rect          = MMI_SNS_IMG_BG_RECT;
    GUI_RECT_T  add_rect      = MMI_SNS_IMG_ADD_RECT;
    GUI_RECT_T  loginfo_rect  = MMI_SNS_IMG_LOG_RECT;
    MMI_RESULT_E  recode      = MMI_RESULT_FALSE;
    int           widget_id   = 0;  
    uint32        time_num    = 500;
#ifdef SNS_PULLING_SUPPORT
    GUI_RECT_T  sync_rect     = MMI_SNS_IMG_SYNC_RECT;
    GUI_RECT_T  setting_rect  = MMI_SNS_IMG_SETTING_RECT;
#endif //SNS_PULLING_SUPPORT
    
    if(win_id == WIDGET_SNS_SINA_WIN_ID)
    {
        widget_id = MMI_SNS_SINA;
    }
    else if(win_id == WIDGET_SNS_FACEBOOK_WIN_ID)
    {
        widget_id = MMI_SNS_FACEBOOK;
    }
    else 
    {
        widget_id = MMI_SNS_TIWTTER;
    }

    //处理MOVE事件
    if(g_pSNSWidgetInfo->widget_info[widget_id].move_timer_id)
    {
        MMK_StopTimer(g_pSNSWidgetInfo->widget_info[widget_id].move_timer_id);
        g_pSNSWidgetInfo->widget_info[widget_id].move_timer_id = 0;
    }
    
    point.x   = MMK_GET_TP_X(param);
    point.y   = MMK_GET_TP_Y(param);
    win_point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);
    
    if(g_pSNSWidgetInfo->widget_info[widget_id].blogin)
    {
        if(GUI_PointIsInRect(win_point, add_rect) && MMI_TP_DOWN == s_tp_status)
        {
            //进入WRE 新建界面
            IMMISNS_CreateObject(&pMe);

            if(pMe)
            {
                //SCI_TRACE_LOW:"renwei --HandleSNSWidgetTpUp  NewEdit---widget_id=%d "
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_SNS_1451_112_2_18_3_8_37_233,(uint8*)"d",widget_id);
                IMMISNS_WidgetNewEdit(pMe, widget_id);
            }

            MMIPUB_OpenAlertWinByTextId(&time_num,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
#ifdef SNS_PULLING_SUPPORT
        else if (GUI_PointIsInRect(win_point, sync_rect) && 
             (MMI_TP_DOWN == s_tp_status) && 
             (PNULL == pMe))
        {           
            // Refresh sns app
            IMMISNS_CreateObject(&pMe);

            if (0 >= IMMISNS_WidgetRefresh(pMe, widget_id))
            {
                IMMISNS_Release(pMe);
                pMe = PNULL;                
            }
            else
            {
                MMIPUB_OpenAlertWinByTextId(&time_num,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
            }
        }
        else if (GUI_PointIsInRect(win_point, setting_rect) && 
             (MMI_TP_DOWN == s_tp_status))
        {
            // Open setting window 
            MMK_CreateWin((uint32*)WIDGET_SNS_SETTING_WIN_TAB,PNULL);          
        }
#endif //SNS_PULLING_SUPPORT       
        else if(!g_is_rolling && g_pSNSWidgetInfo->widget_info[widget_id].curr_num > 0 && GUI_PointIsInRect(win_point, rect) && abs(g_pSNSWidgetInfo->start_point.y - point.y) < MMI_MOVE_CHANGE_MAX)
        {        
            //进入WRE TEXT界面
            int wre_index = 0;
            int cur_index = 0;
            
            if(s_sns_is_slide)
            {
               s_sns_is_slide = FALSE;
               return MMI_RESULT_TRUE;
            }
            
            cur_index = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;
            
            if(MMIAPICOM_Wstrlen(g_pSNSWidgetInfo->widget_info[widget_id].text_info[cur_index].text) > 0)
            {
                IMMISNS_CreateObject(&pMe);

                if(pMe)
                {   
                    wre_index = g_pSNSWidgetInfo->widget_info[widget_id].text_info[cur_index].index;
                    IMMISNS_WidgetEnterTextDetail(pMe, widget_id, wre_index);
                }
            }
            
            time_num = 1000;
            MMIPUB_OpenAlertWinByTextId(&time_num,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
        }
        else if(g_is_rolling && g_pSNSWidgetInfo->widget_info[widget_id].curr_num > 1 && abs(g_pSNSWidgetInfo->start_point.y - point.y) > (int)(MMIWIDGET_SNS_BG2_HEIGHT/MMI_TOTAL_FRAME))
        {
            uint32              current_frame = 0;
            int                 page_index1   = 0;
            int                 page_index2   = 0;
            GUI_LCD_DEV_INFO    lcd_dev_info  = {0};
            GUI_RECT_T          win_rect      = {0};            
                        
            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);

            if(g_pSNSWidgetInfo->start_point.y > point.y)
            {
                if(g_pSNSWidgetInfo->widget_info[widget_id].curr_index)
                {
                    if(!g_is_reload)
                    {
                        g_pSNSWidgetInfo->widget_info[widget_id].curr_index--;
                    }
                    
                }
                else
                {
                    g_pSNSWidgetInfo->widget_info[widget_id].curr_index = g_pSNSWidgetInfo->widget_info[widget_id].curr_num -1;
                }

                if(g_is_reload)
                {
                    page_index1 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;
                    page_index2 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;

                }
                else
                {
                    page_index1 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index +1;
                    page_index2 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;
                }
                //in_data_ptr.is_go_up  = TRUE; 
            }
            else
            {
                if(g_pSNSWidgetInfo->widget_info[widget_id].curr_index < g_pSNSWidgetInfo->widget_info[widget_id].curr_num -1)
                {
                    if(!g_is_reload)
                    {
                        g_pSNSWidgetInfo->widget_info[widget_id].curr_index++;
                    }       
                }
                else
                {
                    g_pSNSWidgetInfo->widget_info[widget_id].curr_index = 0;
                }

                if(g_is_reload)
                {
                    page_index1 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;
                    page_index2 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;

                }
                else
                {
                    page_index1 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index-1;
                    page_index2 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;
                }
                
                //in_data_ptr.is_go_up  = FALSE; 
            }

            g_p_in_data_ptr->is_go_up = g_is_go_up;

            current_frame = (abs(g_pSNSWidgetInfo->start_point.y - point.y)*MMI_TOTAL_FRAME/MMIWIDGET_SNS_BG2_HEIGHT);

            g_p_in_data_ptr->total_frame  = MMI_TOTAL_FRAME;
            g_p_in_data_ptr->is_standby   = SCI_FALSE;
            
            while(current_frame < MMI_TOTAL_FRAME)
            {
                UILAYER_Clear(&g_pSNSWidgetInfo->lcd_dev_info[0]);
                UILAYER_Clear(&g_pSNSWidgetInfo->lcd_dev_info[1]);
                
                point_bg.x = 0;
                point_bg.y = 0;

                GUIRES_DisplayImg(&point_bg,
                      PNULL,
                      PNULL,
                      win_id,
                      IMAGE_WIDGET_SNS_BG2,
                      &g_pSNSWidgetInfo->lcd_dev_info[0]); 
                GUIRES_DisplayImg(&point_bg,
                      PNULL,
                      PNULL,
                      win_id,
                      IMAGE_WIDGET_SNS_BG2,
                      &g_pSNSWidgetInfo->lcd_dev_info[1]);

                if(s_dst_buf_ptr == PNULL)
                {
                    return MMI_RESULT_FALSE;
                }
                
                SCI_MEMSET(s_dst_buf_ptr, 0, 4*MMIWIDGET_SNS_BG_WIDTH * MMIWIDGET_SNS_BG_HEIGHT);

                MMIWidget_SetWinTextData(win_id, page_index1, param, &g_pSNSWidgetInfo->lcd_dev_info[0], &g_pSNSWidgetInfo->widget_info[widget_id]);
                MMIWidget_SetWinTextData(win_id, page_index2, param, &g_pSNSWidgetInfo->lcd_dev_info[1], &g_pSNSWidgetInfo->widget_info[widget_id]);

                g_p_in_data_ptr->cur_frame  = current_frame;
                MMIWidget_SNSRolling(g_p_in_data_ptr);
                UILAYER_CopyBufferToLayer(&lcd_dev_info, &bg_layer_param, &bg_layer_buf);

                point_bg.x = win_rect.left + MMIWIDGET_SNS_ADD_LEFT;
                point_bg.y = win_rect.top  + MMIWIDGET_SNS_ADD_TOP;
                GUIRES_DisplayImg(&point_bg,
                    PNULL,
                    PNULL,
                    win_id,
                    IMAGE_WIDGET_SNS_ADD,
                    &lcd_dev_info);
                
#ifdef SNS_PULLING_SUPPORT
                point_bg.x += MMIWIDGET_SNS_ADD_WIDTH;
                GUIRES_DisplayImg(&point_bg,
                    PNULL,
                    PNULL,
                    win_id,
                    IMAGE_WIDGET_SNS_SYNC,
                    &lcd_dev_info);    

                point_bg.x += MMIWIDGET_SNS_ADD_WIDTH;
                GUIRES_DisplayImg(&point_bg,
                    PNULL,
                    PNULL,
                    win_id,
                    IMAGE_WIDGET_SNS_SETTING,
                    &lcd_dev_info);                
#endif //SNS_PULLING_SUPPORT                
                
                GUILCD_InvalidateRect(0, win_rect,0);
                
                //SCI_SLEEP(100);
                current_frame ++;
                current_frame ++;
            }
            
            recode = MMI_RESULT_TRUE;
        }
    }
    else
    {
        if(GUI_PointIsInRect(win_point, loginfo_rect))
        {
            IMMISNS_CreateObject(&pMe);

            if(pMe)
            {
                if(IMMISNS_IsFixAppInstalled(pMe, widget_id) < 0)
                {
                    IMMISNS_Release(pMe);
                    pMe = PNULL;
                    
                    g_widget_id = widget_id;
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_WHETHER_INSTALL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_OKCANCEL,HandleWidgetWhetherInstallWinMsg);
                    
                }
                else
                {
                    //IMMISNS_WidgetUserLogIn(pMe, widget_id);
                    //MMIPUB_OpenAlertWinByTextId(&time_num,TXT_WIDGET_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);

                    if(MMI_SNS_SINA == widget_id)
                    {
                        MMIWRE_StartAppEx(WRE_FIX_ID_SINA, NULL);
                    }
                    else if(MMI_SNS_FACEBOOK == widget_id)
                    {
                        MMIWRE_StartAppEx(WRE_FIX_ID_FACEBOOK, NULL);
                    }
                    else if(MMI_SNS_TIWTTER == widget_id)
                    {
                        MMIWRE_StartAppEx(WRE_FIX_ID_TWITTER, NULL);
                    }
                    
                    IMMISNS_Release(pMe);
                    pMe = PNULL;
                }
            }
        }
    }
    
    s_tp_status                   = MMI_TP_NONE;
    g_is_rolling                  = FALSE;
    bfirst_dection                = TRUE;   
    g_is_go_up                    = TRUE;
    g_is_reload                   = FALSE;
    g_pSNSWidgetInfo->start_point.x = 0;
    g_pSNSWidgetInfo->start_point.y = 0;
    
    return recode;
}
#endif
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author:
//  Note: 
/*****************************************************************************/
LOCAL void ResetWidgetSNS(void)
{
    s_tp_status = MMI_TP_NONE;
}

/*****************************************************************************/
//  Description :  Get widget NUM
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E MMIWidget_GetItemNum(int widgetid, int *pCurrNum)
{
    int i        = 0;
    int curr_num = 0;
            
    if(PNULL == pCurrNum || widgetid >= MMI_SNS_MAX)
    {
        return MMI_RESULT_FALSE;
    }

    while(i < MMI_WIDGET_TEXT_NUM)
    {
        if(MMIAPICOM_Wstrlen(g_pSNSWidgetInfo->widget_info[widgetid].text_info[i].name) > 0)
        {
            curr_num ++;
        }
        
        i++;
    }

    *pCurrNum  = curr_num;
     

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description :  srv deal with evt from timer
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetEvtFromTimer(MMI_WIN_ID_T win_id , DPARAM param)
{
    GUI_POINT_T                   point        = {0};
    GUI_POINT_T                   point_bg     = {0};    
    int                           widget_id = 0;
	MMI_IDLE_GRID_DATA_T        *idle_grid_ptr = PNULL;
    
    if(win_id == WIDGET_SNS_SINA_WIN_ID)
    {
        widget_id = MMI_SNS_SINA;
    }
    else if(win_id == WIDGET_SNS_FACEBOOK_WIN_ID)
    {
        widget_id = MMI_SNS_FACEBOOK;
    }
    else 
    {
        widget_id = MMI_SNS_TIWTTER;
    }
    
    if(g_pSNSWidgetInfo->widget_info[widget_id].move_timer_id == *((uint8 *) param))
    {
        GUI_POINT_T      win_point     ={0};
        uint32           current_frame = 0;
        //uint32           total_frame   = MMI_TOTAL_FRAME;
        GUI_RECT_T       rect          = MMI_SNS_IMG_BG_RECT;
        MMI_TP_STATUS_E  state         = MMI_TP_NONE;
        GUI_LCD_DEV_INFO  lcd_dev_info = {0};
        GUI_RECT_T        win_rect     = {0};
        //int index =0;
        
        //处理MOVE事件
        MMK_StopTimer(g_pSNSWidgetInfo->widget_info[widget_id].move_timer_id);
        g_pSNSWidgetInfo->widget_info[widget_id].move_timer_id = 0;

#ifdef TOUCH_PANEL_SUPPORT        
        MMK_GetLogicTPMsg(&state, &point);
#endif
        win_point = MMI_ConvertWinPoint(MMICOM_WINPOS_LCD2WIN, win_id, point);

        if(bfirst_dection)
        {
            if(g_pSNSWidgetInfo->start_point.y > point.y)
            {
                g_is_go_up = TRUE;
            }
            else
            {
                g_is_go_up = FALSE;
            }

            bfirst_dection = FALSE;
        }

        if(GUI_PointIsInRect(win_point, rect))
        {
            int page_index1 = 0;
            int page_index2 = 0;

            if(s_dst_buf_ptr == PNULL)
            {
                return MMI_RESULT_FALSE;
            }

            MMK_GetWinRect(win_id, &win_rect);
            MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
            
            UILAYER_Clear(&g_pSNSWidgetInfo->lcd_dev_info[0]);
            UILAYER_Clear(&g_pSNSWidgetInfo->lcd_dev_info[1]);
            
            point_bg.x = 0;
            point_bg.y = 0;

            GUIRES_DisplayImg(&point_bg,
                  PNULL,
                  PNULL,
                  win_id,
                  IMAGE_WIDGET_SNS_BG2,
                  &g_pSNSWidgetInfo->lcd_dev_info[0]); 
            GUIRES_DisplayImg(&point_bg,
                  PNULL,
                  PNULL,
                  win_id,
                  IMAGE_WIDGET_SNS_BG2,
                  &g_pSNSWidgetInfo->lcd_dev_info[1]);
            


            SCI_MEMSET(s_dst_buf_ptr, 0, 4*MMIWIDGET_SNS_BG_WIDTH * MMIWIDGET_SNS_BG_HEIGHT);
            
            current_frame           = (abs(g_pSNSWidgetInfo->start_point.y - point.y)*MMI_TOTAL_FRAME/(MMIWIDGET_SNS_BG2_HEIGHT));
            if (current_frame >=MMI_TOTAL_FRAME)
            {
                current_frame = MMI_TOTAL_FRAME;
            }
            g_p_in_data_ptr->cur_frame   = current_frame;
            g_p_in_data_ptr->total_frame = MMI_TOTAL_FRAME;
            
            if(g_pSNSWidgetInfo->start_point.y > point.y)
            {

                g_p_in_data_ptr->is_go_up   = TRUE;
                g_direction = MMI_DIRECTION_UP;
                if(g_pSNSWidgetInfo->widget_info[widget_id].curr_index)
                {
                    page_index1 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;
                    page_index2 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index -1;
                    MMIWidget_SetWinTextData(win_id, page_index1, param, &g_pSNSWidgetInfo->lcd_dev_info[0], &g_pSNSWidgetInfo->widget_info[widget_id]);
                    MMIWidget_SetWinTextData(win_id, page_index2, param, &g_pSNSWidgetInfo->lcd_dev_info[1], &g_pSNSWidgetInfo->widget_info[widget_id]);
                }
                else
                {
                    page_index1 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;
                    MMIWidget_SetWinTextData(win_id, page_index1, param, &g_pSNSWidgetInfo->lcd_dev_info[0], &g_pSNSWidgetInfo->widget_info[widget_id]);
                    g_is_reload = TRUE;
                    MMIWidget_UpdateWidgetData(widget_id);
                    g_pSNSWidgetInfo->widget_info[widget_id].curr_index = g_pSNSWidgetInfo->widget_info[widget_id].curr_num -1;

                    page_index2 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;
                    MMIWidget_SetWinTextData(win_id, page_index2, param, &g_pSNSWidgetInfo->lcd_dev_info[1], &g_pSNSWidgetInfo->widget_info[widget_id]);
                }
            }
            else
            {
                g_p_in_data_ptr->is_go_up   = FALSE;
                g_direction = MMI_DIRECTION_DOWN;
                
                if(g_pSNSWidgetInfo->widget_info[widget_id].curr_index < g_pSNSWidgetInfo->widget_info[widget_id].curr_num -1)
                {
                    page_index1 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;
                    page_index2 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index +1;
                    MMIWidget_SetWinTextData(win_id, page_index1, param, &g_pSNSWidgetInfo->lcd_dev_info[0], &g_pSNSWidgetInfo->widget_info[widget_id]);
                    MMIWidget_SetWinTextData(win_id, page_index2, param, &g_pSNSWidgetInfo->lcd_dev_info[1], &g_pSNSWidgetInfo->widget_info[widget_id]);

                }
                else
                {
                    page_index1 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;
                    MMIWidget_SetWinTextData(win_id, page_index1, param, &g_pSNSWidgetInfo->lcd_dev_info[0], &g_pSNSWidgetInfo->widget_info[widget_id]);

                    g_is_reload = TRUE;
                    MMIWidget_UpdateWidgetData(widget_id);
                    g_pSNSWidgetInfo->widget_info[widget_id].curr_index = 0;
                    
                    page_index2 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;
                    MMIWidget_SetWinTextData(win_id, page_index2, param, &g_pSNSWidgetInfo->lcd_dev_info[1], &g_pSNSWidgetInfo->widget_info[widget_id]);
                }
            }
            
            g_p_in_data_ptr->is_standby     = SCI_FALSE;
            
            MMIWidget_SNSRolling(g_p_in_data_ptr);      
            MMIWidget_SNSSetBgLayerParam(win_id);
            UILAYER_CopyBufferToLayer(&lcd_dev_info, &bg_layer_param, &bg_layer_buf);

            point_bg.x = win_rect.left + MMIWIDGET_SNS_ADD_LEFT;
            point_bg.y = win_rect.top  + MMIWIDGET_SNS_ADD_TOP;

            GUIRES_DisplayImg(&point_bg,
                PNULL,
                PNULL,
                win_id,
                IMAGE_WIDGET_SNS_ADD,
                &lcd_dev_info);
            
#ifdef SNS_PULLING_SUPPORT
            point_bg.x += MMIWIDGET_SNS_ADD_WIDTH;
            GUIRES_DisplayImg(&point_bg,
                PNULL,
                PNULL,
                win_id,
                IMAGE_WIDGET_SNS_SYNC,
                &lcd_dev_info);

            point_bg.x += MMIWIDGET_SNS_ADD_WIDTH;
            GUIRES_DisplayImg(&point_bg,
                PNULL,
                PNULL,
                win_id,
                IMAGE_WIDGET_SNS_SETTING,
                &lcd_dev_info);            
#endif //SNS_PULLING_SUPPORT            
            
            GUILCD_InvalidateRect(0, win_rect,0);
            
            g_is_rolling = TRUE;
            g_pSNSWidgetInfo->widget_info[widget_id].move_timer_id = 0;
            g_pSNSWidgetInfo->widget_info[widget_id].move_timer_id = MMK_CreateWinTimer(win_id, MMI_SNS_MOVE_TIME, FALSE); 
        }
    }
    else if(g_pSNSWidgetInfo->widget_info[widget_id].timer_id == *((uint8 *) param))
    {
        //处理数据加载
       IMMISNS_CreateObject(&pMe);

       if(pMe)
       {
            if(g_pSNSWidgetInfo->widget_info[widget_id].curr_num == 0)
            {
                g_direction = MMI_DIRECTION_DOWN;

                SCI_MEMSET(g_pSNSWidgetInfo->widget_info[widget_id].text_info, 0, sizeof(T_WIDGET_DETAIL_DATA)*MMI_WIDGET_TEXT_NUM);
                IMMISNS_WidgetCmdGetData(pMe, widget_id, 0, g_direction, (unsigned long int)g_pSNSWidgetInfo->widget_info[widget_id].text_info);
                IMMISNS_Release(pMe);
                pMe = PNULL;

                MMIWidget_GetItemNum(widget_id, &g_pSNSWidgetInfo->widget_info[widget_id].curr_num);

                idle_grid_ptr = (MMI_IDLE_GRID_DATA_T *)MMIAPIGRID_GetGridPtr();
                if (PNULL == idle_grid_ptr)
                {
                    return MMI_RESULT_FALSE;
                }
                if (idle_grid_ptr->is_editable == 0)
                {
                    if(0 == g_pSNSWidgetInfo->widget_info[widget_id].curr_num)
                    {
                        MMK_SendMsg(win_id, MSG_FULL_PAINT, &empty_str);
                    }
                    else
                    {
                        MMK_SendMsg(win_id, MSG_FULL_PAINT, PNULL);
                    }
                }
                else
                {
                    if(0 == g_pSNSWidgetInfo->widget_info[widget_id].curr_num)
                    {
                        MMK_SendMsg(idle_grid_ptr->win_id, MSG_FULL_PAINT, &empty_str);
                    }
                    else
                    {
                        MMK_SendMsg(idle_grid_ptr->win_id, MSG_FULL_PAINT, PNULL);
                    }

                }
                

                
            }
       }

       MMK_StopTimer(g_pSNSWidgetInfo->widget_info[widget_id].timer_id);
       g_pSNSWidgetInfo->widget_info[widget_id].timer_id = 0;

    }

    return MMI_RESULT_TRUE;

}
/*****************************************************************************/
//  Description :  deal with lose focus
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetLoseFocus(MMI_WIN_ID_T win_id , DPARAM param)
{
    if (!UILAYER_IsEmptyLayerHandle(&g_pSNSWidgetInfo->lcd_dev_info[0]))
    {
        MMIWidget_DrawReleaseParam();
    }
    
    return MMI_RESULT_TRUE;
}
/*****************************************************************************/
//  Description :  deal with get focus
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetGetFocus(MMI_WIN_ID_T win_id , DPARAM param)
{
    int widget_id = 0;
    
    if(win_id == WIDGET_SNS_SINA_WIN_ID)
    {
        widget_id = MMI_SNS_SINA;
    }
    else if(win_id == WIDGET_SNS_FACEBOOK_WIN_ID)
    {
        widget_id = MMI_SNS_FACEBOOK;
    }
    else 
    {
        widget_id = MMI_SNS_TIWTTER;
    }

    if(PNULL == pMe)
    {
        IMMISNS_CreateObject(&pMe);
    }

    if(pMe)
    {
        IMMISNS_CheckUserLogIn(pMe, widget_id, (uint32)&g_pSNSWidgetInfo->widget_info[widget_id].blogin);
        
        IMMISNS_Release(pMe);
        pMe = PNULL;
    }
    
    if(0 == g_pSNSWidgetInfo->widget_info[widget_id].curr_num)
    {
        if(g_pSNSWidgetInfo->widget_info[widget_id].blogin && 0 == g_pSNSWidgetInfo->widget_info[widget_id].timer_id)
        {
            g_pSNSWidgetInfo->widget_info[widget_id].timer_id = MMK_CreateWinTimer(win_id, 1000, FALSE); 
        }
    }

    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description :  srv deal with evt from srv
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetEvtFromWre(MMI_WIN_ID_T win_id , DPARAM param)
{
    T_WIDGET_EVT_RESULT *pRres     = PNULL;
    int                  widget_id = 0;
    
    //SCI_TRACE_LOW:"renwei --HandleWidgetEvtFromWre ---win_id=%d "
    SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_SNS_1980_112_2_18_3_8_38_235,(uint8*)"d",win_id);
    SCI_TRACE_LOW("renwei --HandleWidgetEvtFromWre %d", win_id);
    if(win_id == WIDGET_SNS_SINA_WIN_ID)
    {
        widget_id = MMI_SNS_SINA;
    }
    else if(win_id == WIDGET_SNS_FACEBOOK_WIN_ID)
    {
        widget_id = MMI_SNS_FACEBOOK;
    }
    else 
    {
        widget_id = MMI_SNS_TIWTTER;
    }
    
    if(param)
    {
        pRres = (T_WIDGET_EVT_RESULT *)param;

        SCI_TRACE_LOW("renwei --HandleWidgetEvtFromWre commond_type =%d commond_result =%d", pRres->commond_type,pRres->commond_result); 
        switch(pRres->commond_type)
        {
            case MMI_WIDGET_SNS_COMMOND_LOGIN:
            {
                   
                g_pSNSWidgetInfo->widget_info[pRres->wreapp_id].blogin = pRres->commond_result;

                if(pMe)
                {
                    IMMISNS_Release(pMe);
                    pMe = PNULL;
                } 
                break;
            }
#ifdef SNS_PULLING_SUPPORT
            case MMI_WIDGET_SNS_COMMOND_GET_DATA:
                
                if(pMe)
                {
                    IMMISNS_Release(pMe);
                    pMe = PNULL;
                } 
                MMIWRE_Wakeup_IMMISNS_Release(); 
                break;
#endif //SNS_PULLING_SUPPORT            
            case MMI_WIDGET_SNS_COMMOND_NEW:
            case MMI_WIDGET_SNS_COMMOND_ENTER_DETAIL:
            {
                //SCI_TRACE_LOW:"renwei -HandleWidgetEvtFromWre--ENTER_DETAIL"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_SNS_2015_112_2_18_3_8_38_236,(uint8*)"");
                if(pMe)
                {
                    IMMISNS_Release(pMe);
                    pMe = PNULL;
                }    
                break;
            }
            case WRE_COMMOND_WIDGET_DATA_MODIFY:
            {
                if(FALSE == pRres->commond_result)
                {
                    g_pSNSWidgetInfo->widget_info[widget_id].timer_id      = 0;   
                    g_pSNSWidgetInfo->widget_info[widget_id].move_timer_id = 0;
                    g_pSNSWidgetInfo->widget_info[widget_id].g_direction   = 0;
                    g_pSNSWidgetInfo->widget_info[widget_id].curr_num      = 0;
                    g_pSNSWidgetInfo->widget_info[widget_id].curr_index    = 0;
                    
                    SCI_MEMSET(&g_pSNSWidgetInfo->widget_info[widget_id].text_info[0], 0, sizeof(T_WIDGET_DETAIL_DATA)*MMI_WIDGET_TEXT_NUM);
                }
#ifdef SNS_PULLING_SUPPORT
                else if (SNS_COMMAND_RES_NEWPOST == pRres->commond_result)
                { 
                    MMK_PostMsg(win_id, MSG_GET_FOCUS, PNULL, 0);

                    MAIN_UpdateIdleSnsState(TRUE, widget_id);

                    PlayMsgSound();
                }
                else if (SNS_COMMAND_RES_BG_RUN == pRres->commond_result)
                {
                    MMIPUB_OpenAlertWinByTextId(PNULL,TXT_WIDGET_BUSY,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                }
#endif //SNS_PULLING_SUPPORT                
                
                break;
            }
            case WRE_COMMOND_LOGIN:
            {
                SCI_MEMSET(&g_pSNSWidgetInfo->widget_info[widget_id], 0, sizeof(T_WIDGET_INFO));
                break;
            }
            default:
            {
                //SCI_TRACE_LOW:"renwei -HandleWidgetEvtFromWre--default event"
                SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIWIDGET_SNS_2045_112_2_18_3_8_39_238,(uint8*)"");
                break;
            }
                
        }
    }
    
    return MMI_RESULT_TRUE;
}

/*****************************************************************************/
//  Description : draw widget window 
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWidgetSNSWinDraw(MMI_WIN_ID_T win_id, DPARAM param)
{
    MMI_RESULT_E  recode    = MMI_RESULT_FALSE;
    int           widget_id = 0;
    
    if(win_id == WIDGET_SNS_SINA_WIN_ID)
    {
        widget_id = MMI_SNS_SINA;
    }
    else if(win_id == WIDGET_SNS_FACEBOOK_WIN_ID)
    {
        widget_id = MMI_SNS_FACEBOOK;
    }
    else 
    {
        widget_id = MMI_SNS_TIWTTER;
    }
  
    if(g_pSNSWidgetInfo->widget_info[widget_id].blogin)
    {
        if(UILAYER_IsEmptyLayerHandle(&g_pSNSWidgetInfo->lcd_dev_info[0]))
        {
            recode = MMIWidget_DrawInitParam(win_id);

            if(MMI_RESULT_FALSE == recode)
            {
                return recode;
            }
        }
        
        recode = DisplaySNSWinow(win_id, param);
    }
    else
    {
        recode = DisplaySNSLogInWindow(win_id);
    }


    return recode;
}
#ifdef BAR_PHONE_WIDGET 
/*****************************************************************************/
//  Description : handle MSG_APP_DOWN 
//  Global resource dependence : 
//  Author: guodong.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWidgetAppDown(MMI_WIN_ID_T win_id, DPARAM param)
{
    
    GUI_POINT_T         point_bg      = {0};
    MMI_RESULT_E        recode      = MMI_RESULT_FALSE;
    int                 widget_id   = 0;  
    uint32              current_frame = 0;
    int                 page_index1   = 0;
    int                 page_index2   = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info  = {0};
    GUI_RECT_T          win_rect      = {0};      
    if(win_id == WIDGET_SNS_SINA_WIN_ID)
    {
        widget_id = MMI_SNS_SINA;
    }
    else if(win_id == WIDGET_SNS_FACEBOOK_WIN_ID)
    {
        widget_id = MMI_SNS_FACEBOOK;
    }
    else 
    {
        widget_id = MMI_SNS_TIWTTER;
    }
    if(UILAYER_IsEmptyLayerHandle(&g_pSNSWidgetInfo->lcd_dev_info[0]))
    {
        if(MMI_RESULT_FALSE == MMIWidget_DrawMoveInitParam(win_id))
        {
            return MMI_RESULT_FALSE;
        }
    }
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
    if(g_pSNSWidgetInfo->widget_info[widget_id].curr_index < g_pSNSWidgetInfo->widget_info[widget_id].curr_num -1)
    {
        if(!g_is_reload)
        {
            g_pSNSWidgetInfo->widget_info[widget_id].curr_index++;
        }       
    }
    else
    {
        g_pSNSWidgetInfo->widget_info[widget_id].curr_index = 0;
    }
    
    if(g_is_reload)
    {
        page_index1 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;
        page_index2 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;
        
    }
    else
    {
        page_index1 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index-1;
        page_index2 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;
    }
    g_p_in_data_ptr->is_go_up = g_is_go_up;
    
    current_frame = 1;
    
    g_p_in_data_ptr->total_frame  = MMI_TOTAL_FRAME;
    g_p_in_data_ptr->is_standby   = SCI_FALSE;
    
    while(current_frame < MMI_TOTAL_FRAME)
    {
        UILAYER_Clear(&g_pSNSWidgetInfo->lcd_dev_info[0]);
        UILAYER_Clear(&g_pSNSWidgetInfo->lcd_dev_info[1]);
        
        point_bg.x = 0;
        point_bg.y = 0;
        
        GUIRES_DisplayImg(&point_bg,
            PNULL,
            PNULL,
            win_id,
            IMAGE_WIDGET_SNS_BG2,
            &g_pSNSWidgetInfo->lcd_dev_info[0]); 
        GUIRES_DisplayImg(&point_bg,
            PNULL,
            PNULL,
            win_id,
            IMAGE_WIDGET_SNS_BG2,
            &g_pSNSWidgetInfo->lcd_dev_info[1]);
        
        if(s_dst_buf_ptr == PNULL)
        {
            return MMI_RESULT_FALSE;
        }
        
        SCI_MEMSET(s_dst_buf_ptr, 0, 4*MMIWIDGET_SNS_BG_WIDTH * MMIWIDGET_SNS_BG_HEIGHT);
        
        MMIWidget_SetWinTextData(win_id, page_index1, param, &g_pSNSWidgetInfo->lcd_dev_info[0], &g_pSNSWidgetInfo->widget_info[widget_id]);
        MMIWidget_SetWinTextData(win_id, page_index2, param, &g_pSNSWidgetInfo->lcd_dev_info[1], &g_pSNSWidgetInfo->widget_info[widget_id]);
        
        g_p_in_data_ptr->cur_frame  = current_frame;
        MMIWidget_SNSRolling(g_p_in_data_ptr);
        UILAYER_CopyBufferToLayer(&lcd_dev_info, &bg_layer_param, &bg_layer_buf);
        GUILCD_InvalidateRect(0, win_rect,0);
        
        //SCI_SLEEP(100);
        current_frame ++;
        current_frame ++;
    }
    
    recode = MMI_RESULT_TRUE;
    return recode;
    
    
}
/*****************************************************************************/
//  Description : handle MSG_APP_UP 
//  Global resource dependence : 
//  Author: guodong.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWidgetAppUp(MMI_WIN_ID_T win_id, DPARAM param)
{
    GUI_POINT_T point_bg      = {0};
    MMI_RESULT_E  recode      = MMI_RESULT_FALSE;
    int           widget_id   = 0;  
    uint32              current_frame = 0;
    int                 page_index1   = 0;
    int                 page_index2   = 0;
    GUI_LCD_DEV_INFO    lcd_dev_info  = {0};
    GUI_RECT_T          win_rect      = {0};      
    if(win_id == WIDGET_SNS_SINA_WIN_ID)
    {
        widget_id = MMI_SNS_SINA;
    }
    else if(win_id == WIDGET_SNS_FACEBOOK_WIN_ID)
    {
        widget_id = MMI_SNS_FACEBOOK;
    }
    else 
    {
        widget_id = MMI_SNS_TIWTTER;
    }
    if(UILAYER_IsEmptyLayerHandle(&g_pSNSWidgetInfo->lcd_dev_info[0]))
    {
        if(MMI_RESULT_FALSE == MMIWidget_DrawMoveInitParam(win_id))
        {
            return MMI_RESULT_FALSE;
        }
    }
    MMK_GetWinRect(win_id, &win_rect);
    MMK_GetWinLcdDevInfo(win_id,&lcd_dev_info);
    if(g_pSNSWidgetInfo->widget_info[widget_id].curr_index)
    {
        if(!g_is_reload)
        {
            g_pSNSWidgetInfo->widget_info[widget_id].curr_index--;
        }
        
    }
    else
    {
        g_pSNSWidgetInfo->widget_info[widget_id].curr_index = g_pSNSWidgetInfo->widget_info[widget_id].curr_num -1;
    }
    
    if(g_is_reload)
    {
        page_index1 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;
        page_index2 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;
        
    }
    else
    {
        page_index1 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index +1;
        page_index2 = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;
    }
    g_p_in_data_ptr->is_go_up = g_is_go_up;
    
    current_frame = 1;
    
    g_p_in_data_ptr->total_frame  = MMI_TOTAL_FRAME;
    g_p_in_data_ptr->is_standby   = SCI_FALSE;
    
    while(current_frame < MMI_TOTAL_FRAME)
    {
        UILAYER_Clear(&g_pSNSWidgetInfo->lcd_dev_info[0]);
        UILAYER_Clear(&g_pSNSWidgetInfo->lcd_dev_info[1]);
        
        point_bg.x = 0;
        point_bg.y = 0;
        
        GUIRES_DisplayImg(&point_bg,
            PNULL,
            PNULL,
            win_id,
            IMAGE_WIDGET_SNS_BG2,
            &g_pSNSWidgetInfo->lcd_dev_info[0]); 
        GUIRES_DisplayImg(&point_bg,
            PNULL,
            PNULL,
            win_id,
            IMAGE_WIDGET_SNS_BG2,
            &g_pSNSWidgetInfo->lcd_dev_info[1]);
        
        if(s_dst_buf_ptr == PNULL)
        {
            return MMI_RESULT_FALSE;
        }
        
        SCI_MEMSET(s_dst_buf_ptr, 0, 4*MMIWIDGET_SNS_BG_WIDTH * MMIWIDGET_SNS_BG_HEIGHT);
        
        MMIWidget_SetWinTextData(win_id, page_index1, param, &g_pSNSWidgetInfo->lcd_dev_info[0], &g_pSNSWidgetInfo->widget_info[widget_id]);
        MMIWidget_SetWinTextData(win_id, page_index2, param, &g_pSNSWidgetInfo->lcd_dev_info[1], &g_pSNSWidgetInfo->widget_info[widget_id]);
        
        g_p_in_data_ptr->cur_frame  = current_frame;
        MMIWidget_SNSRolling(g_p_in_data_ptr);
        UILAYER_CopyBufferToLayer(&lcd_dev_info, &bg_layer_param, &bg_layer_buf);
        GUILCD_InvalidateRect(0, win_rect,0);
        
        //SCI_SLEEP(100);
        current_frame ++;
        current_frame ++;
    }
    
    recode = MMI_RESULT_TRUE;
    return recode;
    
    
}
/*****************************************************************************/
//  Description : handle MSG_APP_WEB 
//  Global resource dependence : 
//  Author: guodong.liu
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleWidgetAppWeb(MMI_WIN_ID_T win_id, DPARAM param)
{
    MMI_RESULT_E  recode    = MMI_RESULT_FALSE;
    int           widget_id = 0;
    GUI_POINT_T point_bg      = {0}; 
    uint32        time_num    = 500;
    if(win_id == WIDGET_SNS_SINA_WIN_ID)
    {
        widget_id = MMI_SNS_SINA;
    }
    else if(win_id == WIDGET_SNS_FACEBOOK_WIN_ID)
    {
        widget_id = MMI_SNS_FACEBOOK;
    }
    else 
    {
        widget_id = MMI_SNS_TIWTTER;
    }
    
    if(g_pSNSWidgetInfo->widget_info[widget_id].blogin)
    {  
        
        //进入WRE TEXT界面
        int wre_index = 0;
        int cur_index = 0;
        
        if(s_sns_is_slide)
        {
            s_sns_is_slide = FALSE;
            return MMI_RESULT_TRUE;
        }
        
        cur_index = g_pSNSWidgetInfo->widget_info[widget_id].curr_index;
        
        if(MMIAPICOM_Wstrlen(g_pSNSWidgetInfo->widget_info[widget_id].text_info[cur_index].text) > 0)
        {
            IMMISNS_CreateObject(&pMe);
            
            if(pMe)
            {   
                wre_index = g_pSNSWidgetInfo->widget_info[widget_id].text_info[cur_index].index;
                IMMISNS_WidgetEnterTextDetail(pMe, widget_id, wre_index);
            }
        }
        
        time_num = 1000;
        MMIPUB_OpenAlertWinByTextId(&time_num,TXT_COMMON_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);         
        
        
    }
    else
    {
        
        IMMISNS_CreateObject(&pMe);
        
        if(pMe)
        {
            if(IMMISNS_IsFixAppInstalled(pMe, widget_id) < 0)
            {
                IMMISNS_Release(pMe);
                pMe = PNULL;
                
                g_widget_id = widget_id;
                MMIPUB_OpenAlertWinByTextId(PNULL,TXT_WHETHER_INSTALL,TXT_NULL,IMAGE_PUBWIN_WARNING,PNULL,PNULL,MMIPUB_SOFTKEY_OKCANCEL,HandleWidgetWhetherInstallWinMsg);
                
            }
            else
            {
                //IMMISNS_WidgetUserLogIn(pMe, widget_id);
                //MMIPUB_OpenAlertWinByTextId(&time_num,TXT_WIDGET_WAITING,TXT_NULL,IMAGE_PUBWIN_WAIT,PNULL,PNULL,MMIPUB_SOFTKEY_ONE,PNULL);
                
                if(MMI_SNS_SINA == widget_id)
                {
                    MMIWRE_StartAppEx(WRE_FIX_ID_SINA, NULL);
                }
                else if(MMI_SNS_FACEBOOK == widget_id)
                {
                    MMIWRE_StartAppEx(WRE_FIX_ID_FACEBOOK, NULL);
                }
                else if(MMI_SNS_TIWTTER == widget_id)
                {
                    MMIWRE_StartAppEx(WRE_FIX_ID_TWITTER, NULL);
                }
                
                IMMISNS_Release(pMe);
                pMe = PNULL;
            }
        }
    }
    
    return recode;
}

LOCAL void UpdateSNSSoftkey(MMI_WIN_ID_T win_id)
{
    int           widget_id = 0;
    
    if(win_id == WIDGET_SNS_SINA_WIN_ID)
    {
        widget_id = MMI_SNS_SINA;
    }
    else if(win_id == WIDGET_SNS_FACEBOOK_WIN_ID)
    {
        widget_id = MMI_SNS_FACEBOOK;
    }
    else 
    {
        widget_id = MMI_SNS_TIWTTER;
    }
    
    if(g_pSNSWidgetInfo->widget_info[widget_id].blogin)
    {           
        MMIWIDGET_SoftkeySetTextId(win_id,TXT_VIEW,TRUE);
    }
    else
    {
        MMIWIDGET_SoftkeySetTextId(win_id,TXT_COMM_LOGIN,TRUE);
    }
    
}
#endif
/*****************************************************************************/
//  Description : sns widget window handle evt
//  Global resource dependence : 
//  Author: renwei
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E  HandleWidgetSNSWinMsg(
										   MMI_WIN_ID_T        win_id,        //IN:
										   MMI_MESSAGE_ID_E    msg_id,        //IN:
										   DPARAM            param        //IN:
										   )
{
    MMI_RESULT_E            recode       = MMI_RESULT_FALSE;
    
    switch (msg_id)
    {  
        case MSG_OPEN_WINDOW:
        {
            recode = MMIWidget_SNSCreateWin(win_id);
            break;
        }
        case MSG_WIDGET_PAGE_SLIDE_START:
        {
            s_sns_is_slide = TRUE;
            recode = MMI_RESULT_TRUE;
		    break;
        }
#if defined TOUCH_PANEL_SUPPORT        
        case MSG_WIDGET_TP_PRESS_DOWN:
        {
            recode = HandleSNSWidgetTpDown(win_id, param);
            break;
        }
    	case MSG_WIDGET_TP_PRESS_UP:
        {
            recode = HandleSNSWidgetTpUp(win_id, param);
            break;
        }

    	case MSG_WIDGET_TP_PRESS_MOVE:
            recode = HandleSNSWidgetTpMove(win_id, param);
            break;
#endif        
        case MSG_LOSE_FOCUS:
        {
            ResetWidgetSNS();
            recode = HandleWidgetLoseFocus(win_id, param);
            break;
        }
    	case MSG_GET_FOCUS:
        {
            //recode = MMI_RESULT_TRUE;
            recode = HandleWidgetGetFocus(win_id, param);
            break;
        }
    	case MSG_FULL_PAINT:
        {
            recode = HandleWidgetSNSWinDraw(win_id,param);
#ifdef BAR_PHONE_WIDGET             
            UpdateSNSSoftkey(win_id);
#endif
            break;
        }
        case MSG_TIMER:
        {
            recode = HandleWidgetEvtFromTimer(win_id, param);
            break;
        }
        case MSG_SNS_OPERA_RESULT:
        {
            recode = HandleWidgetEvtFromWre(win_id, param);
            break;
        }
        case MSG_LCD_SWITCH:
        {
            break;
        }
#ifdef BAR_PHONE_WIDGET 
       case MSG_APP_UP:
        {
           recode = HandleWidgetAppUp(win_id, param);                
           break;
        }  
        //break;
       case MSG_APP_DOWN:
        {
           recode = HandleWidgetAppDown(win_id, param);                
           break;
        }  
       case MSG_APP_WEB:
        {
           recode = HandleWidgetAppWeb(win_id, param);  
           UpdateSNSSoftkey(win_id);           
           break;
        }  

        
#endif		
        default:
    		recode = MMI_RESULT_FALSE;
    		break;
    }
    
    return recode;        
}

#ifdef SNS_PULLING_SUPPORT
/*****************************************************************************/
//  Description : sns widget setting window handler
//  Global resource dependence : 
//  Author: david.chen
//  Note: 
/*****************************************************************************/
LOCAL MMI_RESULT_E HandleSnsWidgetSettingWinMsg(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;

    switch(msg_id)
    {
    case MSG_OPEN_WINDOW:
        {
            GUILIST_ITEM_T item = {0};
            GUILIST_ITEM_DATA_T item_data  = {0};

            GUILIST_SetMaxItem(MMIWIDGET_SNS_SETTING_CTRL_ID, MMI_SNS_TIMER_PRESET_MAX, FALSE);
            
            item_data.item_content[0].item_data_type = GUIITEM_DATA_TEXT_ID;
            item.item_data_ptr = &item_data;
            item.item_style = GUIITEM_STYLE_ONE_LINE_RADIO;

            item_data.item_content[0].item_data.text_id = TXT_WIDGET_MANUAL;
            GUILIST_AppendItem(MMIWIDGET_SNS_SETTING_CTRL_ID, &item);

            item_data.item_content[0].item_data.text_id = TXT_WIDGET_5_MIN;
            GUILIST_AppendItem(MMIWIDGET_SNS_SETTING_CTRL_ID, &item);
            
            item_data.item_content[0].item_data.text_id = TXT_WIDGET_15_MIN;
            GUILIST_AppendItem(MMIWIDGET_SNS_SETTING_CTRL_ID, &item);

            item_data.item_content[0].item_data.text_id = TXT_WIDGET_30_MIN;
            GUILIST_AppendItem(MMIWIDGET_SNS_SETTING_CTRL_ID, &item);

            item_data.item_content[0].item_data.text_id = TXT_WIDGET_60_MIN;
            GUILIST_AppendItem(MMIWIDGET_SNS_SETTING_CTRL_ID, &item);
            
            GUILIST_SetSelectedItem(MMIWIDGET_SNS_SETTING_CTRL_ID, MMIWRE_GetSnsPullingPresetType(MMI_SNS_FACEBOOK), TRUE);

            MMK_SetAtvCtrl(win_id, MMIWIDGET_SNS_SETTING_CTRL_ID);
        }
        break;

    case MSG_CTL_CANCEL:
        MMK_CloseWin(win_id);
        break;

    case MSG_CTL_PENOK:
        {
            uint16 sel_index = 0;

            GUILIST_GetSelectedItemIndex(MMIWIDGET_SNS_SETTING_CTRL_ID, &sel_index, MMI_SNS_TIMER_PRESET_MAX);
            
            // Set selected to global and NV
            //if (MMISNS_SaveWidgetTimerSetting(widget_id, sel_index))
            if (MMISNS_SaveWidgetTimerSetting(MMI_SNS_FACEBOOK, sel_index))
            {
                // Stop current timer
                MMIWRE_StopSnsPullingTimer(MMI_SNS_FACEBOOK);

                // Setup timer with new setting
                //s_timer_preset_type[MMI_SNS_FACEBOOK] = sel_index;
                //MMIWRE_SetSnsPullingTimer(MMI_SNS_FACEBOOK, s_timer_time_tbl[sel_index]);
                MMIWRE_SetSnsPullingPresetTimer(MMI_SNS_FACEBOOK, sel_index);

                
                // Start current timer
                //if (MMI_SNS_TIMER_PRESET_OFF != s_timer_preset_type[MMI_SNS_FACEBOOK])
                if (MMI_SNS_TIMER_PRESET_OFF != MMIWRE_GetSnsPullingPresetType(MMI_SNS_FACEBOOK))
                {
                    MMIWRE_StartSnsPullingTimer(MMI_SNS_FACEBOOK);
                }

                // Prompt user success
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMPLETE, TXT_NULL, IMAGE_PUBWIN_SUCCESS, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
            else
            {
                // Prompt user fail
                MMIPUB_OpenAlertWinByTextId(PNULL, TXT_ERROR, TXT_NULL, IMAGE_PUBWIN_FAIL, PNULL, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
            }
        }
        break;

    case MSG_CLOSE_WINDOW:
        // Release resource here
        break;
    default:
        break;
    }

    return result;
}

/*****************************************************************************/
//  Description : callback to PlayMsgSound function
//  Global resource dependence : 
//  Author: david.chen
//  Note: 
/*****************************************************************************/
LOCAL void PlaySoundCallBack(MMISRVAUD_REPORT_RESULT_E result, DPARAM param)
{
    MMI_WIN_ID_T win_id = WIDGET_SNS_ALERT_WIN_ID;

    if (MMISRVAUD_REPORT_RESULT_NOT_SUPPORT == result)
    {     
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMMON_NO_SUPPORT, TXT_NULL, IMAGE_PUBWIN_WARNING, &win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
    }
    else if ((MMISRVAUD_REPORT_RESULT_ERROR == result))
    {      
        MMIPUB_OpenAlertWinByTextId(PNULL, TXT_COMMON_NO_SUPPORT, TXT_NULL, IMAGE_PUBWIN_WARNING, &win_id, PNULL, MMIPUB_SOFTKEY_ONE, PNULL);
    }

    MMIAPISET_SetVibratorEx(FALSE,MMISET_RING_TYPE_MSG,FALSE,0);
}

/*****************************************************************************/
//  Description : play message notification sound
//  Global resource dependence : 
//  Author: david.chen
//  Note: 
/*****************************************************************************/
LOCAL void PlayMsgSound(void)
{
    MMISET_MSG_RING_TYPE_E ring_type = MMISET_MSG_RING;
    MN_DUAL_SYS_E dual_sys  = MN_DUAL_SYS_1;//MMIAPIAUT_GetSocketSelectSIM();

    MMIDEFAULT_SetBackLight(TRUE);

    MMIAPISET_SetVibratorEx(FALSE,MMISET_RING_TYPE_MSG,FALSE,0);
    MMIAPISET_StopRing(MMISET_RING_TYPE_MSG);

    //有新的需要播放铃声，把原来的播放铃声先停掉。
    ring_type = (MMISET_MSG_RING_TYPE_E)MMIAPIENVSET_GetActiveModeOptValue(dual_sys, MSG_RING_TYPE);
    
    if (!MMIDEFAULT_IsPermitPlayNewMsgRing())
    {
        if(MMISET_MSG_RING == ring_type || MMISET_MSG_VIBRA_AND_RING == ring_type)
        {
            ring_type = MMISET_MSG_VIBRA;
        }
    }
    if (MMIAPICC_IsInState(CC_CALL_CONNECTED_STATE) && !MMIAPIUDISK_UdiskIsRun())
    {
        SCI_TRACE_LOW("MMS: PlayMMSRing normal call mode");
        MMIAPISET_PlayRing(dual_sys, FALSE,0,1,MMISET_RING_TYPE_MSG_IN_CALL,PlaySoundCallBack);
    }
    else 
    {
        SCI_TRACE_LOW("MMS: MMIMMS_PlayMMSSound ring_type=%d", ring_type);

#ifdef JAVA_SUPPORT
        if(MMIAPIJAVA_IsFocusJAVARunWin())
        {
            ring_type = MMISET_MSG_VIBRA;
        }
#endif

        if(WRE_IsFocusRunWin())
        {
            ring_type = MMISET_MSG_VIBRA;
        }
        
        switch( ring_type )
        {
        case MMISET_MSG_RING:
            //MMIAUDIO_HandleNewMsgRing();
            MMIAPISET_PlayRing(dual_sys, FALSE,0,1,MMISET_RING_TYPE_MSG,PlaySoundCallBack);
            if (GPIO_CheckHeadsetStatus())     //耳机插入时启动振动
            {
                MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,MMISNS_VIBRA_TIMEROUT);
            }
            break;
            
        case MMISET_MSG_VIBRA:
            if(MMIAPISET_IsPermitPlayRing(MMISET_RING_TYPE_MSG))
            {
                MMIAPISET_StopAllVibrator();
                MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,MMISNS_VIBRA_TIMEROUT);
            }
            break;
            
        case MMISET_MSG_SILENT:
            break;
            
        case MMISET_MSG_VIBRA_AND_RING:
            if (MMIAPISET_IsPermitPlayRingForVibrate(MMISET_RING_TYPE_MSG))
            {
                if ( 0 == MMIAPIENVSET_GetCurModeOptValue(dual_sys, MSG_RING_VOL) )
                {
                    //MMIAUDIO_Vibrate(MMISNS_VIBRA_TIMEROUT, FALSE);
                    MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,MMISNS_VIBRA_TIMEROUT);
                }
                else
                {
                    //MMIAUDIO_HandleNewMsgRing();
                    MMIAPISET_PlayRing(dual_sys, FALSE,0,1,MMISET_RING_TYPE_MSG, PlaySoundCallBack);
                    MMIAPISET_SetVibratorEx(TRUE,MMISET_RING_TYPE_MSG,FALSE,MMISNS_VIBRA_TIMEROUT);
                }
            }
            break;
            
        default:
            break;
        }
    }
}
#endif //SNS_PULLING_SUPPORT

#endif
