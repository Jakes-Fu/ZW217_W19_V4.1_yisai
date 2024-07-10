// HexEdit.cpp : implementation file
//

#include "stdafx.h"
#include "msdev.h"
#include "HexEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHexEdit

CHexEdit::CHexEdit()
{
}

CHexEdit::~CHexEdit()
{
}


BEGIN_MESSAGE_MAP(CHexEdit, CEdit)
	//{{AFX_MSG_MAP(CHexEdit)
	ON_WM_CTLCOLOR_REFLECT()
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

HBRUSH CHexEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

BOOL CHexEdit::IsEditFull()
{
    const int MAX_TEXT_LENGTH = 64000;

    CString strText;
    GetWindowText(strText);
    if (strText.GetLength() > MAX_TEXT_LENGTH)
        return TRUE;

    return FALSE;
}

CString CHexEdit::BufferToString(BYTE *pBuffer, int nLength, int nColumns)
{
    CString strHexBuffer;                       // Result string
    CString strHexByte;
    CString strHexColIndex;                     // Column index in Hex value (starting address)
    char    strAscii[100];                      // ASCII string according to hex value
    int     i, j;

    // Add the new buffer content to the edit control
    memset(strAscii, 0, sizeof(strAscii));
    for (i = 0; i < nLength; i++)
    {
        strHexByte.Format("%02X", pBuffer[i]);

        if (i % nColumns == 0)                // The first column
        {
            strHexColIndex.Format("%04X", i);
            strHexBuffer += strHexColIndex + "  " + strHexByte + " ";
        }
        else if ((i + 1) % nColumns == 0)      // The last column ( nColumns * n )
        {
            memcpy(strAscii, &pBuffer[i - (nColumns - 1)], nColumns);
            for (j = 0; j < nColumns; j++)
                if (!isprint(strAscii[j]))      strAscii[j] = '.';

            if (i == nLength - 1)           // The last line
                strHexBuffer += strHexByte + "  " + strAscii;
            else
                strHexBuffer += strHexByte + "  " + strAscii + "\r\n";
        }
        else
            strHexBuffer += strHexByte + " ";
    }

    if (nLength % nColumns != 0)               // The rest
    {
        for (i = 0; i < nColumns - nLength % nColumns; i++)
            strHexBuffer += "   ";
        strHexBuffer += " ";

        memset(strAscii, 0, sizeof(strAscii));
        memcpy(strAscii, &pBuffer[nLength - nLength % nColumns], nLength % nColumns);
        for (i = 0; i < nLength % nColumns; i++)
            if (!isprint(strAscii[i]))      strAscii[i] = '.';
        strHexBuffer += strAscii;
    }

    return strHexBuffer;
}

void CHexEdit::DisplayBuffer(BYTE *pBuffer, int nLength)
{
    int         COLUMN;
    CRect       rcEdit;
    int         cxEdit;
    int         cxChar;
    CDC         *pDC;
    CString     strHexBuffer;
    TEXTMETRIC  textMetric;

    if (IsEditFull())
        SetWindowText("");
    
    GetRect(&rcEdit);
    pDC = GetDC();
    pDC->GetTextMetrics(&textMetric);
    cxEdit = rcEdit.Width();
    cxChar = textMetric.tmAveCharWidth + 1;
    COLUMN = (cxEdit / cxChar - 7) / 4;         // Calculate suitable hex-BYTE number perline
 
    // Get the current buffer content
    GetWindowText(strHexBuffer);
    if (!strHexBuffer.IsEmpty())
    {
        int nBufferLength       = strHexBuffer.GetLength();
        int nLastReturnIndex    = strHexBuffer.ReverseFind((TCHAR)'\r\n');
        int nCharNumOfLastLine  = nBufferLength - nLastReturnIndex;
        int nCharNumPerLine     = 4 + 2 + 3 * COLUMN - 1 + 2 + COLUMN;
        
        if (nCharNumOfLastLine == nCharNumPerLine)
           strHexBuffer += "\r\n";
        else
            strHexBuffer += "\r\n\r\n";
    }

    // Add the new buffer content to the edit control
    strHexBuffer += BufferToString(pBuffer, nLength, COLUMN);

    // Update the buffer content
    SetWindowText(strHexBuffer);

    // Scroll to the end automatically
    int nLineCount  = GetLineCount();
    int nFVIndex    = GetFirstVisibleLine();
    LineScroll(nLineCount - nFVIndex);

    // Release the window DC
    ReleaseDC(pDC);
}