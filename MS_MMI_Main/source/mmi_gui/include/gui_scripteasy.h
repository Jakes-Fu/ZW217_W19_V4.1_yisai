/*****************************************************************************
** File Name:      gui_scripteasy.h                                                 *
** Author:                                                                   *
** Date:           21/05/2004                                               *
** Copyright:      2004 Spreatrum, Incoporated. All Rights Reserved.         *
** Description:    This file provide functions about font liabrary and display string*
******************************************************************************
**                         Important Edit History                            *
** --------------------------------------------------------------------------*
** DATE                    NAME             DESCRIPTION                      *
** 21/05/2004       Jim.Zhang           Create                               *
******************************************************************************/

#ifndef GUI_SCRIPTEASY_H
#define GUI_SCRIPTEASY_H

/*----------------------------------------------------------------------------*/
/*                         DEPENDANCY                                         */
/*----------------------------------------------------------------------------*/
#include "sci_types.h"
#include "guifont.h"
#include "scripteasy.h"
//#include "gui_mlconfig.h"
/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)

    /* unreferenced formal parameter */
    #pragma warning(disable: 4100)

    /* conditional expression is constant */
    #pragma warning(disable: 4127)

    /* unreferenced local function has been removed */
    #pragma warning(disable: 4505)

    /* named type definition in parentheses */
    #pragma warning(disable: 4115)

    /* nonstandard extension used: nameless struct/union */
    #pragma warning(disable: 4201)

    /* nonstandard extension used : bit field types other than int */
    #pragma warning(disable: 4214)

    /* function 'xxxx' selected for automatic inline expansion */
    #pragma warning(disable: 4711)

    /* This function or variable may be unsafe. Consider using xxxx instead. */
    #pragma warning(disable: 4996)

    /* 'conversion' : from data pointer 'type1' to function pointer 'type2' */
    #pragma warning(disable: 4055)

#endif

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------*/
/*                         Macro Declaration                                  */
/*----------------------------------------------------------------------------*/
/***************************************************************************
*                                                                         *
* Color format conversion.                                                *
*                                                                         *
* On Spreatrum platform, the color value is a 16 bits unsigned interger   *
* contains red, green and blue components int the binary format of        *
* "rrrr rggg gggb bbbb". The highest 5 bits is red value. The next 6 bits *
* is green value. The lowest 5 bits is blue value.                        *
*                                                                         *
* The color format of the main LCD can be different from the sub LCD.     *
* Although they are the same currently.                                   *
*                                                                         *
* On ScriptEasy library, the color value is a 32 bits unsigned integer    *
* contains red, green, blue and alpha components in the binary format of  *
* "aaaa aaaa rrrr rrrr gggg gggg bbbb bbbb". Each component is in 8 bits. *
*                                                                         *
***************************************************************************/
#define GUI_565_color_to_888_color(color)   (((color & 0xF800) << 8) | ((color & 0x07E0) << 5) | ((color & 0x001F) << 3))

#define GUI_888_color_to_565_color(color)   (GUI_COLOR_T) ( ((color >> 8) & 0xF800) | ((color >> 5) & 0x07E0) | ((color >> 3) & 0x001F) )


/*----------------------------------------------------------------------------*/
/*                         Data Structures                                    */
/*----------------------------------------------------------------------------*/
typedef struct
{
    GUI_COLOR_T*    buffer;
    int             width;
    int             height;
    int             type;
    GUI_LCD_DEV_INFO const *dev_info_ptr;
} LcdDeviceInfo;

/*----------------------------------------------------------------------------*/
/*                         Function Declare                                   */
/*----------------------------------------------------------------------------*/
/*****************************************************************************/
//  Description : Init SE Library
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Initialize_Scripteasy_Library(int language);
/*****************************************************************************/
//  Description : Init multilanguage
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Initialize_Multilanguage(int language);

/*****************************************************************************/
//  Description : Get Device Context
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
PUBLIC SeDeviceContext* GUI_Get_Default_Device_Context(const GUI_LCD_DEV_INFO *dev_info_ptr);

/*****************************************************************************/
//  Description : Set SE Device Context Font
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Set_Device_Context_Font(SeDeviceContext* dc, GUI_FONT_T font);


/*****************************************************************************/
//  Description : Convert text to unicode text
//  Global resource dependence : 
//   Author: 
//  Note:
/*****************************************************************************/
PUBLIC uint16* GUI_Convert_Text_To_Unicode(const wchar* text_ptr, uint16* len);
/*****************************************************************************/
//  Description : 改变矩形区域的大小
//  Global resource dependence : 
//   Author: apple.zhang
//  Note: 如果dx和dy都为0，矩形区域的大小保持不变
/*****************************************************************************/
PUBLIC void GUI_Inflate_Rect(
                             SeRectangle * box, //[输入/输出] 指向一个GUI_RECT_T数据结构的指针，存放矩形区域的坐标。不能为空指针
                             int dx, //[输入] 调整矩形区域左边和右边的数值。
                                     //   大于0的数值将增加矩形的宽度，矩形左边的坐标减小，矩形右边的坐标增大。
                                     //   小于0的数值将减小矩形的宽度，矩形左边的坐标增大，矩形右边的坐标减小。
                             int dy //[输入] 调整矩形区域上边和下边的数值。
                                    //大于0的数值将增加矩形的高度，矩形上边的坐标减小，矩形下边的坐标增大。
                                    //小于0的数值将减小矩形的高度，矩形上边的坐标增大，矩形下边的坐标减小。
                              );
/*****************************************************************************/
//  Description : 计算指定的文本显示的宽度、高度和行数
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Text_Info(
                              const SeDeviceContext* dc, 
                              const uint16 * text, //[输入] 指向一个字符串缓冲区的指针，存放了需要处理的文本。不能为空指针。
                              int  length, //[输入] text指向的字符串的长度。如果数值小于0，表示该字符串是一个以0结尾的字符串。字符串长度的最大值不能超过SE_MAX_TEXT_LENGTH。
                              int * width, //[输出] 指向一个整数的指针，用于返回文本显示的宽度。可以是空指针，表示不需要返回该信息。
                              int * height, //[输出] 指向一个整数的指针，用于返回文本显示的高度。可以是空指针，表示不需要返回该信息。
                              int * lines); //[输出] 指向一个整数的指针，用于返回文本显示的行数。可以是空指针，表示不需要返回该信息。

/*****************************************************************************/
//  Description : 得到与显示设备和文本显示相关的信息
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void* GUI_Get_DeviceContent(void);
/*****************************************************************************/
//  Description : 放了与显示设备和文本显示相关的信息
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Set_DeviceContent(void* dc);
/*****************************************************************************/
//  Description : 在指定位置显示一段文本
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Draw_text(
                          const SeDeviceContext* dc, 
                          int  x,//[输入] 文本显示的坐标
                          int y,//[输入] 文本显示的坐标
                          const uint16* text, //[输入] 指向一个字符串缓冲区的指针，存放了需要显示的文本。不能为空指针
                          int   length) ;//[输入] text指向的字符串的长度。如果数值小于0，表示该字符串是一个以0结尾的字符串。字符串长度的最大值不能超过SE_MAX_TEXT_LENGTH。
                          
PUBLIC int GUI_Draw_text_Ex(                          
                          const SeDeviceContext* dc, 
                          int  x,//[输入] 文本显示的坐标
                          int y,//[输入] 文本显示的坐标
                          const uint16* text, //[输入] 指向一个字符串缓冲区的指针，存放了需要显示的文本。不能为空指针
                          int   length, //[输入] text指向的字符串的长度。如果数值小于0，表示该字符串是一个以0结尾的字符串。字符串长度的最大值不能超过SE_MAX_TEXT_LENGTH。
                          int dir);
                          
/*****************************************************************************/
//  Description : 创建并返回一个文本编辑器对象
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Create(void);
/*****************************************************************************/
//  Description : 得到初始化文本编辑器所需要的相关信息
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void* GUI_Get_Editor_Init_Info(void);
/*****************************************************************************/
//  Description : 设置 初始化文本编辑器所需要的相关信息
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Set_Editor_Init_Info(void* init);
/*****************************************************************************/
//  Description :  得到新创建的文本编辑器对象的指针
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void* GUI_Get_Editor(void);
/*****************************************************************************/
//  Description : 设置 新创建的文本编辑器对象的指针
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Set_Editor(void* editor);
/*****************************************************************************/
//  Description : 删除一个文本编辑器对象，并释放相关资源
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_destroy( void);
/*****************************************************************************/
//  Description : 获得一个文本编辑器能容纳的文本内容的最大长度。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Max_Buffer( int * max_buffer); //[输出] 指向一个整数的指针，用于返回编辑器能容纳的文本内容的最大长度。不能为空指针。
/*****************************************************************************/
//  Description :获得一个文本编辑器的文本显示边距。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Editor_Margin(SeRectangle*     margin); //[输出] 指向一个GUI_RECT_T数据结构的指针，用于返回编辑器的文本显示边距。不能为空指针。
/*****************************************************************************/
//  Description :获得一个文本编辑器对象的当前位置。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Editor_Pos(SeRectangle*     pos);
/*****************************************************************************/
//  Description : 设置文本编辑器对象的新位置。
/*****************************************************************************/
PUBLIC int GUI_Set_Editor_Pos(const SeRectangle* pos);
PUBLIC int GUI_Editor_Get_Caret_Text_Index(void);

PUBLIC void GUI_Editor_Set_Caret_Text_Index(int text_index);

PUBLIC void GUI_Editor_Delete_Text_Before_Caret(void);
/*****************************************************************************/
//  Description :  得到描述文本编辑器的显示颜色
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void* GUI_Get_Editor_Palette_Info(void);
/*****************************************************************************/
//  Description : 设置描述文本编辑器的显示颜色
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Set_Editor_Palette_Info(void* palette);
/*****************************************************************************/
//  Description :获得一个文本编辑器的显示颜色。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Editor_Palette(void);
/*****************************************************************************/
//  Description :用于显示一个文本编辑器。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Paint_Editor(void);
/*****************************************************************************/
//  Description :锁定并禁止一个文本编辑器更新显示。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Lock(void);
/*****************************************************************************/
//  Description :解除对一个文本编辑器显示更新的锁定。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_UNLock(void);
/*****************************************************************************/
//  Description :获取一个文本编辑器当前文本的内容和长度。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Editor_Text(
                                const uint16** text, //[输出] 指向一个字符串缓冲区指针的指针，用于返回一个指向编辑器内部文本字符串缓冲区的指针，该缓冲区内存放了编辑器当前的文本内容。可以是空指针，表示不需要返回该信息。
                                int*           length, //[输出] 指向一个整数的指针，用于返回编辑器当前文本内容的长度。可以是空指针，表示不需要返回该信息。
                                int*           num_chars) ;
/*****************************************************************************/
//  Description :清空一个文本编辑器的文本。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Clear_Content(void);
/*****************************************************************************/
//  Description :在一个文本编辑器的当前光标位置进行一次退格删除操作。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_BackSpace(void);
/*****************************************************************************/
//  Description :在一个文本编辑器的当前光标位置插入指定的文本。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Insert_Text(
                                const uint16* text,
                                int         length);
/*****************************************************************************/
//  Description :切换一个文本编辑器内光标的显示状态。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Switch_Caret(void);
/*****************************************************************************/
//  Description :将一个文本编辑器内的光标向左移动一次。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Move_Caret_Left(void);
/*****************************************************************************/
//  Description :将一个文本编辑器内的光标向右移动一次。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Move_Caret_Right(void);
/*****************************************************************************/
//  Description :将一个文本编辑器内的光标向上移动一次。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Move_Caret_Up(void);
/*****************************************************************************/
//  Description :将一个文本编辑器内的光标向下移动一次。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Move_Caret_Down(void);
/*****************************************************************************/
//  Description :将一个文本编辑器内的光标向上移动一页。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Move_Caret_PageUp(void);
/*****************************************************************************/
//  Description :将一个文本编辑器内的光标向下移动一页。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Move_Caret_PageDown(void);
/*****************************************************************************/
//  Description :删除一个文本编辑器内光标当前位置的字符。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Delete_Editor_Caret_Char(void);
/*****************************************************************************/
//  Description :清除一个文本编辑器内输入法当前的编辑内容。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Clear_Editor_IME(void);
/*****************************************************************************/
//  Description :结束一个文本编辑器内输入法当前的编辑操作。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_End_Editor_IME(void);
/*****************************************************************************/
//  Description :在一个文本编辑器的当前光标位置插入指定的文本，作为输入法的编辑内容。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Set_Editor_IME(                           
                            const uint16* text,//[输入] 指定要插入的文本内容和长度。text不能为空指针。如果length小于0，表示text指向是一个以0结尾的字符串。
                            int length);
/*****************************************************************************/
//  Description :返回一个文本编辑器当前输入法的编辑内容。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Get_Editor_IME(                          
                            const uint16** text,//[输出] 指向一个字符串缓冲区指针的指针，用于返回一个指向编辑器内部文本字符串缓冲区的指针，该缓冲区内存放了编辑器当前输入法的编辑内容。可以是空指针，表示不需要返回该信息。
                            int *length);
/*****************************************************************************/
//  Description :返回一个指定了red,green,blue颜色分量的颜色值。
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC uint32 GUI_Color_RGB(   
                            uint8 red,
                            uint8 green,
                            uint8 blue);
/*****************************************************************************/
//  Description :获得一个编辑器的滚行信息
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Set_Editor_Scroll_Info(void);
/*****************************************************************************/
//  Description :返回一个编辑器的滚行信息
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC SeScrollInfo GUI_Get_Editor_Scroll_Info(void);
/*****************************************************************************/
//  Description :将一个文本编辑器内的光标移动到给定坐标最近的位置
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Locate_Caret(
                                   int x,
                                   int y
                                    );

/*****************************************************************************/
//            Description : Get char width for ebook
//            Global resource dependence : none
//  Author: ke wen
//            Note:    
/*****************************************************************************/
PUBLIC uint8 GUI_GetCharWidth_EBook(uint16 unicode,GUI_FONT_T font_type);

/******************************************************************************/
/*! \brief draw a string at ellipsis by align.
 *  When the string is too long ,it displays in one line at ellipsis.
 *  keyword is (Reverse, Label, InRect)
 *  \author jianhui.luo 
 *  \param[in]  lcd_id          ID of LCD to display on
 *  \param[in]  rect            the fixed display area
 *  \param[in]  str_ptr         string to display
 *  \param[in]  len             length of string to display 
 *  \param[in]  char_space      space between two chars 
 *  \param[in]  font_color      string color 
 *  \param[in]  font            font type
 *  \param[in]  is_ucs2         is ucs2 code or ascii code. 1:ucs2 0:ascii code
 *  \param[in]  align           the type of aline: Reverse, Label, InRect
 *  \param[in]  line_space      space between 2 lines 
 */ 
/******************************************************************************/    
PUBLIC void LCD_DisplayStringEllipsisByAlign(
                                GUI_LCD_DEV_INFO const *dev_info_ptr,   // block information,
                                GUI_RECT_T              rect,           //the fixed display area
                                wchar*                  str_ptr,        //string to display
                                uint16                  len,            //length of string to display
                                uint16                  char_space,     //space between two chars.
                                GUI_COLOR_T             font_color,     //string color
                                GUI_FONT_T              font,           //font type
                                //BOOLEAN               is_ucs2,
                                GUI_ALIGN_E             align,
                                uint8                   line_space       //space between 2 lines
                                );

/******************************************************************************/
/*
 *  \param[in]  text
 *  \param[in]  length
 *  \param[in]  font_type            font type
 *  \param[in]  width
 *  \param[out]  char_num 
 *  \param[out]  pixel_num
 */ 
/******************************************************************************/    
PUBLIC BOOLEAN GUI_AnalysisNextLine(
                                const wchar*  text,
                                uint16 length,
                                uint32 state,
                                GUI_FONT_T font_type,
                                uint16 width,
                                uint16* char_num,
                                uint16* pixel_num
                                );

PUBLIC void GUI_SetLogicFont(int id);

PUBLIC int  GUI_GetLogicFont(void);

/*****************************************************************************/
//  Description : 计算指定的文本显示的宽度、高度和行数
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Text_Info_Ex(
								const SeDeviceContext* dc, 
								const uint16 * text, //[输入] 指向一个字符串缓冲区的指针，存放了需要处理的文本。不能为空指针。
								int  length, //[输入] text指向的字符串的长度。如果数值小于0，表示该字符串是一个以0结尾的字符串。字符串长度的最大值不能超过SE_MAX_TEXT_LENGTH。
								int dir, // 1 for rtl, 0 for ltr
								int * width, //[输出] 指向一个整数的指针，用于返回文本显示的宽度。可以是空指针，表示不需要返回该信息。
								int * height, //[输出] 指向一个整数的指针，用于返回文本显示的高度。可以是空指针，表示不需要返回该信息。
								int * lines) ;

#ifdef __cplusplus
}
#endif

#endif /* GUI_SCRIPTEASY_H */
