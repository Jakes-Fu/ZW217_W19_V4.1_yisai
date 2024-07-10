/*****************************************************************************
** File Name:       watchface_ctrl_adapter.h                                 *
** Author:          yuming.yang                                              *
** Date:            08/23/2021                                               *
** Copyright:                                                                *
** Description:    This file is used to manage watch face ctrl id            *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                 NAME                  DESCRIPTION                    *
** 08/23/2021       yuming.yang               Create                         *
******************************************************************************/

#ifndef _WATCHFACE_CTRL_ADAPTER_H_
#define _WATCHFACE_CTRL_ADAPTER_H_

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmi_string.h"
#include "ctrlwf_export.h"
#include "mmi_id.h"

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                  *
**---------------------------------------------------------------------------*/

#define WATCHFACE_CTRL_ID_NUM_MAX       (WATCH_FACE_END_CTRL_ID - WATCH_FACE_BEGIN_CTRL_ID)

#define WATCHFACE_Adapter_AppendLayer       CTRLWF_Common_AppendLayer
#define WATCHFACE_Adapter_RemoveLayer       CTRLWF_Common_RemoveLayer

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           FUNCTION DECLARE                                */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : create watch face image
//  Parameter: [In] win_id
//             [In] p_param: 控件绘制需要的所有参数
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T WATCHFACE_Img_Create(MMI_WIN_ID_T                   win_id,
                                                     WATCHFACE_CTRL_PARAM_T*        p_param
                                                       );


/*****************************************************************************/
//  Description : destory watch face image
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_Img_Destory (MMI_HANDLE_T handle);

/*****************************************************************************/
//  Description : create watch face bg
//  Parameter: [In] win_id
//             [In] p_param: 控件绘制需要的所有参数
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T WATCHFACE_Background_Create(
                                                 MMI_WIN_ID_T                    win_id,
                                                 WATCHFACE_CTRL_PARAM_T*         p_param
                                                   );

/*****************************************************************************/
//  Description : destory watch face bg
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_Background_Destory (MMI_HANDLE_T handle);

/*****************************************************************************/
//  Description : create watch face digital time
//  Parameter: [In] win_id
//             [In] p_param: 控件绘制需要的所有参数
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T WATCHFACE_DigitalTime_Create (MMI_WIN_ID_T                    win_id,
                                                               WATCHFACE_CTRL_PARAM_T*          p_param
                                                                 );

/*****************************************************************************/
//  Description : destory watch face digital time
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_DigitalTime_Destory (MMI_HANDLE_T handle);

/*****************************************************************************/
//  Description : create watch face analog time
//  Parameter: [In] win_id
//             [In] p_param: 控件绘制需要的所有参数
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T WATCHFACE_AnalogTime_Create (MMI_WIN_ID_T                    win_id,
                                                                WATCHFACE_CTRL_PARAM_T*         p_param
                                                                );

/*****************************************************************************/
//  Description : destory watch face analog time
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_AnalogTime_Destory (MMI_HANDLE_T handle);


/*****************************************************************************/
//  Description : create watch face day
//  Parameter: [In] win_id
//             [In] p_param: 控件绘制需要的所有参数
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T WATCHFACE_Day_Create (MMI_WIN_ID_T               win_id,
                                                      WATCHFACE_CTRL_PARAM_T*    p_param
                                                      );

/*****************************************************************************/
//  Description : destory watch face day
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_Day_Destory (MMI_HANDLE_T handle);


/*****************************************************************************/
//  Description : create watch face week
//  Parameter: [In] win_id
//             [In] p_param: 控件绘制需要的所有参数
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T WATCHFACE_Week_Create (MMI_WIN_ID_T            win_id,
                                                        WATCHFACE_CTRL_PARAM_T* p_param
                                                        );

/*****************************************************************************/
//  Description : destory watch face week
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_Week_Destory (MMI_HANDLE_T handle);

/*****************************************************************************/
//  Description : create watch face month
//  Parameter: [In] win_id
//             [In] p_param: 控件绘制需要的所有参数
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T WATCHFACE_Month_Create (MMI_WIN_ID_T            win_id,
                                                         WATCHFACE_CTRL_PARAM_T* p_param
                                                         );

/*****************************************************************************/
//  Description : destory watch face week
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_Month_Destory (MMI_HANDLE_T handle);


/*****************************************************************************/
//  Description : create watch face multinum
//  Parameter: [In] win_id
//             [In] p_param: 控件绘制需要的所有参数
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T WATCHFACE_MultiNum_Create (MMI_WIN_ID_T              win_id,
                                                             WATCHFACE_CTRL_PARAM_T*   p_param
                                                             );

/*****************************************************************************/
//  Description : destory watch face week
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_MultiNum_Destory (MMI_HANDLE_T handle);

/*****************************************************************************/
//  Description : create watch face digital date
//  Parameter: [In] win_id
//             [In] p_param: 控件绘制需要的所有参数
//             [Out] None
//             [Return] MMI_HANDLE_T
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T WATCHFACE_DigitalDate_Create (MMI_WIN_ID_T              win_id,
                                                               WATCHFACE_CTRL_PARAM_T*   p_param
                                                                 );

/*****************************************************************************/
//  Description : destory watch face digital date
//  Parameter: [In] handle
//             [Out] None
//             [Return] BOOLEAN
//  Author: qi.zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN WATCHFACE_DigitalDate_Destory (MMI_HANDLE_T handle);

/*****************************************************************************/
//  Description : convert relative position to absolute position
//  Parameter: [In] center_position
//             [Out] None
//             [Return] GUI_POINT_T
//  Author: yuming.yang
//  Note:
/*****************************************************************************/
PUBLIC GUI_POINT_T WATCHFACE_ConvertRatioToPoint(WATCHFACE_RATIO_POSITION_T  center_position);

#endif

