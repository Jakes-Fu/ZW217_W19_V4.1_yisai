; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTestPage
LastTemplate=CPropertyPage
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "VNetTest.h"

ClassCount=5
Class1=CVNetTestApp
Class2=CVNetTestDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_TEST_PAGE (English (U.S.))
Resource2=IDR_MAINFRAME
Class4=CTestSheet
Resource3=IDD_VNETTEST_DIALOG
Class5=CTestPage
Resource4=IDD_ABOUTBOX

[CLS:CVNetTestApp]
Type=0
HeaderFile=VNetTest.h
ImplementationFile=VNetTest.cpp
Filter=N

[CLS:CVNetTestDlg]
Type=0
HeaderFile=VNetTestDlg.h
ImplementationFile=VNetTestDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_BTN_TEST

[CLS:CAboutDlg]
Type=0
HeaderFile=VNetTestDlg.h
ImplementationFile=VNetTestDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_VNETTEST_DIALOG]
Type=1
Class=CVNetTestDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BTN_TEST,button,1342242816

[CLS:CTestSheet]
Type=0
HeaderFile=TestSheet.h
ImplementationFile=TestSheet.cpp
BaseClass=CPropertySheet
Filter=W

[DLG:IDD_TEST_PAGE (English (U.S.))]
Type=1
Class=CTestPage
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[CLS:CTestPage]
Type=0
HeaderFile=TestPage.h
ImplementationFile=TestPage.cpp
BaseClass=CPropertyPage
Filter=D
LastObject=CTestPage

