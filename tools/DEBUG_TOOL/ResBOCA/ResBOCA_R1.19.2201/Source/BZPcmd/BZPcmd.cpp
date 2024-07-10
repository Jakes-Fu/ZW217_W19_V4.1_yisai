// BZPcmd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "BZPcmd.h"
#include "../bzp/BZip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;
/*
#define	MAX_EXT_NUM		24
const TCHAR EXT_KEY[]	= _T("-ext");
const TCHAR EXTZ_KEY[]	= _T("-extz");
*/

void ShowUsage()
{
	cout << _T("usage:") << endl;
	cout << _T("bzpcmd <-code file> <-res file> [-lang file] [-kernz file]<-out file> [-cmp [a|b|c|b2|b3]] [-level n] [-maxram data] [-maxbzp data] [-L]") << endl;
	cout << _T("bzpcmd <-cat> <-ps file>  <-res file> [-dsp file] [-kern[z] file]<-out file> [-maxram data] [-maxbzp data] [-L]") << endl;
	cout << _T("bzpcmd <-cat> <-ps file> <-bzp file> [-dsp file] [-kern[z] file]<-out file> [-maxram data] [-maxbzp data] [-L]") << endl;
	cout << _T("bzpcmd <-cat> <-ps file> [-dsp[z] file] [-kern[z] file]<-2ndBzp> <-code file> [-res file] [-lang file] <-out file> [-cmp [a|b|c|b2|b3]] [-level n] [-maxram data] [-maxbzp data] [-L]") << endl;
	cout << _T("bzpcmd <-cat2> <-usr file> <-res file> <-out file> [-usrpacsize data] [-respacsize data] [-usrcmp [a|b|c|b2|b3]] [-rescmp [a|b|c|b2|b3]]  [-maxram data] [-maxbzp data] [-L]") << endl;
	cout << _T("bzpcmd <-raw> <-file file> <-out file> [-cmp [a|b|c|b2|b3]] [-level n]");
	cout << _T("data: must be a digit number, it maybe plus M or K, such as 2M or 200K") << endl;
}

int IsExtKeyType(LPCTSTR szInKey, BOOL bExtKey)
{
	int nIndex = -1;
	for (int i = 0; i < MAX_EXT_NUM;++i)
	{
		CString strKey;
		bExtKey ? strKey.Format(_T("%s%d"),EXT_KEY,i) : strKey.Format(_T("%s%d"),EXTZ_KEY,i);
		
		if(_tcsicmp(szInKey,strKey.operator LPCTSTR())==0)
		{
			nIndex = i;
			break;
		}
	}
	return nIndex;
}

BOOL CheckExtFiles(CStringArray &agFile)
{
	CFileFind finder;
	BOOL bOK = TRUE;
	for(int i = 0; i< (agFile.GetSize()/2); i++)
	{	
		if(!finder.FindFile(agFile[2*i+1]))
		{
			bOK = FALSE;
			_tprintf(_T("\ncannot find ext file [%s]!\n"),agFile[2*i+1]);
		}
		finder.Close();
	}
	return bOK;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;
	CString strPSFile;
	CString strCodeFile;
	CString strResFile;
	CString strBzpFile;
	CString strOutFile;
	CString strDspFile;
	CString strDspZFile; //@polojiang added on 20111214
	CString strUsrFile;
	CString strFile;// any type file.
	CString strKernFile;
	CString strKernZFile;

	CStringArray agExtFiles;
	CString strKey;

	BZP_PARAM_T param = {0};
	param.dwMaxBzp = 0;
	param.dwMaxRam = 0;
	param.bBigEndian = TRUE;
	param.bCat = FALSE;
	param.nCmpType = BZP_ZIP;
	param.nLevel = -1;
	param.b2ndBzp = FALSE;
	param.bCat2 = FALSE;
	param.nUsrCmpType = -1;
	param.nUsrLevel = -1;
	param.dwUsrPacSize = 4*1024;
	param.nResCmpType = -1;
	param.nResLevel = -1;
	param.dwResPacSize = 4*1024;
	param.bRawCmpData = FALSE;

	CStringArray agLangFile;

	DWORD  dwMulti  = 0;
	int i=0;
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		CBZip bz;
		int nIndex = -1;
		
		//ShowUsage();
		for(i= 1; i<  argc; i++)
		{
 			if(_tcsicmp(argv[i],_T("-cat"))==0)
			{
				param.bCat = TRUE;
			}
			if(_tcsicmp(argv[i],_T("-raw"))==0)
			{
				param.bRawCmpData = TRUE;
			}
			if(_tcsicmp(argv[i],_T("-cat2"))==0)
			{
				param.bCat2 = TRUE;
			}
			else if(_tcsicmp(argv[i],_T("-2ndBzp"))==0)
			{
				param.b2ndBzp = TRUE;
			}
			else if(_tcsicmp(argv[i],_T("-file"))==0)
			{
				if((i+1)<argc)
				{
					strFile = argv[++i];
				}
			}
			else if(_tcsicmp(argv[i],_T("-ps"))==0)
			{
				if((i+1)<argc)
				{
					strPSFile = argv[++i];
				}
			}
			else if(_tcsicmp(argv[i],_T("-usr"))==0)
			{
				if((i+1)<argc)
				{
					strUsrFile = argv[++i];
				}
			}
			else if(_tcsicmp(argv[i],_T("-code"))==0)
			{
				if((i+1)<argc)
				{
					strCodeFile = argv[++i];
				}
			}
			else if(_tcsicmp(argv[i],_T("-res"))==0)
			{
				if((i+1)<argc)
				{
					strResFile = argv[++i];
				}
			}
			else if(_tcsicmp(argv[i],_T("-dsp"))==0)
			{
				if((i+1)<argc)
				{
					strDspFile = argv[++i];
				}
			}
			else if(_tcsicmp(argv[i],_T("-dspz"))==0)//@polojiang added on 20111214
			{
				if((i+1)<argc)
				{
					strDspZFile = argv[++i];
				}
			}//@end polojiang added on 20111214
			else if(_tcsicmp(argv[i],_T("-lang"))==0)
			{
				if((i+1)<argc)
				{
					CString strLang = argv[++i];
					agLangFile.Add(strLang);
				}
			}
			else if(_tcsicmp(argv[i],_T("-bzp"))==0)
			{
				if((i+1)<argc)
				{
					strBzpFile = argv[++i];
				}
			}
			else if(_tcsicmp(argv[i],_T("-kern"))==0)
			{
				if((i+1)<argc)
				{
					strKernFile = argv[++i];
				}
			}
			else if(_tcsicmp(argv[i],_T("-kernz"))==0)
			{
				if((i+1)<argc)
				{
					strKernZFile = argv[++i];
				}
			}
			else if(-1 != (nIndex = IsExtKeyType(argv[i],TRUE)) )//@polojiang added on 20121121
			{
				if((i+1)<argc)
				{
					strKey.Format(_T("%s%d"),EXT_KEY,nIndex);
					agExtFiles.Add(strKey);
					agExtFiles.Add(argv[++i]);
				}
			}
			else if(-1 != (nIndex = IsExtKeyType(argv[i],FALSE)) ) //"EXTZ"
			{
				if((i+1)<argc)
				{
					strKey.Format(_T("%s%d"),EXTZ_KEY,nIndex);
					agExtFiles.Add(strKey);
					agExtFiles.Add(argv[++i]);
				}
			}//@end polojiang added on 20121121
			else if(_tcsicmp(argv[i],_T("-out"))==0)
			{
				if((i+1)<argc)
				{
					strOutFile = argv[++i];
				}
			}
			else if(_tcsicmp(argv[i],_T("-cmp"))==0)
			{
				if((i+1)<argc)
				{
					if(_tcsicmp(argv[i+1],_T("a"))==0)
					{
						param.nCmpType = BZP_ZIP;
						i++;
					}
					else if(_tcsicmp(argv[i+1],_T("b"))==0)
					{
						param.nCmpType = BZP_7Z;
						i++;
					}
					else if(_tcsicmp(argv[i+1],_T("b2"))==0)
					{
						param.nCmpType = BZP_7ZF;
						i++;
					}					
					else if(_tcsicmp(argv[i+1],_T("c"))==0)
					{
						param.nCmpType = BZP_RAW;
						i++;
					}
					else if(_tcsicmp(argv[i+1],_T("b3"))==0)
					{
						param.nCmpType = BZP_7Z_B3;
						i++;
					}
					else
					{
						_tprintf(_T("\nunkonw cmp type [%s]!\n"),argv[i+1]);
						ShowUsage();
						return 1;
					}
				}
			}
			else if(_tcsicmp(argv[i],_T("-usrcmp"))==0)
			{
				if((i+1)<argc)
				{
					if(_tcsicmp(argv[i+1],_T("a"))==0)
					{
						param.nUsrCmpType = BZP_ZIP;
						i++;
					}
					else if(_tcsicmp(argv[i+1],_T("b"))==0)
					{
						param.nUsrCmpType = BZP_7Z;
						i++;
					}
					else if(_tcsicmp(argv[i+1],_T("b2"))==0)
					{
						param.nUsrCmpType = BZP_7ZF;
						i++;
					}					
					else if(_tcsicmp(argv[i+1],_T("c"))==0)
					{
						param.nUsrCmpType = BZP_RAW;
						i++;
					}
					else if(_tcsicmp(argv[i+1],_T("b3"))==0)
					{
						param.nUsrCmpType = BZP_7Z_B3;
						i++;
					}
					else
					{
						_tprintf(_T("\nunkonw cmp type [%s]!\n"),argv[i+1]);
						ShowUsage();
						return 1;
					}
				}
			}
			else if(_tcsicmp(argv[i],_T("-rescmp"))==0)
			{
				if((i+1)<argc)
				{
					if(_tcsicmp(argv[i+1],_T("a"))==0)
					{
						param.nResCmpType = BZP_ZIP;
						i++;
					}
					else if(_tcsicmp(argv[i+1],_T("b"))==0)
					{
						param.nResCmpType = BZP_7Z;
						i++;
					}
					else if(_tcsicmp(argv[i+1],_T("b2"))==0)
					{
						param.nResCmpType = BZP_7ZF;
						i++;
					}					
					else if(_tcsicmp(argv[i+1],_T("c"))==0)
					{
						param.nResCmpType = BZP_RAW;
						i++;
					}
					else if(_tcsicmp(argv[i+1],_T("b3"))==0)
					{
						param.nResCmpType = BZP_7Z_B3;
						i++;
					}
					else
					{
						_tprintf(_T("\nunkonw cmp type [%s]!\n"),argv[i+1]);
						ShowUsage();
						return 1;
					}
				}
			}
			else if(_tcsicmp(argv[i],_T("-level"))==0)
			{
				if((i+1)<argc)
				{
					param.nLevel = _ttoi(argv[++i]);
				}
			}
			else if(_tcsicmp(argv[i],_T("-usrl"))==0)
			{
				if((i+1)<argc)
				{
					param.nUsrLevel = _ttoi(argv[++i]);
				}
			}
			else if(_tcsicmp(argv[i],_T("-resl"))==0)
			{
				if((i+1)<argc)
				{
					param.nResLevel = _ttoi(argv[++i]);
				}
			}
			else if(_tcsicmp(argv[i],_T("-usrpacsize"))==0)
			{
				if((i+1)<argc)
				{
					dwMulti = 1;
					if(_tcsrchr(argv[i+1],'k') != NULL || _tcsrchr(argv[i+1],'K') != NULL)
					{
						dwMulti = 1024;
					}
					if(_tcsrchr(argv[i+1],'m') != NULL || _tcsrchr(argv[i+1],'M') != NULL)
					{
						dwMulti = 1024*1024;
					}
					param.dwUsrPacSize = _ttoi(argv[++i]);
					param.dwUsrPacSize *= dwMulti;
				}
			}
			else if(_tcsicmp(argv[i],_T("-respacsize"))==0)
			{
				if((i+1)<argc)
				{
					dwMulti = 1;
					if(_tcsrchr(argv[i+1],'k') != NULL || _tcsrchr(argv[i+1],'K') != NULL)
					{
						dwMulti = 1024;
					}
					if(_tcsrchr(argv[i+1],'m') != NULL || _tcsrchr(argv[i+1],'M') != NULL)
					{
						dwMulti = 1024*1024;
					}
					param.dwResPacSize = _ttoi(argv[++i]);
					param.dwResPacSize *= dwMulti;
				}
			}
			else if(_tcsicmp(argv[i],_T("-maxram"))==0)
			{
				if((i+1)<argc)
				{
					dwMulti = 1;
					if(_tcsrchr(argv[i+1],'k') != NULL || _tcsrchr(argv[i+1],'K') != NULL)
					{
						dwMulti = 1024;
					}
					if(_tcsrchr(argv[i+1],'m') != NULL || _tcsrchr(argv[i+1],'M') != NULL)
					{
						dwMulti = 1024*1024;
					}
					param.dwMaxRam = _ttoi(argv[++i]);
					param.dwMaxRam *= dwMulti;
				}
			}
			else if(_tcsicmp(argv[i],_T("-maxbzp"))==0)
			{
				if((i+1)<argc)
				{
					dwMulti = 1;
					if(_tcsrchr(argv[i+1],'k') != NULL || _tcsrchr(argv[i+1],'K') != NULL)
					{
						dwMulti = 1024;
					}
					if(_tcsrchr(argv[i+1],'m') != NULL || _tcsrchr(argv[i+1],'M') != NULL)
					{
						dwMulti = 1024*1024;
					}
					param.dwMaxBzp = _ttoi(argv[++i]);
					param.dwMaxBzp *= dwMulti;
				}
			}
			else if(_tcsicmp(argv[i],_T("-L"))==0)
			{
				param.bBigEndian = FALSE;
			}
		}		

		if(strFile.IsEmpty() && strUsrFile.IsEmpty() && strCodeFile.IsEmpty() && 
			strPSFile.IsEmpty() && strResFile.IsEmpty() &&strDspZFile.IsEmpty()&& 
			agLangFile.GetSize()==0 && strKernFile.IsEmpty()&& strKernZFile.IsEmpty())
		{
			_tprintf(_T("\nps,code, res and lang are empty!\n"));
			ShowUsage();
			return 1;
		}

		if(strOutFile.IsEmpty())
		{		
			_tprintf(_T("\nout file can not be empty!\n"));	
			ShowUsage();
			return 1;
		}

	//	if(param.bCat && (  strPSFile.IsEmpty() || (strResFile.IsEmpty() && strBzpFile.IsEmpty())))
		if(param.bCat && !param.b2ndBzp &&( strPSFile.IsEmpty() || 
			( strResFile.IsEmpty() && strBzpFile.IsEmpty() && 
			  strDspFile.IsEmpty() && strDspZFile.IsEmpty() && 
			  strKernFile.IsEmpty() && strKernZFile.IsEmpty())) )
		{
			_tprintf(_T("\nps,res or bzp file can not be empty!\n"));	
			ShowUsage();
			return 1;
		}

		if (param.b2ndBzp && (strCodeFile.IsEmpty() && strResFile.IsEmpty()) )
		{
			_tprintf(_T("\ncode or res can not be empty!\n"));	
			ShowUsage();
			return 1;
		}
	

		CFileFind finder;
		BOOL bOK = TRUE;
		CStringArray agFile;

		bOK = CheckExtFiles(agExtFiles);

		if(!strPSFile.IsEmpty() )
		{
			if(!finder.FindFile(strPSFile))
			{
				bOK = FALSE;
				_tprintf(_T("\ncannot find ps file [%s]!\n"),strPSFile);
			}
			else
			{
				agFile.Add(_T("ps"));
				agFile.Add(strPSFile);
			}
		}

		if(!strFile.IsEmpty() )
		{
			if(!finder.FindFile(strFile))
			{
				bOK = FALSE;
				_tprintf(_T("\ncannot find file [%s]!\n"),strFile);
			}
			else
			{
				agFile.Add(_T("file"));
				agFile.Add(strFile);
			}
		}
		
		if(!strUsrFile.IsEmpty() )
		{
			if(!finder.FindFile(strUsrFile))
			{
				bOK = FALSE;
				_tprintf(_T("\ncannot find usr file [%s]!\n"),strUsrFile);
			}
			else
			{
				agFile.Add(_T("usr"));
				agFile.Add(strUsrFile);
			}
		}
	
		if(!strResFile.IsEmpty() )
		{
			if(!finder.FindFile(strResFile))
			{
				bOK = FALSE;
				_tprintf(_T("\ncannot find res file [%s]!\n"),strResFile);
			}
			else
			{
				agFile.Add(_T("res"));
				agFile.Add(strResFile);
			}
		}

		if(!strCodeFile.IsEmpty() )
		{
			if(!finder.FindFile(strCodeFile))
			{
				bOK = FALSE;
				_tprintf(_T("\ncannot find code file [%s]!\n"),strCodeFile);
			}
			else
			{
				agFile.Add(_T("code"));
				agFile.Add(strCodeFile);
			}
		}

		if(!strKernFile.IsEmpty() )
		{
			if(!finder.FindFile(strKernFile))
			{
				bOK = FALSE;
				_tprintf(_T("\ncannot find kernel file [%s]!\n"),strKernFile);
			}
			else
			{
				agFile.Add(_T("kern"));
				agFile.Add(strKernFile);
			}
		}

		if(!strKernZFile.IsEmpty() )
		{
			if(!finder.FindFile(strKernZFile))
			{
				bOK = FALSE;
				_tprintf(_T("\ncannot find kernelz file [%s]!\n"),strKernZFile);
			}
			else
			{
				agFile.Add(_T("kernz"));
				agFile.Add(strKernZFile);
			}
		}
		
		if(!strDspFile.IsEmpty() )
		{
			if(!finder.FindFile(strDspFile))
			{
				bOK = FALSE;
				_tprintf(_T("\ncannot find dsp file [%s]!\n"),strDspFile);
			}
			else
			{
				agFile.Add(_T("dsp"));
				agFile.Add(strDspFile);
			}
		}
		if(!strDspZFile.IsEmpty() )//@polojiang added on 20111214
		{
			if(!finder.FindFile(strDspZFile))
			{
				bOK = FALSE;
				_tprintf(_T("\ncannot find dspz file [%s]!\n"),strDspZFile);
			}
			else
			{
				agFile.Add(_T("dspz"));
				agFile.Add(strDspZFile);
			}
		}//@end polojiang added on 20111214
		for(i=0; i< agLangFile.GetSize(); i++)
		{
			if(!finder.FindFile(agLangFile[i]))
			{
				bOK = FALSE;
				_tprintf(_T("\ncannot find lang file [%s]!\n"),agLangFile[i]);
			}
			else
			{
				agFile.Add(_T("lang"));
				agFile.Add(agLangFile[i]);
			}
		}

		if(!strBzpFile.IsEmpty() )
		{
			if(!finder.FindFile(strBzpFile))
			{
				bOK = FALSE;
				_tprintf(_T("\ncannot find bzp file [%s]!\n"),strBzpFile);
			}
			else
			{
				agFile.Add(_T("bzp"));
				agFile.Add(strBzpFile);
			}
		}		

		if(finder.FindFile(strOutFile))
		{
			DWORD dwAtt = GetFileAttributes(strOutFile);
			dwAtt &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(strOutFile,dwAtt);
		}



		if(!bOK)
		{
			ShowUsage();
			nRetCode = 1;
		}
		else
		{			
			bz.SetLimitParam(param);

			BOOL bRet = agExtFiles.GetSize()?bz.GenBzpEx(agFile,agExtFiles,strOutFile):bz.GenBzp(agFile,strOutFile);
			//if(bz.GenBzp(agFile,strOutFile))
			if (bRet)
			{
				nRetCode = 0;
				_tprintf(_T("\ngenerate bzp file success!\n\n"));
			}
			else
			{
				nRetCode = 1;
				_tprintf(_T("\ngenerate bzp file failed!\n\n"));
			}
#ifdef _DEBUG
			TCHAR ch = getchar();
#endif 
		}
	
	}

	return nRetCode;
}


//BZPcmd.exe -cat -ps D:\testBZP\PS_29.bin -dspz D:\testBZP\dsp.bin -ext1 d:\testBZP\res.bin -extz3 d:\testBZP\res2.bin -out D:\testBZP\new_EX.bin -B -cmp b -level 2
//BZPcmd.exe -cat -ps D:\testBZP\PS_29.bin -dspz D:\testBZP\dsp.bin -out D:\testBZP\new_EX=FFFFFFFF.bin -B -cmp b -level 2

//BZPcmd.exe -cat -ps D:\testBZP\PS_5.bin -dspz D:\testBZP\dsp.bin -out D:\testBZP\OLD_RE-FF.bin -B -cmp b -level 2