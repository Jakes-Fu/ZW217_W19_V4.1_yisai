// MessageList.cpp: implementation of the CMessageList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChannelServer.h"
#include "MessageList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//CMessageList is used to save the request and response message(Req_Rep) casually
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMessageList::CMessageList()
{
   m_nListNodeCount = 0;
   m_pListHead      = NULL;
   m_pListTail      = NULL;
   //m_hListLock      = CreateEvent(NULL,TRUE,TRUE,NULL);

   m_nTimeouts     = MSG_DELAY_TIME;
   
   m_hListLock = NULL;
}

CMessageList::~CMessageList()
{
    
    RemoveAll();

    if(NULL != m_pListHead )
    {
        delete m_pListHead;
    }
    
    m_pListHead      = NULL;
    m_pListTail      = NULL;
    m_nListNodeCount = 0;
    
    //CloseHandle(m_hListLock);
}

//Initialize the message pool
//Create message pool header, message is duplicate round list
BOOL CMessageList::InitMessagePool()
{
    COMMONMSG_PTR   pTempListNode;

	//Initilize the list
	//add a empty node to list
    pTempListNode = (COMMONMSG_PTR)new char[sizeof(COMMONMSG)];
	if(pTempListNode == NULL)
		return FALSE;

	ZeroMemory(pTempListNode,sizeof(COMMONMSG));    
    pTempListNode->pPrevNode      = pTempListNode;
    pTempListNode->pNextNode      = pTempListNode;
            
	m_pListHead      = m_pListTail = pTempListNode;
    m_nListNodeCount = 0;

	return TRUE;
}

//Add message to message pool
//return the respond message sequence number
int CMessageList::AddMessage(COMMONMSG_PTR pMessage)
{
    //添加客户端连接信息
    COMMONMSG_PTR       pAddListNode ;
    Protocol_Header_Ptr pPktHeader;
    WORD                sequenceNum;
    
//    WaitForSingleObject(m_hListLock, INFINITE);
//    ResetEvent(m_hListLock);

    pAddListNode = pMessage;
    pPktHeader   = (Protocol_Header_Ptr)pAddListNode->lpData ;


    //Check the message pool isn't full
    if(MAX_MSG_POOL_SIZE == m_nListNodeCount)
    {
        pPktHeader->sequenceNum = (DWORD)INVALID_MSG_SEQ;
        
//        SetEvent(m_hListLock);

        return INVALID_MSG_SEQ;
    }    

    /*
        
    //the message is request & response type
    //sequence number is range from 0x11 to 0x100
    if( (MSG_REQ_REP_START <= pPktHeader->type) && 
        (MSG_REQ_START > pPktHeader->type) )
    {      
        //the pool header's nResponseId is used as 
        //the sequence number counter for request&response message
        sequenceNum = m_pListHead->nResponseId;
        
        sequenceNum = (sequenceNum) % MAX_REQREP_INPOOL + 1;
        if(MIN_PKT_SEQ == sequenceNum)
        {    
            sequenceNum = sequenceNum + MAX_REQ_INPOOL;
        }
        
        //save the packet sequence number counter
        m_pListHead->nResponseId = sequenceNum;
        
    }
    //the message is request type
    //sequence number is range from 0x1 to 0x10
    else if( (MSG_REQ_START <= pPktHeader->type) && 
             (MSG_ANONY_START > pPktHeader->type) )
    {
        //the pool header's nResponseId is used as 
        //the sequence number counter for request message
        sequenceNum = m_pListHead->nRepeatCount;
        
        sequenceNum = (sequenceNum ) % MAX_REQ_INPOOL + 1;
        
        m_pListHead->nRepeatCount = sequenceNum;
    }
*/

    sequenceNum = (WORD)m_pListHead->nResponseId;    
    sequenceNum++;    
    //save the packet sequence number counter
    m_pListHead->nResponseId = sequenceNum;

    //Fill the packet sequence number 
    pPktHeader->sequenceNum = sequenceNum;
                  
    
    //Add the new client to tailor
    pAddListNode ->pPrevNode = m_pListTail;
    pAddListNode ->pNextNode = m_pListHead;
    m_pListTail->pNextNode   = pAddListNode;
    m_pListHead->pPrevNode   = pAddListNode;
    m_pListTail = pAddListNode;

    m_nListNodeCount = m_nListNodeCount++;
    
//    SetEvent(m_hListLock);
    
    return sequenceNum;
}

//Remove Message from message popl
int CMessageList::DeleteMessage(int sequenceNum)
{
    //删除客户端连接信息列表
    //添加客户端连接信息
    COMMONMSG_PTR  pListHeadNode=NULL;
	COMMONMSG_PTR  pDeleListNode=NULL;    
    int            nClientID = 0;

//    WaitForSingleObject(m_hListLock, INFINITE);
//    ResetEvent(m_hListLock);

	pListHeadNode = m_pListHead;
	pDeleListNode = pListHeadNode->pNextNode ;

	while(pDeleListNode!=pListHeadNode)
	{
		if(((Protocol_Header_Ptr)pDeleListNode->lpData)->sequenceNum  != (DWORD)sequenceNum)
		{
            pDeleListNode=pDeleListNode->pNextNode;
			continue;
		}

		nClientID = pDeleListNode->nResponseId ;
        pDeleListNode->pPrevNode ->pNextNode = pDeleListNode->pNextNode ;
		pDeleListNode->pNextNode ->pPrevNode = pDeleListNode->pPrevNode ;
                
        m_pListTail = m_pListHead->pPrevNode;
        m_nListNodeCount--;

        //destory message content buffer
        delete pDeleListNode;
        

//        SetEvent(m_hListLock);

		return nClientID;
	}
    
//    SetEvent(m_hListLock);    

	return 0;
}

int CMessageList::DeleteCurMessage( byte btType )
{
    COMMONMSG_PTR  pListHeadNode=NULL;
	COMMONMSG_PTR  pDeleListNode=NULL;    
    int            nClientID = 0;

//    WaitForSingleObject(m_hListLock, INFINITE);
//    ResetEvent(m_hListLock);

	pListHeadNode = m_pListHead;
	pDeleListNode = pListHeadNode->pNextNode ;

	while(pDeleListNode!=pListHeadNode)
	{
		if(((Protocol_Header_Ptr)pDeleListNode->lpData)->type  != btType )
		{
            pDeleListNode=pDeleListNode->pNextNode;
			continue;
		}

		nClientID = pDeleListNode->nResponseId ;
        pDeleListNode->pPrevNode ->pNextNode = pDeleListNode->pNextNode ;
		pDeleListNode->pNextNode ->pPrevNode = pDeleListNode->pPrevNode ;
                
        m_pListTail = m_pListHead->pPrevNode;
        m_nListNodeCount--;

        //destory message content buffer
        delete pDeleListNode;
        

//        SetEvent(m_hListLock);

		return nClientID;
	}
    
//    SetEvent(m_hListLock);    

	return 0;
}
int CMessageList::OnTimeInterval()
{
    COMMONMSG_PTR  pListHeadNode=NULL;
	COMMONMSG_PTR  pCurrListNode=NULL;
    int            nMsgTimeoutsNum = 0;

//    WaitForSingleObject(m_hListLock, INFINITE);
//    ResetEvent(m_hListLock);

	pListHeadNode = m_pListHead;
	pCurrListNode = pListHeadNode->pNextNode ;

    while(pCurrListNode != pListHeadNode)
    {
        pCurrListNode->nDelayTimes = pCurrListNode->nDelayTimes + 1;
        if(pCurrListNode->nDelayTimes > m_nTimeouts)
        {
           nMsgTimeoutsNum ++;
        }
//        fprintf(fpLog,"MsgID = %.2d    Msg Delay Time : %.2d\n",pCurrListNode->MsgID ,pCurrListNode->MsgDelay );
        pCurrListNode = pCurrListNode->pNextNode ;
    }
   
//    SetEvent(m_hListLock);

    return nMsgTimeoutsNum;
}


int CMessageList::DelTimeoutsMessage()
{
    COMMONMSG_PTR  pListHeadNode=NULL;
    COMMONMSG_PTR  pCurrListNode=NULL;
    int            nClientID = 0;
    
//    WaitForSingleObject(m_hListLock, INFINITE);
//    ResetEvent(m_hListLock);
    
    pListHeadNode = m_pListHead;
    pCurrListNode = pListHeadNode->pNextNode ;
    
    if(m_pListHead == pCurrListNode)
    {
//        SetEvent(m_hListLock);

        return 0;
    }

    if(pCurrListNode->nDelayTimes > m_nTimeouts)
    {
        nClientID = pCurrListNode->nResponseId ;
        
        //remove the timeouts message from pool
        pCurrListNode->pPrevNode ->pNextNode = pCurrListNode->pNextNode ;
        pCurrListNode->pNextNode ->pPrevNode = pCurrListNode->pPrevNode ;
        
        m_pListTail = m_pListHead->pPrevNode ;
        m_nListNodeCount--;

        //destory message content buffer
        delete pCurrListNode;

//        SetEvent(m_hListLock);

        return nClientID;
    }
    
//    SetEvent(m_hListLock);

    return 0;
}

//Get the next be sent message from message pool
BOOL CMessageList::GetNextMessage(COMMONMSG_PTR *ppMessage, BOOL bDelLastMsgFlag)
//BOOL CMessageList::GetNextMessage(int *ClientID, LPBYTE *lpMessage, int *iDataLen,int *MsgType,BOOL bFlag)
{
    //according to the flag get the next message infomation
    COMMONMSG_PTR  pCurrListNode=NULL;
    int            sequenceNum = 0;

//    WaitForSingleObject(m_hListLock, INFINITE);
//    ResetEvent(m_hListLock);

    //Set the pointer to  the first list node
    pCurrListNode = m_pListHead->pNextNode ;
    
    
    if(pCurrListNode == m_pListHead)
    {
//        SetEvent(m_hListLock);
        
        *ppMessage = NULL;

        return FALSE;
    }

    if(bDelLastMsgFlag)
    {
        //Save the current MsgID
        sequenceNum = ((Protocol_Header_Ptr)pCurrListNode->lpData)->sequenceNum;

        //Move to the next list node 
        *ppMessage = pCurrListNode->pNextNode ;

        //remove the timeouts message from pool
        pCurrListNode->pPrevNode ->pNextNode = pCurrListNode->pNextNode ;
        pCurrListNode->pNextNode ->pPrevNode = pCurrListNode->pPrevNode ;
        
        m_pListTail = m_pListHead->pPrevNode ;
        m_nListNodeCount--;

        //destory message content buffer
        delete pCurrListNode;

        if(*ppMessage == m_pListHead)
        {

            *ppMessage = NULL;
            
//            SetEvent(m_hListLock);

            return FALSE;
        }

//        SetEvent(m_hListLock);

        return TRUE;
    }
    else
    {
        if(pCurrListNode->nRepeatCount < MSG_SEND_TIMES)
        {
            //get the messge infomation
            *ppMessage = pCurrListNode;
            pCurrListNode->nRepeatCount++;
            
//            SetEvent(m_hListLock);

            return TRUE;
        }
        else
        {
            //get the messge infomation
            *ppMessage = pCurrListNode;
            pCurrListNode->nRepeatCount ++;

//            SetEvent(m_hListLock);

            return FALSE;
        }
    }
}

BOOL CMessageList::RemoveAll()
{
    //删除客户端连接信息列表
    COMMONMSG_PTR  pListHeadNode=NULL;
    COMMONMSG_PTR  pListTailNode=NULL; 
    COMMONMSG_PTR  pDeleListNode=NULL; 
    
//    WaitForSingleObject(m_hListLock, INFINITE);
//    ResetEvent(m_hListLock);

    if((m_pListHead != NULL) && (m_pListTail != NULL))
    {
        pListHeadNode = m_pListHead;
        pDeleListNode = pListTailNode = m_pListTail;       
        
        while(pListHeadNode != pListTailNode)
        {
            pListTailNode = pListTailNode->pPrevNode ;
            pListTailNode->pNextNode = pListHeadNode;
            pListHeadNode->pPrevNode = pListTailNode;
            
            delete[] pDeleListNode;
            
            pDeleListNode = pListTailNode;
        }    
    }

    m_pListTail = m_pListHead ;        
    m_nListNodeCount = 0;

  
//    SetEvent(m_hListLock);

    return TRUE;
}