// MathFormula.cpp: implementation of the CMathFormula class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>
#include "dsplogger.h"
#include "MathFormula.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DOUBLE_PRECISIN 0.000001

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMathFormula::CMathFormula()
{
    //mask
    m_wMask      = 0xffff;
    m_wMoveBit   = 0;
    m_bMoveRight = TRUE;
    //multiply
    m_fConstant   = 1;

	m_dwAdd = 0;
	m_dwSub = 0;
	m_fDiv = 1;
}

CMathFormula::~CMathFormula()
{

}

LPCSTR CMathFormula::GetMathFlagDesc( int nFlag )
{
    _ASSERTE( nFlag >= 0 && nFlag < MATH_FLAG_MAX );
    if (nFlag < 0 || nFlag>= MATH_FLAG_MAX) return NULL;

    static const char * szFlag[] = { 
        { "&" },
        { ">>" },
        { "<<" },
        { "+" },
        { "-" },
        { "*" },
        { "/" },
    };

    return szFlag[nFlag];
}

void CMathFormula::set_mask(WORD wMask)
{
    m_wMask     = wMask;
}

WORD CMathFormula::math_mask(WORD wData)
{
    WORD wResult= 0;
    
    wResult= (WORD)(wData & m_wMask);

    return wResult;
}


void CMathFormula::set_move(WORD wMoveBit,BOOL bRight)
{
    m_wMoveBit  = wMoveBit;
    m_bMoveRight= bRight;
}

DWORD CMathFormula:: math_move(DWORD dwData)
{
    DWORD result=0;
    if (m_bMoveRight)
    {
        result=dwData >> m_wMoveBit;
    }
    else
    {
        result=dwData << m_wMoveBit;
    }
    return result;
}

void CMathFormula::set_mult(double fConstant)
{
    m_fConstant= fConstant;
}

double CMathFormula::math_mult(double wData)
{
    double wResult= 0;   
    wResult =m_fConstant*wData;
    
    return wResult;
}

double CMathFormula::math_deal(double dbData)
{
    double fResult= dbData;
    
    //mask
    if (m_wMask != 0xffff)
    {
        fResult= (double)math_mask((WORD)dbData);
    }
    //move
    if (m_wMoveBit != 0)
    {
        fResult= (double)math_move((WORD)fResult);
    }
    //mult
    if (fabs(m_fConstant-1) > DOUBLE_PRECISIN)
    {
         fResult = math_mult(fResult);
    }

	//add
	if(m_dwAdd != 0)
	{
		fResult = (double)math_add((DWORD)dbData);
	}

	//sub
	if(m_dwSub != 0)
	{
		fResult = (double)math_sub((DWORD)dbData);
	}

	//div
	if(fabs(m_fDiv-1) > DOUBLE_PRECISIN )
	{
		fResult = math_div(fResult);
	}
    return fResult;
}

void   CMathFormula::set_add (DWORD dwAdd)
{
	m_dwAdd = dwAdd;
}

DWORD CMathFormula::math_add(DWORD dwData)
{
	DWORD wResult= 0;   
    wResult = m_dwAdd + dwData;
    
    return wResult;
}

void   CMathFormula::set_sub (DWORD dwSub)
{
	m_dwSub = dwSub;
}

DWORD CMathFormula::math_sub(DWORD dwData)
{
	DWORD wResult= 0;   
    wResult = dwData - m_dwSub;
    
    return wResult;
}

void   CMathFormula::set_div (double fDiv)
{
	m_fDiv = fDiv;
}

double CMathFormula::math_div(double fData)
{
	double fResult= 0;   
    fResult = fData / m_fDiv;
    
    return fResult;
}