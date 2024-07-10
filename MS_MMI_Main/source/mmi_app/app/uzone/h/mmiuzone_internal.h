/*****************************************************************************
** File Name:      mmiuzone_internal.h                                       *
** Author:                                                                   *
** Date:           21/12/2011                                                *
** Copyright:      2003 Spreadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to uzone internal                       *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 12/2011       Jessica             Create
******************************************************************************/

#ifndef _MMIUZONE_INTERNAL_H_
#define _MMIUZONE_INTERNAL_H_ 

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmifilearray_export.h"

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

/*Please make sure the  number of UZONE item >=5*/
/*assert verified*/
#ifdef MMI_AUDIO_PLAYER_SUPPORT 
#define MMIUZONE_F_MP3 
#endif

#ifdef FM_SUPPORT
#define MMIUZONE_F_FM 
#endif
#ifdef PIC_VIEWER_SUPPORT 
#define MMIUZONE_F_IMAGEVIEWER
#endif

#ifdef VIDEO_PLAYER_SUPPORT
#ifndef MMI_VIDEOPLAYER_MINI_FUNCTION
#define MMIUZONE_F_VIDEOPLAYER 
#endif
#endif

#ifdef ATV_SUPPORT
#define MMIUZONE_F_ATV
#endif
#ifdef  CAMERA_SUPPORT
#define MMIUZONE_F_CAMERA
#endif
#define MMIUZONE_F_RECORD 
#ifdef MMI_KING_MOVIE_SUPPORT
#define MMIUZONE_F_KINGMOVIE
#endif
#ifdef EBOOK_SUPPORT
#define MMIUZONE_F_EBOOK 
#endif

#define UZONE_STAR_NUMBER 5



/**--------------------------------------------------------------------------*
 **                         TYPE AND CONSTANT                                *
 **--------------------------------------------------------------------------*/
typedef void (*UZONE_ITEM_CALLBACK)(void);

typedef enum
{
#ifdef MMIUZONE_F_MP3    
    MMIUZONE_ITEM_ID_MP3, 
#endif
#ifdef MMIUZONE_F_FM
    MMIUZONE_ITEM_ID_FM, 
#endif
#ifdef MMIUZONE_F_IMAGEVIEWER
    MMIUZONE_ITEM_ID_IMAGEVIEWER, 
#endif
#ifdef MMIUZONE_F_VIDEOPLAYER
    MMIUZONE_ITEM_ID_VIDEOPLAYER, 
#endif
#ifdef MMIUZONE_F_ATV
    MMIUZONE_ITEM_ID_ATV, 
#endif
#ifdef MMIUZONE_F_CAMERA
    MMIUZONE_ITEM_ID_CAMERA, 
#endif
#ifdef MMIUZONE_F_RECORD
    MMIUZONE_ITEM_ID_RECORD, 
#endif
#ifdef MMIUZONE_F_KINGMOVIE
    MMIUZONE_ITEM_ID_KINGMOVIE, 
#endif
#ifdef MMIUZONE_F_EBOOK
    MMIUZONE_ITEM_ID_EBOOK, 
#endif    
    MMIUZONE_ITEM_ID_MAX,
}MMIUZONE_ITEM_ID_E;

typedef struct
{
    uint16                       cur_item_index;  //current item
}MMIUZONE_NV_INFO_T;


typedef struct
{
    int16 width; //width
    int16 height; //icon height
    int16 shadow_height; //shadow height 
}MMIUZONE_ITEM_SIZE_T;

typedef struct
{
    int16 width; //width
    int16 height; //icon height
}MMIUZONE_STAR_SIZE_T;


typedef struct
{
    MMI_TEXT_ID_T text_id; //the text id for this item.
    MMI_IMAGE_ID_T icon_id; //the icon id for this item.
    MMI_IMAGE_ID_T bg_id;   //the bg image id for this item. 
    UZONE_ITEM_CALLBACK callback; //the callback function
}MMIUZONE_ITEM_RES_T;


typedef struct
{
    MMIUZONE_ITEM_ID_E item_id;  //uzone item id
    MMIUZONE_ITEM_RES_T item_res; //the item resource
    GUI_RECT_T bg_rect; //the bg rect
    GUI_RECT_T icon_rect; //the icon rect
    GUI_RECT_T str_rect; //the str rect
}MMIUZONE_ITEM_INFO_T;

typedef struct
{
    MMIUZONE_ITEM_INFO_T item_info; //the image info of this item
}MMIUZONE_ITEM_T;

typedef struct
{
    MMI_IMAGE_ID_T bg_id; //bg image id
    GUI_BOTH_RECT_T bg_both_rect; //both_rect of bg 
}MMIUZONE_BG_INFO_T;

typedef struct
{
    MMIUZONE_ITEM_T* item_ptr; //the uzone item array.
    uint16 item_num; //uzone item number

    MMIUZONE_ITEM_SIZE_T item_size; //the size of item
    MMIUZONE_STAR_SIZE_T big_star_size; //the big star size
    MMIUZONE_STAR_SIZE_T small_star_size; //the small star size

    GUI_LCD_DEV_INFO page_srclayer_dev_info; //the source layer
    GUI_LCD_DEV_INFO icon_srclayer_dev_info; //the source layer
    GUI_LCD_DEV_INFO str_srclayer_dev_info; //the source layer
    GUI_LCD_DEV_INFO str_shadow_dev_info; //the source layer
    GUI_LCD_DEV_INFO bglayer_dev_info; //the bg layer
    GUI_LCD_DEV_INFO fglayer_dev_info; //the fg layer pool

    GUI_LCD_DEV_INFO bigstarlayer_dev_info; //the big star layer pool
    GUI_LCD_DEV_INFO smallstarlayer_dev_info; //the small star layer pool
    GUI_LCD_DEV_INFO focusitemlayer_dev_info; //the focus item layer

    MMIUZONE_BG_INFO_T bg_info;//the bg info
    BOOLEAN is_landscape; //Is window landscape?
    MMI_WIN_ID_T main_win_id; //handle of main win
    MMI_HANDLE_T applet_handle; //applet handle
}MMIUZONE_RUNNING_INFO;

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : register uzone module applet info
//  Global resource dependence : 
//  Author:
//  Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_RegAppletInfo(void);

/*****************************************************************************/
//  Description : Entry function of uzone
//  Global resource dependence :
//  Author: Jessica
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIUZONE_Entry(void);


/*****************************************************************************/
//  Description : set nv info
//  Global resource dependence :
//  Author: Jessica
//  Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_SetNVInfo(MMIUZONE_NV_INFO_T nv_info);

/*****************************************************************************/
//  Description : get current item index
//  Global resource dependence :
//  Author: Jessica
//  Note:
/*****************************************************************************/
PUBLIC uint16 MMIUZONE_GetCurItemIndex(void);

/*****************************************************************************/
//Description : exit uzone
//Global resource dependence :
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIUZONE_Exit(void);

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif //_MMIUZONE_INTERNAL_H_
