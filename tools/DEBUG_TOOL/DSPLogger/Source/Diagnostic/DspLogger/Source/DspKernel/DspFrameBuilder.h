/*============================================================================*
 *
 *                          Copyright (c) 2002 Spreadtrum
 *                              All Rights Reserved
 *
 *-----------------------------------------------------------------------------
 *
 * File name:
 *      DspFrameBuilder.h
 *
 * Description:
 *      This header file provides the declarations of all the exported functions
 *      of DspFrameBuilder
 *
 * 
 *============================================================================*/


/*============================================================================*
 *                          Edit History
 *-----------------------------------------------------------------------------
 * 2002-11-13   Kai Liu     Created
 *============================================================================*/

#if !defined(AFX_DSPFRAMEBUILDER_H__959A2195_88C4_4A90_95F3_324D96A9ACAD__INCLUDED_)
#define AFX_DSPFRAMEBUILDER_H__959A2195_88C4_4A90_95F3_324D96A9ACAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*============================================================================*
 *                          Header files
 *============================================================================*/

#include "DspKernel.h"

 /*============================================================================*
 *                          Declaration of CDspFrameBuilder
 *============================================================================*/
#define DATA_SIZE (DSP_FRM_SIZE - 5)

class DSPKERNEL_API CDspFrameBuilder  
{
public:
	//ctor and dtor
	CDspFrameBuilder();

	virtual ~CDspFrameBuilder();

public:
// -----------------------------------------------------------------------------
// Name:
//      GetFrame
//
// Function:
//      Get the frame pointer built by the invoke of BuildCommand
// 
// Description:
//      now a command was limited to one frame,future will be changed 
// -----------------------------------------------------------------------------
	DspFrame* GetFrame()
	{
		return &m_CmdString;
	}

// -----------------------------------------------------------------------------
// Name:
//      AddToCmd
//
// Function:
//      Add an element to a command string
// 
// Description:
//      Add an element to compose a command frame,
//      this element maybe a command name,command id,
//      parameter pointer or combine of them
// -----------------------------------------------------------------------------
    UINT AddToCmd(          //return the status of the command
        WORD wElement     //the element added to the command
        );

    UINT AddToCmd(          //return the status of the command
        BYTE* pElement,     //the element pointer
        UINT  uSize         //the element length
        );

// -----------------------------------------------------------------------------
// Name:
//      Reset
//
// Function:
//      Restore the initialize status
// 
// -----------------------------------------------------------------------------
    void Reset()
    {
        m_Status = CMD_NEEDBODY;
        m_nCurPos = 0;
        m_uErrorCode = BUILDCMD_SUCCESS;
        memset(m_CmdString.frame,0xff,DSP_FRM_SIZE);
        m_nParaSize = 0;
        m_wCmdType = BADCOMMAND;
    }

// -----------------------------------------------------------------------------
// Name:
//      GetCmdType
//
// Function:
//      Return last command type
// 
// -----------------------------------------------------------------------------
    DWORD GetCmdType()
    {
        return m_wCmdType;
    }

// -----------------------------------------------------------------------------
// Name:
//      GetFirstPara
//
// Function:
//      Return the first parameter
// 
// -----------------------------------------------------------------------------
    DWORD GetFirstPara()
    {
        return m_wFirstPara;
    }

// -----------------------------------------------------------------------------
// Name:
//      GetSecondPara
//
// Function:
//      Return the second parameter
// 
// -----------------------------------------------------------------------------
    DWORD GetSecondPara()
    {
        return m_wSecondPara;
    }


// -----------------------------------------------------------------------------
// Name:
//      GetLastPara
//
// Function:
//      Return the last parameter
// 
// -----------------------------------------------------------------------------
    DWORD GetLastPara()
    {
        return (DWORD)m_aData;
    }

private:
// -----------------------------------------------------------------------------
// Name:
//      CheckCommand
//
// Function:
//      Check the command type to see if it is a valid command
// 
// -----------------------------------------------------------------------------
	DSP_CMD_TYPE CheckCommand(
		BYTE* pCommand,        //command name
        UINT uSize,            //buffer size
        UINT& uDone            //how many byte it have processed
		);

    DSP_CMD_TYPE CheckCommand(
        WORD wCmd           //command index
        );

// -----------------------------------------------------------------------------
// Name:
//      CheckParameter
//
// Function:
//      Check the parameter
// 
// -----------------------------------------------------------------------------
    BOOL CheckParameter(
        WORD wPara           //parameter data
        );

    BOOL CheckParameter(
        BYTE* pPara,          //parameter data
        UINT uSize,           //buffer size
        UINT& uDone           //how many byte it have processed
        );

// -----------------------------------------------------------------------------
// Name:
//      CheckFirstParameter
//
// Function:
//      Check the first parameter
// 
// -----------------------------------------------------------------------------
    BOOL CheckFirstPara(
        WORD wPara            //parameter data
        );

    BOOL CheckFirstPara(
        BYTE* pPara,        //parameter data
        UINT uSize,         //buffer size
        UINT& uDone         //how many byte it have processed
        );

// -----------------------------------------------------------------------------
// Name:
//      CheckSecondParameter
//
// Function:
//      Check the second parameter
// 
// -----------------------------------------------------------------------------
    BOOL CheckSecondPara(
        WORD wPara            //parameter data
        );

    BOOL CheckSecondPara(
        BYTE* pPara,        //parameter data
        UINT uSize,         //buffer size
        UINT& uDone         //how many byte it have processed
        );

// -----------------------------------------------------------------------------
// Name:
//      CheckDataPara
//
// Function:
//      Check the last parameter
// 
// -----------------------------------------------------------------------------
    BOOL CheckDataPara(
        WORD wData             //parameter data
        )
    {
        //the last parameter can't be given
        //by called this function
        SetErrorCode(BUILDCMD_BADPARA);
        return FALSE;
    }

    BOOL CheckDataPara(
        BYTE* pPara,        //parameter data
        UINT uSize,         //buffer size
        UINT& uDone         //how many byte it have processed
        );

// -----------------------------------------------------------------------------
// Name:
//      CheckRegName
//
// Function:
//      Check if the string contains a valid DSP register name
// 
// -----------------------------------------------------------------------------
	BOOL CheckRegName(
		char* pRegName,     //register name
        DWORD& dwRegType    //register type returned
		);

// -----------------------------------------------------------------------------
// Name:
//      CheckRegister
//
// Function:
//      Check if the parameter is a valid DSP register name
// 
// -----------------------------------------------------------------------------
    DSP_REG_TYPE CheckRegister(
        WORD wReg         //register index
        );

// -----------------------------------------------------------------------------
// Name:
//      GetErrorCode
//
// Function:
//      Get current error code
// 
// -----------------------------------------------------------------------------
    UINT GetErrorCode()
    {
        return m_uErrorCode;
    }

// -----------------------------------------------------------------------------
// Name:
//      SetErrorCode
//
// Function:
//      Set the current error code
// 
// -----------------------------------------------------------------------------
    void SetErrorCode(UINT uError)
    {
        m_uErrorCode = uError;
    }

// -----------------------------------------------------------------------------
// Name:
//      SetStatus
//
// Function:
//      Set the current error code
// 
// -----------------------------------------------------------------------------
    void SetStatus(UINT status)
    {
        m_Status = status;
    }

// -----------------------------------------------------------------------------
// Name:
//      NextStatus
//
// Function:
//      Change to next status
// 
// -----------------------------------------------------------------------------
    void NextStatus()
    {
        if(m_Status != CMD_FINISHED)
            m_Status++;
    }

// -----------------------------------------------------------------------------
// Name:
//      AddToCmdString
//
// Function:
//      Add current element to the command string
// 
// -----------------------------------------------------------------------------
    void AddToCmdString(UINT uStatus);

// -----------------------------------------------------------------------------
// Name:
//      CheckAddr
//
// Function:
//      to see whether the parameter is a valid parameter
// 
// Note:
//      Not implement yet,only return TRUE;
// -----------------------------------------------------------------------------
    BOOL CheckAddr(WORD)
    {
        return TRUE;
    }

// -----------------------------------------------------------------------------
// Name:
//      CheckBlockSize
//
// Function:
//      to see whether the parameter is a valid parameter
// Note:
//      in this version,the block must can be insert in one frame
// -----------------------------------------------------------------------------
    BOOL CheckBlockSize(WORD wBlockSize)
    {
        return (wBlockSize > 0 && wBlockSize < DATA_SIZE);
    }

private:
	DspFrame* m_pFrame;

    DspFrame m_CmdString;
    int m_nCurPos;

    //current state of the command
    UINT m_Status;

    //error code of the command
    UINT m_uErrorCode;

    //current command type
    WORD m_wCmdType;

    //the first parameter
    WORD m_wFirstPara;

    //the second parameter
    WORD m_wSecondPara;

    //the last parameter
    BYTE m_aData[DATA_SIZE];

    //current parameter size
    int m_nParaSize;
};

#endif // !defined(AFX_DSPFRAMEBUILDER_H__959A2195_88C4_4A90_95F3_324D96A9ACAD__INCLUDED_)
