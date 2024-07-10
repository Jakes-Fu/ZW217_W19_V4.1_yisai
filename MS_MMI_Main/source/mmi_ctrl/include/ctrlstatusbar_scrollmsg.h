/*! @file ctrlstatusbar_scrollmsg.h
@brief 此文件介绍了statusbar_data控件的对外接口和数据类型
@author bin.ji
@version 1.0
@date 8/5/2008
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details
@note 09/2012     xiyuan.ma      Create
*******************************************************************************/

/*! @page page_StatusBar_data StatusBar_data

(暂无)

*******************************************************************************/

/*! @addtogroup statusbar_dataGrp StatusBar_data
@brief statusbar_data模块
@details 该文件不对外开放，所有app都应该调用ctrlwin.h中的封装接口
@{
*******************************************************************************/

#ifndef _CTRL_STATUSBAR_SCROLLMSG_H_
#define _CTRL_STATUSBAR_SCROLLMSG_H_

/*----------------------------------------------------------------------------*/
/*                          Include Files                                     */
/*----------------------------------------------------------------------------*/
#include "guictrl.h"
#include "guicommon.h"
#include "guifont.h"
#include "mmitheme_statusbar.h"

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*----------------------------------------------------------------------------*/
/*                         MACRO DEFINITION                                   */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                          TYPE AND STRUCT                                   */
/*----------------------------------------------------------------------------*/
#define GUISTBSCROLLMSG_SCROLL_FRAME_NUM        5       //滚动效果帧数
#define GUISTBSCROLLMSG_MAX_SCROLL_ITEM_NUM     20      //最大滚动信息数目
#define GUISTBSCROLLMSG_TEXT_WIDTH              DP2PX_VALUE(200)     //文本区域宽度

/*! @struct GUISTBSCROLLMSG_SCROLL_STATE_E
@brief 需要滚动显示的数据
*/
typedef enum
{
    GUISTBSCROLLMSG_SCROLL_NONE = 0, /*!< 无*/
    GUISTBSCROLLMSG_SCROLL_START,  	/*!< 开始滚动*/
    GUISTBSCROLLMSG_SCROLL_RUNNING,  /*!< 正在滚动*/
    GUISTBSCROLLMSG_SCROLL_END		/*!< 结束滚动*/
} GUISTBSCROLLMSG_SCROLL_STATE_E;

/*! @struct GUISTBSCROLLMSG_SCROLL_ITEM_E
@brief 需要滚动显示的数据
*/
typedef enum
{
    GUISTBSCROLLMSG_SCROLL_ITEM_NONE = 0,     /*!< 无*/
    GUISTBSCROLLMSG_SCROLL_ITEM_NEW_MESSAGE,     /*!< 未读短信*/
    GUISTBSCROLLMSG_SCROLL_ITEM_NEW_SMSCB,     /*!< 小区广播*/
    GUISTBSCROLLMSG_SCROLL_ITEM_ALARM,  	        /*!< 闹钟*/
    GUISTBSCROLLMSG_SCROLL_ITEM_MISSED_CALL,       /*!< 未接来电*/
    GUISTBSCROLLMSG_SCROLL_ITEM_NEW_MMS,       /*!< 未读彩信*/
    GUISTBSCROLLMSG_SCROLL_ITEM_SCHEDULE,       /*!< 日程*/

    GUISTBSCROLLMSG_SCROLL_ITEM_MAX		        /*!< 最大类型*/
} GUISTBSCROLLMSG_SCROLL_ITEM_E;

/*! @struct GUISTBSCROLLMSG_SCROLL_ITEM_T
@brief 需要滚动显示的数据
*/
typedef struct
{
    GUISTBSCROLLMSG_SCROLL_ITEM_E type;
    BOOLEAN is_valid;
    MMI_IMAGE_ID_T icon_id;
    MMI_STRING_T string_info;
} GUISTBSCROLLMSG_SCROLL_ITEM_T;

/*! @struct GUISTBSCROLLMSG_SCROLL_MESSAGE_T
@brief 需要滚动显示的数据
*/
typedef struct
{
    GUISTBSCROLLMSG_SCROLL_STATE_E scroll_state;
    uint32 cur_frame;       //当前帧
    uint32 cur_item_index;  //当前item index
    uint32 cur_str_pos;     //当前字符串位置
    GUISTBSCROLLMSG_SCROLL_ITEM_T item[GUISTBSCROLLMSG_MAX_SCROLL_ITEM_NUM];
} GUISTBSCROLLMSG_SCROLL_MESSAGE_T;


/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
#ifdef GUIF_STATUSBAR_SCROLL
/*****************************************************************************/
//  Description : 得到当前（第几）帧
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC uint32 CTRLSTBSCROLLMSG_GetCurFrame (
    void
);


/*****************************************************************************/
//  Description : 得到当前滚动状态
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC GUISTBSCROLLMSG_SCROLL_STATE_E CTRLSTBSCROLLMSG_GetCurScrollState (
    void
);


/*****************************************************************************/
//  Description : 显示完一行
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTBSCROLLMSG_ConsumeOneLine (
    void
);


/*****************************************************************************/
//  Description : 显示完一帧
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTBSCROLLMSG_ConsumeOneFrame (
    void
);


/*****************************************************************************/
//  Description : 添加一条滚动信息
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBSCROLLMSG_AppendScrollItem (
    GUISTBSCROLLMSG_SCROLL_ITEM_T *item_ptr
);


/*****************************************************************************/
//  Description : 得到当前行显示数据
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBSCROLLMSG_GetCurLine (
    GUISTBSCROLLMSG_SCROLL_ITEM_T *item_ptr
);


/*****************************************************************************/
//  Description : 得到下一行显示数据
//  Global resource dependence :
//  Author:bin.ji
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLSTBSCROLLMSG_GetNextLine (
    GUISTBSCROLLMSG_SCROLL_ITEM_T *item_ptr
);


/*****************************************************************************/
//  Description : 是否由应用停止
//  Global resource dependence :
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void CTRLSTBSCROLLMSG_SetIsValid (
    GUISTBSCROLLMSG_SCROLL_ITEM_E type,
    BOOLEAN is_valid
);
#endif


/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif // _CTRL_STATUSBAR_DATA_H_


/*@}*/
