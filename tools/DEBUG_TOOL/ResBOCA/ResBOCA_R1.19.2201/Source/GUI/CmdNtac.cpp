#include "stdafx.h"
#include "CmdNtac.h"

CCmdNtac::CCmdNtac(void)
{
}

CCmdNtac::~CCmdNtac(void)
{
}
BOOL CCmdNtac::GenNtac(const NTAC_Args_T& tArgs)
{
	if (LoadNtacFile(tArgs))
	{
		return SaveNtacFile(tArgs);
	}
	else
	{
		return FALSE;
	}
}

BOOL CCmdNtac::LoadNtacFile(const NTAC_Args_T& tArgs)
{

    BOOL bOK = FALSE;
	if (E_EXECL == tArgs.eInType) //Import to excel file
	{
		UINT nRet = 0;
		nRet = m_Parser.ParseNtacSheet(tArgs.szInFile);
		switch(nRet)
		{
		case E_OK:
			bOK = TRUE;
			break;
		case E_DEC_XLS_ERR:
			_tprintf(_T("Parsed NtacSheet excel file failed!\n"));
			break;
		default:
			_tprintf(_T("Invalid NtacSheet excel file!\n"));
			break;
		}
	}
	else
	{
		FILE *pFile = NULL;
		LPBYTE pBuf = NULL;
		do 
		{
			pFile = _tfopen(tArgs.szInFile,_T("rb"));
			if(pFile == NULL)
			{
				_tprintf(_T("Open file \"%s\" failed!\n"),tArgs.szInFile);
				break;
			}
			fseek(pFile,0,SEEK_END);
			DWORD dwSize = ftell(pFile);
			fseek(pFile,0,SEEK_SET);
			if(dwSize == 0)
			{
				_tprintf(_T("File size is zero!\n"));
				break;
			}
			pBuf = new BYTE[dwSize];
			if(dwSize == NULL)
			{
				_tprintf(_T("New buffer fail!\n"));
				break;
			}
			DWORD dwRead = fread(pBuf,1,dwSize,pFile);
			if(dwRead != dwSize)
			{
				_tprintf(_T("Read file failed!\n"));
				break;
			}

			bOK = m_Parser.DecodeNTAC(pBuf,dwSize);
			if(!bOK)
			{
				_tprintf(_T("DecodeNTAC failed,Please check the file format!\n"));
			}
		} while (0);
		
		if(pFile)
		{
			fclose(pFile);
		}
		if (pBuf)
		{
			delete[] pBuf;	
		}	
	}

	return bOK;
}

BOOL CCmdNtac::SaveNtacFile(const NTAC_Args_T& tArgs)
{
	BOOL bOK = FALSE;

	if (E_EXECL == tArgs.eOutType) //Export to excel file
	{
		bOK = m_Parser.CreateNtacSheet(tArgs.szOutFile);
	}
	else
	{
		FILE *pFile = NULL;
		LPBYTE pBuf = NULL;
		DWORD  dwSize = 0;
		do 
		{
			if (!m_Parser.EncodeNTAC(pBuf,dwSize))
			{
				_tprintf(_T("EncodeNTAC failed!\n"));
				break;
			}
			pFile = _tfopen(tArgs.szOutFile,_T("wb"));
			if(pFile == NULL)
			{
				_tprintf(_T("Create file failed!\n"));
				break;
			}
			DWORD dwWirte = fwrite(pBuf,1,dwSize,pFile);
			if(dwWirte != dwSize)
			{
				_tprintf(_T("Save data failed!\n"));
				break;
			}
			bOK = TRUE;
		} while (0);
		if (pFile)
		{
			fclose(pFile);
			pFile = NULL;
		}

		if (pBuf)
		{
			delete[] pBuf;
			pBuf = NULL;
		}
	}
	if (bOK)
	{
		_tprintf(_T("Export complete!\n"));
	}

	return bOK;
}