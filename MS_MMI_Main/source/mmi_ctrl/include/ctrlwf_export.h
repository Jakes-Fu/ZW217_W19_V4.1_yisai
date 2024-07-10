/*****************************************************************************
** File Name:      ctrlwf_export.h                                           *
** Author:                                                                   *
** Date:           08/13/2021                                                *
** Copyright:      2012 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe watch face export API       *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 08/2021      yuming.yang          Create                                  *
******************************************************************************/

#ifndef _CTRLWF_EXPORT_H_
#define _CTRLWF_EXPORT_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#define MAX_IMAGE_COUNT (20)



/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
typedef  BOOLEAN (*SensorData_CallBack)(uint32* p_value);

typedef enum
{
    WATCHFACE_TYPE_DIGITAL_TIME_BASE = 100,
    WATCHFACE_TYPE_DIGITAL_TIME_HOURMIN,
    WATCHFACE_TYPE_DIGITAL_TIME_HOUR,
    WATCHFACE_TYPE_DIGITAL_TIME_MIN,
    WATCHFACE_TYPE_DIGITAL_DATE_BASE = 200,
    WATCHFACE_TYPE_DIGITAL_DATE_YEAR,
    WATCHFACE_TYPE_DIGITAL_DATE_MONTH,
    WATCHFACE_TYPE_DIGITAL_DATE_DAY,
    WATCHFACE_TYPE_DIGITAL_DATE_MD,
    WATCHFACE_TYPE_DIGITAL_DATE_YM,
    WATCHFACE_TYPE_DIGITAL_DATE_YMD,
    WATCHFACE_TYPE_MULTI_NUM_BASE = 300,
    WATCHFACE_TYPE_MULTI_NUM_STEPS,
    WATCHFACE_TYPE_MULTI_NUM_HEARTRATE,
    WATCHFACE_TYPE_MULTI_NUM_POWER,
    WATCHFACE_TYPE_MAX
}WATCHFACE_ALL_TYPE_E;
typedef struct
{
    float  ratio_x;  //横向比例位置
     float  ratio_y;  //纵向比例位置
}WATCHFACE_RATIO_POSITION_T;

typedef struct
{
    WATCHFACE_RATIO_POSITION_T      center_position;
    char*                           p_image_list[MAX_IMAGE_COUNT+1];
    uint32                          update_interval;
    BOOLEAN                         double_digital;
    char*                           p_image_none;
    char*                           p_image_connector;
    WATCHFACE_ALL_TYPE_E            type;
    float                           scale;
}WATCHFACE_CTRL_PARAM_T;


/*---------------------------------------------------------------------------*/
/*                           FUNCTION DECLARE                                */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : create watch face bg ctrl
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] center_position: 中心坐标
//             [In] update_interval: 刷新间隔
//             [In] p_image_name: 资源路径
//             [In] list_num: 资源数目
//             [In] scale: 缩放比例
//             [In] is_loop: 是否循环
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLWF_Bg_Create(MMI_WIN_ID_T              win_id,
                                            MMI_CTRL_ID_T               ctrl_id,
                                            GUI_POINT_T                 center_point,
                                            uint32                      update_interval,
                                            char*                       p_image_list[],
                                            uint8                       list_num,
                                            float                       scale,
                                            BOOLEAN                     is_loop
                                            );

/*****************************************************************************/
//  Description : destory watch face bg ctrl
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Bg_Destory (MMI_HANDLE_T handle);

/*****************************************************************************/
//  Description : create watch face image ctrl
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] center_position: 中心坐标
//             [In] p_image_name: 资源路径
//             [In] scale: 缩放比例
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLWF_Img_Create(MMI_WIN_ID_T         win_id,
                                               MMI_CTRL_ID_T        ctrl_id,
                                               GUI_POINT_T          center_point,
                                               char*                p_image_name,
                                               float                scale
                                               );

/*****************************************************************************/
//  Description : destory watch face image ctrl
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Img_Destory (MMI_HANDLE_T handle);

/*****************************************************************************/
//  Description : create watch face digital time ctrl
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] center_position: 中心坐标
//             [In] update_interval: 刷新间隔/ms
//             [In] p_image_list[]: 数字资源路径
//             [In] p_image_connector: 连接符资源路径
//             [In] scale: 缩放比例
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLWF_DigitalTime_Create (MMI_WIN_ID_T                    win_id,
                                               MMI_CTRL_ID_T                   ctrl_id,
                                               GUI_POINT_T                     center_point,
                                               uint32                          update_interval,
                                               char*                           p_image_list[],
                                               char*                           p_image_connector,
                                               float                           scale,
                                               WATCHFACE_ALL_TYPE_E            type
                                               );

/*****************************************************************************/
//  Description : destory watch face digital time ctrl
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_DigitalTime_Destory (MMI_HANDLE_T handle);

/*****************************************************************************/
//  Description : create watch face analog time ctrl
//  Parameter: [In] win_id
//             [In] center_position: 中心相对坐标
//             [In] update_interval: 刷新间隔/ms
//             [In] image_hour: 时针资源路径
//             [In] image_minute: 分针资源路径
//             [In] image_second: 秒针资源路径
//             [In] scale: 缩放比例
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLWF_AnalogTime_Create (MMI_WIN_ID_T      win_id,
                                                            MMI_CTRL_ID_T   ctrl_id,
                                                            GUI_POINT_T         center_point,
                                                            uint32              update_interval,
                                                            char*               p_image_hour,
                                                            char*               p_image_minute,
                                                            char*               p_image_second,
                                                            float               scale
                                                            );

/*****************************************************************************/
//  Description : destory watch face analog time ctrl
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_AnalogTime_Destory (MMI_HANDLE_T handle);


/*****************************************************************************/
//  Description : create watch face month ctrl
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] center_position:中心坐标
//             [In] p_image_list[]:数字资源路径
//             [In] scale: 缩放比例
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T  CTRLWF_Month_Create(MMI_WIN_ID_T           win_id,
                                               MMI_CTRL_ID_T              ctrl_id,
                                               GUI_POINT_T                center_point,
                                               char*                      p_image_list[],
                                               float                      scale
                                               );

/*****************************************************************************/
//  Description : destory watch face month ctrl
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Month_Destory (MMI_HANDLE_T handle);

/*****************************************************************************/
//  Description : create watch face multinum ctrl
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] timer_interval:刷新间隔
//             [In] getdata_callback:外部传入数据
//             [In] center_position:中心坐标
//             [In] p_image_list[]:数字资源路径
//             [In] p_image_none:无效数字对应的资源路径
//             [In] scale: 缩放比例
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: shuting.ma
//  Note:注意需要在外面销毁内存
/*****************************************************************************/
PUBLIC MMI_HANDLE_T CTRLWF_MultiNum_Create(MMI_WIN_ID_T              win_id,
                                                     MMI_CTRL_ID_T               ctrl_id,
                                                     uint32                      timer_interval,
                                                     SensorData_CallBack         getdata_callback,
                                                     GUI_POINT_T                 center_point,
                                                     char*                       p_image_list[],
                                                     char*                       p_image_none,
                                                     float                       scale
                                                     );


/*****************************************************************************/
//  Description : destory watch face multinum ctrl
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: shuting.ma
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_MultiNum_Destory (MMI_HANDLE_T handle);


/*****************************************************************************/
//  Description : create watch face week ctrl
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] center_position: 中心坐标
//             [In] p_image_list[]: 周的图片资源路径
//             [In] scale: 缩放比例
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T  CTRLWF_Week_Create (
                                                    MMI_WIN_ID_T                 win_id,
                                                    MMI_CTRL_ID_T                ctrl_id,
                                                    GUI_POINT_T                  center_point,
                                                    char*                        p_image_list[],
                                                    float                        scale
                                                    );

/*****************************************************************************/
//  Description : destory watch face week ctrl
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Week_Destory (MMI_HANDLE_T handle);

/*****************************************************************************/
//  Description : create watch face day ctrl
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] center_position: 中心坐标
//             [In] p_image_list[]: 显示天的图片资源路径
//             [In] scale: 缩放比例
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T  CTRLWF_Day_Create (
                                                MMI_WIN_ID_T                 win_id,
                                                MMI_CTRL_ID_T                ctrl_id,
                                                BOOLEAN                      double_digital,
                                                GUI_POINT_T                  center_point,
                                                char*                        p_image_list[],
                                                float                        scale
                                                );

/*****************************************************************************/
//  Description : destory watch face day ctrl
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Day_Destory (MMI_HANDLE_T handle);

/*****************************************************************************/
//  Description : create watch face digital date ctrl
//  Parameter: [In] win_id
//             [In] ctrl_id
//             [In] center_position: 中心坐标
//             [In] p_image_list[]: 数字图片资源路径
//             [In] p_month_image:汉字“月”或者连接符图片资源路径
//             [In] scale:图片缩放比例
//             [In] type:digital date传入type
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: qi.zhang
//  Note:
/*****************************************************************************/

PUBLIC MMI_HANDLE_T  CTRLWF_DigitalDate_Create (
                                        MMI_WIN_ID_T                   win_id,
                                        MMI_CTRL_ID_T                  ctrl_id,
                                        GUI_POINT_T                    center_point,
                                        char*                          p_image_list[],
                                        char*                          p_image_connector,
                                        float                          scale,
                                        WATCHFACE_ALL_TYPE_E           type
                                        );
/*****************************************************************************/
//  Description : destory watch face digital date ctrl
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_DigitalDate_Destory (MMI_HANDLE_T handle);

/*****************************************************************************/
//  Description : append watch face layer
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Common_AppendLayer(MMI_HANDLE_T handle);

/*****************************************************************************/
//  Description : remove watch face layer
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Common_RemoveLayer (MMI_HANDLE_T handle);
/*****************************************************************************/
//  Description : stop timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:用于lose focus时，停止控件内部timer
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_DigitalTime_StopTimer (MMI_HANDLE_T handle);
/*****************************************************************************/
//  Description : restart timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLWF_DigitalTime_RestartTimer (MMI_HANDLE_T handle);
/*****************************************************************************/
//  Description : stop timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:用于lose focus时，停止控件内部timer
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Bg_StopTimer (MMI_HANDLE_T handle);
/*****************************************************************************/
//  Description : restart timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLWF_Bg_RestartTimer (MMI_HANDLE_T handle);
/*****************************************************************************/
//  Description : stop timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:用于lose focus时，停止控件内timer
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_AnalogTime_StopTimer (MMI_HANDLE_T handle);
/*****************************************************************************/
//  Description : restart timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLWF_AnalogTime_RestartTimer (MMI_HANDLE_T handle);

/*****************************************************************************/
//  Description : stop timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:用于lose focus时，停止控件内部timer
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Month_StopTimer (MMI_HANDLE_T handle);
/*****************************************************************************/
//  Description : restart timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLWF_Month_RestartTimer (MMI_HANDLE_T handle);
/*****************************************************************************/
//  Description : stop timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:用于lose focus时，停止控件内部timer
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_MultiNum_StopTimer (MMI_HANDLE_T handle);
/*****************************************************************************/
//  Description : restart timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLWF_MultiNum_RestartTimer (MMI_HANDLE_T handle);
/*****************************************************************************/
//  Description : stop timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:用于lose focus时，停止控件内部timer
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Week_StopTimer (MMI_HANDLE_T handle);
/*****************************************************************************/
//  Description : restart timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLWF_Week_RestartTimer (MMI_HANDLE_T handle);
/*****************************************************************************/
//  Description : stop timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:用于lose focus时，停止控件内部timer
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_Day_StopTimer (MMI_HANDLE_T handle);
/*****************************************************************************/
//  Description : restart timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLWF_Day_RestartTimer (MMI_HANDLE_T handle);
/*****************************************************************************/
//  Description : stop timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:用于lose focus时，停止控件内部timer
/*****************************************************************************/
PUBLIC BOOLEAN CTRLWF_DigitalDate_StopTimer (MMI_HANDLE_T handle);
/*****************************************************************************/
//  Description : restart timer
//  Parameter: [In] handle
//             [Out] None
//             [Return] None
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC void CTRLWF_DigitalDate_RestartTimer (MMI_HANDLE_T handle);



#endif
