/*****************************************************************************
** File Name:      mmienvset_internal.h                                               *
** Author:                                                                   *
** Date:           23/12/2004                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.         *
** Description:    This file is used to describe environment setting         *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2004       maqianlin          Create
** 02/2012       xiyuan.ma          Edit
******************************************************************************/

#ifndef _MMIENVSET_INTERNAL_H_
#define _MMIENVSET_INTERNAL_H_ 

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/

//#include    "mmiset_wintab.h"
//#include "mmienvset.h"
#include "mmienvset_export.h"
//#include "mmiset.h"
#include "mmiset_export.h"
#include "mmi_custom_define.h"
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
    
    // -----------------------------------------  ��������-----------------------------------------------------------------
    // ��ͨ����
#ifdef MONOCHROME_LCD_SUPPORT 
#define MMIENVSET_CALL_RING_VOL_STANDARD_MODE  MMISET_VOL_NINE
#else    
#define MMIENVSET_CALL_RING_VOL_STANDARD_MODE  MMISET_VOL_NINE
#endif 
    // ��������
#define MMIENVSET_CALL_RING_VOL_SILENT_MODE	   MMISET_VOL_ZERO
    // ���黷��
#define MMIENVSET_CALL_RING_VOL_MEETING_MODE   MMISET_VOL_ZERO
    // ���ڻ���
#define MMIENVSET_CALL_RING_VOL_INSIDE_MODE    MMISET_VOL_SEVEN
    // ���⻷��
#define MMIENVSET_CALL_RING_VOL_NOISY_MODE     MMISET_VOL_MAX
   //����ģʽ
#define MMIENVSET_CALL_RING_VOL_EARPHONE_MODE  MMISET_VOL_FIVE
   //����ģʽ
#define MMIENVSET_CALL_RING_VOL_BLUETOOTH_MODE MMISET_VOL_FIVE
    // ���л���
//#define MMIENVSET_CALL_RING_VOL_FLY_MODE       MMISET_VOL_FIVE
    
    // ---------------------------------------------------��������----------------------------------------------------------
    // ��ͨ����
#ifdef MONOCHROME_LCD_SUPPORT 
#define MMIENVSET_MSG_RING_VOL_STANDARD_MODE   MMISET_VOL_NINE
#else    
#define MMIENVSET_MSG_RING_VOL_STANDARD_MODE   MMISET_VOL_NINE
#endif
    // ��������
#define MMIENVSET_MSG_RING_VOL_SILENT_MODE	   MMISET_VOL_ZERO
    // ���黷��
#define MMIENVSET_MSG_RING_VOL_MEETING_MODE    MMISET_VOL_ZERO
    // ���ڻ��� 
#define MMIENVSET_MSG_RING_VOL_INSIDE_MODE     MMISET_VOL_SIX
    // ���⻷��
#define MMIENVSET_MSG_RING_VOL_NOISY_MODE      MMISET_VOL_MAX
   //����ģʽ
#define MMIENVSET_MSG_RING_VOL_EARPHONE_MODE   MMISET_VOL_TWO
   //����ģʽ
#define MMIENVSET_MSG_RING_VOL_BLUETOOTH_MODE  MMISET_VOL_FIVE
    // ���л���
//#define MMIENVSET_MSG_RING_VOL_FLY_MODE        MMISET_VOL_FOUR
    
    // -------------------------------------------------ͨ������-----------------------------------------------------------
    // ��ͨ����
#ifdef MONOCHROME_LCD_SUPPORT 
#define MMIENVSET_COMM_VOL_STANDARD_MODE       MMISET_VOL_SEVEN
#else    
#define MMIENVSET_COMM_VOL_STANDARD_MODE       MMISET_VOL_EIGHT
#endif 
    // ��������
#define MMIENVSET_COMM_VOL_SILENT_MODE	       MMISET_VOL_SEVEN
    // ���黷��
#define MMIENVSET_COMM_VOL_MEETING_MODE        MMISET_VOL_SEVEN
    // ���ڻ���
#define MMIENVSET_COMM_VOL_INSIDE_MODE         MMISET_VOL_SEVEN
    // ���⻷�� 
#define MMIENVSET_COMM_VOL_NOISY_MODE          MMISET_VOL_MAX
    //����ģʽ
#define MMIENVSET_COMM_VOL_EARPHONE_MODE       MMISET_VOL_SEVEN
    //����ģʽ
#define MMIENVSET_COMM_VOL_BLUETOOTH_MODE      MMISET_VOL_SEVEN

    // ���л���
//#define MMIENVSET_COMM_VOL_FLY_MODE            MMISET_VOL_FIVE
    
    // -------------------------------------------------��������------------------------------------------------------------
    // ��ͨ����
#define MMIENVSET_KEY_RING_VOL_STANDARD_MODE       MMISET_VOL_SIX
    // ��������
#define MMIENVSET_KEY_RING_VOL_SILENT_MODE	       MMISET_VOL_ZERO//MMISET_VOL_ZERO
    // ���黷��
#define MMIENVSET_KEY_RING_VOL_MEETING_MODE        MMISET_VOL_ZERO//MMISET_VOL_ZERO
    // ���ڻ���
#define MMIENVSET_KEY_RING_VOL_INSIDE_MODE         MMISET_VOL_THREE
    // ���⻷��
#define MMIENVSET_KEY_RING_VOL_NOISY_MODE          MMISET_VOL_SEVEN
    //����ģʽ
#define MMIENVSET_KEY_RING_VOL_EARPHONE_MODE       MMISET_VOL_TWO
    //����ģʽ
#define MMIENVSET_KEY_RING_VOL_BLUETOOTH_MODE      MMISET_VOL_SIX

    // ���л���
//#define MMIENVSET_KEY_RING_VOL_FLY_MODE            MMISET_VOL_THREE
     // -------------------------------------------------����������------------------------------------------------------------
    // ��ͨ����
#define MMIENVSET_TKEY_RING_VOL_STANDARD_MODE       MMISET_VOL_SIX
    // ��������
#define MMIENVSET_TKEY_RING_VOL_SILENT_MODE	        MMISET_VOL_ZERO//MMISET_VOL_ZERO
    // ���黷��
#define MMIENVSET_TKEY_RING_VOL_MEETING_MODE        MMISET_VOL_ZERO//MMISET_VOL_ZERO
    // ���ڻ���
#define MMIENVSET_TKEY_RING_VOL_INSIDE_MODE         MMISET_VOL_THREE
    // ���⻷��
#define MMIENVSET_TKEY_RING_VOL_NOISY_MODE          MMISET_VOL_SEVEN
    //����ģʽ
#define MMIENVSET_TKEY_RING_VOL_EARPHONE_MODE       MMISET_VOL_TWO
    //����ģʽ
#define MMIENVSET_TKEY_RING_VOL_BLUETOOTH_MODE      MMISET_VOL_SIX
    // --------------------------------------------------���ػ�����-------------------------------------------------------------
    // ��ͨ����
#define MMIENVSET_POWER_RING_VOL_STANDARD_MODE      MMISET_VOL_NINE
    // �������� 
#define MMIENVSET_POWER_RING_VOL_SILENT_MODE	    MMISET_VOL_ZERO
    // ���黷��
#define MMIENVSET_POWER_RING_VOL_MEETING_MODE       MMISET_VOL_ZERO
    // ���ڻ���
#define MMIENVSET_POWER_RING_VOL_INSIDE_MODE        MMISET_VOL_ONE
    // ���⻷��
#define MMIENVSET_POWER_RING_VOL_NOISY_MODE         MMISET_VOL_MAX
    //����ģʽ
#define MMIENVSET_POWER_RING_VOL_EARPHONE_MODE      MMISET_VOL_FOUR
    //����ģʽ
#define MMIENVSET_POWER_RING_VOL_BLUETOOTH_MODE     MMISET_VOL_FOUR
    // ���л���
//#define MMIENVSET_POWER_RING_VOL_FLY_MODE           MMISET_VOL_THREE
    
#ifdef MMI_READMENU_ENABLE
// --------------------------------------------------�˵��ʶ�����-------------------------------------------------------------
// ��ͨ����
#define MMIENVSET_MENU_READ_VOL_STANDARD_MODE       MMISET_VOL_THREE
// �������� 
#define MMIENVSET_MENU_READ_VOL_SILENT_MODE	         MMISET_VOL_ZERO
// ���黷��
#define MMIENVSET_MENU_READ_VOL_MEETING_MODE        MMISET_VOL_ZERO
// ���ڻ���
#define MMIENVSET_MENU_READ_VOL_INSIDE_MODE         MMISET_VOL_FIVE
// ���⻷��
#define MMIENVSET_MENU_READ_VOL_NOISY_MODE          MMISET_VOL_MAX
//����ģʽ
#define MMIENVSET_MENU_READ_VOL_EARPHONE_MODE       MMISET_VOL_THREE
//����ģʽ
#define MMIENVSET_MENU_READ_VOL_BLUETOOTH_MODE      MMISET_VOL_THREE
// ���л���
//#define MMIENVSET_POWER_RING_VOL_FLY_MODE           MMISET_VOL_THREE
#endif

    // ----------------------------------------------������ʾ����----------------------------------------------------------------
    // ��ͨ����
#define MMIENVSET_CALL_RING_TYPE_STANDARD_MODE      MMISET_CALL_VIBRA_AND_RING
    // ��������
#define MMIENVSET_CALL_RING_TYPE_SILENT_MODE        MMISET_CALL_SILENT
    // ���黷��
#define MMIENVSET_CALL_RING_TYPE_MEETING_MODE       MMISET_CALL_VIBRA
    // ���ڻ���
#define MMIENVSET_CALL_RING_TYPE_INSIDE_MODE        MMISET_CALL_RING
    // ���⻷��
#define MMIENVSET_CALL_RING_TYPE_NOISY_MODE         MMISET_CALL_VIBRA_AND_RING
    //����ģʽ
#define MMIENVSET_CALL_RING_TYPE_EARPHONE_MODE      MMISET_CALL_RING
    //����ģʽ
#define MMIENVSET_CALL_RING_TYPE_BLUETOOTH_MODE     MMISET_CALL_RING
    // ���л���
//#define MMIENVSET_CALL_RING_TYPE_FLY_MODE      MMISET_CALL_RING
    
    // ------------------------------------------------������ʾ��ʽ----------------------------------------------------------------
    // ��ͨ����
#define MMIENVSET_MSG_RING_TYPE_STANDARD_MODE  MMISET_MSG_VIBRA_AND_RING
    // ��������
#define MMIENVSET_MSG_RING_TYPE_SILENT_MODE	   MMISET_MSG_SILENT
    // ���黷��
#define MMIENVSET_MSG_RING_TYPE_MEETING_MODE   MMISET_MSG_VIBRA
// ���ڻ���
#define MMIENVSET_MSG_RING_TYPE_INSIDE_MODE    MMISET_MSG_RING
// ���⻷��
#define MMIENVSET_MSG_RING_TYPE_NOISY_MODE     MMISET_MSG_VIBRA_AND_RING
    //����ģʽ
#define MMIENVSET_MSG_RING_TYPE_EARPHONE_MODE  MMISET_MSG_RING
    //����ģʽ
#define MMIENVSET_MSG_RING_TYPE_BLUETOOTH_MODE MMISET_MSG_RING
// ���л���
//#define MMIENVSET_MSG_RING_TYPE_FLY_MODE       MMISET_MSG_RING

// -------------------------------------------- ������ ���� ------------------------------------------------------------------
// ��ͨ����
#define MMIENVSET_KEY_RING_TYPE_STANDARD_MODE         MMISET_OTHER_RING1
// ��������
#define MMIENVSET_KEY_RING_TYPE_SILENT_MODE	          MMISET_OTHER_SILENT
// ���黷��
#define MMIENVSET_KEY_RING_TYPE_MEETING_MODE          MMISET_OTHER_SILENT
// ���ڻ���
#define MMIENVSET_KEY_RING_TYPE_INSIDE_MODE           MMISET_OTHER_RING1
// ���⻷��
#define MMIENVSET_KEY_RING_TYPE_NOISY_MODE            MMISET_OTHER_RING1
    //����ģʽ
#define MMIENVSET_KEY_RING_TYPE_EARPHONE_MODE         MMISET_OTHER_RING1
    //����ģʽ
#define MMIENVSET_KEY_RING_TYPE_BLUETOOTH_MODE        MMISET_OTHER_RING1
// ���л���
//#define MMIENVSET_KEY_RING_TYPE_FLY_MODE              MMISET_OTHER_RING1

// -------------------------------------------- ������ ���� ------------------------------------------------------------------
#define	MMIENVSET_TKEY_RING_TYPE_SWITCH_OPEN					0
#define	MMIENVSET_TKEY_RING_TYPE_SWITCH_CLOSE					1	
// ��ͨ����
#define MMIENVSET_TKEY_RING_TYPE_STANDARD_MODE         MMIENVSET_TKEY_RING_TYPE_SWITCH_CLOSE
// ��������
#define MMIENVSET_TKEY_RING_TYPE_SILENT_MODE	       MMIENVSET_TKEY_RING_TYPE_SWITCH_CLOSE
// ���黷��
#define MMIENVSET_TKEY_RING_TYPE_MEETING_MODE          MMIENVSET_TKEY_RING_TYPE_SWITCH_CLOSE
// ���ڻ���
#define MMIENVSET_TKEY_RING_TYPE_INSIDE_MODE           MMIENVSET_TKEY_RING_TYPE_SWITCH_CLOSE
// ���⻷��
#define MMIENVSET_TKEY_RING_TYPE_NOISY_MODE            MMIENVSET_TKEY_RING_TYPE_SWITCH_CLOSE
    //����ģʽ
#define MMIENVSET_TKEY_RING_TYPE_EARPHONE_MODE         MMIENVSET_TKEY_RING_TYPE_SWITCH_CLOSE
    //����ģʽ
#define MMIENVSET_TKEY_RING_TYPE_BLUETOOTH_MODE        MMIENVSET_TKEY_RING_TYPE_SWITCH_CLOSE
// ���л���
//#define MMIENVSET_TKEY_RING_TYPE_FLY_MODE              MMIENVSET_TKEY_RING_TYPE_SWITCH_OPEN

// -----------------------------------------������ʾ������ ---------------------------------------------------------------------
// ��ͨ����
#define MMIENVSET_FLIP_RING_TYPE_STANDARD_MODE        MMISET_OTHER_RING1
// ��������
#define MMIENVSET_FLIP_RING_TYPE_SILENT_MODE	      MMISET_OTHER_SILENT
// ���黷��
#define MMIENVSET_FLIP_RING_TYPE_MEETING_MODE         MMISET_OTHER_SILENT
// ���ڻ���
#define MMIENVSET_FLIP_RING_TYPE_INSIDE_MODE          MMISET_OTHER_SILENT
// ���⻷��
#define MMIENVSET_FLIP_RING_TYPE_NOISY_MODE           MMISET_OTHER_RING1
    //����ģʽ
#define MMIENVSET_FLIP_RING_TYPE_EARPHONE_MODE        MMISET_OTHER_RING1
    //����ģʽ
#define MMIENVSET_FLIP_RING_TYPE_BLUETOOTH_MODE       MMISET_OTHER_RING1
// ���л���
//#define MMIENVSET_FLIP_RING_TYPE_FLY_MODE             MMISET_OTHER_RING1

// -----------------------------------------������ʾ������ ---------------------------------------------------------------------
// ��ͨ����
#define MMIENVSET_FLIP_DOWN_RING_TYPE_STANDARD_MODE        MMISET_OTHER_RING1
// ��������
#define MMIENVSET_FLIP_DOWN_RING_TYPE_SILENT_MODE	      MMISET_OTHER_SILENT
// ���黷��
#define MMIENVSET_FLIP_DOWN_RING_TYPE_MEETING_MODE         MMISET_OTHER_SILENT
// ���ڻ���
#define MMIENVSET_FLIP_DOWN_RING_TYPE_INSIDE_MODE          MMISET_OTHER_SILENT
// ���⻷��
#define MMIENVSET_FLIP_DOWN_RING_TYPE_NOISY_MODE           MMISET_OTHER_RING1
    //����ģʽ
#define MMIENVSET_FLIP_DOWN_RING_TYPE_EARPHONE_MODE        MMISET_OTHER_RING1
    //����ģʽ
#define MMIENVSET_FLIP_DOWN_RING_TYPE_BLUETOOTH_MODE       MMISET_OTHER_RING1
// ���л���
//#define MMIENVSET_FLIP_RING_TYPE_FLY_MODE             MMISET_OTHER_RING1

// -----------------------------------------���ǣ����ǣ���ʾ������ ---------------------------------------------------------------------
// ��ͨ����
#define MMIENVSET_SLIDE_ON_RING_TYPE_STANDARD_MODE        MMISET_OTHER_RING1
// ��������
#define MMIENVSET_SLIDE_ON_RING_TYPE_SILENT_MODE	      MMISET_OTHER_SILENT
// ���黷��
#define MMIENVSET_SLIDE_ON_RING_TYPE_MEETING_MODE         MMISET_OTHER_SILENT
// ���ڻ���
#define MMIENVSET_SLIDE_ON_RING_TYPE_INSIDE_MODE          MMISET_OTHER_SILENT
// ���⻷��
#define MMIENVSET_SLIDE_ON_RING_TYPE_NOISY_MODE           MMISET_OTHER_RING1
    //����ģʽ
#define MMIENVSET_SLIDE_ON_RING_TYPE_EARPHONE_MODE        MMISET_OTHER_RING1
    //����ģʽ
#define MMIENVSET_SLIDE_ON_RING_TYPE_BLUETOOTH_MODE       MMISET_OTHER_RING1
// ���л���
//#define MMIENVSET_FLIP_RING_TYPE_FLY_MODE             MMISET_OTHER_RING1

// -----------------------------------------���ǣ��ظǣ���ʾ������ ---------------------------------------------------------------------
// ��ͨ����
#define MMIENVSET_SLIDE_OFF_RING_TYPE_STANDARD_MODE        MMISET_OTHER_RING1
// ��������
#define MMIENVSET_SLIDE_OFF_RING_TYPE_SILENT_MODE	       MMISET_OTHER_SILENT
// ���黷��
#define MMIENVSET_SLIDE_OFF_RING_TYPE_MEETING_MODE         MMISET_OTHER_SILENT
// ���ڻ���
#define MMIENVSET_SLIDE_OFF_RING_TYPE_INSIDE_MODE          MMISET_OTHER_SILENT
// ���⻷��
#define MMIENVSET_SLIDE_OFF_RING_TYPE_NOISY_MODE           MMISET_OTHER_RING1
    //����ģʽ
#define MMIENVSET_SLIDE_OFF_RING_TYPE_EARPHONE_MODE        MMISET_OTHER_RING1
    //����ģʽ
#define MMIENVSET_SLIDE_OFF_RING_TYPE_BLUETOOTH_MODE       MMISET_OTHER_RING1
// ���л���
//#define MMIENVSET_FLIP_RING_TYPE_FLY_MODE             MMISET_OTHER_RING1

// -----------------------------------------����/�رյ͵�ѹ�澯��--------------------------------------------------------------
#define	MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_OPEN					0
#define	MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_CLOSE					1						
// ��ͨ����
#define MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_STANDARD_MODE          MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_OPEN
// ��������
#define MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_SILENT_MODE	        MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_CLOSE
// ���黷��
#define MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_MEETING_MODE           MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_CLOSE
// ���ڻ���
#define MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_INSIDE_MODE            MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_OPEN
// ���⻷��
#define MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_NOISY_MODE             MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_OPEN

#define MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_EARPHONE_MODE          MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_OPEN

#define MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_BLUETOOTH_MODE         MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_OPEN
// ���л���
//#define MMIENVSET_NOTIFY_SWITCH_SWITCH_FLY_MODE               MMIENVSET_DOWNLOAD_NOTIFY_SWITCH_OPEN

// -----------------------------------------����/�رյ͵�ѹ�澯��--------------------------------------------------------------
#define	MMIENVSET_VOLTAGE_WARN_SWITCH_OPEN					0
#define	MMIENVSET_VOLTAGE_WARN_SWITCH_CLOSE					1						
// ��ͨ����
#define MMIENVSET_VOLTAGE_WARN_SWITCH_STANDARD_MODE          MMIENVSET_VOLTAGE_WARN_SWITCH_CLOSE
// ��������
#define MMIENVSET_VOLTAGE_WARN_SWITCH_SILENT_MODE	         MMIENVSET_VOLTAGE_WARN_SWITCH_CLOSE
// ���黷��
#define MMIENVSET_VOLTAGE_WARN_SWITCH_MEETING_MODE           MMIENVSET_VOLTAGE_WARN_SWITCH_CLOSE
// ���ڻ���
#define MMIENVSET_VOLTAGE_WARN_SWITCH_INSIDE_MODE            MMIENVSET_VOLTAGE_WARN_SWITCH_CLOSE
// ���⻷��
#define MMIENVSET_VOLTAGE_WARN_SWITCH_NOISY_MODE             MMIENVSET_VOLTAGE_WARN_SWITCH_OPEN
    //����ģʽ
#define MMIENVSET_VOLTAGE_WARN_SWITCH_EARPHONE_MODE          MMIENVSET_VOLTAGE_WARN_SWITCH_OPEN
    //����ģʽ
#define MMIENVSET_VOLTAGE_WARN_SWITCH_BLUETOOTH_MODE         MMIENVSET_VOLTAGE_WARN_SWITCH_OPEN
// ���л���
//#define MMIENVSET_VOLTAGE_WARN_SWITCH_FLY_MODE               MMIENVSET_VOLTAGE_WARN_SWITCH_OPEN

// -----------------------------------------����/�رտ�ʼ�����ʾ�澯��-------------------------------------------------------
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_OPEN                 1
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_CLOSE                0
// ��ͨ����
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_STANDARD_MODE          MMIENVSET_CHARGE_NOTIFY_SWITCH_CLOSE
// ��������
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_SILENT_MODE            MMIENVSET_CHARGE_NOTIFY_SWITCH_CLOSE
// ���黷��
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_MEETING_MODE           MMIENVSET_CHARGE_NOTIFY_SWITCH_CLOSE
// ���ڻ���
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_INSIDE_MODE            MMIENVSET_CHARGE_NOTIFY_SWITCH_CLOSE
// ���⻷��
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_NOISY_MODE             MMIENVSET_CHARGE_NOTIFY_SWITCH_OPEN
// ����ģʽ
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_EARPHONE_MODE          MMIENVSET_CHARGE_NOTIFY_SWITCH_OPEN
// ����ģʽ
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_BLUETOOTH_MODE         MMIENVSET_CHARGE_NOTIFY_SWITCH_OPEN
// ���л���
#define MMIENVSET_CHARGE_NOTIFY_SWITCH_FLY_MODE               MMIENVSET_CHARGE_NOTIFY_SWITCH_CLOSE

// -----------------------------------------����/�رտ��ػ�����--------------------------------------------------------------
#define	MMIENVSET_POWER_RING_SWITCH_OPEN					0
#define	MMIENVSET_POWER_RING_SWITCH_CLOSE					1						
// ��ͨ����
#define MMIENVSET_POWER_RING_SWITCH_STANDARD_MODE          MMIENVSET_POWER_RING_SWITCH_OPEN
// ��������
#define MMIENVSET_POWER_RING_SWITCH_SILENT_MODE	           MMIENVSET_POWER_RING_SWITCH_CLOSE
// ���黷��
#define MMIENVSET_POWER_RING_SWITCH_MEETING_MODE           MMIENVSET_POWER_RING_SWITCH_CLOSE
// ���ڻ���
#define MMIENVSET_POWER_RING_SWITCH_INSIDE_MODE            MMIENVSET_POWER_RING_SWITCH_OPEN
// ���⻷��
#define MMIENVSET_POWER_RING_SWITCH_NOISY_MODE             MMIENVSET_POWER_RING_SWITCH_OPEN
    //����ģʽ
#define MMIENVSET_POWER_RING_SWITCH_EARPHONE_MODE	       MMIENVSET_POWER_RING_SWITCH_OPEN
    //����ģʽ
#define MMIENVSET_POWER_RING_SWITCH_BLUETOOTH_MODE	       MMIENVSET_POWER_RING_SWITCH_OPEN
// ���л���
//#define MMIENVSET_POWER_RING_SWITCH_FLY_MODE               MMIENVSET_POWER_RING_SWITCH_OPEN

// -----------------------------------------������������-------------------------------------------------------------------------
// ��ͨ����
#define MMIENVSET_ALARM_RING_VOL_STANDARD_MODE       MMISET_VOL_NINE
// ��������
#define MMIENVSET_ALARM_RING_VOL_SILENT_MODE	     MMISET_VOL_ZERO
// ���黷��
#define MMIENVSET_ALARM_RING_VOL_MEETING_MODE        MMISET_VOL_ZERO
// ���ڻ���
#define MMIENVSET_ALARM_RING_VOL_INSIDE_MODE         MMISET_VOL_FIVE
// ���⻷��
#define MMIENVSET_ALARM_RING_VOL_NOISY_MODE          MMISET_VOL_MAX
    //����ģʽ
#define MMIENVSET_ALARM_RING_VOL_EARPHONE_MODE       MMISET_VOL_FOUR
    //����ģʽ
#define MMIENVSET_ALARM_RING_VOL_BLUETOOTH_MODE      MMISET_VOL_FOUR
// ���л���
//#define MMIENVSET_ALARM_RING_VOL_FLY_MODE           MMISET_VOL_THREE

// -----------------------------------------������ʾ��ʽ-------------------------------------------------------------------------
// ��ͨ����
#define MMIENVSET_ALARM_RING_TYPE_STANDARD_MODE      MMISET_MSG_VIBRA_AND_RING
// ��������
#define MMIENVSET_ALARM_RING_TYPE_SILENT_MODE	     MMISET_MSG_SILENT
// ���黷��
#define MMIENVSET_ALARM_RING_TYPE_MEETING_MODE       MMISET_MSG_VIBRA
// ���ڻ���
#define MMIENVSET_ALARM_RING_TYPE_INSIDE_MODE        MMISET_MSG_RING
// ���⻷��
#define MMIENVSET_ALARM_RING_TYPE_NOISY_MODE         MMISET_MSG_VIBRA_AND_RING
    //����ģʽ
#define MMIENVSET_ALARM_RING_TYPE_EARPHONE_MODE      MMISET_MSG_RING
    //����ģʽ
#define MMIENVSET_ALARM_RING_TYPE_BLUETOOTH_MODE     MMISET_MSG_RING
// ���л���
//#define MMIENVSET_ALARM_RING_TYPE_FLY_MODE           MMISET_MSG_RING

// -------------------------------------------���ػ���������---------------------------------------------------------------------
// ��ͨ����
#define MMIENVSET_POWER_RING_TYPE_STANDARD_MODE      MMISET_OTHER_RING1
// ��������
#define MMIENVSET_POWER_RING_TYPE_SILENT_MODE	     MMISET_OTHER_SILENT
// ���黷��
#define MMIENVSET_POWER_RING_TYPE_MEETING_MODE       MMISET_OTHER_SILENT
// ���ڻ���
#define MMIENVSET_POWER_RING_TYPE_INSIDE_MODE        MMISET_OTHER_RING1
// ���⻷��
#define MMIENVSET_POWER_RING_TYPE_NOISY_MODE         MMISET_OTHER_RING1
    //����ģʽ
#define MMIENVSET_POWER_RING_TYPE_EARPHONE_MODE	     MMISET_OTHER_RING1
    //����ģʽ
#define MMIENVSET_POWER_RING_TYPE_BLUETOOTH_MODE	 MMISET_OTHER_RING1
// ���л���
//#define MMIENVSET_POWER_RING_TYPE_FLY_MODE         MMISET_OTHER_RING1

//-----------------------------------------�����̴�����------------------------------------------------------------------------
#define	MMIENVSET_DIAL_TP_VIBRATE_SWITCH_OPEN					0
#define	MMIENVSET_DIAL_TP_VIBRATE_SWITCH_CLOSE					1						

// ��ͨ����
#define MMIENVSET_DIAL_TP_VIBRATE_SWITCH_STANDARD_MODE          MMIENVSET_DIAL_TP_VIBRATE_SWITCH_OPEN
// ��������
#define MMIENVSET_DIAL_TP_VIBRATE_SWITCH_SILENT_MODE	        MMIENVSET_DIAL_TP_VIBRATE_SWITCH_CLOSE
// ���黷��
#define MMIENVSET_DIAL_TP_VIBRATE_SWITCH_MEETING_MODE           MMIENVSET_DIAL_TP_VIBRATE_SWITCH_CLOSE
// ���ڻ���
#define MMIENVSET_DIAL_TP_VIBRATE_SWITCH_INSIDE_MODE            MMIENVSET_DIAL_TP_VIBRATE_SWITCH_OPEN
// ���⻷��
#define MMIENVSET_DIAL_TP_VIBRATE_SWITCH_NOISY_MODE             MMIENVSET_DIAL_TP_VIBRATE_SWITCH_OPEN
    //����ģʽ
#define MMIENVSET_DIAL_TP_VIBRATE_SWITCH_EARPHONE_MODE          MMIENVSET_DIAL_TP_VIBRATE_SWITCH_OPEN
    //����ģʽ
#define MMIENVSET_DIAL_TP_VIBRATE_SWITCH_BLUETOOTH_MODE         MMIENVSET_DIAL_TP_VIBRATE_SWITCH_OPEN

#if defined(MMI_DIALPANEL_DTMF_SUPPORT)
//-----------------------------------------������------------------------------------------------------------------------
#define	MMIENVSET_DIAL_TONE_SWITCH_OPEN					    0
#define	MMIENVSET_DIAL_TONE_SWITCH_CLOSE					1						

// ��ͨ����
#define MMIENVSET_DIAL_TONE_SWITCH_STANDARD_MODE          MMIENVSET_DIAL_TONE_SWITCH_OPEN
// ��������
#define MMIENVSET_DIAL_TONE_SWITCH_SILENT_MODE	        MMIENVSET_DIAL_TONE_SWITCH_CLOSE
// ���黷��
#define MMIENVSET_DIAL_TONE_SWITCH_MEETING_MODE           MMIENVSET_DIAL_TONE_SWITCH_CLOSE
// ���ڻ���
#define MMIENVSET_DIAL_TONE_SWITCH_INSIDE_MODE            MMIENVSET_DIAL_TONE_SWITCH_OPEN
// ���⻷��
#define MMIENVSET_DIAL_TONE_SWITCH_NOISY_MODE             MMIENVSET_DIAL_TONE_SWITCH_OPEN
    //����ģʽ
#define MMIENVSET_DIAL_TONE_SWITCH_EARPHONE_MODE          MMIENVSET_DIAL_TONE_SWITCH_OPEN
    //����ģʽ
#define MMIENVSET_DIAL_TONE_SWITCH_BLUETOOTH_MODE         MMIENVSET_DIAL_TONE_SWITCH_OPEN
#endif

//-----------------------------------------��������------------------------------------------------------------------------
#define	MMIENVSET_TP_KEY_LOCK_RING_SWITCH_OPEN					0
#define	MMIENVSET_TP_KEY_LOCK_RING_SWITCH_CLOSE					1						

// ��ͨ����
#define MMIENVSET_TP_KEY_LOCK_RING_SWITCH_STANDARD_MODE          MMIENVSET_TP_KEY_LOCK_RING_SWITCH_OPEN
// ��������
#define MMIENVSET_TP_KEY_LOCK_RING_SWITCH_SILENT_MODE	         MMIENVSET_TP_KEY_LOCK_RING_SWITCH_CLOSE
// ���黷��
#define MMIENVSET_TP_KEY_LOCK_RING_SWITCH_MEETING_MODE           MMIENVSET_TP_KEY_LOCK_RING_SWITCH_CLOSE
// ���ڻ���
#define MMIENVSET_TP_KEY_LOCK_RING_SWITCH_INSIDE_MODE            MMIENVSET_TP_KEY_LOCK_RING_SWITCH_OPEN
// ���⻷��
#define MMIENVSET_TP_KEY_LOCK_RING_SWITCH_NOISY_MODE             MMIENVSET_TP_KEY_LOCK_RING_SWITCH_OPEN
    //����ģʽ
#define MMIENVSET_TP_KEY_LOCK_RING_SWITCH_EARPHONE_MODE          MMIENVSET_TP_KEY_LOCK_RING_SWITCH_OPEN
    //����ģʽ
#define MMIENVSET_TP_KEY_LOCK_RING_SWITCH_BLUETOOTH_MODE         MMIENVSET_TP_KEY_LOCK_RING_SWITCH_OPEN
#ifdef MMI_READMENU_ENABLE
//-----------------------------------------�˵��ʶ�------------------------------------------------------------------------

#define	MMIENVSET_MENU_READ_SWITCH_OPEN					        0
#define	MMIENVSET_MENU_READ_SWITCH_CLOSE			            1						

// ��ͨ����
#define MMIENVSET_MENU_READ_SWITCH_STANDARD_MODE                MMIENVSET_MENU_READ_SWITCH_OPEN
// ��������
#define MMIENVSET_MENU_READ_SWITCH_SILENT_MODE	                MMIENVSET_MENU_READ_SWITCH_CLOSE
// ���黷��
#define MMIENVSET_MENU_READ_SWITCH_MEETING_MODE                 MMIENVSET_MENU_READ_SWITCH_CLOSE
// ���ڻ���
#define MMIENVSET_MENU_READ_SWITCH_INSIDE_MODE                  MMIENVSET_MENU_READ_SWITCH_OPEN
// ���⻷��
#define MMIENVSET_MENU_READ_SWITCH_NOISY_MODE                   MMIENVSET_MENU_READ_SWITCH_OPEN
//����ģʽ
#define MMIENVSET_MENU_READ_SWITCH_EARPHONE_MODE                MMIENVSET_MENU_READ_SWITCH_OPEN
//����ģʽ
#define MMIENVSET_MENU_READ_SWITCH_BLUETOOTH_MODE               MMIENVSET_MENU_READ_SWITCH_OPEN
#endif

/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/



/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/

/*****************************************************************************/
// Description : �ڻ������õ�һ���˵�(listbox)��׷��(append)�б���
//	Global resource dependence : none
// Author: maqianlin
//	Note:  ���listbox���ı��б�
/*****************************************************************************/
void MMIENVSET_MainMenu_Append_LBItem(
                                      MMI_CTRL_ID_T ctrl_id
                                      );


/*****************************************************************************/
// Description : ���µĻ�������д��nv
//	Global resource dependence : none
// Author: maqianlin
//	Note:
/*****************************************************************************/
BOOLEAN MMIENVSET_EnvName_WriteInNV(
                                    MMI_CTRL_ID_T ctrl_id
                                    );

/*****************************************************************************/
// 	Description : �г����е���������
//	 Global resource dependence : 
//  Author: maqianlin
//	 Note:
/*****************************************************************************/
void MMIENVSET_RingType_AppendListItem(
                                       MN_DUAL_SYS_E dual_sys,
                                       MMI_CTRL_ID_T	        ctrl_id,
                                       MMISET_ALL_RING_TYPE_E   ring_type,
                                       MMIENVSET_SET_OPT_E      set_opt
                                       );
#ifndef MMI_ENVSET_MINI_SUPPORT
/*****************************************************************************/
//   Description : ������ɿ����͹ر��б�
//   Global resource dependence : none
//   Author: jiaoyou.wu
//   Note:
//*****************************************************************************/
void MMIENVSET_DownloadCompleteNotify_AppendListItem(
                                       MMI_CTRL_ID_T        ctrl_id
                                       );

/*****************************************************************************/
// 	Description : �͵�ѹ���������͹ر��б�
//	 Global resource dependence : none
//  Author: maqianlin
//	 Note:
//*****************************************************************************/
void MMIENVSET_VoltWarn_AppendListItem(
                                       MMI_CTRL_ID_T	    ctrl_id
                                       );

/*****************************************************************************/
// 	 Description : ���ÿ��ػ�����ѡ����б�
//	 Global resource dependence : none
//   Author: kelly.li
//	 Note:
//*****************************************************************************/
PUBLIC void MMIENVSET_AppendPowerRingSwitchListItem(
                                                    MMI_CTRL_ID_T	    ctrl_id
                                                    );
#endif

/*****************************************************************************/
// 	Description : ��¼ǰһ�ε��龰ģʽ�����ⲿʹ��
//	 Global resource dependence : none
//  Author: juan.wu
//	 Note:
//*****************************************************************************/
PUBLIC void MMIENVSET_SetPreActiveMode(uint8 mode_id);

/*****************************************************************************/
// Description :�ж������õĻ���ģʽ�Ƿ�����ʹ�õĻ���ģʽ,�Ƿ���true �����Ƿ���false 
//	Global resource dependence : none
// Author: maqianlin
//	Note:
/*****************************************************************************/
BOOLEAN MMIENVSET_IsActiveMode(uint8 mode_id);

/*****************************************************************************/
//  Description : ��ʼ���༭��
//	Global resource dependence : none
//  Author: maqianlin
//	Note:
/*****************************************************************************/
void ENVSET_InitChangeNameTextEditBox(
                                      MMI_CTRL_ID_T ctrl_id
                                      );

/*****************************************************************************/
//  Description : ͨ��ָ������Ԥ������
//	Global resource dependence : none
//  Author: maqianlin
//	Note:
/****************************************************************************/
void MMIENVSET_PreviewRingByVol(
                                MN_DUAL_SYS_E dual_sys,
                                MMISET_ALL_RING_TYPE_E  ring_type,
                                uint8 				    ring_vol,
                                uint8				    play_times
                                );
/*****************************************************************************/
//  Description : �ɵ�ǰ��Ӱ��ָ�����Ų�ͬ����������
//	Global resource dependence : none
//  Author: maqianlin
//	Note:
/*****************************************************************************/
void   MMIENVSET_PlayRingByListItem(
                                    MN_DUAL_SYS_E dual_sys,
                                    MMISET_ALL_RING_TYPE_E  	ring_type,   //��������
                                    MMI_CTRL_ID_T	       	ctrl_id,      //�ؼ�ID
                                    uint8                       play_times   //���Ŵ���
                                    );



/*****************************************************************************/
//  Description :ͨ��ָ��ID ��������
//	Global resource dependence : none
//  Author: maqianlin
//	Note:
/*****************************************************************************/
void MMIENVSET_PreviewRingById(
                               MMISET_ALL_RING_TYPE_E  ring_type,	
                               uint8 				    ring_id,
                               uint8				    play_times   //���Ŵ���
                               );


/*****************************************************************************/
//Description : �رմ���֮ǰ�ر����е��𶯺����� 
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
void   MMIENVSET_StopVibraAndRing(
                                  MMISET_ALL_RING_TYPE_E  ring_type  //��������
                                  );


/*****************************************************************************/
//Description : ��ʱ�䵽 
//Global resource dependence : 
//Author: maqianlin
//Note:
/*****************************************************************************/
void   MMIENVSET_VibraTimerOut(
                               MN_DUAL_SYS_E dual_sys,
                               MMISET_ALL_RING_TYPE_E   ring_type,   //��������
                               MMI_CTRL_ID_T	       ctrl_id     //�ؼ�ID
                               );

#ifndef MMI_ENVSET_MINI_SUPPORT
/*****************************************************************************/
//  Description :���������������õ�ͼ�� 
//	Global resource dependence : none
//  Author: maqianlin
//	Note:
/*****************************************************************************/
void MMIENVSET_SetActiveModeIcon(void);
#endif
/*****************************************************************************/
//  Description : �򿪻����������˵�                        
//	Global resource dependence : none
//  Author: maqianlin
//	Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_OpenMainMenuWindow(void) ;

/*****************************************************************************/
// 	Description : play music file 
//	Global resource dependence : 
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIENVSET_GetPlayResult(void);

/*****************************************************************************/
// 	Description : play music file 
//	Global resource dependence : 
//  Author: liqing.peng
//	Note: 
/*****************************************************************************/
PUBLIC void MMIENVSET_SetPlayResult(BOOLEAN result);

/*****************************************************************************/
//  Description : ɾ���Զ����龰ģʽ
//  Global resource dependence : none
//  Author: panvs.zhong
//  Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_HandleDeleteItem(
                                    uint16 item_index // IN
                                    );

/*****************************************************************************/
//  Description : ���޸�����Զ����龰ģʽ
//  Global resource dependence : none
//  Author: panvs.zhong
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIENVSET_HandleAddItem(void);

/*****************************************************************************/
//  Description : ��ȡ������ģʽID
//  Global resource dependence : none
//  Author: haiwu.chen
//  Note: ����ԭ����ƣ��Խ�ģʽ���ܻᱻɾ�����м��©�գ�����ԭ����GetEnvRealIndex
/*****************************************************************************/
PUBLIC uint16 MMIENVSET_GetEnvRealIndex(uint8 select_index);

/*************************************************************************************/
//  Description : update main menu list
//  Global resource dependence : none
//  Author: jian.ma
//  Note:  
/**************************************************************************************/
PUBLIC void MMIENVSET_UpdateMainMenuList(MMI_CTRL_ID_T ctrl_id);

//

/*****************************************************************************/
//Description : �����𶯼�ʱ��,�ü�ʱ����ʾÿ����
//Global resource dependence : 
//Author: maryxiao
//Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_StartWinOnetimeVibTimer(MMI_HANDLE_T win_id);

/*****************************************************************************/
//Description : �ж��Ƿ��𶯼�ʱ��ʱ�䵽 
//Global resource dependence : 
//Author: xiaoqing.lu
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIENVSET_IsOnetimeVibTimerOut(DPARAM param);

/*****************************************************************************/
//Description : �����𶯼�ʱ��,�ü�ʱ����ʾÿ����
//Global resource dependence : 
//Author: xiaoqing.lu 
//Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_StartOnetimeVibTimer(void);

/*****************************************************************************/
//Description : �����𶯼�ʱ��,�ü�ʱ����ʾÿ����
//Global resource dependence : 
//Author: maryxiao
//Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_StartWinOnetimeVibTimer(MMI_HANDLE_T win_id);

/*****************************************************************************/
//  Description : create more ring dir if not exist
//  Global resource dependence : none
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIENVSET_CreateMoreRingDir(void);

/*****************************************************************************/
//  Description : create more ring ini file if not exist
//  Global resource dependence : none
//  Author: jianhui.luo
//  Note:
/*****************************************************************************/
PUBLIC void MMIENVSET_CreateMoreRingFile(void);

/*****************************************************************************/
// 	Description : to enter ring setting menu
//	Global resource dependence : 
//  Author:xiangjie
//	Note: 
/*****************************************************************************/
PUBLIC void MMISET_EnterRingSettingMenu(void);

/**--------------------------------------------------------------------------*
**                         Compiler Flag                                    *
**--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif
    
#endif
