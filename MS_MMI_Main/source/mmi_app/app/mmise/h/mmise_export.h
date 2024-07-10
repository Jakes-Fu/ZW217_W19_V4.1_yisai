/****************************************************************************
** File Name:      mmise_export.h
** Author:         kevin.lou
** Date:            2012/2/2
** Description:
****************************************************************************
**                         Important Edit History
** ------------------------------------------------------------------------*
** DATE           	        NAME                                DESCRIPTION
** 2011.05.11        	        xuanchen.jiang                   Created.
**
****************************************************************************/
#ifndef _MMISE_EXPORT_H_
#define _MMISE_EXPORT_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "os_api.h"
#include "mmk_type.h"
#include "sig_code.h"
#include "graphics_3d_rotate.h"
#include "mmi_appmsg.h"

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif



//cyclone
#define CYCLONE_DEBUGE

#define UP_DOWN_KEY_ANIM_FRAME_NUM                  (36)
#define LEFT_RIGHT_KEY_ANIM_FRAME_NUM               (8)
#define CATCH_FRAME_NUM_FACTOR                      (30)

#define SELECT_ICON_NUM_IN_WIN          (4)
#define ITEM_NUM_ONE_CIRCLE             (8)
#define HIGHLIGHT_ICON_WIDTH            (80)
#define HIGHLIGHT_ICON_HEIGHT           (80)
#define CYCLONE_MENU_ITEM_MAX           (128)
#define CYCLONE_MARGIN                  (24)
#define ANGLE_BETWEEN_ICON              (450)

#define WIN_WIDTH       (MMI_MAINSCREEN_WIDTH)
#define WIN_HEIGHT      (MMI_MAINSCREEN_HEIGHT)

#define START_ANGLE  (-900)
//#define FRONT_ANGLE  ((3600*1*(ICON_NUM-1))/8)

//#define FENG_ANGLE	(3600*ICON_NUM/8)
#define ROTATE_DIRECTION_LEFT               (-1)
#define ROTATE_DIRECTION_RIGHT              (1)
#define ROTATE_TOTAL_FRAME					(16)

#define VELOCITY_TRACKER_ITEM_NUM           (5)

#ifdef WIN32
#define CYCLONE_TP_UP_ACCELERATION          (10)
#else
#define CYCLONE_TP_UP_ACCELERATION          (10)
#endif

#ifdef WIN32
#define CYCLONE_TP_MOVE_DIS           (1)   // 与起点相比较，在多少范围内算是移动了触笔
#else
#define CYCLONE_TP_MOVE_DIS           (3)      // 与起点相比较，在多少范围内算是移动了触笔
#endif

#define CYCLONE_ICON_TEXT_COLOR   0xf800         //字体颜色  默认红色
#define CYCLONE_ICON_TEXT_SIZE    SONG_FONT_16   //字体大小
typedef enum 
{
    MMICYCLONE_MSG_BEGIN = MSG_MENU_CYCLONE_BEGIN,//(( CYCLONE_GROUP << 8 ) | 1 ),
    MMICYCLONE_MSG_DISPLAY_REQ,
    MMICYCLONE_MSG_DISPLAY_IND,
    MMICYCLONE_MSG_ROTATE_REQ,
    MMICYCLONE_MSG_ROTATE_FRAME_IND,
    MMICYCLONE_MSG_ROTATE_FRAME_CNF,
    MMICYCLONE_MSG_ROTATE_END_IND,
    MMICYCLONE_MSG_ROTATEFOREVER_REQ,
    MMICYCLONE_MSG_ROTATEFOREVER_CNF,
    MMICYCLONE_MSG_PARAMINIT_REQ,
    MMICYCLONE_MSG_PARAMINIT_CNF,
    MMICYCLONE_MSG_PARAM_RESET_REQ,
    MMICYCLONE_MSG_PARAM_RESET_CNF,
    MMICYCLONE_MSG_PARAMRELEASE_REQ,
    MMICYCLONE_MSG_PARAMRELEASE_CNF,
    MMICYCLONE_MSG_REFLECTION_UPDATE_REQ,
    MMICYCLONE_MSG_PARAM_BUF_SET_REQ,
    MMICYCLONE_MSG_ANGLESET_REQ,
    MMICYCLONE_MSG_ANGLESET_CNF,
    MMICYCLONE_MSG_ANIM_START_REQ,
    MMICYCLONE_MSG_ANIM_FRAME_IND,
    MMICYCLONE_MSG_ANIM_FRAME_CNF,
    MMICYCLONE_MSG_ANIM_END_IND,
    MMICYCLONE_MSG_HIGHLIGHT_FRAME_REQ,
    MMICYCLONE_MSG_HIGHLIGHT_FRAME_IND,
    MMICYCLONE_MSG_TP_UP_REQ,
    MMICYCLONE_MSG_TP_DOWN_REQ,
    MMICYCLONE_MSG_TP_SCROOL_REQ,
    MMICYCLONE_MSG_TP_SCROOL_IND,
    MMICYCLONE_MSG_TP_SCROOL_CNF,
    MMICYCLONE_MSG_TP_SCROOL_FRAME_IND,
    MMICYCLONE_MSG_SRCOLLANIM_REQ,   
    MMICYCLONE_MSG_SRCOLLANIM_ANGLE_RPT,   
    MMICYCLONE_MSG_SRCOLLANIM_IND,
    MMICYCLONE_MSG_SRCOLLANIM_CNF, 
    MMICYCLONE_MSG_SRCOLLANIM_END_IND, 
    MMICYCLONE_MSG_EXIT_REQ,
    MMICYCLONE_MSG_END
} MSG_CYCLONE_E;
typedef struct
{
    uint8 *all_icon_buffer_ptr;
    uint8 *highlight_buffer_ptr;
    uint16 layer_width;
    uint16 layer_height;
    uint16 window_width;
    uint16 window_height;
    uint16 cur_select_pos_index;
    uint16 cur_item_index;                         //current selected menu index. in option page style, it is also item index
    uint16 cyclone_max_item;//icon_num
} MMICYCLONE_Init_PARAM_T;
typedef struct
{
    int    start_angle;
    int    stop_angle;
    int8   direction;
    int32  total_frame;
} MMICYCLONE_ROTATE_PARAM_T;

typedef struct
{
    int    start_angle;
    int    stop_angle;
    int32  total_frame;
    BOOLEAN is_down_anim;
    BOOLEAN is_first_scroll;

    int    scroll_anim_start_angle;
    int    scroll_angle;
    int    inertia_angle;
    int    catch_angle;
    int    catch_stop_angle;
    int    cur_frame;

    uint16 cur_select_pos_index;
    uint16 cur_item_index;                         //current selected menu index. in option page style, it is also item index
} MMICYCLONE_SCROLL_PARAM_T;
typedef  struct
{
    uint8 *buffer[2];
    uint8 readidx;
    uint8 writeidx;
}DUAL_BUFF_INFO_T;

typedef uint32 (*icon_buff_t)[MENUICON_WIDTH*MENUICON_HEIGHT];
typedef struct CYCLONE_MEM_BLOCK_TAG
{
    uint32   dst_layer_second_buff[WIN_WIDTH*WIN_HEIGHT+ 2 + 1];
    icon_buff_t icon_buff;
    uint32   highlightIcon[SELECT_ICON_NUM_IN_WIN][HIGHLIGHT_ICON_WIDTH*HIGHLIGHT_ICON_HEIGHT];
    uint32** icon_buff_adress;
    uint32** icon_adress_backup;
    uint16*  sort_axisz;
    S3D_ROTATION_GEOMETRY_T* s3d_geometry;
    DUAL_BUFF_INFO_T dbuff_inf;
}CYCLONE_MEM_BLOCK;

//crystal cube
#ifdef WIN32
#define CRYSTALCUBE_TP_MOVE_DIS           1// 与起点相比较，在多少范围内算是移动了触笔
#else
#define CRYSTALCUBE_TP_MOVE_DIS           3// 与起点相比较，在多少范围内算是移动了触笔
#endif

#define CRYSTALCUBE_CIRCLE_ANGLE           (MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM*900)
typedef enum
{
    MMICRYSTALCUBE_TP_DOWN = 0,
    MMICRYSTALCUBE_TP_UP,
    MMICRYSTALCUBE_TP_MOVE,
    MMICRYSTALCUBE_TP_END
}MMICRYSTALCUBE_TP_STATE_E;
typedef struct
{
    uint8 read_idx;
    uint8 write_idx;
    uint8* buffer[2];
}DUALBUFF_INFO_T;
typedef struct
{
    void *src_buffer_ptr[MMITHEME_MENUMAIN_CRYSTALCUBE_PAGE_NUM];
    //uint8 *out_buffer_raw_ptr;
    //uint8 *out_buffer_ptr;
    uint16 layer_width;
    uint16 layer_height;
    uint16 window_width;
    uint16 window_height;
    DUALBUFF_INFO_T* dbuff_info_ptr;
}MMICRYSTALCUBE_INIT_PARAM_T;
typedef struct
{
    MMICRYSTALCUBE_TP_STATE_E tp_state;//0 down; 1 up; 2 move
    uint16 start_xpos;
    uint16 start_ypos;
    uint16 xpos;
    uint16 ypos;
    uint16 pre_xpos; //判断TP up时是否有初速度进行运动或者静止
    uint16 pre_ypos;
    int16 cur_angle;
    uint8* out_buffer_ptr;
}MMICRYSTALCUBE_TP_REQ_T;
typedef enum _MSG_CUBEPAGE
{
    MMICRYSTALCUBE_MSG_BEGIN = MSG_MENU_CRYSTALCUBE_BEGIN, //(( CUBEPAGE_GROUP << 8 ) | 1 ),
    MMICRYSTALCUBE_MSG_DISPLAY_REQ,
    MMICRYSTALCUBE_MSG_DISPLAY_IND,
    MMICRYSTALCUBE_MSG_ROTATE_REQ,
    MMICRYSTALCUBE_MSG_ROTATE_FRAME_IND,
    MMICRYSTALCUBE_MSG_ROTATE_FRAME_CNF,
    MMICRYSTALCUBE_MSG_ROTATE_END_IND,
    MMICRYSTALCUBE_MSG_ROTATEFOREVER_REQ,
    MMICRYSTALCUBE_MSG_ROTATEFOREVER_CNF,
    MMICRYSTALCUBE_MSG_PARAMINIT_REQ,
    MMICRYSTALCUBE_MSG_PARAMINIT_CNF,
    MMICRYSTALCUBE_MSG_PARAM_RESET_REQ,
    MMICRYSTALCUBE_MSG_PARAM_RESET_CNF,
    MMICRYSTALCUBE_MSG_PARAMRELEASE_REQ,
    MMICRYSTALCUBE_MSG_PARAMRELEASE_CNF,
    MMICRYSTALCUBE_MSG_REFLECTION_UPDATE_REQ,
    MMICRYSTALCUBE_MSG_PARAM_BUF_SET_REQ,
    MMICRYSTALCUBE_MSG_ANGLESET_REQ,
    MMICRYSTALCUBE_MSG_ANGLESET_CNF,
    MMICRYSTALCUBE_MSG_ANIM_START_REQ,
    MMICRYSTALCUBE_MSG_ANIM_FRAME_IND,
    MMICRYSTALCUBE_MSG_ANIM_FRAME_CNF,
    MMICRYSTALCUBE_MSG_ANIM_END_IND,
    MMICRYSTALCUBE_MSG_TP_SCROOL_REQ,
    MMICRYSTALCUBE_MSG_TP_SCROOL_FRAME_IND,
    MMICRYSTALCUBE_MSG_TP_CMD_REQ,
    MMICRYSTALCUBE_MSG_TP_CMD_CNF,
    MMICRYSTALCUBE_MSG_SRCOLLANIM_IND,
    MMICRYSTALCUBE_MSG_SRCOLLANIM_CNF, 
    MMICRYSTALCUBE_MSG_SRCOLLANIM_END_IND, 
    MMICRYSTALCUBE_MSG_EXIT_REQ,
    MMICRYSTALCUBE_MSG_END
} MSG_CRYSTALCUBE_E;

typedef struct
{
    BOOLEAN is_response;
    int index;
}MMICRYSTALCUBE_TP_CMD_T;


//cyclone
PUBLIC void MMIAPICYCLONE_DualBuff_init(uint8 * buffer1, uint8 *buffer2);

PUBLIC uint8* MMIAPICYCLONE_DualBuff_GetReadBuff(void);
PUBLIC void MMIAPICYCLONE_Init(MMICYCLONE_Init_PARAM_T* init_param_ptr);

PUBLIC void MMIAPICYCLONE_Reset(MMICYCLONE_Init_PARAM_T* init_param_ptr);

PUBLIC void MMIAPICYCLONE_ReleaseParam(void);

PUBLIC void MMIAPICYCLONE_Destroy(void);

PUBLIC void MMIAPICYCLONE_StartEnterAnim(void);

PUBLIC void MMIAPICYCLONE_GetHighLightFrame(void);

PUBLIC void MMIAPICYCLONE_GetNextEnterAnimFrame(void);

PUBLIC void MMIAPICYCLONE_GetNextRotateFrame(void);

PUBLIC void MMIAPICYCLONE_Rotate(MMICYCLONE_ROTATE_PARAM_T *param);

PUBLIC void MMIAPICYCLONE_TP_Down(MMICYCLONE_SCROLL_PARAM_T *param);

PUBLIC void MMIAPICYCLONE_TP_Move(MMICYCLONE_SCROLL_PARAM_T *param);

PUBLIC void MMIAPICYCLONE_TP_Up(MMICYCLONE_SCROLL_PARAM_T *param);

PUBLIC void MMIAPICYCLONE_GetNextScrollFrame(void);

PUBLIC void MMIAPICYCLONE_TP_Up_Anim(MMICYCLONE_SCROLL_PARAM_T *param);

PUBLIC void MMIAPICYCLONE_GetNextTpUpAnimFrame(void);

PUBLIC void MMIAPICYCLONE_CreatMemBlock(uint16 icon_num);

PUBLIC void MMIAPICYCLONE_ReleaseMemBlock(void);


//crystal cube

/*****************************************************************************/
//  Description : 立方体初始化
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_Init(MMICRYSTALCUBE_INIT_PARAM_T* init_param_ptr);

/*****************************************************************************/
//  Description : 销毁立方体
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_Destroy(void);

/*****************************************************************************/
//  Description : 启动进入动画
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_StartAnim(BOOLEAN is_enter);

/*****************************************************************************/
//  Description : 请求动画的下一帧
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_GetNextAnimFrame(void);

/*****************************************************************************/
//  Description : 更新倒影
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_UpdateReflection(uint16 page_index);

/*****************************************************************************/
//  Description : 请求显示
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_Display(void);

/*****************************************************************************/
//  Description :旋转
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_Rotate(int16 angle);

/*****************************************************************************/
//  Description :请求旋转动画的下一帧
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_GetNextRotateFrame(void);

/*****************************************************************************/
//  Description : 触摸屏移动
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_TpScroll(MMICRYSTALCUBE_TP_REQ_T* tp_param_ptr);

/*****************************************************************************/
//  Description : 请求触摸屏滑动动画的下一帧
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_GetNextTpScrollAnimFrame(void);

/*****************************************************************************/
//  Description : 触摸屏点触
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_TpCmd(int16 xpos,int16 ypos);

/*****************************************************************************/
// 	Description : 添加速度点
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_AddVelocityItem(int32 x,int32 y);

/*****************************************************************************/
//  Description : 立方体参数reset
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_ResetParam(MMICRYSTALCUBE_INIT_PARAM_T* init_param_ptr);

/*****************************************************************************/
//  Description : 释放参数
//  Global resource dependence : 
//  Author: xuanchen.jiang
//  Note:
/*****************************************************************************/
PUBLIC void MMIAPICCUBE_ReleaseParam(void);
PUBLIC DUALBUFF_INFO_T*  MMIAPICCUBE_DualBuff_Init(uint8* buffer1,uint8* buffer2);
PUBLIC uint8* MMIAPICCUBE_DualBuff_GetReadBuf(DUALBUFF_INFO_T* dbuff_info_ptr);
PUBLIC void MMIAPICCUBE_DualBuff_Destroy(DUALBUFF_INFO_T* dbuff_info_ptr);
PUBLIC void  MMIAPICCUBE_DualBuff_Reset(DUALBUFF_INFO_T* dbuff_info_ptr,uint8* buffer1,uint8* buffer2);

/*****************************************************************************/
//  如下宏定义用以实现对外接口更名前后的兼容
/*****************************************************************************/
#ifdef MMI_FUNC_COMPATIBLE_SUPPORT

#define MMICYCLONE_DualBuff_init            MMIAPICYCLONE_DualBuff_init
#define MMICYCLONE_DualBuff_GetReadBuff     MMIAPICYCLONE_DualBuff_GetReadBuff
#define MMICYCLONE_Init                     MMIAPICYCLONE_Init
#define MMICYCLONE_Reset                    MMIAPICYCLONE_Reset
#define MMICYCLONE_ReleaseParam             MMIAPICYCLONE_ReleaseParam
#define MMICYCLONE_Destroy                  MMIAPICYCLONE_Destroy
#define MMICYCLONE_StartEnterAnim           MMIAPICYCLONE_StartEnterAnim
#define MMICYCLONE_GetHighLightFrame        MMIAPICYCLONE_GetHighLightFrame
#define MMICYCLONE_GetNextEnterAnimFrame    MMIAPICYCLONE_GetNextEnterAnimFrame
#define MMICYCLONE_GetNextRotateFrame       MMIAPICYCLONE_GetNextRotateFrame
#define MMICYCLONE_Rotate                   MMIAPICYCLONE_Rotate
#define MMICYCLONE_TP_Down                  MMIAPICYCLONE_TP_Down
#define MMICYCLONE_TP_Move                  MMIAPICYCLONE_TP_Move
#define MMICYCLONE_TP_Up                    MMIAPICYCLONE_TP_Up
#define MMICYCLONE_GetNextScrollFrame       MMIAPICYCLONE_GetNextScrollFrame
#define MMICYCLONE_TP_Up_Anim               MMIAPICYCLONE_TP_Up_Anim
#define MMICYCLONE_GetNextTpUpAnimFrame     MMIAPICYCLONE_GetNextTpUpAnimFrame
#define MMICYCLONE_CreatMemBlock            MMIAPICYCLONE_CreatMemBlock
#define MMICYCLONE_ReleaseMemBlock          MMIAPICYCLONE_ReleaseMemBlock

#define CCUBE_Init                          MMIAPICCUBE_Init
#define CCUBE_Destroy                       MMIAPICCUBE_Destroy
#define CCUBE_StartAnim                     MMIAPICCUBE_StartAnim
#define CCUBE_GetNextAnimFrame              MMIAPICCUBE_GetNextAnimFrame
#define CCUBE_UpdateReflection              MMIAPICCUBE_UpdateReflection
#define CCUBE_Display                       MMIAPICCUBE_Display
#define CCUBE_Rotate                        MMIAPICCUBE_Rotate
#define CCUBE_GetNextRotateFrame            MMIAPICCUBE_GetNextRotateFrame
#define CCUBE_TpScroll                      MMIAPICCUBE_TpScroll
#define CCUBE_GetNextTpScrollAnimFrame      MMIAPICCUBE_GetNextTpScrollAnimFrame
#define CCUBE_TpCmd                         MMIAPICCUBE_TpCmd
#define CCUBE_AddVelocityItem               MMIAPICCUBE_AddVelocityItem
#define CCUBE_ResetParam                    MMIAPICCUBE_ResetParam
#define CCUBE_ReleaseParam                  MMIAPICCUBE_ReleaseParam

#define DualBuff_Init                       MMIAPICCUBE_DualBuff_Init
#define DualBuff_GetReadBuf                 MMIAPICCUBE_DualBuff_GetReadBuf
#define DualBuff_Destroy                    MMIAPICCUBE_DualBuff_Destroy
#define DualBuff_Reset                      MMIAPICCUBE_DualBuff_Reset

#endif //MMI_FUNC_COMPATIBLE_SUPPORT

#ifdef   __cplusplus
}
#endif

#endif //


