/*****************************************************************************
** File Name:      mmidc_internal.h                                          *
** Author:                                                                   *
** Date:           02/28/2012                                                *
** Copyright:      2012 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to camera module                        *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 02/2012       robert.wang         Create
******************************************************************************/

#ifndef _MMIDC_INTERNAL_H_
#define _MMIDC_INTERNAL_H_


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


/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef enum
{
    UP_KEY,
    DOWN_KEY,
    LEFT_KEY,
    RIGHT_KEY,
    OK_KEY,
    CANCEL_KEY,
    S_1_KEY ,
    S_2_KEY,
    S_3_KEY,
    S_4_KEY,
    S_5_KEY,
    S_6_KEY,
    S_7_KEY,
    S_8_KEY,
    S_9_KEY,        
    S_0_KEY,
    DIR_KEY_MAX
}DIRECTION_KEY;

/*
item list display direction
for map
HORIZONTAL : 1 2 3 4 5 
VERTICAL: 1
          2
          3
          4
          5
*/
typedef enum
{
    DISPLAY_DIRECT_VERTICAL = 0,
    DISPLAY_DIRECT_HORIZONTAL,
    DISPLAY_DIRECT_MAX
}DISPLAY_DIRECT_E;

typedef void (*ICON_CALL_BACK)(uint32 x, uint32 y);
typedef void (*ICON_OK_CALL_BACK)(void);

typedef struct 
{
    void                                (*IconDisplay)(void);
    BOOLEAN                             (*IconHandleTpDown)(uint32 x, uint32 y);
    BOOLEAN                             (*IconHandleKeyDown)(DIRECTION_KEY key);
    void                                    (*IconClose)(void);
    BOOLEAN                             (*IconIsOpen)(void);
    
    void                                (*IconSetStartPositionX)(int32 x);
    void                                (*IconSetStartPositionY)(int32 y);
    void                                (*IconAppendImage)(int32 image_data, int32 focus_image_data);
    void                                (*IconSetImage)(int32 index, int32 image_data, int32 focus_image_data);
    
    void                                (*IconFocusNext)(void);
    void                                (*IconFocusPrevious)(void);
    void                                (*IconFocus)(int32 index);
    void                                (*IconSetCallback)(int32 index, ICON_CALL_BACK call_back);
    void                                (*IconSetOKCallback)(int32 index, ICON_OK_CALL_BACK ok_call_back);
    void                                (*IconSetDisplayMode)(DISPLAY_DIRECT_E display_mode);
    int32                               (*IconGetDisplayNumber)(void);
    int32                               (*IconGetIconWidth)(void);
    int32                               (*IconGetFocusIconWidth)(void);
    int32                               (*IconGetFocusIconHeight)(void);
    void                                (*IconClearRect)(void);
}DC_ICONS_OP;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //#ifndef _MMIDC_INTERNAL_H_






