/*****************************************************************************
** File Name:      mmiwidget_aclock_se.c                                       *
** Author:                                                                   *
** Date:                                                                     *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 2012/7/11      Anson.Yin        Creat
******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#ifdef MMIWIDGET_ACLOCK_SE
#include "sci_types.h"
#include "os_api.h"
#include "img_ref_types.h"
#include "graphics_3d_rotate.h"
#include "mmk_type.h"
#include "mmiwidget_aclock_se.h"
#include "mmiwidget_nv.h"

typedef enum
{
	DRAW_TYPE_NONE = 0,
	DRAW_TYPE_DIRECT,
	DRAW_TYPE_SRC,
	DRAW_TYPE_SHADOW,
	DRAW_TYPE_MAX

}DRAW_TYPE_E;

#define  WATCH_HOUR_Z (-7 )
#define  WATCH_MIN_Z   (WATCH_HOUR_Z-2)
#define  WATCH_SECOND_Z (WATCH_HOUR_Z-4)


#define  WATCH_SECOND_X  10
#define  WATCH_SECOND_Y  65
#define  WATCH_MIN_X    8
#define  WATCH_MIN_Y   65
#define  WATCH_HOUR_X 11 
#define  WATCH_HOUR_Y 47 
#define WATCH_LIGHT_X 160
#define WATCH_LIGHT_Y 400
#define WATCH_LIGHT_Z 400
#define SHADOW_OFF_X -2     
#define SHADOW_OFF_Y -2   
#define WATCH_UP_A_X 190
#define WATCH_UP_A_Y 171
#define WATCH_DOWN_A_X 190
#define WATCH_DOWN_A_Y 193
LOCAL int32 s_watch_angle=0;
LOCAL void Object_Swing(	int * angle,
						int frame,
						int total_frame,
						int max_angle)
{    

	
	
	int32 act_times=12;
	int32 stop_point_x=0;
	int32 current_x=0;
	int32 amplitude=0;  
	if(max_angle>=900)
		{
			max_angle=900;
		}
	else if(max_angle<=-900)
		{
			max_angle=-900;
		}
	
	if(max_angle>=0)
		{
			stop_point_x=1800*act_times+900;
			
		}
	else
		{
			stop_point_x=1800*act_times-900;
		}
	
	
	amplitude=10*1800*256*7/22*max_angle/(stop_point_x*(s2d_sin_new(stop_point_x)>>8));
	// SCI_TRACE_LOW("[SWEETS] amplitude  %d",amplitude);
	current_x=(stop_point_x-3600)*frame/total_frame;
	* angle=amplitude*(stop_point_x-current_x)*(22)/(10*1800*7)*(s2d_sin_new(stop_point_x-current_x)>>8)/256;
	//* angle=(amplitude*(stop_point_x-current_x)*(3.1415926)/(100*1800)*s2d_sin_new((stop_point_x-current_x))*10/65536);
	//SCI_TRACE_LOW("[SWEETS] out_angle  %d",* angle);
	 
}

PUBLIC void IMG_Rotation_watch(	IMGREF_SIZE_T window,
								IMGREF_SIZE_T src_all_size,
								T_RECT src_rect,
								IMGREF_3D_FLOAT_P_T In_3D_ver[4],	//base 4
								uint32 *src_ptr,
								uint32 *window_ptr,
								IMGREF_3D_FLOAT_P_T eye,
								ANTIALIAS_TYPE_E anti_type)
{
	int i = 0;
	POINT_2D_FIX8 ver[5];
    POINT_2D_FIX8 sub_ver[4] = {0};
	
	S3D_ALLAXIS_EXP_T exp_struct;

	//exp_struct.anti_type=ANTIALIAS_TYPE_EDGE_ONLY;
	exp_struct.anti_type=anti_type;

	
	for( i = 0 ; i < 4 ; i ++)
	{
		s3d_Free_Point_change_POINT_2D_FLOAT(In_3D_ver[i],eye,&ver[i]);
	}
    sub_ver[0].x = FIX8_SET(0);
	sub_ver[0].y = FIX8_SET(0);

	sub_ver[1].x = FIX8_SET(src_rect.w-1);
	sub_ver[1].y = FIX8_SET(0);

	sub_ver[2].x = FIX8_SET(src_rect.w-1);
	sub_ver[2].y = FIX8_SET(src_rect.h-1);

	sub_ver[3].x = FIX8_SET(0);
	sub_ver[3].y = FIX8_SET(src_rect.h-1);
	 S3D_All_Axis_Interface_Type( window, 
                                             src_all_size, 
                                             sub_ver, 
                                             &ver[0],	
                                             window_ptr, 
                                            S3D_DATA_PAF_RGB888, 
                                             src_ptr, 
                                            S3D_DATA_PAF_RGB888, 
                                            &exp_struct);
	 #if 0
	 for( i = 0 ; i < 4 ; i ++)
	{
		window_ptr[(ver[i].y>>8)*window.w+(ver[i].x>>8)]=0x0000ff00;
	}
	 #endif 
}

LOCAL void Watch_Rotate_Points(IMGREF_3D_FLOAT_P_T * OutPoint,
									int alpha_x,
									int alpha_y,
									IMGREF_3D_FLOAT_P_T InRoPoint,
									IMGREF_3D_FLOAT_P_T InFixPoint)
{	
	
	IMGREF_3D_FLOAT_P_T fix_ro={0};
	IMGREF_3D_FLOAT_P_T fix_temp={0};
	//	alpha_y=-alpha_y;
	if(0!=alpha_x)
		{
			fix_ro.x=InFixPoint.x;
			fix_ro.y=InFixPoint.y;
			fix_temp.x=(InRoPoint.x-fix_ro.x);
			fix_temp.y=(InRoPoint.y-fix_ro.y);
			
			InRoPoint.x=(fix_ro.x+FIX16_2_FLOAT(fix_temp.x*s2d_cos_new(alpha_x))-FIX16_2_FLOAT(fix_temp.y*s2d_sin_new(alpha_x)));
			InRoPoint.y=(fix_ro.y+FIX16_2_FLOAT(fix_temp.x*s2d_sin_new(alpha_x))+FIX16_2_FLOAT(fix_temp.y*s2d_cos_new(alpha_x)));
 			InRoPoint.y=InRoPoint.y;
		}
	if(0!=alpha_y)
		{
			fix_ro.x=InFixPoint.z;
			fix_ro.y=InFixPoint.y;
			fix_temp.x=(InRoPoint.z-fix_ro.x);
			fix_temp.y=(InRoPoint.y-fix_ro.y);
			
			InRoPoint.x=InRoPoint.x;
			InRoPoint.z=(fix_ro.x+FIX16_2_FLOAT(fix_temp.x*s2d_cos_new(alpha_y))-FIX16_2_FLOAT(fix_temp.y*s2d_sin_new(alpha_y)));
			InRoPoint.y=(fix_ro.y+FIX16_2_FLOAT(fix_temp.x*s2d_sin_new(alpha_y))+FIX16_2_FLOAT(fix_temp.y*s2d_cos_new(alpha_y)));
		}
	OutPoint->x=InRoPoint.x;
	OutPoint->y=InRoPoint.y;
	OutPoint->z=InRoPoint.z;

	
}
LOCAL void Watch_Cal_3D_Points(BMP_OBJ * current_bmp,
								IMGREF_3D_FLOAT_P_T watch_centre,
								int32  angle_x,
								IMGREF_POINT_T fix_point,
								int32  angle_y,
								int32 deep,
								IMGREF_3D_FLOAT_P_T * new_vertex)
{
	IMGREF_3D_FLOAT_P_T vertex[4]={0};
	int32 i=0;
	
	
	vertex[0].x=watch_centre.x-fix_point.x;
	vertex[0].y=watch_centre.y-fix_point.y;
	vertex[0].z=deep;
	vertex[1].x=watch_centre.x+(current_bmp->image_size.w-fix_point.x);
	vertex[1].y=watch_centre.y-fix_point.y;
	vertex[1].z=deep;
	vertex[2].x=watch_centre.x+(current_bmp->image_size.w-fix_point.x);
	vertex[2].y=watch_centre.y+(current_bmp->image_size.h-fix_point.y);
	vertex[2].z=deep;
	vertex[3].x=watch_centre.x-fix_point.x;
	vertex[3].y=watch_centre.y+(current_bmp->image_size.h-fix_point.y);
	vertex[3].z=deep;

	
	vertex[0].y=watch_centre.y-fix_point.y;
	vertex[0].z=deep;
	for(i=0;i<4;i++)
		{
			Watch_Rotate_Points(&new_vertex[i],
								angle_x, 
								angle_y, 
								vertex[i], 
								watch_centre);
		}
}
LOCAL void Watch_Make_Rotate(BMP_OBJ * current_bmp,
								IMGREF_3D_FLOAT_P_T * new_vertex,
								IMGREF_SIZE_T out_size,
								uint32 * out_buffer,
								DRAW_TYPE_E Draw_Type,
								IMGREF_3D_FLOAT_P_T ObserveEye,
								IMGREF_3D_FLOAT_P_T * Plane_vers,
								ANTIALIAS_TYPE_E anti_type)
{
	T_RECT src_rect={0};
	IMGREF_3D_FLOAT_P_T line[2]={0};
	
	switch(Draw_Type)
		{
			case DRAW_TYPE_DIRECT:
						
				
				break;
			case DRAW_TYPE_SRC:
				src_rect.x=0;
				src_rect.y=0;
				src_rect.w=current_bmp->image_size.w;
				src_rect.h=current_bmp->image_size.h;
				IMG_Rotation_watch( out_size, 
								current_bmp->image_size, 
								 src_rect, new_vertex,
								current_bmp->image_buffer, 
								out_buffer, 
								ObserveEye,
								anti_type);
				break;
			case DRAW_TYPE_SHADOW:
				
				break;
			default:
				break;
		}

	
}
LOCAL Watch_Draw_Shadow(WATCH_F_PARAM * shadwow_f_param)
{
	IMGREF_POINT_T fix_arm_p[3]={0};
	IMGREF_3D_FLOAT_P_T draw_points[3][4]={0};
	IMGREF_3D_FLOAT_P_T Null_Eye={0};
	int32 i=0,j=0;
	int32 angle_x[3]={0};
	
	fix_arm_p[0].x=shadwow_f_param->resource_collection[5].image_size.w/2;
	fix_arm_p[0].y=WATCH_HOUR_Y;
	fix_arm_p[1].x=shadwow_f_param->resource_collection[6].image_size.w/2;
	fix_arm_p[1].y=WATCH_MIN_Y;
	fix_arm_p[2].x=shadwow_f_param->resource_collection[7].image_size.w/2;
	fix_arm_p[2].y=WATCH_SECOND_Y;
    if((shadwow_f_param->Is_swing)&&(0!=shadwow_f_param->total_frame))
    {
        angle_x[0]=((shadwow_f_param->Hour%12)*300+shadwow_f_param->Min*5)*(s2d_sin_new(-900+1800*shadwow_f_param->cur_frame/shadwow_f_param->total_frame)+65536)/(2*65536);
    	angle_x[1]=shadwow_f_param->Min*60*(s2d_sin_new(-900+1800*shadwow_f_param->cur_frame/shadwow_f_param->total_frame)+65536)/(2*65536);
    	angle_x[2]=shadwow_f_param->Second*60*(s2d_sin_new(-900+1800*shadwow_f_param->cur_frame/shadwow_f_param->total_frame)+65536)/(2*65536);
    }
    else
    {
    	angle_x[0]=(shadwow_f_param->Hour%12)*300+shadwow_f_param->Min*5;
    	angle_x[1]=shadwow_f_param->Min*60;
    	angle_x[2]=shadwow_f_param->Second*60;
    }
	Null_Eye.x=1;
	Null_Eye.y=1;
	Null_Eye.z=1;

	for(i=0;i<3;i++)
		{
			Watch_Cal_3D_Points(&(shadwow_f_param->resource_collection[i+5]),
						shadwow_f_param->watch_centre, 
						 angle_x[i], 
						 fix_arm_p[i], 
						 0, 
						 0, 
						 * (draw_points+i));
			for(j=0;j<4;j++)
				{
					draw_points[i][j].x=draw_points[i][j].x-shadwow_f_param->watch_centre.x+shadwow_f_param->resource_collection[3].image_size.w/2+SHADOW_OFF_X;
					draw_points[i][j].y=draw_points[i][j].y-shadwow_f_param->watch_centre.y+shadwow_f_param->resource_collection[3].image_size.h/2+SHADOW_OFF_Y;
					draw_points[i][j].z=0;
				}
			Watch_Make_Rotate((&shadwow_f_param->resource_collection[5+i]), 
							draw_points[i],
							shadwow_f_param->resource_collection[3].image_size, 
							shadwow_f_param->resource_collection[3].image_buffer,
							DRAW_TYPE_SRC,
							Null_Eye,
							NULL,
							ANTIALIAS_TYPE_TOTAL);
			/*
			Draw_Pure_Quadrangle_PAF( shadwow_f_param->resource_collection[3].image_size, 
										 shadwow_f_param->resource_collection[3].image_buffer,
										  Null_Eye, 
										  draw_points[i],
										  WATCH_SHADOW_COLOR);
										  */
		}
}
PUBLIC void MMIWIDGET_Creat_Watch(WATCH_F_PARAM * indata_param)
{	
	int32 angle_x=0;
	int32 stop_point_x=0,current_x=0,amplitude=250;//(200=>471)
	int32 i=0;
	IMGREF_POINT_T fix_point={0};
	IMGREF_3D_FLOAT_P_T ObserveEye={0};
	IMGREF_3D_FLOAT_P_T Obj_Vers[6][4]={0};
	IMGREF_3D_FLOAT_P_T Shadow_Vers[3][4]={0};
	IMGREF_3D_FLOAT_P_T LinePoints[2]={0};
    static int32 angle_y=0;
	static int move_frame=0;
	int move_total_frame=0;
	int d_angle=25;
	static int start_angle_y=0;
    FIX16_POINT_T offset={0};
    
	if((NULL==indata_param)
		||(indata_param->Min>60)
		||(indata_param->Second>60)
		||(indata_param->Hour>=24))
		{
			return;
		}
	/*
	while(i<100)
	{
      i++;
	((uint32 *)indata_param->out_buffer)[0*240+i+0]=0x00ff0000;
	}
    return;
	*/
	if(indata_param->Is_swing)
		{	move_frame=0;
			if(0==(indata_param->total_frame))
				{
					return;
				}
                        /*

            Object_Swing(&angle_y, 
								(indata_param->cur_frame), 
								(indata_param->total_frame),
								s_watch_angle);
											*/

			stop_point_x=3600*4;
			
			if((indata_param->cur_frame)<=(indata_param->total_frame))
				{
					current_x=stop_point_x+1800-(stop_point_x-1800)*indata_param->cur_frame/(indata_param->total_frame);
				}
			else
				{
					current_x=0;
				}
			if((stop_point_x-900)==current_x)
			{
				current_x=current_x;
			}
			angle_y=amplitude*(current_x)*(22)/(10*1800*7)*(s2d_sin_new(current_x))/65536+0;
		}
    else 
		{
				if(angle_y!=s_watch_angle)
			{	
				if(0==move_frame)
					{
						start_angle_y=angle_y;
					}
				move_total_frame=abs((s_watch_angle-start_angle_y)/d_angle);
				if(0!=move_total_frame)
					{
						angle_y=start_angle_y+(s_watch_angle-start_angle_y)*(s2d_sin_new(-900+1800*move_frame/move_total_frame)+65536)/(2*65536);
					}
				else
					{
						angle_y=s_watch_angle;
					}
				move_frame++;
				if(move_frame>move_total_frame)
					{	
						angle_y=s_watch_angle;
						move_frame=0;
					}
			}
		}
	//angle_y=450;
	ObserveEye.x = indata_param->out_size.w/2;
	ObserveEye.y =indata_param->out_size.h/2;
	ObserveEye.z = -3000;
	#if 1
	fix_point.x=indata_param->resource_collection[4].image_size.w/2;
	fix_point.y=indata_param->resource_collection[4].image_size.h/2;
	Watch_Cal_3D_Points(&(indata_param->resource_collection[4]),
						indata_param->watch_centre, 
						 angle_x, 
						 fix_point, 
						 angle_y, 
						 WATCH_SUR_TWO_Z, 
					 * Obj_Vers);
			

	Watch_Make_Rotate(&(indata_param->resource_collection[4]), 
					* Obj_Vers, 
					indata_param->out_size, 
					(uint32 *)indata_param->out_buffer, 
					DRAW_TYPE_SRC,
					ObserveEye,
					NULL,
					ANTIALIAS_TYPE_EDGE_ONLY);
	//draw shadow to the surface two.
	Watch_Draw_Shadow(indata_param);
	//
	fix_point.x=indata_param->resource_collection[3].image_size.w/2;
	fix_point.y=indata_param->resource_collection[3].image_size.h/2;
	Watch_Cal_3D_Points(&(indata_param->resource_collection[3]),
						indata_param->watch_centre, 
						 angle_x, 
						 fix_point, 
						 angle_y, 
						 WATCH_SUR_ONE_Z, 
						 * (Obj_Vers+1));
	Watch_Make_Rotate(&(indata_param->resource_collection[3]), 
					*(Obj_Vers+1), 
					indata_param->out_size, 
					(uint32 *)indata_param->out_buffer, 
					DRAW_TYPE_SRC,
					ObserveEye,
					NULL,
					ANTIALIAS_TYPE_EDGE_ONLY);
	

	//draw shadows of h,m,s
	//hour shadow start
	fix_point.x=WATCH_HOUR_X;
	fix_point.y=WATCH_HOUR_Y;
    if(indata_param->Is_swing)
    {
        angle_x=((indata_param->Hour%12)*300+indata_param->Min*5)*(s2d_sin_new(-900+1800*(indata_param->cur_frame)/(indata_param->total_frame))+65536)/(2*65536);
    }
    else
    {
	    angle_x=(indata_param->Hour%12)*300+indata_param->Min*5;
    }
	Watch_Cal_3D_Points(&(indata_param->resource_collection[0]),
						indata_param->watch_centre, 
						 angle_x, 
						 fix_point, 
						 angle_y, 
						 WATCH_HOUR_Z, 
						 * (Obj_Vers+2));
	
	//hour shadow end
	
	//min shadow start
	fix_point.x=WATCH_MIN_X;
	fix_point.y=WATCH_MIN_Y;
    if(indata_param->Is_swing)
    {
         angle_x=indata_param->Min*60*(s2d_sin_new(-900+1800*(indata_param->cur_frame)/(indata_param->total_frame))+65536)/(2*65536);
    }
    else
    {
	    angle_x=indata_param->Min*60;
    }
	Watch_Cal_3D_Points(&(indata_param->resource_collection[1]),
						indata_param->watch_centre, 
						 angle_x, 
						 fix_point, 
						 angle_y, 
						 WATCH_MIN_Z, 
						 * (Obj_Vers+3));
	#endif 
	
	//seconds shadow start
	fix_point.x=WATCH_SECOND_X;
	fix_point.y=WATCH_SECOND_Y;
    if(indata_param->Is_swing)
    {
	    angle_x=indata_param->Second*60*(s2d_sin_new(-900+1800*(indata_param->cur_frame)/(indata_param->total_frame))+65536)/(2*65536);
    }
    
    else
    {
        angle_x=indata_param->Second*60;
    }
	Watch_Cal_3D_Points(&(indata_param->resource_collection[2]),
						indata_param->watch_centre, 
						 angle_x, 
						 fix_point, 
						 angle_y, 
						 WATCH_SECOND_Z, 
						 * (Obj_Vers+4));
	
	//draw hour//draw min//draw seconds
	#if 1
	for(i=0;i<3;i++)
		{	
			Watch_Make_Rotate(&(indata_param->resource_collection[i]), 
							*(Obj_Vers+i+2), 
							indata_param->out_size, 
							(uint32 *)indata_param->out_buffer, 
							DRAW_TYPE_SRC,
							ObserveEye,
							NULL,
							ANTIALIAS_TYPE_TOTAL);
		}
	
	#endif
	  //draw caps

	  	#if 1
	fix_point.x=indata_param->resource_collection[8].image_size.w/2;
	fix_point.y=indata_param->resource_collection[8].image_size.h/2;;
//	angle_x=indata_param->Second*60;
	Watch_Cal_3D_Points(&(indata_param->resource_collection[8]),
						indata_param->watch_centre, 
						 0, 
						 fix_point, 
						 angle_y, 
						 WATCH_SECOND_Z, 
						 * (Obj_Vers+5));
	Watch_Make_Rotate(&(indata_param->resource_collection[8]), 
							*(Obj_Vers+5), 
							indata_param->out_size, 
							(uint32 *)indata_param->out_buffer, 
							DRAW_TYPE_SRC,
							ObserveEye,
							NULL,
							ANTIALIAS_TYPE_TOTAL);
    #endif
    #if 0
    offset.x=WATCH_UP_A_X;
    offset.y=WATCH_UP_A_Y;
    S2D_Blending_PAF888( indata_param->out_size,
                    (uint32 *)indata_param->out_buffer, 
                    indata_param->resource_collection[9].image_size, 
                     offset, 
                     indata_param->resource_collection[9].image_buffer, 
                     (uint32 *)indata_param->out_buffer);
    offset.x=WATCH_DOWN_A_X;
    offset.y=WATCH_DOWN_A_Y;
    S2D_Blending_PAF888( indata_param->out_size,
                    (uint32 *)indata_param->out_buffer, 
                    indata_param->resource_collection[10].image_size, 
                     offset, 
                     indata_param->resource_collection[10].image_buffer, 
                     (uint32 *)indata_param->out_buffer);
    #endif
	
}
#if 0
//图片付给Image_Obj[]数组，顺序是时分秒表面一表面二
Image_Obj[0].image_size.w=6;
	Image_Obj[0].image_size.h=57;
	Image_Obj[0].route_str="watch\\hour.bmp";
	Image_Obj[1].image_size.w=4;
	Image_Obj[1].image_size.h=71;
	Image_Obj[1].route_str="watch\\min.bmp";
	Image_Obj[2].image_size.w=20;
	Image_Obj[2].image_size.h=76;
	Image_Obj[2].route_str="watch\\second.bmp";
	Image_Obj[3].image_size.w=172;
	Image_Obj[3].image_size.h=172;
	Image_Obj[3].route_str="watch\\surfaceup.bmp";
	Image_Obj[4].image_size.w=176;
	Image_Obj[4].image_size.h=176;
	Image_Obj[4].route_str="watch\\surfacedown.bmp";
	Image_Obj[5].image_size.w=8;
	Image_Obj[5].image_size.h=59;
	Image_Obj[5].route_str="watch\\sha_hour.bmp";
	Image_Obj[6].image_size.w=6;
	Image_Obj[6].image_size.h=73;
	Image_Obj[6].route_str="watch\\sha_min.bmp";
	Image_Obj[7].image_size.w=6;
	Image_Obj[7].image_size.h=78;
	Image_Obj[7].route_str="watch\\sha_second.bmp";
	Image_Obj[8].image_size.w=12;
	Image_Obj[8].image_size.h=12;
	Image_Obj[8].route_str="watch\\cap.bmp";
watch_fuction_param.out_buffer=Out32_Buffer;
	watch_fuction_param.out_size.w=WIDTH;
	watch_fuction_param.out_size.h=HEIGHT;
	watch_fuction_param.resource_collection=Image_Obj;
	watch_fuction_param.watch_centre.x=WIDTH/2;
	watch_fuction_param.watch_centre.y=HEIGHT/2;
	watch_fuction_param.watch_centre.z=(WATCH_SUR_TWO_Z+WATCH_SUR_ONE_Z)/2;
	watch_fuction_param.Second=frame/5;
	watch_fuction_param.Hour=7;
	watch_fuction_param.Min=12;
	watch_fuction_param.Is_swing=FALSE;
	watch_fuction_param.cur_frame=frame;
	watch_fuction_param.total_frame=24;
#endif


PUBLIC int32 * MMIWIDGET_Aclock_GetWatchAnglePtr(void) 
{
    return &s_watch_angle;
}
#endif
