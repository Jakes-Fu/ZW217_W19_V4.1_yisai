/*============================================================================*
 *
 *                          Copyright (c) 2002 Spreadtrum
 *                              All Rights Reserved
 *
 *-----------------------------------------------------------------------------
 *
 * File name:
 *      DspFrameBuilder.cpp
 *
 * Description:
 *      This .cpp file provides the implementation of all the exported functions
 *      of CDspFrameBuilder
 * 
 *============================================================================*/


/*============================================================================*
 *                          Edit History
 *-----------------------------------------------------------------------------
 * 2002-11-13   Kai Liu     Created
 *============================================================================*/

#include "stdafx.h"
#include "DspFrameBuilder.h"

#include <assert.h>

#include "CommandHelper.h"

/*============================================================================*
 *                          global funtions
 *============================================================================*/
// -----------------------------------------------------------------------------
// Function:
//      IsDigit
//
// Discription:
//      to see if the given string is a valid digit
// 
// -----------------------------------------------------------------------------
#define DECIMAL 1
#define HEXIMAL 2
int IsDigit(              //return the digit base
          char* pDigit  //the given string
         )
{
    if(pDigit == NULL)
        return 0;   //parameter is invalid
    
    int nSize = sizeof(pDigit);
    if(nSize == 0)
        return 0;   //parameter is invalid
    else if(nSize == 1)
    {
        if(!isdigit(pDigit[0]))
            return 0;   //not a digit
        else
            return DECIMAL;   //be a decimal digit
    }
    else if(nSize == 2)
    {
        if(!isdigit(pDigit[0]) && !isdigit(pDigit[1]))
            return 0;   //not a digit
        else
            return DECIMAL;   //be a decimal digit
    }
    else //if(nSize > 2)
    {
        if(pDigit[0] == '0' && (pDigit[1] == 'x' || pDigit[1] == 'X'))
        {
            //a heximal digit
            for(int i=2;i<nSize;i++)
            {
                if(!isdigit(pDigit[i]))
                    return 0;
            }
            return HEXIMAL;
        }
        else
        {
            //a decimal digit
            for(int i=0;i<nSize;i++)
            {
                if(!isdigit(pDigit[i]))
                    return 0;
            }
            return DECIMAL;
        }
    }    
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDspFrameBuilder::CDspFrameBuilder()
{
	m_pFrame = NULL;
    m_uErrorCode = BUILDCMD_NEEDINFO;

    Reset();
}

CDspFrameBuilder::~CDspFrameBuilder()
{
	if(m_pFrame != NULL)
		delete m_pFrame;
}

// -----------------------------------------------------------------------------
// Name:
//      CheckCommand
//
// Function:
//      Check the command type to see if it is a valid command
// 
// -----------------------------------------------------------------------------
DSP_CMD_TYPE CDspFrameBuilder::
CheckCommand(
        BYTE* pCommand,        //command name
        UINT uSize,            //buffer size
        UINT& uDone            //how many byte it have processed
			)
{
    assert(pCommand != NULL);
    assert(uSize != 0);
    assert(pCommand[0] != ' ');

    uDone = 0;
    while(uDone < uSize && pCommand[uDone] != ' ')
        uDone++;

    char* pCmd = new char[uDone + 1];
	if (pCmd)
	{
		memcpy(pCmd,pCommand,uDone);
		pCmd[uDone + 1] = 0;
		
		//to check if it is a valid command
		CDSPCmdHelper CmdHelper;
		m_wCmdType = CmdHelper.CheckCommand(pCmd);

		if (uDone>1)
		{
			delete[] pCmd;
		}
		else
		{
			delete pCmd;
		}
		
		
	}
    

    //if not,set the error code
    if(m_wCmdType == BADCOMMAND)
    {
        SetErrorCode(BUILDCMD_BADCMD);
    }
    else if(m_wCmdType == RRA)
    {
        //don't need any parameter
        SetErrorCode(BUILDCMD_SUCCESS);
        SetStatus(CMD_FINISHED);
    }
    else
    {
        //need more parameter
        SetErrorCode(BUILDCMD_NEEDINFO);
        NextStatus();
    }

    return DSP_CMD_TYPE(m_wCmdType);
}

DSP_CMD_TYPE CDspFrameBuilder::
CheckCommand(
        WORD wCmd           //command index
        )
{
    //to check if it is a valid command
    CDSPCmdHelper CmdHelper;
    m_wCmdType = CmdHelper.CheckCommand(wCmd);

    //if not,set the error code
    if(m_wCmdType == BADCOMMAND)
    {
        SetErrorCode(BUILDCMD_BADCMD);
    }
    else if(m_wCmdType == RRA)
    {
        //don't need any parameter
        SetErrorCode(BUILDCMD_SUCCESS);
        SetStatus(CMD_FINISHED);
    }
    else
    {
        //need more parameter
        SetErrorCode(BUILDCMD_NEEDINFO);
        NextStatus();
    }

    return DSP_CMD_TYPE(m_wCmdType);
}

// -----------------------------------------------------------------------------
// Name:
//      CheckRegName
//
// Function:
//      Check if the string contains a valid DSP register name
// 
// -----------------------------------------------------------------------------
inline BOOL CDspFrameBuilder::CheckRegName(
		char* pRegName,     //register name
        DWORD& dwRegType    //register type returned
		)
{
    CDSPCmdHelper CmdHelper;
    return CmdHelper.CheckRegName(pRegName,dwRegType);
}

// -----------------------------------------------------------------------------
// Name:
//      CheckRegister
//
// Function:
//      Check if the parameter is a valid DSP register name
// 
// -----------------------------------------------------------------------------
DSP_REG_TYPE CDspFrameBuilder::
CheckRegister(
        WORD wReg         //register index
        )
{
    CDSPCmdHelper CmdHelper;
    return CmdHelper.CheckRegister(wReg);
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
UINT CDspFrameBuilder::
    AddToCmd(               //return the status of the command
        WORD wElement     //the element added to the command
        )
{
    switch(m_Status)
    {
        //an empty command yet
        case CMD_NEEDBODY:
        {
            //so this element must a command id
            if(CheckCommand(wElement) != BADCOMMAND)
            {
                //a valid command id
                AddToCmdString(CMD_NEEDBODY);
            }
        }
        break;
        
        //now wait for parameter
        case CMD_NEEDPARA1:
        case CMD_NEEDPARA2:
        case CMD_NEEDDATA:
        {
            UINT status = m_Status;
            if(CheckParameter(wElement))
            {
                //valid parameter
                AddToCmdString(status);
            }
        }    
        break;
 
        //do not need parameter
        case CMD_FINISHED:
            {
                SetErrorCode(BUILDCMD_TOOMANYPARA);
            }
        break;

        default:
        break;
    }

    return GetErrorCode();
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

UINT CDspFrameBuilder::
    AddToCmd(               //return the status of the command
        BYTE* pElement,     //the element pointer
        UINT  uSize         //the element length
        )
{
    if(pElement == NULL || uSize == 0)
    {
        SetErrorCode(BUILDCMD_BADPARA);
        return GetErrorCode();
    }

    //find the first character
    UINT nSt = 0;
    while(nSt < uSize && pElement[nSt] == ' ')
        nSt++;
    //all blanks
    if(nSt == uSize)
    {
        SetErrorCode(BUILDCMD_BADPARA);
        return GetErrorCode();
    }

    UINT nEd;

    switch(m_Status)
    {
        //an empty command yet
        case CMD_NEEDBODY:
        {
            //so this element must a command id
            if(CheckCommand(pElement + nSt,uSize - nSt,nEd) != BADCOMMAND)
            {
                //a valid command id
                AddToCmdString(CMD_NEEDBODY);
            }
        }
        break;
        
        //now wait for parameter
        case CMD_NEEDPARA1:
        case CMD_NEEDPARA2:
        case CMD_NEEDDATA:
        {
            if(CheckParameter(pElement + nSt,uSize - nSt,nEd))
            {
                //valid parameter
                AddToCmdString(CMD_NEEDBODY);
            }
        }    
        break;
 
        //do not need parameter
        case CMD_FINISHED:
            {
                SetErrorCode(BUILDCMD_TOOMANYPARA);
            }
        break;

        default:
        break;
    }

    nEd += nSt;
    while(nEd < uSize && pElement[nEd] == ' ')
        nEd++;

    if(nEd == uSize)
    {
        //no more element
        return GetErrorCode();
    }
    else
    {
        //there is some information in the element
        return AddToCmd(pElement + nEd,uSize - nEd);
    }
}

// -----------------------------------------------------------------------------
// Name:
//      AddToCmdString
//
// Function:
//      Add current element to the command string
// 
// -----------------------------------------------------------------------------
void CDspFrameBuilder::AddToCmdString(UINT uStatus)
{
    switch(uStatus)
    {
    case CMD_NEEDBODY:
        m_CmdString.frame[m_nCurPos++] = CMD_TYPE;
        m_CmdString.frame[m_nCurPos++] = 0;
        //one bytes
        m_CmdString.frame[m_nCurPos++] = LOBYTE(m_wCmdType);
        //m_CmdString.frame[m_nCurPos++] = HIBYTE(m_wCmdType);
        break;

    case CMD_NEEDPARA1:
        {
        //four bytes
            if(m_nParaSize == 2)
            {
                m_CmdString.frame[m_nCurPos++] = HIBYTE(m_wFirstPara);
                m_CmdString.frame[m_nCurPos++] = LOBYTE(m_wFirstPara);
            }
            else
            {
                m_CmdString.frame[m_nCurPos++] = LOBYTE(m_wFirstPara);
            }
        }
        break;

    case CMD_NEEDPARA2:
        {
        //four bytes
            if(m_nParaSize == 2)
            {
                m_CmdString.frame[m_nCurPos++] = HIBYTE(m_wSecondPara);
                m_CmdString.frame[m_nCurPos++] = LOBYTE(m_wSecondPara);
            }
            else
            {
                m_CmdString.frame[m_nCurPos++] = LOBYTE(m_wSecondPara);
            }
        }
        break;

    case CMD_NEEDDATA:
        memcpy(m_CmdString.frame + m_nCurPos,m_aData,DATA_SIZE);    
        break;

    default:
        assert(FALSE);
        break;
    }
}

// -----------------------------------------------------------------------------
// Name:
//      CheckParameter
//
// Function:
//      Check the parameter
// 
// -----------------------------------------------------------------------------
BOOL CDspFrameBuilder::
CheckParameter(
        WORD wPara           //parameter data
        )
{
    switch(m_Status)
    {
    case CMD_NEEDPARA1:
        //the first parameter
        return CheckFirstPara(wPara);
        break;
    case CMD_NEEDPARA2:
        //the second parameter
        return CheckSecondPara(wPara);
        break;
    case CMD_NEEDDATA:
        //the third parameter
        return CheckDataPara(wPara);
        break;
    }

    return TRUE;
}

BOOL CDspFrameBuilder::
CheckParameter(
        BYTE* pPara,          //parameter data
        UINT uSize,           //buffer size
        UINT& uDone           //how many byte it have processed
        )
{
    assert(pPara != NULL);
    assert(uSize != 0);
    assert(pPara[0] != ' ');

    switch(m_Status)
    {
    case CMD_NEEDPARA1:
        //the first parameter
        return CheckFirstPara(pPara,uSize,uDone);
        break;
    case CMD_NEEDPARA2:
        //the second parameter
        return CheckSecondPara(pPara,uSize,uDone);
        break;
    case CMD_NEEDDATA:
        //the third parameter
        return CheckDataPara(pPara,uSize,uDone);
        break;
    }

    return TRUE;
}

// -----------------------------------------------------------------------------
// Name:
//      CheckFirstParameter
//
// Function:
//      Check the first parameter
// 
// -----------------------------------------------------------------------------
BOOL CDspFrameBuilder::
CheckFirstPara(
        WORD wPara            //parameter data
        )
{
    BOOL bRet;
    switch(m_wCmdType)
    {
    case RRA:
        //don't need para
        SetErrorCode(BUILDCMD_TOOMANYPARA);
        bRet = FALSE;
        break;

    case RR:
        //need a register name
        m_wFirstPara = CheckRegister(wPara);
        if(m_wFirstPara == BADREG)
        {
            //invalid register name
            SetErrorCode(BUILDCMD_BADREG);
            bRet = FALSE;
        }
        else
        {
            //do not need parameter
            SetErrorCode(BUILDCMD_SUCCESS);
            SetStatus(CMD_FINISHED);
            m_nParaSize = 1;//one byte
            bRet = TRUE;
        }
        break;

    case WR:
        //need a register name
        m_wFirstPara = CheckRegister(wPara);
        if(m_wFirstPara == BADREG)
        {
            //invalid register name
            SetErrorCode(BUILDCMD_BADREG);
            bRet = FALSE;
        }
        else
        {
            //need more para
            SetErrorCode(BUILDCMD_NEEDINFO);
            NextStatus();
            m_nParaSize = 1;//one byte
            bRet = TRUE;
        }
        break;

    case RBD:
    case RBP:
    case WBD:
    case WBP:
        //need an address
        {
            if(CheckAddr(wPara))
            {
                //a valid dsp address
                SetErrorCode(BUILDCMD_NEEDINFO);
                NextStatus();
                m_wFirstPara = wPara;
                m_nParaSize = 2;//two bytes
                bRet = TRUE;
            }
            else
            {
                SetErrorCode(BUILDCMD_BADADDR);
                bRet = FALSE;
            }
        }
        break;
    
    case KEYE:
        {
            m_wFirstPara = wPara;
            SetErrorCode(BUILDCMD_SUCCESS);
            SetStatus(CMD_FINISHED);
            m_nParaSize = 2;//two bytes
            bRet = TRUE;
        }
        break;

    default:
        //?not implement yet
        SetErrorCode(BUILDCMD_SUCCESS);
        SetStatus(CMD_FINISHED);
        bRet = TRUE;
        break;
    }

    return bRet;
}

BOOL CDspFrameBuilder::
CheckFirstPara(
        BYTE* pPara,        //parameter data
        UINT uSize,         //buffer size
        UINT& uDone         //how many byte it have processed
        )
{
    uDone = 0;
    while(uDone < uSize && pPara[uDone] != ' ')
        uDone++;
    
    char* pReg = new char[uDone + 1];
	if (NULL==pReg)
	{
		return FALSE;
	}
    memcpy(pReg,pPara,uDone);
    pReg[uDone] = 0;

    if(m_wCmdType == RR || m_wCmdType == WR)
    {
       //may be a register name
       if(!isdigit(pPara[0]))
       {
           //is a string
           CDSPCmdHelper helper;
           m_wFirstPara = helper.CheckRegister(pReg);
           delete pReg;

           if(m_wFirstPara == BADREG)
           {
               //invalid register name
               SetErrorCode(BUILDCMD_BADREG);
               return FALSE;
           }
           else
           {
               //do not need parameter
               SetErrorCode(BUILDCMD_SUCCESS);
               SetStatus(CMD_FINISHED);
               return TRUE;
           }
       }
    }

    //must a decimal or heximal number
    int base = IsDigit(pReg);
    if(!base)
    {
        //not a valid number
        SetErrorCode(BUILDCMD_BADPARA);
        if (uDone>1)
		{
			delete[] pReg;
		}
		else
		{
			delete pReg;
		}
        return FALSE;
    }
    else if(base == DECIMAL)
    {
        //a decimal digit
        sscanf(pReg,"%d",&m_wFirstPara);
    }
    else //if(base == HEXIMAL
    {
        //a heximal digit
        sscanf(pReg,"%x",&m_wFirstPara);
    }

	if (uDone>1)
	{
		delete[] pReg;
	}
	else
	{
		delete pReg;
	}

    return CheckFirstPara(m_wFirstPara);
}

// -----------------------------------------------------------------------------
// Name:
//      CheckSecondParameter
//
// Function:
//      Check the second parameter
// 
// -----------------------------------------------------------------------------
BOOL CDspFrameBuilder::
CheckSecondPara(
        WORD wPara            //parameter data
        )
{
    BOOL bRet;

    switch(m_wCmdType)
    {
    case RRA:
    case RR:
        SetErrorCode(BUILDCMD_TOOMANYPARA);
        bRet = FALSE;
        break;

    case WR:
        //only need a dword value
        SetErrorCode(BUILDCMD_SUCCESS);
        SetStatus(CMD_FINISHED);
        m_wSecondPara = wPara;
        m_nParaSize = 2;//two bytes
        bRet = TRUE;
        break;

    case RBD:
    case RBP:
        {
            //need the block size
            if(CheckBlockSize(wPara))
            {
                //a valid block size
                SetErrorCode(BUILDCMD_NEEDINFO);
                SetStatus(CMD_FINISHED);
                m_wSecondPara = wPara;
                m_nParaSize = 1;//one bytes
                bRet = TRUE;
            }
            else
            {
                //block size
                SetErrorCode(BUILDCMD_BADBLKSZ);
                bRet = FALSE;
            }
        }
        break;

    case WBD:
    case WBP:
        {
            //need the block size
            if(CheckBlockSize(wPara))
            {
                //a valid block size
                SetErrorCode(BUILDCMD_NEEDINFO);
                NextStatus();
                m_wSecondPara = wPara;
                m_nParaSize = 1;//one bytes
                bRet = TRUE;
            }
            else
            {
                //block size
                SetErrorCode(BUILDCMD_BADBLKSZ);
                bRet = FALSE;
            }
        }
        break;

      default:
        //?not implement yet
        SetErrorCode(BUILDCMD_SUCCESS);
        SetStatus(CMD_FINISHED);
        bRet = TRUE;
        break;
    }

    return bRet;
}

BOOL CDspFrameBuilder::
CheckSecondPara(
        BYTE* pPara,        //parameter data
        UINT uSize,         //buffer size
        UINT& uDone         //how many byte it have processed
        )
{
    uDone = 0;
    while(uDone < uSize && pPara[uDone] != ' ')
        uDone++;
    
    char* pData = new char[uDone + 1];
	if (NULL == pData)
	{
		return FALSE;
	}
    memcpy(pData,pPara,uDone);
    pData[uDone] = 0;

    //must a decimal or heximal number
    int base = IsDigit(pData);
    if(!base)
    {
        //not a valid number
        SetErrorCode(BUILDCMD_BADPARA);
		if (uDone>1)
		{
			delete[] pData;
		}
		else
		{
			delete pData;
		}
        return FALSE;
    }
    else if(base == DECIMAL)
    {
        //a decimal digit
        sscanf(pData,"%d",&m_wFirstPara);
    }
    else //if(base == HEXIMAL
    {
        //a heximal digit
        sscanf(pData,"%x",&m_wFirstPara);
    }

    if (uDone>1)
	{
		delete[] pData;
	}
	else
	{
		delete pData;
	}

    return CheckSecondPara(m_wFirstPara);
}

// -----------------------------------------------------------------------------
// Name:
//      CheckDataPara
//
// Function:
//      Check the last parameter
// 
// -----------------------------------------------------------------------------
BOOL CDspFrameBuilder::
CheckDataPara(
        BYTE* pPara,        //parameter data
        UINT uSize,         //buffer size
        UINT& uDone         //how many byte it have processed
        )
{
    //because it is the last parameter,so all the data
    //pointed by pPara will be processed,and also because
    //the former parameter has given the data size,so the 
    //lenth of the data must match the third parameter
    if(uSize > m_wSecondPara)
    {
        SetErrorCode(BUILDCMD_DATATOOLONG);
        return FALSE;
    }

    uDone = uSize;
    memset(m_aData,0,DATA_SIZE);
    memcpy(m_aData,pPara,uSize);

    SetErrorCode(BUILDCMD_SUCCESS);
    NextStatus();
    AddToCmdString(CMD_NEEDDATA);

    return TRUE;
}
