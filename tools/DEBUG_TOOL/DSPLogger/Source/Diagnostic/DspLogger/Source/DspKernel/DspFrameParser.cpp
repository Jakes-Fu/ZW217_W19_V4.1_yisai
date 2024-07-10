/*============================================================================*
 *
 *                          Copyright (c) 2002 Spreadtrum
 *                              All Rights Reserved
 *
 *-----------------------------------------------------------------------------
 *
 * File name:
 *      CDspFrameParser.cpp
 *
 * Description:
 *      This .cpp file provides the implementation of all the exported functions
 *      of CDspFrameParser
 * 
 *============================================================================*/


/*============================================================================*
 *                          Edit History
 *-----------------------------------------------------------------------------
 * 2002-11-13   Kai Liu     Created
 *============================================================================*/

#include "stdafx.h"
#include "DspFrameParser.h"

/*============================================================================*
 *                         Global functions
 *============================================================================*/
void* BufferToString(
    void  *pMemBlock,
    int   nSize
)
{
    assert( NULL != pMemBlock );
    assert( 0 < nSize );

    char* pBuf = new char[nSize * 2 + 1];

    for( int i = 0; i < nSize && pBuf; i++ )
    {
        sprintf(pBuf + i * 2, "%02X", ((BYTE *)pMemBlock)[i] );
    }

    return pBuf;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDspFrameParser::CDspFrameParser()
{
    m_hRcvWnd = NULL;

    m_dwGuiMsgId = 0;
}

CDspFrameParser::~CDspFrameParser()
{

}


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
BOOL CDspFrameParser::ParseFrame(   // How many bytes parsed,0 is error
				DspFrame* pFrame    // The frame pointer
				)
{
	if(pFrame == NULL)
		return 0;

    // Search header
    if(pFrame->frame[0] != 0x7e || pFrame->frame[1] != 0x7e)
    {
        int nCount = DSP_FRM_SIZE - 2;
        BYTE *p = pFrame->frame + 2;
        while(1)
        {
            p = (BYTE*)memchr(p,0x7e,nCount);
            if(p == NULL)
            {
                break;
            }
            if((p - pFrame->frame) == (DSP_FRM_SIZE - 1))
            {
                break;
            }
            p++;
            if(*p != 0x7e)
            {
                p++;
            }
            else
            {
                p--;
                break;
            }
            nCount = DSP_FRM_SIZE - (p - pFrame->frame);
        }

        if(p == NULL)
        {
            return DSP_FRM_SIZE;
        }
        else
        {
            return (p - pFrame->frame);
        }
    }

    // Only test point information 
    BYTE* pData = pFrame->frame + 2;
    BYTE* pEnd = pFrame->frame + DSP_FRM_SIZE;

    while(pData < pEnd)
    {
        ParsedInfo* pInfo = new ParsedInfo;
		if (pInfo)
		{
			BOOL bRet = ParseTstPtFrm(&pData,pInfo);
			
			if(bRet)
			{
				NotifyGui(pInfo);
			}
			else
			{
				delete pInfo;
				break;
			}
		}
        
    }

    return pData - pFrame->frame + 2;

/***********************************************************
// Now dsp only send Test point out,so remark this code

    switch(pFrame->frame[0])
    {
    case OTHER_INFO_TYPE:
        {
            //key information or test point
            
            //jump to the message start
            BYTE* pData = (BYTE*)pFrame;
            BYTE* pEnd = pData + DSP_FRM_SIZE;
            pData += 2;
            
            //parse every message
            while(pData < pEnd)
            {
                ParsedInfo* pInfo = new ParsedInfo;
                BOOL bRet;
                switch(pData[0])
                {
                case TEST_PT_TYPE:     //test point message
                    {    
                        bRet = ParseTstPtFrm(&pData,pInfo);
                    }
                    break;
                case KEY_INFO_TYPE:    //key information message
                    {
                        bRet = ParseKeyFrm(&pData,pInfo);
                    }
                    break;
                default:
                    {
                        bRet = FALSE;
                    }
                }
                
                
                if(bRet)
                {
                    //parsed success
                    //gui will free pInfo
                    NotifyGui(pInfo);
                }
                else
                {
                    //parsed failed,free resource
                    //and discard this frame
                    delete pInfo;
                    return DSP_FRM_SIZE;
                }        
                
            }
            break;
        }

    case CMD_RSP_TYPE:
        {
            ParsedInfo* pInfo = new ParsedInfo;
            BOOL bRet;
            bRet = ParseResFrm(&pFrame,pInfo);
            if(bRet)
            {
                //parsed success
                //gui will free pInfo
                NotifyGui(pInfo);
            }
            else
            {
                //parsed failed,free resource
                //and discard this frame
                delete pInfo;
                return DSP_FRM_SIZE;
            }                    
        }
        break;

    }

	return DSP_FRM_SIZE;
*************************************************************/
}

// -----------------------------------------------------------------------------
// Name:
//      ParseResFrm
//
// Function:
//      Parse a Dsp command respond Frame
// 
// 
// -----------------------------------------------------------------------------
int CDspFrameParser::ParseResFrm(   //the message length
		DspFrame** ppFrame,            //the frame pointer
        ParsedInfo* pInfo     //the parsed info pointer
		)
{
    pInfo->dwType = CMD_RSP_TYPE;
    pInfo->dwSubType = (*ppFrame)->frame[1];
    //command id
    switch(pInfo->dwSubType)
    {
    case RRA:
        {
            pInfo->dwData = (DWORD)::BufferToString((void*)((*ppFrame)->frame+2),86);
        }
        break;

    case RR:
        {
            pInfo->dwData = (DWORD)::BufferToString((void*)((*ppFrame)->frame+2),2);
        }
        break;

    case RBD:
    case RBP:
        {
            pInfo->dwData = (DWORD)::BufferToString((void*)((*ppFrame)->frame+2),DSP_FRM_SIZE - 3);
        }
        break;

    case WR:
    case WBD:
    case WBP:
    case GO_BOOT:
    default:
        {
            //just tell that command was executed
            char* pBuf = new char[5];
			if (pBuf)
			{
				strcpy(pBuf,"Done");
				pInfo->dwData = (DWORD)pBuf;
			}
            
        }
        break;

    }
	return TRUE;
}

// -----------------------------------------------------------------------------
// Name:
//      ParseTstPtFrm
//
// Function:
//      Parse a Dsp test point Frame
// 
// -----------------------------------------------------------------------------
BOOL CDspFrameParser::ParseTstPtFrm( //the message length
		BYTE** ppFrame,               //the frame pointer
  		ParsedInfo* pInfo           //the parsed info pointer
		)
{
    BYTE* pFrame = *ppFrame;

    pInfo->dwType = OTHER_INFO_TYPE;
    pInfo->dwSubType = TEST_PT_TYPE;
    
//  pInfo->dwTime = pFrame[2] << 8 | pFrame[3];

    pInfo->dwAddr = pFrame[0] << 8 | pFrame[1];
    
    pInfo->dwData = pFrame[2] << 8 | pFrame[3];

    //*ppFrame += TEST_PT_SIZE;
    

    if(pInfo->dwAddr == 0x7e7e)
    {
        return FALSE;
    }
    else
    {
        *ppFrame += 4;
        return TRUE;
    }
}

// -----------------------------------------------------------------------------
// Name:
//      ParseKeyFrm
//
// Function:
//      Parse a Dsp key information Frame
// 
// -----------------------------------------------------------------------------
BOOL CDspFrameParser::ParseKeyFrm(   //the message length
		BYTE** ppFrame,              //the frame pointer
  		ParsedInfo* pInfo            //the parsed info pointer
		)
{
    BYTE* pFrame = *ppFrame;

    pInfo->dwType = OTHER_INFO_TYPE;
    pInfo->dwSubType = KEY_INFO_TYPE;
	pInfo->dwTaskId = pFrame[1];
	pInfo->dwTime = pFrame[2] << 8 | pFrame[3];

    *ppFrame += 4;

    //parse the message content area
    return ParseTypedKeyFrm(ppFrame,pInfo,FALSE,TRUE);
}

// -----------------------------------------------------------------------------
// Name:
//      ParseTypedKeyFrm
//
// Function:
//      Parse a key information Frame without time and type field
//
// Note:
//      This function only can be called from out of kernel to parse
//      subtype of the key information frame,so the pInfo->dwTaskId 
//      must contain valid key information's task id
// -----------------------------------------------------------------------------
BOOL CDspFrameParser::ParseTypedKeyFrm(
		BYTE** ppFrame,    //the frame pointer
   		ParsedInfo* pInfo     //the parsed info pointer
		)
{
    return ParseTypedKeyFrm(ppFrame,pInfo,TRUE,FALSE);
}


// -----------------------------------------------------------------------------
// Name:
//      ParseTypedKeyFrm
//
// Function:
//      Parse a key information Frame without time and type field
// 
// -----------------------------------------------------------------------------
BOOL CDspFrameParser::ParseTypedKeyFrm(
		BYTE** ppFrame,         //the frame pointer
   		ParsedInfo* pInfo,      //the parsed info pointer
        BOOL bParseSubtype,     //whether parse subtype information
        BOOL bCopyBuffer        //whether copy message to the output buffer
	)
{
    BYTE* pFrame = *ppFrame;

    switch(pInfo->dwTaskId)
    {
    case KEY_INTER_TYPE:
    case KEY_FG_TYPE:
        {
            //interrupt and foreground are the same
            if(bParseSubtype)
            {
                ParseFgtaskFrm(pFrame,pInfo);
            }
            if(bCopyBuffer)
            {
                pInfo->dwAddr = KEY_FG_MSG_SIZE;
                memcpy((void*)pInfo->pMsgContent,(const void*)pFrame,pInfo->dwAddr);
                *ppFrame += KEY_FG_MSG_SIZE;
            }
        }
        break;
        
    case KEY_BG_TYPE:
        {
            //background
            if(bParseSubtype)
            {
                ParseBgtaskFrm(pFrame,pInfo);
            }
            if(bCopyBuffer)
            {
                pInfo->dwAddr = KEY_BG_MSG_SIZE;
                memcpy((void*)pInfo->pMsgContent,(const void*)pFrame,pInfo->dwAddr);
                *ppFrame += KEY_BG_MSG_SIZE;
            }
        }
        break;
        
    case KEY_FRM_TYPE:
        {
            //frame instruction
            if(bParseSubtype)
            {
                ParseFrmInstFrm(pFrame,pInfo);
            }
            if(bCopyBuffer)
            {
                pInfo->dwAddr = KEY_FRM_MSG_SIZE;
                memcpy((void*)pInfo->pMsgContent,(const void*)pFrame,pInfo->dwAddr);
                *ppFrame += KEY_FRM_MSG_SIZE;
            }
        }
        break;
        
    case KEY_MCU_TYPE:
        {
            //MCU command
            if(bParseSubtype)
            {
                ParseMcuFrm(pFrame,pInfo);
            }
            if(bCopyBuffer)
            {
                pInfo->dwAddr = KEY_MCU_MSG_SIZE;
                memcpy((void*)pInfo->pMsgContent,(const void*)pFrame,pInfo->dwAddr);
                *ppFrame += KEY_MCU_MSG_SIZE;
            }
        }
        break;

    case KEY_RSSI_TYPE:
    case KEY_FRE_TYPE:
        {
            //RSSI and frequency are the same
            if(bParseSubtype)
            {
                ParseRssiFrm(pFrame,pInfo);
            }
            if(bCopyBuffer)
            {
                pInfo->dwAddr = KEY_RSSI_MSG_SIZE;
                memcpy((void*)pInfo->pMsgContent,(const void*)pFrame,pInfo->dwAddr);
                *ppFrame += KEY_RSSI_MSG_SIZE;
            }
        }
        break;

    default:
        return FALSE;
        break;
    }

    return TRUE;
}

// -----------------------------------------------------------------------------
// Name:
//      ParseFgtaskFrm
//
// Function:
//      Parse a foreground task key information Frame
// 
// -----------------------------------------------------------------------------
int CDspFrameParser::ParseFgtaskFrm(    //the message length
		BYTE* pFrame,               //the frame pointer
		ParsedInfo* pInfo               //the parsed info pointer
		)
{
    //parse the data bitwise
    BYTE temp = pFrame[0];

    pInfo->stFgTask.run_timer_task = (temp >>= 2);
    
    pInfo->stFgTask.run_mcu_to_dsp_task = (temp >>= 1);
    
    pInfo->stFgTask.run_tx_multislot_task = (temp >>= 1);

    pInfo->stFgTask.run_rx_dispatch_task = (temp >>= 1);

    pInfo->stFgTask.run_rx_deinterleaver_task = (temp >>= 1);

    pInfo->stFgTask.run_rx_task = (temp >> 1);

    temp = pFrame[1];
    pInfo->stFgTask.run_tx_interleaver_task = temp;

    pInfo->stFgTask.run_frame_eng_task = (temp >>= 1);
  
    pInfo->stFgTask.run_uart_fg_task = (temp >>= 1);

    pInfo->stFgTask.run_tx_encoder_task = (temp >>= 1);

    pInfo->stFgTask.fg_dummy_task1 = (temp >>= 1);
    
    return TRUE;
}

// -----------------------------------------------------------------------------
// Name:
//      ParseBgtaskFrm
//
// Function:
//      Parse a background task key information Frame
// 
// -----------------------------------------------------------------------------
int CDspFrameParser::ParseBgtaskFrm(    //the message length
		BYTE* pFrame,               //the frame pointer
		ParsedInfo* pInfo               //the parsed info pointer
		)
{
    //parse the data bitwise
    BYTE temp = pFrame[0];

    pInfo->stBgTask.run_uart_bg_task = (temp >>= 2);

    pInfo->stBgTask.run_tone_bg_task = (temp >>= 5);

    temp = pFrame[1];
    pInfo->stBgTask.run_vocoder_enc_task = temp;

    pInfo->stBgTask.run_vocoder_dec_task = (temp >>= 1);

    return TRUE;
}


// -----------------------------------------------------------------------------
// Name:
//      ParseFrmInstFrm
//
// Function:
//      Parse a frame instruction key information Frame
// 
// -----------------------------------------------------------------------------
int CDspFrameParser::ParseFrmInstFrm(   //the message length
		BYTE* pFrame,               //the frame pointer
		ParsedInfo* pInfo               //the parsed info pointer
		)
{
    //nine words

    //the first word
    DWORD dwTemp = pFrame[0] << 8 | pFrame[1];
    pInfo->stFrmInst.RFOperation = dwTemp;
    pInfo->stFrmInst.CellIndex = (dwTemp >>= 5);
    pInfo->stFrmInst.FrameType = (dwTemp >>= 4);

    
    //the second word
    dwTemp = pFrame[2] << 8 | pFrame[3];
    pInfo->stFrmInst.RxBurstNumber = dwTemp;
    pInfo->stFrmInst.TxBurstNumber = (dwTemp >>= 2);
    pInfo->stFrmInst.RxChannelTag = (dwTemp >>= 2);
    pInfo->stFrmInst.fn1 = (dwTemp >>= 4);

    
    //the third word
    pInfo->stFrmInst.fn2 =
        pFrame[4] << 8 | pFrame[5];

    
    //the forth word
    dwTemp = pFrame[6] << 8 | pFrame[7];
    DWORD dwTemp1 = dwTemp;
    pInfo->stFrmInst.Offset3_u.Offset3_T1.Searching_count = dwTemp;
    pInfo->stFrmInst.Offset3_u.Offset3_T1.TxSendBufferIndex = (dwTemp >>= 12);
    pInfo->stFrmInst.Offset3_u.Offset3_T2.TxSlotMask = dwTemp1;
    pInfo->stFrmInst.Offset3_u.Offset3_T2.FirstRxSlot = (dwTemp1 >>= 8);
    pInfo->stFrmInst.Offset3_u.Offset3_T2.Empty1 = 0;

    
    //the fifth word
    dwTemp = pFrame[8] << 8 | pFrame[9];
    pInfo->stFrmInst.RxSlotMask = dwTemp;
    pInfo->stFrmInst.TSC = (dwTemp >>= 8);
    pInfo->stFrmInst.USFMode = (dwTemp >>= 4);
    pInfo->stFrmInst.TAF = (dwTemp >>= 1);
    pInfo->stFrmInst.DTX = (dwTemp >>= 1);
    pInfo->stFrmInst.EndMR = (dwTemp >>= 1);

    
    //the sixth word
	dwTemp = pFrame[10] << 8 | pFrame[11];
    pInfo->stFrmInst.Empty2 = dwTemp;
    pInfo->stFrmInst.TAUpdatedFlag = (dwTemp >>= 6);
    pInfo->stFrmInst.TAUpdatedIndex = (dwTemp >>= 1);
    pInfo->stFrmInst.PowerUpdateFlag = (dwTemp >>= 2);
    pInfo->stFrmInst.PowerUpdateIndex = (dwTemp >>= 1);
    pInfo->stFrmInst.USFGranuarity = (dwTemp >>= 2);
    pInfo->stFrmInst.ExtDyn = (dwTemp >>= 1);
    pInfo->stFrmInst.ChModeUpdate = (dwTemp >>= 1);
    pInfo->stFrmInst.USFValueUpdate = (dwTemp >>= 1);

	
    //the seventh word
    dwTemp = pFrame[12] << 8 | pFrame[13];
    pInfo->stFrmInst.ARFCN0 = dwTemp;
    pInfo->stFrmInst.GainIndex0 = (dwTemp >>= 10);

    
    //the eighth word
    dwTemp = pFrame[14] << 8 | pFrame[15];
    pInfo->stFrmInst.ARFCN1 = dwTemp;
    pInfo->stFrmInst.GainIndex1 = (dwTemp >>= 10);


    //the ninth word
    dwTemp = pFrame[16] << 8 | pFrame[17];
    pInfo->stFrmInst.ARFCN2 = dwTemp;
    pInfo->stFrmInst.GainIndex2 = (dwTemp >>= 10);

    return TRUE;
}

// -----------------------------------------------------------------------------
// Name:
//      ParseMcuFrm
//
// Function:
//      Parse a MCU command key information Frame
// 
// -----------------------------------------------------------------------------
int CDspFrameParser::ParseMcuFrm(   //the message length
		BYTE* pFrame,           //the frame pointer
		ParsedInfo* pInfo           //the parsed info pointer
		)
{
    //assign the value memberwise
    pInfo->stMcuCmd.Command_Queue_ID = 
        pFrame[0] << 8 | pFrame[1];

    pInfo->stMcuCmd.parameter0 =
        pFrame[2] << 8 | pFrame[3];
    
    pInfo->stMcuCmd.parameter1 =
        pFrame[4] << 8 | pFrame[5];
    
    pInfo->stMcuCmd.parameter2 =
        pFrame[6] << 8 | pFrame[7];
    
    pInfo->stMcuCmd.parameter3 =
        pFrame[8] << 8 | pFrame[9];
    
    return TRUE;
}

// -----------------------------------------------------------------------------
// Name:
//      ParseRssiFrm
//
// Function:
//      Parse a RSSI key information Frame
// 
// -----------------------------------------------------------------------------
inline int CDspFrameParser::ParseRssiFrm(   //the message length
		BYTE* pFrame,                   //the frame pointer
		ParsedInfo* pInfo                   //the parsed info pointer
		)
{
    //just a 16 bit value
    pInfo->dwData = pFrame[0] << 8 | pFrame[1];
    return TRUE;
}