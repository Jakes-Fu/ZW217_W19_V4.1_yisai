#ifndef _GUIRICHTEXT_H_
#define _GUIRICHTEXT_H_

/*---------------------------------------------------------------------------
                          Include Files                                    
 --------------------------------------------------------------------------*/
#include "guictrl.h"
#include "guifont.h"
#include "ctrlrichtext_export.h"

/*------------------------------------------------------------------------------
    Compiler Flag
------------------------------------------------------------------------------*/
#ifdef __cplusplus
extern   "C"
{
#endif

/*------------------------------------------------------------------------------
    FUNCTION DEFINITION
------------------------------------------------------------------------------*/

/***************************************************************************//*!
@brief GUIRICHTEXT_AddItem
@author Lianxiang.Zhou
@param ctrl_id [in] �ؼ�id
@param item_ptr [in] item����
@param index_ptr [in/out]���ص�ǰitem��index���ɸ��ݸ�id����item
@return �Ƿ����ӳɹ� 
@note ������һ��ͼƬitem, ����ͼƬ��buf����ʱ, GUIRICHTEXT_IMAGE_T�ṹ�е�buf_ptr���ɴ��������ڴ��, �ؼ�������������ͷ�
@note �������͵����ã���ο�GUIRICHTEXT_TAG_TYPE_E���崦����ϸ˵��
@sa GUIRICHTEXT_DeleteItem
@sa GUIRICHTEXT_ModifyItem
@sa GUIRICHTEXT_DeleteAllItems
*******************************************************************************/
#define GUIRICHTEXT_AddItem CTRLRICHTEXT_AddItem

/***************************************************************************//*!
@brief GUIRICHTEXT_DeleteItem
@author Lianxiang.Zhou
@param ctrl_id [in] �ؼ�id
@param item_index [in]item��index����add item��ʱ����Եõ�
@return �����Ƿ�ɹ�
@note delete֮�������Ҫˢ����ʾ�������GUIRICHTEXT_Update
@sa GUIRICHTEXT_AddItem
@sa GUIRICHTEXT_ModifyItem
@sa GUIRICHTEXT_DeleteAllItems
*******************************************************************************/
#define GUIRICHTEXT_DeleteItem CTRLRICHTEXT_DeleteItem

/***************************************************************************//*!
@brief GUIRICHTEXT_ModifyItem
@author Lianxiang.Zhou
@param ctrl_id [in] �ؼ�id
@param item_index [in] item��index����add item��ʱ����Եõ�
@param item_ptr [in] item����
@return �����Ƿ�ɹ�
@note modify֮�������Ҫˢ����ʾ�������GUIRICHTEXT_Update
@sa GUIRICHTEXT_AddItem
@sa GUIRICHTEXT_DeleteItem
@sa GUIRICHTEXT_DeleteAllItems
*******************************************************************************/
#define GUIRICHTEXT_ModifyItem CTRLRICHTEXT_ModifyItem

/***************************************************************************//*!
@brief ɾ�����е�item
@author Lianxiang.Zhou
@param ctrl_id [in] �ؼ�id
@return �����Ƿ�ɹ�
@sa GUIRICHTEXT_AddItem
@sa GUIRICHTEXT_DeleteItem
@sa GUIRICHTEXT_ModifyItem
*******************************************************************************/
#define GUIRICHTEXT_DeleteAllItems CTRLRICHTEXT_DeleteAllItems

/***************************************************************************//*!
@brief ��ʾ�ؼ�
@author Lianxiang.Zhou
@param ctrl_id [in] �ؼ�id
@return �����Ƿ�ɹ�
@note ��ˢ��֮ǰ������¿ؼ�������
@sa GUIRICHTEXT_UpdateData
*******************************************************************************/
#define GUIRICHTEXT_Update CTRLRICHTEXT_Update

/***************************************************************************//*!
@brief �����¿ؼ������ݣ�����ˢ��
@author Lianxiang.Zhou
@param ctrl_id [in] �ؼ�id
@return �����Ƿ�ɹ�
@note �кܶ�������ͨ������õ��ģ�������ֻ������Ҫ��ʱ��Ž��У�ͨ��������ʾ��ʱ
�������Ҫ��ʱ����ؼ������ݣ�����Ҫͨ��������������ɡ�
@sa GUIRICHTEXT_Update
*******************************************************************************/
#define GUIRICHTEXT_UpdateData CTRLRICHTEXT_UpdateData

/***************************************************************************//*!
@brief ��ȡָ��index��item
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param item_index [in]item��index����add item��ʱ����Եõ�
@param item_ptr [in]item����ָ��
@return �����Ƿ�ɹ�
@note ��item�����ݲ���ԭʼ���ݣ����ܻ���Ϊ��ʾ��Ҫ���޸Ĺ�������hyper parse����
��ʾ���ַ�����ʽ���ˣ�ͬ��text id��ʽ������Ҳ���������buffer��ʽ��
@sa GUIRICHTEXT_AddItem
*******************************************************************************/
#define GUIRICHTEXT_GetItem CTRLRICHTEXT_GetItem

/***************************************************************************//*!
@brief ��ȡ����Ԫ��
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param element [in]�������ݵ�ָ��
@return �����Ƿ�ɹ�
@note 
- ����ǰ��������Ԫ�ص���Ϣ
- �������ͣ���ο�GUIRICHTEXT_TAG_TYPE_E���崦����ϸ˵��
*******************************************************************************/
#define GUIRICHTEXT_GetFocusElement CTRLRICHTEXT_GetFocusElement

/***************************************************************************//*!
@brief ��ȡ����Item��������
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param index_ptr [in]������ָ��
@return �����Ƿ�ɹ�
*******************************************************************************/
#define GUIRICHTEXT_GetFocusItemIndex CTRLRICHTEXT_GetFocusItemIndex

/***************************************************************************//*!
@brief ���������ؼ���Ĭ������
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param font [in]����
@return �����Ƿ�ɹ�
*******************************************************************************/
#define GUIRICHTEXT_SetFont CTRLRICHTEXT_SetFont

/***************************************************************************//*!
@brief ���������ؼ���Ĭ��������ɫ
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param color [in]��ɫ
@return �����Ƿ�ɹ�
*******************************************************************************/
#define GUIRICHTEXT_SetFontColor CTRLRICHTEXT_SetFontColor

/***************************************************************************//*!
@brief ������ʾ����
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param rect [in]��ʾ����
@return �����Ƿ�ɹ�
*******************************************************************************/
#define GUIRICHTEXT_SetRect CTRLRICHTEXT_SetRect

/***************************************************************************//*!
@brief �����ּ��
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param space [in]�ּ��
@return �����Ƿ�ɹ�
*******************************************************************************/
#define GUIRICHTEXT_SetCharSpace CTRLRICHTEXT_SetCharSpace

/***************************************************************************//*!
@brief �����ı����м��
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param space [in]�м��
@return �����Ƿ�ɹ�
*******************************************************************************/
#define GUIRICHTEXT_SetLineSpace CTRLRICHTEXT_SetLineSpace

/***************************************************************************//*!
@brief ����ͼƬ���ı��ľ���
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param space [in]�ı���ͼƬ֮��ľ���
@return �����Ƿ�ɹ�
*******************************************************************************/
#define GUIRICHTEXT_SetImgTxtSpace CTRLRICHTEXT_SetImgTxtSpace

/***************************************************************************//*!
@brief ����item֮��ļ��
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param space [in]item֮��ļ��
@return �����Ƿ�ɹ�
*******************************************************************************/
#define GUIRICHTEXT_SetItemSpace CTRLRICHTEXT_SetItemSpace

/***************************************************************************//*!
@brief ���ñ߽�࣬��item�Ϳؼ���Ե�ľ���
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param space [in]item�Ϳؼ���Ե�ľ���
@return �����Ƿ�ɹ�
*******************************************************************************/
#define GUIRICHTEXT_SetBorderSpace CTRLRICHTEXT_SetBorderSpace

/***************************************************************************//*!
@brief ���ñ߽�࣬item���ܵ�����
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param space [in]item���ܵ�����
@return �����Ƿ�ɹ�
*******************************************************************************/
#define GUIRICHTEXT_SetItemBorderSpace CTRLRICHTEXT_SetItemBorderSpace

/***************************************************************************//*!
@brief �������ֵ��߿�Ŀհ�
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param margin [in]�������ֵ��߿�Ŀհ�
@return �����Ƿ�ɹ�
*******************************************************************************/
#define GUIRICHTEXT_SetTxtMargin CTRLRICHTEXT_SetTxtMargin

/***************************************************************************//*!
@brief �����л�����ķ�ʽ
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param focus_mode [in]����ģʽ
@return �����Ƿ�ɹ�
@sa _GUIRICHTEXT_FOCUS_SHIFT_E
@sa GUIRICHTEXT_TAG_TYPE_E
*******************************************************************************/
#define GUIRICHTEXT_SetFocusMode CTRLRICHTEXT_SetFocusMode

/***************************************************************************//*!
@brief ���ý����ַ����»�����ɫ
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param color [in] ��ɫ
@return �����Ƿ�ɹ�
*******************************************************************************/
#define GUIRICHTEXT_SetFocusLineColor CTRLRICHTEXT_SetFocusLineColor

/***************************************************************************//*!
@brief ���ý�������ѡ��ʱ�ı���ɫ
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param color [in]��ɫ
@return �����Ƿ�ɹ�
*******************************************************************************/
#define GUIRICHTEXT_SetFocusBgColor CTRLRICHTEXT_SetFocusBgColor

/***************************************************************************//*!
@brief ���ÿؼ��ı���ɫ
@author Lianxiang.Zhou
@param ctrl_id [in] �ؼ�id
@param color [in] ��ɫ
@return �����Ƿ�ɹ�
@note ��û�б���ͼƬ���߱���ͼƬʧЧ��ʱ�򣬱���ɫ������
*******************************************************************************/
#define GUIRICHTEXT_SetBgColor CTRLRICHTEXT_SetBgColor

/***************************************************************************//*!
@brief ���ÿؼ��ı���ͼƬ
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param bg_img_id [in]����ͼƬid
@return �����Ƿ�ɹ�
*******************************************************************************/
#define GUIRICHTEXT_SetBgImage CTRLRICHTEXT_SetBgImage

/***************************************************************************//*!
@brief ���ñ߿�,Ĭ��ΪGUIRICHTEXT_FRAME_NONE
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param border_type [in]�߿�����
@return �����Ƿ�ɹ�
*******************************************************************************/
#define GUIRICHTEXT_SetBorder CTRLRICHTEXT_SetBorder

/***************************************************************************//*!
@brief ���ñ߿����ɫ
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param color [in]��ɫ
@return �����Ƿ�ɹ�
*******************************************************************************/
#define GUIRICHTEXT_SetBorderColor CTRLRICHTEXT_SetBorderColor

/***************************************************************************//*!
@brief ���ý���Ԫ�صı߿���ɫ
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param color [in]��ɫ
@return �����Ƿ�ɹ�
@note ����item����ʾ�߿��ʱ�򣬻��в�ͬ����ɫ
*******************************************************************************/
#define GUIRICHTEXT_SetBorderFocusColor CTRLRICHTEXT_SetBorderFocusColor

/***************************************************************************//*!
@brief ������ͼƬ������������ʾ��ͼƬ
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param img_id [in]ͼƬ��id
@return �����Ƿ�ɹ�
@note ��ЩͼƬ����ʾ��Ҫ�ȴ����ڵȴ������У�����ʾһ���ȴ�ͼƬ
*******************************************************************************/
#define GUIRICHTEXT_SetAnimParsingIcon CTRLRICHTEXT_SetAnimParsingIcon

/***************************************************************************//*!
@brief ������ͼƬ��������ʱ��ʾ��ͼƬ
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param img_id [in]ͼƬ��id
@return �����Ƿ�ɹ�
@note ��ͼƬ������ʾ��ʱ����ʾָ��ͼƬ
*******************************************************************************/
#define GUIRICHTEXT_SetAnimErrorIcon CTRLRICHTEXT_SetAnimErrorIcon

/***************************************************************************//*!
@brief ָ������item
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param item_index [in]item��index����add item��ʱ����Եõ�
@return �����Ƿ�ɹ�
*******************************************************************************/
#define GUIRICHTEXT_SetFocusItem CTRLRICHTEXT_SetFocusItem

/***************************************************************************//*!
@brief �趨ͼƬ�����Ƿ�ͬ��
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param is_img_syn_decode [in]�Ƿ�ͬ������
@return �����Ƿ�ɹ�
@note ��һЩ����ʾ�ٶ���Ҫ���Ӧ�ã�������첽���롣
*******************************************************************************/
#define GUIRICHTEXT_SetImgDecodeSyn CTRLRICHTEXT_SetImgDecodeSyn

/***************************************************************************//*!
@brief ���³�ʼ���ؼ�����ʾ����
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@note �������������ÿؼ���Ŀ����Ҫ����״̬�����
*******************************************************************************/
#define GUIRICHTEXT_ResetState CTRLRICHTEXT_ResetState

/***************************************************************************//*!
@brief ������ʾ������Ԫ������ҳ��
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
*******************************************************************************/
#define GUIRICHTEXT_ScrollToFocus CTRLRICHTEXT_ScrollToFocus

/***************************************************************************//*!
@brief ������ʾָ��item����ҳ��
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@param item_index [in]item��index����add item��ʱ����Եõ�
@return �Ƿ�����ɹ�
@note �¼����item������update֮�������ȷ�Ĺ�����ʾ����Ϊ��update��ʱ�����rect
update�����ĵ�һ��ˢ�»��Զ�����Ҳ���Ե��ú���GUIRICHTEXT_Update�����
*******************************************************************************/
#define GUIRICHTEXT_ScrollToItem CTRLRICHTEXT_ScrollToItem

/***************************************************************************//*!
@brief ���ü����幦��
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@return �Ƿ����óɹ�
@sa GUIRICHTEXT_DisableClipBoard
@note Ĭ�ϼ��а��ǿ�����
-# ѡ�֣�ѡ�������漸�ַ�ʽ��
    -# ͨ�������϶�ѡ��
    -# �������ʣ������˵�ȫѡ
-# ���ƣ��ɸ��Ƶ����������漸�֣�
    -# ѡ��������
    -# ��������
*******************************************************************************/
#define GUIRICHTEXT_EnableClipBoard CTRLRICHTEXT_EnableClipBoard

/***************************************************************************//*!
@brief ���ü����幦��
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@return �Ƿ����óɹ�
@sa GUIRICHTEXT_EnableClipBoard
*******************************************************************************/
#define GUIRICHTEXT_DisableClipBoard CTRLRICHTEXT_DisableClipBoard

/***************************************************************************//*!
@brief ����ؼ�����ʾ�м���Ϣ���������а塢����λ�õ�
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@return �Ƿ񱣴�ɹ�
@sa GUIRICHTEXT_RestoreInfo
@warning һ��Ҫ����GUIRICHTEXT_RestoreInfo���ָ�������������ʾ����
@warning ������Ϣ���޷����棬��Ϊ������ϢҪ������(item)��Ϣ����item����Ϣ��ȫ��
��ʧ��item����Ϣ�ֱȽϸ��ӣ��������ܱ�֤�ڲ���֮��ԭ����item�����ڣ��ʶ����޷�
���档
@details һЩӦ�û���Ϊ��Ӧ�õ���Ҫ���Կؼ����дݻ��ؽ��������ؽ��Ĺ����У���
����һЩ��Ϣ�Ķ�ʧ��������а���Ϣ������λ����Ϣ�ȡ���ʱ�����ǾͿ��Բ��ö�����
��һЩ��Ϣ���б�����
*******************************************************************************/
#define GUIRICHTEXT_SaveInfo CTRLRICHTEXT_SaveInfo

/***************************************************************************//*!
@brief �ָ��ؼ�����ʾ�м���Ϣ���������а塢����λ�õ�
@author Lianxiang.Zhou
@param ctrl_id [in]�ؼ�id
@return �Ƿ�ָ��ɹ�
@sa GUIRICHTEXT_SaveInfo
*******************************************************************************/
#define GUIRICHTEXT_RestoreInfo CTRLRICHTEXT_RestoreInfo

/*****************************************************************************/
//  Description : get cursor rect,include multi line
//  Global resource dependence : 
//  Author: hua.fang
//  Note:
/*****************************************************************************/
#define GUIRICHTEXT_GetHighlightPos CTRLRICHTEXT_GetHighlightPos

/*------------------------------------------------------------------------------
    Compiler Flag
------------------------------------------------------------------------------*/
#ifdef   __cplusplus
}
#endif

#endif //_GUIRICHTEXT_H_

/*@}*/

