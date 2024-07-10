// OtherPicFormatToBMP.cpp: implementation of the COtherPicFormatToBMP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OtherPicFormatToBMP.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COtherPicFormatToBMP::COtherPicFormatToBMP( )
:m_IPicture(NULL),m_hBMP(NULL)
{

}

COtherPicFormatToBMP::~COtherPicFormatToBMP()
{
    int iCount = m_allResource.size();
    for( int i=0; i<iCount; ++i )
    {
        m_allResource[i]->Release();
        m_allResource.clear();
    }    
}
//convert to BMP format
COtherPicFormatToBMP::operator HBITMAP()const 
{
    ASSERT( m_IPicture );
    m_IPicture->get_Handle((unsigned int*) &m_hBMP );
    ASSERT( m_hBMP );
    
    return m_hBMP;
}
// load pic resource from resource. 
//can also load from file, not support now
void COtherPicFormatToBMP::Init(UINT uintResID, LPCSTR strResType)
{
    if( m_IPicture )
     {
         m_IPicture = NULL;
     }
#ifdef _UNICODE
	int nLen = strlen(strResType);
	TCHAR* pszTmp = new TCHAR[nLen * 2];
	MultiByteToWideChar(CP_ACP,0,strResType,-1,pszTmp,nLen*2);

    HRSRC hRes = FindResource( AfxGetResourceHandle(),
        MAKEINTRESOURCE(uintResID),
        pszTmp
        );
	if(pszTmp != NULL)
	{
		delete [] pszTmp;
		pszTmp = NULL;
	}
#else
	 HRSRC hRes = FindResource( AfxGetResourceHandle(),
        MAKEINTRESOURCE(uintResID),
        strResType
        );
#endif
    if( NULL == hRes )
    {
        AfxMessageBox(_T("Init Failed ."));
        return;
    }
    
    HGLOBAL hGlobalRes = LoadResource(AfxGetResourceHandle(),hRes);
    if( NULL == hGlobalRes )
    {
        AfxMessageBox(_T("Init Failed ."));
        return;
    }
    
    LPVOID lpResData = LockResource( hGlobalRes );
    
    if( NULL == lpResData )
    {
        AfxMessageBox(_T("Init Failed ."));
        return;
    }
    
    DWORD dwSize = SizeofResource(  AfxGetResourceHandle(),hRes );
    if( !InitPictureData( (LPBYTE)lpResData,dwSize ) )
    {
        AfxMessageBox(_T("Init Failed ."));
    }
    
}

BOOL COtherPicFormatToBMP::InitPictureData(LPBYTE lpData,DWORD nSize )
{
    BOOL bResult = FALSE;
    
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);
    
    if(hGlobal == NULL)
    {
        
        return FALSE;
    }
    
    void* pData = GlobalLock(hGlobal);
    memcpy(pData, lpData, nSize);
    GlobalUnlock(hGlobal);
    
    IStream* pStream = NULL;
    
    if(CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
    {
        HRESULT hr;
        if((hr = OleLoadPicture(pStream, nSize, FALSE, IID_IPicture, (LPVOID *)&m_IPicture)) == E_NOINTERFACE)
        {
            m_allResource.push_back( m_IPicture );
            GlobalFree( hGlobal );
            return FALSE;
        }
        else // S_OK
        {
            m_allResource.push_back( m_IPicture );
            pStream->Release();
            pStream = NULL;
            bResult = TRUE;
        }
    }    
    GlobalFree( hGlobal );
    return bResult;
}
