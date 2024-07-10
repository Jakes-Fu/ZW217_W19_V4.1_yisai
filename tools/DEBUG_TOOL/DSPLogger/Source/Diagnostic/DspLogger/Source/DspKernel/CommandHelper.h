/*============================================================================*
 *
 *                          Copyright (c) 2002 Spreadtrum
 *                              All Rights Reserved
 *
 *-----------------------------------------------------------------------------
 *
 * File name:
 *      CommandHelper.h
 *
 * Description:
 *      This header file provides the declarations of all the functions
 *      of CCommandHelper
 *
 * 
 *============================================================================*/


/*============================================================================*
 *                          Edit History
 *-----------------------------------------------------------------------------
 * 2002-11-26   Kai Liu     Create
 *============================================================================*/


#if !defined(AFX_COMMANDHELPER_H__25160A56_DFD6_4C74_98F7_930920E0FB3F__INCLUDED_)
#define AFX_COMMANDHELPER_H__25160A56_DFD6_4C74_98F7_930920E0FB3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DspKernel.h"
class CDspFrameBuilder;

class CDSPCmdHelper  
{
public:
	CDSPCmdHelper();
	virtual ~CDSPCmdHelper();

public:
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
    BOOL GetCmdComment(                  //success or not
                       char* pCmd,       //command string,in parameter
                       char* pComment,   //command commend,out parameter
                       int&  nSize       //size of the pComment,in-out parameter
                      );

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
    BOOL GetCmdComment(                             //success or not
                       DSP_CMD_TYPE cmdType,        //command string,in parameter
                       char* pComment,              //command commend,out parameter
                       int&  nSize                  //size of the pComment,in-out parameter
                      );

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
    BOOL GetCmdString(                  //success or not
                       char* pCmd,       //command string,in parameter
                       char* pComment,   //command commend,out parameter
                       int&  nSize       //size of the pComment,in-out parameter
                      );

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
    BOOL GetCmdString(                             //success or not
                       DSP_CMD_TYPE cmdType,        //command string,in parameter
                       char* pComment,              //command commend,out parameter
                       int&  nSize                  //size of the pComment,in-out parameter
                      );


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
    BOOL FormatResponse(                                 // Success or not
                       CDspFrameBuilder* pBuilder,        // Command parameter
                       char** pComment,                  // pointer to the formatted string
                       char* szRes                       // Response string
                      );

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
    BOOL FormatCommand(                                 // Success or not
                       CDspFrameBuilder* pBuilder,        // Command parameter
                       char** pComment                  // pointer to the formatted string
                      );


// -----------------------------------------------------------------------------
// Name:
//      CheckCommand
//
// Function:
//      Get Command type
// -----------------------------------------------------------------------------
        DSP_CMD_TYPE CheckCommand(
                        char* pCommand          //command string
                        );

        DSP_CMD_TYPE CheckCommand(
                        WORD wCmd             //command index
                        );

// -----------------------------------------------------------------------------
// Name:
//      CheckRegister
//
// Function:
//      Get register type
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

    DSP_REG_TYPE CheckRegister(
        char* pRegName         //regitster name string
        );


// -----------------------------------------------------------------------------
// Name:
//      GetRegName
//
// Function:
//      Get register name
// -----------------------------------------------------------------------------
    BOOL GetRegName(
                    DSP_REG_TYPE regType,   //register type
                    char* pRegName,         //buffer to contain register name
                    int& nSize              //size of the buffer
                   );

    
// -----------------------------------------------------------------------------
// Name:
//      FreeBuffer
//
// Function:
//      Free buffer allocated before
//
// Description:
//      Client call this function to free the buffer
//      allocated in the FormatResponse function.
// -----------------------------------------------------------------------------
    void FreeBuffer(char* pBuf)
    {
        delete pBuf;
    }

protected:
    //command name array
	static string m_Commands[DSPCMD_NUM];

    //comment array
    static string m_Comments[DSPCMD_NUM];

  	//register name array
	static string m_Registers[DSPREG_NUM];
};

#endif // !defined(AFX_COMMANDHELPER_H__25160A56_DFD6_4C74_98F7_930920E0FB3F__INCLUDED_)
