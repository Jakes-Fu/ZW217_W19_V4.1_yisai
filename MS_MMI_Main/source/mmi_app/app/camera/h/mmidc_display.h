#ifndef _MMIDC_DISPLAY_H
#define _MMIDC_DISPLAY_H

#include "mmidc_camera_image.h"
#include "mmidc_camera_text.h"
#include "mmidc_flow.h"
#include "mmidc_gui.h"
#include "mmipub.h"
#include "mmidc_setting.h"
#include "guistring.h"
#include "guilcd.h"
#include "mmk_type.h"


//#ifdef MAINLCD_SIZE_240X320
//#define DC_SUPPORT_PNG
//#endif
#if !defined(MAINLCD_SIZE_128X160)    
#define MMIDC_TIP_ADJUST_SPACE 10 // between icon and 1X 
#else
#define MMIDC_TIP_ADJUST_SPACE 2 // between icon and 1X
#endif

#if defined(MAINLCD_SIZE_240X320) || defined(MAINLCD_SIZE_240X400)
#if defined (MAINLCD_SIZE_240X320) && defined(VECTOR_FONT_SUPPORT_ETRUMP)
	#define ICON_SIZE_FONT  SONG_FONT_12
	#define ICON_NORMAL_FONT SONG_FONT_14  //VECTOR_FONT_SUPPORT_ETRUMP引擎下界面icon字体太小
	#define ICON_SIZE_RIGHT_OFFSET    3
#else
    #define ICON_SIZE_FONT  SONG_FONT_14 //SONG_FONT_10
    //#ifdef TOUCH_PANEL_SUPPORT
        #define ICON_NORMAL_FONT SONG_FONT_14 //SONG_FONT_10
    //#else
    //    #define ICON_NORMAL_FONT SONG_FONT_14
    //#endif
#endif  /*(MAINLCD_SIZE_240X320) && defined(VECTOR_FONT_SUPPORT_ETRUMP)*/
    #define ICON_OFF_LEFT     24
    #define ICON_OFF_RIGHT    3
    
#elif  MAINLCD_SIZE_320X480

    #define ICON_SIZE_FONT  SONG_FONT_12
    #define ICON_NORMAL_FONT SONG_FONT_16

    #define ICON_OFF_LEFT    29
    #define ICON_OFF_RIGHT   3

#endif


#define DC_SHOW_TIME_LENGTH				64

// FONT and FONT SIZE
#define MMIDC_TIP_FONT          MMI_DEFAULT_SMALL_FONT
#define MMIDC_TIP_FONT_SIZE     MMI_DEFAULT_SMALL_FONT_SIZE

#if (defined MAINLCD_SIZE_176X220)
#define MMIDC_TEXT_FONT         DP_FONT_14//MMI_DEFAULT_NORMAL_FONT
#define MMIDC_TEXT_FONT_SIZE    DP_FONT_14          //MMI_DEFAULT_NORMAL_FONT_SIZE
#elif defined MAINLCD_SIZE_128X160
#define MMIDC_TEXT_FONT         DP_FONT_12//MMI_DEFAULT_NORMAL_FONT
#define MMIDC_TEXT_FONT_SIZE    DP_FONT_12          //MMI_DEFAULT_NORMAL_FONT_SIZE
#elif (defined MAINLCD_SIZE_240X320)
#define MMIDC_TEXT_FONT         DP_FONT_18//MMI_DEFAULT_NORMAL_FONT
#define MMIDC_TEXT_FONT_SIZE    DP_FONT_18          //MMI_DEFAULT_NORMAL_FONT_SIZE
#elif(defined DC_WATCH_UI_SUPPORT)
#define MMIDC_TEXT_FONT         DP_FONT_24//MMI_DEFAULT_NORMAL_FONT
#define MMIDC_TEXT_FONT_SIZE    DP_FONT_24          //MMI_DEFAULT_NORMAL_FONT_SIZE
#else
#define MMIDC_TEXT_FONT         DP_FONT_20 //MMI_DEFAULT_BIG_FONT
#define MMIDC_TEXT_FONT_SIZE    DP_FONT_20          //MMI_DEFAULT_BIG_FONT_SIZE
#endif

#define MMIDC_TEXT_COLOR        0xffbf//白色反色后边框为纯黑，与透明色一样，描边不起效果//MMI_WHITE_COLOR

#if defined MMIDC_ADD_SPECIAL_EFFECT || defined VECTOR_FONT_SUPPORT_ETRUMP
#define MMIDC_CHILDWIN_BACKGROUND    0xe73c//灰色
#define MMIDC_CHILDWIN_CTRL_BACKGROUND    0x5acb//深灰色
#endif

#ifdef MMIDC_MINI_RESOURCE_SIZE
#define MMIDC_MENU_TITLE_BACKGROUND    0x3165//褐色	
#endif
//TRANSPARENT COLOR
#define MMIDC_TRANSPARENT_COLOR     MMI_BLACK_COLOR


#ifdef  MAINLCD_SIZE_176X220
    //SPACE and Text Height
    #define MMIDC_TIP_TEXT_TOP_SPACE  1
    #define MMIDC_TIP_TEXT_HEIGHT       (2*MMIDC_TIP_TEXT_TOP_SPACE + MMIDC_TIP_FONT_SIZE)
    #define MMIDC_TIP_TEXT_HOR_SPACE  1    //horizontal space

    #define MMIDC_ITEM_TEXT_HEIGHT      (MMIDC_TEXT_FONT_SIZE+DP2PX_VALUE(8))

    #define MMIDC_TIP_ICON_SPACE      4

    //ADJUST ICON  SPACE
    #define MMIDC_ADJUST_ICON_SPACE   4

    #define MMIDC_MENU_MARGIN         2
#ifdef MMIDC_ADD_SPECIAL_EFFECT    
	#define MMIDC_CHILDWIN_HOR_SPACE    30         
	#define MMIDC_CHILDWIN_VER_SPACE    40
	#define MMIDC_CHILDWIN_ADJUST_SPACE    2
#endif
#elif defined MAINLCD_SIZE_128X160
   //SPACE and Text Height
    #define MMIDC_TIP_TEXT_TOP_SPACE  1
    #define MMIDC_TIP_TEXT_HEIGHT       (2*MMIDC_TIP_TEXT_TOP_SPACE + MMIDC_TIP_FONT_SIZE)
    #define MMIDC_TIP_TEXT_HOR_SPACE  1    //horizontal space

    #define MMIDC_ITEM_TEXT_HEIGHT      (MMIDC_TEXT_FONT_SIZE+DP2PX_VALUE(8))

    #define MMIDC_TIP_ICON_SPACE      1

    //ADJUST ICON  SPACE
    #define MMIDC_ADJUST_ICON_SPACE   4

    #define MMIDC_MENU_MARGIN         2
#ifdef MMIDC_ADD_SPECIAL_EFFECT    
	#define MMIDC_CHILDWIN_HOR_SPACE    13         
	#define MMIDC_CHILDWIN_VER_SPACE    36
	#define MMIDC_CHILDWIN_ADJUST_SPACE    4
#endif
#else
    //SPACE and Text Height
    #define MMIDC_TIP_TEXT_TOP_SPACE  2
    #define MMIDC_TIP_TEXT_HEIGHT       (2*MMIDC_TIP_TEXT_TOP_SPACE + MMIDC_TIP_FONT_SIZE)
    #define MMIDC_TIP_TEXT_HOR_SPACE  2    //horizontal space

    #define MMIDC_ITEM_TEXT_HEIGHT    (MMIDC_TEXT_FONT_SIZE+DP2PX_VALUE(8))

    #define MMIDC_TIP_ICON_SPACE     5

    //ADJUST ICON  SPACE
    #define MMIDC_ADJUST_ICON_SPACE  8

    #define MMIDC_MENU_MARGIN        5

    //BOLD FONT
    #define MMIDC_BOLD_FONT     1
#ifdef MMIDC_ADD_SPECIAL_EFFECT    
	#define MMIDC_CHILDWIN_HOR_SPACE    40         
	#define MMIDC_CHILDWIN_VER_SPACE    60
	#define MMIDC_CHILDWIN_ADJUST_SPACE    2
#endif    
#endif
#define     FRAME_DISPLAY_POS_HVGA_VER_X_1           0
#define     FRAME_DISPLAY_POS_HVGA_VER_Y_1           205

#define     FRAME_DISPLAY_POS_HVGA_VER_X_2           25
#define     FRAME_DISPLAY_POS_HVGA_VER_Y_2           195

#define     FRAME_DISPLAY_POS_HVGA_VER_X_3           31
#define     FRAME_DISPLAY_POS_HVGA_VER_Y_3           130

#define     FRAME_DISPLAY_POS_HVGA_VER_X_4           18
#define     FRAME_DISPLAY_POS_HVGA_VER_Y_4           208

#define     FRAME_DISPLAY_POS_HVGA_HOR_X_1           20
#define     FRAME_DISPLAY_POS_HVGA_HOR_Y_1           44

#define     FRAME_DISPLAY_POS_HVGA_HOR_X_2           107
#define     FRAME_DISPLAY_POS_HVGA_HOR_Y_2           75

#define     FRAME_DISPLAY_POS_HVGA_HOR_X_3           112
#define     FRAME_DISPLAY_POS_HVGA_HOR_Y_3           49

#define     FRAME_DISPLAY_POS_HVGA_HOR_X_4           18
#define     FRAME_DISPLAY_POS_HVGA_HOR_Y_4           49

#define     FRAME_DISPLAY_POS_QVGA_VER_X_1           0
#define     FRAME_DISPLAY_POS_QVGA_VER_Y_1           125

#define     FRAME_DISPLAY_POS_QVGA_VER_X_2           25
#define     FRAME_DISPLAY_POS_QVGA_VER_Y_2           155

#define     FRAME_DISPLAY_POS_QVGA_VER_X_3           31
#define     FRAME_DISPLAY_POS_QVGA_VER_Y_3           98

#define     FRAME_DISPLAY_POS_QVGA_VER_X_4           10
#define     FRAME_DISPLAY_POS_QVGA_VER_Y_4           39

#define     FRAME_DISPLAY_POS_QVGA_HOR_X_1           0
#define     FRAME_DISPLAY_POS_QVGA_HOR_Y_1           44

#define     FRAME_DISPLAY_POS_QVGA_HOR_X_2           66
#define     FRAME_DISPLAY_POS_QVGA_HOR_Y_2           75

#define     FRAME_DISPLAY_POS_QVGA_HOR_X_3           70
#define     FRAME_DISPLAY_POS_QVGA_HOR_Y_3           49

#define     FRAME_DISPLAY_POS_QVGA_HOR_X_4           18
#define     FRAME_DISPLAY_POS_QVGA_HOR_Y_4           39

#define     FRAME_DISPLAY_POS_WQVGA_VER_X_1           0
#define     FRAME_DISPLAY_POS_WQVGA_VER_Y_1           205

#define     FRAME_DISPLAY_POS_WQVGA_VER_X_2           25
#define     FRAME_DISPLAY_POS_WQVGA_VER_Y_2           195

#define     FRAME_DISPLAY_POS_WQVGA_VER_X_3           31
#define     FRAME_DISPLAY_POS_WQVGA_VER_Y_3           130

#define     FRAME_DISPLAY_POS_WQVGA_VER_X_4           18
#define     FRAME_DISPLAY_POS_WQVGA_VER_Y_4           208

#define     FRAME_DISPLAY_POS_WQVGA_HOR_X_1           20
#define     FRAME_DISPLAY_POS_WQVGA_HOR_Y_1           44

#define     FRAME_DISPLAY_POS_WQVGA_HOR_X_2           107
#define     FRAME_DISPLAY_POS_WQVGA_HOR_Y_2           75

#define     FRAME_DISPLAY_POS_WQVGA_HOR_X_3           112
#define     FRAME_DISPLAY_POS_WQVGA_HOR_Y_3           49

#define     FRAME_DISPLAY_POS_WQVGA_HOR_X_4           18
#define     FRAME_DISPLAY_POS_WQVGA_HOR_Y_4           49

#define     FRAME_DISPLAY_POS_QCIF_VER_X_1           0
#define     FRAME_DISPLAY_POS_QCIF_VER_Y_1           20

#define     FRAME_DISPLAY_POS_QCIF_VER_X_2           0
#define     FRAME_DISPLAY_POS_QCIF_VER_Y_2           20

#define     FRAME_DISPLAY_POS_QCIF_VER_X_3           0
#define     FRAME_DISPLAY_POS_QCIF_VER_Y_3           20

#define     FRAME_DISPLAY_POS_QCIF_VER_X_4           10
#define     FRAME_DISPLAY_POS_QCIF_VER_Y_4           20

#define     FRAME_DISPLAY_POS_QCIF_HOR_X_1           20
#define     FRAME_DISPLAY_POS_QCIF_HOR_Y_1           0

#define     FRAME_DISPLAY_POS_QCIF_HOR_X_2           47
#define     FRAME_DISPLAY_POS_QCIF_HOR_Y_2           0

#define     FRAME_DISPLAY_POS_QCIF_HOR_X_3           22
#define     FRAME_DISPLAY_POS_QCIF_HOR_Y_3           0

#define     FRAME_DISPLAY_POS_QCIF_HOR_X_4           20
#define     FRAME_DISPLAY_POS_QCIF_HOR_Y_4           10

#define     LAST_FRAME_BUFFER_ARGB888                   4

//#define DC_OSD_DBG

typedef enum
{
    MOVE_UP = 0,
        MOVE_DOWN,
        MOVE_LEFT,
        MOVE_RIGHT,
        MOVE_MAX
}MOVE_DIRECTION_E;


//added, @robert.wang, 09-11-25, cr161690
typedef struct
{
    GUI_RECT_T  rect;    //display string  rect
    MMI_STRING_T    *text_ptr;    //text string

    GUISTR_STATE_T  state;

    GUISTR_STYLE_T text_style;
} MMIDC_STRING_STYLE_T;
//end cr161690

typedef enum
{
    DESKTOP_STYLE_TIP,
    DESKTOP_STYLE_ICON,
    DESKTOP_STYLE_PDA,
    DESKTOP_STYLE_MAX
}MMIDC_DESKTOP_STYLE_E;

PUBLIC void MMIDC_SetTransparentColor(GUI_COLOR_T color);
PUBLIC GUI_COLOR_T MMIDC_GetTransparentColor(void);
PUBLIC GUI_LCD_DEV_INFO MMIDC_GetOSDBlockInfo(void);


PUBLIC void MMIDC_SetReviewMenu(void);
PUBLIC void MMIDC_CloseOsd(void);
PUBLIC void MMIDC_GUIBLOCK_ClearBlock(GUI_LCD_DEV_INFO* dev_info_ptr, GUI_COLOR_T color);
PUBLIC void MMIDC_ClearOSDBlock(void);
PUBLIC void MMIDC_ClearMainBlock(GUI_COLOR_T color);
PUBLIC void MMIDC_CompleteDisplayOSD(void);
PUBLIC void MMIDC_DisplaySoftKey(DC_FLOW_MODE_E mode);

#if defined MMIDC_MINI_DISPLAY_STYLE || defined MMIDC_MINI_3264_STYLE
PUBLIC void MMIDC_ShowDVReviewSoftKey();
#endif

PUBLIC void MMIDC_DisplayVideoRecordTip(DC_FLOW_MODE_E mode);
PUBLIC void MMIDC_DisplayCurrentFileName(void);
PUBLIC void	MMIDC_DisplayZoomTip(int32 zoom);
PUBLIC void MMIDC_DisplayVideoReview(void);

/*****************************************************************************/
// 	Description : display all setting tips in preview mode
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void	MMIDC_DisplaySettingTip(void);

PUBLIC void MMIDC_DisplayDelaySecondsTip(uint8 seconds);
PUBLIC void MMIDC_DisplayVideoRecordTip(DC_FLOW_MODE_E mode);
PUBLIC void	MMIDC_DisplayBrightTip(void);
PUBLIC void MMIDC_DisplayFrame(void);
/*****************************************************************************/
// 	Description : display video record tip
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayPatchOSDRecordTip(void);

/*****************************************************************************/
// 	Description : display soft key base hor or ver screen
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMIDC_IsMiddleSoftKeyNULL(void);
/*****************************************************************************/
// 	Description : display string scroll if the length is too long
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayString(GUI_RECT_T  rect,MMI_STRING_T  *text_ptr,GUI_COLOR_T bg_color,BOOLEAN is_auto_scroll);

/*****************************************************************************/
//  Description : start text scroll timer
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_StartTextScrollTimer(void);

/*****************************************************************************/
//  Description : stop text scroll timer
//  Global resource dependence : 
//  Author: ryan.xu
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_StopTextScrollTimer(void);

/*****************************************************************************/
// 	Description : reset need scroll number
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ResetScrollNumber(void);

/*****************************************************************************/
// 	Description : display bmp
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayBmp(int16 x, int16 y, MMI_IMAGE_ID_T	image_id);

/*****************************************************************************/
// 	Description : start osd menu display timer
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_StartTipDisplayTimer(void);

/*****************************************************************************/
// 	Description : set lcd infor for rotate dc
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_SetLcdForRotate(BOOLEAN set);

/*****************************************************************************/
// 	Description : open alert window
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_OpenAlertWin(									 
                               MMIPUB_SOFTKEY_STYLE_E  softkey_type,   //softkey type
                               MMI_TEXT_ID_T          text_id,        //text id
                               MMI_IMAGE_ID_T            image_id,       //image ptr
                               uint32                 time_out);       //time out

/*****************************************************************************/
// 	Description : display photo frame 0
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC GUI_POINT_T MMIDC_GetFrameDisplayPos(FRAME_E id);

/*****************************************************************************/
// 	Description : display softkey
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayQuickViewSoftkey(void);

/*****************************************************************************/
// 	Description : display background bmp by picture
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayBackgroundBMP(uint16 x, uint16 y, GUI_RECT_T rect,MMI_IMAGE_ID_T image_id);

/*****************************************************************************/
// 	Description : display string background bmp mask by picture
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayBMPMask(uint16 x, uint16 y, GUI_RECT_T rect);

/*****************************************************************************/
// 	Description : display title background bmp by picture
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayTitleBackground(uint16 x, uint16 y, GUI_RECT_T rect);

/*****************************************************************************/
// 	Description : get title text string display
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayTitleString(GUI_RECT_T  rect,MMI_STRING_T  *text_ptr,GUI_COLOR_T bg_color,BOOLEAN is_auto_scroll);

/*****************************************************************************/
// 	Description : get item text string display
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplaySpecialString(GUI_RECT_T  rect,
                                                        MMI_STRING_T  *text_ptr,
                                                        GUI_COLOR_T bg_color,
                                                        BOOLEAN is_auto_scroll);

/*****************************************************************************/
// 	Description : get text string height pixels as normal text font
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetStringHeight(MMI_STRING_T *string);

/*****************************************************************************/
// 	Description : get text string width pixels as normal text font
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetStringWidth(MMI_STRING_T  *string);

/*****************************************************************************/
// 	Description : get special text string width pixels as special text font
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetSpecialStringWidth(MMI_STRING_T  *string);

/*****************************************************************************/
// 	Description : get special text string height pixels as special text font
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetSpecialStringHeight(MMI_STRING_T *string);

/*****************************************************************************/
// 	Description : clear rect by  color
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ClearRect(GUI_RECT_T  rect,GUI_COLOR_T  color);

/*****************************************************************************/
// 	Description : get gray item text string display
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayGrayString(GUI_RECT_T  rect,
                                                        MMI_STRING_T  *text_ptr,
                                                        GUI_COLOR_T bg_color,
                                                        BOOLEAN is_auto_scroll);

/*****************************************************************************/
// 	Description : get osd font state
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
//  Param:
//      Return: GUISTR_STATE_T
/*****************************************************************************/
PUBLIC GUISTR_STATE_T MMIDC_GetOSDFontState(void);

/*****************************************************************************/
// 	Description : clean all osd in preview status
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_CloseAllOSDMenu(void);

/*****************************************************************************/
// 	Description : clear all osd in preview status 
//                and display current mode softkey
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ClearMenuInPreview(void);

/*****************************************************************************/
//  Description : clear sublcd screen rect by  color
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC void MMIDC_ClearSUBLCD(void);

PUBLIC GUI_RECT_T MMIDC_GetFullScreenRect(GUI_LCD_ID_E lcd_id);

/*****************************************************************************/
//  Description : show all menu and softkey 
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_ShowAllOSD(void);

/*****************************************************************************/
// 	Description : clear photo frame rect
//	Global resource dependence : 
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ClearFrameRect(void);

/*****************************************************************************/
// 	Description : clear softkey rect
//	Global resource dependence : none
//  Author: ryan.xu
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_ClearSoftkeyRect(void);

/*****************************************************************************/
//  Description : show all menu and softkey 
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_CleanAllOSDRect(void);

/*****************************************************************************/
// 	Description : display dv last frame
//	Global resource dependence : none
//  Author: robert.wang
//	Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayLastFrame(void);

/*****************************************************************************/
//Description :  get desktop layout
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC MMIDC_DESKTOP_STYLE_E MMIDC_GetDesktopStyle(void);

/*****************************************************************************/
//  Description : clear all icon, menu, softkey, frame
//  Global resource dependence : none
//  Author: robert.wang
//  Note:
/*****************************************************************************/
PUBLIC int32 MMIDC_ClearAllRect(void);

/*****************************************************************************/
//Description : get normal text string height pixels by font
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetStringHeightByFont(GUI_FONT_T font,
                                                    MMI_STRING_T *string //in
                                                    );
                                                    
/*****************************************************************************/
//Description : get normal text string width pixels by font
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC uint16 MMIDC_GetStringWidthByFont(GUI_FONT_T font,
                                                    MMI_STRING_T  *string //in
                                                    );
                                                    
/*****************************************************************************/
//Description : get item text string display by font
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayStringByFont(GUI_FONT_T font,
                                               GUI_RECT_T  rect,
                                               MMI_STRING_T  *text_ptr,
                                               GUI_COLOR_T bg_color,
                                               BOOLEAN is_auto_scroll
                                               );
                                               
/*****************************************************************************/
//Description : get item text string display by font
//Global resource dependence : none
//Author: robert.wang
//Note:
/*****************************************************************************/
PUBLIC void MMIDC_DisplayDateOSDStringByFont(GUI_FONT_T font,
                                               GUI_RECT_T  rect,
                                               MMI_STRING_T  *text_ptr,
                                               GUI_COLOR_T bg_color,
                                               BOOLEAN is_auto_scroll
                                               );

#if 0
PUBLIC void MMIDC_DisplayWaitPhoto(void);
#endif

#ifdef DC_WATCH_UI_SUPPORT 

#define RGB888_TO_RGB565(r, g, b)  ((((r) << 8) & 0xf800) |(((b) >> 3) & 0x1f) |  ( ( ((g)>>0x02)&0x3f)<<5) ) 
#define RGB5652ARGB888(rgb565,alpha) ( ((alpha)<<24) |((((rgb565) >> 8) & 0xf8)<<16)|((((rgb565) >> 3) & 0xfc)<<8)|(((rgb565) << 3)  & 0xf8))

typedef enum{
    ICON_OPEN_GALLERY=0,
    ICON_CAPTURE_IMAGE,
    ICON_RECORD_START,
    ICON_RECORD_STOP,
    ICON_SWITCH_DV,
    ICON_SWITCH_DC,
    ICON_DV_PLAY,
    ICON_DV_PAUSE,
    ICON_DC_CANCEL,
    ICON_DC_CONFIRM,
    ICON_DV_CANCEL,
    ICON_DV_CONFIRM,
//    ICON_WAITING,
    ICON_MAX
}DC_ICON_INDEX_E;
typedef struct _DC_ICON_INFO_T{
    MMI_IMAGE_ID_T  image_id;
    GUI_RECT_T      rect;
}DC_ICON_INFO_T;

#define MAX_ICON_ON_SCREEN 3

typedef struct _DC_OSD_LAYOUT_T{
    DC_ICON_INDEX_E icon_list[MAX_ICON_ON_SCREEN+1]; 
    uint16 focus_index;
    uint16 default_focus_index;
    BOOLEAN visiable;
    BOOLEAN default_visiable;
}DC_OSD_LAYOUT_T;


/*****************************************************************************/
//  Description: get image display rect by icon index.
//  Parameter: [In] icon_index: icon index in s_icon_table
//             [Out] None
//             [Return] GUI_RECT
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC GUI_RECT_T WDC_GetIconRect(DC_ICON_INDEX_E icon_index);

/*****************************************************************************/
//  Description: get image id by icon index.
//  Parameter: [In] icon_index: icon index in s_icon_table
//             [Out] None
//             [Return] IMAGE_ID
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC MMI_IMAGE_ID_T WDC_GetIconImageID(DC_ICON_INDEX_E icon_index);

/*****************************************************************************/
//  Description: get osd visiable or not at current flow mode.
//  Parameter: [In] mode: flow mode.  
//             [Out] None
//             [Return] BOOLEAN: : true if visable
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN WDC_GetOsdVisiablity(DC_FLOW_MODE_E mode);

/*****************************************************************************/
//  Description: set osd visiable or not at current flow mode.
//  Parameter: [In] mode: flow mode.  
//                  visiable: true if set to visable
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC void WDC_SetOsdVisiablity(DC_FLOW_MODE_E mode, BOOLEAN visiable);

/*****************************************************************************/
//  Description: get icon index in s_osd_layout_list[mode].icon_list[index]
//  Parameter: [In] mode: flow mode.  
//                  index: index in s_osd_layout_list. 
//             [Out] None
//             [Return] icon_index: icon index in s_icon_table
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC DC_ICON_INDEX_E WDC_GetOsdIconIndex(DC_FLOW_MODE_E mode, uint16 index);

/*****************************************************************************/
//  Description: change icon index in s_osd_layout_list[mode].icon_list[index] 
//  Parameter: [In] mode: flow mode.  
//                  index: index in s_osd_layout_list. 
//                  icon_index: icon index in s_icon_table
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC void WDC_ChangeOsdIconIndex(DC_FLOW_MODE_E mode, uint16 index, DC_ICON_INDEX_E icon_index);

/*****************************************************************************/
//  Description: reset icon focus to default.
//  Parameter: [In] mode: flow mode.
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC void WDC_ResetOsdIconFocus(DC_FLOW_MODE_E mode);

/*****************************************************************************/
//  Description: get focused index in s_osd_layout_list.
//  Parameter: [In] mode: flow mode.
//             [Out] None
//             [Return] focused index
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC uint16 WDC_GetOsdIconFocus(DC_FLOW_MODE_E mode);

/*****************************************************************************/
//  Description: set icon focus.
//  Parameter: [In] mode: flow mode,  
//                  index: index in s_osd_layout_list.
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC void WDC_SetOsdIconFocus(DC_FLOW_MODE_E mode, uint16 index);

/*****************************************************************************/
//  Description: change play control icon to playing state icon.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN , TRUE if OK.
//  Author: fulu.song
//  Note: pause/stop state display ICON_DV_PLAY, playing state display ICON_DV_PAUSE
/*****************************************************************************/
PUBLIC BOOLEAN WDC_SetPlayingStateIcon(void);

/*****************************************************************************/
//  Description: change play control icon to pause/stop state icon.
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN , TRUE if OK.
//  Author: fulu.song
//  Note: pause/stop state display ICON_DV_PLAY, playing state display ICON_DV_PAUSE
/*****************************************************************************/
PUBLIC BOOLEAN WDC_SetPausedOrStopedStateIcon(void);

/*****************************************************************************/
//  Description: clear icons before exit flow mode.
//  Parameter: [In] mode: flow mode
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC void WDC_Display_FlowModeExit(DC_FLOW_MODE_E mode);

/*****************************************************************************/
//  Description: display ui when enter ? flow mode.
//  Parameter: [In] mode: flow mode.
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC void WDC_Display_FlowModeEnter(DC_FLOW_MODE_E mode);

/*****************************************************************************/
//  Description: draw video recording time and red dot.
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC void WDC_DisplayVideoRecordTip(void);

/*****************************************************************************/
//  Description: reset icon focus on screen.
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC void WDC_ResetIconFocus(void);

/*****************************************************************************/
//  Description: show all osd and commit to lcd
//  Parameter: [In] None
//             [Out] None
//             [Return] BOOLEAN, TRUE if need to display icons.
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC BOOLEAN WDC_ShowAllOSD(void);

/*****************************************************************************/
//  Description: clear all icons rect
//  Parameter: [In] None
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC void WDC_ClearAllRect(void);

/*****************************************************************************/
//  Description: 
//  Parameter: [In] None
//             [Out] None
//             [Return] return 240 for lcd height.
//  Author: fulu.song
//  Note: temp function, on phone lcd, get screen height function will return 320
//       only for developing, avoid hard coding in other functions.
/*****************************************************************************/
PUBLIC int16 WDC_GetOsdHeight(void);

/*****************************************************************************/
//  Description: set icon focus.
//  Parameter: [In] mode: flow mode,  
//                  icon: icon name.
//             [Out] None
//             [Return] None
//  Author: fulu.song
//  Note: 
/*****************************************************************************/
PUBLIC void WDC_IconSetFocus(DC_FLOW_MODE_E mode, DC_ICON_INDEX_E icon);
#if defined(DC_OSD_DBG)
PUBLIC void WDC_DrawTPFocus(MMI_HANDLE_T win_id, uint32 color, GUI_RECT_T rect);
#endif

#endif

#endif

