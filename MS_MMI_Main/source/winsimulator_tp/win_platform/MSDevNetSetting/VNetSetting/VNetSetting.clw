; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=7
Class1=CVNetSettingApp
LastClass=CCloseWaitDlg
NewFileInclude2=#include "VNetSetting.h"
ResourceCount=8
NewFileInclude1=#include "stdafx.h"
Resource1=IDD_NETSETTING_PANAL (English (U.S.))
Class2=CNetsettingDlg
LastTemplate=CDialog
Class3=CNetstateDlg
Resource2=IDD_IPSEARCHING (English (U.S.))
Class4=CIpTestingDlg
Resource3=IDD_IPTESTING (English (U.S.))
Resource4= (English (U.S.))
Resource5=IDD_NETSTATE_PANAL (English (U.S.))
Class5=CMacTestingDlg
Class6=CIpSearchDlg
Resource6=IDD_IPSEARCHING1 (English (U.S.))
Resource7=IDD_MACTESTING (English (U.S.))
Class7=CCloseWaitDlg
Resource8=IDD_CLOSEWAIT (English (U.S.))

[CLS:CVNetSettingApp]
Type=0
HeaderFile=VNetSetting.h
ImplementationFile=VNetSetting.cpp
Filter=N
LastObject=CVNetSettingApp

[CLS:CNetsettingDlg]
Type=0
HeaderFile=NetsettingDlg.h
ImplementationFile=NetsettingDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CNetsettingDlg
VirtualFilter=dWC

[DLG:IDD_NETSTATE_PANAL (English (U.S.))]
Type=1
Class=CNetstateDlg
ControlCount=27
Control1=IDC_STATIC_PARAM_SIMU,button,1342177287
Control2=ID_STATIC,static,1342308354
Control3=IDC_EDIT_MAC,edit,1342244864
Control4=ID_STATIC,static,1342308354
Control5=IDC_EDIT_IP,edit,1342244864
Control6=ID_STATIC,static,1342308354
Control7=IDC_EDIT_SNMASK,edit,1342244864
Control8=ID_STATIC,static,1342308354
Control9=IDC_EDIT_GATEWAY,edit,1342244864
Control10=ID_STATIC,static,1342308354
Control11=IDC_EDIT_DNS1,edit,1342244864
Control12=IDC_EDIT_DNS2,edit,1342244864
Control13=ID_STATIC,static,1342308354
Control14=IDC_EDIT_NETID,edit,1342244864
Control15=ID_STATIC,static,1342308354
Control16=IDC_EDIT_CFGFILE,edit,1342244992
Control17=IDC_BTN_SETTING,button,1342242816
Control18=IDC_STATIC_PARAM_ETHER,button,1342177287
Control19=ID_STATIC,static,1342308354
Control20=IDC_EDIT_DEVICE_DESC,edit,1342244864
Control21=ID_STATIC,static,1342308354
Control22=IDC_EDIT_DEVICE_NAME,edit,1342244864
Control23=ID_STATIC,static,1342308354
Control24=IDC_EDIT_DEVICE_STATE,edit,1342244864
Control25=IDC_CHECK_RECV,button,1476460547
Control26=IDC_CHECK_SEND,button,1476460547
Control27=IDC_BTN_DRV_INSTALL,button,1342242816

[CLS:CNetstateDlg]
Type=0
HeaderFile=NetstateDlg.h
ImplementationFile=NetstateDlg.cpp
BaseClass=CPropertyPage
Filter=D
VirtualFilter=idWC
LastObject=CNetstateDlg

[DLG:IDD_NETSETTING_PANAL (English (U.S.))]
Type=1
Class=CNetsettingDlg
ControlCount=36
Control1=IDC_STATIC_PARAM_SIMU,button,1342177287
Control2=IDC_STATIC_NOTE1,static,1342308352
Control3=IDC_STATIC_NOTE2,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_MAC1,edit,1484849288
Control6=IDC_EDIT_MAC2,edit,1350631560
Control7=IDC_EDIT_MAC3,edit,1350631560
Control8=IDC_EDIT_MAC4,edit,1350631560
Control9=IDC_EDIT_MAC5,edit,1350631560
Control10=IDC_EDIT_MAC6,edit,1350631560
Control11=IDC_BTN_MACTEST,button,1342242816
Control12=IDC_STATIC,static,1342308352
Control13=IDC_IPADDRESS_SIMU,SysIPAddress32,1342242816
Control14=IDC_CHK_DHCP,button,1342242819
Control15=IDC_CHK_IPSEARCH,button,1342242819
Control16=IDC_BTN_IPTEST,button,1342242816
Control17=IDC_STATIC_PARAM_LOCAL,button,1342177287
Control18=IDC_STATIC,static,1342308352
Control19=IDC_EDIT_DEVDESC,edit,1342244864
Control20=IDC_STATIC,static,1342308352
Control21=IDC_COMBO_ADAPTER,combobox,1344339971
Control22=IDC_STATIC,static,1342308352
Control23=IDC_EDIT_LMAC,edit,1342244864
Control24=IDC_STATIC,static,1342308352
Control25=IDC_EDIT_LIP,edit,1342244864
Control26=IDC_STATIC,static,1342308352
Control27=IDC_EDIT_SNMASK,edit,1342244864
Control28=IDC_STATIC,static,1342308352
Control29=IDC_EDIT_GATEWAY,edit,1342244864
Control30=IDC_STATIC,static,1342308352
Control31=IDC_COMBO_DNS1,combobox,1344339971
Control32=IDC_COMBO_DNS2,combobox,1344339971
Control33=IDC_STATIC,static,1342308352
Control34=IDC_EDIT_DHCP,edit,1342244864
Control35=IDOK,button,1342242817
Control36=IDCANCEL,button,1342242816

[CLS:CIpTestingDlg]
Type=0
HeaderFile=IpTestingDlg.h
ImplementationFile=IpTestingDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CIpTestingDlg
VirtualFilter=dWC

[DLG:IDD_IPTESTING (English (U.S.))]
Type=1
Class=CIpTestingDlg
ControlCount=2
Control1=IDC_STATIC_NOTES,static,1342308353
Control2=IDOK,button,1342242817

[DLG:IDD_IPSEARCHING (English (U.S.))]
Type=1
Class=CIpSearchDlg
ControlCount=12
Control1=IDC_STATIC,button,1342177287
Control2=IDC_IPADDRESS_BEGIN,SysIPAddress32,1342242816
Control3=IDC_STATIC_RANGE,static,1342308353
Control4=IDC_IPADDRESS_END,SysIPAddress32,1342242816
Control5=IDC_RADIO_FORWARD,button,1342308361
Control6=IDC_RADIO_BACKWARD,button,1342308361
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC_NOTES,static,1342308353
Control9=IDC_STATIC,static,1342308354
Control10=IDC_COMBO_RESULT,combobox,1344339971
Control11=IDC_BTN_NEXT,button,1342242817
Control12=IDOK,button,1342242817

[DLG:IDD_MACTESTING (English (U.S.))]
Type=1
Class=CMacTestingDlg
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_STATIC_NOTES,static,1342308353

[CLS:CMacTestingDlg]
Type=0
HeaderFile=MacTestingDlg.h
ImplementationFile=MacTestingDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CMacTestingDlg
VirtualFilter=dWC

[CLS:CIpSearchDlg]
Type=0
HeaderFile=ipsearchdlg.h
ImplementationFile=ipsearchdlg.cpp
BaseClass=CDialog
LastObject=CIpSearchDlg
Filter=D
VirtualFilter=dWC

[DLG:IDD_IPSEARCHING1 (English (U.S.))]
Type=1
Class=?
ControlCount=12
Control1=IDC_STATIC,button,1342177287
Control2=IDC_IPADDRESS_BEGIN,SysIPAddress32,1342242816
Control3=IDC_STATIC_RANGE,static,1342308353
Control4=IDC_IPADDRESS_END,SysIPAddress32,1342242816
Control5=IDC_RADIO_FORWARD,button,1342308361
Control6=IDC_RADIO_BACKWARD,button,1342308361
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC_NOTES,static,1342308353
Control9=IDC_STATIC,static,1342308354
Control10=IDC_COMBO_RESULT,combobox,1344339971
Control11=IDC_BTN_NEXT,button,1342242817
Control12=IDOK,button,1342242817

[DLG:IDD_CLOSEWAIT (English (U.S.))]
Type=1
Class=CCloseWaitDlg
ControlCount=3
Control1=IDC_STATIC_NOTES,static,1342308353
Control2=IDOK,button,1342242817
Control3=IDC_STATIC_TIMER,static,1342308353

[CLS:CCloseWaitDlg]
Type=0
HeaderFile=CloseWaitDlg.h
ImplementationFile=CloseWaitDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CCloseWaitDlg
VirtualFilter=dWC

