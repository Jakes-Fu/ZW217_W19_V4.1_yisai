/*****************************************************************************
** File Name:      mmiwidget_weather.c                                        *
** Author:                                                                   *
** Date:           06/26/2012                                                 *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe widget weather special effect interface                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                  NAME             DESCRIPTION                        *
** 06/26/2012         anson.yin              Creat
******************************************************************************/
#if defined MMIWIDGET_WEATHER_SE
#include "sci_types.h"
#include "os_api.h"
//#include "mmiset.h"
#include "img_ref_types.h"
#include "mmk_type.h"
#include "graphics_3d_rotate.h"
#include "mmiwidget_weather_se.h"

#define WEATHER_ANAMI_C1_START_X 138
#define WEATHER_ANAMI_C1_START_Y 110
#define WEATHER_ANAMI_C1_END_X 240
#define WEATHER_ANAMI_C1_END_Y 190
#define WEATHER_ANAMI_C2_START_X 100
#define WEATHER_ANAMI_C2_START_Y 97
#define WEATHER_ANAMI_C2_END_X 51
#define WEATHER_ANAMI_C2_END_Y 35
#define WEATHER_ANAMI_C3_START_X 147
#define WEATHER_ANAMI_C3_START_Y 89
#define WEATHER_ANAMI_C3_END_X 214
#define WEATHER_ANAMI_C3_END_Y 47
#define WEATHER_ANAMI_C4_START_X 104
#define WEATHER_ANAMI_C4_START_Y 108
#define WEATHER_ANAMI_C4_END_X 40
#define WEATHER_ANAMI_C4_END_Y 174
#define WEATHER_BAR_ONE_X 91
#define WEATHER_BAR_TWO_X 152
#define WEATHER_BAR_ONE_Y 198
#define WEATHER_BAR_TWO_Y 198
#define WEATHER_DATE_CHANGE_X 83
#define WEATHER_DATE_CHANGE_Y 148
#define WEATHER_CITY_CHANGE_X 184
#define WEATHER_CITY_CHANGE_Y 147
#if 0
#define WEATHER_SUNDAY_X 43
#define WEATHER_SUNDAY_Y 189
#define WEATHER_MONDAY_X 104
#define WEATHER_MONDAY_Y 189
#define WEATHER_TUESDAY_X 164
#define WEATHER_TUESDAY_Y 189
#else
#define WEATHER_SUNDAY_X 34
#define WEATHER_SUNDAY_Y 189
#define WEATHER_MONDAY_X 95
#define WEATHER_MONDAY_Y 189
#define WEATHER_TUESDAY_X 156
#define WEATHER_TUESDAY_Y 189
#endif
#define WEATHER_TBIG_X 57
#define WEATHER_TBIG_Y 50
#define WEATHER_TSMALL_UP_X 173
#define WEATHER_TSMALL_UP_Y 45
#define WEATHER_TSMALL_DOWN_X 173
#define WEATHER_TSMALL_DOWN_Y 63
#define WEATHER_SUNNY_X 59
#define WEATHER_SUNNY_Y 24
#define WEATHER_SUNNY_RADIUS 45
#define WEATHER_CLOUD_L_X  44
#define WEATHER_CLOUD_L_MOVE_X  30
#define WEATHER_CLOUD_L_Y  58
#define WEATHER_CLOUD_R_X  104
#define WEATHER_CLOUD_R_MOVE_X  118
#define WEATHER_CLOUD_R_Y  75
#define WEATHER_RAIN_CLOUD_X 48
#define WEATHER_RAIN_CLOUD_Y 57
#define WEATHER_RAIN_S_X 48
#define WEATHER_RAIN_S_Y 73
#define WEATHER_RAIN_E_X 67
#define WEATHER_RAIN_E_Y 98
#define WEATHER_SUNC_CLOUD_L_Y_START  82
#define WEATHER_SUNC_CLOUD_R_Y_START  100
#define WEATHER_SUNC_CLOUD_L_X  43
#define WEATHER_SUNC_CLOUD_L_MOVE_X  30
#define WEATHER_SUNC_CLOUD_L_Y  69
#define WEATHER_SUNC_CLOUD_R_X  107
#define WEATHER_SUNC_CLOUD_R_MOVE_X  120
#define WEATHER_SUNC_CLOUD_R_Y  85
#define WEATHER_NA_C_YSTART  98
#define WEATHER_NA_TEXT_YSTART  88
#define WEATHER_NA_CLOUD_X 50
#define WEATHER_NA_CLOUD_Y 71
#define WEATHER_NA_TEXT_X 78
#define WEATHER_NA_TEXT_Y 68
#define WEATHER_BG_OFFSET_X 27
#define WEATHER_BG_OFFSET_Y (18-3)
#define WEATHER_FOGGY_X 42
#define WEATHER_FOGGY_Y 42
#define WEATHER_LIGHTING_ONE_X 78
#define WEATHER_LIGHTING_ONE_Y 105
#define MINUS_Y 20
#define MINUS_X 8
#define WEATHER_SNOWY_LEFT_ONE_X   (67-MINUS_X)
#define WEATHER_SNOWY_LEFT_ONE_Y   (102-MINUS_Y)
#define WEATHER_SNOWY_LEFT_TWO_X   (90-MINUS_X)
#define WEATHER_SNOWY_LEFT_TWO_Y   (112-MINUS_Y)
#define WEATHER_SNOWY_LEFT_THREE_X (60-MINUS_X)
#define WEATHER_SNOWY_LEFT_THREE_Y (125-MINUS_Y)
#define WEATHER_SNOWY_LEFT_FOUR_X  (85-MINUS_X)
#define WEATHER_SNOWY_LEFT_FOUR_Y  (141-MINUS_Y)
#define WEATHER_SNOWY_RIGHT_ONE_X  (110-MINUS_X)
#define WEATHER_SNOWY_RIGHT_ONE_Y  (99-MINUS_Y)
#define WEATHER_SNOWY_RIGHT_TWO_X  (130-MINUS_X)
#define WEATHER_SNOWY_RIGHT_TWO_Y  (112-MINUS_Y)
#define WEATHER_SNOWY_RIGHT_THREE_X  (100-MINUS_X)
#define WEATHER_SNOWY_RIGHT_THREE_Y  (133-MINUS_Y)
#define WEATHER_SNOWY_RIGHT_FOUR_X  (129-MINUS_X)
#define WEATHER_SNOWY_RIGHT_FOUR_Y  (143-MINUS_Y)

#define WEATHER_SNOWY_S_START_ONE_X  70
#define WEATHER_SNOWY_S_START_ONE_Y  106
#define WEATHER_SNOWY_S_END_ONE_X  70
#define WEATHER_SNOWY_S_END_ONE_Y  147

#define WEATHER_SNOWY_S_START_TWO_X  112
#define WEATHER_SNOWY_S_START_TWO_Y  105
#define WEATHER_SNOWY_S_END_TWO_X  112
#define WEATHER_SNOWY_S_END_TWO_Y  165

#define WEATHER_SNOWY_S_START_THREE_X  160
#define WEATHER_SNOWY_S_START_THREE_Y  101
#define WEATHER_SNOWY_S_END_THREE_X  160
#define WEATHER_SNOWY_S_END_THREE_Y  143

#define WEATHER_DISTANCE_Y  (-60)
#define WEATHER_DISAPPEAR_TOTAL_NUM 20
#define WEATHER_FRAME_ANAMI_IN  (65*2)
#define WEATHER_FRAME_ANAMI_TIMES  (WEATHER_FRAME_ANAMI_IN/(65))
PUBLIC void SetImgColor(uint32 * InColor,IMGREF_SIZE_T InSize)
{
    int32 i=0;
    while(i<InSize.w*InSize.h)
    {
       if(InColor[i]<=0x08000000)
       {
          InColor[i]=0;
       }
       i++;
    }
}
void Words_Make_Rotate(IMGREF_SIZE_T out_size,
							IMGREF_SIZE_T img_size,
							uint32 * Out_Color_Buffer,
							COLOR32 * In_Color_Buffer,
							ROTA_DATA_T img_data,
							int is_aixs_x,
							uint8 transparency)
{    
    S3D_ROTATION_GEOMETRY_T  s3d_geometry;
    S3D_ROTATION_IMG_T   s3d_src_img;
    S3D_ROTATION_INOUT_IMG_INFO_T  s3d_img;

    s3d_geometry.img_size.w=img_size.w;
    s3d_geometry.img_size.h=img_size.h;
    s3d_geometry.img_size.x =0;
    s3d_geometry.img_size.y = 0;

    s3d_geometry.angle=img_data.angle;
    s3d_geometry.eye.x=out_size.w/2;
    s3d_geometry.eye.y=out_size.h/2;
    s3d_geometry.eye.z=-2*out_size.w;

    s3d_geometry.axis.x=img_data.axis_x;
    s3d_geometry.axis.y=img_data.axis_y;
    s3d_geometry.axis.z=img_data.axis_z;
    s3d_geometry.axis.axis=is_aixs_x;
    s3d_geometry.is_axis_use_default = SCI_FALSE;
    s3d_geometry.is_eye_use_default = SCI_FALSE;

    s3d_src_img.src_ptr=In_Color_Buffer;
    s3d_src_img.src_type=S3D_DATA_PAF_RGB888;

    s3d_img.color_type=S3D_DATA_PAF_RGB888;
    s3d_img.color_ptr=Out_Color_Buffer;
    s3d_img.size.h=out_size.h;
    s3d_img.size.w=out_size.w;
	S3D_AxisMidRotation_Img ( &(s3d_geometry),
								&(s3d_src_img),
								&(s3d_img),
								transparency,
								NULL);
}
LOCAL void Weather_RoExist_Item(int32 cur_frame,
									  int32 start_frame,
									  int32 end_frame,
									  int16 offset_x,
									  int16 offset_y,
									  BMP_OBJ * image_obj,
									  uint32 * out_bg,
									IMGREF_SIZE_T bg_size
									  )
{	int32 current_t=0,current_f=0;
	ROTA_DATA_T  dates_rota={0};
	uint8 alpha_para=0;
	if((cur_frame>=start_frame))
	{	
		current_t=end_frame-start_frame;
		if(0 == current_t)
		{
			return;
		}
		current_f = cur_frame-start_frame;
		
		dates_rota.axis_x = offset_x+image_obj->image_size.w/2;
		dates_rota.axis_y = offset_y+image_obj->image_size.h/2;
		dates_rota.axis_z = 0;
		if(cur_frame <= end_frame)
		{
			dates_rota.angle = 900+900*current_f/current_t;
			alpha_para = 255*s2d_sin_new(900*current_f/current_t)/65536;
		}	
		else
		{
			dates_rota.angle = 1800;
			alpha_para = 255;
		}
		
		Words_Make_Rotate(bg_size, 
							image_obj->image_size,
							out_bg,
							 image_obj->image_buffer, 
							dates_rota,
							1, 
							alpha_para);
		
	}
}
LOCAL void Weather_ZoomExist_Item(int32 cur_frame,
									  int32 start_frame,
									  int32 end_frame,
									  int16 offset_x,
									  int16 offset_y,
									  uint8 alpha_start,
									  uint8 alpha_end,
									  int32 z_para_start,
									  int32 z_para_end,
									  BMP_OBJ * image_obj,
									  uint32 * out_bg,
									  IMGREF_SIZE_T bg_size,
									 FIX16_POINT_T * Point_Add_ptr)
{	
	int32 current_t=0,current_f=0;
	IMGREF_POINT_T win_mid={0},img_mid={0};
	uint8 alpha_para=0;
	int32 z_para=0;
	if((cur_frame>=start_frame))
	{	
		current_t=end_frame-start_frame;
		if(0==current_t)
		{
			return;
		}
		current_f = cur_frame-start_frame;
		img_mid.x = image_obj->image_size.w/2;
		img_mid.y = image_obj->image_size.h/2;
		win_mid.x = offset_x+image_obj->image_size.w/2;
		win_mid.y = offset_y+image_obj->image_size.h/2;
		if(cur_frame <= end_frame)
		{
			z_para = z_para_start+(z_para_end-z_para_start)*s2d_sin_new(900*current_f/current_t)/65536;
			alpha_para = alpha_start+(alpha_end-alpha_start)*s2d_sin_new(900*current_f/current_t)/65536;
		}	
		else
		{
			z_para = z_para_end;
			alpha_para = alpha_end;
		}
		
		S2d_4_in_one_fast_PAF888_Zerosize(bg_size, 
							image_obj->image_size,
							win_mid,
							 img_mid, 0, 
							 image_obj->image_buffer, 
							 out_bg,
							 2,
							  z_para, 
							  alpha_para, 
							   Point_Add_ptr);
		
	}
}
LOCAL void Weather_MoveExist_Item(int32 cur_frame,
									  int32 start_frame,
									  int32 end_frame,
									  IMGREF_POINT_T start_point,
									  IMGREF_POINT_T end_point,
									  uint8 alpha_start,
									  uint8 alpha_end,
									  int32 z_para_start,
									  int32 z_para_end,
									  BMP_OBJ * image_obj,
									  uint32 * out_bg,
									  IMGREF_SIZE_T bg_size,
									 FIX16_POINT_T * Point_Add_ptr)
{	
	int32 current_f=0;
	int32 current_t=0;
	IMGREF_POINT_T offset={0};
	
	current_f=cur_frame-start_frame;
	current_t=end_frame-start_frame;
	if(current_t!=0)
	{
		offset.x=start_point.x+(end_point.x-start_point.x)*current_f/current_t;
		offset.y=start_point.y+(end_point.y-start_point.y)*current_f/current_t;
	}
	Weather_ZoomExist_Item( cur_frame, 
					start_frame,
					end_frame,
					offset.x, 
					offset.y,
					alpha_start,
					alpha_end,
					z_para_start,
					z_para_end,
					image_obj, 
					out_bg, 
					bg_size, 
					Point_Add_ptr);
}

void WeatherForcast_Dates_Rotation(int32 cur_frame,
									int32 t_frame,
									BMP_OBJ * weather_change,
									BMP_OBJ * city_change,
									BMP_OBJ * sunday,
									BMP_OBJ * monday,
									BMP_OBJ * tuesday,
									BMP_OBJ * bar,
									uint32 * out_bg,
									IMGREF_SIZE_T bg_size,
									FIX16_POINT_T * Point_Add_ptr)
{	
	uint8 j=0;
	int32 frame_array[10]={0};
	int32 current_f=0;
	int32 current_t=0;
	int32 z_para=0;
	int32 alpha_para=0;
	BMP_OBJ *  temp_bmp={0};
	
	IMGREF_POINT_T window_mid[3]={0};
	IMGREF_POINT_T img_mid={0};
	
	if(0==t_frame)
	{
		return;
	}

	frame_array[0]=t_frame*65*WEATHER_FRAME_ANAMI_TIMES/t_frame;	
	frame_array[1]=t_frame*77*WEATHER_FRAME_ANAMI_TIMES/t_frame;	
	frame_array[2]=t_frame*65*WEATHER_FRAME_ANAMI_TIMES/t_frame;	
	frame_array[3]=t_frame*71*WEATHER_FRAME_ANAMI_TIMES/t_frame;	
	frame_array[4]=t_frame*67*WEATHER_FRAME_ANAMI_TIMES/t_frame;	
	frame_array[5]=t_frame*77*WEATHER_FRAME_ANAMI_TIMES/t_frame;	
	frame_array[6]=t_frame*69*WEATHER_FRAME_ANAMI_TIMES/t_frame;	
	frame_array[7]=t_frame*79*WEATHER_FRAME_ANAMI_TIMES/t_frame;	
	frame_array[8]=t_frame*71*WEATHER_FRAME_ANAMI_TIMES/t_frame;	
	frame_array[9]=t_frame*81*WEATHER_FRAME_ANAMI_TIMES/t_frame;	
	


	img_mid.x=bar->image_size.w/2;
	img_mid.y=bar->image_size.h/2;
	window_mid[0].x=img_mid.x+WEATHER_BAR_ONE_X;
	window_mid[0].y=img_mid.y+WEATHER_BAR_ONE_Y;
	window_mid[1].x=img_mid.x+WEATHER_BAR_TWO_X;
	window_mid[1].y=img_mid.y+WEATHER_BAR_TWO_Y;

	if((cur_frame<frame_array[1])&&(cur_frame>=frame_array[0]))
	{	
		current_t=frame_array[1]-frame_array[0];
		if(0==current_t)
		{
			return;
		}
		current_f=cur_frame-frame_array[1];
		alpha_para=255*s2d_sin_new(900*current_f/current_t)/65536;
		z_para=4096;
		
		while(j<2)
		{	S2d_4_in_one_fast_PAF888_Zerosize(bg_size, 
								bar->image_size,
								 window_mid[j],
								 img_mid, 0, 
								 bar->image_buffer, 
								 out_bg, 
								 1,
								  z_para, 
								  alpha_para, 
								   Point_Add_ptr);
	j++;
	#if 0
			S2d_4_in_one_fast(bg_size, 
								bar->image_size,
								 window_mid[j],
								 img_mid, 0, 
								 bar->image_buffer, 
								 out_bg, 
								  z_para, 
								  alpha_para, 
								   Point_Add_ptr);
	#endif
		}
	}
	else
	{
		alpha_para=255;
		z_para=4096;
		
		while(j<2)
		{	
			S2d_4_in_one_fast_PAF888_Zerosize(bg_size, 
								bar->image_size,
								 window_mid[j],
								 img_mid, 0, 
								 bar->image_buffer, 
								 out_bg, 
								 1,
								  z_para, 
								  alpha_para, 
								   Point_Add_ptr);
			j++;

		#if 0
			S2d_4_in_one_fast(bg_size, 
								bar->image_size,
								 window_mid[j],
								 img_mid, 0, 
								 bar->image_buffer, 
								 out_bg, 
								  z_para, 
								  alpha_para, 
								   Point_Add_ptr);
		#endif
		}
	}
	//add change weather icon
	Weather_RoExist_Item( cur_frame, 
						frame_array[2], 
						frame_array[3], 
						WEATHER_DATE_CHANGE_X, 
						WEATHER_DATE_CHANGE_Y,
						weather_change, 
						 out_bg,
						 bg_size);
	Weather_RoExist_Item( cur_frame, 
						frame_array[2], 
						frame_array[3], 
						WEATHER_CITY_CHANGE_X, 
						WEATHER_CITY_CHANGE_Y,
						city_change, 
						 out_bg,
						 bg_size);
	// add sunday
	Weather_RoExist_Item( cur_frame, 
						frame_array[4], 
						frame_array[5], 
						WEATHER_SUNDAY_X, 
						WEATHER_SUNDAY_Y,
						sunday, 
						 out_bg,
						 bg_size);
	// add monday
	Weather_RoExist_Item( cur_frame, 
						frame_array[6], 
						frame_array[7], 
						WEATHER_MONDAY_X, 
						WEATHER_MONDAY_Y,
						monday, 
						 out_bg,
						 bg_size);
	// add tuesday
	Weather_RoExist_Item( cur_frame, 
						frame_array[8], 
						frame_array[9], 
						WEATHER_TUESDAY_X, 
						WEATHER_TUESDAY_Y,
						tuesday, 
						 out_bg,
						 bg_size);
		
	
	
}

LOCAL void WeatherForcast_T_Exist(int32 cur_frame,
									int32 start_frame,
									int32 end_frame,
									BMP_OBJ * big_t,
									BMP_OBJ * small_t_up,
									BMP_OBJ * small_t_down,
									uint32* out_bg,
									IMGREF_SIZE_T bg_size,
									FIX16_POINT_T * Point_Add_ptr)
{	
	uint8 j=0;
	int32 frame_array[8]={0};
	int32 current_f=0;
	int32 current_t=0;
	int32 z_para=0;
	int32 alpha_para=0;
	BMP_OBJ *  temp_bmp={0};

	IMGREF_POINT_T window_mid={0};
	IMGREF_POINT_T img_mid={0};

	
	
	
	Weather_ZoomExist_Item( cur_frame, 
							start_frame, 
							end_frame, 
							WEATHER_TBIG_X, 
							WEATHER_TBIG_Y,
							0,
							255,
							4096,4096,
							big_t, 
							out_bg,  bg_size,  Point_Add_ptr);
	Weather_ZoomExist_Item( cur_frame, 
							start_frame, 
							end_frame, 
							WEATHER_TSMALL_UP_X, 
							WEATHER_TSMALL_UP_Y, 
							0,
							255,
							4096,4096,small_t_up, 
							out_bg,  bg_size,  Point_Add_ptr);
	Weather_ZoomExist_Item( cur_frame, 
							start_frame, 
							end_frame, 
							WEATHER_TSMALL_DOWN_X, 
							WEATHER_TSMALL_DOWN_Y, 
							0,
							255,
							4096,4096,
							small_t_down, 
							out_bg,  bg_size,  Point_Add_ptr);
			
	
	
}

void WeatherForcast_Anami_Cloud_IN(int32 cur_frame,
									int32 t_frame,
									BMP_OBJ * cloud_one,
									BMP_OBJ * cloud_two,
									uint32 * out_bg,
									IMGREF_SIZE_T bg_size,
									FIX16_POINT_T * Point_Add_ptr)
{	
	uint8 j=0;
	int32 frame_array[8]={0};
	int32 current_f=0;
	int32 current_t=0;
	int32 z_para=0;
	int32 alpha_para=0;
	BMP_OBJ *  temp_bmp={0};
	IMGREF_POINT_T move_points[8]={0};
	IMGREF_POINT_T window_mid={0};
	IMGREF_POINT_T img_mid={0};
	if(0==t_frame)
		{
			return;
		}

	frame_array[0]=t_frame*0/t_frame;	
	frame_array[1]=t_frame*20*WEATHER_FRAME_ANAMI_TIMES/t_frame;	
	frame_array[2]=t_frame*15*WEATHER_FRAME_ANAMI_TIMES/t_frame;	
	frame_array[3]=t_frame*35*WEATHER_FRAME_ANAMI_TIMES/t_frame;	
	frame_array[4]=t_frame*30*WEATHER_FRAME_ANAMI_TIMES/t_frame;	
	frame_array[5]=t_frame*50*WEATHER_FRAME_ANAMI_TIMES/t_frame;	
	frame_array[6]=t_frame*45*WEATHER_FRAME_ANAMI_TIMES/t_frame;	
	frame_array[7]=t_frame*WEATHER_FRAME_ANAMI_IN/t_frame;	

	move_points[0].x=WEATHER_ANAMI_C1_START_X ;	
	move_points[0].y=WEATHER_ANAMI_C1_START_Y ;	
	move_points[1].x=WEATHER_ANAMI_C1_END_X ;	
	move_points[1].y=WEATHER_ANAMI_C1_END_Y ;	
	move_points[2].x=WEATHER_ANAMI_C2_START_X ;	
	move_points[2].y=WEATHER_ANAMI_C2_START_Y ;	
	move_points[3].x=WEATHER_ANAMI_C2_END_X ;	
	move_points[3].y=WEATHER_ANAMI_C2_END_Y ;	
	move_points[4].x=WEATHER_ANAMI_C3_START_X ;	
	move_points[4].y=WEATHER_ANAMI_C3_START_Y ;	
	move_points[5].x=WEATHER_ANAMI_C3_END_X ;	
	move_points[5].y=WEATHER_ANAMI_C3_END_Y ;	
	move_points[6].x=WEATHER_ANAMI_C4_START_X ;	
	move_points[6].y=WEATHER_ANAMI_C4_START_Y ;	
	move_points[7].x=WEATHER_ANAMI_C4_END_X ;	
	move_points[7].y=WEATHER_ANAMI_C4_END_Y ;	
	
	while(j<4)
	{	
		if((j==0)||(j==2))
		{
			temp_bmp=cloud_one;
		}
		else
		{
			temp_bmp=cloud_two;
		}
		if((cur_frame<=frame_array[j*2+1])&&(cur_frame>=frame_array[j*2]))
		{	
			current_t=frame_array[j*2+1]-frame_array[j*2];
			if(0==current_t)
			{
				return;
			}
			current_f=cur_frame-frame_array[j*2];
			alpha_para=255*s2d_sin_new(1800*current_f/current_t)/65536;
			z_para=4096*60/100+4096*s2d_sin_new(900*current_f/current_t)/65536;
			window_mid.x=move_points[j*2].x+(move_points[j*2+1].x-move_points[j*2].x)*current_f/current_t;
			window_mid.y=move_points[j*2].y+(move_points[j*2+1].y-move_points[j*2].y)*current_f/current_t;
			img_mid.x=temp_bmp->image_size.w/2;
			img_mid.y=temp_bmp->image_size.h/2;
			S2d_4_in_one_fast_PAF888_Zerosize(bg_size, 
								temp_bmp->image_size,
								 window_mid,
								 img_mid, 0, 
								 temp_bmp->image_buffer, 
								 out_bg, 
								 2,
								  z_para, 
								  alpha_para, 
								   Point_Add_ptr);
			
		}
		j++;
	}
	
	
}

void WeatherForcast_NA(int32 cur_frame,
									int32 t_frame,
									BMP_OBJ * cloud_left,
									BMP_OBJ * na,
									uint32 * out_bg,
									IMGREF_SIZE_T bg_size,
									FIX16_POINT_T * Point_Add_ptr)
{	
	uint8 j=0;
	int32 frame_array[2]={0};
	int32 current_f=0;
	int32 current_t=0;
	int32 z_para=0;
	int32 alpha_para=0;
	int32 cur_angle=0;
	BMP_OBJ *  temp_bmp={0};
	int32 rota_angle=0;
	static int32 loop_frame=0;
	static int32 loop_total_frame=0;
	FIX16_POINT_T offset_fix16={0};
	int32 y_distance=0;
	IMGREF_POINT_T offset={0};
	IMGREF_POINT_T move_points[3]={0};
	int32 y_start[2]={0};
	int32 stop_point_x=0,current_x=0,amplitude=0;
	if(0==t_frame)
	{
		return;
	}
	stop_point_x=3600*2;
	loop_total_frame=50;
	y_start[0]=WEATHER_NA_C_YSTART;
	y_start[1]=WEATHER_NA_TEXT_YSTART;
	frame_array[0]=t_frame*137/213;	
	frame_array[1]=t_frame*153/213;
	frame_array[2]=t_frame*173/213;
	move_points[0].x=WEATHER_NA_CLOUD_X;
	move_points[0].y=WEATHER_NA_CLOUD_Y;
	
	move_points[1].x=WEATHER_NA_TEXT_X;
	move_points[1].y=WEATHER_NA_TEXT_Y;
	amplitude=15;
	//clouds exits start
	if((cur_frame>=frame_array[0])&&(cur_frame<frame_array[2]))
	{	
		current_f=cur_frame-frame_array[0];
		current_t=frame_array[1]-frame_array[0];
		offset.x=move_points[0].x;
		if(cur_frame<=frame_array[1])
		{
			offset.y=y_start[0]+(move_points[0].y-y_start[0])*s2d_sin_new(900*(current_f)/current_t)/65536;
		}
		else
		{
			offset.y=move_points[0].y;
		}
		Weather_ZoomExist_Item( cur_frame, 
								frame_array[0], 
								frame_array[1], 
								offset.x, 
								offset.y,
								50,
								255,
								4096,4096,
								cloud_left, 
								out_bg,  bg_size,  Point_Add_ptr);
		offset.x=move_points[1].x;
		if(cur_frame<=frame_array[1])
		{
			offset.y=y_start[1]+(move_points[1].y-y_start[1])*s2d_sin_new(900*(current_f)/current_t)/65536;
		}
		else
		{
			offset.y=move_points[1].y;
		}
		Weather_ZoomExist_Item( cur_frame, 
								frame_array[0], 
								frame_array[1], 
								offset.x, 
								offset.y,
								0,
								255,
								4096,4096,
								na, 
								out_bg,  bg_size,  Point_Add_ptr);
	}
	//clouds exits end
	else if(cur_frame>=frame_array[2])
	{	if(cur_frame==frame_array[2])
    	{
            loop_frame=0;
    	}
		if(loop_frame<=(loop_total_frame*3/5))
		{
			current_x=stop_point_x-stop_point_x*loop_frame/(loop_total_frame*3/5);
		}
		else
		{
			current_x=0;
		}
		//current_x=current_x+3600;
		y_distance=amplitude*(current_x)*(22)/(10*1800*7)*(s2d_sin_new(current_x))+move_points[0].y*65536;
		offset_fix16.y=y_distance/65536;
		offset_fix16.y_dec=y_distance%65536;
		
		if(offset_fix16.y_dec>65536/2)
		{
			offset_fix16.y++;
		}
		offset_fix16.y_dec=0;
		offset_fix16.x=move_points[0].x;
		offset_fix16.x_dec=0;
		
		S2D_Blending_PAF888( bg_size,
					  out_bg, 
					  cloud_left->image_size, 
					  offset_fix16, 
					  cloud_left->image_buffer, 
					  out_bg);
		
		
		y_distance=amplitude*(current_x)*(22)/(10*1800*7)*(s2d_sin_new(current_x))+move_points[1].y*65536;
		offset_fix16.y=y_distance/65536;
		offset_fix16.y_dec=y_distance%65536;
		
		if(offset_fix16.y_dec>65536/2)
		{
			offset_fix16.y++;
		}
		offset_fix16.y_dec=0;
		offset_fix16.x=move_points[1].x;
		offset_fix16.x_dec=0;
		
		
		
		S2D_Blending_PAF888( bg_size,
					  out_bg, 
					  na->image_size, 
					  offset_fix16, 
					  na->image_buffer, 
					  out_bg);
		
		loop_frame++;
		if(loop_frame>=loop_total_frame)
		{
			loop_frame=0;
		}
	}
		//clouds exits end

	
}

void WeatherForcast_SUNCHANGE_CLOUD(int32 cur_frame,
									int32 t_frame,
									BMP_OBJ * cloud_left,
									BMP_OBJ * cloud_right,
									uint32 * out_bg,
									IMGREF_SIZE_T bg_size,
									FIX16_POINT_T * Point_Add_ptr)
{	
	uint8 j=0;
	int32 frame_array[2]={0};
	int32 current_f=0;
	int32 current_t=0;
	int32 z_para=0;
	int32 alpha_para=0;
	int32 cur_angle=0;
	BMP_OBJ *  temp_bmp={0};
	int32 rota_angle=0;
	static int32 loop_frame=0;
	static int32 loop_total_frame=0;
	FIX16_POINT_T offset_fix16={0};
	int32 x_distance=0;
	IMGREF_POINT_T offset={0};
	IMGREF_POINT_T move_points[4]={0};
	int32 y_start[2]={0};
	if(0==t_frame)
	{
		return;
	}
	loop_total_frame=40*3/2;
	y_start[0]=WEATHER_SUNC_CLOUD_L_Y_START;
	y_start[1]=WEATHER_SUNC_CLOUD_R_Y_START;
	frame_array[0]=t_frame*141/210;	
	frame_array[1]=t_frame*158/210;	
	move_points[0].x=WEATHER_SUNC_CLOUD_L_X;
	move_points[0].y=WEATHER_SUNC_CLOUD_L_Y;
	move_points[1].x=WEATHER_SUNC_CLOUD_L_MOVE_X;
	move_points[1].y=WEATHER_SUNC_CLOUD_L_Y;
	move_points[2].x=WEATHER_SUNC_CLOUD_R_X;
	move_points[2].y=WEATHER_SUNC_CLOUD_R_Y;
	move_points[3].x=WEATHER_SUNC_CLOUD_R_MOVE_X;
	move_points[3].y=WEATHER_SUNC_CLOUD_R_Y;
	//clouds exits start
	if((cur_frame>=frame_array[0])&&(cur_frame<frame_array[1]))
	{	current_f=cur_frame-frame_array[0];
		current_t=frame_array[1]-frame_array[0];
		offset.x=move_points[0].x;
		offset.y=y_start[0]+(move_points[0].y-y_start[0])*s2d_sin_new(900*(current_f)/current_t)/65536;
		Weather_ZoomExist_Item( cur_frame, 
								frame_array[0], 
								frame_array[1], 
								offset.x, 
								offset.y,
								50,
								255,
								4096,4096,
								cloud_left, 
								out_bg,  bg_size,  Point_Add_ptr);
		offset.x=move_points[2].x;
		offset.y=y_start[1]+(move_points[2].y-y_start[1])*s2d_sin_new(900*(current_f)/current_t)/65536;
		Weather_ZoomExist_Item( cur_frame, 
								frame_array[0], 
								frame_array[1], 
								offset.x, 
								offset.y,
								0,
								255,
								4096,4096,
								cloud_right, 
								out_bg,  bg_size,  Point_Add_ptr);
	}
	//clouds exits end
	else if(cur_frame>=frame_array[1])
	{	if(frame_array[1]==cur_frame)
    	{
            loop_frame=0;
    	}
		//x_distance=move_points[0].x*65536+(move_points[1].x-move_points[0].x)*s2d_sin_new(1800*loop_frame/loop_total_frame);
		x_distance=(move_points[0].x+move_points[1].x)*65536/2+(move_points[0].x-move_points[1].x)*s2d_cos_new(3600*loop_frame/loop_total_frame)/2;
		
		offset_fix16.x=x_distance/65536;
		offset_fix16.x_dec=x_distance%65536;
		
		if(offset_fix16.x_dec>65536/2)
		{
				offset_fix16.x++;
		}
		offset_fix16.x_dec=0;
		
		offset_fix16.y=move_points[0].y;
		offset_fix16.y_dec=0;
	
		S2D_Blending_PAF888( bg_size,
					  out_bg, 
					  cloud_left->image_size, 
					  offset_fix16, 
					  cloud_left->image_buffer, 
					  out_bg);
		
		//x_distance=move_points[2].x*65536+(move_points[3].x-move_points[2].x)*s2d_sin_new(1800*loop_frame/loop_total_frame);
		x_distance=(move_points[2].x+move_points[3].x)*65536/2+(move_points[2].x-move_points[3].x)*s2d_cos_new(3600*loop_frame/loop_total_frame)/2;

		offset_fix16.x=x_distance/65536;
		offset_fix16.x_dec=x_distance%65536;
		
		if(offset_fix16.x_dec>65536/2)
		{
				offset_fix16.x++;
		}
		offset_fix16.x_dec=0;
		
		offset_fix16.y=move_points[2].y;
		offset_fix16.y_dec=0;
		S2D_Blending_PAF888( bg_size,
					  out_bg, 
					  cloud_right->image_size, 
					  offset_fix16, 
					  cloud_right->image_buffer, 
					  out_bg);
		loop_frame++;
		if(loop_frame>loop_total_frame)
		{
			loop_frame=0;
		}
	}
	
		//clouds exits end

	
}

void WeatherForcast_RAINNY(int32 cur_frame,
									int32 t_frame,
									BMP_OBJ * cloud_left,
									BMP_OBJ * rain,
									uint32 * out_bg,
									IMGREF_SIZE_T bg_size,
									FIX16_POINT_T * Point_Add_ptr)
{	
	uint8 j=0;
	int32 frame_array[8]={0};
	int32 current_f=0;
	int32 current_t=0;
	int32 z_para=0;
	int32 alpha_para=0;
	int32 cur_angle=0;
	BMP_OBJ *  temp_bmp={0};
	int32 rota_angle=0;
	static int32 loop_frame=0;
	static int32 loop_total_frame=0;
	IMGREF_POINT_T offset={0};
	int32 x_distance=0;
	
	IMGREF_POINT_T move_points[3]={0};
	int32 start_frame=0,end_frame=0;
	if(0==t_frame)
	{
		return;
	}
	loop_total_frame=40*3/2;
	
	frame_array[0]=t_frame*66/145;	
	frame_array[1]=t_frame*86/145;
	frame_array[2]=t_frame*85/145;	
	frame_array[3]=t_frame*91/145;	
	frame_array[4]=t_frame*145/145;	
	frame_array[5]=t_frame*85/145;
	frame_array[6]=t_frame*98/145;	
	frame_array[7]=t_frame*114/145;
	
	move_points[0].x=WEATHER_RAIN_CLOUD_X;
	move_points[0].y=WEATHER_RAIN_CLOUD_Y;
	move_points[1].x=WEATHER_RAIN_S_X;
	move_points[1].y=WEATHER_RAIN_S_Y;
	move_points[2].x=WEATHER_RAIN_E_X;
	move_points[2].y=WEATHER_RAIN_E_Y;
	
	//clouds exits start
	 
	Weather_ZoomExist_Item( cur_frame, 
									frame_array[0], 
									frame_array[1], 
									move_points[0].x, 
									move_points[0].y,
									0,
									255,
									2048,4096,
									cloud_left, 
									out_bg,  bg_size,  Point_Add_ptr);
	
	
	//clouds exits end
	//start loop rain 
 
	
    if(cur_frame>=frame_array[1])
	{	
		if(cur_frame<=(frame_array[3]))
		{
			Weather_ZoomExist_Item( loop_frame, 
							frame_array[2],
							frame_array[3],
							move_points[1].x, 
							move_points[1].y,
							0,
							255*3/10,
							4096,4096,
							rain, 
							out_bg,  bg_size,  Point_Add_ptr);
		}
		else
		{
			Weather_ZoomExist_Item( loop_frame, 
							frame_array[3],
							frame_array[4],
							move_points[1].x, 
							move_points[1].y,
							255*3/10,
							255,
							4096,4096,
							rain, 
							out_bg,  bg_size,  Point_Add_ptr);
		}
		current_t=(frame_array[7]-frame_array[5]);
		if(0==current_t)
		{
			return;
		}
		offset.x=move_points[1].x+(move_points[2].x-move_points[1].x)*loop_frame/current_t;
		offset.y=move_points[1].y+(move_points[2].y-move_points[1].y)*loop_frame/current_t;
		if(loop_frame<=(frame_array[6]-frame_array[5]))
		{
			Weather_ZoomExist_Item( loop_frame, 
							frame_array[2]-frame_array[2], 
							frame_array[3]-frame_array[2], 
							offset.x, 
							offset.y,
							0,
							255,
							4096,4096,
							rain, 
							out_bg,  bg_size,  Point_Add_ptr);
		}
		else if((loop_frame>(frame_array[6]-frame_array[5]))&&(loop_frame<=(frame_array[7]-frame_array[5])))
		{
			Weather_ZoomExist_Item( loop_frame, 
							frame_array[3]-frame_array[2], 
							frame_array[4]-frame_array[2], 
							offset.x, 
							offset.y,
							255,
							0,
							4096,4096,
							rain, 
							out_bg,  bg_size,  Point_Add_ptr);
		}
		loop_frame++;
		if(loop_frame>(frame_array[7]-frame_array[5]))
		{
			loop_frame=0;
		}
	}
	
	//end loop rain 
 	
}

void WeatherForcast_CLOUDY(int32 cur_frame,
									int32 t_frame,
									BMP_OBJ * cloud_left,
									BMP_OBJ * cloud_right,
									uint32 * out_bg,
									IMGREF_SIZE_T bg_size,
									FIX16_POINT_T * Point_Add_ptr)
{	
	uint8 j=0;
	int32 frame_array[2]={0};
	int32 current_f=0;
	int32 current_t=0;
	int32 z_para=0;
	int32 alpha_para=0;
	int32 cur_angle=0;
	BMP_OBJ *  temp_bmp={0};
	int32 rota_angle=0;
	static int32 loop_frame=0;
	static int32 loop_total_frame=0;
	FIX16_POINT_T offset_fix16={0};
	int32 x_distance=0;
	IMGREF_POINT_T img_mid={0};
	IMGREF_POINT_T move_points[4]={0};
	
	if(0==t_frame)
	{
		return;
	}
	loop_total_frame=50;
	frame_array[0]=t_frame*66/142;	
	frame_array[1]=t_frame*86/142;	
	move_points[0].x=WEATHER_CLOUD_L_X;
	move_points[0].y=WEATHER_CLOUD_L_Y;
	move_points[1].x=WEATHER_CLOUD_L_MOVE_X;
	move_points[1].y=WEATHER_CLOUD_L_Y;
	move_points[2].x=WEATHER_CLOUD_R_X;
	move_points[2].y=WEATHER_CLOUD_R_Y;
	move_points[3].x=WEATHER_CLOUD_R_MOVE_X;
	move_points[3].y=WEATHER_CLOUD_R_Y;
	//clouds exits start
	
	if((cur_frame>=frame_array[0])&&(cur_frame<frame_array[1]))
	{
		Weather_ZoomExist_Item( cur_frame, 
								frame_array[0], 
								frame_array[1], 
								WEATHER_CLOUD_L_X, 
								WEATHER_CLOUD_L_Y,
								0,
								255,
								0,4096,
								cloud_left, 
								out_bg,  bg_size,  Point_Add_ptr);
		Weather_ZoomExist_Item( cur_frame, 
								frame_array[0], 
								frame_array[1], 
								WEATHER_CLOUD_R_X, 
								WEATHER_CLOUD_R_Y,
								0,
								255,
								0,4096,
								cloud_right, 
								out_bg,  bg_size,  Point_Add_ptr);
	}
	//clouds exits end
	else if(cur_frame>=frame_array[1])
	{	if(frame_array[1]==cur_frame)
    	{
          loop_frame=0;
    	}
		//x_distance=move_points[0].x*65536+(move_points[1].x-move_points[0].x)*s2d_sin_new(1800*loop_frame/loop_total_frame);
		x_distance=(move_points[0].x+move_points[1].x)*65536/2+(move_points[0].x-move_points[1].x)*s2d_cos_new(3600*loop_frame/loop_total_frame)/2;
		
		offset_fix16.x=x_distance/65536;
		offset_fix16.x_dec=x_distance%65536;
		
		if(offset_fix16.x_dec>65536/2)
		{
		    offset_fix16.x++;
		}
		offset_fix16.x_dec=0;
		
		offset_fix16.y=move_points[0].y;
		offset_fix16.y_dec=0;
	
		S2D_Blending_PAF888( bg_size,
					  out_bg, 
					  cloud_left->image_size, 
					  offset_fix16, 
					  cloud_left->image_buffer, 
					  out_bg);
		
		//x_distance=move_points[2].x*65536+(move_points[3].x-move_points[2].x)*s2d_sin_new(1800*loop_frame/loop_total_frame);
		x_distance=(move_points[2].x+move_points[3].x)*65536/2+(move_points[2].x-move_points[3].x)*s2d_cos_new(3600*loop_frame/loop_total_frame)/2;

		offset_fix16.x=x_distance/65536;
		offset_fix16.x_dec=x_distance%65536;
		
		if(offset_fix16.x_dec>65536/2)
		{
			offset_fix16.x++;
		}
		offset_fix16.x_dec=0;
		
		offset_fix16.y=move_points[2].y;
		offset_fix16.y_dec=0;
		S2D_Blending_PAF888( bg_size,
					  out_bg, 
					  cloud_right->image_size, 
					  offset_fix16, 
					  cloud_right->image_buffer, 
					  out_bg);
		loop_frame++;
		if(loop_frame>loop_total_frame)
		{
			loop_frame=0;
		}
	}
	
		//clouds exits end
}

void WeatherForcast_SUNNY(int32 cur_frame,
									int32 t_frame,
									BMP_OBJ * sun,
									BMP_OBJ * Glow,
									BMP_OBJ * sun_bg,
									uint32 * out_bg,
									IMGREF_SIZE_T bg_size,
									FIX16_POINT_T * Point_Add_ptr)
{	
	uint8 j=0;
	int32 frame_array[8]={0};
	int32 current_f=0;
	int32 current_t=0;
	int32 z_para=0;
	int32 alpha_para=0;
	int32 cur_angle=0;
	BMP_OBJ *  temp_bmp={0};
	int32 rota_angle=0;
	static int32 loop_frame=0;
	static int32 loop_total_frame=0;
	IMGREF_POINT_T window_mid={0};
	IMGREF_POINT_T img_mid={0};
	int32 further_frame=20;
	if(0==t_frame)
	{
		return;
	}
	/*
	frame_array[0]=t_frame*(93-further_frame)/193;	
	frame_array[1]=t_frame*(113-further_frame)/193;	
	frame_array[2]=t_frame*(114-further_frame)/193;	
	frame_array[3]=t_frame*(125-further_frame)/193;	
	frame_array[4]=t_frame*(125-further_frame)/193;	
	frame_array[5]=t_frame*(172-further_frame)/193;	
	frame_array[6]=t_frame*(172-further_frame)/193;	
	frame_array[7]=t_frame*(193-further_frame)/193;	
	*/
	frame_array[0]=WEATHER_FRAME_ANAMI_TIMES*(93-further_frame);	
	frame_array[1]=WEATHER_FRAME_ANAMI_TIMES*(113-further_frame);	
	frame_array[2]=WEATHER_FRAME_ANAMI_TIMES*(114-further_frame);	
	frame_array[3]=WEATHER_FRAME_ANAMI_TIMES*(125-further_frame);	
	frame_array[4]=WEATHER_FRAME_ANAMI_TIMES*(125-further_frame);	
	frame_array[5]=WEATHER_FRAME_ANAMI_TIMES*(172-further_frame);	
	frame_array[6]=WEATHER_FRAME_ANAMI_TIMES*(172-further_frame);	
	frame_array[7]=WEATHER_FRAME_ANAMI_TIMES*(193-further_frame);	
	loop_total_frame=120;
	//Ì«ÑôÉú³ÉÐý×ª
	if((cur_frame>=frame_array[0])&&(cur_frame<=frame_array[1]))
	{	
		current_t=frame_array[1]-frame_array[0];
		if(0==current_t)
			{
				return;
			}
		current_f=cur_frame-frame_array[0];
		alpha_para=255*s2d_sin_new(900*current_f/current_t)/65536;
		z_para=2048+2048*s2d_sin_new(900*current_f/current_t)/65536;
		
		img_mid.x=sun->image_size.w/2;
		img_mid.y=sun->image_size.h/2;
		window_mid.x=WEATHER_SUNNY_X+img_mid.x;
		window_mid.y=WEATHER_SUNNY_Y+img_mid.y;
		cur_angle=800+(-800)*current_f/current_t;
		S2d_4_in_one_fast_PAF888_Zerosize(bg_size, 
							sun->image_size,
							 window_mid,
							 img_mid,
							 cur_angle, 
							 sun->image_buffer, 
							 out_bg,
							 2,
							  z_para, 
							  alpha_para, 
							   Point_Add_ptr);
		#if 0
		S2d_4_in_one_fast(bg_size, 
							sun->image_size,
							 window_mid,
							 img_mid,
							 cur_angle, 
							 sun->image_buffer, 
							 out_bg, 
							  z_para, 
							  alpha_para, 
							   Point_Add_ptr);
		#endif
	}
	else
	{	
		
		//ÌùÌ«Ñô±³ºóµÄ¹âÊø
		current_t=frame_array[7]-frame_array[2];
		if(0==current_t)
			{
				return;
			}
		current_f=cur_frame-frame_array[2];
		alpha_para=255;
		z_para=4096;
		
		img_mid.x=sun_bg->image_size.w/2;
		img_mid.y=sun_bg->image_size.h/2;
		window_mid.x=WEATHER_SUNNY_X+img_mid.x;
		window_mid.y=WEATHER_SUNNY_Y+img_mid.y;
		cur_angle=(-3600)*loop_frame/loop_total_frame;
		loop_frame++;
		if(loop_frame>loop_total_frame)
			{
				loop_frame=0;
			}
		S2d_4_in_one_fast_PAF888_Zerosize(bg_size, 
							sun_bg->image_size,
							 window_mid,
							 img_mid,
							 cur_angle, 
							 sun_bg->image_buffer, 
							 out_bg, 
							 2,
							  z_para, 
							  alpha_para, 
							   Point_Add_ptr);
		#if 0
		S2d_4_in_one_fast(bg_size, 
							sun_bg->image_size,
							 window_mid,
							 img_mid,
							 cur_angle, 
							 sun_bg->image_buffer, 
							 out_bg, 
							  z_para, 
							  alpha_para, 
							   Point_Add_ptr);
		#endif
		//Ìù¾²Ö¹µÄÌ«Ñô
		img_mid.x=sun->image_size.w/2;
		img_mid.y=sun->image_size.h/2;
		window_mid.x=WEATHER_SUNNY_X+img_mid.x;
		window_mid.y=WEATHER_SUNNY_Y+img_mid.y;
		cur_angle=0;
		alpha_para=255;
		z_para=4096;
		
		S2d_4_in_one_fast_PAF888_Zerosize(bg_size, 
							sun->image_size,
							 window_mid,
							 img_mid,
							 cur_angle, 
							 sun->image_buffer, 
							 out_bg, 
							 1,
							  z_para, 
							  alpha_para, 
							   Point_Add_ptr);
		//
	}
	if((cur_frame>=frame_array[2])&&(cur_frame<=frame_array[7]))
	{	
		current_f=cur_frame-frame_array[2];
		current_t=frame_array[7]-frame_array[2];
		if(0==current_t)
		{
			return;
		}
		rota_angle=-2250+3150*s2d_sin_new(900*current_f/current_t)/65536;
		img_mid.x=Glow->image_size.w/2;
		img_mid.y=Glow->image_size.h/2;
		window_mid.x=WEATHER_SUNNY_X+sun_bg->image_size.w/2+WEATHER_SUNNY_RADIUS*s2d_cos_new(rota_angle)/65536;
		window_mid.y=WEATHER_SUNNY_Y+sun_bg->image_size.h/2+WEATHER_SUNNY_RADIUS*s2d_sin_new(rota_angle)/65536;
		if((cur_frame<=frame_array[3])&&(cur_frame>=frame_array[2]))
		{	
			current_f=cur_frame-frame_array[2];
			current_t=frame_array[3]-frame_array[2];
			if(0==current_t)
			{
				return;
			}
			alpha_para=255*current_f/current_t;
			z_para=4096*8/10*current_f/current_t;
		}
		else if((cur_frame<=frame_array[5])&&(cur_frame>=frame_array[4]))
		{
			current_f=cur_frame-frame_array[4];
			current_t=frame_array[5]-frame_array[4];
			if(0==current_t)
			{
				return;
			}
			alpha_para=255;
			z_para=4096*8/10+4096*2/10*current_f/current_t;
		}
		else if((cur_frame<=frame_array[7])&&(cur_frame>=frame_array[6]))
		{
			current_f=cur_frame-frame_array[6];
			current_t=frame_array[7]-frame_array[6];
			if(0==current_t)
			{
				return;
			}
			alpha_para=255-255*current_f/current_t;
			z_para=4096-4096*7/10*current_f/current_t;
		}
		S2d_4_in_one_fast_PAF888_Zerosize(bg_size, 
							Glow->image_size,
							 window_mid,
							 img_mid,
							 (rota_angle+1350), 
							 Glow->image_buffer, 
							 out_bg, 
							 2,
							  z_para, 
							  alpha_para, 
							   Point_Add_ptr);
	}
	
	
}

void WeatherForcast_FOGGY(int32 cur_frame,
									int32 t_frame,
									BMP_OBJ * fog,
									uint32 * out_bg,
									IMGREF_SIZE_T bg_size,
									FIX16_POINT_T * Point_Add_ptr)
{	
	int32 frame_array[8]={0};
	int32 z_para=0;
	int32 alpha_para=0;
	static int32 loop_frame=0;
	static int32 loop_total_frame=0;
	IMGREF_POINT_T move_points={0};
	
	if(0==t_frame)
	{
		return;
	}
	loop_total_frame=20;
	
	frame_array[0]=t_frame*128/193;	
	frame_array[1]=t_frame*163/193;
	frame_array[2]=t_frame*178/193;	

	
	move_points.x=WEATHER_FOGGY_X;
	move_points.y=WEATHER_FOGGY_Y;

    if(cur_frame>=frame_array[0])
	{	
		if(cur_frame<=(frame_array[1]))
		{
			Weather_ZoomExist_Item( cur_frame, 
							frame_array[0],
							frame_array[1],
							move_points.x, 
							move_points.y,
							0,
							255*9/10,
							4096*3/10,4096,
							fog, 
							out_bg,  bg_size,  Point_Add_ptr);
		}
		else
		{
			if(loop_frame<=(frame_array[2]-frame_array[1]))
				{
					Weather_ZoomExist_Item( loop_frame, 
							0,
							(frame_array[2]-frame_array[1]),
							move_points.x, 
							move_points.y,
							255*9/10,
							255*6/10,
							4096,4096,
							fog, 
							out_bg,  bg_size,  Point_Add_ptr);
				}
			else
				{
					Weather_ZoomExist_Item( loop_frame-((frame_array[2]-frame_array[1])), 
							0,
							(t_frame-frame_array[2]),
							move_points.x, 
							move_points.y,
							255*6/10,
							255*9/10,
							4096,4096,
							fog, 
							out_bg,  bg_size,  Point_Add_ptr);
				}
			
			loop_frame++;
			if(loop_frame>(t_frame-frame_array[1]))
			{
				loop_frame=0;
			}
		}
		
	}
	
	//end loop rain 
 	
}

void WeatherForcast_LIGHTING(int32 cur_frame,
									int32 t_frame,
									BMP_OBJ * light_one,
									BMP_OBJ * light_two,
									uint32 * out_bg,
									IMGREF_SIZE_T bg_size,
									FIX16_POINT_T * Point_Add_ptr)
{	
	uint8 j=0;
	int32 frame_array[9]={0};
	int32 current_f=0;
	int32 current_t=0;
	int32 z_para=0;
	int32 alpha_para=0;
	int32 cur_angle=0;
	BMP_OBJ *  temp_bmp={0};
	int32 rota_angle=0;
	static int32 loop_frame=0;
	static int32 loop_total_frame=0;
	IMGREF_POINT_T offset={0};
	int32 x_distance=0;
	
	IMGREF_POINT_T move_points[2]={0};
	int32 free_frame=30;
	if(0==t_frame)
	{
		return;
	}
	loop_total_frame=40*3/2;
	
	frame_array[0]=t_frame*114/142;	
	frame_array[1]=t_frame*116/142;
	frame_array[2]=t_frame*118/142;	
	
	frame_array[3]=t_frame*118/142;	
	frame_array[4]=t_frame*120/142;	
	frame_array[5]=t_frame*122/142;
	
	frame_array[6]=t_frame*122/142;
	frame_array[7]=t_frame*129/142;
	frame_array[8]=t_frame*136/142;


	
	move_points[0].x=WEATHER_LIGHTING_ONE_X;
	move_points[0].y=WEATHER_LIGHTING_ONE_Y;
	//move_points[1].x=WEATHER_LIGHTING_TWO_X;
	//move_points[1].y=WEATHER_LIGHTING_TWO_X;

	if(cur_frame>=frame_array[0])
	{	
		//DRAW light one 
		if(loop_frame<=(frame_array[1]-frame_array[0]))
		{
			Weather_ZoomExist_Item( loop_frame, 
							0, 
							(frame_array[1]-frame_array[0]), 
							move_points[0].x, 
							move_points[0].y,
							0,
							255,
							4096,4096,
							light_one, 
							out_bg,  bg_size,  Point_Add_ptr);
		}
		else if((loop_frame>(frame_array[1]-frame_array[0]))&&(loop_frame<=(frame_array[2]-frame_array[0])))
		{
			Weather_ZoomExist_Item( loop_frame, 
							(frame_array[1]-frame_array[0]), 
							(frame_array[2]-frame_array[0]), 
							move_points[0].x, 
							move_points[0].y,
							255,
							0,
							4096,4096,
							light_one, 
							out_bg,  bg_size,  Point_Add_ptr);
		}
		//draw light two
		if(loop_frame<=(frame_array[4]-frame_array[0]))
		{
			Weather_ZoomExist_Item( loop_frame, 
							(frame_array[3]-frame_array[0]), 
							(frame_array[4]-frame_array[0]), 
							move_points[0].x, 
							move_points[0].y,
							0,
							255/2,
							4096,4096,
							light_two, 
							out_bg,  bg_size,  Point_Add_ptr);
		}
		else if((loop_frame>(frame_array[4]-frame_array[0]))&&(loop_frame<=(frame_array[5]-frame_array[0])))
		{
			Weather_ZoomExist_Item( loop_frame, 
							(frame_array[4]-frame_array[0]), 
							(frame_array[5]-frame_array[0]), 
							move_points[0].x, 
							move_points[0].y,
							255/2,
							0,
							4096,4096,
							light_two, 
							out_bg,  bg_size,  Point_Add_ptr);
		}
		//slowly draw light one
		if(loop_frame<=(frame_array[7]-frame_array[0]))
		{
			Weather_ZoomExist_Item( loop_frame, 
							(frame_array[6]-frame_array[0]), 
							(frame_array[7]-frame_array[0]), 
							move_points[0].x, 
							move_points[0].y,
							0,
							255,
							4096,4096,
							light_one, 
							out_bg,  bg_size,  Point_Add_ptr);
		}
		else if((loop_frame>(frame_array[7]-frame_array[0]))&&(loop_frame<=(frame_array[8]-frame_array[0])))
		{
			Weather_ZoomExist_Item( loop_frame, 
							(frame_array[7]-frame_array[0]), 
							(frame_array[8]-frame_array[0]), 
							move_points[0].x, 
							move_points[0].y,
							255,
							0,
							4096,4096,
							light_one, 
							out_bg,  bg_size,  Point_Add_ptr);
		}
		
		loop_frame++;
		if(loop_frame>((frame_array[8]-frame_array[0])+free_frame))
			{
				loop_frame=0;
			}
	}
	
	//end loop rain 
 	
}

void WeatherForcast_SNOWY(int32 cur_frame,
									int32 t_frame,
									BMP_OBJ * cloud,
									BMP_OBJ * Snow_big,
									BMP_OBJ * Snow_small,
									BOOLEAN is_cloud_exist,
									uint32 * out_bg,
									IMGREF_SIZE_T bg_size,
									FIX16_POINT_T * Point_Add_ptr)
{	
	uint8 j=0;
	int32 frame_array[10]={0};
	int32 current_f=0;
	int32 current_t=0;
	int32 z_para=0;
	int32 cur_angle=0;
	static int32 loop_frame=0;
	static int32 loop_total_frame=0;
	IMGREF_POINT_T offset={0};
	int32 angle=0;
	#if 0
	int32 alpha_para=0;
	int32 angle=0;
	IMGREF_POINT_T win_mid={0},img_mid={0};
	int32 i=0;
	IMGREF_POINT_T small_snow_points[6]={0};
	int32 small_snow_frame[4]={0};

	#endif 
	IMGREF_POINT_T move_points[9]={0};
	if(0==t_frame)
		{
			return;
		}
	
	frame_array[0]=t_frame*(85)/145;	
	frame_array[1]=t_frame*(104)/145;
	frame_array[2]=t_frame*(124)/145;	
	frame_array[3]=t_frame*(144)/145;	
	frame_array[4]=t_frame*(86)/145;	
	frame_array[5]=t_frame*(105)/145;
	frame_array[6]=t_frame*(125)/145;	
	frame_array[7]=t_frame*(145)/145;
	frame_array[8]=t_frame*(66)/145;	
	frame_array[9]=t_frame*(86)/145; 
	
	move_points[0].x=WEATHER_SNOWY_LEFT_ONE_X;
	move_points[0].y=WEATHER_SNOWY_LEFT_ONE_Y;
	move_points[1].x=WEATHER_SNOWY_LEFT_TWO_X;
	move_points[1].y=WEATHER_SNOWY_LEFT_TWO_Y;
	move_points[2].x= WEATHER_SNOWY_LEFT_THREE_X;
	move_points[2].y= WEATHER_SNOWY_LEFT_THREE_Y;
	move_points[3].x= WEATHER_SNOWY_LEFT_FOUR_X;
	move_points[3].y= WEATHER_SNOWY_LEFT_FOUR_Y;
	move_points[4].x= WEATHER_SNOWY_RIGHT_ONE_X;
	move_points[4].y= WEATHER_SNOWY_RIGHT_ONE_Y;
	move_points[5].x= WEATHER_SNOWY_RIGHT_TWO_X;
	move_points[5].y= WEATHER_SNOWY_RIGHT_TWO_Y;
	move_points[6].x= WEATHER_SNOWY_RIGHT_THREE_X;
	move_points[6].y= WEATHER_SNOWY_RIGHT_THREE_Y;
	move_points[7].x= WEATHER_SNOWY_RIGHT_FOUR_X;
	move_points[7].y= WEATHER_SNOWY_RIGHT_FOUR_Y;
	move_points[8].x= WEATHER_RAIN_CLOUD_X;
	move_points[8].y= WEATHER_RAIN_CLOUD_Y;
#if 0
	small_snow_points[0].x=WEATHER_SNOWY_S_START_ONE_X;
	small_snow_points[0].y=WEATHER_SNOWY_S_START_ONE_Y;
	small_snow_points[1].x=WEATHER_SNOWY_S_END_ONE_X;
	small_snow_points[1].y=WEATHER_SNOWY_S_END_ONE_Y;
	small_snow_points[2].x= WEATHER_SNOWY_S_START_TWO_X;
	small_snow_points[2].y= WEATHER_SNOWY_S_START_TWO_Y;
	small_snow_points[3].x= WEATHER_SNOWY_S_END_TWO_X;
	small_snow_points[3].y= WEATHER_SNOWY_S_END_TWO_Y;
	small_snow_points[4].x= WEATHER_SNOWY_S_START_THREE_X;
	small_snow_points[4].y= WEATHER_SNOWY_S_START_THREE_Y;
	small_snow_points[5].x= WEATHER_SNOWY_S_END_THREE_X;
	small_snow_points[5].y= WEATHER_SNOWY_S_END_THREE_Y;

	small_snow_frame[0]=t_frame*(85)/145;	
	small_snow_frame[1]=t_frame*(95)/145;
	small_snow_frame[2]=t_frame*(135)/145;	
	small_snow_frame[3]=t_frame*(145)/145;	
	//start  falling  snow
 #endif 
	
    if(cur_frame>=frame_array[0])
	{	//draw left big snow 
		if((loop_frame+frame_array[0])<=(frame_array[1]))
		{	
			Weather_MoveExist_Item( (loop_frame+frame_array[0]), 
							frame_array[0],
							frame_array[1],
							move_points[0], 
							move_points[1],
							255*0/10,
							255*6/10,
							4096,4096,
							Snow_big, 
							out_bg,  bg_size,  Point_Add_ptr);
		}
		else if(((loop_frame+frame_array[0])>frame_array[1])&&((loop_frame+frame_array[0])<=frame_array[2]))
		{
			Weather_MoveExist_Item( (loop_frame+frame_array[0]), 
							frame_array[1],
							frame_array[2],
							move_points[1], 
							move_points[2],
							255*6/10,
							255*8/10,
							4096,4096,
							Snow_big, 
							out_bg,  bg_size,  Point_Add_ptr);
		}
		else if(((loop_frame+frame_array[0])>frame_array[2])&&((loop_frame+frame_array[0])<=frame_array[3]))
		{
			Weather_MoveExist_Item( (loop_frame+frame_array[0]), 
							frame_array[2],
							frame_array[3],
							move_points[2], 
							move_points[3],
							255*8/10,
							0,
							4096,4096,
							Snow_big, 
							out_bg,  bg_size,  Point_Add_ptr);
		}
		//draw right big snow 
		if(((loop_frame+frame_array[0])<=frame_array[5])&&((loop_frame+frame_array[0])>=frame_array[4]))
		{	
			Weather_MoveExist_Item( (loop_frame+frame_array[0]), 
							frame_array[4],
							frame_array[5],
							move_points[4], 
							move_points[5],
							0,
							255/2,
							4096,4096,
							Snow_big, 
							out_bg,  bg_size,  Point_Add_ptr);
		}
		else if(((loop_frame+frame_array[0])>frame_array[5])&&((loop_frame+frame_array[0])<=frame_array[6]))
		{
			Weather_MoveExist_Item( (loop_frame+frame_array[0]), 
							frame_array[5],
							frame_array[6],
							move_points[5], 
							move_points[6],
							255/2,
							255/2,
							4096,4096,
							Snow_big, 
							out_bg,  bg_size,  Point_Add_ptr);
		}
		else if(((loop_frame+frame_array[0])>frame_array[6])&&((loop_frame+frame_array[0])<=frame_array[7]))
		{
			Weather_MoveExist_Item( (loop_frame+frame_array[0]), 
							frame_array[6],
							frame_array[7],
							move_points[6], 
							move_points[7],
							255/2,
							0,
							4096,4096,
							Snow_big, 
							out_bg,  bg_size,  Point_Add_ptr);
		}
		
//start rotation small snow 	
#if 0
		for(i=0;i<3;i++)
			{	
				current_t=small_snow_frame[3]-small_snow_frame[0];
				if(0==current_t)
					{
						return;
					}
				angle=3600*loop_frame/current_t;
				img_mid.x=Snow_small->image_size.w/2;
				img_mid.y=Snow_small->image_size.h/2;
				win_mid.x=small_snow_points[2*i].x+(small_snow_points[2*i+1].x-small_snow_points[2*i].x)*loop_frame/current_t+Snow_small->image_size.w/2;
				win_mid.y=small_snow_points[2*i].y+(small_snow_points[2*i+1].y-small_snow_points[2*i].y)*loop_frame/current_t+Snow_small->image_size.h/2;
				if((loop_frame+small_snow_frame[0])<=(small_snow_frame[1]))
							{	
								if(0!=((small_snow_frame[1])-small_snow_frame[0]))
									{
										alpha_para=255*(loop_frame)/((small_snow_frame[1])-small_snow_frame[0]);
									}
							}
						else if(((loop_frame+small_snow_frame[0])>small_snow_frame[1])&&((loop_frame+small_snow_frame[0])<=small_snow_frame[2]))
							{
								
										alpha_para=255;
									
							}
						else if(((loop_frame+small_snow_frame[0])>small_snow_frame[2])&&((loop_frame+small_snow_frame[0])<=small_snow_frame[3]))
							{
								if(0!=((small_snow_frame[3])-small_snow_frame[2]))
									{
										alpha_para=255-255*(loop_frame-((small_snow_frame[2])-small_snow_frame[0]))/((small_snow_frame[3])-small_snow_frame[2]);
									}
							}
				
				 S2d_4_in_one_fast_PAF888_Zerosize(bg_size, 
											Snow_small->image_size,
											win_mid,
											 img_mid, angle, 
											 Snow_small->image_buffer, 
											 out_bg,
											 2,
											  4096, 
											  alpha_para, 
											   Point_Add_ptr);
			}
		#endif
//end rotation small snow 

		loop_frame++;
		if(loop_frame>(frame_array[7]-frame_array[0]))
		{
			loop_frame=0;
		}
	}
	 // falling  snow end

	
	
	//clouds exits start
	if(is_cloud_exist)
	{
		Weather_ZoomExist_Item( cur_frame, 
								frame_array[8], 
								frame_array[9], 
								move_points[8].x, 
								move_points[8].y,
								0,
								255,
								2048,4096,
								cloud, 
								out_bg,  bg_size,  Point_Add_ptr);

	}
	//clouds exits end

 	
}

void WeatherForcast_Schedule(WEATHER_F_PARAM * in_param_data_t)
{
	uint32* bg_buffer=NULL;
	int32 i=0,j=0;
	int32 step_one_frame=0;
	BOOLEAN Is_T_exist=TRUE;
	FIX16_POINT_T bg_offset={0};
    WEATHER_RESOURCE_STRUCT_T  *Weather_R_Instance= NULL;
	if((NULL==in_param_data_t)||(in_param_data_t->total_frame==0))
	{
		return;
	}
    Weather_R_Instance = in_param_data_t->Weather_R_Instance;
	step_one_frame=65;
	bg_buffer=((uint32 *)(in_param_data_t->out_buffer));
	//draw bg
	bg_offset.x=WEATHER_BG_OFFSET_X;
	bg_offset.y=WEATHER_BG_OFFSET_Y;
	for(i=0;i<Weather_R_Instance->bg.image_size.h;i++)
	{
		for(j=0;j<Weather_R_Instance->bg.image_size.w;j++)
		{
			bg_buffer[(i+bg_offset.y)*(in_param_data_t->out_size.w)+j+bg_offset.x]=Weather_R_Instance->bg.image_buffer[i*Weather_R_Instance->bg.image_size.w+j];
		}
	}
	//
	if(in_param_data_t->cur_frame<=WEATHER_FRAME_ANAMI_IN)
	{
		WeatherForcast_Anami_Cloud_IN( in_param_data_t->cur_frame,
									step_one_frame, 
									&Weather_R_Instance->cloud_one,
									&Weather_R_Instance->cloud_two, 
									bg_buffer, 
									in_param_data_t->out_size, 
									in_param_data_t->Point_Add_ptr);
	}
	if(in_param_data_t->cur_frame>=WEATHER_FRAME_ANAMI_IN)
	{
		 WeatherForcast_Dates_Rotation(in_param_data_t->cur_frame,
								in_param_data_t->total_frame, 
								&Weather_R_Instance->weather_change,
								&Weather_R_Instance->city_change,
								&Weather_R_Instance->first_day,
								&Weather_R_Instance->second_day,
								&Weather_R_Instance->third_day,
								&Weather_R_Instance->divide_bar,
								bg_buffer,
								in_param_data_t->out_size, 
								in_param_data_t->Point_Add_ptr);
	}
	switch(in_param_data_t->cur_weather)
	{
	case WEATHER_STATE_SNOWYANDRAINY:
		
		if((in_param_data_t->cur_frame>=WEATHER_FRAME_ANAMI_IN)
				&&(in_param_data_t->cur_frame<=in_param_data_t->total_frame))
		{	
			WeatherForcast_SNOWY(in_param_data_t->cur_frame,
									in_param_data_t->total_frame, 
									&Weather_R_Instance->cloud_left,
									&Weather_R_Instance->snow_big,
									&Weather_R_Instance->snow_small,
									FALSE,
									bg_buffer,
									in_param_data_t->out_size, 
									in_param_data_t->Point_Add_ptr);
			WeatherForcast_RAINNY(in_param_data_t->cur_frame,
									in_param_data_t->total_frame, 
									&Weather_R_Instance->cloud_left,
									&Weather_R_Instance->rain,
									bg_buffer,
									in_param_data_t->out_size, 
									in_param_data_t->Point_Add_ptr);
									
		}
			
			break;
	case WEATHER_STATE_SNOWY:
		
		if((in_param_data_t->cur_frame>=WEATHER_FRAME_ANAMI_IN)
				&&(in_param_data_t->cur_frame<=in_param_data_t->total_frame))
		{	
			WeatherForcast_SNOWY(in_param_data_t->cur_frame,
									in_param_data_t->total_frame, 
									&Weather_R_Instance->cloud_left,
									&Weather_R_Instance->snow_big,
									&Weather_R_Instance->snow_small,
									TRUE,
									bg_buffer,
									in_param_data_t->out_size, 
									in_param_data_t->Point_Add_ptr);
			
									
		}
		break;
        
	case WEATHER_STATE_FOGGY:
		if((in_param_data_t->cur_frame>=73*WEATHER_FRAME_ANAMI_TIMES)
				&&(in_param_data_t->cur_frame<=in_param_data_t->total_frame))
		{
			WeatherForcast_SUNNY(in_param_data_t->cur_frame,
									in_param_data_t->total_frame, 
									&Weather_R_Instance->sun,
									&Weather_R_Instance->glow,
									&Weather_R_Instance->sun_bg,
									bg_buffer,
									in_param_data_t->out_size, 
									in_param_data_t->Point_Add_ptr);
		}
        WeatherForcast_FOGGY(in_param_data_t->cur_frame,
        						in_param_data_t->total_frame, 
        						&Weather_R_Instance->fog,
        						bg_buffer,
        						in_param_data_t->out_size, 
        						in_param_data_t->Point_Add_ptr);
			
		break;
        
	case WEATHER_STATE_LIGHTING:
		 WeatherForcast_LIGHTING(in_param_data_t->cur_frame,
										in_param_data_t->total_frame, 
										&Weather_R_Instance->light_one,
										&Weather_R_Instance->light_two,
										bg_buffer,
										in_param_data_t->out_size, 
										in_param_data_t->Point_Add_ptr);
		if((in_param_data_t->cur_frame>=WEATHER_FRAME_ANAMI_IN)
				&&(in_param_data_t->cur_frame<=in_param_data_t->total_frame))
		{	
			WeatherForcast_RAINNY(in_param_data_t->cur_frame,
									in_param_data_t->total_frame, 
									&Weather_R_Instance->cloud_left,
									&Weather_R_Instance->rain,
									bg_buffer,
									in_param_data_t->out_size, 
									in_param_data_t->Point_Add_ptr);
									
		}
		break;
        
	case WEATHER_STATE_SUNNY:
		if((in_param_data_t->cur_frame>=73*WEATHER_FRAME_ANAMI_TIMES)
				&&(in_param_data_t->cur_frame<=in_param_data_t->total_frame))
		{
			WeatherForcast_SUNNY(in_param_data_t->cur_frame,
									in_param_data_t->total_frame, 
									&Weather_R_Instance->sun,
									&Weather_R_Instance->glow,
									&Weather_R_Instance->sun_bg,
									bg_buffer,
									in_param_data_t->out_size, 
									in_param_data_t->Point_Add_ptr);
		}
		break;
        
	case WEATHER_STATE_CLOUDY:
		if((in_param_data_t->cur_frame>=WEATHER_FRAME_ANAMI_IN)
				&&(in_param_data_t->cur_frame<=in_param_data_t->total_frame))
		{
			WeatherForcast_CLOUDY(in_param_data_t->cur_frame,
									in_param_data_t->total_frame, 
									&Weather_R_Instance->cloud_left,
									&Weather_R_Instance->cloud_right,
									bg_buffer,
									in_param_data_t->out_size, 
									in_param_data_t->Point_Add_ptr);
		}
		break;
        
	case WEATHER_STATE_RAINNY:
		
		if((in_param_data_t->cur_frame>=WEATHER_FRAME_ANAMI_IN)
				&&(in_param_data_t->cur_frame<=in_param_data_t->total_frame))
		{	
			WeatherForcast_RAINNY(in_param_data_t->cur_frame,
									in_param_data_t->total_frame, 
									&Weather_R_Instance->cloud_left,
									&Weather_R_Instance->rain,
									bg_buffer,
									in_param_data_t->out_size, 
									in_param_data_t->Point_Add_ptr);
									
		}
		break;
        
	case WEATHER_STATE_NA:
		if((in_param_data_t->cur_frame>=73*WEATHER_FRAME_ANAMI_TIMES)
				&&(in_param_data_t->cur_frame<=in_param_data_t->total_frame))
		{
			WeatherForcast_SUNNY(in_param_data_t->cur_frame,
									in_param_data_t->total_frame, 
									&Weather_R_Instance->sun,
									&Weather_R_Instance->glow,
									&Weather_R_Instance->sun_bg,
									bg_buffer,
									in_param_data_t->out_size, 
									in_param_data_t->Point_Add_ptr);
             WeatherForcast_NA(in_param_data_t->cur_frame,
							in_param_data_t->total_frame, 
							&Weather_R_Instance->cloud_left,
							&Weather_R_Instance->na,
							bg_buffer,
							in_param_data_t->out_size, 
							in_param_data_t->Point_Add_ptr);
		}
       
        Is_T_exist=FALSE;
		break;
        
	case WEATHER_STATE_SUNCHANGE:
		if((in_param_data_t->cur_frame>=73*WEATHER_FRAME_ANAMI_TIMES)
				&&(in_param_data_t->cur_frame<=in_param_data_t->total_frame))
		{
			WeatherForcast_SUNNY(in_param_data_t->cur_frame,
									in_param_data_t->total_frame, 
									&Weather_R_Instance->sun,
									&Weather_R_Instance->glow,
									&Weather_R_Instance->sun_bg,
									bg_buffer,
									in_param_data_t->out_size, 
									in_param_data_t->Point_Add_ptr);
		}
		WeatherForcast_SUNCHANGE_CLOUD(in_param_data_t->cur_frame,
									in_param_data_t->total_frame, 
									&Weather_R_Instance->cloud_left,
									&Weather_R_Instance->cloud_right,
									bg_buffer,
									in_param_data_t->out_size, 
									in_param_data_t->Point_Add_ptr);
		break;
        
	default:
		break;
	}
	if(Is_T_exist)
	{
		WeatherForcast_T_Exist(in_param_data_t->cur_frame,
								100*WEATHER_FRAME_ANAMI_TIMES, 
								125*WEATHER_FRAME_ANAMI_TIMES, 
								&Weather_R_Instance->big_temperature,
								&Weather_R_Instance->s_up_temperature,
								&Weather_R_Instance->s_down_temperature,
								bg_buffer,
								in_param_data_t->out_size, 
								in_param_data_t->Point_Add_ptr);
	}
	
}
LOCAL void Weather_FocusP_Item(int32 cur_frame,
									  int32 start_frame,
									  int32 end_frame,
									  IMGREF_POINT_T img_mid,
									  IMGREF_POINT_T start_win_mid,
									  IMGREF_POINT_T end_win_mid,
									  uint8 alpha_start,
									  uint8 alpha_end,
									  int32 z_para_start,
									  int32 z_para_end,
									  BMP_OBJ * image_obj,
									  uint32 * out_bg,
									  IMGREF_SIZE_T bg_size,
									 FIX16_POINT_T * Point_Add_ptr)
{	
	int32 current_f=0;
	int32 current_t=0;
	IMGREF_POINT_T win_mid={0};
	uint8 alpha_para=0;
	int32 z_para=0;
	
	current_f=cur_frame-start_frame;
	current_t=end_frame-start_frame;
	if(current_t!=0)
		{
			win_mid.x=start_win_mid.x+(end_win_mid.x-start_win_mid.x)*current_f/current_t;
			win_mid.y=start_win_mid.y+(end_win_mid.y-start_win_mid.y)*current_f/current_t;

		}
	
	if((cur_frame>=start_frame))
		{	
		
			if(0==current_t)
				{
					return;
				}
			//img_mid.x=image_obj->image_size.w/2;
			//img_mid.y=image_obj->image_size.h/2;
			//win_mid.x=offset_x+image_obj->image_size.w/2;
			//win_mid.y=offset_y+image_obj->image_size.h/2;
			if(cur_frame<=end_frame)
				{
					z_para=z_para_start+(z_para_end-z_para_start)*s2d_sin_new(900*current_f/current_t)/65536;
					alpha_para=alpha_start+(alpha_end-alpha_start)*s2d_sin_new(900*current_f/current_t)/65536;
				}	
			else
				{
					z_para=z_para_end;
					alpha_para=alpha_end;
				}
			
			S2d_4_in_one_fast_PAF888_Zerosize(bg_size, 
								image_obj->image_size,
								win_mid,
								 img_mid, 0, 
								 image_obj->image_buffer, 
								 out_bg,
								 2,
								  z_para, 
								  alpha_para, 
								   Point_Add_ptr);
			
		}
}
void Item_Disappear(int cur_frame,
						int total_frame,
						int item_num,
						BMP_OBJ * img_collection[],
						IMGREF_POINT_T * img_location,
						int distance_y,
						uint32 * out_bg, 
						IMGREF_SIZE_T bg_size,
						FIX16_POINT_T * Point_Add_ptr)
{
	int i=0;
	int offset_y=0;
	IMGREF_POINT_T end_location={0},img_mid={0};
	IMGREF_POINT_T Focus_P={0};
	if(0==total_frame)
		{
			return;
		}
	Focus_P.x=img_location[0].x+img_collection[0]->image_size.w/2;
	Focus_P.y=img_location[0].y+img_collection[0]->image_size.h/2;
	end_location.x=Focus_P.x;
	end_location.y=Focus_P.y+distance_y;
	for(i=0;i<item_num;i++)
		{	
			if(NULL==img_collection[i])
				{
					continue;
				}
			
			img_mid.x=Focus_P.x-img_location[i].x;
			img_mid.y=Focus_P.y-img_location[i].y;	
			Weather_FocusP_Item( cur_frame, 
									 0,
									 total_frame,
									 img_mid, 
									 Focus_P,
									 end_location, 
									 255, 
									 0, 
									 4096, 
									4096*130/100, 
									img_collection[i], 
									out_bg, 
								       bg_size,
									Point_Add_ptr);
		}
}

void WeatherForcast_Dates_Disappear(int32 cur_frame,
											int32 start_frame,
											BMP_OBJ * weather_change,
											BMP_OBJ * city_change,
											BMP_OBJ * sunday,
											BMP_OBJ * monday,
											BMP_OBJ * tuesday,
											BMP_OBJ * bar,
											uint32 * out_bg,
											IMGREF_SIZE_T bg_size,
											FIX16_POINT_T * Point_Add_ptr)
{	
	uint8 j=0;
	int32 frame_array[10]={0};
	int32 current_f=0;
	int32 current_t=0;
	int32 z_para=0;
	int32 alpha_para=0;
	BMP_OBJ *  temp_bmp={0};
	
	IMGREF_POINT_T window_mid[3]={0};
	IMGREF_POINT_T img_mid={0};
	
	

	frame_array[0]=0;	
	frame_array[1]=12;	
	frame_array[2]=0;	
	frame_array[3]=6;	
	frame_array[4]=2;	
	frame_array[5]=8;	
	frame_array[6]=4;	
	frame_array[7]=10;	
	frame_array[8]=6;	
	frame_array[9]=12;	

	
	current_f=cur_frame-start_frame;

	if((current_f<0)||(current_f>frame_array[9]))
		{
			return;
		}
	current_f=frame_array[9]-current_f;//µ¹×Å²¥·Å
	img_mid.x=bar->image_size.w/2;
	img_mid.y=bar->image_size.h/2;
	window_mid[0].x=img_mid.x+WEATHER_BAR_ONE_X;
	window_mid[0].y=img_mid.y+WEATHER_BAR_ONE_Y;
	window_mid[1].x=img_mid.x+WEATHER_BAR_TWO_X;
	window_mid[1].y=img_mid.y+WEATHER_BAR_TWO_Y;

	if((current_f<frame_array[1])&&(current_f>=frame_array[0]))
		{	
			current_t=frame_array[1]-frame_array[0];
			if(0==current_t)
				{
					return;
				}
			//current_f=cur_frame-frame_array[1];
			alpha_para=255*s2d_sin_new(900*current_f/current_t)/65536;
			z_para=4096;
			
			while(j<2)
				{	S2d_4_in_one_fast_PAF888_Zerosize(bg_size, 
										bar->image_size,
										 window_mid[j],
										 img_mid, 0, 
										 bar->image_buffer, 
										 out_bg, 
										 1,
										  z_para, 
										  alpha_para, 
										   Point_Add_ptr);
					j++;
				}
		
		}
	else
		{
			alpha_para=255;
			z_para=4096;
			
			while(j<2)
				{	
					S2d_4_in_one_fast_PAF888_Zerosize(bg_size, 
										bar->image_size,
										 window_mid[j],
										 img_mid, 0, 
										 bar->image_buffer, 
										 out_bg, 
										 1,
										  z_para, 
										  alpha_para, 
										   Point_Add_ptr);
					j++;
				}
		}
	//add change weather icon
	Weather_RoExist_Item( current_f, 
						frame_array[2], 
						frame_array[3], 
						WEATHER_DATE_CHANGE_X, 
						WEATHER_DATE_CHANGE_Y,
						weather_change, 
						 out_bg,
						 bg_size);
	Weather_RoExist_Item( current_f, 
						frame_array[2], 
						frame_array[3], 
						WEATHER_CITY_CHANGE_X, 
						WEATHER_CITY_CHANGE_Y,
						city_change, 
						 out_bg,
						 bg_size);
	// add sunday
	Weather_RoExist_Item( current_f, 
						frame_array[4], 
						frame_array[5], 
						WEATHER_SUNDAY_X, 
						WEATHER_SUNDAY_Y,
						sunday, 
						 out_bg,
						 bg_size);
	// add monday
	Weather_RoExist_Item( current_f, 
						frame_array[6], 
						frame_array[7], 
						WEATHER_MONDAY_X, 
						WEATHER_MONDAY_Y,
						monday, 
						 out_bg,
						 bg_size);
	// add tuesday
	Weather_RoExist_Item( current_f, 
						frame_array[8], 
						frame_array[9], 
						WEATHER_TUESDAY_X, 
						WEATHER_TUESDAY_Y,
						tuesday, 
						 out_bg,
						 bg_size);
		
	
	
}

PUBLIC void WeatherForcast_Disappear(WEATHER_F_PARAM * in_param_data_t)
{
	uint32* bg_buffer=NULL;
	int32 i=0,j=0;
	BOOLEAN Is_T_exist=TRUE;
	FIX16_POINT_T bg_offset={0};
	//disapear
	int item_num=0;
	BMP_OBJ * bmp_ptr_array[15]={0};
	IMGREF_POINT_T img_location_array[15]={0};
	//disapear
	WEATHER_RESOURCE_STRUCT_T  *Weather_R_Instance= NULL;
	if((NULL==in_param_data_t)||(in_param_data_t->total_frame==0))
	{
		return;
	}
	Weather_R_Instance = in_param_data_t->Weather_R_Instance;
	bg_buffer=((uint32 *)(in_param_data_t->out_buffer));
	//draw bg
	bg_offset.x=WEATHER_BG_OFFSET_X;
	bg_offset.y=WEATHER_BG_OFFSET_Y;
	for(i=0;i<Weather_R_Instance->bg.image_size.h;i++)
		{
			for(j=0;j<Weather_R_Instance->bg.image_size.w;j++)
				{
					bg_buffer[(i+bg_offset.y)*(in_param_data_t->out_size.w)+j+bg_offset.x]=Weather_R_Instance->bg.image_buffer[i*Weather_R_Instance->bg.image_size.w+j];
				}
		}
	//

	 WeatherForcast_Dates_Disappear(in_param_data_t->cur_frame,
							0, 
							&Weather_R_Instance->weather_change,
							&Weather_R_Instance->city_change,
							&Weather_R_Instance->first_day,
							&Weather_R_Instance->second_day,
							&Weather_R_Instance->third_day,
							&Weather_R_Instance->divide_bar,
							bg_buffer,
							in_param_data_t->out_size, 
							in_param_data_t->Point_Add_ptr);
	switch(in_param_data_t->cur_weather)
						{
						case WEATHER_STATE_SNOWYANDRAINY:
							bmp_ptr_array[0]=&Weather_R_Instance->cloud_left;
							img_location_array[0].x=WEATHER_RAIN_CLOUD_X;
							img_location_array[0].y=WEATHER_RAIN_CLOUD_Y;
							bmp_ptr_array[1]=&Weather_R_Instance->snow_big;
							img_location_array[1].x=WEATHER_SNOWY_LEFT_FOUR_X;
							img_location_array[1].y=WEATHER_SNOWY_LEFT_FOUR_Y;
							bmp_ptr_array[2]=&Weather_R_Instance->rain;
							img_location_array[2].x=WEATHER_RAIN_S_X;
							img_location_array[2].y=WEATHER_RAIN_S_Y;
							item_num=3;
							Item_Disappear(in_param_data_t->cur_frame,
										in_param_data_t->total_frame, 
										 item_num, 
										bmp_ptr_array, 
										img_location_array, 
										WEATHER_DISTANCE_Y, 
										bg_buffer,
										in_param_data_t->out_size, 
										in_param_data_t->Point_Add_ptr);
						
								
								break;
						case WEATHER_STATE_SNOWY:
							
							bmp_ptr_array[0]=&Weather_R_Instance->cloud_left;
							img_location_array[0].x=WEATHER_RAIN_CLOUD_X;
							img_location_array[0].y=WEATHER_RAIN_CLOUD_Y;
							bmp_ptr_array[1]=&Weather_R_Instance->snow_big;
							img_location_array[1].x=WEATHER_SNOWY_LEFT_FOUR_X;
							img_location_array[1].y=WEATHER_SNOWY_LEFT_FOUR_Y;
							
							item_num=2;
							Item_Disappear(in_param_data_t->cur_frame,
										in_param_data_t->total_frame, 
										 item_num, 
										bmp_ptr_array, 
										img_location_array, 
										WEATHER_DISTANCE_Y, 
										bg_buffer,
										in_param_data_t->out_size, 
										in_param_data_t->Point_Add_ptr);
							
								break;
						case WEATHER_STATE_FOGGY:
							bmp_ptr_array[0]=&Weather_R_Instance->sun;
							img_location_array[0].x=WEATHER_SUNNY_X;
							img_location_array[0].y=WEATHER_SUNNY_Y;
							bmp_ptr_array[1]=&Weather_R_Instance->fog;
							img_location_array[1].x=WEATHER_FOGGY_X;
							img_location_array[1].y=WEATHER_FOGGY_Y;
							item_num=2;
							Item_Disappear(in_param_data_t->cur_frame,
										in_param_data_t->total_frame, 
										 item_num, 
										bmp_ptr_array, 
										img_location_array, 
										WEATHER_DISTANCE_Y, 
										bg_buffer,
										in_param_data_t->out_size, 
										in_param_data_t->Point_Add_ptr);
						
								
							break;
						case WEATHER_STATE_LIGHTING:
							bmp_ptr_array[0]=&Weather_R_Instance->cloud_left;
							img_location_array[0].x=WEATHER_RAIN_CLOUD_X;
							img_location_array[0].y=WEATHER_RAIN_CLOUD_Y;
							bmp_ptr_array[1]=&Weather_R_Instance->light_one;
							img_location_array[1].x=WEATHER_SNOWY_LEFT_FOUR_X;
							img_location_array[1].y=WEATHER_SNOWY_LEFT_FOUR_Y;
							bmp_ptr_array[2]=&Weather_R_Instance->rain;
							img_location_array[2].x=WEATHER_LIGHTING_ONE_X;
							img_location_array[2].y=WEATHER_LIGHTING_ONE_Y;
							item_num=3;
							Item_Disappear(in_param_data_t->cur_frame,
										in_param_data_t->total_frame, 
										 item_num, 
										bmp_ptr_array, 
										img_location_array, 
										WEATHER_DISTANCE_Y, 
										bg_buffer,
										in_param_data_t->out_size, 
										in_param_data_t->Point_Add_ptr);
							 
							break;
						case WEATHER_STATE_SUNNY:
							bmp_ptr_array[0]=&Weather_R_Instance->sun;
							img_location_array[0].x=WEATHER_SUNNY_X;
							img_location_array[0].y=WEATHER_SUNNY_Y;
							item_num=1;
							Item_Disappear(in_param_data_t->cur_frame,
										in_param_data_t->total_frame, 
										 item_num, 
										bmp_ptr_array, 
										img_location_array, 
										WEATHER_DISTANCE_Y, 
										bg_buffer,
										in_param_data_t->out_size, 
										in_param_data_t->Point_Add_ptr);
							break;
						case WEATHER_STATE_CLOUDY:
							bmp_ptr_array[0]=&Weather_R_Instance->cloud_left;
							img_location_array[0].x=WEATHER_CLOUD_L_X;
							img_location_array[0].y=WEATHER_CLOUD_L_Y;
							bmp_ptr_array[1]=&Weather_R_Instance->cloud_right;
							img_location_array[1].x=WEATHER_CLOUD_R_X;
							img_location_array[1].y=WEATHER_CLOUD_R_Y;
						
							item_num=2;
							Item_Disappear(in_param_data_t->cur_frame,
										in_param_data_t->total_frame, 
										 item_num, 
										bmp_ptr_array, 
										img_location_array, 
										WEATHER_DISTANCE_Y, 
										bg_buffer,
										in_param_data_t->out_size, 
										in_param_data_t->Point_Add_ptr);
							
							
							
							break;
						case WEATHER_STATE_RAINNY:
							bmp_ptr_array[0]=&Weather_R_Instance->cloud_left;
							img_location_array[0].x=WEATHER_RAIN_CLOUD_X;
							img_location_array[0].y=WEATHER_RAIN_CLOUD_Y;
							bmp_ptr_array[1]=&Weather_R_Instance->rain;
							img_location_array[1].x=WEATHER_RAIN_S_X;
							img_location_array[1].y=WEATHER_RAIN_S_Y;
						
							item_num=2;
							Item_Disappear(in_param_data_t->cur_frame,
										in_param_data_t->total_frame, 
										 item_num, 
										bmp_ptr_array, 
										img_location_array, 
										WEATHER_DISTANCE_Y, 
										bg_buffer,
										in_param_data_t->out_size, 
										in_param_data_t->Point_Add_ptr);
							
							
							
								
							
							break;
						case WEATHER_STATE_NA:
							bmp_ptr_array[0]=&Weather_R_Instance->sun;
							img_location_array[0].x=WEATHER_SUNNY_X;
							img_location_array[0].y=WEATHER_SUNNY_Y;
							bmp_ptr_array[1]=&Weather_R_Instance->cloud_left;
							img_location_array[1].x=WEATHER_NA_CLOUD_X;
							img_location_array[1].y=WEATHER_NA_CLOUD_Y;
							bmp_ptr_array[2]=&Weather_R_Instance->na;
							img_location_array[2].x=WEATHER_NA_TEXT_X;
							img_location_array[2].y=WEATHER_NA_TEXT_Y;
							
							item_num=3;
							Item_Disappear(in_param_data_t->cur_frame,
										in_param_data_t->total_frame, 
										 item_num, 
										bmp_ptr_array, 
										img_location_array, 
										WEATHER_DISTANCE_Y, 
										bg_buffer,
										in_param_data_t->out_size, 
										in_param_data_t->Point_Add_ptr);
							Is_T_exist=FALSE;
							
							break;
						case WEATHER_STATE_SUNCHANGE:
							bmp_ptr_array[0]=&Weather_R_Instance->sun;
							img_location_array[0].x=WEATHER_SUNNY_X;
							img_location_array[0].y=WEATHER_SUNNY_Y;
							bmp_ptr_array[1]=&Weather_R_Instance->cloud_left;
							img_location_array[1].x=WEATHER_SUNC_CLOUD_L_X;
							img_location_array[1].y=WEATHER_SUNC_CLOUD_L_Y;
							bmp_ptr_array[2]=&Weather_R_Instance->cloud_right;
							img_location_array[2].x=WEATHER_SUNC_CLOUD_R_X;
							img_location_array[2].y=WEATHER_SUNC_CLOUD_R_Y;
							
							item_num=3;
							Item_Disappear(in_param_data_t->cur_frame,
										in_param_data_t->total_frame, 
										 item_num, 
										bmp_ptr_array, 
										img_location_array, 
										WEATHER_DISTANCE_Y, 
										bg_buffer,
										in_param_data_t->out_size, 
										in_param_data_t->Point_Add_ptr);
							
								
							break;
						default:
							break;
						}
	//temperature disapper
	if(Is_T_exist)
		{
			bmp_ptr_array[0]=&Weather_R_Instance->big_temperature;
			img_location_array[0].x=WEATHER_TBIG_X;
			img_location_array[0].y=WEATHER_TBIG_Y;
			bmp_ptr_array[1]=&Weather_R_Instance->s_up_temperature;
			img_location_array[1].x=WEATHER_TSMALL_UP_X;
			img_location_array[1].y=WEATHER_TSMALL_UP_Y;
			bmp_ptr_array[2]=&Weather_R_Instance->s_down_temperature;
			img_location_array[2].x=WEATHER_TSMALL_DOWN_X;
			img_location_array[2].y=WEATHER_TSMALL_DOWN_Y;
			
			item_num=3;
			Item_Disappear(in_param_data_t->cur_frame,
						in_param_data_t->total_frame, 
						 item_num, 
						bmp_ptr_array, 
						img_location_array, 
						WEATHER_DISTANCE_Y, 
						bg_buffer,
						in_param_data_t->out_size, 
						in_param_data_t->Point_Add_ptr);
	
}
	//
	
}
PUBLIC int32 MMIWIDGET_WEATHER_GetSEFrame(WEATHER_STATUS_E weather_state)
{
    int32  total_frame = 0;
    
    switch(weather_state)  
    {                                                
    case WEATHER_STATE_SNOWYANDRAINY:
		total_frame=145*WEATHER_FRAME_ANAMI_TIMES;
		break;
	case WEATHER_STATE_SNOWY:
		total_frame=145*WEATHER_FRAME_ANAMI_TIMES;
		break;
	case WEATHER_STATE_LIGHTING:
		total_frame=145*WEATHER_FRAME_ANAMI_TIMES;
		break;
	case WEATHER_STATE_FOGGY:
		total_frame=193*WEATHER_FRAME_ANAMI_TIMES;
		break;
	case WEATHER_STATE_SUNNY:
		total_frame=193*WEATHER_FRAME_ANAMI_TIMES;
		break;
	case WEATHER_STATE_CLOUDY:
		total_frame=142*WEATHER_FRAME_ANAMI_TIMES;
		break;
	case WEATHER_STATE_RAINNY:
		total_frame=145*WEATHER_FRAME_ANAMI_TIMES;
		break;
	case WEATHER_STATE_NA:
		total_frame=213*WEATHER_FRAME_ANAMI_TIMES;
		break;
	case WEATHER_STATE_SUNCHANGE:
		total_frame=210*WEATHER_FRAME_ANAMI_TIMES;
		break;
	default:
		break;                               
    }   
    return total_frame;
}
#endif
