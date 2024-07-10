/*****************************************************************************
** File Name:      mmidc_guiiconbar.h                                                                                *
** Author:                                                                                                                       *
** Date:           11/12/2011                                                                                             *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.                                  *
** Description:    This file is used to camera pda                                                                 *
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 11/2011       robert.wang         Create
******************************************************************************/

#ifndef _MMIDC_GUIICONBAR_H_
#define _MMIDC_GUIICONBAR_H_ 
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/


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
#if defined(MMIDC_F_DV_ZOOM)
#if defined(MAINLCD_SIZE_320X480)
    #define ICON_ZOOM_H_X 300
    #define ICON_ZOOM_H_Y 3
    #define ICON_ZOOM_V_X 200
    #define ICON_ZOOM_V_Y 3
#elif defined(MAINLCD_SIZE_240X400)
    #define ICON_ZOOM_H_X 250
    #define ICON_ZOOM_H_Y 3
    #define ICON_ZOOM_V_X 147
    #define ICON_ZOOM_V_Y 3
#else
    #define ICON_ZOOM_H_X 153
    #define ICON_ZOOM_H_Y 3
    #define ICON_ZOOM_V_X 151
    #define ICON_ZOOM_V_Y 1
#endif
#endif

 /**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
 
 typedef enum
{
    ICON_IMG_BRIGHTNESS,
    ICON_IMG_BRIGHT_UP,
    ICON_IMG_GRAY_BRIGHT_UP,
    ICON_IMG_FOCUS_BRIGHT_UP,
    ICON_IMG_BRIGHT_DOWN,
    ICON_IMG_GRAY_BRIGHT_DOWN,
    ICON_IMG_FOCUS_BRIGHT_RIGHT,
    ICON_IMG_ZOOM,
    ICON_IMG_CONTRAST,
    ICON_IMG_UP,
    ICON_IMG_GRAY_UP,
    ICON_IMG_FOCUS_UP,
    ICON_IMG_DOWN,
    ICON_IMG_GRAY_DOWN,
    ICON_IMG_FOCUS_DOWN,
    ICON_IMG_PHOTO,
    ICON_IMG_VIDEO,
    ICON_IMG_SENSOR_REAR,
    ICON_IMG_SENSOR_FRONT,
    ICON_IMG_SIZE,
    ICON_IMG_NIGHT,
    ICON_IMG_NIGHT_OFF,
    ICON_IMG_EF_NORMAL,    
    ICON_IMG_EF_BLACKWHITE,
    ICON_IMG_EF_RED,       
    ICON_IMG_EF_GREEN,     
    ICON_IMG_EF_BLUE,      
    ICON_IMG_EF_YELLOW,    
    ICON_IMG_EF_NEGATIVE,  
    ICON_IMG_EF_CANVAS,    
    ICON_IMG_WB_INCANDESCENCE,
    ICON_IMG_WB_SUN,
    ICON_IMG_WB_CLOUDY,
    ICON_IMG_WB_FLUORESCENT,
    ICON_IMG_WB_AUTO,
    ICON_IMG_10S,
    ICON_IMG_3S,
    ICON_IMG_DELAY_OFF,
    ICON_IMG_NO_MULTI,
    ICON_IMG_3_MULTI,
    ICON_IMG_6_MULTI,
    ICON_IMG_MAX
}MMIDC_ICON_IMG_E;

typedef enum
{
    ICON_RECT_LEFT,
    ICON_RECT_RIGHT,
    ICON_RECT_UP,
    ICON_RECT_DOWN,
    ICON_RECT_CAPTURE,
    ICON_RECT_RECORDER,
    ICON_RECT_SENSOR,
    ICON_RECT_SIZE,
    ICON_RECT_NIGHT, 
    ICON_RECT_EFFECT = ICON_RECT_NIGHT, //same region 
    ICON_RECT_WB,
    ICON_RECT_DELAY,
    ICON_RECT_STORE,
    ICON_RECT_MULTI,
    ICON_RECT_BRIGHTNESS,
    ICON_RECT_ZOOM,
    ICON_RECT_CONTRAST = ICON_RECT_ZOOM, // same region
    ICON_RECT_MODE,
    ICON_RECT_MAX
}MMIDC_ICON_RECT_E;

typedef struct
{
    MMIDC_ICON_RECT_E rect_index;
    GUI_RECT_T rect;
}MMIDC_ICON_DISP_T;

typedef struct
{
    BOOLEAN is_open;
    int16 icon_disp_number;
    MMIDC_ICON_DISP_T  icon_disp[ICON_RECT_MAX];
}MMIDC_GUI_DESKTOP_ICON_T;

 /**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
#ifdef MMIDC_DESKTOP_ICON_BAR
/*****************************************************************************/
//Description :  get icon desktop rect list
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC int16 MMIDC_GetIconDesktopRectList(int16 max_number, 
                                                    MMIDC_ICON_DISP_T *disp_list //out
                                                    );

/*****************************************************************************/
//Description :  shortcut for capture rect by TP
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForCapture(void);

/*****************************************************************************/
//Description :  shortcut for recorder rect by TP
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForRecorder(void);

/*****************************************************************************/
//Description :  shortcut for camera mode
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForCameraMode(void);

/*****************************************************************************/
//Description :  shortcut for camera mode
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForSize(void);

/*****************************************************************************/
//Description :  shortcut for environment
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForEnvironment(void);

/*****************************************************************************/
//Description :  shortcut for white balance
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForWhiteBalance(void);

/*****************************************************************************/
//Description :  shortcut for delay time
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForDelayTime(void);

/*****************************************************************************/
//Description :  shortcut for store
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForStore(void);

/*****************************************************************************/
//Description :  shortcut for brightness to high
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForBrightnessToHigh(void);

/*****************************************************************************/
//Description :  shortcut for contrast to high
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForContrastToHigh(void);

/*****************************************************************************/
//Description :  shortcut for brightness to low
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForBrightnessToLow(void);

/*****************************************************************************/
//Description :  shortcut for contrast to low
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForContrastToLow(void);

/*****************************************************************************/
//Description :  shortcut for zoom to high
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForZoomToHigh(void);

/*****************************************************************************/
//Description :  shortcut for zoom to low
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForZoomToLow(void);

/*****************************************************************************/
//Description :  shortcut for effect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForEffect(void);

/*****************************************************************************/
//Description :  shortcut for multi shot
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ShortcutForMultiShot(void);
#endif

/*****************************************************************************/
//Description :  clear icon desktop rect list
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ClearIconDesktopRectList(void);

/*****************************************************************************/
//Description :  close GUI icon desktop data
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseGUIIconDesktop(void);

/*****************************************************************************/
//Description :   open icon desktop
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenIconDesktop(void);

/*****************************************************************************/
//Description :   GUI icon desktop is open or not
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsIconDesktopOpen(void);

/*****************************************************************************/
//Description :  display icon desktop
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DisplayIconDesktop(void);

/*****************************************************************************/
//Description : set icon desktop visible or not
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetIconDesktopVisibility(BOOLEAN is_visible);

/*****************************************************************************/
//Description :   Open OSD menu
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenMoreOption(void);

#if defined(MMIDC_F_DV_ZOOM)
/*****************************************************************************/
//Description : display zoom button
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayZoomButton(void);

/*****************************************************************************/
//Description : get zoom button rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_GetZoomButtonRect(GUI_RECT_T * rect_ptr);
#endif

 /**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIDC_GUIICONBAR_H_

 
