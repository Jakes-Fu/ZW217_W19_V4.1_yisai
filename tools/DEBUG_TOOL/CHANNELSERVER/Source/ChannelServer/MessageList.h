// MessageList.h: interface for the CMessageList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGELIST_H__09745F7B_D518_400B_9CE3_C12D37CE185C__INCLUDED_)
#define AFX_MESSAGELIST_H__09745F7B_D518_400B_9CE3_C12D37CE185C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Global.h"

class CMessageList  
{
public:
    CMessageList();
	virtual ~CMessageList();

public:
    //Initialize the message pool
   	BOOL InitMessagePool();

    //Add message to message pool
    int  AddMessage(COMMONMSG_PTR pMessage);

    //Get the next be sent message from message pool
	BOOL GetNextMessage(COMMONMSG_PTR *ppMessage, BOOL bDelLastMsgFlag);
    
    //Delete the message that timeouts in message pool
	int DelTimeoutsMessage();

    //Remove Message from message popl
	int DeleteMessage(int nPkt_Seq);

	int DeleteCurMessage(  byte btType );
    //Add delay time for all message in pool
    int OnTimeInterval();

    //Clear the message pool
    BOOL       RemoveAll();

public:
    //Added for timeouts depended on user setting 2002-04-12 By Leo.Feng
    int        m_nTimeouts;
	int        m_nListNodeCount;
    HANDLE     m_hListLock;

private:
	COMMONMSG_PTR m_pListTail;
	COMMONMSG_PTR m_pListHead;

private: 	
    
};

#endif // !defined(AFX_MESSAGELIST_H__09745F7B_D518_400B_9CE3_C12D37CE185C__INCLUDED_)
