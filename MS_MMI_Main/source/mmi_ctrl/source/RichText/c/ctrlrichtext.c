/*! @file guirichtext.c
@brief richtext�ĺ���ʵ���ļ�
@author Lianxiang.Zhou
@version 1.0
@date 2011/03/28
@remarks Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
@note 05/30/2007   liqing.peng     Create
@note 05/30/2009   lianxiang.zhou  Modified
@note 2010/06/18   Lianxiang.Zhou  refactoring
@details ����ļ��������richtext��ص����д��롣
*******************************************************************************/


/*! @page page_RichTextBody RichText��ʵ��(RichText)

@section sec_richtext_body_design_brief ���˵����Ҫ

-# ���ڼ��ݵ���Ҫ��ԭ����item��item������������֮ǰ�Ľӿ�
-# ����ļ�
    -# guirichtext.h ����ӿڣ��ṩ�ⲿ��������ݽṹ�͹��ܺ���
    -# guirichtext_internal.h �ڲ�ʵ�ֵ�һЩ���壬�ᱻ���ײ��ģ����ã��鷳
-# ����RichText
    -# ��ʾͼƬ������
    -# ʶ�������ַ�
    -# �Կ�ʶ��Ĳ����ṩ�����л����ܣ������ܸ��ݽ���ȡ�������Ϣ
-# һЩ����
    -# һ��RichText��Ϊ�ܶ��С�ĵ�Ԫ��ÿ����Ԫ�����������޵�
        -# ÿ����Ԫ���ֻ����һ��ͼƬ��һ������
    -# ����ָ���ļ�����ʾ��ͼƬ��Ϊ�˼���ƣ��̶���ʾ��СΪ���ȫ������߱�4:3
-# һЩϸ��
    -# ���������Զ��ж��ģ�������ָ���Ƿ���ʾ
    -# ��ͬʱ��ʾ�����ͼƬʱ�����ܻ���Ϊϵͳ���ƶ�������ʾ
    -# �绰������ж�������ʽд�úֲܴڣ���Ϊ��û�����״�#*�ĺ�����ʲô��
-# ���˼·
    -# item����RichText
    -# ÿ��item���ٰ���һ��ͼƬ��һ�����֣�������һ��ͼƬ��һ������
    -# item����Ϣ�����ݼ����Թ��ɣ����ݷ�Ϊԭʼ���ݡ���չ����
    -# ����RichText���һ��item��ʱ�����ṩԭʼ���ݣ���ԭʼ���ݵó���չ���ݣ�
    �������ݼ�������ԡ���ͬʱ���¼���RichText����ʾrect
    -# ��ʾRichText��ʱ��������ʾ��Ҫ��ʾ��item�Ĳ��ֻ���ȫ��
    -# ���� ���¼������л����㼰����
    -# ���� �����л����㣬�������������

@section sec_richtext_body_refactory_brief �ع���Ҫ˵��

-# ���һЩδ��ȷ��������մ���һ�£��ᵼ�·�ҳ�Ĳ���ȷ
-# ����Զ���ҳ�ĵ������������µĿ���
-# ȥ��ͼƬ��Ϊ�����ʱ��ı�����ʾ����Ϊ����û������
-# ������Ԫ�ص���һ�����Ͷ���
-# �޸�tp��Ϣ�Ĵ���������������
    -# tp down����ͬһ���㣬�򷢳�penok
    -# tp down����ͬһ���㣬�򷢳�switch focus by tp
    -# tp move����ͬһ���㣬����
    -# tp move����ͬһ���㣬�򷢳�switch focus by tp
    -# tp up������
    @note ����ʹ��penokʼ���Ǹ������������Ϣ̫�����ˣ�����й������䣬�ܿ��ܻ���
 -# �봥�ʵ�penokһ�������������Ѿ��ǻ����ѷ��ˣ�ֻ��ͨ��ע����˵�������˵����һ
 ���õ���Ƶ���Ҫ��Ҳ����˵�����ܹ������һ���ǳ����������顣
 -# ɾ���ؼ����� is_active����Ϊû����
 -# ɾ���ⲿ����GUIRICHTEXT_SetNeedScroll����Ϊ���Զ����ڵ�
 -# �޸�tpdown�Ĵ�����ȷ��λ�����µĽ���Ԫ�أ���rect���㺯��
 -# �޸�theme�еĲ�����Ĭ��ֵ���������Ϊ�˼��ݲ���Ӧ������border space����

@section sec_richtext_body_modify20100728 20100728 �������� - ѡ��������

-# ֧�ֶ���ѡ��
-# ��Ҫ��������:
    -# tp down - reset select rect
    -# tp move - update select rect
    -# tp up - �������
        -# ��ѡ�� - ���� ����-ȫѡ �˵�
        -# ����ԭ�����л�������߼�
    -# tp long - ���� ����-ȫѡ �˵�
    -# ���� ��ȡ��ѡ��
    -# ȫѡ �����趨ѡ��
-# ����ѡ���ַ���ȷ��
    -# �������յ�õ�һ��select rect
    -# մ���ߵ��ַ�����ѡ��
    -# ���к�ĩ���ر���������ص�
    -# �м���ȫ��ѡ��
    -# ���С�ĩ���ַ�
        -# stop point���£������д�start point��ʼ���ĩ�д�stop point��ǰ
        -# stop point���ϣ������д�stop point���ĩ�д�start point��ǰ
-# tp start point �� end point �����������������ʾ���������ģ���ΪҪ֧���϶�ѡ
�� - ������move tp���ؼ�rect֮���ʱ�򣬻�����ķ���������ѡ���ַ���

@section sec_richtext_body_modify20100805 20100805 modify

---- the mail
Jassmine Meng/Spreadtrum
2010-08-03 19:51
text/edti ����/���������¸��汾����м�ǿ���޸ġ�

�����srs�Ѿ���zhenȷ�Ϲ���
�޸�������tp long����������
�϶�������������tp up����������
�и�������
    tp�ڸ��������ϣ� down��ʲô������
        up��������������
        long������������
        move�����¶�λ��������
    tp�ڷǸ�������
        down����������
֧��tp�϶���edit/text���򣬸�������
tp��ס����ʱ��������Ӧkey����
----

@section sec_richtext_body_modify20101102 20101102 modify

@warning ÿһ�����ֶ�����line space��������ʾ��Ч���ر����»��ߡ����line space
�������ᵼ���»��߲�����ʾ

@section sec_richtext_body_modify20101108 20101108 modify

-# ����tag�������е�hyper string��ĿǰΪֹû��Ӧ�á�����������Ӧ�ò�����Ӧ�á���
���������Ϊ��ͬ���ᵽҪ����һ��������ʾ���ַ�����������û���á���˵�����������
Ӧ����ɾ���ģ�����ֻ���ܻ�����ά���ɱ������ԣ�һ����һ�죬��������ά��richtext��
�һ�û��Ӧ����Ҫ�˹��ܣ���Ӧ��ɾ����
-# richtext�кܶ�����ù��ܶ���û��Ӧ�õģ�������Ŀǰ��û�������ô������Щ��

@section sec_richtext_body_modify20110105 20110105 modify

-# ȥ�����役����ʾ�ı�����ʾ���滻Ϊ�߿���ʾ
-# �߿����ϵͳ�ṩ��2���ؿ��Բ�ǿ�
    - ���еı߿���ʾ��ռ��space�����ԣ�һ����ʾ�߿�Ҫ������Ӧ��space
        - item - ռ��border space��item space
        - img & text - ռ��item border space ��img text space
-# ���ӱ߿򽹵���ɫ����
-# ����text����ɫ����
-# ���ӽ������ͣ��ý�����ֵͬ��ʾ������������ֱ�ʶ
-# ��������top���ж���

@section sec_richtext_body_modify20110117 20110117 modify

-# ����ͼ�����ʾλ�ã�������ͼ���ʱ��ͼ����ʾ����ʾ��λ
-# ����ͼ�������Ű������reverse������default���෴��
-# �޸Ļ�ȡtp�ĵ��λ�ã���ϵͳ��spml�ṩ�ĺ�����������ø���ȷ��λ��
-# �޸�handle tp�е�tp���λ���жϣ��ú��϶�ѡ��ͬһ������

*******************************************************************************/

/**--------------------------------------------------------------------------*
 **                         Include Files                                    *
 **--------------------------------------------------------------------------*/
#include "mmi_gui_trc.h"
#include "mmk_type.h"
#include "mmk_app.h"
#include "mmk_timer.h"
#include "guires.h"
#include "mmi_theme.h"
#include "guilcd.h"
#include "guistring.h"
#include "spml_api.h"
#ifndef WIN32
#include "chng_freq.h"
#endif

#include "ctrlrichtext.h"
#include "ctrlrichtext_export.h"
#include "mmi_string.h"

/**--------------------------------------------------------------------------*
 **                         MACRO DEFINITION                                 *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         STATIC DEFINITION                                *
 **--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         TYPE AND CONSTANT                                 */
/*---------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         EXTERNAL DEFINITION                              *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/
 
/**--------------------------------------------------------------------------*
 **                     Inline Function(����С����)                          *
 **--------------------------------------------------------------------------*/
 
/***************************************************************************//*!
@brief �ؼ�����Ŀ����ռ�õĿ��
@author Lianxiang.Zhou
@param richtext_ptr [in] �ؼ�
@return ��ȥ������space֮��Ŀ��
*******************************************************************************/
LOCAL __inline int16 GetItemWidth(CTRLRICHTEXT_OBJ_T const *richtext_ptr)
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return 0;
    }

    return (GetContWidth(richtext_ptr) - 2*richtext_ptr->border_space - 2*richtext_ptr->item_border_space);
}

/***************************************************************************//*!
@brief �ؼ�����Ŀ����ռ�õ�һ����Ļ�ĸ߶�
@author Lianxiang.Zhou
@param richtext_ptr [in] �ؼ�
@return ��ȥ������space֮��ĸ߶�
@note һ��itemͨ������ռ�ó���һ����Ļ�ģ�����ͼƬͨ����һ����Ļ��ʾ��
*******************************************************************************/
LOCAL __inline int16 GetItemHieght(CTRLRICHTEXT_OBJ_T const *richtext_ptr)
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return 0;
    }

    return (GetContHeight(richtext_ptr) - 2*richtext_ptr->border_space - 2*richtext_ptr->item_border_space);
}

/***************************************************************************//*!
@brief ͼƬ��Ĭ�ϸ߶�
@author Lianxiang.Zhou
@param richtext_ptr [in] �ؼ�
@return һ��������ֵ��ȡ�Ŀ�ر���ֵ
@note ��Ϊ�������ᵼ�¿�ȵı仯���ʶ��˴�Ҫȥ����������Ӱ�죬������������Ϊ����
*******************************************************************************/
LOCAL __inline int16 GetImgDefaultHieght(CTRLRICHTEXT_OBJ_T const *richtext_ptr)
{
    int16 img_height = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return 0;
    }

    //�Կ��Ϊ����
    img_height = ((CTRLBASE_OBJ_T*)richtext_ptr)->rect.right - ((CTRLBASE_OBJ_T*)richtext_ptr)->rect.left + 1;
    img_height -= 2*richtext_ptr->border_space;
    img_height -= 2*richtext_ptr->item_border_space;
    //����ָ����ϵ������ϵ�����⾭��ֵ
    img_height *= 3;
    img_height /= 4;

    return img_height;
}

/***************************************************************************//*!
@brief ���������������Ƿ��㽶
@author Lianxiang.Zhou
@param rect1 [in] rect 1
@param rect3 [in] rect 2
@return TRUE or FALSE
@note ��Ϊrichtext���������еģ�����ֻ��Ҫ��������ཻ
*******************************************************************************/
LOCAL __inline BOOLEAN IsRectHIntersect(GUI_RECT_T rect1, GUI_RECT_T rect2)
{
    return ((rect1.bottom > rect2.top) && (rect2.bottom > rect1.top));
}

/***************************************************************************//*!
@brief �ж�һ����ʾ�����Ƿ������richtext��ǰ��ʾ����ʾ
@author Lianxiang.Zhou
@param richtext_ptr [in] �ؼ�
@param rect [in] ��ʾ����
@return TRUE or FALSE
@note ����һ���ض����ж�
- ���Ժ�����ж�
- ���������Ǹ�����ʾ��top���bottom
*******************************************************************************/
LOCAL __inline BOOLEAN NeedDispRect(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    GUI_BIG_RECT_T rect
)
{
    int32 top = 0;
    int32 bottom = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return FALSE;
    }

    top = richtext_ptr->disp_top;
    bottom = top + richtext_ptr->cont_rect.bottom - richtext_ptr->cont_rect.top;

    return ((rect.bottom > top) && (bottom > rect.top));
}

/***************************************************************************//*!
@brief ÿ�ι���һҳ��Ĭ�Ͼ���
@author Lianxiang.Zhou
@param richtext_ptr [in] �ؼ�
@return Ĭ�ϵĹ���ҳ��
@note ��Ҫ�����������������Ϊȥ��һ�������ֵĸ߶�
*******************************************************************************/
LOCAL __inline int16 GetDefaultScrollDistance(CTRLRICHTEXT_OBJ_T const *richtext_ptr)
{
    int16 page_height = 0;
    uint8 line_height = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return 0;
    }

    page_height = GetContHeight(richtext_ptr);
    line_height = GUI_GetFontHeight(richtext_ptr->font, UNICODE_HANZI);
    line_height += richtext_ptr->line_space;

    return (page_height - line_height);
}

/***************************************************************************//*!
@brief ��ȡ�������
@author Lianxiang.Zhou
@param richtext_ptr [in] �ؼ�
@param item_pre [in] ��ʾ��
@return �������
*******************************************************************************/
LOCAL __inline GUI_FONT_T GetItemFont(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    RICHTEXT_ITEM_T const *item_ptr
)
{
    SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != item_ptr); /*assert verified*/

    return (item_ptr->data.orig_data.text_set_font ?
        item_ptr->data.orig_data.setting_font : richtext_ptr->font);
}

/***************************************************************************//*!
@brief ��ȡ���������ɫ
@author Lianxiang.Zhou
@param richtext_ptr [in] �ؼ�
@param item_pre [in] ��ʾ��
@return ���������ɫ
*******************************************************************************/
LOCAL __inline GUI_COLOR_T GetItemFontColor(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    RICHTEXT_ITEM_T const *item_ptr
)
{
    SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != item_ptr); /*assert verified*/

    return (item_ptr->data.orig_data.text_set_font_color ?
        item_ptr->data.orig_data.setting_font_color : richtext_ptr->font_color);
}

/***************************************************************************//*!
@brief ��ȡ��Ķ��뷽ʽ
@author Lianxiang.Zhou
@param richtext_ptr [in] �ؼ�
@param item_pre [in] ��ʾ��
@return ��Ķ��뷽ʽ
*******************************************************************************/
LOCAL __inline GUI_ALIGN_E GetItemAlign(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    RICHTEXT_ITEM_T const *item_ptr
)
{
    SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != item_ptr); /*assert verified*/

    return (item_ptr->data.orig_data.text_set_align ?
        item_ptr->data.orig_data.setting_align : richtext_ptr->disp_align);
}

/***************************************************************************//*!
@brief ��ȡ����ͼƬ�Ķ��뷽ʽ
@author Lianxiang.Zhou
@param richtext_ptr [in] �ؼ�
@param item_pre [in] ��ʾ��
@return ����ͼƬ�Ķ��뷽ʽ
*******************************************************************************/
LOCAL __inline GUIRICHTEXT_IMG_ALIGN_T GetImgAlign(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    RICHTEXT_ITEM_T const *item_ptr
)
{
    GUIRICHTEXT_IMG_ALIGN_T align = GUIRICHTEXT_IMG_ALIGN_DEFAULT;

    SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    SCI_ASSERT(PNULL != item_ptr); /*assert verified*/

    if (item_ptr->data.orig_data.img_set_align)
    {
        align = item_ptr->data.orig_data.img_setting_align;
    }

    if (GUIRICHTEXT_IMG_ALIGN_DEFAULT == align)
    {
        align = GUIRICHTEXT_IMG_ALIGN_MIDDLE;
    }

    //������ж�Ҫ��default֮����Ϊdefault������auto����reverse
    if (GUIRICHTEXT_IMG_ALIGN_AUTO == align)
    {
        if (ALIGN_LEFT == richtext_ptr->disp_align)
        {
            align = GUIRICHTEXT_IMG_ALIGN_LEFT;
        }
        else
        {
            align = GUIRICHTEXT_IMG_ALIGN_RIGHT;
        }
    }
    else if (GUIRICHTEXT_IMG_ALIGN_REVERSE == align)
    {
        if (ALIGN_LEFT == richtext_ptr->disp_align)
        {
            align = GUIRICHTEXT_IMG_ALIGN_RIGHT;
        }
        else
        {
            align = GUIRICHTEXT_IMG_ALIGN_LEFT;
        }
    }

    return align;
}

/***************************************************************************//*!
@brief ��ȡ�����ض������ֵĳ���
@author Lianxiang.Zhou
@param item_pre [in] ��ʾ��
@param line [in] �кţ���0��ʼ
@return �����ֵĳ���
*******************************************************************************/
LOCAL __inline uint16 GetLineLen(RICHTEXT_ITEM_T const *item_ptr, int32 line)
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if (PNULL == item_ptr)
    {
        return 0;
    }

    return item_ptr->data.txt_ext_data.line_start_pos_ptr[line + 1]
        - item_ptr->data.txt_ext_data.line_start_pos_ptr[line];
}

/***************************************************************************//*!
@brief ��ȡ��ʾ�����bottom
@author Lianxiang.Zhou
@param richtext_ptr [in] �ؼ�
@return ��ʾ�����bottom
@note ����ֻ��¼��ʾ�����top��bottom��̬���
*******************************************************************************/
LOCAL __inline int32 GetDispBottom(CTRLRICHTEXT_OBJ_T const *richtext_ptr)
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return 0;
    }

    return (richtext_ptr->disp_top + richtext_ptr->cont_rect.bottom - richtext_ptr->cont_rect.top);
}


/***************************************************************************//*!
@brief һ�����Ƿ���һ���������
@author Lianxiang.Zhou
@param x [in] x
@param y [in] y
@param rect [in] rect
@return �Ƿ���
*******************************************************************************/
LOCAL __inline BOOLEAN IsPointInBigRect(int32 x, int32 y, GUI_BIG_RECT_T const *rect_ptr)
{
    return ((rect_ptr->left <= x && x <= rect_ptr->right)
        && (rect_ptr->top <= y && y <= rect_ptr->bottom));
}


/***************************************************************************//*!
@brief ������������
@author Lianxiang.Zhou
@param co [in] 32λ������
@return �������16λ����
@note �����곬���ܶ��ʱ��ֻҪ֪�������Ϳ�����
@note ���Ի�ȡ��Ļ�ĸ߶ȣ�����û�����壬ֻҪС��(0xffff/2)�����ڿ��ܵ���Ļ�����
*******************************************************************************/
LOCAL __inline int16 FixHCoordinate(int32 co)
{
    int16 fix_co = 0;
    
    if (co < 0)
    {
        fix_co = -1;
    }
    else if (co > 1024)
    {
        fix_co = 1024;
    }
    else
    {
        fix_co = co;
    }

    return fix_co;
}

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : init richtext class
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RichTextCtrlInitVtbl(
    CTRLRICHTEXT_VTBL_T        *richtext_vtbl_ptr
);

/*****************************************************************************/
//  Description : richtext construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RichTextCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLRICHTEXT_INIT_PARAM_T   *richtext_param_ptr
);

/*****************************************************************************/
//  Description : richtext destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RichTextCtrlDestruct (
    CTRLBASE_OBJ_T        *base_ctrl_ptr
);

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void RichTextCtrlPackInitParam (
    GUIRICHTEXT_INIT_DATA_T     *richtext_init_ptr,   //in
    CTRLRICHTEXT_INIT_PARAM_T   *richtext_param_ptr   //out
);

/*****************************************************************************/
//  Description : handle richtext msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E RichTextCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
);

/*****************************************************************************/
//  Description : get richtext pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLRICHTEXT_OBJ_T* GetRichTextPtr (
    MMI_HANDLE_T     ctrl_handle
);

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN RichTextTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
);

LOCAL void FreeItemBuffer(
    RICHTEXT_ITEM_T *item_ptr
);

LOCAL void DestroyItemControl(
    RICHTEXT_ITEM_T *item_ptr
);

LOCAL void FreeAllItems(
    CTRLRICHTEXT_OBJ_T *richtext_ptr
);


LOCAL BOOLEAN RichTextSetRect(
    CTRLBASE_OBJ_T       *me_ptr,
    const GUI_RECT_T     *rect_ptr
);

LOCAL void RichTextInit(
    CTRLBASE_OBJ_T*	base_ctrl_ptr,
    CTRLRICHTEXT_INIT_PARAM_T*	richtext_param_ptr
);

LOCAL void DrawRichText(
    CTRLRICHTEXT_OBJ_T*	richtext_ptr
);

LOCAL void UpdateData(CTRLRICHTEXT_OBJ_T *richtext_ptr);

LOCAL void ContinueDisplayImg(
    CTRLRICHTEXT_OBJ_T *richtext_ptr
);

LOCAL void DrawItem(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    RICHTEXT_ITEM_T *item_ptr
);

LOCAL RICHTEXT_IMG_DISPLAY_E DisplayImage(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    RICHTEXT_ITEM_T *item_ptr
);

LOCAL void DisplayText(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    RICHTEXT_ITEM_T const *item_ptr
);

LOCAL void DisplayItemBg(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    RICHTEXT_ITEM_T const *item_ptr
);

LOCAL void DisplayItemFrame(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    RICHTEXT_ITEM_T const *item_ptr
);

LOCAL void FillRectInCtrl(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    GUI_RECT_T rect,
    GUI_COLOR_T color
);

LOCAL void DrawRectInCtrl(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    GUI_RECT_T rect,
    GUI_COLOR_T color
);
#ifdef TOUCH_PANEL_SUPPORT
LOCAL MMI_RESULT_E HandleTP(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    GUI_POINT_T point,
    BOOLEAN *is_on_focus_ptr
);
#endif
LOCAL MMI_RESULT_E HandleAppDown(
    CTRLRICHTEXT_OBJ_T *richtext_ptr
);

LOCAL MMI_RESULT_E HandleAppUp(
    CTRLRICHTEXT_OBJ_T *richtext_ptr
);

LOCAL void GetTopCrossItem(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    int32 top,
    RICHTEXT_ITEM_T **item_pptr
);

LOCAL void GetBotCrossItem(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    int16 bottom,
    RICHTEXT_ITEM_T **item_pptr
);

LOCAL int16 GetTxtFixHeight(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    int32 v_top,
    BOOLEAN get_hide
);

LOCAL MMI_RESULT_E ScrollDown(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    uint16 max_space
);

LOCAL MMI_RESULT_E ScrollUp(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    uint16              max_space
);

LOCAL MMI_RESULT_E ScrollToLine(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    int32 top
);

LOCAL MMI_RESULT_E CalcItemExtData(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    RICHTEXT_ITEM_T *item_ptr
);

LOCAL MMI_RESULT_E SetItemFocusElememt(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    RICHTEXT_ITEM_T *item_ptr
);

LOCAL MMI_RESULT_E CalcImgWidthHeight(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    GUIRICHTEXT_IMAGE_TYPE_E type,
    GUIRICHTEXT_DATA_IMAGE_U const *data,
    uint16 *width_ptr,
    uint16 *height_ptr
);

LOCAL void CalcVirtualRect(
    CTRLRICHTEXT_OBJ_T *richtext_ptr
);

LOCAL void CalcItemVirtualRect(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    RICHTEXT_ITEM_T *item_ptr
);

LOCAL RICHTEXT_FOCUS_DISP_E LocateFocusItem(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    RICHTEXT_ITEM_T const *item_ptr,
    uint16 focus_index,
    GUI_BIG_RECT_T *focus_rect
);

LOCAL MMI_RESULT_E ParseTagString(
    RICHTEXT_ITEM_T *item_ptr
);

LOCAL MMI_RESULT_E ParseHyperString(
    RICHTEXT_ITEM_T *item_ptr
);

LOCAL MMI_RESULT_E ParseGeneralString(
    RICHTEXT_ITEM_T *item_ptr
);

LOCAL MMI_RESULT_E FormatAbsPos(
    RICHTEXT_ITEM_T *item_ptr
);

LOCAL MMI_RESULT_E GetItemByIndex(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    uint16 item_index,
    RICHTEXT_ITEM_T **item_pptr
);

LOCAL void InitItem(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    GUIRICHTEXT_ITEM_T const *item_ptr,
    RICHTEXT_ITEM_T *new_item_ptr
);

LOCAL MMI_RESULT_E SetData(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    RICHTEXT_SETTINGS_E setting_type,
    void const * data
);

LOCAL MMI_RESULT_E SetDataById(
    MMI_CTRL_ID_T ctrl_id,
    RICHTEXT_SETTINGS_E setting_type,
    void const * data
);

LOCAL void ResetState(CTRLRICHTEXT_OBJ_T *richtext_ptr);

LOCAL MMI_RESULT_E AddFocusElement(
    RICHTEXT_ITEM_T *item_ptr,
    GUIRICHTEXT_TAG_TYPE_E tag_type,
    uint16 abs_begin,
    uint16 abs_end
);

LOCAL void ReleaseFocusItem(RICHTEXT_ITEM_T *item_ptr);

LOCAL MMI_RESULT_E CombineFocusElement(
    RICHTEXT_ITEM_T *item_ptr
);

LOCAL void ScrollToFocus(CTRLRICHTEXT_OBJ_T *richtext_ptr);

LOCAL BOOLEAN GetTpPos(
    GUI_RECT_T const *text_rect_ptr,
    GUI_ALIGN_E text_align,
    wchar const *str_ptr,
    uint32 str_len,
    int16 tp_x,
    GUI_FONT_T font,
    uint32 char_space,
    int32 *nearest_chr_ptr,
    int32 *under_chr_ptr,
    int32 *under_symb_len_ptr
);


#ifdef GUIF_CLIPBOARD
LOCAL uint32 GetSelectLen(CTRLRICHTEXT_OBJ_T *richtext_ptr);

LOCAL void ShowClipboardMenu(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    GUI_POINT_T *tp_point_ptr,
    BOOLEAN always_show
);

LOCAL void HandleClipbrdMenuCopyMsg(
    CTRLRICHTEXT_OBJ_T *richtext_ptr
);

LOCAL void HandleClipbrdMenuSelectAllMsg(
    CTRLRICHTEXT_OBJ_T *richtext_ptr
);

LOCAL void HandleClipbrdMenuSelectMsg(
    CTRLRICHTEXT_OBJ_T *richtext_ptr
);

LOCAL void ScrollByDragTick(uint8  timer_id, uint32 param);

LOCAL void GenSelectData(CTRLRICHTEXT_OBJ_T *richtext_ptr);

LOCAL void SetStartPoint(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    GUI_POINT_T *point_ptr
);

LOCAL void SetStopPoint(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    GUI_POINT_T *point_ptr
);

LOCAL void ReGenLineInfo(CTRLRICHTEXT_OBJ_T *richtext_ptr);

LOCAL void ClearClipBoardInfo(CTRLRICHTEXT_OBJ_T *richtext_ptr);

#ifdef TOUCH_PANEL_SUPPORT
LOCAL void CreateRichtextSlider(
    CTRLRICHTEXT_OBJ_T  *richtext_ptr  //in:
);

LOCAL void DestroyRichtextSlider(
    CTRLRICHTEXT_OBJ_T  *richtext_ptr  //in:
);

PUBLIC void ShowRichtextSlider(
    CTRLRICHTEXT_OBJ_T  *richtext_ptr
);
#endif
PUBLIC void ChangeRichtextDragState(
    CTRLRICHTEXT_OBJ_T  *richtext_ptr,
    GUIRICHTEXT_DRAGSTATE_E drag_state
);
#ifdef TOUCH_PANEL_SUPPORT
LOCAL void StartRichtextSliderTimer(
    CTRLRICHTEXT_OBJ_T    *richtext_ptr  //in:
);

PUBLIC void StopRichtextSliderTimer(
    CTRLRICHTEXT_OBJ_T    *richtext_ptr  //in:
);

PUBLIC void HandleRichtextSliderTimer(
    CTRLRICHTEXT_OBJ_T  *richtext_ptr  //in:
);

LOCAL void HandleRichtextBeginSlider(
    CTRLRICHTEXT_OBJ_T     *richtext_ptr,
    DPARAM             param
);

LOCAL void HandleRichtextMoveSlider(
    CTRLRICHTEXT_OBJ_T     *richtext_ptr,
    DPARAM             param
);

LOCAL void HandleRichtextEndSlider(
    CTRLRICHTEXT_OBJ_T     *richtext_ptr,
    DPARAM             param
);
#endif
LOCAL BOOLEAN GetHighlightRect(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    BOOLEAN             is_start,
    GUI_RECT_T          *rect_ptr
    );

LOCAL BOOLEAN IsInTextRegion(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    GUI_POINT_T *point_ptr,
    BOOLEAN *is_find_ptr,
    RICHTEXT_SEL_INFO_T *sel_info_ptr
    );

LOCAL BOOLEAN CheckHighlightPos(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    RICHTEXT_SEL_INFO_T *start_info_ptr,
    RICHTEXT_SEL_INFO_T *stop_info_ptr
    );

#endif //GUIF_CLIPBOARD

LOCAL void ClearCtrlInfo(CTRLRICHTEXT_OBJ_T *richtext_ptr);


LOCAL int16 FixDispTop(CTRLRICHTEXT_OBJ_T *richtext_ptr, int16 disp_top);

LOCAL void SlideFlingTick(uint8 timer_id, uint32 param);

LOCAL BOOLEAN SlideRecover(CTRLRICHTEXT_OBJ_T *richtext_ptr, BOOLEAN need_draw);

LOCAL void SlideRecoverTick(uint8 timer_id, uint32 param);

LOCAL void SlideClean(CTRLRICHTEXT_OBJ_T *richtext_ptr);

/**--------------------------------------------------------------------------*
 **                         GLOBAL DEFINITION                                *
 **--------------------------------------------------------------------------*/

/**--------------------------------------------------------------------------*
 **                         FUNCTION DEFINITION                              *
 **--------------------------------------------------------------------------*/

/*****************************************************************************/
//  Description : get richtext type
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC TYPE_TAG_T RICHTEXT_GetType()
{
    static TYPE_TAG_T    obj_type = 0;

    if (0 == obj_type)
    {
        obj_type = TYPEMNG_Register (
                       CTRL_BASE_TYPE,                                  // parent class type
                       "richtext",                                         // control name
                       sizeof (CTRLRICHTEXT_OBJ_T),                        // size of control object struct
                       (OBJECT_CONSTRUCTOR_FUNC) RichTextCtrlConstruct,    // control constructor function
                       (OBJECT_DESTRUCTOR_FUNC) RichTextCtrlDestruct,      // control destructor function
                       sizeof (CTRLRICHTEXT_INIT_PARAM_T),                 // size of init param struct
                       (PARAM_PACKING_FUNC) RichTextCtrlPackInitParam,     // convert init param
                       sizeof (CTRLRICHTEXT_VTBL_T),                       // size of virtual table
                       (VTBL_INIT_FUNC) RichTextCtrlInitVtbl);             // init virtual table function
    }

    return obj_type;
}

/*****************************************************************************/
//  Description : init richtext class
//  Global resource dependence :
//  Author: feng.xiao
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RichTextCtrlInitVtbl (
    CTRLRICHTEXT_VTBL_T        *richtext_vtbl_ptr
)
{
    CTRLBASE_VTBL_T  *base_ctrl_vtbl_ptr = (CTRLBASE_VTBL_T*) richtext_vtbl_ptr;

    if (PNULL == base_ctrl_vtbl_ptr)
    {
        return FALSE;
    }

    base_ctrl_vtbl_ptr->HandleEvent = RichTextCtrlHandleMsg;
    base_ctrl_vtbl_ptr->SetRect = RichTextSetRect;
    base_ctrl_vtbl_ptr->GetDisplayRect = BaseCtrlGetDisplayRectNotImp;

    return TRUE;
}

/*****************************************************************************/
//  Description : convert init param
//  Global resource dependence :
//  Author: feng.xiao
//  Note:
// �÷������ڽ��ؼ��Ķ����ʼ�������ṹת��Ϊ�ڲ��ĳ�ʼ�������ṹ
// ���ڿؼ��м̳й�ϵ���ؼ���ʼ��ʱ�ִ��в����������ڲ��ĳ�ʼ������Ҳ�����м̳й�ϵ
/*****************************************************************************/
LOCAL void RichTextCtrlPackInitParam (
    GUIRICHTEXT_INIT_DATA_T     *richtext_init_ptr,   //in
    CTRLRICHTEXT_INIT_PARAM_T   *richtext_param_ptr   //out
)
{
    CTRLBASE_INIT_PARAM_T   *base_ctrl_param_ptr = (CTRLBASE_INIT_PARAM_T*) richtext_param_ptr;

    if (PNULL == richtext_init_ptr || PNULL == richtext_param_ptr)
    {
        return;
    }

	/* base class init-param pack */
    base_ctrl_param_ptr->both_rect = richtext_init_ptr->both_rect;

 	/* inherit class init-param pack */
	richtext_param_ptr->bg_image_id = richtext_init_ptr->bg_image_id;
}

/*****************************************************************************/
//  Description : richtext construct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RichTextCtrlConstruct (
    CTRLBASE_OBJ_T           *base_ctrl_ptr,
    CTRLRICHTEXT_INIT_PARAM_T   *richtext_param_ptr
)
{
    BOOLEAN             result = TRUE;

    if (PNULL == base_ctrl_ptr || PNULL == richtext_param_ptr)
    {
        return FALSE;
    }

    RichTextInit(base_ctrl_ptr, richtext_param_ptr);

    return (result);
}

/*****************************************************************************/
//  Description : richtext destruct
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL BOOLEAN RichTextCtrlDestruct (
    CTRLBASE_OBJ_T    *base_ctrl_ptr
)
{
    BOOLEAN             result = TRUE;
    CTRLRICHTEXT_OBJ_T     *richtext_ptr = (CTRLRICHTEXT_OBJ_T*) base_ctrl_ptr;

    if (PNULL == richtext_ptr)
    {
        return FALSE;
    }

    FreeAllItems(richtext_ptr);

#ifdef GUIF_CLIPBOARD
    // �������
#ifdef TOUCH_PANEL_SUPPORT
    DestroyRichtextSlider(richtext_ptr);
#endif
#endif

    return (result);
}

/*****************************************************************************/
//  Description : handle richtext msg function
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL MMI_RESULT_E RichTextCtrlHandleMsg (
    CTRLBASE_OBJ_T        *base_ctrl_ptr,   //control pointer
    MMI_MESSAGE_ID_E      msg_id,           //message
    DPARAM                param             //add data
)
{
    MMI_RESULT_E    recode = MMI_RESULT_TRUE;
    MMI_RESULT_E    result = MMI_RESULT_FALSE;
    CTRLRICHTEXT_OBJ_T     *richtext_ctrl_ptr = (CTRLRICHTEXT_OBJ_T*) base_ctrl_ptr;
//  CTRLBASE_VTBL_T     *parent_vtbl_ptr = PNULL;
#ifdef TOUCH_PANEL_SUPPORT //xiyuan edit for pclint
    GUI_POINT_T point = {0};
    BOOLEAN is_tp_on_foucs = FALSE;
#endif
    GUIANIM_DISPLAY_IND_T anim_display_ind = {0};

    if (PNULL == richtext_ctrl_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (msg_id)
    {
    case MSG_CTL_OPEN:
        richtext_ctrl_ptr->is_focus = TRUE;
        break;

    case MSG_CTL_CLOSE:
        SlideClean(richtext_ctrl_ptr);
        break;

    case MSG_CTL_GET_FOCUS:
        richtext_ctrl_ptr->is_focus = TRUE;
        break;
		
    case MSG_CTL_LOSE_FOCUS:
        richtext_ctrl_ptr->is_focus = FALSE;
        SlideClean(richtext_ctrl_ptr);
        break;

    case MSG_CTL_GET_ACTIVE:
        break;

    case MSG_CTL_LOSE_ACTIVE:
        SlideClean(richtext_ctrl_ptr);

#ifdef GUIF_CLIPBOARD
        ChangeRichtextDragState(richtext_ctrl_ptr, GUIRICHTEXT_DRAGSTATE_NORMAL);
#endif
        break;

    case MSG_CTL_PAINT:
        DrawRichText(richtext_ctrl_ptr);
        break;

    case MSG_LCD_SWITCH:
#ifdef GUIF_CLIPBOARD
        if (richtext_ctrl_ptr->is_clipboard_support)
        {
            richtext_ctrl_ptr->is_line_data_dirty = TRUE;

            ChangeRichtextDragState(richtext_ctrl_ptr, GUIRICHTEXT_DRAGSTATE_NORMAL);

            if (0 != richtext_ctrl_ptr->clipbrd_win_handle)
            {
                MMITHEME_CloseClipbrdMenu(richtext_ctrl_ptr->clipbrd_win_handle);
            }
        }
#endif
        SlideClean(richtext_ctrl_ptr);
        break;

    case MSG_APP_OK:
        recode = GUICTRL_PostNotify(((CTRLBASE_OBJ_T*)richtext_ctrl_ptr)->handle, MSG_NOTIFY_OK);
        break;

    case MSG_APP_WEB:
        recode = GUICTRL_PostNotify(((CTRLBASE_OBJ_T*)richtext_ctrl_ptr)->handle, MSG_NOTIFY_MIDSK);
        break;

    case MSG_APP_CANCEL:
        recode = GUICTRL_PostNotify(((CTRLBASE_OBJ_T*)richtext_ctrl_ptr)->handle, MSG_NOTIFY_CANCEL);
        break;

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_DOWN:

        point.x = MMK_GET_TP_X(param);
        point.y = MMK_GET_TP_Y(param);

        richtext_ctrl_ptr->tp_start_point = point;

#ifdef GUIF_CLIPBOARD
        if (richtext_ctrl_ptr->is_clipboard_support)
        {
            //״̬����
            if (RICHTEXT_STATE_SELECTING == richtext_ctrl_ptr->select_state)
            {
                richtext_ctrl_ptr->select_state = RICHTEXT_STATE_SELECTED;
            }
            else
            {
                ChangeRichtextDragState(richtext_ctrl_ptr, GUIRICHTEXT_DRAGSTATE_NORMAL);
            }
        }
#else
        if (GUI_PointIsInRect(point, richtext_ctrl_ptr->cont_rect))
        {
            result = HandleTP(richtext_ctrl_ptr, point, &is_tp_on_foucs);

            if (MMI_RESULT_TRUE == result)
            {
                GUICTRL_PostNotify(((CTRLBASE_OBJ_T*)richtext_ctrl_ptr)->handle, MSG_NOTIFY_RICHTEXT_SWITCHFOCUS_BY_TP);
            }
            else if (is_tp_on_foucs)
            {
                recode = GUICTRL_PostNotify(((CTRLBASE_OBJ_T*)richtext_ctrl_ptr)->handle, MSG_NOTIFY_PENOK);
            }
        }
#endif
        //slide...
        MMK_StopTimer(richtext_ctrl_ptr->slide_timer);
        richtext_ctrl_ptr->slide_timer = 0;

        MMK_ResetVelocityItem();
        MMK_AddVelocityItem(point.x, point.y);

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_SHORT:
        if (RICHTEXT_TPMODE_NONE == richtext_ctrl_ptr->tp_mode
#ifdef GUIF_CLIPBOARD
            && MMITHEME_DRAGMODE_DEFAULT == richtext_ctrl_ptr->drag_mode
#endif
            )
        {
            richtext_ctrl_ptr->tp_mode = RICHTEXT_TPMODE_DRAG;
        }
        
        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527


#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_MOVE:

        if (RICHTEXT_TPMODE_NONE == richtext_ctrl_ptr->tp_mode)
        {
            richtext_ctrl_ptr->tp_mode = RICHTEXT_TPMODE_SLIDE;
        }

        if (RICHTEXT_TPMODE_SLIDE == richtext_ctrl_ptr->tp_mode)
        {
            int16 delta_y = 0;

            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);

            //slide...
            MMK_AddVelocityItem(point.x, point.y);

            //slide
            delta_y = point.y - richtext_ctrl_ptr->tp_start_point.y;

            richtext_ctrl_ptr->disp_top -= delta_y;

            DrawRichText(richtext_ctrl_ptr);

            richtext_ctrl_ptr->tp_start_point = point;
        }
        else
        {
            SCI_ASSERT(RICHTEXT_TPMODE_DRAG == richtext_ctrl_ptr->tp_mode); /*assert verified*/

            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);

#ifdef GUIF_CLIPBOARD

            if (richtext_ctrl_ptr->is_clipboard_support)
            {
                /*
                1 ��ǰ��ѡ����
                        1 ����ѡ����
                        2 ��ʼѡ��
                2 ��ǰ����ѡ����
                    1 ����ѡ��

                ��������:
                    1 ���Ǽ���ѡ��
                    2 ��ѡ���У������ѡ��״̬
                */

                if (RICHTEXT_STATE_SELECTING != richtext_ctrl_ptr->select_state)
                {
                    richtext_ctrl_ptr->select_state = RICHTEXT_STATE_SELECTING;
                    //�ӵ�ǰҳ����������ȡѡ�������������
                    richtext_ctrl_ptr->v_start_x = richtext_ctrl_ptr->tp_start_point.x - richtext_ctrl_ptr->cont_rect.left;
                    richtext_ctrl_ptr->v_start_y = richtext_ctrl_ptr->disp_top + richtext_ctrl_ptr->tp_start_point.y - richtext_ctrl_ptr->cont_rect.top;

                    //ɾ������Ԫ��
                    if (PNULL != richtext_ctrl_ptr->focus_item_ptr)
                    {
                        richtext_ctrl_ptr->focus_item_ptr = PNULL;
                        GUICTRL_PostNotify(((CTRLBASE_OBJ_T*)richtext_ctrl_ptr)->handle, MSG_NOTIFY_RICHTEXT_SWITCHFOCUS_BY_TP);
                    }
                }

                richtext_ctrl_ptr->tp_stop_point = point;

                //��������ƶ���rect֮�⣬���ȹ�������ѡ��
                if (!GUI_PointIsInRect(point, richtext_ctrl_ptr->cont_rect))
                {
                    if (0 == richtext_ctrl_ptr->drag_select_timer)
                    {
                        ScrollByDragTick(0, (uint32)richtext_ctrl_ptr);
                    }
                }
                else
                {
                    if (GUIRICHTEXT_SLIDER_LEFT_SELECTOR == richtext_ctrl_ptr->tpdown_slider)
                    {
                        SetStartPoint(richtext_ctrl_ptr, &point);
                    }
                    else if (GUIRICHTEXT_SLIDER_RIGHT_SELECTOR == richtext_ctrl_ptr->tpdown_slider
                        || MMITHEME_DRAGMODE_DEFAULT == richtext_ctrl_ptr->drag_mode)
                    {
                        SetStopPoint(richtext_ctrl_ptr, &point);
                    }

                    //�˴����ж��Ƿ���Ҫ���»��Ƶ������Ƚϸ��ӣ��ʶ����ж�
                    DrawRichText(richtext_ctrl_ptr);
                }
            }
#else
            if (GUI_PointIsInRect(point, richtext_ctrl_ptr->cont_rect))
            {
                result = HandleTP(richtext_ctrl_ptr, point, &is_tp_on_foucs);

                if (MMI_RESULT_TRUE == result)
                {
                    GUICTRL_PostNotify(((CTRLBASE_OBJ_T*)richtext_ctrl_ptr)->handle, MSG_NOTIFY_RICHTEXT_SWITCHFOCUS_BY_TP);
                }
            }
#endif
        }

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_UP:

        if (RICHTEXT_TPMODE_SLIDE == richtext_ctrl_ptr->tp_mode)
        {
            //slide...
            MMK_ComputeCurrentVelocity( PNULL, &(richtext_ctrl_ptr->slide_v), FLING_MAX_VELOCITY, FLING_MAX_VELOCITY );

            SlideFlingTick(0, (uint32)richtext_ctrl_ptr);
        }
#ifdef GUIF_CLIPBOARD
        else
        {
            point.x = MMK_GET_TP_X(param);
            point.y = MMK_GET_TP_Y(param);

            if (richtext_ctrl_ptr->is_clipboard_support)
            {
                /*
                1 ��ǰ��ѡ����
                    1 ��ǰ��ѡ��
                        1 ȡ��ѡ��
                        2 ����������tp����
                    2 ��ǰ��ѡ��
                        1 ����������tp����
                2 ��ǰ����ѡ����
                    1 ������ѡ��
                    2 �����༭�˵� ???

                ��������:
                    1 ��ѡ���У���ѡ��ȡ��ѡ��
                    2 ��ѡ���У����ǽ���������tp����
                    3 ѡ���ã��л�����ѡ��״̬�������༭�˵�
                */
                if (richtext_ctrl_ptr->drag_select_timer > 0)
                {
                    MMK_StopTimer(richtext_ctrl_ptr->drag_select_timer);
                    richtext_ctrl_ptr->drag_select_timer = 0;
                }

                if (RICHTEXT_STATE_SELECTING == richtext_ctrl_ptr->select_state)
                {
                    richtext_ctrl_ptr->select_state = RICHTEXT_STATE_SELECTED;
                    ShowClipboardMenu(richtext_ctrl_ptr, &point, FALSE);

                    break;
                }
                else if (RICHTEXT_STATE_SELECTED == richtext_ctrl_ptr->select_state)
                {
                    //ͨ���ļ򵥵����ȡ��select״̬
                    ClearClipBoardInfo(richtext_ctrl_ptr);
                    DrawRichText(richtext_ctrl_ptr);
                }

                ChangeRichtextDragState(richtext_ctrl_ptr, GUIRICHTEXT_DRAGSTATE_NORMAL);
            }

            if (!GUI_PointIsInRect(point, richtext_ctrl_ptr->cont_rect))
            {
                break;
            }

            result = HandleTP(richtext_ctrl_ptr, point, &is_tp_on_foucs);

            if (MMI_RESULT_TRUE == result)
            {
                GUICTRL_PostNotify(((CTRLBASE_OBJ_T*)richtext_ctrl_ptr)->handle, MSG_NOTIFY_RICHTEXT_SWITCHFOCUS_BY_TP);
            }
            else if (is_tp_on_foucs)
            {
                recode = GUICTRL_PostNotify(((CTRLBASE_OBJ_T*)richtext_ctrl_ptr)->handle, MSG_NOTIFY_PENOK);
            }
        }
#endif

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527

#ifdef GUIF_CLIPBOARD
#ifdef TOUCH_PANEL_SUPPORT //IGNORE9527
    case MSG_TP_PRESS_LONG:

        if (RICHTEXT_TPMODE_DRAG == richtext_ctrl_ptr->tp_mode
            || MMITHEME_DRAGMODE_DEFAULT != richtext_ctrl_ptr->drag_mode)
        {
            if (richtext_ctrl_ptr->is_clipboard_support)
            {
                /*
                ���ǵ����༭�˵�
                */
                point.x = MMK_GET_TP_X(param);
                point.y = MMK_GET_TP_Y(param);

                if (RICHTEXT_STATE_SELECTING != richtext_ctrl_ptr->select_state)
                {
                    if (MMITHEME_DRAGMODE_DEFAULT != richtext_ctrl_ptr->drag_mode)
                    {
                        BOOLEAN is_find = FALSE;
                        RICHTEXT_SEL_INFO_T sel_info = {0};

                        IsInTextRegion(richtext_ctrl_ptr, &point, &is_find, &sel_info);

                        if (!is_find)
                        {
                            break;
                        }

                        HandleClipbrdMenuSelectMsg(richtext_ctrl_ptr);

                        // Ǳ����
                        // �����򿪴��ڣ���LOSE FOCUS��Ϣ��LCD���ݣ��������㻹û����LCD�ϣ�
                        // ������ʾ��������������������һ��
                        //MMITHEME_UpdateRect();
                        GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );
                    }

                    ShowClipboardMenu(richtext_ctrl_ptr, &point, TRUE);
                }
            }
        }

        break;
#endif //TOUCH_PANEL_SUPPORT //IGNORE9527
#endif

    case MSG_APP_UP:
        result = HandleAppUp(richtext_ctrl_ptr);

        if (MMI_RESULT_TRUE == result)
        {
            GUICTRL_PostNotify(((CTRLBASE_OBJ_T*)richtext_ctrl_ptr)->handle, MSG_NOTIFY_RICHTEXT_SWITCHFOCUS_BY_KEY);
        }

        break;

    case MSG_APP_DOWN:
        result = HandleAppDown(richtext_ctrl_ptr);

        if (MMI_RESULT_TRUE == result)
        {
            GUICTRL_PostNotify(((CTRLBASE_OBJ_T*)richtext_ctrl_ptr)->handle, MSG_NOTIFY_RICHTEXT_SWITCHFOCUS_BY_KEY);
        }

        break;

    case MSG_CTL_PRGBOX_MOVEUP_ONE:
        ScrollUp(richtext_ctrl_ptr, richtext_ctrl_ptr->scroll_step);
        break;

    case MSG_CTL_PRGBOX_MOVEUP_PAGE:
        ScrollUp(richtext_ctrl_ptr, 0);
        break;

    case MSG_CTL_PRGBOX_MOVEDOWN_ONE:
        ScrollDown(richtext_ctrl_ptr, richtext_ctrl_ptr->scroll_step);
        break;

    case MSG_CTL_PRGBOX_MOVEDOWN_PAGE:
        ScrollDown(richtext_ctrl_ptr, 0);
        break;

    case MSG_CTL_PRGBOX_MOVE:
        {
            int32 top_line = (int32)(*(uint32*)param);

            ScrollToLine(richtext_ctrl_ptr, top_line);
        }
        break;

    case MSG_CTL_ANIM_DISPLAY_IND:

        if (!richtext_ctrl_ptr->is_focus
                || (PNULL == richtext_ctrl_ptr->img_appending_item_ptr))
        {
            break;
        }

        anim_display_ind = *(GUIANIM_DISPLAY_IND_T *)param;

        if (GUIANIM_RESULT_INTERRUPT != anim_display_ind.result)
        {
            if (GUIANIM_RESULT_NO_SUPPORT == anim_display_ind.result)
            {
                richtext_ctrl_ptr->img_appending_item_ptr->data.orig_data.img_type = GUIRICHTEXT_IMAGE_RES;
                richtext_ctrl_ptr->img_appending_item_ptr->data.orig_data.img_data.res.id = richtext_ctrl_ptr->anim_error_icon_id;
            }

            ContinueDisplayImg(richtext_ctrl_ptr);
        }

        break;

#ifdef GUIF_CLIPBOARD
    case MSG_NOTIFY_CLIPBRD_CUT:
        SCI_PASSERT(FALSE, ("can not be cut")); /*assert verified*/
        break;

    case MSG_NOTIFY_CLIPBRD_COPY:

        if (richtext_ctrl_ptr->is_clipboard_support)
        {
            HandleClipbrdMenuCopyMsg(richtext_ctrl_ptr);
        }

        break;

    case MSG_NOTIFY_CLIPBRD_PASTE:
        SCI_PASSERT(FALSE, ("can not be paste")); /*assert verified*/
        break;

    case MSG_NOTIFY_CLIPBRD_SELECTALL:

        if (richtext_ctrl_ptr->is_clipboard_support)
        {
            HandleClipbrdMenuSelectAllMsg(richtext_ctrl_ptr);
        }

        break;

    case MSG_NOTIFY_CLIPBRD_MENU_CLOSED:
        richtext_ctrl_ptr->clipbrd_win_handle = 0;
        if (!MMK_IsFocusWin(richtext_ctrl_ptr->win_id))
        {
            ChangeRichtextDragState(richtext_ctrl_ptr, GUIRICHTEXT_DRAGSTATE_NORMAL);
        }
        break;
#ifdef TOUCH_PANEL_SUPPORT
    case MSG_NOTIFY_BEGIN_SLIDER:
        HandleRichtextBeginSlider(richtext_ctrl_ptr, param);
        break;

    case MSG_NOTIFY_MOVE_SLIDER:
        HandleRichtextMoveSlider(richtext_ctrl_ptr, param);
        break;

    case MSG_NOTIFY_END_SLIDER:
        HandleRichtextEndSlider(richtext_ctrl_ptr, param);
        break;

    case MSG_TIMER:
        if (richtext_ctrl_ptr->slider_dis_timer_id == *(uint8*)param)
        {
            HandleRichtextSliderTimer(richtext_ctrl_ptr);
        }
        break;
#endif
#endif

    default:
        recode = MMI_RESULT_FALSE;
        break;
    }

    return recode;
}

/*****************************************************************************/
//  Description : get richtext pointer by control id
//  Global resource dependence :
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL CTRLRICHTEXT_OBJ_T* GetRichTextPtr (
    MMI_HANDLE_T     ctrl_handle
)
{
    CTRLBASE_OBJ_T  *ctrl_ptr = (CTRLBASE_OBJ_T*)MMK_GetCtrlPtr (ctrl_handle);

    if (PNULL != ctrl_ptr)
    {
        SCI_ASSERT (RichTextTypeOf (ctrl_ptr)); /*assert verified*/
    }

    return (CTRLRICHTEXT_OBJ_T*) ctrl_ptr;
}

/*****************************************************************************/
//  Description : ext type of
//  Global resource dependence :
//  Author:hua.fang
//  Note: judge control type of
/*****************************************************************************/
LOCAL BOOLEAN RichTextTypeOf (
    CTRLBASE_OBJ_T     *ctrl_ptr
)
{
    return TYPEMNG_IsTypeOf((OBJECT_TYPE_PTR)ctrl_ptr, CTRL_RICHTEXT_TYPE);
}

/*==============================================================================
Description: RichTextSetRect
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL BOOLEAN RichTextSetRect(
	CTRLBASE_OBJ_T* me_ptr, 
	const GUI_RECT_T* rect_ptr
)
{
    CTRLRICHTEXT_OBJ_T*	richtext_ptr = PNULL;
    RICHTEXT_FOCUS_DISP_E pre_focus_disp = RICHTEXT_FOCUS_UNKNOW;
    RICHTEXT_FOCUS_DISP_E focus_disp = RICHTEXT_FOCUS_UNKNOW;
    GUI_BIG_RECT_T focus_rect = {0};

    if (PNULL == me_ptr || PNULL == rect_ptr)
    {
        return FALSE;
    }

    richtext_ptr = (CTRLRICHTEXT_OBJ_T *)me_ptr;

    if (PNULL != richtext_ptr->focus_item_ptr)
    {
        pre_focus_disp = LocateFocusItem(richtext_ptr,
                                         richtext_ptr->focus_item_ptr,
                                         richtext_ptr->focus_item_ptr->focus_index,
                                         &focus_rect);
    }

    SetData(richtext_ptr, RICHTEXT_SETTINGS_RECT, rect_ptr);

    //�������
    //��ת��ʱ��top�ǲ���ģ����ԣ�ֻ��Ҫ����֮ǰ�ڣ�֮����ʾ���˺�������
    //��ʱ���������������Ļ�ڼ���
    //�н��㣬�ҽ��㲿����ʾ����Ļ��
    if (RICHTEXT_FOCUS_IN == pre_focus_disp
            || RICHTEXT_FOCUS_BOTTOM_OVER == pre_focus_disp)
    {
        //������ʾ����
        CalcVirtualRect(richtext_ptr);

        focus_disp = LocateFocusItem(richtext_ptr,
                                     richtext_ptr->focus_item_ptr,
                                     richtext_ptr->focus_item_ptr->focus_index,
                                     &focus_rect);

        if (RICHTEXT_FOCUS_BOTTOM_OUT == focus_disp) //��ȥ��
        {
            int16 page_height = GetContHeight(richtext_ptr);

            ScrollToLine(richtext_ptr, (focus_rect.bottom - page_height + 1));
        }
    }


    return TRUE;
}

/*==============================================================================
Description:
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void FreeItemBuffer(
    RICHTEXT_ITEM_T *item_ptr
)
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if (PNULL == item_ptr)
    {
        return;
    }

    //�ڲ���̬�ڴ�
    if (GUIRICHTEXT_TEXT_BUF == item_ptr->data.orig_data.text_type)
    {
        SCI_FREE(item_ptr->data.orig_data.text_data.buf.str_ptr);
        item_ptr->data.orig_data.text_data.buf.str_ptr = PNULL;
        SCI_FREE(item_ptr->data.txt_ext_data.line_start_pos_ptr);
        item_ptr->data.txt_ext_data.line_start_pos_ptr = PNULL;
    }

    if (GUIRICHTEXT_IMAGE_FILE == item_ptr->data.orig_data.img_type)
    {
        SCI_FREE(item_ptr->data.orig_data.img_data.file.filename_ptr);
        item_ptr->data.orig_data.img_data.file.filename_ptr = PNULL;
    }

    ReleaseFocusItem(item_ptr);

    return;
}


/*==============================================================================
Description: DestroyItemControl
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void DestroyItemControl(
    RICHTEXT_ITEM_T *item_ptr
)
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if (PNULL == item_ptr)
    {
        return;
    }

    //anim
    if (PNULL != item_ptr->data.img_ext_data.anim_ptr)
    {
        MMK_DestroyControl(GUICTRL_GetCtrlHandle((IGUICTRL_T*)item_ptr->data.img_ext_data.anim_ptr));
        item_ptr->data.img_ext_data.anim_ptr = PNULL;
    }

    return;
}


/*==============================================================================
Description: FreeAllItems
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void FreeAllItems(
    CTRLRICHTEXT_OBJ_T *richtext_ptr
)
{
    RICHTEXT_ITEM_T *cur_item_ptr = PNULL;
    RICHTEXT_ITEM_T *next_item_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return;
    }

    //��������item
    cur_item_ptr = richtext_ptr->first_item_ptr;

    while (PNULL != cur_item_ptr)
    {
        next_item_ptr = cur_item_ptr->next_ptr;

        FreeItemBuffer(cur_item_ptr);
        DestroyItemControl(cur_item_ptr);

        SCI_FREE(cur_item_ptr);

        cur_item_ptr = next_item_ptr;
    }

    richtext_ptr->first_item_ptr = PNULL;
    richtext_ptr->focus_item_ptr = PNULL;

    return;
}

/*==============================================================================
Description: RichTextInit
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void RichTextInit(
    CTRLBASE_OBJ_T *ctrl_ptr,
    CTRLRICHTEXT_INIT_PARAM_T *init_ptr
)
{
    CTRLRICHTEXT_OBJ_T*	richtext_ptr = (CTRLRICHTEXT_OBJ_T*)ctrl_ptr;
    MMI_RICHTEXT_STYLE_T richtext_style = {0};
    GUI_LCD_DEV_INFO lcd_dev_info = {GUI_MAIN_LCD_ID, GUI_BLOCK_MAIN};
    BOOLEAN need_scroll = FALSE;
    GUI_RECT_T cur_rect = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != ctrl_ptr); /*assert verified*/
    if (PNULL == ctrl_ptr)
    {
        return;
    }

    MMITHEME_GetRichTextStyle(&richtext_style);

    ((CTRLBASE_OBJ_T*)richtext_ptr)->lcd_dev_info = lcd_dev_info;
    richtext_ptr->win_id = MMK_GetWinHandleByCtrl(((CTRLBASE_OBJ_T*)richtext_ptr)->handle);

    richtext_ptr->is_focus = FALSE;

    richtext_ptr->need_cal_ext_data = FALSE;
    richtext_ptr->need_cal_rect = FALSE;
//     ((CTRLBASE_OBJ_T*)richtext_ptr)->both_rect = init_ptr->both_rect;

    cur_rect = MMK_IsWindowLandscape(richtext_ptr->win_id) ? ((CTRLBASE_INIT_PARAM_T*)init_ptr)->both_rect.h_rect : ((CTRLBASE_INIT_PARAM_T*)init_ptr)->both_rect.v_rect;
    SetData(richtext_ptr, RICHTEXT_SETTINGS_RECT, &cur_rect);

    richtext_ptr->need_scroll = FALSE;
    richtext_ptr->scrollbar_width = MMITHEME_GetScrollBarWidthByState(TRUE);
    //Ĭ������û�й�����
    SetData(richtext_ptr, RICHTEXT_SETTINGS_SCROLL_BAR, &need_scroll);

    richtext_ptr->disp_align = (MMI_DIRECTION_RIGHT == MMITHEME_GetDisDirection()) ?
                               ALIGN_RIGHT : ALIGN_LEFT;

    richtext_ptr->first_item_ptr = PNULL;

    richtext_ptr->bg_img_id = (MMITHEME_CheckImageID(init_ptr->bg_image_id)) ?
                              init_ptr->bg_image_id : richtext_style.bg_image_id;

    richtext_ptr->bg_color = richtext_style.bg_color;
    richtext_ptr->anim_parsing_icon_id = richtext_style.anim_parsing_icon_id;
    richtext_ptr->anim_error_icon_id = richtext_style.anim_error_icon_id;

    richtext_ptr->focus_mode = GUIRICHTEXT_FOCUS_SHIFT_NONE;
    richtext_ptr->focus_item_ptr = PNULL;

    richtext_ptr->border_type = GUIRICHTEXT_FRAME_NONE;
    richtext_ptr->border_color = richtext_style.border_color;
    richtext_ptr->border_focus_color = richtext_style.border_focus_color;

    richtext_ptr->font = richtext_style.font;
    richtext_ptr->font_color = richtext_style.font_color;
    richtext_ptr->focusline_color = richtext_style.focusline_color;
    richtext_ptr->focus_bg_color = richtext_style.focus_bg_color;

    richtext_ptr->border_space = richtext_style.border_space;
    richtext_ptr->item_border_space = RICHTEXT_DEFAULT_ITEM_BORDER;
    richtext_ptr->char_space = richtext_style.char_space;
    richtext_ptr->line_space = richtext_style.text_line_space;
    richtext_ptr->item_space = richtext_style.item_space;
    richtext_ptr->img_text_space = richtext_style.img_text_space;
    richtext_ptr->txt_margin = richtext_style.txt_margin;

    //Ĭ��ÿ�ι���һ��
    richtext_ptr->scroll_step = GUI_GetFontHeight(richtext_ptr->font, UNICODE_HANZI)
                                + richtext_ptr->line_space;
#ifdef GUIF_CLIPBOARD
    //Ĭ������֧�ֵ�
    richtext_ptr->is_clipboard_support = TRUE;
    richtext_ptr->drag_mode = richtext_style.drag_mode;
    richtext_ptr->slider_display_period = richtext_style.slider_display_period;
#endif

    return;
}


/*==============================================================================
Description: DrawRichText
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void DrawRichText(
    CTRLRICHTEXT_OBJ_T*	richtext_ptr
)
{
    CTRLBASE_OBJ_T* basectrl_ptr = (CTRLBASE_OBJ_T*)richtext_ptr;
    RICHTEXT_ITEM_T *item_ptr = PNULL;
    BOOLEAN is_bg_disp = FALSE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return;
    }

    //�����Ļ
    is_bg_disp = GUIRES_DisplayImg(PNULL,
                                   &(basectrl_ptr->rect),
                                   &(basectrl_ptr->rect),
                                   richtext_ptr->win_id,
                                   richtext_ptr->bg_img_id,
                                   &(basectrl_ptr->lcd_dev_info));

    if (!is_bg_disp)
    {
        LCD_FillRect(&(basectrl_ptr->lcd_dev_info),
                     basectrl_ptr->rect,
                     richtext_ptr->bg_color);
    }

    //���걳��֮�����û��item���򷵻�
    if (PNULL == richtext_ptr->first_item_ptr)
    {
        return;
    }

    UpdateData(richtext_ptr);

#ifdef GUIF_CLIPBOARD
    if (richtext_ptr->is_clipboard_support)
    {
        GenSelectData(richtext_ptr);
    }
#endif

    //���ò���
    richtext_ptr->img_appending_item_ptr = PNULL;

    //��ʾ����item
    item_ptr = richtext_ptr->first_item_ptr;

    while (PNULL != item_ptr)
    {
        //���item��rect��virtual_rect_disp�������ཻ����˵��item������ʾ����
        if (NeedDispRect(richtext_ptr, item_ptr->virtual_rect))
        {
            DrawItem(richtext_ptr, item_ptr);
        }
        else if (PNULL != item_ptr->data.img_ext_data.anim_ptr)
        {
            //...ʵʱ���٣���û��ѹ���ɱ�
            MMK_DestroyControl(GUICTRL_GetCtrlHandle((IGUICTRL_T*)item_ptr->data.img_ext_data.anim_ptr));
            item_ptr->data.img_ext_data.anim_ptr = PNULL;
        }

        item_ptr = item_ptr->next_ptr;
    }

    if (richtext_ptr->need_scroll)
    {
        int32 top = (richtext_ptr->disp_top > 0) ? richtext_ptr->disp_top : 0;
        
        GUIPRGBOX_SetPosByPtr(TRUE,
                              top,
                              top,
                              richtext_ptr->prgbox_ptr);
    }

    //��ʾ�߿�
    if (GUIRICHTEXT_FRAME_NORMAL == richtext_ptr->border_type)
    {
        LCD_DrawRect(&(basectrl_ptr->lcd_dev_info), basectrl_ptr->rect, richtext_ptr->border_color);
    }

#if defined(GUIF_CLIPBOARD)
    ShowRichtextSlider(richtext_ptr);
#endif

    return;
}


/*==============================================================================
Description: UpdateData
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void UpdateData(CTRLRICHTEXT_OBJ_T *richtext_ptr)
{
    BOOLEAN need_keep_scroll = FALSE;
    int32 bak_top = 0;
    int32 bak_height = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return;
    }

    //���¼������������
    if (!richtext_ptr->need_cal_rect && !richtext_ptr->need_cal_ext_data)
    {
        return;
    }

    //����λ��, �������λ��
    if (richtext_ptr->disp_top > 0 && richtext_ptr->disp_height > 0)
    {
        need_keep_scroll = TRUE;
        bak_top = richtext_ptr->disp_top;
        bak_height = richtext_ptr->disp_height;
    }

    CalcVirtualRect(richtext_ptr);

    //��ԭλ��
    if (need_keep_scroll)
    {
        int16 page_height = GetContHeight(richtext_ptr);
        int32 ctrl_height = richtext_ptr->disp_height;
        int32 new_scroll_top = 0;

        SCI_ASSERT(0 != bak_height); /*assert verified*/

        new_scroll_top = (int32)((double)bak_top * richtext_ptr->disp_height / bak_height + 0.5); //��������

        //����һҳ���ȫ��ʾ������һҳ����ԭ�ȵ�λ�ù���
        if (page_height > ctrl_height)
        {
            richtext_ptr->disp_top = 0;
        }
        else
        {
            //����������ʾ���һҳ
            if (new_scroll_top + page_height >= richtext_ptr->disp_height)
            {
                richtext_ptr->disp_top = richtext_ptr->disp_height - page_height;
            }
            else
            {
                richtext_ptr->disp_top = new_scroll_top;
            }
        }
    }

    return;
}


/*==============================================================================
Description: ContinueDisplayImg
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void ContinueDisplayImg(
    CTRLRICHTEXT_OBJ_T *richtext_ptr
)
{
    RICHTEXT_ITEM_T *item_ptr = PNULL;
    BOOLEAN need_draw = FALSE;
    BOOLEAN need_decode = FALSE;
    RICHTEXT_IMG_DISPLAY_E result = RICHTEXT_IMG_DISPLAY;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return;
    }

    if (PNULL == richtext_ptr->img_appending_item_ptr)
    {
        return;
    }

    //�����������item
    item_ptr = richtext_ptr->img_appending_item_ptr->next_ptr;

    //reset
    richtext_ptr->img_appending_item_ptr = PNULL;

    while (PNULL != item_ptr)
    {
        need_draw = NeedDispRect(richtext_ptr, item_ptr->virtual_rect);

        if (!need_draw) break;

        need_decode = (GUIRICHTEXT_IMAGE_BUF == item_ptr->data.orig_data.img_type
                       || GUIRICHTEXT_IMAGE_FILE == item_ptr->data.orig_data.img_type);

        if (need_decode)
        {
            result = DisplayImage(richtext_ptr, item_ptr);

            if (RICHTEXT_IMG_DISPLAY_APPENDING == result)
            {
                richtext_ptr->img_appending_item_ptr = item_ptr;
                break;
            }
        }

        item_ptr = item_ptr->next_ptr;
    }

    return;
}


/*==============================================================================
Description: DrawItem
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void DrawItem(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    RICHTEXT_ITEM_T *item_ptr
)
{
    RICHTEXT_IMG_DISPLAY_E result = RICHTEXT_IMG_DISPLAY;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if ((PNULL == richtext_ptr)||(PNULL == item_ptr))
    {
        return;
    }
    

    //0 draw bg rect
    DisplayItemBg(richtext_ptr, item_ptr);

    //1 draw image
    if (GUIRICHTEXT_IMAGE_NONE != item_ptr->data.orig_data.img_type)
    {
        result = DisplayImage(richtext_ptr, item_ptr);

        if (RICHTEXT_IMG_DISPLAY_APPENDING == result)
        {
            richtext_ptr->img_appending_item_ptr = item_ptr;
        }
    }

    //2 draw text
    if (GUIRICHTEXT_TEXT_NONE != item_ptr->data.orig_data.text_type)
    {
        DisplayText(richtext_ptr, item_ptr);
    }

    //3 ��ʾ�߿�
    DisplayItemFrame(richtext_ptr, item_ptr);

    return;
}


/*==============================================================================
Description: DisplayImage
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL RICHTEXT_IMG_DISPLAY_E DisplayImage(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    RICHTEXT_ITEM_T *item_ptr
)
{
    CTRLBASE_OBJ_T* basectrl_ptr = (CTRLBASE_OBJ_T*)richtext_ptr;
    RICHTEXT_IMG_DISPLAY_E result = RICHTEXT_IMG_DISPLAY;
    BOOLEAN need_draw = FALSE;

    GUI_RECT_T disp_rect = {0};
    GUI_RECT_T full_disp_rect = {0};
    GUI_RECT_T img_rect = {0};

    int32 y_offset = 0;
    int16 delta_top = 0;
    int16 delta_bottom = 0;

    GUIRICHTEXT_IMG_ALIGN_T align = GUIRICHTEXT_IMG_ALIGN_DEFAULT;


    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if ((PNULL == richtext_ptr)||(PNULL == item_ptr))
    {
        return result;
    }

    need_draw = NeedDispRect(richtext_ptr, item_ptr->data.img_ext_data.disp_rect);

    if (!need_draw)
    {
        //���ٿ���ʾitem�в�����ʾͼƬ����ȥ����λ��
        if (PNULL != item_ptr->data.img_ext_data.anim_ptr)
        {
            MMK_DestroyControl(GUICTRL_GetCtrlHandle((IGUICTRL_T*)item_ptr->data.img_ext_data.anim_ptr));
            item_ptr->data.img_ext_data.anim_ptr = PNULL;
        }

        return RICHTEXT_IMG_DISPLAY_FAIL;
    }

    y_offset = richtext_ptr->cont_rect.top - richtext_ptr->disp_top;

    //������ʾ����, image�Ĵ�С�����������disp_rect�������
    disp_rect.left = richtext_ptr->cont_rect.left + item_ptr->data.img_ext_data.disp_rect.left;
    disp_rect.right = richtext_ptr->cont_rect.left + item_ptr->data.img_ext_data.disp_rect.right;
    disp_rect.top = y_offset + item_ptr->data.img_ext_data.disp_rect.top;
    disp_rect.bottom = y_offset + item_ptr->data.img_ext_data.disp_rect.bottom;

    //��ʾλ��
    align = GetImgAlign(richtext_ptr, item_ptr);

    if (GUIRICHTEXT_IMAGE_RES == item_ptr->data.orig_data.img_type)
    {
        int16 dx = 0;
        int16 dy = 0;

        //������������ʾ
        dx = (disp_rect.right - disp_rect.left + 1) - item_ptr->data.img_ext_data.orig_width;
        dx /= 2;

        if (dx > 0)
        {
            disp_rect.left += dx;
            disp_rect.right = disp_rect.left + item_ptr->data.img_ext_data.orig_width - 1;
        }

        dy = (disp_rect.bottom - disp_rect.top + 1) - item_ptr->data.img_ext_data.orig_height;
        dy /= 2;

        if (dy > 0)
        {
            disp_rect.top += dy;
            disp_rect.bottom = disp_rect.top + item_ptr->data.img_ext_data.orig_height - 1;
        }

        //�������ж��Ƿ�Ҫ��ʾ
        need_draw = IsRectHIntersect(disp_rect, richtext_ptr->cont_rect);

        if (!need_draw)
        {
            return RICHTEXT_IMG_DISPLAY;
        }

        delta_top = richtext_ptr->cont_rect.top - disp_rect.top;
        delta_bottom = disp_rect.bottom - richtext_ptr->cont_rect.bottom;

        img_rect.left = 0;
        img_rect.right = item_ptr->data.img_ext_data.orig_width - 1;
        img_rect.top = 0;
        img_rect.bottom = item_ptr->data.img_ext_data.orig_height - 1;

        if (delta_top > 0)
        {
            disp_rect.top += delta_top;
            img_rect.top += delta_top;
        }

        if (delta_bottom > 0)
        {
            disp_rect.bottom -= delta_bottom;
            img_rect.bottom -= delta_bottom;
        }

        SCI_ASSERT(disp_rect.top <= disp_rect.bottom); /*assert verified*/
        SCI_ASSERT(img_rect.top <= img_rect.bottom); /*assert verified*/

        IMG_EnableTransparentColor(TRUE);

        GUIRES_DisplayImg(PNULL,
                          &disp_rect,
                          &img_rect,
                          richtext_ptr->win_id,
                          item_ptr->data.orig_data.img_data.res.id,
                          &(basectrl_ptr->lcd_dev_info));

        IMG_EnableTransparentColor(FALSE);
    }
    else if (GUIRICHTEXT_IMAGE_BUF == item_ptr->data.orig_data.img_type
             || GUIRICHTEXT_IMAGE_FILE == item_ptr->data.orig_data.img_type)
    {
        GUIANIM_CTRL_INFO_T ctrl_info = {0};
        GUIANIM_DATA_INFO_T data_info = {0};
        GUIANIM_FILE_INFO_T file_info = {0};
        GUIANIM_DISPLAY_INFO_T display_info = {0};
        GUIANIM_DATA_INFO_T *data_info_ptr = PNULL;
        GUIANIM_FILE_INFO_T *file_info_ptr = PNULL;
        GUIANIM_RESULT_E    retcode = GUIANIM_RESULT_SUCC;

        full_disp_rect = disp_rect;

        delta_top = richtext_ptr->cont_rect.top - disp_rect.top;
        delta_bottom = disp_rect.bottom - richtext_ptr->cont_rect.bottom;

        if (delta_top > 0)
        {
            disp_rect.top += delta_top;
        }

        if (delta_bottom > 0)
        {
            disp_rect.bottom -= delta_bottom;
        }

        SCI_ASSERT(disp_rect.top <= disp_rect.bottom); /*assert verified*/

        if (PNULL == item_ptr->data.img_ext_data.anim_ptr)
        {
            //û�оʹ���һ��
            GUIANIM_INIT_DATA_T init_data = {0};
            MMI_CONTROL_CREATE_T create = {0};

            //��Ϊ��ʾ����ͼƬ��������
            if (PNULL != richtext_ptr->img_appending_item_ptr)
            {
                return RICHTEXT_IMG_DISPLAY_FAIL;
            }

            init_data.both_rect.h_rect = init_data.both_rect.v_rect = full_disp_rect;

            create.ctrl_id = 0;
            create.guid = SPRD_GUI_ANIM_ID;
            create.parent_win_handle = richtext_ptr->win_id;
            create.parent_ctrl_handle = basectrl_ptr->handle;
            create.init_data_ptr = &init_data;

            item_ptr->data.img_ext_data.anim_ptr = MMK_CreateControl(&create);

            SCI_ASSERT(PNULL != item_ptr->data.img_ext_data.anim_ptr); /*assert verified*/

            //set default amin icon
            GUIANIM_SetDefaultIconByPtr(item_ptr->data.img_ext_data.anim_ptr,
                                        richtext_ptr->anim_parsing_icon_id,
                                        richtext_ptr->anim_error_icon_id);

            ctrl_info.is_wallpaper_ctrl = FALSE;
            ctrl_info.is_ctrl_id = FALSE;
            ctrl_info.ctrl_id = 0;
            ctrl_info.ctrl_ptr = item_ptr->data.img_ext_data.anim_ptr;

            if (GUIRICHTEXT_IMAGE_BUF == item_ptr->data.orig_data.img_type)
            {
                data_info_ptr = &data_info;
                file_info_ptr = PNULL;

                if (item_ptr->data.orig_data.is_bit_map)
                {
                    data_info.is_bitmap = TRUE;
                    data_info.bitmap_data.bit_ptr    = item_ptr->data.orig_data.img_data.buf.buf_ptr;
                    data_info.bitmap_data.img_height = item_ptr->data.orig_data.img_data.buf.height;
                    data_info.bitmap_data.img_width  = item_ptr->data.orig_data.img_data.buf.width;
                }
                else
                {
                    data_info.is_bitmap = FALSE;
                    data_info.data_ptr = item_ptr->data.orig_data.img_data.buf.buf_ptr;
                    data_info.data_size = item_ptr->data.orig_data.img_data.buf.size;
                }
                data_info.anim_id = 0;
                data_info.img_id = 0;
            }
            else
            {
                data_info_ptr = PNULL;
                file_info_ptr = &file_info;

                file_info.full_path_wstr_ptr = item_ptr->data.orig_data.img_data.file.filename_ptr;
                file_info.full_path_wstr_len = item_ptr->data.orig_data.img_data.file.filename_len;
            }

            display_info.is_syn_decode = richtext_ptr->img_is_syn_decode;
            display_info.is_update = FALSE;
            display_info.bg.bg_type = GUI_BG_IMG;
            display_info.bg.img_id  = richtext_ptr->bg_img_id;
            display_info.align_style = GUIANIM_ALIGN_HVMIDDLE;

            retcode = GUIANIM_SetParam(&ctrl_info,
                                       data_info_ptr,
                                       file_info_ptr,
                                       &display_info);

            if (GUIANIM_RESULT_WAIT == retcode)
            {
                result = RICHTEXT_IMG_DISPLAY_APPENDING;
            }
        }

        //set anim ctrl rect
        GUIANIM_SetCtrlRectByPtr(item_ptr->data.img_ext_data.anim_ptr, &full_disp_rect);

        //set image display rect
        GUIANIM_SetDisplayRectByPtr(item_ptr->data.img_ext_data.anim_ptr, &disp_rect, TRUE);
    }

    return result;
}


/*==============================================================================
Description: DisplayText
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void DisplayText(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    RICHTEXT_ITEM_T const *item_ptr
)
{
    CTRLBASE_OBJ_T* basectrl_ptr = (CTRLBASE_OBJ_T*)richtext_ptr;
    BOOLEAN need_draw = FALSE;
    int32 begin_line = 0;
    int32 end_line = 0;
    int32 v_top = 0;
    int32 v_bottom = 0;
    GUI_RECT_T draw_rect = {0}; //д�ֵ����򣬷���ʾ��дȫ��
    GUI_RECT_T disp_rect = {0}; //��ʾ���������ģ�����ֻ��ʾ����
    GUI_RECT_T line_draw_rect = {0}; //������ʾʱ��д��������ȫ����
    GUI_RECT_T line_disp_rect = {0}; //������ʾʱ���������򣬼�����ֻ�а���

    int32 y_offset = 0;
    int32 delta_top = 0;
    int32 delta_bottom = 0;
    int16 line_height = 0;
    int32 out_line = 0;
    int32 disp_line = 0;

    wchar *str_ptr = PNULL;
    uint16 str_len = 0;
    uint16 *start_pos_ptr = PNULL;

    GUISTR_STYLE_T  str_style = {0}; /*lint !e64*/
#ifdef SCRIPTEASY_SUPPORT
    GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE;
#else
    GUISTR_STATE_T  str_state = GUISTR_STATE_ALIGN | GUISTR_STATE_SINGLE_LINE | GUISTR_STATE_ALIGN_BY_DIR;
#endif    
    MMI_STRING_T    str_info = {0};

    uint16 focus_index = 0;
    GUIFONT_REGION_STYLE_T region_style[RICHTEXT_TAG_STRING_MAX_PER_LINE] = {0};
    uint16 region_count = 0;

    /*
    ����"�س�, ����"����ʾ��������ʾΪ�ո�
    "\r\n", "\n\r", "\n", "\r"
    ����Ϊһ���ո񣬲��ҽ�ȡ���Ĳ��� - �������߼������û������
    */
    wchar fix_str[GUIRICHTEXT_LINE_LEN_MAX] = {0};
    int i = 0;


    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if ((PNULL == richtext_ptr)||(PNULL == item_ptr))
    {
        return;
    }
    
    SCI_ASSERT(GUIRICHTEXT_TEXT_BUF == item_ptr->data.orig_data.text_type); /*assert verified*/

    need_draw = NeedDispRect(richtext_ptr, item_ptr->data.txt_ext_data.disp_rect);

    if (!need_draw)
    {
        return;
    }

    str_ptr = item_ptr->data.orig_data.text_data.buf.str_ptr;
    str_len = item_ptr->data.orig_data.text_data.buf.len;
    start_pos_ptr = item_ptr->data.txt_ext_data.line_start_pos_ptr;

    if (PNULL == str_ptr || 0 == str_len || PNULL == start_pos_ptr)
    {
        return;
    }

    y_offset = richtext_ptr->cont_rect.top - richtext_ptr->disp_top;

    v_top = y_offset + item_ptr->data.txt_ext_data.disp_rect.top;
    v_bottom = y_offset + item_ptr->data.txt_ext_data.disp_rect.bottom;
    //����margin
    v_top += richtext_ptr->txt_margin;
    v_bottom -= richtext_ptr->txt_margin;

    line_height = item_ptr->data.txt_ext_data.line_height + richtext_ptr->line_space;

    delta_top = richtext_ptr->disp_top - item_ptr->data.txt_ext_data.disp_rect.top;
    delta_top -= richtext_ptr->txt_margin;
    delta_bottom = item_ptr->data.txt_ext_data.disp_rect.bottom - GetDispBottom(richtext_ptr);
    delta_bottom -= richtext_ptr->txt_margin;

    begin_line = 0;
    end_line = item_ptr->data.txt_ext_data.line_num - 1;

    if (delta_top > 0)
    {
        out_line = (delta_top + richtext_ptr->line_space) / line_height;
        begin_line += out_line;
        draw_rect.top = v_top + out_line * line_height;
        disp_rect.top = v_top + delta_top;
    }
    else
    {
        draw_rect.top = v_top;
        disp_rect.top = v_top;
    }

    if (delta_bottom > 0)
    {
        out_line = (delta_bottom + richtext_ptr->line_space) / line_height;
        end_line -= out_line;
        draw_rect.bottom = v_bottom - out_line * line_height;
        disp_rect.bottom = v_bottom - delta_bottom;
    }
    else
    {
        draw_rect.bottom = v_bottom;
        disp_rect.bottom = v_bottom;
    }

    //������ʾ����
    disp_rect.left = richtext_ptr->cont_rect.left + item_ptr->data.txt_ext_data.disp_rect.left;
    disp_rect.right = richtext_ptr->cont_rect.left + item_ptr->data.txt_ext_data.disp_rect.right;

    //����margin
    disp_rect.left += richtext_ptr->txt_margin;
    disp_rect.right -= richtext_ptr->txt_margin;

    draw_rect.left = disp_rect.left;
    draw_rect.right = disp_rect.right;

    str_style.font = GetItemFont(richtext_ptr, item_ptr);
    str_style.font_color = GetItemFontColor(richtext_ptr, item_ptr);
    str_style.align = GetItemAlign(richtext_ptr, item_ptr);
    str_style.bg_color = richtext_ptr->focus_bg_color;
    str_style.line_color = richtext_ptr->focusline_color;
    str_style.line_space = richtext_ptr->line_space;
    str_style.char_space = richtext_ptr->char_space;

    line_draw_rect = draw_rect;
    line_draw_rect.bottom = line_draw_rect.top + line_height - 1;
    line_disp_rect = line_draw_rect;
    //��top��bottom����Ӧ�Ĳ���
    line_disp_rect.top = disp_rect.top;
    line_disp_rect.bottom = MIN(line_draw_rect.bottom, disp_rect.bottom);

    //text_dir = SPMLAPI_GetTextDirection(str_ptr, str_len);

    for (disp_line = begin_line; disp_line <= end_line; disp_line++)
    {
        SCI_ASSERT(line_disp_rect.top <= line_disp_rect.bottom); /*assert verified*/

        str_info.wstr_len = start_pos_ptr[disp_line+1] - start_pos_ptr[disp_line];
        str_info.wstr_ptr = str_ptr + start_pos_ptr[disp_line];

        //����������ʾ���� <--begin
        //��ʾ�»��ߺͽ���
        SCI_MEMSET(region_style, 0, sizeof(region_style));
        region_count = 0;

        if (GUIRICHTEXT_FOCUS_SHIFT_NONE != richtext_ptr->focus_mode)
        {
            SCI_ASSERT(0 == item_ptr->focus_count || PNULL != item_ptr->focus_elem); /*assert verified*/

            for (focus_index = 0; focus_index < item_ptr->focus_count; focus_index++)
            {
                if (GUIRICHTEXT_TAG_TXT_ALL != item_ptr->focus_elem[focus_index].tag_type
                        && GUIRICHTEXT_TAG_PHONENUM != item_ptr->focus_elem[focus_index].tag_type
                        && GUIRICHTEXT_TAG_EMAIL != item_ptr->focus_elem[focus_index].tag_type
                        && GUIRICHTEXT_TAG_URL  != item_ptr->focus_elem[focus_index].tag_type)
                {
                    continue;
                }

                //end - check first~~ before "begin"
                if (item_ptr->focus_elem[focus_index].line_end < disp_line)
                {
                    continue;
                }
                else if (item_ptr->focus_elem[focus_index].line_end == disp_line)
                {
                    region_style[region_count].length = item_ptr->focus_elem[focus_index].pos_end + 1;
                }
                else // (end > disp)
                {
                    region_style[region_count].length = str_info.wstr_len;
                }

                //begin
                if (item_ptr->focus_elem[focus_index].line_begin < disp_line)
                {
                    region_style[region_count].start = 0;
                }
                else if (item_ptr->focus_elem[focus_index].line_begin == disp_line)
                {
                    region_style[region_count].start = item_ptr->focus_elem[focus_index].pos_begin;
                }
                else // (begin > disp)
                {
                    break;
                }

                //������ȥ�����֮ǰ����
                region_style[region_count].length -= region_style[region_count].start;

                //focus
                if ((richtext_ptr->focus_item_ptr == item_ptr)
                        && (item_ptr->focus_index == focus_index))
                {
#ifdef MAINLCD_SIZE_128X64//syy modify 2010.12.1 -begin
                    region_style[region_count].state |= GUISTR_STATE_BOLD|GUISTR_STATE_UNDERLINE;
#else
                    region_style[region_count].state |= GUISTR_STATE_INVERSE|GUISTR_STATE_UNDERLINE;
#endif//syy modify 2010.12.1 -end
                }
                else
                {
                    region_style[region_count].state |= GUISTR_STATE_UNDERLINE;
                }

                region_style[region_count].font_color = str_style.font_color;

                region_count++;

                if (region_count == RICHTEXT_TAG_STRING_MAX_PER_LINE) break;
            }
        }

#ifdef GUIF_CLIPBOARD
        if (richtext_ptr->is_clipboard_support)
        {
            BOOLEAN need_draw_select = (richtext_ptr->is_clipboard_support
                && (RICHTEXT_STATE_SELECT_NONE != richtext_ptr->select_state)
                && (item_ptr->data.txt_ext_data.select_len > 0));
            uint16 start_pos = 0;
            uint16 end_pos = 0;

            if (need_draw_select && region_count < RICHTEXT_TAG_STRING_MAX_PER_LINE)
            {
                if (item_ptr->data.txt_ext_data.select_line_begin <= disp_line
                        && disp_line <= item_ptr->data.txt_ext_data.select_line_end)
                {
                    //start
                    if (item_ptr->data.txt_ext_data.select_line_begin == disp_line)
                    {
                        start_pos = item_ptr->data.txt_ext_data.select_pos_begin;
                    }
                    else
                    {
                        start_pos = 0;
                    }

                    //end
                    if (item_ptr->data.txt_ext_data.select_line_end == disp_line)
                    {
                        end_pos = item_ptr->data.txt_ext_data.select_pos_end;
                    }
                    else
                    {
                        end_pos = str_info.wstr_len - 1;
                    }

                    region_style[region_count].start = start_pos;
                    region_style[region_count].length = end_pos - start_pos + 1;

                    region_style[region_count].state = GUISTR_STATE_INVERSE;
                    region_style[region_count].font_color = str_style.font_color;

                    region_count++;
                }
            }
        }

#endif

        str_style.region_ptr = region_style;
        str_style.region_num = region_count;
        //����������ʾ���� end-->

        //дһ��
#ifdef MAINLCD_SIZE_128X64 //�޸ķ���ͬCR220730
        if (line_disp_rect.bottom - line_disp_rect.top + 1 >= item_ptr->data.txt_ext_data.line_height)
#endif
        {
            if (str_info.wstr_len > GUIRICHTEXT_LINE_LEN_MAX)
            {
                str_info.wstr_len = GUIRICHTEXT_LINE_LEN_MAX;
            }

            for (i = 0; i < str_info.wstr_len; i++)
            {
                if('\r' == str_info.wstr_ptr[i])
                {
                    fix_str[i] = ' ';
                    str_info.wstr_len = i + 1;
                    break;
                }
                else if ('\n' == str_info.wstr_ptr[i])
                {
                    fix_str[i] = ' ';
                    str_info.wstr_len = i + 1;
                    break;
                }
                else
                {
                    fix_str[i] = str_info.wstr_ptr[i];
                }
            }

            str_info.wstr_ptr = fix_str;
            
            GUISTR_DrawTextToLCDInRect(
                (const GUI_LCD_DEV_INFO*)&(basectrl_ptr->lcd_dev_info),
                (const GUI_RECT_T*)&line_draw_rect,       //the fixed display area
                (const GUI_RECT_T*)&line_disp_rect,       //�û�Ҫ���е�ʵ������
                (const MMI_STRING_T*)&str_info,
                &str_style,
                str_state,
                GUISTR_TEXT_DIR_AUTO
	        );
        }

        line_draw_rect.top += line_height;
        line_draw_rect.bottom += line_height;
        //����һ���⣬top������ͬ��
        line_disp_rect.top = line_draw_rect.top;
        //���һ�е�bottm���ܻ᲻һ��
        line_disp_rect.bottom = MIN(line_draw_rect.bottom, disp_rect.bottom);
    }

    return;
}


/*==============================================================================
Description: DisplayItemBg
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void DisplayItemBg(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    RICHTEXT_ITEM_T const *item_ptr
)
{
    BOOLEAN need_draw = FALSE;
    GUI_RECT_T txt_rect = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if ((PNULL == richtext_ptr)||(PNULL == item_ptr))
    {
        return;
    }
    need_draw = NeedDispRect(richtext_ptr, item_ptr->virtual_rect);

    if (!need_draw)
    {
        return;
    }

    //text bg
    if (item_ptr->data.orig_data.text_set_bg)
    {
        int32 y_offset = richtext_ptr->cont_rect.top - richtext_ptr->disp_top;

        //������ʾ����
        txt_rect.left = richtext_ptr->cont_rect.left + item_ptr->data.txt_ext_data.disp_rect.left;
        txt_rect.right = richtext_ptr->cont_rect.left + item_ptr->data.txt_ext_data.disp_rect.right;
        txt_rect.top = y_offset + item_ptr->data.txt_ext_data.disp_rect.top;
        txt_rect.bottom = y_offset + item_ptr->data.txt_ext_data.disp_rect.bottom;

        FillRectInCtrl(richtext_ptr, txt_rect, item_ptr->data.orig_data.setting_bg_color);
    }

    return;
}


/*==============================================================================
Description: DisplayItemFrame
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void DisplayItemFrame(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    RICHTEXT_ITEM_T const *item_ptr
)
{
    BOOLEAN need_draw             = FALSE;
    BOOLEAN is_cur_focus        = FALSE;
    BOOLEAN is_item_focus         = FALSE;
    BOOLEAN is_img_focus          = FALSE;
    BOOLEAN is_txt_focus          = FALSE;
    GUIRICHTEXT_TAG_TYPE_E cur_tag_type = GUIRICHTEXT_TAG_NONE;
    GUI_RECT_T item_rect          = {0};
    GUI_RECT_T img_rect           = {0};
    GUI_RECT_T txt_rect           = {0};
    GUI_COLOR_T item_border_color = 0;
    GUI_COLOR_T img_border_color  = 0;
    GUI_COLOR_T txt_border_color  = 0;


    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if ((PNULL == richtext_ptr)||(PNULL == item_ptr))
    {
        return;
    }

    need_draw = NeedDispRect(richtext_ptr, item_ptr->virtual_rect);

    if (!need_draw)
    {
        return;
    }

    is_cur_focus = ((GUIRICHTEXT_FOCUS_SHIFT_NONE != richtext_ptr->focus_mode)
                      && (richtext_ptr->focus_item_ptr == item_ptr));

    item_border_color = richtext_ptr->border_color;
    img_border_color = richtext_ptr->border_color;
    txt_border_color = richtext_ptr->border_color;

    if(is_cur_focus)
    {
        is_item_focus = (GUIRICHTEXT_TAG_NORMAL == item_ptr->data.orig_data.tag_type);

        if(is_item_focus)
        {
            item_border_color = richtext_ptr->border_focus_color;
        }
        else
        {
            SCI_ASSERT(0 == item_ptr->focus_count || PNULL != item_ptr->focus_elem); /*assert verified*/

            if(item_ptr->focus_count > 0)
            {
                cur_tag_type = item_ptr->focus_elem[item_ptr->focus_index].tag_type;

                if (GUIRICHTEXT_TAG_IMG_NORMAL == cur_tag_type)
                {
                    is_img_focus = TRUE;
                    img_border_color = richtext_ptr->border_focus_color;
                }
                else if (GUIRICHTEXT_TAG_TXT_NORMAL == cur_tag_type)
                {
                    is_txt_focus = TRUE;
                    txt_border_color = richtext_ptr->border_focus_color;
                }
            }
        }
    }

    //����ؼ�����ʾ��ֹ��

    //image frame
    if ((GUIRICHTEXT_IMAGE_NONE != item_ptr->data.orig_data.img_type)
        && ((GUIRICHTEXT_FRAME_NORMAL == item_ptr->data.orig_data.img_frame_type)
            || is_img_focus))
    {
        int32 y_offset = richtext_ptr->cont_rect.top - richtext_ptr->disp_top;

        img_rect.left = richtext_ptr->cont_rect.left + item_ptr->data.img_ext_data.disp_rect.left;
        img_rect.right = richtext_ptr->cont_rect.left + item_ptr->data.img_ext_data.disp_rect.right;
        img_rect.top = FixHCoordinate(y_offset + item_ptr->data.img_ext_data.disp_rect.top);
        img_rect.bottom = FixHCoordinate(y_offset + item_ptr->data.img_ext_data.disp_rect.bottom);

        DrawRectInCtrl(richtext_ptr, img_rect, img_border_color);
    }

    //text frame
    if ((GUIRICHTEXT_TEXT_NONE != item_ptr->data.orig_data.text_type)
        && ((GUIRICHTEXT_FRAME_NORMAL == item_ptr->data.orig_data.text_frame_type)
            || is_txt_focus))
    {
        int32 y_offset = richtext_ptr->cont_rect.top - richtext_ptr->disp_top;

        txt_rect.left = richtext_ptr->cont_rect.left + item_ptr->data.txt_ext_data.disp_rect.left;
        txt_rect.right = richtext_ptr->cont_rect.left + item_ptr->data.txt_ext_data.disp_rect.right;
        txt_rect.top = FixHCoordinate(y_offset + item_ptr->data.txt_ext_data.disp_rect.top);
        txt_rect.bottom = FixHCoordinate(y_offset + item_ptr->data.txt_ext_data.disp_rect.bottom);

        DrawRectInCtrl(richtext_ptr, txt_rect, txt_border_color);
    }

    if ((GUIRICHTEXT_FRAME_NORMAL == item_ptr->data.orig_data.frame_type)
        || is_item_focus)
    {
        int32 y_offset = richtext_ptr->cont_rect.top - richtext_ptr->disp_top;

        item_rect.left = richtext_ptr->cont_rect.left + item_ptr->virtual_rect.left;
        item_rect.right = richtext_ptr->cont_rect.left + item_ptr->virtual_rect.right;
        item_rect.top = FixHCoordinate(y_offset + item_ptr->virtual_rect.top);
        item_rect.bottom = FixHCoordinate(y_offset + item_ptr->virtual_rect.bottom);

        DrawRectInCtrl(richtext_ptr, item_rect, item_border_color);
    }

    return;
}


/*==============================================================================
Description: FillRectInCtrl
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void FillRectInCtrl(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    GUI_RECT_T rect,
    GUI_COLOR_T color
)
{
    CTRLBASE_OBJ_T* basectrl_ptr = (CTRLBASE_OBJ_T*)richtext_ptr;
    BOOLEAN need_draw = FALSE;
    GUI_RECT_T draw_rect = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return;
    }

    need_draw = GUI_IntersectRect(&draw_rect, rect, richtext_ptr->cont_rect);

    if (!need_draw) return;

    LCD_FillRect(&(basectrl_ptr->lcd_dev_info), draw_rect, color);

    return;
}


/*==============================================================================
Description: DrawRectInCtrl
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void DrawRectInCtrl(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    GUI_RECT_T rect,
    GUI_COLOR_T color
)
{
    CTRLBASE_OBJ_T* basectrl_ptr = (CTRLBASE_OBJ_T*)richtext_ptr;
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return;
    }

    //border������ռ�ÿռ�ģ���Ŀ����ϵͳԲ�ǿ������
    rect.left -= RICHTEXT_BORAD_WIDTH;
    rect.top -= RICHTEXT_BORAD_WIDTH;
    rect.right += RICHTEXT_BORAD_WIDTH;
    rect.bottom += RICHTEXT_BORAD_WIDTH;

    LCD_DrawShadowRoundedRect(&(basectrl_ptr->lcd_dev_info),
                              rect,
                              richtext_ptr->cont_rect,
                              color);

    return;
}

#ifdef TOUCH_PANEL_SUPPORT
/*==============================================================================
Description: HandleTP
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
  MMI_RESULT_FALSE - �н����л�
  MMI_RESULT_TRUE - �޽����л�
==============================================================================*/
LOCAL MMI_RESULT_E HandleTP(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    GUI_POINT_T point,
    BOOLEAN *is_on_focus_ptr
)
{
    int32 x_offset = 0;
    int32 y_offset = 0;
    int32 v_x = 0;
    int32 v_y = 0;
    RICHTEXT_ITEM_T *item_ptr = 0;
    BOOLEAN is_focus_switch = FALSE;
    int32 elem = 0;
    BOOLEAN is_gotten_point = FALSE;
    int32 p_line = 0;
    int32 p_pos = 0;
    int32 p_len = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    //û��item
    if (PNULL == richtext_ptr->first_item_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    //�ǽ���ģʽ
    if (GUIRICHTEXT_FOCUS_SHIFT_NONE == richtext_ptr->focus_mode
            || richtext_ptr->focus_mode >= GUIRICHTEXT_FOCUS_SHIFT_MAX)
    {
        return MMI_RESULT_FALSE;
    }

    x_offset = richtext_ptr->cont_rect.left;
    y_offset = richtext_ptr->cont_rect.top - richtext_ptr->disp_top;

    v_x = point.x - x_offset;
    v_y = point.y - y_offset;

    item_ptr = richtext_ptr->first_item_ptr;

    //��ȡ���item
    while (PNULL != item_ptr)
    {
        if (IsPointInBigRect(v_x, v_y, &(item_ptr->virtual_rect)))
        {
            break;
        }

        item_ptr = item_ptr->next_ptr;
    }

    //δ����item�����ߵ�ǰitem�޽��㣬���޽���Ԫ��
    if (PNULL == item_ptr
            || GUIRICHTEXT_TAG_NONE == item_ptr->data.orig_data.tag_type
            || 0 == item_ptr->focus_count)
    {
        if ((GUIRICHTEXT_FOCUS_SHIFT_BASIC == richtext_ptr->focus_mode)
                && (PNULL != richtext_ptr->focus_item_ptr))
        {
            richtext_ptr->focus_item_ptr = PNULL;
            is_focus_switch = TRUE;

            DrawRichText(richtext_ptr);
        }

        return (is_focus_switch ? MMI_RESULT_TRUE : MMI_RESULT_FALSE);
    }

    //��ȡ�����µĽ���Ԫ�أ�����û��
    SCI_ASSERT(PNULL != item_ptr->focus_elem); /*assert verified*/
    *is_on_focus_ptr = FALSE;

    //�½�һ��item������index
    if (richtext_ptr->focus_item_ptr != item_ptr)
    {
        item_ptr->focus_index = item_ptr->focus_count;
    }


    for (elem = 0; elem < item_ptr->focus_count && !(*is_on_focus_ptr); elem++)
    {
        switch (item_ptr->focus_elem[elem].tag_type)
        {
        case GUIRICHTEXT_TAG_TXT_NORMAL:

            if (IsPointInBigRect(v_x, v_y, &(item_ptr->data.txt_ext_data.disp_rect)))
            {
                if (item_ptr->focus_index != elem)
                {
                    item_ptr->focus_index = elem;
                    is_focus_switch = TRUE;
                }

                *is_on_focus_ptr = TRUE;
            }

            break;

        case GUIRICHTEXT_TAG_IMG_NORMAL:

            if (IsPointInBigRect(v_x, v_y, &(item_ptr->data.img_ext_data.disp_rect)))
            {
                if (item_ptr->focus_index != elem)
                {
                    item_ptr->focus_index = elem;
                    is_focus_switch = TRUE;
                }

                *is_on_focus_ptr = TRUE;
            }

            break;

        case GUIRICHTEXT_TAG_NORMAL:

            if (item_ptr->focus_index != elem)
            {
                item_ptr->focus_index = elem;
                is_focus_switch = TRUE;
            }

            *is_on_focus_ptr = TRUE;

            break;

        case GUIRICHTEXT_TAG_PHONENUM:
        case GUIRICHTEXT_TAG_EMAIL:
        case GUIRICHTEXT_TAG_URL:
        case GUIRICHTEXT_TAG_TXT_ALL:

            do
            {
                int32 real_top = item_ptr->data.txt_ext_data.disp_rect.top + richtext_ptr->txt_margin;
                uint16 str_len = 0;
                uint16 line_height = 0;
                GUI_RECT_T line_rect = {0};
                BOOLEAN is_find = 0;
                int32 nearest_chr = 0;
                int32 under_chr = 0;
                int32 under_symb_len = 0;

                p_line = v_y - real_top;

                if (p_line < 0) break;

                line_height = item_ptr->data.txt_ext_data.line_height + richtext_ptr->line_space;

                p_line /= line_height;

                //�˴��� y ��������ģ����ԣ�������㸳ֵ
                line_rect.left = item_ptr->data.txt_ext_data.disp_rect.left;
                line_rect.right = item_ptr->data.txt_ext_data.disp_rect.right;
                line_rect.top = 0;
                line_rect.bottom = line_rect.top + line_height - 1;

                str_len = item_ptr->data.txt_ext_data.line_start_pos_ptr[p_line + 1] - item_ptr->data.txt_ext_data.line_start_pos_ptr[p_line];

                is_find = GetTpPos(&line_rect,
                                   GetItemAlign(richtext_ptr, item_ptr),
                                   item_ptr->data.orig_data.text_data.buf.str_ptr + item_ptr->data.txt_ext_data.line_start_pos_ptr[p_line],
                                   str_len,
                                   (int16)v_x,
                                   GetItemFont(richtext_ptr, item_ptr),
                                   richtext_ptr->char_space,
                                   &nearest_chr,
                                   &under_chr,
                                   &under_symb_len);

                if (is_find)
                {
                    is_gotten_point = TRUE;

                    if (-1 != under_chr)
                    {
                        p_pos = under_chr;
                        p_len = under_symb_len;
                    }
                    else
                    {
                        p_pos = nearest_chr;
                        p_len = 1;
                    }
                }
            } while (0);

            //check the point info
            if (is_gotten_point)
            {
                if (p_line < item_ptr->focus_elem[elem].line_begin)
                {
                    break;
                }

                if (p_line == item_ptr->focus_elem[elem].line_begin)
                {
                    if (p_pos + p_len <= item_ptr->focus_elem[elem].pos_begin) break;
                }

                if (p_line == item_ptr->focus_elem[elem].line_end)
                {
                    if (p_pos > item_ptr->focus_elem[elem].pos_end) break;
                }

                if (p_line > item_ptr->focus_elem[elem].line_end)
                {
                    break;
                }

                if (item_ptr->focus_index != elem)
                {
                    item_ptr->focus_index = elem;
                    is_focus_switch = TRUE;
                }

                *is_on_focus_ptr = TRUE;
            }

            break;

        default:
            break;
        }

    }//for

    //�������޽���Ԫ��
    if (!(*is_on_focus_ptr))
    {
        if ((GUIRICHTEXT_FOCUS_SHIFT_BASIC == richtext_ptr->focus_mode)
                && (PNULL != richtext_ptr->focus_item_ptr))
        {
            richtext_ptr->focus_item_ptr = PNULL;
            is_focus_switch = TRUE;

            DrawRichText(richtext_ptr);
        }

        return (is_focus_switch ? MMI_RESULT_TRUE : MMI_RESULT_FALSE);
    }

    //���㱻�л�
    if (is_focus_switch)
    {
        if (richtext_ptr->focus_item_ptr != item_ptr)
        {
            richtext_ptr->focus_item_ptr = item_ptr;
        }

        DrawRichText(richtext_ptr);
    }

    return (is_focus_switch ? MMI_RESULT_TRUE : MMI_RESULT_FALSE);
}
#endif

/*==============================================================================
Description: HandleAppDown
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
  MMI_RESULT_FALSE - no focus switch
  MMI_RESULT_TRUE - focus switch
==============================================================================*/
LOCAL MMI_RESULT_E HandleAppDown(
    CTRLRICHTEXT_OBJ_T *richtext_ptr
)
{
    BOOLEAN is_focus_switch = FALSE;
    RICHTEXT_ITEM_T *next_focus_item_ptr = PNULL;
    uint8 next_focus_index = 0;
    RICHTEXT_FOCUS_DISP_E next_focus_disp = RICHTEXT_FOCUS_UNKNOW;
    RICHTEXT_FOCUS_DISP_E cur_focus_disp = RICHTEXT_FOCUS_UNKNOW;
    GUI_BIG_RECT_T focus_rect = {0};
    RICHTEXT_ITEM_T *cur_item_ptr = PNULL;
    MMI_RESULT_E scroll_result = MMI_RESULT_FALSE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (PNULL == richtext_ptr->first_item_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (GUIRICHTEXT_FOCUS_SHIFT_NONE == richtext_ptr->focus_mode
            || richtext_ptr->focus_mode >= GUIRICHTEXT_FOCUS_SHIFT_MAX)
    {
        ScrollDown(richtext_ptr, 0);
        return MMI_RESULT_FALSE;
    }

    cur_item_ptr = richtext_ptr->focus_item_ptr;

    //��ǰ�Ľ�����δ��ȫ��ʾ�����·�һҳ
    if (PNULL != cur_item_ptr)
    {
        cur_focus_disp = LocateFocusItem(richtext_ptr, cur_item_ptr, cur_item_ptr->focus_index, &focus_rect);

        if (RICHTEXT_FOCUS_OVER == cur_focus_disp
                || RICHTEXT_FOCUS_BOTTOM_OVER == cur_focus_disp)
        {
            ScrollDown(richtext_ptr, 0);
            return MMI_RESULT_FALSE;
        }
    }

    //��ȡnext_focus_item_ptr
    if (PNULL == cur_item_ptr)
    {
        cur_item_ptr = richtext_ptr->first_item_ptr;

        while (PNULL != cur_item_ptr)
        {
            if (cur_item_ptr->focus_count > 0)
            {
                next_focus_item_ptr = cur_item_ptr;
                break;
            }

            cur_item_ptr = cur_item_ptr->next_ptr;
        }
    }
    else if (cur_item_ptr->focus_index < cur_item_ptr->focus_count - 1)
    {
        next_focus_item_ptr = cur_item_ptr;
        next_focus_index = cur_item_ptr->focus_index + 1;
    }
    else if (PNULL != cur_item_ptr->next_ptr)
    {
        cur_item_ptr = cur_item_ptr->next_ptr;

        while (PNULL != cur_item_ptr)
        {
            if (cur_item_ptr->focus_count > 0)
            {
                next_focus_item_ptr = cur_item_ptr;
                break;
            }

            cur_item_ptr = cur_item_ptr->next_ptr;
        }
    }


    if (PNULL == next_focus_item_ptr)
    {
        //basic ��Ҫ��"����"֮���л�
        if ((GUIRICHTEXT_FOCUS_SHIFT_BASIC == richtext_ptr->focus_mode)
                && (PNULL != richtext_ptr->focus_item_ptr))
        {
            richtext_ptr->focus_item_ptr = PNULL;
            is_focus_switch = TRUE;
        }

        scroll_result = ScrollDown(richtext_ptr, 0);

        if (is_focus_switch && MMI_RESULT_TRUE != scroll_result)
        {
            DrawRichText(richtext_ptr);
        }

        return (is_focus_switch ? MMI_RESULT_TRUE : MMI_RESULT_FALSE);
    }

    next_focus_disp = LocateFocusItem(richtext_ptr, next_focus_item_ptr, next_focus_index, &focus_rect);

    switch (next_focus_disp)
    {
    case RICHTEXT_FOCUS_IN:
        richtext_ptr->focus_item_ptr = next_focus_item_ptr;
        richtext_ptr->focus_item_ptr->focus_index = next_focus_index;
        is_focus_switch = TRUE;
        DrawRichText(richtext_ptr);
        break;

    case RICHTEXT_FOCUS_OVER:
    case RICHTEXT_FOCUS_BOTTOM_OVER:
        richtext_ptr->focus_item_ptr = next_focus_item_ptr;
        richtext_ptr->focus_item_ptr->focus_index = next_focus_index;
        is_focus_switch = TRUE;
        ScrollToLine(richtext_ptr, focus_rect.top);
        break;

    case RICHTEXT_FOCUS_TOP_OUT:
    case RICHTEXT_FOCUS_TOP_OVER:
    case RICHTEXT_FOCUS_BOTTOM_OUT:
    case RICHTEXT_FOCUS_UNKNOW:
        ScrollDown(richtext_ptr, 0);
        break;

    default:
        break;
    }

    return (is_focus_switch ? MMI_RESULT_TRUE : MMI_RESULT_FALSE);
}


/*==============================================================================
Description: HandleAppUp
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
  MMI_RESULT_FALSE - no focus switch
  MMI_RESULT_TRUE - focus switch
==============================================================================*/
LOCAL MMI_RESULT_E HandleAppUp(
    CTRLRICHTEXT_OBJ_T *richtext_ptr
)
{
    BOOLEAN is_focus_switch = FALSE;
    RICHTEXT_ITEM_T *next_focus_item_ptr = PNULL;
    uint8 next_focus_index = 0;
    RICHTEXT_FOCUS_DISP_E next_focus_disp = RICHTEXT_FOCUS_UNKNOW;
    RICHTEXT_FOCUS_DISP_E cur_focus_disp = RICHTEXT_FOCUS_UNKNOW;
    GUI_BIG_RECT_T focus_rect = {0};
    RICHTEXT_ITEM_T *cur_item_ptr = PNULL;
    MMI_RESULT_E scroll_result = MMI_RESULT_FALSE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (PNULL == richtext_ptr->first_item_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (GUIRICHTEXT_FOCUS_SHIFT_NONE == richtext_ptr->focus_mode
            || richtext_ptr->focus_mode >= GUIRICHTEXT_FOCUS_SHIFT_MAX)
    {
        ScrollUp(richtext_ptr, 0);
        return MMI_RESULT_FALSE;
    }

    cur_item_ptr = richtext_ptr->focus_item_ptr;

    //��ǰ�Ľ�����δ��ȫ��ʾ�����·�һҳ
    if (PNULL != cur_item_ptr)
    {
        cur_focus_disp = LocateFocusItem(richtext_ptr, cur_item_ptr, cur_item_ptr->focus_index, &focus_rect);

        if (RICHTEXT_FOCUS_OVER == cur_focus_disp
                || RICHTEXT_FOCUS_TOP_OVER == cur_focus_disp)
        {
            ScrollUp(richtext_ptr, 0);
            return FALSE;
        }
    }

    //��ȡnext_focus_item_ptr
    if (PNULL == cur_item_ptr)
    {
        cur_item_ptr = richtext_ptr->first_item_ptr->prev_ptr;

        do
        {
            if (cur_item_ptr->focus_count > 0)
            {
                next_focus_item_ptr = cur_item_ptr;
                next_focus_index = cur_item_ptr->focus_count - 1;
                break;
            }

            cur_item_ptr = cur_item_ptr->prev_ptr;
        }
        while (cur_item_ptr != richtext_ptr->first_item_ptr->prev_ptr);
    }
    else if (cur_item_ptr->focus_index > 0)
    {
        next_focus_item_ptr = cur_item_ptr;
        next_focus_index = cur_item_ptr->focus_index - 1;
    }
    else if (PNULL != cur_item_ptr->prev_ptr && richtext_ptr->first_item_ptr != cur_item_ptr)
    {
        cur_item_ptr = cur_item_ptr->prev_ptr;

        while (PNULL != cur_item_ptr)
        {
            if (cur_item_ptr->focus_count > 0)
            {
                next_focus_item_ptr = cur_item_ptr;
                break;
            }

            if (richtext_ptr->first_item_ptr == cur_item_ptr)
            {
                break;
            }

            cur_item_ptr = cur_item_ptr->prev_ptr;

        }
    }

    if (PNULL == next_focus_item_ptr)
    {
        //basic ��Ҫ��"����"֮���л�
        if ((GUIRICHTEXT_FOCUS_SHIFT_BASIC == richtext_ptr->focus_mode)
                && (PNULL != richtext_ptr->focus_item_ptr))
        {
            richtext_ptr->focus_item_ptr = PNULL;
            is_focus_switch = TRUE;

        }

        scroll_result = ScrollUp(richtext_ptr, 0);

        if (is_focus_switch && MMI_RESULT_TRUE != scroll_result)
        {
            DrawRichText(richtext_ptr);
        }

        return (is_focus_switch ? MMI_RESULT_TRUE : MMI_RESULT_FALSE);
    }

    next_focus_disp = LocateFocusItem(richtext_ptr, next_focus_item_ptr, next_focus_index, &focus_rect);

    switch (next_focus_disp)
    {
    case RICHTEXT_FOCUS_IN:
        richtext_ptr->focus_item_ptr = next_focus_item_ptr;
        richtext_ptr->focus_item_ptr->focus_index = next_focus_index;
        is_focus_switch = TRUE;
        DrawRichText(richtext_ptr);
        break;

    case RICHTEXT_FOCUS_TOP_OVER:
    case RICHTEXT_FOCUS_OVER:
        richtext_ptr->focus_item_ptr = next_focus_item_ptr;
        richtext_ptr->focus_item_ptr->focus_index = next_focus_index;
        is_focus_switch = TRUE;
        ScrollToLine(richtext_ptr, (focus_rect.bottom - GetContHeight(richtext_ptr) + 1));
        break;

    case RICHTEXT_FOCUS_TOP_OUT:
    case RICHTEXT_FOCUS_BOTTOM_OVER:
    case RICHTEXT_FOCUS_BOTTOM_OUT:
    case RICHTEXT_FOCUS_UNKNOW:
        ScrollUp(richtext_ptr, 0);
        break;

    default:
        break;
    }

    return (is_focus_switch ? MMI_RESULT_TRUE : MMI_RESULT_FALSE);
}


/*==============================================================================
Description: GetTopCrossItem
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void GetTopCrossItem(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    int32 top,
    RICHTEXT_ITEM_T **item_pptr
)
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != item_pptr); /*assert verified*/
    if ((PNULL == richtext_ptr)||(PNULL == item_pptr))
    {
        return;
    }

    *item_pptr = richtext_ptr->first_item_ptr;

    while (PNULL != *item_pptr)
    {
        if ((*item_pptr)->virtual_rect.top < top)
        {
            if ((*item_pptr)->virtual_rect.bottom > top)
            {
                break;
            }
        }
        else
        {
            *item_pptr = PNULL;
            break;
        }

        *item_pptr = (*item_pptr)->next_ptr;
    }

    return;
}

/*==============================================================================
Description: GetBotCrossItem
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void GetBotCrossItem(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    int16 bottom,
    RICHTEXT_ITEM_T **item_pptr
)
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != item_pptr); /*assert verified*/
    if ((PNULL == richtext_ptr)||(PNULL == item_pptr))
    {
        return;
    }

    *item_pptr = richtext_ptr->first_item_ptr;

    if (PNULL != *item_pptr)
    {
        *item_pptr = (*item_pptr)->prev_ptr;
    }
    else
    {
        return;
    }

    while (PNULL != *item_pptr)
    {
        if ((*item_pptr)->virtual_rect.bottom > bottom)
        {
            if ((*item_pptr)->virtual_rect.top < bottom)
            {
                break;
            }
        }
        else
        {
            *item_pptr = PNULL;
            break;
        }

        *item_pptr = (*item_pptr)->prev_ptr;
    }

    return;
}



/*==============================================================================
Description: GetTxtFixHeight
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: ��ȡĳ��λ����ʾ��ʱ�򣬶�������ס�İ������ֵĸ߶�
==============================================================================*/
LOCAL int16 GetTxtFixHeight(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    int32 v_top,
    BOOLEAN get_hide
)
{
    RICHTEXT_ITEM_T *vf_item_ptr = PNULL;
    int32 hide_height = 0;
    int16 line_height = 0;
    int16 fix_height = 0;

    GetTopCrossItem(richtext_ptr,
                    v_top,
                    &vf_item_ptr);

    //û�п�Խtop��item
    if (PNULL == vf_item_ptr)
    {
        return 0;
    }

    //��Խtop��itemû������
    if (GUIRICHTEXT_TEXT_NONE == vf_item_ptr->data.orig_data.text_type)
    {
        return 0;
    }

    //���ֿ�û�п���top
    if (vf_item_ptr->data.txt_ext_data.disp_rect.top > v_top
        || vf_item_ptr->data.txt_ext_data.disp_rect.bottom < v_top)
    {
        return 0;
    }

    //����һ����ʾ������û��ʾ�����ĳ���
    hide_height = v_top - vf_item_ptr->data.txt_ext_data.disp_rect.top;
    hide_height -= richtext_ptr->txt_margin;

    //ֻ��marginû��ʾ
    if (hide_height <= 0)
    {
        return 0;
    }

    line_height = vf_item_ptr->data.txt_ext_data.line_height + richtext_ptr->line_space;

    //���һ�е�hide�߶�
    hide_height %= line_height;

    //δ��ʾ����С��һ�У��������м�࣬���ٹ���δ��ʾ�Ĳ���
    if (hide_height >= vf_item_ptr->data.txt_ext_data.line_height)
    {
        return 0;
    }

    if (get_hide)
    {
        fix_height = hide_height;
    }
    else
    {
        fix_height = vf_item_ptr->data.txt_ext_data.line_height - hide_height;
    }


    return fix_height;
}


/*==============================================================================
Description: ScrollDown
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
1 �õ����׵ľ��� a
2 �����򷵻� a == 0 (a<0,assert)
3 ��ȡĬ�����������룬����Ǽ򵥼�����Ĺ������� b
4 �����������룬��������
    1 ��֤�ϴ�δ������ʾ��item������ʾ b-
    2 ��֤��������������ʾ b-
5 ���ù�������Ϊmin(a,b)
==============================================================================*/
LOCAL MMI_RESULT_E ScrollDown(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    uint16 max_space
)
{
    int16 down_step = 0;
    int32 space_to_bottom = 0;
    RICHTEXT_ITEM_T *last_item_ptr = PNULL;
    int32 last_item_to_top = 0;
    int32 last_item_remain = 0;
    BOOLEAN need_fix_item = FALSE;

    if (PNULL == richtext_ptr->first_item_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    space_to_bottom = richtext_ptr->disp_height - 1 - GetDispBottom(richtext_ptr);

    if (space_to_bottom <= 0)
    {
        return MMI_RESULT_FALSE;
    }

    if (0 == max_space)
    {
        down_step = GetDefaultScrollDistance(richtext_ptr);
    }
    else
    {
        down_step = max_space;
    }

    //�����������룬��֤һ��������ʾȴ�������ɲ������������ֱ�������ʾ
    GetBotCrossItem(richtext_ptr,
                    GetDispBottom(richtext_ptr),
                    &last_item_ptr);

    // 1 ��item
    // 2 ��item������һ����ʾ
    // 3 Ҫ��ʾ��item���ƶ�����С�ڵ�ǰ���ƶ�����
    if (PNULL != last_item_ptr)
    {
        last_item_to_top = last_item_ptr->virtual_rect.top - richtext_ptr->disp_top;
        last_item_remain = last_item_ptr->virtual_rect.bottom - GetDispBottom(richtext_ptr);
        need_fix_item = (last_item_to_top >= last_item_remain && last_item_to_top < down_step);
    }

    if (need_fix_item)
    {
        down_step = last_item_to_top;
    }

    if (space_to_bottom < down_step)
    {
        down_step = space_to_bottom;
    }
    else if (!need_fix_item)
    {
        //�����ַ���������ʾ����
        int32 v_top = richtext_ptr->disp_top + down_step;
        int16 hide_height = GetTxtFixHeight(richtext_ptr, v_top, TRUE);

        if (hide_height > 0)
        {
            down_step -= hide_height;
        }
    }

    richtext_ptr->disp_top += down_step;

    //�ػ�����
    DrawRichText(richtext_ptr);

    return MMI_RESULT_TRUE;
}


/*==============================================================================
Description: ScrollUp
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: ���¹�����max_space==0����һ��
==============================================================================*/
LOCAL MMI_RESULT_E ScrollUp(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    uint16 max_space
)
{
    int32 space_to_top = 0;
    int16 up_step = 0;
    RICHTEXT_ITEM_T *first_item_ptr = PNULL;
    int32 first_item_to_bottom = 0;
    int32 first_item_remain = 0;
    BOOLEAN need_fix_item = FALSE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (PNULL == richtext_ptr->first_item_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    space_to_top = richtext_ptr->disp_top;

    if (space_to_top <= 0)
    {
        return MMI_RESULT_FALSE;
    }

    if (0 == max_space)
    {
        up_step = GetDefaultScrollDistance(richtext_ptr);
    }
    else
    {
        up_step = max_space;
    }

    //�����������룬��֤һ��������ʾȴ�������ɲ������������ֱ�������ʾ
    GetTopCrossItem(richtext_ptr, richtext_ptr->disp_top, &first_item_ptr);

    // 1 ��item
    // 2 ��item������һ����ʾ
    // 3 Ҫ��ʾ��item���ƶ�����С�ڵ�ǰ���ƶ�����
    if (PNULL != first_item_ptr)
    {
        first_item_to_bottom = GetDispBottom(richtext_ptr) - first_item_ptr->virtual_rect.bottom;
        first_item_remain = richtext_ptr->disp_top - first_item_ptr->virtual_rect.top;
        need_fix_item = (first_item_to_bottom >= first_item_remain && first_item_to_bottom < up_step);
    }


    if (need_fix_item)
    {
        up_step = first_item_to_bottom;
    }

    if (space_to_top < up_step)
    {
        up_step = space_to_top;
    }
    else if (!need_fix_item)
    {
        //�����ַ���������ʾ����
        int32 v_top = richtext_ptr->disp_top - up_step;
        int16 show_height = GetTxtFixHeight(richtext_ptr, v_top, FALSE);

        if (show_height > 0)
        {
            up_step -= show_height;
        }
    }

    richtext_ptr->disp_top -= up_step;

    //�ػ�����
    DrawRichText(richtext_ptr);

    return MMI_RESULT_TRUE;
}


/*==============================================================================
Description: ScrollToLine
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL MMI_RESULT_E ScrollToLine(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    int32 top
)
{
    int16 page_height = 0;
    int32 new_bottom = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (top < 0) top = 0;

    page_height = GetContHeight(richtext_ptr);
    new_bottom = MIN(richtext_ptr->disp_height, top + page_height) - 1;

    if (GetDispBottom(richtext_ptr) == new_bottom)
    {
        return MMI_RESULT_FALSE;
    }

    richtext_ptr->disp_top = new_bottom - page_height + 1;

    //�ػ�����
    DrawRichText(richtext_ptr);

    return MMI_RESULT_TRUE;
}


/*==============================================================================
Description: CalcItemExtData
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL MMI_RESULT_E CalcItemExtData(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    RICHTEXT_ITEM_T *item_ptr
)
{
    BOOLEAN is_img_none = FALSE;
    BOOLEAN is_txt_none = FALSE;
    BOOLEAN is_big_img = FALSE;

    MMI_RESULT_E result = MMI_RESULT_FALSE;
    uint16 img_width = 0;
    uint16 img_height = 0;
    uint16 disp_width = 0;
    uint16 disp_height = 0;

    uint16 text_width = 0;
    int32 text_height = 0;
    uint32 line_num = 0;
    uint8 font_height = 0;
    uint8 font_width = 0;
    BOOLEAN is_height_get_max = FALSE; //�Ƿ��ȡͼ������ֵ����߶���Ϊitem�ĸ߶�

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if ((PNULL == richtext_ptr)||(PNULL == item_ptr))
    {
        return MMI_RESULT_FALSE;
    }

    font_height = GUI_GetFontHeight(richtext_ptr->font, UNICODE_HANZI);

    is_img_none = (GUIRICHTEXT_IMAGE_NONE == item_ptr->data.orig_data.img_type);
    is_txt_none = (GUIRICHTEXT_TEXT_NONE == item_ptr->data.orig_data.text_type);

    //����
    item_ptr->width = GetItemWidth(richtext_ptr);
    item_ptr->height = 0;

    if (is_img_none && is_txt_none)
    {
        //Ӧ��Ҫ��յ�item�������ĵ�����ͼ��itemͬ�߶�
        //û����ʾԪ�ص�ʱ����ʾһ�����еĿհ� - ��ΪһЩӦ�õ��ر��Ҫ��
        item_ptr->width += 2 * richtext_ptr->item_border_space;
        item_ptr->height = font_height;
        item_ptr->height += 2 * richtext_ptr->item_border_space;
        item_ptr->height += 2 * richtext_ptr->txt_margin;

        return MMI_RESULT_TRUE;
    }

    //����ext data
    //image ext data
    if (!is_img_none)
    {
        //ȡ��ͼƬ��Ϣ
        result = CalcImgWidthHeight(richtext_ptr,
                                    item_ptr->data.orig_data.img_type,
                                    &(item_ptr->data.orig_data.img_data),
                                    &img_width,
                                    &img_height);

        if (MMI_RESULT_TRUE != result)
        {
            item_ptr->data.orig_data.img_type = GUIRICHTEXT_IMAGE_INVALID;
            return MMI_RESULT_FALSE;
        }

        //orig size
        item_ptr->data.img_ext_data.orig_width = img_width;
        item_ptr->data.img_ext_data.orig_height = img_height;

        //disp size
        if (item_ptr->data.orig_data.img_set_size)
        {
            disp_width = item_ptr->data.orig_data.img_setting_width;
            disp_height = item_ptr->data.orig_data.img_setting_height;
        }
        else
        {
            disp_width = item_ptr->data.img_ext_data.orig_width;
            disp_height = item_ptr->data.img_ext_data.orig_height;
        }

        //������ȹ����ͼƬ��ʾ����
        is_big_img = (disp_width > item_ptr->width);

        if (is_big_img)
        {
            item_ptr->data.img_ext_data.disp_width = item_ptr->width;
            item_ptr->data.img_ext_data.disp_height = (uint16)(disp_height * disp_width / item_ptr->width);
        }
        else
        {
            item_ptr->data.img_ext_data.disp_width = disp_width;
            item_ptr->data.img_ext_data.disp_height = disp_height;
        }

        if (item_ptr->data.img_ext_data.disp_height > GetItemHieght(richtext_ptr))
        {
            item_ptr->data.img_ext_data.disp_height = GetItemHieght(richtext_ptr);
        }

        item_ptr->height += item_ptr->data.img_ext_data.disp_height;
    } // if !is_img_none

    //text ext data
    if (!is_txt_none)
    {
        //����text width
        if (GUIRICHTEXT_IMAGE_NONE != item_ptr->data.orig_data.img_type)
        {
            if (item_ptr->data.orig_data.align_type >= GUIRICHTEXT_ALIGN_INVALID)
            {
                item_ptr->data.orig_data.align_type = GUIRICHTEXT_ALIGN_DEFAULT;
            }

            //����text width
            switch (item_ptr->data.orig_data.align_type)
            {
            case GUIRICHTEXT_ALIGN_DOWN:
            case GUIRICHTEXT_ALIGN_UP:
                text_width = item_ptr->width;
                is_height_get_max = FALSE;
                break;

            case GUIRICHTEXT_ALIGN_DEFAULT: //as right or left
            case GUIRICHTEXT_ALIGN_REVERSE: //as right or left
            case GUIRICHTEXT_ALIGN_RIGHT:
            case GUIRICHTEXT_ALIGN_LEFT:
            default:
                text_width = item_ptr->width - item_ptr->data.img_ext_data.disp_width;
                text_width -= richtext_ptr->img_text_space;

                is_big_img = (text_width - 2 * richtext_ptr->txt_margin < font_width);

                //�Զ��������з�ʽ����resize rect�󽫲��ɻָ�
                //��Ϊ���������е�ʱ���п���ʣ�µĿռ�һ���ֶ��Ų���
                if (is_big_img)
                {
                    item_ptr->data.orig_data.align_type = GUIRICHTEXT_ALIGN_DOWN;
                    is_height_get_max = FALSE;
                }
                else
                {
                    is_height_get_max = TRUE;
                }

                break;
            }
        }
        else
        {
            is_height_get_max = TRUE;
            text_width = item_ptr->width;
        }

        //ȥ��ҳ�߾�
        text_width -= 2 * richtext_ptr->txt_margin;

        //text info
        if (text_width > 0)
        {
            uint16 pos = 0;
            BOOLEAN is_valid_line = FALSE;
            wchar *str_ptr = PNULL;
            uint16 str_len = 0;
            uint16 *start_pos_ptr = PNULL;
            uint16 char_num = 0;
            uint16 pixel_num = 0;

            //��ʱӦ�ö��Ѿ�ת������bufferģʽ���ַ���
            SCI_ASSERT(GUIRICHTEXT_TEXT_BUF == item_ptr->data.orig_data.text_type); /*assert verified*/

            str_ptr = item_ptr->data.orig_data.text_data.buf.str_ptr;
            str_len = item_ptr->data.orig_data.text_data.buf.len;
            start_pos_ptr = item_ptr->data.txt_ext_data.line_start_pos_ptr;

            while (pos < str_len)
            {
                //ȡһ�е���Ϣ
                is_valid_line = GUI_GetStringInfoInLine(str_ptr + pos,
                                                        str_len - pos,
                                                        GetItemFont(richtext_ptr, item_ptr),
                                                        richtext_ptr->char_space,
                                                        text_width,
                                                        &char_num,
                                                        &pixel_num,
                                                        TRUE);

                //ȡ��Ϣʧ�ܣ�����һ���ַ���Ϊ�㣬��Ƿ�
                if (!is_valid_line || 0 == char_num)
                {
                    line_num = 0;
                    break;
                }

                //��¼ÿ�е���㣬�յ�����һ�е����-1
                start_pos_ptr[line_num] = pos;

                pos += char_num;

                line_num++;
            }

            //���һ�е��յ�洢�����һ�к���һ�У�����ͳһ����
            start_pos_ptr[line_num] = str_len;
        }//text info

        //���պ��ֵĸ߶�ȷ���ָ�
        font_height = GUI_GetFontHeight(GetItemFont(richtext_ptr, item_ptr), UNICODE_HANZI);

        item_ptr->data.txt_ext_data.line_num = line_num;
        item_ptr->data.txt_ext_data.line_height = font_height;

        if (item_ptr->data.txt_ext_data.line_num > 0)
        {
            //ÿһ�ж�����line_space���������һ�У�������ʾ��Ч�������»���
            text_height = (font_height + richtext_ptr->line_space) * line_num;
            text_height += 2 * richtext_ptr->txt_margin;

            if (is_height_get_max)
            {
                item_ptr->height = MAX(item_ptr->height, text_height);
            }
            else
            {
                item_ptr->height += richtext_ptr->img_text_space;
                item_ptr->height += text_height;
            }
        }
    } // if !is_txt_none

    if (GUIRICHTEXT_FOCUS_SHIFT_NONE != richtext_ptr->focus_mode
            && GUIRICHTEXT_TEXT_NONE != item_ptr->data.orig_data.text_type)
    {
        FormatAbsPos(item_ptr);
    }

    if (0 == item_ptr->height)
    {
        item_ptr->height = font_height;
    }

    //�ڼ����ʼ���ø߶ȵ�ʱ��Ŷ���ǽ�item borderȥ����
    item_ptr->width += 2 * richtext_ptr->item_border_space;
    item_ptr->height += 2 * richtext_ptr->item_border_space;

    richtext_ptr->need_cal_rect = TRUE;

    return MMI_RESULT_TRUE;
}


/*==============================================================================
Description: SetItemFocusElememt
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL MMI_RESULT_E SetItemFocusElememt(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    RICHTEXT_ITEM_T *item_ptr
)
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if ((PNULL == richtext_ptr)||(PNULL == item_ptr))
    {
        return MMI_RESULT_FALSE;
    }

    if (GUIRICHTEXT_FOCUS_SHIFT_NONE == richtext_ptr->focus_mode)
    {
        return MMI_RESULT_FALSE;
    }

    SCI_ASSERT(PNULL == item_ptr->focus_header_ptr); /*assert verified*/
    SCI_ASSERT(PNULL == item_ptr->focus_tailer_ptr); /*assert verified*/

    //reset the count
    ReleaseFocusItem(item_ptr);

    //���役��
    if (GUIRICHTEXT_TAG_NORMAL == item_ptr->data.orig_data.tag_type)
    {
        AddFocusElement(item_ptr, GUIRICHTEXT_TAG_NORMAL, 0, 0);
    }
    else
    {
        //img
        if (GUIRICHTEXT_IMAGE_NONE != item_ptr->data.orig_data.img_type)
        {
            switch (item_ptr->data.orig_data.tag_type & GUIRICHTEXT_TAG_IMG_MASK) /*lint !e655*/
            {
            case GUIRICHTEXT_TAG_IMG_NORMAL:
                AddFocusElement(item_ptr, GUIRICHTEXT_TAG_IMG_NORMAL, 0, 0);
                break;

            default:
                break;
            }
        }

        //text
        if ((GUIRICHTEXT_TEXT_NONE != item_ptr->data.orig_data.text_type))
        {
            switch (item_ptr->data.orig_data.tag_type & GUIRICHTEXT_TAG_TXT_MASK) /*lint !e655*/
            {
            case GUIRICHTEXT_TAG_TXT_NORMAL:
            case GUIRICHTEXT_TAG_TXT_ALL:
            case GUIRICHTEXT_TAG_PHONENUM:
            case GUIRICHTEXT_TAG_EMAIL:
            case GUIRICHTEXT_TAG_URL:
                ParseTagString(item_ptr);
                break;

            case GUIRICHTEXT_TAG_HYPER:
                ParseHyperString(item_ptr);
                break;

            default:

                if (GUIRICHTEXT_TAG_PARSE & (item_ptr->data.orig_data.tag_type & GUIRICHTEXT_TAG_TXT_MASK)) /*lint !e655*/
                {
                    ParseGeneralString(item_ptr);
                }

                break;
            }
        }
    }

    CombineFocusElement(item_ptr);

    return MMI_RESULT_TRUE;
}


/*==============================================================================
Description: CalcImgWidthHeight
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL MMI_RESULT_E CalcImgWidthHeight(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    GUIRICHTEXT_IMAGE_TYPE_E type,
    GUIRICHTEXT_DATA_IMAGE_U const *data,
    uint16 *width_ptr,
    uint16 *height_ptr
)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (type)
    {
    case GUIRICHTEXT_IMAGE_RES:
        {
            BOOLEAN get_img_ok = FALSE;
            uint16 img_width = 0;
            uint16 img_height = 0;

            get_img_ok = GUIRES_GetImgWidthHeight(&img_width,
                                                  &img_height,
                                                  data->res.id,
                                                  richtext_ptr->win_id);

            if (get_img_ok)
            {
                *width_ptr = img_width;
                *height_ptr = img_height;

                result = MMI_RESULT_TRUE;
            }
        }

        break;
    case GUIRICHTEXT_IMAGE_BUF:

        if (data->buf.width > 0 && data->buf.height > 0)
        {
            *width_ptr = data->buf.width;
            *height_ptr = data->buf.height;
        }
        else
        {
            *width_ptr = GetItemWidth(richtext_ptr);
            *height_ptr = GetImgDefaultHieght(richtext_ptr);
        }

        result = MMI_RESULT_TRUE;
        break;

    case GUIRICHTEXT_IMAGE_FILE:
        *width_ptr = GetItemWidth(richtext_ptr);
        *height_ptr = GetImgDefaultHieght(richtext_ptr);
        result = MMI_RESULT_TRUE;
        break;

    default:
        break;
    }

    return result;
}


/*==============================================================================
Description: CalcVirtualRect
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void CalcVirtualRect(
    CTRLRICHTEXT_OBJ_T *richtext_ptr
)
{
    CTRLBASE_OBJ_T* basectrl_ptr = (CTRLBASE_OBJ_T*)richtext_ptr;
    RICHTEXT_ITEM_T *item_ptr = PNULL;
    BOOLEAN need_scroll_in_fact = FALSE;
    int16 page_height = 0;
    int32 set_scroll_times = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return;
    }

    do
    {
        item_ptr = richtext_ptr->first_item_ptr;

        if (PNULL == item_ptr)
        {
            return;
        }

        //���������border space
        richtext_ptr->disp_height = richtext_ptr->border_space;

        //calc virtual_rect
        while (PNULL != item_ptr)
        {
            if (richtext_ptr->need_cal_ext_data)
            {
                CalcItemExtData(richtext_ptr, item_ptr);
            }

            item_ptr->virtual_rect.left = richtext_ptr->border_space;
            item_ptr->virtual_rect.right = item_ptr->virtual_rect.left + item_ptr->width - 1;
            item_ptr->virtual_rect.top = richtext_ptr->disp_height - 1;
            item_ptr->virtual_rect.bottom = item_ptr->virtual_rect.top + item_ptr->height - 1;

            richtext_ptr->disp_height += item_ptr->height;
            richtext_ptr->disp_height += richtext_ptr->item_space;

            CalcItemVirtualRect(richtext_ptr, item_ptr);

            item_ptr = item_ptr->next_ptr;
        }

        //�����һ�μ��ϵĵ�Ԫ���ȥ��
        richtext_ptr->disp_height -= richtext_ptr->item_space;
        //�����±ߵ�border
        richtext_ptr->disp_height += richtext_ptr->border_space;

        //calc virtual_rect_disp
        page_height = GetContHeight(richtext_ptr);

        need_scroll_in_fact = (page_height < richtext_ptr->disp_height);

        if (richtext_ptr->need_scroll != need_scroll_in_fact)
        {
            //������ַ������ù��������϶�Ҫ������ѭ��
            SCI_ASSERT(0 == set_scroll_times); /*assert verified*/
            set_scroll_times++;

            SetData(richtext_ptr, RICHTEXT_SETTINGS_SCROLL_BAR, &need_scroll_in_fact);
            //���¼���
            continue;
        }

        if (richtext_ptr->need_scroll)
        {
            if (PNULL == richtext_ptr->prgbox_ptr)
            {
                MMI_CONTROL_CREATE_T create = {0};
                GUIPRGBOX_INIT_DATA_T init_data = {0};

                init_data.lcd_id = basectrl_ptr->lcd_dev_info.lcd_id;
                init_data.both_rect.h_rect = init_data.both_rect.v_rect = richtext_ptr->scroll_rect;
                init_data.style = GUIPRGBOX_STYLE_VERTICAL_WINDOWS;
                init_data.is_forbid_paint = TRUE;

                create.ctrl_id = 0;
                create.guid = SPRD_GUI_PRGBOX_ID;
                create.parent_win_handle = richtext_ptr->win_id;
                create.parent_ctrl_handle = basectrl_ptr->handle;
                create.init_data_ptr = &init_data;

                richtext_ptr->prgbox_ptr = MMK_CreateControl(&create);
            }

            GUIPRGBOX_SetParamByPtr(richtext_ptr->prgbox_ptr,
                                    richtext_ptr->disp_height,
                                    GetContHeight(richtext_ptr));
        }

        richtext_ptr->need_cal_ext_data = FALSE;
        richtext_ptr->need_cal_rect = FALSE;

    }
    while (richtext_ptr->need_cal_rect || richtext_ptr->need_cal_ext_data);

    if (!richtext_ptr->is_info_protected)
    {
        richtext_ptr->disp_top = 0;
    }

    return;
}


/*==============================================================================
Description: CalcItemVirtualRect
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: ����item�ڲ���image��text�����virtual rect�ľ���λ��
==============================================================================*/
LOCAL void CalcItemVirtualRect(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    RICHTEXT_ITEM_T *item_ptr
)
{
    GUI_BIG_RECT_T item_disp_rect = {0};
    int16 text_height = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if ((PNULL == richtext_ptr)||(PNULL == item_ptr))
    {
        return;
    }

    item_disp_rect = item_ptr->virtual_rect;
    //�����߿�
    item_disp_rect.top += richtext_ptr->item_border_space;
    item_disp_rect.bottom -= richtext_ptr->item_border_space;
    item_disp_rect.left += richtext_ptr->item_border_space;
    item_disp_rect.right -= richtext_ptr->item_border_space;

    //sub virtual text
    if (GUIRICHTEXT_IMAGE_NONE == item_ptr->data.orig_data.img_type)
    {
        SCI_MEMSET(&(item_ptr->data.img_ext_data.disp_rect), 0, sizeof(GUI_BIG_RECT_T));
        item_ptr->data.txt_ext_data.disp_rect = item_disp_rect;
    }
    else if (GUIRICHTEXT_TEXT_NONE == item_ptr->data.orig_data.text_type)
    {
        GUIRICHTEXT_IMG_ALIGN_T align = GetImgAlign(richtext_ptr, item_ptr);

        item_ptr->data.img_ext_data.disp_rect = item_disp_rect;
        SCI_MEMSET(&(item_ptr->data.txt_ext_data.disp_rect), 0, sizeof(GUI_BIG_RECT_T));

        switch (align)
        {
        case GUIRICHTEXT_IMG_ALIGN_LEFT:
            item_ptr->data.img_ext_data.disp_rect.right = item_ptr->data.img_ext_data.disp_rect.left + item_ptr->data.img_ext_data.disp_width - 1;
            break;

        case GUIRICHTEXT_IMG_ALIGN_RIGHT:
            item_ptr->data.img_ext_data.disp_rect.left = item_ptr->data.img_ext_data.disp_rect.right - item_ptr->data.img_ext_data.disp_width + 1;
            break;

        default:
            break;
        }
    }
    else
    {
        //��ͼ������
        text_height = (item_ptr->data.txt_ext_data.line_height + richtext_ptr->line_space) * item_ptr->data.txt_ext_data.line_num
                      + richtext_ptr->txt_margin * 2;
        item_ptr->data.img_ext_data.disp_rect = item_disp_rect;
        item_ptr->data.txt_ext_data.disp_rect = item_disp_rect;

        if (GUIRICHTEXT_ALIGN_DEFAULT == item_ptr->data.orig_data.align_type)
        {
            if (ALIGN_RIGHT == richtext_ptr->disp_align)
            {
                item_ptr->data.orig_data.align_type = GUIRICHTEXT_ALIGN_LEFT;
            }
            else
            {
                item_ptr->data.orig_data.align_type = GUIRICHTEXT_ALIGN_RIGHT;
            }
        }
        else if (GUIRICHTEXT_ALIGN_REVERSE == item_ptr->data.orig_data.align_type)
        {
            if (ALIGN_RIGHT == richtext_ptr->disp_align)
            {
                item_ptr->data.orig_data.align_type = GUIRICHTEXT_ALIGN_RIGHT;
            }
            else
            {
                item_ptr->data.orig_data.align_type = GUIRICHTEXT_ALIGN_LEFT;
            }
        }

        switch (item_ptr->data.orig_data.align_type)
        {
        case GUIRICHTEXT_ALIGN_RIGHT:
            item_ptr->data.img_ext_data.disp_rect.right =
                item_ptr->data.img_ext_data.disp_rect.left + item_ptr->data.img_ext_data.disp_width - 1;
            item_ptr->data.img_ext_data.disp_rect.bottom =
                item_ptr->data.img_ext_data.disp_rect.top + item_ptr->data.img_ext_data.disp_height - 1;

            item_ptr->data.txt_ext_data.disp_rect.left =
                item_ptr->data.img_ext_data.disp_rect.right + richtext_ptr->img_text_space;
            item_ptr->data.txt_ext_data.disp_rect.bottom =
                item_ptr->data.txt_ext_data.disp_rect.top + text_height - 1;
            break;

        case GUIRICHTEXT_ALIGN_LEFT:
            item_ptr->data.img_ext_data.disp_rect.left =
                item_ptr->data.img_ext_data.disp_rect.right - item_ptr->data.img_ext_data.disp_width + 1;
            item_ptr->data.img_ext_data.disp_rect.bottom =
                item_ptr->data.img_ext_data.disp_rect.top + item_ptr->data.img_ext_data.disp_height - 1;

            item_ptr->data.txt_ext_data.disp_rect.right =
                item_ptr->data.img_ext_data.disp_rect.left - richtext_ptr->img_text_space;
            item_ptr->data.txt_ext_data.disp_rect.bottom =
                item_ptr->data.txt_ext_data.disp_rect.top + text_height - 1;

            break;

        case GUIRICHTEXT_ALIGN_DOWN:
            item_ptr->data.img_ext_data.disp_rect.right =
                item_ptr->data.img_ext_data.disp_rect.left + item_ptr->data.img_ext_data.disp_width - 1;
            item_ptr->data.img_ext_data.disp_rect.bottom =
                item_ptr->data.img_ext_data.disp_rect.top + item_ptr->data.img_ext_data.disp_height - 1;

            item_ptr->data.txt_ext_data.disp_rect.top =
                item_ptr->data.img_ext_data.disp_rect.bottom + richtext_ptr->img_text_space;
            item_ptr->data.txt_ext_data.disp_rect.bottom =
                item_ptr->data.txt_ext_data.disp_rect.top + text_height - 1;

            break;

        case GUIRICHTEXT_ALIGN_UP:
            item_ptr->data.img_ext_data.disp_rect.right =
                item_ptr->data.img_ext_data.disp_rect.left + item_ptr->data.img_ext_data.disp_width - 1;
            item_ptr->data.img_ext_data.disp_rect.top =
                item_ptr->data.img_ext_data.disp_rect.bottom - item_ptr->data.img_ext_data.disp_height + 1;

            item_ptr->data.txt_ext_data.disp_rect.bottom =
                item_ptr->data.txt_ext_data.disp_rect.top + text_height - 1;

            break;

        default:
            SCI_ASSERT(0); /*assert verified*/
            break;
        }
    }

    return;
}


/*==============================================================================
Description: ����ָ������Ԫ�ص����rect
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: locate��һ������
==============================================================================*/
LOCAL RICHTEXT_FOCUS_DISP_E LocateFocusItem(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    RICHTEXT_ITEM_T const *item_ptr,
    uint16 focus_index,
    GUI_BIG_RECT_T *focus_rect
)
{
    BOOLEAN is_top_up = FALSE;
    BOOLEAN is_top_in = FALSE;
    BOOLEAN is_top_down = FALSE;
    BOOLEAN is_bottom_up = FALSE;
    BOOLEAN is_bottom_in = FALSE;
    BOOLEAN is_bottom_down = FALSE;
    RICHTEXT_FOCUS_DISP_E place = RICHTEXT_FOCUS_UNKNOW;
    int16 line_height = 0;
    int16 disp_lines = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if ((PNULL == richtext_ptr)||(PNULL == item_ptr))
    {
        return RICHTEXT_FOCUS_UNKNOW;
    }

    if (focus_index >= item_ptr->focus_count || 0 == item_ptr->focus_count)
    {
        return RICHTEXT_FOCUS_UNKNOW;
    }

    SCI_ASSERT(0 == item_ptr->focus_count || PNULL != item_ptr->focus_elem); /*assert verified*/

    //ȷ�����㵥Ԫ��full rect
    if (GUIRICHTEXT_TAG_NORMAL == item_ptr->focus_elem[focus_index].tag_type)
    {
        *focus_rect = item_ptr->virtual_rect;
    }
    else if (item_ptr->focus_elem[focus_index].tag_type & GUIRICHTEXT_TAG_IMG_MASK) /*lint !e655*/
    {
        *focus_rect = item_ptr->data.img_ext_data.disp_rect;
    }
    else if (item_ptr->focus_elem[focus_index].tag_type & GUIRICHTEXT_TAG_TXT_MASK) /*lint !e655*/
    {
        if (GUIRICHTEXT_TAG_TXT_NORMAL == item_ptr->focus_elem[focus_index].tag_type)
        {
            *focus_rect = item_ptr->data.txt_ext_data.disp_rect;
        }
        else
        {
            line_height = item_ptr->data.txt_ext_data.line_height + richtext_ptr->line_space;
            disp_lines = item_ptr->focus_elem[focus_index].line_end - item_ptr->focus_elem[focus_index].line_begin + 1;
            *focus_rect = item_ptr->data.txt_ext_data.disp_rect;
            focus_rect->top += item_ptr->focus_elem[focus_index].line_begin * line_height;
            focus_rect->bottom = focus_rect->top + disp_lines * line_height - 1;
            focus_rect->bottom -= richtext_ptr->line_space;
        }
    }
    else
    {
        return RICHTEXT_FOCUS_UNKNOW;
    }

    //�ж����㵥Ԫ����ʾ�������Թ�ϵ
    //top
    if (focus_rect->top < richtext_ptr->disp_top)
    {
        is_top_up = TRUE;
    }
    else if (focus_rect->top > GetDispBottom(richtext_ptr))
    {
        is_top_down = TRUE;
    }
    else
    {
        is_top_in = TRUE;
    }

    //bottom
    if (focus_rect->bottom < richtext_ptr->disp_top)
    {
        is_bottom_up = TRUE;
    }
    else if (focus_rect->bottom > GetDispBottom(richtext_ptr))
    {
        is_bottom_down = TRUE;
    }
    else
    {
        is_bottom_in = TRUE;
    }

    //�ۺ�top&bottom������
    if (is_bottom_up)
    {
        place = RICHTEXT_FOCUS_TOP_OUT;
    }
    else if (is_top_up && is_bottom_in)
    {
        place = RICHTEXT_FOCUS_TOP_OVER;
    }
    else if (is_top_in && is_bottom_in)
    {
        place = RICHTEXT_FOCUS_IN;
    }
    else if (is_top_up && is_bottom_down)
    {
        place = RICHTEXT_FOCUS_OVER;
    }
    else if (is_top_in && is_bottom_down)
    {
        place = RICHTEXT_FOCUS_BOTTOM_OVER;
    }
    else if (is_top_down)
    {
        place = RICHTEXT_FOCUS_BOTTOM_OUT;
    }

    return place;
}


/*==============================================================================
Description: ParseTagString
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �����ָ�������ͣ������ж�ֱ������Ϊ�����ͣ�����һ��������
==============================================================================*/
LOCAL MMI_RESULT_E ParseTagString(
    RICHTEXT_ITEM_T *item_ptr
)
{
    uint16 len = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if (PNULL == item_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    //�������
    SCI_ASSERT(GUIRICHTEXT_TEXT_BUF == item_ptr->data.orig_data.text_type); /*assert verified*/
    len = item_ptr->data.orig_data.text_data.buf.len;

    AddFocusElement(item_ptr,
                    item_ptr->data.orig_data.tag_type & GUIRICHTEXT_TAG_TXT_MASK, /*lint !e655*/
                    0,
                    (len > 0 ? len - 1 : 0));

    return MMI_RESULT_TRUE;
}


/*==============================================================================
Description: �������ı��ִ�
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
  ��ʶ�����¼򵥵��Զ����ǩ��ֻ���嵥�ַ���ǩ���򻯴���
  <s>...</s> string
  <p>...</p> phone number
  <u>...</u> url
  <e>...</e> email
  ��"\"��ת�壬��Ҫת����ַ���: "\<>"
  �������ʧ�ܣ���ԭ����ʾ
==============================================================================*/
LOCAL MMI_RESULT_E ParseHyperString(
    RICHTEXT_ITEM_T *item_ptr
)
{
    wchar *src_str_ptr = PNULL;
    wchar *dst_str_ptr = PNULL;
    int16 src_len = 0;
    int16 dst_len = 0;
    GUIRICHTEXT_TAG_TYPE_E hyper_state = GUIRICHTEXT_TAG_NONE;
    wchar *p = PNULL;
    wchar *search_p = 0;
    int16 pos = 0;
    int16 search_pos = 0;
    int16 i = 0;
    BOOLEAN has_got_tags = FALSE;
    GUIRICHTEXT_TAG_TYPE_E tag_type;
    uint16 abs_begin;


    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if (PNULL == item_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (GUIRICHTEXT_TAG_HYPER != (item_ptr->data.orig_data.tag_type & GUIRICHTEXT_TAG_TXT_MASK)) /*lint !e655*/
    {
        return MMI_RESULT_FALSE;
    }

    //�������
    SCI_ASSERT(GUIRICHTEXT_TEXT_BUF == item_ptr->data.orig_data.text_type); /*assert verified*/

    dst_str_ptr = item_ptr->data.orig_data.text_data.buf.str_ptr;
    dst_len = item_ptr->data.orig_data.text_data.buf.len;

    if (PNULL == dst_str_ptr || 0 == dst_len)
    {
        return MMI_RESULT_FALSE;
    }

    //�Ƚ�Ŀ���ִ����Ƶ�Դ�ִ�
    src_len = dst_len;
    src_str_ptr = (wchar*)SCI_ALLOC_APP((uint16)src_len * sizeof(wchar));
    MMI_WSTRNCPY(src_str_ptr, src_len, dst_str_ptr, dst_len, src_len);
    //���Ŀ���ִ�
    dst_len = 0;

    for (pos = 0; pos < src_len;/* no code, in the body */)
    {
        if (item_ptr->focus_count >= RICHTEXT_TAG_STRING_MAX_NUM)
        {
            break;
        }

        p = src_str_ptr + pos;

        //����ת���ַ�
        if ('\\' == p[0])
        {
            if (pos < src_len - 1) //����һ���ַ�
            {
                dst_str_ptr[dst_len] = p[1];
                dst_len++;
                pos += 2;
            }
            else
            {
                dst_str_ptr[dst_len] = p[0];
                dst_len++;
                pos += 1;
            }

            continue;
        }

        switch (hyper_state)
        {
        case GUIRICHTEXT_TAG_NONE:
            has_got_tags = FALSE;

            if ('<' == p[0] && pos < src_len - 6 && '>' == p[2]) /* "x></x>" - 6���ַ� */
            {
                has_got_tags = TRUE;

                switch (p[1])
                {
                case 's':
                    hyper_state = GUIRICHTEXT_TAG_TXT_ALL;
                    break;
                case 'p':
                    hyper_state = GUIRICHTEXT_TAG_PHONENUM;
                    break;
                case 'u':
                    hyper_state = GUIRICHTEXT_TAG_URL;
                    break;
                case 'e':
                    hyper_state = GUIRICHTEXT_TAG_EMAIL;
                    break;
                default:
                    has_got_tags = FALSE;
                    break;
                }
            }

            if (has_got_tags)
            {
                pos += 3;
            }
            else
            {
                dst_str_ptr[dst_len] = p[0];
                dst_len++;
                pos += 1;
            }

            break;

        case GUIRICHTEXT_TAG_TXT_ALL:
        case GUIRICHTEXT_TAG_PHONENUM:
        case GUIRICHTEXT_TAG_URL:
        case GUIRICHTEXT_TAG_EMAIL:
            has_got_tags = FALSE;

            for (search_pos = pos; search_pos < src_len; search_pos++)
            {
                search_p = src_str_ptr + search_pos;

                if ('\\' == search_p[0])
                {
                    search_pos++;
                    continue;
                }

                if ('<' == search_p[0] && pos < src_len - 3 /* "/x>" - 3���ַ� */
                        && '/' == search_p[1] && '>' == search_p[3])
                {
                    if (('s' == search_p[2] && GUIRICHTEXT_TAG_TXT_ALL == hyper_state)
                            || ('p' == search_p[2] && GUIRICHTEXT_TAG_PHONENUM == hyper_state)
                            || ('u' == search_p[2] && GUIRICHTEXT_TAG_URL == hyper_state)
                            || ('e' == search_p[2] && GUIRICHTEXT_TAG_EMAIL == hyper_state))
                    {
                        has_got_tags = TRUE;
                        break;
                    }
                }
            }

            if (has_got_tags)
            {
                tag_type = hyper_state;
                abs_begin = dst_len;

                for (i = pos; i < search_pos; i++)
                {
                    //����ת���ַ�
                    if ('\\' == src_str_ptr[i])
                    {
                        dst_str_ptr[dst_len] = src_str_ptr[i+1];
                        dst_len++;
                        i += 1;
                        continue;
                    }

                    dst_str_ptr[dst_len] = src_str_ptr[i];
                    dst_len++;
                }

                if (search_pos > pos)
                {
                    AddFocusElement(item_ptr, tag_type, abs_begin, dst_len - 1);
                }

                pos = search_pos;
                pos += 4;
            }
            else
            {
                dst_str_ptr[dst_len] = p[-3];
                dst_len++;
                dst_str_ptr[dst_len] = p[-2];
                dst_len++;
                dst_str_ptr[dst_len] = p[-1];
                dst_len++;
            }

            hyper_state = GUIRICHTEXT_TAG_NONE;
            break;

        default:
            SCI_ASSERT(0); /*assert verified*/
            break;
        }
    }

    item_ptr->data.orig_data.text_data.buf.len = dst_len;

    return MMI_RESULT_TRUE;
}


/*==============================================================================
Description: ����һ���ַ�����ʶ����ַ�
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �򵥵Ľ����㷨����guiurl.c���
==============================================================================*/
LOCAL MMI_RESULT_E ParseGeneralString(
    RICHTEXT_ITEM_T *item_ptr
)
{
    wchar *str_ptr = PNULL;
    GUIRICHTEXT_TAG_TYPE_E tag_type = GUIRICHTEXT_TAG_NONE;
    URL_PARSE_TYPE_T parse_type = 0;
    int32 url_data_cnt = 0;
    URL_DATA_T *url_data_ptr = PNULL;
    URL_DATA_T *cur_node_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if (PNULL == item_ptr)
    {
        return MMI_RESULT_FALSE;
    }
    
	tag_type = (item_ptr->data.orig_data.tag_type & GUIRICHTEXT_TAG_TXT_MASK);/*lint !e655*/

    if (!(GUIRICHTEXT_TAG_PARSE & tag_type)) /*lint !e655*/
    {
        return MMI_RESULT_FALSE;
    }

    //�������
    SCI_ASSERT(GUIRICHTEXT_TEXT_BUF == item_ptr->data.orig_data.text_type); /*assert verified*/

    str_ptr = item_ptr->data.orig_data.text_data.buf.str_ptr;

    if (PNULL == str_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (GUIRICHTEXT_TAG_PARSE_EMAIL & tag_type) /*lint !e655*/
    {
        parse_type |= URL_PARSE_EMAIL; /*lint !e655*/
    }

    if (GUIRICHTEXT_TAG_PARSE_URL & tag_type) /*lint !e655*/
    {
        parse_type |= URL_PARSE_URL; /*lint !e655*/
    }

    if (GUIRICHTEXT_TAG_PARSE_PHONENUM & tag_type) /*lint !e655*/
    {
        parse_type |= URL_PARSE_PHONENUM; /*lint !e655*/
    }

    url_data_cnt = URL_ParseDataGen(str_ptr, parse_type, &url_data_ptr);

    if (0 == url_data_cnt)
    {
        return MMI_RESULT_TRUE;
    }

    cur_node_ptr = url_data_ptr;

    while (cur_node_ptr)
    {
        GUIRICHTEXT_TAG_TYPE_E type = GUIRICHTEXT_TAG_NONE;

        switch (cur_node_ptr->type)
        {
        case URL_PARSE_EMAIL:
            type = GUIRICHTEXT_TAG_EMAIL;
            break;

        case URL_PARSE_URL:
            type = GUIRICHTEXT_TAG_URL;
            break;

        case URL_PARSE_PHONENUM:
            type = GUIRICHTEXT_TAG_PHONENUM;
            break;

        default:
            break;
        }

        if (GUIRICHTEXT_TAG_NONE != type)
        {
            AddFocusElement(item_ptr, type, cur_node_ptr->off_set, cur_node_ptr->off_set + cur_node_ptr->len - 1);
        }

        cur_node_ptr = cur_node_ptr->next;
    }

    URL_ParseDataRelease(url_data_ptr);

    return MMI_RESULT_TRUE;
}


/*==============================================================================
Description: ���ַ��ľ�������ת��������ص��������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: format abs_pos to "line" and "pos in line"
==============================================================================*/
LOCAL MMI_RESULT_E FormatAbsPos(
    RICHTEXT_ITEM_T *item_ptr
)
{
    int16 line = 0;
    int16 elem = 0;
    uint16 *start_pos_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if (PNULL == item_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    //�������
    SCI_ASSERT(GUIRICHTEXT_TEXT_BUF == item_ptr->data.orig_data.text_type); /*assert verified*/

    start_pos_ptr = item_ptr->data.txt_ext_data.line_start_pos_ptr;

    if (PNULL == start_pos_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    SCI_ASSERT(0 == item_ptr->focus_count || PNULL != item_ptr->focus_elem); /*assert verified*/

    //format abs_pos to "line" and "pos in line"
    for (elem = 0; elem < item_ptr->focus_count; elem++)
    {
        if ((item_ptr->focus_elem[elem].tag_type & GUIRICHTEXT_TAG_IMG_MASK) /*lint !e655*/
                || !(item_ptr->focus_elem[elem].tag_type & GUIRICHTEXT_TAG_TXT_MASK)) /*lint !e655*/
        {
            continue;
        }

        while (item_ptr->focus_elem[elem].abs_begin >= start_pos_ptr[line+1])
        {
            line++;
        }

        item_ptr->focus_elem[elem].line_begin = line;
        item_ptr->focus_elem[elem].pos_begin  =
            item_ptr->focus_elem[elem].abs_begin - start_pos_ptr[line];

        while (item_ptr->focus_elem[elem].abs_end >= start_pos_ptr[line+1])
        {
            line++;
        }

        item_ptr->focus_elem[elem].line_end = line;
        item_ptr->focus_elem[elem].pos_end  =
            item_ptr->focus_elem[elem].abs_end - start_pos_ptr[line];
    } // for elem

    return MMI_RESULT_TRUE;
}


/*==============================================================================
Description: ������ֵ�ҵ���Ԫ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL MMI_RESULT_E GetItemByIndex(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    uint16 item_index,
    RICHTEXT_ITEM_T **item_pptr
)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    RICHTEXT_ITEM_T *item_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != item_pptr); /*assert verified*/
    if ((PNULL == richtext_ptr)||(PNULL == item_pptr))
    {
        return MMI_RESULT_FALSE;
    }

    if (PNULL == richtext_ptr->first_item_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    item_ptr = richtext_ptr->first_item_ptr;

    while (PNULL != item_ptr)
    {
        if (item_ptr->index == item_index)
        {
            *item_pptr = item_ptr;
            result = MMI_RESULT_TRUE;
            break;
        }

        item_ptr = item_ptr->next_ptr;
    }

    return result;
}


/*==============================================================================
Description: ��ʼ��item���ڲ�����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void InitItem(
    CTRLRICHTEXT_OBJ_T const *richtext_ptr,
    GUIRICHTEXT_ITEM_T const *item_ptr,
    RICHTEXT_ITEM_T *new_item_ptr
)
{
    CTRLBASE_OBJ_T* basectrl_ptr = (CTRLBASE_OBJ_T*)richtext_ptr;
    uint32 buf_len = 0;
    MMI_STRING_T res_id_str = {0};
    wchar *bak_str_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    //SCI_ASSERT(PNULL != new_item_ptr); /*assert verified*/
    if ((PNULL == richtext_ptr)||(PNULL == item_ptr)||(PNULL == new_item_ptr))
    {
        return;
    }
    
    

    SCI_MEMSET(new_item_ptr, 0, sizeof(RICHTEXT_ITEM_T));

    //��ֵ
    new_item_ptr->data.orig_data = *item_ptr;

    //�ر�����Ҫ�����ڴ�Ĳ���
    //1 �ļ���ͼƬ
    if (GUIRICHTEXT_IMAGE_FILE == item_ptr->img_type)
    {
        new_item_ptr->data.orig_data.img_data.file.filename_len =
            MIN(item_ptr->img_data.file.filename_len, GUIRICHTEXT_FILENAME_MAX_LEN);
        buf_len = new_item_ptr->data.orig_data.img_data.file.filename_len + 1;
        new_item_ptr->data.orig_data.img_data.file.filename_ptr = (wchar*)SCI_ALLOC_APP(buf_len * sizeof(wchar));
        SCI_MEMSET(new_item_ptr->data.orig_data.img_data.file.filename_ptr, 0, (buf_len * sizeof(wchar)));

        MMI_WSTRNCPY(new_item_ptr->data.orig_data.img_data.file.filename_ptr,
                     new_item_ptr->data.orig_data.img_data.file.filename_len,
                     item_ptr->img_data.file.filename_ptr,
                     item_ptr->img_data.file.filename_len,
                     new_item_ptr->data.orig_data.img_data.file.filename_len);
    }

    //2 �ַ���
    //��TEXT IDת�����ַ���
    if (GUIRICHTEXT_TEXT_RES == new_item_ptr->data.orig_data.text_type)
    {
        MMITHEME_GetResText(
            new_item_ptr->data.orig_data.text_data.res.id,
            basectrl_ptr->handle,
            &res_id_str
        );

        if (PNULL == res_id_str.wstr_ptr)
        {
            new_item_ptr->data.orig_data.text_type = GUIRICHTEXT_TEXT_NONE;
        }
        else
        {
            //reset text
            new_item_ptr->data.orig_data.text_type = GUIRICHTEXT_TEXT_BUF;
            new_item_ptr->data.orig_data.text_data.buf.len = MIN(res_id_str.wstr_len, GUIRICHTEXT_TEXT_BUF_MAX_LEN);
            new_item_ptr->data.orig_data.text_data.buf.str_ptr = res_id_str.wstr_ptr;
        }
    }

    //�����մ�����ȥ����
    if (GUIRICHTEXT_TEXT_BUF == new_item_ptr->data.orig_data.text_type)
    {
        if (PNULL == new_item_ptr->data.orig_data.text_data.buf.str_ptr
                || 0 == new_item_ptr->data.orig_data.text_data.buf.len)
        {
            new_item_ptr->data.orig_data.text_type = GUIRICHTEXT_TEXT_NONE;
        }
    }

    if (GUIRICHTEXT_TEXT_BUF == new_item_ptr->data.orig_data.text_type)
    {
        size_t fix_len = MMIAPICOM_Wstrlen(new_item_ptr->data.orig_data.text_data.buf.str_ptr);
        //�����볤�ȳ���ʵ�ʳ���ʱ������Ϊʵ�ʳ���
        fix_len = MIN(fix_len, GUIRICHTEXT_TEXT_BUF_MAX_LEN);

        bak_str_ptr = new_item_ptr->data.orig_data.text_data.buf.str_ptr;
        new_item_ptr->data.orig_data.text_data.buf.len =
            MIN(new_item_ptr->data.orig_data.text_data.buf.len, fix_len);
        buf_len = new_item_ptr->data.orig_data.text_data.buf.len + 1;
        new_item_ptr->data.orig_data.text_data.buf.str_ptr = (wchar*)SCI_ALLOC_APP(buf_len * sizeof(wchar));
        new_item_ptr->data.txt_ext_data.line_start_pos_ptr = (uint16*)SCI_ALLOC_APP(buf_len * sizeof(uint16));
        SCI_MEMSET(new_item_ptr->data.orig_data.text_data.buf.str_ptr, 0, (buf_len * sizeof(wchar)));
        SCI_MEMSET(new_item_ptr->data.txt_ext_data.line_start_pos_ptr, 0, (buf_len * sizeof(wchar)));

        MMI_WSTRNCPY(new_item_ptr->data.orig_data.text_data.buf.str_ptr,
                     new_item_ptr->data.orig_data.text_data.buf.len,
                     bak_str_ptr,
                     new_item_ptr->data.orig_data.text_data.buf.len,
                     new_item_ptr->data.orig_data.text_data.buf.len);
    }

    return;
}


/*==============================================================================
Description: SetData
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL MMI_RESULT_E SetData(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    RICHTEXT_SETTINGS_E setting_type,
    void const * data
)
{
    CTRLBASE_OBJ_T* basectrl_ptr = (CTRLBASE_OBJ_T*)richtext_ptr;
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    BOOLEAN need_re_cal_data = FALSE;
    BOOLEAN need_re_cal_rect = FALSE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    switch (setting_type)
    {
        //display - font
    case RICHTEXT_SETTINGS_FONT:
        {
            GUI_FONT_T font = *(GUI_FONT_T*)data;

            if (font < GUI_MAX_FONT_NUM)
            {
                richtext_ptr->font = font;
                need_re_cal_data = TRUE;
                need_re_cal_rect = TRUE;
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }

        }
        break;

    case RICHTEXT_SETTINGS_FONT_COLOR:
        richtext_ptr->font_color = *(GUI_COLOR_T*)data;
        break;

        //display - space
    case RICHTEXT_SETTINGS_RECT:
        {
            GUI_RECT_T rect = *(GUI_RECT_T*)data;

            basectrl_ptr->rect = rect;

            richtext_ptr->cont_rect = rect;

            if (richtext_ptr->need_scroll)
            {
                if (ALIGN_LEFT == richtext_ptr->disp_align)
                {
                    richtext_ptr->cont_rect.right = rect.right - richtext_ptr->scrollbar_width;
                    richtext_ptr->scroll_rect = rect;
                    richtext_ptr->scroll_rect.left = richtext_ptr->cont_rect.right + 1;
                }
                else
                {
                    richtext_ptr->cont_rect.left = rect.left + richtext_ptr->scrollbar_width;
                    richtext_ptr->scroll_rect = rect;
                    richtext_ptr->scroll_rect.right = richtext_ptr->cont_rect.left - 1;
                }
            }

            GUI_SetVOrHRect( basectrl_ptr->handle, &rect, &(basectrl_ptr->both_rect));

            //�������ٹ�����
            if (PNULL != richtext_ptr->prgbox_ptr)
            {
                MMK_DestroyControl(GUICTRL_GetCtrlHandle((IGUICTRL_T *)richtext_ptr->prgbox_ptr));
                richtext_ptr->prgbox_ptr = PNULL;
            }

            need_re_cal_data = TRUE;
            need_re_cal_rect = TRUE;
        }
        break;

    case RICHTEXT_SETTINGS_SPACE_CHAR:

        if (*(uint8*)data <= GUIRICHTEXT_SETTINGS_SPACE_LIMIT)
        {
            richtext_ptr->char_space = *(uint8*)data;
            need_re_cal_data = TRUE;
            need_re_cal_rect = TRUE;
        }

        break;

    case RICHTEXT_SETTINGS_SPACE_LINE:

        if (*(uint8*)data <= GUIRICHTEXT_SETTINGS_SPACE_LIMIT)
        {
            richtext_ptr->line_space = *(uint8*)data;
            need_re_cal_data = TRUE;
            need_re_cal_rect = TRUE;
        }

        break;

    case RICHTEXT_SETTINGS_SPACE_IMG_TXT:

        if (*(uint8*)data <= GUIRICHTEXT_SETTINGS_SPACE_LIMIT)
        {
            richtext_ptr->img_text_space = *(uint8*)data;
            need_re_cal_data = TRUE;
            need_re_cal_rect = TRUE;
        }

        break;

    case RICHTEXT_SETTINGS_SPACE_ITEM:

        if (*(uint8*)data <= GUIRICHTEXT_SETTINGS_SPACE_LIMIT)
        {
            richtext_ptr->item_space = *(uint8*)data;
            //need_re_cal_data = TRUE;
            need_re_cal_rect = TRUE;
        }

        break;

    case RICHTEXT_SETTINGS_SPACE_BORDER:

        if (*(uint8*)data <= GUIRICHTEXT_SETTINGS_SPACE_LIMIT)
        {
            richtext_ptr->border_space = *(uint8*)data;
            need_re_cal_data = TRUE;
            need_re_cal_rect = TRUE;
        }

        break;

    case RICHTEXT_SETTINGS_SPACE_ITEM_BORDER:

        if (*(uint8*)data <= GUIRICHTEXT_SETTINGS_SPACE_LIMIT)
        {
            richtext_ptr->item_border_space = *(uint8*)data;
            need_re_cal_data = TRUE;
            need_re_cal_rect = TRUE;
        }

        break;


    case RICHTEXT_SETTINGS_TXT_MARGIN:

        if (*(uint8*)data <= GUIRICHTEXT_SETTINGS_SPACE_LIMIT)
        {
            richtext_ptr->txt_margin = *(uint8*)data;
            need_re_cal_data = TRUE;
            need_re_cal_rect = TRUE;
        }

        break;

        //display - focus
    case RICHTEXT_SETTINGS_FOCUS_MODE:
        {
            GUIRICHTEXT_FOCUS_SHIFT_E focus_mode = *(GUIRICHTEXT_FOCUS_SHIFT_E*)data;

            if (focus_mode < GUIRICHTEXT_FOCUS_SHIFT_MAX)
            {
                richtext_ptr->focus_mode = focus_mode;

                if (GUIRICHTEXT_FOCUS_SHIFT_NONE != focus_mode)
                {
                    RICHTEXT_ITEM_T *cur_item_ptr = richtext_ptr->first_item_ptr;

                    while (PNULL != cur_item_ptr)
                    {
                        SetItemFocusElememt(richtext_ptr, cur_item_ptr);
                        cur_item_ptr = cur_item_ptr->next_ptr;
                    }
                }//�����Ѿ������item��focus

                need_re_cal_data = TRUE;
                need_re_cal_rect = TRUE;
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }

        }
        break;

    case RICHTEXT_SETTINGS_FOCUS_LINE_COLOR:
        richtext_ptr->focusline_color = *(GUI_COLOR_T*)data;
        break;

    case RICHTEXT_SETTINGS_FOCUS_BG_COLOR:
        richtext_ptr->focus_bg_color = *(GUI_COLOR_T*)data;
        break;

        //display - bg
    case RICHTEXT_SETTINGS_BG_COLOR:
        richtext_ptr->bg_color = *(GUI_COLOR_T*)data;
        richtext_ptr->bg_img_id = 0;
        break;

    case RICHTEXT_SETTINGS_BG_IMG:

        if (MMITHEME_CheckImageID(*(MMI_IMAGE_ID_T*)data))
        {
            richtext_ptr->bg_img_id = *(MMI_IMAGE_ID_T*)data;
        }

        break;

        //display - frame
    case RICHTEXT_SETTINGS_FRAME_TYPE:
        {
            GUIRICHTEXT_FRAME_TYPE_E border_type = *(GUIRICHTEXT_FRAME_TYPE_E*)data;

            if (border_type < GUIRICHTEXT_FRAME_MAX)
            {
                richtext_ptr->border_type = border_type;
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }
        }
        break;

    case RICHTEXT_SETTINGS_FRAME_COLOR:
        richtext_ptr->border_color = *(GUI_COLOR_T*)data;
        break;

    case RICHTEXT_SETTINGS_FRAME_FOCUS_COLOR:
        richtext_ptr->border_focus_color = *(GUI_COLOR_T*)data;
        break;

        //display - anim
    case RICHTEXT_SETTINGS_ANIM_PARSING_ICON:

        if (MMITHEME_CheckImageID(*(MMI_IMAGE_ID_T*)data))
        {
            richtext_ptr->anim_parsing_icon_id = *(MMI_IMAGE_ID_T*)data;
        }

        break;

    case RICHTEXT_SETTINGS_ANIM_ERROR_ICON:

        if (MMITHEME_CheckImageID(*(MMI_IMAGE_ID_T*)data))
        {
            richtext_ptr->anim_error_icon_id = *(MMI_IMAGE_ID_T*)data;
        }

        break;

        //control
    case RICHTEXT_SETTINGS_FOCUS_ITEM:
        {
            uint16 index = *(uint16*)data;
            RICHTEXT_ITEM_T *item_ptr = PNULL;
            MMI_RESULT_E retcode = MMI_RESULT_FALSE;

            retcode = GetItemByIndex(richtext_ptr, index, &item_ptr);

            if (MMI_RESULT_TRUE == retcode
                    && item_ptr->data.orig_data.tag_type != GUIRICHTEXT_TAG_NONE)
            {
                richtext_ptr->focus_item_ptr = item_ptr;
            }
            else
            {
                result = MMI_RESULT_FALSE;
            }
        }
        break;

    case RICHTEXT_SETTINGS_SCROLL_BAR:
        {
            BOOLEAN need_scroll = *(BOOLEAN*)data;

            if (need_scroll != richtext_ptr->need_scroll)
            {
                richtext_ptr->need_scroll = need_scroll;
                SetData(richtext_ptr, RICHTEXT_SETTINGS_RECT, &basectrl_ptr->rect);

                if (!need_scroll && (PNULL != richtext_ptr->prgbox_ptr))
                {
                    MMK_DestroyControl(GUICTRL_GetCtrlHandle((IGUICTRL_T *)richtext_ptr->prgbox_ptr));
                    richtext_ptr->prgbox_ptr = PNULL;
                }
            }

        }
        break;

    case RICHTEXT_SETTINGS_IMG_SYN_DECODE:
        {
            BOOLEAN is_syn_decode = *(BOOLEAN*)data;

            if (is_syn_decode != richtext_ptr->img_is_syn_decode)
            {
                richtext_ptr->img_is_syn_decode = is_syn_decode;
            }

        }
        break;

    default:
        result = MMI_RESULT_FALSE;
        break;
    }

#ifdef GUIF_CLIPBOARD
    ClearCtrlInfo(richtext_ptr);
#endif

    richtext_ptr->need_cal_ext_data = (richtext_ptr->need_cal_ext_data || need_re_cal_data);
    richtext_ptr->need_cal_rect = (richtext_ptr->need_cal_rect || need_re_cal_rect);

    return result;
}


/*==============================================================================
Description: SetDataById
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL MMI_RESULT_E SetDataById(
    MMI_CTRL_ID_T ctrl_id,
    RICHTEXT_SETTINGS_E setting_type,
    void const * data
)
{
    CTRLRICHTEXT_OBJ_T *richtext_ptr = PNULL;
    richtext_ptr = GetRichTextPtr(ctrl_id);

    if (PNULL == richtext_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    return SetData(richtext_ptr, setting_type, data);
}


/*==============================================================================
Description: ResetState
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void ResetState(CTRLRICHTEXT_OBJ_T *richtext_ptr)
{
    richtext_ptr->need_cal_ext_data = TRUE;
    richtext_ptr->need_cal_rect = TRUE;

    richtext_ptr->first_item_ptr = PNULL;
    richtext_ptr->focus_item_ptr = PNULL;

    richtext_ptr->img_appending_item_ptr = PNULL;

    if (!richtext_ptr->is_info_protected)
    {
        richtext_ptr->disp_height = 0;
        richtext_ptr->disp_top = 0;
    }

    return;
}


/*==============================================================================
Description: �ڵ�ǰitem������һ����ʱ����Ԫ��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: ��ʱ����Ԫ�ز����������ʽ���ڽ�����֮��ϲ������飬��Ϊ��ʼ��֪���ж��
==============================================================================*/
LOCAL MMI_RESULT_E AddFocusElement(
    RICHTEXT_ITEM_T *item_ptr,
    GUIRICHTEXT_TAG_TYPE_E tag_type,
    uint16 abs_begin,
    uint16 abs_end
)
{
    RICHTEXT_FOCUS_ELEMENT_NODE_T * new_node_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if (PNULL == item_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    new_node_ptr = (RICHTEXT_FOCUS_ELEMENT_NODE_T*)SCI_ALLOC_APP(sizeof(RICHTEXT_FOCUS_ELEMENT_NODE_T));
    SCI_MEMSET(new_node_ptr, 0, sizeof(RICHTEXT_FOCUS_ELEMENT_NODE_T));
    new_node_ptr->tag_type = tag_type;
    new_node_ptr->abs_begin = abs_begin;
    new_node_ptr->abs_end = abs_end;

    if (PNULL == item_ptr->focus_header_ptr)
    {
        item_ptr->focus_tailer_ptr = new_node_ptr;
        item_ptr->focus_header_ptr = item_ptr->focus_tailer_ptr;
        SCI_ASSERT(item_ptr->focus_count == 0); /*assert verified*/
    }
    else
    {
        item_ptr->focus_tailer_ptr->next_ptr = new_node_ptr;
        item_ptr->focus_tailer_ptr = new_node_ptr;
    }

    item_ptr->focus_count++;


    return MMI_RESULT_TRUE;
}


/*==============================================================================
Description: CombineFocusElement
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL MMI_RESULT_E CombineFocusElement(
    RICHTEXT_ITEM_T *item_ptr
)
{
    uint32 buffer_size = 0;
    int32 index = 0;
    RICHTEXT_FOCUS_ELEMENT_NODE_T * cur_node_ptr = PNULL;
    RICHTEXT_FOCUS_ELEMENT_NODE_T * bak_node_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if (PNULL == item_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (item_ptr->focus_count == 0)
    {
        return MMI_RESULT_TRUE;
    }

    buffer_size = item_ptr->focus_count * sizeof(RICHTEXT_FOCUS_ELEM_T);
    item_ptr->focus_elem = (RICHTEXT_FOCUS_ELEM_T *)SCI_ALLOC_APP(buffer_size);
    SCI_MEMSET(item_ptr->focus_elem, 0, buffer_size);

    cur_node_ptr = item_ptr->focus_header_ptr;

    while (PNULL != cur_node_ptr)
    {
        item_ptr->focus_elem[index].tag_type = cur_node_ptr->tag_type;
        item_ptr->focus_elem[index].abs_begin = cur_node_ptr->abs_begin;
        item_ptr->focus_elem[index].abs_end = cur_node_ptr->abs_end;

        index++;

        bak_node_ptr = cur_node_ptr;
        cur_node_ptr = cur_node_ptr->next_ptr;
        SCI_FREE(bak_node_ptr);
    }

    item_ptr->focus_header_ptr = PNULL;
    item_ptr->focus_tailer_ptr = PNULL;

    SCI_ASSERT(index == item_ptr->focus_count); /*assert verified*/

    return MMI_RESULT_TRUE;
}


/*==============================================================================
Description: �ͷ�item������Ϣ��ռ���ڴ�
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void ReleaseFocusItem(RICHTEXT_ITEM_T *item_ptr)
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != item_ptr); /*assert verified*/
    if (PNULL == item_ptr)
    {
        return;
    }

    if (PNULL != item_ptr->focus_elem)
    {
        SCI_FREE(item_ptr->focus_elem);
        item_ptr->focus_elem = PNULL;
    }

    item_ptr->focus_count = 0;
    item_ptr->focus_index = 0;

    return;
}


/*==============================================================================
Description: ������ʾ������Ԫ������ҳ��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void ScrollToFocus(CTRLRICHTEXT_OBJ_T *richtext_ptr)
{
    GUIRICHTEXT_TAG_TYPE_E tag_type = GUIRICHTEXT_TAG_NONE;
    int16 out_of_item_height = 0;
    int32 focus_top = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return;
    }

    if (PNULL == richtext_ptr->focus_item_ptr)
    {
        return;
    }

    if (0 == richtext_ptr->focus_item_ptr->focus_count)
    {
        return;
    }

    //�˴�Ӧ��assert����Ϊ���������������Ȼ�Ǵ���
    SCI_ASSERT(PNULL != richtext_ptr->focus_item_ptr->focus_elem); /*assert verified*/

    tag_type = richtext_ptr->focus_item_ptr->focus_elem[richtext_ptr->focus_item_ptr->focus_index].tag_type;

    if (GUIRICHTEXT_TAG_PHONENUM == tag_type
            || GUIRICHTEXT_TAG_EMAIL == tag_type
            || GUIRICHTEXT_TAG_URL == tag_type)
    {
        focus_top = richtext_ptr->focus_item_ptr->virtual_rect.top;

        //������ʼ��
        out_of_item_height = richtext_ptr->focus_item_ptr->data.txt_ext_data.line_height + richtext_ptr->line_space;
        out_of_item_height *= richtext_ptr->focus_item_ptr->focus_elem[richtext_ptr->focus_item_ptr->focus_index].line_begin;

        focus_top += out_of_item_height;
    }
    else
    {
        focus_top = richtext_ptr->focus_item_ptr->virtual_rect.top;
    }

    ScrollToLine(richtext_ptr, focus_top);

    return;
}



/*==============================================================================
Description: ��ȡ��ǰtp���ַ�����λ��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: ����һ������
0 - ��һ���ַ�֮ǰ
==============================================================================*/
LOCAL BOOLEAN GetTpPos(
    GUI_RECT_T const *text_rect_ptr,
    GUI_ALIGN_E text_align,
    wchar const *str_ptr,
    uint32 str_len,
    int16 tp_x,
    GUI_FONT_T font,
    uint32 char_space,
    int32 *nearest_chr_ptr,
    int32 *under_chr_ptr,
    int32 *under_symb_len_ptr
)
{
    BOOLEAN is_find = FALSE;

    if (PNULL == text_rect_ptr || PNULL == str_ptr)
    {
        *nearest_chr_ptr = -1;
        return FALSE;
    }

    //����ϵͳ�����б����ַ��м�����
    is_find = GUI_GetCursorPosEx(text_align,
                                 text_rect_ptr,
                                 TEXT_DIR_AUTO,
                                 str_ptr,
                                 str_len,
                                 tp_x,
                                 font,
                                 char_space,
                                 nearest_chr_ptr,
                                 under_chr_ptr,
                                 under_symb_len_ptr);


    return is_find;
}


#ifdef GUIF_CLIPBOARD

/*==============================================================================
Description: ��ȡ��ǰ��ѡ������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: ��ʵ���ַ����ȵ���ϣ�ÿһ�μ�һ������
==============================================================================*/
LOCAL uint32 GetSelectLen(CTRLRICHTEXT_OBJ_T *richtext_ptr)
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return 0;
    }

    GenSelectData(richtext_ptr);

    //����ѡ��item��������ͬ��item��'\n'����
    return (richtext_ptr->select_len + richtext_ptr->select_item_cnt);
}

/*==============================================================================
Description: ShowClipboardMenu
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void ShowClipboardMenu(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    GUI_POINT_T *tp_point_ptr,
    BOOLEAN always_show
)
{
    CTRLBASE_OBJ_T* basectrl_ptr = (CTRLBASE_OBJ_T*)richtext_ptr;
    BOOLEAN no_select = FALSE;
    BOOLEAN no_focus = FALSE;
    CLIPBRDMENU_DATA_T  clipbrdmenu_data = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return;
    }

    if (0 != richtext_ptr->clipbrd_win_handle)
    {
        return;
    }

    //if select sth. then can copy
    no_select = (GetSelectLen(richtext_ptr) == 0);
    no_focus = (PNULL == richtext_ptr->focus_item_ptr);
    clipbrdmenu_data.is_copy_disabled = (no_select && no_focus);

    //�ڷ� - ���ǵ��� - ����£����� - �ɸ��� - �������Ƿ񵯳��˵�
    if (clipbrdmenu_data.is_copy_disabled && !always_show)
    {
        return;
    }

    clipbrdmenu_data.dst_ctrl_handle = basectrl_ptr->handle;
    //no cut, no paste
    clipbrdmenu_data.is_cut_disabled = TRUE;
    clipbrdmenu_data.is_paste_disabled = TRUE;

    //always can select all
    clipbrdmenu_data.is_select_all_disabled = FALSE;
    clipbrdmenu_data.is_select_disabled = TRUE;

    // get tp position
    clipbrdmenu_data.start_point = *tp_point_ptr;

    if (MMITHEME_DRAGMODE_DEFAULT != richtext_ptr->drag_mode)
    {
        clipbrdmenu_data.start_point.y -= MMITHEME_GetClipbrdHeight();
    }

    // show clipboard menu
    richtext_ptr->clipbrd_win_handle = MMITHEME_ShowClipbrdMenu(&clipbrdmenu_data);

    return;
}


/*==============================================================================
Description: HandleClipbrdMenuCopyMsg
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void HandleClipbrdMenuCopyMsg(
    CTRLRICHTEXT_OBJ_T *richtext_ptr
)
{
    RICHTEXT_ITEM_T *item_ptr = PNULL;
    wchar *select_str_ptr = PNULL;
    uint32 select_str_len = 0;
    uint32 select_str_size = 0;
    uint32 cur_str_begin = 0;
    uint32 cur_str_len = 0;
    uint32 copy_str_len = 0;
    BOOLEAN has_selected = FALSE;
    BOOLEAN has_focus = FALSE;


    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return;
    }

    has_selected = (RICHTEXT_STATE_SELECTED == richtext_ptr->select_state);
    has_focus = (PNULL != richtext_ptr->focus_item_ptr);

    if (has_selected)
    {
        item_ptr = richtext_ptr->first_item_ptr;

        if (PNULL == item_ptr)
        {
            return;
        }

        //get highlight string
        //1 ����ѡ���ַ��ĳ���
        //2 ����ռ�
        //3 ��ѡ���ַ����Ƶ���ʱ����
        //4 ��ֵ��ϵͳ���а�
        //...

        select_str_len = GetSelectLen(richtext_ptr); //���ٿռ�����һ�����з�

        select_str_size = select_str_len * sizeof(wchar);

        select_str_ptr = (wchar*)SCI_ALLOCA(select_str_size);

        if (PNULL == select_str_ptr)
        {
            //SCI_TRACE_LOW:"richtext copy failed, malloc size: %d."
            SCI_TRACE_ID(TRACE_TOOL_CONVERT,GUIRICHTEXT_5919_112_2_18_3_21_2_284,(uint8*)"d", select_str_size);
            return;
        }

        copy_str_len = 0;

        item_ptr = richtext_ptr->first_item_ptr;

        for (; PNULL != item_ptr; item_ptr = item_ptr->next_ptr)
        {
            if (0 == item_ptr->data.txt_ext_data.select_len) continue;

            cur_str_begin = item_ptr->data.txt_ext_data.select_pos;
            cur_str_len = item_ptr->data.txt_ext_data.select_len;

            SCI_MEMCPY(select_str_ptr + copy_str_len,
                       item_ptr->data.orig_data.text_data.buf.str_ptr + cur_str_begin,
                       cur_str_len * sizeof(wchar));
            copy_str_len += cur_str_len;

            *(select_str_ptr + copy_str_len) = '\n';
            copy_str_len += 1; //ÿ��item֮�䣬��'\n'����
        }

        SCI_ASSERT(select_str_len == copy_str_len); /*assert verified*/

        // set text into clipboard
        MMITHEME_SetClipbrdData(
            CLIPBRD_FORMAT_TEXT,
            (void*)select_str_ptr,
            select_str_size - sizeof(wchar)); //���һ�����з���Ҫ

        SCI_FREE(select_str_ptr);

        //ClearClipBoardInfo(richtext_ptr);
    }
    else if (has_focus)
    {
        RICHTEXT_FOCUS_ELEM_T *elem_ptr = PNULL;
        BOOLEAN is_text_tags = FALSE;

        item_ptr = richtext_ptr->focus_item_ptr;
        elem_ptr = &(item_ptr->focus_elem[item_ptr->focus_index]);

        is_text_tags = (!(elem_ptr->tag_type & GUIRICHTEXT_TAG_IMG_MASK) /*lint !e655*/
                        && (elem_ptr->tag_type & GUIRICHTEXT_TAG_TXT_MASK)); /*lint !e655*/

        if (is_text_tags)
        {
            select_str_ptr = item_ptr->data.orig_data.text_data.buf.str_ptr + elem_ptr->abs_begin;
            select_str_len = elem_ptr->abs_end - elem_ptr->abs_begin + 1;
        }
        else if (GUIRICHTEXT_TAG_NORMAL == elem_ptr->tag_type)
        {
            if (GUIRICHTEXT_TEXT_BUF == item_ptr->data.orig_data.text_type)
            {
                is_text_tags = TRUE;
                select_str_ptr = item_ptr->data.orig_data.text_data.buf.str_ptr;
                select_str_len = item_ptr->data.orig_data.text_data.buf.len;
            }
        }

        if (is_text_tags)
        {
            // set text into clipboard
            MMITHEME_SetClipbrdData(
                CLIPBRD_FORMAT_TEXT,
                (void*)select_str_ptr,
                select_str_len*sizeof(wchar));
        }
    }
    else
    {
        return;
    }

    ChangeRichtextDragState(richtext_ptr, GUIRICHTEXT_DRAGSTATE_NORMAL);

    return;
}

/*==============================================================================
Description: HandleClipbrdMenuSelectAllMsg
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void HandleClipbrdMenuSelectAllMsg(
    CTRLRICHTEXT_OBJ_T *richtext_ptr
)
{
    CTRLBASE_OBJ_T* basectrl_ptr = (CTRLBASE_OBJ_T*)richtext_ptr;
    GUI_POINT_T     clipbrd_pt = {0};

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return;
    }

    richtext_ptr->select_state = RICHTEXT_STATE_SELECTED;

    //ɾ������Ԫ��
    if (PNULL != richtext_ptr->focus_item_ptr)
    {
        richtext_ptr->focus_item_ptr = PNULL;
        GUICTRL_PostNotify(basectrl_ptr->handle, MSG_NOTIFY_RICHTEXT_SWITCHFOCUS_BY_TP);
    }

    richtext_ptr->v_start_x = 0;
    richtext_ptr->v_start_y = 0;
    richtext_ptr->v_stop_x = richtext_ptr->cont_rect.right - richtext_ptr->cont_rect.left;
    richtext_ptr->v_stop_y = richtext_ptr->disp_height - 1;

    richtext_ptr->is_select_data_dirty = TRUE;

    if (MMITHEME_DRAGMODE_DEFAULT != richtext_ptr->drag_mode)
    {
        ChangeRichtextDragState(richtext_ptr, GUIRICHTEXT_DRAGSTATE_SELECT);

        // Ǳ����
        // �����򿪴��ڣ���LOSE FOCUS��Ϣ��LCD���ݣ��������㻹û����LCD�ϣ�
        // ������ʾ��������������������һ��
        GUILCD_Invalidate( GUI_MAIN_LCD_ID, 0 );

        clipbrd_pt.x = richtext_ptr->cont_rect.left;
        clipbrd_pt.y = richtext_ptr->cont_rect.top;

        ShowClipboardMenu(richtext_ptr, &clipbrd_pt, TRUE);
    }

    return;
}

/*==============================================================================
Description: HandleClipbrdMenuSelectMsg
Global resource dependence: NONE
Author: hua.fang
Note:
==============================================================================*/
LOCAL void HandleClipbrdMenuSelectMsg(
    CTRLRICHTEXT_OBJ_T *richtext_ptr
)
{
    CTRLBASE_OBJ_T* basectrl_ptr = (CTRLBASE_OBJ_T*)richtext_ptr;
    GUI_POINT_T     point = {0};
    GUI_RECT_T      min_highlight_rect = {0};
    GUI_RECT_T      max_highlight_rect = {0};
    RICHTEXT_ITEM_T *focus_item_ptr = PNULL;

    if (RICHTEXT_STATE_SELECTED != richtext_ptr->select_state)
    {
        if (PNULL != richtext_ptr->focus_item_ptr)
        {
            focus_item_ptr = richtext_ptr->focus_item_ptr;

            // ��ԭfocus״̬��item���normal״̬���������¶�Ӧ�ĸ�����Ϣ
            if (focus_item_ptr->focus_elem[focus_item_ptr->focus_index].tag_type & GUIRICHTEXT_TAG_TXT_MASK) /*lint !e655*/
            {
                richtext_ptr->select_state = RICHTEXT_STATE_SELECTED;
                richtext_ptr->is_select_data_dirty = TRUE;

                //ɾ������Ԫ��
                if (PNULL != richtext_ptr->focus_item_ptr)
                {
                    richtext_ptr->focus_item_ptr = PNULL;
                    GUICTRL_PostNotify(basectrl_ptr->handle, MSG_NOTIFY_RICHTEXT_SWITCHFOCUS_BY_TP);
                }

                // ȷ���¿�ʼ�������λ��
                richtext_ptr->start_info.item_index = focus_item_ptr->index;
                richtext_ptr->start_info.line_index = focus_item_ptr->focus_elem[focus_item_ptr->focus_index].line_begin;
                richtext_ptr->start_info.pos_index = focus_item_ptr->focus_elem[focus_item_ptr->focus_index].pos_begin;

                richtext_ptr->stop_info.item_index = focus_item_ptr->index;
                richtext_ptr->stop_info.line_index = focus_item_ptr->focus_elem[focus_item_ptr->focus_index].line_end;
                richtext_ptr->stop_info.pos_index = focus_item_ptr->focus_elem[focus_item_ptr->focus_index].pos_end;

                // ��ȡԭ��������Ӧ������
                GetHighlightRect(richtext_ptr, TRUE, &min_highlight_rect);

                richtext_ptr->v_start_x = min_highlight_rect.left - richtext_ptr->cont_rect.left;
                richtext_ptr->v_start_y = 
                            richtext_ptr->disp_top
                            + (min_highlight_rect.top + min_highlight_rect.bottom) / 2
                            - richtext_ptr->cont_rect.top;

                GetHighlightRect(richtext_ptr, FALSE, &max_highlight_rect);

                richtext_ptr->v_stop_x = max_highlight_rect.left - richtext_ptr->cont_rect.left;
                richtext_ptr->v_stop_y = 
                            richtext_ptr->disp_top
                            + (max_highlight_rect.top + max_highlight_rect.bottom) / 2
                            - richtext_ptr->cont_rect.top;

                ChangeRichtextDragState(richtext_ptr, GUIRICHTEXT_DRAGSTATE_SELECT);
            }
        }
        else
        {
            point = richtext_ptr->tp_start_point;
            SetStartPoint(richtext_ptr, &point);

            point.x += 1;
            SetStopPoint(richtext_ptr, &point);

            richtext_ptr->select_state = RICHTEXT_STATE_SELECTING;

            DrawRichText(richtext_ptr);

            ChangeRichtextDragState(richtext_ptr, GUIRICHTEXT_DRAGSTATE_SELECT);
        }
    }
}

/*==============================================================================
Description: �϶���richtext֮��ʱÿtick�����Ĵ���
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void ScrollByDragTick(uint8 timer_id, uint32 param)
{
    CTRLRICHTEXT_OBJ_T *richtext_ptr = (CTRLRICHTEXT_OBJ_T *)param;
    uint32 tp_distance = 0;
    uint32 tick_out = 0;

    if (PNULL == richtext_ptr)
    {
        return;
    }

    SCI_ASSERT(timer_id == richtext_ptr->drag_select_timer); /*assert verified*/

    //��ʼ��timer id
    richtext_ptr->drag_select_timer = 0;

    if (RICHTEXT_STATE_SELECTING != richtext_ptr->select_state)
    {
        return;
    }

    if (richtext_ptr->tp_stop_point.y <= richtext_ptr->cont_rect.top)
    {
        //�������ϣ�����һ�в�ѡ��
        ScrollUp(richtext_ptr, richtext_ptr->scroll_step);
        tp_distance = richtext_ptr->cont_rect.top - richtext_ptr->tp_stop_point.y;
    }
    else if (richtext_ptr->tp_stop_point.y >= richtext_ptr->cont_rect.bottom)
    {
        //�������£�����һ�в�ѡ��
        ScrollDown(richtext_ptr, richtext_ptr->scroll_step);
        tp_distance = richtext_ptr->tp_stop_point.y - richtext_ptr->cont_rect.bottom;
    }
    else
    {
        return;
    }

    //���á���ʾ
    if (GUIRICHTEXT_SLIDER_LEFT_SELECTOR == richtext_ptr->tpdown_slider)
    {
        SetStartPoint(richtext_ptr, &(richtext_ptr->tp_stop_point));
    }
    else if (GUIRICHTEXT_SLIDER_RIGHT_SELECTOR == richtext_ptr->tpdown_slider
        || MMITHEME_DRAGMODE_DEFAULT == richtext_ptr->drag_mode)
    {
        SetStopPoint(richtext_ptr, &(richtext_ptr->tp_stop_point));
    }

    //��ʾѡ��
    DrawRichText(richtext_ptr);

    tick_out = RICHTEXT_DRAG_SELECT_TICK;
    //ÿ�����ؼ�10ms������tick*3/4 ms
    tp_distance *= 10;

    if (tp_distance > RICHTEXT_DRAG_SELECT_TICK*3 / 4)
    {
        tp_distance = RICHTEXT_DRAG_SELECT_TICK * 3 / 4;
    }

    tick_out -= tp_distance;

    richtext_ptr->drag_select_timer = MMK_CreateTimerCallback(
                                          tick_out,
                                          ScrollByDragTick,
                                          (uint32)richtext_ptr,
                                          FALSE);

    return;
}


/*==============================================================================
Description: ����ѡ��������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: ���ݴ��ʻ�������ֹ�����ѡ�����ַ���ָ�뼰����
==============================================================================*/
LOCAL void GenSelectData(CTRLRICHTEXT_OBJ_T *richtext_ptr)
{
    RICHTEXT_ITEM_T *item_ptr = PNULL;
    BOOLEAN is_start_upon_stop = FALSE;
    BOOLEAN is_start_index_done = FALSE;
    int32 start_x = 0;
    int32 start_y = 0;
    int32 stop_x = 0;
    int32 stop_y = 0;
    int32 select_top = 0;
    int32 select_bottom = 0;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return;
    }

    //�н����ʱ�����������ѡ������
    if (PNULL != richtext_ptr->focus_item_ptr)
    {
        if (RICHTEXT_STATE_SELECT_NONE != richtext_ptr->select_state)
        {
            ClearClipBoardInfo(richtext_ptr);
        }

        return;
    }

    //����δ�࣬�򲻸�������
    if (!richtext_ptr->is_select_data_dirty)
    {
        if (richtext_ptr->is_line_data_dirty)
        {
            ReGenLineInfo(richtext_ptr);
        }

        return;
    }

    richtext_ptr->is_select_data_dirty = FALSE;

    if (RICHTEXT_STATE_SELECT_NONE == richtext_ptr->select_state)
    {
        return;
    }

    item_ptr = richtext_ptr->first_item_ptr;

    if (PNULL == item_ptr)
    {
        return;
    }

    /*
    �����㷨
    1 ���촥���ƶ�������������ֹ��Ϊ�Խ��ߵ�rect1
    2 ���촥���ƶ����������������ֹ��Ϊ���±߽磬���������ռ��ȵ�rect2
    3 һ�����ֵ��Ƿ�ѡ����������������rect��ȷ��
    4 ��ѡ����rect1�н�����text - �˴��������������û������⣬����Ӧ�ò�����
    5 ѡ��������text��rect��rect2�Ľ���������
    6 �ס�ĩ������ֹ��ǰ��Ĳ��֣��������ַ�����ȷ��
    7 һ���ǹ���6����ϸ˵��
    8 ������ϣ�ֹ�����£������ǰ��Ĳ��ֺ�ֹ�����Ĳ�����ȥ
    9 ������£�ֹ�����ϣ���������Ĳ��ֺ�ֹ��ǰ��Ĳ�����ȥ
    */

    /*
    ����a��ѡ����b��Ľ�����롮��b��ѡ����a��Ľ������һ����
    ���ԣ�������������ֹ�㣬ֻ�����ĸ����ϣ��ĸ��������ǵ����
    */
    if (MMITHEME_DRAGMODE_DEFAULT == richtext_ptr->drag_mode)
    {
        is_start_upon_stop = (richtext_ptr->v_start_y < richtext_ptr->v_stop_y);
    }
    else
    {
        is_start_upon_stop = TRUE;
    }

    if (is_start_upon_stop)
    {
        start_x = richtext_ptr->v_start_x;
        start_y = richtext_ptr->v_start_y;
        stop_x = richtext_ptr->v_stop_x;
        stop_y = richtext_ptr->v_stop_y;
    }
    else
    {
        stop_x = richtext_ptr->v_start_x;
        stop_y = richtext_ptr->v_start_y;
        start_x = richtext_ptr->v_stop_x;
        start_y = richtext_ptr->v_stop_y;
    }

    select_top = start_y;
    select_bottom = stop_y;

    /*
    1 �ж��Ƿ��н���
    2 top�Ƿ�����
    3 bottom�Ƿ�����
    */

    richtext_ptr->select_len = 0;
    richtext_ptr->select_item_cnt = 0;

    for (; PNULL != item_ptr; item_ptr = item_ptr->next_ptr)
    {
        BOOLEAN is_item_select = FALSE;
        BOOLEAN is_top_out = FALSE;
        BOOLEAN is_bottom_out = FALSE;
        uint16 abs_start_pos = 0;
        uint16 abs_stop_pos = 0;
        uint16 start_line = 0;
        uint16 line_len = 0;
        int32 start_pos = 0;
        int32 start_len = 0;
        uint16 stop_line = 0;
        int32 stop_pos = 0;
        int32 stop_len = 0;
        GUI_BIG_RECT_T line_rect = {0};
        GUI_RECT_T pos_line_rect = {0};
        uint16 line_idx = 0;
        BOOLEAN is_find = 0;
        int32 nearest_chr = 0;
        int32 under_chr = 0;
        int32 under_symb_len = 0;

        //��ʼ��
        item_ptr->data.txt_ext_data.select_len = 0;

        if (GUIRICHTEXT_TEXT_BUF != item_ptr->data.orig_data.text_type) continue;

        if (0 == item_ptr->data.txt_ext_data.line_num) continue;

        is_item_select = ((select_bottom > item_ptr->data.txt_ext_data.disp_rect.top + richtext_ptr->txt_margin)
            && (item_ptr->data.txt_ext_data.disp_rect.bottom - richtext_ptr->txt_margin > select_top));

        if (!is_item_select) continue;

        line_idx = 0;
        line_rect = item_ptr->data.txt_ext_data.disp_rect;
        line_rect.left += richtext_ptr->txt_margin;
        line_rect.right -= richtext_ptr->txt_margin;
        line_rect.top += richtext_ptr->txt_margin;

        //�������������ʼ�к���ʼ����Ҫ����
        is_top_out = (line_rect.top < select_top);

        if (is_top_out)
        {
            //1 �ҵ������п�ʼѡ��
            for (; line_idx < item_ptr->data.txt_ext_data.line_num; line_idx++)
            {
                line_rect.bottom = line_rect.top + item_ptr->data.txt_ext_data.line_height + richtext_ptr->line_space - 1;

                if (line_rect.top <= start_y && start_y <= line_rect.bottom)
                {
                    break;
                }

                line_rect.top = line_rect.bottom + 1;
            }

            start_line = line_idx;

            //2 �ҵ����Ǹ��ַ���ʼѡ��
            line_rect.top = line_rect.bottom - (item_ptr->data.txt_ext_data.line_height + richtext_ptr->line_space) + 1;
            line_len = GetLineLen(item_ptr, start_line);
            
            pos_line_rect.left = line_rect.left;
            pos_line_rect.right = line_rect.right;
            pos_line_rect.top = 0;
            pos_line_rect.bottom = item_ptr->data.txt_ext_data.line_height + richtext_ptr->line_space - 1;

            is_find = GetTpPos(&pos_line_rect,
                               GetItemAlign(richtext_ptr, item_ptr),
                               item_ptr->data.orig_data.text_data.buf.str_ptr + item_ptr->data.txt_ext_data.line_start_pos_ptr[start_line],
                               line_len,
                               start_x,
                               GetItemFont(richtext_ptr, item_ptr),
                               richtext_ptr->char_space,
                               &nearest_chr,
                               &under_chr,
                               &under_symb_len);
            if (is_find)
            {
                if (under_chr >= 0)
                {
                    start_pos = under_chr;
                    start_len = under_symb_len;
                }
                else
                {
                    start_pos = nearest_chr;
                }
            }
            else
            {
                start_pos = (nearest_chr == line_len) ? line_len : (-1);
            }
        }
        else
        {
            //���е�һ��
            start_line = 0;
            start_pos = -1;
        }

        //�ײ����������ֹ�к���ֹ����Ҫ����
        is_bottom_out = (item_ptr->data.txt_ext_data.disp_rect.bottom > select_bottom);

        if (is_bottom_out)
        {
            //1 ����һ�н���������֮ǰ���ж�֮�����
            for (; line_idx < item_ptr->data.txt_ext_data.line_num; line_idx++)
            {
                line_rect.bottom = line_rect.top + item_ptr->data.txt_ext_data.line_height + richtext_ptr->line_space - 1;

                if (line_rect.top <= stop_y && stop_y <= line_rect.bottom)
                {
                    break;
                }

                line_rect.top = line_rect.bottom + 1;
            }

            stop_line = line_idx;

            //2 ���ĸ��ַ�����
            line_rect.top = line_rect.bottom - (item_ptr->data.txt_ext_data.line_height + richtext_ptr->line_space) + 1;
            line_len = GetLineLen(item_ptr, stop_line);

            pos_line_rect.left = line_rect.left;
            pos_line_rect.right = line_rect.right;
            pos_line_rect.top = 0;
            pos_line_rect.bottom = item_ptr->data.txt_ext_data.line_height + richtext_ptr->line_space - 1;

            is_find = GetTpPos(&pos_line_rect,
                               GetItemAlign(richtext_ptr, item_ptr),
                               item_ptr->data.orig_data.text_data.buf.str_ptr + item_ptr->data.txt_ext_data.line_start_pos_ptr[stop_line],
                               line_len,
                               stop_x,
                               GetItemFont(richtext_ptr, item_ptr),
                               richtext_ptr->char_space,
                               &nearest_chr,
                               &under_chr,
                               &under_symb_len);

            if (is_find)
            {
                if (under_chr >= 0)
                {
                    stop_pos = under_chr;
                    stop_len = under_symb_len;
                }
                else
                {
                    stop_pos = nearest_chr;
                }
            }
            else
            {
                stop_pos = (nearest_chr == line_len) ? line_len : (-1);
            }
        }
        else
        {
            //ĩ�����һ��
            stop_line = item_ptr->data.txt_ext_data.line_num - 1;
            stop_pos = GetLineLen(item_ptr, stop_line);
        }

        //start line <= stop line
        //if start line == stop line, then start pos <= stop pos
        SCI_ASSERT(start_line <= stop_line); /*assert verified*/

        if (start_line == stop_line)
        {
            if (start_pos == stop_pos)
            {
                if (0 == start_len && 0 == stop_len)
                {
                    continue;
                }
                else
                {
                    if (start_len > stop_len) stop_len = start_len;
                }
            }
            else if (start_pos > stop_pos)
            {
                int32 temp_pos = start_pos;

                start_pos = stop_pos;
                stop_pos = temp_pos;
                stop_len = start_len;
            }
        }

        stop_pos += stop_len;
        stop_pos -= 1;

        if (start_pos < 0) start_pos = 0;

        //�������һ����ѡ������
        if (stop_pos < 0)
        {
            stop_line -= 1;
            stop_pos = GetLineLen(item_ptr, stop_line) - 1;
        }

        SCI_ASSERT(start_line <= stop_line); /*assert verified*/

        if (is_start_index_done)
        {
            richtext_ptr->stop_info.item_index = item_ptr->index;
            richtext_ptr->stop_info.line_index = stop_line;
            richtext_ptr->stop_info.pos_index = stop_pos;
        }
        else
        {
            richtext_ptr->start_info.item_index = item_ptr->index;
            richtext_ptr->start_info.line_index = start_line;
            richtext_ptr->start_info.pos_index = start_pos;

            richtext_ptr->stop_info.item_index = item_ptr->index;
            richtext_ptr->stop_info.line_index = stop_line;
            richtext_ptr->stop_info.pos_index = stop_pos;

            is_start_index_done = TRUE;
        }

        item_ptr->data.txt_ext_data.select_line_begin = start_line;
        item_ptr->data.txt_ext_data.select_line_end = stop_line;
        item_ptr->data.txt_ext_data.select_pos_begin = start_pos;
        item_ptr->data.txt_ext_data.select_pos_end = stop_pos;

        //cal select len & pos
        abs_start_pos = item_ptr->data.txt_ext_data.line_start_pos_ptr[start_line] + start_pos;
        abs_stop_pos = item_ptr->data.txt_ext_data.line_start_pos_ptr[stop_line] + stop_pos;
        item_ptr->data.txt_ext_data.select_pos = abs_start_pos;
        item_ptr->data.txt_ext_data.select_len = abs_stop_pos - abs_start_pos + 1;

        if (item_ptr->data.txt_ext_data.select_len > 0)
        {
            richtext_ptr->select_len += item_ptr->data.txt_ext_data.select_len;
            richtext_ptr->select_item_cnt += 1;
        }
    }


    return;
}

/*==============================================================================
Description: �������Ƿ�Ϸ�
Global resource dependence: NONE
Author: hua.fang
Note:
==============================================================================*/
LOCAL BOOLEAN CheckHighlightPos(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    RICHTEXT_SEL_INFO_T *start_info_ptr,
    RICHTEXT_SEL_INFO_T *stop_info_ptr
    )
{
    BOOLEAN is_valid = FALSE;

    if (PNULL == start_info_ptr || PNULL == stop_info_ptr)
    {
        return FALSE;
    }

    if (start_info_ptr->item_index < stop_info_ptr->item_index)
    {
        is_valid = TRUE;
    }
    else if (start_info_ptr->item_index == stop_info_ptr->item_index)
    {
        if (start_info_ptr->line_index < stop_info_ptr->line_index)
        {
            is_valid = TRUE;
        }
        else if (start_info_ptr->line_index == stop_info_ptr->line_index)
        {
            if (start_info_ptr->pos_index < stop_info_ptr->pos_index)
            {
                is_valid = TRUE;
            }
            else if (start_info_ptr->pos_index == stop_info_ptr->pos_index)
            {
                RICHTEXT_ITEM_T *item_ptr = PNULL;

                if (MMI_RESULT_TRUE == GetItemByIndex(richtext_ptr, start_info_ptr->item_index, &item_ptr))
                {
                    if (start_info_ptr->pos_index != GetLineLen(item_ptr, start_info_ptr->line_index))
                    {
                        is_valid = TRUE;
                    }
                }
            }
        }
    }

    return is_valid;
}

/*==============================================================================
Description: ��start��������cont rect��֮��ֵv stop point
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void SetStartPoint(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    GUI_POINT_T *point_ptr
)
{
    GUI_POINT_T start_point = {0};

    if (PNULL == richtext_ptr || PNULL == point_ptr)
    {
        return;
    }

    start_point = *point_ptr;

    if (!GUI_PointIsInRect(start_point, richtext_ptr->cont_rect))
    {
        if (start_point.x < richtext_ptr->cont_rect.left)
        {
            start_point.x = richtext_ptr->cont_rect.left;
        }
        else if (start_point.x > richtext_ptr->cont_rect.right)
        {
            start_point.x = richtext_ptr->cont_rect.right;
        }

        if (start_point.y < richtext_ptr->cont_rect.top)
        {
            start_point.y = richtext_ptr->cont_rect.top;
        }
        else if (start_point.y > richtext_ptr->cont_rect.bottom)
        {
            start_point.y = richtext_ptr->cont_rect.bottom;
        }
    }

    if (MMITHEME_DRAGMODE_DEFAULT != richtext_ptr->drag_mode
        && RICHTEXT_STATE_SELECTING == richtext_ptr->select_state)
    {
        BOOLEAN is_in = FALSE;
        BOOLEAN is_find = FALSE;
        RICHTEXT_SEL_INFO_T sel_info = {0};
        GUI_RECT_T  end_rect = {0};

        GetHighlightRect(richtext_ptr, FALSE, &end_rect);

        if (start_point.y > end_rect.bottom
            || (start_point.y >= end_rect.top && start_point.y <= end_rect.bottom && start_point.x > end_rect.left))
        {
            return;
        }

        is_in = IsInTextRegion(richtext_ptr, &start_point, &is_find, &sel_info);

        if (!is_in)
        {
            return;
        }

        if (!CheckHighlightPos(richtext_ptr, &sel_info, &richtext_ptr->stop_info))
        {
            return;
        }
    }

    richtext_ptr->v_start_x = start_point.x - richtext_ptr->cont_rect.left;
    richtext_ptr->v_start_y = richtext_ptr->disp_top + start_point.y - richtext_ptr->cont_rect.top;

    richtext_ptr->is_select_data_dirty = TRUE;

    return;
}


/*==============================================================================
Description: ��stop��������cont rect��֮��ֵv stop point
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void SetStopPoint(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    GUI_POINT_T *point_ptr
)
{
    GUI_POINT_T stop_point = {0};
    GUI_RECT_T  start_rect = {0};
    BOOLEAN is_in = FALSE;
    BOOLEAN is_find = FALSE;
    RICHTEXT_SEL_INFO_T sel_info = {0};

    if (PNULL == richtext_ptr || PNULL == point_ptr)
    {
        return;
    }

    stop_point = *point_ptr;

    if (!GUI_PointIsInRect(stop_point, richtext_ptr->cont_rect))
    {
        if (stop_point.x < richtext_ptr->cont_rect.left)
        {
            stop_point.x = richtext_ptr->cont_rect.left;
        }
        else if (stop_point.x > richtext_ptr->cont_rect.right)
        {
            stop_point.x = richtext_ptr->cont_rect.right;
        }

        if (stop_point.y < richtext_ptr->cont_rect.top)
        {
            stop_point.y = richtext_ptr->cont_rect.top;
        }
        else if (stop_point.y > richtext_ptr->cont_rect.bottom)
        {
            stop_point.y = richtext_ptr->cont_rect.bottom;
        }
    }

    if (MMITHEME_DRAGMODE_DEFAULT != richtext_ptr->drag_mode
        && RICHTEXT_STATE_SELECTING == richtext_ptr->select_state)
    {
        GetHighlightRect(richtext_ptr, TRUE, &start_rect);

        if (stop_point.y < start_rect.top
            || (stop_point.y >= start_rect.top && stop_point.y <= start_rect.bottom && stop_point.x <= start_rect.right))
        {
            return;
        }

        is_in = IsInTextRegion(richtext_ptr, &stop_point, &is_find, &sel_info);

        if (!is_in)
        {
            return;
        }

        if (!CheckHighlightPos(richtext_ptr, &richtext_ptr->start_info, &sel_info))
        {
            return;
        }
    }

    richtext_ptr->v_stop_x = stop_point.x - richtext_ptr->cont_rect.left;
    richtext_ptr->v_stop_y = richtext_ptr->disp_top + stop_point.y - richtext_ptr->cont_rect.top;

    richtext_ptr->is_select_data_dirty = TRUE;

    return;
}


/*==============================================================================
Description: ���ݾ����������¹�������Ϣ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void ReGenLineInfo(CTRLRICHTEXT_OBJ_T *richtext_ptr)
{
    RICHTEXT_ITEM_T *item_ptr = PNULL;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return;
    }

    richtext_ptr->is_line_data_dirty = FALSE;

    if (RICHTEXT_STATE_SELECT_NONE == richtext_ptr->select_state)
    {
        return;
    }

    item_ptr = richtext_ptr->first_item_ptr;

    if (PNULL == item_ptr)
    {
        return;
    }

    for (; PNULL != item_ptr; item_ptr = item_ptr->next_ptr)
    {
        uint16 abs_start_pos = 0;
        uint16 abs_stop_pos = 0;
        uint16 start_line = 0;
        uint16 start_pos = 0;
        uint16 stop_line = 0;
        uint16 stop_pos = 0;
        uint16 line_idx = 0;

        if (GUIRICHTEXT_TEXT_BUF != item_ptr->data.orig_data.text_type) continue;

        if (0 == item_ptr->data.txt_ext_data.select_len) continue;

        abs_start_pos = item_ptr->data.txt_ext_data.select_pos;
        abs_stop_pos = item_ptr->data.txt_ext_data.select_pos + item_ptr->data.txt_ext_data.select_len - 1;

        for (; line_idx < item_ptr->data.txt_ext_data.line_num; line_idx++)
        {
            if (abs_start_pos < item_ptr->data.txt_ext_data.line_start_pos_ptr[line_idx+1])
            {
                start_line = line_idx;
                start_pos = abs_start_pos - item_ptr->data.txt_ext_data.line_start_pos_ptr[line_idx];
                break;
            }
        }

        for (; line_idx < item_ptr->data.txt_ext_data.line_num; line_idx++)
        {
            if (abs_stop_pos < item_ptr->data.txt_ext_data.line_start_pos_ptr[line_idx+1])
            {
                stop_line = line_idx;
                stop_pos = abs_stop_pos - item_ptr->data.txt_ext_data.line_start_pos_ptr[line_idx];
                break;
            }
        }

        item_ptr->data.txt_ext_data.select_line_begin = start_line;
        item_ptr->data.txt_ext_data.select_line_end = stop_line;
        item_ptr->data.txt_ext_data.select_pos_begin = start_pos;
        item_ptr->data.txt_ext_data.select_pos_end = stop_pos;
    }


    return;
}

/*==============================================================================
Description: ClearClipBoardInfo
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void ClearClipBoardInfo(CTRLRICHTEXT_OBJ_T *richtext_ptr)
{
    if (richtext_ptr->is_clipboard_support)
    {
        //�����������ã�Ҫ��������
        richtext_ptr->is_select_data_dirty = TRUE;
        richtext_ptr->is_line_data_dirty = FALSE;
        richtext_ptr->select_len = 0;
        richtext_ptr->select_item_cnt = 0;
        SCI_MEMSET(&richtext_ptr->start_info, 0, sizeof(RICHTEXT_SEL_INFO_T));
        SCI_MEMSET(&richtext_ptr->stop_info, 0, sizeof(RICHTEXT_SEL_INFO_T));

        if (richtext_ptr->drag_select_timer > 0)
        {
            MMK_StopTimer(richtext_ptr->drag_select_timer);
            richtext_ptr->drag_select_timer = 0;
        }

        if (!richtext_ptr->is_info_protected)
        {
            richtext_ptr->select_state = RICHTEXT_STATE_SELECT_NONE;
            richtext_ptr->tp_start_point.x = 0;
            richtext_ptr->tp_start_point.y = 0;
            richtext_ptr->tp_stop_point.x = 0;
            richtext_ptr->tp_stop_point.y = 0;
            richtext_ptr->v_start_x = 0;
            richtext_ptr->v_start_y = 0;
            richtext_ptr->v_stop_x = 0;
            richtext_ptr->v_stop_y = 0;
        }
    }

    return;
}
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : create slider
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void CreateRichtextSlider(
    CTRLRICHTEXT_OBJ_T  *richtext_ptr  //in:
)
{
    CTRLBASE_OBJ_T* basectrl_ptr = (CTRLBASE_OBJ_T*)richtext_ptr;
    int32 i = 0;

    if (MMITHEME_DRAGMODE_DEFAULT != richtext_ptr->drag_mode)
    {
        for (i = 0; i < MMITHEME_SLIDER_TYPE_MAX; i++)
        {
            richtext_ptr->slider[i] =
                MMITHEME_CreateSlider(i, richtext_ptr->win_id, basectrl_ptr->handle);
        }
    }
}

/*****************************************************************************/
//  Description : destroy slider
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void DestroyRichtextSlider(
    CTRLRICHTEXT_OBJ_T  *richtext_ptr  //in:
)
{
    int32 i = 0;

    // �������
    for (i = 0; i < MMITHEME_SLIDER_TYPE_MAX; i++)
    {
        if (0 != richtext_ptr->slider[i])
        {
            MMITHEME_DestroySlider(richtext_ptr->slider[i]);
        }
    }
}

/*****************************************************************************/
//  Description : ��ʾ����
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void ShowRichtextSlider(
    CTRLRICHTEXT_OBJ_T  *richtext_ptr
)
{
    if (MMITHEME_DRAGMODE_DEFAULT != richtext_ptr->drag_mode)
    {
        if (0 == richtext_ptr->slider[0])
        {
            CreateRichtextSlider(richtext_ptr);
        }

        if (GUIRICHTEXT_DRAGSTATE_SELECT == richtext_ptr->drag_state)
        {
            MMITHEME_HideSlider(richtext_ptr->slider[MMITHEME_SLIDER_TYPE_BLOCK]);
            MMITHEME_ShowSlider(richtext_ptr->slider[MMITHEME_SLIDER_TYPE_LEFT_SELECTOR], 0, 0);
            MMITHEME_ShowSlider(richtext_ptr->slider[MMITHEME_SLIDER_TYPE_RIGHT_SELECTOR], 0, 0);
        }
        else
        {
            MMITHEME_HideSlider(richtext_ptr->slider[MMITHEME_SLIDER_TYPE_BLOCK]);
            MMITHEME_HideSlider(richtext_ptr->slider[MMITHEME_SLIDER_TYPE_LEFT_SELECTOR]);
            MMITHEME_HideSlider(richtext_ptr->slider[MMITHEME_SLIDER_TYPE_RIGHT_SELECTOR]);
        }
    }
}
#endif
/*****************************************************************************/
//  Description : �ı们��״̬
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC void ChangeRichtextDragState(
    CTRLRICHTEXT_OBJ_T  *richtext_ptr,
    GUIRICHTEXT_DRAGSTATE_E drag_state
)
{
    if (MMITHEME_DRAGMODE_DEFAULT != richtext_ptr->drag_mode)
    {
        if (GUIRICHTEXT_DRAGSTATE_NORMAL == drag_state)
        {
            richtext_ptr->drag_state = GUIRICHTEXT_DRAGSTATE_NORMAL;
            richtext_ptr->tp_mode = RICHTEXT_TPMODE_NONE;

            if (richtext_ptr->is_clipboard_support)
            {
                //�����������ã�Ҫ��������
                richtext_ptr->is_select_data_dirty = TRUE;
                richtext_ptr->is_line_data_dirty = FALSE;
                richtext_ptr->select_len = 0;
                richtext_ptr->select_item_cnt = 0;
                SCI_MEMSET(&richtext_ptr->start_info, 0, sizeof(RICHTEXT_SEL_INFO_T));
                SCI_MEMSET(&richtext_ptr->stop_info, 0, sizeof(RICHTEXT_SEL_INFO_T));

                if (richtext_ptr->drag_select_timer > 0)
                {
                    MMK_StopTimer(richtext_ptr->drag_select_timer);
                    richtext_ptr->drag_select_timer = 0;
                }

                if (!richtext_ptr->is_info_protected)
                {
                    richtext_ptr->select_state = RICHTEXT_STATE_SELECT_NONE;
                    richtext_ptr->v_start_x = 0;
                    richtext_ptr->v_start_y = 0;
                    richtext_ptr->v_stop_x = 0;
                    richtext_ptr->v_stop_y = 0;
                }
            }
        }
        else if (GUIRICHTEXT_DRAGSTATE_SELECT == drag_state)
        {
            richtext_ptr->drag_state = GUIRICHTEXT_DRAGSTATE_SELECT;
            richtext_ptr->select_state = RICHTEXT_STATE_SELECTED;
        }
#if defined(GUIF_CLIPBOARD)
        StopRichtextSliderTimer(richtext_ptr);

        if (GUIRICHTEXT_DRAGSTATE_SELECT == richtext_ptr->drag_state)
        {
            StartRichtextSliderTimer(richtext_ptr);
        }

        ShowRichtextSlider(richtext_ptr);
#endif
    }
}
#ifdef TOUCH_PANEL_SUPPORT
/*****************************************************************************/
//  Description : start slider timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
LOCAL void StartRichtextSliderTimer(
    CTRLRICHTEXT_OBJ_T    *richtext_ptr  //in:
)
{
    CTRLBASE_OBJ_T* basectrl_ptr = (CTRLBASE_OBJ_T*)richtext_ptr;
    //include progress bar
    if ((0 == richtext_ptr->slider_dis_timer_id) &&
        (MMK_IsFocusWin(richtext_ptr->win_id)))
    {
        richtext_ptr->slider_dis_timer_id = MMK_CreateWinTimer(basectrl_ptr->handle,
                                                richtext_ptr->slider_display_period,
                                                FALSE);
    }
}

/*****************************************************************************/
//  Description : stop slider timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void StopRichtextSliderTimer(
    CTRLRICHTEXT_OBJ_T    *richtext_ptr  //in:
)
{
    if (0 < richtext_ptr->slider_dis_timer_id)
    {
        MMK_StopTimer(richtext_ptr->slider_dis_timer_id);
        richtext_ptr->slider_dis_timer_id = 0;
    }
}

/*****************************************************************************/
//  Description : handle slider timer
//  Global resource dependence : 
//  Author: Jassmine
//  Note:
/*****************************************************************************/
PUBLIC void HandleRichtextSliderTimer(
    CTRLRICHTEXT_OBJ_T  *richtext_ptr  //in:
)
{
    ChangeRichtextDragState(richtext_ptr, GUIRICHTEXT_DRAGSTATE_NORMAL);

    if (0 != richtext_ptr->clipbrd_win_handle)
    {
        MMITHEME_CloseClipbrdMenu(richtext_ptr->clipbrd_win_handle);
    }
}

/*****************************************************************************/
//  Description : ��ʼ�����϶�
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void HandleRichtextBeginSlider(
    CTRLRICHTEXT_OBJ_T     *richtext_ptr,
    DPARAM             param
)
{
    int32 slider_type = (int32)param;
#if defined(GUIF_CLIPBOARD)
    StopRichtextSliderTimer(richtext_ptr);
#endif
    switch(slider_type)
    {
    case MMITHEME_SLIDER_TYPE_LEFT_SELECTOR:
        richtext_ptr->tpdown_slider = GUIRICHTEXT_SLIDER_LEFT_SELECTOR;
        richtext_ptr->tp_mode = RICHTEXT_TPMODE_DRAG;
        richtext_ptr->select_state = RICHTEXT_STATE_SELECTING;
        break;

    case MMITHEME_SLIDER_TYPE_RIGHT_SELECTOR:
        richtext_ptr->tpdown_slider = GUIRICHTEXT_SLIDER_RIGHT_SELECTOR;
        richtext_ptr->tp_mode = RICHTEXT_TPMODE_DRAG;
        richtext_ptr->select_state = RICHTEXT_STATE_SELECTING;
        break;

    default:
        richtext_ptr->tpdown_slider = GUIRICHTEXT_SLIDER_NONE;
        richtext_ptr->select_state = RICHTEXT_STATE_SELECT_NONE;
        richtext_ptr->tp_mode = RICHTEXT_TPMODE_NONE;
        break;
    }
}

/*****************************************************************************/
//  Description : �϶�����
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void HandleRichtextMoveSlider(
    CTRLRICHTEXT_OBJ_T     *richtext_ptr,
    DPARAM             param
)
{
    MMITHEME_SLIDER_MSG_PARAM_T *slider_param_ptr = PNULL;
    MMI_MULTI_KEY_TP_MSG_PARAM_T    tp_msg = {0};

    if (GUIRICHTEXT_SLIDER_NONE == richtext_ptr->tpdown_slider)
    {
        return;
    }

    slider_param_ptr = (MMITHEME_SLIDER_MSG_PARAM_T*)param;
    tp_msg.cur_tp_point = slider_param_ptr->tp_point;

    RichTextCtrlHandleMsg((CTRLBASE_OBJ_T*)richtext_ptr, MSG_TP_PRESS_MOVE, &tp_msg);
}

/*****************************************************************************/
//  Description : �����϶�����
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
LOCAL void HandleRichtextEndSlider(
    CTRLRICHTEXT_OBJ_T     *richtext_ptr,
    DPARAM             param
)
{
    BOOLEAN         result = FALSE;
    GUI_POINT_T     tp_point = {0};
    GUI_RECT_T      cursor_rect = {0};

    richtext_ptr->tpdown_slider = GUIRICHTEXT_SLIDER_NONE;
    richtext_ptr->select_state = RICHTEXT_STATE_SELECTED;

    StartRichtextSliderTimer(richtext_ptr);

    if ((int32)MMITHEME_SLIDER_TYPE_LEFT_SELECTOR == (int32)param)
    {
        result = GetHighlightRect(richtext_ptr, TRUE, &cursor_rect);
    }
    else
    {
        result = GetHighlightRect(richtext_ptr, FALSE, &cursor_rect);
    }

    if (result)
    {
        tp_point.x = cursor_rect.left;
        tp_point.y = cursor_rect.top;

        ShowClipboardMenu(richtext_ptr, &tp_point, TRUE);
    }
}
#endif
/*==============================================================================
Description: whether point in text region
Global resource dependence: NONE
Author: hua.fang
Note:
==============================================================================*/
LOCAL BOOLEAN IsInTextRegion(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    GUI_POINT_T *point_ptr,
    BOOLEAN *is_find_ptr,
    RICHTEXT_SEL_INFO_T *sel_info_ptr
    )
{
    BOOLEAN is_in = FALSE;
    RICHTEXT_ITEM_T *item_ptr = PNULL;
    int16 pt_y = 0;
    int16 pt_x = 0;

    if (PNULL == richtext_ptr || PNULL == point_ptr
        || PNULL == sel_info_ptr || PNULL == is_find_ptr)
    {
        return FALSE;
    }

    item_ptr = richtext_ptr->first_item_ptr;

    if (PNULL == item_ptr)
    {
        return FALSE;
    }

    pt_x = point_ptr->x - richtext_ptr->cont_rect.left;
    pt_y = richtext_ptr->disp_top + point_ptr->y - richtext_ptr->cont_rect.top;

    for (; PNULL != item_ptr; item_ptr = item_ptr->next_ptr)
    {
        BOOLEAN is_item_select = FALSE;
        GUI_BIG_RECT_T line_rect = {0};
        uint16 line_idx = 0;
        BOOLEAN is_find = 0;
        int32 nearest_chr = 0;
        int32 under_chr = 0;
        int32 under_symb_len = 0;
        uint16  line_len = 0;
        GUI_RECT_T  pos_line_rect = {0};

        if (GUIRICHTEXT_TEXT_BUF != item_ptr->data.orig_data.text_type) continue;

        if (0 == item_ptr->data.txt_ext_data.line_num) continue;

        is_item_select = ((pt_y > item_ptr->data.txt_ext_data.disp_rect.top + richtext_ptr->txt_margin)
            && (item_ptr->data.txt_ext_data.disp_rect.bottom - richtext_ptr->txt_margin > pt_y));

        if (!is_item_select) continue;

        sel_info_ptr->item_index = item_ptr->index;

        line_idx = 0;
        line_rect = item_ptr->data.txt_ext_data.disp_rect;
        line_rect.left += richtext_ptr->txt_margin;
        line_rect.right -= richtext_ptr->txt_margin;
        line_rect.top += richtext_ptr->txt_margin;

        //1 �ҵ������п�ʼѡ��
        for (; line_idx < item_ptr->data.txt_ext_data.line_num; line_idx++)
        {
            line_rect.bottom = line_rect.top + item_ptr->data.txt_ext_data.line_height + richtext_ptr->line_space - 1;

            if (line_rect.top <= pt_y && pt_y <= line_rect.bottom)
            {
                break;
            }

            line_rect.top = line_rect.bottom + 1;
        }

        sel_info_ptr->line_index = line_idx;

        //2 �ҵ����Ǹ��ַ���ʼѡ��
        line_len = GetLineLen(item_ptr, line_idx);

        pos_line_rect.left = line_rect.left;
        pos_line_rect.right = line_rect.right;
        pos_line_rect.top = 0;
        pos_line_rect.bottom = item_ptr->data.txt_ext_data.line_height + richtext_ptr->line_space - 1;

        is_find = GetTpPos(&pos_line_rect,
                           GetItemAlign(richtext_ptr, item_ptr),
                           item_ptr->data.orig_data.text_data.buf.str_ptr + item_ptr->data.txt_ext_data.line_start_pos_ptr[line_idx],
                           line_len,
                           pt_x,
                           GetItemFont(richtext_ptr, item_ptr),
                           richtext_ptr->char_space,
                           &nearest_chr,
                           &under_chr,
                           &under_symb_len);
        if (is_find)
        {
            if (under_chr >= 0)
            {
                sel_info_ptr->pos_index = under_chr;
            }
            else
            {
                sel_info_ptr->pos_index = nearest_chr;
            }
        }
        else
        {
            sel_info_ptr->pos_index = (nearest_chr == line_len) ? line_len : (-1);
        }

        *is_find_ptr = is_find;

        is_in = TRUE;

        break;
    }

    return is_in;
}

/*==============================================================================
Description: get highlight rect
Global resource dependence: NONE
Author: hua.fang
Note:
==============================================================================*/
LOCAL BOOLEAN GetHighlightRect(
    CTRLRICHTEXT_OBJ_T *richtext_ptr,
    BOOLEAN             is_start,
    GUI_RECT_T          *rect_ptr
    )
{
    int16       highlight_pos = 0;
    uint16      item_index = 0;
    uint16      line_index = 0;
    uint16      char_index = 0;
    GUI_RECT_T  line_rect = {0};
    GUI_BIG_RECT_T  v_line_rect = {0};
    RICHTEXT_ITEM_T *item_ptr = PNULL;
    uint16 *start_pos_ptr = PNULL;
    wchar *str_ptr = PNULL;

    if (is_start)
    {
        item_index = richtext_ptr->start_info.item_index;
    }
    else
    {
        item_index = richtext_ptr->stop_info.item_index;
    }

    if (PNULL == rect_ptr
        || MMI_RESULT_TRUE != GetItemByIndex(richtext_ptr, item_index, &item_ptr))
    {
        return FALSE;
    }

    if (is_start)
    {
        line_index = richtext_ptr->start_info.line_index;//item_ptr->data.txt_ext_data.select_line_begin;
        char_index = richtext_ptr->start_info.pos_index;//item_ptr->data.txt_ext_data.select_pos_begin;
    }
    else
    {
        line_index = richtext_ptr->stop_info.line_index;//item_ptr->data.txt_ext_data.select_line_end;
        char_index = richtext_ptr->stop_info.pos_index;//item_ptr->data.txt_ext_data.select_pos_end;

        if (GetLineLen(item_ptr, line_index) > 0)
        {
            char_index++;
        }
    }

    v_line_rect = item_ptr->data.txt_ext_data.disp_rect;
    v_line_rect.left += richtext_ptr->txt_margin;
    v_line_rect.right -= richtext_ptr->txt_margin;
    v_line_rect.top += richtext_ptr->txt_margin;

    v_line_rect.top += (item_ptr->data.txt_ext_data.line_height + richtext_ptr->line_space)*line_index;
    v_line_rect.bottom = v_line_rect.top + item_ptr->data.txt_ext_data.line_height + richtext_ptr->line_space - 1;

    line_rect.left = v_line_rect.left + richtext_ptr->cont_rect.left;
    line_rect.top = v_line_rect.top - richtext_ptr->disp_top + richtext_ptr->cont_rect.top;
    line_rect.right = v_line_rect.right + richtext_ptr->cont_rect.left;
    line_rect.bottom = v_line_rect.bottom - richtext_ptr->disp_top + richtext_ptr->cont_rect.top;

    str_ptr = item_ptr->data.orig_data.text_data.buf.str_ptr;
    start_pos_ptr = item_ptr->data.txt_ext_data.line_start_pos_ptr;

    //set cursor left by text rect
    highlight_pos = GUI_GetCusorCoordinate(
        GetItemAlign(richtext_ptr, item_ptr),
        &line_rect,
        TEXT_DIR_AUTO,
        str_ptr + start_pos_ptr[line_index],
        GetLineLen(item_ptr, line_index),
        char_index,
        GetItemFont(richtext_ptr, item_ptr),
        richtext_ptr->char_space);

    rect_ptr->left = highlight_pos;
    rect_ptr->top = line_rect.top;
    rect_ptr->right = rect_ptr->left + 1;
    rect_ptr->bottom = line_rect.bottom;

    return TRUE;
}

#endif //GUIF_CLIPBOARD


/*==============================================================================
Description: ClearCtrlInfo
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void ClearCtrlInfo(CTRLRICHTEXT_OBJ_T *richtext_ptr)
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return;
    }

#ifdef GUIF_CLIPBOARD
    ClearClipBoardInfo(richtext_ptr);
#endif

    return;
}


/*==============================================================================
Description: ���ƻ����հײ���������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL int16 FixDispTop(CTRLRICHTEXT_OBJ_T *richtext_ptr, int16 disp_top)
{
    int32 fix_top = 0;
    int16 cont_height = 0;

    
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return 0;
    }

    cont_height = GetContHeight(richtext_ptr);
    
    if (disp_top < (-1)*cont_height/2)
    {
        richtext_ptr->slide_v = 0;
        fix_top = (-1)*cont_height/2;
    }
    else if (disp_top > richtext_ptr->disp_height - cont_height/2)
    {
        richtext_ptr->slide_v = 0;
        fix_top = richtext_ptr->disp_height - cont_height/2;
    }
    else
    {
        fix_top = disp_top;
    }

    return fix_top;
}


/*==============================================================================
Description: ��������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: ���ƻ����հײ���������
==============================================================================*/
LOCAL void SlideFlingTick(uint8 timer_id, uint32 param)
{
    CTRLRICHTEXT_OBJ_T *richtext_ptr = (CTRLRICHTEXT_OBJ_T *)param;

    if (PNULL == richtext_ptr)
    {
        return;
    }

    SCI_ASSERT(timer_id == richtext_ptr->slide_timer); /*assert verified*/

    //��ʼ��timer id
    richtext_ptr->slide_timer = 0;

    if (richtext_ptr->slide_v < (-1)*FLING_MIN_VELOCITY|| FLING_MIN_VELOCITY < richtext_ptr->slide_v)
    {
        int32 delta_y = 0;
        
        //Fling...
#ifdef TOUCH_PANEL_SUPPORT        
        delta_y  = MMK_GetFlingOffset(
                       0,
                       richtext_ptr->slide_v,
                       RICHTEXT_SLIDE_FLING_TICK/1000.0f,
                       &(richtext_ptr->slide_v));
#endif

        richtext_ptr->disp_top = FixDispTop(richtext_ptr, richtext_ptr->disp_top - delta_y);

        DrawRichText(richtext_ptr);

    }

    if ((-1)*FLING_MIN_VELOCITY < richtext_ptr->slide_v && richtext_ptr->slide_v < FLING_MIN_VELOCITY)
    {
        richtext_ptr->slide_v = 0;
        SlideRecoverTick(0, (uint32)richtext_ptr);
    }
    else
    {
        //go on
        richtext_ptr->slide_timer = MMK_CreateTimerCallback(
                                        RICHTEXT_SLIDE_FLING_TICK,
                                        SlideFlingTick,
                                        (uint32)richtext_ptr,
                                        FALSE);
    }

    return;
}


/*==============================================================================
Description: �����ָ�
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �����Ƿ�ȫ������Ҫ�����ָ�
!!!��֤�����հײ���������
==============================================================================*/
LOCAL BOOLEAN SlideRecover(CTRLRICHTEXT_OBJ_T *richtext_ptr, BOOLEAN need_draw)
{
    CTRLBASE_OBJ_T* basectrl_ptr = (CTRLBASE_OBJ_T*)richtext_ptr;
    int32 last_top = 0;
    int32 disp_bottom = 0;
    int16 cont_height = 0;
    int32 real_bottom = 0;
    int32 delta_y = 0;
    BOOLEAN need_go_on = FALSE;

    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return FALSE;
    }

    disp_bottom = GetDispBottom(richtext_ptr);
    cont_height = GetContHeight(richtext_ptr);

    real_bottom = MAX(cont_height, richtext_ptr->disp_height);
    real_bottom -= 1;

    if (richtext_ptr->disp_top < 0)
    {
        last_top = 0;
    }
    else if (richtext_ptr->disp_top > 0 && disp_bottom > real_bottom)
    {
        if (cont_height < richtext_ptr->disp_height)
        {
            last_top = richtext_ptr->disp_height - cont_height;
        }
        else
        {
            last_top = 0;
        }
    }
    else
    {
        richtext_ptr->tp_mode = RICHTEXT_TPMODE_NONE;
        return FALSE;
    }

    delta_y = richtext_ptr->disp_top - last_top;

    if (need_draw)
    {
#if 0
        need_go_on = TRUE;

        if (delta_y < (-1)*RICHTEXT_SLIDE_RECOVER_MIN)
        {
            richtext_ptr->disp_top += RICHTEXT_SLIDE_RECOVER_MIN;
        }
        else if (RICHTEXT_SLIDE_RECOVER_MIN < delta_y)
        {
            richtext_ptr->disp_top -= RICHTEXT_SLIDE_RECOVER_MIN;
        }
        else
        {
            richtext_ptr->disp_top -= delta_y;
            richtext_ptr->tp_mode = RICHTEXT_TPMODE_NONE;
            need_go_on = FALSE;
        }

        DrawRichText(richtext_ptr);
#else
        int32 i = 0;
        int16 original_top = richtext_ptr->disp_top;

#ifndef WIN32
        CHNG_FREQ_SetArmClk(MMITHEME_GetFreqHandler(), FREQ_INDEX_MP3);      
#endif  

        for (i = MMK_RECOVER_SLIDE_START_FRAME; i < MMK_RECOVER_SLIDE_TOTAL_FRAME; i++)
        {
            int32 dy = MMK_ClcltDampPlayOffset(DAMP_PLAY_TYPE_1, delta_y, MMK_RECOVER_SLIDE_TOTAL_FRAME, i);
            
            richtext_ptr->disp_top = original_top - dy;

            DrawRichText(richtext_ptr);

            GUILCD_InvalidateRect(basectrl_ptr->lcd_dev_info.lcd_id, basectrl_ptr->rect,0);
        }

        richtext_ptr->tp_mode = RICHTEXT_TPMODE_NONE;

#ifndef WIN32
        CHNG_FREQ_RestoreARMClk(MMITHEME_GetFreqHandler());
#endif

#endif

    }
    else
    {
        richtext_ptr->tp_mode = RICHTEXT_TPMODE_NONE;
        richtext_ptr->disp_top -= delta_y;
    }

    return need_go_on;
}



/*==============================================================================
Description: �����ָ�
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void SlideRecoverTick(uint8 timer_id, uint32 param)
{
    CTRLRICHTEXT_OBJ_T *richtext_ptr = (CTRLRICHTEXT_OBJ_T *)param;
    BOOLEAN need_go_on = FALSE;

    if (PNULL == richtext_ptr)
    {
        return;
    }

    SCI_ASSERT(timer_id == richtext_ptr->slide_timer); /*assert verified*/

    //��ʼ��timer id
    richtext_ptr->slide_timer = 0;

    //recover...
    need_go_on = SlideRecover(richtext_ptr, TRUE);

    //go on
    if (need_go_on)
    {
        richtext_ptr->slide_timer = MMK_CreateTimerCallback(
                                        RICHTEXT_SLIDE_RECOVER_TICK,
                                        SlideRecoverTick,
                                        (uint32)richtext_ptr,
                                        FALSE);
    }

    return;
}


/*==============================================================================
Description: ���slideЧ��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
LOCAL void SlideClean(CTRLRICHTEXT_OBJ_T *richtext_ptr)
{
    //kevin.lou modified:delete assert
    //SCI_ASSERT(PNULL != richtext_ptr); /*assert verified*/
    if (PNULL == richtext_ptr)
    {
        return;
    }

    MMK_StopTimer(richtext_ptr->slide_timer);
    richtext_ptr->slide_timer = 0;

    SlideRecover(richtext_ptr, FALSE);
    
    return;
}




/*==============================================================================
Description: GUIRICHTEXT_AddItem
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
   ������һ��ͼƬitem, ����ͼƬ��buf����ʱ, GUIRICHTEXT_IMAGE_T
   �ṹ�е�buf_ptr���ɴ��������ڴ��, �ؼ�������������ͷ�
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_AddItem(
    MMI_CTRL_ID_T ctrl_id,
    GUIRICHTEXT_ITEM_T const *item_ptr,
    uint16 *index_ptr
)
{
    CTRLRICHTEXT_OBJ_T *richtext_ptr = PNULL;
    RICHTEXT_ITEM_T *new_item_ptr = PNULL;

    if (PNULL == item_ptr || PNULL == index_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    //controler
    richtext_ptr = GetRichTextPtr(ctrl_id);

    if (PNULL == richtext_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    //���䲢��ʼ���ڴ�
    new_item_ptr = (RICHTEXT_ITEM_T*)SCI_ALLOCA(sizeof(RICHTEXT_ITEM_T));

    SCI_ASSERT(PNULL != new_item_ptr); /*assert verified*/

    //��ʼ��
    InitItem(richtext_ptr, item_ptr, new_item_ptr);

    //��ӵ��뻷��������ĩβָ������ͷָ���ǰ�档prevָ��ջ���nextָ�뿪��
    if (PNULL == richtext_ptr->first_item_ptr)
    {
        new_item_ptr->prev_ptr = new_item_ptr;
        new_item_ptr->next_ptr = PNULL;
        richtext_ptr->first_item_ptr = new_item_ptr;

        new_item_ptr->index = 0;
    }
    else
    {
        new_item_ptr->prev_ptr = richtext_ptr->first_item_ptr->prev_ptr;
        new_item_ptr->next_ptr = PNULL;
        richtext_ptr->first_item_ptr->prev_ptr->next_ptr = new_item_ptr;

        richtext_ptr->first_item_ptr->prev_ptr = new_item_ptr;

        new_item_ptr->index = new_item_ptr->prev_ptr->index + 1;
    }

    //���������Ԫ��
    if (GUIRICHTEXT_FOCUS_SHIFT_NONE != richtext_ptr->focus_mode)
    {
        SetItemFocusElememt(richtext_ptr, new_item_ptr);
    }

    if (!richtext_ptr->need_cal_ext_data)
    {
        CalcItemExtData(richtext_ptr, new_item_ptr);
    }

    //���¼���virtual rect
    richtext_ptr->need_cal_rect = TRUE;

    //����ֵ
    *index_ptr = new_item_ptr->index;

    return MMI_RESULT_TRUE;
}


/*==============================================================================
Description: GUIRICHTEXT_DeleteItem
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_DeleteItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16 item_index
)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    CTRLRICHTEXT_OBJ_T *richtext_ptr = PNULL;
    RICHTEXT_ITEM_T *item_ptr = PNULL;
    RICHTEXT_ITEM_T *prev_item_ptr = PNULL;
    RICHTEXT_ITEM_T *next_item_ptr = PNULL;

    //controler
    richtext_ptr = GetRichTextPtr(ctrl_id);

    if (PNULL == richtext_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    result = GetItemByIndex(richtext_ptr, item_index, &item_ptr);

    if (MMI_RESULT_TRUE != result)
    {
        return MMI_RESULT_FALSE;
    }

    //1 ɾ������
    prev_item_ptr = item_ptr->prev_ptr;
    next_item_ptr = item_ptr->next_ptr;

    if (item_ptr == richtext_ptr->img_appending_item_ptr)
    {
        richtext_ptr->img_appending_item_ptr = PNULL;
    }

    if (item_ptr == richtext_ptr->focus_item_ptr)
    {
        richtext_ptr->focus_item_ptr = PNULL;
    }

    if (item_ptr == richtext_ptr->first_item_ptr)
    {
        //the first item
        SCI_ASSERT(PNULL != prev_item_ptr); /*assert verified*/
        SCI_ASSERT(PNULL != next_item_ptr); /*assert verified*/
        SCI_ASSERT(PNULL == prev_item_ptr->next_ptr); /*assert verified*/
        next_item_ptr->prev_ptr = prev_item_ptr;
        richtext_ptr->first_item_ptr = next_item_ptr;

        //ɾ����
        if (PNULL == richtext_ptr->first_item_ptr)
        {
            //�������ÿؼ�����
            ResetState(richtext_ptr);
        }
    }
    else if (item_ptr == richtext_ptr->first_item_ptr->prev_ptr)
    {
        //the last item
        SCI_ASSERT(PNULL != prev_item_ptr); /*assert verified*/
        SCI_ASSERT(PNULL == next_item_ptr); /*assert verified*/
        prev_item_ptr->next_ptr = PNULL; //next_item_ptr;
        richtext_ptr->first_item_ptr->prev_ptr = prev_item_ptr;
    }
    else
    {
        SCI_ASSERT(PNULL != prev_item_ptr); /*assert verified*/
        SCI_ASSERT(PNULL != next_item_ptr); /*assert verified*/
        prev_item_ptr->next_ptr = next_item_ptr;
        next_item_ptr->prev_ptr = prev_item_ptr;
    }

    //2 ɾ����̬����
    FreeItemBuffer(item_ptr);
    DestroyItemControl(item_ptr);


    //3 �ͷ��ڴ�
    SCI_FREE(item_ptr);

    //���¼���virtual rect
    richtext_ptr->need_cal_rect = TRUE;

    ClearCtrlInfo(richtext_ptr);

    return MMI_RESULT_TRUE;
}


/*==============================================================================
Description: GUIRICHTEXT_ModifyItem
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_ModifyItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16 item_index,
    GUIRICHTEXT_ITEM_T const *item_ptr
)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    CTRLRICHTEXT_OBJ_T *richtext_ptr = PNULL;
    RICHTEXT_ITEM_T *ext_item_ptr = PNULL;
    RICHTEXT_ITEM_T bak_item = {0};

    if (PNULL == item_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    //controler
    richtext_ptr = GetRichTextPtr(ctrl_id);

    if (PNULL == richtext_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    result = GetItemByIndex(richtext_ptr, item_index, &ext_item_ptr);

    if (MMI_RESULT_TRUE != result)
    {
        return MMI_RESULT_FALSE;
    }

    //��������
    bak_item = *ext_item_ptr;

    //��ʼ��
    InitItem(richtext_ptr, item_ptr, ext_item_ptr);

    //ɾ����̬����
    //ɾ����̬���ݱ�����InitItem()֮����Ϊitem_ptr�п����д�bak_item��ȡ��������
    FreeItemBuffer(&bak_item);
    DestroyItemControl(&bak_item);

    //�ָ�����
    ext_item_ptr->index = bak_item.index;
    ext_item_ptr->prev_ptr = bak_item.prev_ptr;
    ext_item_ptr->next_ptr = bak_item.next_ptr;

    //���������Ԫ��
    if (GUIRICHTEXT_FOCUS_SHIFT_NONE != richtext_ptr->focus_mode)
    {
        SetItemFocusElememt(richtext_ptr, ext_item_ptr);
    }

    if (!richtext_ptr->need_cal_ext_data)
    {
        CalcItemExtData(richtext_ptr, ext_item_ptr);
    }

    //���¼���virtual rect
    richtext_ptr->need_cal_rect = TRUE;

    ClearCtrlInfo(richtext_ptr);

    return MMI_RESULT_TRUE;
}


/*==============================================================================
Description: GUIRICHTEXT_DeleteAllItems
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_DeleteAllItems(
    MMI_CTRL_ID_T ctrl_id
)
{
    CTRLRICHTEXT_OBJ_T *richtext_ptr = PNULL;

    richtext_ptr = GetRichTextPtr(ctrl_id);

    if (PNULL == richtext_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    //delete items
    FreeAllItems(richtext_ptr);

    //reset param
    if (PNULL != richtext_ptr->prgbox_ptr)
    {
        MMK_DestroyControl(GUICTRL_GetCtrlHandle((IGUICTRL_T *)richtext_ptr->prgbox_ptr));
        richtext_ptr->prgbox_ptr = PNULL;
    }

    //�������ÿؼ�����
    ResetState(richtext_ptr);

#ifdef GUIF_CLIPBOARD
    ClearClipBoardInfo(richtext_ptr);
#endif

    return MMI_RESULT_TRUE;
}


/*==============================================================================
Description: GUIRICHTEXT_Update
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_Update(
    MMI_CTRL_ID_T ctrl_id
)
{
    CTRLRICHTEXT_OBJ_T *richtext_ptr = PNULL;

    richtext_ptr = GetRichTextPtr(ctrl_id);

    if (PNULL == richtext_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    DrawRichText(richtext_ptr);

    return MMI_RESULT_TRUE;
}


/*==============================================================================
Description: GUIRICHTEXT_Update
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_UpdateData(
    MMI_CTRL_ID_T ctrl_id
)
{
    CTRLRICHTEXT_OBJ_T *richtext_ptr = PNULL;

    richtext_ptr = GetRichTextPtr(ctrl_id);

    if (PNULL == richtext_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    UpdateData(richtext_ptr);

    return MMI_RESULT_TRUE;
}


/*==============================================================================
Description: ��ȡָ��index��item
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: ��item�����ݲ���ԭʼ���ݣ����ܻ���Ϊ��ʾ��Ҫ���޸Ĺ���
      *hyper parse��bug(Ҳ����bug�����Ǻ���ģ�������ʾ���ַ�����ʽ����)
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_GetItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16 item_index,
    GUIRICHTEXT_ITEM_T *item_ptr
)
{
    MMI_RESULT_E result = MMI_RESULT_FALSE;
    CTRLRICHTEXT_OBJ_T *richtext_ptr = PNULL;
    RICHTEXT_ITEM_T *ext_item_ptr = PNULL;

    if (PNULL == item_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    //controler
    richtext_ptr = GetRichTextPtr(ctrl_id);

    if (PNULL == richtext_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    result = GetItemByIndex(richtext_ptr, item_index, &ext_item_ptr);

    if (MMI_RESULT_TRUE != result)
    {
        return MMI_RESULT_FALSE;
    }

    *item_ptr = ext_item_ptr->data.orig_data;

    return MMI_RESULT_TRUE;
}


/*==============================================================================
Description: ��ȡ����Ԫ��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_GetFocusElement(
    MMI_CTRL_ID_T ctrl_id,
    GUIRICHTEXT_FOCUS_ELEM_T *element_ptr
)
{
    MMI_RESULT_E result = MMI_RESULT_TRUE;
    CTRLRICHTEXT_OBJ_T *richtext_ptr = PNULL;
    RICHTEXT_FOCUS_ELEM_T *elem_ptr = PNULL;
    RICHTEXT_ITEM_T *item_ptr = PNULL;
    BOOLEAN is_text_tags = FALSE;
    BOOLEAN is_not_null_text = FALSE;
    int32 i = 0;
    int32 new_len = 0;

    if (PNULL == element_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    richtext_ptr = GetRichTextPtr(ctrl_id);

    if (PNULL == richtext_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    item_ptr = richtext_ptr->focus_item_ptr;

    if (PNULL == item_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (item_ptr->focus_count == 0)
    {
        return MMI_RESULT_FALSE;
    }

    if (item_ptr->focus_index >= item_ptr->focus_count)
    {
        return MMI_RESULT_FALSE;
    }

    SCI_ASSERT(0 == item_ptr->focus_count || PNULL != item_ptr->focus_elem); /*assert verified*/

    elem_ptr = &(item_ptr->focus_elem[item_ptr->focus_index]);

    element_ptr->type = (GUIRICHTEXT_FOCUS_TYPE_T)elem_ptr->tag_type;

    is_text_tags = (!(elem_ptr->tag_type & GUIRICHTEXT_TAG_IMG_MASK) /*lint !e655*/
                    && (elem_ptr->tag_type & GUIRICHTEXT_TAG_TXT_MASK)); /*lint !e655*/
    is_not_null_text = (GUIRICHTEXT_TEXT_NONE != item_ptr->data.orig_data.text_type);

    if (is_text_tags && is_not_null_text)
    {
        element_ptr->data.str_ptr = item_ptr->data.orig_data.text_data.buf.str_ptr
                                    + elem_ptr->abs_begin;
        element_ptr->data.len = elem_ptr->abs_end - elem_ptr->abs_begin + 1;

        //�ر���绰���룬ȥ�����ӷ�"-"
        //������ǿ��ָ���ĵ绰����
        if ((GUIRICHTEXT_TAG_PHONENUM == element_ptr->type)
                && (GUIRICHTEXT_TAG_PHONENUM != item_ptr->data.orig_data.tag_type))
        {
            for (i = 0; i < element_ptr->data.len; i++)
            {
                if (0 == element_ptr->data.str_ptr[i]) break;

                if ('-' == element_ptr->data.str_ptr[i]) continue;

                richtext_ptr->edit_phonenumber[new_len] = element_ptr->data.str_ptr[i];
                new_len++;
            }

            SCI_ASSERT(new_len <= GUIURL_PHONE_NUMBER_LEN_MAX); /*assert verified*/

            richtext_ptr->edit_phonenumber[new_len] = '\0';
            element_ptr->data.str_ptr = richtext_ptr->edit_phonenumber;
            element_ptr->data.len = new_len;
        }
    }
    else
    {
        element_ptr->data.str_ptr = PNULL;
        element_ptr->data.len = 0;
    }


    return result;
}


/*==============================================================================
Description: ��ȡ����Item��������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_GetFocusItemIndex(
    MMI_CTRL_ID_T ctrl_id,
    uint16 *index_ptr
)
{
    CTRLRICHTEXT_OBJ_T *richtext_ptr = PNULL;

    if (PNULL == index_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    richtext_ptr = GetRichTextPtr(ctrl_id);

    if (PNULL == richtext_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    if (PNULL == richtext_ptr->focus_item_ptr)
    {
        return MMI_RESULT_FALSE;
    }

    *index_ptr = richtext_ptr->focus_item_ptr->index;

    return MMI_RESULT_TRUE;
}


//���ò����ĺ����� RICHTEXT_SETTINGS_E �г�����Ŀ
/*==============================================================================
Description: ���������ؼ���Ĭ������
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetFont(
    MMI_CTRL_ID_T ctrl_id,
    GUI_FONT_T font
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_FONT, &font);
}

/*==============================================================================
Description: ���������ؼ���Ĭ��������ɫ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetFontColor(
    MMI_CTRL_ID_T ctrl_id,
    GUI_COLOR_T color
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_FONT_COLOR, &color);
}

/*==============================================================================
Description: ������ʾ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �б�Ҫ��ô? ��ʼ���������ø�...
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetRect(
    MMI_CTRL_ID_T ctrl_id,
    GUI_RECT_T  rect
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_RECT, &rect);
}


/*==============================================================================
Description: �����ּ��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetCharSpace(
    MMI_CTRL_ID_T ctrl_id,
    uint8 space
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_SPACE_CHAR, &space);

}


/*==============================================================================
Description: �����ı����м��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetLineSpace(
    MMI_CTRL_ID_T ctrl_id,
    uint8 space
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_SPACE_LINE, &space);
}


/*==============================================================================
Description: ����ͼƬ���ı��ľ���
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetImgTxtSpace(
    MMI_CTRL_ID_T ctrl_id,
    uint8 space
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_SPACE_IMG_TXT, &space);
}


/*==============================================================================
Description: ����item֮��ļ��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetItemSpace(
    MMI_CTRL_ID_T ctrl_id,
    uint8 space
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_SPACE_ITEM, &space);
}


/*==============================================================================
Description: ���ñ߽�࣬�ؼ����ܵ�����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetBorderSpace(
    MMI_CTRL_ID_T ctrl_id,
    uint8 space
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_SPACE_BORDER, &space);
}


/*==============================================================================
Description: ���ñ߽�࣬item���ܵ�����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetItemBorderSpace(
    MMI_CTRL_ID_T ctrl_id,
    uint8 space
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_SPACE_ITEM_BORDER, &space);
}


/*==============================================================================
Description: �������ֵ��߿�Ŀհ�
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetTxtMargin(
    MMI_CTRL_ID_T ctrl_id,
    uint8 margin
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_TXT_MARGIN, &margin);
}


/*==============================================================================
Description: �����л�����ķ�ʽ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetFocusMode(
    MMI_CTRL_ID_T ctrl_id,
    GUIRICHTEXT_FOCUS_SHIFT_E focus_mode
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_FOCUS_MODE, &focus_mode);
}


/*==============================================================================
Description: ���ý����ַ����»�����ɫ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetFocusLineColor(
    MMI_CTRL_ID_T ctrl_id,
    GUI_COLOR_T color
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_FOCUS_LINE_COLOR, &color);
}


/*==============================================================================
Description: ���ý�������ѡ��ʱ�ı���ɫ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetFocusBgColor(
    MMI_CTRL_ID_T ctrl_id,
    GUI_COLOR_T color
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_FOCUS_BG_COLOR, &color);
}


/*==============================================================================
Description: ���ÿؼ��ı���ɫ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetBgColor(
    MMI_CTRL_ID_T ctrl_id,
    GUI_COLOR_T color
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_BG_COLOR, &color);
}


/*==============================================================================
Description: GUIRICHTEXT_SetBgImage
Global resource ���ÿؼ��ı���ͼƬ: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetBgImage(
    MMI_CTRL_ID_T ctrl_id,
    MMI_IMAGE_ID_T bg_img_id
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_BG_IMG, &bg_img_id);
}


/*==============================================================================
Description: ���ñ߿�,Ĭ��ΪGUIRICHTEXT_FRAME_NONE
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetBorder(
    MMI_CTRL_ID_T ctrl_id,
    GUIRICHTEXT_FRAME_TYPE_E border_type
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_FRAME_TYPE, &border_type);
}


/*==============================================================================
Description: ���ñ߿����ɫ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetBorderColor(
    MMI_CTRL_ID_T ctrl_id,
    GUI_COLOR_T color
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_FRAME_COLOR, &color);
}


/*==============================================================================
Description: ���ñ߿����ɫ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetBorderFocusColor(
    MMI_CTRL_ID_T ctrl_id,
    GUI_COLOR_T color
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_FRAME_FOCUS_COLOR, &color);
}


/*==============================================================================
Description: ������ͼƬ������������ʾ��ͼƬ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetAnimParsingIcon(
    MMI_CTRL_ID_T ctrl_id,
    MMI_IMAGE_ID_T img_id
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_ANIM_PARSING_ICON, &img_id);
}


/*==============================================================================
Description: ������ͼƬ��������ʱ��ʾ��ͼƬ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetAnimErrorIcon(
    MMI_CTRL_ID_T ctrl_id,
    MMI_IMAGE_ID_T img_id
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_ANIM_ERROR_ICON, &img_id);
}


/*==============================================================================
Description: ָ������item
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetFocusItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16 item_index
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_FOCUS_ITEM, &item_index);
}


/*==============================================================================
Description: �趨ͼƬ�����Ƿ�ͬ��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC MMI_RESULT_E CTRLRICHTEXT_SetImgDecodeSyn(
    MMI_CTRL_ID_T ctrl_id,
    BOOLEAN is_img_syn_decode
)
{
    return SetDataById(ctrl_id, RICHTEXT_SETTINGS_IMG_SYN_DECODE, &is_img_syn_decode);
}

/*==============================================================================
Description: ���³�ʼ���ؼ�����ʾ����
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �������������ÿؼ���Ŀ����Ҫ����״̬�����������ʾ����
==============================================================================*/
PUBLIC void CTRLRICHTEXT_ResetState(
    MMI_CTRL_ID_T ctrl_id
)
{
    CTRLRICHTEXT_OBJ_T *richtext_ptr = GetRichTextPtr(ctrl_id);

    if (PNULL == richtext_ptr)
    {
        return;
    }

    ClearCtrlInfo(richtext_ptr);

    return;
}


/*==============================================================================
Description: ������ʾ������Ԫ������ҳ��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC void CTRLRICHTEXT_ScrollToFocus(
    MMI_CTRL_ID_T ctrl_id
)
{
    CTRLRICHTEXT_OBJ_T *richtext_ptr = GetRichTextPtr(ctrl_id);

    if (PNULL == richtext_ptr)
    {
        return;
    }

    ScrollToFocus(richtext_ptr);

    return;
}




/*==============================================================================
Description: ������ʾָ��item����ҳ��
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
�¼����item������update֮�������ȷ�Ĺ�����ʾ����Ϊ��update��ʱ�����rect
update�����ĵ�һ��ˢ�»��Զ�����Ҳ���Ե��ú���GUIRICHTEXT_Update�����
==============================================================================*/
PUBLIC BOOLEAN CTRLRICHTEXT_ScrollToItem(
    MMI_CTRL_ID_T ctrl_id,
    uint16 item_index
)
{
    CTRLRICHTEXT_OBJ_T *richtext_ptr = GetRichTextPtr(ctrl_id);
    RICHTEXT_ITEM_T *item_ptr = PNULL;
    MMI_RESULT_E retcode = MMI_RESULT_FALSE;

    if (PNULL == richtext_ptr)
    {
        return FALSE;
    }

    retcode = GetItemByIndex(richtext_ptr, item_index, &item_ptr);

    if (MMI_RESULT_TRUE != retcode)
    {
        return FALSE;
    }

    retcode = ScrollToLine(richtext_ptr, item_ptr->virtual_rect.top);

    return (MMI_RESULT_TRUE == retcode);
}


/*==============================================================================
Description: GUIRICHTEXT_EnableClipBoard
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN CTRLRICHTEXT_EnableClipBoard(MMI_CTRL_ID_T ctrl_id)
{
    BOOLEAN ret = FALSE;
    CTRLRICHTEXT_OBJ_T *richtext_ptr = GetRichTextPtr(ctrl_id);

    if (PNULL == richtext_ptr)
    {
        return FALSE;
    }

#ifdef GUIF_CLIPBOARD
    richtext_ptr->is_clipboard_support = TRUE;
    //��ռ��а�״̬�����ֳ�ʼ���Ĵ���״̬
    ClearClipBoardInfo(richtext_ptr);
    
    ret = TRUE;
#endif

    return ret;
}


/*==============================================================================
Description: GUIRICHTEXT_DisableClipBoard
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN CTRLRICHTEXT_DisableClipBoard(MMI_CTRL_ID_T ctrl_id)
{
    BOOLEAN ret = FALSE;
    CTRLRICHTEXT_OBJ_T *richtext_ptr = GetRichTextPtr(ctrl_id);

    if (PNULL == richtext_ptr)
    {
        return FALSE;
    }

#ifdef GUIF_CLIPBOARD
    richtext_ptr->is_clipboard_support = FALSE;
    ret = TRUE;
#endif

    return ret;
}


/*==============================================================================
Description: �����ؼ����м���Ϣ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note: �������а���Ϣ��������Ϣ�ȣ���Ϊ������Ϣ��ϵ��item�����ԣ�������Ϣ�޷�����
==============================================================================*/
PUBLIC BOOLEAN CTRLRICHTEXT_SaveInfo(MMI_CTRL_ID_T ctrl_id)
{
    CTRLRICHTEXT_OBJ_T *richtext_ptr = GetRichTextPtr(ctrl_id);

    if (PNULL == richtext_ptr)
    {
        return FALSE;
    }

    richtext_ptr->is_info_protected = TRUE;

    return TRUE;
}


/*==============================================================================
Description: �ָ��ؼ����м���Ϣ
Global resource dependence: NONE
Author: Lianxiang.Zhou
Note:
==============================================================================*/
PUBLIC BOOLEAN CTRLRICHTEXT_RestoreInfo(MMI_CTRL_ID_T ctrl_id)
{
    CTRLRICHTEXT_OBJ_T *richtext_ptr = GetRichTextPtr(ctrl_id);

    if (PNULL == richtext_ptr)
    {
        return FALSE;
    }

    richtext_ptr->is_info_protected = FALSE;
    UpdateData(richtext_ptr);
#ifdef GUIF_CLIPBOARD
    GenSelectData(richtext_ptr);
#endif
    return TRUE;
}

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
PUBLIC BOOLEAN CTRLRICHTEXT_GetHighlightPos(
    MMI_CTRL_ID_T    ctrl_id,
    GUI_RECT_T       *min_highlight_rect_ptr,
    GUI_RECT_T       *max_highlight_rect_ptr
)
{
#ifdef GUIF_CLIPBOARD
    BOOLEAN     result = FALSE;
    CTRLRICHTEXT_OBJ_T *richtext_ptr = GetRichTextPtr(ctrl_id);

    if (PNULL == richtext_ptr)
    {
        return FALSE;
    }

    if (PNULL != min_highlight_rect_ptr)
    {
        result = GetHighlightRect(richtext_ptr, TRUE, min_highlight_rect_ptr);
    }

    if (PNULL != max_highlight_rect_ptr)
    {
        result = GetHighlightRect(richtext_ptr, FALSE, max_highlight_rect_ptr);
    }

    return result;

#else
    return FALSE;
#endif
}

