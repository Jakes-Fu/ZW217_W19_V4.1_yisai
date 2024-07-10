/****************************************************************************
** File Name:      mmislide_export.h 
** Author:                                                                  
** Date:           2012/02/10
** Copyright:      2012 Spreadtrum, Incorporated. All Rights Reserved.       
** Description:    This is mmi slide win include file.
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE               NAME             DESCRIPTION                             
** 02/10/2012        xiaoming             Create
** 
****************************************************************************/
#ifndef _MMISLIDE_EXPORT_H
#define _MMISLIDE_EXPORT_H

#ifdef MMI_SLIDE_SUPPORT
/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmk_app.h"
#include "sci_types.h"
#include "guitab.h"
#include "Mmk_tp.h"


/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#define MMISLIDE_SCROLL_TIME              20                  //ˢ����ʱ��ʱ��

#define MMISLIDE_WIN_TOTAL_COUNT          10                  //ʵ����������󴰿���
#define MMISLIDE_TOTAL_COUNT              8                   //���ʵ����Ŀ

#define MMISLIDE_TP_MOVE_DIS              5                   //������ֵ����Ϊ��slide move

#define WIN_SYM_RADIUS                    4                   //����ָʾ���İ뾶
#define WIN_SYM_BORDER_COLOR              MMI_GRAY_COLOR      //����ָʾ���ı߿���ɫ
#define WIN_SYM_NORMAL_FILL_COLOR         MMI_WHITE_COLOR     //����ָʾ���������ɫ
#define WIN_SYM_FOCUS_FILL_COLOR          MMI_BLUE_COLOR      //����ָʾ���Ľ�����ɫ

#define MSG_SLIDE_MOVE_BEGIN              0xf100              //move����ʱ�����ڷ��͵���Ϣ
#define MSG_SLIDE_MOVE_END                0xf101              //moveֹͣʱ�����ڷ��͵���Ϣ
/**--------------------------------------------------------------------------*
**                         STATIC DEFINITION                                *
**--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          TYPE AND CONSTANT                                */
/*---------------------------------------------------------------------------*/
typedef enum
{
    MMI_SLIDE_ALL_CHILD_WIN = 0xff,      //���д��ھ�Ϊ�Ӵ��ڣ������Ǵ���ͬһ��������
    MMI_SLIDE_ALL_NO_CHILD_WIN,          //���д��ھ����Ӵ���
    MMI_SLIDE_MANY_PAINT_ONE_WIN,         //����һ�����ڣ����ˢ��(��δʵ��)
    MMI_SLIDE_INVALID
}MMI_SLIDE_TYPE;

typedef enum
{
    MMI_SLIDE_HANDLE_START = 0xff,  //��ʼ��� 255
    MMI_SLIDE_HANDLE_MAX   = MMI_SLIDE_HANDLE_START + MMISLIDE_TOTAL_COUNT,
    MMI_SLIDE_HANDLE_INVALID
}MMI_SLIDE_HANDLE;

typedef enum
{
    SLIDE_RECT_TYPE_FULLSCREEN = 0xff, //��������Ϊȫ��
    SLIDE_RECT_TYPE_CLIENTRECT,        //��������Ϊ�ͻ�����(��Ĭ�ϴ���Ϊ��׼)
    SLIDE_RECT_TYPE_CUSTOM,            //�Զ��廬�����򣬳�ʼ��ʱboth_rect ������Ϊ��
    SLIDE_RECT_TYPE_INVALID
}MMI_SLIDE_RECT_TYPE;

typedef enum
{
    SLIDE_SYMBOL_TYPE_DEFAULT = 0xff, //Ĭ��(�ޱ�����ָʾ��Ϊgui���Ƶ�Բ��)
    SLIDE_SYMBOL_TYPE_CUSTOM_IMG,     //�Զ�����ͼ
    SLIDE_SYMBOL_TYPE_OWNERDRAW,      //�Ի�
    SLIDE_SYMBOL_TYPE_INVALID
}MMI_SLIDE_SYMBOL_TYPE;

typedef enum
{
    SLIDE_SYM_WORK_NONE,              //����ʾ
    SLIDE_SYM_WORK_NORMAL,            //������ʱ��ʾ
    SLIDE_SYM_WORK_ALWAYS,            //һֱ��ʾ
    SLIDE_SYM_WORK_AUTO_HIDE,         //�Զ�����
}SLIDE_SYM_WORK_TYPR_E;

typedef enum
{
    SYMBOL_NORMAL_IMG,               //Ĭ��
    SYMBOL_FOCUS_IMG,                //����
    SYMBOL_BG_IMG,                   //����
    SYMBOL_BGL_IMG,                  //�󱳾�
    SYMBOL_BGR_IMG,                  //�ұ���
    SYMBOL_IMG_MAX
}MMI_SLIDE_SYMBOL_IMG_E;

typedef enum
{
    MMISLIDE_NORMAL_SWITCH,                  //Ĭ��Ч����֧��ƽ���ƶ���Ч
    MMISLIDE_PROMPTLY_SWITCH,                //�����л�������Ч(normalģʽ�ڴ治��ʱ����ʱ�л�Ϊ��ģʽ)
    MMISLIDE_NONE_SWITCH,                    //���л�����(ĳЩ�������Ҫ��slide�����е�ĳһ������)
}SLIDE_WIN_SWITCH_TYPE_E;

typedef struct
{
    MMI_SLIDE_TYPE      type;                  //����
    MMI_WIN_ID_T       *win_id_arr_ptr;        //����id����ָ��
    uint32              win_id_arr_size;       //����id����size
    MMI_WIN_ID_T        default_win;           //����ϵͳ���Ĭ�ϴ���id
    MMI_SLIDE_RECT_TYPE slide_rect_type;       //������������
    GUI_BOTH_RECT_T    *slide_both_rect_ptr;   //��������ָ��
    MMI_CTRL_ID_T      *parent_win_tab_ptr;    //�����ڵ�tab�ؼ�idָ�룬�޸��������ÿ�
    BOOLEAN             is_has_symbol;         //�Ƿ��������ָʾ��
}MMI_SLIDE_INIT_T;

typedef void (*SLIDE_DRAW_SYMBOL_BG_FUNC)   (GUI_LCD_DEV_INFO* dev_info_ptr, GUI_RECT_T bg_rect);
typedef void (*SLIDE_DRAW_SYMBOL_ITEM_FUNC) (GUI_LCD_DEV_INFO* dev_info_ptr, GUI_RECT_T item_rect, BOOLEAN is_focus);

//�ṹ��Ϣ
typedef struct 
{
    SLIDE_SYM_WORK_TYPR_E       work_type;                  //����
    MMI_SLIDE_SYMBOL_TYPE       dis_type;                   //��ʾ����
    MMI_IMAGE_ID_T              img_id_arr[SYMBOL_IMG_MAX]; //ͼƬ����
    SLIDE_DRAW_SYMBOL_BG_FUNC   OwnerDrawSymBgFunc;         //���Ʊ���
    SLIDE_DRAW_SYMBOL_ITEM_FUNC OwnerDrawSymItemFunc;       //����ÿһ��
}SLIDE_SYM_DIS_INFO_T;

//����������Ϣ
typedef struct 
{
    GUI_RECT_T       main_rect;
    GUI_RECT_T       top_bg_rect;
    GUI_RECT_T       bottom_bg_rect;
}SLIDE_CLIP_INFO_T;

typedef struct
{
    MMI_SLIDE_TYPE   type;                                 //����
    MMI_WIN_ID_T     win_array[MMISLIDE_WIN_TOTAL_COUNT];  //����id����
    uint16           win_total_num;                        //��������
    MMI_WIN_ID_T     default_win_id;                       //Ĭ�ϴ���id
    MMI_WIN_ID_T     cur_win_id;                           //��ǰ����id
    MMI_CTRL_ID_T    parent_win_tab_id;                    //������tab id
    GUI_BOTH_RECT_T  slide_both_rect;                      //���������� 
    BOOLEAN          is_loop_move;                          //ѭ������
    BOOLEAN          is_sym_on_top;                         //ָʾ��λ�ڶ���
    BOOLEAN          is_valid;                              //����״̬
    GUI_POINT_T      symbol_point;                          //ָʾ��λ��
    uint8            sym_remove_timer_id;                   //ָʾ���Ƴ�timer
    uint32           out_state;                             //������״̬

    SLIDE_WIN_SWITCH_TYPE_E     win_switch_type;            //�����л�����
    SLIDE_SYM_DIS_INFO_T        sym_dis_info;              //ָʾ����ʾ��Ϣ    
    SLIDE_CLIP_INFO_T           clip_info;                 //�������������Ϣ
    MMI_SLIDE_HANDLE handle;                                //���
    MMI_SLIDE_HANDLE *handle_ptr;                           //���ָ��
}MMI_SLIDE_INFO_T;
/**--------------------------------------------------------------------------*
**                         EXTERNAL DECLARE                                 *
**--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
**                         LOCAL DEFINITION                                *
**--------------------------------------------------------------------------*/

#define MMISLIDE_Creat                          MMIAPISLIDE_Creat
#define MMISLIDE_Release                        MMIAPISLIDE_Release
#define MMISLIDE_Active                         MMIAPISLIDE_Active
#define MMISLIDE_Deactive                       MMIAPISLIDE_Deactive
#define MMISLIDE_ReleaseEx                      MMIAPISLIDE_ReleaseEx
#define MMISLIDE_IsActive                       MMIAPISLIDE_IsActive
#define MMISLIDE_SetLoopSlide                   MMIAPISLIDE_SetLoopSlide
#define MMISLIDE_SetSymbolIsOnTop               MMIAPISLIDE_SetSymbolIsOnTop
#define MMISLIDE_SetSymbolPosition              MMIAPISLIDE_SetSymbolPosition
#define MMISLIDE_SetSymbolWorkType              MMIAPISLIDE_SetSymbolWorkType
#define MMISLIDE_SetSymbolDisplayInfo           MMIAPISLIDE_SetSymbolDisplayInfo
#define MMISLIDE_SetDefaultWin                  MMIAPISLIDE_SetDefaultWin
#define MMISLIDE_SetSetCurWin                   MMIAPISLIDE_SetCurWin
#define MMISLIDE_SetWinSwitchType               MMIAPISLIDE_SetWinSwitchType
#define MMISLIDE_GetInfo                        MMIAPISLIDE_GetInfo
#define MMISLIDE_SwitchToWin                    MMIAPISLIDE_SwitchToWin
/**--------------------------------------------------------------------------*
**                         GLOBAL DEFINITION                                *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : ����Slideʵ��
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_Creat(MMI_SLIDE_INIT_T *slide_init_ptr, MMI_SLIDE_HANDLE *handle_ptr);

/*****************************************************************************/
// 	Description : ����Slideϵͳ
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_Release(MMI_SLIDE_HANDLE *handle_ptr);

/*****************************************************************************/
// 	Description : ����
//	Global resource dependence: 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_Active(MMI_SLIDE_HANDLE *handle_ptr);

/*****************************************************************************/
// 	Description : ȥ����
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_Deactive(MMI_SLIDE_HANDLE *handle_ptr);

/*****************************************************************************/
// 	Description : ����Slideϵͳ
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_ReleaseEx(MMI_SLIDE_HANDLE *handle_ptr, BOOLEAN is_close_window);

/*****************************************************************************/
// 	Description : �Ƿ񼤻�
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_IsActive(MMI_SLIDE_HANDLE *handle_ptr);

/*****************************************************************************/
// 	Description : ����ѭ������
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetLoopSlide(MMI_SLIDE_HANDLE *handle_ptr, BOOLEAN is_loop);

/*****************************************************************************/
// 	Description : ����ָʾ���Ƿ��ڴ��ڶ���
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetSymbolIsOnTop(MMI_SLIDE_HANDLE *handle_ptr, BOOLEAN is_on_top);

/*****************************************************************************/
// 	Description : ����ָʾ�����λ��
//	Global resource dependence : 
//  Author: Jiaoyou.wu
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetSymbolPosition(MMI_SLIDE_HANDLE *handle_ptr, GUI_POINT_T *symbol_point_ptr);

/*****************************************************************************/
// 	Description : ����ָʾ������ʾ����
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetSymbolWorkType(MMI_SLIDE_HANDLE *handle_ptr, SLIDE_SYM_WORK_TYPR_E work_type);

/*****************************************************************************/
// 	Description : ����symbol ��ʾ��Ϣ
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetSymbolDisplayInfo(MMI_SLIDE_HANDLE *handle_ptr, SLIDE_SYM_DIS_INFO_T *dis_info_ptr);

/*****************************************************************************/
// 	Description : ����Ĭ�ϴ��ڵ�id
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetDefaultWin(MMI_SLIDE_HANDLE *handle_ptr, MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : ���õ�ǰ���ڵ�id
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetCurWin(MMI_SLIDE_HANDLE *handle_ptr, MMI_WIN_ID_T win_id);

/*****************************************************************************/
// 	Description : ���ô����л�����
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SetWinSwitchType(MMI_SLIDE_HANDLE *handle_ptr, SLIDE_WIN_SWITCH_TYPE_E switch_type);

/*****************************************************************************/
// 	Description : ��ȡSlide Info
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_GetInfo(MMI_SLIDE_HANDLE *handle_ptr, MMI_SLIDE_INFO_T *slide_info_ptr);

/*****************************************************************************/
// 	Description : �л���ĳһ������
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:  
/*****************************************************************************/
PUBLIC BOOLEAN MMIAPISLIDE_SwitchToWin(MMI_SLIDE_HANDLE *handle_ptr, MMI_WIN_ID_T win_id);
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
#endif /*#ifdef MMI_SLIDE_SUPPORT*/
#endif //_MMISLIDE_EXPORT_H


