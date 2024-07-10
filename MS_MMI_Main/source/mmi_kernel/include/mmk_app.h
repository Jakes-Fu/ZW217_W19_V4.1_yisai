/*******************************************************************************
** File Name:      mmk_app.h                                               	   *
** Author:         Great.Tian                                                  *
** Date:           04/11/2003                                                  *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.           *
** Description:    This file is used to describe the data struct of            *
**                 system, applicaiton, window and control                     *
********************************************************************************
**                         Important Edit History                              *
** ----------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                                 *
** 11/2003        Great.Tian       Create									   *
*******************************************************************************/
#ifndef  _MMK_APP_H_    
#define  _MMK_APP_H_   

/*----------------------------------------------------------------------------*/
/*                         DEPENDANCY                                         */
/*----------------------------------------------------------------------------*/
#include "mmk_type.h"
#include "mmk_msg.h" 
#include "mmi_osbridge.h"

#include "cafcontrol.h"
#include "cafapplet_gen.h"
#include "ui_layer.h"

#ifdef ATEST_SUPPORT
// add by xiaolei.zheng for autotest
#include "mmk_tree.h"
#endif // ATEST_SUPPORT

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*----------------------------------------------------------------------------*/
/*                         Constant Variables                                 */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                         Macro Declaration    							  */
/*----------------------------------------------------------------------------*/
typedef BOOLEAN         (*PARSE_FUNC)(MMI_HANDLE_T win_handle,uint32 **value_pptr);
typedef MMI_RESULT_E    (* PROCESSMSG_FUNC)(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id, DPARAM param);


typedef uint32 WINDOW_DISPLAY_STYLE;
#define WS_DISABLE_RETURN_WIN           0x01    //������������, ��������᷵�ص�idle
#define WS_HAS_PROMPT_WIN			    0x02    //pubwin����
#define WS_HAS_TRANSPARENT              0x04    //����͸����ʾ
#define WS_HAS_ANIM_BG                  0x08    //idle������anim�ؼ�Ϊ����, �Ȼ�anim�󻭴���
#define WS_DISABLE_HWICON               0x10    //pubwin�ȴ��ڲ���ӦӲͼ��
#define WS_DISABLE_COMMON_BG            0x20    //���ù�������
#define WS_DISABLE_REDRAW_CHILDWIN      0x40    //��ֹˢ���Ӵ���
#define WS_DISPATCH_TO_CHILDWIN         0x80    //�Ӵ����ȴ���kbd
#define WS_HAS_BUTTON_SOFTKEY           0x100   //����button softkey
#define WS_NO_DEFAULT_STYLE             0x200   // ����ϵͳĬ�ϵĴ��ڷ��
#define WS_POPMENU_WIN			        0x400   //popmenu����
#define WS_NEED_DBLCLK                  0x800   // ������Ҫ����DBLCLK��Ϣ
#define WS_NEED_PAINT_PRE_WIN           0x1000  // ���ڴ�ʱ���Ƿ���Ҫˢ��ǰһ������
#define WS_TOOL_WIN                     0x2000  // ���ߴ��ڣ������ǰһ�����ڷ�lost focus
#define WS_WIDGET_WIN                   0x4000  // widget���ڣ�����lost focus������ӿؼ���lost focus��Ϣ
#define WS_WIN_DIRTY                    0x8000  // ��������
#define WS_DISABLE_FLING_CLOSE_WIN      0x10000  // ���û����ش�Ĭ�ϴ���
#define WS_HAS_STATUSBAR                0x10000000   // ����״̬��(�ڲ�ʹ��)
#define WS_HAS_TITLE                    0x20000000   // ����������(�ڲ�ʹ��)
#define WS_HAS_SOFTKEY                  0x40000000   // ���������(�ڲ�ʹ��)
#define WS_DISABLE_SOFTKEY_TO_TITLE     0x80000000   // Istyle�²�����SoftkeyתTitle,���ȴ�WS_HAS_SOFTKEY(�ڲ�ʹ��)
//#define WS_HAS_COVER_BAR               0x200    //����coverbar

//#define WS_DISABLE_LCD_SWITCH          0x100     //��ֹ�������л�

#define WS_MASK                        0xFFFFFFFF //�������û�ȥ����������״̬

typedef uint32 WINDOW_SUPPORT_ANGLE;
#define WIN_SUPPORT_ANGLE_ALL           0
#define WIN_SUPPORT_ANGLE_0             0x1
#define WIN_SUPPORT_ANGLE_90            0x2
#define WIN_SUPPORT_ANGLE_180           0x4
#define WIN_SUPPORT_ANGLE_270           0x8
#define WIN_SUPPORT_ANGLE_CUR           0x100 //lcd��ǰ�Ƕ�, ֻ�ܵ���ʹ��
#define WIN_SUPPORT_ANGLE_INIT          0x200 //lcd��ʼ�Ƕ�, ֻ�ܵ���ʹ��

typedef uint32 WINDOW_MOVE_STYLE;
#define MOVE_BY_SETTING			       0
#define MOVE_HORIZONTAL_DIRECTION      1
#define MOVE_VERTICAL_DIRECTION        2
#define MOVE_NOT_FULL_SCREEN_WINDOW	   4
#define MOVE_FORBIDDEN       		   5

#define MMI_MAX_HARDWARE_ICON          5

#define MMK_GET_TP_GESTURE(param)  (((MMI_MULTI_KEY_TP_MSG_PARAM_T *)(param))->gesture_type) //���x����
#define MMK_GET_TP_X(param)        (((MMI_MULTI_KEY_TP_MSG_PARAM_T *)(param))->cur_tp_point.x) //���x����
#define MMK_GET_TP_Y(param)        (((MMI_MULTI_KEY_TP_MSG_PARAM_T *)(param))->cur_tp_point.y) //���y����
#define MMK_GET_TP_SLIDE(param)    (((MMI_MULTI_KEY_TP_MSG_PARAM_T *)(param))->is_slide)       //tp�Ƿ����𻬶�


#define VIRTUAL_WIN_ID             ( MMK_GetFirstAppletHandle() )

typedef uint32 MMI_APPLET_STATE_T;
#define MMI_APPLET_STATE_DYNAMIC        0x01   //��̬Ӧ��, ֧������
#define MMI_APPLET_STATE_BG             0x02   //֧�ֺ�̨����, ��ر����д���
#define MMI_APPLET_STATE_BG_HIDE_WIN    0x04   //֧�ֺ�̨����, ���������д���
#define MMI_APPLET_STATE_CLOSE_WITH_WIN 0x08   //�ر�applet�����д���, appletҲ��ر�
#define MMI_APPLET_STATE_DISABLE_RETURN_IDLE  0x10      //��ֹ�����idle
#define MMI_APPLET_STATE_START_BG       0x20   //��ʱΪ��̨
#define MMI_APPLET_STATE_MEM_CONFLICT   0x40   //������ڴ��ͻ���޷���Ӧ�ú�̨
#define MMI_APPLET_STATE_TASK_MANAGER   0x80   //����ʾ�������������

// �����ٶȺͼ��ٶȵĺ궨��
#define FLING_MIN_INVALID_TIME    (500)     // ����֮��ļ������500����ʱ��ʱ����Ч
#define FLING_TIME                (0.05f)  // �൱��40֡ÿ��
#define FLING_FRICTION            (50.f)    // ��С���ٶ�
#define FLING_RUN_MIN_VELOCITY    (50.f)    // �����˶�ʱ����С�ٶ�
#define FLING_MIN_VELOCITY        (200.f)   // ��С��ʼ�ٶ� ����ÿ��
#define FLING_MAX_VELOCITY        (5000.f)  // ��������ٶ� ����ÿ��
#define FLING_MAX_SUM_VELOCITY    (1500.f)  // ����ۼ��ٶ� ����ÿ��
#define FLING_MAX_DISTANCE        (160)     // ÿ֡�ƶ������������
#define FLING_MIN_DISTANCE        (5)       // ÿ֡�ƶ�����С������
#define FLING_0_VELOCITY          (0.001f)  // 0�ٶ�

// ������ʼ��tp move�ĺ궨��
#define MMK_TPMOVE_POINT_MIN_NUM  (2)     // ����������ֵ��move

// �ж��Ƿ�move
#define IS_TP_MOVE_X( _x1, _x2 ) ( (abs((_x1) - (_x2))) > (MMK_TPMOVE_POINT_MIN_NUM) )
#define IS_TP_MOVE_Y( _y1, _y2 ) ( (abs((_y1) - (_y2))) > (MMK_TPMOVE_POINT_MIN_NUM) )

#define IS_TP_MOVE_X_EXT( _x1, _x2, _param ) ( abs((_x1) - (_x2)) > (_param) )
#define IS_TP_MOVE_Y_EXT( _y1, _y2, _param ) ( abs((_y1) - (_y2)) > (_param) )

// �ָ���������ʼ֡������֡��
#define MMK_RECOVER_SLIDE_TOTAL_FRAME         20
#define MMK_RECOVER_SLIDE_START_FRAME         12

#define MMK_AddVelocityItem(x, y)
#define VELOCITY_TRACKER_ITEM_NUM (8)
/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
// �����津�ʵ��˶�����
typedef enum
{
    MMK_TP_SLIDE_NONE,      // ��״̬
    MMK_TP_SLIDE_TP_SCROLL, // �津���ƶ�
    MMK_TP_SLIDE_FLING,     // ����̧���Ļ���
    MMK_TP_SLIDE_RECOVER,   // �ָ��ƶ�
    MMK_TP_SLIDE_MAX        // ϵͳ����
}MMK_TP_SLIDE_E;

// �������˶�����
typedef enum
{
    DAMP_PLAY_TYPE_0,  // none
	DAMP_PLAY_TYPE_1,  // ǿ����������
	DAMP_PLAY_TYPE_2,  // ��ͬ���ټ���
	DAMP_PLAY_TYPE_3,  // ������ǿ����
	DAMP_PLAY_TYPE_4,  // ������
	DAMP_PLAY_TYPE_5,//�ȼ����˶�
	DAMP_PLAY_TYPE_MAX // max
} MMK_DAMP_PLAY_TYPE_E;

// �����ƶ���������
typedef enum
{
    MMI_WIN_ANIM_MOVE_FRTL,
    MMI_WIN_ANIM_MOVE_FLTR,
    MMI_WIN_ANIM_MOVE_FTTB,
    MMI_WIN_ANIM_MOVE_FBTT,
	MMI_WIN_ANIM_MOVE_FORBIDDEN,
    MMI_WIN_ANIM_MOVE_MAX
} MMI_WIN_ANIM_MOVE_TYPE_E;

typedef enum
{
    MMI_WIN_ANIM_MOVE_SETTING_FORBID,
    MMI_WIN_ANIM_MOVE_SETTING_HORIZONTAL,
    MMI_WIN_ANIM_MOVE_SETTING_VERTICAL,
    MMI_WIN_ANIM_MOVE_SETTING_MAX
} MMI_WIN_ANIM_MOVE_SETTING_TYPE_E;

/*! \brief MMI component type */
typedef enum
{
    MMI_HANDLE_NONE,
    MMI_HANDLE_APPLET,                                      /*!< applet type */
	MMI_HANDLE_WINDOW,										/*!< window type */
	MMI_HANDLE_CONTROL										/*!< control type */
}MMI_HANDLE_TYPE_E;

/*! \brief MMI component type */
typedef enum
{
	CT_APPLICATION,									/*!< application type */
	CT_WINDOW,										/*!< window type */
	CT_CONTROL										/*!< control type */
}MMI_COMPONENT_TYPE_E;

typedef struct  
{
    int32 orig_offset_y; // ��¼��ʼ��offset
    int32 orig_offset_x; // ��¼��ʼ��offset
    int32 run_distance;  // �ƶ�����λ��
    int32 odd_distance;  // �ƶ���ʣ�����λ��

}MMK_SLIDE_RECOVER_INFO_T;

/*! \brief MMI application data structrue */
typedef struct application_tag
{	
    MMI_RESULT_E 				(*ProcessMsg)( PWND app_ptr, uint16 msg_id, DPARAM param);  	/*!< the process message function of the application */	
    MMI_COMPONENT_TYPE_E		component_type;													/*!< the component type, used in the type check */	
    ADD_DATA					adddata_ptr;												/*!< the additional data point of the application */

}MMI_APPLICATION_T;

/*! \brief hardware icon structure */
typedef struct
{
    GUI_RECT_T rect;
    MMI_RESULT_E (*ProcessMsg)(MMI_MESSAGE_ID_E msg_id, DPARAM param); /*!< the process message function of the window	 */
    uint32     virtual_key_code;    //�������������ֵ(��dal_keypad.h����)��ֻ��TP����������̲�ʹ��
}MMI_HW_ICON_T;

//���ڰ�ť����
typedef enum
{
    MMI_WIN_BUTTON_GRAYED,      // ���      
    MMI_WIN_BUTTON_PRESSED,     // ����
    MMI_WIN_BUTTON_RELEASED     // �ͷ�        
}  MMI_WIN_BUTTON_TYPE_E;

//define the button of the softkey bar
typedef struct mmk_win_button_tag
{
     MMI_IMAGE_ID_T    icon_down_id;        // icon id
     MMI_IMAGE_ID_T    icon_up_id;          // icon id
     MMI_IMAGE_ID_T    gray_icon_id;        // icon id
     BOOLEAN           is_show;             // TRUE - show;
     BOOLEAN           is_gray;             // TRUE - show;
     BOOLEAN           is_pressed;          // TRUE - press down, False - up
     uint8             pad1;
     GUI_RECT_T        rect;                // region 
} MMK_WIN_BUTTON_T;

//#ifdef GUI_CTRL_SCROLL_KEY
typedef struct mmk_scroll_button_tag
{
     MMI_IMAGE_ID_T    icon_down_id;        // icon id
     MMI_IMAGE_ID_T    icon_up_id;          // icon id
	 MMI_IMAGE_ID_T    icon_confirm_id;     // icon id     
     MMI_IMAGE_ID_T    confirm_point_id;    // icon id
     BOOLEAN           is_show;             // TRUE - show;
     BOOLEAN           is_gray;             // TRUE - show;
     BOOLEAN           is_pressed;          // TRUE - press down, False - up
     uint8             pad1;
     GUI_RECT_T        rect;                // region 
} MMK_SCROLL_BUTTON_T;
//#endif

//��̬�����Ϣ
typedef struct  
{
    CAF_GUID_T      guid;                                                    //��̬���guid
    CAF_RESULT_E    (*ClassNew)( CAF_GUID_T guid, void ** object_pptr );     //������̬��Ļص�
    uint32          reserved;                                                //����
}CAF_STATIC_CLASS_T;

//���о�̬����Ϣ�Ľṹ
typedef struct  
{
    CAF_STATIC_CLASS_T* class_list_ptr;   //��̬����Ϣ������
    uint32              num_of_class;     //��̬����Ϣ�ĸ���

}CAF_STATIC_CLASS_INFO_T;

#define GUID_NAME_DEF( guid )  guid, #guid

//��̬Ӧ�õ���Ϣ
typedef struct  
{
    CAF_GUID_T          guid;      //��̬Ӧ�õ�guid
    const char*         name;
    uint32              size;      //��̬Ӧ�õĴ�С
    APPLET_HANDLEEVENT  func;      //��̬Ӧ�õĴ�����
    MMI_IMAGE_ID_T      image_id;  //image id
    MMI_IMAGE_ID_T      text_id;   //text id
    
}CAF_STATIC_APPLET_T;

//��̬��Ӧ����Ϣ�Ľṹ
typedef struct  
{
    CAF_STATIC_APPLET_T* applet_ptr;    //��̬Ӧ�õ�����
    uint32               num_of_applet; //��̬Ӧ�õĸ���

}CAF_STATIC_APPLET_INFO_T;

typedef struct
{
    MODULEENTRY                 entry;              //��̬ģ�����ں���, ƽ̨��ֻ��һ����̬ģ��
    CAF_STATIC_APPLET_INFO_T**  applet_info_pptr;   //��̬Ӧ�õ���Ϣ
    uint32                      applet_info_num;    //��̬Ӧ����Ϣ�ĸ���
}CAF_STATIC_MODULE_T;

//�������ڱ�Ľṹ
typedef struct  
{
    MMI_HANDLE_T         applet_handle;        //Ӧ�õľ��
    MMI_HANDLE_T         parent_win_handle;    //�����ڵľ��, ��0��Ϊ�Ӵ���
    const uint32*        win_table_ptr;        //���ڱ�
    PROCESSMSG_FUNC      func;                 //���ڻص�����
    ADD_DATA             add_data_ptr;         //���ڵĶ�������
    ADD_DATA             add_data_ex_ptr;      //���ڵĶ�������

}MMI_WINDOW_TABLE_CREATE_T;

//�������ڵĽṹ
typedef struct  
{
    MMI_HANDLE_T             applet_handle;      //Ӧ�õľ��
    MMI_HANDLE_T             parent_win_handle;  //�����ڵľ��, ��0��Ϊ�Ӵ���
    MMI_WIN_ID_T             win_id;             //���ڵ�id
    PROCESSMSG_FUNC          func;               //���ڵĻص�����
    ADD_DATA                 add_data_ptr;       //���ڵĶ�������
    ADD_DATA                 add_data_ex_ptr;    //���ڵĶ�������
    MMI_WIN_PRIORITY_E       win_priority;       //���ڵ����ȼ�
    MMI_SOFT_ICON            softicon_state;     //���ڵ����״̬
    WINDOW_DISPLAY_STYLE     window_style;       //���ڵ���ʾ���
    WINDOW_MOVE_STYLE	     window_move_style;  //���ڵĻ������
    BOOLEAN                  is_user_open_win;   //�û��ֶ�����open window�ӿ�
    
}MMI_WINDOW_CREATE_T;

//�����ؼ��Ľṹ
typedef struct  
{
    MMI_CTRL_ID_T       ctrl_id;             //�ؼ���id
    CAF_GUID_T          guid;                //�ؼ���guid
    MMI_HANDLE_T        parent_win_handle;   //�����ڵľ��
    MMI_HANDLE_T        parent_ctrl_handle;  //���ؼ��ľ��, ��0��Ϊ�ӿؼ�
    void*               init_data_ptr;       //�ؼ��ĳ�ʼ������
    GUICTRL_STATE_T     state;               //�ؼ���״̬
    
}MMI_CONTROL_CREATE_T;

//����Ӧ�õĽṹ
typedef struct  
{
    CAF_GUID_T         guid;                 //Ӧ�õ�guid
    MMI_APPLET_STATE_T state;                //Ӧ�õ�״̬
    void*              param_ptr;            //Ӧ�����յ�start��Ϣʱ����Ϣ����
    uint32             size_of_param;        //Ӧ�����յ�start��Ϣʱ����Ϣ������С
    MMI_HANDLE_T       parent_applet_handle; //��Ӧ�õľ��, ��0��Ϊ��Ӧ��
        
}MMI_APPLET_START_T;

//Ӧ������ʱ��Ϣ
typedef struct  
{
    CAF_GUID_T         guid;             //Ӧ�õ�guid
    MMI_HANDLE_T       handle;           //Ӧ�õ�handle
}MMI_APPLET_INFO_T;

typedef struct  
{
    MMI_APPLET_INFO_T*  list_ptr;         //Ӧ������
    uint32              cur_index;        //��ǰӦ��
    uint32              num_of_list;      //�������
}MMI_APPLET_INFO_LIST_T;
typedef struct  
{
    int32  x;
    int32  y;
    uint32 time;
	MMI_TP_STATUS_E tp_action;

}VELOCITY_TRACKER_ITEM_T;
typedef struct  
{
    VELOCITY_TRACKER_ITEM_T  item[VELOCITY_TRACKER_ITEM_NUM];

    uint8 item_num;

}VELOCITY_TRACKER_T;
/* applet travel state */
typedef uint32 MMI_APPLET_INFO_STATE_T;
#define MMI_APPLET_INFO_BY_ORDER   0x000000001   //˳�����, ����Ϊ����

// use for window parse keyword
#define WIN_KEYWORD(keyword, func, param_num)   \
    func, param_num

// window parse keyword
typedef struct
{
    PARSE_FUNC  parse_func;
    uint32      param_num;
    //char        *keyword_name;
} MMK_KEYWORD_INFO_T;

// ���ڴ���Ե�����
typedef enum
{
    MMK_TEST_LOW_MEM,      // ���ڴ�
    MMK_TEST_NO_MEM,       // ���ڴ�
    MMK_TEST_MEM_NUM
}MMK_TEST_MEMORY_E;
//caf interface
/*****************************************************************************/
// 	Description : ���ݴ��ڱ�������
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_CreateWinTable(
                                       const MMI_WINDOW_TABLE_CREATE_T* create_ptr
                                       );

/*****************************************************************************/
// 	Description : ��������
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_CreateWindow(
                                     const MMI_WINDOW_CREATE_T* create_ptr
                                     );

/*****************************************************************************/
// 	Description : �����ؼ�
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC IGUICTRL_T* MMK_CreateControl(
                                     const MMI_CONTROL_CREATE_T* create_ptr
                                     );

/*****************************************************************************/
// 	Description : ����Ӧ��
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_StartApplet( 
                                    const MMI_APPLET_START_T* start_ptr
                                    );

/*****************************************************************************/
// 	Description : ���ٴ���
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseWin( MMI_HANDLE_T win_handle );

/*****************************************************************************/
// 	Description : ���ٿؼ�
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_DestroyControl( MMI_HANDLE_T ctrl_handle );

/*****************************************************************************/
// 	Description : destroy an appointed control (need control id and win handle)
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_DestroyAppointControl(
                                         MMI_HANDLE_T win_handle,
                                         MMI_HANDLE_T ctrl_handle
                                         );

/*****************************************************************************/
// 	Description : destroy all control of window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_DestroyAllControl(
                                     MMI_HANDLE_T win_handle
                                     );

/*****************************************************************************/
// 	Description : ����Ӧ��
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseApplet( 
                               MMI_HANDLE_T applet_handle
                               );

/*****************************************************************************/
// 	Description : stop applet
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
// PUBLIC BOOLEAN MMK_CloseAppletByGuid( 
//                                      CAF_GUID_T guid
//                                      );

/*****************************************************************************/
// 	Description : close all applet
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseAllDynamicApplet( void );

/*****************************************************************************/
// 	Description : ���ݴ��ھ���Ϳؼ�id��ÿؼ������ָ��
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC IGUICTRL_T* MMK_GetCtrlPtrByWin(
                                       MMI_HANDLE_T   win_handle,
                                       MMI_CTRL_ID_T  ctrl_id
                                       );

/*****************************************************************************/
// 	Description : ���ݴ��ھ���Ϳؼ�id��ÿؼ��ľ��
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetCtrlHandleByWin(
                                           MMI_HANDLE_T win_handle,
                                           MMI_CTRL_ID_T ctrl_id
                                           );

/*****************************************************************************/
//  Description : get root form control handle from the appointed
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetRootFormCtrlHandle(
                                              MMI_HANDLE_T  win_handle
                                              );

/*****************************************************************************/
//  Description : the ancestor_handle is ctrl_handle's ancestor
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsAncestorCtrl(
                                  MMI_HANDLE_T      ancestor_handle,
                                  MMI_HANDLE_T      ctrl_handle
                                  );

/*****************************************************************************/
// 	Description : get ctrl handle by handle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:���ݵ�ǰӦ���µĴ���\�ؼ�\Ӧ�õ�handle���ָ���ؼ���handle
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetCtrlHandleByHandle(
                                              MMI_HANDLE_T  src_handle, // ����\�ؼ�\Ӧ�õ�handle
                                              MMI_WIN_ID_T  win_id,     // win id
                                              MMI_CTRL_ID_T ctrl_id     // control id
                                              );

/*****************************************************************************/
// 	Description : get win handle by handle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:���ݵ�ǰӦ���µĴ���\�ؼ�\Ӧ�õ�handle���ָ�����ڵ�handle
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetWinHandleByHandle(
                                             MMI_HANDLE_T  src_handle, // ����\�ؼ�\Ӧ�õ�handle
                                             MMI_WIN_ID_T  win_id      // win id
                                             );

/*****************************************************************************/
// 	Description : get applet handle by handle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:���ݵ�ǰӦ���µĴ���\�ؼ�\Ӧ�õ�handle���applet��handle
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetAppletHandleByHandle(
                                                MMI_HANDLE_T  src_handle // ����\�ؼ�\Ӧ�õ�handle
                                                );

/*****************************************************************************/
// 	Description : hide applet window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:��applet���д��ڷŵ�zorder system�����ش��ڶ���
/*****************************************************************************/
PUBLIC BOOLEAN MMK_HideAppletWindow(
                                    MMI_HANDLE_T applet_handle,
                                    BOOLEAN      is_hide
                                    );

/*****************************************************************************/
// 	Description : grab window foucus by applet 
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:��ʾapplet�����д���, ���ᵽzorder system�Ķ���
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GrabWinFocusByApplet(
                                        MMI_HANDLE_T applet_handle
                                        );

/*****************************************************************************/
// 	Description : hide win by appelt
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:����applet�����д���
/*****************************************************************************/
PUBLIC BOOLEAN MMK_HideWinByApplet(
                                   MMI_HANDLE_T applet_handle
                                   );

/*****************************************************************************/
// 	Description : hide window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:����\��ʾ����
/*****************************************************************************/
PUBLIC BOOLEAN MMK_HideWindow(
                              MMI_HANDLE_T win_handle,
                              BOOLEAN      is_hide //�Ƿ�����
                              );

/*****************************************************************************/
// 	Description : �ر�applet�����д���
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseAllWinByApplet(
                                       MMI_HANDLE_T applet_handle
                                       );

/*****************************************************************************/
// 	Description : ���ݿؼ������ÿؼ������ָ��
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC IGUICTRL_T* MMK_GetCtrlPtr( 
                                  MMI_HANDLE_T ctrl_handle
                                  );

/*****************************************************************************/
// 	Description : ���ݿؼ������ÿؼ���id
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMK_GetCtrlId(
                                   MMI_HANDLE_T ctrl_handle
                                   );

/*****************************************************************************/
// 	Description : ���ݿؼ��������丸���ڵľ��
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetWinHandleByCtrl( 
                                           MMI_HANDLE_T ctrl_handle
                                           );

/*****************************************************************************/
// 	Description : ����Ӧ�õľ���ʹ��ڵ�id��ô��ڵľ��
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetWinHandle(
                                     MMI_HANDLE_T applet_handle,
                                     MMI_WIN_ID_T win_id
                                     );

/*****************************************************************************/
// 	Description : ����Ӧ�õľ�����Ӧ�û����ָ��
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC IAPPLET_T* MMK_GetAppletPtr(
                                   MMI_HANDLE_T applet_handle
                                   );

/*****************************************************************************/
// 	Description : ���ݴ��ھ�����Ӧ�û����ָ��
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC IAPPLET_T* MMK_GetAppletPtrByWin(
                                        MMI_HANDLE_T win_handle
                                        );

/*****************************************************************************/
// 	Description : ���ݴ��ھ�����Ӧ�õľ��
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetAppletHandleByWin(
                                             MMI_HANDLE_T win_handle
                                             );

/*****************************************************************************/
// 	Description : ����guid���Ӧ�õ�ָ��
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
// PUBLIC IAPPLET_T* MMK_GetAppletPtrByGuid(
//                                          CAF_GUID_T   guid,         //applet��guid
//                                          MMI_HANDLE_T first_handle, //���ҵ���ʼhandle, Ϊ0��Ĭ�ϴ�ͷ\β����
//                                          BOOLEAN      is_order      //˳��\�������
//                                          );

/*****************************************************************************/
// 	Description : ����guid���Ӧ�õľ��
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
// PUBLIC MMI_HANDLE_T MMK_GetAppletHandleByGuid(
//                                               CAF_GUID_T guid
//                                               );

/*****************************************************************************/
// 	Description : set the active control 
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetActiveCtrl(
                                 MMI_HANDLE_T  ctrl_handle,
                                 BOOLEAN       is_need_paint //�Ƿ���Ҫˢ��
                                 );

/*****************************************************************************/
// 	Description : set the active control 
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetAtvCtrl(
                              MMI_HANDLE_T   win_handle,     // the parent window handle
                              MMI_HANDLE_T   ctrl_handle
                              );

/*****************************************************************************/
// 	Description : set the active control 
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetAtvCtrlEx(
                                MMI_HANDLE_T  win_handle, // the parent window handle
                                MMI_HANDLE_T  ctrl_handle, // the control handle
                                BOOLEAN       is_need_paint //�Ƿ���Ҫˢ��
                                );

/******************************************************************************/
/*! \brief get the active control id of the window 
 *  \author Great.Tian 
 *  \param[in]	win_id			the window id 
 *  \return the id of actived control in this window 
 */ 
/******************************************************************************/
PUBLIC MMI_CTRL_ID_T MMK_GetActiveCtrlId( MMI_HANDLE_T win_handle );

/*****************************************************************************/
// 	Description : get the active control id of the window 
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetActiveCtrl(
                                      MMI_HANDLE_T win_handle
                                      );

/*****************************************************************************/
// 	Description : is active ctrl
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsActiveCtrl(
                                MMI_HANDLE_T ctrl_handle
                                );

/******************************************************************************/
/*! \brief check the control id 
 *  \author Great.Tian 
 *  \param[in]	 win_id			the parent window id
 *  \param[in]	 ctrl_id		the control id
 *  \return 
 *		- <i><c>TRUE</c></i>: 	success 	
 * 		- <i><c>FALSE</c></i>:	failed
 */ 
/******************************************************************************/
// PUBLIC BOOLEAN MMK_CheckCtrlId(
//                                MMI_HANDLE_T  win_handle,
//                                MMI_CTRL_ID_T ctrl_id 	
//                                );

/******************************************************************************/
/*! \brief get focus window id 
*  \author Great.Tian 
*  \return the id of focused window.
*/
/******************************************************************************/
MMI_HANDLE_T MMK_GetFocusWinHandle( void );

/*****************************************************************************/
// 	Description : get focus window id
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetFocusParentWinHandle(
                                                void
                                                );

/*******************************************************************/
//  Interface:		MMK_GerFocusMainWinHandle
//  Description : 	MMK_GerFocusMainWinHandle
//  Global resource dependence : 
//  Author: Cheng.luo
//  Note:
/*******************************************************************/
PUBLIC MMI_HANDLE_T MMK_GerFocusMainWinHandle(
                                              void
                                              );

/*****************************************************************************/
// 	Description : get focus window id
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetParentWinHandle(
                                           MMI_HANDLE_T win_handle
                                           );

#ifdef ATEST_SUPPORT
/*****************************************************************************/
//  Description : ��ȡ����ؼ������ڵ�
//  Global resource dependence :
//  Author:xiaolei.zheng
//  Note:
/*****************************************************************************/
PUBLIC MMI_TREE_NODE_T* MMK_GetCtrlTreeRootByWin(
                                            MMI_HANDLE_T win_handle
                                            );

/*****************************************************************************/
//  Description : ��ȡ���㴰��
//  Global resource dependence :
//  Author:xiaolei.zheng
//  Note:
/*****************************************************************************/                                            
PUBLIC MMI_HANDLE_T MMK_GetTopmostWindow(
                                    void
                                    );                                            
#endif // ATEST_SUPPORT
                                            
/******************************************************************************/
/*! \brief get the add data pointer of the window
 *  \author Great.Tian 
 *  \param[in]	 win_id			the window id
 *  \return the data pointer of window
 */
/******************************************************************************/
PUBLIC ADD_DATA MMK_GetWinAddDataPtr( MMI_HANDLE_T win_handle );

/*****************************************************************************/
// 	Description : get the add data pointer of the window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:���ṩ��pub winʹ��
/*****************************************************************************/
PUBLIC ADD_DATA MMK_GetWinAddDataEx(
                                    MMI_HANDLE_T win_handle
                                    );

/******************************************************************************/
/*! \brief the window get the active, all controls of the window lose active
 *  \author Great.Tian 
 *  \param[in]	 win_id 		the window id
 *  \return 
 *		- <i><c>TRUE</c></i>: 	success 	
 * 		- <i><c>FALSE</c></i>:	failed
 */
/******************************************************************************/
PUBLIC BOOLEAN MMK_WinGetActive( MMI_HANDLE_T win_handle );


/*****************************************************************************/
// 	Description : all controls of the window are inactive controls
//	Global resource dependence : 
//  Author:xiaoqing.lu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_WinInactiveCtrl(
                                   MMI_HANDLE_T win_handle,
                                   BOOLEAN is_update
                                   );

/******************************************************************************/
/*! \brief get the prev window id
 *  \author Great.Tian 
 *  \param[in]	 win_id 		the window id
 *  \return the id of prev window
 */
/******************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetPrevWinHandle(
                                         MMI_HANDLE_T win_handle
                                         );

/******************************************************************************/
/*! \brief return the appointed window
 *  \author Great.Tian 
 *  \param[in]	 win_id 		the window id
 *  \return 
 *		- <i><c>TRUE</c></i>: 	return success 	
 * 		- <i><c>FALSE</c></i>:	return failed
 */
/******************************************************************************/
BOOLEAN MMK_ReturnAppointedWin(
                               MMI_WIN_ID_T win_id
                               );

/******************************************************************************/
/*! \brief whether the window is focus
 *  \author Great.Tian 
 *  \param[in]	 win_id			the window id
 *  \return 
 *		- <i><c>TRUE</c></i>: 	focused
 * 		- <i><c>FALSE</c></i>:	not focused
 */
/******************************************************************************/
PUBLIC BOOLEAN MMK_IsFocusWin( MMI_HANDLE_T win_handle );

/******************************************************************************/
/*! \brief whether the anim ctrl is need paint
 *  \author cheng.luo
 *  \param[in]	 win_id			the window id
 *  \return 
 *		- <i><c>TRUE</c></i>: 	yes
 * 		- <i><c>FALSE</c></i>:	no
 */
/******************************************************************************/
PUBLIC BOOLEAN MMK_IsWinNeedPaint(
                                  MMI_HANDLE_T win_handle     //[IN] the window id
                                  );

/*****************************************************************************/
// 	Description : whether the win handle full paint
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsWinOnPaint(
                                MMI_HANDLE_T win_handle     //[IN] the window id
                                );

/******************************************************************************/
/*! \brief the window grabs focus
 *  \author Great.Tian 
 *  \param[in]	 win_id			the window id
 *  \return 
 *		- <i><c>TRUE</c></i>: 	grab focus sucess
 * 		- <i><c>FALSE</c></i>:	grab focus failed
 */
/******************************************************************************/
PUBLIC BOOLEAN MMK_WinGrabFocus( MMI_HANDLE_T win_handle );

/*****************************************************************************/
// 	Description : �ر�first applet���ָ��id��Χ�ڵĴ���
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseMiddleWin(
                                  MMI_WIN_ID_T begin_win_id, 
                                  MMI_WIN_ID_T end_win_id    
                                  );

/*****************************************************************************/
// 	Description : �ر�ָ��applet���ָ��id��Χ�ڵĴ���
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseMiddleWinByApplet(
                                          MMI_HANDLE_T applet_handle,
                                          MMI_WIN_ID_T begin_win_id, 
                                          MMI_WIN_ID_T end_win_id    
                                          );

/*****************************************************************************/
// 	Description : ���ÿؼ���block��Ϣ
//	Global resource dependence : 
//  Author: gang.tong
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetCtrlBlockInfo(
									MMI_HANDLE_T win_handle,
									MMI_CTRL_ID_T ctrl_id,		// �ؼ���ID
									GUI_LCD_DEV_INFO dev_info
                                    );

/*****************************************************************************/
//     Description : set control lcd dev info
//    Global resource dependence : 
//  Author: bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetCtrlLcdDevInfo(
                             MMI_HANDLE_T    ctrl_handle,
                             GUI_LCD_DEV_INFO     *lcd_dev_info_ptr    // [IN] point
                             );

/*****************************************************************************/
//     Description : whether the point is in the control ctl_ptr!
//    Global resource dependence : 
//  Author: Robert.Lu
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_PosIsCtrl(
                             MMI_HANDLE_T    ctrl_handle,
                             GUI_POINT_T     point    // [IN] point
                             );

/*****************************************************************************/
//     Description : the controls notify the parent  window 
//    Global resource dependence : 
//  Author:Robert.Lu
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_DefNotifyProc(                               
                                   MMI_HANDLE_T       win_handle,
                                   MMI_HANDLE_T       ctrl_handle,
                                   MMI_MESSAGE_ID_E   notify_code,  //Notification Code
                                   ADD_DATA           data          //Added data
                                   );

/*****************************************************************************/
//    Description : set the pen capture 
//    Global resource dependence : 
//    Author:Robert Lu
//    Note:
/*****************************************************************************/
// PUBLIC void MMK_SetCapture(
//                            MMI_HANDLE_T cap_handle
//                            );

/*****************************************************************************/
//    Description : get the pen capture 
//    Global resource dependence : 
//    Author:Robert Lu
//    Note:
/*****************************************************************************/
/*PUBLIC MMI_HANDLE_T MMK_GetCapture( void );*/

/*****************************************************************************/
//    Description : release the pen capture 
//    Global resource dependence : 
//    Author:Robert Lu
//    Note:
/*****************************************************************************/
/*PUBLIC void MMK_ReleaseCapture( void );*/

/*****************************************************************************/
// Description : if window include statusbar control
// Global resource dependence : 
// Author:Ming.Song
// Note:2007-8-1
/*****************************************************************************/
BOOLEAN MMK_IsIncludeStatusBar( MMI_HANDLE_T win_handle );

/*****************************************************************************/
// Description : if parent window include statusbar control
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
BOOLEAN MMK_IsParentWinIncStb( MMI_HANDLE_T win_handle );

/*****************************************************************************/
// Description : get win title handle
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetWinTitleHandle(
                                          MMI_HANDLE_T win_handle 
                                          );

/*****************************************************************************/
// Description : get win title handle
// Global resource dependence : 
// Author:xiaoqing.lu
// Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetWinStatusBarHandle(
                                              MMI_HANDLE_T win_handle 
                                              );

/*****************************************************************************/
// Description : if window include tips control
// Global resource dependence : 
// Author:
// Note:
/*****************************************************************************/
BOOLEAN MMK_IsIncludeTips( MMI_HANDLE_T win_handle );

/*****************************************************************************/
// 	Description : Set Hardware Icon Is Enable
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMK_SetTPHardwareIconIsEnable(
                                          BOOLEAN is_hw_icon_enable,
                                          uint16 max_hw_icon_count
                                          );

/*****************************************************************************/
// 	Description : Set Hardware Icon Button For TP
//	Global resource dependence : 
//  Author: wancan.you
//	Note:
/*****************************************************************************/
PUBLIC void MMK_SetTPHardwareIcon(
                                  BOOLEAN is_cur_hw_icon_enable,
                                  MMI_HW_ICON_T *hardware_icon_ptr,
                                  uint16 hw_icon_index
                                  );

/*****************************************************************************/
//     Description : set window move type
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC void MMK_SetWinDefaultAnimMoveStyle(MMI_WIN_ANIM_MOVE_SETTING_TYPE_E type);

/*****************************************************************************/
//     Description : get window move type
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ANIM_MOVE_SETTING_TYPE_E MMK_GetWinDefaultAnimMoveStyle(void);

/*****************************************************************************/
// Description : handle window move type by msg
// Global resource dependence : 
// Author: ryan.xu
// Note:
/*****************************************************************************/
PUBLIC void MMK_HandleWinMoveMsg(
                                 MMI_HANDLE_T win_handle, 
                                 MMI_MESSAGE_ID_E msg,
                                 DPARAM param
                                 );


/*****************************************************************************/
// Description : handle window pre-moving type by msg
// Global resource dependence : 
// Author: Yintang.Ren
// Note:
/*****************************************************************************/
PUBLIC void MMK_HandleWinPreMoveMsg(
                                    MMI_HANDLE_T     win_handle,
                                    MMI_MESSAGE_ID_E msg,
                                    DPARAM           param
                                    );

/*****************************************************************************/
// 	Description : get the window list pointer  
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_TYPE_E MMK_GetHandleType(
                                           MMI_HANDLE_T handle
                                           );

/*****************************************************************************/
// 	Description : get the guid by handle
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC CAF_GUID_T MMK_GetGuidByHandle(
                                      MMI_HANDLE_T handle
                                      );

/*****************************************************************************/
//     Description : set window priority
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinPriority( 
                                  MMI_HANDLE_T win_handle,
                                  MMI_WIN_PRIORITY_E win_priority
                                  );

/*****************************************************************************/
//     Description : set window priority
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC MMI_WIN_PRIORITY_E MMK_GetWinPriority( 
                                             MMI_HANDLE_T win_handle
                                             );

/*****************************************************************************/
//     Description : Get window move style
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC WINDOW_MOVE_STYLE MMK_GetWinMoveStyle(MMI_HANDLE_T win_handle);

/*****************************************************************************/
//     Description : set window id
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinId( 
                            MMI_HANDLE_T win_handle,
                            MMI_WIN_ID_T win_id
                            );

/*****************************************************************************/
//     Description : get window id
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMK_GetWinId( 
                                 MMI_HANDLE_T win_handle
                                 );

/*****************************************************************************/
//     Description : set window id
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinFunc( 
                              MMI_HANDLE_T        win_handle,
                              PROCESSMSG_FUNC func
                              );

/*****************************************************************************/
//     Description : set window hook function
//    Global resource dependence : 
//  Author: hua.fang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinHookFunc(
                                  MMI_HANDLE_T      win_handle,
                                  PROCESSMSG_FUNC   func
                                  );

/*****************************************************************************/
//     Description : set window display style
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinDisplayStyleState( 
                                           MMI_HANDLE_T         win_handle,
                                           WINDOW_DISPLAY_STYLE style,
                                           BOOLEAN              is_true
                                           );

/*****************************************************************************/
//     Description : get window display style
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetWinDisplayStyleState( 
                                           MMI_HANDLE_T         win_handle,
                                           WINDOW_DISPLAY_STYLE style
                                           );

/*****************************************************************************/
//     Description : get window display style
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetWinDisplayStyle( 
                                      MMI_HANDLE_T          win_handle,
                                      WINDOW_DISPLAY_STYLE* style_ptr
                                      );

/*****************************************************************************/
//     Description : set window move style
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinMoveStyle( 
                                   MMI_HANDLE_T      win_handle,
                                   WINDOW_MOVE_STYLE style
                                   );

/*****************************************************************************/
//     Description : set window common bg on bottom
//    Global resource dependence : 
//  Author:bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinCommonBgOnBottom( 
                                          MMI_HANDLE_T win_handle,
                                          BOOLEAN is_common_bg_on_bottom
                                          );

/*****************************************************************************/
//     Description : set window bg image id
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinBackgroundImageId( 
                                           MMI_HANDLE_T   win_handle,
                                           MMI_IMAGE_ID_T image_id
                                           );

/*****************************************************************************/
//     Description : get window bg image id
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T MMK_GetWinBackgroundImageId( 
                                                  MMI_HANDLE_T   win_handle
                                                  );

/*****************************************************************************/
//     Description : get window title id
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinRect( 
                              MMI_HANDLE_T   win_handle,
                              const GUI_RECT_T*    rect_ptr
                              );

/*****************************************************************************/
//     Description : get window title id
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetWinRect( 
                              MMI_HANDLE_T   win_handle,
                              GUI_RECT_T*    rect_ptr
                              );

/*****************************************************************************/
//     Description : get window dc
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC IDC_T* MMK_GetDC( 
                        MMI_HANDLE_T handle
                        );
//internal interface
/******************************************************************************/
/*! \brief init the system struct 
 *  \author Great.Tian 
 *  \return 
 *		- <i><c>TRUE</c></i>: 	init sucess	
 * 		- <i><c>FALSE</c></i>:	init failed 
 */ 
/******************************************************************************/
BOOLEAN MMK_InitSystem(void);


/******************************************************************************/
/*! \brief return to the idle window
 *  \author Great.Tian 
 *  \return the id of idle window
 */
/******************************************************************************/
BOOLEAN MMK_ReturnIdleWin(void);

/*****************************************************************************/
// 	Description : ���ϵͳ�еĵ�һ��Ӧ�õľ��, ��idle applet�ľ��
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: 
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetFirstAppletHandle( void );

/******************************************************************************/
/*! \brief create a window in parseing window table
 *  \author Great.Tian 
 *  \param[in]	 win_table_ptr	the pointer fo win table
 *  \param[in]	 add_data_ptr	the additional data
 *  \return 
 *		- <i><c>TRUE</c></i>: 	return success 	
 * 		- <i><c>FALSE</c></i>:	return failed
 */
/******************************************************************************/
PUBLIC MMI_HANDLE_T MMK_CreateWin(
                                  uint32*            win_table_ptr,
                                  ADD_DATA           add_data_ptr
                                  );

/*****************************************************************************/
// 	Description : create win by applet guid
//	Global resource dependence : 
//  Author James.Zhang
//	Note: ���޾�̬applet��������ʹ��
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_CreateWinByApplet(
                                          MMI_HANDLE_T  applet_handle,
                                          uint32*       win_table_ptr,
                                          ADD_DATA      add_data_ptr
                                          );

/*****************************************************************************/
// 	Description : free the window addtional data
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_FreeWinAddData(
                                  MMI_HANDLE_T win_handle
					              );

/*****************************************************************************/
// 	Description : free the window addtional data
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:���ṩ��pub winʹ��
/*****************************************************************************/
PUBLIC BOOLEAN MMK_FreeWinAddDataEx(
                                    MMI_HANDLE_T win_handle
                                    );

/******************************************************************************/
/*! \brief check the window is open 
 *  \author Great.Tian 
 *  \param[in]	win_id			the window id 
 *  \return 
 *		- <i><c>TRUE</c></i>: 	the window is open 	
 * 		- <i><c>FALSE</c></i>:	the window is not open
 */ 
/******************************************************************************/
PUBLIC BOOLEAN MMK_IsOpenWin(
                             MMI_WIN_ID_T win_id
                             );

/*****************************************************************************/
// 	Description : get the window pointer that is processing the message 
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetProcMsgHandle(
                                         void
                                         );

/*****************************************************************************/
// 	Description : set the window pointer that is processing the message 
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetProcMsgHandle(
                                    MMI_HANDLE_T handle
                                    );

/*****************************************************************************/
// 	Description : get the focus window id,if have parent win,return parent win id
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMK_GetFocusWinId(void);

/*****************************************************************************/
// 	Description : get the focus child window id
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMK_GetFocusChildWinId(void);

/*****************************************************************************/
// 	Description : Get the prev window id
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_WIN_ID_T MMK_GetPrevWinId(
                                     MMI_WIN_ID_T win_id // the window id
                                     );

/*****************************************************************************/
//     Description : switch to focus tab win
//    Global resource dependence : 
//  Author:jibin
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseParentWin(
                                  MMI_HANDLE_T parent_win_handle
                                  );

/*****************************************************************************/
//     Description : switch to focus child win
//    Global resource dependence : 
//  Author:jibin
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetChildWinFocus(
									 MMI_WIN_ID_T parent_win_id,
									 MMI_WIN_ID_T win_id
									 );

/*****************************************************************************/
//     Description : �����Ӵ���
//    Global resource dependence : 
//  Author:jibin
//    Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T  MMK_CreateChildWin(
                                   MMI_WIN_ID_T	parent_win_id,
                                   uint32*		win_table_ptr,
                                   ADD_DATA		add_data_ptr		// the additional data
                                   );

/*****************************************************************************/
//     Description : create tab win
//    Global resource dependence : 
//  Author:jibin
//    Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_CreateParentWin(
                                uint32*            win_table_ptr,
                                ADD_DATA           add_data_ptr		// the additional data
                                );

/*****************************************************************************/
// 	Description : destroy a dyanmic control (need control id)
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_DestroyDynaCtrl(
                                   MMI_CTRL_ID_T    ctrl_id     // the parent window id
                                   );

/*****************************************************************************/
// 	Description : is child window
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsChildWin(
                              MMI_HANDLE_T win_handle
                              );

/******************************************************************************/
/*! \brief get the parent ctrl handle
*  \author Great.Tian 
*  \param[in]	 ctrl handle
*  \return the parent ctrl handle
*/
/******************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetParentCtrlHandle(
                                            MMI_HANDLE_T ctrl_handle
                                            );



/*****************************************************************************/
// 	Description : set the window title string
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMK_GetWinSoftkeyCtrlId(
                                             MMI_HANDLE_T		win_handle
                                             );

/*****************************************************************************/
// 	Description : set the window title string
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinSoftkeyCtrlId(
									   MMI_HANDLE_T		win_handle,
									   MMI_CTRL_ID_T	softkey_ctrl_id
									   );

/*****************************************************************************/
// 	Description : ��idת��Ϊ���
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: id���ƺ;�������Ǽ��ݵ�, ��ʹ�þ����������߲�ѯ��ָ���Ч��
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_ConvertIdToHandle(
                                          uint32 id
                                          );

/*****************************************************************************/
// 	Description : set the window title string
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinTitleCtrlId(
									   MMI_HANDLE_T		win_handle,
									   MMI_CTRL_ID_T	title_ctrl_id
									   );

/*****************************************************************************/
// 	Description : set the window title string
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMK_GetWinTitleCtrlId(
                                           MMI_HANDLE_T		win_handle
                                           );

/*****************************************************************************/
// 	Description : is exist window
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:�жϴ����Ƿ����, ����رմ�����Ϣ�д�id��ͬ���ڵ�����
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsExistWin(
                              MMI_HANDLE_T applet_handle,
                              MMI_WIN_ID_T win_id
                              );

/*****************************************************************************/
// 	Description : delay update screen
//	Global resource dependence : 
//  Author:James.Zhang
//	Note: Ϊ����ˢ��, �ӳ�ˢ����Ļ
/*****************************************************************************/
PUBLIC void MMK_DelayUpdateScreen( void );

/*****************************************************************************/
//     Description : set applet state
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetAppletState( 
                                  MMI_HANDLE_T       applet_handle,
                                  MMI_APPLET_STATE_T state,
                                  BOOLEAN            is_true
                                  );

/*****************************************************************************/
//     Description : get applet state
//    Global resource dependence : 
//  Author:gang.tong
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetAppletState( 
                                  MMI_HANDLE_T       applet_handle,
                                  MMI_APPLET_STATE_T state
                                  );

/*****************************************************************************/
// 	Description : open a window
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_OpenWin(
                           MMI_HANDLE_T win_handle,
                           MMI_HANDLE_T parent_win_handle
                           );

/*****************************************************************************/
// 	Description : create applet info list
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:����applet��Ϣlist
/*****************************************************************************/
PUBLIC MMI_APPLET_INFO_LIST_T* MMK_CreateAppletInfoList(
                                                        MMI_APPLET_INFO_STATE_T state
                                                        );

/*****************************************************************************/
// 	Description : destory applet info list
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:����applet��Ϣ��list
/*****************************************************************************/
PUBLIC BOOLEAN MMK_DestroyAppletInfoList( 
                                         MMI_APPLET_INFO_LIST_T* list_ptr
                                         );

/*****************************************************************************/
// 	Description : start DLL
//	Global resource dependence : 
//  Author:andrew.Zhang
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_StartDLL(const MMI_APPLET_START_T* start_ptr, void **object_pptr);


/*****************************************************************************/
// 	Description : stop DLL
//	Global resource dependence : 
//  Author:andrew.zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CloseDLL( 
                               MMI_HANDLE_T applet_handle
                               );

/*****************************************************************************/
// 	Description : check the applet is active or not
//	Global resource dependence : 
//  Author:aoke.hu
//	Note:�ж�applet�Ǵ���ǰ̨���Ǻ�̨����
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsActiveApplet(
                                  MMI_HANDLE_T applet_handle
                                  );

/*****************************************************************************/
//     Description : set window lcd dev info and notify ctrl
//    Global resource dependence : 
//  Author:bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinLcdDevInfoNotifyCtrl( 
                            MMI_HANDLE_T win_handle,
                            const GUI_LCD_DEV_INFO *dev_info_ptr  //in
                            );

/*****************************************************************************/
//     Description : get window lcd dev info
//    Global resource dependence : 
//  Author:bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetWinLcdDevInfo( 
                                    MMI_HANDLE_T win_handle,
                                    GUI_LCD_DEV_INFO *dev_info_ptr  //out
                                    );
    
/*****************************************************************************/
//     Description : set window lcd dev info
//    Global resource dependence : 
//  Author:bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinLcdDevInfo( 
                                    MMI_HANDLE_T win_handle,
                                    const GUI_LCD_DEV_INFO *dev_info_ptr  //in
                                    );

/*****************************************************************************/
//     Description : set window both rect and notify ctrl
//    Global resource dependence : 
//  Author:bin.ji
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetBothRectNotifyCtrl( 
                               MMI_HANDLE_T           handle,
                               const GUI_BOTH_RECT_T* both_rect_ptr
                               );

/*****************************************************************************/
// 	Description : is update screen
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:����Ӧ�÷�full paintˢ�º�ϵͳˢ��
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsUpdateScreen( void );

/*****************************************************************************/
// 	Description : update screen
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_UpdateScreen( void );


/*****************************************************************************/
//     Description : get both rect
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetBothRect( 
                               MMI_HANDLE_T           handle,
                               const GUI_BOTH_RECT_T* both_rect_ptr
                               );

/*****************************************************************************/
//     Description : get both rect
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetBothRect( 
                               MMI_HANDLE_T     handle,
                               GUI_BOTH_RECT_T* both_rect_ptr
                               );

/*****************************************************************************/
//     Description : set win both rect
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinBothRect( 
                                  MMI_HANDLE_T           handle,
                                  const GUI_BOTH_RECT_T* both_rect_ptr
                                  );

/*****************************************************************************/
//     Description : get win both rect
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetWinBothRect( 
                                  MMI_HANDLE_T     handle,
                                  GUI_BOTH_RECT_T* both_rect_ptr
                                  );

/*****************************************************************************/
//     Description : get handle by pos
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetHandleByPos( 
                                  GUI_POINT_T   point,
                                  BOOLEAN       is_find_parent,
                                  MMI_HANDLE_T* win_handle_ptr,
                                  MMI_HANDLE_T* ctrl_handle_ptr,
                                  MMI_HANDLE_T  exclude_handle //�ų���ctrl handle, ����ҵ�, ��Ҫ��������һ��
                                  );

/*****************************************************************************/
//  Description : get handle of child control that is at the position of screen
//  Global resource dependence : 
//  Author:Jassmine
//  Note: ����λ�ò���ĳ�ؼ��µ������ӿؼ�
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetChildHandleByPos(
                                       GUI_POINT_T  point,              //in:
                                       MMI_HANDLE_T parent_handle,      //in:
                                       MMI_HANDLE_T *child_handle_ptr   //in/out:
                                       );

/*****************************************************************************/
//  Description : MMK_SetScreenAngle
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: ����ƽ̨ȫ�ֽǶ�,ֻ��gsensor\�Ƹ�\����ģʽ��ʹ��,��ͨapp��ֹ����
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetScreenAngle( 
                                  LCD_ANGLE_E angle
                                  );

/*****************************************************************************/
//  Description : MMK_GetScreenAngle
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: ���ƽ̨ȫ�ֽǶ�
/*****************************************************************************/
PUBLIC LCD_ANGLE_E MMK_GetScreenAngle( void );

/*****************************************************************************/
//  Description : get window angle
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetWinAngle(
                               MMI_HANDLE_T     win_handle, //in:
                               LCD_ANGLE_E      *angle_ptr  //in/out:
                               );

/*****************************************************************************/
//  Description : MMK_SetScreenAngle
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: ���õ�ǰ���ڽǶ�,����ı�ƽ̨ȫ�ֽǶ�
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinAngle( 
                               MMI_HANDLE_T win_handle,
                               LCD_ANGLE_E angle,
                               BOOLEAN is_update
                               );

/*****************************************************************************/
//     Description : set window support angle
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinSupportAngle( 
                                      MMI_HANDLE_T         win_handle,
                                      WINDOW_SUPPORT_ANGLE angle
                                      );

/*****************************************************************************/
//     Description : Get window support angel
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC WINDOW_SUPPORT_ANGLE MMK_GetWinSupportAngle(
                                                   MMI_HANDLE_T win_handle
                                                   );

/*****************************************************************************/
//     Description : Whether the control has response to  the pressing the touch pannel.
//    Global resource dependence : 
//  Author:Robert Lu
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsResponseToClick(
                                MMI_HANDLE_T   ctl_handle
                                );

/*****************************************************************************/
// 	Description : is window landscape
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:����ȡ��MMITHEME_IsMainScreenLandscape�ӿ�, ȫ�ֱ������ܺ�win��angle��һ��
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsWindowLandscape( 
                                     MMI_HANDLE_T win_handle
                                     );

/*****************************************************************************/
// 	Description : �����ٶȸ�����
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC void* MMK_CreateTracker( void );

/*****************************************************************************/
// 	Description : �ͷ��ٶȸ�����
//	Global resource dependence :
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_DestroyTracker(
                               void    *tracker_ptr
                               );

/*****************************************************************************/
// 	Description : is tp single click
//	Global resource dependence : 
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsSingleClick( 
                                 MMI_MESSAGE_ID_E   msg_id, 
                                 DPARAM             param
                                 );

#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
// 	Description : ����ٶȵ�
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_AddVelocityItemByTracker(
                                         void    *tracker_ptr,
                                         int32   x,
                                         int32   y
                                         );

/*****************************************************************************/
// 	Description : �����ٶȵ�
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_ResetVelocityItemByTracker(
                                           void    *tracker_ptr
                                           );

/*****************************************************************************/
// 	Description : ���㵱ǰ�ٶ�
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_ComputeCurrentVelocityByTracker(
                                                   void   *tracker_ptr,
                                                   float  * vx_ptr, // x�����ٶ�, in/out
                                                   float  * vy_ptr, // y�����ٶ�, in/out
                                                   float  max_x_velocity,
												   float  max_y_velocity
                                                   );

/*****************************************************************************/
// 	Description : ����ٶȵ�
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
/*PUBLIC void MMK_AddVelocityItem(
                                int32 x,
                                int32 y
                                );*/

/*****************************************************************************/
// 	Description : �����ٶȵ�
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_ResetVelocityItem( void );

/*****************************************************************************/
// 	Description : ���㵱ǰ�ٶ�
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_ComputeCurrentVelocity(
                                          float* vx_ptr, // x�����ٶ�
                                          float* vy_ptr, // y�����ٶ�
                                          float max_x_velocity,
										  float max_y_velocity
                                          );
/*****************************************************************************/
// 	Description : ��ȡ��ǰ�ٶ�׷��������Ϣ
//	Global resource dependence :
//  Author: longwei.qiao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetCurrentVelocityTracker(VELOCITY_TRACKER_T* velocity_tracker_ptr);//in ,out;

/*****************************************************************************/
// 	Description : ����ٶȵ�
//	Global resource dependence :
//  Author:arvin.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMK_AddVelocityItemByApp(
                                     VELOCITY_TRACKER_T *velocity_tracker_ptr,
                                     int32 x,
                                     int32 y,
                                     MMI_TP_STATUS_E tp_action
                                     );

/*****************************************************************************/
// 	Description : �����ٶȵ�
//	Global resource dependence :
//  Author: arvin.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMK_ResetVelocityItemByApp( VELOCITY_TRACKER_T *velocity_tracker_ptr);

/*****************************************************************************/
// 	Description : ���㵱ǰ�ٶ�
//	Global resource dependence :
//  Author: arvin.wu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_ComputeCurrentVelocityByApp(
                                               VELOCITY_TRACKER_T *velocity_tracker_ptr,
                                               float *vx_ptr, // x�����ٶ�, in/out
                                               float *vy_ptr, // y�����ٶ�, in/out
                                               float max_x_velocity,
                                               float max_y_velocity
                                               );

/*****************************************************************************/
// 	Description : 
//	Global resource dependence :
//  Author: James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC int32 MMK_GetFlingOffset( 
                                int32  cur_pos,
                                float  velocity,
                                float  time,
                                float* out_velocity
                                );

/*****************************************************************************/
//  Description : �ж��Ƿ�ı��˷���
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsWayChanged(
                                float            cur_velocity, // ��ǰ���ٶ�
                                GUI_POINT_T      *start_point_ptr, // ��ʼ��
                                GUI_POINT_T      *cur_point_ptr, // ��ǰ��
                                BOOLEAN          is_x_way  // �Ƿ�x����
                                ); 

/*****************************************************************************/
//  Description : ������Ϣ
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: from tim.xia
/*****************************************************************************/
PUBLIC void MMK_ResetSlideRecoverInfo(void);

/*****************************************************************************/
//  Description : �����ƶ���y����ԭʼƫ��
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMK_SetRecoverOrigY(int32 orig_offset_y);

/*****************************************************************************/
//  Description : �����ƶ���x����ԭʼƫ��
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMK_SetRecoverOrigX(int32 orig_offset_x);

/*****************************************************************************/
//  Description : �����ƶ���ʣ��λ��
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMK_SetRecoverOddDistance(int32 odd_distance);

/*****************************************************************************/
//  Description : �����ƶ���ԭʼλ��
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC void MMK_SetRecoverRunDistance(int32 run_distance);

/*****************************************************************************/
//  Description : ����ƶ���y����ԭʼƫ��
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMK_GetRecoverOrigY(void);

/*****************************************************************************/
//  Description : ����ƶ���x����ԭʼƫ��
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMK_GetRecoverOrigX(void);

/*****************************************************************************/
//  Description : ����ƶ���ʣ��λ��
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMK_GetRecoverOddDistance(void);

/*****************************************************************************/
//  Description : ����ƶ���ԭʼλ��
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMK_GetRecoverRunDistance(void);
#endif
/*****************************************************************************/
//  Description : ���������˶���λ��ƫ��
//  Global resource dependence : 
//  Author:xiaoqing.lu
//  Note: from tim.xia
/*****************************************************************************/
PUBLIC int32 MMK_ClcltDampPlayOffset(
                                     MMK_DAMP_PLAY_TYPE_E   type,		//�˶�����
                                     int16	                iLen,			//�˶���λ��
                                     int32                  iTotalFrame,	//��֡��
                                     int32                  iNowFrameNum	//��ǰ֡
                                     );


/*****************************************************************************/
//  Description : ����guid��ÿؼ�handle
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetCtrlHandleByGuid(
                                            MMI_HANDLE_T win_handle,
                                            CAF_GUID_T   guid
                                            ); 

/*****************************************************************************/
//  Description : draw all child ctrl
//  Global resource dependence : 
//  Author:James.Zhang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_DrawAllChildCtrl(
                                    MMI_HANDLE_T ctrl_handle
                                    );

/*****************************************************************************/
//     Description : dispatch the touch pannel message after translate the signal of RTOS to
//                MMI Message
//    Global resource dependence : 
//  Author:Robert Lu
//    Note:
/*****************************************************************************/
PUBLIC void MMK_RedispatchMSGTpDown(
                                    uint16 x,
                                    uint16 y
                                    );

/*****************************************************************************/
// 	Description : redispatch keypress
//	Global resource dependence : none
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_RedispatchKeypress(
                                      MMI_MESSAGE_ID_E    msg_id,     //the identify of message
                                      DPARAM              param_ptr   //the param of the message
                                      );

//#ifdef GUI_CTRL_SCROLL_KEY
/*****************************************************************************/
// 	Description : set the window title string
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC MMI_CTRL_ID_T MMK_GetWinScrollkeyCtrlId(
                                             MMI_HANDLE_T		win_handle
                                             );

/*****************************************************************************/
// 	Description : set the window title string
//	Global resource dependence : 
//  Author:Great.Tian
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinScrollkeyCtrlId(
									   MMI_HANDLE_T		win_handle,
									   MMI_CTRL_ID_T	scrollkey_ctrl_id
									   );
//#endif

/*****************************************************************************/
//     Description : get applet start time
//    Global resource dependence : 
//  Author:fen.xie
//    Note:
/*****************************************************************************/
PUBLIC uint32 MMK_GetAppletStartTime( 
                                     MMI_HANDLE_T       applet_handle
                                     );



/*****************************************************************************/
// 	Description : is handle valid or not?
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsHandleValid(
                                 MMI_HANDLE_T handle
                                 );

/*****************************************************************************/
// 	Description : get focus child win handle
//	Global resource dependence : 
//  Author:xiaoming.ren
//	Note:
/*****************************************************************************/
PUBLIC MMI_HANDLE_T MMK_GetFocusChildWinHandle(
                                       MMI_HANDLE_T parent_handle
                                       );

/*****************************************************************************/
// 	Description : get all child win handle
//	Global resource dependence : 
//  Author:hua.fang
//	Note:
// return actual child win number
/*****************************************************************************/
PUBLIC uint16 MMK_GetAllChildWinHandle(
                                       MMI_HANDLE_T   parent_handle,
                                       MMI_HANDLE_T   *child_win_arr_ptr,
                                       uint16         child_win_arr_size
                                       );

/*****************************************************************************/
// 	Description : check win active while ctrl is hiden or shown
//	Global resource dependence : 
//  Author: hua.fang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_CheckActive(
                               MMI_HANDLE_T ctrl_handle,
                               BOOLEAN      is_hiden
                               );

/*****************************************************************************/
// 	Description : is win need copy lcd when lose focus
//	Global resource dependence : 
//  Author:Arvin.wu
//	Note:
/*****************************************************************************/
PUBLIC void MMK_SetWinNeedCopyLcdInLoseFocus(
                                                                                                MMI_HANDLE_T		win_handle,
                                                                                                BOOLEAN is_need_copy
                                                                                                );
/*****************************************************************************/
// 	Description : is win need copy lcd when lose focus
//	Global resource dependence : 
//  Author:Arvin.wu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_IsWinNeedCopyLcdInLoseFocus(MMI_HANDLE_T	win_handle);
#ifdef GUI_DUMP_CTRL_TREE
/*****************************************************************************/
// 	Description : dump focus window
//	Global resource dependence : 
//  Author:hua.fang
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMK_DumpCurrentWindow(
                                    MMI_HANDLE_T    *form_arr_ptr,
                                    uint16          form_arr_size
                                    );

/*****************************************************************************/
// 	Description : dump all window
//	Global resource dependence : 
//  Author:hua.fang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_DumpAllWindows(void);
#endif

/*****************************************************************************/
//  Description : get static applet ptr
//  Global resource dependence : 
//  Author: James.Zhang
//  Note: 
/*****************************************************************************/
PUBLIC const CAF_STATIC_APPLET_T* MMK_GetStaticAppletPtr( 
                                                         CAF_GUID_T guid
                                                         );

/*****************************************************************************/
//     Description : test memory
//    Global resource dependence : 
//  Author:   James.Zhang
//    Note:�����ٷ���buf_size��ʣ���ڴ��Ƿ��ǵ��ڴ�
/*****************************************************************************/
PUBLIC BOOLEAN MMK_TestMemory(
                              MMK_TEST_MEMORY_E type,
                              uint32            buf_size
                              );

/*****************************************************************************/
// 	Description : clear bg apllet
//	Global resource dependence : 
//  Author:aoke.hu
//	Note:���ڴ�ʱ���Զ��رպ�̨���ڴ�Ӧ��
/*****************************************************************************/
PUBLIC BOOLEAN MMK_ClearBgApplet( void );

/*****************************************************************************/
// 	Description : print info
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_PrintInfo(void);
PUBLIC BOOLEAN MMK_SetFocusWin(
                          MMI_HANDLE_T win_handle
                          );

/*****************************************************************************/
//  Description:    check mem overwrite
//	Global resource dependence: 
//  Author: 		James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC void MMK_CheckAllocatedMemInfo(
                                      const char* file_name_ptr,
                                      uint32 line
                                      );

#if !(defined PLATFORM_SC8501C) && (defined WIN32 || defined MMI_CHECK_MEMINFO_SUPPORT)
#define MMI_CheckAllocatedMemInfo() MMK_CheckAllocatedMemInfo( _D_FILE_NAME, _D_FILE_LINE )
#else
#define MMI_CheckAllocatedMemInfo() 
#endif

/*****************************************************************************/
//     Description : set child win lost focus
//    Global resource dependence : 
//  Author:James.Zhang
//    Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetChildWinLostFocus(
                                        MMI_HANDLE_T win_handle,
                                        BOOLEAN is_update
                                        );

/*****************************************************************************/
// 	Description : set window user data
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinUserData(
                                  MMI_HANDLE_T win_handle,
                                  ADD_DATA     data_ptr
                                  );

/*****************************************************************************/
// 	Description : get window user data
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC ADD_DATA MMK_GetWinUserData(
                                   MMI_HANDLE_T win_handle
                                   );

/*****************************************************************************/
// 	Description : set window dirty
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_SetWinDirty(
                               MMI_HANDLE_T win_handle,
                               BOOLEAN is_dirty
                               );

/*****************************************************************************/
// 	Description : get window dirty
//	Global resource dependence : 
//  Author:James.Zhang
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_GetWinDirty(
                               MMI_HANDLE_T win_handle
                               );

/*----------------------------------------------------------------------------*/
/*                         Compiler Flag                                      */
/*----------------------------------------------------------------------------*/

#ifdef   __cplusplus
    }
#endif

#endif /* END _MMK_APP_H_  */
/*! @} */
