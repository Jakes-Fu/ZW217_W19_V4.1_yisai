// File Name:   MultiLangPrompt.h 
// Author:      anli.wei
// Date:        2005-04-28
// Copyright:   Copyright (2004) Spreatrum Incop. All Rights Reserved.
// Description: �����������Ϣ��ʾ�������Ҫ�������Ե�֧�֣�ֱ����MLP_DEF��
//              ��Ӽ��ɣ�ע�����ʱ��������һ��Ҫ��IDS_MLP_NULL��IDS_MLP_MAX
//              ֮�䣬�����������ȷ����
///////////////////////////////////////////////////////////////////////////////

// MLP_DEF(ID, ���ļ���, Ӣ��)

MLP_DEF(IDS_MLP_NULL, _T(""), _T(""))

MLP_DEF(IDS_MLP_CONNECTION_FAIL,     _T("δ��⵽�ֻ�!"),        _T("No detect Phone!"))
MLP_DEF(IDS_MLP_DISABLEECHO_FAIL,    _T("��ֹ����Ļ���ʧ��!"),  _T("Disable echo fail!"))
MLP_DEF(IDS_MLP_SETMODE_FAIL,        _T("���ö���ģʽʧ��!"),    _T("Set SMS's Mode fail!"))
MLP_DEF(IDS_MLP_QUERYNETSTATUS_FAIL, _T("��ѯ����״̬ʧ��!"),    _T("Query net status fail!"))  

// [[ ע�⣺���ɸ�������״̬��˳��
MLP_DEF(IDS_MLP_NET_UNREG,   _T("δע������!"),       _T("Unregistered network!"))
MLP_DEF(IDS_MLP_NET_REG,     _T("�ɹ�ע������!"),     _T("Registered network!"))
MLP_DEF(IDS_MLP_NET_SEARCH,  _T("������������!"),     _T("Searching network!"))
MLP_DEF(IDS_MLP_NET_DENIED,  _T("����ע�ᱻ�ܾ�!"),   _T("Denied register network!"))
MLP_DEF(IDS_MLP_NET_UNKNOWN, _T("δ֪״̬!"),         _T("Unknown status!"))
MLP_DEF(IDS_MLP_NET_ROAMING, _T("�ɹ�ע����������!"), _T("Roaming network!"))
// ]

MLP_DEF(IDS_MLP_QUERY_CENTER, _T("��ѯ�������ĺ���ʧ��!"), _T("Query ServiceCenter Fail!"))
MLP_DEF(IDS_MLP_SET_CENTER,   _T("���ö������ĺ���ʧ��!"), _T("Set ServiceCenter Fail!"))

MLP_DEF(IDS_MLP_REG_INCSMS,   _T("ע������Զ���ʾʧ��!"), _T("Register incoming SMS Fail!"))

MLP_DEF(IDS_MLP_SELECT_SP,    _T("ѡ����Ŵ洢λ��ʧ��!"), _T("Select SMS's store place Fail!"))
MLP_DEF(IDS_MLP_QUERYCAPACITY,_T("��ѯ��������ʧ��!"),     _T("Query capacity Fail!"))
   
MLP_DEF(IDS_MLP_READALLSMS,   _T("��ȡ���ж���ʧ��!"),     _T("Read all SMS Fail!"))
MLP_DEF(IDS_MLP_READSMS,      _T("��ȡ����ʧ��!"),         _T("Read SMS Fail!"))

MLP_DEF(IDS_MLP_SENDSMS,      _T("���Ͷ���ʧ��!"),         _T("Send SMS Fail!"))

MLP_DEF(IDS_MLP_DELETESMS,    _T("ɾ������ʧ��!"),         _T("Delete SMS Fail!"))
MLP_DEF(IDS_MLP_DELETEALLSMS, _T("ɾ��ȫ������ʧ��!"),     _T("Delete All SMS Fail!"))

MLP_DEF(IDS_MLP_MAX, _T(""), _T(""))
