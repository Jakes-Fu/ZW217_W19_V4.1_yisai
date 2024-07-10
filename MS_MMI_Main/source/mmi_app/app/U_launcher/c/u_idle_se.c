/*****************************************************************************
** File Name:      u_idle_se.c                                       *
** Author: kyle.jin         kyle                                              *
** Date:           14/02/2012                                                *
** Copyright:      2004 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe go menu se                *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2012      kyle            Creat
******************************************************************************/
#if defined(PDA_UI_SUPPORT_U_IDLE)
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_menu_trc.h"
#include "u_launcher_internal.h"
#include "u_launcher_export.h"
#include "special_effect.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                              STRUCT DEFINITION                        *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERN VARIABLE DEFINITION                        *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
 
/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLE DEFINITION                        *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : 斜面计算函数
//  Global resource dependence : 
//  Author: ANSON
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIdle_U_Effect_Slope_Type_Calc(		GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
																		GO_MAINMENU_PAGE_INFO_T *next_page_info,//下一页信息
																		GO_MAINMENU_PAGE_INFO_T *third_page_info,
																		uint16 cur_frame_num,//当前帧
																		uint16 total_frame_num,//总帧数   暂定20
																		IMGREF_SIZE_T bg_window_size,//屏幕尺寸
																		uint32* bg_window_buf,//背景buffer
																		GO_MOVE_DIRECTION_E direction,//移动方向
																		FIX16_POINT_T* calc_point_buf,
																		GO_CALC_DATA_INFO_T* calc_data_info //申请计算所需的BUFFER
																		);
/*****************************************************************************/
//  Description : 计算函数
//  Global resource dependence : 
//  Author: Ben
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIdle_U_Effect_Bookpage_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
																GO_MAINMENU_PAGE_INFO_T *next_page_info,//下一页信息
																GO_MAINMENU_PAGE_INFO_T *third_page_info,
																uint16 cur_frame_num,//当前帧
																uint16 total_frame_num,//总帧数   暂定20
																IMGREF_SIZE_T bg_window_size,//屏幕尺寸
																uint32* bg_window_buf,//背景buffer
																GO_MOVE_DIRECTION_E direction,//移动方向
																FIX16_POINT_T* calc_point_buf,
																GO_CALC_DATA_INFO_T* calc_data_info //申请计算所需的BUFFER
																);
/*****************************************************************************/
//  Description : 计算函数
//  Global resource dependence : 
//  Author: Ben
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIdle_U_Effect_MidRotate_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
													GO_MAINMENU_PAGE_INFO_T *next_page_info,//下一页信息
													GO_MAINMENU_PAGE_INFO_T *third_page_info,
													uint16 cur_frame_num,//当前帧
													uint16 total_frame_num,//总帧数   暂定20
													IMGREF_SIZE_T bg_window_size,//屏幕尺寸
													uint32* bg_window_buf,//背景buffer
													GO_MOVE_DIRECTION_E direction,//移动方向
													FIX16_POINT_T* calc_point_buf,
													GO_CALC_DATA_INFO_T* calc_data_info //申请计算所需的BUFFER
													);
/*****************************************************************************/
//  Description : 计算函数
//  Global resource dependence : 
//  Author: Ben
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIdle_U_Effect_SmallItems_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
													GO_MAINMENU_PAGE_INFO_T *next_page_info,//下一页信息
													GO_MAINMENU_PAGE_INFO_T *third_page_info,
													uint16 cur_frame_num,//当前帧
													uint16 total_frame_num,//总帧数   暂定20
													IMGREF_SIZE_T bg_window_size,//屏幕尺寸
													uint32* bg_window_buf,//背景buffer
													GO_MOVE_DIRECTION_E direction,//移动方向
													FIX16_POINT_T* calc_point_buf,//申请计算所需的BUFFER
													GO_CALC_DATA_INFO_T* calc_data_info 
													);
/*****************************************************************************/
//  Description : 计算函数
//  Global resource dependence : 
//  Author: Ben
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIdle_U_Effect_Classic_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
													GO_MAINMENU_PAGE_INFO_T *next_page_info,//下一页信息
													GO_MAINMENU_PAGE_INFO_T *third_page_info,
													uint16 cur_frame_num,//当前帧
													uint16 total_frame_num,//总帧数   暂定20
													IMGREF_SIZE_T bg_window_size,//屏幕尺寸
													uint32* bg_window_buf,//背景buffer
													GO_MOVE_DIRECTION_E direction,//移动方向
													FIX16_POINT_T* calc_point_buf,
													GO_CALC_DATA_INFO_T* calc_data_info //申请计算所需的BUFFER
													);
/*****************************************************************************/
//  Description : 计算函数
//  Global resource dependence : 
//  Author: Ben
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIdle_U_Effect_Boxrot_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
																GO_MAINMENU_PAGE_INFO_T *next_page_info,//下一页信息
																GO_MAINMENU_PAGE_INFO_T *third_page_info,
																uint16 cur_frame_num,//当前帧
																uint16 total_frame_num,//总帧数   暂定20
																IMGREF_SIZE_T bg_window_size,//屏幕尺寸
																uint32* bg_window_buf,//背景buffer
																GO_MOVE_DIRECTION_E direction,//移动方向
																FIX16_POINT_T* calc_point_buf,
																GO_CALC_DATA_INFO_T* calc_data_info //申请计算所需的BUFFER
																);
/*****************************************************************************/
//  Description :是否执行光照效果
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DoShiningEffect(void);

//每种特效的特殊信息
LOCAL const GO_CALC_STYLE_INFO_T g_uidle_effect_type_calc_table[UIDLE_STYLE_MAX] = 
{
#ifdef UIDLE_CLASSIC_SE_SUPPORT  
     {(go_calc_func)MMIIdle_U_Effect_Classic_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif 
#ifdef UIDLE_SLOPE_SE_SUPPORT  
     {(go_calc_func)MMIIdle_U_Effect_Slope_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif 
#ifdef UIDLE_BOXROT_SE_SUPPORT  
     {(go_calc_func)MMIIdle_U_Effect_Boxrot_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif 
#ifdef UIDLE_BOOKPAGE_SE_SUPPORT  
     {(go_calc_func)MMIIdle_U_Effect_Bookpage_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif 
#ifdef UIDLE_MIDROTATE_SE_SUPPORT  
     {(go_calc_func)MMIIdle_U_Effect_MidRotate_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif 
#ifdef UIDLE_SMALLITEMS_SE_SUPPORT  
     {(go_calc_func)MMIIdle_U_Effect_SmallItems_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif 
};
/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description :获取算法callback
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
PUBLIC go_calc_func UAPIIdle_GetCallBackfunc(UIDLE_STYLE_E style_index)
{
    if(style_index>=UIDLE_STYLE_MAX)
    {
        return PNULL;
    }
    
    return g_uidle_effect_type_calc_table[style_index].calc_func;
}
/*****************************************************************************/
//  Description :获取算法STYLE slide type
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
PUBLIC GUIMENU_GO_SLIDE_TYPE_E UAPIIdle_GetCalcStyleSlideType(UIDLE_STYLE_E style_index)
{
    if(style_index>=UIDLE_STYLE_MAX)
    {
        return GO_SLIDE_SUPPORT_ONLY_X;
    }
    
    return g_uidle_effect_type_calc_table[style_index].slide_type;
}
/*****************************************************************************/
//  Description :获取算法STYLE Total frame
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
PUBLIC uint16 UAPIIdle_GetCalcStyleCalcTime(UIDLE_STYLE_E style_index)
{
    if(style_index>=UIDLE_STYLE_MAX)
    {
        return GO_DEFAULT_COEFFCIENT;
    }
    
    return g_uidle_effect_type_calc_table[style_index].calc_time;
}
/*****************************************************************************/
//  Description :获取算法STYLE信息
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
PUBLIC GO_CALC_STYLE_INFO_T* UAPIIdle_GetCalcStyleInfo(UIDLE_STYLE_E style_index)
{
    if(style_index>=UIDLE_STYLE_MAX)
    {
        return PNULL;
    }
    
    return &g_uidle_effect_type_calc_table[style_index];
}
/*****************************************************************************/
//  Description :是否执行光照效果
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DoShiningEffect(void)
{
    return FALSE;
}
#ifdef UIDLE_SLOPE_SE_SUPPORT  
/*****************************************************************************/
//  Description : 斜面特效执行函数，包含是否打光特效
//  Global resource dependence : 
//  Author: BENNY
//  Note:
/*****************************************************************************/
LOCAL void Make_Rotate_Slope_shining(GO_MOVE_DIRECTION_E direction,
										uint16 cur_frame,//当前帧
										uint16 total_frame,//总帧数   暂定20
										COLOR32 * tempColor, 
										GO_ROTA_DATA_T img_data,
										GO_MAINMENU_ICON_INFO_T thumb,  
										IMGREF_SIZE_T icon_size,
										IMGREF_SIZE_T win_size, 
										int angle,
										GO_CALC_DATA_INFO_T* calc_data_info)
{   
    S3D_ROTATION_GEOMETRY_T  s3d_geometry={0};
    S3D_ROTATION_IMG_T   s3d_src_img={0};
    S3D_ROTATION_INOUT_IMG_INFO_T  s3d_img={0};
    LIGHT_SHINING_T ls_para = {0};
    FIX16_POINT_T point1 = {0};//st
    FIX16_POINT_T point2 = {0};//ed
    
    int32 radius = 140;
    COLOR32 colorSet = 0xdfffffff;
    int32 x_temp = 0;
    int32 x_delta_max = 480;
    int32 angle_temp= 0;
    int32 angle_max= 400;
    int32 frame_runing = total_frame*2/5;
	int32 x_gap = 90, x_slopeCtrl = 300;
    
    if(0==total_frame)
        return;

	if(GO_MOVE_LEFT== direction)
    {
        point1.x = -x_gap;
	    point1.y = 0;
	    point2.x = point1.x-x_slopeCtrl;
	    point2.y = win_size.h;
	    
	    if(cur_frame<=frame_runing)
	    {
	        angle_temp = cur_frame*angle_max/frame_runing;
	    }
	    else if((total_frame-cur_frame)<=frame_runing)
	    {
	        angle_temp = (total_frame-cur_frame)*angle_max/frame_runing;
	    }
	    else
	    {
	        angle_temp = angle_max;
	    }
	    
	    x_temp = angle_temp*x_delta_max/angle_max;
	    
	    point1.x += x_temp;
	    point2.x += x_temp;         
    }
    else if(GO_MOVE_RIGHT== direction)
    {
        point1.x = win_size.w+x_gap;
	    point1.y = 0;
	    point2.x = point1.x+x_slopeCtrl;
	    point2.y = win_size.h;
	    
	    if(cur_frame<=frame_runing)
	    {
	        angle_temp = cur_frame*angle_max/frame_runing;
	    }
	    else if((total_frame-cur_frame)<=frame_runing)
	    {
	        angle_temp = (total_frame-cur_frame)*angle_max/frame_runing;
	    }
	    else
	    {
	        angle_temp = angle_max;
	    }
	    
	    x_temp = angle_temp*x_delta_max/angle_max;
	    
	    point1.x -= x_temp;
	    point2.x -= x_temp;  
    }
    else
    {
        return;
    }
    
    s3d_geometry.img_size.w=icon_size.w;
    s3d_geometry.img_size.h=icon_size.h;
    s3d_geometry.img_size.x =0;
    s3d_geometry.img_size.y = 0;
    
    s3d_geometry.angle=angle;
    s3d_geometry.eye.x=win_size.w/2;
    s3d_geometry.eye.y=win_size.h/2;
    s3d_geometry.eye.z=-2*win_size.h;
    
    s3d_geometry.axis.x=img_data.axis_x;
    s3d_geometry.axis.y=img_data.axis_y;
    s3d_geometry.axis.z=img_data.axis_z;
    s3d_geometry.axis.axis=1;
    s3d_geometry.is_axis_use_default = SCI_FALSE;
    s3d_geometry.is_eye_use_default = SCI_FALSE;
    
    s3d_src_img.src_ptr=thumb.icon_buf;

    s3d_src_img.src_type=calc_data_info->input_type;
    s3d_img.color_type=calc_data_info->output_type;
    
    s3d_img.color_ptr=tempColor;
    s3d_img.size.h=win_size.h;
    s3d_img.size.w=win_size.w;
    
    ls_para.color = colorSet;
    ls_para.ed_point = &point2;
    ls_para.st_point = &point1;
    ls_para.rds = ((radius*s2d_cos(angle_max-angle_temp))>>16);
    if(DoShiningEffect())
        {
            S3D_AxisMidRotation_Img_shining( &(s3d_geometry),&(s3d_src_img),&(s3d_img),255,&ls_para,calc_data_info->param);
        }
       else
        {
            S3D_AxisMidRotation_Img( &(s3d_geometry),&(s3d_src_img),&(s3d_img),255,calc_data_info->param);
    }
}


/*****************************************************************************/
//  Description : 计算帧数据函数
//  Global resource dependence : 
//  Author: ANSON
//  Note:
/*****************************************************************************/
LOCAL void Calc_Frame_Data(GO_MAINMENU_PAGE_INFO_T * page_info,
                            COLOR32 * out_buff,
                            IMGREF_SIZE_T win_size,
                            int c_frame,
                            int total_frame,
                            GO_MOVE_DIRECTION_E direction,
                            int absolute_gap,
                            GO_CALC_DATA_INFO_T* calc_data_info
                            )
{  	 
    int i=0,j=0,temp=0,rotate_frame=0,max_angle=400,step_frame=0,distance=0,axis_angle=0;
    GO_ROTA_DATA_T  result={0};
    IMGREF_SIZE_T icon_size = {0};
    //rotate_frame 等于总帧数的三分之一，旋转三分之一的时间平移三分之一的时间。
    
    rotate_frame=total_frame*1/3;
    //根据帧数确定角度。
    if(0==rotate_frame)
    {
        return ;
    }
    if(c_frame<=rotate_frame)
    {
        axis_angle=max_angle*c_frame/rotate_frame;
    }
    else if((c_frame>rotate_frame)&&(c_frame<=(total_frame-rotate_frame)))
    {
        axis_angle=max_angle;
    }
    else if((c_frame>(total_frame-rotate_frame))&&(c_frame<=total_frame))
    {	
        step_frame=c_frame-(total_frame-rotate_frame);
        axis_angle=max_angle*(rotate_frame-step_frame)/rotate_frame;
    }
    if(DoShiningEffect())
    {
        if(c_frame<=rotate_frame)
        {
            distance = 0;
        }
        else if(c_frame>=total_frame - rotate_frame)
        {
            distance=win_size.w;
        }
        else
        {
            distance=win_size.w*(c_frame-rotate_frame)/(total_frame-rotate_frame*2);
        }
    }
    else
    {
        distance=win_size.w*c_frame/total_frame;
    }
    //distance=win_size.w*c_frame/total_frame;
    
    
    //axis_angle=axis_angle;
    //确定每个图标的位置，这里用到了absolute_gap来区分页数。
    if(GO_MOVE_RIGHT==direction)
    {     
        axis_angle=-(axis_angle);
        distance=distance-absolute_gap*win_size.w;
    }
    else if(GO_MOVE_LEFT==direction)
    {	
        distance=-distance+absolute_gap*win_size.w;
    }
    for(i = 0; i < page_info->icon_num; ++i)
    {        
        temp=i;  
        if(PNULL!=(*(page_info->icon_info+temp)).icon_buf)
        {		
            icon_size.w = (page_info->icon_info+temp)->icon_size.w;
            icon_size.h = (page_info->icon_info+temp)->icon_size.h;
            //确定小图标转轴信息，主要根据他的左上点左边和旋转角度。
            if((axis_angle)>=0)
            {		
                result.axis_x= (((page_info->icon_info+temp)->icon_offset.x +icon_size.w/2+distance)*s2d_cos(axis_angle))>>16;
                result.axis_y=(page_info->icon_info+temp)->icon_offset.y+icon_size.h/2;
                result.axis_z= (((page_info->icon_info+temp)->icon_offset.x+icon_size.w/2+distance)*s2d_sin(axis_angle))>>16;
                
            }
            else
            {
                result.axis_x=win_size.w-(((win_size.w-((page_info->icon_info+temp)->icon_offset.x+icon_size.w/2+distance))*s2d_cos(-axis_angle))>>16);
                result.axis_y=(page_info->icon_info+temp)->icon_offset.y+icon_size.h/2;
                result.axis_z= ((win_size.w-((page_info->icon_info+temp)->icon_offset.x+icon_size.w/2+distance))*s2d_sin(-axis_angle))>>16;
                
            }
            //旋转函数每个小图标，都旋转一次根据不同的转轴点。
            Make_Rotate_Slope_shining(direction,c_frame,total_frame,out_buff,result,(*(page_info->icon_info+temp)), icon_size, win_size, axis_angle+1800,calc_data_info);

        }
    }
    
   /* for(i=0;i<page_info->total_row_num;i++)
    {
        for(j=0;j<page_info->total_col_num;j++)
        {
        
            temp=i*page_info->total_col_num+j;  
            if(PNULL!=(*(page_info->icon_info+i*page_info->total_col_num+j)).icon_buf)
            {		
                icon_size.w = (page_info->icon_info+i*page_info->total_col_num+j)->icon_size.w;
                icon_size.h = (page_info->icon_info+i*page_info->total_col_num+j)->icon_size.h;
                //确定小图标转轴信息，主要根据他的左上点左边和旋转角度。
                if((axis_angle)>=0)
                {		
                    result.axis_x= (((page_info->icon_info+temp)->icon_offset.x +icon_size.w/2+distance)*s2d_cos(axis_angle))>>16;
                    result.axis_y=(page_info->icon_info+temp)->icon_offset.y+icon_size.h/2;
                    result.axis_z= (((page_info->icon_info+temp)->icon_offset.x+icon_size.w/2+distance)*s2d_sin(axis_angle))>>16;
                    
                }
                else
                {
                    result.axis_x=win_size.w-(((win_size.w-((page_info->icon_info+temp)->icon_offset.x+icon_size.w/2+distance))*s2d_cos(-axis_angle))>>16);
                    result.axis_y=(page_info->icon_info+temp)->icon_offset.y+icon_size.h/2;
                    result.axis_z= ((win_size.w-((page_info->icon_info+temp)->icon_offset.x+icon_size.w/2+distance))*s2d_sin(-axis_angle))>>16;
                    
                }
                //旋转函数每个小图标，都旋转一次根据不同的转轴点。
                Make_Rotate_Slope_shining(direction,c_frame,total_frame,out_buff,result,(*(page_info->icon_info+temp)), icon_size, win_size, axis_angle+1800,calc_data_info);

            }
        }
    }*/
}
/*****************************************************************************/
//  Description : 斜面计算函数
//  Global resource dependence : 
//  Author: ANSON
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIdle_U_Effect_Slope_Type_Calc(		GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
																		GO_MAINMENU_PAGE_INFO_T *next_page_info,//下一页信息
																		GO_MAINMENU_PAGE_INFO_T *third_page_info,
																		uint16 cur_frame_num,//当前帧
																		uint16 total_frame_num,//总帧数   暂定20
																		IMGREF_SIZE_T bg_window_size,//屏幕尺寸
																		uint32* bg_window_buf,//背景buffer
																		GO_MOVE_DIRECTION_E direction,//移动方向
																		FIX16_POINT_T* calc_point_buf,
																		GO_CALC_DATA_INFO_T* calc_data_info //申请计算所需的BUFFER
																		)

{
    //ROTA_DATA_T image_rota_data[5][16]={0};
    
    //wid=(*image_collection).img_size.w;
    //height=(*image_collection).img_size.h;
    if(PNULL == cur_page_info
        ||PNULL == next_page_info
        //||PNULL == third_page_info
        ||PNULL == bg_window_buf
        ||PNULL == calc_point_buf
        ||(cur_frame_num > total_frame_num))
    {
        return FALSE;
    }
    
    //Calc_Frame_Data()这个函数根据帧数计算图标应该在空间所在的位置，最后一个参数表示它的页数。
    Calc_Frame_Data(cur_page_info,bg_window_buf,  bg_window_size, cur_frame_num, total_frame_num, direction,0,calc_data_info);
    Calc_Frame_Data(next_page_info,bg_window_buf, bg_window_size, cur_frame_num, total_frame_num, direction,1,calc_data_info);
    //Calc_Frame_Data(third_page_info, bg_window_buf,cur_page_info->icon_size, bg_window_size, cur_frame_num, total_frame_num, direction,2);
    return TRUE;
    
}
#endif
#ifdef UIDLE_BOOKPAGE_SE_SUPPORT 
/*****************************************************************************/
//  Description : Bookpage 特效执行函数，包含是否打光特效
//  Global resource dependence : 
//  Author: BENNY
//  Note:
/*****************************************************************************/

LOCAL void Make_Bookpage_Shining(uint16 cur_frame,//当前帧
										uint16 total_frame,//总帧数   暂定20
										GO_MOVE_DIRECTION_E direction,
										IMGREF_SIZE_T win_size,
										S3D_ROTATION_GEOMETRY_T *s3d_geometry_ptr,
                								S3D_ROTATION_IMG_T 	*s3d_src_img_ptr,
                								S3D_ROTATION_INOUT_IMG_INFO_T *s3d_img_ptr,
                								uint8 	bright,
                								DPARAM param)										
{
        LIGHT_SHINING_T ls_para = {0};
        FIX16_POINT_T point1 = {0};//st
        FIX16_POINT_T point2 = {0};//ed
        int32 radius = 140;
        int32 delta_xy = 200,x_margin = 10;
        int32 x_st = -80, x_ed = win_size.w+80;
        int32 frame = 0 ;

        frame = cur_frame;

        if(0==total_frame)
            return;
    
        if(GO_MOVE_LEFT== direction)
        {
            x_st = (-1)*radius/2-x_margin;
            x_ed = win_size.w+radius/2+delta_xy;           
        }
        else if(GO_MOVE_RIGHT== direction)
        {
            x_st = win_size.w+radius/2+delta_xy+x_margin;
            x_ed = (-1)*radius/2;    
        }
        else
        {
            return;
        }

        if(frame<=total_frame/4)
        {    //point1.x = (x_ed-x_st)*frame*2/total_frame+x_st;
        }
        else
        {
            point1.x = (x_ed-x_st)*(frame-total_frame/4)*4/3/total_frame+x_st;
            //point1.x = (x_ed - x_st)*(total_frame-frame)*2/total_frame+x_st;
        }

        point1.y = 0;
        point2.x = point1.x - delta_xy;
        point2.y = win_size.h;

        ls_para.color = 0xdfffffff;
        ls_para.ed_point = &point2;
        ls_para.st_point = &point1;
        ls_para.rds = radius;   
        if(DoShiningEffect())
        {
            S3D_AxisMidRotation_Img_shining( s3d_geometry_ptr,s3d_src_img_ptr,s3d_img_ptr,bright,&ls_para,param);
        }
        else
        {
            S3D_AxisMidRotation_Img( s3d_geometry_ptr,s3d_src_img_ptr,s3d_img_ptr,bright,param);
        }
}

/*****************************************************************************/
//  Description : 计算函数
//  Global resource dependence : 
//  Author: Ben
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIdle_U_Effect_Bookpage_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
																GO_MAINMENU_PAGE_INFO_T *next_page_info,//下一页信息
																GO_MAINMENU_PAGE_INFO_T *third_page_info,
																uint16 cur_frame_num,//当前帧
																uint16 total_frame_num,//总帧数   暂定20
																IMGREF_SIZE_T bg_window_size,//屏幕尺寸
																uint32* bg_window_buf,//背景buffer
																GO_MOVE_DIRECTION_E direction,//移动方向
																FIX16_POINT_T* calc_point_buf,
																GO_CALC_DATA_INFO_T* calc_data_info //申请计算所需的BUFFER
																)
{
	int32 i = 0, angle_temp = 0;
	int32 run_direction 		= 1;// 1:left, -1:right
	int32 bright_by_z 		= 0;
	int32 bright_i = 2;
	int32 bright_min 			= 0;
	int32 bright_max 			= 255;

	int32 frame_temp 		= 0;//0~3600
	int32 frame_max 			= 900;
	int32 frame_icon_max 	= 0;
	S3D_ROTATION_GEOMETRY_T s3d_geometry 	= {0};
	S3D_ROTATION_IMG_T 	s3d_src_img			= {0};
	S3D_ROTATION_INOUT_IMG_INFO_T s3d_img	= {0};

	S3D_AXIS_T rotation_axis		= {0};
//入参有效性检验
	if(PNULL == cur_page_info
		||PNULL == next_page_info
		||PNULL == bg_window_buf
		||PNULL == calc_point_buf
		||(cur_frame_num > total_frame_num)
		||0 == total_frame_num)
	{
		return FALSE;
	}

	//if(0 >= frame_max)//避规除0操作
	//{
	//	return FALSE;
	//}
//帧转换，实际计算中帧细化到frame_max帧	
//除零问题，上一段已经检测
	frame_temp = frame_max*cur_frame_num/total_frame_num;
	//frame_temp = 600;
//划动方向判断
	if(GO_MOVE_LEFT== direction)
	{
		run_direction = 1;
	}
	else if(GO_MOVE_RIGHT== direction)
	{
		run_direction = -1;
	}
	else
	{
		return SCI_FALSE;
	}
	
	rotation_axis.axis = 1;//y axis
	rotation_axis.x = 0;
	rotation_axis.y = 0;// it can be any value, because y axis is the rotation axis
	rotation_axis.z = 0;

	//前后景排序，向左时，下一页的图标在后面，先画
	if(run_direction == 1)
	{
		angle_temp = run_direction*(frame_temp-900);			        
	        s3d_geometry.angle=(-1)*(angle_temp+1800)%3600;
	        s3d_geometry.eye.x=bg_window_size.w/2;
	        s3d_geometry.eye.y=bg_window_size.h/2;
	        s3d_geometry.eye.z=-2*bg_window_size.h;

	        s3d_geometry.img_size.x =0;
	        s3d_geometry.img_size.y = 0;

		s3d_geometry.axis.axis=1;
		s3d_geometry.is_axis_use_default = SCI_FALSE;
		s3d_geometry.is_eye_use_default = SCI_FALSE;

        s3d_src_img.src_type=calc_data_info->input_type;
        s3d_img.color_type=calc_data_info->output_type;
		s3d_img.color_ptr=bg_window_buf;
		s3d_img.size.h = bg_window_size.h;
		s3d_img.size.w = bg_window_size.w;
			
		for(i = 0; i < next_page_info->icon_num; ++i)
		{	
			s3d_geometry.img_size.w=next_page_info->icon_info[i].icon_size.w; 
            s3d_geometry.img_size.h=next_page_info->icon_info[i].icon_size.h;
	        s3d_geometry.axis.x= rotation_axis.x + (((next_page_info->icon_info[i].icon_offset.x + next_page_info->icon_info[i].icon_size.w/2 - rotation_axis.x)*s2d_cos_new(angle_temp))/65536);
	        s3d_geometry.axis.y= next_page_info->icon_info[i].icon_offset.y + next_page_info->icon_info[i].icon_size.h/2;
	        s3d_geometry.axis.z= rotation_axis.z - (((next_page_info->icon_info[i].icon_offset.x + next_page_info->icon_info[i].icon_size.w/2 - rotation_axis.x)*s2d_sin_new(angle_temp))/65536);
	        s3d_src_img.src_ptr=next_page_info->icon_info[i].icon_buf;
		
                bright_by_z = abs(s3d_geometry.angle+1800)*(bright_min - bright_max)/frame_max+bright_max;
			Make_Bookpage_Shining(cur_frame_num,total_frame_num,direction,bg_window_size,&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z,calc_data_info->param);
			//S3D_ImageMidAxis565Rotation(&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z);

		}

		angle_temp = run_direction*frame_temp;		        
	      	s3d_geometry.angle=(-1)*(angle_temp+1800)%3600;
		
		for(i = 0; i < cur_page_info->icon_num; ++i)
		{		
				s3d_geometry.img_size.w=cur_page_info->icon_info[i].icon_size.w; 
	            s3d_geometry.img_size.h=cur_page_info->icon_info[i].icon_size.h; 
		        s3d_geometry.axis.x= rotation_axis.x + (((cur_page_info->icon_info[i].icon_offset.x + cur_page_info->icon_info[i].icon_size.w/2 - rotation_axis.x)*s2d_cos_new(angle_temp))/65536);
		        s3d_geometry.axis.y= cur_page_info->icon_info[i].icon_offset.y + cur_page_info->icon_info[i].icon_size.h/2;
		        s3d_geometry.axis.z= rotation_axis.z - (((cur_page_info->icon_info[i].icon_offset.x + cur_page_info->icon_info[i].icon_size.w/2 - rotation_axis.x)*s2d_sin_new(angle_temp))/65536);
		        s3d_src_img.src_ptr=cur_page_info->icon_info[i].icon_buf;
			bright_by_z = abs(s3d_geometry.angle+1800)*(bright_min - bright_max)/frame_max+bright_max;
			S3D_AxisMidRotation_Img(&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z,calc_data_info->param);

		}	
	}
	else//向右时，下一页的图标在前面，后画
	{
		angle_temp = run_direction*frame_temp;		        
	        s3d_geometry.angle=(-1)*(angle_temp+1800)%3600;
	        s3d_geometry.eye.x=bg_window_size.w/2;
	        s3d_geometry.eye.y=bg_window_size.h/2;
	        s3d_geometry.eye.z=-2*bg_window_size.h;
		//bright_by_z = abs(s3d_geometry.angle+1800)*(bright_min - bright_max)/frame_max+bright_max;

	        s3d_geometry.img_size.x =0;
	        s3d_geometry.img_size.y = 0;
		s3d_geometry.axis.axis=1;
	        s3d_geometry.is_axis_use_default = SCI_FALSE;
	        s3d_geometry.is_eye_use_default = SCI_FALSE;

            s3d_src_img.src_type=calc_data_info->input_type;
            s3d_img.color_type=calc_data_info->output_type;
	        s3d_img.color_ptr=bg_window_buf;
	        s3d_img.size.h = bg_window_size.h;
	        s3d_img.size.w = bg_window_size.w;
		for(i = 0; i < cur_page_info->icon_num; ++i)
		{		
		//计算3d位置
				s3d_geometry.img_size.w=cur_page_info->icon_info[i].icon_size.w; 
	            s3d_geometry.img_size.h=cur_page_info->icon_info[i].icon_size.h; 
		        s3d_geometry.axis.x= rotation_axis.x + (((cur_page_info->icon_info[i].icon_offset.x + cur_page_info->icon_info[i].icon_size.w/2 - rotation_axis.x)*s2d_cos_new(angle_temp))/65536);
		        s3d_geometry.axis.y= cur_page_info->icon_info[i].icon_offset.y + cur_page_info->icon_info[i].icon_size.h/2;
		        s3d_geometry.axis.z= rotation_axis.z - (((cur_page_info->icon_info[i].icon_offset.x + cur_page_info->icon_info[i].icon_size.w/2 - rotation_axis.x)*s2d_sin_new(angle_temp))/65536);		        
		        s3d_src_img.src_ptr=cur_page_info->icon_info[i].icon_buf;
			bright_by_z = abs(s3d_geometry.angle+1800)*(bright_min - bright_max)/frame_max+bright_max;
			S3D_AxisMidRotation_Img(&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z,calc_data_info->param);

		}	

		angle_temp = run_direction*(frame_temp-900);			        
	        s3d_geometry.angle=(-1)*(angle_temp+1800)%3600;
		//bright_by_z = abs(s3d_geometry.angle+1800)*(bright_min - bright_max)/frame_max+bright_max;

		for(i = 0; i < next_page_info->icon_num; ++i)
		{	
				s3d_geometry.img_size.w=next_page_info->icon_info[i].icon_size.w; 
	            s3d_geometry.img_size.h=next_page_info->icon_info[i].icon_size.h;
		        s3d_geometry.axis.x= rotation_axis.x + (((next_page_info->icon_info[i].icon_offset.x + next_page_info->icon_info[i].icon_size.w/2 - rotation_axis.x)*s2d_cos_new(angle_temp))/65536);
		        s3d_geometry.axis.y= next_page_info->icon_info[i].icon_offset.y + next_page_info->icon_info[i].icon_size.h/2;
		        s3d_geometry.axis.z= rotation_axis.z - (((next_page_info->icon_info[i].icon_offset.x + next_page_info->icon_info[i].icon_size.w/2 - rotation_axis.x)*s2d_sin_new(angle_temp))/65536);
		        s3d_src_img.src_ptr=next_page_info->icon_info[i].icon_buf;
		        bright_by_z = abs(s3d_geometry.angle+1800)*(bright_min - bright_max)/frame_max+bright_max;
			Make_Bookpage_Shining(cur_frame_num,total_frame_num,direction,bg_window_size,&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z,calc_data_info->param);
			//S3D_ImageMidAxis565Rotation(&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z);
		}
	}

	return SCI_TRUE;

}
#endif
#ifdef UIDLE_MIDROTATE_SE_SUPPORT
/*****************************************************************************/
//  Description : MidRotate 特效执行函数，包含是否打光特效
//  Global resource dependence : 
//  Author: BENNY
//  Note:
/*****************************************************************************/

LOCAL void Make_Effect_MidRotate_Shining(uint16 cur_frame,//当前帧
										uint16 total_frame,//总帧数   暂定20
										GO_MOVE_DIRECTION_E direction,
										COLOR32 * tempColor, 
										GO_ROTA_DATA_T img_data,
										GO_MAINMENU_ICON_INFO_T thumb,  
										IMGREF_SIZE_T icon_size,
										IMGREF_SIZE_T win_size, 
										int angle,
										int alpha,
										GO_CALC_DATA_INFO_T* calc_data_info)
{
        S3D_ROTATION_GEOMETRY_T  s3d_geometry={0};
        S3D_ROTATION_IMG_T   s3d_src_img={0};
        S3D_ROTATION_INOUT_IMG_INFO_T  s3d_img={0};
        LIGHT_SHINING_T ls_para = {0};
        FIX16_POINT_T point1 = {0};//st
        FIX16_POINT_T point2 = {0};//ed
        int32 radius = 220;
        int32 delta_xy = 160,x_margin = 0;
        int32 x_st = -80, x_ed = win_size.w+80;
        int32 frame = 0 ;

        frame = cur_frame;

        if(0==total_frame)
            return;
        if(GO_MOVE_LEFT== direction)
        {
            /*x_st = (-1)*radius/2-x_margin;
            x_ed = win_size.w+radius/2+delta_xy; */
			x_st = (-1)*radius/2-x_margin-delta_xy;
            x_ed = win_size.w+x_margin;
			delta_xy = -delta_xy;
			
        }
        else if(GO_MOVE_RIGHT== direction)
        {
            x_st = win_size.w+radius/2+delta_xy+x_margin;
            x_ed = (-1)*radius/2;    
        }
        else
        {
            return;
        }

        if(frame<=total_frame/4)
        {
            //point1.x = (x_ed-x_st)*frame*2/total_frame+x_st;
			point1.x = x_st;
        }
        else
        {
            point1.x = (x_ed-x_st)*(frame-total_frame/4)*4/3/total_frame+x_st;
            //point1.x = (x_ed - x_st)*(total_frame-frame)*2/total_frame+x_st;
        }
        point1.y = 0;
        point2.x = point1.x - delta_xy;
        point2.y = win_size.h;

        s3d_geometry.img_size.w=icon_size.w;
        s3d_geometry.img_size.h=icon_size.h;
        s3d_geometry.img_size.x =0;
        s3d_geometry.img_size.y = 0;

        s3d_geometry.angle=angle;
        s3d_geometry.eye.x=win_size.w/2;
        s3d_geometry.eye.y=win_size.h/2;
        s3d_geometry.eye.z=-2*win_size.h;

        s3d_geometry.axis.x=img_data.axis_x;
        s3d_geometry.axis.y=img_data.axis_y;
        s3d_geometry.axis.z=img_data.axis_z;
        s3d_geometry.axis.axis=1;
        s3d_geometry.is_axis_use_default = SCI_FALSE;
        s3d_geometry.is_eye_use_default = SCI_FALSE;

        s3d_src_img.src_ptr=thumb.icon_buf;

        s3d_src_img.src_type=calc_data_info->input_type;
        s3d_img.color_type=calc_data_info->output_type;
        s3d_img.color_ptr=tempColor;
        s3d_img.size.h=win_size.h;
        s3d_img.size.w=win_size.w;

        ls_para.color = 0xf0ffffff;
        ls_para.ed_point = &point2;
        ls_para.st_point = &point1;
        ls_para.rds = radius;   
        if(DoShiningEffect())
        {
            S3D_AxisMidRotation_Img_shining( &(s3d_geometry),&(s3d_src_img),&(s3d_img),alpha,&ls_para,calc_data_info->param);
        }
        else
        {
            S3D_AxisMidRotation_Img( &(s3d_geometry),&(s3d_src_img),&(s3d_img),alpha,calc_data_info->param);
        }
        
}

PUBLIC BOOLEAN MMIIdle_U_Effect_MidRotate_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
													GO_MAINMENU_PAGE_INFO_T *next_page_info,//下一页信息
													GO_MAINMENU_PAGE_INFO_T *third_page_info,
													uint16 cur_frame_num,//当前帧
													uint16 total_frame_num,//总帧数   暂定20
													IMGREF_SIZE_T bg_window_size,//屏幕尺寸
													uint32* bg_window_buf,//背景buffer
													GO_MOVE_DIRECTION_E direction,//移动方向
													FIX16_POINT_T* calc_point_buf,
													GO_CALC_DATA_INFO_T* calc_data_info //申请计算所需的BUFFER
													)
{
	int angle=0,n=0,i=0,j=0,temp=0,axis_angle = 0,z_deep=0,color_deep=0;
	GO_ROTA_DATA_T axis_data={0};

	if(PNULL == cur_page_info
		||PNULL == next_page_info
		||PNULL == bg_window_buf
		||PNULL == calc_point_buf
		||(cur_frame_num > total_frame_num))
	{
		return FALSE;
	}
	if(0==total_frame_num)
		{
		 	return FALSE;
		}
	
	//根据帧数确定坐标
	if(GO_MOVE_RIGHT==direction)
		{	axis_angle=1800*cur_frame_num/total_frame_num;
			angle=1800*cur_frame_num/total_frame_num+1800;
			if(angle>=2700)
				{
					angle=-1800*(total_frame_num-cur_frame_num)/total_frame_num+1800;
				}
			z_deep=(bg_window_size.w/2*s2d_sin(axis_angle))>>16;
			
		}
	else if(GO_MOVE_LEFT==direction)
		{	axis_angle=-1800*cur_frame_num/total_frame_num;
			angle=-1800*cur_frame_num/total_frame_num+1800;
			if(angle<=900)
				{
				angle=1800*(total_frame_num-cur_frame_num)/total_frame_num+1800;
				}
			z_deep=(bg_window_size.w/2*s2d_sin(-axis_angle))>>16;
		}

    if(cur_frame_num<total_frame_num/2)
      	{
	      	for(i = 0; i < cur_page_info->icon_num; ++i)
            {
                temp=i;
        		if(PNULL!=(cur_page_info->icon_info+temp)->icon_buf)
        			{	
        				//离屏幕远的部分颜色更暗。
        				color_deep=255*abs(abs(axis_angle)-900)/900;
        				if(255<color_deep)
        				{
        					color_deep = 255;
        				}
        				//计算转轴信息
        				axis_data.axis_x=( (((cur_page_info->icon_info+temp)->icon_offset.x +(cur_page_info->icon_info+temp)->icon_size.w/2-bg_window_size.w/2)*s2d_cos(axis_angle))>>16)+bg_window_size.w/2;
        				axis_data.axis_y=(cur_page_info->icon_info+temp)->icon_offset.y+(cur_page_info->icon_info+temp)->icon_size.h/2;
        				axis_data.axis_z= ((((cur_page_info->icon_info+temp)->icon_offset.x+(cur_page_info->icon_info+temp)->icon_size.w/2-bg_window_size.w/2)*s2d_sin(axis_angle))>>16)+z_deep;
        				//进行旋转
        				Make_Effect_MidRotate_Shining(cur_frame_num,total_frame_num,direction,bg_window_buf, axis_data, * (cur_page_info->icon_info+temp), (cur_page_info->icon_info+temp)->icon_size, bg_window_size, angle,color_deep,calc_data_info);
        			}

		}
      	}
	  //超过总帧数的一半下一页进行旋转
	else if(cur_frame_num>=total_frame_num/2)
		{   n=-1;
        	      	for(i = 0; i < next_page_info->icon_num; ++i)
                    {
                        temp=i;
				if(PNULL!=(next_page_info->icon_info+temp)->icon_buf)
					{	
						//离屏幕远的颜色更暗。
						color_deep=255*abs(abs(axis_angle)-900)/900;
						if(255<color_deep)
						{
							color_deep = 255;
						}
						//计算转轴信息
						axis_data.axis_x=( (((next_page_info->icon_info+temp)->icon_offset.x +(next_page_info->icon_info+temp)->icon_size.w/2-bg_window_size.w/2)*s2d_cos(axis_angle))>>16)*n+bg_window_size.w/2;
						axis_data.axis_y=(next_page_info->icon_info+temp)->icon_offset.y+(next_page_info->icon_info+temp)->icon_size.h/2;
						axis_data.axis_z= ((((next_page_info->icon_info+temp)->icon_offset.x+(next_page_info->icon_info+temp)->icon_size.w/2-bg_window_size.w/2)*s2d_sin(axis_angle))>>16)*n+z_deep;
						//进行旋转
						Make_Effect_MidRotate_Shining(cur_frame_num,total_frame_num,direction,bg_window_buf, axis_data, * (next_page_info->icon_info+temp), (next_page_info->icon_info+temp)->icon_size, bg_window_size, angle,color_deep,calc_data_info);
					}
			}
      		}
   return TRUE;
}
#endif
#ifdef UIDLE_SMALLITEMS_SE_SUPPORT
/*****************************************************************************/
//  Description : SmallItems 特效执行函数，包含是否打光特效
//  Global resource dependence : 
//  Author: BENNY
//  Note:
/*****************************************************************************/

LOCAL void Make_SmallItems_Shining(uint16 cur_frame,//当前帧
										uint16 total_frame,//总帧数   暂定20
										GO_MOVE_DIRECTION_E direction,
										const IMGREF_SIZE_T window,			//输出buf大小
                            						const IMGREF_SIZE_T img,					//图像大小
                            						IMGREF_POINT_T window_mid,					//图像整体位移
                            						IMGREF_POINT_T img_mid,						//旋转/放大中心点
                            						int32 angle,								//旋转角度
                            						COLOR32 *in_src_ptr,						//输入buf指针
                            						COLOR32 *out_src_ptr,						//输出buf指针
                            						int32 z_para,								//缩放比率 放大倍数 = z_para/4096
                            						uint8 alpha_para,							//alpha衰减值
                            						FIX16_POINT_T *Point_Add_ptr,
                            						GO_CALC_DATA_INFO_T* calc_data_info
										)
							
{
        LIGHT_SHINING_T ls_para = {0};
        FIX16_POINT_T point1 = {0};//st
        FIX16_POINT_T point2 = {0};//ed
        int32 radius = 60;	
        int32 delta_xy = 260,x_margin = 10;	
        int32 x_st = -80, x_ed = window.w+80;
        int32 frame = 0 ;

        frame = cur_frame;

        if(0==total_frame)
            return;
    
        if(GO_MOVE_LEFT== direction)
        {
            x_st = (-1)*radius/2-x_margin;
            x_ed = window.w+radius/2+delta_xy;           
        }
        else if(GO_MOVE_RIGHT== direction)
        {
            x_st = window.w+radius/2+delta_xy+x_margin;
            x_ed = (-1)*radius/2;    
        }
        else
        {
            return;
        }

        /*if(frame<=total_frame/4)
        {    //point1.x = (x_ed-x_st)*frame*2/total_frame+x_st;
        }
        else
        {
            point1.x = (x_ed-x_st)*(frame-total_frame/4)*4/3/total_frame+x_st;
            //point1.x = (x_ed - x_st)*(total_frame-frame)*2/total_frame+x_st;
        }*/
        if(frame<=total_frame/4)
        {    //point1.x = (x_ed-x_st)*frame*2/total_frame+x_st;
        	point1.x = x_st;
        }
        else
        {
            point1.x = (x_ed-x_st)*(frame-total_frame/4)*4/3/total_frame+x_st;
            //point1.x = (x_ed - x_st)*(total_frame-frame)*2/total_frame+x_st;
        }
        point1.y = 0;
        point2.x = point1.x - delta_xy;
        point2.y = window.h;

        ls_para.color = 0xdfffffff;
        ls_para.ed_point = &point2;
        ls_para.st_point = &point1;
        ls_para.rds = radius;   

        if(DoShiningEffect())
        {
            S2d_Type_4_in_one_fast_shining(window,img,window_mid,img_mid,angle,in_src_ptr,calc_data_info->input_type,out_src_ptr,calc_data_info->output_type,z_para,alpha_para,&ls_para,Point_Add_ptr,calc_data_info->param);
        }
        else
        {
            S2d_Type_4_in_one_fast(window,img,window_mid,img_mid,angle,in_src_ptr,calc_data_info->input_type,out_src_ptr,calc_data_info->output_type,z_para,alpha_para,Point_Add_ptr,calc_data_info->param);
        }
        
}

   
PUBLIC BOOLEAN MMIIdle_U_Effect_SmallItems_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
													GO_MAINMENU_PAGE_INFO_T *next_page_info,//下一页信息
													GO_MAINMENU_PAGE_INFO_T *third_page_info,
													uint16 cur_frame_num,//当前帧
													uint16 total_frame_num,//总帧数   暂定20
													IMGREF_SIZE_T bg_window_size,//屏幕尺寸
													uint32* bg_window_buf,//背景buffer
													GO_MOVE_DIRECTION_E direction,//移动方向
													FIX16_POINT_T* calc_point_buf,//申请计算所需的BUFFER
													GO_CALC_DATA_INFO_T* calc_data_info 
													)
{
	int i=0,j=0,dx=0,dy=0,shrik_wid=0,n=0;
	int pre_alpha=0,next_alpha=0,pre_z=0,next_z=0,h_frame=0,radius=0,vector=0,angle=0,distance=0;
	BOOLEAN Is_pre_out=FALSE;
	IMGREF_POINT_T window_offset={0};
	IMGREF_POINT_T img_offset={0};
	//FIX16_POINT_T Rota_buff[2048];
	//IMGREF_POINT_T  pre_thumb_array[6][6]={0};
	//IMGREF_POINT_T  next_thumb_array[6][6]={0};
    int32 temp = 0;

	if(PNULL == cur_page_info
		||PNULL == next_page_info
		||PNULL == bg_window_buf
		||PNULL == calc_point_buf
		||(cur_frame_num > total_frame_num))
	{
		return FALSE;
	}

	h_frame=total_frame_num*2/3;
	if(h_frame==0)
		{
			return FALSE;
		}
	angle=900*cur_frame_num/total_frame_num;
	radius=bg_window_size.w/2;
	//row=cur_page_info->total_row_num;
	//column=cur_page_info->total_col_num;
	


	
	
	//next
	next_alpha=255*cur_frame_num/total_frame_num;
	//pre

	if(cur_frame_num>h_frame)
	{	
	//当前页出去需要总帧数一半时间。
		Is_pre_out=TRUE;
	}

	if(GO_MOVE_RIGHT==direction)
	{
		n=1;
	}
	else if(GO_MOVE_LEFT==direction)
	{	
		n=-1;
	}
	if(!Is_pre_out&&cur_frame_num<=h_frame)
	{	
	    pre_alpha=255*(h_frame-cur_frame_num)/(h_frame);
		vector=radius*(cur_frame_num)/(h_frame);
		distance=vector;
		pre_z=radius*4096/(radius+distance);
		dx=(bg_window_size.w/2-(bg_window_size.w/2)*pre_z/4096);
		dy=(bg_window_size.h/2-(bg_window_size.h/2)*pre_z/4096);
        
              shrik_wid=bg_window_size.w*pre_z/4096;
		for(i = 0; i < cur_page_info->icon_num; ++i)
             {
                    temp=i;   
                    window_offset.x=(cur_page_info->icon_info+temp)->icon_offset.x*pre_z/4096+dx+vector*n;
        		window_offset.y=(cur_page_info->icon_info+temp)->icon_offset.y*pre_z/4096+dy;
        		//根据位置，透明度等，缩放度画出当前帧。
        		if((PNULL!=(*(cur_page_info->icon_info+temp)).icon_buf)&&(window_offset.x<bg_window_size.w)&&(window_offset.x+shrik_wid)>0)
        		{        			
        			S2d_Type_4_in_one_fast(bg_window_size, (cur_page_info->icon_info+temp)->icon_size, window_offset,img_offset, 0, (*(cur_page_info->icon_info+temp)).icon_buf , calc_data_info->input_type,bg_window_buf,calc_data_info->output_type, pre_z, pre_alpha,calc_point_buf,calc_data_info->param);
        		}
		}
	}
	next_alpha=255*(cur_frame_num)/(total_frame_num);
	vector=radius*(cur_frame_num)/(total_frame_num);
	distance=radius-vector;
	next_z=radius*4096/(radius+distance);
	dx=(bg_window_size.w/2-(bg_window_size.w/2)*next_z/4096);
	dy=(bg_window_size.h/2-(bg_window_size.h/2)*next_z/4096);

        //根据位置，透明度等，缩放度画出下一帧。
       shrik_wid=bg_window_size.w*next_z/4096;
        for(i = 0; i < next_page_info->icon_num; ++i)
        {
               temp=i;     
               window_offset.x=(next_page_info->icon_info+temp)->icon_offset.x*next_z/4096+dx+(vector-bg_window_size.w/2)*(n);
        	    window_offset.y=(next_page_info->icon_info+temp)->icon_offset.y*next_z/4096+dy;
               if((PNULL!=(*(next_page_info->icon_info+temp)).icon_buf)&&(window_offset.x+shrik_wid>0)&&(window_offset.x<bg_window_size.w))
	        {        		
        		Make_SmallItems_Shining(cur_frame_num,total_frame_num,direction,bg_window_size, (next_page_info->icon_info+temp)->icon_size, window_offset,img_offset, 0, (*(next_page_info->icon_info+temp)).icon_buf , bg_window_buf, next_z, next_alpha,calc_point_buf,calc_data_info);
        		//S2d_4_in_one_fast(bg_window_size, cur_page_info->icon_size, window_offset,img_offset, 0, (*(next_page_info->icon_info+i*column+j)).icon_buf , bg_window_buf, next_z, next_alpha,calc_point_buf);
                }
        }		

	return TRUE;
	
}

#endif
#ifdef UIDLE_CLASSIC_SE_SUPPORT
/*****************************************************************************/
//  Description : Classic 特效执行函数，包含是否打光特效
//  Global resource dependence : 
//  Author: BENNY
//  Note:
/*****************************************************************************/

LOCAL void Make_Classic_Shining(uint16 cur_frame,//当前帧
										uint16 total_frame,//总帧数   暂定20
										GO_MOVE_DIRECTION_E direction,
										COLOR32 * tempColor, 
										GO_ROTA_DATA_T img_data,
										COLOR32	*up_buf,  
										IMGREF_SIZE_T icon_size,
										IMGREF_SIZE_T win_size, 
										int angle,
										GO_CALC_DATA_INFO_T* calc_data_info)
{
        S3D_ROTATION_GEOMETRY_T  s3d_geometry={0};
        S3D_ROTATION_IMG_T   s3d_src_img={0};
        S3D_ROTATION_INOUT_IMG_INFO_T  s3d_img={0};
        LIGHT_SHINING_T ls_para = {0};
        FIX16_POINT_T point1 = {0};//st
        FIX16_POINT_T point2 = {0};//ed
        int32 radius = 180;
        int32 delta_xy = 280,x_margin = 10;
        int32 x_st = -80, x_ed = win_size.w+80;
        int32 frame = 0 ;
		int32 add_frame = 2;

        frame = cur_frame;

        if(0==total_frame)
            return;
        if(GO_MOVE_LEFT== direction)
        {
            x_st = (-1)*radius/2-x_margin;
            x_ed = win_size.w+radius/2+delta_xy;           
        }
        else if(GO_MOVE_RIGHT== direction)
        {
            x_st = win_size.w+radius/2+delta_xy+x_margin;
            x_ed = (-1)*radius/2;    
        }
        else
        {
            return;
        }

        point1.x = (x_ed-x_st)*frame/(total_frame+add_frame)+x_st;
		/*if(frame<=total_frame/4)
        {    //point1.x = (x_ed-x_st)*frame*2/total_frame+x_st;
        }
        else
        {
            point1.x = (x_ed-x_st)*(frame-total_frame/4)*4/3/total_frame+x_st;
            //point1.x = (x_ed - x_st)*(total_frame-frame)*2/total_frame+x_st;
        }*/
        point1.y = 0;
        point2.x = point1.x - delta_xy;
        point2.y = win_size.h;

        s3d_geometry.img_size.w=icon_size.w;
        s3d_geometry.img_size.h=icon_size.h;
        s3d_geometry.img_size.x =0;
        s3d_geometry.img_size.y = 0;

        s3d_geometry.angle=angle;
        s3d_geometry.eye.x=win_size.w/2;
        s3d_geometry.eye.y=win_size.h/2;
        s3d_geometry.eye.z=-2*win_size.h;

        s3d_geometry.axis.x=img_data.axis_x+icon_size.w/2;
        s3d_geometry.axis.y=img_data.axis_y+icon_size.h/2;
        s3d_geometry.axis.z=img_data.axis_z;
        s3d_geometry.axis.axis=1;
        s3d_geometry.is_axis_use_default = SCI_FALSE;
        s3d_geometry.is_eye_use_default = SCI_FALSE;

        s3d_src_img.src_ptr=up_buf;

        s3d_src_img.src_type=calc_data_info->input_type;
        s3d_img.color_type=calc_data_info->output_type;
        s3d_img.color_ptr=tempColor;
        s3d_img.size.h=win_size.h;
        s3d_img.size.w=win_size.w;

        ls_para.color = 0xdfffffff;
        ls_para.ed_point = &point2;
        ls_para.st_point = &point1;
        ls_para.rds = radius;   
        if(DoShiningEffect())
        {
            S3D_AxisMidRotation_Img_shining( &(s3d_geometry),&(s3d_src_img),&(s3d_img),255,&ls_para,calc_data_info->param);
        }
        else
        {
            S3D_AxisMidRotation_Img( &(s3d_geometry),&(s3d_src_img),&(s3d_img),255,calc_data_info->param);
        }
        
}
PUBLIC BOOLEAN MMIIdle_U_Effect_Classic_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
													GO_MAINMENU_PAGE_INFO_T *next_page_info,//下一页信息
													GO_MAINMENU_PAGE_INFO_T *third_page_info,
													uint16 cur_frame_num,//当前帧
													uint16 total_frame_num,//总帧数   暂定20
													IMGREF_SIZE_T bg_window_size,//屏幕尺寸
													uint32* bg_window_buf,//背景buffer
													GO_MOVE_DIRECTION_E direction,//移动方向
													FIX16_POINT_T* calc_point_buf,
													GO_CALC_DATA_INFO_T* calc_data_info //申请计算所需的BUFFER
													)
{
	int i=0,j=0,dv=0,n=0,temp=0;
	FIX16_POINT_T window_offset={0};
	GO_ROTA_DATA_T result = {0};
    
	if(PNULL == cur_page_info
		||PNULL == next_page_info
		||PNULL == bg_window_buf
		||PNULL == calc_point_buf
		||(cur_frame_num > total_frame_num))
	{
		return FALSE;
	}
    
	//计算dv,dv就是标志平移量的一个长度。
	if(0==total_frame_num)
		{
			return FALSE;
		}
	else
		{	
		      if(GO_MOVE_RIGHT==direction)
			      	{	n=-1;
					dv=bg_window_size.w*cur_frame_num/total_frame_num;
			      	}
			else if(GO_MOVE_LEFT==direction)
				{	n=1;
					dv=-bg_window_size.w*cur_frame_num/total_frame_num;
				}
		}
	//根据dv画出前后两页的图标，这里用n 
    //控制位移的正负，达到控制左右的问题。
    for(i = 0; i < cur_page_info->icon_num; ++i)
    {
        temp=i;
        if(NULL!=(cur_page_info->icon_info +temp)->icon_buf)
        {
        	window_offset.x=(cur_page_info->icon_info+temp)->icon_offset.x+dv;
        	window_offset.y=(cur_page_info->icon_info+temp)->icon_offset.y;
        	if((window_offset.x<bg_window_size.w)&&(window_offset.x+(cur_page_info->icon_info +temp)->icon_size.w)>0)
        		{
        			result.axis_x = window_offset.x;
                    result.axis_y = window_offset.y;
                    result.axis_z = 0;
        			//Make_Classic_Shining(cur_frame_num,total_frame_num,direction,bg_window_buf,result,(cur_page_info->icon_info +temp)->icon_buf,(cur_page_info->icon_info +temp)->icon_size,bg_window_size,1800,calc_data_info);
        			//S2D_Blending(bg_window_size, bg_window_buf,(cur_page_info->icon_info +temp)->icon_size,window_offset,(cur_page_info->icon_info +temp)->icon_buf, bg_window_buf);
        			S2D_TypeBlending(bg_window_size, bg_window_buf,calc_data_info->output_type, (cur_page_info->icon_info +temp)->icon_size, window_offset,(cur_page_info->icon_info +temp)->icon_buf,calc_data_info->input_type, bg_window_buf,calc_data_info->param);              
        		}
        }
    }
    for(i = 0; i < next_page_info->icon_num; ++i)
    {
        temp=i;
        if(NULL!=(next_page_info->icon_info +temp)->icon_buf)
        {
        	window_offset.x=(next_page_info->icon_info+temp)->icon_offset.x+dv+n*bg_window_size.w;
            window_offset.y=(next_page_info->icon_info+temp)->icon_offset.y;
            if((window_offset.x<bg_window_size.w)&&(window_offset.x+(next_page_info->icon_info +temp)->icon_size.w)>0)
        		{
        			result.axis_x = window_offset.x;
                    result.axis_y = window_offset.y;
                    result.axis_z = 0;
        			//Make_Classic_Shining(cur_frame_num,total_frame_num,direction,bg_window_buf,result,(next_page_info->icon_info +temp)->icon_buf,(next_page_info->icon_info +temp)->icon_size,bg_window_size,1800,calc_data_info);
        			//S2D_Blending(bg_window_size, bg_window_buf,(next_page_info->icon_info +temp)->icon_size,window_offset,(next_page_info->icon_info +temp)->icon_buf, bg_window_buf);
        			S2D_TypeBlending(bg_window_size, bg_window_buf,calc_data_info->output_type, (next_page_info->icon_info +temp)->icon_size, window_offset, (next_page_info->icon_info +temp)->icon_buf,calc_data_info->input_type, bg_window_buf,calc_data_info->param);              
        		}
        }
    }

	
	return TRUE;
}
#endif
#ifdef UIDLE_BOXROT_SE_SUPPORT 
/*****************************************************************************/
//  Description : Boxrot 特效执行函数，包含是否打光特效
//  Global resource dependence : 
//  Author: BENNY
//  Note:
/*****************************************************************************/

LOCAL void Make_Boxrot_Shining(uint16 cur_frame,//当前帧
										uint16 total_frame,//总帧数   暂定20
										GO_MOVE_DIRECTION_E direction,
										IMGREF_SIZE_T win_size,
										S3D_ROTATION_GEOMETRY_T *s3d_geometry_ptr,
                								S3D_ROTATION_IMG_T 	*s3d_src_img_ptr,
                								S3D_ROTATION_INOUT_IMG_INFO_T *s3d_img_ptr,
                								uint8 	bright,
                								DPARAM param)										
{
        LIGHT_SHINING_T ls_para = {0};
        FIX16_POINT_T point1 = {0};//st
        FIX16_POINT_T point2 = {0};//ed
        int32 radius = 140;
        int32 delta_xy = 200,x_margin = 10;
        int32 x_st = -80, x_ed = win_size.w+80;
        int32 frame = 0 ;
        IMGREF_3D_POINT_T in_Point = {0};

        frame = cur_frame;

        if(0==total_frame)
            return;
        
        in_Point.x = s3d_geometry_ptr->axis.x + (s3d_geometry_ptr->img_size.w/2)*s2d_cos(s3d_geometry_ptr->angle)/65536;
        in_Point.y = s3d_geometry_ptr->axis.y - (s3d_geometry_ptr->img_size.y+s3d_geometry_ptr->img_size.h/2);
        in_Point.z = s3d_geometry_ptr->axis.z + (s3d_geometry_ptr->img_size.w/2)*s2d_sin(s3d_geometry_ptr->angle)/65536;
        x_st = (((-s3d_geometry_ptr->eye.z) * ( in_Point.x*256 - s3d_geometry_ptr->eye.x*256 )) 
    		/ (in_Point.z - s3d_geometry_ptr->eye.z)*256 + s3d_geometry_ptr->eye.x*65536)/65536;

        in_Point.x = s3d_geometry_ptr->axis.x - (s3d_geometry_ptr->img_size.w/2)*s2d_cos(s3d_geometry_ptr->angle)/65536;
        in_Point.z = s3d_geometry_ptr->axis.z - (s3d_geometry_ptr->img_size.w/2)*s2d_sin(s3d_geometry_ptr->angle)/65536;
        x_ed = (((-s3d_geometry_ptr->eye.z) * ( in_Point.x*256 - s3d_geometry_ptr->eye.x*256 )) 
    		/ (in_Point.z - s3d_geometry_ptr->eye.z)*256 + s3d_geometry_ptr->eye.x*65536)/65536;

        if(x_ed<x_st)
        {
            return;
        }
        
        if(GO_MOVE_LEFT== direction)
        {
            x_st = (-1)*radius/2-x_margin;
            x_ed = win_size.w+radius/2+delta_xy;           
        }
        else if(GO_MOVE_RIGHT== direction)
        {
            x_st = win_size.w+radius/2+delta_xy+x_margin;
            x_ed = (-1)*radius/2;    
        }
        else
        {
            return;
        }

        point1.x = (x_ed-x_st)*frame/total_frame+x_st;
        point1.y = 0;
        point2.x = point1.x - delta_xy;
        point2.y = win_size.h;

        ls_para.color = 0xdfffffff;
        ls_para.ed_point = &point2;
        ls_para.st_point = &point1;
        ls_para.rds = radius;   
        if(DoShiningEffect())
        {
            S3D_AxisMidRotation_Img_shining( s3d_geometry_ptr,s3d_src_img_ptr,s3d_img_ptr,255,&ls_para,param);
        }
        else
        {
            S3D_AxisMidRotation_Img( s3d_geometry_ptr,s3d_src_img_ptr,s3d_img_ptr,255,param);
        }
}

/*****************************************************************************/
//  Description : 计算函数
//  Global resource dependence : 
//  Author: Ben
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIIdle_U_Effect_Boxrot_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
																GO_MAINMENU_PAGE_INFO_T *next_page_info,//下一页信息
																GO_MAINMENU_PAGE_INFO_T *third_page_info,
																uint16 cur_frame_num,//当前帧
																uint16 total_frame_num,//总帧数   暂定20
																IMGREF_SIZE_T bg_window_size,//屏幕尺寸
																uint32* bg_window_buf,//背景buffer
																GO_MOVE_DIRECTION_E direction,//移动方向
																FIX16_POINT_T* calc_point_buf,
																GO_CALC_DATA_INFO_T* calc_data_info //申请计算所需的BUFFER
																)
{

	int32 i = 0, radius = 0, angle_temp = 0;
	int32 run_direction = -1;
	int32 bright_by_z = 0;
	//int32 bright_min = 0;
	int32 bright_max = 255;

	int32 frame_temp = 0;//0~3600
	int32 frame_max = 900;
	//int32 frame_icon_max = 0;


	int32 Center_point_x = 0,Center_point_z = 0;//w_interval = 0,

	S3D_ROTATION_GEOMETRY_T s3d_geometry	= {0};
	S3D_ROTATION_IMG_T 	s3d_src_img			= {0};
	S3D_ROTATION_INOUT_IMG_INFO_T s3d_img	= {0};
	S3D_AXIS_T rotation_axis					= {0};
//入参有效性检验
	if(PNULL == cur_page_info
		||PNULL == next_page_info
		||PNULL == bg_window_buf
		||PNULL == calc_point_buf
		||(cur_frame_num > total_frame_num)
		||0 == total_frame_num)
	{
		return FALSE;
	}

	//if(0 == frame_max)//避规除0操作
	//{
	//	return FALSE;
	//}
//帧转换，实际计算中帧细化到frame_max帧	
//除零问题，上一段已经检测
	frame_temp = frame_max*cur_frame_num/total_frame_num;
	//frame_temp = 100;
//划动方向判断
	if(GO_MOVE_LEFT== direction)
	{
		run_direction = 1;
	}
	else if(GO_MOVE_RIGHT== direction)
	{
		run_direction = -1;
	}
	else
	{
		return SCI_FALSE;
	}
	radius = bg_window_size.w/2;

	rotation_axis.axis = 1;//y axis
	rotation_axis.x = bg_window_size.w/2;
	rotation_axis.y = 0;// it can be any value, because y axis is the rotation axis
	rotation_axis.z = bg_window_size.w/2;
	//前后景排序
	angle_temp = (-1)*run_direction*frame_temp;
	Center_point_x = rotation_axis.x + ((radius*s2d_sin_new(angle_temp))>>16);
	Center_point_z = rotation_axis.z - ((radius*s2d_cos_new(angle_temp))>>16);
	s3d_geometry.img_size.x =0;
	s3d_geometry.img_size.y = 0;
	
    s3d_geometry.angle=angle_temp+1800;
    s3d_geometry.eye.x=bg_window_size.w/2;
    s3d_geometry.eye.y=bg_window_size.h/2;
    s3d_geometry.eye.z=-1*bg_window_size.h;
    s3d_geometry.axis.axis=1;
    s3d_geometry.is_axis_use_default = SCI_FALSE;
    s3d_geometry.is_eye_use_default = SCI_FALSE;

    s3d_src_img.src_type=calc_data_info->input_type;
    s3d_img.color_type=calc_data_info->output_type;
	//s3d_src_img.src_type=S3D_DATA_PAF_RGB888;//S3D_DATA_ARGB888;	    
        s3d_img.color_ptr=bg_window_buf;
        s3d_img.size.h = bg_window_size.h;
        s3d_img.size.w = bg_window_size.w;
	for(i = 0; i < cur_page_info->icon_num; ++i)
	{   
		//计算个图标在盒子面上的3d位置
			s3d_geometry.img_size.w=cur_page_info->icon_info[i].icon_size.w; 
	        s3d_geometry.img_size.h=cur_page_info->icon_info[i].icon_size.h; 
	        s3d_geometry.axis.x= Center_point_x + (((cur_page_info->icon_info[i].icon_offset.x + cur_page_info->icon_info[i].icon_size.w/2 - bg_window_size.w/2)*s2d_cos_new(angle_temp))/65536);
	        s3d_geometry.axis.y= cur_page_info->icon_info[i].icon_offset.y + cur_page_info->icon_info[i].icon_size.h/2;
	        s3d_geometry.axis.z= Center_point_z + (((cur_page_info->icon_info[i].icon_offset.x + cur_page_info->icon_info[i].icon_size.w/2- bg_window_size.w/2)*s2d_sin_new(angle_temp))/65536);
	        s3d_src_img.src_ptr=cur_page_info->icon_info[i].icon_buf;

		Make_Boxrot_Shining(cur_frame_num,total_frame_num,direction,bg_window_size,&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z,calc_data_info->param);
		
	}

	angle_temp = (-1)*run_direction*(frame_temp-900);
	Center_point_x = rotation_axis.x + ((radius*s2d_sin_new(angle_temp))>>16);
	Center_point_z = rotation_axis.z - ((radius*s2d_cos_new(angle_temp))>>16);

	s3d_geometry.angle=angle_temp+1800;
	bright_by_z = bright_max - bright_by_z;
	for(i = 0; i < next_page_info->icon_num; ++i)
	{	
		//计算个图标在盒子面上的3d位置
			s3d_geometry.img_size.w=next_page_info->icon_info[i].icon_size.w; 
            s3d_geometry.img_size.h=next_page_info->icon_info[i].icon_size.h;
	        s3d_geometry.axis.x= Center_point_x + (((next_page_info->icon_info[i].icon_offset.x + next_page_info->icon_info[i].icon_size.w/2 - bg_window_size.w/2)*s2d_cos_new(angle_temp))/65536);
	        s3d_geometry.axis.y= next_page_info->icon_info[i].icon_offset.y + next_page_info->icon_info[i].icon_size.h/2;
	        s3d_geometry.axis.z= Center_point_z + (((next_page_info->icon_info[i].icon_offset.x + next_page_info->icon_info[i].icon_size.w/2 - bg_window_size.w/2)*s2d_sin_new(angle_temp))/65536);
	        s3d_src_img.src_ptr=next_page_info->icon_info[i].icon_buf;	

		Make_Boxrot_Shining(cur_frame_num,total_frame_num,direction,bg_window_size,&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z,calc_data_info->param);
	}
	return TRUE;

}

#endif
#endif
