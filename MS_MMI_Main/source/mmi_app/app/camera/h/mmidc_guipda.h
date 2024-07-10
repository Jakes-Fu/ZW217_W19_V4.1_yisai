/*****************************************************************************
** File Name:      mmidc_guipda.h                                            *
** Author:                                                                   *
** Date:           07/16/2011                                                *
** Copyright:      2011 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to camera pda                           *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 07/2011       robert.wang         Create
******************************************************************************/

#ifndef _MMIDC_GUIPDA_H_
#define _MMIDC_GUIPDA_H_ 
/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "guianim.h"
#include "mmidc_setting.h"

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

#define PDA_ITEM_MAX PHOTO_SIZE_MAX


#define PDA_SEEKBAR_TIMER  10 //ms
#define PDA_SEEKBAR_DISPLAY_TIMER  3000 //ms
#define PDA_SEEKBAR_MAP_UNIT 2 //move 1 ,skip 2 step

#define OSD_ANIM_TIMER 10 //MS

#define  MAX_PDA_SETTING_ITEM_NUMBER      10
#define  MAX_PDA_SETTING_VAL_SET_NUMBERT  6
#define  ABS(x) ((x) > 0 ? (x) : -(x))

#define MMIDC_ANIM_VERTICAL_SLEEP_TIME 20

#define MMIDC_PDA_LIST_BG_COLOR 0x18C3

#if defined(MMIDC_F_EFFECT_360)
#define MMIDC_PDA_EFFECT_360_ARROW_SPACE 5
#endif

/*
@ th macro are defined as order
*/
#if defined(MAINLCD_SIZE_320X480)
    /* 
        ver and hor screen
    */
    //button list
    #define BUTTONLIST_WIDTH         60
    #define BUTTONLIST_HEIGHT        (5 * 64)
    
    #define SEEKBAR_FRAME_WIDTH       39

    //seekbar
    #define SEEKBAR_BG_INTERVAL_X  9      //to frame space
    #define SEEKBAR_BG_INTERVAL_Y  47    
    #define SEEKBAR_MASK_INTERVAL_X 11    //to frame space
    #define SEEKBAR_MASK_INTERVAL_Y 49
    #define SEEKBAR_SCALE_INTERVAL_V_SPACE 6  //to bg space
    #define SEEKBAR_SCALE_INTERVAL_H_SPACE 2 //to bg space
    #define SEEKBAR_SCALE_CALIBRATE_VALUE 0 //up or down to bg
    #define SEEKBAR_SCALE_FOCUS_LINE_HEIGHT 2 //line height

    #define PIC_PROPERTY_SEEKBAR_Y_OFFSET  72
    #define PIC_PROPERTY_BETWEEN_SEEKBAR_X_OFFSET 29
    #define DV_PIC_PROPERTY_BETWEEN_SEEKBAR_X_OFFSET 29
    
    #define PIC_PROPERTY_BETWEEN_ICON_X_OFFSET    35
    #define DV_PIC_PROPERTY_BETWEEN_ICON_X_OFFSET 35

    
    /*
        hor screen
    */
    #define EXPAND_MOVE_LEN          53
    #define BOUNDARY_WIDTH           3
    #define LIST_WIDTH               230
    #define LIST_HEIGHT              320
    #define MOVE_WIDTH               (LIST_WIDTH + BUTTONLIST_WIDTH + BOUNDARY_WIDTH)
    #define EXPAND_BTN_WIDTH         30

    #define PIC_PROPERTY_BUTTONLIST_H_OFFSET 8


    #define BTN_CAPTURE_H_X     429
    #define BTN_CAPTURE_H_Y     69
    #define BTN_SENSOR_H_X      373
    #define BTN_SENSOR_H_Y      13
    #define BTN_ZOOM_H_X        429
    #define BTN_ZOOM_H_Y        13

#if defined (MMIDC_F_U_CAMERA)
    #define BTN_UCAMERA_H_X     429
    #define BTN_UCAMERA_H_Y     171
#endif
    
    //pic button
    #define BTN_PIC_H_X         408
    #define BTN_PIC_H_Y         256
    #define BTN_PIC_IMG_INTERVAL_X 11
    #define BTN_PIC_IMG_INTERVAL_Y 11
    #define BTN_PIC_IMG_WIDTH 80//38
    #define BTN_PIC_IMG_HEIGHT 60//46

#if defined (MMIDC_F_N_IN_ONE)
    #define BTN_PIC_NINONE_H_X         349
    #define BTN_PIC_NINONE_H_Y         5
#endif


    //cmd button
    #define BTN_CMD_INTERVAL_H_Y 24

    #define BTN_ARROW_LEFT_H_X 324
    #define BTN_ARROW_LEFT_H_Y 32
    #define BTN_ARROW_INTERVAL_H_X 2

    #define BTN_HIDE_EXPAND_H_LEFT      0
    #define BTN_HIDE_EXPAND_H_TOP       110

    #define BTN_SHOW_EXPAND_H_LEFT      (BTN_HIDE_EXPAND_H_LEFT + MOVE_WIDTH)
    #define BTN_SHOW_EXPAND_H_TOP       110


    //button list, mode
    #define BTN_SHOW_MODE_H_LEFT     0
    #define BTN_SHOW_MODE_H_TOP      0
    
 
    //zoom seekbar
    #define ZOOM_SEEKBAR_FRAME_H_X      0
    #define ZOOM_SEEKBAR_FRAME_H_Y      0

    
    //brightness icon   
    #define PIC_PROPERTY_BRIGHTNESS_H_X      (24 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_H_OFFSET)
    #define PIC_PROPERTY_BRIGHTNESS_H_Y      34
    
    #define DV_PIC_PROPERTY_BRIGHTNESS_H_X   (54 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_H_OFFSET)
    #define DV_PIC_PROPERTY_BRIGHTNESS_H_Y    PIC_PROPERTY_BRIGHTNESS_V_Y

    
    //brightness seekbar
    #define BRIGHTNESS_SEEKBAR_FRAME_H_X      (14 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_H_OFFSET)
    #define BRIGHTNESS_SEEKBAR_FRAME_H_Y      PIC_PROPERTY_SEEKBAR_Y_OFFSET
    
    #define DV_BRIGHTNESS_SEEKBAR_FRAME_H_X   (44 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_H_OFFSET)
    #define DV_BRIGHTNESS_SEEKBAR_FRAME_H_Y   BRIGHTNESS_SEEKBAR_FRAME_H_Y

   
    /*
        vertical screen 
    */
    #define EXPAND_MOVE_LEN_V       48
    #define BOUNDARY_HEIGHT_V       3
    #define LIST_WIDTH_V            260
    #define LIST_HEIGHT_V           320
    #define MOVE_HEIGHT_V           (LIST_HEIGHT_V + BOUNDARY_HEIGHT_V)
    #define EXPAND_BTN_HEIGHT_V     30

    #define PIC_PROPERTY_BUTTONLIST_V_OFFSET 23


    #define BTN_CAPTURE_V_X     168
    #define BTN_CAPTURE_V_Y     430
    #define BTN_SENSOR_V_X      266
    #define BTN_SENSOR_V_Y      378
    #define BTN_ZOOM_V_X        266
    #define BTN_ZOOM_V_Y        430
    #define BTN_PIC_V_X         13
    #define BTN_PIC_V_Y         416

#if defined (MMIDC_F_U_CAMERA)
    #define BTN_UCAMERA_V_X     116
    #define BTN_UCAMERA_V_Y     430
#endif

#if defined (MMIDC_F_N_IN_ONE)
    #define BTN_PIC_NINONE_V_X         249
    #define BTN_PIC_NINONE_V_Y         364
#endif


    #define BTN_HIDE_EXPAND_V_LEFT      110
    #define BTN_HIDE_EXPAND_V_TOP       0

    #define BTN_SHOW_EXPAND_V_LEFT      110
    #define BTN_SHOW_EXPAND_V_TOP       (BTN_HIDE_EXPAND_V_TOP + MOVE_HEIGHT_V)


    //cmd button
    #define BTN_CMD_INTERVAL_V_X 24
    
    #define BTN_ARROW_LEFT_V_X 237


    //zoom seekbar
    #define ZOOM_SEEKBAR_FRAME_V_X      0
    #define ZOOM_SEEKBAR_FRAME_V_Y      0


    #define BTN_SHOW_MODE_V_LEFT     0
    #define BTN_SHOW_MODE_V_TOP      0

    #define BTN_HIDE_MODE_V_LEFT     0
    #define BTN_HIDE_MODE_V_TOP      (-MOVE_HEIGHT_V)

    //brightness icon   
    #define PIC_PROPERTY_BRIGHTNESS_V_X      (24 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_V_OFFSET)
    #define PIC_PROPERTY_BRIGHTNESS_V_Y      34

    #define DV_PIC_PROPERTY_BRIGHTNESS_V_X   (54 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_V_OFFSET)
    #define DV_PIC_PROPERTY_BRIGHTNESS_V_Y    PIC_PROPERTY_BRIGHTNESS_V_Y
    

    //brightness seekbar
    #define BRIGHTNESS_SEEKBAR_FRAME_V_X      (14 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_V_OFFSET)
    #define BRIGHTNESS_SEEKBAR_FRAME_V_Y      PIC_PROPERTY_SEEKBAR_Y_OFFSET
    
    #define DV_BRIGHTNESS_SEEKBAR_FRAME_V_X   (44 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_V_OFFSET)
    #define DV_BRIGHTNESS_SEEKBAR_FRAME_V_Y   BRIGHTNESS_SEEKBAR_FRAME_V_Y

 
#elif defined(MAINLCD_SIZE_240X400)
    /* 
        ver and hor screen
    */
    //button list
    #define BUTTONLIST_WIDTH         45
    #define BUTTONLIST_HEIGHT        (5 * 48)

    
    #define SEEKBAR_FRAME_WIDTH       32


    //seekbar
    #define SEEKBAR_BG_INTERVAL_X  9      //to frame space
    #define SEEKBAR_BG_INTERVAL_Y  31    
    #define SEEKBAR_MASK_INTERVAL_X 9    //to frame space
    #define SEEKBAR_MASK_INTERVAL_Y 31
    #define SEEKBAR_SCALE_INTERVAL_V_SPACE 6  //to bg space
    #define SEEKBAR_SCALE_INTERVAL_H_SPACE 2 //to bg space
    #define SEEKBAR_SCALE_CALIBRATE_VALUE 0 //up or down to bg
    #define SEEKBAR_SCALE_FOCUS_LINE_HEIGHT 2 //line height


    #define PIC_PROPERTY_SEEKBAR_Y_OFFSET  54
    #define PIC_PROPERTY_BETWEEN_SEEKBAR_X_OFFSET 20
    #define DV_PIC_PROPERTY_BETWEEN_SEEKBAR_X_OFFSET 32
    
    #define PIC_PROPERTY_BETWEEN_ICON_X_OFFSET    30
    #define DV_PIC_PROPERTY_BETWEEN_ICON_X_OFFSET 36


    /*
        hor screen
    */
    #define EXPAND_MOVE_LEN          48
    #define BOUNDARY_WIDTH           1
    #define LIST_WIDTH               182
    #define LIST_HEIGHT              240
    #define MOVE_WIDTH               (LIST_WIDTH + BUTTONLIST_WIDTH + BOUNDARY_WIDTH)
    #define EXPAND_BTN_WIDTH         23


    #define PIC_PROPERTY_BUTTONLIST_H_OFFSET 8


    #define BTN_CAPTURE_H_X     360
    #define BTN_CAPTURE_H_Y     55
    #define BTN_SENSOR_H_X      318
    #define BTN_SENSOR_H_Y      8
    #define BTN_ZOOM_H_X        360
    #define BTN_ZOOM_H_Y        8

#if defined (MMIDC_F_U_CAMERA)
    #define BTN_UCAMERA_H_X     360
    #define BTN_UCAMERA_H_Y     133
#endif

    //pic button
    #define BTN_PIC_H_X         345
    #define BTN_PIC_H_Y         193
    #define BTN_PIC_IMG_INTERVAL_X 11
    #define BTN_PIC_IMG_INTERVAL_Y 11
    #define BTN_PIC_IMG_WIDTH 80//38
    #define BTN_PIC_IMG_HEIGHT 60//46

#if defined (MMIDC_F_N_IN_ONE)
    #define BTN_PIC_NINONE_H_X         295
    #define BTN_PIC_NINONE_H_Y         6
#endif
    

    //cmd button
    #define BTN_CMD_INTERVAL_H_Y 24

    #define BTN_ARROW_LEFT_H_X 273
    #define BTN_ARROW_LEFT_H_Y 30
    #define BTN_ARROW_INTERVAL_H_X 2


    #define BTN_HIDE_EXPAND_H_LEFT      0
    #define BTN_HIDE_EXPAND_H_TOP       83

    #define BTN_SHOW_EXPAND_H_LEFT      (BTN_HIDE_EXPAND_H_LEFT + MOVE_WIDTH)
    #define BTN_SHOW_EXPAND_H_TOP       83
    

    //button list, mode
    #define BTN_SHOW_MODE_H_LEFT     0
    #define BTN_SHOW_MODE_H_TOP      0
    

    //zoom seekbar
    #define ZOOM_SEEKBAR_FRAME_H_X      0
    #define ZOOM_SEEKBAR_FRAME_H_Y      0

    
    //brightness icon   
    #define PIC_PROPERTY_BRIGHTNESS_H_X      (18 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_H_OFFSET)
    #define PIC_PROPERTY_BRIGHTNESS_H_Y      27
    
    #define DV_PIC_PROPERTY_BRIGHTNESS_H_X   (40 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_H_OFFSET)
    #define DV_PIC_PROPERTY_BRIGHTNESS_H_Y    PIC_PROPERTY_BRIGHTNESS_V_Y

    
    //brightness seekbar
    #define BRIGHTNESS_SEEKBAR_FRAME_H_X      (8 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_H_OFFSET)
    #define BRIGHTNESS_SEEKBAR_FRAME_H_Y      PIC_PROPERTY_SEEKBAR_Y_OFFSET
    
    #define DV_BRIGHTNESS_SEEKBAR_FRAME_H_X   (28 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_H_OFFSET)
    #define DV_BRIGHTNESS_SEEKBAR_FRAME_H_Y   BRIGHTNESS_SEEKBAR_FRAME_H_Y

   
    /*
        vertical screen 
    */
    #define EXPAND_MOVE_LEN_V       42
    #define BOUNDARY_HEIGHT_V       1
    #define LIST_WIDTH_V            195
    #define LIST_HEIGHT_V           240
    #define MOVE_HEIGHT_V           (LIST_HEIGHT_V + BOUNDARY_HEIGHT_V)
    #define EXPAND_BTN_HEIGHT_V     23

    #define PIC_PROPERTY_BUTTONLIST_V_OFFSET 16


    #define BTN_CAPTURE_V_X     121
    #define BTN_CAPTURE_V_Y     357
    #define BTN_SENSOR_V_X      195
    #define BTN_SENSOR_V_Y      311
    #define BTN_ZOOM_V_X        195
    #define BTN_ZOOM_V_Y        357
    #define BTN_PIC_V_X         7
    #define BTN_PIC_V_Y         350

#if defined (MMIDC_F_U_CAMERA)
    #define BTN_UCAMERA_V_X     78
    #define BTN_UCAMERA_V_Y     357
#endif

#if defined (MMIDC_F_N_IN_ONE)
    #define BTN_PIC_NINONE_V_X         184
    #define BTN_PIC_NINONE_V_Y         300
#endif

    
    #define BTN_HIDE_EXPAND_V_LEFT      83
    #define BTN_HIDE_EXPAND_V_TOP       0

    #define BTN_SHOW_EXPAND_V_LEFT      83
    #define BTN_SHOW_EXPAND_V_TOP       (BTN_HIDE_EXPAND_V_TOP + MOVE_HEIGHT_V)


    //cmd button
    #define BTN_CMD_INTERVAL_V_X 24
    
    #define BTN_ARROW_LEFT_V_X 168


    //zoom seekbar
    #define ZOOM_SEEKBAR_FRAME_V_X      0
    #define ZOOM_SEEKBAR_FRAME_V_Y      0


    #define BTN_SHOW_MODE_V_LEFT     0
    #define BTN_SHOW_MODE_V_TOP      0

    #define BTN_HIDE_MODE_V_LEFT     0
    #define BTN_HIDE_MODE_V_TOP      (-MOVE_HEIGHT_V)

    //brightness icon   
    #define PIC_PROPERTY_BRIGHTNESS_V_X      (18 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_V_OFFSET)
    #define PIC_PROPERTY_BRIGHTNESS_V_Y      27

    #define DV_PIC_PROPERTY_BRIGHTNESS_V_X   (40 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_V_OFFSET)
    #define DV_PIC_PROPERTY_BRIGHTNESS_V_Y    PIC_PROPERTY_BRIGHTNESS_V_Y
    

    //brightness seekbar
    #define BRIGHTNESS_SEEKBAR_FRAME_V_X      (8 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_V_OFFSET)
    #define BRIGHTNESS_SEEKBAR_FRAME_V_Y      PIC_PROPERTY_SEEKBAR_Y_OFFSET
    
    #define DV_BRIGHTNESS_SEEKBAR_FRAME_V_X   (28 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_V_OFFSET)
    #define DV_BRIGHTNESS_SEEKBAR_FRAME_V_Y   BRIGHTNESS_SEEKBAR_FRAME_V_Y

   
#else
    /* 
        ver and hor screen
    */
    //button list
    #define BUTTONLIST_WIDTH         45
    #define BUTTONLIST_HEIGHT        (5 * 48)

    
    #define SEEKBAR_FRAME_WIDTH       32


    //seekbar
    #define SEEKBAR_BG_INTERVAL_X  9      //to frame space
    #define SEEKBAR_BG_INTERVAL_Y  31    
    #define SEEKBAR_MASK_INTERVAL_X 9    //to frame space
    #define SEEKBAR_MASK_INTERVAL_Y 31
    #define SEEKBAR_SCALE_INTERVAL_V_SPACE 6  //to bg space
    #define SEEKBAR_SCALE_INTERVAL_H_SPACE 2 //to bg space
    #define SEEKBAR_SCALE_CALIBRATE_VALUE 0 //up or down to bg
    #define SEEKBAR_SCALE_FOCUS_LINE_HEIGHT 2 //line height


    #define PIC_PROPERTY_SEEKBAR_Y_OFFSET  54
    #define PIC_PROPERTY_BETWEEN_SEEKBAR_X_OFFSET 20
    #define DV_PIC_PROPERTY_BETWEEN_SEEKBAR_X_OFFSET 32
    
    #define PIC_PROPERTY_BETWEEN_ICON_X_OFFSET    27
    #define DV_PIC_PROPERTY_BETWEEN_ICON_X_OFFSET 38


    /*
        hor screen
    */
    #define EXPAND_MOVE_LEN          48
    #define BOUNDARY_WIDTH           1
    #define LIST_WIDTH               182
    #define LIST_HEIGHT              240
    #define MOVE_WIDTH               (LIST_WIDTH + BUTTONLIST_WIDTH + BOUNDARY_WIDTH)
    #define EXPAND_BTN_WIDTH         23


    #define PIC_PROPERTY_BUTTONLIST_H_OFFSET 8


    #define BTN_CAPTURE_H_X     280
    #define BTN_CAPTURE_H_Y     55
    #define BTN_SENSOR_H_X      238
    #define BTN_SENSOR_H_Y      8
    #define BTN_ZOOM_H_X        280
    #define BTN_ZOOM_H_Y        8

    #if defined (MMIDC_F_U_CAMERA)
    #define BTN_UCAMERA_H_X     280
    #define BTN_UCAMERA_H_Y     133
#endif

    //pic button
    #define BTN_PIC_H_X         265
    #define BTN_PIC_H_Y         193
    #define BTN_PIC_IMG_INTERVAL_X 11
    #define BTN_PIC_IMG_INTERVAL_Y 11
    #define BTN_PIC_IMG_WIDTH 80//38
    #define BTN_PIC_IMG_HEIGHT 60//46

#if defined (MMIDC_F_N_IN_ONE)
    #define BTN_PIC_NINONE_H_X         215
    #define BTN_PIC_NINONE_H_Y         6
#endif


    //cmd button
    #define BTN_CMD_INTERVAL_H_Y 24

    #define BTN_ARROW_LEFT_H_X 193
    #define BTN_ARROW_LEFT_H_Y 30
    #define BTN_ARROW_INTERVAL_H_X 2


    #define BTN_HIDE_EXPAND_H_LEFT      0
    #define BTN_HIDE_EXPAND_H_TOP       83

    #define BTN_SHOW_EXPAND_H_LEFT      (BTN_HIDE_EXPAND_H_LEFT + MOVE_WIDTH)
    #define BTN_SHOW_EXPAND_H_TOP       83
    

    //button list, mode
    #define BTN_SHOW_MODE_H_LEFT     0
    #define BTN_SHOW_MODE_H_TOP      0
    

    //zoom seekbar
    #define ZOOM_SEEKBAR_FRAME_H_X      0
    #define ZOOM_SEEKBAR_FRAME_H_Y      0

    
    //brightness icon   
    #define PIC_PROPERTY_BRIGHTNESS_H_X      (20 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_H_OFFSET)
    #define PIC_PROPERTY_BRIGHTNESS_H_Y      27
    
    #define DV_PIC_PROPERTY_BRIGHTNESS_H_X   (40 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_H_OFFSET)
    #define DV_PIC_PROPERTY_BRIGHTNESS_H_Y    PIC_PROPERTY_BRIGHTNESS_V_Y

    
    //brightness seekbar
    #define BRIGHTNESS_SEEKBAR_FRAME_H_X      (8 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_H_OFFSET)
    #define BRIGHTNESS_SEEKBAR_FRAME_H_Y      PIC_PROPERTY_SEEKBAR_Y_OFFSET
    
    #define DV_BRIGHTNESS_SEEKBAR_FRAME_H_X   (28 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_H_OFFSET)
    #define DV_BRIGHTNESS_SEEKBAR_FRAME_H_Y   BRIGHTNESS_SEEKBAR_FRAME_H_Y

   
    /*
        vertical screen 
    */
    #define EXPAND_MOVE_LEN_V       42
    #define BOUNDARY_HEIGHT_V       1
    #define LIST_WIDTH_V            195
    #define LIST_HEIGHT_V           240
    #define MOVE_HEIGHT_V           (LIST_HEIGHT_V + BOUNDARY_HEIGHT_V)
    #define EXPAND_BTN_HEIGHT_V     23

    #define PIC_PROPERTY_BUTTONLIST_V_OFFSET 16


    #define BTN_CAPTURE_V_X     121
    #define BTN_CAPTURE_V_Y     277
    #define BTN_SENSOR_V_X      195
    #define BTN_SENSOR_V_Y      231
    #define BTN_ZOOM_V_X        195
    #define BTN_ZOOM_V_Y        277
    #define BTN_PIC_V_X         7
    #define BTN_PIC_V_Y         270

#if defined (MMIDC_F_U_CAMERA)
    #define BTN_UCAMERA_V_X     78
    #define BTN_UCAMERA_V_Y     277
#endif

#if defined (MMIDC_F_N_IN_ONE)
    #define BTN_PIC_NINONE_V_X         184
    #define BTN_PIC_NINONE_V_Y         220
#endif


    #define BTN_HIDE_EXPAND_V_LEFT      83
    #define BTN_HIDE_EXPAND_V_TOP       0
    
    #define BTN_SHOW_EXPAND_V_LEFT      83
    #define BTN_SHOW_EXPAND_V_TOP       (BTN_HIDE_EXPAND_V_TOP + MOVE_HEIGHT_V)


    //cmd button
    #define BTN_CMD_INTERVAL_V_X 24
    
    #define BTN_ARROW_LEFT_V_X 168


    //zoom seekbar
    #define ZOOM_SEEKBAR_FRAME_V_X      0
    #define ZOOM_SEEKBAR_FRAME_V_Y      0


    #define BTN_SHOW_MODE_V_LEFT     0
    #define BTN_SHOW_MODE_V_TOP      0

    #define BTN_HIDE_MODE_V_LEFT     0
    #define BTN_HIDE_MODE_V_TOP      (-MOVE_HEIGHT_V)

    //brightness icon   
    #define PIC_PROPERTY_BRIGHTNESS_V_X      (20 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_V_OFFSET)
    #define PIC_PROPERTY_BRIGHTNESS_V_Y      27

    #define DV_PIC_PROPERTY_BRIGHTNESS_V_X   (40 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_V_OFFSET)
    #define DV_PIC_PROPERTY_BRIGHTNESS_V_Y    PIC_PROPERTY_BRIGHTNESS_V_Y
    

    //brightness seekbar
    #define BRIGHTNESS_SEEKBAR_FRAME_V_X      (8 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_V_OFFSET)
    #define BRIGHTNESS_SEEKBAR_FRAME_V_Y      PIC_PROPERTY_SEEKBAR_Y_OFFSET
    
    #define DV_BRIGHTNESS_SEEKBAR_FRAME_V_X   (28 + BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_V_OFFSET)
    #define DV_BRIGHTNESS_SEEKBAR_FRAME_V_Y   BRIGHTNESS_SEEKBAR_FRAME_V_Y
 
#endif

/*
@ th macro are defined as order
*/

/*
    hor screen
*/

//zoom seekbar
#define ZOOM_SEEKBAR_FRAME_H_X      0
#define ZOOM_SEEKBAR_FRAME_H_Y      0


//button list bg
#define BTNLIST_BG_SHOW_MODE_H_LEFT     0
#define BTNLIST_BG_SHOW_MODE_H_TOP      0
#define BTNLIST_BG_SHOW_MODE_H_RIGHT    (BUTTONLIST_WIDTH - 1)
#define BTNLIST_BG_SHOW_MODE_H_BOTTEOM  (BUTTONLIST_HEIGHT - 1)

#define BTNLIST_BG_HIDE_MODE_H_LEFT     (BTNLIST_BG_SHOW_MODE_H_LEFT - MOVE_WIDTH)
#define BTNLIST_BG_HIDE_MODE_H_TOP      0
#define BTNLIST_BG_HIDE_MODE_H_RIGHT    (BTNLIST_BG_SHOW_MODE_H_RIGHT - MOVE_WIDTH)
#define BTNLIST_BG_HIDE_MODE_H_BOTTEOM  BTNLIST_BG_SHOW_MODE_H_BOTTEOM


//button list, mode
#define BTN_SHOW_MODE_H_LEFT     0
#define BTN_SHOW_MODE_H_TOP      0

#define BTN_HIDE_MODE_H_LEFT     (BTN_SHOW_MODE_H_LEFT - MOVE_WIDTH)
#define BTN_HIDE_MODE_H_TOP      0


//list area    
#define LIST_SHOW_RECT_H_LEFT     BUTTONLIST_WIDTH
#define LIST_SHOW_RECT_H_TOP      0
#define LIST_SHOW_RECT_H_RIGHT     (LIST_SHOW_RECT_H_LEFT + LIST_WIDTH - 1)
#define LIST_SHOW_RECT_H_BOTTOM    (LIST_HEIGHT - 1)

#define LIST_HIDE_RECT_H_LEFT    (LIST_SHOW_RECT_H_LEFT - MOVE_WIDTH)
#define LIST_HIDE_RECT_TOP       0
#define LIST_HIDE_RECT_RIGHT     (LIST_SHOW_RECT_H_RIGHT - MOVE_WIDTH)
#define LIST_HIDE_RECT_BOTTOM    LIST_SHOW_RECT_H_BOTTOM


//label
#define LABEL_SHOW_RECT_H_LEFT      LIST_SHOW_RECT_H_LEFT
#define LABEL_SHOW_RECT_H_TOP       0
#define LABEL_SHOW_RECT_H_RIGHT     LIST_SHOW_RECT_H_RIGHT
//#define LABEL_SHOW_RECT_H_BOTTOM    25

#define LABEL_HIDE_RECT_H_LEFT      (LABEL_SHOW_RECT_H_LEFT - MOVE_WIDTH)
#define LABEL_HIDE_RECT_H_TOP       LABEL_SHOW_RECT_H_TOP
#define LABEL_HIDE_RECT_H_RIGHT     (LABEL_SHOW_RECT_H_RIGHT - MOVE_WIDTH)
//#define LABEL_HIDE_RECT_H_BOTTOM    LABEL_SHOW_RECT_H_BOTTOM


//boundary
#define PIC_BOUNDARY_SHOW_RECT_H_LEFT     (LIST_SHOW_RECT_H_RIGHT + 1)
#define PIC_BOUNDARY_SHOW_RECT_H_TOP      0

#define PIC_BOUNDARY_HIDE_RECT_LEFT       (PIC_BOUNDARY_SHOW_RECT_H_LEFT - MOVE_WIDTH)
#define PIC_BOUNDARY_HIDE_RECT_TOP        0


//seekbar background bmp
#define PIC_PROPERTY_BG_H_X       (BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_H_OFFSET)
#define PIC_PROPERTY_BG_H_Y       PIC_PROPERTY_SEEKBAR_Y_OFFSET

#define HIDE_PIC_PROPERTY_BG_H_X  (PIC_PROPERTY_BG_H_X - MOVE_WIDTH)
#define HIDE_PIC_PROPERTY_BG_H_Y  PIC_PROPERTY_BG_H_Y

//brightness icon   
#define PIC_PROPERTY_ICON_H_SPACE     PIC_PROPERTY_BETWEEN_ICON_X_OFFSET

#define HIDE_PIC_PROPERTY_BRIGHTNESS_H_X      (PIC_PROPERTY_BRIGHTNESS_H_X)
#define HIDE_PIC_PROPERTY_BRIGHTNESS_H_Y      (PIC_PROPERTY_BRIGHTNESS_H_Y - MOVE_WIDTH)

#define DV_PIC_PROPERTY_ICON_H_SPACE     DV_PIC_PROPERTY_BETWEEN_ICON_X_OFFSET

#define DV_HIDE_PIC_PROPERTY_BRIGHTNESS_H_X   (DV_PIC_PROPERTY_BRIGHTNESS_H_X - MOVE_WIDTH)    
#define DV_HIDE_PIC_PROPERTY_BRIGHTNESS_H_Y    HIDE_PIC_PROPERTY_BRIGHTNESS_H_Y


//brightness seekbar
#define PIC_PROPERTY_SEEKBAR_H_SPACE      (PIC_PROPERTY_BETWEEN_SEEKBAR_X_OFFSET + SEEKBAR_FRAME_WIDTH)
#define DV_PIC_PROPERTY_SEEKBAR_H_SPACE   (DV_PIC_PROPERTY_BETWEEN_SEEKBAR_X_OFFSET + SEEKBAR_FRAME_WIDTH)


//contrast seekbar
#define CONTRAST_SEEKBAR_FRAME_H_X      (BRIGHTNESS_SEEKBAR_FRAME_H_X + PIC_PROPERTY_SEEKBAR_H_SPACE)
#define CONTRAST_SEEKBAR_FRAME_H_Y      BRIGHTNESS_SEEKBAR_FRAME_H_Y

#define DV_CONTRAST_SEEKBAR_FRAME_H_X    (DV_BRIGHTNESS_SEEKBAR_FRAME_H_X + DV_PIC_PROPERTY_SEEKBAR_H_SPACE)
#define DV_CONTRAST_SEEKBAR_FRAME_H_Y    BRIGHTNESS_SEEKBAR_FRAME_H_Y


//exposal compensate
#define EC_SEEKBAR_FRAME_H_X      (CONTRAST_SEEKBAR_FRAME_H_X + PIC_PROPERTY_SEEKBAR_H_SPACE)
#define EC_SEEKBAR_FRAME_H_Y      (BRIGHTNESS_SEEKBAR_FRAME_H_Y)


/*
    vertical screen 
*/

//zoom seekbar
#define ZOOM_SEEKBAR_FRAME_V_X      0
#define ZOOM_SEEKBAR_FRAME_V_Y      0


//button list, bg
#define BTNLIST_BG_SHOW_MODE_V_LEFT     0
#define BTNLIST_BG_SHOW_MODE_V_TOP      0
#define BTNLIST_BG_SHOW_MODE_V_RIGHT    (BUTTONLIST_WIDTH - 1)   
#define BTNLIST_BG_SHOW_MODE_V_BOTTEOM  (BUTTONLIST_HEIGHT - 1)

#define BTNLIST_BG_HIDE_MODE_V_LEFT     0
#define BTNLIST_BG_HIDE_MODE_V_TOP      (BTNLIST_BG_SHOW_MODE_V_TOP - MOVE_HEIGHT_V)
#define BTNLIST_BG_HIDE_MODE_V_RIGHT    BTNLIST_BG_SHOW_MODE_V_RIGHT
#define BTNLIST_BG_HIDE_MODE_V_BOTTEOM  (BTNLIST_BG_SHOW_MODE_V_BOTTEOM - MOVE_HEIGHT_V)



//button list, mode
#define BTN_SHOW_MODE_V_LEFT     0
#define BTN_SHOW_MODE_V_TOP      0

#define BTN_HIDE_MODE_V_LEFT     0
#define BTN_HIDE_MODE_V_TOP      (-MOVE_HEIGHT_V)


//list area
#define LIST_SHOW_RECT_V_LEFT      BUTTONLIST_WIDTH
#define LIST_SHOW_RECT_V_TOP         0
#define LIST_SHOW_RECT_V_RIGHT     (LIST_SHOW_RECT_V_LEFT + LIST_WIDTH_V - 1)
#define LIST_SHOW_RECT_V_BOTTOM      (LIST_SHOW_RECT_V_TOP + LIST_HEIGHT_V - 1)

#define LIST_HIDE_RECT_V_LEFT      LIST_SHOW_RECT_V_LEFT
#define LIST_HIDE_RECT_V_TOP       (LIST_SHOW_RECT_V_TOP - MOVE_HEIGHT_V)
#define LIST_HIDE_RECT_V_RIGHT     LIST_SHOW_RECT_V_RIGHT
#define LIST_HIDE_RECT_V_BOTTOM    (LIST_SHOW_RECT_V_BOTTOM - MOVE_HEIGHT_V)


//label
#define LABEL_SHOW_RECT_V_LEFT      LIST_SHOW_RECT_V_LEFT
#define LABEL_SHOW_RECT_V_TOP       0
#define LABEL_SHOW_RECT_V_RIGHT     LIST_SHOW_RECT_V_RIGHT
//#define LABEL_SHOW_RECT_V_BOTTOM    25

#define LABEL_HIDE_RECT_V_LEFT      LIST_SHOW_RECT_V_LEFT
#define LABEL_HIDE_RECT_V_TOP       (LABEL_SHOW_RECT_V_TOP - MOVE_HEIGHT_V)
#define LABEL_HIDE_RECT_V_RIGHT     LIST_SHOW_RECT_V_RIGHT
//#define LABEL_HIDE_RECT_V_BOTTOM    (LABEL_SHOW_RECT_V_BOTTOM - MOVE_HEIGHT_V)


//boundary
#define PIC_BOUNDARY_SHOW_RECT_V_LEFT      0
#define PIC_BOUNDARY_SHOW_RECT_V_TOP       (LIST_SHOW_RECT_V_BOTTOM + 1)

#define PIC_BOUNDARY_HIDE_RECT_V_LEFT      0
#define PIC_BOUNDARY_HIDE_RECT_V_TOP       (PIC_BOUNDARY_SHOW_RECT_V_TOP  - MOVE_HEIGHT_V)


//seekbar background bmp
#define PIC_PROPERTY_BG_V_X          (BUTTONLIST_WIDTH + PIC_PROPERTY_BUTTONLIST_V_OFFSET)
#define PIC_PROPERTY_BG_V_Y          PIC_PROPERTY_SEEKBAR_Y_OFFSET

#define HIDE_PIC_PROPERTY_BG_V_X     (PIC_PROPERTY_BG_V_X - MOVE_HEIGHT_V)
#define HIDE_PIC_PROPERTY_BG_V_Y     (PIC_PROPERTY_BG_V_Y - MOVE_HEIGHT_V)


//brightness icon   
#define PIC_PROPERTY_ICON_V_SPACE     PIC_PROPERTY_BETWEEN_ICON_X_OFFSET

#define HIDE_PIC_PROPERTY_BRIGHTNESS_V_X      (PIC_PROPERTY_BRIGHTNESS_V_X)
#define HIDE_PIC_PROPERTY_BRIGHTNESS_V_Y      (PIC_PROPERTY_BRIGHTNESS_V_Y - MOVE_HEIGHT_V)

#define DV_PIC_PROPERTY_ICON_V_SPACE     DV_PIC_PROPERTY_BETWEEN_ICON_X_OFFSET

#define DV_HIDE_PIC_PROPERTY_BRIGHTNESS_V_X   (DV_PIC_PROPERTY_BRIGHTNESS_V_X)
#define DV_HIDE_PIC_PROPERTY_BRIGHTNESS_V_Y    HIDE_PIC_PROPERTY_BRIGHTNESS_V_Y


//brightness seekbar
#define PIC_PROPERTY_SEEKBAR_V_SPACE        (PIC_PROPERTY_BETWEEN_SEEKBAR_X_OFFSET + SEEKBAR_FRAME_WIDTH)
#define DV_PIC_PROPERTY_SEEKBAR_V_SPACE     (DV_PIC_PROPERTY_BETWEEN_SEEKBAR_X_OFFSET + SEEKBAR_FRAME_WIDTH)

//contrast seekbar
#define CONTRAST_SEEKBAR_FRAME_V_X      (BRIGHTNESS_SEEKBAR_FRAME_V_X + PIC_PROPERTY_SEEKBAR_V_SPACE)
#define CONTRAST_SEEKBAR_FRAME_V_Y      BRIGHTNESS_SEEKBAR_FRAME_V_Y

#define DV_CONTRAST_SEEKBAR_FRAME_V_X   (DV_BRIGHTNESS_SEEKBAR_FRAME_V_X + DV_PIC_PROPERTY_SEEKBAR_V_SPACE)
#define DV_CONTRAST_SEEKBAR_FRAME_V_Y   DV_BRIGHTNESS_SEEKBAR_FRAME_V_Y


//exposal compensate
#define EC_SEEKBAR_FRAME_V_X      (CONTRAST_SEEKBAR_FRAME_V_X + PIC_PROPERTY_SEEKBAR_V_SPACE)
#define EC_SEEKBAR_FRAME_V_Y      BRIGHTNESS_SEEKBAR_FRAME_V_Y

/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef void (*PDA_ITEM_CALL_BACK)(void);

typedef enum
{
    PREVIEW_ANIM_STEP_IMG,
    PREVIEW_ANIM_STEP_ANIM,
    PREVIEW_ANIM_STEP_MAX
}MMIDC_PREVIEW_ANIM_STEP_E;

typedef enum
{
    MMIDC_PDA_CMD_CAPTURE,
    MMIDC_PDA_CMD_RECORD,
    MMIDC_PDA_CMD_PAUSE_RECORD,
    MMIDC_PDA_CMD_MAX
}MMIDC_PDA_CMD_E;

typedef enum
{
    PDA_ICON_MODE,
    PDA_ICON_IMAGE_ATTR,
    PDA_ICON_WHITE_BALANCE,        
    PDA_ICON_EFFECT,  
    PDA_ICON_SETTING,   
    //DC
    PDA_ICON_SETTING_DC_FLASH,
    PDA_ICON_SETTING_DC_QUALITY,    
    PDA_ICON_SETTING_DC_SIZE,    
    PDA_ICON_SETTING_DC_PIC_MODE,
    PDA_ICON_SETTING_DC_SELF_SHOT,    
    PDA_ICON_SETTING_DC_CORUSCATE,    
    PDA_ICON_SETTING_DC_STORAGE,
    //DV
    PDA_ICON_SETTING_DV_FLASH,
    PDA_ICON_SETTING_DV_SIZE,        
    PDA_ICON_SETTING_DV_CORUSCATE,    
    PDA_ICON_SETTING_DV_VIDEOFORMAT,
    PDA_ICON_SETTING_DV_STORAGE,    
    PDA_ICON_MAX
}MMIDC_PDA_ICON_E;

typedef enum
{
    PDA_RECT_ZOOM,
    PDA_RECT_EXPAND,
    PDA_RECT_IMAGE_PROPERTY,
    PDA_RECT_BTN_LIST_BG,
    PDA_RECT_LIST_SHADOW,   
    PDA_RECT_LABEL_RECT,
    PDA_RECT_LIST_RECT_BG,    
    PDA_RECT_PIC_PROPERTY_BRIGHTNESS_ICON,
    PDA_RECT_PIC_PROPERTY_CONTRAST_ICON,
    PDA_RECT_PIC_PROPERTY_EC_ICON,
    PDA_RECT_PIC_PROPERTY_DV_BRIGHTNESS_ICON,
    PDA_RECT_PIC_PROPERTY_DV_CONTRAST_ICON,
    PDA_RECT_PIC_PROPERTY_SEEKBAR_BG, //background picture as seekbar area
    PDA_RECT_PIC_PROPERTY_DV_SEEKBAR_BG, //background picture as seekbar area
    PDA_RECT_MAX
}MMIDC_PDA_ICON_RECT_E;

typedef enum
{
    IMAGE_ATTR_BRIGHTNESS,
    IMAGE_ATTR_CONTRAST,
    IMAGE_ATTR_EXPOSALCOMPENSATE,
    IMAGE_ATTR_MAX
}IMAGE_ATTR_E;

typedef enum
{
    STRING_TYPE_NONE, //normall
    STRING_TYPE_STRING1, //text array is string struct
    STRING_TYPE_STRING2, //text1 array is string struct
    STRING_TYPE_TEXT_SIZE, //text array is size string struct
    STRING_TYPE_MAX
}ITEM_STRING_TYPE_E;

typedef enum
{
    BUTTON_LIST_FOCUS_MODE,
    BUTTON_LIST_FOCUS_IMAGE_PROPERTY,
    BUTTON_LIST_FOCUS_WHITEBLANCE,
    BUTTON_LIST_FOCUS_EFFECT,
    BUTTON_LIST_FOCUS_SETTING,
    BUTTON_LIST_FOCUS_MAX,
}BUTTON_LIST_FOCUS_E;

typedef enum
{
    MENU_KEY_NONE,
    MENU_KEY_DC_EDIT_PIC,
    MENU_KEY_MAX
}PDA_MENU_KEY_STATE_E;

typedef enum
{
    MMIDC_ITEM_TYPE_CTRL, //ctrl
    MMIDC_ITEM_TYPE_SELF_DRAW, //self draw
    MMIDCITEM_TYPE_MAX
}MMIDC_ITEM_TYPE_E;

#ifdef MMIDC_F_LAST_IMAGE
typedef enum
{
    MMIDC_GET_LAST_DATA_PHOTO,
    MMIDC_GET_LAST_DATA_VIDEO,
    MMIDC_GET_LAST_DATA_TEMP_FILE,
    MMIDC_GET_LAST_DATA_BUFFER,
    MMIDC_GET_LAST_THUMBNAIL_DATA_MAX
}MMIDC_GET_LAST_THUMBNAIL_DATA_E;
#endif

typedef union
{
    MMIDC_PDA_ICON_RECT_E index;
    MMI_CTRL_ID_T ctrl_id;
}MMIDC_ITEM_DATA_U;

typedef struct
{
    MMIDC_ITEM_DATA_U item_data;
    MMIDC_ITEM_TYPE_E type;
}MMIDC_ITEM_CONTENT_T;

typedef struct
{
   uint16 max_item_num;
   uint16 cur_item;
   MMIDC_PDA_ICON_E icon_id;
   MMI_TEXT_ID_T title_text_id;
   MMI_TEXT_ID_T text_array[PDA_ITEM_MAX];
   MMI_TEXT_ID_T text1_array[PDA_ITEM_MAX];   
   MMI_IMAGE_ID_T image_array[PDA_ITEM_MAX];
   MMI_STRING_T string_array[PDA_ITEM_MAX];
   ITEM_STRING_TYPE_E string_type_array[PDA_ITEM_MAX];
   PDA_ITEM_CALL_BACK item_callback[PDA_ITEM_MAX];
   BOOLEAN gray_array[PDA_ITEM_MAX];
   uint32 user_data[PDA_ITEM_MAX];
}MMIDC_LIST_CTRL_ITEM_T;

typedef struct
{
    GUI_RECT_T rect;
    MMIDC_PDA_ICON_E icon;
    MMIDC_LIST_CTRL_ITEM_T list_item;
}MMIDC_PDA_ITEM_T;

typedef struct
{
    wchar full_path_name[MMIDC_FULL_PATH_LENGTH+1];    
    uint16 actual_width;
    uint16 actual_height;
    uint32 img_data_size;
}MMIDC_SAVE_THUMBNAIL_DATA_T;

typedef struct
{
    //GUIANIM_FILE_INFO_T file_info;            //文件指针
    wchar full_path_name[MMIDC_FULL_PATH_LENGTH+1];
    BOOLEAN is_img_data_valid;
    uint8  *img_data_buffer;
    uint32 img_data_size;
    //uint16 colorkey;
    int16 offset_x;
    int16 offset_y;
    uint16 img_width;
    uint16 img_height;
    uint16 actual_width;
    uint16 actual_height;
    GUI_RECT_T img_display_rect;                   //osd 图片的显示坐标
    //GUI_LCD_DEV_INFO lcd_dev;            //lcd dev
}MMIDC_IMG_INFO_T;

typedef struct
{
    BOOLEAN is_visible;
    int16 icon_disp_number;
    MMIDC_PDA_ITEM_T pda_item;
#if defined(MMIDC_F_DV_ZOOM)
    uint16 zoom_seekbar_width;
#endif
}MMIDC_DESKTOP_PDA_T;

typedef struct
{
    MMI_IMAGE_ID_T img_id;
    GUI_BOTH_RECT_T both_rect;
    GUI_RECT_T cur_rect;
    uint16 width;
    uint16 height;
}MMIDC_PICTURE_INFO_T;

typedef struct
{
    MMIDC_PICTURE_INFO_T frame;
    MMIDC_PICTURE_INFO_T bg;
    MMIDC_PICTURE_INFO_T mask;
    MMIDC_PICTURE_INFO_T scale;
    uint16 cur_value;
    uint16 total_value;
    uint16 cur_step;
    uint16 total_step;
    BOOLEAN is_hor_draw;
}MMIDC_SEEKBAR_INFO_T;

typedef struct
{
    uint16 timer_id;
    uint16 display_timer_id;
    MMIDC_SEEKBAR_INFO_T seekbar_info;
    GUI_POINT_T down_point;
    BOOLEAN is_tp_down;
    BOOLEAN is_display_seekbar;
}MMIDC_ZOOM_SEEKBAR_INFO_T;

typedef struct
{
    BOOLEAN is_press; //hit button
    BOOLEAN is_visible;
    MMIDC_PICTURE_INFO_T pic_info;
}MMIDC_ICON_INFO_T;

typedef struct
{
    MMIDC_ICON_INFO_T icon_info[PDA_RECT_MAX];
}MMIDC_PDA_ICON_RECT_T;

typedef struct
{
    uint16 timer_id;
    GUI_POINT_T down_point;
    BOOLEAN is_tp_down;
    uint16 total_num; //total attribute number
    IMAGE_ATTR_E cur_attr;
    MMIDC_SEEKBAR_INFO_T attr[IMAGE_ATTR_MAX];    
}MMIDC_IMAGE_ATTR_INFO_T;

typedef struct
{
    uint16 timer_id;
    MMI_CTRL_ID_T ctrl_id;
    MMI_IMAGE_ID_T start_img_id;
    uint16 total_img_number;
    GUI_BOTH_RECT_T img_rect;
}MMIDC_OSD_ANIM_T;

typedef struct
{
    MMI_MESSAGE_ID_E msg_id;
    DC_PDA_KEY_E key;
}MMIDC_PDA_MSG_MAP_T;

typedef struct
{
    BOOLEAN is_glide;    
    BOOLEAN is_contract;
    GUI_POINT_T down_point;    
    GUI_POINT_T up_point;
}MMIDC_PDA_EXPAND_INFO_T;



/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//Description : create desktop pda
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN  MMIDC_CreateDesktopPDA(void);

/*****************************************************************************/
//Description : destroy desktop pda
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_DestroyDesktopPDA(void);

/*****************************************************************************/
//Description : process msg
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DesktopProcessMsg(MMI_MESSAGE_ID_E msg_id, //in
                                                    void *param //in
                                                    );

 /*****************************************************************************/
//Description :  start preview anim
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_StartPreviewAnim(MMIDC_PREVIEW_ANIM_STEP_E step 
                                           );

/*****************************************************************************/
//Description : process list ctrl 
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDCProcessListCtrl(MMI_HANDLE_T ctrl_id
                             );
                             
/*****************************************************************************/
//Description :  display pda desktop
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_DisplayPDADesktop(void);

/*****************************************************************************/
//Description :  display button cmd image
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayAllCmdBtns(DC_FLOW_MODE_E mode);

/*****************************************************************************/
//Description :  restore to pda desktop
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_RestorePDADesktop(void);

/*****************************************************************************/
//Description :  close all pda timer
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseAllPDATimer(void);

/*****************************************************************************/
//Description :  reset all pda ctrls dev info
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ResetAllPDACtrlsDevInfo(void);

/*****************************************************************************/
//Description :  pda for lose focus
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_PDAForLoseFocus(void);

#if defined(MMIDC_F_DV_ZOOM)
/*****************************************************************************/
//Description :  get zoom seekbar width
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetZoomSeekBarWidth(void);

/*****************************************************************************/
//Description :  set zoom seekbar width
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetZoomSeekBarWidth(uint16 width);
#endif //#if defined(MMIDC_F_DV_ZOOM)

#if defined (MMIDC_F_U_CAMERA)
/*****************************************************************************/
//Description :  get button ucamera both rect
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_ResetListItem(void);
#endif

/*****************************************************************************/
//Description :  update zoom seekbar
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_UpdateZoomSeekBar(void);

/*****************************************************************************/
//Description : init pda key status parameter
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_InitPDAKeyStatusParameter(void);

#ifdef MMIDC_F_LAST_IMAGE
/*****************************************************************************/
//Description : get last thumbnail info ptr
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC MMIDC_IMG_INFO_T* MMIDC_GetLastThumbnailInfoPtr(void);

/*****************************************************************************/
//Description :  init last thumbnail info
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_CreateLastThumbnailInfo(void);

/*****************************************************************************/
//Description :  destroy last thumbnail info data
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_DestroyLastThumbnailInfoData(void);

/*****************************************************************************/
//Description : get last thumbnail info ptr
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC MMIDC_IMG_INFO_T* MMIDC_GetPreviewImgInfoPtr(void);

/*****************************************************************************/
//Description :  get last thumbnail data 
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC MMIDC_GET_LAST_THUMBNAIL_DATA_E MMIDC_GetLastThumbnailData(CAMERA_MODE_E camera_mode);

/*****************************************************************************/
//Description :  set last thumbnail data valid flag
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetLastThumbnailDataValidFlag(BOOLEAN flag);

/*****************************************************************************/
//Description :  save last thumbnail data to file
//Global resource dependence : none
//Author: chunyou
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_SaveLastThumbnailDataToFile(void);
#endif

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIDC_GUIPDA_H_

