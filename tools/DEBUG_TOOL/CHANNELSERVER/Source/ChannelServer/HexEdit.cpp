// HexEdit.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrm.h"
#include "HexEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

char HexTable[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
#define TOHEX(DigVal, HexAscii)	{\
                                *(HexAscii)++ = HexTable[((DigVal) >> 4) & 0x0F]; \
                                *(HexAscii)++ = HexTable[(DigVal) & 0x0F]; \
}
/////////////////////////////////////////////////////////////////////////////
// CHexEdit

CHexEdit::CHexEdit()
{
    m_nTextLength = 0;
    m_strHexBuffer = _T("");
}

CHexEdit::~CHexEdit()
{
}


BEGIN_MESSAGE_MAP(CHexEdit, CEdit)
	//{{AFX_MSG_MAP(CHexEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHexEdit message handlers


void CHexEdit::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CEdit::PreSubclassWindow();

    // Set the READ_ONLY attribute
    SendMessage(EM_SETREADONLY, ES_READONLY);
}

char *CHexEdit::BufferToString(BYTE *pBuffer, int nLength, int nColumns, int *szResultLen)
{
    char          *pResultBuffer    = NULL;        //Result String
    int           nResultBufferSize;
    char          *pAddressBuf      = NULL;        //A pointer points to address buffer
    char          *pHexContentBuf   = NULL;        //A pointer points to hexacedimal content buffer
    char          *pAsciiContentBuf = NULL;        //A pointer points to Ascii content buffer
    const    int  nAddrLen          = 0x08;        //Address length
    const    int  nBreakLen         = 0x02;        //Break Length
    const    int  nSpaceChar        = 0x20;
    const    int  nBaseDigit        = '0';
    const    int  nHighestDigit     = '9';
    const    int  nDotChar          = '.';
    const    int  nLineBreak        = '\r';
    const    int  nNewline          = '\n';
    const    int  nStringTerminal   = '\0';
    const    int  nDiffHexCharDigit = 0x07;
    const    int  nHalfByte         = 0x04;   
    const    int  nBaseHighAscii    = 0x80;
    const    int  nLowFourBitsMask  = 0x0F;    
    register int  nRow, nCol;
    int           nBytesPerRow, nRowCount;
    int           nColumnsPerRow;
    int           nCurColIndex;         
    int           nDataAddress;
    int           nTempAdrress;
    char          *pSrcBuf;
    char          *pHexBuf;
    
    

    /*********************************************************************************/
    /*FFFFFFFF       01 02 03 04 05 06 07 08 09       123456789            '\r\n'    */
    /* Address  Break       Hex Content        Break Ascii Content Break  Line Break */
    /*  8         2         nColumns * 3         2     nColumns      2        2      */    
    /*********************************************************************************/
    /*nBytesPerRow = nAddrLen + nBreakLen + (nColumns * 3) + nBreakLen + nColumns +nBreakLen + nBreakLen;*/
    //Caluclate the bytes per row    
    nBytesPerRow = nAddrLen + (nBreakLen * 4 )+ (nColumns * 4) ;
    //Caluclate the Row count
    if((nLength % nColumns) == 0)
    {
       nRowCount = (nLength / nColumns);
    }
    else
    {
       nRowCount = (nLength / nColumns) + 1;
    }
    //Caluclate the result buffer size
    nResultBufferSize = nRowCount * nBytesPerRow;
    //Allocate the result buffer
    pResultBuffer  = new char[nResultBufferSize + 1];
    if(pResultBuffer == NULL)
    {
        return NULL;
    }
    //Fill the result buffer with SPACE
    memset(pResultBuffer, nSpaceChar, nResultBufferSize + 1);
    
    //Start convert buffer to string
    
    //Initialize local variable
    pSrcBuf          = (char *)pBuffer;
    pAddressBuf      = pResultBuffer;
    pHexContentBuf   = pAddressBuf    + nAddrLen + nBreakLen;
    pAsciiContentBuf = pHexContentBuf + nColumns * 3 + nBreakLen;
    nDataAddress     = 0;
    nColumnsPerRow   = nColumns;
    for(nRow = 0; nRow < (nRowCount - 1); nRow++)
    {
        //Write Address

        /******************************************************************/
        /*Convert a integer value into a hexacedimal ascii string         */
        /*The Converting sequecne is from low bit to high bit but         */
        /*the writing sequence is from high address to low address        */
        /*Example:  Value = 0x12345678                                    */
        /*Convert sequence: bit0-bit7  : 0x08,0x07,0x06,...,0x01          */
        /*Writing sequence: addr7-addr0: 0x08,0x07,0x06,...,0x01          */
        /******************************************************************/        
        nTempAdrress = nDataAddress;
        for(nCol = (nAddrLen - 1); nCol >= 0; nCol--)
        {
            pAddressBuf[nCol]  = char((nTempAdrress & nLowFourBitsMask) + nBaseDigit);
            if(pAddressBuf[nCol] > nHighestDigit)
            {
              pAddressBuf[nCol] += nDiffHexCharDigit;
            }
            nTempAdrress       = nTempAdrress >> nHalfByte;
        }
        
        /******************************************************************/
        /*Convert a byte value into a hexacedimal ascii string            */
        /*The Converting sequecne is from low bit to high bit but         */
        /*the writing sequence is from high address to low address        */
        /*Example:  Value = 0x21                                          */
        /*Convert sequence: bit0-bit1  : 0x01,0x02                        */
        /*Writing sequence: addr1-addr0: 0x01,0x02                        */
        /******************************************************************/    
        nCurColIndex = 0;
        for(nCol = 0; nCol < nColumnsPerRow; nCol++)
        {
            pHexBuf = &pHexContentBuf[nCurColIndex];
            TOHEX(*(pSrcBuf), pHexBuf);
            
            if((*(pSrcBuf) >= nSpaceChar) && (*(pSrcBuf) < nBaseHighAscii))
            {
               pAsciiContentBuf[nCol] = *(pSrcBuf) ;
            }
            else
            {
               pAsciiContentBuf[nCol] = nDotChar;
            }
           
            //move to the next byte
            pSrcBuf++;
            nCurColIndex += 3;
        }

        //Add wrap into the current row
        pAsciiContentBuf[nCol + nBreakLen]     = nLineBreak;
        pAsciiContentBuf[nCol + nBreakLen + 1] = nNewline;
        
        pHexContentBuf   += nBytesPerRow;
        pAsciiContentBuf += nBytesPerRow;
        pAddressBuf      += nBytesPerRow;
        nDataAddress     += nColumnsPerRow;
    }


    //Convert the last row data to avoid the nLength is not divide exactly by nColumns
    //Caluclate the Row count
    nColumnsPerRow = (nLength % nColumns);
    if(nColumnsPerRow == 0)
    {
        nColumnsPerRow = nColumns;
    }
    
    nCurColIndex = 0;
    nTempAdrress = nDataAddress;
    for(nCol = (nAddrLen - 1); nCol >= 0; nCol--)
    {
        pAddressBuf[nCol]  = char((nTempAdrress & nLowFourBitsMask) + nBaseDigit);
        if(pAddressBuf[nCol] > nHighestDigit)
        {
            pAddressBuf[nCol] += nDiffHexCharDigit;
        }
        nTempAdrress       = nTempAdrress >> nHalfByte;
    }

    for(nCol = 0; nCol < nColumnsPerRow; nCol++)
    {
        pHexBuf = &pHexContentBuf[nCurColIndex];
        TOHEX(*(pSrcBuf), pHexBuf);    
        
        
        if((*(pSrcBuf) >= nSpaceChar) && (*(pSrcBuf) < nBaseHighAscii))
        {
            pAsciiContentBuf[nCol] = *(pSrcBuf) ;
        }
        else
        {
            pAsciiContentBuf[nCol] = nDotChar;
        }
        
        //move to the next byte
        pSrcBuf++;
        nCurColIndex += 3;
    }
    
    //Add wrap into the current row
    pAsciiContentBuf[nCol + nBreakLen]     = nLineBreak;
    pAsciiContentBuf[nCol + nBreakLen + 1] = nNewline;

    //Add string terminal flag
    pAsciiContentBuf[nCol + nBreakLen + 2] = nStringTerminal;
   
    *szResultLen = nResultBufferSize;

    return pResultBuffer;
}

void CHexEdit::DisplayBuffer(char *pBuffer, int nLength)
{
    const int MAX_TEXT_LENGTH = 0x10000;  //The Edit buffer limit. 64K
    if (m_nTextLength > MAX_TEXT_LENGTH)
    {
        SetWindowText("");
        m_nTextLength = 0;
        m_strHexBuffer = _T("");
    }
    else
    {
        m_strHexBuffer += "\r\n";        
        m_nTextLength  = m_nTextLength + nLength + 2;
        m_strHexBuffer += pBuffer;
    }
    
    // Update the buffer content
    SetWindowText(m_strHexBuffer);    
}

void CHexEdit:: Clear()
{
    SetWindowText("");
    m_nTextLength = 0;
    m_strHexBuffer = _T("");
}