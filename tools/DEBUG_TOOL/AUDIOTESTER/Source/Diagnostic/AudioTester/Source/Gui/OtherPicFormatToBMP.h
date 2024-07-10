// OtherPicFormatToBMP.h: interface for the COtherPicFormatToBMP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OTHERPICFORMATTOBMP_H__176D3B46_6037_4B18_A5CD_1F0AD3EC27E0__INCLUDED_)
#define AFX_OTHERPICFORMATTOBMP_H__176D3B46_6037_4B18_A5CD_1F0AD3EC27E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma  warning(push,3)
#include <vector>
#pragma  warning(pop)
class COtherPicFormatToBMP  
{
public:
	COtherPicFormatToBMP();
	virtual ~COtherPicFormatToBMP();
public:
    // load pic resource from resource. 
    //can also load from file, not support now
    void Init(UINT uintResID,LPCSTR strResType);

    //convert to BMP format
    operator HBITMAP ()const; 
private:
    BOOL InitPictureData(LPBYTE lpData,DWORD uSize );

    //current IPicture 
    IPicture* m_IPicture;

    //current bmp handle
    HBITMAP m_hBMP;

    //all picture resource , it will release all picture resource
    //as soon as the instance of class COtherPicFormatToBMP leave its lifetime scope
    std::vector<IPicture*>m_allResource;
};

#endif // !defined(AFX_OTHERPICFORMATTOBMP_H__176D3B46_6037_4B18_A5CD_1F0AD3EC27E0__INCLUDED_)
