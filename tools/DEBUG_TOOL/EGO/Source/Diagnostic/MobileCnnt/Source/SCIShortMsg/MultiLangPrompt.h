// File Name:   MultiLangPrompt.h 
// Author:      anli.wei
// Date:        2005-04-28
// Copyright:   Copyright (2004) Spreatrum Incop. All Rights Reserved.
// Description: 定义多语言信息提示，如果需要增加语言的支持，直接在MLP_DEF中
//              添加即可，注意添加时，新增项一定要在IDS_MLP_NULL和IDS_MLP_MAX
//              之间，否则程序能正确处理
///////////////////////////////////////////////////////////////////////////////

// MLP_DEF(ID, 中文简体, 英文)

MLP_DEF(IDS_MLP_NULL, _T(""), _T(""))

MLP_DEF(IDS_MLP_CONNECTION_FAIL,     _T("未检测到手机!"),        _T("No detect Phone!"))
MLP_DEF(IDS_MLP_DISABLEECHO_FAIL,    _T("禁止命令的回声失败!"),  _T("Disable echo fail!"))
MLP_DEF(IDS_MLP_SETMODE_FAIL,        _T("设置短信模式失败!"),    _T("Set SMS's Mode fail!"))
MLP_DEF(IDS_MLP_QUERYNETSTATUS_FAIL, _T("查询网络状态失败!"),    _T("Query net status fail!"))  

// [[ 注意：不可更改网络状态的顺序
MLP_DEF(IDS_MLP_NET_UNREG,   _T("未注册网络!"),       _T("Unregistered network!"))
MLP_DEF(IDS_MLP_NET_REG,     _T("成功注册网络!"),     _T("Registered network!"))
MLP_DEF(IDS_MLP_NET_SEARCH,  _T("正在搜索网络!"),     _T("Searching network!"))
MLP_DEF(IDS_MLP_NET_DENIED,  _T("网络注册被拒绝!"),   _T("Denied register network!"))
MLP_DEF(IDS_MLP_NET_UNKNOWN, _T("未知状态!"),         _T("Unknown status!"))
MLP_DEF(IDS_MLP_NET_ROAMING, _T("成功注册漫游网络!"), _T("Roaming network!"))
// ]

MLP_DEF(IDS_MLP_QUERY_CENTER, _T("查询短信中心号码失败!"), _T("Query ServiceCenter Fail!"))
MLP_DEF(IDS_MLP_SET_CENTER,   _T("设置短信中心号码失败!"), _T("Set ServiceCenter Fail!"))

MLP_DEF(IDS_MLP_REG_INCSMS,   _T("注册短信自动提示失败!"), _T("Register incoming SMS Fail!"))

MLP_DEF(IDS_MLP_SELECT_SP,    _T("选择短信存储位置失败!"), _T("Select SMS's store place Fail!"))
MLP_DEF(IDS_MLP_QUERYCAPACITY,_T("查询短信容量失败!"),     _T("Query capacity Fail!"))
   
MLP_DEF(IDS_MLP_READALLSMS,   _T("读取所有短信失败!"),     _T("Read all SMS Fail!"))
MLP_DEF(IDS_MLP_READSMS,      _T("读取短信失败!"),         _T("Read SMS Fail!"))

MLP_DEF(IDS_MLP_SENDSMS,      _T("发送短信失败!"),         _T("Send SMS Fail!"))

MLP_DEF(IDS_MLP_DELETESMS,    _T("删除短信失败!"),         _T("Delete SMS Fail!"))
MLP_DEF(IDS_MLP_DELETEALLSMS, _T("删除全部短信失败!"),     _T("Delete All SMS Fail!"))

MLP_DEF(IDS_MLP_MAX, _T(""), _T(""))
