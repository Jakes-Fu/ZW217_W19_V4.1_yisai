// stdafx.cpp : source file that includes just the standard includes
//	CityMap.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


int EnumKeys(LPCTSTR pSection,CStringArray* pKeys)
{
    ASSERT(pSection != NULL);
    ASSERT(pKeys != NULL);
	if(pKeys == NULL ||  pSection == NULL)
	{
		return 0;
	}
	
    int number = 0;
    for(;;)
    {
        CString strLine = pSection;
        int nLen = strLine.GetLength();
        if(0 == nLen)
        {
            // All keys are read
            break;
        }
        
        int nIndex = strLine.Find(_T('='));
        if(-1 == nIndex)
        {
            // Not a valid key
            continue;
        }
        CString strKey = strLine.Left(nIndex);
        strKey.TrimLeft();
        strKey.TrimRight();
        pKeys->Add(strKey);
		
        CString strData = strLine.Right(nLen - nIndex - 1);
        strData.TrimLeft();
        strData.TrimRight();
        pKeys->Add(strData);
		
        number++;
		
        pSection += nLen + 1;
		
    }
	
    return number;
}

