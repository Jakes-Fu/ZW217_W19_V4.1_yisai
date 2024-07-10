/******************************************************************************
 ** File Name:      dal_keypad.h                                              *
 ** Author:         Richard.Yang                                              *
 ** DATE:           10/11/2004                                                *
 ** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME            DESCRIPTION                               *
 ** 10/11/2004      Richard.Yang   Create.                                   *
 ******************************************************************************/
#ifndef _DAL_KEYPAD_H
#define _DAL_KEYPAD_H

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "sci_types.h"
#include "sci_service.h"
#include "tasks_id.h"

/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/**---------------------------------------------------------------------------*
 **                         Constant Variables                                *
 **---------------------------------------------------------------------------*/
// Define virtual key code.
#define SCI_VK_0                ('0')
#define SCI_VK_1                ('1')
#define SCI_VK_2                ('2')
#define SCI_VK_3                ('3')
#define SCI_VK_4                ('4')
#define SCI_VK_5                ('5')
#define SCI_VK_6                ('6')
#define SCI_VK_7                ('7')
#define SCI_VK_8                ('8')
#define SCI_VK_9                ('9')
#define SCI_VK_STAR             ('*')
#define SCI_VK_POUND            ('#')
#define SCI_VK_AT               ('@')     //added by yuehz 20060406
                                
#define SCI_VK_CALL             (0x01)
#define SCI_VK_POWER            (0x02)    // If a key is used as both SCI_VK_POWER and SCI_VK_CANCEL, only use SCI_VK_POWER
#define SCI_VK_CANCEL           (0x03)    // If a key is used as both SCI_VK_POWER and SCI_VK_CANCEL, only use SCI_VK_POWER
#define SCI_VK_UP               (0x04)
#define SCI_VK_DOWN             (0x05)
#define SCI_VK_LEFT             (0x06)
#define SCI_VK_RIGHT            (0x07)
                                
#define SCI_VK_MENU_SELECT      (0x08)
#define SCI_VK_MENU_CANCEL      (0x09)
#define SCI_VK_SIDE_UP          (0x0A)
#define SCI_VK_SIDE_DOWN        (0x0B)
#define SCI_VK_OK               (0x0C)
#define SCI_VK_WEB              (0x0D)
#define SCI_VK_CAMERA           (0x0E)

#define SCI_CUSTEMIZE_KEY_BASE  (0x10)

#define SCI_GPIO_SIG1           (0x10)
#define SCI_GPIO_SIG2           (0x11)

#define SCI_VK_FLIP             (SCI_CUSTEMIZE_KEY_BASE + 2)
#define SCI_VK_HEADSET_BUTTOM   (SCI_CUSTEMIZE_KEY_BASE + 3)
#define SCI_VK_HEADSET_DETECT   (SCI_CUSTEMIZE_KEY_BASE + 4)
#define SCI_VK_SDCARD_DETECT    (SCI_CUSTEMIZE_KEY_BASE + 5)
#define SCI_VK_PAUSE            (SCI_CUSTEMIZE_KEY_BASE + 6)
#define SCI_VK_REWIND           (SCI_CUSTEMIZE_KEY_BASE + 7)
#define SCI_VK_FASTFORWARD      (SCI_CUSTEMIZE_KEY_BASE + 8)
#define SCI_VK_SLIDE            (SCI_CUSTEMIZE_KEY_BASE + 9)
#define SCI_VK_TV            	(SCI_CUSTEMIZE_KEY_BASE + 10)
#define SCI_VK_CALL2            (SCI_CUSTEMIZE_KEY_BASE + 11)
#define SCI_VK_CALL3            (SCI_CUSTEMIZE_KEY_BASE + 12)
#define SCI_VK_MP3              (SCI_CUSTEMIZE_KEY_BASE + 13)
#define SCI_VK_NOTES            (SCI_CUSTEMIZE_KEY_BASE + 14)
#define SCI_VK_SMS              (SCI_CUSTEMIZE_KEY_BASE + 15)
#define SCI_VK_CALENDER		    (SCI_CUSTEMIZE_KEY_BASE + 16)
#define SCI_VK_IE               (SCI_CUSTEMIZE_KEY_BASE + 17)
#define SCI_VK_HANG             (SCI_CUSTEMIZE_KEY_BASE + 18)
#define SCI_VK_VOL_UP           (SCI_CUSTEMIZE_KEY_BASE + 20)
#define SCI_VK_VOL_DOWN         (SCI_CUSTEMIZE_KEY_BASE + 21)

#define SCI_VK_MO                  (SCI_CUSTEMIZE_KEY_BASE + 22)

#ifdef DPHONE_SUPPORT	// zhiguo.li_cr225830
#define SCI_VK_HOOK             (SCI_CUSTEMIZE_KEY_BASE + 23)
#define SCI_VK_HANDFREE		(SCI_CUSTEMIZE_KEY_BASE + 24)
#endif

#define SCI_VK_TOUCHPANEL	    (0x2F)
                                
/*for qwery keypad*/            
#define SCI_QWERTY_KEY_BASE     (0x40)
                                
#define SCI_VK_Q                ( SCI_QWERTY_KEY_BASE + 0x01 )
#define SCI_VK_W                ( SCI_QWERTY_KEY_BASE + 0x02 )
#define SCI_VK_E                ( SCI_QWERTY_KEY_BASE + 0x03 )
#define SCI_VK_R                ( SCI_QWERTY_KEY_BASE + 0x04 )
#define SCI_VK_T                ( SCI_QWERTY_KEY_BASE + 0x05 )
#define SCI_VK_Y                ( SCI_QWERTY_KEY_BASE + 0x06 )
#define SCI_VK_U                ( SCI_QWERTY_KEY_BASE + 0x07 )
#define SCI_VK_I                ( SCI_QWERTY_KEY_BASE + 0x08 )
#define SCI_VK_O                ( SCI_QWERTY_KEY_BASE + 0x09 )
#define SCI_VK_P                ( SCI_QWERTY_KEY_BASE + 0x0a )
#define SCI_VK_A                ( SCI_QWERTY_KEY_BASE + 0x0b )
#define SCI_VK_S                ( SCI_QWERTY_KEY_BASE + 0x0c )
#define SCI_VK_D                ( SCI_QWERTY_KEY_BASE + 0x0d )
#define SCI_VK_F                ( SCI_QWERTY_KEY_BASE + 0x0e )
#define SCI_VK_G                ( SCI_QWERTY_KEY_BASE + 0x0f )
#define SCI_VK_H                ( SCI_QWERTY_KEY_BASE + 0x10 )
#define SCI_VK_J                ( SCI_QWERTY_KEY_BASE + 0x11 )
#define SCI_VK_K                ( SCI_QWERTY_KEY_BASE + 0x12 )
#define SCI_VK_L                ( SCI_QWERTY_KEY_BASE + 0x13 )
#define SCI_VK_DEL              ( SCI_QWERTY_KEY_BASE + 0x14 )	  
#define SCI_VK_Z                ( SCI_QWERTY_KEY_BASE + 0x15 )
#define SCI_VK_X                ( SCI_QWERTY_KEY_BASE + 0x16 )
#define SCI_VK_C                ( SCI_QWERTY_KEY_BASE + 0x17 )
#define SCI_VK_V                ( SCI_QWERTY_KEY_BASE + 0x18 )
#define SCI_VK_B                ( SCI_QWERTY_KEY_BASE + 0x19 )
#define SCI_VK_N                ( SCI_QWERTY_KEY_BASE + 0x1a )
#define SCI_VK_M                ( SCI_QWERTY_KEY_BASE + 0x1b )
#define SCI_VK_COMMA            ( SCI_QWERTY_KEY_BASE + 0x1c )
#define SCI_VK_PERIOD           ( SCI_QWERTY_KEY_BASE + 0x1d )
#define SCI_VK_ENTER            ( SCI_QWERTY_KEY_BASE + 0x1e )	//SCI_VK_OK	// ENTER = 0K
#define SCI_VK_FN    	        ( SCI_QWERTY_KEY_BASE + 0x1f )
#define SCI_VK_SHIFT	        ( SCI_QWERTY_KEY_BASE + 0x20 )
#define SCI_VK_AT_QWERTY        ( SCI_QWERTY_KEY_BASE + 0X21 )
#define SCI_VK_SPACE	        ( SCI_QWERTY_KEY_BASE + 0x22 )
#define SCI_VK_AND		        ( SCI_QWERTY_KEY_BASE + 0x23 )
#define SCI_VK_QUESTION	        ( SCI_QWERTY_KEY_BASE + 0x24 )
#define SCI_VK_CTRL		        ( SCI_QWERTY_KEY_BASE + 0x25 )
#define SCI_VK_ALT    	        ( SCI_QWERTY_KEY_BASE + 0x26 )
#define SCI_VK_EXCLAMATION      ( SCI_QWERTY_KEY_BASE + 0x27 )

#define SCI_VK_SHORTCUT  (0X6F)

#define 	KPD_VK_NUM		    (0x1000)
#define SCI_VK_GPIO_KEY         (0x2000)
#define SCI_VK_DIAG_KEY         (0x4000)
#define SCI_VK_INVALID_KEY      (0xFFFF)

// Below definition is for D-Phone
#define SCI_VK_EXT1             SCI_VK_MENU_SELECT
#define SCI_VK_EXT2             SCI_VK_MENU_CANCEL
#define SCI_VK_EXT3             SCI_VK_SIDE_UP
#define SCI_VK_EXT4             SCI_VK_SIDE_DOWN
#define SCI_VK_EXT5             SCI_VK_OK

#define SCI_GPIO_SIG_FLIP               SCI_VK_FLIP
#define SCI_GPIO_SIG_HEADSET_BUTTIN     SCI_VK_HEADSET_BUTTOM
#define SCI_GPIO_SIG_HEADSET_DETECT     SCI_VK_HEADSET_DETECT
// End of definition for D_Phone

// Definition for Keypad message
typedef enum
{
	// Keypad message.
	KPD_DOWN = (KPDSVR << 8) | 1 ,
	KPD_UP,
	KPD_RESERVED1,          // User cann't use it
	KPD_SOFT_POWER_ON,
    KPD_SOFT_POWER_OFF,
    KPD_RESERVED2,          // User cann't use it
	KPD_MSG_MAX
} KPDSVR_MSG_SERVICE_E;

#define KPD_MSG_MAX_NUM     (KPD_MSG_MAX & 0x00FF)
// End of definition for keypad message

//@Robert.Lu, 2005.03.16
typedef enum
{
	// Keypad message.
#ifdef WIN32
	TP_DOWN =  0x211 ,
#else
	TP_DOWN =  (TP_SERVICE << 8) | 1 ,
#endif
	TP_UP,
	TP_MOVE,
    TP_GESURE,
	TP_NOTMOVE,
	TP_DRAG,
    TP_LONG,
	TP_DCLICK,
    TP_RESERVED1,          // User cann't use it
    TP_RESERVED2,          // User cann't use it
	TP_MSG_MAX
} TOUCHPANEL_MSG_SERVICE_E;

#define TP_MSG_MAX_NUM     (TP_MSG_MAX & 0x00FF)
// End of definition for touch panel keypad message
//@Robert.Lu

typedef struct
{
    int32     left;       
    int32     top;       
    int32     right;    
    int32     bottom;  
}VIR_KEY_RECT_T;

typedef struct
{
    VIR_KEY_RECT_T      rect;
    uint32      vir_key_code;  
}VIR_KEY_T;

/*****************************************************************************/
//  Description:    This function retrieves the status of the specified virtual key
//                  The status specifies whether the key is up, down.
//  Author:         Jimmy.Jia
//	Note:           
//  Input:			// virtual-key code
//  Return:			1: down 0: up
/*****************************************************************************/
PUBLIC uint32 KPDSVR_GetKeyState (uint16 nVirtKey);
PUBLIC const VIR_KEY_T* KPD_GetVirKeyMap( uint32* virkey_size);


/**---------------------------------------------------------------------------*
 **                         Compiler Flag                                     *
 **---------------------------------------------------------------------------*/
#ifdef __cplusplus
    }
#endif

#endif  // _DAL_KEYPAD_H
