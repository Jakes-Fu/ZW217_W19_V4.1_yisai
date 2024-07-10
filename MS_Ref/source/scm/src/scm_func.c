#include "ms_ref_scm_trc.h"
#include "sci_types.h"
#include "scm_api.h"
#include "scm_irp.h"
#include "scm_devmgr.h"
#include "sci_api.h"

typedef struct
{
	BLOCK_ID clientId;
	SCM_EVENT_E event;
	SCM_EVENT_CALLBACK callback;
}SCM_CLIENT_T;

#define SCM_MAX_CLIENT_NO 5
typedef struct SLOT_CONTEXT_TAG
{
	BOOLEAN		flag; // TRUE is in use ,FALSE is in unused
	uint32		sleepValue;
	BOOLEAN		slot_IsPwrOn;	//TRUE : slot is pwron, FALSE : slot is pwroff

	SCM_OPEN	OpenProtocol;
	SCM_CLOSE	CloseProtocol;

	SCM_INIT	InitCard;
	SCM_READ	Read;
	SCM_WRITE	Write;
	SCM_READEXT	ReadExt;
	SCM_WRITEEXT	WriteExt;
	SCM_ERASE	Erase;
	SCM_GETCAPCITY GetCapcity;
#if defined(SPRD_SUPPORT_MCEX)
	SCM_ENTERMCEX	EnterMcex;
	SCM_RESETTRM	ResetTrm;
	SCM_SENDPSI	SendPsi;
	SCM_READSECCMD	ReadRecCmd;
	SCM_WRITESECCMD	WriteRecCmd;
	SCM_EXITMCEX	ExitMcex;
#endif
	SCM_ERRPROC	ErrProc;

	SCM_ISEXIST		IsExist;

	SCM_DEVICE_NAME_E DevNO;
	SCM_STATUS status;

//---------------
	BOOLEAN		if_PwrSaving;
//	BOOLEAN		if_EvtInd;
//	BOOLEAN		if_ErrProc;

	SCM_CLIENT_T client[SCM_MAX_CLIENT_NO];
}SLOT_CONTEXT_T;


LOCAL SLOT_CONTEXT_T s_slotLst[SCM_MAX_SLOT_NO];

#define SCM_SLOT_ENTER(slotNO) \
	_DevEntry(s_slotLst[slotNO].DevNO);

#define SCM_SLOT_EXIT(slotNO) \
	_DevExit(s_slotLst[slotNO].DevNO);


#define SCM_LIFE_TIME 10

#define SCM_SLOT_PWRON(slotNO) \
{\
	_DevPwr(s_slotLst[slotNO].DevNO,slotNO,SLOT_PWRON); \
	s_slotLst[slotNO].sleepValue = SCM_LIFE_TIME;\
	s_slotLst[slotNO].slot_IsPwrOn = TRUE;\
	_SCM_SetTimeEvt();\
}

#define SCM_SLOT_PWROFF(slotNO) \
{\
	_DevPwr(s_slotLst[slotNO].DevNO,slotNO,SLOT_PWROFF); \
	s_slotLst[slotNO].sleepValue = 0x00;\
	s_slotLst[slotNO].slot_IsPwrOn = FALSE;\
}

#define SCM_SLOT_RESET_SLEEP_VALUE(slotNO) \
{\
	s_slotLst[slotNO].sleepValue = SCM_LIFE_TIME;\
}

#define SCM_SET_FLAG(slotNO) \
{ \
	SCI_DisableIRQ(); \
	s_slotLst[slotNO].flag = TRUE; \
	SCI_RestoreIRQ(); \
}

#define SCM_Clear_FLAG(slotNO) \
{ \
	SCI_DisableIRQ(); \
	s_slotLst[slotNO].flag = FALSE; \
	SCI_RestoreIRQ(); \
}

#define SCM_Get_FLAG(slotNO,flag) \
{ \
	SCI_DisableIRQ(); \
	flag = s_slotLst[slotNO].flag; \
	SCI_RestoreIRQ(); \
}

extern BOOLEAN g_LdoEmmcPwrOffFlag;

LOCAL void _ResetSlot(SCM_SLOT_NAME_E slotNO)
{
	uint32 i = 0;

	s_slotLst[slotNO].sleepValue = 0x00;
	s_slotLst[slotNO].slot_IsPwrOn = FALSE;
	s_slotLst[slotNO].OpenProtocol	=	NULL;
	s_slotLst[slotNO].CloseProtocol	=	NULL;
	s_slotLst[slotNO].InitCard	= NULL;

	s_slotLst[slotNO].Read	= NULL;
	s_slotLst[slotNO].Write	= NULL;
	s_slotLst[slotNO].ReadExt	= NULL;
	s_slotLst[slotNO].WriteExt	= NULL;
	s_slotLst[slotNO].Erase	= NULL;
	s_slotLst[slotNO].GetCapcity	= NULL;
#if defined(SPRD_SUPPORT_MCEX)
	s_slotLst[slotNO].EnterMcex	= NULL;
	s_slotLst[slotNO].ResetTrm	= NULL;
	s_slotLst[slotNO].SendPsi	= NULL;
	s_slotLst[slotNO].ReadRecCmd	= NULL;
	s_slotLst[slotNO].WriteRecCmd	= NULL;
	s_slotLst[slotNO].ExitMcex = NULL;
#endif
	s_slotLst[slotNO].ErrProc	= NULL;

	s_slotLst[slotNO].IsExist= NULL;

	s_slotLst[slotNO].DevNO	= NULL;
	s_slotLst[slotNO].status	= SCM_SYSTEMERR;
	s_slotLst[slotNO].if_PwrSaving	= SCM_SERVICE_ON;
//	s_slotLst[slotNO].if_EvtInd		= SCM_SERVICE_ON;
//	s_slotLst[slotNO].if_ErrProc		= SCM_SERVICE_ON;
	for(i = 0; i < SCM_MAX_CLIENT_NO; i++)
	{
		s_slotLst[slotNO].client[i].clientId = SCI_INVALID_BLOCK_ID;
		s_slotLst[slotNO].client[i].event = NULL;
		s_slotLst[slotNO].client[i].callback = NULL;
	}
}

PUBLIC void _InitSlotLst()
{
	uint32 i;

	for(i = 0; i < SCM_MAX_SLOT_NO; i++)
	{
		SCM_Clear_FLAG(i);
		_ResetSlot(i);
	}
}

LOCAL void _SendEvtToClient(SCM_SLOT_NAME_E slotNO,SCM_EVENT_E scmEvent)
{
	uint32 i;

	SCI_ASSERT(slotNO < SCM_MAX_SLOT_NO);	/*assert verified*/

	//SCM_FUNC_PRINT:"SCM _SendEvtToClient slot 0x%x evt 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_165_112_2_18_1_52_23_0,(uint8*)"dd",slotNO,scmEvent);

	for(i = 0; i < SCM_MAX_CLIENT_NO; i++)
	{
		if(SCI_INVALID_BLOCK_ID != s_slotLst[slotNO].client[i].clientId)
		{
			if( 0 != ((s_slotLst[slotNO].client[i].event)&scmEvent))  /*lint !e655*/
			{
				SCI_ASSERT(0 != s_slotLst[slotNO].client[i].callback);	/*assert verified*/
				s_slotLst[slotNO].client[i].callback(slotNO,scmEvent);
			}
		}
	}
}

/*
	used for card sleep
*/
PUBLIC _SCM_WAIT_TYPE _SCM_PwrScan(void)
{
	uint32 i;
	uint32 tCnt;
	_SCM_WAIT_TYPE waitType;

	waitType = _SCM_WAIT_2SEC;
	tCnt = 0;
	for(i = 0; i < SCM_MAX_SLOT_NO; i++)
	{
		if(TRUE == s_slotLst[i].flag)
		{
SCM_SLOT_ENTER(i);
			if(0 != s_slotLst[i].sleepValue)
			{
				tCnt++;
				s_slotLst[i].sleepValue--;
				if(0 == s_slotLst[i].sleepValue )
				{
					//SCM_FUNC_PRINT:"SCM Slot 0x%x sleep"
					SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_201_112_2_18_1_52_23_1,(uint8*)"d",i);
					if(SCM_SERVICE_ON == s_slotLst[i].if_PwrSaving)
					{
						SCM_SLOT_PWROFF(i);
					}
					tCnt--;
				}
			}
		}
	}
//---
	if(0 == tCnt)
	{
		waitType = _SCM_WAITFOREVER;
	}
//---
	for(i = 0; i < SCM_MAX_SLOT_NO; i++)
	{
		if(TRUE == s_slotLst[i].flag)
		{
SCM_SLOT_EXIT(i);
		}
	}
	return waitType;
}



/*
	when card is in sleep mode ,this function used to wake up the card and re init it
	can be used in scm task and other task
*/
LOCAL BOOLEAN _SCM_ReInit(SCM_SLOT_NAME_E slotNO)
{
	//SCM_FUNC_PRINT:"SCM _SCM_ReInit slot 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_234_112_2_18_1_52_23_2,(uint8*)"d",slotNO);
	SCI_ASSERT(slotNO < SCM_MAX_SLOT_NO);	/*assert verified*/
	SCM_SLOT_PWRON(slotNO);
	return s_slotLst[slotNO].InitCard();
}

/*
	can be used in scm task and other task
*/
LOCAL BOOLEAN _SCM_ErrProc(SCM_SLOT_NAME_E slotNO,SCM_FUN_E cmd,SCM_PARAM_T* param)
{
	SCI_ASSERT(slotNO < SCM_MAX_SLOT_NO);	/*assert verified*/

	//SCM_FUNC_PRINT:"SCM _SCM_ErrProc, slot 0x%x cmd = %x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_246_112_2_18_1_52_23_3,(uint8*)"dd",slotNO,cmd);

	if(TRUE == s_slotLst[slotNO].IsExist())
	{
		if(TRUE == s_slotLst[slotNO].ErrProc(cmd,param))
		{
			//SCM_FUNC_PRINT:"SCM _SCM_ErrProc : Error process succuss"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_252_112_2_18_1_52_23_4,(uint8*)"");
#if defined(SPRD_SUPPORT_MCEX)
			if(
				(SCM_FUNC_READ_REC_CMD == cmd)
				||(SCM_FUNC_WRITE_REC_CMD == cmd)
				||(SCM_FUNC_SEND_PSI == cmd)
			)
			{
				return FALSE;
//				SCI_ASSERT(0);
			}
#endif
			s_slotLst[slotNO].status = SCM_STATUS_NORMAL;
			return TRUE;
		}
		else
		{
			//SCM_FUNC_PRINT:"SCM _SCM_ErrProc : Error process fail"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_269_112_2_18_1_52_23_5,(uint8*)"");
			SCM_SLOT_PWROFF(slotNO);
			s_slotLst[slotNO].status = SCM_STATUS_ERROR;
			_SendEvtToClient(slotNO,SCM_ERROR_EVT);
			return FALSE;
		}
	}
	else
	{
		//SCM_FUNC_PRINT:"SCM _SCM_ErrProc : Card has been removed from slot"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_278_112_2_18_1_52_23_6,(uint8*)"");
		SCM_SLOT_PWROFF(slotNO);
		s_slotLst[slotNO].status = SCM_NOT_EXIST;
		_SendEvtToClient(slotNO,SCM_PLUG_OUT_EVT);
		return FALSE;
	}
}


/*
	该函数被调用时必须和其它功能函数互斥，该函数目前在任务启动时被任务调用
	但目前该函数只在系统启动时被调用用于配置slot端口信息
*/
PUBLIC BOOLEAN _SCM_RegSlot(SCM_SLOT_DESCRIPTION_T* description)
{
	SCM_SLOT_NAME_E slotNO = description->slot_name;
	SCI_ASSERT(slotNO < SCM_MAX_SLOT_NO);	/*assert verified*/

	//SCM_FUNC_PRINT:"SCM _SCM_RegSlot"
	mon_Event(0xc1110001);
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_295_112_2_18_1_52_23_7,(uint8*)"");

	if(TRUE == s_slotLst[slotNO].flag)
	{
		SCI_ASSERT(0);	/*assert verified*/
		return FALSE;     /*lint !e527*/
	}

	s_slotLst[slotNO].OpenProtocol	=	description->OpenProtocol;
	s_slotLst[slotNO].CloseProtocol	=	description->CloseProtocol;
	
	s_slotLst[slotNO].InitCard	=	description->InitCard;

	s_slotLst[slotNO].Read	=	description->Read;
	s_slotLst[slotNO].Write	=	description->Write;
	s_slotLst[slotNO].ReadExt	=	description->ReadExt;
	s_slotLst[slotNO].WriteExt	=	description->WriteExt;
	s_slotLst[slotNO].Erase	=	description->Erase;
	s_slotLst[slotNO].GetCapcity	=	description->GetCapcity;
#if defined(SPRD_SUPPORT_MCEX)
	s_slotLst[slotNO].EnterMcex	= 	description->EnterMcex;
	s_slotLst[slotNO].ResetTrm	= 	description->ResetTrm;
	s_slotLst[slotNO].SendPsi	= 	description->SendPsi;
	s_slotLst[slotNO].ReadRecCmd	= description->ReadRecCmd;
	s_slotLst[slotNO].WriteRecCmd	= description->WriteRecCmd;
	s_slotLst[slotNO].ExitMcex	= description->ExitMcex;
#endif
	s_slotLst[slotNO].ErrProc	=	description->ErrProc;

	s_slotLst[slotNO].IsExist	= description->IsExist;

	s_slotLst[slotNO].DevNO	= description->DeviceName;
	s_slotLst[slotNO].status		= SCM_NOT_EXIST;

	s_slotLst[slotNO].if_PwrSaving	= SCM_SERVICE_ON;
//	s_slotLst[slotNO].if_EvtInd		= SCM_SERVICE_ON;
//	s_slotLst[slotNO].if_ErrProc		= SCM_SERVICE_ON;
	_AddSlotToDevice(slotNO,description->DeviceName,description->pwr);
	mon_Event(0xc1110002);

	SCM_SLOT_ENTER(slotNO);

	if(FALSE == s_slotLst[slotNO].OpenProtocol())
	{	
	      mon_Event(0xc1110004);

		if(0 != _DeleteSlotFromDevice(slotNO,s_slotLst[slotNO].DevNO))
		{
			SCM_SLOT_EXIT(slotNO);
            	mon_Event(0xc1110003);

		}
		_ResetSlot(slotNO);
		return FALSE;
	}
	if(TRUE == s_slotLst[slotNO].IsExist())
	{
		SCM_SLOT_PWRON(slotNO);
        	mon_Event(0xc1110005);

		if(TRUE == s_slotLst[slotNO].InitCard())
		{
			s_slotLst[slotNO].status = SCM_STATUS_NORMAL;
		}
		else
		{
			_SCM_ErrProc(slotNO,SCM_FUNC_REINIT,0);
		}
	}
	else
	{
		s_slotLst[slotNO].status = SCM_NOT_EXIST;
	}
	SCM_SLOT_EXIT(slotNO);

	SCM_SET_FLAG(slotNO);
    	mon_Event(0xc1110006);

	return TRUE;
}


PUBLIC BOOLEAN _SCM_UnRegSlot(SCM_SLOT_NAME_E slotNO)
{
	SCI_ASSERT(slotNO < SCM_MAX_SLOT_NO);	/*assert verified*/

	//SCM_FUNC_PRINT:"SCM _SCM_UnRegSlot"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_372_112_2_18_1_52_23_8,(uint8*)"");

	if(FALSE == s_slotLst[slotNO].flag)
	{
		SCI_ASSERT(0);	/*assert verified*/
		return TRUE;     /*lint !e527*/
	}
	//Risk with IOCTL , Jason.wu
	SCM_Clear_FLAG(slotNO);

	SCM_SLOT_ENTER(slotNO);

	SCM_SLOT_PWROFF(slotNO);
	s_slotLst[slotNO].CloseProtocol();

	if(0 != _DeleteSlotFromDevice(slotNO,s_slotLst[slotNO].DevNO))
	{
		SCM_SLOT_EXIT(slotNO);
	}
	_ResetSlot(slotNO);

	return TRUE;
}

/*
	slotNO: slot number where you want to recieve msg from
	clientId: the taskId that want to recieve msg
	scmEvent: msg
	callback: when msg happend ,this callback will be called
*/
PUBLIC BOOLEAN _SCM_RegEvent(SCM_SLOT_NAME_E slotNO,BLOCK_ID clientId,SCM_EVENT_E scmEvent,SCM_EVENT_CALLBACK callback)
{
	uint32 i;

	SCI_ASSERT(SCI_INVALID_BLOCK_ID != clientId);	/*assert verified*/
	SCI_ASSERT(slotNO < SCM_MAX_SLOT_NO);	/*assert verified*/
	_SCM_WaitReady();

	//SCM_FUNC_PRINT:"SCM _SCM_RegEvent slot 0x%x, evt 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_409_112_2_18_1_52_23_9,(uint8*)"dd",slotNO,scmEvent);
	if(FALSE == s_slotLst[slotNO].flag)
	{
		SCI_ASSERT(0);	/*assert verified*/
		return FALSE;     /*lint !e527*/
	}

SCM_SLOT_ENTER(slotNO);
	for(i = 0; i < SCM_MAX_CLIENT_NO; i++)
	{
		if(clientId == s_slotLst[slotNO].client[i].clientId)
		{
			if(SCM_NONE_EVT == scmEvent)
			{
				s_slotLst[slotNO].client[i].clientId = SCI_INVALID_BLOCK_ID;
				s_slotLst[slotNO].client[i].event = NULL;
				s_slotLst[slotNO].client[i].callback = NULL;
			}
			else
			{
				s_slotLst[slotNO].client[i].event = scmEvent;
				SCI_ASSERT(callback == s_slotLst[slotNO].client[i].callback);	/*assert verified*/
			}
			SCM_SLOT_EXIT(slotNO);
			return TRUE;
		}
	}
	for(i = 0; i < SCM_MAX_CLIENT_NO; i++)
	{
		if(SCI_INVALID_BLOCK_ID == s_slotLst[slotNO].client[i].clientId)
		{
			if(SCM_NONE_EVT != scmEvent)
			{
				SCI_ASSERT(0 != callback);	/*assert verified*/
				s_slotLst[slotNO].client[i].clientId = clientId;
				s_slotLst[slotNO].client[i].event = scmEvent;
				s_slotLst[slotNO].client[i].callback = callback;
			}
			SCM_SLOT_EXIT(slotNO);
			return TRUE;
		}
	}
SCM_SLOT_EXIT(slotNO);
	return FALSE;
}

/*
	get status of the card that in slot
*/
PUBLIC SCM_STATUS _SCM_GetSlotStatus(SCM_SLOT_NAME_E slotNO)
{
	SCM_STATUS status;

	SCI_ASSERT(slotNO < SCM_MAX_SLOT_NO);	/*assert verified*/
	_SCM_WaitReady();

	//SCM_FUNC_PRINT:"SCM _SCM_GetSlotStatus slot 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_464_112_2_18_1_52_24_10,(uint8*)"d",slotNO);
	if(FALSE == s_slotLst[slotNO].flag)
	{
		SCI_ASSERT(0);	/*assert verified*/
		return SCM_SYSTEMERR;     /*lint !e527*/
	}
	SCI_ASSERT(slotNO < SCM_MAX_SLOT_NO);	/*assert verified*/

	SCM_SLOT_ENTER(slotNO);
	status = s_slotLst[slotNO].status;
	SCM_SLOT_EXIT(slotNO);

	return status;
}


PUBLIC void _SCM_SetService(SCM_SLOT_NAME_E slotNO, SCM_SERVICE_ID serviceId, SCM_SERVICE_ONOFF onOff)
{
	SCI_ASSERT(slotNO < SCM_MAX_SLOT_NO);	/*assert verified*/
	SCI_ASSERT((SCM_SERVICE_ON == onOff)||(SCM_SERVICE_OFF == onOff));	/*assert verified*/
	_SCM_WaitReady();

	//SCM_FUNC_PRINT:"SCM _SCM_SetService slot 0x%x,service 0x%x, onoff = 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_486_112_2_18_1_52_24_11,(uint8*)"ddd",slotNO,serviceId, onOff);
	if(FALSE == s_slotLst[slotNO].flag)
	{
		//SCM_FUNC_PRINT:"SCM _SCM_SetService invalid slot id."
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_489_112_2_18_1_52_24_12,(uint8*)"");
		SCI_ASSERT(0);	/*assert verified*/
	}
	if(SCM_PWR_SAVING == serviceId)
	{
		SCM_SLOT_ENTER(slotNO);
		s_slotLst[slotNO].if_PwrSaving = onOff;
		SCM_SLOT_EXIT(slotNO);
	}
	else
	{
		//SCM_FUNC_PRINT:"SCM invalid service id"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_500_112_2_18_1_52_24_13,(uint8*)"");
		SCI_ASSERT(0);	/*assert verified*/
	}
	return;
}

LOCAL void _SCM_PlugIn(SCM_SLOT_NAME_E slotNO)
{
	SCI_ASSERT(slotNO < SCM_MAX_SLOT_NO);	/*assert verified*/

	//SCM_FUNC_PRINT:"SCM _SCM_PlugIn"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_510_112_2_18_1_52_24_14,(uint8*)"");

	if(FALSE == s_slotLst[slotNO].flag)
	{
		return;
	}

	SCM_SLOT_ENTER(slotNO);
	if(SCM_NOT_EXIST != s_slotLst[slotNO].status)
	{
		SCM_SLOT_EXIT(slotNO);
		return;
	}
	if(TRUE == s_slotLst[slotNO].IsExist())
	{
		_SendEvtToClient(slotNO,SCM_PLUG_IN_EVT);
		SCM_SLOT_PWRON(slotNO);
		if(TRUE == s_slotLst[slotNO].InitCard())
		{
			s_slotLst[slotNO].status = SCM_STATUS_NORMAL;
		}
		else
		{
			_SCM_ErrProc(slotNO,SCM_FUNC_REINIT,0);
		}
	}

	SCM_SLOT_EXIT(slotNO);
}


LOCAL void _SCM_PlugOut(SCM_SLOT_NAME_E slotNO)
{
	SCI_ASSERT(slotNO < SCM_MAX_SLOT_NO);	/*assert verified*/

	//SCM_FUNC_PRINT:"SCM _SCM_PlugOut"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_545_112_2_18_1_52_24_15,(uint8*)"");

	if(FALSE == s_slotLst[slotNO].flag)
	{
		return;
	}

	SCM_SLOT_ENTER(slotNO);

	if(SCM_NOT_EXIST == s_slotLst[slotNO].status)
	{
		SCM_SLOT_EXIT(slotNO);
		return;
	}
	if(FALSE == s_slotLst[slotNO].IsExist())
	{
		SCM_SLOT_PWROFF(slotNO);
		s_slotLst[slotNO].status = SCM_NOT_EXIST;
		_SendEvtToClient(slotNO,SCM_PLUG_OUT_EVT);
	}
	
	SCM_SLOT_EXIT(slotNO);

}


PUBLIC _SCM_WAIT_TYPE _SCM_PlugEvtProcess(SCM_SLOT_NAME_E slotNO, uint32 evt)
{
	SCI_ASSERT(slotNO < SCM_MAX_SLOT_NO);	/*assert verified*/

	if(FALSE == s_slotLst[slotNO].flag)
	{
		//SCM_FUNC_PRINT:"SCM invalid plug event slot:0x%x"
		SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_577_112_2_18_1_52_24_16,(uint8*)"d",slotNO);
		return _SCM_WAIT_500MS;
	}
	//SCM_FUNC_PRINT:"SCM plug evt 0x%x"
	SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_580_112_2_18_1_52_24_17,(uint8*)"d",evt);

	SCM_SLOT_ENTER(slotNO);
	if(SCM_NOT_EXIST == s_slotLst[slotNO].status)
	{
		if(TRUE == s_slotLst[slotNO].IsExist())
		{
			_SendEvtToClient(slotNO,SCM_PLUG_IN_EVT);
			SCM_SLOT_PWRON(slotNO);
			if(TRUE == s_slotLst[slotNO].InitCard())
			{
				s_slotLst[slotNO].status = SCM_STATUS_NORMAL;
			}
			else
			{
				_SCM_ErrProc(slotNO,SCM_FUNC_REINIT,0);
			}
		}
	}
	else
	{
		if(FALSE == s_slotLst[slotNO].IsExist())
		{
			SCM_SLOT_PWROFF(slotNO);
			s_slotLst[slotNO].status = SCM_NOT_EXIST;
			_SendEvtToClient(slotNO,SCM_PLUG_OUT_EVT);
		}
		else
		{
			if(0 != (_SCM_PLUG_OUT&evt))
			{
				SCM_SLOT_PWROFF(slotNO);
			}
		}
	}
	SCM_SLOT_EXIT(slotNO);

	return _SCM_WAIT_500MS;
}


PUBLIC BOOLEAN _SCM_IOCTL(SCM_SLOT_NAME_E slotNO,SCM_FUN_E cmd,SCM_PARAM_T* param)
{
	BOOLEAN result;
	BOOLEAN flag;
	SCM_Get_FLAG(slotNO,flag);

	if(FALSE == flag)
	{
		return FALSE;
	}
//	SCM_FUNC_PRINT(("SCM _SCM_IOCTL"));
	do
	{
		SCM_SLOT_ENTER(slotNO);

		if(SCM_NOT_EXIST == s_slotLst[slotNO].status)
		{
			//SCM_FUNC_PRINT:"SCM _SCM_IOCTL : Slot is empty state"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_638_112_2_18_1_52_24_18,(uint8*)"");
			SCM_SLOT_EXIT(slotNO);
			return FALSE;
		}

		if(SCM_STATUS_ERROR == s_slotLst[slotNO].status)
		{
			//SCM_FUNC_PRINT:"SCM _SCM_IOCTL : Slot is error"
			SCI_TRACE_ID(TRACE_TOOL_CONVERT,SCM_FUNC_645_112_2_18_1_52_24_19,(uint8*)"");
			SCM_SLOT_EXIT(slotNO);
			return FALSE;
		}
		
		if(FALSE == s_slotLst[slotNO].slot_IsPwrOn)
		{
			result = _SCM_ReInit(slotNO);
			if(FALSE == result)
			{
				result = _SCM_ErrProc(slotNO,SCM_FUNC_REINIT,0);
				if(FALSE == result)
				{
					SCM_SLOT_EXIT(slotNO);
					return FALSE;
				}
			}

		}

		if(SCM_FUNC_REINIT == cmd)
		{
			SCM_SLOT_RESET_SLEEP_VALUE(slotNO);
			SCM_SLOT_EXIT(slotNO);
			return TRUE;
		}
		else if(SCM_FUNC_READ == cmd)
		{
			if(NULL == s_slotLst[slotNO].Read)
			{
				SCM_SLOT_EXIT(slotNO);
				SCI_ASSERT(0);	/*assert verified*/
				return FALSE;     /*lint !e527*/
			}
            
#if defined(PLATFORM_UWS6121E)
                     if(!g_LdoEmmcPwrOffFlag)    
                     {
                        flag = s_slotLst[slotNO].Read(param->readParam.startBlock,param->readParam.num,param->readParam.buf);
                     }
                     else       //deep sleep ldo of sd is off,no need to do read
                     {
                        flag = FALSE;
                     }
                     if (TRUE == flag)
#else
			if(TRUE == s_slotLst[slotNO].Read(param->readParam.startBlock,param->readParam.num,param->readParam.buf))
#endif
                    {
				SCM_SLOT_RESET_SLEEP_VALUE(slotNO);
				SCM_SLOT_EXIT(slotNO);
				return TRUE;
			}
			else 
			{
				_SCM_ReInit(slotNO);
				
				if(TRUE == s_slotLst[slotNO].Read(param->writeParam.startBlock,param->writeParam.num,param->writeParam.buf))
				{
				    SCM_SLOT_RESET_SLEEP_VALUE(slotNO);
				    SCM_SLOT_EXIT(slotNO);
				    return TRUE;
				} 
			}
			break;
		}
		else if(SCM_FUNC_WRITE == cmd)
		{
			if(NULL == s_slotLst[slotNO].Write)
			{
				SCM_SLOT_EXIT(slotNO);
				SCI_ASSERT(0);	/*assert verified*/
				return FALSE;     /*lint !e527*/
			}

			if(TRUE == s_slotLst[slotNO].Write(param->writeParam.startBlock,param->writeParam.num,param->writeParam.buf))
			{
				SCM_SLOT_RESET_SLEEP_VALUE(slotNO);
				SCM_SLOT_EXIT(slotNO);
				return TRUE;
			} 
			else 
			{
				_SCM_ReInit(slotNO);
				
				if(TRUE == s_slotLst[slotNO].Write(param->writeParam.startBlock,param->writeParam.num,param->writeParam.buf))
				{
				    SCM_SLOT_RESET_SLEEP_VALUE(slotNO);
				    SCM_SLOT_EXIT(slotNO);
				    return TRUE;
				} 
			}
			break;

		}
		else if(SCM_FUNC_READEXT == cmd)
		{
			if(NULL == s_slotLst[slotNO].ReadExt)
			{
				SCM_SLOT_EXIT(slotNO);
				return FALSE;
			}
			if(TRUE == s_slotLst[slotNO].ReadExt(param->readExtParam.startBlock,param->readExtParam.sctInfo))
			{
				SCM_SLOT_RESET_SLEEP_VALUE(slotNO);
				SCM_SLOT_EXIT(slotNO);
				return TRUE;
			}
			break;
		}
		else if(SCM_FUNC_WRITEEXT == cmd)
		{
			if(NULL == s_slotLst[slotNO].WriteExt)
			{
				SCM_SLOT_EXIT(slotNO);
				return FALSE;
			}
			if(TRUE == s_slotLst[slotNO].WriteExt(param->writeExtParam.startBlock,param->writeExtParam.sctInfo))
			{
				SCM_SLOT_RESET_SLEEP_VALUE(slotNO);
				SCM_SLOT_EXIT(slotNO);
				return TRUE;
			}
			break;
		}
		else if(SCM_FUNC_ERASE == cmd)
		{
			if(NULL == s_slotLst[slotNO].Erase)
			{
				SCM_SLOT_EXIT(slotNO);
				return FALSE;     /*lint !e527*/
			}
			if(TRUE == s_slotLst[slotNO].Erase(param->eraseParam.startBlock,param->eraseParam.endBlock))
			{
				SCM_SLOT_RESET_SLEEP_VALUE(slotNO);
				SCM_SLOT_EXIT(slotNO);
				return TRUE;
			}
			break;
		}
		else if(SCM_FUNC_GETCAPCITY == cmd)
		{
			if(NULL == s_slotLst[slotNO].GetCapcity)
			{
				SCM_SLOT_EXIT(slotNO);
				return FALSE;     /*lint !e527*/
			}
			param->getCapcityParam.capcity = s_slotLst[slotNO].GetCapcity();
			if(0 != param->getCapcityParam.capcity)
			{
				SCM_SLOT_RESET_SLEEP_VALUE(slotNO);
				SCM_SLOT_EXIT(slotNO);
				return TRUE;
			}
			break;
		}
#if defined(SPRD_SUPPORT_MCEX)
		else if(SCM_FUNC_ENTERMCEX == cmd)
		{
			if(NULL == s_slotLst[slotNO].EnterMcex)
			{
				SCM_SLOT_EXIT(slotNO);
				SCI_ASSERT(0);	/*assert verified*/
				return FALSE;  /*lint !e527 */
			}
			if(TRUE == s_slotLst[slotNO].EnterMcex(param->enterMcex.ifEnterMcex))
			{
				SCM_SLOT_RESET_SLEEP_VALUE(slotNO);
				SCM_SLOT_EXIT(slotNO);
				return TRUE;
			}
			break;
		}
		else if(SCM_FUNC_RESETTRM == cmd)
		{
			if(NULL == s_slotLst[slotNO].ResetTrm)
			{
				SCM_SLOT_EXIT(slotNO);
				SCI_ASSERT(0);	/*assert verified*/
				return FALSE;  /*lint !e527 */
			}
			if(TRUE == s_slotLst[slotNO].ResetTrm())
			{
				SCM_SLOT_RESET_SLEEP_VALUE(slotNO);
				SCM_SLOT_EXIT(slotNO);
				return TRUE;
			}
			break;
		}
		else if(SCM_FUNC_SEND_PSI == cmd)
		{
			if(NULL == s_slotLst[slotNO].SendPsi)
			{
				SCM_SLOT_EXIT(slotNO);
				SCI_ASSERT(0);	/*assert verified*/
				return FALSE;  /*lint !e527 */
			}
			if(TRUE == s_slotLst[slotNO].SendPsi(param->sendPsi.arg,param->sendPsi.buf))
			{
				SCM_SLOT_RESET_SLEEP_VALUE(slotNO);
				SCM_SLOT_EXIT(slotNO);
				return TRUE;
			}
			break;
		}
		else if(SCM_FUNC_READ_REC_CMD == cmd)
		{
			if(NULL == s_slotLst[slotNO].ReadRecCmd)
			{
				SCM_SLOT_EXIT(slotNO);
				SCI_ASSERT(0);	/*assert verified*/
				return FALSE;  /*lint !e527 */
			}
			if(TRUE == s_slotLst[slotNO].ReadRecCmd(param->readRecCmd.buf))
			{
				SCM_SLOT_RESET_SLEEP_VALUE(slotNO);
				SCM_SLOT_EXIT(slotNO);
				return TRUE;
			}
			break;
		}
		else if(SCM_FUNC_WRITE_REC_CMD == cmd)
		{
			if(NULL == s_slotLst[slotNO].WriteRecCmd)
			{
				SCM_SLOT_EXIT(slotNO);
				SCI_ASSERT(0);	/*assert verified*/
				return FALSE;   /*lint !e527 */
			}
			if(TRUE == s_slotLst[slotNO].WriteRecCmd(param->writeRecCmd.buf))
			{
				SCM_SLOT_RESET_SLEEP_VALUE(slotNO);
				SCM_SLOT_EXIT(slotNO);
				return TRUE;
			}
			break;
		}
		else if(SCM_FUNC_EXITMCEX == cmd)
		{
			if(NULL == s_slotLst[slotNO].ExitMcex)
			{
				SCM_SLOT_EXIT(slotNO);
				SCI_ASSERT(0);	/*assert verified*/
				return FALSE;   /*lint !e527 */
			}
			if(TRUE == s_slotLst[slotNO].ExitMcex())
			{
				SCM_SLOT_RESET_SLEEP_VALUE(slotNO);
				SCM_SLOT_EXIT(slotNO);
				return TRUE;
			}
			break;
		}

#endif
		else
		{
			SCM_SLOT_EXIT(slotNO);
			SCI_ASSERT(0);	/*assert verified*/
			return FALSE;     /*lint !e527*/
		}
	}
	while(0);
//errorProcss

	result = _SCM_ErrProc(slotNO,cmd,param);
	if(TRUE == result)
	{
		SCM_SLOT_RESET_SLEEP_VALUE(slotNO);
	}
	SCM_SLOT_EXIT(slotNO);
	return result;
	
}





