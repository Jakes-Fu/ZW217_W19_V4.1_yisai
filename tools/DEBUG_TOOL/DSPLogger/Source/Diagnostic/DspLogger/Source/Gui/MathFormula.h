// MathFormula.h: interface for the CMathFormula class.
//
//////////////////////////////////////////////////////////////////////
#ifndef _MATHFORMULA_H_ 
#define _MATHFORMULA_H_

#define MATH_FLAG_MAX 7

enum{
     MATH_MASK =0,
     MATH_MOVER,
     MATH_MOVEL,
     MATH_ADD,
     MATH_SUB,
     MATH_MUL,
     MATH_DIV,
    };

class CMathFormula  
{
public:
	CMathFormula();
	virtual ~CMathFormula();

public:

    static LPCSTR GetMathFlagDesc( int nFlag );

           void   set_mask(WORD wMask);
    inline WORD   math_mask(WORD wData);
           
           void   set_move(WORD wMoveBit,BOOL bRight);
    inline DWORD  math_move(DWORD dwData);

           void   set_mult (double fConstant); 
    inline double math_mult(double wData);
	
		   void   set_add (DWORD dwAdd); 
    inline DWORD math_add(DWORD dwData);

		   void   set_sub (DWORD dwSub); 
    inline DWORD math_sub(DWORD dwData);

		   void   set_div (double fDiv); 
    inline double math_div(double fData);

     
    double math_deal(double dbData);
 
private:
//mask
    WORD m_wMask;

//move
    WORD m_wMoveBit;
    BOOL m_bMoveRight;

//multiply 
    double m_fConstant;

//add
	DWORD m_dwAdd;

//sub
	DWORD m_dwSub;

//div
	double m_fDiv;
};

#endif // _MATHFORMULA_H_