/*****************************************************************************
** File Name:      mmipic_edit.h                                                  *
** Author:                                                                   *
** Date:           01/10/2008                                                *
** Copyright:      2008 preadtrum, Incoporated. All Rights Reserved.        *
** Description:    This file is used to mmipic_edit.c  mmipic_edit_wintab.c                 *
*****************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                               *
** 01/2008       Vincent.shang           Create
******************************************************************************/
//macro replace
//#ifdef MMI_PIC_EDITOR_ENABLE
#ifdef PIC_EDITOR_SUPPORT
#ifndef _MMIPIC_H_
#define _MMIPIC_H_ 

/**--------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
#include "mmifmm_export.h"
#include "pe_process.h"
#include "guiimg.h"
#include "sysTypes.h"
#include "block_mem.h"
#include "guifont.h"
#include "guianim.h"
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

//#ifndef MMIPIC_ASSERT_MODE
//#define MMIPIC_ASSERT_MODE
//#endif

#ifdef MMIPIC_ASSERT_MODE
    #define MMIPIC_ASSERT_WARNING    MMI_ASSERT_WARNING
#else
    #define MMIPIC_ASSERT_WARNING    SCI_TRACE_LOW
#endif

#if defined(MAINLCD_SIZE_240X320) || defined(MAINLCD_SIZE_320X480) || defined(MAINLCD_SIZE_240X400)
    #define MMIPIC_SUPPORT_PNG
#endif
    
//#define MMIPIC_EDIT_BUFFER_SIZE  (MMI_MAINSCREEN_WIDTH*MMI_MAINSCREEN_HEIGHT*2)
//process buffer size changes from 240*320*2 to 240*320*3 to 240*320*4
//  width*height*4  to width*height*3+72K
//to support 480X320,so 72K to 100K
#define MMIPIC_PROCESS_BUFFER_FIXED_SIZE   (100*1024)
//#define MMIPIC_PROCESS_BUFFER_SIZE  (MMI_MAINSCREEN_WIDTH*MMI_MAINSCREEN_HEIGHT*3+MMIPIC_PROCESS_BUFFER_FIXED_SIZE)
#define MMIPIC_SAVE_SUBFIX            (g_mmipic_save_subfix_wstr)//"\x00\x2e\x00\x6a\x00\x70\x00\x67"      // pic文件后缀名.jpg
#define MMIPIC_SAVE_SUBFIX_LEN        4
    
#define MMIPIC_SUFFIX_MAX_LEN            12              //max file suffix len (bytes)
    
#define MMIPIC_MINIATURE_PATH              (g_mmipic_miniature_path_wstr)//"\x00\x5f\x00\x30\x00\x31"  //_01
#define MMIPIC_MINIATURE_PATH_LEN          3                     //缩略图路径长度_01
#define MMIPIC_MINIATURE_FULL_PATH_LEN     GUIANIM_MINIATURE_FULL_PATH_LEN //注意结束符号
    
#define MMIPIC_ELLIPSIS_3                   MMIFMM_ELLIPSIS_3//"\x00\x2e\x00\x2e\x00\x2e"    //"..."
#define MMIPIC_ELLIPSIS_3_LEN               MMIFMM_ELLIPSIS_3_LEN

#define MMIPIC_DOT                         MMIFILE_DOT


#define MMIPIC_GRADE_MAX    8
#define MMIPIC_GRADE_MIN    0


#define MMIPIC_CHAR_TEXT_MAX_LEN            50  // 图片编辑输入文字的最大长度

#define MMIPIC_PROGRESSBAR_SPACE            10

#define MMIPIC_FONT_TYPE    MMI_DEFAULT_SMALL_FONT

//根据不同的屏幕大小，来调整显示的坐标  
#if defined (MAINLCD_SIZE_176X220) || defined (MAINLCD_SIZE_128X160)        
    //RGB 
    #define MMIPIC_RGB_MOVING_VALUE  8
    
    //bright constract saturtion hue    
    #define MMIPIC_BRIGHT_MOVING_VALUE   12
#elif defined(MAINLCD_SIZE_240X320) || defined(MAINLCD_SIZE_320X480) || defined(MAINLCD_SIZE_240X400)
    //default values    
   //RGB 
    #define MMIPIC_RGB_MOVING_VALUE  10    
    //bright constract saturtion hue
    #define MMIPIC_BRIGHT_MOVING_VALUE   10    
#else  
    //default values    
   //RGB 
    #define MMIPIC_RGB_MOVING_VALUE  11
    
    //bright constract saturtion hue
    #define MMIPIC_BRIGHT_MOVING_VALUE   16
#endif


#define MMIPIC_ARROW_SPACE 20
#define MMIPIC_ARROW_TOP_SPACE 2

#if defined MAINLCD_SIZE_320X480
#ifdef PIC_EDIT_U200_SUPPORT
#define MMIPICEDIT_BUTTON_MARGIN_WIDTH      10
#define MMIPICEDIT_TITLE_HEIGHT             50
#endif
#define MMIPICEDIT_FORM_HEIGHT              60
#define MMIPICEDIT_ICONLIST_HEIGHT          75
#define MMIPICEDIT_PROPANEL_TOPSPACE        10
#define MMIPICEDIT_ICON_MARGIN_V            6
#define MMIPICEDIT_ICON_MARGIN_H            16
#define MMIPICEDIT_TITLE_FONT               SONG_FONT_26

#else
#ifdef PIC_EDIT_U200_SUPPORT
#define MMIPICEDIT_BUTTON_MARGIN_WIDTH      8
#define MMIPICEDIT_TITLE_HEIGHT             40
#endif
#define MMIPICEDIT_FORM_HEIGHT              46
#define MMIPICEDIT_ICONLIST_HEIGHT          60
#define MMIPICEDIT_PROPANEL_TOPSPACE        8
#define MMIPICEDIT_ICON_MARGIN_V            4
#define MMIPICEDIT_ICON_MARGIN_H            11
#define MMIPICEDIT_TITLE_FONT               MMI_DEFAULT_BIG_FONT

#endif
#ifdef PIC_EDIT_FRAME_SUPPORT
#define PIC_EDIT_MAIN_MENU_NUM              6
#else
#define PIC_EDIT_MAIN_MENU_NUM              5
#endif

//char font
#define MMI_PIC_CHAR_LARGE_FONT				SONG_FONT_26
#define MMI_PIC_CHAR_LARGE_FONT_SIZE		26
#define MMI_PIC_CHAR_NORMAL_FONT			SONG_FONT_20
#define MMI_PIC_CHAR_NORMAL_FONT_SIZE		20
#define MMI_PIC_CHAR_SMALL_FONT				SONG_FONT_14
#define MMI_PIC_CHAR_SMALL_FONT_SIZE		14

typedef  enum 
{
    MMIPIC_BRI_TYPE,//stopwatch
    MMIPIC_COLOR_TYPE, //countdown
	MMIPIC_TYPE_MAX
} MMIPIC_ADJUST_TYPE_E;


/**--------------------------------------------------------------------------*
**                         TYPE AND CONSTANT                                *
**--------------------------------------------------------------------------*/

typedef GUIANIM_FILE_INFO_T PIC_EDIT_FILE_INFO_T;

#define GROUP_MAX   4

typedef enum
{
    FULL_RECT_INDEX = 0,//progress item rect
    GRADE_RECT_INDEX,   //progress line display rect
    SLIDE_RECT_INDEX,   //progress glide rect
    RECT_INDEX_MAX
}RECT_INDEX_E;

typedef struct
{
    GUI_RECT_T item[RECT_INDEX_MAX];
}ITEM_RECT_T;

typedef struct
{
    ITEM_RECT_T group[GROUP_MAX];
}MMIPIC_PROGRESSBAR_RECT_T;

typedef enum
{
    IMG_ITEM_TYPE = 0,
    COL_ITEM_TYPE,
    ITEM_TYPE_MAX
}MMIPIC_PROITEM_TYPE_E;

typedef union
{
    MMI_IMAGE_ID_T      img_id;
    uint16              col_num;
}MMIPIC_PROGRESSBAR_ITEM_U;

typedef struct
{
    uint8 grade; // progress bar level 
    uint8 total_level; //total level
    MMIPIC_PROITEM_TYPE_E   item_type;
    MMIPIC_PROGRESSBAR_ITEM_U item[2];//0:left text, 1:right text
}MMPIC_PROGRESSBAR_ITEM_T;

typedef struct
{
    uint8 total;
    uint8 vertical_space; //vertical space of group
    uint8 focus_item; //active item
    uint8 unit_width;   //unit width for drawing
    MMPIC_PROGRESSBAR_ITEM_T group[GROUP_MAX];
    MMI_IMAGE_ID_T img_bg_id; //background image id
    MMI_WIN_ID_T win_id; //window id 
}MMIPIC_PROGRESSBAR_T;


//file info
typedef struct 
{
    GUIIMG_BITMAP_T *bitmap_ptr;//osd bitmap
    GUI_RECT_T      display_rect;                 //display rect
    
} PIC_EDIT_OSD_INFO_T;

//file info
typedef struct 
{
    GUIIMG_BITMAP_T *icon_bitmap_ptr;
    GUI_RECT_T      display_rect;
} PIC_EDIT_ROTATE_ICON_INFO_T;

typedef enum
{
    MMI_ROTATION_DEGREE_90 = 0,
        MMI_ROTATION_DEGREE_180,
        MMI_ROTATION_DEGREE_270 ,
        MMI_ROTATION_DEGREE_MIRROR ,
        MMI_ROTATION_DEGREE_MAX
} MMI_ROTATION_E;





typedef enum
{
    MMI_PIC_FILE_GET_ERR_NONE=0,
    MMI_PIC_FILE_OVERSIZE,
    MMI_PIC_FILE_ASY_READ_ERR,
    MMI_PIC_FILE_GET_ERROR_MAX
} MMI_PIC_FILE_GET_ERR_E;

typedef enum
{
    MMI_PIC_EFFECT_GRAY=0,
        MMI_PIC_EFFECT_SEPIA,
        MMI_PIC_EFFECT_SHARPEN,
        MMI_PIC_EFFECT_SMOOTH,
        MMI_PIC_EFFECT_DIFFUSION,
        MMI_PIC_EFFECT_EMBOSS,
        MMI_PIC_EFFECT_EXPOSURE,
        MMI_PIC_EFFECT_NEGATIVE_RGB,
        MMI_PIC_EFFECT_DENOISE,
        MMI_PIC_EFFECT_MAX
}MMI_PIC_EFFECT_E;

typedef enum
{
    MMI_PIC_ADJUSTMENT_BRIGHTNESS=0,
        MMI_PIC_ADJUSTMENT_CONSTRACT,
        MMI_PIC_ADJUSTMENT_SATURATION,
        MMI_PIC_ADJUSTMENT_HUE,
        MMI_PIC_ADJUSTMENT_MAX
}MMI_PIC_ADJUSTMENT_E;
/*
typedef enum
{
MMI_PIC_ADJUSTMENT_SATURATION=0,
MMI_PIC_ADJUSTMENT_HUE,
MMI_PIC_ADJUSTMENT_SAT_HUE_MAX
}MMI_PIC_ADJUST_SAT_HUE_E;*/

//added by robert.wang for cr145791
//目前平台中只支持2种字体
typedef enum
{
    MMI_PIC_CHAR_FONT_LARGE = 0,                   //大
    MMI_PIC_CHAR_FONT_NORMAL,                      //中
        MMI_PIC_CHAR_FONT_LITTLE,                           // 小
        MMI_PIC_CHAR_FONT_MAX
} MMI_PIC_CHAR_FONT_E;
//end cr145791

typedef enum
{
    MMI_PIC_RESIZE_LARGE = 0,                   //大
        MMI_PIC_RESIZE_MIDDLE,                                  //中
        MMI_PIC_RESIZE_LITTLE,                           // 小
        MMI_PIC_RESIZE_MAX
} MMI_PIC_RESIZE_E;

typedef enum
{
    MMI_PIC_DIRECTION_HORIZONTAL= 0,            //横向
        MMI_PIC_DIRECTION_VERTICAL,                            //竖向
        MMI_PIC_DIRECTION_MAX
} MMI_PIC_DIRECTION_E;

typedef enum
{
    MMI_PIC_EDIT_SAVE=0,
        MMI_PIC_EDIT_SAVEAS,
        MMI_PIC_EDIT_UNDO,
        MMI_PIC_EDIT_ZOOM,
        MMI_PIC_EDIT_ROTATE,
        MMI_PIC_EDIT_PASTE,
        MMI_PIC_EDIT_CHARACTER,
        MMI_PIC_EDIT_FRAME,
        MMI_PIC_EDIT_RESIZE,
        MMI_PIC_EDIT_RGB,
        MMI_PIC_EDIT_ADJUSTMENT,
        MMI_PIC_EDIT_EFFECT,
        MMI_PIC_EDIT_FUNC_MAX
} MMI_FUNC_TYPE_E;

typedef enum
{
    MMIPIC_CHAR_NAME=0,
        MMIPIC_CHAR_SIZE,
        MMIPIC_CHAR_WORD_COLOR,
        MMIPIC_CHAR_FRAME_COLOR,
        MMIPIC_CHAR_DIRECTION,
        MMIPIC_CHAR_MAX,
}MMIPIC_CHAR_ITEM_E;

typedef struct 
{
    uint8 *osd_ptr;                     //decompressed data
    uint8  *osd_data_buffer;       //osd data buffer
    uint16 colorkey;
    int16 offset_x;
    int16 offset_y;
    uint16 width;
    uint16 height;
    
}MMIPIC_OSD_PARAM_T;


typedef struct
{
    uint8                           *undo_buf_ptr;              //undo_buf指针
    uint8                             *process_buf_ptr;                      //中间 处理指针
    uint8                           *output_buf_ptr;                //编辑完的输出buf指针
    BOOLEAN                         is_update;                      //当前图片是否有改动，决定撤销是否灰显
    PIC_EDIT_FILE_INFO_T      file_info;            //文件指针
    GUI_RECT_T                     display_rect;                    //display rect
    uint8                              *file_data_buffer;       //file data buffer
    uint32                              file_data_size;          //file data size
    uint16                              undo_pic_width;
    uint16                              undo_pic_heigh;
    int16                               undo_pic_x_start;
    int16                               undo_pic_y_start;
    GUIIMG_BITMAP_T                  *display_bitmap_ptr;              //当前显示buf指针
    MMIPIC_OSD_PARAM_T       osd_image;             //osd图片    
    GUI_RECT_T                      osd_display_rect;                   //osd 图片的显示坐标
    GUI_LCD_DEV_INFO            lcd_dev;            //lcd dev
    
    //added, @robert.wang, 09-10-26, cr158180
    uint16                              output_pic_width; 
    uint16                              output_pic_height; 
    GUI_RECT_T                      output_display_rect;
    // end cr158180
} PIC_EDIT_PARAM_T;

//add character info
typedef struct
{
    //BOOLEAN                 is_ucs2;            // type of content
    uint8                   length;             // of content
    wchar                   content[MMIPIC_CHAR_TEXT_MAX_LEN+1];
    MMI_PIC_CHAR_FONT_E    size_type;
    MMI_PIC_DIRECTION_E direction_type;
    uint32                          color_index;
    uint32                          frame_color_index;
    uint16  *                      char_font_ptr;
    uint32                          char_font_heigh;
    uint16                          char_num;
    GUI_RECT_T                 box;
    uint16                          space_two_chars;
    uint16                          space_two_lines;
    GUI_FONT_T                  font;
    uint16                          char_string_width;
    uint16                          char_string_heigh;
} PIC_CHAR_PARAM_T;

//unify with GUIANIM_MINIATURE_FULL_PATH_T
//miniature full path info
// typedef struct 
// {
//     wchar       full_path_wstr[MMIPIC_MINIATURE_FULL_PATH_LEN + 1]; //full path
//     uint16      full_path_len;                              //full path length
// } PIC_MINIATURE_FULL_PATH_T;

typedef GUIANIM_MINIATURE_FULL_PATH_T PIC_MINIATURE_FULL_PATH_T;


//added, @robert.wang, 09-10-26, cr158188
typedef enum
{
    MMI_PIC_SUCCESS = 0,          //成功
    MMI_PIC_ERR_NO_SPACE ,       // 空间不足
    MMI_PIC_ERR_EXIT,            // 其他需要退出图片编辑器的错误
    MMI_PIC_ERR_INVALID_NAME,    //无效文件名
    MMI_PIC_ERR_LONG_FILENAME,   //文件名超过长度
    MMI_PIC_ERR_QUERY_OVERWRITE  //询问是否覆盖文件
}MMI_PIC_ERROR_TYPE_E;
//end cr158188


extern const wchar g_mmipic_save_subfix_wstr[];
extern const wchar g_mmipic_miniature_path_wstr[];
/**--------------------------------------------------------------------------*
**                         FUNCTION DEFINITION                              *
**--------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : open pic editor
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIC_OpenPicEditor(
                                    const wchar     *full_path_ptr,     //in
                                    uint16         full_path_len      //in:unicode个数
                                    );
/*****************************************************************************/
//  Description : Decode jpg image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIC_DecodeJpgImage(PIC_EDIT_FILE_INFO_T    *file_info_ptr );
/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_InitModule(void);
/*****************************************************************************/
//  Description : open pic editor main win
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIC_OpenPicEditorMainWin(   const wchar          *full_path_ptr,     //in
                                              uint16         full_path_len      //in:字节数
                                              );

/*****************************************************************************/
//  Description : get small(<1M) file data buffer
//  Global resource dependence : 
//  Author:jassmine.meng
//  Note: 
/*****************************************************************************/
//cr135943
PUBLIC MMI_PIC_FILE_GET_ERR_E MMIPIC_GetCommonFileDataBuf(
                                           PIC_EDIT_FILE_INFO_T    *file_info_ptr  //in
                                           );

/*****************************************************************************/
//  Description : pic editor init
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_PicEditorInit (PIC_EDIT_FILE_INFO_T    *file_info_ptr);

/*****************************************************************************/
//  Description : alloc undo buffer
//  Global resource dependence : 
//  Author: jassmine
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIPIC_AllocUndoBuf(void);

/*****************************************************************************/
//  Description : exit pic editor
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_PicEditorExit(void);

/*****************************************************************************/
//  Description :Malloc  PROCESS TARGET Mem
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIPIC_MallocMemForPicProcess(void);
/*****************************************************************************/
//  Description : display image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_DisplayImage(void);

/*****************************************************************************/
//  Description : display output image
//  Global resource dependence : 
//  Author: robert.wang
//  Date: 09-10-26
//  Note: 
//      显示处理后的图片
/*****************************************************************************/
PUBLIC void MMIPIC_DisplayOutputImage(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_TouchPenMoveMainImage(GUI_POINT_T point,MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
//  Description : save image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC MMI_PIC_ERROR_TYPE_E  MMIPIC_Save(BOOLEAN is_overwrite);

/*****************************************************************************/
//  Description : 
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIPIC_GetFileInfo(PIC_EDIT_FILE_INFO_T * param_to_save_as_win);

/*****************************************************************************/
//  Description : save as image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC MMI_PIC_ERROR_TYPE_E  MMIPIC_SaveAs(
                           const wchar *full_path_ptr,     //in
                           uint16     full_path_len       //in, 双字节为单位
                           );

/*****************************************************************************/
//  Description : undo editor operation
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIPIC_Undo(void);

/*****************************************************************************/
//  Description : undo width,heigh,x,y
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_UndoFileInfo(void);

/*****************************************************************************/
//  Description : copy output image data to original image data
//  Global resource dependence : 
//  Author: robert.wang
//  Date: 09-10-26
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_CopyOutputImgDataToOrgImgData(void);

/*****************************************************************************/
//  Description : save to undo buf,save width and height,save x,y
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_SaveToUndoBuf(void);

/*****************************************************************************/
//  Description : update file name
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIPIC_UpdateFileName(MMI_WIN_ID_T     win_id);

/*****************************************************************************/
//  Description : add rotated image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPIC_AppendRotatePreviewImageData(void);

/*****************************************************************************/
//  Description : handle  tp down msg of rotated preview window
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
//PUBLIC BOOLEAN   MMIPIC_HandleRotatedPreviewTpDown(GUI_POINT_T point);
PUBLIC BOOLEAN   MMIPIC_HandleRotatedPreviewTpDown(GUI_POINT_T point, BOOLEAN *is_in_rect );
/*****************************************************************************/
//  Description : display osd image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIPIC_FreeOsdBuf(void);

/*****************************************************************************/
//  Description : free rotate preview image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void   MMIPIC_FreeRotatedPreviewImage(void);
/*****************************************************************************/
//  Description : display all rotated preview image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIPIC_DisplayAllRotatedPreviewImage(uint16 cur_index);

/*****************************************************************************/
//  Description : display selected flag
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPIC_DisplayCurSelectedFlag(GUI_RECT_T display_rect);
/*****************************************************************************/
//  Description : get rotated image data
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIC_GetRotatedImageData(uint16 cur_image_index);

/*****************************************************************************/
//  Description : get current paste image data
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIPIC_GetCurrentPasteImageData(uint32 img_id,MMI_WIN_ID_T win_id);

/*****************************************************************************/
//  Description : display osd image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/

PUBLIC void MMIPIC_DisplayOSDImage(void);

/*****************************************************************************/
//  Description : merge image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
//PUBLIC void MMIPIC_MergeImage(void);
PUBLIC BOOLEAN MMIPIC_MergeImage(void);

/*****************************************************************************/
//  Description : draw text on image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIPIC_DrawText(
                                  GUI_RECT_T * box,         //the fixed display area
                                  wchar * text_ptr,         //string to display
                                  uint16 len,               //length of string to display
                                  uint16 space,             //space between two chars.
                                  GUI_COLOR_T color,        //string color
                                  GUI_FONT_T  font,     //font pointer  
//                                  BOOLEAN is_ucs2,          //is ucs2 code or ascii code. 1:ucs2 0:ascii code.  
                                  uint16 line_space ,      //space between 2 lines       
                                  GUI_COLOR_T edge_color,//font edge color/font frame color)
                                  MMI_PIC_DIRECTION_E direction_type
                                  );

/*****************************************************************************/
//  Description : move text on image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_MoveText(
                            GUI_RECT_T * box_ptr,       //the fixed display area
                            wchar * text_ptr,       //string to display
                            uint16 len,                 //length of string to display
                            uint16 space,           //space between two chars.
                            GUI_COLOR_T color,      //string color
                            GUI_FONT_T  font,       //font pointer  
//                            BOOLEAN is_ucs2,            //is ucs2 code or ascii code. 1:ucs2 0:ascii code.  
                            uint16 line_space ,      //space between 2 lines     
                            GUI_COLOR_T edge_color,//font edge color/font frame color)
                            MMI_PIC_DIRECTION_E direction_type,
                            MMI_MESSAGE_ID_E    msg_id,
                            uint16   char_string_width,
                            uint16   char_string_heigh
                            );

/*****************************************************************************/
//  Description : merge character with image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_MergeCharacter(uint16 * char_font_ptr, uint32  char_font_heigh,GUI_COLOR_T font_color,GUI_COLOR_T frame_color,uint16 char_num, MMI_PIC_DIRECTION_E direction_type,uint16 buf_width);

/*****************************************************************************/
//  Description : move osd image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_MoveOSDImage(MMI_MESSAGE_ID_E    msg_id);

/*****************************************************************************/
//  Description : handle tp move osd image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_TouchPenMoveOSDImage(GUI_POINT_T point,MMI_MESSAGE_ID_E  msg_id);

//add for CR251783
/*****************************************************************************/
//  Description : handle tp move char 
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_TouchPenMoveChar(PIC_CHAR_PARAM_T* add_char_info,GUI_COLOR_T* char_color,GUI_POINT_T point,MMI_MESSAGE_ID_E  msg_id);
//add for CR251783

/*****************************************************************************/
//  Description : get frame image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
//PUBLIC void MMIPIC_GetFrameImage(MMI_IMAGE_ID_T  image_id,MMI_WIN_ID_T win_id);
PUBLIC BOOLEAN MMIPIC_GetFrameImage(MMI_IMAGE_ID_T  image_id,MMI_WIN_ID_T win_id);
/*****************************************************************************/
//  Description : get effect image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
//PUBLIC void MMIPIC_GetEffectImage(MMI_PIC_EFFECT_E  mmi_effect);
PUBLIC BOOLEAN MMIPIC_GetEffectImage(MMI_PIC_EFFECT_E  mmi_effect);
/*****************************************************************************/
//  Description : resize image
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
//PUBLIC void MMIPIC_ResizeImage(MMI_PIC_RESIZE_E resize);
PUBLIC BOOLEAN MMIPIC_ResizeImage(MMI_PIC_RESIZE_E resize);

/*****************************************************************************/
//  Description : resert adjustment
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_ResetAdjustment(void);

/*****************************************************************************/
//  Description : resert RGB value
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_ResetRGB(void);

/*****************************************************************************/
//  Description : adjust RGB value
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
//PUBLIC void MMIPIC_AdjustRGB(PEPROCESS_COLOR_BALANCE_E color,BOOLEAN is_add_RGB);
PUBLIC BOOLEAN MMIPIC_AdjustRGB(PEPROCESS_COLOR_BALANCE_E color,BOOLEAN is_add_RGB);

/*****************************************************************************/
//  Description : adjust bright ,constract,hue,saturation
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
//PUBLIC void MMIPIC_Adjustment(MMI_PIC_ADJUSTMENT_E adjustment,BOOLEAN is_add);
PUBLIC BOOLEAN MMIPIC_Adjustment(MMI_PIC_ADJUSTMENT_E adjustment,BOOLEAN is_add);


/*****************************************************************************/
//  Description : exit from pic editor
//  Global resource dependence : none
//  Author: vincent.shang
//  Note:
/*****************************************************************************/
PUBLIC void MMIPIC_ExitFromPicEditor(void);

/*****************************************************************************/
//  Description : judge if the bmp image can be saved as jpg file accroding to its width and height
//  Global resource dependence : 
//  Author: JerryLiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPIC_IfBmpCanBeProcessed(void);

/*****************************************************************************/
//  Description : judege if the bmp can be pasted the clip according its width and height
//  Global resource dependence : 
//  Author: JerryLiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPIC_IfBmpCanBePasted(void);

/*****************************************************************************/
//  Description : judege if the bmp can be add character according to its width and height
//  Global resource dependence : 
//  Author: JerryLiang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN  MMIPIC_IfBmpCanBeAddCharacter(void);

/*****************************************************************************/
//  Description : get the current picture's width and height
//  Global resource dependence : 
//  Author: Robert.wang
//  Date:  2009-8-6
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_GetPictureHeightAndWidth(
                                           int16    *height,  //out
                                           int16    *width      //out
                                           );

/*****************************************************************************/
//  Description : get the current picture's  status  as changing
//  Global resource dependence : 
//  Author: Robert.wang
//  Date:  2009-8-20
//  Note: 
/*****************************************************************************/
PUBLIC  BOOLEAN  MMIPIC_IsPictureChanged(void);

/*****************************************************************************/
// 	Description : handle waiting  win msg  for  saving  as 
//	Global resource dependence : 
//  Author: robert.wang
//  Date: 09-8-20
//	Note: 
/*****************************************************************************/
PUBLIC  MMI_RESULT_E HandleCopyFileWaitWinMsg(
                                       MMI_WIN_ID_T        win_id,     //in
                                       MMI_MESSAGE_ID_E    msg_id,  //in
                                       DPARAM              param           //in
                                       );

/*****************************************************************************/
//  Description : copy original image  data  to  output image data
//  Global resource dependence : 
//  Author: robert.wang
//  Date: 09-10-26
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_CopyOrgImgDataToOutputImgData(void);

/*****************************************************************************/
//  Description : handle touch pen move
//  Global resource dependence : 
//  Author: robert.wang
//  Date: 09-10-26
//  Note: 
//        显示OUTPUT 图片
/*****************************************************************************/
PUBLIC void MMIPIC_TouchPenMoveOutputImage(GUI_POINT_T  point,
                                           MMI_WIN_ID_T win_id,
                                           MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
//  Description : reset picture display rect
//  Global resource dependence : 
//  Author: Robert.wang
//  Date:  2009-10-26
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_ResetPictureDisplayRect(void);

/*****************************************************************************/
//  Description : copy output rect to original rect
//  Global resource dependence : 
//  Author: robert.wang
//  Date: 09-10-26
//  Note: for  the picture  be changed, ie. resize and rotation
/*****************************************************************************/
PUBLIC void MMIPIC_CopyOutputRectToOrgRect(void);

/*****************************************************************************/
// 	Description : handle error  win msg  
//	Global resource dependence : 
//  Author: robert.wang
//  Date: 09-11-17
//	Note: 
//         close self window and  main window, exit pic editor
/*****************************************************************************/
PUBLIC  MMI_RESULT_E HandleErrorWinMsg(
                                       MMI_WIN_ID_T        win_id,   //in
                                       MMI_MESSAGE_ID_E    msg_id,  //in  
                                       DPARAM              param    //in
                                       );

/*****************************************************************************/
//  Description : save to undo buf ,save width and height,save x,y,
//                     not copy output img data to org img data
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_OnlySaveOrgImgToUndoBuf(void);

/*****************************************************************************/
//  Description : set picture RGB value as grade
//  Global resource dependence : 
//  Author: Robert.wang
//  Note: 设置RGB颜色值的等级，共9级(0~8)
//          颜色值(-100 ~ 100)对应0~8级
//  Param: [Return] value of MMI_PIC_ERROR_TYPE_E
/*****************************************************************************/
PUBLIC MMI_PIC_ERROR_TYPE_E MMIPIC_SetRGBValue(int32 grade,  // [in] 0~8
                                    PEPROCESS_COLOR_BALANCE_E r_g_b  // [in] 0~2
                                    );
                                    
/*****************************************************************************/
//  Description : set picture adjustment value as grade
//  Global resource dependence : 
//  Author: Robert.wang
//  Note: 
//  Param: [Return] MMI_PIC_ERROR_TYPE_E
/*****************************************************************************/
PUBLIC MMI_PIC_ERROR_TYPE_E MMIPIC_SetAdjustmentValue(int32 grade,  // [in]
                                            MMI_PIC_ADJUSTMENT_E adjustment  // [in] 
                                            );

/*****************************************************************************/
//  Description : Set osd image position
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_SetOSDImagePosition(void);

/*****************************************************************************/
//  Description : show error or normal message
//  Global resource dependence : 
//  Author:robert.wang
//  Note: 
//  Param 
//      Input:  fs_error   value of MMIFILE_ERROR_E type
/*****************************************************************************/
PUBLIC void  MMIPIC_ShowMsg(MMI_PIC_ERROR_TYPE_E pic_error);

/*****************************************************************************/
//  Description : draw progressbar group
//  Global resource dependence : 
//  Author: vincent
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_DrawProgressBarPanel(MMI_WIN_ID_T            win_id,
                                        MMIPIC_PROGRESSBAR_T    *progressbar_ptr, //in
                                        MMIPIC_PROGRESSBAR_RECT_T  *progressbar_rect //out
                                        );

/*****************************************************************************/
//  Description : Draw Focus Rect 
//  Global resource dependence : 
//  Author: vincent
//  Note:   
/*****************************************************************************/
PUBLIC void MMIPIC_EditWinDrawFocusRect(
                                GUI_RECT_T rect,
                                GUI_COLOR_T color,
                                int16 interval
                                );
                                
/*****************************************************************************/
//  Description : get edit buffer size
//  Global resource dependence : 
//  Author: robert.wang
//  Note: lcd_width*lcd_height*2
/*****************************************************************************/
PUBLIC uint32 MMIPIC_GetEditBufferSize(void);

/*****************************************************************************/
//  Description : get process buffer size
//  Global resource dependence : 
//  Author: robert.wang
//  Note: lcd_width*lcd_height*3 + MMIPIC_PROCESS_BUFFER_FIXED_SIZE
/*****************************************************************************/
PUBLIC uint32 MMIPIC_GetProcessBufferSize(void);

/*****************************************************************************/
//  Description : init pic process parameter 
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIPIC_InitProcessParam(void);

/*****************************************************************************/
//  Description : get fill color rect width  
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC int32 MMIPIC_GetFillColorRectWidth(void);

/*****************************************************************************/
//  Description : get file latter name to combine full file name
//  Global resource dependence : 
//  Author: Robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC void  MMIPIC_GetSaveLatterName(void);

/*****************************************************************************/
//  Description : get the client rect of pic_edit wins  
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIPIC_GetClientWinRect(MMI_WIN_ID_T  win_id);

/*****************************************************************************/
//  Description : get the relative rect off pic  
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC GUI_RECT_T MMIPIC_GetRelativeRect(MMI_WIN_ID_T  win_id);

// /*****************************************************************************/
// //  Description : change the positon of display image by msg_id
// //  Global resource dependence : 
// //  Author: vincent
// //  Note: 
// /*****************************************************************************/
// PUBLIC void MMIPIC_MoveDisplayImage(MMI_MESSAGE_ID_E    msg_id);

// /*****************************************************************************/
// //  Description : change the position of output image
// //  Global resource dependence : 
// //  Author: yaye.jiang
// //  Date: 11-12-07
// //  Note: 
// //      处理图片
// /*****************************************************************************/
// PUBLIC void MMIPIC_MoveOutputImage(MMI_WIN_ID_T win_id, MMI_MESSAGE_ID_E msg_id);

/*****************************************************************************/
//  Description : judge if click the character rect  
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIC_IfClickCharRect(MMI_WIN_ID_T win_id, PIC_CHAR_PARAM_T* add_char_info, GUI_POINT_T tp_point);

/*****************************************************************************/
//  Description : judge if click the character rect  
//  Global resource dependence : 
//  Author: robert.wang
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN MMIPIC_IfClickClipRect(GUI_POINT_T tp_point);

/*****************************************************************************/
//  Description : Get the info of bitmap
//  Global resource dependence : 
//  Author: yaye.jiang
//  Note: 
/*****************************************************************************/
PUBLIC void MMIPIC_GetBitmapInfo(GUIIMG_BITMAP_T * img_info_ptr);

#endif
#endif

