/*****************************************************************************
** File Name:      mmiidle_func.h                                            *
** Author:                                                                   *
** Date:           11/07/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to describe punctuation                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2011        jian.ma              Create                                 *       
** 02/2012        murphy.xie           Modify some interface                 *
******************************************************************************/

#ifndef _MMIIDLE_FUNC_H_
#define _MMIIDLE_FUNC_H_

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "sci_types.h"
#include "mmk_type.h"
#include "mmi_font.h"
#include "mmi_common.h"
#include "mmi_nv.h"
#include "guistring.h"
#include "mn_type.h"
#include "mmi_signal_ext.h"
//#include "mmiset.h"
#include "mmiset_export.h"
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
//������״̬��ʾ
typedef uint8  BLUETOOTH_STATE_TYPE;
#ifdef BT_DIALER_SUPPORT
#define ON_AND_DISCONNECT  0 //������δ����
#define ON_AND_CONNECT     1 //������������ 
#else
#define ON_AND_VISIBLE  0 //�������ҿɼ�
#define ON_AND_HIDDEN   1 //������������
#endif
#ifdef ADULT_WATCH_SUPPORT
#define ON_AND_DISCONNECT  0 //������δ����
#define ON_AND_CONNECT     1 //������������ 
#endif
#define OFF_BLUETOOTH   3 //�����ر�
#ifdef MAINLCD_SIZE_128X160
#define MMI_IDLE_SIM_ICON_INTERVAL 0  //sim icon������/128X160�Ŀ��̫С��ʾ����please insert SIM card,������ʾplease insert SIM ��card������Ļ
#else
#define MMI_IDLE_SIM_ICON_INTERVAL 4  //sim icon������
#endif
/**--------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
// @Great.Tian, ���岦�Ŵ����е�PhoneNum����
#define MMIDIAL_PHONENUM_LEN 40
#define MMIIDLE_TIME_LEN        10
#define MMIIDLE_COUNT_TIME_LEN        20//10
//#define MMI_MAIN_SPN_MAX_LEN  50
// the idle window information
// ʵ�������µ���������Щ�����ǿ���ֱ�ӻ�ȡ�ġ�
typedef struct
{
    MN_DUAL_SYS_E   dual_sys;
    uint8           time_id;
    uint8           rxlevel[MMI_DUAL_SYS_MAX];       // С���ź�ǿ��(0-5, 6 level)����phoneapp�����ã� APP_MN_SCELL_RSSI_IND
    uint8           batlevel;      // ��ص���(0-5, 6 level)����phoneapp�����ã� CHR_CAP_IND
#ifdef MMI_DUAL_BATTERY_SUPPORT
    uint8           batlevel_aux; 
#endif
    //CALL_STATE_TYPE call_state;    // call״̬����callapp������
    //BOOLEAN         call_is_flash; // ֻ�е�call_state == NEW_CALLʱ����Ч
    BOOLEAN         is_gprs[MMI_DUAL_SYS_MAX];      //�Ƿ�֧��GPRS
    BOOLEAN         is_roaming[MMI_DUAL_SYS_MAX];		//�Ƿ�����
    BOOLEAN         is_calling;  //��ǰ�Ƿ���ͨ����
    BOOLEAN         is_missed_call; // ��ǰ�Ƿ���δ������
    BOOLEAN         is_alarm;      // ��ǰ�Ƿ��������ӡ���setapp������
    BOOLEAN         is_shake;      // ��ǰ�Ƿ������𶯡���setapp������
    BOOLEAN         is_silent;     // ��ǰ�Ƿ����þ�������setapp������
    BOOLEAN         is_charge; //�Ƿ��ڳ��
    BOOLEAN         is_display_mp3; //�Ƿ񲥷�mp3
    BOOLEAN         is_display_spn; //��ǰ�Ƿ�Ҫ��ʾSPN(Service Providor Number)
    BOOLEAN			is_forward[MMI_DUAL_SYS_MAX];		//��ǰ�Ƿ������˺���ת��

    BOOLEAN         is_display_stk_idle_text; //�Ƿ���ʾSTK IDLE Text

    uint8           rxlevel_step[MMI_DUAL_SYS_MAX];       // ��¼��ǰ��ʾ��С���ź�ǿ��(0-5, 6 level)����phoneapp�����ã� APP_MN_SCELL_RSSI_IND

    uint8           cur_batlevel; //��ǰ��ص���
    // ������Ӫ�̣�ʱ�����ڣ�����ͼƬ,��Ļ�����ȵȡ�
    // ������Ļ������������������һ�����ڣ��������֮��ͻص�Idle���ڡ�
    // ������������Call��ʱ����Ļ�������ھ�û�б�ɱ����
    MMI_IMAGE_ID_T  image_id;      // ��setapp������Idle�µı���ͼƬ,���ͨ��ID�ò���Data�������Ĭ�ϵ�
    BLUETOOTH_STATE_TYPE   bluetooth_state; //������״̬
//add for cphs icon begin
    uint8           line_state[MMI_DUAL_SYS_MAX];           //ͨ����·��״̬����setapp�����ã�0Ϊ��·1,1Ϊ��·2
    BOOLEAN         is_new_vm;          //�Ƿ����µ�������Ϣ������������·1����·2
//add for cphs icon end
}
IDLE_WIN_INFO_T;

// the stk idle text
typedef struct
{
    BOOLEAN is_scroll; //�Ƿ����
    MMI_STRING_T stk_idle_str;
    uint16 str_width;
    uint16 str_index;
    uint8 idle_display_timer;
    MN_DUAL_SYS_E dual_sys;
} IDLE_STK_TEXT_INFO;

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#if defined MMI_SMART_IDLE_SUPPORT_LARGE_ICON || defined MMI_SMART_IDLE_SUPPORT_SMALL_ICON || defined MMI_COMMON_IDLE_SUPPORT ||defined BAR_PHONE_WIDGET  
#ifdef CALENDAR_SUPPORT
/*****************************************************************************/
//  Description : display schedule
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIDLE_DisplaySchedule(const GUI_LCD_DEV_INFO *lcd_dev_ptr,uint8 direction, uint16 max_len, GUI_POINT_T  *dis_point,GUI_FONT_T font_type);
PUBLIC uint16 MMIIDLE_DisplayScheduleEx(
                                        GUI_LCD_DEV_INFO *lcd_dev_ptr,
                                        GUI_RECT_T      *rect_ptr,
                                        GUI_FONT_T       font_type
                                        );
#endif
/*****************************************************************************/
//  Description : ���idle�ϵ��ı����
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_GetIdleTextStyle(
                            GUISTR_STYLE_T *style_ptr,
                            GUI_FONT_T     font,
                            GUI_COLOR_T    font_color
                            );

/*****************************************************************************/
//  Description : display string
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIDLE_DisplayIdleText(//string pixel width
                             const GUI_LCD_DEV_INFO *lcd_dev_ptr,
                             const GUI_POINT_T      *point_ptr,
                             GUI_FONT_T             font,
                             GUI_COLOR_T            font_color,
                             wchar                  *wstr_ptr,
                             uint16                 wstr_len
                             );

PUBLIC uint16 MMIIDLE_DisplayLineTextInRect(
                              GUI_LCD_DEV_INFO *lcd_dev_ptr,
                              GUI_RECT_T      *rect_ptr,
                             GUI_FONT_T             font,
                             GUI_COLOR_T            font_color,
                             wchar                  *wstr_ptr,
                             uint16                 wstr_len
                             );
/*****************************************************************************/
//  Description : get display string
//  Global resource dependence : 
//  Author: 
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_GetDisPlayString(wchar const * src_str,     //in
                            uint16  src_len,     //in
                            uint16  max_display_len,//in
                            GUI_FONT_T font_type, //font
                            wchar * des_str,     //out
                            uint16* des_len      //out
                            );
#endif //#if defined MMI_SMART_IDLE_SUPPORT || defined MMI_COMMON_IDLE_SUPPORT
/*****************************************************************************/
//  Description : init win Info
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC void MMIIDLE_InitWinInfo(void);

/*****************************************************************************/
//  Description : get idle info
//  Global resource dependence : 
//  Author: jian.ma
//  Note:
/*****************************************************************************/
PUBLIC IDLE_WIN_INFO_T* MMIIDLE_GetIdleWinInfo(void);

/*****************************************************************************/
//  Description : display the main and sub idle soft icon 
//  Global resource dependence : 
//  Author: Great.Tian
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIIDLE_SendMsgToIdleWin(
                               MMI_MESSAGE_ID_E msg_id,
                               DPARAM            param_ptr
                               );

/*****************************************************************************/
//  Description : display string
//  Global resource dependence : 
//  Author: xiaoqing.lu
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIIDLE_DisplayIdleTextEx(
                             const GUI_LCD_DEV_INFO *lcd_dev_ptr,
                             const GUI_POINT_T      *point_ptr,
                             GUI_FONT_T             font,
                             GUI_COLOR_T            font_color,
                             wchar                  *wstr_ptr,
                             uint16                 wstr_len,
                             GUISTR_STATE_T  text_state
                             );
#ifdef MMI_IDLE_STKTEXT_SUPPORT
/*****************************************************************************/
//  Description : start stk idle display timer
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MAIN_StartIdleSTKTextTimer(void);

/*****************************************************************************/
//  Description : stop stk idle display timer
//  Global resource dependence : 
//  Author: wancan.you
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MAIN_StopIdleSTKTextTimer(void);

/*****************************************************************************/
//  Description : draw stk set idle mode text
//  Global resource dependence : 
//  Author: songbin.zeng
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MAIN_DrawStkIdleModeText(uint32 block_id);

/*****************************************************************************/
//  Description : get stk text info
//  Global resource dependence : 
//  Author: jian.ma
//  Note: 
/*****************************************************************************/
PUBLIC IDLE_STK_TEXT_INFO* MMIIDLECOM_GetStkTextInfo(void);

#endif
/*****************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif
