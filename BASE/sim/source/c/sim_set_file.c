/******************************************************************************
 ** File Name:      sim_set_file.c                                            *
 ** Author:         Raislin.Kong                                              *
 ** Date:           2003.6                                                    *
 ** Copyright:      Copyright 2001-2010 by Spreadtrum Communications,Inc.     *
 **                 All Rights Reserved.                                      *
 **                 This software is supplied under the terms of a license    *
 **                 agreement or non-disclosure agreement with Spreadtrum.    *
 **                 Passing on and copying of this document,and communication *
 **                 of its contents is not permitted without prior written    *
 **                 authorization.                                            *
 ** Description:    This funciton define the fucntion to get the sim set file *
 **                 directory                                                 *
 ******************************************************************************

 ******************************************************************************
 **                        Edit History                                       *
 ** --------------------------------------------------------------------------*
 ** Date           Name             Description                               *
 **---------------------------------------------------------------------------*
 ** 2003.6         Raislin.Kong     Create                                    *
 ******************************************************************************/
#include <stdio.h>
#include "assert.h"
#include "windows.h"
#include "shlobj.h"

//the base directory decided or not
static BOOLEAN bBaseDirDecided[4] ={FALSE, FALSE,FALSE,FALSE};
//the SIM card info file directory
static char base_directory[4][MAX_PATH];


/**********************************************************************
//    Description:
//      Get the base directory of the sim set files
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
static void GetBaseDir(int card_num)
{
    char ini_file[MAX_PATH];
    char gsm_file[MAX_PATH];
	int nIndex;
	BOOLEAN bReg = TRUE;
	long lReturn;
	HANDLE hFind = NULL;
	LPTSTR ptemp,pend;
	BROWSEINFO binfo;
	WIN32_FIND_DATA findData;
	LPITEMIDLIST lpid;
	char btitle[] = "请选择SIM卡的设置文件路径"; 

    //check the current directory of the mobile exe or sim simulator exe for
    //sim set file first.
    GetModuleFileName(NULL,base_directory[card_num],MAX_PATH);
    ptemp = strchr(base_directory[card_num],'\\');
    while(ptemp)
    {
        ptemp += 1;
        pend = ptemp;
        ptemp = strchr(ptemp,'\\');
    }
    *pend = '\0';
    
    sprintf(gsm_file, "%sSIM_Set_Files\\SIM%d\\MF_SELECT.inf", base_directory[card_num], (card_num+1));

    hFind = FindFirstFile(gsm_file,&findData);
	if (INVALID_HANDLE_VALUE != hFind)
	{
		FindClose(hFind);
        sprintf(base_directory[card_num] + strlen(base_directory[card_num]), "SIM_Set_Files\\SIM%d\\", (card_num+1));
		bBaseDirDecided[card_num] = TRUE;
		return;
	}
    sprintf(ini_file,"%sSIMTest.ini",base_directory[card_num]);
	lReturn = GetPrivateProfileString("SIMTest","BaseDir",NULL,base_directory[card_num],MAX_PATH,ini_file);
	if (lReturn == ERROR_SUCCESS)
	{
		bReg = FALSE;
	}
	if (bReg)
	{
CHECK_DIR:
        nIndex = strlen(base_directory[card_num]);
		if (base_directory[card_num][nIndex - 1] != '\\')
		{
			base_directory[card_num][nIndex] = '\\';
			base_directory[card_num][nIndex + 1] = '\0';
		}
        sprintf(base_directory[card_num] + strlen(base_directory[card_num]), "SIM%d\\", (card_num+1));
        sprintf(gsm_file, "%sMF_SELECT.inf", base_directory[card_num]);

        hFind = FindFirstFile(gsm_file,&findData);
		if (INVALID_HANDLE_VALUE != hFind)
		{
			
			FindClose(hFind);
			bBaseDirDecided[card_num] = TRUE;
			return;
		}
	}
	
	memset(&binfo,0x0,sizeof(BROWSEINFO));
	binfo.pszDisplayName = base_directory[card_num];
	binfo.lpszTitle = btitle;

	lpid = SHBrowseForFolder(&binfo);
	if (lpid != NULL)
	{
		if (SHGetPathFromIDList(lpid,base_directory[card_num]))
		{		
			WritePrivateProfileString("SIMTest","BaseDir",base_directory[card_num],ini_file);
			goto CHECK_DIR;
		}
	}

}

/**********************************************************************
//    Description:
//      Get the base directory of the sim set files
//    Global resource dependence :
//    Author:Raislin.kong
//    Note:
***********************************************************************/
void SIMTIW_GetSettingFileDir(
                                char* lpszDir,
                                int card_num
                                )
{
    assert(NULL != lpszDir);/*assert verified */

    if (!bBaseDirDecided[card_num])
	{
		GetBaseDir(card_num);
	}

    memcpy(lpszDir,base_directory[card_num],strlen(base_directory[card_num]));
}
