/*==================================================================
=  文件名:CIniFile类实现文件                                       =
=  修改日期:2002-12-28                                             = 
=  作者:阿皮                                                       =  
=   E_Mail:peijikui@sd365.com  msn:peijikui@sina.com                                                              = 
====================================================================*/
#include "stdafx.h"
#include "CIniFile.h"

CIniFile::CIniFile():m_sfFile()
{
    m_bInitialized = FALSE;
}


CIniFile::~CIniFile()
{
    if (m_bInitialized)
    {
	    Close();
    }
}

BOOL CIniFile::OpenIniFile( LPCTSTR lpszFileName, 
                    CFileException* pError /*= NULL*/ )
{
    //maybe another file is open, so close it.
    if (m_bInitialized)
    {
            m_sfFile.Close();
    }
    m_csFile = lpszFileName;
    if (!m_sfFile.Open(lpszFileName, CFile::modeRead | CFile::shareDenyWrite, pError))
    {
           char szMsgError[512];
           pError->GetErrorMessage(szMsgError, sizeof(szMsgError));
        return FALSE;
    }
    else
    {
        m_bInitialized = TRUE;
        m_bModified = FALSE;
        CString strFileLine;      
        m_sfFile.SeekToBegin();
        while (TRUE)
        {
            try
            {                
                if (m_sfFile.ReadString(strFileLine) == FALSE)
                {
                    break;
                }  
                //note strFileLine does not contain the last '\n';
    		    m_aFileContainer.Add(strFileLine);
            }
            catch(CFileException *pe)
            {
                char szMsgError[512];
                pe->GetErrorMessage(szMsgError, sizeof(szMsgError));
            }            
        }
        m_sfFile.Close();
    }

    return TRUE;

}


BOOL CIniFile::Flush()
{
    if (m_bInitialized && m_bModified)
    {
        CFileException feFile;
        if (!m_sfFile.Open(m_csFile, 
                            CFile::modeCreate | CFile::modeWrite | CFile::shareDenyWrite, 
                            &feFile))
        {
            char szMsgError[512];
            feFile.GetErrorMessage(szMsgError, sizeof(szMsgError));
            return FALSE;
        }
        else
        {
            int i = 0;
            int nLineNum = m_aFileContainer.GetSize();
            while (i < nLineNum)
            {
                m_sfFile.WriteString(m_aFileContainer.GetAt(i) + "\n");
                i++;
            }
            m_bModified = FALSE;
            m_sfFile.Close();
        }
    }
    return TRUE;
}

void CIniFile::Close()
{
    if (m_bInitialized)
    {
        if (m_bModified)
        {
           Flush();
        }
        m_aFileContainer.RemoveAll();
        m_bInitialized = FALSE;
    }

}


BOOL CIniFile::GetVar(const CString & strSection,
                      const CString & strVarName,
                      CString &strReturnValue)
{
    if (!m_bInitialized)
    {
        return FALSE;
    }
	if (m_aFileContainer.GetSize() < 0)
    {
        return FALSE;
    }
	
    int nLine = SearchLine(strSection, strVarName);
    if (nLine <= 0)
    {
        return FALSE;
    }
   	
    CString	strParam = m_aFileContainer[nLine - 1];    
    strReturnValue = strParam.Mid(strParam.Find("=") + 1);
    strReturnValue.TrimLeft();
    strReturnValue.TrimRight();
    
    return TRUE;

}

BOOL CIniFile::GetVarStr(const CString & strSection,
                         const CString & strVarName,
                         CString &strReturnValue)
{
	return (GetVar(strSection, strVarName, strReturnValue));
}
BOOL CIniFile::GetVarInt(const CString & strSection,const CString & strVarName,int & nValue)
{
    CString  strReturnVar;

    if (!GetVar(strSection, strVarName, strReturnVar))
    {
        return FALSE;
    }

    strReturnVar.TrimLeft();
    strReturnVar.TrimRight();
    int nLen = strReturnVar.GetLength();
    nValue = atoi(strReturnVar.GetBuffer(nLen));
    return TRUE;
    
}

BOOL CIniFile::SetVar(const CString & strSection,
                      const CString & strVarName,
                      const CString & strVar,
                      const int iType)
{
    if (!m_bInitialized)
    {
        return FALSE;
    }
	
    CString strSectionLow;
    CString strVarNameLow;

    strSectionLow = strSection;
    strVarNameLow = strVarName;
    strSectionLow.MakeLower();
    strVarNameLow.MakeLower();

	int i = 0;
	int iFileLines = m_aFileContainer.GetSize();
	
    while (i< iFileLines)
	{
	
		CString		strValue = m_aFileContainer.GetAt(i++);
        CString     strValueLow = strValue;
        strValueLow.MakeLower();
		strValueLow.TrimLeft();
		if ((strValueLow.Find("[") >= 0) && 
            (strValueLow.Find("]") >= 0) &&
            (strValueLow.Find(strSectionLow) >= 0))
		{	
			while (i < iFileLines)
			{
			
				CString strSectionListLow = m_aFileContainer[i++];
                strSectionListLow.MakeLower();
				strSectionListLow.TrimLeft();
				if (strSectionListLow.Find("#") >=0)//找到注释行
					continue;
				
				if (strSectionListLow.Find(strVarNameLow)>=0)//找到
				{
				
					CString strParam = strVarName + "=" + strVar;
					m_aFileContainer[i-1] = strParam;
					return TRUE;
				}
				if (strSectionListLow.Find("[",0)>=0)//在原来文件的SECTION中,没有相应的变量需要添加而且,这种情况下,下边还有别的section
				{
					//处理流程是这样的,首先把当前的数值依次向后移动,然后在当前位置加入新出现的数值
					if (iType != 0)
						return FALSE;
					CString strParam;
					m_aFileContainer.Add(strParam);
					int iPre = m_aFileContainer.GetSize() - 1;
					while (iPre >= i)
					{
						CString strBehind = m_aFileContainer[iPre -1];
						m_aFileContainer[iPre] = strBehind;
						iPre --;

					}
					strParam = strVarName + "=" + strVar + "\n";
					m_aFileContainer.SetAt(i-1,strParam);
					return TRUE;
				}
				if (i == iFileLines && iType == 0)
				{
					m_aFileContainer.Add(strVarName + "=" + strVar + "\n");
					return TRUE;
				}
			}
		}
	}

	if (iType == 0)
	{
		m_aFileContainer.Add("[" + strSection + "]" + "\n\n");
		m_aFileContainer.Add(strVarName + "=" + strVar + "\n");
        return TRUE;
	}
	
	return FALSE;

}
BOOL CIniFile::SetVarStr(const CString & strSection,const CString & strVarName,const CString & strValue,const int iType)
{
    BOOL bResult = SetVar(strSection,strVarName,strValue,iType);

    if (bResult)
    {
        m_bModified = TRUE;
    }

	return bResult;
}
BOOL CIniFile::SetVarInt(const CString & strSection,const CString & strVarName,const int & iValue,const int iType)
{
	CString strValue;
	strValue.Format("%d",iValue);
    
    BOOL bResult = SetVar(strSection,strVarName,strValue,iType);

    if (bResult)
    {
        m_bModified = TRUE;
    }

	return bResult;

}
int CIniFile::SearchLine(const CString & strSection,const CString & strVarName)
{

    CString strSectionLow = strSection;
    CString strVarNameLow = strVarName;
    strSectionLow.MakeLower();
    strVarNameLow.MakeLower();


	if (m_aFileContainer.GetSize() > 0)
	{
			
		int i = 0;
		int iFileLines = m_aFileContainer.GetSize();
		while (i< iFileLines)
		{
			
			CString strValue = m_aFileContainer[i++];
            strValue.MakeLower();
			strValue.TrimLeft();
			if (strValue.Find("[") >= 0 && 
                strValue.Find("]") >= 0 &&
                strValue.Find(strSectionLow,1) >= 0 
                )
			{	
				while (i < iFileLines)
				{
				
					CString strSectionListLow = m_aFileContainer[i++];
                    strSectionListLow.MakeLower();
					strSectionListLow.TrimLeft();
					if (strSectionListLow.Find("#") >= 0)//找到注释行
						continue;
					if (strSectionListLow.Find(strVarNameLow) >= 0)//找到
					{
						return i;
					}
					if (strSectionListLow.Find("[",0) >= 0)//另外一个段落出现,寻找失败
					{
						return -2;
					}
					
				}
			}
		}
	}
	return -1;
}