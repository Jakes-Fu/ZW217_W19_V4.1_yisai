/*============================================================================*
 *
 *                          Copyright (c) 2002 Spreadtrum
 *                              All Rights Reserved
 *
 *-----------------------------------------------------------------------------
 *
 * File name:
 *      CDspFrameParser.h
 *
 * Description:
 *      This header file provides the declarations of all the exported functions
 *      of CDspFrameParser
 *
 * 
 *============================================================================*/


/*============================================================================*
 *                          Edit History
 *-----------------------------------------------------------------------------
 * 2002-11-13   Kai Liu     Created
 *============================================================================*/

#if !defined(AFX_DSPFRAMEPARSER_H__D6088991_7089_4D6A_A436_870CD6E193A3__INCLUDED_)
#define AFX_DSPFRAMEPARSER_H__D6088991_7089_4D6A_A436_870CD6E193A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DspKernel.h"
#include <assert.h>

class DSPKERNEL_API CDspFrameParser  
{
public:
	CDspFrameParser();
	virtual ~CDspFrameParser();

//public functions
public:
// -----------------------------------------------------------------------------
// Name:
//      ParseFrame
//
// Function:
//      Parse a Dsp Frame
// 
// Discription:
//      classify a Dsp frame and forward the frame 
//      to the right parse
// -----------------------------------------------------------------------------
	BOOL ParseFrame(
		DspFrame* pFrame    //the frame pointer
		);

// -----------------------------------------------------------------------------
// Name:
//      ParseTypedKeyFrm
//
// Function:
//      Parse a key information Frame without time and type field
// 
// Note:
//      The pInfo->dwTaskId must contain valid key information's task id
// -----------------------------------------------------------------------------
	BOOL ParseTypedKeyFrm(
		BYTE** ppFrame,    //the frame pointer
   		ParsedInfo* pInfo     //the parsed info pointer
		);

// -----------------------------------------------------------------------------
// Name:
//      SetReceiver
//
// Function:
//      tell the kernel who is to receive the message
//
//      
// -----------------------------------------------------------------------------
    DWORD SetReceiver(HWND hWnd)
    {
        //get the unique gui message id 
        m_dwGuiMsgId = RegisterWindowMessage(GUI_MSG_NAME);

        m_hRcvWnd = hWnd;

        return m_dwGuiMsgId;
    }

protected:
// -----------------------------------------------------------------------------
// Name:
//      ParseResFrm
//
// Function:
//      Parse a Dsp command respond Frame
// 
// 
// -----------------------------------------------------------------------------
	BOOL ParseResFrm(
		DspFrame** ppFrame,   //the frame pointer
        ParsedInfo* pInfo     //the parsed info pointer
		);

// -----------------------------------------------------------------------------
// Name:
//      ParseTstPtFrm
//
// Function:
//      Parse a Dsp test point Frame
// 
// -----------------------------------------------------------------------------

 	BOOL ParseTstPtFrm(      //the message length
		BYTE** ppFrame,       //the frame pointer
  		ParsedInfo* pInfo           //the parsed info pointer
		);

// -----------------------------------------------------------------------------
// Name:
//      ParseKeyFrm
//
// Function:
//      Parse a Dsp key information Frame
// 
// -----------------------------------------------------------------------------
	BOOL ParseKeyFrm(        //the message length
		BYTE** ppFrame,       //the frame pointer
   		ParsedInfo* pInfo           //the parsed info pointer
		);

// -----------------------------------------------------------------------------
// Name:
//      ParseTypedKeyFrm
//
// Function:
//      Parse a key information Frame without time and type field
// 
// Note:
//      The pInfo->dwTaskId must contain valid key information's task id
// -----------------------------------------------------------------------------
	BOOL ParseTypedKeyFrm(
		BYTE** ppFrame,         //the frame pointer
   		ParsedInfo* pInfo,      //the parsed info pointer
        BOOL bParseSubtype,     //whether parse subtype information
        BOOL bCopyBuffer        //whether copy message to the output buffer
		);

// -----------------------------------------------------------------------------
// Name:
//      ParseFgtaskFrm
//
// Function:
//      Parse a foreground task key information Frame
// 
// -----------------------------------------------------------------------------
	BOOL ParseFgtaskFrm(     //the message length
		BYTE* pFrame,   //the frame pointer
  		ParsedInfo* pInfo               //the parsed info pointer
		);

// -----------------------------------------------------------------------------
// Name:
//      ParseBgtaskFrm
//
// Function:
//      Parse a background task key information Frame
// 
// -----------------------------------------------------------------------------
	BOOL ParseBgtaskFrm(     //the message length
		BYTE* pFrame,   //the frame pointer
		ParsedInfo* pInfo               //the parsed info pointer
		);

// -----------------------------------------------------------------------------
// Name:
//      ParseFrmInstFrm
//
// Function:
//      Parse a frame instruction key information Frame
// 
// -----------------------------------------------------------------------------
	BOOL ParseFrmInstFrm(    //the message length
		BYTE* pFrame,   //the frame pointer
		ParsedInfo* pInfo               //the parsed info pointer
		);

// -----------------------------------------------------------------------------
// Name:
//      ParseMcuFrm
//
// Function:
//      Parse a MCU command key information Frame
// 
// -----------------------------------------------------------------------------
	BOOL ParseMcuFrm(        //the message length
		BYTE* pFrame,   //the frame pointer
		ParsedInfo* pInfo               //the parsed info pointer
		);

// -----------------------------------------------------------------------------
// Name:
//      ParseRssiFrm
//
// Function:
//      Parse a RSSI key information Frame
// 
// -----------------------------------------------------------------------------
	BOOL ParseRssiFrm(       //the message length
		BYTE* pFrame,   //the frame pointer
		ParsedInfo* pInfo               //the parsed info pointer
		);

public:
// -----------------------------------------------------------------------------
// Name:
//      NotifyGui
//
// Function:
//      Notify the gui some data was ready
//      
// -----------------------------------------------------------------------------
    void NotifyGui(ParsedInfo* pInfo)
    {
        ::PostMessage(m_hRcvWnd,m_dwGuiMsgId,WPARAM(pInfo),0);
    }

protected:
    //the id of the message used to notify gui
    UINT m_dwGuiMsgId;

    //Gui window handle to receive the notify message
    HWND m_hRcvWnd;

};

#endif // !defined(AFX_DSPFRAMEPARSER_H__D6088991_7089_4D6A_A436_870CD6E193A3__INCLUDED_)
