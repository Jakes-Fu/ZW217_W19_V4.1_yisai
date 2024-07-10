/*! @file guibutton.h
@brief ���ļ�������button�ؼ��Ķ���ӿں��������� 
@author Jassmine
@version 1.0
@date  04/26/2004        
@remarks Copyright:  2003 Spreadtrum, Incoporated. All Rights Reserved
@details 
@note 11/2009      Jassmine              Creat
*******************************************************************************/

/*! @page page_Button ��ť(Button)

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

@section sec_button_function �ؼ�����

-# Button��Ŧ�ۻ��Ϊ��ť�ؼ���Button�ؼ���ҪӦ���ڴ��������ֻ������У������ḻ���ڽ������ʾЧ���������ʵ��Button��ʱ�򣬿��Ե���Ӧ�õĽӿڴ���Ӧ�����ݡ�
-# Button�ؼ����в�ͬ�ı߿���������Ρ�3D���Ρ�Բ�Ǿ��λ�û�б߿�ȡ�
-# Button�ؼ���ʾ���ı���Ϣͬ�����в�ͬ�ķ���û������������ִ�С���ּ�ࡢ�ı���ɫ����Ϣ��
-# Button���зḻ�ı���Ч�����û��������ñ���ͼƬ��Ҳ����ֻ���ñ�����ɫ��
-# Button���Դ����û����ݣ����統Button����ʱ���Դ��µĴ��ڻ�ִ������������
-# Button�����ʵ��ʱ������Ч����
-# ����Ч����ֻ��Ҫ����button����������Ч���ɿؼ���ʾ
-# ���µ���Ч������Ҫ����button���º͵���ʱ����ͼ��

@section sec_button_def �궨�����

- ���ڴ�����ť�ؼ���
@code
#define CREATE_BUTTON_CTRL(_BG_IMAGE_ID, _CTRL_ID_) \
        MMK_CREATE_BUTTON, _BG_IMAGE_ID, _CTRL_ID_
@endcode
    - _ BG_IMAGE _ID_ ����ͼƬID
    - _CTRL_ID_ Button�ؼ�ID

@section sec_button_create �ؼ�����

- ���������˵Button�ؼ�ʹ�������Ƚϼ򵥣����ǽ�����������Ȼ����sample_test_ctrl.c�ļ��У����ﲻ��Ϊ�������µ�ģ�顣
- ����������У��������Ŵ�������Button(Button1��Button2)������Label��Label1��Label2������Button1����ʱ��Label1��ʾ��Button1���ı�����Button2����ʱ��Label2��ʾ��Button2���ı���

@subsection ssec_button_create_add_source ��һ���������Դ

-# �ı���Դ�������str_table.xls�ļ��У���
@code
TXT_HELLO_TEST_BUTTON_TEXT  20  Button Test ����Button
@endcode
-# ͼƬ��Դ��     
    - �����ԴID�����ļ�sample_modu_def.h������´��룩��
@code
RES_ADD_STRING(TXT_HELLO_TEST_BUTTON_TEXT,"Test Button")
RES_ADD_IMG(IMAGE_TEST_BUTTON_LEFT_OFF,"\\MMI_RES_##\\IMAG\\IMAGE_TEST_BUTTON_LEFT_OFF.bmp",IMG_CMP_BMP_16_565_RLE,0)
RES_ADD_IMG(IMAGE_TEST_BUTTON_RIGHT_OFF,"\\MMI_RES_##\\IMAG\\IMAGE_TEST_BUTTON_RIGHT_OFF.bmp",IMG_CMP_BMP_16_565_RLE,0)
@endcode
-# ��Ӵ���ID�Ϳؼ�ID��
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

@subsection ssec_button_create_win_table �ڶ�������������Table

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

�ô���Table��Ԫ��������Button�ؼ�������Label�ؼ�������ؼ���Softkey�ؼ�������ID�ʹ��ڻص�������

@subsection ssec_button_create_callback ��������ʵ�ִ��ڻص�����

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
        // ����Button1����Ļص�����
  GUILABEL_SetRect(,MMI_TEST_BUTTON_LABEL1_CTRL_ID, & label1rect, FALSE);
GUILABEL_SetRect(,MMI_TEST_BUTTON_LABEL2_CTRL_ID, & label2_rect, FALSE);

GUIBUTTON_SetRect(MMI_TEST_BUTTON1_CTRL_ID��& button1_rect)��
GUIBUTTON_SetRect(MMI_TEST_BUTTON2_CTRL_ID��& button1_rect)��     
GUIBUTTON_SetCallBackFuction(MMI_TEST_BUTTON1_CTRL_ID, MSG_TP_PRESS_UP, LeftButtonCallBack);
        // ����Button2����Ļص�����
        GUIBUTTON_SetCallBackFuction(MMI_TEST_BUTTON2_CTRL_ID, MSG_TP_PRESS_UP, RightButtonCallBack);
        break;
    case MSG_GET_FOCUS:
        break;
    case MSG_LOSE_FOCUS:
        break;
case MSG_FULL_PAINT:
    // �����ڵı���
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

����button�Ļص�����ʵ�����£�
@code
// ����Label1����ʾ���ַ�����Button1��
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
// ����Label2����ʾ���ַ�����Button2��
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

@subsection ssec_button_create_func ���Ĳ���������ں���

����ʵ�ִ���Button���Դ��ڵĺ�����
@code
PUBLIC MMI_HANDLE_T MMIHELLO_CreateButtonWin(void)
{
    return MMK_CreateWin((uint32*)MMI_TEST_BUTTON_WIN_TAB, PNULL);
}
@endcode
������Hello Sprd�Ĵ������������´��룬�����Ǵ򿪴��ڵ���ڣ�
@code
case MSG_APP_2:
        MMIHELLO_CreateButtonWin();
        break;
@endcode

@subsection ssec_button_create_run ���岽������

������Դ����������ģ��������hello sprd�����а�2������Button���Դ�����ͼ 2 29��ʾ������������ô��ʣ���꣩�ֱ���һ������button����һ��ʲô�������
@image html button_test_win.jpg
ͼ 2 29 Button���Դ���

@section sec_button_function_list �ؼ��ӿڽ���

- @ref buttonGrp

@section sec_button_relationship �ӿڵ��ù�ϵ

�Ͻ������о���Button�ؼ�����õļ����ӿڣ�����Button�ؼ������úͶ�̬����һ�㶼���ڴ��ڵ�Open��Ϣ�н��еģ����ڵĵ���û���Ⱥ��ϵ��������Ƕ�̬������Button�ؼ��������ӿڶ�Ӧ���ڴ����ؼ�֮�󱻵��ã���ܺ���⣬��Ϊ�ؼ���δ����ʱ�ǲ����������ǵ����Եģ���ʵ�����ؼ�Ҳ��������

@section sec_button_app �ؼ�Ӧ��

Button�ؼ���ҪӦ���ڴ��������ֻ������У���Ϊ��Ŀǰ��֧�ְ�����Ϣ��Button�ؼ������ḻ���ڽ������ʾ�������û����ݡ���ͼ������ʱ�䴰����ʹ��button�ؼ��л�����ʱ�䣺
@image html button_app_example.jpg
ͼ 2 30 �ؼ���ʾ��ͼ

����Ӧ�ã��粦�����У�ʹ��buttonģ�¼��̲��ŵȡ�

*******************************************************************************/

/*! @addtogroup buttonGrp Button
@brief button�ؼ�
@details button�ؼ�
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
