// CommandHelper.cpp: implementation of the CCommandHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommandHelper.h"

#include "DspFrameBuilder.h"

#pragma warning(push,3)
#include <string>
#pragma warning(pop)
#pragma warning( disable : 4245 4710)

using namespace std;

/*============================================================================*
 *                        static variable definition  
 *============================================================================*/
string CDSPCmdHelper::m_Commands[DSPCMD_NUM] = 
{
		"RRA",
		"RR",
		"RBD",
		"RBP",
		"WR",
		"WBD",
		"WBP",
		"CKS",
		"GO_BOOT",
		"RXINJ",
		"TXINJ",
		"KEYE"
};

string CDSPCmdHelper::m_Comments[DSPCMD_NUM] = 
{
        "Read all registers",
        "Read the value of register",
        "Read a block of DSP data memory",
        "Read a block of DSP program memory",
        "Write the value to register",
        "Write a block of DSP data memory",
        "Write a block of DSP program memory",
        "CKS",
        "GO_BOOT",
        "RXINJ",
        "TXINJ",
        "KEYE"
};

string CDSPCmdHelper::m_Registers[DSPREG_NUM] = 
	{
	    "R0","R1","R2","R3","R4","R5","RB",      
        "LC","ST0","A0H","A0L","ST1","A1H",
		"A1L","PH","PL","PC","SP","ST2",
		"CFGI","CFGJ","ICR","X","Y","B0E",
		"B0H","B0L","B1E","B1H","B1L","SV",
		"DVM","CFGIB","R0B","R1B","R4B",
		"MIXP","REPC","ST0S","ST1S","ST2S",
		"PGSW"    
	};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDSPCmdHelper::CDSPCmdHelper()
{

}

CDSPCmdHelper::~CDSPCmdHelper()
{

}


// -----------------------------------------------------------------------------
// Name:
//      GetCmdComment
//
// Function:
//      Get command describe string
//
//Describe:
//      the nSize parameter shows the length or pComment,if it can't contain
//      the comment,nSize will contain the real size of the comment and the
//      function will return false
// -----------------------------------------------------------------------------
BOOL CDSPCmdHelper::GetCmdComment(                  //success or not
                                    char* pCmd,       //command string,in parameter
                                    char* pComment,   //command commend,out parameter
                                    int&  nSize       //size of the pComment,in-out parameter
                                  )
{
    DSP_CMD_TYPE cmdType = CheckCommand(pCmd);
    if(BADCOMMAND == cmdType)
    {
        nSize = 0;
        return FALSE;
    }

    return GetCmdComment(cmdType,pComment,nSize);
}

// -----------------------------------------------------------------------------
// Name:
//      GetCmdComment
//
// Function:
//      Get command describe string
//
//Describe:
//      the nSize parameter shows the length or pComment,if it can't contain
//      the comment,nSize will contain the real size of the comment and the
//      function will return false
// -----------------------------------------------------------------------------
BOOL CDSPCmdHelper::GetCmdComment(                             //success or not
                                   DSP_CMD_TYPE cmdType,        //command string,in parameter
                                   char* pComment,              //command commend,out parameter
                                   int&  nSize                  //size of the pComment,in-out parameter
                                  )
{
   	//comment size
    int size = m_Comments[cmdType].size();
    
    //buffer enough?
    if(nSize < size || pComment == NULL)
    {
        //not enough,return the size needed
        nSize = size;
        return FALSE;
    }

	m_Comments[cmdType].copy(pComment,size);
	pComment[size] = 0;
    nSize = size;

    return TRUE;
}

// -----------------------------------------------------------------------------
// Name:
//      GetCmdString
//
// Function:
//      Get command string
//
//Describe:
//      the nSize parameter shows the length or pComment,if it can't contain
//      the comment,nSize will contain the real size of the comment and the
//      function will return false
// -----------------------------------------------------------------------------
BOOL CDSPCmdHelper::
GetCmdString(                  //success or not
                       char* pCmd,       //command string,in parameter
                       char* pComment,   //command commend,out parameter
                       int&  nSize       //size of the pComment,in-out parameter
                      )
{
    DSP_CMD_TYPE cmdType = CheckCommand(pCmd);
    if(BADCOMMAND == cmdType)
    {
        nSize = 0;
        return FALSE;
    }

    return GetCmdString(cmdType,pComment,nSize);
}


// -----------------------------------------------------------------------------
// Name:
//      GetCmdString
//
// Function:
//      Get command string
//
//Describe:
//      the nSize parameter shows the length or pComment,if it can't contain
//      the comment,nSize will contain the real size of the comment and the
//      function will return false
// -----------------------------------------------------------------------------
BOOL CDSPCmdHelper::
GetCmdString(                             //success or not
                       DSP_CMD_TYPE cmdType,        //command string,in parameter
                       char* pComment,              //command commend,out parameter
                       int&  nSize                  //size of the pComment,in-out parameter
                      )
{

   	//comment size
    int size = m_Commands[cmdType].size();
    
    //buffer enough?
    if(nSize < size || pComment == NULL)
    {
        //not enough,return the size needed
        nSize = size;
        return FALSE;
    }

	m_Commands[cmdType].copy(pComment,size);
	pComment[size] = 0;
    nSize = size;

    return TRUE;
}


// -----------------------------------------------------------------------------
// Name:
//      CheckCommand
//
// Function:
//      Get Command type
// -----------------------------------------------------------------------------
DSP_CMD_TYPE CDSPCmdHelper::
CheckCommand(
             char* pCommand          //command string
            )
{
    //check parameter
    if(pCommand == NULL)
        return BADCOMMAND;

    for(int i=0;i<DSPCMD_NUM;i++)
	{
		if(!m_Commands[i].compare(pCommand))
			return DSP_CMD_TYPE(i);
	}

    return BADCOMMAND;
}

DSP_CMD_TYPE CDSPCmdHelper::
CheckCommand(
             WORD wCmd             //command index
            )
{
    if(wCmd >= 0 || wCmd < DSPCMD_NUM)
        return DSP_CMD_TYPE(wCmd);
    else
        return BADCOMMAND;
}


// -----------------------------------------------------------------------------
// Name:
//      CheckRegister
//
// Function:
//      Get register name
// -----------------------------------------------------------------------------
BOOL CDSPCmdHelper::CheckRegName(
                                 char* pRegName,     //register name
                                 DWORD& dwRegType    //register type returned
                                 )
{
   	//check parameter
    if(pRegName == NULL)
		return FALSE;
	
	for(int i=0;i<DSPREG_NUM;i++)
	{
		if(!m_Registers[i].compare(pRegName))
        {
            dwRegType = i;
            return TRUE;  //find a match
        }
	}

	//no match
	return FALSE;
}

// -----------------------------------------------------------------------------
// Name:
//      CheckRegister
//
// Function:
//      Check if the parameter is a valid DSP register name
// 
// -----------------------------------------------------------------------------
DSP_REG_TYPE CDSPCmdHelper::
CheckRegister(
        WORD wReg         //register index
        )
{
    if(wReg >= 0 && wReg < DSPREG_NUM)
        return DSP_REG_TYPE(wReg);
    else
        return BADREG;
}

DSP_REG_TYPE CDSPCmdHelper::
CheckRegister(
        char* pRegName         //regitster name string
        )
{
  	//check parameter
    if(pRegName == NULL)
		return BADREG;
	
	for(int i=0;i<DSPREG_NUM;i++)
	{
		if(!m_Registers[i].compare(pRegName))
        {
            return DSP_REG_TYPE(i);     //find a match
        }
	}

    return BADREG;
}

// -----------------------------------------------------------------------------
// Name:
//      GetRegName
//
// Function:
//      Get register name
// -----------------------------------------------------------------------------
BOOL CDSPCmdHelper::GetRegName(
                    DSP_REG_TYPE regType,   //register type
                    char* pRegName,         //buffer to contain register name
                    int& nSize              //size of the buffer
                   )
{
    int size = m_Registers[regType].size();
    
    //buffer enough?
    if(nSize < size || pRegName == NULL)
    {
        //not enough,return the size needed
        nSize = size;
        return FALSE;
    }

	m_Registers[regType].copy(pRegName,size);
	pRegName[size] = 0;
    nSize = size;

    return TRUE;

}

// -----------------------------------------------------------------------------
// Name:
//      FormatResponse
//
// Function:
//      Change binary response data to a readable string
//
//Describe:
//      In the function a piece of buffer will be allocated to
//      contain the formatted string,
//      and the pointer to the buffer will be placed in the 
//      address pComment points to. The caller should call
//      free function to free the buffer after use.
// -----------------------------------------------------------------------------
BOOL CDSPCmdHelper::
FormatResponse(                                 // Success or not
               CDspFrameBuilder* pBuilder,        // Command parameter
               char** pComment,                  // pointer to the formatted string
               char* szRes                       // Response string
               )
{
    string strFormated;

    switch(pBuilder->GetCmdType())
    {
    case RRA:
        {
            for(int i=0;i<DSPREG_NUM;i++)
            {
                strFormated += m_Registers[i];
                strFormated += " = 0x";
                strFormated += szRes[i*4+2];
                strFormated += szRes[i*4+3];                
                strFormated += szRes[i*4+0];
                strFormated += szRes[i*4+1];

                if(i % 8 != 0)
                {
                    strFormated += "\n";
                }
            }
        }
        break;

    case RR:
        {
            strFormated += m_Registers[pBuilder->GetFirstPara()];
            strFormated += " = 0x";
            strFormated += szRes[2];
            strFormated += szRes[3];
            strFormated += szRes[0];
            strFormated += szRes[1];
            strFormated += "\n";
        }
        break;

    case RBD:
    case RBP:
        {
            char szAddr[11];
            DWORD dwAddr = pBuilder->GetFirstPara();
            int nSize = pBuilder->GetSecondPara();
            int nReadNum = 0;
            while(nSize > 0)
            {
                sprintf(szAddr,"0x%08x  ",dwAddr);
                strFormated += szAddr;
                int num = nSize;
                if(nSize > 8)
                {
                    num = 8;
                }
                for(int i=0;i<num;i++)
                {
                    strFormated += szRes[nReadNum++];
                    strFormated += szRes[nReadNum++];
                    strFormated += ' ';
                }
                strFormated += "\n";

                dwAddr += num;
                nSize -= num;
            }
        }
        break;

    default:
        {
            strFormated = "Command execute success!";
        }
    }

    *pComment = new char[strFormated.size()];
	if (*pComment)
	{
		strFormated.copy(*pComment,strFormated.size());
	}
    

    return TRUE;
}


// -----------------------------------------------------------------------------
// Name:
//      FormatCommand
//
// Function:
//      Change binary command data to a readable string
//
//Describe:
//      In the function a piece of buffer will be allocated to
//      contain the formatted string,
//      and the pointer to the buffer will be placed in the 
//      address pComment points to. The caller should call
//      free function to free the buffer after use.
// -----------------------------------------------------------------------------
BOOL CDSPCmdHelper::
FormatCommand(                                 // Success or not
              CDspFrameBuilder* pBuilder,      // Command parameter
              char** pComment                  // pointer to the formatted string
              )
{
    string szCmd;

    szCmd += m_Commands[pBuilder->GetCmdType()];
    szCmd += ' ';
    
    switch(pBuilder->GetCmdType())
    {
    case RRA:
        {
        }
        break;
        
    case RR:
        {
            szCmd += m_Registers[pBuilder->GetFirstPara()];
        }
        break;

    case WR:
        {
            szCmd += m_Registers[pBuilder->GetFirstPara()];
            szCmd += " 0x";
            char szValue[3];
            sprintf(szValue,"%x",HIBYTE(pBuilder->GetSecondPara()));
            szCmd += szValue;
            sprintf(szValue,"%x",LOBYTE(pBuilder->GetSecondPara()));
            szCmd += szValue;
        }
        break;

    case RBD:
    case RBP:
        {
            char szValue[11];
            sprintf(szValue,"0x%08x ",pBuilder->GetFirstPara());
            szCmd += szValue;
            sprintf(szValue,"0x%x\n",pBuilder->GetSecondPara());
            szCmd += szValue;
        }
        break;

    case WBP:
    case WBD:
        {
            char szValue[11];
            sprintf(szValue,"0x%08x ",pBuilder->GetFirstPara());
            szCmd += szValue;
            sprintf(szValue,"0x%x\n",pBuilder->GetSecondPara());
            szCmd += szValue;

            BYTE* pData = (BYTE*)pBuilder->GetLastPara();
            for(int i=0;i<pBuilder->GetSecondPara();i++)
            {
                sprintf(szValue,"%x ",pData[i]);
                szCmd += szValue;
                if(i % 8 == 0)
                {
                    szCmd += "\n";
                }
            }
        }
        break;

    default:
        return FALSE;
    }

    szCmd += "\n";

    *pComment = new char[szCmd.size()];
	if (*pComment)
	{
		szCmd.copy(*pComment,szCmd.size());
	}
    

    return TRUE;
}
