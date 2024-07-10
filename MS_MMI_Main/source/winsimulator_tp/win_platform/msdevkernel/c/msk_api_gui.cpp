/******************************************************************************
 ** File Name:      msk_api_gui.cpp                                           *
 ** Author:         Liu Jun                                                   *
 ** Date:           25/02/2003                                                *
 ** Copyright:      2001 Spreadtrum, Incoporated. All Rights Reserved.         *
 ** Description:    This file implements the API functions declared in        *
 **                 msk_api_gui.h                                             *
 ******************************************************************************

 ******************************************************************************
 **                         Edit History                                      *
 ** ------------------------------------------------------------------------- *
 ** DATE            NAME        DESCRIPTION                                   *
 ** 25/03/2003      Liu Jun     Create.                                       *
 ******************************************************************************/

/**---------------------------------------------------------------------------*
 **                         Dependencies                                      *
 **---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "msk_api_gui.h"

/**---------------------------------------------------------------------------*
 **                         External Function                                 *
 **---------------------------------------------------------------------------*/
extern void BL_RegisterMSSimFunList(const MSK_MSSIM_FUN_LIST *pFunList);
extern void BL_StartRTOS();
extern void BL_StopRTOS();
extern BOOL BL_IsPowerOff();
extern int  BL_GetKeyCodeByName(const char * lpszKeyName);
extern void BL_SendKeycodeToRTOS(WORD wKeyCode, UINT msg);
extern void BL_SendTPcodeToRTOS(WORD xKeyCode, WORD yKeyCode, UINT msg);
extern UINT BL_SendMsgToRTOS(BYTE *pBuffer, int nLength);
extern void BL_Free(void *pBuffer);
extern "C" char g_file_buff[255];

#include "LoadLibrary.h"
extern "C" DLL_INFO_S g_dll_info[FEATURE_COUNT];
extern "C" void LoadDllModule(int nFeatureIndex);

/**---------------------------------------------------------------------------*
 **                         Function Implementations                          *
 **---------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Function:    MSK_RegisterMSSimFunList
// Author:      Liu Jun
// Purpose:     Register the functions defined in the MSSim GUI module
// Calls:       BL_RegisterMSSimFunList()
// Called by:   The MSSim GUI module
// Remarks:     1. MSK_MSSIM_FUN_LIST is defined in "msk_type.h"
//              2. Call this function when the MSSim starts up, thus, MSKernel
//                 can call the GUI functions via 'pFunList'.
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI void 
MSK_RegisterMSSimFunList(const MSK_MSSIM_FUN_LIST *pFunList) // function pointers
{
    BL_RegisterMSSimFunList(pFunList);
}

#ifdef PRJ_TDPS_UEIT_MMI
#include <stdio.h>
/* +cr168477 */
#include "sim_simu_engine_interface.h"
/* -cr168477 */

static int sPortUuid = -1;
extern "C" 
{
	int setenvfile( const char *env_file );
/* +CR188901 */
	//int nv_fs_set_def_config_file(const char *def_config_file);
	int nv_fs_set_def_config_file(const char *def_config_file, int PortUuid);
/* -CR188901 */	
	int sim_fs_set_def_config_file(const char *def_config_file);
	int set_ue_config_file( const char *ue_config_file );
/* +cr230395 */
	int get_env_psport();
	HWND g_hWnd = NULL;
/* -cr230395 */

}
/*
MSKERNEL_API_GUI void 
MSK_InitRTOS(
	int         PortUuid,	// The uniqued port number (tcp port number)
	const char *NvCfgPath,	// The path of nvitem.bin
	const char *SimCfgPath	// The path of SIM config path (SIMTest.ini)
	)
*/
void MSK_InitRTOS(void)
{
	// Store UUID of port.
	//sPortUuid = /*PortUuid*/4679;
	sPortUuid = get_env_psport();
	
/* +cr230395 */
	char winText[256];
	sprintf(winText, /*"MSSim(%d)", sPortUuid*/"MSDev");
	HWND hwnd = ::FindWindow(0, winText);
	if( !hwnd )
	{
		HWND hDesWnd = ::GetDesktopWindow();
		HWND hChldWnd = ::GetWindow(hDesWnd, GW_CHILD);
		while(::IsWindow(hChldWnd))
		{
			::GetWindowText(hChldWnd, winText, sizeof(winText));
			if( strstr(winText, /*"MSSim"*/"MSDev") )
			{
				g_hWnd = hChldWnd;
				break;
			}
			hChldWnd = ::GetWindow(hChldWnd, GW_HWNDNEXT);
		}
	}
	else
	{
		::GetWindowText(g_hWnd=hwnd, winText, sizeof(winText));
	}
/* -cr230395 */


	// Makeup Default NV config file name
	const char *MssimEnvCfgPath = NULL;
	char def_mssim_env_file[MAX_PATH];
	if(!MssimEnvCfgPath) {
		sprintf(def_mssim_env_file, "temp\\mssim_s_env_%d.ini", sPortUuid);
	} else {
		sprintf(def_mssim_env_file, "%s\\mssim_s_env_%d.ini", MssimEnvCfgPath, sPortUuid);
	}

	// Makeup Default NV config file name
	char def_nv_config_file[MAX_PATH];
	if(/*!NvCfgPath*/1) {
		sprintf(def_nv_config_file, "temp\\nvitem_%d.bin", sPortUuid);
	} else {
		//sprintf(def_nv_config_file, "%s\\nvitem_%d.bin", NvCfgPath, PortUuid);
	}

/* +cr168477 */
	// Makeup Default SIM config file name
	char def_sim_config_file[MAX_PATH];
	if(/*!SimCfgPath*/1) {
		sprintf(def_sim_config_file, "temp\\uicc_%d.idf", sPortUuid);
	} else {
		//sprintf(def_sim_config_file, "%s\\temp\\uicc_%d.idf", SimCfgPath, sPortUuid);
	}
/* -cr168477 */


	// Makeup Default ue_config file name
	char ue_config_file[MAX_PATH]={0};
	sprintf(ue_config_file, "temp\\ue_config_%d.ini", sPortUuid);

	// Init libs config setting: NV, SIM
	setenvfile(def_mssim_env_file);
/* +CR188901 */
	//nv_fs_set_def_config_file(def_nv_config_file);
	nv_fs_set_def_config_file(def_nv_config_file, sPortUuid);
/* -CR188901 */
/* +cr168477 */
	sim_simu_search_engine_set_idf_path((signed char *)def_sim_config_file);
//	sim_fs_set_def_config_file(def_sim_config_file);
/* -cr168477 */
    set_ue_config_file(ue_config_file);

}
#endif

//-----------------------------------------------------------------------------
// Function:    MSK_StartRTOS
// Author:      Liu Jun
// Purpose:     Start the real-time OS
// Calls:       BL_StartRTOS()
// Called by:   The MSSim GUI module
// Remarks:     This function must be invoked in an individual thread
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI void 
MSK_StartRTOS()
{
	//load control panel dll module
#ifdef FILESYS_WIN32
	strcpy( g_dll_info[FILESYSTEM_FEATURE].szDllFilePath, "filesys.dll");
	LoadDllModule(FILESYSTEM_FEATURE);
	// Bluetooth simulation is not ready
	//strcpy( g_dll_info[BLUETOOTH_FEATURE].szDllFilePath, "bluetooth.dll");
	//LoadDllModule(BLUETOOTH_FEATURE);	
#endif	
	strcpy( g_dll_info[NET_SETTING_FEATURE].szDllFilePath, "VNetSetting.dll");
	LoadDllModule(NET_SETTING_FEATURE);
#ifdef PRJ_TDPS_UEIT_MMI
	MSK_InitRTOS();
#endif
    BL_StartRTOS();
}

//-----------------------------------------------------------------------------
// Function:    MSK_StopRTOS
// Author:      Liu Jun
// Purpose:     Stop the real-time OS
// Calls:       BL_StopRTOS()
// Called by:   The MSSim GUI module
// Remarks:     N/A
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI void 
MSK_StopRTOS()
{
    BL_StopRTOS();
}

//-----------------------------------------------------------------------------
// Function:    MSK_IsPowerOff
// Author:      Liu Jun
// Purpose:     Judge the MS whether it's powered on or off
// Calls:       BL_IsPowerOff()
// Called by:   The MSSim GUI module
// Remarks:     N/A
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI BOOL // TRUE indicates powered off
MSK_IsPowerOff()
{
    return BL_IsPowerOff();
}

//-----------------------------------------------------------------------------
// Function:    MSK_GetKeyCodeByName
// Author:      Liu Jun
// Purpose:     Get the virtual key code of a specified key
// Calls:       BL_GetKeyCodeByName()
// Called by:   The MSSim GUI module
// Remarks:     1. Key names are defined in the configuration file, MSSim.ini
//              2. Specify the key via it's name, such as '1', '*', ...
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI int                           // virtual key code
MSK_GetKeyCodeByName(const char * lpszKeyName) // key name in string format
{
    return BL_GetKeyCodeByName(lpszKeyName);
}

//-----------------------------------------------------------------------------
// Function:    MSK_SendKeycodeToRTOS
// Author:      Liu Jun
// Purpose:     Send the virtual key code to the real-time OS
// Calls:       BL_SendKeycodeToRTOS()
// Called by:   The MSSim GUI module
// Remarks:     Call this function when a key is pressed down/up
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI void
MSK_SendKeycodeToRTOS(WORD wKeyCode, // virtual key code
                      UINT state)    // key state: KPD_DOWN or KPD_UP
{
    BL_SendKeycodeToRTOS(wKeyCode, state);
}

//-----------------------------------------------------------------------------
// Function:    MSK_SendTPcodeToRTOS
// Author:      Beijing team
// Purpose:     Send the virtual touch panel key code to the real-time OS
// Calls:       BL_SendTPcodeToRTOS()
// Called by:   The MSSim GUI module
// Remarks:    
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI void
MSK_SendTPcodeToRTOS(WORD xKeyCode, WORD yKeyCode,// virtual key code
                      UINT state)    // key state: KPD_DOWN or KPD_UP
{
    BL_SendTPcodeToRTOS(xKeyCode, yKeyCode,state);
}

//-----------------------------------------------------------------------------
// Function:    MSK_SendMsgToRTOS
// Author:      Liu Jun
// Purpose:     Send a message to the real-time OS
// Calls:       BL_SendMsgToRTOS()
// Called by:   The MSSim GUI module
// Remarks:     Call this function when a key is pressed down/up
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI UINT             // sending results: SCI_SUCCESS indicates success
MSK_SendMsgToRTOS(BYTE *pBuffer,  // message body
                  int   nLength)  // message size
{
    return BL_SendMsgToRTOS(pBuffer, nLength);
}

//-----------------------------------------------------------------------------
// Function:    MSK_Free
// Author:      Liu Jun
// Purpose:     Free the spcified memory block
// Calls:       BL_Free()
// Called by:   The MSSim GUI module
// Remarks:     Call this function to free a memory block which is allocated in
//              the MSKernel module.
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI void 
MSK_Free(void *pBuffer) // memory block to be freed
{
    BL_Free(pBuffer);
}

//-----------------------------------------------------------------------------
// Function:    MSK_GetmessagefromMSDev
// Author:      Liu Jun
// Purpose:     Get resource file path
// Remarks:     Call this function to Get resource file path
//-----------------------------------------------------------------------------
MSKERNEL_API_GUI char* 
MSK_GetResourceFile()
{
	return g_file_buff; // Get resource file path
}