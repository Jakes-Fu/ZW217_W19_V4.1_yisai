/*==============================================================================
File Name: mmiim_touch_ime_sogou.h
Author: Lianxiang.Zhou
Date: 2011/12/14
Copyright: 2011 Spreadtrum, Incorporated. All Rights Reserved.
Description:
--------------------------------------------------------------------------------
Edit History
--------------------------------------------------------------------------------
DATE         NAME            DESCRIPTION
2011/12/14   Lianxiang.Zhou  Create.
==============================================================================*/


#ifndef _MMIIM_TOUCH_IME_SOGOU_H_
#define _MMIIM_TOUCH_IME_SOGOU_H_



/*------------------------------------------------------------------------------
Include files
------------------------------------------------------------------------------*/
#include "mmiim_touch_ime_common.h"

#if defined (MMI_IM_PDA_SUPPORT) && defined(IM_ENGINE_SOGOU)

#include "WD_def.h"
#include "iHCR.h"
#include "WD_HwDef.h"

#include "mmiim_sogou.h"



//========================sogou����Ķ���
#define IME_LIST_MAX        10

#define SOGOU_PINYIN_MATCH_INDEX_NONE 0xFFFF

//�������KERNEL_DATA_SOGOU_T��С��hw�Ƚṹ�岻ͬ�����쳣
#define CHECK_ENGINE_DATA(data_ptr) \
    do {\
        CHECK_DATA(data_ptr); \
        SCI_ASSERT(PNULL != ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))); /*assert verified*/ \
        SCI_ASSERT(PNULL != ((KERNEL_DATA_SOGOU_T*)(data_ptr->data))->pime_result); /*assert verified*/ \
    } while (0)

typedef enum _SOGOU_CAPLOCK_E
{
    E_CAPLOCK_LOWER,
    E_CAPLOCK_LEAD_UPPER,
    E_CAPLOCK_UPPER,
    E_CAPLOCK_MAX,
} SOGOU_CAPLOCK_E;

typedef enum _SOGOU_SELECT_E
{
    E_SELECT_NONE,
    E_SELECT_CAND,
    E_SELECT_LETTER,
    E_SELECT_MAX,
} SOGOU_SELECT_E;


// ��ѡ�ִ����ҳ��
#define MAX_SOGOU_CANDIDATE_PAGE_CHINESE MAX_PAGE_FOR_SOGOU
#define MAX_SOGOU_CANDIDATE_PAGE_ENGLISH 20

#define MAX_SOGOU_CANDIDATE_PAGE (20)       //��ѡ�ִ����ҳ��,����ǲ��Գ����ģ���ʱû�в��ҵ�����ĵ�



typedef struct KERNEL_DATA_SOGOU_TAG
{
    //���뷨�������
    WD_imeResult         *pime_result;
    WD_UINT8             lastdisp;

    //��������״̬����ʹ�õ�����
    uint8 choice_pinyin_page;     //9��ƴ������ʱ��choice��ҳ��
} KERNEL_DATA_SOGOU_T;


//sogou���뷨�������������

// ==============�ѹ����뷨ָ�����
//extern uint8 g_FlashData[FLASH_CAPACITY];


/* thumb en */
PUBLIC void ThumbLatinSwitchExpandKey(IM_DATA_T * data_ptr);
PUBLIC void ThumbLatinDispEdit(IM_DATA_T *data_ptr);
PUBLIC WD_imeResult* MMIIM_TOUCH_Sogou_GetImeResult(IM_DATA_T *data_ptr);

/* qwert en */
PUBLIC void QwertLatinClear(IM_DATA_T *data_ptr);
PUBLIC void QwertSmartSwitchExpandKey(IM_DATA_T * data_ptr);



#if defined (IM_SIMP_CHINESE_SUPPORT)
/* thumb pinyin */
PUBLIC BOOLEAN ThumbPinyinInit(IM_DATA_T *data_ptr, WD_UINT8 method);
PUBLIC BOOLEAN ThumbPinyinTerm(IM_DATA_T *data_ptr);
PUBLIC void ThumbPinyinDraw(IM_DATA_T * data_ptr);
PUBLIC void ThumbPinyinInitKb(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN ThumbPinyinCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbPinyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbPinyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbPinyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbPinyinCustUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbPinyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbPinyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC void ThumbPinyinClear(IM_DATA_T *data_ptr);
PUBLIC void ThumbPinyinUpdate(IM_DATA_T *data_ptr);
PUBLIC void ThumbPinyinSetCand(IM_DATA_T *data_ptr);
PUBLIC void ThumbPinyinDispEdit(IM_DATA_T *data_ptr);
PUBLIC void ThumbPinyinSetCust(IM_DATA_T *data_ptr);
#endif

#if defined (IM_SIMP_CHINESE_SUPPORT)
/* Qwert pinyin */
PUBLIC BOOLEAN QwertPinyinInit(IM_DATA_T *data_ptr, WD_UINT8 method);
PUBLIC BOOLEAN QwertPinyinTerm(IM_DATA_T *data_ptr);
PUBLIC void QwertPinyinDraw(IM_DATA_T * data_ptr);
PUBLIC void QwertPinyinInitKb(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN QwertPinyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertPinyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertPinyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertPinyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN QwertPinyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC void QwertPinyinClear(IM_DATA_T *data_ptr);
PUBLIC void QwertPinyinUpdate(IM_DATA_T *data_ptr);
PUBLIC void QwertPinyinSetCand(IM_DATA_T *data_ptr);
PUBLIC void QwertPinyinDispEdit(IM_DATA_T *data_ptr);
#endif

#if (defined (IM_SIMP_CHINESE_SUPPORT) || defined (IM_TRAD_CHINESE_SUPPORT))	//�������ĸ��ü������ĵıʻ����뷨
/* thumb stroke */
PUBLIC BOOLEAN ThumbStrokeInit(IM_DATA_T *data_ptr, WD_UINT8 method);
PUBLIC BOOLEAN ThumbStrokeTerm(IM_DATA_T *data_ptr);
PUBLIC void ThumbStrokeDraw(IM_DATA_T * data_ptr);
PUBLIC void ThumbStrokeInitKb(IM_DATA_T *data_ptr);
PUBLIC BOOLEAN ThumbStrokeCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbStrokeCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbStrokeCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbStrokeSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC BOOLEAN ThumbStrokeCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);
PUBLIC void ThumbStrokeClear(IM_DATA_T *data_ptr);
PUBLIC void ThumbStrokeUpdate(IM_DATA_T *data_ptr);
PUBLIC void ThumbStrokeSetCand(IM_DATA_T *data_ptr);
PUBLIC void ThumbStrokeDispEdit(IM_DATA_T *data_ptr);
#endif

#if defined (IM_TRAD_CHINESE_SUPPORT)
/*==============================================================================
Description: ThumbZhuyinInit
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinInit(IM_DATA_T *data_ptr, WD_UINT8 method);


/*==============================================================================
Description: ThumbZhuyinTerm
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinTerm(IM_DATA_T *data_ptr);


/*==============================================================================
Description: ThumbAbcDraw
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinDraw(IM_DATA_T * data_ptr);


/*==============================================================================
Description: ��ʼ������ΪĴָӢ����ĸ����
Global resource dependence: NONE
Author: yaoguang.chen
Note: �˴���ȫ��������д�룬����Ӧ��
==============================================================================*/
PUBLIC void ThumbZhuyinInitKb(IM_DATA_T *data_ptr);


/*==============================================================================
Description: Ĵָ����Zhuyin���볤�� - character��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - character��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: Ĵָ����Zhuyin���볤�� - control��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - custom��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCustUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: ThumbZhuyinSetLetterCand
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinSetLetterCand(IM_DATA_T *data_ptr);


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
���������ֺ�ѡ��ť
1 ��ѡ��
2 ��ѡƴ��
==============================================================================*/
PUBLIC BOOLEAN ThumbZhuyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: ThumbZhuyinClear
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinClear(IM_DATA_T *data_ptr);


/*==============================================================================
Description: ThumbZhuyinUpdate
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinUpdate(IM_DATA_T *data_ptr);


/*==============================================================================
Description: ThumbZhuyinSetCand
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinSetCand(IM_DATA_T *data_ptr);


/*==============================================================================
Description: ��ʾpre edit
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinDispEdit(IM_DATA_T *data_ptr);


/*==============================================================================
Description: ThumbZhuyinSetCust
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void ThumbZhuyinSetCust(IM_DATA_T *data_ptr);


/*==============================================================================
Description: QwertZhuyinInit
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinInit(IM_DATA_T *data_ptr, WD_UINT8 method);


/*==============================================================================
Description: QwertZhuyinTerm
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinTerm(IM_DATA_T *data_ptr);


/*==============================================================================
Description: QwertAbcDraw
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void QwertZhuyinDraw(IM_DATA_T * data_ptr);


/*==============================================================================
Description: ��ʼ������ΪĴָӢ����ĸ����
Global resource dependence: NONE
Author: yaoguang.chen
Note: �˴���ȫ��������д�룬����Ӧ��
==============================================================================*/
PUBLIC void QwertZhuyinInitKb(IM_DATA_T *data_ptr);


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - character��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: Ĵָ����Zhuyin���볤�� - control��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: Ĵָ����Zhuyin���밴�����𴥱� - control��
Global resource dependence: NONE
Author: yaoguang.chen
Note:
���������ֺ�ѡ��ť
1 ��ѡ��
2 ��ѡƴ��
==============================================================================*/
PUBLIC BOOLEAN QwertZhuyinCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*==============================================================================
Description: QwertZhuyinClear
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void QwertZhuyinClear(IM_DATA_T *data_ptr);


/*==============================================================================
Description: QwertZhuyinUpdate
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void QwertZhuyinUpdate(IM_DATA_T *data_ptr);


/*==============================================================================
Description: QwertZhuyinSetCand
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void QwertZhuyinSetCand(IM_DATA_T *data_ptr);


/*==============================================================================
Description: ��ʾpre edit
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC void QwertZhuyinDispEdit(IM_DATA_T *data_ptr);
#endif

/*----------------------------------------------------------------------------------------------------------------------
Name:   MultiCharKeyInit

Func:   ��ʼ��һ�����ַ�����

Input:
            key_ptr:    �����İ���
            text_ptr:   �����е��ַ�����

Output:

Return:
            err: < 0  succ: == 0

Thoughtway:

Create: yaoguangchen@spread. 2011-10-11

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 MultiCharKeyInit(KEY_INFO_T *key_ptr, const wchar *const text_ptr);



/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartInit

Func:   �������뷨��ʼ��

Input:
            data_ptr:    ���뷨ָ��

Output:

Return:
            TRUE:�ɹ���FALSE:�������Ϸ���ʧ��

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartInit(IM_DATA_T *data_ptr, WD_UINT8 sogou_method);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartTerm

Func:   9���������뷨�ر�

Input:
            data_ptr:    ���뷨ָ��

Output:

Return:
            TRUE:�ɹ���FALSE:�������Ϸ���ʧ��

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartTerm(IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartDraw

Func:   9���������뷨����

Input:
            data_ptr:    ���뷨ָ��

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartDraw(IM_DATA_T * data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartInitKb

Func:   9���������뷨���̳�ʼ��

Input:
            data_ptr:    ���뷨ָ��

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartInitKb(IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCharLong

Func:   9���������뷨�������ַ���0 ~ 9��Ϣ�������Ϊ�����׸��ַ�ʱ��������ֱ������

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�İ��������ȼ��ð����Ƿ�Ϊ�ַ�����������������

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCharLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);

/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCharUp

Func:   9���������뷨���ַ���0 ~ 9��Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�İ��������ȼ��ð����Ƿ�Ϊ�ַ�����������������

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);

/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCtrlLong

Func:   9���������뷨�����Ƽ�������Ϣ����,�˴���ʱֱ�ӵ���

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�Ŀ��ư���

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCtrlUp

Func:   9���������뷨�����Ƽ���Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�Ŀ��ư���

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCustUp

Func:   9���������뷨���û��Զ��尴����Ϣ�����˴�Ĭ�϶���Ϊ�������밴��

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�Ŀ��ư���

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCustUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartSysUp

Func:   9���������뷨��ϵͳ������Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�Ŀ��ư���

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartCandUp

Func:   9���������뷨����ѡ�ֵ����Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ�ĺ�ѡ�ְ���

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN ThumbSmartCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);



/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartClear

Func:   9���������뷨�������������

Input:
            data_ptr:    ���뷨ָ��

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartClear(IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartUpdate

Func:   9���������뷨������ˢ��

Input:
            data_ptr:    ���뷨ָ��

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartUpdate(IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   SmartSetCand

Func:   �������뷨�����ú�ѡ��

Input:
            data_ptr:    ���뷨ָ��

Output:

Return: TRUE:success;  else fail;

Thoughtway:

Create: yaoguangchen@spread. 2011-10-04

Modify:
        1.From ThumbSmartSetCand to SmartSetCand
        2.��������9����26����latin���ú�ѡ�֣�����ά��������
----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void SmartSetCand (IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbSmartDispEdit

Func:   9���������뷨����ǰ��������ַ�����ʾ

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbSmartDispEdit(IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartInit

Func:   26���������뷨��ʼ��

Input:
            data_ptr:    ���뷨ָ��
            method:     sogou�����Ӧ�����뷨

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartInit(IM_DATA_T *data_ptr, WD_UINT8 sogou_method);


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartTerm

Func:   9���������뷨�ر�

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartTerm(IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartDraw

Func:   26���������뷨����

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void QwertSmartDraw(IM_DATA_T * data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartInitKb

Func:   26���������뷨��ʼ���������

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void QwertSmartInitKb(IM_DATA_T *data_ptr);



/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartUpdate

Func:   26���������뷨ˢ��

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void QwertSmartUpdate(IM_DATA_T *data_ptr);



/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartCharUp

Func:   26���������뷨����

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartCharUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);



/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartCtrlLong

Func:   26���������뷨���ư���������Ϣ����

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartCtrlLong(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartCtrlUp

Func:   26���������뷨���ư�����Ϣ����

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartCtrlUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartSysUp

Func:   26���������뷨ϵͳ������Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ��ϵͳ����

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartSysUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartCandUp

Func:   26���������뷨��ѡ����Ϣ����

Input:
            data_ptr:    ���뷨ָ��
            key_ptr:    ��Ӧ��ϵͳ����

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN QwertSmartCandUp(IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   QwertSmartClear

Func:   26���������뷨�����������

Input:
            data_ptr:    ���뷨ָ��

Create: yaoguangchen@spread. 2011-10-04

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void QwertSmartClear(IM_DATA_T *data_ptr);


PUBLIC int32 Sogou_AssociateTo2DemensionArray(wchar *array, wchar *sogou_cand, uint16 array_len);

/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_ImeInit

Func:   sogou���뷨�����ʼ��

Input:  g_imeResult: ��̬���������뷨���������������ݽṹ
           g_FlashData: ���뷨��Ҫ�����ݿ�
           MMIAPIIM_GetLanguage():  ��ǰ��������

Output:
            g_imeResult:��̬���������뷨���������������ݽṹ

Return:
            == 0�ɹ��� > 0ʧ��

Thoughtway:
            1. sogou���뷨�����ʼ������Ҫ�������Բ������ʶԵ�ǰ������һ��������ϵ

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint8 Sogou_ImeInit(IM_DATA_T *data_ptr, WD_UINT8 method);

/*==============================================================================
Description: if a key in a keyboard bmp, it will need to paint the bg, so caused 
	need to repaint all other keys.
Global resource dependence: NONE
Author: yaoguang.chen
Note:
==============================================================================*/
PUBLIC BOOLEAN MMIIM_BmpBgKeyCausedRepaint (IM_DATA_T *data_ptr, KEY_INFO_T *key_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ConvertSpCapToSogou

Func:   ������ƽ̨ʹ�õĴ�Сдת��Ϊsogou���뷨��Сд���ã���������м��ĸ��
            ���ں��ڿ���ά��������ƽ̨�����뷨����

Input:  CAPITTAL_MODE_T cap:ƽ̨��Сдֵ

Output:

Return:
            SOGOU_CAPLOCK_E:    sogou���뷨����Ĵ�Сдֵ

Thoughtway:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC SOGOU_CAPLOCK_E ConvertSpCapToSogou(CAPITTAL_MODE_T cap);

/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_SetCapsLock

Func:   ����sogou���뷨��Сд

Input:
            set_caps: sogou �����Ӧ�Ĵ�Сд

Output:

Return:
            < 0 err, else 0

Thoughtway:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 Sogou_SetCapsLock(IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ConvetIndexToVKey

Func:   26��������̵İ���������ͨ������ֵ���������ص������ģ����ð���������
            ���뷨����֮��İ�������˳����Ȼ���ԣ���Ҫ��һ��ת��

Input:
            index:  26����������еİ�ť����ֵ

Output:

Return:
            err:  0
            > 0 ������̶�Ӧ������Ӧ�İ�����

Thoughtway:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint16 ConvetIndexToVKey(uint16 index);


/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_SimulatCandPage

Func:   ����sogou���뷨�Ƿ������һҳ��ѡ��

Input:
            data_ptr:  ��ǰ���뷨������

Output:

Return:

Thoughtway:
            1. sogou���뷨û����һ�θ����ж���ҳ��ѡ�֣���Ҫ�Լ�ģ������Ƿ��к�һҳ

Notes:

Create: yaoguangchen@spread. 2011-09-21

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC BOOLEAN Sogou_TestCandNextPage(IM_DATA_T *data_ptr);



/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_SimulatCandPage

Func:   ģ��sogou���뷨��ѡ�ַ�ҳ����

Input:
            data_ptr:  ��ǰ���뷨������
            pageup: ���Ϸ�ҳor���·�ҳ

Output:

Return:
            err:  0
            > 0 ������̶�Ӧ������Ӧ�İ�����

Thoughtway:
            1.  ������ͨ������WD_SetVariableֱ�ӽ���ҳǰ��״̬ת��Ϊѡ��״̬�ģ����ƺ���Ч��
                ��ֻ��ͨ�������¼��ķ�ʽ��������״̬ת����ѡ��״̬

Notes:  �ú����и����������������⣬������sogou�⺯���У���sogou�Ǳߴ���

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 Sogou_SimulatCandPage(IM_DATA_T *data_ptr, BOOLEAN pageup);



/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_SimulatLetterPage

Func:   ģ��sogou���뷨ƴ����ҳ����

Input:
            data_ptr:  ��ǰ���뷨������
            pageup: ���Ϸ�ҳor���·�ҳ

Return:
            err: < 0
            succ:  0

Thoughtway:
            ��ѡƴ����ҳ

Notes:  �ú����и����������������⣬������sogou�⺯���У���sogou�Ǳߴ���

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC int32 Sogou_SimulatLetterPage(IM_DATA_T *data_ptr, BOOLEAN pageup);


/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_SetLastDisp

Func:   �������뷨��һ����ʾ�ĺ�ѡ�ֻ�������

Input:
            data_ptr:  ��ǰ���뷨������
            lastdisp:   ��һ����ʾ�ĺ�ѡ�ֻ�������

Output:
            ����һ�εĺ�ѡ�ֻ����������浽���뷨��������

Return:
            PNULL:������Ч������ָ��Ϊ��

Thoughtway:
            1.  �ú�����Ҫ����Ϊ���뷨������ÿ�ΰ���֮ǰ����Ҫ֪����ǰһ����ʾ�ĺ�ѡ��
                �����������ȡ�ĺ�ѡ�ֻ᲻׼ȷ�������������
Notes:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void Sogou_SetLastDisp(IM_DATA_T *data_ptr, uint8 lastdisp);


/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_GetLastDisp

Func:   ��ȡ���뷨��һ����ʾ�ĺ�ѡ�ֻ�������

Input:
            data_ptr:  ��ǰ���뷨������

Output:
            ����һ�εĺ�ѡ�ֻ�����������

Return:
            0: ������Ч���״���ʾʱ�ĺ�ѡ�ָ���Ϊ0

Thoughtway:
            1.  �ú�����Ҫ����Ϊ���뷨������ÿ�ΰ���֮ǰ����Ҫ֪����ǰһ����ʾ�ĺ�ѡ��
                �����������ȡ�ĺ�ѡ�ֻ᲻׼ȷ�������������
            2.  �ú���ÿ�ε���ʱ�������½�lastdispֵд����ԭ���ǣ����ǵ�UI��ʱ��Ὣ��ѡ��
                չ�������𣬵�����ʾ�ĺ�ѡ�ָ�����ͬ
Notes:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint8 Sogou_GetLastDisp(IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_GetCandDisplayCnt

Func:   ��ȡ���뷨��ǰ��ʾ�ĺ�ѡ�ֻ�������

Input:
            data_ptr:  ��ǰ���뷨������

Output:
            ���ݺ�ѡ�ָ����ͺ�ѡ��ť�������жϵ�ǰ��ʾ�ĺ�ѡ�ָ���

Return:
            ���뷨��ǰ��ʾ�ĺ�ѡ�ֻ�������

Thoughtway:
            1.  �ú�����Ҫ����Ϊ���뷨������ÿ�ΰ���֮ǰ����Ҫ֪����ǰһ����ʾ�ĺ�ѡ��
                �����������ȡ�ĺ�ѡ�ֻ᲻׼ȷ�������������
            2. ������Sogou_GetLastDisp��ͬ�ĵط���Sogou_GetLastDisp����ȡ��ֵ����һ�����沢����
Notes:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint8 Sogou_GetCandDisplayCnt(IM_DATA_T *data_ptr);



PUBLIC int32 Sogou_DrawVkbSkin (IM_DATA_T *data_ptr, GUILCD_DRAW_DC_T *pdc_data);

/*----------------------------------------------------------------------------------------------------------------------
Name:   Sogou_GetLetterTotalCnt

Func:   ��ȡ���뷨��ǰ��ѡƴ�����ܸ���

Input:
            data_ptr:  ��ǰ���뷨������

Return:
            ���غ�ѡƴ���ܸ���

Thoughtway:
            1.��ѡƴ���ܸ�����������ʾ��ǰ��ѡƴ���ַ����Ƿ������һҳ
Notes:

Create: yaoguangchen@spread. 2011-09-09

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC uint8 Sogou_GetLetterTotalCnt(IM_DATA_T *data_ptr);


//�ϼ��������⴦��
#if defined IM_BENGALI_SUPPORT
PUBLIC BOOLEAN ThumbBengaliInit (IM_DATA_T *data_ptr);


/*----------------------------------------------------------------------------------------------------------------------
Name:   ThumbBengaliInitKb

Func:   9���������뷨���̳�ʼ��

Input:
            data_ptr:    ���뷨ָ��

Output:

Return: None

Thoughtway:

Create: yaoguangchen@spread. 2012-07-17

Modify:

----------------------------------------------------------------------------------------------------------------------*/
PUBLIC void ThumbBengaliInitKb(IM_DATA_T *data_ptr);


PUBLIC BOOLEAN ThumbBengaliTerm (IM_DATA_T *data_ptr);

#endif		//bengali end


#endif //defined (MMI_IM_PDA_SUPPORT) && defined(IM_ENGINE_SOGOU)

#endif /*_MMIIM_TOUCH_IME_SOGOU_H_*/


