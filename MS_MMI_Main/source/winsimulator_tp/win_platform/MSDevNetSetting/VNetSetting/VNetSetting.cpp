// VNetSetting.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "VNetSetting.h"

#include "netstatedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CVNetSettingApp

BEGIN_MESSAGE_MAP(CVNetSettingApp, CWinApp)
	//{{AFX_MSG_MAP(CVNetSettingApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVNetSettingApp construction

CVNetSettingApp::CVNetSettingApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CVNetSettingApp::~CVNetSettingApp()
{
    if( NULL != g_pPage )
    {
        delete g_pPage;
        g_pPage = NULL;
    }
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CVNetSettingApp object

CVNetSettingApp theApp;

CNetstateDlg *g_pPage = NULL;


/////////////////////////////////////////////////////////////////////////////
// Export Function

/////////////////////////////////////////////////////////////////////////////
// Export Function,Client will call this function to display ethernet parameter
// settings dialog
// 
BOOL ShowVNetSettings(						// if success return TRUE,or return FALSE
					  BOOL bMsStartup		// Simulator startup status,TRUE means simulator
											// is started,FALSE means simulator is power off
					 )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//!! TODO: Display your dialog here
//    CNetstateDlg dlg;
//    dlg.DoModal();

    if(g_pPage == NULL)
    {
        CNetstateDlg *pPage = new CNetstateDlg();
        //pPage->Construct(IDD_NETSTATE_PANAL,NULL);

        g_pPage = pPage;

        HWND handle=::FindWindow(NULL,_T("Test"));

        if (NULL!=handle)
        {
		    CPropertySheet objSheet;
		    objSheet.Attach(handle);
		    objSheet.AddPage(pPage);
		    objSheet.SetActivePage(pPage);
		    objSheet.Detach();
		    
        }   
    }


	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Export function,When simulator is powered on or off,client will call this 
// function to notify it
void SetMsState(
					BOOL bMsStartup			// Simulator startup status,TRUE means simulator
											// is started,FALSE means simulator is power off
				)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//!! TODO: add your code here

}

//////////////////////////////////////////////////////////////////////////////
//!! Dummy functions for test only, should import from msdevkernel.dll in release version

int MSK_VNet_RenewNetInterface( int )
{
	return 0;
}

int MSK_VNet_GetIpForMac( long* ipaddr, char* mac, char* netdev, int index )
{
	return 0;
}

int MSK_VNet_EnableDataSend( int index, BOOL snd_enable )
{
	return 0;
}

int MSK_VNet_EnableDataRecv( int index, BOOL rcv_enable )
{
	return 0;
}

int MSK_VNet_GetNetInterfaceState( int index )
{
	return 0;
}
