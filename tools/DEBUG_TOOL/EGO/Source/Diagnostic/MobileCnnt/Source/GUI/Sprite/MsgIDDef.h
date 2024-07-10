
#if !defined(__MSGIDDEF_H__)
#define __MSGIDDEF_H__

#pragma once

/////////////////////////////////////////////////////////////////////////////
// ��Ϣ��Ӧ������ʽΪ void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
// ����ϢID
/////////////////////////////////////////////////////////////////////////////

// �û��ı��˳��������
#define WM_LANGUAGE_CHANGE     (WM_USER+600)     

// �û�ѡ��ͬ����Ϣ����
#define WM_CATEGORY_CHANGE     (WM_USER+601)     

// ��Ŀ�����ݸı�
#define WM_UPDATE_ITEM_CONTENT (WM_USER+602)

// �绰Ⱥ��ı�
#define WM_UPDATE_GROUP_CHANGE (WM_USER+604)     
typedef struct _UIC_HINT
{
	DWORD dwFlag;
	DWORD dwData;
} UIC_HINT, * PUIC_HINT;


/////////////////////////////////////////////////////////////////////////////
// ��Ϣ��Ӧ������ʽΪ LRESULT OnXXX(WPARAM wParam, LPARAM lParam)
// ����ϢID
/////////////////////////////////////////////////////////////////////////////

// �������, wParam��ʾ��Ҫ��ʾ����Ϣ��ID
#define WM_TRANSMIT_END     (WM_USER+602)     

// ��������ָʾ��Ϣ, wParam����̵�״̬�� lParam ��λ��
#define WM_PROGRESS         (WM_USER+603)     
// ���̵�״̬                                           
const int PROG_BEGIN   = 0;
const int PROG_PROCEED = 1;
const int PROG_END     = 2;
const int PROG_TITLE   = 3;

#endif // __MSGIDDEF_H__