#include "stdafx.h"
#include "Function.h"

/*********************************************************/
void GetAppPath(TCHAR *AppPath)
{
    //Get the current running path
	TCHAR Path[MAX_PATH];
    TCHAR *pFoundChar,*pNextChar;

	GetModuleFileName(NULL,Path,MAX_PATH);

    pFoundChar=pNextChar=Path;
	while(pFoundChar)
	{
		//Search the first char '\'
		pFoundChar = _tcschr(pNextChar+1,'\\');
	    //find the char '\'
		if(pFoundChar != NULL)
		{
           //move to the next char pointer to the current address 
             pNextChar = pFoundChar;
			 continue;
		}
	}
 
	//escape the program name 
	*pNextChar = '\0';

	_tcscpy(AppPath,Path);
}
