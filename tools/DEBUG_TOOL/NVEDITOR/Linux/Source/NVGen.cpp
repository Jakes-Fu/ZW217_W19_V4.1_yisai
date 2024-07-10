// NVEditor.cpp : Defines the class behaviors for the application.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <sys/stat.h>
#include "NVItemDef.h"
#include "FileParser.h"

#define VERSION_INFO  _T("NVGen_R2.17.1901\n")
ST_NVCmdLine		g_stCmd;  
VEC_FILEINFO		g_vecFileInfo;

extern   BOOL		g_bBigEndian;

typedef enum FILE_TYPE_T 
{ 
    E_DOT_H_FILE	= 0,
	E_DOT_BIN_FILE	= 1,
	E_DOT_PRJ_FILE,
	E_DOT_XPRJ_FILE,
	E_DOT_NV_FILE,
	E_DOT_TXT_FILE,
	E_UNDEF_TYPE
};

void ShowUsage()
{
    _tprintf(_T("\n\nNVGen Usage:\n"));
    _tprintf(_T("NVGen.exe <xxx.prj> [-L] [yyy.nv] [dsp.bin] [zzz.txt] [-W] [-id FileID FilePath] [-T] [[-m <MergeFilePath> yyy_config.h]|[-c <MergeFilePath> Moderm.bin]] [-v] [-f FixBinFile] [-h HashValue.h] [-FastMode]\n\n"));
}
BOOL IsExistFile(TCHAR* lpFile)
{
    BOOL bOK = FALSE;
    if(lpFile && _tcslen(lpFile)&& -1 != _taccess(lpFile, 0) )
    {
        bOK = TRUE;
    }
    else
    {
        _tprintf(_T("File[%s] don't exist\n"),lpFile );
    }
	
    return bOK;
}
DWORD GetFileSize( TCHAR* lpFileName)
{
    DWORD dwSize = 0;
    struct stat finfo = {0};
    if(IsExistFile(lpFileName))
    {
        stat(lpFileName, &finfo);
        dwSize = finfo.st_size;
    }
    return dwSize;
	
}

BOOL IsSpecifiedFile(TCHAR* lpFile,int nFileType)
{
    BOOL bRet = FALSE;
    if(NULL == lpFile)
    {
        return FALSE;
    }
	
    TCHAR* pFileExt = _tcsrchr(lpFile,_T('.'));
    if(pFileExt)
    {
        switch(nFileType)
        {
        case E_DOT_H_FILE:
            if ( 0 ==_tcsicmp(pFileExt,_T(".h")) ) 
            {
                bRet = TRUE;
            }		
            break;
        case E_DOT_BIN_FILE:
            if ( 0 ==_tcsicmp(pFileExt,_T(".bin")) ) 
            {
                bRet = TRUE;
            }		
            break;
        case E_DOT_PRJ_FILE:
            if ( 0 ==_tcsicmp(pFileExt,_T(".prj")) ) 
            {
                bRet = TRUE;
            }
            break;
        case E_DOT_XPRJ_FILE:
            if ( 0 ==_tcsicmp(pFileExt,_T(".xprj")) ) 
            {
                bRet = TRUE;
            }
            break;
			
        case E_DOT_NV_FILE:
            if ( 0 ==_tcsicmp(pFileExt,_T(".nv")) ) 
            {
                bRet = TRUE;
            }		
            break;
        case E_DOT_TXT_FILE:
            if ( 0 ==_tcsicmp(pFileExt,_T(".txt")) ) 
            {
                bRet = TRUE;
            }		
            break;
			
        default:
            break;
        }
    }
	
    return bRet;
	
}

BOOL ParseParam(TCHAR* lpszParam, BOOL bFlag)
{
	BOOL bValidPara = TRUE;
	if (NULL == lpszParam)
	{
		return FALSE;
	}
	if(0 == g_stCmd.nParaCount )
	{
        if( !bFlag && IsExistFile(lpszParam) && IsSpecifiedFile(lpszParam,E_DOT_PRJ_FILE) )
		{
			_tcscpy(g_stCmd.strFileName, lpszParam);
		}
		/*xml file support by xmzhang add begin*/
        else if(!bFlag && IsExistFile(lpszParam) && IsSpecifiedFile(lpszParam,E_DOT_XPRJ_FILE) )
		{
			_tcscpy(g_stCmd.strFileName, lpszParam);
		}
		/*xml file support by xmzhang add end*/
	}
	else
	{
        if(IsSpecifiedFile(lpszParam,E_DOT_NV_FILE))			//nv
		{
			_tcscpy(g_stCmd.strImportFile,  lpszParam);
		}
        else if(IsSpecifiedFile(lpszParam,E_DOT_BIN_FILE))	//bin
        {
            if (GetFileSize(lpszParam) < MAX_DSP_CODE_SIZE)
			{
				_tcscpy(g_stCmd.strDspBinFile, lpszParam);
			}
			else
			{
				bValidPara = FALSE;
				_tprintf(_T("DspBin [%s] size can't over %dk\n"),lpszParam,MAX_DSP_CODE_SIZE/1024);
			}
			
		}
        else if(IsSpecifiedFile(lpszParam,E_DOT_TXT_FILE))	//txt
		{
			_tcscpy(g_stCmd.strErrorFile, lpszParam);
		}
		else if(
			0 == _tcsicmp(lpszParam,_T("littleendian"))	||
			0 == _tcsicmp(lpszParam,_T("le"))			||
			(bFlag && 0 == _tcsicmp(lpszParam,_T("l")) )
			)
		{
			g_stCmd.bBigEndian = FALSE;
		}
		else if( bFlag && 0 == _tcsicmp(lpszParam,_T("w")) )
		{
			g_stCmd.bWait = TRUE;
		}
		else if( bFlag && 0 == _tcsicmp(lpszParam,_T("t")) )
		{
			g_stCmd.bTail = FALSE;
		}
		// add for version info
		else if ( bFlag && 0 == _tcsicmp(lpszParam,_T("v")) )
		{
			g_stCmd.bVersion = TRUE;
		}
		
	}
	g_stCmd.nParaCount++;
	
	return bValidPara;
}

BOOL ParseCommandLine(int argc, TCHAR* argv[])
{
	BOOL bValidPara = TRUE;
	for (int i = 1; i < argc; ++i)
	{
		BOOL    bFlag = FALSE;
		if( 0 == _tcsicmp(argv[i],_T("-id")) )	//-id FileID FilePath
		{
            if( (i+2)<argc && IsExistFile(argv[i+2]) )
			{
				
                if (GetFileSize(argv[i+2]) < FILE_TOTAL_SIZE)
				{
					ST_FILE_INFO sFileInfo;
					if(
						NULL != _tcschr(argv[i+1], 'x')	||
						NULL != _tcschr(argv[i+1], 'X')
						)
					{
						sFileInfo.nFileID  = _tcstol(argv[i+1], NULL, 16);
					}
					else
					{
						sFileInfo.nFileID  = _tcstol(argv[i+1], NULL, 10);
					}													
					_tcscpy(sFileInfo.szFilePath, argv[i+2]);
					sFileInfo.bCheckFlag = TRUE;
					
					g_vecFileInfo.push_back(sFileInfo);
					i += 2;
				}
				else
				{
					bValidPara = FALSE;
					_tprintf(_T("File [%s] size can't over %dk\n"),argv[i+2],FILE_TOTAL_SIZE/1024);
					break;
				}
				
			}
			else
			{
				bValidPara = FALSE;
                _tprintf(_T("-id's parameter format is not correct. [-id FileID FilePath]\n"));
                break;
			}
		}
		else if ( 0 == _tcsicmp(argv[i],_T("-m")) )	//-m <MergeFilePath> yyy_config.h
		{
            if( (i+1)<argc && 0 <_tcslen(argv[i+1]) )
			{
				g_stCmd.bHeaderFile = TRUE;
				TCHAR* pPath = argv[i+1];
                _tcscpy(g_stCmd.strMergePath, pPath);
				int nLen = _tcslen(pPath);
				if(pPath[nLen-1]  != _T('/'))
				{
					_tcscat(g_stCmd.strMergePath, _T("/"));
				}
				
                if( (i+2)<argc && IsSpecifiedFile(argv[i+2],E_DOT_H_FILE) )
				{
					_tcscpy(g_stCmd.strHeaderFile, argv[i +2]);
					i += 2;
				}
				else
				{
					i += 1;
				}
				
			}
			else
			{
				bValidPara = FALSE;
                _tprintf(_T("-m's parameter format is not correct. [-m <MergeFilePath> [HeaderFile]]\n"));
                break;
			}
			
		}
		// add for Other customer NV update 
		else if ( 0 == _tcsicmp(argv[i],_T("-c")) )	//-c <MergeFilePath> Moderm.bin
		{
            if( (i+1)<argc && 0 <_tcslen(argv[i+1]))
            {
                g_stCmd.bHeaderFile = FALSE;
				TCHAR* pPath = argv[i+1];
                _tcscpy(g_stCmd.strMergePath, pPath);
				int nLen = _tcslen(pPath);
				if(pPath[nLen-1]  != _T('/'))
				{
					_tcscat(g_stCmd.strMergePath, _T("/"));
				}
				
                if( (i+2) < argc && IsSpecifiedFile(argv[i+2],E_DOT_BIN_FILE))
				{
					_tcscpy(g_stCmd.strHeaderFile, argv[i + 2]);
					i += 2;
				}
				else
				{
					i += 1;
				}
				
			}
			else
			{
				bValidPara = FALSE;
                _tprintf(_T("-c's parameter format is not correct. [-m <MergeFilePath> [ModemBinFile]]\n"));
                break;
			}
			
		}
		//[[ add for 7731  nv modify: merge fix bin
		else if ( 0 == _tcsicmp(argv[i],_T("-f")) )
		{
            if( (i+1)<argc && IsExistFile(argv[i+1]) )
            {
                if ( GetFileSize(argv[i+1]) < MAX_TOTAL_ITEM_CONTENT)
				{
					_tcscpy(g_stCmd.strFixBinFile, argv[i + 1]);
					i += 1;	
				}
				else
				{
					bValidPara = FALSE;
					_tprintf(_T("FixNV [%s] size can't over %dk\n"),argv[i+1],MAX_TOTAL_ITEM_CONTENT/1024);
					break;
				}
				
			}
			else
			{
				bValidPara = FALSE;
                _tprintf(_T("-f's parameter format is not correct. [-f FixNVFile]\n"));
                break;
			}
			
			
		}
		// add for hash value
		else if (0 == _tcsicmp(argv[i],_T("-h")))
		{
			// if generate hashValue headerFile, nvitem.bin must be including version info.
			if( (i+1)<argc )
			{
				g_stCmd.bVersion = TRUE;  
				_tcscpy(g_stCmd.strHashHeader, argv[i + 1]);
				i += 1;
			}
			else
			{
				bValidPara = FALSE;
                _tprintf(_T("-h's parameter format is not correct. [-h HashFile]\n"));
                break;
			}
			
		}
		// add for hash value
		else if (0 == _tcsicmp(argv[i],_T("-FastMode")))
		{
			g_stCmd.bCheckItemName = FALSE;
		}
		else 
		{
			TCHAR* pszPara = argv[i];
            if ( pszPara[0] == _T('-'))
			{
				bFlag = TRUE;
				++pszPara;
			}
			if(!ParseParam( pszPara, bFlag))
			{
				bValidPara = FALSE;
				break;
			}
		}
	}
	
	if (!bValidPara)
	{
		ShowUsage();
	}
	
	return bValidPara;
	
}

// open a nv project file and generate nvitem.bin without gui
BOOL DoCmdLine()
{
	g_bBigEndian = g_stCmd.bBigEndian;
	
	TCHAR sz_not_find_prj[]			= _T( "Not find prj/xprj bin file\r\n" );/*xml file support by xmzhang modify*/
	TCHAR sz_not_find_dsp[]			= _T( "Not find dsp bin file\r\n" );
	TCHAR sz_not_find_nv[]			= _T( "Not find nv bin file\r\n" );
	TCHAR sz_open_prj[]				= _T( "Open project...\r\n" );
	TCHAR sz_save_img[]				= _T( "Save nvitem bin file...\r\n" );
	TCHAR sz_success[]				= _T( "finished\r\n" );
	TCHAR sz_failed[]				= _T( "failed\r\n" );
	TCHAR sz_load_imp_file_failed[] = _T( "Load import file failed\r\n" );
	TCHAR sz_save_prj_failed[]		= _T( "Save project file failed\r\n" );
	
	BOOL bCheckOK = FALSE;
	do
	{
		
		if(0 == _tcslen(g_stCmd.strFileName) || -1 == _taccess(g_stCmd.strFileName, 0))
		{
			_tprintf(_T("%s : %s"), sz_not_find_prj, g_stCmd.strFileName);
			break;
		}
		
		if(0 != _tcslen(g_stCmd.strImportFile)  && -1 == _taccess(g_stCmd.strImportFile, 0))
		{
			_tprintf(_T("%s : %s"), sz_not_find_nv, g_stCmd.strImportFile);
			break;
		}
		
		if(0 != _tcslen(g_stCmd.strDspBinFile) && -1 == _taccess(g_stCmd.strDspBinFile, 0))
		{
			_tprintf(_T("%s : %s"), sz_not_find_dsp, g_stCmd.strDspBinFile);
			break;
		}
		bCheckOK = TRUE;
		
	}while(0);
	
	if(!bCheckOK)
	{
		return FALSE;
	}
	
	CFileParser* pParser	= NULL;
	BOOL bRet				= FALSE;
	BOOL bIsXMLPrj			= FALSE;
	bIsXMLPrj				= IsSpecifiedFile(g_stCmd.strFileName,E_DOT_XPRJ_FILE);
	do 
	{
		
		pParser = new CFileParser();
		if ( NULL == pParser)
		{
			_tprintf(_T("Failure to new CFileParser\n"));
			break;
		}
		_tprintf(sz_open_prj);
		
		bRet = bIsXMLPrj ? pParser->LoadProjectXML(g_stCmd.strFileName,g_stCmd.bVersion) : pParser->LoadProject(g_stCmd.strFileName,g_stCmd.bVersion);
		if(!bRet)
		{
			_tprintf(_T("%s [%s] fail.\n"),bIsXMLPrj ? _T("LoadProjectXML") : _T("LoadProject"),g_stCmd.strFileName);
			break;
		}
		pParser->ClearCalibrationFlag();
		//import file
		if( 0 != _tcslen(g_stCmd.strImportFile) && IsExistFile(g_stCmd.strImportFile))
		{
			// Import file 
			bRet = pParser->ImportItem( g_stCmd.strImportFile );
			if( !bRet )
			{
				_tprintf(_T("%s : %s"), sz_load_imp_file_failed, g_stCmd.strImportFile);
				break;
			}
			
		}
		bRet = bIsXMLPrj ? pParser->SaveProjectXML():pParser->SaveProject();
			
		if( !bRet )
		{
			_tprintf(sz_save_prj_failed);
			break;
		}
		
		pParser->SetVersionFlag(g_stCmd.bVersion);
		
		
		//[[ add for fix bin merge
		if (0 != _tcslen(g_stCmd.strFixBinFile) && IsExistFile(g_stCmd.strFixBinFile))
		{
			pParser->SetWriteFix(g_stCmd.strFixBinFile);
		}
		//]]
		
		// for hash header
		if (0 != _tcslen(g_stCmd.strHashHeader))
		{
			pParser->SetHashHeaderPath(g_stCmd.strHashHeader);
		}
		
		
		if(0 !=  _tcslen(g_stCmd.strDspBinFile))
		{
			pParser->SetWriteDsp(TRUE);
		}
		else
		{
			pParser->SetWriteDsp(FALSE);
		}
		
		if (g_vecFileInfo.size()>0)
		{
			CUIntArray uIntArray;
			pParser->SetMulFileAg(TRUE, g_vecFileInfo);
			if( pParser->IsExistedID(uIntArray) )
			{
				string strErrMsg("The following ID had existed,please input news:\n");
				for (int i = 0; i < uIntArray.size(); i++)
				{
					_tprintf( _T("FileID %d existed \n"), uIntArray[i]);
					
				}	
				break;
			}
		}
		
		//_tprintf(sz_save_img);
		pParser->SetWriteTail(g_stCmd.bTail);
		_tprintf(sz_save_img);
		
		bRet = pParser->SaveToImage(g_stCmd.strDspBinFile);
		
		//[[ support for SamSung NV update by Joly
		if(0 !=  _tcslen(g_stCmd.strMergePath))
		{
			if (g_stCmd.bHeaderFile)
			{
				if (0 == _tcslen(g_stCmd.strHeaderFile))
				{
					_tcscpy(g_stCmd.strHeaderFile, g_stCmd.strMergePath);
					_tcscat(g_stCmd.strHeaderFile, _T("ss_nv_cofigure.h"));
					
				}
			}
			
			bRet = pParser->GenerateConfigFile(g_stCmd.strMergePath, g_stCmd.strHeaderFile, g_stCmd.bHeaderFile);
			if (!bRet)
			{
				break;
			}
			_tprintf(_T("Merge Configure NV files  %s finished\n"),  g_stCmd.strHeaderFile);
			
			
		}
		
		if( bRet )
		{
			_tprintf(sz_success);
		}
		else
		{
			_tprintf(sz_failed);
		}
		
		bRet = TRUE;
		
	} while(0);
	
	SAFE_DELETE(pParser);
	
	return bRet;
}
int main(int argc, TCHAR* argv[])
{
	_tprintf(VERSION_INFO);
	if (argc <= 1)
	{
		ShowUsage();
		return 1;
	}
	if (!ParseCommandLine(argc, argv))
	{
		return 1;
	}	
	
    BOOL bRet = !DoCmdLine();
	return bRet;
}
