// BufPool.cpp: implementation of the CBufPool class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BufPool.h"

#include <assert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBufPool::CBufPool():m_cUnitSize(10240)
{
    m_pBuf = new BYTE[10240];
    m_pReadPt = m_pBuf;
    m_pWritePt = m_pBuf;
    m_nTotalSize = m_cUnitSize;
}

CBufPool::~CBufPool()
{
    if(m_pBuf != NULL)
    {
        delete m_pBuf;
    }
}

void CBufPool::ReAlloc(int newsize)
{
    assert(newsize > m_nTotalSize);
    int nBlock = (newsize - m_nTotalSize) % m_cUnitSize + 1;
    newsize = m_nTotalSize + nBlock * m_cUnitSize;

    BYTE* pBuf = new BYTE[newsize];
	if (NULL == pBuf)
	{
		return;
	}
    int nSize;

    if(m_pReadPt < m_pWritePt)
    {
        nSize = m_pWritePt - m_pReadPt;
        memcpy(pBuf,m_pReadPt,nSize);
    }
    else
    {
        int size = m_nTotalSize - (m_pReadPt - m_pBuf);
        memcpy(pBuf,m_pReadPt,size);
        nSize = size;
        size = m_pWritePt - m_pBuf;
        memcpy(pBuf+size,m_pBuf,size);
        nSize += size;
    }
    
    delete m_pBuf;
    m_pBuf = pBuf;
    m_pReadPt = m_pBuf;
    m_pWritePt = m_pBuf + nSize;
    m_nTotalSize = newsize;
}

void CBufPool::Append(BYTE* pData,int uSize)
{
    assert(pData != NULL);
    assert(uSize > 0);

    if(m_nTotalSize - GetSize() < uSize)
    {
        ReAlloc(m_nTotalSize + uSize);
    }
    
    int nSize = m_nTotalSize - (m_pWritePt - m_pBuf);

    if(nSize < uSize)
    {
        memcpy(m_pWritePt,pData,nSize);
        memcpy(m_pBuf,pData + nSize,uSize - nSize);
        m_pWritePt = m_pBuf + uSize - nSize;
    }
    else
    {
        memcpy(m_pWritePt,pData,uSize);
        m_pWritePt += uSize;
    }
}

void CBufPool::Remove(int uSize)
{
    assert(uSize > 0);
    assert(uSize < m_nTotalSize);

    m_pReadPt += uSize;
    if(m_pReadPt - m_pBuf > m_nTotalSize)
    {
        m_pReadPt -= m_nTotalSize;
    }
}

int CBufPool::GetSize()
{
    int nSize;
    if(m_pReadPt > m_pWritePt)
    {
        nSize = m_nTotalSize - (m_pReadPt - m_pWritePt);
    }
    else
    {
        nSize = m_pWritePt - m_pReadPt;
    }
    return nSize;
}

void CBufPool::Copy(BYTE* pDes,int uSize)
{
    assert(pDes != NULL);
    assert(uSize > 0);
    assert(uSize <= GetSize());

    int size = m_nTotalSize - (m_pReadPt - m_pBuf);
    if(size < uSize)
    {
        memcpy(pDes,m_pReadPt,size);
        memcpy(pDes + size,m_pBuf,uSize - size);
    }
    else
    {
        memcpy(pDes,m_pReadPt,uSize);
    }    
}
