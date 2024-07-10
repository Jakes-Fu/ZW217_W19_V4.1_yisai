/*! @file ctrllabel_export.h
@brief 
���ļ�������Label�ؼ��Ķ���ӿں��������� 
@author Jassmine
@version 1.0
@date 04/26/2004
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@details 
@note 11/2009      Jassmine              Creat
*******************************************************************************/

/*! @page page_Label ��ǩ(Label)

-# @ref sec_label_function
-# @ref sec_label_usage
    -# @ref ssec_label_usage_def
    -# @ref ssec_label_usage_create
    -# @ref ssec_label_usage_function_list
-# @ref sec_label_app

@section sec_label_function �ؼ��Ĺ���

Label�ؼ���һ���ı�����ؼ����ܹ��������ֵ���ʾλ�á���ʾ���򡢴�С������ȷ�������ͨ��LCD�ӿ�������û���Label�ؼ��ܹ���ʾ�����ı�����ʾ���ı����ܱ�ֱ�ӱ༭�����ı��Ŀ�ȳ����ؼ�����ʾ����ʱ�����Թ�����ʾ�ı����ݡ�Label�ؼ�������ʾ�ı�֮�⣬���������ı��������ʾһ��ͼ�꣨ICON����Label�ڽ���״̬ʱ���Թ�����ʾ���ǽ���ʱ��ʾʡ�Ե㣬��Ȼ�û������������۽���ͷǽ��㶼���й�����

@section sec_label_usage �ؼ�ʹ��

@subsection ssec_label_usage_def �궨�����

@code
#define CREATE_LABEL_CTRL(_ALIGN_, _CTRL_ID_) \
        MMK_CREATE_LABEL, _ALIGN_, _CTRL_ID_
@endcode

- ���ڴ�����ǩ�ؼ���
    - _ALIGN_ �ı����뷽ʽ
    - _CTRL_ID_ �ؼ�ID

@subsection ssec_label_usage_create �ؼ�����

���б�ؼ���ͬ������Label����ǩ���ؼ�ʱ�����ÿؼ���ID���ؼ���ʾ�ı��Ķ��뷽ʽ�Ϳ����ˣ�������ʾ��������Ҫ�ڴ��ڵ�Open��Ϣ�����õģ����֧�ֺ�����������Ҫ����both rect�����磺

������lab_rect {10, 120, 200, 150}��ʾһ��Label�ؼ�����ô��Ҫ�ڴ���Table�����ӣ�
@code
CREATE_LABEL_CTRL(GUILABEL_LEFT, ctrl_id),
@endcode
Ȼ���ڴ���Open��Ϣ�����ӣ�
@code
GUILABEL_SetRect(ctrl_id, &lab_rect, FALSE); 
@endcode
@note FALSE ��ʾ��������ˢ�¡�

@subsection ssec_label_usage_function_list �ӿڽ���

�ڴ��ڱ�����CREATE_LABEL_CTRL�ֻ꣬��˵�������Ѿ������˿ؼ���ʵ��������������ʾ�κ����ݣ���Ϊ�û���û�ж�������ʲô��Ϣ��

-# @ref GUILABEL_SetText
    - ����Label�ؼ����ı�һ��������ڴ��ڵ�Open��Ϣ����ʱ���þͿ����ˣ���ʱ����Ҫ����ˢ�£���Ϊ���滹����յ�Paint��Ϣ���������ˢ���ˣ���ô���ܻᵼ����Ļˢ�²�ͬ������������������
    - Ȼ����ʵ�ʵ�Ӧ���У��кܶ��������Ҫ��ʱ����Label����ʾ���ݵģ����簴ĳһ��������Label���ݣ����ʱ����Ҫ��������Label��ʾ���ı�����ôҲ��Ҫ��ʱ��ˢ���ˣ����ʱ�򲻻��������������ԭ���ǵ�ǰ���ڻ����ڽ���״̬��
    - ��ʵ����
@code
MMI_STRING_T    item_str = {0};
MMI_GetLabelTextByLang(TXT_CL_TIMER_LAST_CALL,&item_str);
GUILABEL_SetText(ctrl_id,&item_str,FALSE);
@endcode
    - ����MMI_GetLabelTextByLang�ӿ���������ȡ�ַ�����ԴTXT_CL_TIMER_LAST_CALL���ı����ݵġ�
    - ��һ�����������Ҫ��ʾһ���Զ�����ַ������硰hello SPRD������ô����ֱ�����ã�
@code
MMI_STRING_T    item_str = {0};
wchar w_text[] = {'F', 'A', 'L', 'S', 'E', 0};
item_str.wstr_ptr = w_text;
item_str.wstr_len = 6;
GUILABEL_SetText(ctrl_id,&item_str,FALSE);
@code
-# @ref GUILABEL_SetTextById
    - ��ʵ����
@code
GUILABEL_SetTextById(ctrl_id,TXT_CL_TIMER_LAST_CALL, FALSE);
@endcode
-# @ref GUILABEL_SetBackgroundColor
    - Label�ı���֧����ɫ��ͼƬ����������ͼƬ���������ȼ����ߣ�Ҳ����˵һ���Ѿ�������ͼƬ��������ôLabel�ؼ���������ʾͼƬ�����Ա���ɫ���������ڼ���û������Label�ı���ͼƬ��
    - Label�ؼ�Ĭ�ϵı�����ɫΪMMI_TRANSPARENCE_COLOR����ɫֵ��0x0��Ϊ��ɫ�����ڴ����ؼ���ʱ��ͨ��MMITHEME_GetLabelStyle��ȡ�ġ���ƽ̨�У���ɫ����ʾʱ�Ǳ�͸����ɫ�ʣ����仰˵������û�û����������Label�ؼ��ı���ɫ�Ļ�����ô�ؼ���ֱ����ʾ�ڴ��ڵı����ϣ���ע�⣬�������û�����ñ���ͼƬ��
    - Label�ؼ���ʾ��ʱ��������ֱ�����ɫ��Ĭ����ɫ��ͬ����ô�ᷢ��Full paint��Ϣ�����ڣ��ô���ͳһˢ�±�������ΪLabel���ı���ֱ����ʾ�ڴ��ڵı����ϵģ���Ҳ�ᵼ�´�������ˢ����һ�Ρ��������ɫ��ͬ����ô����Ҫˢ�´��ڵı�����
    - Label�ǿ����ṩ��������ʾ��ʽ�ģ����ַ����Ŀ�ȳ����˿ؼ�����ʾ�����ʱ��Label�ؼ���������һ��Timer��ʵ�ֹ�����ʾ�����ʱ�����û�����ñ�����ɫ����ôÿ��Timer������ʱ��ͻ�����ˢ��һ�δ��ڡ��ںܶ�����£�����ˢ�´��ڻᵼ�³����Ч�ʽ��ͣ�����ҽ�����������Ҫ��Label���ַ�����ȳ���Label������������볬��Label��������ô�����ú�͸��ɫ����ͬ�ı���ɫ�����ס����ʱû����ȫ�����İ취��
    - ��Ȼ������㲻��Ҫ���ǳ����Ч�ʣ�ȷʵϣ����ʾ�ڴ��ڱ����ϵ�Label�ؼ�������ʾ�Ļ�����ô��ǧ��Ҫ��SetText�Ľӿڷ��ڴ��ڵ�Full pain��Ϣ�������ã���Ϊÿ��ˢ��ʱ��Ҫ�������ã�����������֮��ͻ��ͷ��ʼ��ʾ������Ȼ���ܴﵽ������ʾ��Ч����������˵��õ�����Label���ݵķ�ʽ���ڴ���Open��Ϣ��ʱ��
    - Label��һ���Ƚϼ򵥵Ŀؼ����������ڱ��������⣬�������ò�Ƹ�������࣬��ʵ��Ӧ�ù����У������ʹ���˼��Σ���ȻҲ�����������Ĺ����ˡ�������Ҫ���Ҽ���һ�£�����ؼ���û�а���ʵ�ֵĸ�����ʵ�ã����ǽ����Ժ�Ŀ��������о����޸ġ�
    - ��ʵ�������ÿؼ��ı���Ϊ��ɫ����ʾЧ����ͼ 2 20��
@code
GUILABEL_SetBackgroundColor(ctrl_id,0xFFFF);
@endcode
-# GUILABEL_SetBackgroundImage
    - Label�ؼ���Ĭ���������û�б���ͼƬ�ģ�ֻ��Ĭ�ϵı�����ɫ����һ���ڽ������ñ�����ɫ�ӿ�ʱ�Ѿ�����������
���û���Ҫ��ʾ����ͼƬ��ʱ����ô�ڿؼ���ʾ֮ǰ���øýӿڣ���ν�ڿؼ���ʾ֮ǰ���ã�Ҳ����˵���ڿؼ����յ�ctrl paint��Ϣ֮ǰ���������ǽ������ڵ���Ϣ����ʱ���������Ϣ�����̡��㽫�ῴ������������ʱOpen��Ϣ����paint��Ϣ֮ǰ���͵ģ������open��Ϣ��������Ϊ���գ���Ҳ��һ��һ�����ݵİ취����ΪOpen��Ϣֻ��һ�Σ����ú�֮���Ժ�Դ��ڵ��κβ����������������ÿؼ��ı����ˡ���ʵ��������Label�ؼ������������ؼ�Ҳһ������������ǰ�Ľ������б�ؼ�Append�б����ʱ��Ҳ�Ƿ���Open��Ϣ����ɵġ�
    - ��Ȼ���ؼ��ı������ǲ��ܸı�ģ��������Ҫ�ڲ������ڵĹ����У����簴ĳһ�����ı�ؼ��ı�������ô�ڴ�����ʱ�������������������ݡ�
    - ��ʵ����
@code
GUILABEL_SetBackgroundImage(ctrl_id, img_id);
@endcode
-# GUILABEL_SetFontParam
    - Label�ؼ������þ�����ʾһ����ǩ��������Ҫ�����Ծ����ı��������ı��ķ�񣬰����ı�����ɫ�����塢�ּ������ݣ�Ĭ�����ֵ���ڴ����ؼ�ʱ����MMITHEME_GetLabelStyle�ӿ����õģ��û����Ե���GUILABEL_SetFontParam���������ı�����ʾ���
    - ��ʵ����
@code
GUILABEL_SetFontParam(ctrl_id, font, color, space);
@endcode

@section sec_label_app �ؼ���Ӧ��

Label�Ǳ�ǩ����˼������ʾ����������༭���޸ġ��������������Σ����û���Ҫ��ʾһ������硰���������������⣺���������ڣ����ȣ������ʾ�Ĺ��ܾ�����ʾ�û�������ʲô���ݡ���ʱ����ʹ��Label�ؼ����������ճ̵ı༭����ʹ��������ǩ�ؼ�����ͼ 2 20��ʾ�����С����⣺���������ڣ�������ʱ�䣺���������������������͡�������ֹ���ڡ��ȡ�
@image html label_app_example.jpg 
ͼ 2 20Label�ؼ���Ӧ����ͼ
*******************************************************************************/

/*! @addtogroup  labelGrp Label
@brief label�ؼ�
@details 
-#  �����ı���ʾ
@{
*******************************************************************************/


#ifndef _CTRLLABEL_EXPORT_H_
#define _CTRLLABEL_EXPORT_H_ 

/*---------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
//#include "guictrl.h"
#include "ctrlbase_export.h"
#include "guistring.h"

/*---------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef __cplusplus
    extern   "C"
    {
#endif

/*---------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/
// #ifdef MMI_PDA_SUPPORT
#define GUILABEL_ALIGN_DEFAULT  GUILABEL_ALIGN_LEFT
// #else
//     #define GUILABEL_ALIGN_DEFAULT  GUILABEL_ALIGN_MIDDLE
// #endif

/*---------------------------------------------------------------------------*
 **                         TYPE AND STRUCT                                  *
 **--------------------------------------------------------------------------*/
/*! @enum GUILABEL_ALIGN_E
@brief ��ǩ�ؼ����з�ʽ
*/
typedef enum
{
    GUILABEL_ALIGN_NONE,    /*!<����λ*/
    GUILABEL_ALIGN_LEFT,    /*!<����*/
    GUILABEL_ALIGN_MIDDLE,  /*!<����*/
    GUILABEL_ALIGN_RIGHT,   /*!<����*/
    GUILABEL_ALIGN_MAX      /*!<����λ*/
} GUILABEL_ALIGN_E;

/*! @struct GUILABEL_INIT_DATA_T
@brief ��ǩ�ؼ���ʼ������
*/
typedef struct
{
    GUI_BOTH_RECT_T     both_rect;  /*!<����������*/
    GUILABEL_ALIGN_E    align;      /*!<���з�ʽ*/
} GUILABEL_INIT_DATA_T;

enum
{
    GUILABEL_IMAGE_NONE,
    GUILABEL_IMAGE_RES,
    GUILABEL_IMAGE_BUF,
    GUILABEL_IMAGE_FILE,

    GUILABEL_IMAGE_INVALID,
};
typedef uint8 GUILABEL_IMAGE_TYPE_E;

typedef struct
{
    MMI_IMAGE_ID_T id;
} GUILABEL_DATA_IMAGE_RES_T;

//5.1.2 ????,?bmp buf
typedef struct
{
    uint8 *buf_ptr;
    uint32 size;
    uint16 width;
    uint16 height;
} GUILABEL_DATA_IMAGE_BUF_T;

//5.1.3 ????,??????????
typedef struct
{
    wchar *filename_ptr;
    uint16 filename_len;
} GUILABEL_DATA_IMAGE_FILE_T;

typedef union
{
    GUILABEL_DATA_IMAGE_RES_T res;
    GUILABEL_DATA_IMAGE_BUF_T buf;
    GUILABEL_DATA_IMAGE_FILE_T file;
} GUILABEL_DATA_IMAGE_U;

/*---------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief ����ı�����
@author Jassmine
@param ctrl_id  [in] �ؼ�id
@param text_ptr [in/out] �ı�����
@return �Ƿ�����ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_GetText(
                                MMI_CTRL_ID_T   ctrl_id,    //in
                                MMI_STRING_T    *text_ptr   //in/out
                                );

/***************************************************************************//*!
@brief �����ı�����
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param text_ptr  [in] �ı�����
@param is_update [in] �Ƿ�����ˢ��
@return �����Ƿ�ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetText(
                                MMI_CTRL_ID_T   ctrl_id,    //in
                                MMI_STRING_T    *text_ptr,  //in
                                BOOLEAN         is_update   //in
                                );

/***************************************************************************//*!
@brief ͨ���ı�id�����ı�����
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param text_id   [in] �ı�id
@param is_update [in] �Ƿ�����ˢ��
@return �Ƿ�����ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetTextById(
                                    MMI_CTRL_ID_T   ctrl_id,    //in
                                    MMI_TEXT_ID_T   text_id,    //in
                                    BOOLEAN         is_update   //in
                                    );


/***************************************************************************//*!
@brief �����ı���Ч
@author Jassmine
@param ctrl_id    [in] �ؼ�id
@param region_ptr [in] ��Ч
@param region_num [in] ��Ч����
@return �Ƿ�����ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetTextRegion(
                                      MMI_CTRL_ID_T             ctrl_id,    //in:
                                      GUIFONT_REGION_STYLE_T    *region_ptr,//in:
                                      uint16                    region_num  //in:
                                      );

/***************************************************************************//*!
@brief ���ñ�ǩ�ؼ�ͼƬ
@author Jassmine
@param ctrl_id  [in] �ؼ�id
@param icon_id  [in] ͼƬid
@return �Ƿ�����ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetIcon(
                                MMI_CTRL_ID_T   ctrl_id,//in
                                MMI_IMAGE_ID_T  icon_id //in
                                );

/***************************************************************************//*!
@brief ���ÿؼ�ҳ�߾�
@author Jassmine
@param ctrl_id      [in] �ؼ�id
@param margin_space [in] �߾�
@return �Ƿ�����ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetMargin(
                                  MMI_CTRL_ID_T ctrl_id,    //in
                                  uint16        margin_space//in
                                  );

/*****************************************************************************/
// 	Description : set label margin
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetMarginEx(
                                    MMI_CTRL_ID_T   ctrl_id,    //in
                                    uint16          margin_lr,  //in:left and right margin
                                    uint16          margin_tb   //in:top and bottom margin
                                    );

/***************************************************************************//*!
@brief ���ÿؼ�����
@author Jassmine
@param ctrl_id [in] �ؼ�id
@param bg_ptr  [in] ����
@return �Ƿ�����ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetBg(
                              MMI_CTRL_ID_T ctrl_id,//in
                              GUI_BG_T      *bg_ptr //in
                              );

/***************************************************************************//*!
@brief ���ÿؼ�����Ϊ��ɫ,��ɫֵ
@author Jassmine
@param ctrl_id  [in] �ؼ�id
@param bg_color [in] ��ɫ
@return �Ƿ�����ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetBackgroundColor(
                                           MMI_CTRL_ID_T    ctrl_id,
                                           GUI_COLOR_T      bg_color
                                           );

/***************************************************************************//*!
@brief ���ÿؼ�����
@author Jassmine
@param ctrl_id    [in] �ؼ�id
@param font       [in] �����С
@param font_color [in] ������ɫ
@return �Ƿ�����ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetFont(
                                MMI_CTRL_ID_T   ctrl_id,    //in:
                                GUI_FONT_T      font,       //in:
                                GUI_COLOR_T     font_color  //in:
                                );

/***************************************************************************//*!
@brief ��ÿؼ�����
@author Jassmine
@param ctrl_id  [in]     �ؼ�id
@param rect_ptr [in/out] ����
@return �Ƿ�����ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_GetRect(
                                MMI_CTRL_ID_T   ctrl_id,    //in:
                                GUI_RECT_T      *rect_ptr   //in/out:
                                );


/***************************************************************************//*!
@brief ���ÿؼ�����
@author Jassmine
@param ctrl_id   [in] �ؼ�id
@param rect_ptr  [in] ����
@param is_update [in] �Ƿ�����ˢ��
@return �Ƿ�����ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetRect(
                                MMI_CTRL_ID_T   ctrl_id,    //in:
                                GUI_RECT_T      *rect_ptr,  //in:
                                BOOLEAN         is_update   //in
                                );


/***************************************************************************//*!
@brief ���ÿؼ�������Ӧ����
@author Jassmine
@param ctrl_id  [in] �ؼ�id
@param rect_ptr [in] ����
@return �Ƿ�����ɹ�
@note only for form
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetTpRect(
                                  MMI_CTRL_ID_T ctrl_id,    //in:
                                  GUI_RECT_T    *rect_ptr   //in/out:
                                  );


/***************************************************************************//*!
@brief ���ÿؼ��Ƿ�ɼ�
@author Jassmine
@param ctrl_id    [in] �ؼ�id
@param is_visible [in] �Ƿ�ɼ�
@param is_update  [in] �Ƿ�����ˢ��
@return �Ƿ�����ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetVisible(
                                   MMI_CTRL_ID_T    ctrl_id,
                                   BOOLEAN          is_visible,
                                   BOOLEAN          is_update
                                   );

/*****************************************************************************/
// 	Description : set label icon Ex
//	Global resource dependence : 
//  Author: Jassmine
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetIconEx(
                                MMI_CTRL_ID_T   ctrl_id,//in
                                BOOLEAN is_bit_map,//IN
                                GUILABEL_IMAGE_TYPE_E img_type,//IN
                                GUILABEL_DATA_IMAGE_U img_data//IN
                                );
/*****************************************************************************/
// 	Description : flush label backgroud buffer
//	Global resource dependence : 
//  Author: feng.xiao
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_FlushBg(
                                MMI_CTRL_ID_T   ctrl_id
                                );

/*****************************************************************************/
//  Description : set label align
//  Parameter: [In] ctrl_id : ctrl id
//             [In] align : align
//             [Out] None
//             [Return] result :TRUE(set success) FALSE(set failed)
//  Author: fangfang.yao
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetAlign (
        MMI_CTRL_ID_T   ctrl_id,
        GUILABEL_ALIGN_E align
);

/*****************************************************************************/
//  Description : string is auto scroll if string width > diaplay width whether label is active or not
//  Parameter: [In] ctrl_id : ctrl id
//             [In] is_auto_scroll : is_auto_scroll
//             [Out] None
//  Author: miao.liu2
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetIsAutoScroll (
        MMI_CTRL_ID_T   ctrl_id,
        BOOLEAN           is_auto_scroll
);

/*****************************************************************************/
//  Description : label string is need Ellipsis
//  Parameter: [In] ctrl_id : ctrl id
//             [In] is_ellipsis_ex : is_ellipsis_ex
//             [Out] None
//  Author: miao.liu2
//  Note:���ַ�����ȳ�����ʾ����ʱ�������Ƿ���ʾʡ�Ե�
/*****************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetIsEllipsisEx (
        MMI_CTRL_ID_T   ctrl_id,
        BOOLEAN           is_ellipsis_ex
);

/***************************************************************************//*!
@brief ���ÿؼ��ı���ICON�߾�
@author Jassmine
@param ctrl_id      [in] �ؼ�id
@param margin_space [in] �߾�
@return �Ƿ�����ɹ�
@note 
*******************************************************************************/
PUBLIC BOOLEAN CTRLLABEL_SetTextMargin(
                                  MMI_CTRL_ID_T ctrl_id,    //in
                                  uint16        margin_space//in
                                  );

/**--------------------------------------------------------------------------*
 **                         Compiler Flag                                    *
 **--------------------------------------------------------------------------*/
#ifdef   __cplusplus
    }
#endif

#endif

/*@}*/
