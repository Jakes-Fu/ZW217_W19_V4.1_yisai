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
//  Description : �ı��������Ĵ�С
//  Global resource dependence : 
//   Author: apple.zhang
//  Note: ���dx��dy��Ϊ0����������Ĵ�С���ֲ���
/*****************************************************************************/
PUBLIC void GUI_Inflate_Rect(
                             SeRectangle * box, //[����/���] ָ��һ��GUI_RECT_T���ݽṹ��ָ�룬��ž�����������ꡣ����Ϊ��ָ��
                             int dx, //[����] ��������������ߺ��ұߵ���ֵ��
                                     //   ����0����ֵ�����Ӿ��εĿ�ȣ�������ߵ������С�������ұߵ���������
                                     //   С��0����ֵ����С���εĿ�ȣ�������ߵ��������󣬾����ұߵ������С��
                             int dy //[����] �������������ϱߺ��±ߵ���ֵ��
                                    //����0����ֵ�����Ӿ��εĸ߶ȣ������ϱߵ������С�������±ߵ���������
                                    //С��0����ֵ����С���εĸ߶ȣ������ϱߵ��������󣬾����±ߵ������С��
                              );
/*****************************************************************************/
//  Description : ����ָ�����ı���ʾ�Ŀ�ȡ��߶Ⱥ�����
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Text_Info(
                              const SeDeviceContext* dc, 
                              const uint16 * text, //[����] ָ��һ���ַ�����������ָ�룬�������Ҫ������ı�������Ϊ��ָ�롣
                              int  length, //[����] textָ����ַ����ĳ��ȡ������ֵС��0����ʾ���ַ�����һ����0��β���ַ������ַ������ȵ����ֵ���ܳ���SE_MAX_TEXT_LENGTH��
                              int * width, //[���] ָ��һ��������ָ�룬���ڷ����ı���ʾ�Ŀ�ȡ������ǿ�ָ�룬��ʾ����Ҫ���ظ���Ϣ��
                              int * height, //[���] ָ��һ��������ָ�룬���ڷ����ı���ʾ�ĸ߶ȡ������ǿ�ָ�룬��ʾ����Ҫ���ظ���Ϣ��
                              int * lines); //[���] ָ��һ��������ָ�룬���ڷ����ı���ʾ�������������ǿ�ָ�룬��ʾ����Ҫ���ظ���Ϣ��

/*****************************************************************************/
//  Description : �õ�����ʾ�豸���ı���ʾ��ص���Ϣ
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void* GUI_Get_DeviceContent(void);
/*****************************************************************************/
//  Description : ��������ʾ�豸���ı���ʾ��ص���Ϣ
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Set_DeviceContent(void* dc);
/*****************************************************************************/
//  Description : ��ָ��λ����ʾһ���ı�
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Draw_text(
                          const SeDeviceContext* dc, 
                          int  x,//[����] �ı���ʾ������
                          int y,//[����] �ı���ʾ������
                          const uint16* text, //[����] ָ��һ���ַ�����������ָ�룬�������Ҫ��ʾ���ı�������Ϊ��ָ��
                          int   length) ;//[����] textָ����ַ����ĳ��ȡ������ֵС��0����ʾ���ַ�����һ����0��β���ַ������ַ������ȵ����ֵ���ܳ���SE_MAX_TEXT_LENGTH��
                          
PUBLIC int GUI_Draw_text_Ex(                          
                          const SeDeviceContext* dc, 
                          int  x,//[����] �ı���ʾ������
                          int y,//[����] �ı���ʾ������
                          const uint16* text, //[����] ָ��һ���ַ�����������ָ�룬�������Ҫ��ʾ���ı�������Ϊ��ָ��
                          int   length, //[����] textָ����ַ����ĳ��ȡ������ֵС��0����ʾ���ַ�����һ����0��β���ַ������ַ������ȵ����ֵ���ܳ���SE_MAX_TEXT_LENGTH��
                          int dir);
                          
/*****************************************************************************/
//  Description : ����������һ���ı��༭������
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Create(void);
/*****************************************************************************/
//  Description : �õ���ʼ���ı��༭������Ҫ�������Ϣ
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void* GUI_Get_Editor_Init_Info(void);
/*****************************************************************************/
//  Description : ���� ��ʼ���ı��༭������Ҫ�������Ϣ
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Set_Editor_Init_Info(void* init);
/*****************************************************************************/
//  Description :  �õ��´������ı��༭�������ָ��
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void* GUI_Get_Editor(void);
/*****************************************************************************/
//  Description : ���� �´������ı��༭�������ָ��
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Set_Editor(void* editor);
/*****************************************************************************/
//  Description : ɾ��һ���ı��༭�����󣬲��ͷ������Դ
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_destroy( void);
/*****************************************************************************/
//  Description : ���һ���ı��༭�������ɵ��ı����ݵ���󳤶ȡ�
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Max_Buffer( int * max_buffer); //[���] ָ��һ��������ָ�룬���ڷ��ر༭�������ɵ��ı����ݵ���󳤶ȡ�����Ϊ��ָ�롣
/*****************************************************************************/
//  Description :���һ���ı��༭�����ı���ʾ�߾ࡣ
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Editor_Margin(SeRectangle*     margin); //[���] ָ��һ��GUI_RECT_T���ݽṹ��ָ�룬���ڷ��ر༭�����ı���ʾ�߾ࡣ����Ϊ��ָ�롣
/*****************************************************************************/
//  Description :���һ���ı��༭������ĵ�ǰλ�á�
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Editor_Pos(SeRectangle*     pos);
/*****************************************************************************/
//  Description : �����ı��༭���������λ�á�
/*****************************************************************************/
PUBLIC int GUI_Set_Editor_Pos(const SeRectangle* pos);
PUBLIC int GUI_Editor_Get_Caret_Text_Index(void);

PUBLIC void GUI_Editor_Set_Caret_Text_Index(int text_index);

PUBLIC void GUI_Editor_Delete_Text_Before_Caret(void);
/*****************************************************************************/
//  Description :  �õ������ı��༭������ʾ��ɫ
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void* GUI_Get_Editor_Palette_Info(void);
/*****************************************************************************/
//  Description : ���������ı��༭������ʾ��ɫ
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Set_Editor_Palette_Info(void* palette);
/*****************************************************************************/
//  Description :���һ���ı��༭������ʾ��ɫ��
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Editor_Palette(void);
/*****************************************************************************/
//  Description :������ʾһ���ı��༭����
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Paint_Editor(void);
/*****************************************************************************/
//  Description :��������ֹһ���ı��༭��������ʾ��
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Lock(void);
/*****************************************************************************/
//  Description :�����һ���ı��༭����ʾ���µ�������
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_UNLock(void);
/*****************************************************************************/
//  Description :��ȡһ���ı��༭����ǰ�ı������ݺͳ��ȡ�
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Editor_Text(
                                const uint16** text, //[���] ָ��һ���ַ���������ָ���ָ�룬���ڷ���һ��ָ��༭���ڲ��ı��ַ�����������ָ�룬�û������ڴ���˱༭����ǰ���ı����ݡ������ǿ�ָ�룬��ʾ����Ҫ���ظ���Ϣ��
                                int*           length, //[���] ָ��һ��������ָ�룬���ڷ��ر༭����ǰ�ı����ݵĳ��ȡ������ǿ�ָ�룬��ʾ����Ҫ���ظ���Ϣ��
                                int*           num_chars) ;
/*****************************************************************************/
//  Description :���һ���ı��༭�����ı���
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Clear_Content(void);
/*****************************************************************************/
//  Description :��һ���ı��༭���ĵ�ǰ���λ�ý���һ���˸�ɾ��������
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_BackSpace(void);
/*****************************************************************************/
//  Description :��һ���ı��༭���ĵ�ǰ���λ�ò���ָ�����ı���
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Insert_Text(
                                const uint16* text,
                                int         length);
/*****************************************************************************/
//  Description :�л�һ���ı��༭���ڹ�����ʾ״̬��
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Switch_Caret(void);
/*****************************************************************************/
//  Description :��һ���ı��༭���ڵĹ�������ƶ�һ�Ρ�
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Move_Caret_Left(void);
/*****************************************************************************/
//  Description :��һ���ı��༭���ڵĹ�������ƶ�һ�Ρ�
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Move_Caret_Right(void);
/*****************************************************************************/
//  Description :��һ���ı��༭���ڵĹ�������ƶ�һ�Ρ�
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Move_Caret_Up(void);
/*****************************************************************************/
//  Description :��һ���ı��༭���ڵĹ�������ƶ�һ�Ρ�
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Move_Caret_Down(void);
/*****************************************************************************/
//  Description :��һ���ı��༭���ڵĹ�������ƶ�һҳ��
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Move_Caret_PageUp(void);
/*****************************************************************************/
//  Description :��һ���ı��༭���ڵĹ�������ƶ�һҳ��
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Editor_Move_Caret_PageDown(void);
/*****************************************************************************/
//  Description :ɾ��һ���ı��༭���ڹ�굱ǰλ�õ��ַ���
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Delete_Editor_Caret_Char(void);
/*****************************************************************************/
//  Description :���һ���ı��༭�������뷨��ǰ�ı༭���ݡ�
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Clear_Editor_IME(void);
/*****************************************************************************/
//  Description :����һ���ı��༭�������뷨��ǰ�ı༭������
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_End_Editor_IME(void);
/*****************************************************************************/
//  Description :��һ���ı��༭���ĵ�ǰ���λ�ò���ָ�����ı�����Ϊ���뷨�ı༭���ݡ�
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Set_Editor_IME(                           
                            const uint16* text,//[����] ָ��Ҫ������ı����ݺͳ��ȡ�text����Ϊ��ָ�롣���lengthС��0����ʾtextָ����һ����0��β���ַ�����
                            int length);
/*****************************************************************************/
//  Description :����һ���ı��༭����ǰ���뷨�ı༭���ݡ�
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC void GUI_Get_Editor_IME(                          
                            const uint16** text,//[���] ָ��һ���ַ���������ָ���ָ�룬���ڷ���һ��ָ��༭���ڲ��ı��ַ�����������ָ�룬�û������ڴ���˱༭����ǰ���뷨�ı༭���ݡ������ǿ�ָ�룬��ʾ����Ҫ���ظ���Ϣ��
                            int *length);
/*****************************************************************************/
//  Description :����һ��ָ����red,green,blue��ɫ��������ɫֵ��
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC uint32 GUI_Color_RGB(   
                            uint8 red,
                            uint8 green,
                            uint8 blue);
/*****************************************************************************/
//  Description :���һ���༭���Ĺ�����Ϣ
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Set_Editor_Scroll_Info(void);
/*****************************************************************************/
//  Description :����һ���༭���Ĺ�����Ϣ
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC SeScrollInfo GUI_Get_Editor_Scroll_Info(void);
/*****************************************************************************/
//  Description :��һ���ı��༭���ڵĹ���ƶ����������������λ��
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
//  Description : ����ָ�����ı���ʾ�Ŀ�ȡ��߶Ⱥ�����
//  Global resource dependence : 
//   Author: apple.zhang
//  Note:
/*****************************************************************************/
PUBLIC int GUI_Get_Text_Info_Ex(
								const SeDeviceContext* dc, 
								const uint16 * text, //[����] ָ��һ���ַ�����������ָ�룬�������Ҫ������ı�������Ϊ��ָ�롣
								int  length, //[����] textָ����ַ����ĳ��ȡ������ֵС��0����ʾ���ַ�����һ����0��β���ַ������ַ������ȵ����ֵ���ܳ���SE_MAX_TEXT_LENGTH��
								int dir, // 1 for rtl, 0 for ltr
								int * width, //[���] ָ��һ��������ָ�룬���ڷ����ı���ʾ�Ŀ�ȡ������ǿ�ָ�룬��ʾ����Ҫ���ظ���Ϣ��
								int * height, //[���] ָ��һ��������ָ�룬���ڷ����ı���ʾ�ĸ߶ȡ������ǿ�ָ�룬��ʾ����Ҫ���ظ���Ϣ��
								int * lines) ;

#ifdef __cplusplus
}
#endif

#endif /* GUI_SCRIPTEASY_H */
