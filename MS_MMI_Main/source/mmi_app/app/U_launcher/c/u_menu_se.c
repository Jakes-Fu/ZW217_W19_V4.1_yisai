/*****************************************************************************
** File Name:      u_menu_se.c                                       *
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

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_app_menu_trc.h"
#include "u_launcher_internal.h"
#include "u_launcher_export.h"
#include "special_effect.h"
#include "mmi_mainmenu_export.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/**--------------------------------------------------------------------------*
**                              STRUCT DEFINITION                        *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         EXTERN VARIABLE DEFINITION                        *
**--------------------------------------------------------------------------*/
extern  int s2d_sin(int angle);
extern  int s2d_cos(int angle);
/**--------------------------------------------------------------------------*
 **                         TYPE DEFINITION                                  *
 **--------------------------------------------------------------------------*/
 
/**--------------------------------------------------------------------------*
**                         LOCAL VARIABLE DEFINITION                        *
**--------------------------------------------------------------------------*/

/**---------------------------------------------------------------------------*
 **                         Functions                                         *
 **---------------------------------------------------------------------------*/
 
#ifdef GO_BINARY_STAR_MAIN_MENU_SUPPORT  
/*****************************************************************************/
//  Description : 双子星计算函数
//  Global resource dependence : 
//  Author: ANSON
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_Go_Effect_Binary_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
												GO_MAINMENU_PAGE_INFO_T *next_page_info,//下一页信息
												GO_MAINMENU_PAGE_INFO_T *third_page_info,//下一页信息
												uint16 cur_frame_num,//当前帧
												uint16 total_frame_num,//总帧数   暂定20
												IMGREF_SIZE_T bg_window_size,//屏幕尺寸
												uint32* bg_window_buf,//背景buffer
												GO_MOVE_DIRECTION_E direction,//移动方向
												FIX16_POINT_T* calc_point_buf,//申请计算所需的BUFFER
												GO_CALC_DATA_INFO_T* calc_data_info 
												);
#endif

#ifdef GO_SHUTTER_MAIN_MENU_SUPPORT  
/*****************************************************************************/
//  Description : 百叶窗计算函数
//  Global resource dependence : 
//  Author: ANSON
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_Go_Effect_Shutters_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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

#endif
#ifdef GO_SLOPE_MAIN_MENU_SUPPORT  
/*****************************************************************************/
//  Description : 斜面计算函数
//  Global resource dependence : 
//  Author: ANSON
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_Go_Effect_Slope_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
                            );

#endif

#ifdef GO_SPHERE_MAIN_MENU_SUPPORT  
/*****************************************************************************/
//  Description : 球形桌面计算函数
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_Go_Effect_Sphere_Type_Calc(
                                                	GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
                                                	GO_MAINMENU_PAGE_INFO_T *next_page_info,//下一页信息
                                                	GO_MAINMENU_PAGE_INFO_T *third_page_info,//下一页信息
                                                	uint16 cur_frame_num,//当前帧
                                                	uint16 total_frame_num,//总帧数   暂定20
                                                	IMGREF_SIZE_T bg_window_size,//屏幕尺寸
                                                	uint32* bg_window_buf,//背景buffer
                                                	GO_MOVE_DIRECTION_E direction,//移动方向
                                                	FIX16_POINT_T* calc_point_buf,
                                                	GO_CALC_DATA_INFO_T* calc_data_info //申请计算所需的BUFFER
                                                );
#endif

#ifdef GO_CYLINDER_MAIN_MENU_SUPPORT
/*****************************************************************************/
//  Description : 圆柱体计算函数
//  Global resource dependence : 
//  Author: Ben
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_Go_Effect_Cylinder_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
#endif
#ifdef GO_WHEEL_MAIN_MENU_SUPPORT 
/*****************************************************************************/
//  Description : 滚轮计算函数
//  Global resource dependence : 
//  Author: Ben
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_Go_Effect_Wheel_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
#endif	
#ifdef GO_OCEANWAVE_MAIN_MENU_SUPPORT  
PUBLIC BOOLEAN MMIMENU_Go_Effect_Oceanwave_Type_Calc(	GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
#endif
#ifdef GO_PEGTOP_MAIN_MENU_SUPPORT  
PUBLIC BOOLEAN MMIMENU_Go_Effect_Pegtop_Type_Calc(	GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
#endif
#ifdef GO_TURNTABLE_MAIN_MENU_SUPPORT  
PUBLIC BOOLEAN MMIMENU_Go_Effect_Turntable_Type_Calc(	GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
#endif
#ifdef GO_BOXROT_MAIN_MENU_SUPPORT 
/*****************************************************************************/
//  Description : 计算函数
//  Global resource dependence : 
//  Author: Ben
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_Go_Effect_Boxrot_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
#endif
#ifdef GO_ICONBOXROT_MAIN_MENU_SUPPORT 
/*****************************************************************************/
//  Description : 计算函数
//  Global resource dependence : 
//  Author: Ben
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_Go_Effect_Iconboxrot_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
#endif
#ifdef GO_BOOKPAGE_MAIN_MENU_SUPPORT 
/*****************************************************************************/
//  Description : 计算函数
//  Global resource dependence : 
//  Author: Ben
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_Go_Effect_Bookpage_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
#endif

#ifdef GO_MIDROTATE_MAIN_MENU_SUPPORT
PUBLIC BOOLEAN MMIMENU_Go_Effect_MidRotate_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
#endif

#ifdef GO_SMALLITEMS_MAIN_MENU_SUPPORT
PUBLIC BOOLEAN MMIMENU_Go_Effect_SmallItems_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
#endif

//#ifdef GO_CLASSIC_MAIN_MENU_SUPPORT
PUBLIC BOOLEAN MMIMENU_Go_Effect_Classic_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
//#endif
#ifdef GO_FLASH_MAIN_MENU_SUPPORT
PUBLIC BOOLEAN MMIMENU_Go_Effect_Flash_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
#endif
#ifdef GO_CONSTICK_MAIN_MENU_SUPPORT
PUBLIC BOOLEAN MMIMENU_Go_Effect_ConStick_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
#endif
#ifdef GO_SYMMETRY_MAIN_MENU_SUPPORT
PUBLIC BOOLEAN MMIMENU_Go_Effect_Symmetry_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
#endif
#ifdef GO_BENZITEM_MAIN_MENU_SUPPORT
PUBLIC BOOLEAN MMIMENU_Go_Effect_BenzItem_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
#endif
/*****************************************************************************/
//  Description :是否执行光照效果
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DoShiningEffect(void);

//每种特效的特殊信息
LOCAL const GO_CALC_STYLE_INFO_T g_umenu_effect_type_calc_table[GO_MENU_STYLE_MAX] = 
{
//#ifdef GO_CLASSIC_MAIN_MENU_SUPPORT  
     {(go_calc_func)MMIMENU_Go_Effect_Classic_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
//#endif 
     {NULL,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},//随机,只支持X，XY
#ifdef GO_FLASH_MAIN_MENU_SUPPORT  
     {(go_calc_func)MMIMENU_Go_Effect_Flash_Type_Calc,GO_SLIDE_SUPPORT_ONLY_Y,GO_DEFAULT_COEFFCIENT},
#endif 
#ifdef GO_BINARY_STAR_MAIN_MENU_SUPPORT  
     {(go_calc_func)MMIMENU_Go_Effect_Binary_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif
#ifdef GO_SHUTTER_MAIN_MENU_SUPPORT  
     {(go_calc_func)MMIMENU_Go_Effect_Shutters_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif
#ifdef GO_SPHERE_MAIN_MENU_SUPPORT  
     {(go_calc_func)MMIMENU_Go_Effect_Sphere_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif
#ifdef GO_CYLINDER_MAIN_MENU_SUPPORT  
     {(go_calc_func)MMIMENU_Go_Effect_Cylinder_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif  
#ifdef GO_SLOPE_MAIN_MENU_SUPPORT  
     {(go_calc_func)MMIMENU_Go_Effect_Slope_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif
#ifdef GO_WHEEL_MAIN_MENU_SUPPORT  
     {(go_calc_func)MMIMENU_Go_Effect_Wheel_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif
    
#ifdef GO_OCEANWAVE_MAIN_MENU_SUPPORT  
     {(go_calc_func)MMIMENU_Go_Effect_Oceanwave_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif    
#ifdef GO_PEGTOP_MAIN_MENU_SUPPORT  
     {(go_calc_func)MMIMENU_Go_Effect_Pegtop_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif    
#ifdef GO_TURNTABLE_MAIN_MENU_SUPPORT  
     {(go_calc_func)MMIMENU_Go_Effect_Turntable_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif    
#ifdef GO_BOXROT_MAIN_MENU_SUPPORT  
     {(go_calc_func)MMIMENU_Go_Effect_Boxrot_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif 
#ifdef GO_ICONBOXROT_MAIN_MENU_SUPPORT  
     {(go_calc_func)MMIMENU_Go_Effect_Iconboxrot_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif 
#ifdef GO_BOOKPAGE_MAIN_MENU_SUPPORT  
     {(go_calc_func)MMIMENU_Go_Effect_Bookpage_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif 
#ifdef GO_MIDROTATE_MAIN_MENU_SUPPORT  
     {(go_calc_func)MMIMENU_Go_Effect_MidRotate_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif 
#ifdef GO_SMALLITEMS_MAIN_MENU_SUPPORT  
     {(go_calc_func)MMIMENU_Go_Effect_SmallItems_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif 
#ifdef GO_CONSTICK_MAIN_MENU_SUPPORT
     {(go_calc_func)MMIMENU_Go_Effect_ConStick_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif
#ifdef GO_SYMMETRY_MAIN_MENU_SUPPORT
     {(go_calc_func)MMIMENU_Go_Effect_Symmetry_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif
#ifdef GO_BENZITEM_MAIN_MENU_SUPPORT
     {(go_calc_func)MMIMENU_Go_Effect_BenzItem_Type_Calc,GO_SLIDE_SUPPORT_ONLY_X,GO_DEFAULT_COEFFCIENT},
#endif  
};

/*****************************************************************************/
//  Description :是否执行光照效果
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN DoShiningEffect(void)
{
#ifdef PDA_UI_SUPPORT_LIGHT_SHADOW_RENDER
    MMISET_EFFECT_RENDER_SETTING* render_setting_ptr = PNULL;
    render_setting_ptr = MMISET_GetLightShadowRenderSetting();
    return render_setting_ptr->render_array[MMISET_EFFECT_RENDER_SETTING_GO_MENU];
#else
    return TRUE;
#endif
}
/*****************************************************************************/
//  Description : Data Type Adapt
//  Global resource dependence : 
//  Author: KYLE
//  Note:上层数据类型与算法独立数据类型的适配
/*****************************************************************************/
PUBLIC S3D_DATA_TYPE_E SEAPI_DataTypeAdapt(DISPLAY_DATA_TYPE_E data_type)
{
    S3D_DATA_TYPE_E s3d_data_type = S3D_DATA_ARGB888;
    
    switch(data_type)
    {
        case DATA_TYPE_ARGB888:
            s3d_data_type = S3D_DATA_ARGB888;
            break;
        case DATA_TYPE_RGB565:
            s3d_data_type = S3D_DATA_RGB565;
            break;
        case DATA_TYPE_PMARGB888:
            s3d_data_type = S3D_DATA_PAF_RGB888;
            break;
        default:
            //SCI_TRACE_LOW:"DataTypeAdapt   data_type = %d"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_454_112_2_18_2_33_16_60,(uint8*)"d",data_type);
            break;
    }

    return s3d_data_type;
}
/*****************************************************************************/
//  Description :
//  Global resource dependence : 
//  Author: KYLE
//  Note:获取算法输出的类型
/*****************************************************************************/
PUBLIC DISPLAY_DATA_TYPE_E SEAPI_GetCalcOutputDataType(void)
{
    DISPLAY_DATA_TYPE_E data_type = DATA_TYPE_ARGB888;

    //根据宏开关设置算法输出数据的类型
    data_type = GO_CALC_OUTPUT_DATA_TYPE;
    
    return data_type;
}
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: KYLE
//  Note:获取算法输入BUF类型
/*****************************************************************************/
PUBLIC DISPLAY_DATA_TYPE_E SEAPI_GetCalcInputDataType(void)
{
    DISPLAY_DATA_TYPE_E data_type = DATA_TYPE_ARGB888;

    //根据宏开关设置算法输入数据的类型
    data_type = GO_CALC_INPUT_DATA_TYPE;

    return data_type;
}

/*****************************************************************************/
//  Description :获取算法callback
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
PUBLIC go_calc_func UAPIMenu_GetCallBackfunc(GO_MENU_STYLE_E style_index)
{
    if(style_index>=GO_MENU_STYLE_MAX)
    {
        return PNULL;
    }
    
    return g_umenu_effect_type_calc_table[style_index].calc_func;
}
/*****************************************************************************/
//  Description :获取算法STYLE slide type
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
PUBLIC GUIMENU_GO_SLIDE_TYPE_E UAPIMenu_GetCalcStyleSlideType(GO_MENU_STYLE_E style_index)
{
    if(style_index>=GO_MENU_STYLE_MAX)
    {
        return GO_SLIDE_SUPPORT_ONLY_X;
    }
    
    return g_umenu_effect_type_calc_table[style_index].slide_type;
}
/*****************************************************************************/
//  Description :获取算法STYLE Total frame
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
PUBLIC uint16 UAPIMenu_GetCalcStyleCalcTime(GO_MENU_STYLE_E style_index)
{
    if(style_index>=GO_MENU_STYLE_MAX)
    {
        return GO_DEFAULT_COEFFCIENT;
    }
    
    return g_umenu_effect_type_calc_table[style_index].calc_time;
}
/*****************************************************************************/
//  Description :获取算法STYLE信息
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
PUBLIC GO_CALC_STYLE_INFO_T* UAPIMenu_GetCalcStyleInfo(GO_MENU_STYLE_E style_index)
{
    if(style_index>=GO_MENU_STYLE_MAX)
    {
        return PNULL;
    }
    
    return &g_umenu_effect_type_calc_table[style_index];
}
/*****************************************************************************/
//  Description :画展开的页面
//  Global resource dependence : 
//  Author:kyle.jin
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_Go_Draw_Unfold_Page(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
	int i = 0;
	FIX16_POINT_T window_offset={0};
    
    if(PNULL == cur_page_info
        ||PNULL == next_page_info
        ||PNULL == bg_window_buf
        ||PNULL == calc_point_buf
        ||(cur_frame_num > total_frame_num)
        )
    {
        return FALSE;
    }
	
	if(0 == cur_frame_num)
	{
		for(i=0;i<cur_page_info->icon_num;i++)
		{
	        if(NULL!=(*(cur_page_info->icon_info+i)).icon_buf)
	        {	
	            //贴图标
	            window_offset.x=(cur_page_info->icon_info+i)->icon_offset.x;
	            window_offset.y=(cur_page_info->icon_info+i)->icon_offset.y;
	            S2D_TypeBlending(bg_window_size, bg_window_buf,calc_data_info->output_type,(*(cur_page_info->icon_info+i)).icon_size,window_offset,(cur_page_info->icon_info +i)->icon_buf,calc_data_info->input_type, bg_window_buf,PNULL);
	        }
	    }
	}
	else if(total_frame_num == cur_frame_num)
	{
		for(i=0;i<next_page_info->icon_num;i++)
		{
	        if(NULL!=(*(next_page_info->icon_info+i)).icon_buf)
	        {	
	            //贴图标
	            window_offset.x=(next_page_info->icon_info+i)->icon_offset.x;
	            window_offset.y=(next_page_info->icon_info+i)->icon_offset.y;
	            S2D_TypeBlending(bg_window_size, bg_window_buf,calc_data_info->output_type,(*(next_page_info->icon_info+i)).icon_size,window_offset,(next_page_info->icon_info +i)->icon_buf,calc_data_info->input_type, bg_window_buf,PNULL);
	        }
	    }
	}
	else
	{
		//SCI_TRACE_LOW:"MMIMENU_Go_Draw_Unfold_Page     ERROR   cur_frame_num=%d"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_596_112_2_18_2_33_16_61,(uint8*)"d",cur_frame_num);
	}
	return TRUE;
}
#ifdef GO_BINARY_STAR_MAIN_MENU_SUPPORT  
/*****************************************************************************/
//  Description : 双子星计算函数
//  Global resource dependence : 
//  Author: ANSON
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_Go_Effect_Binary_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
    int i=0,j=0,wid=0,height=0,row=0,column=0,border_one=0,border_two=0,count_frame=0,n=0,step_frame=0;
    IMGREF_POINT_T pre_position={0};
    IMGREF_POINT_T next_position={0};
    FIX16_POINT_T window_offset={0};
    IMGREF_SIZE_T icon_size={0};
    
    if(PNULL == cur_page_info
        ||PNULL == next_page_info
        ||PNULL == bg_window_buf
        ||PNULL == calc_point_buf
        ||(cur_frame_num > total_frame_num))
    {
        return FALSE;
    }
    
    wid=cur_page_info->icon_info[0].icon_size.w;//jinx_temp
    height=cur_page_info->icon_info[0].icon_size.h;
    row=cur_page_info->total_row_num;
    column=cur_page_info->total_col_num;

    
    //判断移动方向，从而确定两个缩小的位置
    if(GO_MOVE_RIGHT==direction)
    {
        n=1;
        pre_position.x=bg_window_size.w-wid;
        pre_position.y=bg_window_size.h/2-height/2;
        next_position.x=0;
        next_position.y=pre_position.y;
    }
    else if(GO_MOVE_LEFT==direction)
    {	
        n=-1;
        next_position.x=bg_window_size.w-wid;
        next_position.y=bg_window_size.h/2-height/2;
        pre_position.x=0;
        pre_position.y=next_position.y;
        
    }
    //当前菜单完成缩小需要二分之一总真数，缩小后平移出屏幕需要四分之一个总帧数
    border_one=total_frame_num*1/2;
    border_two=total_frame_num*1/4;
    if((0==border_one)||(0==border_two))
    {
        return FALSE;
    }
    //帧数小于二分之一总帧数时当前菜单的图标缩在一起
    if(cur_frame_num<=border_one)
    {    
        for(i=0;i<row;i++)
        {
            for(j=0;j<column;j++)
            {    
                if(PNULL!=(*(cur_page_info->icon_info+i*column+j)).icon_buf)
                {	
                    window_offset.x=(cur_page_info->icon_info+i*column+j)->icon_offset.x+(pre_position.x-(cur_page_info->icon_info+i*column+j)->icon_offset.x)*cur_frame_num/border_one;
                    window_offset.y=(cur_page_info->icon_info +i*column+j)->icon_offset.y+(pre_position.y-(cur_page_info->icon_info +i*column+j)->icon_offset.y)*cur_frame_num/border_one;

                    /*if( == calc_data_info->output_type)
                        S2D_Blending_PAF888(bg_window_size, bg_window_buf,icon_size,window_offset,(cur_page_info->icon_info +i*column+j)->icon_buf, bg_window_buf);
                    #else
                        S2D_Blending(bg_window_size, bg_window_buf,icon_size,window_offset,(cur_page_info->icon_info +i*column+j)->icon_buf, bg_window_buf);
                    #endif*/
                    icon_size.w=(cur_page_info->icon_info+i*column+j)->icon_size.w;
                    icon_size.h=(cur_page_info->icon_info+i*column+j)->icon_size.h;
                    S2D_TypeBlending(bg_window_size, bg_window_buf,calc_data_info->output_type,icon_size,window_offset,(cur_page_info->icon_info +i*column+j)->icon_buf,calc_data_info->input_type, bg_window_buf,calc_data_info->param);
                }
            }
        }
        
        
    }
    //下一页菜单展开，需要二分之一总帧数，进入需要四分之一总帧数。
    //这里判断什么时候开始进入，满足条件的话下一菜单的图标龟缩一起进入
    if((cur_frame_num>=(total_frame_num-border_two-border_one))&&(cur_frame_num<=(total_frame_num-border_one)))
    {	count_frame=border_two;
	 		step_frame=cur_frame_num-(total_frame_num-border_two-border_one);
            for(i=0;i<row;i++)
            {
                for(j=0;j<column;j++)
                {    
                    if(PNULL!=(*(next_page_info->icon_info+i*column+j)).icon_buf)
                    {
                        window_offset.x=next_position.x-n*wid*(count_frame-step_frame)/count_frame;
                        window_offset.y=next_position.y;
                        icon_size.w=(next_page_info->icon_info+i*column+j)->icon_size.w;
                        icon_size.h=(next_page_info->icon_info+i*column+j)->icon_size.h;
                        S2D_TypeBlending(bg_window_size, bg_window_buf,calc_data_info->output_type,icon_size,window_offset,(next_page_info->icon_info +i*column+j)->icon_buf,calc_data_info->input_type, bg_window_buf,calc_data_info->param);
                    }
                }
            }
    }
    //当前菜单的图标龟缩一起，平移出屏幕。
    if((cur_frame_num>border_one)&&(cur_frame_num<=border_one+border_two))
    {
        count_frame=border_two;
        for(i=0;i<row;i++)
        {
            for(j=0;j<column;j++)
            {    
                if(PNULL!=(*(cur_page_info->icon_info+i*column+j)).icon_buf)
                {	
                    window_offset.x=pre_position.x+n*wid*(cur_frame_num-border_one)/count_frame;
                    window_offset.y=pre_position.y;
                    icon_size.w=(cur_page_info->icon_info+i*column+j)->icon_size.w;
                    icon_size.h=(cur_page_info->icon_info+i*column+j)->icon_size.h;
                    S2D_TypeBlending(bg_window_size, bg_window_buf,calc_data_info->output_type,icon_size,window_offset,(cur_page_info->icon_info +i*column+j)->icon_buf,calc_data_info->input_type, bg_window_buf,calc_data_info->param);
                }
            }
        }
        
    }
    //下一菜单的图标展开。
    if((cur_frame_num>border_one)&&(cur_frame_num<=total_frame_num))
    {
        count_frame=border_one;
        step_frame=cur_frame_num-border_one;
        for(i=0;i<row;i++)
        {
            for(j=0;j<column;j++)
            {    
                if(PNULL!=(*(next_page_info->icon_info+i*column+j)).icon_buf)
                {	
                    window_offset.x=next_position.x+((next_page_info->icon_info+i*column+j)->icon_offset.x-next_position.x)*step_frame/count_frame;
                    window_offset.y=next_position.y+((next_page_info->icon_info+i*column+j)->icon_offset.y-next_position.y)*step_frame/count_frame;
                    icon_size.w=(next_page_info->icon_info+i*column+j)->icon_size.w;
                    icon_size.h=(next_page_info->icon_info+i*column+j)->icon_size.h;
                    S2D_TypeBlending(bg_window_size, bg_window_buf,calc_data_info->output_type,icon_size,window_offset,(next_page_info->icon_info +i*column+j)->icon_buf,calc_data_info->input_type, bg_window_buf,calc_data_info->param);
                }
            }
        }
    }
    return TRUE;	
}
#endif

#ifdef GO_SLOPE_MAIN_MENU_SUPPORT  
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
       /* #ifdef GO_CALC_TEST
                        s3d_src_img.src_type=S3D_DATA_PAF_RGB888;
        s3d_img.color_type=S3D_DATA_PAF_RGB888;
                    #else
                        s3d_src_img.src_type=S3D_DATA_ARGB888;
                    s3d_img.color_type=S3D_DATA_RGB565;
                    #endif*/
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
    
    for(i=0;i<page_info->total_row_num;i++)
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
    }
}
/*****************************************************************************/
//  Description : 斜面计算函数
//  Global resource dependence : 
//  Author: ANSON
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIMENU_Go_Effect_Slope_Type_Calc(		GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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

#ifdef GO_SHUTTER_MAIN_MENU_SUPPORT  
/*****************************************************************************/
//  Description : shutter 特效执行函数，包含是否打光特效
//  Global resource dependence : 
//  Author: BENNY
//  Note:
/*****************************************************************************/

LOCAL void Make_Shutter_Shining(uint16 cur_frame,//当前帧
										uint16 total_frame,//总帧数   暂定20
										GO_MOVE_DIRECTION_E direction,
										COLOR32 * tempColor, 
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
    int32 radius = 160;
    int32 delta_xy = 120,x_margin = 10;
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
    
    /*if(frame<=(total_frame+2)/4)
    {    //point1.x = (x_ed-x_st)*frame*2/total_frame+x_st;
    }
    else
    {
    point1.x = (x_ed-x_st)*(frame-(total_frame+2)/4)*4/3/(total_frame+2)+x_st;
    //point1.x = (x_ed - x_st)*(total_frame-frame)*2/total_frame+x_st;
}*/
    if(frame<=(total_frame)/2)
    {    point1.x = (x_ed-x_st)*frame*2/total_frame+x_st;
    }
    else
    {
        //point1.x = (x_ed-x_st)*(frame-(total_frame+2)/4)*4/3/(total_frame+2)+x_st;
        //point1.x = (x_ed - x_st)*(total_frame-frame)*2/total_frame+x_st;
        point1.x = (x_ed - x_st)*(frame-total_frame/2)*2/total_frame+x_st;
    }
    
    point1.y = 0;
    point2.x = point1.x - delta_xy;
    point2.y = win_size.h;
    
    s3d_geometry.img_size.w=icon_size.w;
    s3d_geometry.img_size.h=icon_size.h;
    s3d_geometry.img_size.x =0;
    s3d_geometry.img_size.y = 0;
    
    s3d_geometry.angle=angle;
    s3d_geometry.eye.x=thumb.icon_offset.x+icon_size.w/2;
    s3d_geometry.eye.y=thumb.icon_offset.y+icon_size.h/2;
    s3d_geometry.eye.z=-2*win_size.h;
    
    s3d_geometry.axis.x=thumb.icon_offset.x+icon_size.w/2;
    s3d_geometry.axis.y=thumb.icon_offset.y+icon_size.h/2;
    s3d_geometry.axis.z=icon_size.w/2*abs(s2d_sin(angle))/65536;
    s3d_geometry.axis.axis=1;
    s3d_geometry.is_axis_use_default = SCI_FALSE;
    s3d_geometry.is_eye_use_default = SCI_FALSE;
    
    s3d_src_img.src_ptr=thumb.icon_buf;
        /*#ifdef GO_CALC_TEST
                        s3d_src_img.src_type=S3D_DATA_PAF_RGB888;
        s3d_img.color_type=S3D_DATA_PAF_RGB888;
                    #else
                        s3d_src_img.src_type=S3D_DATA_ARGB888;
                    s3d_img.color_type=S3D_DATA_RGB565;
                    #endif
    */
    s3d_src_img.src_type=calc_data_info->input_type;
    s3d_img.color_type=calc_data_info->output_type;
    
    s3d_img.color_ptr=tempColor;
    s3d_img.size.h=win_size.h;
    s3d_img.size.w=win_size.w;
    
    ls_para.color = 0xdfffffff;
    ls_para.ed_point = &point2;
    ls_para.st_point = &point1;
    ls_para.rds = radius;   
    /*if(DoShiningEffect())
    {
            S3D_AxisMidRotation_Img_shining( &(s3d_geometry),&(s3d_src_img),&(s3d_img),255,&ls_para,calc_data_info->param);
        }
        else
        {
            S3D_AxisMidRotation_Img( &(s3d_geometry),&(s3d_src_img),&(s3d_img),255,calc_data_info->param);
    }*/
    S3D_AxisMidRotation_Img( &(s3d_geometry),&(s3d_src_img),&(s3d_img),255,calc_data_info->param);
    
}

/*****************************************************************************/
//  Description : 百叶窗计算函数
//  Global resource dependence : 
//  Author: ANSON
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_Go_Effect_Shutters_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
	int angle=0,p_row=0,p_column=0,i=0,j=0,temp=0,n_row=0,n_column=0;

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
	//当前，之后页面的图标的行数和列数，
	p_row=cur_page_info->total_row_num;
	p_column=cur_page_info->total_col_num;
	n_row=next_page_info->total_row_num;
	n_column=next_page_info->total_col_num;
	//根据方向不同计算每帧对应的旋转角度。
	if(GO_MOVE_RIGHT==direction)
		{	
			angle=1800*cur_frame_num/total_frame_num+1800;
			if(angle>=2700)
				{
					angle=-1800*(total_frame_num-cur_frame_num)/total_frame_num+1800;
				}
		}
	else if(GO_MOVE_LEFT==direction)
		{
			angle=-1800*cur_frame_num/total_frame_num+1800;
			if(angle<=900)
				{
				angle=1800*(total_frame_num-cur_frame_num)/total_frame_num+1800;
				}
		}
	//当前帧不到总帧数一半时前一页图标旋转
      if(cur_frame_num<total_frame_num/2)
      	{
	      	for(i=0;i<p_row;i++)
					{
						for(j=0;j<p_column;j++)
							{	temp=i*p_column+j;
								if(PNULL!=(cur_page_info->icon_info+temp)->icon_buf)
									{
									    
										Make_Shutter_Shining(cur_frame_num,total_frame_num,direction,bg_window_buf, * (cur_page_info->icon_info+temp),(cur_page_info->icon_info+temp)->icon_size,bg_window_size, angle,calc_data_info);
									}
							}
					}
      	}
	  //超过总帧数一半时 后一页图标旋转
	else if(cur_frame_num>=total_frame_num/2)
		{
	      	for(i=0;i<p_row;i++)
					{
						for(j=0;j<p_column;j++)
							{	temp=i*p_column+j;
								if(PNULL!=(next_page_info->icon_info+temp)->icon_buf)
									{
										Make_Shutter_Shining(cur_frame_num,total_frame_num,direction,bg_window_buf, * (next_page_info->icon_info+temp),(next_page_info->icon_info+temp)->icon_size,bg_window_size, angle,calc_data_info);
									}
							}
					}
      		}


	return TRUE;
}
#endif

#ifdef GO_SPHERE_MAIN_MENU_SUPPORT  
/*****************************************************************************/
//  Description : 球形桌面计算函数
//  Global resource dependence : 
//  Author: ANSON
//  Note:
/*****************************************************************************/

PUBLIC BOOLEAN MMIMENU_Go_Effect_Sphere_Type_Calc
(
	GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
	GO_MAINMENU_PAGE_INFO_T *next_page_info,//下一页信息
	GO_MAINMENU_PAGE_INFO_T *third_page_info,//下一页信息
	uint16 cur_frame_num,//当前帧
	uint16 total_frame_num,//总帧数   暂定20
	IMGREF_SIZE_T bg_window_size,//屏幕尺寸
	uint32* bg_window_buf,//背景buffer
	GO_MOVE_DIRECTION_E direction,//移动方向
	FIX16_POINT_T* calc_point_buf,
	GO_CALC_DATA_INFO_T* calc_data_info //申请计算所需的BUFFER
)
{
    int32                   i = 0;
    SE_SOLID_INIT_PARAM_T   init = {0};
    SE_HANDLE               handle = 0;
    SE_RESULT_E             ret	= SE_RESULT_SUCCESS;
    SE_SOLID_RENDER_IN_PARAM_T  *render_in_ptr = PNULL;
    SE_SOLID_RENDER_OUT_PARAM_T render_out = {0};
    SE_SOLID_ADVANCED_PARAM_T   adv_param = {0};
    BOOLEAN                 ret_result = TRUE;

    //input parameter check;
    if (PNULL == cur_page_info)
    {
        //SCI_TRACE_LOW:"== calcfuncSphere == Input param cur_page_info error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1259_112_2_18_2_33_17_62,(uint8*)"");
        ret_result = FALSE;
        goto _Exit_;
    }
    if (PNULL == next_page_info)
    {
        //SCI_TRACE_LOW:"== calcfuncSphere == Input param next_page_info error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1265_112_2_18_2_33_17_63,(uint8*)"");
        ret_result = FALSE;
        goto _Exit_;
    }
    if (PNULL == bg_window_buf)
    {
        //SCI_TRACE_LOW:"== calcfuncSphere == Input param bg_window_buf error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1271_112_2_18_2_33_17_64,(uint8*)"");
        ret_result = FALSE;
        goto _Exit_;
    }
    if (cur_frame_num > total_frame_num)
    {
        //SCI_TRACE_LOW:"== calcfuncSphere == Input param cur_frame_num=%d & total_frame_num=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1277_112_2_18_2_33_17_65,(uint8*)"dd",cur_frame_num,total_frame_num);
        ret_result = FALSE;
        goto _Exit_;
    }
    if (0 == total_frame_num)
    {
        //SCI_TRACE_LOW:"== calcfuncSphere == Input param total_frame_num=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1283_112_2_18_2_33_17_66,(uint8*)"d",total_frame_num);
        ret_result = FALSE;
        goto _Exit_;
    }
    if (direction >= GO_MOVE_MAX)
    {
        //SCI_TRACE_LOW:"== calcfuncSphere == Input param direction=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1289_112_2_18_2_33_17_67,(uint8*)"d",direction);
        ret_result = FALSE;
        goto _Exit_;
    }
    if (0 == bg_window_size.w)
    {
        //SCI_TRACE_LOW:"== calcfuncSphere == Input param bg_window_size.w=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1295_112_2_18_2_33_17_68,(uint8*)"d",bg_window_size.w);
        ret_result = FALSE;
        goto _Exit_;
    }
    if (0 == bg_window_size.h)
    {
        //SCI_TRACE_LOW:"== calcfuncSphere == Input param bg_window_size.h=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1301_112_2_18_2_33_17_69,(uint8*)"d",bg_window_size.h);
        ret_result = FALSE;
        goto _Exit_;
    }
    if (0 == cur_page_info->total_col_num)
    {
        //SCI_TRACE_LOW:"== calcfuncSphere == Input param cur_page_info->total_col_num=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1307_112_2_18_2_33_17_70,(uint8*)"d",cur_page_info->total_col_num);
        ret_result = FALSE;
        goto _Exit_;
    }
    if (0 == cur_page_info->total_row_num)
    {
        //SCI_TRACE_LOW:"== calcfuncSphere == Input param cur_page_info->total_row_num=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1313_112_2_18_2_33_17_71,(uint8*)"d",cur_page_info->total_row_num);
        ret_result = FALSE;
        goto _Exit_;
    }
    for (i = 0; i < cur_page_info->icon_num; i++)
    {
        if (PNULL == cur_page_info->icon_info[i].icon_buf)
        {
            //SCI_TRACE_LOW:"== calcfuncSphere == Input param cur_page_info->icon_info[%d].icon_buf==PNULL error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1321_112_2_18_2_33_17_72,(uint8*)"d",i);
            ret_result = FALSE;
            goto _Exit_;
        }
    }
    for (i = 0; i < next_page_info->icon_num; i++)
    {
        if (PNULL == next_page_info->icon_info[i].icon_buf)
        {
            //SCI_TRACE_LOW:"== calcfuncSphere == Input param next_page_info->icon_info[%d].icon_buf==PNULL error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1330_112_2_18_2_33_17_73,(uint8*)"d",i);
            ret_result = FALSE;
            goto _Exit_;
        }
    }
	
	init.icon_format = IMGREF_FORMAT_ARGB888;
	init.icon_size.w = cur_page_info->icon_size.w;
	init.icon_size.h = cur_page_info->icon_size.h;

	init.target_format = IMGREF_FORMAT_ARGB888;
	init.target_size.w = bg_window_size.w;
	init.target_size.h = bg_window_size.h;
    init.target_rect.x = 0;
    init.target_rect.y = 0;
    init.target_rect.w = bg_window_size.w;
    init.target_rect.h = bg_window_size.h;
    init.icon_columns = cur_page_info->total_col_num;
	init.icon_rows = cur_page_info->total_row_num;
    init.type = SE_SOLID_SPHERE;

    // 创建对象
   	handle = SE_Create(SE_ID_SOLID, &init);
   	if(0 == handle)                       //  没有创建成功，需要错误处理
   	{
        //SCI_TRACE_LOW:"== calcfuncSphere == SE_Create error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1355_112_2_18_2_33_17_74,(uint8*)"");
        ret_result = FALSE;
        goto _Exit_;
    }

    //由于SE_SOLID_RENDER_IN_PARAM_T结构中有较大的数组，建议用动态分配
    render_in_ptr = SCI_ALLOCA(sizeof(SE_SOLID_RENDER_IN_PARAM_T));
    if (NULL == render_in_ptr)
    {
        //SCI_TRACE_LOW:"== calcfuncSphere == alloca mem error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1364_112_2_18_2_33_17_75,(uint8*)"");
        ret_result = FALSE;
        goto _Exit_;
    }
    SCI_MEMSET(render_in_ptr, 0, sizeof(SE_SOLID_RENDER_IN_PARAM_T));

    //切换到获取render默认参数操作
    render_in_ptr->operation = SE_SOLID_OP_GET_DEFAULT_PARAM;
    render_in_ptr->adv_param_ptr = &adv_param;

    ret = SE_Render(handle, render_in_ptr, NULL);
  	if (SE_RESULT_SUCCESS != ret)
  	{
        //SCI_TRACE_LOW:"== calcfuncSphere == SE_Render to get param error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1377_112_2_18_2_33_17_76,(uint8*)"");
        ret_result = FALSE;
        goto _Exit_;
    }

    //设置初始旋转角度
    render_in_ptr->rot_angle.x = 0;
	if (GO_MOVE_LEFT == direction)
	{
        if (total_frame_num > 0)
		{
		    render_in_ptr->rot_angle.y = cur_frame_num*180/total_frame_num;
		}
		else
        {
            //SCI_TRACE_LOW:"== calcfuncSphere == Rotation count total_frame_num error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1392_112_2_18_2_33_17_77,(uint8*)"");
            ret_result = FALSE;
            goto _Exit_;
        }
	}
	else
	{
        if (total_frame_num > 0)
		{
		    render_in_ptr->rot_angle.y = -cur_frame_num*180/total_frame_num;
		}
		else
        {
            //SCI_TRACE_LOW:"== calcfuncSphere == Rotation count total_frame_num error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1405_112_2_18_2_33_17_78,(uint8*)"");
            ret_result = FALSE;
            goto _Exit_;
        }
	}
    render_in_ptr->rot_angle.z = 0;

    // 设置目标背景层
    render_in_ptr->backside.target_buf.ptr = (void *)bg_window_buf;
    render_in_ptr->backside.target_buf.size = bg_window_size.w*bg_window_size.h*2;

    // 设置目标前景层
    render_in_ptr->frontage.target_buf.ptr = (void *)bg_window_buf;
    render_in_ptr->frontage.target_buf.size = bg_window_size.w*bg_window_size.h*2;

    /*if (PNULL == render_in_ptr->backside.target_buf.ptr
        || PNULL == render_in_ptr->frontage.target_buf.ptr)
    {
        //SCI_TRACE_LOW:"== calcfuncSphere == target layer error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1423_112_2_18_2_33_17_79,(uint8*)"");
        ret_result = FALSE;
        goto _Exit_;
    }*/
    
	// 设置源数据
	for (i = 0; i < cur_page_info->icon_num; i++)
    {
        render_in_ptr->frontage.icon[i] = (uint32*)cur_page_info->icon_info[i].icon_buf;
	}
	for (i = 0; i < next_page_info->icon_num; i++)
    {
        render_in_ptr->backside.icon[i] = (uint32*)next_page_info->icon_info[i].icon_buf;
	}

    // 设置背景的亮度
    render_in_ptr->backside.intensity = cur_frame_num*128/total_frame_num;
    if (render_in_ptr->backside.intensity > 128)
    {
        render_in_ptr->backside.intensity = 128;
    }
    else if (render_in_ptr->backside.intensity < 1)
    {
        render_in_ptr->backside.intensity = 1;
    }

    // 开始算法
    render_in_ptr->operation = SE_SOLID_OP_RENDER;
    ret = SE_Render(handle, render_in_ptr, &render_out);
    if (SE_RESULT_SUCCESS != ret)
    {
        //SCI_TRACE_LOW:"== SlideMenuMoveSphere == SE_Render error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1454_112_2_18_2_33_18_80,(uint8*)"");
    }

_Exit_:
    if (0 != handle)
    {
        SE_Destory(handle);
    }

    if (PNULL != render_in_ptr)
    {
        SCI_FREE(render_in_ptr);
    }
    return ret_result;
}
#endif//sphere

#ifdef GO_OCEANWAVE_MAIN_MENU_SUPPORT  
PUBLIC BOOLEAN MMIMENU_Go_Effect_Oceanwave_Type_Calc(	GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
    int32          i = 0;
    int32          j = 0;
    S3D_ROTATION_GEOMETRY_T  s3d_geometry;
	S3D_ROTATION_IMG_T   s3d_src_img;
	S3D_ROTATION_INOUT_IMG_INFO_T  s3d_img;
    int16 angle = 0;
    int16 r = bg_window_size.w/3;
    int16 x0 = 0;
    int16 x_bound = 0;
    int16 icon_x;
	FIX16_POINT_T iron_offset = {0};
	IMGREF_POINT_T iron_rotation_mid = {0};
    uint8  alpha_para = 0;
	BOOLEAN roll_flag = FALSE;
    uint16 cur_total_row_num = 0;
    uint16 cur_total_col_num = 0;
	uint16 cur_iron_num = 0;
	uint8  actual_row = 0;

    //input parameter check;
    if (PNULL == cur_page_info)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param cur_page_info error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1507_112_2_18_2_33_18_81,(uint8*)"");
	    return FALSE;
    }
    if (PNULL == next_page_info)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param next_page_info error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1512_112_2_18_2_33_18_82,(uint8*)"");
	    return FALSE;
    }
    if (PNULL == bg_window_buf)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param bg_window_buf error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1517_112_2_18_2_33_18_83,(uint8*)"");
	    return FALSE;
    }
    if (cur_frame_num > total_frame_num)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param cur_frame_num=%d & total_frame_num=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1522_112_2_18_2_33_18_84,(uint8*)"dd",cur_frame_num,total_frame_num);
	    return FALSE;
    }
    if (2 >= total_frame_num)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param total_frame_num=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1527_112_2_18_2_33_18_85,(uint8*)"d",total_frame_num);
	    return FALSE;
    }
    if (direction >= GO_MOVE_MAX)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param direction=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1532_112_2_18_2_33_18_86,(uint8*)"d",direction);
	    return FALSE;
    }
    if (0 == bg_window_size.w)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param bg_window_size.w=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1537_112_2_18_2_33_18_87,(uint8*)"d",bg_window_size.w);
	    return FALSE;
    }
    if (0 == bg_window_size.h)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param bg_window_size.h=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1542_112_2_18_2_33_18_88,(uint8*)"d",bg_window_size.h);
	    return FALSE;
    }
    if (0 == cur_page_info->total_col_num)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param cur_page_info->total_col_num=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1547_112_2_18_2_33_18_89,(uint8*)"d",cur_page_info->total_col_num);
	    return FALSE;
    }
    if (0 == cur_page_info->total_row_num)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param cur_page_info->total_row_num=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1552_112_2_18_2_33_18_90,(uint8*)"d",cur_page_info->total_row_num);
	    return FALSE;
    }
    /*for (i = 0; i < cur_page_info->icon_num; i++)
    {
        if (PNULL == cur_page_info->icon_info[i].icon_buf)
        {
            //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param cur_page_info->icon_info[%d].icon_buf==PNULL error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1559_112_2_18_2_33_18_91,(uint8*)"d",i);
	        return FALSE;
        }
    }
    for (i = 0; i < next_page_info->icon_num; i++)
    {
        if (PNULL == next_page_info->icon_info[i].icon_buf)
        {
            //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Oceanwave_Type_Calc Input param next_page_info->icon_info[%d].icon_buf==PNULL error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1567_112_2_18_2_33_18_92,(uint8*)"d",i);
	        return FALSE;
        }
    }*/
    
    cur_total_row_num = cur_page_info->total_row_num;
    cur_total_col_num = cur_page_info->total_col_num;
    cur_iron_num = cur_page_info->icon_num;

	s3d_geometry.eye.x = bg_window_size.w/2;
    s3d_geometry.eye.y = bg_window_size.h/2;
    s3d_geometry.eye.z = -2*bg_window_size.h;
    s3d_geometry.img_size.x = 0;
    s3d_geometry.img_size.y = 0;

    s3d_geometry.axis.axis = 1;
	s3d_geometry.is_axis_use_default = SCI_FALSE;
	s3d_geometry.is_eye_use_default = SCI_FALSE;

    /*#ifdef GO_CALC_TEST
                        s3d_src_img.src_type=S3D_DATA_PAF_RGB888;
    s3d_img.color_type=S3D_DATA_PAF_RGB888;
                    #else
                        s3d_src_img.src_type=S3D_DATA_ARGB888;
                    s3d_img.color_type=S3D_DATA_RGB565;
                    #endif*/
    s3d_src_img.src_type=calc_data_info->input_type;
    s3d_img.color_type=calc_data_info->output_type;
    
	s3d_img.color_ptr=bg_window_buf;
	s3d_img.size.h=bg_window_size.h;
	s3d_img.size.w=bg_window_size.w;


    x0 = bg_window_size.w * cur_frame_num/(total_frame_num>>1);
    if (GO_MOVE_LEFT == direction)
    {
        x0 = bg_window_size.w-x0;
    }

	if (GO_MOVE_RIGHT == direction)
	{
		x_bound = x0 - r;
	}
	else
	{
		x_bound = x0 + r - next_page_info->icon_info[0].icon_size.w;//jinx_temp
	}
	
	for (i = 0; i < next_page_info->icon_num; i++)
    {
        if (PNULL == next_page_info->icon_info[i].icon_buf)
        {
            break;
        }

		icon_x = next_page_info->icon_info[i].icon_offset.x;
			
		if ((GO_MOVE_RIGHT == direction && x_bound > icon_x)|| \
	       (GO_MOVE_LEFT == direction && x_bound < icon_x))
		{
            if (abs(x_bound - icon_x) > 4*next_page_info->icon_info[i].icon_size.w)
            {
				alpha_para = 255;
            } 
            else
            {
                if (next_page_info->icon_info[i].icon_size.w > 0)
                {
				    alpha_para = (abs(x_bound - icon_x)*255/next_page_info->icon_info[i].icon_size.w)>>2;
                }
                else
                {
                    alpha_para = 0;
                }
            }
        }
        else
        {
            alpha_para = 0;
        }
        S2d_Type_4_in_one_fast(
            bg_window_size,			
			next_page_info->icon_info[i].icon_size,					
			next_page_info->icon_info[i].icon_offset,	
			iron_rotation_mid,						    
			0,								            
			next_page_info->icon_info[i].icon_buf,	
			calc_data_info->input_type,
			bg_window_buf,
			calc_data_info->output_type,						       
			4096,								        
			alpha_para,					
			calc_point_buf,
			calc_data_info->param);
	}


	actual_row = cur_iron_num/cur_total_col_num;
	if (GO_MOVE_LEFT == direction)
	{
		for (i = 0; i < cur_iron_num; i++)
        {
            if (PNULL == cur_page_info->icon_info[i].icon_buf)
            {
                break;
            }
			icon_x = cur_page_info->icon_info[i].icon_offset.x + cur_page_info->icon_info[i].icon_size.w/2;
			roll_flag = x0 < icon_x;
			
			if (roll_flag)
			{      
				angle = (x0 - icon_x)*1800/(bg_window_size.w);
                s3d_geometry.img_size.w =cur_page_info->icon_info[i].icon_size.w;
                s3d_geometry.img_size.h =cur_page_info->icon_info[i].icon_size.h;
				s3d_geometry.axis.x = x0 - ((r*s2d_sin_new(angle))>>16);
				s3d_geometry.axis.y = cur_page_info->icon_info[i].icon_offset.y+cur_page_info->icon_info[i].icon_size.h/2;
				s3d_geometry.axis.z = -r + ((r*s2d_cos_new(angle))>>16);
				s3d_geometry.angle = angle+1850;
				if (abs(angle) >= 900)
				{
					s3d_src_img.src_ptr = cur_page_info->icon_info[i].icon_buf;
					S3D_AxisMidRotation_Img(&(s3d_geometry),&(s3d_src_img),&(s3d_img),128,calc_data_info->param);
				}
				else
				{
					s3d_src_img.src_ptr = cur_page_info->icon_info[i].icon_buf;
					S3D_AxisMidRotation_Img(&(s3d_geometry),&(s3d_src_img),&(s3d_img),255,calc_data_info->param);
				}
				
			}
			else
			{
				iron_offset.x = cur_page_info->icon_info[i].icon_offset.x;
				iron_offset.y = cur_page_info->icon_info[i].icon_offset.y;
                S2D_TypeBlending(bg_window_size, bg_window_buf,calc_data_info->output_type, cur_page_info->icon_info[i].icon_size, iron_offset, cur_page_info->icon_info[i].icon_buf,calc_data_info->input_type,bg_window_buf,calc_data_info->param);
			}
		}

	}
	else
	{
		for (i = 0; i < actual_row; i++)
		{
			for (j = cur_total_col_num-1; j >= 0; j--)
			{
                if (PNULL == cur_page_info->icon_info[i*cur_total_col_num+j].icon_buf)
                {
                    break;
                }
				icon_x = cur_page_info->icon_info[i*cur_total_col_num+j].icon_offset.x + cur_page_info->icon_info[i*cur_total_col_num+j].icon_size.w/2;
				roll_flag = x0 > icon_x;
				
				if (roll_flag)
				{      
					angle = (x0 - icon_x)*1800/(bg_window_size.w);
                    s3d_geometry.img_size.w =cur_page_info->icon_info[i*cur_total_col_num+j].icon_size.w;
                    s3d_geometry.img_size.h =cur_page_info->icon_info[i*cur_total_col_num+j].icon_size.h;
					s3d_geometry.axis.x = x0 - ((r*s2d_sin_new(angle))>>16);
					s3d_geometry.axis.y = cur_page_info->icon_info[i*cur_total_col_num+j].icon_offset.y+cur_page_info->icon_info[i*cur_total_col_num+j].icon_size.h/2;
					s3d_geometry.axis.z = -r + ((r*s2d_cos_new(angle))>>16);
					s3d_geometry.angle = angle+1850;
					if (abs(angle) >= 900)
					{
						s3d_src_img.src_ptr = cur_page_info->icon_info[i*cur_total_col_num+j].icon_buf;
						S3D_AxisMidRotation_Img(&(s3d_geometry),&(s3d_src_img),&(s3d_img),128,calc_data_info->param);
					}
					else
					{
						s3d_src_img.src_ptr = cur_page_info->icon_info[i*cur_total_col_num+j].icon_buf;
						S3D_AxisMidRotation_Img (&(s3d_geometry),&(s3d_src_img),&(s3d_img),255,calc_data_info->param);
					}
					
				}
				else
				{
					iron_offset.x = cur_page_info->icon_info[i*cur_total_col_num+j].icon_offset.x;
					iron_offset.y = cur_page_info->icon_info[i*cur_total_col_num+j].icon_offset.y;
                     S2D_TypeBlending(bg_window_size, bg_window_buf,calc_data_info->output_type, cur_page_info->icon_info[i*cur_total_col_num+j].icon_size, iron_offset, cur_page_info->icon_info[i*cur_total_col_num+j].icon_buf,calc_data_info->input_type,bg_window_buf,calc_data_info->param);   
				}
			}
		}

		for (i = cur_iron_num-1; i >= actual_row*cur_total_col_num; i--)
		{
            if (PNULL == cur_page_info->icon_info[i].icon_buf)
            {
                break;
            }

			icon_x = cur_page_info->icon_info[i].icon_offset.x + cur_page_info->icon_info[i].icon_size.w/2;
			roll_flag = x0 > icon_x;
			
			if (roll_flag)
			{      
				angle = (x0 - icon_x)*1800/(bg_window_size.w);
                s3d_geometry.img_size.w =cur_page_info->icon_info[i].icon_size.w;
                s3d_geometry.img_size.h =cur_page_info->icon_info[i].icon_size.h;

				s3d_geometry.axis.x = x0 - ((r*s2d_sin_new(angle))>>16);
				s3d_geometry.axis.y = cur_page_info->icon_info[i].icon_offset.y+cur_page_info->icon_info[i].icon_size.h/2;
				s3d_geometry.axis.z = -r + ((r*s2d_cos_new(angle))>>16);
				s3d_geometry.angle = angle+1850;
				if (abs(angle) >= 900)
				{
					s3d_src_img.src_ptr = cur_page_info->icon_info[i].icon_buf;
					S3D_AxisMidRotation_Img(&(s3d_geometry),&(s3d_src_img),&(s3d_img),128,calc_data_info->param);
				}
				else
				{
					s3d_src_img.src_ptr = cur_page_info->icon_info[i].icon_buf;
					S3D_AxisMidRotation_Img(&(s3d_geometry),&(s3d_src_img),&(s3d_img),255,calc_data_info->param);
				}
				
			}
			else
			{
				iron_offset.x = cur_page_info->icon_info[i].icon_offset.x;
				iron_offset.y = cur_page_info->icon_info[i].icon_offset.y;
                S2D_TypeBlending(bg_window_size, bg_window_buf,calc_data_info->output_type, cur_page_info->icon_info[i].icon_size, iron_offset, cur_page_info->icon_info[i].icon_buf,calc_data_info->input_type, bg_window_buf,calc_data_info->param);              
			}
		}

	}

	return TRUE;
}
#endif//oceanwave

#ifdef GO_PEGTOP_MAIN_MENU_SUPPORT  
PUBLIC BOOLEAN MMIMENU_Go_Effect_Pegtop_Type_Calc(	GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
    int32          i = 0;
    IMGREF_POINT_T bg_window_mid = {0};
    IMGREF_POINT_T img_mid = {0};
    int32 angle = 0;
    int32 z_para = 0;
    uint8 alpha_para = 0;

    //input parameter check;
    if (PNULL == cur_page_info)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param cur_page_info error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1798_112_2_18_2_33_18_93,(uint8*)"");
	    return FALSE;
    }
    if (PNULL == next_page_info)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param next_page_info error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1803_112_2_18_2_33_18_94,(uint8*)"");
	    return FALSE;
    }
    if (PNULL == bg_window_buf)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param bg_window_buf error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1808_112_2_18_2_33_18_95,(uint8*)"");
	    return FALSE;
    }
    if (cur_frame_num > total_frame_num)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param cur_frame_num=%d & total_frame_num=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1813_112_2_18_2_33_18_96,(uint8*)"dd",cur_frame_num,total_frame_num);
	    return FALSE;
    }
    if (0 == total_frame_num)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param total_frame_num=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1818_112_2_18_2_33_18_97,(uint8*)"d",total_frame_num);
	    return FALSE;
    }
    if (direction >= GO_MOVE_MAX)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param direction=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1823_112_2_18_2_33_18_98,(uint8*)"d",direction);
	    return FALSE;
    }
    if (0 == bg_window_size.w)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param bg_window_size.w=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1828_112_2_18_2_33_18_99,(uint8*)"d",bg_window_size.w);
	    return FALSE;
    }
    if (0 == bg_window_size.h)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param bg_window_size.h=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1833_112_2_18_2_33_18_100,(uint8*)"d",bg_window_size.h);
	    return FALSE;
    }
    if (0 == cur_page_info->total_col_num)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param cur_page_info->total_col_num=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1838_112_2_18_2_33_18_101,(uint8*)"d",cur_page_info->total_col_num);
	    return FALSE;
    }
    if (0 == cur_page_info->total_row_num)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param cur_page_info->total_row_num=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1843_112_2_18_2_33_18_102,(uint8*)"d",cur_page_info->total_row_num);
	    return FALSE;
    }
    /*for (i = 0; i < cur_page_info->icon_num; i++)
    {
        if (PNULL == cur_page_info->icon_info[i].icon_buf)
        {
            //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param cur_page_info->icon_info[%d].icon_buf==PNULL error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1850_112_2_18_2_33_18_103,(uint8*)"d",i);
            return FALSE;
        }
    }*/
    /*for (i = 0; i < next_page_info->icon_num; i++)
    {
        if (PNULL == next_page_info->icon_info[i].icon_buf)
        {
            //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Pegtop_Type_Calc Input param next_page_info->icon_info[%d].icon_buf==PNULL error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_1858_112_2_18_2_33_18_104,(uint8*)"d",i);
            return FALSE;
        }
    }*/
	if(0 == cur_frame_num)
	{
		return MMIMENU_Go_Draw_Unfold_Page(cur_page_info,//当前页信息
									next_page_info,//下一页信息
									third_page_info,
									cur_frame_num,//当前帧
									total_frame_num,//总帧数   暂定20
									bg_window_size,//屏幕尺寸
									bg_window_buf,//背景buffer
									direction,//移动方向
									calc_point_buf,
									calc_data_info //申请计算所需的BUFFER
									);
	}
    z_para = cur_frame_num*4096/total_frame_num;
    alpha_para = cur_frame_num*256/total_frame_num;
    
    if (GO_MOVE_LEFT == direction)
    {
        angle = cur_frame_num*3600/total_frame_num;
    }
    else
    {
        angle = -cur_frame_num*3600/total_frame_num;
    }
    
    bg_window_mid.x = bg_window_size.w>>1;
    bg_window_mid.y = bg_window_size.h>>1;

    //if (cur_frame_num < 12)
    {
        for (i = 0; i < cur_page_info->icon_num; i++)
        {
            if (PNULL == cur_page_info->icon_info[i].icon_buf)
            {
                continue;
            }
            img_mid.x = bg_window_mid.x - cur_page_info->icon_info[i].icon_offset.x;
            img_mid.y = bg_window_mid.y - cur_page_info->icon_info[i].icon_offset.y;
            S2d_Type_4_in_one_fast(
                        bg_window_size,			
						cur_page_info->icon_info[i].icon_size,					
						bg_window_mid,	
						img_mid,						    
						angle,								            
						cur_page_info->icon_info[i].icon_buf,	
						calc_data_info->input_type,
						bg_window_buf,
						calc_data_info->output_type,						       
						4096 - z_para,								        
						256 - alpha_para,					
						calc_point_buf,
						calc_data_info->param);
        }
    }

    //if (cur_frame_num > 8)
    {
    	for (i = 0; i < next_page_info->icon_num; i++)
        {
            if (PNULL == next_page_info->icon_info[i].icon_buf)
            {
                continue;
            }
            img_mid.x = bg_window_mid.x - next_page_info->icon_info[i].icon_offset.x;
            img_mid.y = bg_window_mid.y - next_page_info->icon_info[i].icon_offset.y;
            S2d_Type_4_in_one_fast(
                        bg_window_size,			
						cur_page_info->icon_info[i].icon_size,					
						bg_window_mid,	
						img_mid,						    
						3600 - angle,								            
						next_page_info->icon_info[i].icon_buf,	
						calc_data_info->input_type,
						bg_window_buf,
						calc_data_info->output_type,						       
						z_para,								        
						alpha_para,					
						calc_point_buf,
						calc_data_info->param);
        }
    }
	return TRUE;
}
#endif//pegtop

#ifdef GO_TURNTABLE_MAIN_MENU_SUPPORT  
/*****************************************************************************/
//  Description : Turntable 特效执行函数，包含是否打光特效
//  Global resource dependence : 
//  Author: BENNY
//  Note:
/*****************************************************************************/

LOCAL void Make_Turntable_Shining(uint16 cur_frame,//当前帧
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
            						GO_CALC_DATA_INFO_T* calc_data_info)
							
{  
        LIGHT_SHINING_T ls_para = {0};
        FIX16_POINT_T point1 = {0};//st
        FIX16_POINT_T point2 = {0};//ed
        int32 radius = 150;
        int32 delta_xy = -160,x_margin = 10;
        int32 x_st = -80, x_ed = window.w+80;
        int32 frame = 0 ;

        frame = cur_frame;
        delta_xy = -window.h;

        if(0==total_frame)
            return;
        
        if(GO_MOVE_LEFT== direction)
        {
            x_st = (-1)*radius/2-x_margin+delta_xy;
            x_ed = window.w+radius/2;           
        }
        else if(GO_MOVE_RIGHT== direction)
        {
            x_st = window.w+radius/2+x_margin;
            x_ed = (-1)*radius/2+delta_xy;    
        }
        else
        {
            return;
        }

        point1.x = (x_ed-x_st)*frame/total_frame+x_st;
        point1.y = 0;
        point2.x = point1.x - delta_xy;
        point2.y = window.h;
        
        ls_para.color = 0xffffffff;
        ls_para.ed_point = &point2;
        ls_para.st_point = &point1;
        ls_para.rds = radius;   
   
        //if(0)
        //{
            /*#ifdef GO_CALC_TEST
                    S2d_4_in_one_fast_PAF888_shining(window,img,window_mid,img_mid,angle,in_src_ptr,out_src_ptr,z_para,alpha_para,&ls_para,Point_Add_ptr);
            #else
                    S2d_4_in_one_fast_shining(window,img,window_mid,img_mid,angle,in_src_ptr,out_src_ptr,z_para,alpha_para,&ls_para,Point_Add_ptr);
            #endif
            */
            
            //S2d_Type_4_in_one_fast_shining(window,img,window_mid,img_mid,angle,in_src_ptr,calc_data_info->input_type,out_src_ptr,calc_data_info->output_type,z_para,alpha_para,&ls_para,Point_Add_ptr,calc_data_info->param);
            
        //}
        //else
        {
            /*#ifdef GO_CALC_TEST
                    S2d_4_in_one_fast_PAF888(window,img,window_mid,img_mid,angle,in_src_ptr,out_src_ptr,z_para,alpha_para,Point_Add_ptr);
            #else
                    S2d_4_in_one_fast(window,img,window_mid,img_mid,angle,in_src_ptr,out_src_ptr,z_para,alpha_para,Point_Add_ptr);
            #endif*/
            S2d_Type_4_in_one_fast(window,img,window_mid,img_mid,angle,in_src_ptr,calc_data_info->input_type,out_src_ptr,calc_data_info->output_type,z_para,alpha_para,Point_Add_ptr,calc_data_info->param);
        }
}


PUBLIC BOOLEAN MMIMENU_Go_Effect_Turntable_Type_Calc(	GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
    int32          i = 0;
    IMGREF_POINT_T bg_window_mid = {0};
    IMGREF_POINT_T img_mid = {0};
    int32 cur_angle = 0;
    int32 next_angle = 0;
    int32 z_para = 4096; //cur_frame_num*cur_frame_num*4096/(total_frame_num*total_frame_num);

    //input parameter check;
    if (PNULL == cur_page_info)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Turntable_Type_Calc Input param cur_page_info error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_2048_112_2_18_2_33_19_105,(uint8*)"");
	    return FALSE;
    }
    if (PNULL == next_page_info)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Turntable_Type_Calc Input param next_page_info error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_2053_112_2_18_2_33_19_106,(uint8*)"");
	    return FALSE;
    }
    if (PNULL == bg_window_buf)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Turntable_Type_Calc Input param bg_window_buf error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_2058_112_2_18_2_33_19_107,(uint8*)"");
	    return FALSE;
    }
    if (cur_frame_num > total_frame_num)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Turntable_Type_Calc Input param cur_frame_num=%d & total_frame_num=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_2063_112_2_18_2_33_19_108,(uint8*)"dd",cur_frame_num,total_frame_num);
	    return FALSE;
    }
    if (0 == total_frame_num)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Turntable_Type_Calc Input param total_frame_num=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_2068_112_2_18_2_33_19_109,(uint8*)"d",total_frame_num);
	    return FALSE;
    }
    if (direction >= GO_MOVE_MAX)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Turntable_Type_Calc Input param direction=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_2073_112_2_18_2_33_19_110,(uint8*)"d",direction);
	    return FALSE;
    }
    if (0 == bg_window_size.w)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Turntable_Type_Calc Input param bg_window_size.w=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_2078_112_2_18_2_33_19_111,(uint8*)"d",bg_window_size.w);
	    return FALSE;
    }
    if (0 == bg_window_size.h)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Turntable_Type_Calc Input param bg_window_size.h=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_2083_112_2_18_2_33_19_112,(uint8*)"d",bg_window_size.h);
	    return FALSE;
    }
    if (0 == cur_page_info->total_col_num)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Turntable_Type_Calc Input param cur_page_info->total_col_num=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_2088_112_2_18_2_33_19_113,(uint8*)"d",cur_page_info->total_col_num);
	    return FALSE;
    }
    if (0 == cur_page_info->total_row_num)
    {
        //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Turntable_Type_Calc Input param cur_page_info->total_row_num=%d error"
        SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_2093_112_2_18_2_33_19_114,(uint8*)"d",cur_page_info->total_row_num);
	    return FALSE;
    }
    /*for (i = 0; i < cur_page_info->icon_num; i++)
    {
        if (PNULL == cur_page_info->icon_info[i].icon_buf)
        {
            //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Turntable_Type_Calc Input param cur_page_info->icon_info[%d].icon_buf==PNULL error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_2100_112_2_18_2_33_19_115,(uint8*)"d",i);
            return FALSE;
        }
    }
    for (i = 0; i < next_page_info->icon_num; i++)
    {
        if (PNULL == next_page_info->icon_info[i].icon_buf)
        {
            //SCI_TRACE_LOW:"MMIMENU_Go_Effect_Turntable_Type_Calc Input param next_page_info->icon_info[%d].icon_buf==PNULL error"
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,MMIMENU_GO_SE_2108_112_2_18_2_33_19_116,(uint8*)"d",i);
            return FALSE;
        }
    }*/

    if (GO_MOVE_LEFT == direction)
    {
        cur_angle = cur_frame_num*900/total_frame_num;
        next_angle = cur_angle - 900;
    }
    else
    {
        cur_angle = -cur_frame_num*900/total_frame_num;
        next_angle = cur_angle + 900;
    }
    
    bg_window_mid.x = 0;
    bg_window_mid.y = 0;

    for (i = 0; i < cur_page_info->icon_num; i++)
    {
        if (PNULL == cur_page_info->icon_info[i].icon_buf)
        {
            continue;
        }
        img_mid.x = bg_window_mid.x - cur_page_info->icon_info[i].icon_offset.x;
        img_mid.y = bg_window_mid.y - cur_page_info->icon_info[i].icon_offset.y;

		/*S2d_4_in_one_fast(bg_window_size,			
					cur_page_info->icon_size,					
					bg_window_mid,	
					img_mid,						    
					cur_angle,								            
					cur_page_info->icon_info[i].icon_buf,	
					bg_window_buf,						       
					z_para,								        
					255,					
					calc_point_buf);*/
		Make_Turntable_Shining(cur_frame_num,
		                    total_frame_num,
					direction,
					bg_window_size,			
					cur_page_info->icon_info[i].icon_size,					
					bg_window_mid,	
					img_mid,						    
					cur_angle,								            
					cur_page_info->icon_info[i].icon_buf,	
					bg_window_buf,						       
					z_para,								        
					255,					
					calc_point_buf,
					calc_data_info);
    }

	for (i = 0; i < next_page_info->icon_num; i++)
    {
        if (PNULL == next_page_info->icon_info[i].icon_buf)
        {
            continue;
        }
        img_mid.x = bg_window_mid.x - next_page_info->icon_info[i].icon_offset.x;
        img_mid.y = bg_window_mid.y - next_page_info->icon_info[i].icon_offset.y;

		/*S2d_4_in_one_fast(bg_window_size,			
					next_page_info->icon_size,					
					bg_window_mid,	
					img_mid,						    
					next_angle,								            
					next_page_info->icon_info[i].icon_buf,	
					bg_window_buf,						       
					z_para,								        
					255,					
					calc_point_buf);*/
		Make_Turntable_Shining(cur_frame_num,
		                    total_frame_num,
					direction,
					bg_window_size,			
					next_page_info->icon_info[i].icon_size,					
					bg_window_mid,	
					img_mid,						    
					next_angle,								            
					next_page_info->icon_info[i].icon_buf,	
					bg_window_buf,						       
					z_para,								        
					255,					
					calc_point_buf,
					calc_data_info);
    }
	return TRUE;
}
#endif//TURNTABLE

#ifdef GO_WHEEL_MAIN_MENU_SUPPORT 
/*****************************************************************************/
//  Description : 滚轮计算函数
//  Global resource dependence : 
//  Author: Ben
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_Go_Effect_Wheel_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
	int32 i = 0, angle_interval = 0, anlge_onset = 0, angle_temp = 0, radius = 0, rotation_dist = 0, rotation_frame = 1800,total_frame = 3600;
	
	IMGREF_POINT_T rotation_Point_c,base_rotation_Point;
	int32 frame_i = 0,frame_period = 0;
	int32 run_direction = 1;// 1:left, -1:right

	int32 frame_temp = 0;//0~3600

	IMGREF_POINT_T img_mid				= {0};
	IMGREF_POINT_T window_mid			= {0};
	IMGREF_POINT_T window_mid_Start	= {0};	
	IMGREF_POINT_T window_mid_End		= {0};	

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
//帧转换，实际计算中帧细化到total_frame帧
//除零问题，上一段已经检测
	frame_temp = total_frame*cur_frame_num/total_frame_num;
	//frame_temp = 1800;
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
	
	radius = MIN(bg_window_size.w,bg_window_size.h)*5/16;
//当前页处理
	base_rotation_Point.y = bg_window_size.h/2;
	if(1 == run_direction)
	{
		base_rotation_Point.x = 0;	
	}
	else
	{
		base_rotation_Point.x = bg_window_size.w;
	}
    if(0 != cur_page_info->icon_num)
    {
	    angle_interval = 3600/cur_page_info->icon_num;
    }
	if(frame_temp<(total_frame-rotation_frame))//直线运动加自身旋转
	{
		rotation_Point_c.x = base_rotation_Point.x;//bg_window_size.w/2;
		rotation_Point_c.y = base_rotation_Point.y;
		anlge_onset = 7200;
		
		frame_period = total_frame-rotation_frame;
		//if(0 < frame_period)//frame_period小于0没有意义，同时也为了后面避规除0操作
		//{		
		frame_i = frame_temp;	

		for(i = 0; i < cur_page_info->icon_num; ++i)
		{
            if (PNULL == cur_page_info->icon_info[i].icon_buf)
            {
                continue;
            }
			img_mid.x = cur_page_info->icon_info[i].icon_size.w/2;
		    img_mid.y = cur_page_info->icon_info[i].icon_size.h/2;		
			angle_temp = (anlge_onset - i * angle_interval)%3600;
			window_mid_End.x = rotation_Point_c.x + ((radius*s2d_cos(angle_temp))>>16);
			window_mid_End.y = rotation_Point_c.y + ((radius*s2d_sin(angle_temp))>>16);
			window_mid_Start.x = cur_page_info->icon_info[i].icon_offset.x + cur_page_info->icon_info[i].icon_size.w/2;
			window_mid_Start.y = cur_page_info->icon_info[i].icon_offset.y + cur_page_info->icon_info[i].icon_size.h/2;
			window_mid.x = frame_i*(window_mid_End.x-window_mid_Start.x)/frame_period + window_mid_Start.x;
			window_mid.y = frame_i*(window_mid_End.y-window_mid_Start.y)/frame_period + window_mid_Start.y;
			angle_temp = (angle_temp+2700)%3600;// angle at the end
			angle_temp = angle_temp-3600;//rotate direction ctrl
			angle_temp = frame_i*angle_temp/frame_period;
			//S2d_rotation_slim(bg_window_size,cur_page_info->icon_size,window_mid,img_mid,angle_temp,cur_page_info->icon_info[i].icon_buf,bg_window_buf,4096,calc_point_buf);
            S2d_Type_4_in_one_fast(bg_window_size,cur_page_info->icon_info[i].icon_size,window_mid,img_mid,angle_temp,cur_page_info->icon_info[i].icon_buf,calc_data_info->input_type,bg_window_buf,calc_data_info->output_type,4096,255,calc_point_buf,calc_data_info->param);
		}
		//}
	}
	else// if(0 < rotation_frame)//旋转运动加自身旋转,同时也为了后面避规除0操作
	{
		frame_period = rotation_frame;
		frame_i = frame_temp-(total_frame-rotation_frame);		
		rotation_dist = radius+MAX(cur_page_info->icon_info[0].icon_size.h,cur_page_info->icon_info[0].icon_size.w)/2;//jinx_temp
		rotation_Point_c.x = base_rotation_Point.x-run_direction*(frame_i*rotation_dist/frame_period);//bg_window_size.w/2;
		rotation_Point_c.y = base_rotation_Point.y;
		if((frame_period*2*3*radius)>0)//避规除0操作
		{
			anlge_onset = 7200-run_direction*((frame_i*rotation_dist)*3600/(frame_period*2*3*radius));//(frame_temp*rotation_dist/rotation_frame)*3600/(2*3.1415926*radius)

			for(i = 0; i < cur_page_info->icon_num; ++i)
			{
                if(PNULL == cur_page_info->icon_info[i].icon_buf)
                {
                    continue;
                }
				img_mid.x = cur_page_info->icon_info[i].icon_size.w/2;
			    img_mid.y = cur_page_info->icon_info[i].icon_size.h/2;		
				angle_temp = (anlge_onset - i * angle_interval)%3600;
				window_mid.x = rotation_Point_c.x + ((radius*s2d_cos(angle_temp))>>16);
				window_mid.y = rotation_Point_c.y + ((radius*s2d_sin(angle_temp))>>16);
				angle_temp = (angle_temp+2700)%3600;
				//S2d_rotation_slim(bg_window_size,cur_page_info->icon_size,window_mid,img_mid,angle_temp,cur_page_info->icon_info[i].icon_buf,bg_window_buf,4096,calc_point_buf);
                S2d_Type_4_in_one_fast(bg_window_size,cur_page_info->icon_info[i].icon_size,window_mid,img_mid,angle_temp,cur_page_info->icon_info[i].icon_buf,calc_data_info->input_type,bg_window_buf,calc_data_info->output_type,4096,255,calc_point_buf,calc_data_info->param);
			}
		}
	}
//下一页处理
	if(1 == run_direction)
	{
		base_rotation_Point.x = bg_window_size.w;
	}
	else
	{
		base_rotation_Point.x = 0;	
	}
    if(0 != next_page_info->icon_num)
    {
	    angle_interval = 3600/next_page_info->icon_num;
    }
    
	if(frame_temp<rotation_frame)
	{
		frame_period = rotation_frame;
		frame_i = frame_period - frame_temp;		
		rotation_dist = radius+MAX(next_page_info->icon_info[0].icon_size.h,next_page_info->icon_info[0].icon_size.w)/2;//jinx_temp
		rotation_Point_c.x = base_rotation_Point.x + run_direction*(frame_i*rotation_dist/frame_period);//output_size.w/2;
		rotation_Point_c.y = bg_window_size.h/2;
		if((frame_period*2*3*radius)>0)//避规除0操作
		{
			anlge_onset = 7200+run_direction*((frame_i*rotation_dist)*3600/(frame_period*2*3*radius));//(frame_temp*rotation_dist/rotation_frame)*3600/(2*3.1415926*radius)
			//angle_interval = 3600/flw_icon_num;	

			for(i = 0; i < next_page_info->icon_num; ++i)
			{
                if(PNULL == next_page_info->icon_info[i].icon_buf)
                {
                    continue;
                }
			    img_mid.x = next_page_info->icon_info[i].icon_size.w/2;
			    img_mid.y = next_page_info->icon_info[i].icon_size.h/2;	
				angle_temp = (anlge_onset - i * angle_interval)%3600;
				window_mid.x = rotation_Point_c.x + ((radius*s2d_cos(angle_temp))>>16);
				window_mid.y = rotation_Point_c.y + ((radius*s2d_sin(angle_temp))>>16);
				angle_temp = (angle_temp+2700)%3600;
				//S2d_rotation_slim(bg_window_size,next_page_info->icon_size,window_mid,img_mid,angle_temp,next_page_info->icon_info[i].icon_buf,bg_window_buf,4096,calc_point_buf);
                S2d_Type_4_in_one_fast(bg_window_size,next_page_info->icon_info[i].icon_size,window_mid,img_mid,angle_temp,next_page_info->icon_info[i].icon_buf,calc_data_info->input_type,bg_window_buf,calc_data_info->output_type,4096,255,calc_point_buf,calc_data_info->param);
			}	
		}
	}
	else
	{
		rotation_Point_c.x = base_rotation_Point.x;//bg_window_size.w/2;
		rotation_Point_c.y = bg_window_size.h/2;
		anlge_onset = 7200;
		//angle_interval = 3600/flw_icon_num;
		frame_period = total_frame-rotation_frame;
		//if(0 < frame_period)//frame_period小于0没有意义，同时也为了后面避规除0操作
		//{	
		frame_i = frame_period - (frame_temp-rotation_frame);	

		for(i = 0; i < next_page_info->icon_num; ++i)
		{
            if(PNULL == next_page_info->icon_info[i].icon_buf)
            {
                continue;
            }
			img_mid.x = next_page_info->icon_info[i].icon_size.w/2;
		    img_mid.y = next_page_info->icon_info[i].icon_size.h/2;	
			angle_temp = (anlge_onset - i * angle_interval)%3600;
			window_mid_End.x = rotation_Point_c.x + ((radius*s2d_cos(angle_temp))>>16);
			window_mid_End.y = rotation_Point_c.y + ((radius*s2d_sin(angle_temp))>>16);
			window_mid_Start.x = next_page_info->icon_info[i].icon_offset.x + next_page_info->icon_info[i].icon_size.w/2;
			window_mid_Start.y = next_page_info->icon_info[i].icon_offset.y + next_page_info->icon_info[i].icon_size.h/2;
			window_mid.x = frame_i*(window_mid_End.x-window_mid_Start.x)/frame_period + window_mid_Start.x;
			window_mid.y = frame_i*(window_mid_End.y-window_mid_Start.y)/frame_period + window_mid_Start.y;
			angle_temp = (angle_temp+2700)%3600;// angle at the end
			angle_temp = angle_temp-3600;//rotate direction ctrl
			angle_temp = frame_i*angle_temp/frame_period;
			//S2d_rotation_slim(bg_window_size,next_page_info->icon_size,window_mid,img_mid,angle_temp,next_page_info->icon_info[i].icon_buf,bg_window_buf,4096,calc_point_buf);
            S2d_Type_4_in_one_fast(bg_window_size,next_page_info->icon_info[i].icon_size,window_mid,img_mid,angle_temp,next_page_info->icon_info[i].icon_buf,calc_data_info->input_type,bg_window_buf,calc_data_info->output_type,4096,255,calc_point_buf,calc_data_info->param);
        }
		
	}
	return TRUE;
}
#endif

#ifdef GO_BOXROT_MAIN_MENU_SUPPORT 
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
LOCAL BOOLEAN MMIMENU_Go_Effect_Boxrot_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
        /*#ifdef GO_CALC_TEST
                    s3d_src_img.src_type=S3D_DATA_PAF_RGB888;
        s3d_img.color_type=S3D_DATA_PAF_RGB888;
#else
                    s3d_src_img.src_type=S3D_DATA_ARGB888;
s3d_img.color_type=S3D_DATA_RGB565;
#endif*/
    s3d_src_img.src_type=calc_data_info->input_type;
    s3d_img.color_type=calc_data_info->output_type;
	//s3d_src_img.src_type=S3D_DATA_PAF_RGB888;//S3D_DATA_ARGB888;	    
        s3d_img.color_ptr=bg_window_buf;
        s3d_img.size.h = bg_window_size.h;
        s3d_img.size.w = bg_window_size.w;
	for(i = 0; i < cur_page_info->icon_num; ++i)
	{   
        if(PNULL == cur_page_info->icon_info[i].icon_buf)
        {
            continue;
        }
		//计算个图标在盒子面上的3d位置
			s3d_geometry.img_size.w=cur_page_info->icon_info[i].icon_size.w; 
	        s3d_geometry.img_size.h=cur_page_info->icon_info[i].icon_size.h; 
	        s3d_geometry.axis.x= Center_point_x + (((cur_page_info->icon_info[i].icon_offset.x + cur_page_info->icon_info[i].icon_size.w/2 - bg_window_size.w/2)*s2d_cos_new(angle_temp))/65536);
	        s3d_geometry.axis.y= cur_page_info->icon_info[i].icon_offset.y + cur_page_info->icon_info[i].icon_size.h/2;
	        s3d_geometry.axis.z= Center_point_z + (((cur_page_info->icon_info[i].icon_offset.x + cur_page_info->icon_info[i].icon_size.w/2- bg_window_size.w/2)*s2d_sin_new(angle_temp))/65536);
	        s3d_src_img.src_ptr=cur_page_info->icon_info[i].icon_buf;
		//bright_by_z = (frame_max - frame_temp)*(bright_max-bright_min)/(frame_max)+bright_min;
		/*if(0 < (cur_page_info->total_col_num-1))//避规除0操作
		{
			frame_icon_max = frame_max - ((cur_page_info->total_col_num-1)*(1+run_direction)/2 - run_direction*cur_page_info->icon_info[i].item_col_num)*600/(cur_page_info->total_col_num-1);
			if(0 >= frame_icon_max)//避规除0操作
				continue;
			bright_by_z = (frame_icon_max - frame_temp)*(bright_max-bright_min)/(frame_icon_max)+bright_min;
			if(bright_by_z<bright_min)
				continue;
		}
		else
		{
			//避规除0操作一开始已经做了
			bright_by_z = (frame_max - frame_temp)*(bright_max-bright_min)/(frame_max)+bright_min;
		}*/
		Make_Boxrot_Shining(cur_frame_num,total_frame_num,direction,bg_window_size,&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z,calc_data_info->param);
		//S3D_ImageMidAxis565Rotation(&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z);
		
	}

	angle_temp = (-1)*run_direction*(frame_temp-900);
	Center_point_x = rotation_axis.x + ((radius*s2d_sin_new(angle_temp))>>16);
	Center_point_z = rotation_axis.z - ((radius*s2d_cos_new(angle_temp))>>16);

	s3d_geometry.angle=angle_temp+1800;
	bright_by_z = bright_max - bright_by_z;
	for(i = 0; i < next_page_info->icon_num; ++i)
	{	
        if(PNULL == next_page_info->icon_info[i].icon_buf)
        {
            continue;
        }
		//计算个图标在盒子面上的3d位置
			s3d_geometry.img_size.w=next_page_info->icon_info[i].icon_size.w; 
            s3d_geometry.img_size.h=next_page_info->icon_info[i].icon_size.h;
	        s3d_geometry.axis.x= Center_point_x + (((next_page_info->icon_info[i].icon_offset.x + next_page_info->icon_info[i].icon_size.w/2 - bg_window_size.w/2)*s2d_cos_new(angle_temp))/65536);
	        s3d_geometry.axis.y= next_page_info->icon_info[i].icon_offset.y + next_page_info->icon_info[i].icon_size.h/2;
	        s3d_geometry.axis.z= Center_point_z + (((next_page_info->icon_info[i].icon_offset.x + next_page_info->icon_info[i].icon_size.w/2 - bg_window_size.w/2)*s2d_sin_new(angle_temp))/65536);
	        s3d_src_img.src_ptr=next_page_info->icon_info[i].icon_buf;	
		/*if(0 < (next_page_info->total_col_num-1))//避规除0操作
		{
			frame_icon_max = frame_max - ((next_page_info->total_col_num-1)*(1-run_direction)/2 + run_direction*next_page_info->icon_info[i].item_col_num)*600/(next_page_info->total_col_num-1);
			if(frame_temp<(frame_max-frame_icon_max)
				||0 >= frame_icon_max)//避规除0操作
			{
				continue;
			}
			bright_by_z = (frame_icon_max - (frame_temp-(frame_max-frame_icon_max)))*(bright_min-bright_max)/(frame_icon_max)+bright_max;
		}
		else
		{
			//避规除0操作一开始已经做了
			bright_by_z = (frame_max - frame_temp)*(bright_max-bright_min)/(frame_max)+bright_min;
		}*/
		Make_Boxrot_Shining(cur_frame_num,total_frame_num,direction,bg_window_size,&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z,calc_data_info->param);
		//S3D_ImageMidAxis565Rotation(&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z);
	}
	return TRUE;

}

#endif
#ifdef GO_ICONBOXROT_MAIN_MENU_SUPPORT 
/*****************************************************************************/
//  Description : Iconboxrot 特效执行函数，包含是否打光特效
//  Global resource dependence : 
//  Author: BENNY
//  Note:
/*****************************************************************************/

LOCAL void Make_Iconboxrot_Shining(uint16 cur_frame,//当前帧
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
        int32 delta_xy = 160,x_margin = 10;
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
LOCAL BOOLEAN MMIMENU_Go_Effect_Iconboxrot_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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

	int32 i = 0, angle_temp = 0,angle_temp_i = 0, angle_interval = 225;
	int32 run_direction 	= -1;// 1:left, -1:right
	uint8 bright_by_z 	= 0;
	uint8 bright_min 		= 0;
	uint8 bright_max 		= 255;
	int32 frame_temp 	= 0;//0~3600
	int32 frame_max 		= 900;

	S3D_ROTATION_GEOMETRY_T s3d_geometry	= {0};
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

//帧转换，实际计算中帧细化到frame_max+angle_interval*(cur_page_info->total_col_num-1)帧	，
//因为要做相位延迟，所以总帧数要相应变大
//除零问题，上一段已经检测
	frame_temp = (frame_max+angle_interval*(cur_page_info->total_col_num-1))*cur_frame_num/total_frame_num;
	//frame_temp = 450;
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
	
	//frame_temp = (frame_max+angle_interval*(cur_page_info->total_col_num-1))*1/total_frame_num;
	//run_direction = -1;

	rotation_axis.axis = 1;//y axis
	rotation_axis.x = bg_window_size.w/2;
	rotation_axis.y = 0;// it can be any value, because y axis is the rotation axis
	rotation_axis.z = bg_window_size.w/2;

	
	//前后景排序
		
		s3d_geometry.img_size.x =0;
		s3d_geometry.img_size.y = 0;
		s3d_img.color_ptr=bg_window_buf;
	      s3d_img.size.h = bg_window_size.h;
	      s3d_img.size.w = bg_window_size.w;
		
        	s3d_geometry.eye.x=bg_window_size.w/2;
        	s3d_geometry.eye.y=bg_window_size.h/2;
        	s3d_geometry.eye.z=-1*bg_window_size.h;	
		//bright_by_z = 255;
		s3d_geometry.axis.axis=1;
		s3d_geometry.is_axis_use_default = SCI_FALSE;
		s3d_geometry.is_eye_use_default = SCI_FALSE;

		for(i = 0; i < cur_page_info->icon_num; ++i)
		{
            if(PNULL == cur_page_info->icon_info[i].icon_buf)
            {
                continue;
            }
            
			//相位按列循序延迟
			s3d_geometry.img_size.w=cur_page_info->icon_info[i].icon_size.w; 
		    s3d_geometry.img_size.h=cur_page_info->icon_info[i].icon_size.h; 
			angle_temp_i = frame_temp - ((cur_page_info->total_col_num-1)*(1+run_direction)/2-run_direction*cur_page_info->icon_info[i].item_col_num)*angle_interval;
			angle_temp_i = angle_temp_i<0 ? 0:angle_temp_i;
			angle_temp_i = angle_temp_i>frame_max ? frame_max:angle_temp_i;
			angle_temp = (-1)*run_direction*angle_temp_i;	
			s3d_geometry.angle=angle_temp+1800;			
		        s3d_src_img.src_ptr=cur_page_info->icon_info[i].icon_buf;
		        //s3d_src_img.src_type=S3D_DATA_ARGB888;
		        /*#ifdef GO_CALC_TEST
                        s3d_src_img.src_type=S3D_DATA_PAF_RGB888;
                        s3d_img.color_type=S3D_DATA_PAF_RGB888;
                    #else
                        s3d_src_img.src_type=S3D_DATA_ARGB888;
                        s3d_img.color_type=S3D_DATA_RGB565;
                    #endif*/
            s3d_src_img.src_type=calc_data_info->input_type;
            s3d_img.color_type=calc_data_info->output_type;
			//计算每个图标在小盒子旋转后的3d位置
			s3d_geometry.axis.x= cur_page_info->icon_info[i].icon_offset.x + cur_page_info->icon_info[i].icon_size.w/2 + (((cur_page_info->icon_info[i].icon_size.w/2)*s2d_sin_new(angle_temp))/65536);
		        s3d_geometry.axis.y= cur_page_info->icon_info[i].icon_offset.y + cur_page_info->icon_info[i].icon_size.h/2;
		        s3d_geometry.axis.z= cur_page_info->icon_info[i].icon_size.w/2 - (((cur_page_info->icon_info[i].icon_size.w/2)*s2d_cos_new(angle_temp))/65536);
			//当前页透明度在旋转45度就消失，这样可以保持最多只有六列需要计算，加快运算速度
			if(frame_max-450 <= angle_temp_i
				||450 >= frame_max)//避规除0操作
				continue;
			bright_by_z = (frame_max-450 - angle_temp_i)*(bright_max-bright_min)/(frame_max-450)+bright_min;
			Make_Iconboxrot_Shining(cur_frame_num,total_frame_num,direction,bg_window_size,&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z,calc_data_info->param);
			//S3D_ImageMidAxis565Rotation(&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z);

		}
        
		s3d_geometry.img_size.x =0;
		s3d_geometry.img_size.y = 0;
		
	        s3d_geometry.eye.x=bg_window_size.w/2;
	        s3d_geometry.eye.y=bg_window_size.h/2;
	        s3d_geometry.eye.z=-1*bg_window_size.h;
		s3d_geometry.axis.axis=1;
		s3d_geometry.is_axis_use_default = SCI_FALSE;
		s3d_geometry.is_eye_use_default = SCI_FALSE;
		s3d_img.color_ptr=bg_window_buf;
	        s3d_img.size.h = bg_window_size.h;
	        s3d_img.size.w = bg_window_size.w;
		        
		for(i = 0; i < next_page_info->icon_num; ++i)
		{
            if(PNULL == next_page_info->icon_info[i].icon_buf)
            {
                continue;
            }

			//相位按列循序延迟
			s3d_geometry.img_size.w=next_page_info->icon_info[i].icon_size.w; 
		    s3d_geometry.img_size.h=next_page_info->icon_info[i].icon_size.h; 
			angle_temp_i = frame_temp - ((next_page_info->total_col_num-1)*(1+run_direction)/2-run_direction*next_page_info->icon_info[i].item_col_num)*angle_interval;
			angle_temp_i = angle_temp_i<0 ? 0:angle_temp_i;
			angle_temp_i = angle_temp_i>frame_max ? frame_max:angle_temp_i;
			angle_temp = (-1)*run_direction*(angle_temp_i-900);     
		 	s3d_geometry.angle=angle_temp+1800;
			//计算每个图标在小盒子旋转后的3d位置
		        s3d_geometry.axis.x= next_page_info->icon_info[i].icon_offset.x + next_page_info->icon_info[i].icon_size.w/2 + (((next_page_info->icon_info[i].icon_size.w/2)*s2d_sin_new(angle_temp))/65536);
		        s3d_geometry.axis.y= next_page_info->icon_info[i].icon_offset.y + next_page_info->icon_info[i].icon_size.h/2;
		        s3d_geometry.axis.z= next_page_info->icon_info[i].icon_size.w/2 - (((next_page_info->icon_info[i].icon_size.w/2)*s2d_cos_new(angle_temp))/65536);
      	    
		        s3d_src_img.src_ptr=next_page_info->icon_info[i].icon_buf;
		        //s3d_src_img.src_type=S3D_DATA_ARGB888;	
                    /*#ifdef GO_CALC_TEST
                        s3d_src_img.src_type=S3D_DATA_PAF_RGB888;
                        s3d_img.color_type=S3D_DATA_PAF_RGB888;
                    #else
                        s3d_src_img.src_type=S3D_DATA_ARGB888;
                        s3d_img.color_type=S3D_DATA_RGB565;
                    #endif*/
            s3d_src_img.src_type=calc_data_info->input_type;
            s3d_img.color_type=calc_data_info->output_type;
				//下一页透明度变化在旋转0-90度，90度完全显示
			bright_by_z = (frame_max - angle_temp_i)*(bright_min-bright_max)/(frame_max)+bright_max;
			Make_Iconboxrot_Shining(cur_frame_num,total_frame_num,direction,bg_window_size,&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z,calc_data_info->param);
			//S3D_ImageMidAxis565Rotation(&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z);
		}	
	return TRUE;

}
#endif
#ifdef GO_BOOKPAGE_MAIN_MENU_SUPPORT 
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
LOCAL BOOLEAN MMIMENU_Go_Effect_Bookpage_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
        //s3d_src_img.src_type=S3D_DATA_ARGB888;
        /*#ifdef GO_CALC_TEST
        s3d_src_img.src_type=S3D_DATA_PAF_RGB888;
        s3d_img.color_type=S3D_DATA_PAF_RGB888;
        #else
        s3d_src_img.src_type=S3D_DATA_ARGB888;
        s3d_img.color_type=S3D_DATA_RGB565;
#endif*/
        s3d_src_img.src_type=calc_data_info->input_type;
        s3d_img.color_type=calc_data_info->output_type;
        s3d_img.color_ptr=bg_window_buf;
        s3d_img.size.h = bg_window_size.h;
        s3d_img.size.w = bg_window_size.w;
        
        for(i = 0; i < next_page_info->icon_num; ++i)
        {	
            if(PNULL == next_page_info->icon_info[i].icon_buf)
            {
                continue;
            }
            s3d_geometry.img_size.w=next_page_info->icon_info[i].icon_size.w; 
            s3d_geometry.img_size.h=next_page_info->icon_info[i].icon_size.h;
            s3d_geometry.axis.x= rotation_axis.x + (((next_page_info->icon_info[i].icon_offset.x + next_page_info->icon_info[i].icon_size.w/2 - rotation_axis.x)*s2d_cos_new(angle_temp))/65536);
            s3d_geometry.axis.y= next_page_info->icon_info[i].icon_offset.y + next_page_info->icon_info[i].icon_size.h/2;
            s3d_geometry.axis.z= rotation_axis.z - (((next_page_info->icon_info[i].icon_offset.x + next_page_info->icon_info[i].icon_size.w/2 - rotation_axis.x)*s2d_sin_new(angle_temp))/65536);
            s3d_src_img.src_ptr=next_page_info->icon_info[i].icon_buf;
            
            //frame_icon_max = frame_max - ((next_page_info->total_col_num-1)*(1-run_direction)/2 + run_direction*next_page_info->icon_info[i].item_col_num)*600/(next_page_info->total_col_num-1);
            if((next_page_info->total_col_num-1)>0)
            {
                frame_icon_max = frame_max - next_page_info->icon_info[i].item_col_num*300/(next_page_info->total_col_num-1);
                if(frame_temp<(frame_max-frame_icon_max)
                    ||0 >= frame_icon_max)//避规除0操作
                {
                    continue;
                }
                bright_by_z = (frame_icon_max - (frame_temp-(frame_max-frame_icon_max)))*(bright_min-bright_max)/(frame_icon_max)+bright_max;
            }
            else
            {
                //避规除0操作一开始已经包含
                bright_by_z = abs(s3d_geometry.angle+1800)*(bright_min - bright_max)/frame_max+bright_max;
            }	
            bright_by_z = bright_by_z*bright_i;
            if(255<bright_by_z)
            {
                bright_by_z = 255;
            }
            Make_Bookpage_Shining(cur_frame_num,total_frame_num,direction,bg_window_size,&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z,calc_data_info->param);
            //S3D_ImageMidAxis565Rotation(&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z);
            
        }
        
        angle_temp = run_direction*frame_temp;		        
	      	s3d_geometry.angle=(-1)*(angle_temp+1800)%3600;
            
            for(i = 0; i < cur_page_info->icon_num; ++i)
            {		
                if(PNULL == cur_page_info->icon_info[i].icon_buf)
                {
                    continue;
                }
                s3d_geometry.img_size.w=cur_page_info->icon_info[i].icon_size.w; 
                s3d_geometry.img_size.h=cur_page_info->icon_info[i].icon_size.h; 
                s3d_geometry.axis.x= rotation_axis.x + (((cur_page_info->icon_info[i].icon_offset.x + cur_page_info->icon_info[i].icon_size.w/2 - rotation_axis.x)*s2d_cos_new(angle_temp))/65536);
                s3d_geometry.axis.y= cur_page_info->icon_info[i].icon_offset.y + cur_page_info->icon_info[i].icon_size.h/2;
                s3d_geometry.axis.z= rotation_axis.z - (((cur_page_info->icon_info[i].icon_offset.x + cur_page_info->icon_info[i].icon_size.w/2 - rotation_axis.x)*s2d_sin_new(angle_temp))/65536);
                s3d_src_img.src_ptr=cur_page_info->icon_info[i].icon_buf;
                if((cur_page_info->total_col_num-1)>0)//避规除0操作
                {		
                    frame_icon_max = frame_max - cur_page_info->icon_info[i].item_col_num*300/(cur_page_info->total_col_num-1);
                    if(0 >= frame_icon_max)//避规除0操作
                        continue;
                    bright_by_z = (frame_icon_max - frame_temp)*(bright_max-bright_min)/(frame_icon_max)+bright_min;
                    //bright_by_z = abs(s3d_geometry.angle+1800)*(bright_min - bright_max)/frame_max+bright_max;
                    if(bright_by_z<bright_min)
                        continue;
                }
                else
                {
                    //避规除0操作一开始已经包含
                    bright_by_z = abs(s3d_geometry.angle+1800)*(bright_min - bright_max)/frame_max+bright_max;
                }
                
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
        //s3d_src_img.src_type=S3D_DATA_ARGB888;	
        /*#ifdef GO_CALC_TEST
        s3d_src_img.src_type=S3D_DATA_PAF_RGB888;
        s3d_img.color_type=S3D_DATA_PAF_RGB888;
        #else
        s3d_src_img.src_type=S3D_DATA_ARGB888;
        s3d_img.color_type=S3D_DATA_RGB565;
#endif*/
        s3d_src_img.src_type=calc_data_info->input_type;
        s3d_img.color_type=calc_data_info->output_type;
        s3d_img.color_ptr=bg_window_buf;
        s3d_img.size.h = bg_window_size.h;
        s3d_img.size.w = bg_window_size.w;
        for(i = 0; i < cur_page_info->icon_num; ++i)
        {		
            if(PNULL == cur_page_info->icon_info[i].icon_buf)
            {
                continue;
            }
            //计算3d位置
            s3d_geometry.img_size.w=cur_page_info->icon_info[i].icon_size.w; 
            s3d_geometry.img_size.h=cur_page_info->icon_info[i].icon_size.h; 
            s3d_geometry.axis.x= rotation_axis.x + (((cur_page_info->icon_info[i].icon_offset.x + cur_page_info->icon_info[i].icon_size.w/2 - rotation_axis.x)*s2d_cos_new(angle_temp))/65536);
            s3d_geometry.axis.y= cur_page_info->icon_info[i].icon_offset.y + cur_page_info->icon_info[i].icon_size.h/2;
            s3d_geometry.axis.z= rotation_axis.z - (((cur_page_info->icon_info[i].icon_offset.x + cur_page_info->icon_info[i].icon_size.w/2 - rotation_axis.x)*s2d_sin_new(angle_temp))/65536);		        
            s3d_src_img.src_ptr=cur_page_info->icon_info[i].icon_buf;
            if((cur_page_info->total_col_num-1)>0)//避规除0操作
            {
                //这里的透明度计算为了减少计算量，只有在前景页的消失一列后，后景才可以多出一列，保持计算最多6列
                frame_icon_max = frame_max - cur_page_info->icon_info[i].item_col_num*300/(cur_page_info->total_col_num-1);		
                if(0 >= frame_icon_max)//避规除0操作
                    continue;
                bright_by_z = (frame_icon_max - frame_temp)*(bright_max-bright_min)/(frame_icon_max)+bright_min;
                //bright_by_z = abs(s3d_geometry.angle+1800)*(bright_min - bright_max)/frame_max+bright_max;
                if(bright_by_z<bright_min)
                    continue;
            }
            else
            {
                bright_by_z = abs(s3d_geometry.angle+1800)*(bright_min - bright_max)/frame_max+bright_max;
            }
            
            S3D_AxisMidRotation_Img(&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z,calc_data_info->param);
            
        }	
        
        angle_temp = run_direction*(frame_temp-900);			        
        s3d_geometry.angle=(-1)*(angle_temp+1800)%3600;
        //bright_by_z = abs(s3d_geometry.angle+1800)*(bright_min - bright_max)/frame_max+bright_max;
        
        for(i = 0; i < next_page_info->icon_num; ++i)
        {	
            if(PNULL == next_page_info->icon_info[i].icon_buf)
            {
                continue;
            }
            
            s3d_geometry.img_size.w=next_page_info->icon_info[i].icon_size.w; 
            s3d_geometry.img_size.h=next_page_info->icon_info[i].icon_size.h;
            s3d_geometry.axis.x= rotation_axis.x + (((next_page_info->icon_info[i].icon_offset.x + next_page_info->icon_info[i].icon_size.w/2 - rotation_axis.x)*s2d_cos_new(angle_temp))/65536);
            s3d_geometry.axis.y= next_page_info->icon_info[i].icon_offset.y + next_page_info->icon_info[i].icon_size.h/2;
            s3d_geometry.axis.z= rotation_axis.z - (((next_page_info->icon_info[i].icon_offset.x + next_page_info->icon_info[i].icon_size.w/2 - rotation_axis.x)*s2d_sin_new(angle_temp))/65536);
            s3d_src_img.src_ptr=next_page_info->icon_info[i].icon_buf;
            if((next_page_info->total_col_num-1)>0)//避规除0操作
            {
                //这里的透明度计算为了减少计算量，只有在前景页的消失一列后，后景才可以多出一列，保持计算最多6列
                frame_icon_max = frame_max - next_page_info->icon_info[i].item_col_num*300/(next_page_info->total_col_num-1);
                if(frame_temp<(frame_max-frame_icon_max)
                    || 0 >= frame_icon_max)//避规除0操作
                {
                    continue;
                }
                bright_by_z = (frame_icon_max - (frame_temp-(frame_max-frame_icon_max)))*(bright_min-bright_max)/(frame_icon_max)+bright_max;
            }
            else
            {
                bright_by_z = abs(s3d_geometry.angle+1800)*(bright_min - bright_max)/frame_max+bright_max;
            }
            bright_by_z = bright_by_z*bright_i;
            if(255<bright_by_z)
            {
                bright_by_z = 255;
            }
            Make_Bookpage_Shining(cur_frame_num,total_frame_num,direction,bg_window_size,&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z,calc_data_info->param);
            //S3D_ImageMidAxis565Rotation(&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z);
        }
    }
    
    return SCI_TRUE;
    
}
#endif
#ifdef GO_CYLINDER_MAIN_MENU_SUPPORT
/*****************************************************************************/
//  Description : 圆柱体计算函数
//  Global resource dependence : 
//  Author: Ben
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN MMIMENU_Go_Effect_Cylinder_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
	int32 i = 0, radius = 0, angle_interval = 0, anlge_onset = 0, angle_temp = 0;//x = 0, y = 0,row_num = 4, column_num = 4, x_border = 20, y_border = 20,, x_interval = 0, y_interval = 0,
	//int32 row_index = 0, column_index = 0;
	int32 run_direction 	= -1;// 1:left, -1:right
	uint8 bright_by_z 	= 0;
	uint8 bright_min 		= 50;
	uint8 bright_max 		= 255;

	int32 frame_temp 	= 0;//0~3600
	int32 frame_period 	= 3600;

	S3D_ROTATION_GEOMETRY_T s3d_geometry 	= {0};
	S3D_ROTATION_IMG_T 	s3d_src_img			= {0};
	S3D_ROTATION_INOUT_IMG_INFO_T s3d_img	= {0};

	S3D_AXIS_T rotation_axis 		= {0};

	if(PNULL == cur_page_info
		||PNULL == next_page_info
		||PNULL == bg_window_buf
		||PNULL == calc_point_buf
		||(cur_frame_num > total_frame_num)
		||0 == total_frame_num)
	{
		return FALSE;
	}

	frame_temp = 1800*cur_frame_num/total_frame_num;
	//frame_temp = 200;
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
    
    radius = bg_window_size.w*5/12;//(bg_window_size.w-40)/2; when bg_window_size.w=240
    if(radius<=0)//避规除0操作
    {
        return FALSE;
    }
        
    rotation_axis.axis = 1;//y axis
    rotation_axis.x = bg_window_size.w/2;
    rotation_axis.y = 0;// it can be any value, because y axis is the rotation axis
    rotation_axis.z = radius;
    
    s3d_geometry.img_size.x =0;
    s3d_geometry.img_size.y = 0;
    
    s3d_geometry.eye.x=bg_window_size.w/2;
    s3d_geometry.eye.y=bg_window_size.h/2;
    s3d_geometry.eye.z=-2*bg_window_size.h;	
    s3d_geometry.axis.axis=1;
    s3d_geometry.is_axis_use_default = SCI_FALSE;
    s3d_geometry.is_eye_use_default = SCI_FALSE;
    s3d_img.color_ptr=bg_window_buf;
    s3d_img.size.h = bg_window_size.h;
    s3d_img.size.w = bg_window_size.w;
    //s3d_src_img.src_type=S3D_DATA_ARGB888;	
    /*#ifdef GO_CALC_TEST
    s3d_src_img.src_type=S3D_DATA_PAF_RGB888;
    s3d_img.color_type=S3D_DATA_PAF_RGB888;
    #else
    s3d_src_img.src_type=S3D_DATA_ARGB888;
    s3d_img.color_type=S3D_DATA_RGB565;
#endif*/
    s3d_src_img.src_type=calc_data_info->input_type;
    s3d_img.color_type=calc_data_info->output_type;
    
    if (0 != cur_page_info->total_col_num)
    {
        angle_interval = 1800/cur_page_info->total_col_num;
        anlge_onset = angle_interval/2;
        
        for(i = 0; i < cur_page_info->icon_num; ++i)
        {	
            if(PNULL == cur_page_info->icon_info[i].icon_buf)
            {
                continue;
            }
            s3d_geometry.img_size.w=cur_page_info->icon_info[i].icon_size.w;
            s3d_geometry.img_size.h=cur_page_info->icon_info[i].icon_size.h;
            angle_temp = (-1)*run_direction*frame_temp + 1800+anlge_onset+cur_page_info->icon_info[i].item_col_num*angle_interval;
            if((angle_temp%frame_period)>(frame_period/2))//前后景排序控制，先画圆柱体后半部分
            {continue;}				        
            s3d_geometry.angle=angle_temp+900+1800;		
            // 3d位置计算
            s3d_geometry.axis.x= rotation_axis.x + ((radius*s2d_cos_new(angle_temp))>>16);
            s3d_geometry.axis.y= cur_page_info->icon_info[i].icon_offset.y + cur_page_info->icon_info[i].icon_size.h/2;
            s3d_geometry.axis.z= rotation_axis.z + ((radius*s2d_sin_new(angle_temp))>>16);        
            
            s3d_src_img.src_ptr=cur_page_info->icon_info[i].icon_buf;
            //透明度根据z值计算      
            bright_by_z = (-1)*(s3d_geometry.axis.z-(rotation_axis.z+radius))*(bright_max-bright_min)/(2*radius)+bright_min;
            //if(s3d_geometry.axis.z>(rotation_axis.z+7*radius/8))
            //	continue;
            //bright_by_z = (-1)*(s3d_geometry.axis.z-(rotation_axis.z+7*radius/8))*(bright_max-bright_min)*8/(15*radius)+bright_min;
            S3D_AxisMidRotation_Img(&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z,calc_data_info->param);		
        }
    }
    
    if (0 != next_page_info->total_col_num)
    {
        angle_interval = 1800/next_page_info->total_col_num;
        anlge_onset = angle_interval/2;
        
        for(i = 0; i < next_page_info->icon_num; ++i)
        {	
            if(PNULL == next_page_info->icon_info[i].icon_buf)
            {continue;}
            s3d_geometry.img_size.w=next_page_info->icon_info[i].icon_size.w;
            s3d_geometry.img_size.h=next_page_info->icon_info[i].icon_size.h;
            angle_temp = (-1)*run_direction*frame_temp + anlge_onset+next_page_info->icon_info[i].item_col_num*angle_interval;//1800+1800 = 3600 => 0;
            if((angle_temp%frame_period)>(frame_period/2))//前后景排序控制，先画圆柱体后半部分
            {continue;}		
            // 3d位置计算
            s3d_geometry.angle=angle_temp+900+1800;				        
            s3d_geometry.axis.x= rotation_axis.x + ((radius*s2d_cos_new(angle_temp))>>16);
            s3d_geometry.axis.y= next_page_info->icon_info[i].icon_offset.y + next_page_info->icon_info[i].icon_size.h/2;
            s3d_geometry.axis.z= rotation_axis.z + ((radius*s2d_sin_new(angle_temp))>>16);				    
            s3d_src_img.src_ptr=next_page_info->icon_info[i].icon_buf;
            //透明度根据z值计算      
            bright_by_z = (-1)*(s3d_geometry.axis.z-(rotation_axis.z+radius))*(bright_max-bright_min)/(2*radius)+bright_min;
            //if(s3d_geometry.axis.z>(rotation_axis.z+7*radius/8))
            //	continue;
            //bright_by_z = (-1)*(s3d_geometry.axis.z-(rotation_axis.z+7*radius/8))*(bright_max-bright_min)*8/(15*radius)+bright_min;
            S3D_AxisMidRotation_Img(&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z,calc_data_info->param);
        }
    }
    
    if (0 != cur_page_info->total_col_num)
    {
        angle_interval = 1800/cur_page_info->total_col_num;
        anlge_onset = angle_interval/2;
        for(i = 0; i < cur_page_info->icon_num; ++i)
        {	
            if(PNULL == cur_page_info->icon_info[i].icon_buf)
            {
                continue;
            }
            s3d_geometry.img_size.w=cur_page_info->icon_info[i].icon_size.w;
            s3d_geometry.img_size.h=cur_page_info->icon_info[i].icon_size.h;
            angle_temp = (-1)*run_direction*frame_temp + 1800+anlge_onset+cur_page_info->icon_info[i].item_col_num*angle_interval;
            if((angle_temp%frame_period)<=(frame_period/2))//前后景排序控制，先画圆柱体后半部分
            {continue;}		        	        
            s3d_geometry.angle=angle_temp+900+1800;			        
            s3d_geometry.axis.x= rotation_axis.x + ((radius*s2d_cos_new(angle_temp))>>16);
            s3d_geometry.axis.y= cur_page_info->icon_info[i].icon_offset.y + cur_page_info->icon_info[i].icon_size.h/2;
            s3d_geometry.axis.z= rotation_axis.z + ((radius*s2d_sin_new(angle_temp))>>16);			    
            s3d_src_img.src_ptr=cur_page_info->icon_info[i].icon_buf;
            //透明度根据z值计算      
            bright_by_z = (-1)*(s3d_geometry.axis.z-(rotation_axis.z+radius))*(bright_max-bright_min)/(2*radius)+bright_min;
            //if(s3d_geometry.axis.z>(rotation_axis.z+7*radius/8))
            //	continue;
            //bright_by_z = (-1)*(s3d_geometry.axis.z-(rotation_axis.z+7*radius/8))*(bright_max-bright_min)*8/(15*radius)+bright_min;
            S3D_AxisMidRotation_Img(&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z,calc_data_info->param);
        }
    }
    
    if (0 != next_page_info->total_col_num)
    {
        angle_interval = 1800/next_page_info->total_col_num;
        anlge_onset = angle_interval/2;
        for(i = 0; i < next_page_info->icon_num; ++i)
        {	
            if(PNULL == next_page_info->icon_info[i].icon_buf)
            {continue;}
            s3d_geometry.img_size.w=next_page_info->icon_info[i].icon_size.w;
            s3d_geometry.img_size.h=next_page_info->icon_info[i].icon_size.h;
            angle_temp = (-1)*run_direction*frame_temp + anlge_onset+next_page_info->icon_info[i].item_col_num*angle_interval;//1800+1800 = 3600 => 0;
            if((angle_temp%frame_period)<=(frame_period/2))//前后景排序控制，先画圆柱体后半部分
            {continue;}		        
            s3d_geometry.angle=angle_temp+900+1800;				        
            s3d_geometry.axis.x= rotation_axis.x + ((radius*s2d_cos_new(angle_temp))>>16);
            s3d_geometry.axis.y= next_page_info->icon_info[i].icon_offset.y + next_page_info->icon_info[i].icon_size.h/2;
            s3d_geometry.axis.z= rotation_axis.z + ((radius*s2d_sin_new(angle_temp))>>16);		    
            s3d_src_img.src_ptr=next_page_info->icon_info[i].icon_buf;
            //透明度根据z值计算  
            bright_by_z = (-1)*(s3d_geometry.axis.z-(rotation_axis.z+radius))*(bright_max-bright_min)/(2*radius)+bright_min;
            //if(s3d_geometry.axis.z>(rotation_axis.z+7*radius/8))
            //	continue;
            //bright_by_z = (-1)*(s3d_geometry.axis.z-(rotation_axis.z+7*radius/8))*(bright_max-bright_min)*8/(15*radius)+bright_min;
            S3D_AxisMidRotation_Img(&(s3d_geometry),&(s3d_src_img),&(s3d_img),bright_by_z,calc_data_info->param);
        }
    }
    
    return SCI_TRUE;
}
#endif
#ifdef GO_MIDROTATE_MAIN_MENU_SUPPORT
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
        //s3d_src_img.src_type=S3D_DATA_ARGB888;
        /*#ifdef GO_CALC_TEST
                        s3d_src_img.src_type=S3D_DATA_PAF_RGB888;
        s3d_img.color_type=S3D_DATA_PAF_RGB888;
                    #else
                        s3d_src_img.src_type=S3D_DATA_ARGB888;
                    s3d_img.color_type=S3D_DATA_RGB565;
                    #endif*/
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

PUBLIC BOOLEAN MMIMENU_Go_Effect_MidRotate_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
	int angle=0,p_row=0,p_column=0,n=0,i=0,j=0,temp=0,n_row=0,n_column=0,axis_angle = 0,z_deep=0,color_deep=0,base_color=0;
	GO_ROTA_DATA_T axis_data={0};
	int32 bright_i = 2;

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
	p_row=cur_page_info->total_row_num;
	p_column=cur_page_info->total_col_num;
	n_row=next_page_info->total_row_num;
	n_column=next_page_info->total_col_num;
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
	//z_deep=-z_deep;
	//对前一页进行旋转
      if(cur_frame_num<total_frame_num/2)
      	{
	      	for(i=0;i<p_row;i++)
					{
						for(j=0;j<p_column;j++)
							{	temp=i*p_column+j;
								if(PNULL!=(cur_page_info->icon_info+temp)->icon_buf)
									{	
										if(GO_MOVE_RIGHT==direction)
										{	
											base_color=p_column-j-1;
										}
										else if(GO_MOVE_LEFT==direction)
										{
											base_color=j;
										}
										//离屏幕远的部分颜色更暗。
										color_deep=255*base_color/p_column+(255-255*base_color/p_column)*abs(abs(axis_angle)-900)/900;
										color_deep = color_deep*bright_i;
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
      	}
	  //超过总帧数的一半下一页进行旋转
	else if(cur_frame_num>=total_frame_num/2)
		{   n=-1;
	      	for(i=0;i<p_row;i++)
					{
						for(j=0;j<p_column;j++)
							{	temp=i*p_column+j;
								if(PNULL!=(next_page_info->icon_info+temp)->icon_buf)
									{	if(GO_MOVE_RIGHT==direction)
										{	
										    base_color=j;
								        }
										else if(GO_MOVE_LEFT==direction)
										{   
										    base_color=p_column-j-1;
										}
										//离屏幕远的颜色更暗。
										color_deep=255*base_color/p_column+(255-255*base_color/p_column)*abs(abs(axis_angle)-900)/900;
										color_deep = color_deep*bright_i;
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
      		}
   return TRUE;
}
#endif

#ifdef GO_SMALLITEMS_MAIN_MENU_SUPPORT
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

PUBLIC BOOLEAN MMIMENU_Go_Effect_SmallItems_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
	int row=0,column=0,pre_alpha=0,next_alpha=0,pre_z=0,next_z=0,h_frame=0,radius=0,vector=0,angle=0,distance=0;
	BOOLEAN Is_pre_out=FALSE;
	IMGREF_POINT_T window_offset={0};
	IMGREF_POINT_T img_offset={0};
	//FIX16_POINT_T Rota_buff[2048];
	IMGREF_POINT_T  pre_thumb_array[6][6]={0};
	IMGREF_POINT_T  next_thumb_array[6][6]={0};

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
	row=cur_page_info->total_row_num;
	column=cur_page_info->total_col_num;
	


	
	
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
			if(!Is_pre_out)
				{	pre_alpha=255*(h_frame-cur_frame_num)/(h_frame);
					/*vector=(radius*s2d_sin(angle*2))>>16;
					distance=(radius*(65536-s2d_cos(angle*2)))>>16;
					pre_z=radius*4096/(radius+distance);
					*/
					vector=radius*(cur_frame_num)/(h_frame);
					distance=vector;
					pre_z=radius*4096/(radius+distance);
					dx=(bg_window_size.w/2-(bg_window_size.w/2)*pre_z/4096);
					dy=(bg_window_size.h/2-(bg_window_size.h/2)*pre_z/4096);
					for(i=0;i<row;i++)
						{	
								for(j=0;j<column;j++)
							{    
								
								pre_thumb_array[i][j].x=(cur_page_info->icon_info+i*column+j)->icon_offset.x*pre_z/4096+dx+vector*n;
								pre_thumb_array[i][j].y=(cur_page_info->icon_info+i*column+j)->icon_offset.y*pre_z/4096+dy;
								
							}
						}
				}
			next_alpha=255*(cur_frame_num)/(total_frame_num);
			vector=radius*(cur_frame_num)/(total_frame_num);
					distance=radius-vector;
					next_z=radius*4096/(radius+distance);
					/*
					vector=(radius*(65536-s2d_cos(angle)))>>16;
					distance=(radius*(65536-s2d_sin(angle)))>>16;
					next_z=radius*4096/(radius+distance);
					*/
					dx=(bg_window_size.w/2-(bg_window_size.w/2)*next_z/4096);
					dy=(bg_window_size.h/2-(bg_window_size.h/2)*next_z/4096);
			for(i=0;i<row;i++)
				{
						for(j=0;j<column;j++)
					{    
						next_thumb_array[i][j].x=(next_page_info->icon_info+i*column+j)->icon_offset.x*next_z/4096+dx+(vector-bg_window_size.w/2)*(n);
						next_thumb_array[i][j].y=(next_page_info->icon_info+i*column+j)->icon_offset.y*next_z/4096+dy;
					}
				}
		
	
			
			
		
	shrik_wid=bg_window_size.w*pre_z/4096;
	//根据位置，透明度等，缩放度画出当前帧。
	if(cur_frame_num<=h_frame)
		{
			for(i=0;i<row;i++)
				{
						for(j=0;j<column;j++)
					{     
						if((PNULL!=(*(cur_page_info->icon_info+i*column+j)).icon_buf)&&(pre_thumb_array[i][j].x<bg_window_size.w)&&(pre_thumb_array[i][j].x+shrik_wid)>0)
							{	
								window_offset.x=pre_thumb_array[i][j].x;
								window_offset.y=pre_thumb_array[i][j].y;
								S2d_Type_4_in_one_fast(bg_window_size, (cur_page_info->icon_info+i*column+j)->icon_size, window_offset,img_offset, 0, (*(cur_page_info->icon_info+i*column+j)).icon_buf , calc_data_info->input_type,bg_window_buf,calc_data_info->output_type, pre_z, pre_alpha,calc_point_buf,calc_data_info->param);
								
							}
					}
				}
		}
	//根据位置，透明度等，缩放度画出下一帧。
	shrik_wid=bg_window_size.w*next_z/4096;
	for(i=0;i<row;i++)
		{
				for(j=0;j<column;j++)
			{    
				
				if((PNULL!=(*(next_page_info->icon_info+i*column+j)).icon_buf)&&(next_thumb_array[i][j].x+shrik_wid>0)&&(next_thumb_array[i][j].x<bg_window_size.w))
					{	
						window_offset.x=next_thumb_array[i][j].x;
						window_offset.y=next_thumb_array[i][j].y;
						Make_SmallItems_Shining(cur_frame_num,total_frame_num,direction,bg_window_size, (next_page_info->icon_info+i*column+j)->icon_size, window_offset,img_offset, 0, (*(next_page_info->icon_info+i*column+j)).icon_buf , bg_window_buf, next_z, next_alpha,calc_point_buf,calc_data_info);
						//S2d_4_in_one_fast(bg_window_size, cur_page_info->icon_size, window_offset,img_offset, 0, (*(next_page_info->icon_info+i*column+j)).icon_buf , bg_window_buf, next_z, next_alpha,calc_point_buf);
					}
			}
		}
			
		

    //SCI_FREE(Rota_buff);
	return TRUE;
	
}
#endif
//#ifdef GO_CLASSIC_MAIN_MENU_SUPPORT
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
        //s3d_src_img.src_type=S3D_DATA_ARGB888;
        /*#ifdef GO_CALC_TEST
                        s3d_src_img.src_type=S3D_DATA_PAF_RGB888;
        s3d_img.color_type=S3D_DATA_PAF_RGB888;
                    #else
                        s3d_src_img.src_type=S3D_DATA_ARGB888;
                    s3d_img.color_type=S3D_DATA_RGB565;
                    #endif*/
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
PUBLIC BOOLEAN MMIMENU_Go_Effect_Classic_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
	int i=0,j=0,row=0,column=0,dv=0,n=0,temp=0;
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

	row=cur_page_info->total_row_num;
	column=cur_page_info->total_col_num;

    
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
	for(i=0;i<row;i++)
		{	
				for(j=0;j<column;j++)
			{    	temp=i*column+j;
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
		}
	
	return TRUE;
}
//#endif

#ifdef GO_FLASH_MAIN_MENU_SUPPORT
/*****************************************************************************/
//  Description : Classic 特效执行函数，包含是否打光特效
//  Global resource dependence : 
//  Author: BENNY
//  Note:
/*****************************************************************************/

LOCAL void Make_Flash_Shining(uint16 cur_frame,//当前帧
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
        int32 radius = 140;
        int32 delta_xy = 160,x_margin = 10;
        int32 x_st = -80, x_ed = win_size.w+80;
        int32 frame = 0 ;
		int32 add_frame = 2;
		

        frame = cur_frame;

        if(0==total_frame)
            return;
        if(GO_MOVE_UP== direction)
        {
            x_st = (-1)*radius/2-x_margin;
            x_ed = win_size.w+radius/2+delta_xy;           
        }
        else if(GO_MOVE_DOWN== direction)
        {
            x_st = win_size.w+radius/2+delta_xy+x_margin;
            x_ed = (-1)*radius/2;    
        }
        else
        {
            return;
        }

        point1.x = (x_ed-x_st)*frame/(total_frame+add_frame)+x_st;
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
        //s3d_src_img.src_type=S3D_DATA_ARGB888;
        /*#ifdef GO_CALC_TEST
                        s3d_src_img.src_type=S3D_DATA_PAF_RGB888;
                        s3d_img.color_type=S3D_DATA_PAF_RGB888;
                    #else
                        s3d_src_img.src_type=S3D_DATA_ARGB888;
                    s3d_img.color_type=S3D_DATA_RGB565;
                    #endif*/
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
            S3D_AxisMidRotation_Img_shining( &(s3d_geometry),&(s3d_src_img),&(s3d_img),255,&ls_para,calc_data_info->param);
        }
        else
        {
            S3D_AxisMidRotation_Img( &(s3d_geometry),&(s3d_src_img),&(s3d_img),255,calc_data_info->param);
        }
        
}

PUBLIC BOOLEAN MMIMENU_Go_Effect_Flash_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
	int i=0,j=0,row=0,column=0,dv=0,n=0,temp=0;

	FIX16_POINT_T window_offset={0};
    IMGREF_SIZE_T new_window_size={0};
	GO_ROTA_DATA_T result = {0};
    
	if(PNULL == cur_page_info
	||PNULL == next_page_info
	||PNULL == bg_window_buf
	||PNULL == calc_point_buf
	||(cur_frame_num > total_frame_num))
	{
		return SCI_FALSE;
	}

	row=cur_page_info->total_row_num;
	column=cur_page_info->total_col_num;

    new_window_size.h=bg_window_size.h;//-40;
    new_window_size.w=bg_window_size.w;
	//计算dv,dv就是标志上下移动量的一个长度。
	if(0==total_frame_num)
		{
			return SCI_FALSE;
		}
	else
		{	
		      if(GO_MOVE_UP==direction)
			      	{	n=1;
					dv=-new_window_size.h*cur_frame_num/total_frame_num;
			      	}
			else if(GO_MOVE_DOWN==direction)
				{	n=-1;
					dv=new_window_size.h*cur_frame_num/total_frame_num;
				}
		}
	//根据dv画出前后两页的图标，这里用n 
//控制位移的正负，达到控制左右的问题。
	for(i=0;i<row;i++)
		{	
				for(j=0;j<column;j++)
			{    	temp=i*column+j;
				if(NULL!=(cur_page_info->icon_info +temp)->icon_buf)
					{
						window_offset.x=(cur_page_info->icon_info+temp)->icon_offset.x;
						window_offset.y=(cur_page_info->icon_info+temp)->icon_offset.y+dv;
						if((window_offset.y<new_window_size.h)&&(window_offset.y+(cur_page_info->icon_info +temp)->icon_size.h)>0)
							{
								result.axis_x = window_offset.x;
                                result.axis_y = window_offset.y;
                                result.axis_z = 0;
								//Make_Flash_Shining(cur_frame_num,total_frame_num,direction,bg_window_buf,result,(cur_page_info->icon_info +temp)->icon_buf,(cur_page_info->icon_info +temp)->icon_size,bg_window_size,1800,calc_data_info);
								//S2D_Blending(new_window_size, bg_window_buf,(cur_page_info->icon_info +temp)->icon_size,window_offset,(cur_page_info->icon_info +temp)->icon_buf, bg_window_buf);
								S2D_TypeBlending(bg_window_size, bg_window_buf,calc_data_info->output_type, (cur_page_info->icon_info +temp)->icon_size, window_offset, (cur_page_info->icon_info +temp)->icon_buf,calc_data_info->input_type, bg_window_buf,calc_data_info->param);
							}
					}
				if(NULL!=(next_page_info->icon_info +temp)->icon_buf)
					{
						window_offset.x=(next_page_info->icon_info+temp)->icon_offset.x;
                        			window_offset.y=(next_page_info->icon_info+temp)->icon_offset.y+dv+n*new_window_size.h;
                       			 if((window_offset.y<new_window_size.h)&&(window_offset.y+(next_page_info->icon_info+temp)->icon_size.h)>0)
							{
								result.axis_x = window_offset.x;
                                result.axis_y = window_offset.y;
                                result.axis_z = 0;
								//Make_Flash_Shining(cur_frame_num,total_frame_num,direction,bg_window_buf,result,(next_page_info->icon_info +temp)->icon_buf,(next_page_info->icon_info+temp)->icon_size,bg_window_size,1800,calc_data_info);
								//S2D_Blending(new_window_size, bg_window_buf,(next_page_info->icon_info+temp)->icon_size,window_offset,(next_page_info->icon_info +temp)->icon_buf, bg_window_buf);
								S2D_TypeBlending(bg_window_size, bg_window_buf,calc_data_info->output_type, (next_page_info->icon_info+temp)->icon_size, window_offset, (next_page_info->icon_info +temp)->icon_buf,calc_data_info->input_type, bg_window_buf,calc_data_info->param);
							}
					}
			}
		}
	return SCI_TRUE;
}

#endif
#ifdef GO_CONSTICK_MAIN_MENU_SUPPORT
PUBLIC BOOLEAN MMIMENU_Go_Effect_ConStick_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
	
	int i=0,j=0,symbol=0,step_frame=0,icon_wid=0;
	int row=0,column=0,gather_frame=0,d_wid=0,go_distance=0;
	FIX16_POINT_T window_offset={0};
	
	//用于存放聚集的图标的坐标，固定值，平移过程中不发生变化。
	IMGREF_POINT_T  pre_thumb_array[6][6]={0};
	IMGREF_POINT_T  next_thumb_array[6][6]={0};

	if(PNULL == cur_page_info
		||PNULL == next_page_info
		||PNULL == bg_window_buf
		||PNULL == calc_point_buf
		||(cur_frame_num > total_frame_num))
	{
		return SCI_FALSE;
	}

	gather_frame=total_frame_num/4;
	if(0==gather_frame)
		{
			return SCI_FALSE;
		}
	//symbol控制左右移动时的，移动量的符号，进而实现左右移动效果。
	if(GO_MOVE_LEFT==direction)
		{
			
			symbol=-1;
		}
	else if(GO_MOVE_RIGHT==direction)
		{
			
			symbol=1;
		}
	
	d_wid=cur_page_info->icon_info[0].icon_size.w/4;	//jinx_temp
	row=cur_page_info->total_row_num;
	column=cur_page_info->total_col_num;
	
	//向左或向右滑动时，图标应该聚集的位置放在pre_thumb_array和next_thumb_array数组内。
	for(i=0;i<row;i++)
	{	
			for(j=0;j<column;j++)
		{    
		    icon_wid=(cur_page_info->icon_info+i*column)->icon_size.w;
			if(GO_MOVE_LEFT==direction)
				{
					pre_thumb_array[i][j].x=(cur_page_info->icon_info+i*column)->icon_offset.x+j*d_wid;
					pre_thumb_array[i][j].y=(cur_page_info->icon_info+i*column)->icon_offset.y;
					next_thumb_array[i][j].x=(next_page_info->icon_info+i*column)->icon_offset.x+j*d_wid+bg_window_size.w;
					next_thumb_array[i][j].y=(next_page_info->icon_info+i*column)->icon_offset.y;
				}
			else if(GO_MOVE_RIGHT==direction)
				{
					pre_thumb_array[i][j].x=(cur_page_info->icon_info+(column-1))->icon_offset.x-(column-1-j)*d_wid;
					pre_thumb_array[i][j].y=(cur_page_info->icon_info+i*column)->icon_offset.y;
					next_thumb_array[i][j].x=(next_page_info->icon_info+(column-1))->icon_offset.x-(column-1-j)*d_wid-bg_window_size.w;
					next_thumb_array[i][j].y=(next_page_info->icon_info+i*column)->icon_offset.y;
				}

			
			
		}
	}
	//go_distance是滑动过程中华东的距离
	go_distance=(d_wid*(column-1)+cur_page_info->icon_info[0].icon_size.w)/2+cur_page_info->icon_info->icon_offset.x+bg_window_size.w/2;
	if(cur_frame_num<=gather_frame)
		{	//聚集过程。
			for(i=0;i<row;i++)
				{
						for(j=0;j<column;j++)
					{    
						if(PNULL!=(*(cur_page_info->icon_info+i*column+j)).icon_buf)
							{	
								window_offset.x=(cur_page_info->icon_info+i*column+j)->icon_offset.x+(pre_thumb_array[i][j].x-(cur_page_info->icon_info+i*column+j)->icon_offset.x)*cur_frame_num/gather_frame;
								window_offset.y=(cur_page_info->icon_info +i*column+j)->icon_offset.y;
                                S2D_TypeBlending(bg_window_size, bg_window_buf,calc_data_info->output_type,(cur_page_info->icon_info+i*column+j)->icon_size,window_offset,(cur_page_info->icon_info +i*column+j)->icon_buf,calc_data_info->input_type, bg_window_buf,calc_data_info->param);
							}
					}
				}
		}
	else if((cur_frame_num>gather_frame)&&(cur_frame_num<=(total_frame_num-gather_frame)))
		{	//聚集完成，滑动过程。
			if(0==(total_frame_num-2*gather_frame))
				{
					return SCI_FALSE;
				}
				
			go_distance=symbol*go_distance*(cur_frame_num-gather_frame)/(total_frame_num-2*gather_frame);
			for(i=0;i<row;i++)
				{
						for(j=0;j<column;j++)
					{    
						icon_wid=(cur_page_info->icon_info+i*column+j)->icon_size.w;
						window_offset.x=pre_thumb_array[i][j].x+go_distance;
						window_offset.y=pre_thumb_array[i][j].y;
						if((PNULL!=(*(cur_page_info->icon_info+i*column+j)).icon_buf)&&((window_offset.x<bg_window_size.w)||((window_offset.x+icon_wid)>0)))
					    {
                                S2D_TypeBlending(bg_window_size, bg_window_buf,calc_data_info->output_type,(cur_page_info->icon_info+i*column+j)->icon_size,window_offset,(cur_page_info->icon_info +i*column+j)->icon_buf,calc_data_info->input_type, bg_window_buf,calc_data_info->param);
							}
						
								window_offset.x=next_thumb_array[i][j].x+go_distance;
								window_offset.y=next_thumb_array[i][j].y;
								if((PNULL!=(*(next_page_info->icon_info+i*column+j)).icon_buf)&&((window_offset.x<bg_window_size.w)||((window_offset.x+icon_wid)>0)))
							{	
                                S2D_TypeBlending(bg_window_size, bg_window_buf,calc_data_info->output_type,(next_page_info->icon_info+i*column+j)->icon_size,window_offset,(next_page_info->icon_info +i*column+j)->icon_buf,calc_data_info->input_type, bg_window_buf,calc_data_info->param);
							}
					}
				}
			
		}
	else if((cur_frame_num>(total_frame_num-gather_frame))&&(cur_frame_num<=total_frame_num))
		{	//散开的过程。
			step_frame=cur_frame_num-(total_frame_num-gather_frame);
			go_distance=symbol*go_distance;
			for(i=0;i<row;i++)
				{
						for(j=0;j<column;j++)
					{    
						if(PNULL!=(*(next_page_info->icon_info+i*column+j)).icon_buf)
							{	
								window_offset.x=next_thumb_array[i][j].x+go_distance+((next_page_info->icon_info+i*column+j)->icon_offset.x-(next_thumb_array[i][j].x+go_distance))*step_frame/(gather_frame);
								window_offset.y=(next_page_info->icon_info +i*column+j)->icon_offset.y;
								S2D_TypeBlending(bg_window_size, bg_window_buf,calc_data_info->output_type,(next_page_info->icon_info+i*column+j)->icon_size,window_offset,(next_page_info->icon_info +i*column+j)->icon_buf,calc_data_info->input_type,bg_window_buf,calc_data_info->param);
							}
					}
				}
			
		}
		
	return SCI_TRUE;
}
#endif
#ifdef GO_SYMMETRY_MAIN_MENU_SUPPORT
#define ALPHAWIDTH 40
/*****************************************************************************/
//  Description : 当前页的图标裁剪函数
//  Global resource dependence : 
//  Author: ANSON
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Cur_Page_Cut(uint32 * bg_window_buf, 
								GO_MAINMENU_ICON_INFO_T * icon,
								IMGREF_SIZE_T icon_size,
								IMGREF_SIZE_T window_size,
								int angle,//直线与X轴正方向的夹角.
								int x_zero,//直线与X轴的节距。
								GO_CALC_DATA_INFO_T* calc_data_info
								)
{	
	int i=0,result_one=0,result_two=0,StartPointx = 0,line_x=0,icon_y=0;
	int32 sin_Data=0,cos_Data=0,cot_Data=0,EndPointx = 0,cmp_distance=0;
	FIX16_POINT_T window_offset={0};
	int icon_bottom=0,icon_top=0,icon_left=0,icon_right=0;
	register uint32 temp1,temp2,temp3,temp4,alpha;
	uint32 *pOut=PNULL;
	uint32 *pSrc=PNULL;
	
	if(PNULL==icon->icon_buf)
		{
			return FALSE;
		}
	if(0==s2d_cos_new(angle))
		{	
			return FALSE; 
		}
	//for语句经常将用到的cos,sin,cot值。
	sin_Data=s2d_sin_new(angle);
	cos_Data=s2d_cos_new(angle);
	
		
	result_one=s2d_sin_new(angle)*(icon->icon_offset.x+icon_size.w-x_zero)/s2d_cos_new(angle)-(icon->icon_offset.y);
	
	window_offset.x=icon->icon_offset.x;
	window_offset.y=icon->icon_offset.y;
	//如果图标的右下角点带入直线方程小于零，说明这个图片未被直线扫过。
	if(result_one<=0)
		{
            S2D_TypeBlending(window_size, bg_window_buf,calc_data_info->output_type,icon_size,window_offset,icon->icon_buf,calc_data_info->input_type, bg_window_buf,calc_data_info->param);
			return TRUE;
		}
	//
	if(0==s2d_sin_new(angle))
		{	
			return FALSE; 
		}
	cot_Data=(s2d_cos_new(angle)<<8)/s2d_sin_new(angle);
	//确定图标的左右上下边界在，屏幕坐标系下的坐标。
	icon_bottom=window_offset.y;
	icon_top=window_offset.y+icon_size.h;
	icon_left=window_offset.x;
	icon_right=window_offset.x+icon_size.w;
	EndPointx=icon_right;
	StartPointx=icon_left;
	//打alpha的区间。ALPHAWIDTH是像素点与 直线的垂直距离。
	cmp_distance=(ALPHAWIDTH<<16)/sin_Data;
	//图标右下角带入直线方程的值为正，说明，之歌图标被直线全部扫过或者部分扫过。
	if(result_one>0)
		{	//将图标的左上角点带入，直线方程。
			result_two=s2d_sin_new(angle)*(icon->icon_offset.x-x_zero)/s2d_cos_new(angle)-(icon->icon_offset.y+icon_size.h);
			//图标的左上角带入直线方程值小于零说明，图标属于被对称线切割的状态。
			if(result_two<0)
				{	
					for(icon_y=icon_bottom;icon_y<icon_top;icon_y++)
						{	//对称线上y值对应的X值。
							line_x=((icon_y*cot_Data)>>8)+x_zero;
							StartPointx=icon_left;
							EndPointx=MIN(line_x, icon_right);
							//找到原像，进行图像填充.
							if(StartPointx<EndPointx)
								{	pOut=&(bg_window_buf[icon_y*window_size.w+StartPointx]);
									pSrc=&(icon->icon_buf[(icon_y-icon_bottom)*icon_size.w+(StartPointx-icon_left)]);
									
									for(i=StartPointx;i<EndPointx;i++)
										{	temp1= * pSrc;
											alpha = ((temp1>>27))&0x1f;
											if((EndPointx-i)<cmp_distance)
												{
													alpha=((alpha*(uint32)((EndPointx-i)*sin_Data))>>16)/ALPHAWIDTH;
												}
											if( alpha >= 31)
											{
												temp2 = ((temp1&0x000000f8)>>3)|((temp1&0x0000fc00)>>5)|((temp1&0x00f80000)>>8);
												*pOut = temp2;
											}
											else
											{
												temp2 = (((temp1&0x000000f8)>>3)|((temp1&0x00f80000)>>8))*alpha;
												temp3 = ((temp1&0x0000fc00)>>5)*alpha;
												alpha = 32 - alpha;
												temp1 = *pOut;
												temp4 = (temp1&0xf81f)*alpha;
												temp1 = (temp1&0x07e0)*alpha;
												
												temp4 = (temp4+temp2)&(0xf81f<<5);
												temp1 = (temp1+temp3)&(0x07e0<<5);
												*pOut = (temp1 + temp4)>>5;
											}
											
											pSrc++;
											pOut++;
										}
									
						             }
						}
					return TRUE;
				}
		}
	
	return FALSE; 
}
/*****************************************************************************/
//  Description : 下一页的图标裁剪函数
//  Global resource dependence : 
//  Author: ANSON
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Next_Page_Cut(uint32 * bg_window_buf, GO_MAINMENU_ICON_INFO_T * icon,IMGREF_SIZE_T icon_size,IMGREF_SIZE_T window_size,int angle,int x_zero,GO_CALC_DATA_INFO_T* calc_data_info)
{	//注释部分与CUR_Page_Cut 雷同 不加赘述.
	int i=0,result_one=0,result_two=0,StartPointx = 0,EndPointx = 0,line_x=0,icon_y=0;
	int icon_bottom=0,icon_top=0,icon_left=0,icon_right=0;
	int32 sin_Data=0,cos_Data=0,cot_Data=0,cmp_distance=0;
	register uint32 temp1,temp2,temp3,temp4,alpha;

	FIX16_POINT_T window_offset={0};
	uint32 *pOut=PNULL;
	uint32 *pSrc=PNULL;
	if((PNULL==icon->icon_buf))
		{
			return FALSE;
		}
	if(0==s2d_sin_new(angle))
		{	
			return FALSE; 
		}
	
	window_offset.x=icon->icon_offset.x;
	window_offset.y=icon->icon_offset.y;
	
	
	if(0==s2d_cos_new(angle))
	{	
        S2D_TypeBlending(window_size, bg_window_buf,calc_data_info->output_type,icon_size,window_offset,icon->icon_buf,calc_data_info->input_type, bg_window_buf,calc_data_info->param);
       
		return TRUE;
			
	}
	result_one=s2d_sin_new(angle)*(icon->icon_offset.x-x_zero)/s2d_cos_new(angle)-(icon->icon_offset.y+icon_size.h);
	
	if(result_one>=0)
		{	
            S2D_TypeBlending(window_size, bg_window_buf,calc_data_info->output_type,icon_size,window_offset,icon->icon_buf,calc_data_info->input_type, bg_window_buf,calc_data_info->param);
            
			return TRUE;
		}
	//for语句经常将用到的cos,sin,cot值。
	sin_Data=s2d_sin_new(angle);
	cos_Data=s2d_cos_new(angle);
	cot_Data=(s2d_cos_new(angle)<<8)/s2d_sin_new(angle);
	
	icon_bottom=window_offset.y;
	icon_top=window_offset.y+icon_size.h;
	icon_left=window_offset.x;
	icon_right=window_offset.x+icon_size.w;
	EndPointx=icon_right;
	StartPointx=icon_left;
	cmp_distance=(ALPHAWIDTH<<16)/sin_Data;
	if(result_one<0)
		{
			result_two=sin_Data*(icon->icon_offset.x+icon_size.w-x_zero)/cos_Data-(icon->icon_offset.y);
			if(result_two>0)
				{	
					for(icon_y=icon_bottom;icon_y<icon_top;icon_y++)
						{
							line_x=((icon_y*cot_Data)>>8)+x_zero;
							StartPointx=MAX(line_x, icon_left);
							if(StartPointx<EndPointx)
								{	pOut=&(bg_window_buf[icon_y*window_size.w+StartPointx]);
									pSrc=&(icon->icon_buf[(icon_y-icon_bottom)*icon_size.w+(StartPointx-icon_left)]);
									
									for(i=StartPointx;i<EndPointx;i++)
										{	temp1= * pSrc;
											alpha = ((temp1>>27))&0x1f;
											if((i-StartPointx)<cmp_distance)
											{
												alpha=alpha*(((i-StartPointx)*sin_Data)>>16)/ALPHAWIDTH;
											}
											if( alpha >= 31)
											{
												temp2 = ((temp1&0x000000f8)>>3)|((temp1&0x0000fc00)>>5)|((temp1&0x00f80000)>>8);
												*pOut = temp2;
											}
											else
											{
												temp2 = (((temp1&0x000000f8)>>3)|((temp1&0x00f80000)>>8))*alpha;
												temp3 = ((temp1&0x0000fc00)>>5)*alpha;
												alpha = 32 - alpha;
												temp1 = *pOut;
												temp4 = (temp1&0xf81f)*alpha;
												temp1 = (temp1&0x07e0)*alpha;
												
												temp4 = (temp4+temp2)&(0xf81f<<5);
												temp1 = (temp1+temp3)&(0x07e0<<5);
												*pOut = (temp1 + temp4)>>5;
											}
											
											pSrc++;
											pOut++;
										}
									
						             }
						}
					return TRUE;
				}
		}
	
	 return FALSE;      
	
	
}
/*****************************************************************************/
//  Description : 判断图标对称反转后是否部分或全部落在屏幕内落在屏幕内
//                   值为真反之为假
//  Global resource dependence : 
//  Author: ANSON
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN Get_Range(GO_MAINMENU_ICON_INFO_T * icon,IMGREF_SIZE_T icon_size,IMGREF_SIZE_T window_size,int angle,int x_zero)
{	
	
	int result_one=0,ro_angle=0;
      IMGREF_POINT_T sym_p={0},rota_p={0};
	BOOLEAN is_turned_over=FALSE;
	BOOLEAN is_inside=FALSE;

	if(PNULL==icon->icon_buf)
		{
			return FALSE;
		}
	if(0!=s2d_cos_new(angle))
	{	//图标右下角点坐标带入直线方程.
		result_one=s2d_sin_new(angle)*(icon->icon_offset.x+icon_size.w-x_zero)/s2d_cos_new(angle)-icon->icon_offset.y;
	}
	else
	{
		is_turned_over=TRUE;
	}
	if(result_one>0)
	{is_turned_over=TRUE;}
	else
	{	return FALSE;}
	//判断图标是否与对称线相交。
	 result_one=s2d_sin_new(angle)*(icon->icon_offset.x-x_zero)/s2d_cos_new(angle)-(icon->icon_offset.y+icon_size.h);
	if(result_one<=0)
		{
			return TRUE;
		}
	else
		{
			//如果不相交就查看，对称后离对称轴最近的两点是否在屏幕区域外

			ro_angle=-(1800-2*angle);
			rota_p.x=((s2d_cos_new(ro_angle)*(2*x_zero-icon->icon_offset.x-x_zero)-s2d_sin_new(ro_angle)*(icon->icon_offset.y+icon_size.h))>>16)+x_zero;
			rota_p.y=((s2d_sin_new(ro_angle)*(2*x_zero-icon->icon_offset.x-x_zero)+s2d_cos_new(ro_angle)*(icon->icon_offset.y+icon_size.h))>>16);
			sym_p.x=((s2d_cos_new(ro_angle)*(2*x_zero-icon->icon_offset.x-x_zero)-s2d_sin_new(ro_angle)*(icon->icon_offset.y))>>16)+x_zero;
			sym_p.y=((s2d_sin_new(ro_angle)*(2*x_zero-icon->icon_offset.x-x_zero)+s2d_cos_new(ro_angle)*(icon->icon_offset.y))>>16);
			
			if(((rota_p.x>0)&&(rota_p.x<window_size.w)&&(rota_p.y<window_size.h)&&(rota_p.y>0))||((sym_p.x>0)&&(sym_p.x<window_size.w)&&(sym_p.y<window_size.h)&&(sym_p.y>0)))
				{
					is_inside=TRUE;
				}
			
			if(is_inside)
				{
					return TRUE;
				}
			else
				{
					return FALSE;
				}
		}
	
}

PUBLIC BOOLEAN MMIMENU_Go_Effect_Symmetry_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
	int p_row=0,p_column=0,n_row=0,n_column=0,i=0,j=0,x_zero=0,angle=0;

	GO_MAINMENU_ICON_INFO_T * cur_icon_temp=PNULL; 
	GO_MAINMENU_ICON_INFO_T * next_icon_temp=PNULL;
	IMGREF_RECT_T icon_rect={0};

	//当前页的与下一页的行数列数赋值.
	p_row=cur_page_info->total_row_num;
	p_column=cur_page_info->total_col_num;
	n_row=next_page_info->total_row_num;
	n_column=next_page_info->total_col_num;

	//除法保护.
	if(0==total_frame_num)
		{
			return FALSE;
		}
	//根据方向设置对称线和页面的值.
	if(GO_MOVE_LEFT==direction)
		{	
			x_zero =bg_window_size.w*(total_frame_num-cur_frame_num)/total_frame_num;
			angle   =300+600*cur_frame_num/total_frame_num;
			cur_icon_temp=cur_page_info->icon_info;
			next_icon_temp=next_page_info->icon_info;
			
		}
	else if(GO_MOVE_RIGHT==direction)
		{
			x_zero=bg_window_size.w*cur_frame_num/total_frame_num;
			angle=300+600*(total_frame_num-cur_frame_num)/total_frame_num;
			cur_icon_temp=next_page_info->icon_info;
			next_icon_temp=cur_page_info->icon_info;
			
		}
	else
	    {
            return FALSE;
	    }
	for(i=0;i<p_row;i++)
		{
			for(j=0;j<p_column;j++)
				{	
					//对前一页的图标进行裁剪。
					Cur_Page_Cut(bg_window_buf,cur_icon_temp+i*p_column+j, (cur_icon_temp+i*p_column+j)->icon_size, bg_window_size,angle, x_zero,calc_data_info);
					
				}
		}
	for(i=0;i<p_row;i++)
		{
			for(j=0;j<p_column;j++)
				{	
					//找出翻过来后进入屏幕内的图标。
					if(Get_Range(cur_icon_temp+i*p_column+j, (cur_icon_temp+i*p_column+j)->icon_size, bg_window_size,angle, x_zero))
						{	
						    icon_rect.w=(cur_icon_temp+i*p_column+j)->icon_size.w;
	                        icon_rect.h=(cur_icon_temp+i*p_column+j)->icon_size.h;
							icon_rect.x=(cur_icon_temp+i*p_column+j)->icon_offset.x;
							icon_rect.y=(cur_icon_temp+i*p_column+j)->icon_offset.y;
							//运用对称函数对符合要求的图标进行对称。
							S2d_rotation_Symmetry(bg_window_size, 
													bg_window_buf, 
													(cur_icon_temp+i*p_column+j)->icon_buf,
													icon_rect,
													angle, 
													x_zero,
													calc_point_buf);
						
						}
					
				}
		}
	for(i=0;i<n_row;i++)
		{
			for(j=0;j<n_column;j++)
				{	
					//对后一页的图标进行裁剪。
					Next_Page_Cut(bg_window_buf,next_icon_temp+i*n_column+j, (next_icon_temp+i*n_column+j)->icon_size, bg_window_size,angle, x_zero,calc_data_info);
					
				}
		}
	return TRUE;
}
#endif

#ifdef GO_BENZITEM_MAIN_MENU_SUPPORT
typedef struct 
{     int               rotation_angle;
	BOOLEAN       is_right;
	int16 		angle;
	int16		radius;	
}BENZ_INFO_T;
PUBLIC BOOLEAN	BenzItem_Blending(BENZ_INFO_T * items_ptr,
										GO_MAINMENU_PAGE_INFO_T *page_info,
										IMGREF_SIZE_T bg_window_size,
										IMGREF_POINT_T cur_arms_center,
										uint32* bg_window_buf,
										BOOLEAN Is_assemble,
										int icons_offset,
										int m_angle,
										int c_frame,
										int step_frame,
									
										FIX16_POINT_T* calc_point_buf,
										GO_CALC_DATA_INFO_T* calc_data_info)
{	
	int cur_angle=0,cur_radius=0,i=0,half_wid=0,half_height=0,ultra_x=0,ultra_y=0;

	IMGREF_POINT_T window_offset={0};
	IMGREF_POINT_T img_offset={0};


	if(0==step_frame)
		{
			return SCI_FALSE;
		}
	for(i=0;i<page_info->icon_num;i++)
			{	
				if(PNULL==(page_info->icon_info+i)->icon_buf )
					{
						return SCI_FALSE;
					}
                half_wid=(page_info->icon_info+i)->icon_size.w/2;
            	half_height=(page_info->icon_info+i)->icon_size.h/2;
            	img_offset.x=half_wid;
            	img_offset.y=half_height;
				cur_angle=items_ptr[i].angle+m_angle;
				cur_radius=items_ptr[i].radius;
				ultra_x=((cur_radius*s2d_cos_new(cur_angle))>>16)+cur_arms_center.x;
				ultra_y=((cur_radius*s2d_sin_new(cur_angle))>>16)+cur_arms_center.y; 
			

				//把极坐标改成直角坐标系上的坐标。
				if(Is_assemble)
					{
						window_offset.x=(page_info->icon_info+i)->icon_offset.x+icons_offset+half_wid+(ultra_x-(page_info->icon_info+i)->icon_offset.x-half_wid-icons_offset)*c_frame/step_frame;
						window_offset.y=(page_info->icon_info+i)->icon_offset.y+half_height+(ultra_y-(page_info->icon_info+i)->icon_offset.y-half_height)*c_frame/step_frame;
						cur_angle=(items_ptr[i].rotation_angle+m_angle)*c_frame/step_frame;
					}
				else
					{	
						window_offset.x=(page_info->icon_info+i)->icon_offset.x+icons_offset+half_wid+(ultra_x-(page_info->icon_info+i)->icon_offset.x-half_wid-icons_offset)*(step_frame-c_frame)/step_frame;
						window_offset.y=(page_info->icon_info+i)->icon_offset.y+half_height+(ultra_y-(page_info->icon_info+i)->icon_offset.y-half_height)*(step_frame-c_frame)/step_frame;
						cur_angle=(items_ptr[i].rotation_angle+m_angle)*(step_frame-c_frame)/step_frame;
					}				
                                
                S2d_Type_4_in_one_fast(bg_window_size, 
					(page_info->icon_info+i)->icon_size, 
					window_offset,
					img_offset, 
					cur_angle,
					(page_info->icon_info+i)->icon_buf , 
					calc_data_info->input_type,
					bg_window_buf,
					calc_data_info->output_type, 
					4096, 
					255,
					calc_point_buf,
					calc_data_info->param);
			}
	return SCI_TRUE;
}
PUBLIC BOOLEAN MMIMENU_Go_Effect_BenzItem_Type_Calc(GO_MAINMENU_PAGE_INFO_T *cur_page_info,//当前页信息
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
	int i=0,radius=80,arm_num=0,arm_location=0,icon_per_arm=0,d_icon=0;
	int gather_frame=0,step_frame=0,total_step_frame=0,move_anlge=0;
	int max_angle=0,symbol=0,d_angle=450;
	int gather_location=0;
	IMGREF_POINT_T cur_arms_center={0};
	IMGREF_POINT_T next_arms_center={0};
	uint16 fix_angle=450;
	BENZ_INFO_T * cur_items_ptr=PNULL;
	BENZ_INFO_T * next_items_ptr=PNULL;
	
	
	if(PNULL == cur_page_info
		||PNULL == next_page_info
		||PNULL == bg_window_buf
		||PNULL == calc_point_buf
		||(cur_frame_num > total_frame_num))
	{
		return SCI_FALSE;
	}
	gather_frame=total_frame_num/4;
	if(0==gather_frame)
		{
			return SCI_FALSE;
		}
	gather_location=bg_window_size.w*cur_frame_num/total_frame_num;
	
	//当前页的图标的极坐标数组
	cur_items_ptr=SCI_ALLOCA(sizeof(BENZ_INFO_T) *cur_page_info->icon_num);
	if(PNULL==cur_items_ptr)
		{
		 	return SCI_FALSE;
		}
	//下一页的图标的极坐标数组
	next_items_ptr=SCI_ALLOCA(sizeof(BENZ_INFO_T)*next_page_info->icon_num);
	if(PNULL==next_items_ptr)
		{
		 	return SCI_FALSE;
		}
	
	//对左右聚集点赋值。
	
	max_angle=2400;
	
	//对中心赋初值
	cur_arms_center.x=bg_window_size.w/2;
	cur_arms_center.y=bg_window_size.h/2;
	next_arms_center.y=cur_arms_center.y;
	if(GO_MOVE_LEFT==direction)
		{
			
			symbol=-1;
		}
	else if(GO_MOVE_RIGHT==direction)
		{
			
			symbol=1;
		}
	//为当前页各个图标赋极坐标
	icon_per_arm=cur_page_info->icon_num/3;
	d_icon=radius/MAX(icon_per_arm, 1);
	for(i=0;i<cur_page_info->icon_num;i++)
		{	
			arm_location=i/3;
			arm_num=i-3*arm_location;
			cur_items_ptr[i].angle=fix_angle+1200*arm_num;
			cur_items_ptr[i].radius=radius-arm_location*d_icon;
			
			if(0==(arm_location%2))
				{
					cur_items_ptr[i].is_right=TRUE;
					cur_items_ptr[i].rotation_angle=d_angle;
				}
			else
				{
					cur_items_ptr[i].is_right=FALSE;
					cur_items_ptr[i].rotation_angle=2*d_angle;
				}
		}
	//为下一页各个图标赋极坐标
	icon_per_arm=next_page_info->icon_num/3;
	d_icon=radius/MAX(icon_per_arm, 1);
	for(i=0;i<next_page_info->icon_num;i++)
		{	
			arm_location=i/3;
			arm_num=i-3*arm_location;
			next_items_ptr[i].angle=fix_angle+1200*arm_num;
			next_items_ptr[i].radius=radius-arm_location*d_icon;
			next_items_ptr[i].rotation_angle=(arm_location)*d_angle;
		}

	//转轮中心移动。
	//

	//图标聚集为转轮
	if(cur_frame_num<=gather_frame)
		{	
			//计算转轮中心的位置
			cur_arms_center.x=cur_arms_center.x+symbol*bg_window_size.w*cur_frame_num/total_frame_num;
			next_arms_center.x=cur_arms_center.x-symbol*bg_window_size.w;
			//画出当前页
			BenzItem_Blending(cur_items_ptr, 
			            cur_page_info, 
						bg_window_size, 
						cur_arms_center, 
						bg_window_buf, TRUE,
						0,
						move_anlge,
						cur_frame_num,
						gather_frame, 
						calc_point_buf,
						calc_data_info
						);	
			//画出下一页
			BenzItem_Blending(next_items_ptr, 
			            next_page_info, 
						bg_window_size, 
						next_arms_center, 
						bg_window_buf,
						TRUE,
						-symbol*bg_window_size.w,
						move_anlge, 
						cur_frame_num,
						gather_frame, 
						calc_point_buf,
						calc_data_info
						);	
		}
	else if((cur_frame_num>gather_frame)&&(cur_frame_num<=(total_frame_num-gather_frame)))
		{	
			total_step_frame=	total_frame_num-2*gather_frame;
			if(0==(total_step_frame))
				{
					return SCI_FALSE;
				}
			
			step_frame=cur_frame_num-gather_frame;
			//计算转轮中心的位置
			cur_arms_center.x=cur_arms_center.x+symbol*gather_location+symbol*(bg_window_size.w-gather_location)*step_frame/total_step_frame;
			next_arms_center.x=cur_arms_center.x-symbol*bg_window_size.w;
			//计算平移时的角度
			move_anlge=symbol*max_angle*step_frame/total_step_frame;
			//画出当前页
			BenzItem_Blending(cur_items_ptr, cur_page_info, 
						bg_window_size, cur_arms_center, 
 						bg_window_buf, TRUE,
						0,
						move_anlge,1,
						1, calc_point_buf,calc_data_info
						);	
			//画出下一页
			BenzItem_Blending(next_items_ptr, next_page_info, 
						bg_window_size, next_arms_center, 
						bg_window_buf,TRUE,
						0,
						move_anlge, 1,
						1, calc_point_buf,calc_data_info
						);	
			
		}
	else if((cur_frame_num>(total_frame_num-gather_frame))&&(cur_frame_num<=total_frame_num))
		{	
			step_frame=cur_frame_num-(total_frame_num-gather_frame);
				//计算转轮中心的位置
			cur_arms_center.x=cur_arms_center.x+symbol*bg_window_size.w;
			next_arms_center.x=cur_arms_center.x-symbol*bg_window_size.w;
			//画出当前页
			BenzItem_Blending(cur_items_ptr, cur_page_info, 
						bg_window_size, cur_arms_center, 
						bg_window_buf, FALSE,
						symbol*bg_window_size.w,
						max_angle,step_frame,
						gather_frame, calc_point_buf,calc_data_info
						);	
			//画出下一页
			BenzItem_Blending(next_items_ptr, next_page_info, 
						bg_window_size, next_arms_center, 
						bg_window_buf,FALSE,
						0,
						max_angle, step_frame,
						gather_frame, calc_point_buf,calc_data_info);	
			
		}
	
	SCI_FREE(cur_items_ptr);
	SCI_FREE(next_items_ptr);
	return SCI_TRUE;

}
#endif


