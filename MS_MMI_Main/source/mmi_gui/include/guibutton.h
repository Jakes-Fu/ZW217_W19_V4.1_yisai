/*! @file guibutton.h
@brief 此文件介绍了button控件的对外接口和数据类型 
@author Jassmine
@version 1.0
@date  04/26/2004        
@remarks Copyright:  2003 Spreadtrum, Incoporated. All Rights Reserved
@details 
@note 11/2009      Jassmine              Creat
*******************************************************************************/

/*! @page page_Button 按钮(Button)

-# @ref sec_button_function
-# @ref sec_button_def
-# @ref sec_button_create
    -# @ref ssec_button_create_add_source
    -# @ref ssec_button_create_win_table
    -# @ref ssec_button_create_callback
    -# @ref ssec_button_create_func
    -# @ref ssec_button_create_run
-# @ref sec_button_function_list
-# @ref sec_button_relationship
-# @ref sec_button_app

@section sec_button_function 控件功能

-# Button即纽扣或称为按钮控件，Button控件主要应用在触摸屏的手机窗口中，用来丰富窗口界面的显示效果。当触笔点击Button的时候，可以调用应用的接口处理应用数据。
-# Button控件具有不同的边框风格，例如矩形、3D矩形、圆角矩形或没有边框等。
-# Button控件显示的文本信息同样具有不同的风格，用户可以设置文字大小、字间距、文本颜色等信息。
-# Button具有丰富的背景效果，用户可以设置背景图片，也可以只设置背景颜色。
-# Button可以处理用户数据，例如当Button按下时可以打开新的窗口或执行其他操作。
-# Button被触笔点击时有两种效果：
-# 光晕效果，只需要设置button背景，光晕效果由控件显示
-# 按下弹起效果，需要设置button按下和弹起时两种图标

@section sec_button_def 宏定义解析

- 用于创建按钮控件。
@code
#define CREATE_BUTTON_CTRL(_BG_IMAGE_ID, _CTRL_ID_) \
        MMK_CREATE_BUTTON, _BG_IMAGE_ID, _CTRL_ID_
@endcode
    - _ BG_IMAGE _ID_ 背景图片ID
    - _CTRL_ID_ Button控件ID

@section sec_button_create 控件创建

- 由于相对来说Button控件使用起来比较简单，我们将事例代码仍然放在sample_test_ctrl.c文件中，这里不再为它创建新的模块。
- 在这个例子中，我们试着创建两个Button(Button1和Button2)和两个Label（Label1和Label2），当Button1按下时，Label1显示“Button1”文本；当Button2按下时，Label2显示“Button2”文本。

@subsection ssec_button_create_add_source 第一步：添加资源

-# 文本资源（添加至str_table.xls文件中）：
@code
TXT_HELLO_TEST_BUTTON_TEXT  20  Button Test 测试Button
@endcode
-# 图片资源：     
    - 添加资源ID（在文件sample_modu_def.h添加如下代码）：
@code
RES_ADD_STRING(TXT_HELLO_TEST_BUTTON_TEXT,"Test Button")
RES_ADD_IMG(IMAGE_TEST_BUTTON_LEFT_OFF,"\\MMI_RES_##\\IMAG\\IMAGE_TEST_BUTTON_LEFT_OFF.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(IMAGE_TEST_BUTTON_RIGHT_OFF,"\\MMI_RES_##\\IMAG\\IMAGE_TEST_BUTTON_RIGHT_OFF.bmp",IMG_CMP_BMP_16_565_RLE,0)
@endcode
-# 添加窗口ID和控件ID：
@code
// sample_id.h
typedef enum
{
    MMIHELLO_WIN_ID_START = (MMI_MODULE_SAMPLE << 16),
        
    MMI_HELLO_SPRD_WIN_ID,
    MMI_TEST_MENU_WIN_ID,
    MMI_TEST_LIST_WIN_ID,
    MMI_TEST_TEXTBOX_WIN_ID,
    MMI_TEST_BUTTON_WIN_ID,

    MMIHELLO_MAX_WIN_ID
}MMIHELLO_WINDOW_ID_E;

#undef WIN_ID_DEF

// control ID
typedef enum
{
    MMIHELLO_CTRL_ID_ID_START = MMIHELLO_MAX_WIN_ID,
    MMI_TEST_MENU_CTRL_ID,
    MMI_TEST_LIST_CTRL_ID,
    MMI_TEST_TEXTBOX_CTRL_ID,
    MMI_TEST_BUTTON1_CTRL_ID,
    MMI_TEST_BUTTON2_CTRL_ID,
    MMI_TEST_BUTTON_LABEL1_CTRL_ID,
    MMI_TEST_BUTTON_LABEL2_CTRL_ID,
    MMIHELLO_MAX_CTRL_ID
}MMIHELLO_CONTROL_ID_E;
@endcode

@subsection ssec_button_create_win_table 第二步：创建窗口Table

@code
WINDOW_TABLE( MMI_TEST_BUTTON_WIN_TAB ) = 
{
    WIN_ID( MMI_TEST_BUTTON_WIN_ID ),
    WIN_FUNC((uint32) HandleTestButtonWinMsg ),    
    WIN_TITLE(TXT_HELLO_TEST_BUTTON_TEXT),
    
    CREATE_LABEL_CTRL(GUILABEL_LEFT,MMI_TEST_BUTTON_LABEL1_CTRL_ID),
    CREATE_LABEL_CTRL(GUILABEL_LEFT,MMI_TEST_BUTTON_LABEL2_CTRL_ID),
    
    CREATE_BUTTON_CTRL(IMAGE_TEST_BUTTON_LEFT_OFF,  MMI_TEST_BUTTON1_CTRL_ID),
    CREATE_BUTTON_CTRL(IMAGE_TEST_BUTTON_RIGHT_OFF, MMI_TEST_BUTTON2_CTRL_ID),
    
    WIN_SOFTKEY(STXT_OK, TXT_NULL, STXT_RETURN),
    END_WIN
};
@endcode

该窗口Table的元素有两个Button控件、两个Label控件、标题控件、Softkey控件、窗口ID和窗口回调函数。

@subsection ssec_button_create_callback 第三步：实现窗口回调函数

@code
LOCAL MMI_RESULT_E HandleTestButtonWinMsg(
                               MMI_WIN_ID_T     win_id, 
                               MMI_MESSAGE_ID_E msg_id, 
                               DPARAM           param
                               )
{
    MMI_RESULT_E        recode = MMI_RESULT_TRUE;
    GUI_LCD_DEV_INFO    lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    MMI_STRING_T        string = {0};
    GUI_RECT_T          rect = {0, 0, 239, 319};
label1_rect = {0, MMI_TITLE_HEIGHT, 239, MMI_TITLE_HEIGHT + 25};
label2_rect = {0, MMI_TITLE_HEIGHT + 30, 239, MMI_TITLE_HEIGHT + 55};
button1_rect = {0, MMI_TITLE_HEIGHT + 60,120, MMI_TITLE_HEIGHT + 60 + 23};
button2_rect = {0, MMI_TITLE_HEIGHT + 60 + 23 + 5,120, MMI_TITLE_HEIGHT + 60 + 23 + 5 + 23};
    
    switch(msg_id)
    {
case MSG_OPEN_WINDOW:
        // 设置Button1处理的回调函数
  GUILABEL_SetRect(,MMI_TEST_BUTTON_LABEL1_CTRL_ID, & label1rect, FALSE);
GUILABEL_SetRect(,MMI_TEST_BUTTON_LABEL2_CTRL_ID, & label2_rect, FALSE);

GUIBUTTON_SetRect(MMI_TEST_BUTTON1_CTRL_ID，& button1_rect)；
GUIBUTTON_SetRect(MMI_TEST_BUTTON2_CTRL_ID，& button1_rect)；     
GUIBUTTON_SetCallBackFuction(MMI_TEST_BUTTON1_CTRL_ID, MSG_TP_PRESS_UP, LeftButtonCallBack);
        // 设置Button2处理的回调函数
        GUIBUTTON_SetCallBackFuction(MMI_TEST_BUTTON2_CTRL_ID, MSG_TP_PRESS_UP, RightButtonCallBack);
        break;
    case MSG_GET_FOCUS:
        break;
    case MSG_LOSE_FOCUS:
        break;
case MSG_FULL_PAINT:
    // 画窗口的背景
        GUIRES_DisplayImg(PNULL, &rect, &rect, win_id, IMAGE_COMMON_BG, &lcd_dev_info);
        break;
    case MSG_APP_OK:
    case MSG_APP_CANCEL:
        MMK_CloseWin(win_id);
        break;
    case MSG_CLOSE_WINDOW:
        break;
    default:
        recode = MMI_RESULT_FALSE;
        break;
    }
    return recode;
}
@endcode

其中button的回调函数实现如下：
@code
// 设置Label1上显示的字符串“Button1”
LOCAL MMI_RESULT_E LeftButtonCallBack(void)
{
    MMI_RESULT_E result  = MMI_RESULT_TRUE;
    wchar w_text[] = {'B', 'u', 't', 't', 'o', 'n', '1', 0};
    uint16 len = MMIAPICOM_Wstrlen(w_text);
    MMI_STRING_T        string = {0};

    string.wstr_len = len;
    string.wstr_ptr = w_text;

    GUILABEL_SetText(MMI_TEST_BUTTON_LABEL1_CTRL_ID, &string, TRUE);
    return result;
}
// 设置Label2上显示的字符串“Button2”
LOCAL MMI_RESULT_E RightButtonCallBack(void)
{
    MMI_RESULT_E result  = MMI_RESULT_TRUE;
    wchar w_text[] = {'B', 'u', 't', 't', 'o', 'n', '2', 0};
    uint16 len = MMIAPICOM_Wstrlen(w_text);
    MMI_STRING_T        string = {0};
    string.wstr_len = len;
    string.wstr_ptr = w_text;

    GUILABEL_SetText(MMI_TEST_BUTTON_LABEL2_CTRL_ID, &string, TRUE);
    return result;
}
@endcode

@subsection ssec_button_create_func 第四步：增加入口函数

首先实现创建Button测试窗口的函数；
@code
PUBLIC MMI_HANDLE_T MMIHELLO_CreateButtonWin(void)
{
    return MMK_CreateWin((uint32*)MMI_TEST_BUTTON_WIN_TAB, PNULL);
}
@endcode
我们在Hello Sprd的窗口中增加如下代码，这里是打开窗口的入口：
@code
case MSG_APP_2:
        MMIHELLO_CreateButtonWin();
        break;
@endcode

@subsection ssec_button_create_run 第五步：运行

编译资源，编译运行模拟器，在hello sprd窗口中按2键，打开Button测试窗口如图 2 29所示，你可以试着用触笔（鼠标）分别点击一下两个button，看一下什么结果？：
@image html button_test_win.jpg
图 2 29 Button测试窗口

@section sec_button_function_list 关键接口介绍

- @ref buttonGrp

@section sec_button_relationship 接口调用关系

上节我们列举了Button控件中最常用的几个接口，对于Button控件的设置和动态创建一般都是在窗口的Open消息中进行的，窗口的调用没有先后关系，但如果是动态创建的Button控件，其它接口都应该在创建控件之后被调用，这很好理解，因为控件尚未创建时是不能设置它们的属性的，其实其它控件也是这样。

@section sec_button_app 控件应用

Button控件主要应用在触摸屏的手机窗口中，因为它目前不支持按键消息，Button控件用来丰富窗口界面的显示，处理用户数据。下图在世界时间窗口中使用button控件切换城市时间：
@image html button_app_example.jpg
图 2 30 控件显示例图

其他应用，如拨号盘中，使用button模仿键盘拨号等。

*******************************************************************************/

/*! @addtogroup buttonGrp Button
@brief button控件
@details button控件
@{
*******************************************************************************/

#ifndef _GUIBUTTON_H_
#define _GUIBUTTON_H_ 

/*---------------------------------------------------------------------------
                          Include Files                                    
 --------------------------------------------------------------------------*/
#include "guictrl.h"
#include "guifont.h"
#include "ctrlbutton_export.h"
#include "ctrlcheckbox_export.h"

/*---------------------------------------------------------------------------
                         Compiler Flag                                   
 --------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*--------------------------------------------------------------------------
                          MACRO DEFINITION                                 
 --------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                          TYPE AND STRUCT                                  
 --------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                          FUNCTION DEFINITION                              
 --------------------------------------------------------------------------*/
#define GUIBUTTON_CreateDynamic             CTRLBUTTON_CreateDynamic     
#define GUIBUTTON_SetStyle                  CTRLBUTTON_SetStyle          
#define GUIBUTTON_SetBg                     CTRLBUTTON_SetBg             
#define GUIBUTTON_SetFg                     CTRLBUTTON_SetFg             
#define GUIBUTTON_SetPressedBg              CTRLBUTTON_SetPressedBg      
#define GUIBUTTON_SetPressedFg              CTRLBUTTON_SetPressedFg      
#define GUIBUTTON_SetGrayed                 CTRLBUTTON_SetGrayed         
#define GUIBUTTON_SetGrayedFg               CTRLBUTTON_SetGrayedFg       
#define GUIBUTTON_SetTextId                 CTRLBUTTON_SetTextId         
#define GUIBUTTON_SetText                   CTRLBUTTON_SetText           
#define GUIBUTTON_SetTextAlign              CTRLBUTTON_SetTextAlign      
#define GUIBUTTON_SetCallBackFunc           CTRLBUTTON_SetCallBackFunc   
#define GUIBUTTON_SetCallBackFuncExt        CTRLBUTTON_SetCallBackFuncExt
#define GUIBUTTON_SetHandleLong             CTRLBUTTON_SetHandleLong     
#define GUIBUTTON_SetTransparent            CTRLBUTTON_SetTransparent    
#define GUIBUTTON_SetRunSheen               CTRLBUTTON_SetRunSheen       
#define GUIBUTTON_SetRect                   CTRLBUTTON_SetRect           
#define GUIBUTTON_PermitBorder              CTRLBUTTON_PermitBorder      
#define GUIBUTTON_SetFont                   CTRLBUTTON_SetFont           
#define GUIBUTTON_SetVisible                CTRLBUTTON_SetVisible        
#define GUIBUTTON_SetOwnerDraw              CTRLBUTTON_SetOwnerDraw      
#define GUIBUTTON_Update                    CTRLBUTTON_Update            
#define GUIBUTTON_UpdateText                CTRLBUTTON_UpdateText        
#define GUIBUTTON_SetForbidPaint            CTRLBUTTON_SetForbidPaint    
#define GUIBUTTON_SetBorder                 CTRLBUTTON_SetBorder         
#define GUIBUTTON_SetPopAnimInfo            CTRLBUTTON_SetPopAnimInfo    

#define GUIBUTTON_GetSelect                 CTRLCHECKBOX_GetSelect         
#define GUIBUTTON_SetSelect                 CTRLCHECKBOX_SetSelect         

#define GUIBUTTON_SetFontColor              CTRLBUTTON_SetFontColor      
/*****************************************************************************/
//  Description : set button type
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN GUIBUTTON_SetType(
                                 MMI_CTRL_ID_T      ctrl_id,    //in
                                 GUIBUTTON_TYPE_E   type        //in:
                                 );

/*--------------------------------------------------------------------------
                          Compiler Flag                                    
 --------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
